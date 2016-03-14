//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Jump point entities
// 
//*****************************************************************************

#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Components/3dLayout/Vu3dLayoutComponent.h"
#include "VuEngine/Components/Transform/VuTransformComponent.h"
#include "VuEngine/Properties/VuEnumProperty.h"
#include "VuEngine/HAL/Keyboard/VuKeyboard.h"


//*****************************************************************************
//  Jump point entity
//*****************************************************************************
class VuJumpPointEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuJumpPointEntity();

private:
	// scripting
	VuRetVal			Trigger(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;
	Vu3dLayoutComponent	*mp3dLayoutComponent;

	bool				mGhosting;
};


IMPLEMENT_RTTI(VuJumpPointEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuJumpPointEntity);

//*****************************************************************************
VuJumpPointEntity::VuJumpPointEntity():
	mGhosting(true)
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this, 150, false));
	addComponent(mp3dLayoutComponent = new Vu3dLayoutComponent(this));

	addProperty(new VuBoolProperty("Ghosting", mGhosting));

	mpTransformComponent->setMask(VuTransformComponent::TRANS|VuTransformComponent::ROT);

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuJumpPointEntity, Trigger);
}

//*****************************************************************************
VuRetVal VuJumpPointEntity::Trigger(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	VuEntity *pEntity = (accessor.getNextType() == VuParams::Entity) ? accessor.getEntity() : VUNULL;

	VuCarEntity *pCar = VUNULL;
	if ( pEntity && pEntity->isDerivedFrom(VuCarEntity::msRTTI) )
		pCar = static_cast<VuCarEntity *>(pEntity);
	else if ( VuCarManager::IF()->getLocalHumanCarCount() )
		pCar = VuCarManager::IF()->getLocalHumanCar(0);

	if ( pCar )
		pCar->recoverTo(mpTransformComponent->getWorldPosition(), mpTransformComponent->getWorldRotation(), mGhosting, true);

	return VuRetVal();
}


//*****************************************************************************
//  Developer jump point entity
//*****************************************************************************
class VuDevJumpPointEntity : public VuEntity, VuKeyboard::Callback
{
	DECLARE_RTTI

public:
	VuDevJumpPointEntity();

	void				onGameInitialize();
	void				onGameRelease();

private:
	// VuKeyboard::Callback
	virtual void		onKeyDown(VUUINT32 key);

	// components
	Vu3dLayoutComponent		*mp3dLayoutComponent;

	// properties
	int					mKey;
	bool				mbShift;
	bool				mbAlt;
};


IMPLEMENT_RTTI(VuDevJumpPointEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuDevJumpPointEntity);


//*****************************************************************************
VuDevJumpPointEntity::VuDevJumpPointEntity():
	mKey(VUKEY_NONE),
	mbShift(false),
	mbAlt(false)
{
	// components
	addComponent(mp3dLayoutComponent = new Vu3dLayoutComponent(this));

	mpTransformComponent->setMask(VuTransformComponent::TRANS|VuTransformComponent::ROT);

	// properties
	static VuStaticIntEnumProperty::Choice sChoices[] =
	{
		{ "<none>", VUKEY_NONE },
		{ "1", VUKEY_1 }, { "2", VUKEY_2 }, { "3", VUKEY_3 }, { "4", VUKEY_4 }, { "5", VUKEY_5 },
		{ "6", VUKEY_6 }, { "7", VUKEY_7 }, { "8", VUKEY_8 }, { "9", VUKEY_9 }, { "0", VUKEY_0 },
		{ VUNULL }
	};
	addProperty(new VuStaticIntEnumProperty("Key", mKey, sChoices));
	addProperty(new VuBoolProperty("Shift", mbShift));
	addProperty(new VuBoolProperty("Alt", mbAlt));
}

//*****************************************************************************
void VuDevJumpPointEntity::onGameInitialize()
{
	VuKeyboard::IF()->addCallback(this);
}

//*****************************************************************************
void VuDevJumpPointEntity::onGameRelease()
{
	VuKeyboard::IF()->removeCallback(this);
}

//*****************************************************************************
void VuDevJumpPointEntity::onKeyDown(VUUINT32 key)
{
	if ( mbShift && !VuKeyboard::IF()->isKeyDown(VUKEY_SHIFT) )
		return;

	if ( mbAlt && !VuKeyboard::IF()->isKeyDown(VUKEY_ALT) )
		return;

	if ( key == mKey )
	{
		if ( VuCarManager::IF()->getLocalHumanCarCount() )
		{
			VuCarEntity *pCar = VuCarManager::IF()->getLocalHumanCar(0);
			pCar->recoverTo(mpTransformComponent->getWorldPosition(), mpTransformComponent->getWorldRotation(), true, true);
			pCar->setHasCheated();
		}
	}
}
