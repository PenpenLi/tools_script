//*****************************************************************************
//
//  Copyright (c) 2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuGameServicesEntity
// 
//*****************************************************************************

#include "VuPurple/Managers/VuGameServicesManager.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"


//*****************************************************************************
//  Sign in entity
//*****************************************************************************
class VuGameServicesEntity : public VuEntity, VuGameServicesManager::VuListener
{
	DECLARE_RTTI

public:
	VuGameServicesEntity();

	virtual void		onGameInitialize();
	virtual void		onGameRelease();

protected:
	// scripting
	VuRetVal			AutoSignIn(const VuParams &params);
	VuRetVal			ForceSignIn(const VuParams &params);
	VuRetVal			ForceSignOut(const VuParams &params);
	VuRetVal			IsSignedIn(const VuParams &params);
	VuRetVal			ShowAchievements(const VuParams &params);

	// VuSignInManager::VuListener
	virtual void		onSignIn();
	virtual void		onSignOut();

	// components
	VuScriptComponent	*mpScriptComponent;
};


IMPLEMENT_RTTI(VuGameServicesEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuGameServicesEntity);


//*****************************************************************************
VuGameServicesEntity::VuGameServicesEntity()
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuGameServicesEntity, AutoSignIn);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuGameServicesEntity, ForceSignIn);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuGameServicesEntity, ForceSignOut);
	ADD_SCRIPT_INPUT(mpScriptComponent, VuGameServicesEntity, IsSignedIn, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuGameServicesEntity, ShowAchievements);

	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnSignIn);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnSignOut);
}

//*****************************************************************************
void VuGameServicesEntity::onGameInitialize()
{
	VuGameServicesManager::IF()->addListener(this);
}

//*****************************************************************************
void VuGameServicesEntity::onGameRelease()
{
	VuGameServicesManager::IF()->removeListener(this);
}

//*****************************************************************************
VuRetVal VuGameServicesEntity::AutoSignIn(const VuParams &params)
{
	VuGameServicesManager::IF()->startAutoSignIn();

	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuGameServicesEntity::ForceSignIn(const VuParams &params)
{
	VuGameServicesManager::IF()->startSignIn();

	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuGameServicesEntity::ForceSignOut(const VuParams &params)
{
	VuGameServicesManager::IF()->startSignOut();

	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuGameServicesEntity::IsSignedIn(const VuParams &params)
{
	return VuRetVal(VuGameServicesManager::IF()->isSignedIn());
}

//*****************************************************************************
VuRetVal VuGameServicesEntity::ShowAchievements(const VuParams &params)
{
	VuGameServicesManager::IF()->showAchievements();

	return VuRetVal();
}

//*****************************************************************************
void VuGameServicesEntity::onSignIn()
{
	mpScriptComponent->getPlug("OnSignIn")->execute();
}

//*****************************************************************************
void VuGameServicesEntity::onSignOut()
{
	mpScriptComponent->getPlug("OnSignOut")->execute();
}
