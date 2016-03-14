//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Missile PowerUp class
// 
//*****************************************************************************

#include "VuHomingMissileEntity.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarSuspension.h"
#include "VuPurple/PowerUps/VuPowerUpUtil.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Dynamics/VuCollisionTypes.h"
#include "VuEngine/Entities/VuEntityRepository.h"
#include "VuEngine/Dynamics/Util/VuDynamicsRayTest.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Managers/VuExplosionManager.h"
#include "VuEngine/Pfx/VuPfxManager.h"
#include "VuEngine/Pfx/VuPfxEntity.h"
#include "VuEngine/Pfx/VuPfx.h"
#include "VuEngine/Math/VuMathUtil.h"
#include "VuEngine/Math/VuUnitConversion.h"
#include "VuEngine/Util/VuAudioUtil.h"


IMPLEMENT_RTTI(VuHomingMissileEntity, VuEntity);


class VuMissileFlightRayTestResult : public VuDynamicsRayTest::VuClosestResult
{
public:
	virtual bool needsCollision(VuRigidBody *pRigidBody)
	{
		if ( pRigidBody->getCollisionGroup() & (COL_ENGINE_STATIC_PROP|COL_ENGINE_DYNAMIC_PROP|COL_GAME_OBSTACLE) )
			return true;

		return false;
	}
};

