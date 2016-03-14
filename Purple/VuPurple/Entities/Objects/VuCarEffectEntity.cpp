//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuCarEffectEntity
// 
//*****************************************************************************

#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarEffectController.h"
#include "VuPurple/Dynamics/VuCollisionTypes.h"
#include "VuEngine/Entities/VuEntityRepository.h"
#include "VuEngine/Components/3dLayout/Vu3dLayoutComponent.h"
#include "VuEngine/Components/RigidBody/VuRigidBodyComponent.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Properties/VuDBEntryProperty.h"
#include "VuEngine/Managers/VuTickManager.h"


class VuCarEffectEntity : public VuEntity, public VuRigidBodyContactCallback
{
	DECLARE_RTTI

public:
	VuCarEffectEntity();

	void				onGameInitialize();
	void				onGameRelease();

protected:
	// VuRigidBodyContactCallback
	virtual bool		onRigidBodyContactAdded(VuContactPoint &cp);

	// scripting
	VuRetVal			Enable(const VuParams &params) { enable(); return VuRetVal(); }
	VuRetVal			Disable(const VuParams &params) { disable(); return VuRetVal(); }

	bool				enable();
	void				disable();

	void				drawLayout(const Vu3dLayoutDrawParams &params);

	void				tickDecision(float fdt);

	// components
	Vu3dLayoutComponent		*mp3dLayoutComponent;
	VuRigidBodyComponent	*mpRigidBodyComponent;
	VuScriptComponent		*mpScriptComponent;

	// properties
	std::string				mCarEffect;
	bool					mInitiallyEnabled;
	float					mRepeatDelay;

	struct RepeatEntry
	{
		static bool	isExpired(const RepeatEntry &entry) { return entry.mTimer <= 0.0f; }
		VUUINT32	mCarHash;
		float		mTimer;
	};
	typedef std::vector<RepeatEntry> RepeatEntries;
	RepeatEntries			mRepeatEntries;
	bool					mEnabled;
	VUUINT32				mTriggerCarHash;
};

IMPLEMENT_RTTI(VuCarEffectEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuCarEffectEntity);


//*****************************************************************************
VuCarEffectEntity::VuCarEffectEntity():
	mInitiallyEnabled(true),
	mRepeatDelay(1.0f),
	mEnabled(false),
	mTriggerCarHash(0)
{
	// components
	addComponent(mp3dLayoutComponent = new Vu3dLayoutComponent(this));
	addComponent(mpRigidBodyComponent = new VuRigidBodyComponent(this));
	addComponent(mpScriptComponent = new VuScriptComponent(this, 100, false));

	mp3dLayoutComponent->setDrawMethod(this, &VuCarEffectEntity::drawLayout);

	// properties
	addProperty(new VuDBEntryProperty("Car Effect", mCarEffect, "CarEffectDB"));
	addProperty(new VuBoolProperty("Initially Enabled", mInitiallyEnabled));
	addProperty(new VuFloatProperty("Repeat Delay", mRepeatDelay));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuCarEffectEntity, Enable);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuCarEffectEntity, Disable);
}

//*****************************************************************************
void VuCarEffectEntity::onGameInitialize()
{
	mpRigidBodyComponent->setCollisionGroup(COL_GAME_CAR_EFFECT);
	mpRigidBodyComponent->setCollisionMask(COL_GAME_CAR);
	mpRigidBodyComponent->createRigidBody();
	mpRigidBodyComponent->setCollisionFlags(mpRigidBodyComponent->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	mpRigidBodyComponent->setContactCallback(this);

	if ( mInitiallyEnabled )
		enable();
}

//*****************************************************************************
void VuCarEffectEntity::onGameRelease()
{
	disable();

	mpRigidBodyComponent->destroyRigidBody();
}

//*****************************************************************************
bool VuCarEffectEntity::enable()
{
	if ( !mEnabled )
	{
		mEnabled = true;

		mpRigidBodyComponent->addToWorld();

		VuTickManager::IF()->registerHandler(this, &VuCarEffectEntity::tickDecision, "Decision");
	}

	return mEnabled;
}

//*****************************************************************************
void VuCarEffectEntity::disable()
{
	if ( mEnabled )
	{
		mEnabled = false;

		mpRigidBodyComponent->removeFromWorld();

		VuTickManager::IF()->unregisterHandler(this, "Decision");

		mRepeatEntries.clear();
		mTriggerCarHash = 0;
	}
}

//*****************************************************************************
bool VuCarEffectEntity::onRigidBodyContactAdded(VuContactPoint &cp)
{
	VUUINT32 carHash = cp.mpOtherBody->getEntity()->getHashedLongNameFast();

	for ( RepeatEntries::iterator iter = mRepeatEntries.begin(); iter != mRepeatEntries.end(); iter++ )
		if ( iter->mCarHash == carHash )
			return false;

	mTriggerCarHash = carHash;

	return false;
}

//*****************************************************************************
void VuCarEffectEntity::drawLayout(const Vu3dLayoutDrawParams &params)
{
	if ( params.mbSelected )
		mpRigidBodyComponent->draw(VuColor(255,255,0,128), params.mCamera);
}

//*****************************************************************************
void VuCarEffectEntity::tickDecision(float fdt)
{
	if ( mTriggerCarHash )
	{
		RepeatEntry repeatEntry;
		repeatEntry.mCarHash = mTriggerCarHash;
		repeatEntry.mTimer = mRepeatDelay;

		mRepeatEntries.push_back(repeatEntry);

		VuCarEntity *pCar = static_cast<VuCarEntity *>(VuEntityRepository::IF()->findEntity(mTriggerCarHash));
		mTriggerCarHash = 0;

		pCar->getEffectController()->applyEffect(mCarEffect.c_str(), VUNULL);
	}

	for ( RepeatEntries::iterator iter = mRepeatEntries.begin(); iter != mRepeatEntries.end(); iter++ )
		iter->mTimer -= fdt;

	mRepeatEntries.erase(std::remove_if(mRepeatEntries.begin(), mRepeatEntries.end(), RepeatEntry::isExpired), mRepeatEntries.end());
}
