//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Spring Obstacle class
// 
//*****************************************************************************

#include "VuSpringObstacleEntity.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarEffectController.h"
#include "VuPurple/Dynamics/VuCollisionTypes.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuPurple/Util/VuCollisionUtil.h"
#include "VuEngine/Entities/VuEntityRepository.h"
#include "VuEngine/Components/Transform/VuTransformComponent.h"
#include "VuEngine/Components/3dDraw/Vu3dDrawComponent.h"
#include "VuEngine/Assets/VuAnimatedModelAsset.h"
#include "VuEngine/Assets/VuAnimationAsset.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/HAL/Audio/VuAudio.h"
#include "VuEngine/Animation/VuAnimatedSkeleton.h"
#include "VuEngine/Animation/VuAnimationControl.h"
#include "VuEngine/Math/VuUnitConversion.h"


// constants
#define SELF_COLLISION_TIME 5.0f

IMPLEMENT_RTTI(VuSpringObstacleEntity, VuEntity);


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
VuSpringObstacleEntity::VuSpringObstacleEntity(const VuJsonContainer &data, VuCarEntity *pCar, const VuMatrix &transform):
	mpLauncher(pCar),
	mTransform(transform),
	mpAnimatedSkeleton(VUNULL),
	mAddedToWorld(false),
	mAge(0.0f),
	mSpringNow(false),
	mSprungTimer(0.0f),
	mAlpha(1.0f),
	mState(STATE_WHOLE)
{
	// components
	addComponent(mp3dDrawComponent = new Vu3dDrawComponent(this));

	mp3dDrawComponent->setDrawMethod(this, &VuSpringObstacleEntity::draw);
	mp3dDrawComponent->setDrawShadowMethod(this, &VuSpringObstacleEntity::drawShadow);

	// color
	mColor = pCar->getDrawColor();

	// animated model
	mAnimatedModelInstance.setModelAsset(data["Model"].asString());
	mAnimatedModelInstance.setColor(mColor);

	// animated skeleton
	mpAnimatedSkeleton = new VuAnimatedSkeleton(mAnimatedModelInstance.getSkeleton());

	// animation control
	mpAnimationControl = new VuAnimationControl(data["Animation"].asString());
	mpAnimationControl->setLooping(false);
	mpAnimationControl->setTimeFactor(0.0f);
	mpAnimatedSkeleton->addAnimationControl(mpAnimationControl);

	// collision shape
	VuCollisionUtil::createCollisionShape(data, mpShape, mpChildShape);

	// rigid body
	btRigidBody::btRigidBodyConstructionInfo info(0.0f, VUNULL, mpShape);
	info.m_startWorldTransform = VuDynamicsUtil::toBtTransform(mTransform);
	mpRigidBody = new VuSpringRigidBody(info, this, COL_GAME_SPRING, COL_GAME_CAR);
	mpRigidBody->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	mpRigidBody->setExtendedFlags(EXT_COL_ENGINE_BREAKABLE);
	mpRigidBody->setContactCallback(this);
	mpRigidBody->mpIgnoreBody = pCar->getRigidBody();

	// more params
	mDrawDistance = data["DrawDistance"].asFloat();
	mCarEffect = data["CarEffect"].asString();
	mSpringSfx = data["SpringSfx"].asString();
	mFadeStartTime = data["FadeStartTime"].asFloat();
	mFadeDuration = data["FadeDuration"].asFloat();
}

//*****************************************************************************
VuSpringObstacleEntity::~VuSpringObstacleEntity()
{
	mpAnimatedSkeleton->removeRef();
	mpAnimationControl->removeRef();
	delete mpShape;
	delete mpChildShape;
	delete mpRigidBody;
}

//*****************************************************************************
void VuSpringObstacleEntity::onGameInitialize()
{
	mpTransformComponent->setWorldTransform(mTransform);

	mp3dDrawComponent->show();

	addToWorld();

	// register phased ticks
	VuTickManager::IF()->registerHandler(this, &VuSpringObstacleEntity::tickDecision, "Decision");
	VuTickManager::IF()->registerHandler(this, &VuSpringObstacleEntity::tickBuild, "Build");
}

//*****************************************************************************
void VuSpringObstacleEntity::onGameRelease()
{
	mp3dDrawComponent->hide();

	removeFromWorld();

	// unregister phased tick
	VuTickManager::IF()->unregisterHandlers(this);
}

//*****************************************************************************
bool VuSpringObstacleEntity::onRigidBodyContactAdded(VuContactPoint &cp)
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
void VuSpringObstacleEntity::addToWorld()
{
	if ( !mAddedToWorld )
	{
		VuDynamics::IF()->addRigidBody(mpRigidBody);
		mAddedToWorld = true;
	}
}

//*****************************************************************************
void VuSpringObstacleEntity::removeFromWorld()
{
	if ( mAddedToWorld )
	{
		VuDynamics::IF()->removeRigidBody(mpRigidBody);
		mAddedToWorld = false;
	}
}

//*****************************************************************************
void VuSpringObstacleEntity::tickDecision(float fdt)
{
	mAge += fdt;
	if ( mAge > SELF_COLLISION_TIME )
		mpRigidBody->mpIgnoreBody = VUNULL;

	if (mSpringNow)
	{
		removeFromWorld();

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
		if ( VuCarEntity *pCar = mBreakerEntity.get() )
		{
			pCar->getEffectController()->applyEffect(mCarEffect.c_str(), mpLauncher);
		}

		mSpringNow = false;
		mBreakerEntity = VUNULL;

		// next state
		mState = STATE_ANIMATING;
		mpAnimationControl->setTimeFactor(1.0f);
	}
	else if (mState == STATE_ANIMATING)
	{
		mSprungTimer += fdt;
		mAlpha = 1.0f - VuLinStep(mFadeStartTime, mFadeStartTime + mFadeDuration, mSprungTimer);

		if (mAlpha <= 0.0f)
			VuEntityRepository::IF()->removeManagedEntity(this);
	}
}

//*****************************************************************************
void VuSpringObstacleEntity::tickBuild(float fdt)
{
	// update animation
	mpAnimatedSkeleton->advance(fdt);
	mpAnimatedSkeleton->build();

	mAnimatedModelInstance.setPose(mpAnimatedSkeleton);
	mAnimatedModelInstance.finalizePose();

	// update visibility
	VuAabb aabb(mAnimatedModelInstance.getLocalAabb(), mTransform);
	mp3dDrawComponent->updateVisibility(aabb);
}

//*****************************************************************************
void VuSpringObstacleEntity::draw(const VuGfxDrawParams &params)
{
	if ( params.mbDrawReflection )
		return;

	float distSquared = (mTransform.getTrans() - params.mEyePos).magSquared();
	if ( distSquared > VuSquare(mDrawDistance) )
		return;

	VuColor color(255, 255, 255);
	color.mA = (VUUINT8)VuRound(mAlpha*255.0f);

	mAnimatedModelInstance.setColor(color);
	mAnimatedModelInstance.draw(mTransform, params);
}

//*****************************************************************************
void VuSpringObstacleEntity::drawShadow(const VuGfxDrawShadowParams &params)
{
	if ( params.mbDrawReflection )
		return;

	if (mAlpha < 1.0f)
		return;

	float distSquared = (mTransform.getTrans() - params.mEyePos).magSquared();
	if ( distSquared > VuSquare(mDrawDistance) )
		return;

	mAnimatedModelInstance.drawShadow(mTransform, params);
}
