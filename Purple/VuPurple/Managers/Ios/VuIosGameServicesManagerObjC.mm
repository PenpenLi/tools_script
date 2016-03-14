//*****************************************************************************
//
//  Copyright (c) 2012-2013 Vector Unit Inc
//  Confidential Trade Secrets
//
//  Objective-C support for Ios Game Services manager.
//
//*****************************************************************************


#import <GameKit/GameKit.h>
#include "VuIosGameServicesManagerObjC.h"

@interface VuGameServicesManager : NSObject <GKGameCenterControllerDelegate>
{
}
+ (VuGameServicesManager *)instance;
@end

@implementation VuGameServicesManager

+ (VuGameServicesManager *)instance
{
	static VuGameServicesManager *GameServicesManager = NULL;
	@synchronized(self)
	{
		if (GameServicesManager == NULL)
		{
			GameServicesManager = [[VuGameServicesManager alloc] init];
		}
	}
	
    return(GameServicesManager);
}

- (void)gameCenterViewControllerDidFinish:(GKGameCenterViewController *)viewController
{
	[[[[UIApplication sharedApplication] keyWindow] rootViewController] dismissViewControllerAnimated: YES completion: NULL];
    
    if ([GKLocalPlayer localPlayer].isAuthenticated)
    {
        VuIosGameServicesManagerObjC::onSignInSuccess([[GKLocalPlayer localPlayer].playerID UTF8String], [[GKLocalPlayer localPlayer].alias UTF8String]);
    }
    else
    {
        VuIosGameServicesManagerObjC::onSignInFailure();
    }
}

@end


//*****************************************************************************
void VuIosGameServicesManagerObjC::showAchievements()
{
	GKGameCenterViewController *viewController = [[GKGameCenterViewController alloc] init];
	if ( viewController )
	{
		viewController.gameCenterDelegate = [VuGameServicesManager instance];
		viewController.viewState = GKGameCenterViewControllerStateAchievements;
		[[[[UIApplication sharedApplication] keyWindow] rootViewController] presentViewController: viewController animated: YES completion: NULL];
	}
}

//*****************************************************************************
void VuIosGameServicesManagerObjC::unlockAchievement(const char *name)
{
	GKAchievement *achievement = [[GKAchievement alloc] initWithIdentifier: [NSString stringWithUTF8String:name]];
	if ( achievement )
	{
		achievement.percentComplete = 100.0f;
		achievement.showsCompletionBanner = false;
		[achievement reportAchievementWithCompletionHandler:^(NSError *error)
		 {
			 VuIosGameServicesManagerObjC::onUnlockAchievementResult(error == NULL);
		 }];
    }
	else
	{
		VuIosGameServicesManagerObjC::onUnlockAchievementResult(false);
	}
}

#define SYSTEM_VERSION_LESS_THAN(v) ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] == NSOrderedAscending)

//*****************************************************************************
static bool IsGameCenterAvailable()
{
	// check for presence of GKLocalPlayer API
	Class gcClass = (NSClassFromString(@"GKLocalPlayer"));
	
	// check if the device is running iOS 4.1 or later
	NSString *reqSysVer = @"4.1";
	NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
	BOOL osVersionSupported = ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending);
	
	return (gcClass && osVersionSupported);
}
static bool sIsRegistered = false;