class VuMissileGroundRayTestResult : public VuDynamicsRayTest::VuClosestResult
{
public:
	virtual bool needsCollision(VuRigidBody *pRigidBody)
	{
		if ( pRigidBody->getCollisionGroup() & COL_ENGINE_STATIC_PROP )
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
VuHomingMissileEntity::VuHomingMissileEntity(const VuJsonContainer &data, VuCarEntity *pLauncher, VuCarEntity *pTarget, const VuMatrix &transform):
	mData(data),
	mpLauncher(pLauncher),
	mpTarget(pTarget),
	mTestCars(false),
	mTransform(transform),
	mDistance(0.0f),
	mCollidedHash(0),
	mFlightPfx(VUNULL)
{
	// params
	mRange = data["Range"].asFloat();
	mPreHomingRange = data["PreHomingRange"].asFloat();
	mCosTargetAcquisitionCone = VuCos(0.5f*VuDegreesToRadians(data["TargetAcquisitionCone"].asFloat()));
	mAltitute = data["Altitute"].asFloat();
	mSpeed = VuMphToMetersPerSecond(data["Speed"].asFloat());
	mGravity = 9.806f*data["Gravity"].asFloat();
	mGroundSnapFactor = data["GroundSnapFactor"].asFloat();
	mCarCollisionRadius = data["CarCollisionRadius"].asFloat();

	mLaunchVelocity = mSpeed*mTransform.getAxisY();
	mLinearVelocity = mLaunchVelocity;

	mLastPosition = mTransform.getTrans();
}

//*****************************************************************************
void VuHomingMissileEntity::onGameInitialize()
{
	VuTickManager::IF()->registerHandler(this, &VuHomingMissileEntity::tickDecision, "Decision");

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
}

//*****************************************************************************
void VuHomingMissileEntity::onGameRelease()
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
}

//*****************************************************************************
void VuHomingMissileEntity::tickDecision(float fdt)
{
	VuVector3 curPos = mpTransformComponent->getWorldPosition();

	// check if missile has hit any cars
	if ( VuDistSquared(mLastPosition, curPos) > FLT_MIN )
	{
		if ( !mTestCars )
		{
			// don't test against other cars until missile has traveled outside our collision radius
			VuAabb collisionAabb = mpLauncher->getCollisionAabb();
			collisionAabb.mMin -= VuVector3(mCarCollisionRadius);
			collisionAabb.mMax += VuVector3(mCarCollisionRadius);

			if ( !VuMathUtil::testIntersectionLineSegBox(mLastPosition, curPos, collisionAabb, mpLauncher->getModelMatrix()) )
				mTestCars = true;
		}

		if ( mTestCars )
		{
			for ( int i = 0; i < VuCarManager::IF()->getCarCount(); i++ )
			{
				VuCarEntity *pOtherCar = VuCarManager::IF()->getCar(i);
				if ( pOtherCar != mpLauncher )
				{
					if ( !pOtherCar->isGhostly() )
					{
						VuAabb collisionAabb = pOtherCar->getCollisionAabb();
						collisionAabb.mMin -= VuVector3(mCarCollisionRadius);
						collisionAabb.mMax += VuVector3(mCarCollisionRadius);

						if ( VuMathUtil::testIntersectionLineSegBox(mLastPosition, curPos, collisionAabb, pOtherCar->getModelMatrix()) )
						{
							VuVector3 impactPoint = pOtherCar->getModelMatrix().transform(collisionAabb.getCenter());
							mTransform.setTrans(impactPoint);
							mCollidedHash = pOtherCar->getHashedLongNameFast();
							break;
						}
					}
				}
			}
		}
	}

	if ( mCollidedHash )
	{
		if ( VuEntity *pEntity = VuEntityRepository::IF()->findEntity(mCollidedHash) )
		{
			VuParams params;
			params.addVector3(mTransform.getTrans());
			params.addVector3(mLinearVelocity);
			params.addString(mData["CarEffect"].asCString());
			params.addEntity(mpLauncher);
			pEntity->handleEvent("OnHitByMissile", params);
		}

		explode();
	}
	else
	{
		// update distance
		mDistance += (curPos - mLastPosition).mag();
		if ( mDistance >= mRange )
			explode();
	}

	if ( mFlightPfx )
	{
		VuPfxEntity *pPfxEntity = VuPfxManager::IF()->getEntity(mFlightPfx);
		pPfxEntity->getSystemInstance()->setMatrix(mpTransformComponent->getWorldTransform());
	}

	if ( mStartSfx.active() )
		mStartSfx.set3DAttributes(&mpTransformComponent->getWorldPosition(), &mLinearVelocity, VUNULL);

	if ( mFlightSfx.active() )
		mFlightSfx.set3DAttributes(&mpTransformComponent->getWorldPosition(), &mLinearVelocity, VUNULL);

	// refresh target
	if ( mpTarget == VUNULL )
	{
		if ( mTestCars )
		{
			float bestDist = mRange;
			for ( int i = 0; i < VuCarManager::IF()->getCarCount(); i++ )
			{
				VuCarEntity *pOtherCar = VuCarManager::IF()->getCar(i);
				if ( pOtherCar != mpLauncher )
				{
					if ( !pOtherCar->isGhostly() && !pOtherCar->hasCrashed() )
					{
						// check distance
						VuVector3 otherPos = pOtherCar->getModelMatrix().transform(pOtherCar->getCollisionAabb().getCenter());
						VuVector3 delta = otherPos - mpTransformComponent->getWorldPosition();
						float dist = delta.normalize();
						if ( dist < bestDist )
						{
							// check target acquisition cone
							if ( VuDot(delta, mpTransformComponent->getWorldTransform().getAxisY()) > mCosTargetAcquisitionCone )
							{
								mpTarget = pOtherCar;
								bestDist = dist;
							}
						}
					}
				}
			}
		}
	}
	else
	{
		if ( mpTarget->isGhostly() )
			mpTarget = VUNULL;
	}

	mLastPosition = curPos;
}

//*****************************************************************************
void VuHomingMissileEntity::onDynamicsApplyForces(float fdt)
{
	// flight collision
	if ( !mCollidedHash )
	{
		VuVector3 prevPos = mTransform.getTrans();
		VuVector3 nextPos = prevPos;

		if ( mpTarget )
		{
			mLinearVelocity = mSpeed*(mpTarget->getRigidBody()->getVuCenterOfMassPosition() - prevPos).normal();
			if ( mDistance < mPreHomingRange )
				mLinearVelocity = VuLerp(mLaunchVelocity, mLinearVelocity, mDistance/mPreHomingRange);
			nextPos = prevPos + mLinearVelocity*fdt;
		}
		else
		{
			VuVector3 accel(0.0f, 0.0f, -mGravity);
			nextPos = prevPos + mLinearVelocity*fdt + (0.5f*fdt*fdt)*accel;
			mLinearVelocity += accel*fdt;
		}

		// check ground height
		{
			float rayLength = 1.0f*mAltitute;
			VuMissileGroundRayTestResult rayTestResult;
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
			VuMissileFlightRayTestResult rayTestResult;
			VuDynamicsRayTest::test(prevPos, nextPos, rayTestResult);

			if ( rayTestResult.mbHasHit )
			{
				const VuRigidBody *pRigidBody = rayTestResult.mpRigidBody;
				if ( VuEntity *pEntity = pRigidBody->getEntity() )
				{
					mCollidedHash = pEntity->getHashedLongNameFast();
				}

				mTransform.setTrans(VuLerp(prevPos, nextPos, rayTestResult.mHitFraction));
			}
		}

		VuMathUtil::buildOrientationMatrix(mLinearVelocity, mTransform.getAxisZ(), mTransform);
		mTransform.setTrans(nextPos);
	}
}

//*****************************************************************************
void VuHomingMissileEntity::onDynamicsAdvanceEnvironment(float fdt, bool bSimStep)
{
	if ( !bSimStep )
	{
		if ( mCollidedHash )
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
void VuHomingMissileEntity::explode()
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
