//*****************************************************************************
//
//  Copyright (c) 2012-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  News entities
// 
//*****************************************************************************

#include "VuPurple/Managers/VuCommunityManager.h"
#include "VuPurple/Managers/VuAnalyticsManager.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Cloud/VuCloudManager.h"
#include "VuEngine/DB/VuStringDB.h"
#include "VuEngine/Util/VuBase64.h"
#include "VuEngine/Util/VuImageUtil.h"


//*****************************************************************************
// NewsAvailable
//*****************************************************************************

class VuNewsInfoEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuNewsInfoEntity();

protected:
	// scripting
	VuRetVal			IsAvailable(const VuParams &params);
	VuRetVal			IsInterstitial(const VuParams &params);
	VuRetVal			IsUnread(const VuParams &params);

	// events
	void				OnNewsReceived(const VuParams &params) { mpScriptComponent->getPlug("OnReceived")->execute(); }

	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuNewsInfoEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuNewsInfoEntity);

//*****************************************************************************
VuNewsInfoEntity::VuNewsInfoEntity()
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	// scripting
	ADD_SCRIPT_INPUT(mpScriptComponent, VuNewsInfoEntity, IsAvailable, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT(mpScriptComponent, VuNewsInfoEntity, IsInterstitial, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT(mpScriptComponent, VuNewsInfoEntity, IsUnread, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnReceived);

	// event handlers
	REG_EVENT_HANDLER(VuNewsInfoEntity, OnNewsReceived);
}

//*****************************************************************************
VuRetVal VuNewsInfoEntity::IsAvailable(const VuParams &params)
{
	bool available = false;

	if ( VuCloudManager::IF() )
		available = VuCloudManager::IF()->isNewsAvailable();

	return VuRetVal(available);
}

//*****************************************************************************
VuRetVal VuNewsInfoEntity::IsInterstitial(const VuParams &params)
{
	bool interstitial = false;

	if ( VuCloudManager::IF() )
		interstitial = VuCloudManager::IF()->isNewsAvailable() && VuCloudManager::IF()->isNewsInterstitial();

	return VuRetVal(interstitial);
}

//*****************************************************************************
VuRetVal VuNewsInfoEntity::IsUnread(const VuParams &params)
{
	bool unread = false;

	if ( VuCloudManager::IF() )
		unread = VuCloudManager::IF()->isNewsAvailable() && VuCloudManager::IF()->isNewsUnread();

	return VuRetVal(unread);
}


//*****************************************************************************
// NewsTrack
//*****************************************************************************

class VuNewsTrackEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuNewsTrackEntity();

protected:
	// scripting
	VuRetVal			Trigger(const VuParams &params);

	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuNewsTrackEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuNewsTrackEntity);

//*****************************************************************************
VuNewsTrackEntity::VuNewsTrackEntity()
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this, 100));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuNewsTrackEntity, Trigger);
}

//*****************************************************************************
VuRetVal VuNewsTrackEntity::Trigger(const VuParams &params)
{
	if ( VuCloudManager::IF() )
		VuCloudManager::IF()->onNewsLaunched();

	return VuRetVal();
}
