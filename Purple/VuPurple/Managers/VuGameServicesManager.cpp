//*****************************************************************************
//
//  Copyright (c) 2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuGameServicesManager
// 
//*****************************************************************************

#include "VuGameServicesManager.h"
#include "VuAchievementManager.h"
#include "VuAnalyticsManager.h"
#include "VuPurple/Managers/VuStatsManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Managers/VuProfileManager.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/DB/VuStringDB.h"
#include "VuEngine/Util/VuSpreadsheetQuery.h"


#define REFRESH_TIMER 10.0f // seconds


//*****************************************************************************
VuGameServicesManager::VuGameServicesManager():
	mIsSignedIn(false),
	mIsBusy(false),
	mTimer(0.0f)
{
	mGamerTag = VuStringDB::IF()->getString("Common_Player");
	mGamerID = "";
}

//*****************************************************************************
VuGameServicesManager::~VuGameServicesManager()
{
	VUASSERT(mListeners.size() == 0, "VuGameServicesManager listener leak");
}

//*****************************************************************************
bool VuGameServicesManager::init()
{
	VuTickManager::IF()->registerHandler(this, &VuGameServicesManager::tickDecision, "Decision");

	return true;
}

//*****************************************************************************
void VuGameServicesManager::postInit()
{
	// build list of native achievement IDs
	if ( mAuthType.length() )
	{
		const VuSpreadsheetAsset *pSA = VuGameUtil::IF()->achievementSpreadsheet();
		int nativeIdColumn = pSA->getColumnIndex(mAuthType.c_str());
		if ( nativeIdColumn >= 0 )
		{
			for ( const auto &ach : VuAchievementManager::IF()->getAchievements() )
			{
				int row = VuSpreadsheetQuery::findFirstRow(pSA, VuSpreadsheetQuery::VuIntEqual("ID", ach.mId));
				const char *nativeId = pSA->getField(row, nativeIdColumn).asCString();
				if ( nativeId[0] != '\0' )
				{
					mAchievements[ach.mId].mNativeId = nativeId;
				}
			}
		}
	}
}

//*****************************************************************************
void VuGameServicesManager::release()
{
	VuTickManager::IF()->unregisterHandler(this, "Decision");
}

//*****************************************************************************
void VuGameServicesManager::startAutoSignIn()
{
	if ( VuProfileManager::IF()->dataRead()["GameServices"]["AutoSignIn"].asBool() )
	{
		startSignIn();
	}
}

//*****************************************************************************
void VuGameServicesManager::startSignIn()
{
	if ( !isBusy() && !isSignedIn() )
	{
		mIsBusy = true;
		startSignInAsync();
	}
}

//*****************************************************************************
void VuGameServicesManager::startSignOut()
{
	if ( !isBusy() && isSignedIn() )
	{
		mIsBusy = true;
		startSignOutAsync();
	}
}

//*****************************************************************************
const VuGameServicesManager::FriendInfo *VuGameServicesManager::getFriendInfo(const char *gamerID)
{
	for ( const auto &info : mFriends )
		if ( info.mGamerID == gamerID )
			return &info;

	return VUNULL;
}

//*****************************************************************************
void VuGameServicesManager::submitScore(const char *lbName, int value)
{
	if ( isSignedIn() )
	{
		const std::string &nativeId = VuGameUtil::IF()->leaderboardDB()[lbName][mAuthType].asString();
		if ( nativeId.length() )
			submitScoreAsync(nativeId.c_str(), value);
	}
}

//*****************************************************************************
void VuGameServicesManager::onSignInSuccess(const char *gamerID, const char *gamerTag)
{
	mIsBusy = false;
	if ( !isSignedIn() )
	{
		mIsSignedIn = true;
		mGamerID = gamerID;
		mGamerTag = gamerTag;

		VuProfileManager::IF()->dataWrite()["GameServices"]["AutoSignIn"].putValue(true);

		// let listeners know
		for ( Listeners::iterator iter = mListeners.begin(); iter != mListeners.end(); iter++ )
			(*iter)->onSignIn();

		// analytics - When are players signing into game services (if at all)?
		if ( !VuProfileManager::IF()->dataRead()["GameServices"]["EverSignedIn"].asBool() )
		{
			VuAnalyticsManager::IF()->logEvent("Game Services Activated", "Highest Event Played", VuStatsManager::IF()->getHighestEventPlayedName().c_str());

			VuProfileManager::IF()->dataWrite()["GameServices"]["EverSignedIn"].putValue(true);
		}
	}
}

//*****************************************************************************
void VuGameServicesManager::onSignInFailure()
{
	mIsBusy = false;
}

//*****************************************************************************
void VuGameServicesManager::onSignOut()
{
	mIsBusy = false;
	if ( isSignedIn() )
	{
		mIsSignedIn = false;
		mGamerID = "";
		mGamerTag = VuStringDB::IF()->getString("Common_Player");

		VuProfileManager::IF()->dataWrite()["GameServices"]["AutoSignIn"].putValue(false);

		// let listeners know
		for ( Listeners::iterator iter = mListeners.begin(); iter != mListeners.end(); iter++ )
			(*iter)->onSignOut();

		resetAchievements();
	}
}

//*****************************************************************************
void VuGameServicesManager::onFriendsRetrieved(const Friends &friends)
{
	mFriends = friends;

	// let listeners know
	for ( Listeners::iterator iter = mListeners.begin(); iter != mListeners.end(); iter++ )
		(*iter)->onFriendsRetrieved();
}

//*****************************************************************************
void VuGameServicesManager::tickDecision(float fdt)
{
	if ( isSignedIn() )
	{
		mTimer += fdt;
		if ( mTimer >= REFRESH_TIMER )
		{
			mTimer = 0.0f;
			syncAchievements();
		}
	}
	else
	{
		mTimer = 0.0f;
	}
}

//*****************************************************************************
void VuGameServicesManager::syncAchievements()
{
	for ( const auto &ach : mAchievements )
	{
		if ( VuAchievementManager::IF()->isUnlocked(ach.first) && !ach.second.mUnlocked )
		{
			setAchievementUnlocked(ach.second.mNativeId.c_str());
			break;
		}
	}
}

//*****************************************************************************
void VuGameServicesManager::resetAchievements()
{
	for ( auto &ach : mAchievements )
		ach.second.mUnlocked = false;
}