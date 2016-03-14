//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Missile PowerUp class
// 
//*****************************************************************************

#include "VuEarthStrikeEntity.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarSuspension.h"
#include "VuPurple/Entities/Car/Parts/VuCarEffectController.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Managers/VuAchievementManager.h"
#include "VuEngine/Dynamics/Util/VuDynamicsRayTest.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Managers/VuExplosionManager.h"
#include "VuEngine/Managers/VuTireTrackManager.h"
#include "VuEngine/Entities/VuEntityRepository.h"
#include "VuEngine/Pfx/VuPfxManager.h"
#include "VuEngine/Pfx/VuPfxEntity.h"
#include "VuEngine/Pfx/VuPfx.h"
#include "VuEngine/Math/VuMathUtil.h"
#include "VuEngine/Math/VuUnitConversion.h"
#include "VuEngine/Util/VuAudioUtil.h"
#include "VuEngine/Util/VuDataUtil.h"


IMPLEMENT_RTTI(VuEarthStrikeEntity, VuEntity);


class VuFlightRayTestResult : public VuDynamicsRayTest::VuClosestResult
{
public:
	virtual bool needsCollision(VuRigidBody *pRigidBody)
	{
		if ( (pRigidBody->getCollisionGroup() & COL_ENGINE_STATIC_PROP) && !(pRigidBody->getExtendedFlags() & EXT_COL_ENGINE_BREAKABLE) )
			return true;

		return false;
	}
};

class VuGroundRayTestResult : public VuDynamicsRayTest::VuClosestResult
{
public:
	virtual bool needsCollision(VuRigidBody *pRigidBody)
	{
		if ( (pRigidBody->getCollisionGroup() & COL_ENGINE_STATIC_PROP) && !(pRigidBody->getExtendedFlags() & EXT_COL_ENGINE_BREAKABLE) )
			return true;

		return false;
	}
	virtual bool addResult(const VuRigidBody *pRigidBody, float hitFraction, int triangleIndex, const VuVector3 &normal)
	{
		if ( (hitFraction <= mHitFraction) && (normal.mZ > 0.707107f) )
		{
			mpRigidBody = pRigidBody;
			mHitFraction = hitFraction;
			mTriangleIndex = triangleIndex;
			mHitNormal = normal;
			return true;
		}
		return false;
	}
};

//*****************************************************************************
class VuSmashConvexResult : public btCollisionWorld::ConvexResultCallback
{
public:
	typedef VuArray<VuVector3> Verts;

	VuSmashConvexResult() : mNumVictims(0) {}

	virtual bool needsCollision(btBroadphaseProxy *proxy0) const
	{
		const btCollisionObject *pColObj = static_cast<const btCollisionObject *>(proxy0->m_clientObject);
		if ( pColObj->getInternalType() != btCollisionObject::CO_RIGID_BODY )
			return false;

		const VuRigidBody *pRigidBody = static_cast<const VuRigidBody *>(pColObj);
		if ( pRigidBody->getCollisionGroup() & COL_ENGINE_DYNAMIC_PROP )
			return true;

		if ( (pRigidBody->getCollisionGroup() & COL_ENGINE_STATIC_PROP) && (pRigidBody->getExtendedFlags() & EXT_COL_ENGINE_BREAKABLE) )
			return true;

		return false;
	}

	virtual	btScalar addSingleResult(btCollisionWorld::LocalConvexResult &convexResult, bool normalInWorldSpace)
	{
		const VuRigidBody *pRigidBody = static_cast<const VuRigidBody *>(convexResult.m_hitCollisionObject);

		if ( mNumVictims < MAX_VICTIMS )
		{
			if ( VuEntity *pEntity = pRigidBody->getEntity() )
			{
				for ( int i = 0; i < mNumVictims; i++ )
					if ( mpVictims[i] == pEntity )
						return 1.0f;

				mpVictims[mNumVictims] = pEntity;
				mNumVictims++;
			}
		}

		return 1.0f;
	}

	enum { MAX_VICTIMS = 16 };
	int			mNumVictims;
	VuEntity	*mpVictims[MAX_VICTIMS];
};