//*****************************************************************************
void VuIosGameServicesManagerObjC::startSignInAsync()
{
	// is game center available?
	if ( IsGameCenterAvailable() )
	{
		GKLocalPlayer *localPlayer = [GKLocalPlayer localPlayer];
		
		if(localPlayer.authenticated == NO)
		{
            if (!sIsRegistered)
            {
                [localPlayer setAuthenticateHandler:(^(UIViewController* viewcontroller, NSError *error)
                {
                    if ( viewcontroller )
                    {
                        [[[[UIApplication sharedApplication] keyWindow] rootViewController] presentViewController:viewcontroller animated:YES completion:nil];
                    }
                    else if ( [GKLocalPlayer localPlayer].isAuthenticated )
                    {
                        const char* pId = [[GKLocalPlayer localPlayer].playerID UTF8String];
                        const char* pTag = [[GKLocalPlayer localPlayer].alias UTF8String];
                        
                        onSignInSuccess(pId, pTag);
                        
                        requestIosFriends();
                    }
                    else
                    {
                        onSignInFailure();
                    }
                })];
                
                sIsRegistered = true;
            }
            else
            {
                // Already registered
                
                // Get our root view controller
                auto view = [[[UIApplication sharedApplication] keyWindow] rootViewController];
                
                // Create a GC login view controller
                GKGameCenterViewController* controller = [[GKGameCenterViewController alloc] init];
                
                // Call back to our Manager's instance to report the screen has closed
                controller.gameCenterDelegate = VuGameServicesManager.instance;
                
                // Present the login
                [view presentViewController:controller animated:YES completion:nil];
            }
		}
		else
		{
			onSignInSuccess([[GKLocalPlayer localPlayer].playerID UTF8String], [[GKLocalPlayer localPlayer].alias UTF8String]);
		}
	}
	else
	{
		onSignInFailure();
	}
}

//*****************************************************************************
void VuIosGameServicesManagerObjC::checkState()
{
	if ( IsGameCenterAvailable() && ([GKLocalPlayer localPlayer].authenticated == YES) )
	{
		onCheckStateResult(true,
						   [[GKLocalPlayer localPlayer].playerID UTF8String],
						   [[GKLocalPlayer localPlayer].alias UTF8String]);
	}
	else
	{
		onCheckStateResult(false);
	}
}

//*****************************************************************************
void VuIosGameServicesManagerObjC::requestIosFriends()
{
	if ( IsGameCenterAvailable() )
	{
		GKLocalPlayer *localPlayer = [GKLocalPlayer localPlayer];
		
		if(localPlayer.authenticated == YES)
		{
			[localPlayer loadFriendsWithCompletionHandler:^(NSArray *friendIds, NSError *error)
			 {
				 if (friendIds != nil)
				 {
					 for (NSString* idStr in friendIds)
					 {
						 NSLog(@"%@", idStr);
					 }
					 
					 
					 [GKPlayer loadPlayersForIdentifiers:friendIds withCompletionHandler:^(NSArray *players, NSError *error)
					  {
						 if (error != nil)
						 {
							 // Something went wrong
							 NSLog(@"Unable to retrieve GKPlayer records for friends.");
							 NSLog(@"%@", [error localizedDescription]);
							 
							 return;
						 }
						  
						 if (players != nil)
						 {
							 // Pass the array of player names back to the game
							 NSString* data = @"[\"";
							 
							 NSUInteger count = [friendIds count];
							 for (NSUInteger index = 0; index < count; index++)
							 {
								 NSString* friendId = friendIds[index];
								 GKPlayer* friendPlayer = players[index];
								 
								 data = [data stringByAppendingString:friendId];
								 data = [data stringByAppendingString:@"\",\""];
								 data = [data stringByAppendingString:friendPlayer.alias];
								 data = [data stringByAppendingString:@"\""];
								 
								 if (index < count-1)
								 {
									 data = [data stringByAppendingString:@","];
								 }
							 }
							 data = [data stringByAppendingString:@"]"];
							 
							 const char* pString = [data UTF8String];
							 
							 VuIosGameServicesManagerObjC::onIosFriendsReceived(pString);
						 }
					  }];
				 }
			 }];
		}
	}
}

//*****************************************************************************

void VuIosGameServicesManagerObjC::submitScore(const char* pLBName, VUUINT64 score)
{
	if (pLBName == VUNULL)
	{
		return;
	}
	
	NSString* lbName = [NSString stringWithUTF8String:pLBName];
	
	GKScore *scoreReporter = [[GKScore alloc] initWithLeaderboardIdentifier: lbName];
    
	[scoreReporter setValue:score];
	[scoreReporter setShouldSetDefaultLeaderboard:YES];
	[scoreReporter setContext:0ULL];
	
	[scoreReporter reportScoreWithCompletionHandler:^(NSError *error)
	{
		if (error != nil)
		{
			NSLog(@"Error: reportScoreWithCompletionHandler failed: %@", [error localizedDescription]);
		}
	}];
}





