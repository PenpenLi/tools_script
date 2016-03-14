//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Game Event entities
// 
//*****************************************************************************

#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Properties/VuBasicProperty.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/HAL/GamePad/VuGamePad.h"
#include "VuEngine/HAL/Keyboard/VuKeyboard.h"


//*****************************************************************************
//
// Any button or key
//
//*****************************************************************************
class VuOnAnyButtonOrKeyEntity : public VuEntity, VuKeyboard::Callback
{
	DECLARE_RTTI

public:
	VuOnAnyButtonOrKeyEntity();

	virtual void	onGameInitialize();
	virtual void	onGameRelease();

private:
	// scripting
	VuRetVal			Enable(const VuParams &params) { mEnabled = true; return VuRetVal(); }
	VuRetVal			Disable(const VuParams &params) { mEnabled = false; return VuRetVal(); }

	// VuKeyboard::Callback
	void				onKeyDown(VUUINT32 key) { mKeyPressed = true; }

	void				onTickDecision(float fdt);

	// properties
	bool				mEnabled;

	VuScriptComponent	*mpScriptComponent;
	VUUINT32			mLastButtons;
	bool				mKeyPressed;
};

IMPLEMENT_RTTI(VuOnAnyButtonOrKeyEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuOnAnyButtonOrKeyEntity);


//*****************************************************************************
VuOnAnyButtonOrKeyEntity::VuOnAnyButtonOrKeyEntity():
	mEnabled(true),
	mLastButtons(~0),
	mKeyPressed(false)
{
	// create/add script component
	addComponent(mpScriptComponent = new VuScriptComponent(this, 120));

	// properties
	addProperty(new VuBoolProperty("Enabled", mEnabled));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuOnAnyButtonOrKeyEntity, Enable);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuOnAnyButtonOrKeyEntity, Disable);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, Trigger);
}

//*****************************************************************************
void VuOnAnyButtonOrKeyEntity::onGameInitialize()
{
	VuTickManager::IF()->registerHandler(this, &VuOnAnyButtonOrKeyEntity::onTickDecision, "Decision");
	VuKeyboard::IF()->addCallback(this, false);
}

//*****************************************************************************
void VuOnAnyButtonOrKeyEntity::onGameRelease()
{
	VuTickManager::IF()->unregisterHandlers(this);
	VuKeyboard::IF()->removeCallback(this);
}

//*****************************************************************************
void VuOnAnyButtonOrKeyEntity::onTickDecision(float fdt)
{
	// check buttons
	VUUINT32 buttons = 0;
	for ( int i = 0; i < VuGamePad::MAX_NUM_PADS; i++ )
		buttons |= VuGamePad::IF()->getController(i).mButtons;
	VUUINT32 debouncedButtons = buttons & (~mLastButtons);
	mLastButtons = buttons;

	// check keys
	bool keyPressed = mKeyPressed;
	mKeyPressed = false;

	if ( mEnabled )
	{
		if ( debouncedButtons || keyPressed )
			mpScriptComponent->getPlug("Trigger")->execute();
	}
}


//*****************************************************************************
// StartLights
//*****************************************************************************
class VuStartLightsEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuStartLightsEntity();

private:
	// event handlers
	void				OnStartLightSequence(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuStartLightsEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuStartLightsEntity);

//*****************************************************************************
VuStartLightsEntity::VuStartLightsEntity()
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this, 100));

	// scripting
	const VuJsonContainer &startLightSequence = VuGameUtil::IF()->constantDB()["StartLightSequence"];
	for ( int i = 0; i < startLightSequence.size(); i++ )
		mpScriptComponent->addPlug(new VuScriptOutputPlug(startLightSequence[i]["Event"].asCString()));

	// register event handlers
	REG_EVENT_HANDLER(VuStartLightsEntity, OnStartLightSequence);
}

//*****************************************************************************
void VuStartLightsEntity::OnStartLightSequence(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	const char *step = accessor.getString();

	if ( VuScriptPlug *pPlug = mpScriptComponent->getPlug(step) )
		pPlug->execute();
}


//*****************************************************************************
// OnStartCinematicIntro
//*****************************************************************************
class VuOnStartCinematicIntroEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuOnStartCinematicIntroEntity();

private:
	// event handlers
	void				OnStartCinematicIntro(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuOnStartCinematicIntroEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuOnStartCinematicIntroEntity);

//*****************************************************************************
VuOnStartCinematicIntroEntity::VuOnStartCinematicIntroEntity()
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	// scripting
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, Trigger);

	// register event handlers
	REG_EVENT_HANDLER(VuOnStartCinematicIntroEntity, OnStartCinematicIntro);
}

//*****************************************************************************
void VuOnStartCinematicIntroEntity::OnStartCinematicIntro(const VuParams &params)
{
	mpScriptComponent->getPlug("Trigger")->execute();
}


//*****************************************************************************
// OnPreGameEnter
//*****************************************************************************
class VuOnPreGameEnterEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuOnPreGameEnterEntity();

private:
	// event handlers
	void				OnPreGameEnter(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuOnPreGameEnterEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuOnPreGameEnterEntity);

//*****************************************************************************
VuOnPreGameEnterEntity::VuOnPreGameEnterEntity()
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this, 100));

	// scripting
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, Trigger);

	// register event handlers
	REG_EVENT_HANDLER(VuOnPreGameEnterEntity, OnPreGameEnter);
}

//*****************************************************************************
void VuOnPreGameEnterEntity::OnPreGameEnter(const VuParams &params)
{
	mpScriptComponent->getPlug("Trigger")->execute();
}


//*****************************************************************************
// VuOnCarFinishedEntity
//*****************************************************************************
class VuOnCarFinishedEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuOnCarFinishedEntity();

private:
	// event handlers
	void				OnCarFinished(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;

	// properties
	int					mViewport;
};

IMPLEMENT_RTTI(VuOnCarFinishedEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuOnCarFinishedEntity);

//*****************************************************************************
VuOnCarFinishedEntity::VuOnCarFinishedEntity():
	mViewport(0)
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	// properties
	addProperty(new VuIntProperty("Viewport", mViewport));

	// scripting
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, Trigger);

	// register event handlers
	REG_EVENT_HANDLER(VuOnCarFinishedEntity, OnCarFinished);
}

//*****************************************************************************
void VuOnCarFinishedEntity::OnCarFinished(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	VuEntity *pEntity = accessor.getEntity();

	if ( pEntity == VuCarManager::IF()->getCameraTargetForViewport(mViewport) )
		mpScriptComponent->getPlug("Trigger")->execute();
}