class VuSmashRayTestResult : public VuDynamicsRayTest::VuClosestResult
{
public:
	virtual bool needsCollision(VuRigidBody *pRigidBody)
	{
		if ( pRigidBody->getCollisionGroup() & COL_ENGINE_DYNAMIC_PROP )
			return true;

		if ( (pRigidBody->getCollisionGroup() & COL_ENGINE_STATIC_PROP) && (pRigidBody->getExtendedFlags() & EXT_COL_ENGINE_BREAKABLE) )
			return true;

		return false;
	}
};


//*****************************************************************************
VuEarthStrikeEntity::VuEarthStrikeEntity(const VuJsonContainer &data, VuCarEntity *pCar, const VuMatrix &transform):
	mData(data),
	mpLauncher(pCar),
	mTransform(transform),
	mDistance(0.0f),
	mWallCollision(false),
	mFlightPfx(VUNULL),
	mCollisionRadius(1.0f)
{
	// params
	mRange = data["Range"].asFloat();
	mCosTargetAcquisitionCone = VuCos(0.5f*VuDegreesToRadians(data["TargetAcquisitionCone"].asFloat()));
	mAltitute = data["Altitute"].asFloat();
	mSpeed = VuMphToMetersPerSecond(data["Speed"].asFloat());
	mGravity = 9.806f*data["Gravity"].asFloat();
	mGroundSnapFactor = data["GroundSnapFactor"].asFloat();
	VuDataUtil::getValue(data["CollisionRadius"], mCollisionRadius);
	mCarEffect = data["CarEffect"].asString();

	mLinearVelocity = mSpeed*mTransform.getAxisY();

	mLastPosition = mTransform.getTrans();
}

//*****************************************************************************
void VuEarthStrikeEntity::onGameInitialize()
{
	VuTickManager::IF()->registerHandler(this, &VuEarthStrikeEntity::tickDecision, "Decision");

	// register dynamics methods
	VuDynamics::IF()->registerStepCallback(this);

	mpTransformComponent->setWorldTransform(mTransform);

	if ( (mFlightPfx = VuPfxManager::IF()->createEntity(mData["LoopingPfx"].asCString(), false)) )
	{
		VuPfxEntity *pPfxEntity = VuPfxManager::IF()->getEntity(mFlightPfx);
		pPfxEntity->getSystemInstance()->setMatrix(mTransform);
		pPfxEntity->getSystemInstance()->start();
	}

	if ( mStartSfx.create(mData["StartSfx"].asCString()) )
	{
		mStartSfx.set3DAttributes(&mTransform.getTrans(), &mLinearVelocity, VUNULL);
		mStartSfx.start();
	}

	if ( mFlightSfx.create(mData["LoopingSfx"].asCString()) )
	{
		mFlightSfx.set3DAttributes(&mTransform.getTrans(), &mLinearVelocity, VUNULL);
		mFlightSfx.start();
	}

	VuTireTrackParams tireTrackParams;
	mData["TireTrackType"].getValue(tireTrackParams.mType);
	mData["TireTrackWidth"].getValue(tireTrackParams.mWidth);
	mData["TireTrackFadeTime"].getValue(tireTrackParams.mFadeTime);
	mData["TireTrackScaleV"].getValue(tireTrackParams.mScaleV);
	mpTireTrack = VuTireTrackManager::IF()->createTireTrack(tireTrackParams);
}

//*****************************************************************************
void VuEarthStrikeEntity::onGameRelease()
{
	VuTickManager::IF()->unregisterHandlers(this);

	VuDynamics::IF()->unregisterStepCallback(this);

	if ( mFlightPfx )
	{
		VuPfxManager::IF()->releaseEntity(mFlightPfx, true);
		mFlightPfx = VUNULL;
	}

	mStartSfx.release(VuAudioEvent::STOP_IMMEDIATE);
	mFlightSfx.release(VuAudioEvent::STOP_IMMEDIATE);

	VuTireTrackManager::IF()->releaseTireTrack(mpTireTrack);
}

