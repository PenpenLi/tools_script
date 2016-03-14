//*****************************************************************************
//
//  Copyright (c) 2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuStubGameServicesManager
//
//*****************************************************************************

#include "VuPurple/Managers/VuGameServicesManager.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Events/VuEventMap.h"
#include "VuEngine/Managers/VuMessageBoxManager.h"
#include "VuEngine/Dev/VuDevConfig.h"


class VuStubGameServicesManager : public VuGameServicesManager
{
	DECLARE_EVENT_MAP

public:
	VuStubGameServicesManager();

protected:
	virtual void	startSignInAsync();
	virtual void	startSignOutAsync();
	virtual void	setAchievementUnlocked(const char *nativeId) {}
	virtual void	submitScoreAsync(const char *nativeId, int value) {}
	virtual void	showAchievements() {}

	// event handlers
	void			OnStubSignInSuccess(const VuParams &params);
	void			OnStubSignInFailure(const VuParams &params) { onSignInFailure(); }
	void			OnStubSignOut(const VuParams &params) { onSignOut(); }
};

// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuGameServicesManager, VuStubGameServicesManager);


//*****************************************************************************
VuStubGameServicesManager::VuStubGameServicesManager()
{
	REG_EVENT_HANDLER(VuStubGameServicesManager, OnStubSignInSuccess);
	REG_EVENT_HANDLER(VuStubGameServicesManager, OnStubSignInFailure);
	REG_EVENT_HANDLER(VuStubGameServicesManager, OnStubSignOut);

	mAuthType = "Dev";
}

//*****************************************************************************
void VuStubGameServicesManager::startSignInAsync()
{
	const std::string &gamerID = VuDevConfig::IF()->getParam("StubGamerID").asString();
	const std::string &gamerTag = VuDevConfig::IF()->getParam("StubGamerTag").asString();
	if ( gamerID.length() && gamerTag.length() )
	{
		VuParams params;
		params.addString(gamerID.c_str());
		params.addString(gamerTag.c_str());
		VuEventManager::IF()->broadcastDelayed(0.5f, true, "OnStubSignInSuccess", params);
	}
	else
	{
		VuMessageBoxParams messageBoxParams;
		messageBoxParams.mType = "SimpleA";
		messageBoxParams.mHeading = "StubSignInManager";
		messageBoxParams.mBody = "\"StubGamerID\" and/or \"StubGamerTag\" missing from DevConfig";
		messageBoxParams.mTextA = "OK";

		VuMessageBox *pMB = VuMessageBoxManager::IF()->create(messageBoxParams);
		pMB->removeRef();

		VuEventManager::IF()->broadcastDelayed(0.5f, true, "OnStubSignInFailure");
	}
}

//*****************************************************************************
void VuStubGameServicesManager::startSignOutAsync()
{
	VuEventManager::IF()->broadcastDelayed(0.5f, true, "OnStubSignOut");
}

//*****************************************************************************
void VuStubGameServicesManager::OnStubSignInSuccess(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	const char *gamerID = accessor.getString();
	const char *gamerTag = accessor.getString();

	const VuJsonContainer &friends = VuDevConfig::IF()->getParam("StubFriends");
	mFriends.resize(friends.size());
	for ( int i = 0; i < friends.size(); i++ )
	{
		mFriends[i].mGamerID = friends[i][0].asString();
		mFriends[i].mGamerTag = friends[i][1].asString();
	}

	onSignInSuccess(gamerID, gamerTag);
}
