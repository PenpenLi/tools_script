//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Spring Ball class
// 
//*****************************************************************************

#include "VuSpringBallEntity.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarEffectController.h"
#include "VuPurple/Dynamics/VuCollisionTypes.h"
#include "VuEngine/Entities/VuEntityRepository.h"
#include "VuEngine/Components/3dDraw/Vu3dDrawComponent.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/HAL/Audio/VuAudio.h"
#include "VuEngine/Json/VuJsonContainer.h"
#include "VuEngine/Math/VuUnitConversion.h"
#include "VuEngine/Util/VuAudioUtil.h"


IMPLEMENT_RTTI(VuSpringBallEntity, VuEntity);


class VuSpringBallRigidBody : public VuRigidBody
{
public:
	VuSpringBallRigidBody(const btRigidBody::btRigidBodyConstructionInfo &info, VuEntity *pEntity, VUINT16 collisionGroup, VUINT16 collisionMask):
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
VuSpringBallEntity::VuSpringBallEntity(const VuJsonContainer &data, VuCarEntity *pCar, const VuMatrix &launchTransform, const VuVector3 &launchVel):
	mData(data),
	mpLauncher(pCar),
	mLaunchTransform(launchTransform),
	mLaunchVelocity(launchVel),
	mpShape(VUNULL),
	mAge(0.0f),
	mSpringNow(false)
{
	// components
	addComponent(mp3dDrawComponent = new Vu3dDrawComponent(this));

	mp3dDrawComponent->setDrawMethod(this, &VuSpringBallEntity::draw);
	mp3dDrawComponent->setDrawShadowMethod(this, &VuSpringBallEntity::drawShadow);

	// static model
	mModelInstance.setModelAsset(data["Model"].asString());
	mModelInstance.setRejectionScaleModifier(0.0f);

	// more params
	mRadius = data["Radius"].asFloat();
	mMass = data["Mass"].asFloat();
	mLinearDamping = data["LinearDamping"].asFloat();
	mDrawDistance = data["DrawDistance"].asFloat();
	mCarEffect = data["CarEffect"].asString();
	mSelfCollisionTime = data["SelfCollisionTime"].asFloat();
	mLifeTime = data["LifeTime"].asFloat();
}

//*****************************************************************************
void VuSpringBallEntity::onGameInitialize()
{
	mpTransformComponent->setWorldTransform(mLaunchTransform);

	if ( mModelInstance.getAabb().isValid() )
	{
		mpTransformComponent->setLocalScale(VuVector3(mRadius*mModelInstance.getAabb().mMax.mX));

		VuMatrix mat = mpTransformComponent->getWorldTransform();
		mat.scaleLocal(mpTransformComponent->getWorldScale());
		mp3dDrawComponent->updateVisibility(mModelInstance.getAabb(), mat);
		mp3dDrawComponent->show();
	}

	// shape
	mpShape = new btSphereShape(mRadius);

	// rigid body
	btRigidBody::btRigidBodyConstructionInfo info(mMass, this, mpShape);
	mpShape->calculateLocalInertia(mMass, info.m_localInertia);
	info.m_startWorldTransform = VuDynamicsUtil::toBtTransform(mLaunchTransform);
	info.m_linearDamping = mLinearDamping;
	mpRigidBody = new VuSpringBallRigidBody(info, this, COL_GAME_SPRING, COL_EVERYTHING);
	mpRigidBody->setExtendedFlags(EXT_COL_ENGINE_BREAKABLE);
	mpRigidBody->setContactCallback(this);
	mpRigidBody->mpIgnoreBody = mpLauncher->getRigidBody();
	mpRigidBody->setSurfaceType("Plastic");
	mpRigidBody->setLinearVelocity(VuDynamicsUtil::toBtVector3(mLaunchVelocity));

	VuDynamics::IF()->addRigidBody(mpRigidBody);

	// register phased ticks
	VuTickManager::IF()->registerHandler(this, &VuSpringBallEntity::tickDecision, "Decision");

	// start sfx
	VuAudioUtil::playSfx(mData["StartSfx"].asCString(), mLaunchTransform.getTrans());
}

//*****************************************************************************
void VuSpringBallEntity::onGameRelease()
{
	mp3dDrawComponent->hide();

	VuDynamics::IF()->removeRigidBody(mpRigidBody);
	delete mpRigidBody;
	delete mpShape;

	// unregister phased tick
	VuTickManager::IF()->unregisterHandler(this, "Decision");
}

//*****************************************************************************
void VuSpringBallEntity::getWorldTransform(btTransform& worldTrans) const
{
	worldTrans = VuDynamicsUtil::toBtTransform(mpTransformComponent->getWorldTransform());
}

//*****************************************************************************
void VuSpringBallEntity::setWorldTransform(const btTransform& worldTrans)
{
	mpTransformComponent->setWorldTransform(VuDynamicsUtil::toVuMatrix(worldTrans), false);
}

//*****************************************************************************
bool VuSpringBallEntity::onRigidBodyContactAdded(VuContactPoint &cp)
{
	// check if we should break
	if ( cp.mpOtherBody->hasContactResponse() && !mSpringNow )
	{
		if ( cp.mpOtherBody->getCollisionGroup() & COL_GAME_CAR )
		{
			VuEntity *pOtherEntity = cp.mpOtherBody->getEntity();
			VuCarEntity *pCar = static_cast<VuCarEntity *>(pOtherEntity);

			mBreakerEntity = pCar;
			mSpringNow = true;
		}
	}

	return !mSpringNow;
}

//*****************************************************************************
void VuSpringBallEntity::tickDecision(float fdt)
{
	bool done = false;

	mAge += fdt;
	if ( mAge > mSelfCollisionTime )
		mpRigidBody->mpIgnoreBody = VUNULL;
	if ( mAge > mLifeTime )
		done = true;

	if ( mSpringNow )
	{
		// sfx
		#if !VU_DISABLE_AUDIO
		FMOD::Event *pEvent;
		if ( VuAudio::IF()->eventSystem()->getEvent(mData["SpringSfx"].asCString(), FMOD_EVENT_NONBLOCKING, &pEvent) == FMOD_OK )
		{
			FMOD_VECTOR pos = VuAudio::toFmodVector(mpTransformComponent->getWorldPosition());
			pEvent->set3DAttributes(&pos, VUNULL, VUNULL);
			pEvent->start();
		}
		#endif

		// apply effect to car
		if ( VuCarEntity *pCar = mBreakerEntity.get() )
		{
			pCar->getEffectController()->applyEffect(mCarEffect.c_str(), mpLauncher);
		}

		done = true;
	}

	if ( done )
	{
		VuEntityRepository::IF()->removeManagedEntity(this);
	}
	else
	{
		VuMatrix mat = mpTransformComponent->getWorldTransform();
		mat.scaleLocal(mpTransformComponent->getWorldScale());
		mp3dDrawComponent->updateVisibility(mModelInstance.getAabb(), mat);
	}
}

//*****************************************************************************
void VuSpringBallEntity::draw(const VuGfxDrawParams &params)
{
	if ( params.mbDrawReflection )
		return;

	float distSquared = (mpTransformComponent->getWorldPosition() - params.mEyePos).magSquared();
	if ( distSquared > VuSquare(mDrawDistance) )
		return;

	VuMatrix mat = mpTransformComponent->getWorldTransform();
	mat.scaleLocal(mpTransformComponent->getWorldScale());
	mModelInstance.draw(mat, params);
}

//*****************************************************************************
void VuSpringBallEntity::drawShadow(const VuGfxDrawShadowParams &params)
{
	if ( params.mbDrawReflection )
		return;

	float distSquared = (mpTransformComponent->getWorldPosition() - params.mEyePos).magSquared();
	if ( distSquared > VuSquare(mDrawDistance) )
		return;

	VuMatrix mat = mpTransformComponent->getWorldTransform();
	mat.scaleLocal(mpTransformComponent->getWorldScale());
	mModelInstance.drawShadow(mat, params);
}