//*****************************************************************************
void VuEarthStrikeEntity::tickDecision(float fdt)
{
	VuVector3 curPos = mpTransformComponent->getWorldPosition();

	if ( VuDistSquared(mLastPosition, curPos) > FLT_MIN )
	{
		// check if missile has hit any cars
		for ( int i = 0; i < VuCarManager::IF()->getCarCount(); i++ )
		{
			VuCarEntity *pOtherCar = VuCarManager::IF()->getCar(i);
			if ( pOtherCar != mpLauncher )
			{
				if ( !pOtherCar->isGhostly() )
				{
					VuAabb collisionAabb = pOtherCar->getCollisionAabb();
					collisionAabb.mMin -= VuVector3(mCollisionRadius);
					collisionAabb.mMax += VuVector3(mCollisionRadius);

					if ( VuMathUtil::testIntersectionLineSegBox(mLastPosition, curPos, collisionAabb, pOtherCar->getModelMatrix()) )
					{
						pOtherCar->getEffectController()->applyEffect(mCarEffect.c_str(), mpLauncher);

						if ( !pOtherCar->getEffectController()->isActive(VuCarEffectController::SHIELD) )
						{
							if ( mpLauncher->getDriver()->isHuman() && pOtherCar->getEffectController()->isActive(VuCarEffectController::LOW_GRAVITY) )
								VuAchievementManager::IF()->unlock(39); // Use Earth Strike on an opponent who's experiencing Low Gravity
						}
					}
				}
			}
		}

		// check if missile has smashed anything
		VuSmashConvexResult result;

		btTransform from;
		from.setIdentity();
		from.setOrigin(VuDynamicsUtil::toBtVector3(mLastPosition));

		btTransform to;
		to.setIdentity();
		to.setOrigin(VuDynamicsUtil::toBtVector3(curPos));

		btSphereShape sphereShape(mCollisionRadius);
		VuDynamics::IF()->getDynamicsWorld()->convexSweepTest(&sphereShape, from, to, result);
		for ( int i = 0; i < result.mNumVictims; i++ )
		{
			VuParams params;
			params.addVector3(mTransform.getTrans());
			params.addVector3(mLinearVelocity);
			params.addString(mData["CarEffect"].asCString());
			params.addEntity(mpLauncher);
			result.mpVictims[i]->handleEvent("OnHitByMissile", params);
		}
	}

	// wall collision?
	if ( mWallCollision )
	{
		explode();
	}
	else
	{
		// update distance
		mDistance += (curPos - mLastPosition).mag();
		if ( mDistance >= mRange )
			explode();
	}

	// update flight pfx & tire track
	{
		float rayLength = 2.0f*mAltitute;
		VuVector3 from = mpTransformComponent->getWorldPosition();
		VuVector3 to = from + VuVector3(0.0f, 0.0f, -rayLength);
		VuGroundRayTestResult rayTestResult;
		VuDynamicsRayTest::test(from, to, rayTestResult);

		if ( rayTestResult.mbHasHit )
		{
			VuVector3 contact = VuLerp(from, to, rayTestResult.mHitFraction);

			VuMatrix pfxTransform = mpTransformComponent->getWorldTransform();
			pfxTransform.setTrans(contact);
			if ( mFlightPfx )
			{
				VuPfxEntity *pPfxEntity = VuPfxManager::IF()->getEntity(mFlightPfx);
				pPfxEntity->getSystemInstance()->setMatrix(pfxTransform);
			}
			VuTireTrackManager::IF()->updateTireTrack(mpTireTrack, true, false, 0, contact, rayTestResult.mHitNormal, mLinearVelocity, 1.0f, 1.0f);
		}
		else
		{
			if ( mFlightPfx )
			{
				VuPfxEntity *pPfxEntity = VuPfxManager::IF()->getEntity(mFlightPfx);
				pPfxEntity->getSystemInstance()->setMatrix(mpTransformComponent->getWorldTransform());
			}
			VuTireTrackManager::IF()->updateTireTrack(mpTireTrack, false, false, 0, mpTransformComponent->getWorldPosition(), VuVector3(0,0,1), mLinearVelocity, 1.0f, 1.0f);
		}
	}

	if ( mStartSfx.active() )
		mStartSfx.set3DAttributes(&mpTransformComponent->getWorldPosition(), &mLinearVelocity, VUNULL);

	if ( mFlightSfx.active() )
		mFlightSfx.set3DAttributes(&mpTransformComponent->getWorldPosition(), &mLinearVelocity, VUNULL);

	mLastPosition = curPos;
}

