//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Fake PowerUp class
// 
//*****************************************************************************

#include "VuFakePowerUpEntity.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarEffectController.h"
#include "VuPurple/Dynamics/VuCollisionTypes.h"
#include "VuPurple/Util/VuCollisionUtil.h"
#include "VuEngine/Entities/VuEntityRepository.h"
#include "VuEngine/Components/3dDraw/Vu3dDrawComponent.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Pfx/VuPfx.h"
#include "VuEngine/Pfx/VuPfxManager.h"
#include "VuEngine/Pfx/VuPfxEntity.h"
#include "VuEngine/HAL/Audio/VuAudio.h"


// constants
#define SELF_COLLISION_TIME 5.0f

IMPLEMENT_RTTI(VuFakePowerUpEntity, VuEntity);


class VuFakePowerUpRigidBody : public VuRigidBody
{
public:
	VuFakePowerUpRigidBody(const btRigidBody::btRigidBodyConstructionInfo &info, VuEntity *pEntity, VUINT16 collisionGroup, VUINT16 collisionMask):
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
VuFakePowerUpEntity::VuFakePowerUpEntity(const VuJsonContainer &data, VuCarEntity *pCar, const VuMatrix &transform):
	mData(data),
	mpLauncher(pCar),
	mTransform(transform),
	mpStaticPfxSystem(VUNULL),
	mAge(0.0f),
	mBreakNow(false),
	mCollisionVel(0,0,0)
{
	// components
	addComponent(mp3dDrawComponent = new Vu3dDrawComponent(this));

	mp3dDrawComponent->setDrawMethod(this, &VuFakePowerUpEntity::draw);

	// collision shape
	VuCollisionUtil::createCollisionShape(data, mpShape, mpChildShape);

	// rigid body
	btRigidBody::btRigidBodyConstructionInfo info(0.0f, VUNULL, mpShape);
	info.m_startWorldTransform = VuDynamicsUtil::toBtTransform(mTransform);
	mpRigidBody = new VuFakePowerUpRigidBody(info, this, COL_GAME_POWERUP, COL_GAME_CAR);
	mpRigidBody->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	mpRigidBody->setContactCallback(this);
	mpRigidBody->mpIgnoreBody = pCar->getRigidBody();

	// more params
	mStaticPfx = data["StaticPfx"].asString();
	mBreakPfx = data["BreakPfx"].asString();
	mBreakSfx = data["BreakSfx"].asString();
	mDrawDistance = data["DrawDistance"].asFloat();
}

//****************************************************************************
VuFakePowerUpEntity::~VuFakePowerUpEntity()
{
	delete mpShape;
	delete mpChildShape;
	delete mpRigidBody;
}

//*****************************************************************************
void VuFakePowerUpEntity::onGameInitialize()
{
	mpTransformComponent->setWorldTransform(mTransform);

	if ( (mpStaticPfxSystem = VuPfx::IF()->createSystemInstance(mStaticPfx.c_str())) )
	{
		mpStaticPfxSystem->setMatrix(mpTransformComponent->getWorldTransform());
		mpStaticPfxSystem->start();

		VuDynamics::IF()->addRigidBody(mpRigidBody);

		mp3dDrawComponent->show();

		VuTickManager::IF()->registerHandler(this, &VuFakePowerUpEntity::tickDecision, "Decision");
		VuTickManager::IF()->registerHandler(this, &VuFakePowerUpEntity::tickBuild, "Build");
	}
}

//*****************************************************************************
void VuFakePowerUpEntity::onGameRelease()
{
	if ( mpStaticPfxSystem )
	{
		VuPfx::IF()->releaseSystemInstance(mpStaticPfxSystem);
		mpStaticPfxSystem = VUNULL;

		VuDynamics::IF()->removeRigidBody(mpRigidBody);

		mp3dDrawComponent->hide();

		VuTickManager::IF()->unregisterHandlers(this);
	}
}

//*****************************************************************************
bool VuFakePowerUpEntity::onRigidBodyContactAdded(VuContactPoint &cp)
{
	// check if we should break
	if ( cp.mpOtherBody->hasContactResponse() && !mBreakNow )
	{
		VuEntity *pOtherEntity = cp.mpOtherBody->getEntity();
		if ( pOtherEntity && pOtherEntity->isDerivedFrom(VuCarEntity::msRTTI) )
		{
			VuCarEntity *pCar = static_cast<VuCarEntity *>(pOtherEntity);

			mCollisionVel = cp.mpOtherBody->getVuLinearVelocity();
			mBreakerEntity = pCar;
			mBreakNow = true;
		}
	}

	return false;
}

//*****************************************************************************
void VuFakePowerUpEntity::tickDecision(float fdt)
{
	mAge += fdt;
	if ( mAge > SELF_COLLISION_TIME )
		mpRigidBody->mpIgnoreBody = VUNULL;

	if ( mBreakNow )
	{
		// pfx
		if ( VUUINT32 hPfx = VuPfxManager::IF()->createEntity(mBreakPfx.c_str(), true) )
		{
			if ( VuPfxEntity *pPfxEntity = VuPfxManager::IF()->getEntity(hPfx) )
			{
				pPfxEntity->getSystemInstance()->setMatrix(mpTransformComponent->getWorldTransform());
				pPfxEntity->getSystemInstance()->setLinearVelocity(mCollisionVel);
				pPfxEntity->getSystemInstance()->start();
			}
		}

		// sfx
		#if !VU_DISABLE_AUDIO
		if ( mBreakSfx.length() )
		{
			FMOD::Event *pEvent;
			if ( VuAudio::IF()->eventSystem()->getEvent(mBreakSfx.c_str(), FMOD_EVENT_NONBLOCKING, &pEvent) == FMOD_OK )
			{
				FMOD_VECTOR pos = VuAudio::toFmodVector(mpTransformComponent->getWorldPosition());
				pEvent->set3DAttributes(&pos, VUNULL, VUNULL);
				pEvent->start();
			}
		}
		#endif

		if ( mBreakerEntity.get() )
			mBreakerEntity->getEffectController()->applyEffect(mData["CarEffect"].asCString(), mpLauncher);

		mBreakNow = false;
		mBreakerEntity = VUNULL;

		VuEntityRepository::IF()->removeManagedEntity(this);
	}
}

//*****************************************************************************
void VuFakePowerUpEntity::tickBuild(float fdt)
{
	mpStaticPfxSystem->tick(fdt, false);
	if ( mpStaticPfxSystem->getAabb().isValid() )
		mp3dDrawComponent->updateVisibility(mpStaticPfxSystem->getAabb());
}

//*****************************************************************************
void VuFakePowerUpEntity::draw(const VuGfxDrawParams &params)
{
	if ( params.mbDrawReflection )
		return;

	float distSquared = (mTransform.getTrans() - params.mEyePos).magSquared();
	if ( distSquared > VuSquare(mDrawDistance) )
		return;

	if ( mpStaticPfxSystem )
		mpStaticPfxSystem->draw(params);
}
