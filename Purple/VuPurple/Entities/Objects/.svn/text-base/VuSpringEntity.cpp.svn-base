//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Spring class
// 
//*****************************************************************************

#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Dynamics/VuCollisionTypes.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/3dLayout/Vu3dLayoutComponent.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Components/RigidBody/VuRigidBodyComponent.h"
#include "VuEngine/Properties/VuAudioProperty.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Dynamics/VuDynamics.h"
#include "VuEngine/HAL/Audio/VuAudio.h"
#include "VuEngine/Math/VuUnitConversion.h"


class VuSpringRigidBody;


class VuSpringEntity : public VuEntity, public VuRigidBodyContactCallback
{
	DECLARE_RTTI

public:
	VuSpringEntity();

	virtual void	onGameInitialize();
	virtual void	onGameRelease();

protected:
	// scripting
	VuRetVal		Enable(const VuParams &params)	{ enable(); return VuRetVal(); }
	VuRetVal		Disable(const VuParams &params)	{ disable(); return VuRetVal(); }

	// VuRigidBodyContactCallback
	virtual bool	onRigidBodyContactAdded(VuContactPoint &cp);

	void			enable();
	void			disable();

	void			tickDecision(float fdt);

	void			drawLayout(const Vu3dLayoutDrawParams &params);

	// components
	Vu3dLayoutComponent		*mp3dLayoutComponent;
	VuScriptComponent		*mpScriptComponent;
	VuRigidBodyComponent	*mpRigidBodyComponent;

	// properties
	bool					mInitiallyEnabled;
	float					mVerticalSpeed;
	std::string				mSpringSfx;
	float					mResetTime;

	enum eState { STATE_DISABLED, STATE_ACTIVE, STATE_SPRUNG };
	eState					mState;
	float					mSprungTimer;
	bool					mSpringNow;
	VuWeakRef<VuCarEntity>	mBreakerEntity;
};

IMPLEMENT_RTTI(VuSpringEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuSpringEntity);


class VuSpringRigidBody : public VuRigidBody
{
public:
	VuSpringRigidBody(const btRigidBody::btRigidBodyConstructionInfo &info, VuEntity *pEntity, VUINT16 collisionGroup, VUINT16 collisionMask):
		VuRigidBody(info, pEntity, collisionGroup, collisionMask),
		mpIgnoreBody(VUNULL)
	{
		m_checkCollideWith = true;
	}

	virtual bool checkCollideWithOverride(const  btCollisionObject* co) const
	{
		const VuRigidBody *pOtherBody = static_cast<const VuRigidBody *>(co);
		if ( pOtherBody == mpIgnoreBody )
			return false;

		return true;
	}

	VuRigidBody	*mpIgnoreBody;
};


//*****************************************************************************
VuSpringEntity::VuSpringEntity():
	mInitiallyEnabled(true),
	mVerticalSpeed(50.0f),
	mResetTime(5.0f),
	mState(STATE_DISABLED),
	mSprungTimer(0.0f),
	mSpringNow(false)
{
	// components
	addComponent(mp3dLayoutComponent = new Vu3dLayoutComponent(this));
	addComponent(mpScriptComponent = new VuScriptComponent(this, 150, false));
	addComponent(mpRigidBodyComponent = new VuRigidBodyComponent(this));

	mp3dLayoutComponent->setDrawMethod(this, &VuSpringEntity::drawLayout);

	// properties
	addProperty(new VuBoolProperty("Initially Active", mInitiallyEnabled));
	addProperty(new VuFloatProperty("Vertical Speed", mVerticalSpeed));
	addProperty(new VuFloatProperty("Reset Time", mResetTime));
	addProperty(new VuAudioEventNameProperty("Spring Sfx", mSpringSfx));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuSpringEntity, Enable);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuSpringEntity, Disable);
	ADD_SCRIPT_OUTPUT(mpScriptComponent, OnSprung, VuRetVal::Void, VuParamDecl(1, VuParams::Entity));
}

