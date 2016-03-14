//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Obstacle PowerUp class
// 
//*****************************************************************************

#include "VuObstacleEntity.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarEffectController.h"
#include "VuPurple/Dynamics/VuCollisionTypes.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuPurple/Util/VuCollisionUtil.h"
#include "VuEngine/Entities/VuEntityRepository.h"
#include "VuEngine/Components/Transform/VuTransformComponent.h"
#include "VuEngine/Components/3dDraw/Vu3dDrawComponent.h"
#include "VuEngine/Assets/VuStaticModelAsset.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Pfx/VuPfx.h"
#include "VuEngine/Pfx/VuPfxManager.h"
#include "VuEngine/Pfx/VuPfxEntity.h"
#include "VuEngine/HAL/Audio/VuAudio.h"
#include "VuEngine/Math/VuUnitConversion.h"


// constants
#define SELF_COLLISION_TIME 5.0f

IMPLEMENT_RTTI(VuObstacleEntity, VuEntity);


class VuObstacleRigidBody : public VuRigidBody
{
public:
	VuObstacleRigidBody(const btRigidBody::btRigidBodyConstructionInfo &info, VuEntity *pEntity, VUINT16 collisionGroup, VUINT16 collisionMask):
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
VuObstacleEntity::VuObstacleEntity(const VuJsonContainer &data, VuCarEntity *pCar, const VuMatrix &transform):
	mData(data),
	mpLauncher(pCar),
	mTransform(transform),
	mAddedToWorld(false),
	mAge(0.0f),
	mBreakNow(false),
	mBreakTimer(0.0f),
	mCollisionVel(0,0,0),
	mState(STATE_WHOLE)
{
	// components
	addComponent(mp3dDrawComponent = new Vu3dDrawComponent(this));

	mp3dDrawComponent->setDrawMethod(this, &VuObstacleEntity::draw);
	mp3dDrawComponent->setDrawShadowMethod(this, &VuObstacleEntity::drawShadow);

	// color
	mColor = pCar->getDrawColor();

	// static model
	mStaticModelInstance.setModelAsset(data["Model"].asString());
	mStaticModelInstance.setColor(mColor);

	// breakable model
	mBreakableModelInstance.setModelAsset(data["BreakableModel"].asString());
	VuDataUtil::getValue(data["MinLinVel"], mBreakableModelInstance.mMinPieceLinVel);
	VuDataUtil::getValue(data["MaxLinVel"], mBreakableModelInstance.mMaxPieceLinVel);
	VuDataUtil::getValue(data["MinAngVel"], mBreakableModelInstance.mMinPieceAngVel);
	VuDataUtil::getValue(data["MaxAngVel"], mBreakableModelInstance.mMaxPieceAngVel);
	VuDataUtil::getValue(data["MinVelDamping"], mBreakableModelInstance.mMinVelocityDamping);
	VuDataUtil::getValue(data["MaxVelDamping"], mBreakableModelInstance.mMaxVelocityDamping);

	mBreakableModelInstance.mMinPieceAngVel = VuDegreesToRadians(mBreakableModelInstance.mMinPieceAngVel);
	mBreakableModelInstance.mMaxPieceAngVel = VuDegreesToRadians(mBreakableModelInstance.mMaxPieceAngVel);

	// collision shape
	VuCollisionUtil::createCollisionShape(data, mpShape, mpChildShape);

	// rigid body
	btRigidBody::btRigidBodyConstructionInfo info(0.0f, VUNULL, mpShape);
	info.m_startWorldTransform = VuDynamicsUtil::toBtTransform(mTransform);
	mpRigidBody = new VuObstacleRigidBody(info, this, COL_GAME_OBSTACLE, COL_ENGINE_DYNAMIC_PROP|COL_ENGINE_RAGDOLL|COL_GAME_CAR);
	mpRigidBody->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	mpRigidBody->setExtendedFlags(EXT_COL_ENGINE_BREAKABLE|EXT_COL_ENGINE_DETECT_EXPLOSIONS);
	mpRigidBody->setContactCallback(this);
	mpRigidBody->mpIgnoreBody = pCar->getRigidBody();

	// more params
	mBreakPfx = data["BreakPfx"].asString();
	mBreakSfx = data["BreakSfx"].asString();
	mFadeDelay = data["FadeDelay"].asFloat();
	mFadeTime = data["FadeTime"].asFloat();
	mDrawDistance = data["DrawDistance"].asFloat();

	// get data from prop db
	const VuJsonContainer &propData = VuGameUtil::IF()->propDB()[data["PropType"].asString()];
	mSpeedPenalty = propData["Speed Penalty"].asFloat();

	const VuJsonContainer &cameraShakeData = propData["Camera Shake"];
	mCamShakeMagnitude = cameraShakeData["Magnitude"].asFloat();
	mCamShakeDuration = cameraShakeData["Duration"].asFloat();
	mCamShakeFalloffTime = cameraShakeData["Falloff Time"].asFloat();
	mCamShakeFrequency = cameraShakeData["Frequency"].asFloat();

	// register event handlers
	REG_EVENT_HANDLER(VuObstacleEntity, OnExplosion);
	REG_EVENT_HANDLER(VuObstacleEntity, OnHitByMissile);
}

//*****************************************************************************
VuObstacleEntity::~VuObstacleEntity()
{
	delete mpShape;
	delete mpChildShape;
	delete mpRigidBody;
}

//*****************************************************************************
void VuObstacleEntity::onGameInitialize()
{
	mpTransformComponent->setWorldTransform(mTransform);

	mp3dDrawComponent->updateVisibility(mStaticModelInstance.getAabb(), mTransform);
	mp3dDrawComponent->show();

	addToWorld();

	// register phased ticks
	VuTickManager::IF()->registerHandler(this, &VuObstacleEntity::tickDecision, "Decision");
}

//*****************************************************************************
void VuObstacleEntity::onGameRelease()
{
	mp3dDrawComponent->hide();

	removeFromWorld();

	// unregister phased tick
	VuTickManager::IF()->unregisterHandlers(this);
}

//*****************************************************************************
void VuObstacleEntity::OnExplosion(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	VuVector3 explosionPos = accessor.getVector3();
	float dist = accessor.getFloat();
	const VuJsonContainer &data = *static_cast<const VuJsonContainer *>(accessor.getPointer());
	VuEntity *pOriginator = accessor.getEntity();

	if ( !mBreakNow && dist < data["BreakableDist"].asFloat() )
	{
		mBreakNow = true;

		mCollisionVel = mpTransformComponent->getWorldPosition() - explosionPos;
		mCollisionVel.normalize();
		mCollisionVel *= VuMphToMetersPerSecond(data["BreakableSpeed"].asFloat());
	}
}

//*****************************************************************************
void VuObstacleEntity::OnHitByMissile(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	VuVector3 missilePos = accessor.getVector3();
	VuVector3 missileVel = accessor.getVector3();
	const char *carEffect = accessor.getString();
	VuEntity *pOriginator = accessor.getEntity();

	if ( !mBreakNow )
	{
		mBreakNow = true;
		mCollisionVel = missileVel;
	}
}

//*****************************************************************************
bool VuObstacleEntity::onRigidBodyContactAdded(VuContactPoint &cp)
{
	// check if we should break
	if ( cp.mpOtherBody->hasContactResponse() && !mBreakNow )
	{
		VuEntity *pOtherEntity = cp.mpOtherBody->getEntity();
		VuCarEntity *pCar = (pOtherEntity && pOtherEntity->isDerivedFrom(VuCarEntity::msRTTI)) ? static_cast<VuCarEntity *>(pOtherEntity) : VUNULL;

		mCollisionVel = cp.mpOtherBody->getVuLinearVelocity();
		mBreakerEntity = pOtherEntity;
		mBreakNow = true;

		if ( pCar )
		{
			float penalty = mSpeedPenalty*pCar->getBreakablePenalty();
			VuVector3 linVel = pCar->getRigidBody()->getVuLinearVelocity();
			linVel *= (1.0f - penalty);
			pCar->getRigidBody()->setVuLinearVelocity(linVel);
		}
	}

	return false;
}

//*****************************************************************************
void VuObstacleEntity::addToWorld()
{
	if ( !mAddedToWorld )
	{
		VuDynamics::IF()->addRigidBody(mpRigidBody);
		mAddedToWorld = true;
	}
}

//*****************************************************************************
void VuObstacleEntity::removeFromWorld()
{
	if ( mAddedToWorld )
	{
		VuDynamics::IF()->removeRigidBody(mpRigidBody);
		mAddedToWorld = false;
	}
}

//*****************************************************************************
void VuObstacleEntity::tickDecision(float fdt)
{
	mAge += fdt;
	if ( mAge > SELF_COLLISION_TIME )
		mpRigidBody->mpIgnoreBody = VUNULL;

	if ( mBreakNow )
	{
		removeFromWorld();

		mBreakableModelInstance.initializePieces(mTransform, mCollisionVel);
		mBreakTimer = mFadeDelay + mFadeTime;
		mState = STATE_BREAKING;

		if ( mBreakerEntity )
		{
			// camera shake
			{
				VuParams params;
				params.addFloat(mCamShakeMagnitude);
				params.addFloat(mCamShakeDuration);
				params.addFloat(mCamShakeFalloffTime);
				params.addFloat(mCamShakeFrequency);
				mBreakerEntity->handleEvent("OnCameraShake", params);
			}
		}

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

		if ( mBreakerEntity.get() && mBreakerEntity.get()->isDerivedFrom(VuCarEntity::msRTTI) )
		{
			VuCarEntity *pCar = static_cast<VuCarEntity *>(mBreakerEntity.get());
			const VuJsonContainer &carEffects = mData["CarEffects"];
			int carEffectIndex = VuRand::global().range(0, carEffects.size());
			pCar->getEffectController()->applyEffect(carEffects[carEffectIndex].asCString(), mpLauncher);
		}

		mBreakNow = false;
		mBreakerEntity = VUNULL;
	}

	if ( mState == STATE_BREAKING )
	{
		mBreakTimer -= fdt;
		if ( mBreakTimer < 0 )
		{
			mp3dDrawComponent->hide();
			mBreakTimer = 0.0f;
			mState = STATE_DONE;
			VuEntityRepository::IF()->removeManagedEntity(this);
		}
		else
		{
			VuAabb aabb;
			mBreakableModelInstance.updatePieces(fdt, aabb);
			if ( aabb.isValid() )
				mp3dDrawComponent->updateVisibility(aabb);
		}
	}
}

//*****************************************************************************
void VuObstacleEntity::draw(const VuGfxDrawParams &params)
{
	if ( params.mbDrawReflection )
		return;

	float distSquared = (mTransform.getTrans() - params.mEyePos).magSquared();
	if ( distSquared > VuSquare(mDrawDistance) )
		return;

	if ( mState == STATE_WHOLE )
	{
		mStaticModelInstance.draw(mTransform, params);
	}
	else if ( mState == STATE_BREAKING )
	{
		float fAlpha = VuMin(mBreakTimer/mFadeTime, 1.0f);
		mColor.mA = (VUUINT8)VuRound(fAlpha*255.0f);

		mBreakableModelInstance.setColor(mColor);
		mBreakableModelInstance.drawPieces(params);
	}
}

//*****************************************************************************
void VuObstacleEntity::drawShadow(const VuGfxDrawShadowParams &params)
{
	if ( params.mbDrawReflection )
		return;

	float distSquared = (mTransform.getTrans() - params.mEyePos).magSquared();
	if ( distSquared > VuSquare(mDrawDistance) )
		return;

	mStaticModelInstance.drawShadow(mTransform, params);
}