//*****************************************************************************
void VuEarthStrikeEntity::onDynamicsApplyForces(float fdt)
{
	// flight collision
	if ( !mWallCollision )
	{
		VuVector3 prevPos = mTransform.getTrans();
		VuVector3 nextPos = prevPos;

		VuVector3 accel(0.0f, 0.0f, -mGravity);
		nextPos = prevPos + mLinearVelocity*fdt + (0.5f*fdt*fdt)*accel;
		mLinearVelocity += accel*fdt;

		// check ground height
		{
			float rayLength = 1.0f*mAltitute;
			VuGroundRayTestResult rayTestResult;
			VuDynamicsRayTest::test(nextPos, nextPos + VuVector3(0.0f, 0.0f, -rayLength), rayTestResult);

			if ( rayTestResult.mbHasHit && (VuDot(mLinearVelocity, rayTestResult.mHitNormal) < 0) )
			{
				float height = rayTestResult.mHitFraction*rayLength;

				float delta = mAltitute - height;
				nextPos.mZ += mGroundSnapFactor*delta;

				mLinearVelocity.mZ -= VuDot(mLinearVelocity, rayTestResult.mHitNormal)*rayTestResult.mHitNormal.mZ;
			}
		}

		// adjust velocity
		mLinearVelocity.normalize();
		VuVector3 flightDirection = mLinearVelocity;
		mLinearVelocity *= mSpeed;
		nextPos = prevPos + mLinearVelocity*fdt;
		
		// flight ray test
		{
			VuFlightRayTestResult rayTestResult;
			VuDynamicsRayTest::test(prevPos, nextPos, rayTestResult);

			if ( rayTestResult.mbHasHit )
			{
				mWallCollision = true;
				mTransform.setTrans(VuLerp(prevPos, nextPos, rayTestResult.mHitFraction));
			}
		}

		VuMathUtil::buildOrientationMatrix(mLinearVelocity, mTransform.getAxisZ(), mTransform);
		mTransform.setTrans(nextPos);
	}
}

//*****************************************************************************
void VuEarthStrikeEntity::onDynamicsAdvanceEnvironment(float fdt, bool bSimStep)
{
	if ( !bSimStep )
	{
		if ( mWallCollision )
		{
			mpTransformComponent->setWorldTransform(mTransform, false);
		}
		else
		{
			float localTime = VuDynamics::IF()->getLocalTime();

			// interpolate transform
			VuMatrix interpTransform = mTransform;
			interpTransform.setTrans(interpTransform.getTrans() + mLinearVelocity*localTime);
			mpTransformComponent->setWorldTransform(interpTransform, false);
		}
	}
}

//*****************************************************************************
void VuEarthStrikeEntity::explode()
{
	VuVector3 explosionPos = mTransform.getTrans();

	// pfx
	if ( VUUINT32 hPfx = VuPfxManager::IF()->createEntity(mData["EndPfx"].asCString(), true) )
	{
		VuPfxEntity *pPfxEntity = VuPfxManager::IF()->getEntity(hPfx);
		pPfxEntity->getSystemInstance()->setMatrix(mTransform);
		pPfxEntity->getSystemInstance()->start();
	}

	// sfx
	VuAudioUtil::playSfx(mData["EndSfx"].asCString(), explosionPos);

	// explosion
	VuExplosionManager::IF()->createExplosion(explosionPos, mData["Explosion"].asCString(), mpLauncher);

	VuEntityRepository::IF()->removeManagedEntity(this);
}