//*****************************************************************************
void VuSpringEntity::onGameInitialize()
{
	mpRigidBodyComponent->setContactCallback(this);
	mpRigidBodyComponent->setCollisionGroup(COL_GAME_SPRING);
	mpRigidBodyComponent->setCollisionMask(COL_GAME_CAR);
	mpRigidBodyComponent->setCollisionFlags(mpRigidBodyComponent->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	mpRigidBodyComponent->createRigidBody();

	VUUINT32 test = mpRigidBodyComponent->getCollisionFlags();

	if ( mInitiallyEnabled )
		enable();

	// register phased ticks
	VuTickManager::IF()->registerHandler(this, &VuSpringEntity::tickDecision, "Decision");
}

//*****************************************************************************
void VuSpringEntity::onGameRelease()
{
	disable();
	mpRigidBodyComponent->destroyRigidBody();

	// unregister phased tick
	VuTickManager::IF()->unregisterHandlers(this);
}

//*****************************************************************************
bool VuSpringEntity::onRigidBodyContactAdded(VuContactPoint &cp)
{
	// check if we should break
	if (cp.mpOtherBody->hasContactResponse() && !mSpringNow)
	{
		VuEntity *pOtherEntity = cp.mpOtherBody->getEntity();
		VuCarEntity *pCar = (pOtherEntity && pOtherEntity->isDerivedFrom(VuCarEntity::msRTTI)) ? static_cast<VuCarEntity *>(pOtherEntity) : VUNULL;

		mBreakerEntity = pCar;
		mSpringNow = true;
	}

	return false;
}

//*****************************************************************************
void VuSpringEntity::enable()
{
	if ( mState != STATE_ACTIVE )
	{
		mpRigidBodyComponent->addToWorld();
		mState = STATE_ACTIVE;
	}
}

//*****************************************************************************
void VuSpringEntity::disable()
{
	mpRigidBodyComponent->removeFromWorld();
	mState = STATE_DISABLED;
	mSpringNow = false;
}

//*****************************************************************************
void VuSpringEntity::tickDecision(float fdt)
{
	if ( mSpringNow )
	{
		mpRigidBodyComponent->removeFromWorld();

		// sfx
		#if !VU_DISABLE_AUDIO
		if ( mSpringSfx.length() )
		{
			FMOD::Event *pEvent;
			if ( VuAudio::IF()->eventSystem()->getEvent(mSpringSfx.c_str(), FMOD_EVENT_NONBLOCKING, &pEvent) == FMOD_OK )
			{
				FMOD_VECTOR pos = VuAudio::toFmodVector(mpTransformComponent->getWorldPosition());
				pEvent->set3DAttributes(&pos, VUNULL, VUNULL);
				pEvent->start();
			}
		}
		#endif

		// apply effect to car
		VuCarEntity *pCar = mBreakerEntity.get();
		if ( pCar )
		{
			VuVector3 normal = mpTransformComponent->getWorldTransform().getAxisZ();
			VuVector3 linVel = pCar->getLinearVelocity();
			linVel -= normal*VuDot(linVel, normal);

			linVel += VuMphToMetersPerSecond(mVerticalSpeed)*normal;

			pCar->getRigidBody()->setVuLinearVelocity(linVel);
		}

		mSpringNow = false;
		mBreakerEntity = VUNULL;

		// next state
		mState = STATE_SPRUNG;
		mSprungTimer = 0.0f;

		VuParams params;
		params.addEntity(pCar);
		mpScriptComponent->getPlug("OnSprung")->execute(params);
	}
	else if ( mState == STATE_SPRUNG )
	{
		mSprungTimer += fdt;

		if ( mSprungTimer >= mResetTime )
			enable();
	}
}

//*****************************************************************************
void VuSpringEntity::drawLayout(const Vu3dLayoutDrawParams &params)
{
	mpRigidBodyComponent->draw(VuColor(255,0,255), params.mCamera);
}
