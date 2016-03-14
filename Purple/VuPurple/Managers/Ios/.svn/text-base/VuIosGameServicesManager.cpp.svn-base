//*****************************************************************************
//
//  Copyright (c) 2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuIosGameServicesManager
//
//*****************************************************************************

#include "VuPurple/Managers/VuGameServicesManager.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Events/VuEventMap.h"
#include "VuEngine/Json/VuJsonContainer.h"
#include "VuEngine/Json/VuJsonReader.h"
#include "VuEngine/Managers/VuTickManager.h"

#include "VuPurple/Managers/Ios/VuIosGameServicesManagerObjC.h"

class VuIosGameServicesManager : public VuGameServicesManager
{
	DECLARE_EVENT_MAP

public:
	VuIosGameServicesManager();
	
	// platform-specific functionality
	static VuIosGameServicesManager *IF() { return static_cast<VuIosGameServicesManager *>(VuGameServicesManager::IF()); }
	void			onIosCheckStateResult(bool signedIn, const char* pId, const char* pTag);

protected:
	virtual void	startSignInAsync();
	virtual void	startSignOutAsync() {}
	void			tickDecision(float fdt);

	virtual void	setAchievementUnlocked(const char *nativeId);
	virtual void	showAchievements();
	virtual void	submitScoreAsync(const char *nativeId, int value);

	float			mCheckTimer;
	
	// event handlers
	void			OnIosSignInSuccess(const VuParams &params);
	void			OnIosSignInFailure(const VuParams &params);
	void			OnIosFriendsReceived(const VuParams &params);
	void			OnIosAddFriend(const VuParams &params);
	
private:
	Friends			mTempFriendList;
};

// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuGameServicesManager, VuIosGameServicesManager);

//*****************************************************************************
VuIosGameServicesManager::VuIosGameServicesManager()
{
	mCheckTimer = 0.0f;

	VuTickManager::IF()->registerHandler(this, &VuIosGameServicesManager::tickDecision, "Decision");

	REG_EVENT_HANDLER(VuIosGameServicesManager, OnIosSignInSuccess);
	REG_EVENT_HANDLER(VuIosGameServicesManager, OnIosSignInFailure);
	REG_EVENT_HANDLER(VuIosGameServicesManager, OnIosFriendsReceived);
	REG_EVENT_HANDLER(VuIosGameServicesManager, OnIosAddFriend);
	
	mAuthType = "GameCenter";
}

//*****************************************************************************
//
// Sign-In
//
//*****************************************************************************

#define SIGN_IN_CHECK_PERIOD 10.0f

//*****************************************************************************
void VuIosGameServicesManager::tickDecision(float fdt)
{
	mCheckTimer += fdt;
	if (mCheckTimer > SIGN_IN_CHECK_PERIOD)
	{
		mCheckTimer = 0.0f;

		VuIosGameServicesManagerObjC::checkState();
	}
}

//*****************************************************************************
void VuIosGameServicesManager::startSignInAsync()
{
	VuIosGameServicesManagerObjC::startSignInAsync();
}


//*****************************************************************************
//
// Called as part of periodic check that GameCenter is still available and that
// the local player is still authenticated.
//
void VuIosGameServicesManager::onIosCheckStateResult(bool signedIn, const char* pId, const char* pTag)
{
	if (isSignedIn() && !signedIn)
	{
		onSignOut();
	}

	if (signedIn && pId != VUNULL && pTag != VUNULL)
	{
		onSignInSuccess(mGamerID.c_str(), mGamerTag.c_str());
	}
}

//
// Achievements
//

//*****************************************************************************
void VuIosGameServicesManager::setAchievementUnlocked(const char *nativeId)
{
	if (isSignedIn() && !isBusy())
	{
		VuIosGameServicesManagerObjC::unlockAchievement(nativeId);
	}
}

//*****************************************************************************
void VuIosGameServicesManager::showAchievements()
{
	if (isSignedIn() && !isBusy())
	{
		VuIosGameServicesManagerObjC::showAchievements();
	}
}

//
// Leaderboards
//

//*****************************************************************************
void VuIosGameServicesManager::submitScoreAsync(const char *nativeId, int value)
{
	if (isSignedIn())
	{
		VuIosGameServicesManagerObjC::submitScore(nativeId, value);
	}
}


// Event Handlers for straddling ObjC and C++. These events are posted by
// VuIosGameServicesManagerObjC:: namespace functions, and are handled by
// these functions below:

//*****************************************************************************
void VuIosGameServicesManager::OnIosSignInSuccess(const VuParams &params)
{
	VuParams::VuAccessor data(params);
	
	const char* pId = data.getString();
	const char* pTag = data.getString();
	
	onSignInSuccess(pId, pTag);
}

//*****************************************************************************
void VuIosGameServicesManager::OnIosSignInFailure(const VuParams &params)
{
	onSignInFailure();
}

//*****************************************************************************
void VuIosGameServicesManager::OnIosFriendsReceived(const VuParams &params)
{
	onFriendsRetrieved(mTempFriendList);
	
	mTempFriendList.clear();
}

//*****************************************************************************
void VuIosGameServicesManager::OnIosAddFriend(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	
	FriendInfo info;
	
	info.mGamerID = accessor.getString();
	info.mGamerTag = accessor.getString();
	
	mTempFriendList.push_back(info);
}

// Objective C glue (VuIosGameServicesManagerObjC namespace)
//*****************************************************************************
void VuIosGameServicesManagerObjC::onSignInSuccess(const char* pId, const char* pTag)
{
	VuParams params;
	
	params.addString(pId);
	params.addString(pTag);
	
	VuEventManager::IF()->broadcastDelayed(0.0f, true, "OnIosSignInSuccess", params);
}

//*****************************************************************************
void VuIosGameServicesManagerObjC::onSignInFailure()
{
	VuEventManager::IF()->broadcastDelayed(0.0f, true, "OnIosSignInFailure");
}

//*****************************************************************************
void VuIosGameServicesManagerObjC::onCheckStateResult(bool signedIn, const char* pId, const char* pTag)
{
	VuIosGameServicesManager::IF()->onIosCheckStateResult(signedIn, pId, pTag);
}

//*****************************************************************************
void VuIosGameServicesManagerObjC::onUnlockAchievementResult(bool success)
{
	// report success?? don't care?
	
}

//*****************************************************************************
void VuIosGameServicesManagerObjC::onIosFriendsReceived(const char* pData)
{
	if (!pData)
	{
		return;
	}

	VuJsonContainer friends;
	VuJsonReader reader;
	
	if (reader.loadFromString(friends, pData))
	{
		
		for (int i=0; i<friends.size()-1; i+=2)
		{
			VuParams params;
			
			const char* pString = friends[i].asString().c_str();
			params.addString(pString);
			pString = friends[i+1].asString().c_str();
			params.addString(pString);
			
			VuEventManager::IF()->broadcastDelayed(0.0f, true, "OnIosAddFriend", params);
		}
		
		VuEventManager::IF()->broadcastDelayed(0.0f, true, "OnIosFriendsReceived");
	}
}

//*****************************************************************************

