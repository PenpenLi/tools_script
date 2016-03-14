//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Advertising Logic entities
// 
//*****************************************************************************

#include "VuPurple/Managers/VuAdManager.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Properties/VuEnumProperty.h"


//*****************************************************************************
// AdUnit
//*****************************************************************************
class VuAdUnitEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuAdUnitEntity();

private:
	VuRetVal			IsReady(const VuParams &params);
	VuRetVal			Show(const  VuParams &params);

	// event handlers
	void				OnAdFinished(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;

	// properties
	int					mAdUnit;
};

IMPLEMENT_RTTI(VuAdUnitEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuAdUnitEntity);

//*****************************************************************************
VuAdUnitEntity::VuAdUnitEntity():
	mAdUnit(VuAdManager::AD_UNIT_INTERSTITIAL)
{
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	// properties
	static VuStaticIntEnumProperty::Choice sAdUnitTypes[] =
	{
		{ "Interstitial", VuAdManager::AD_UNIT_INTERSTITIAL},
		{ "Incentivized", VuAdManager::AD_UNIT_INCENTIVIZED},
		{ "Reward", VuAdManager::AD_UNIT_REWARD},
		{ VUNULL }
	};
	addProperty(new VuStaticIntEnumProperty("Type", mAdUnit, sAdUnitTypes));

	ADD_SCRIPT_INPUT(mpScriptComponent, VuAdUnitEntity, IsReady, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuAdUnitEntity, Show);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnCompleted);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnAborted);

	// event handlers
	REG_EVENT_HANDLER(VuAdUnitEntity, OnAdFinished);
}

//*****************************************************************************
VuRetVal VuAdUnitEntity::IsReady(const VuParams &params)
{
	
	bool isReady = VuAdManager::IF()->isAdReady(mAdUnit);

	//return VuRetVal(isReady);
	//modify by liudan
	return VuRetVal(false);
}

//*****************************************************************************
VuRetVal VuAdUnitEntity::Show(const VuParams &params)
{
	if ( !VuAdManager::IF()->showAd(mAdUnit, this) )
		mpScriptComponent->getPlug("OnAborted")->execute();
		
	return VuRetVal();
}

//*****************************************************************************
void VuAdUnitEntity::OnAdFinished(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	bool completed = accessor.getBool();

	if ( completed )
		mpScriptComponent->getPlug("OnCompleted")->execute();
	else
		mpScriptComponent->getPlug("OnAborted")->execute();
}


//*****************************************************************************
// CheckForAds
//*****************************************************************************
class VuCheckForAdsEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuCheckForAdsEntity();

private:
	VuRetVal			Check(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuCheckForAdsEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuCheckForAdsEntity);

//*****************************************************************************
VuCheckForAdsEntity::VuCheckForAdsEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this, 100));

	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuCheckForAdsEntity, Check);
}

//*****************************************************************************
VuRetVal VuCheckForAdsEntity::Check(const VuParams &params)
{
	VuAdManager::IF()->checkForAds();
	return VuRetVal();
}
