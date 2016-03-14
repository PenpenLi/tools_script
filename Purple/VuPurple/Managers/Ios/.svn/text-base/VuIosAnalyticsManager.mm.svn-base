//*****************************************************************************
//
//  Copyright (c) 2012-2013 Vector Unit Inc
//  Confidential Trade Secrets
//
//  Ios Analytics manager
//
//*****************************************************************************

#import <Flurry.h>
#import <FuseApi.h>

#include "VuPurple/Managers/VuAnalyticsManager.h"


class VuIosAnalyticsManager : public VuAnalyticsManager
{
public:
	virtual void	logEventInternal(const char *eventName, const char *paramName, const char *paramValue, const VuJsonContainer &variables);
	virtual void	setLevelInternal(int level);
	virtual void	setCurrencyInternal(int type, int balance);
	virtual void	displayNotificationsInternal();
};


// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuAnalyticsManager, VuIosAnalyticsManager);


//*****************************************************************************
void VuIosAnalyticsManager::logEventInternal(const char *eventName, const char *paramName, const char *paramValue, const VuJsonContainer &variables)
{
	NSString* eventNameString = [NSString stringWithUTF8String:eventName];
	NSString* paramNameString = [NSString stringWithUTF8String:paramName];
	NSString* paramValueString = [NSString stringWithUTF8String:paramValue];
	
	// Flurry
	if ( paramName[0] )
	{
		NSMutableDictionary *flurryDict = [[NSMutableDictionary alloc] initWithCapacity:1];
		[flurryDict setValue:paramValueString forKey:paramNameString];
		[Flurry logEvent:eventNameString withParameters:flurryDict];
	}
	else
	{
		[Flurry logEvent:eventNameString];
	}
	
	// Fuse
	NSMutableDictionary *fuseDict = nil;
	if ( variables.numMembers() )
	{
		fuseDict = [[NSMutableDictionary alloc] initWithCapacity:variables.numMembers()];
		for ( int i = 0; i < variables.numMembers(); i++ )
		{
			const std::string &key = variables.getMemberKey(i);
			const VuJsonContainer &value = variables[key];
			
			if ( value.isInt() )
				[fuseDict setObject:[NSNumber numberWithInt:value.asInt()] forKey:[NSString stringWithUTF8String:key.c_str()]];
			else if ( value.isFloat() )
				[fuseDict setObject:[NSNumber numberWithFloat:value.asFloat()] forKey:[NSString stringWithUTF8String:key.c_str()]];
			else if ( value.isBool() )
				[fuseDict setObject:[NSNumber numberWithInt:(int)value.asBool()] forKey:[NSString stringWithUTF8String:key.c_str()]];
		}
	}
	
	[FuseAPI registerEvent:eventNameString ParameterName:paramNameString ParameterValue:paramValueString Variables:fuseDict];
}

//*****************************************************************************
void VuIosAnalyticsManager::setLevelInternal(int level)
{
	[FuseAPI registerLevel:level];
}

//*****************************************************************************
void VuIosAnalyticsManager::setCurrencyInternal(int type, int balance)
{
	[FuseAPI registerCurrency:type Balance:balance];
}

//*****************************************************************************
void VuIosAnalyticsManager::displayNotificationsInternal()
{
	[FuseAPI displayNotifications];
}

