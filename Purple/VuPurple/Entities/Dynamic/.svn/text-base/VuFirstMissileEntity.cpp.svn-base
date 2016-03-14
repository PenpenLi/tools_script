//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Missile PowerUp class
// 
//*****************************************************************************

#include "VuFirstMissileEntity.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarSuspension.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/Entities/Car/Camera/VuCarCamera.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Managers/VuAchievementManager.h"
#include "VuPurple/Dynamics/VuCollisionTypes.h"
#include "VuPurple/Track/VuTrackSector.h"
#include "VuEngine/Dynamics/Util/VuDynamicsRayTest.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Managers/VuExplosionManager.h"
#include "VuEngine/Entities/VuEntityRepository.h"
#include "VuEngine/Pfx/VuPfxManager.h"
#include "VuEngine/Pfx/VuPfxEntity.h"
#include "VuEngine/Pfx/VuPfx.h"
#include "VuEngine/Math/VuMathUtil.h"
#include "VuEngine/Math/VuUnitConversion.h"
#include "VuEngine/Util/VuAudioUtil.h"


IMPLEMENT_RTTI(VuFirstMissileEntity, VuEntity);


class VuFirstMissileFlightRayTestResult : public VuDynamicsRayTest::VuClosestResult
{
public:
	virtual bool needsCollision(VuRigidBody *pRigidBody)
	{
		if ( pRigidBody->getCollisionGroup() & COL_ENGINE_DYNAMIC_PROP )
			return true;

		if ( (pRigidBody->getCollisionGroup() & COL_ENGINE_STATIC_PROP) && (pRigidBody->getExtendedFlags() & EXT_COL_ENGINE_BREAKABLE) )
			return true;

		if ( pRigidBody->getCollisionGroup() & COL_GAME_OBSTACLE )
			return true;

		return false;
	}
};

class VuFirstMissileTargetRayTestResult : public VuDynamicsRayTest::VuClosestResult
{
public:
	virtual bool needsCollision(VuRigidBody *pRigidBody)
	{
		if ( (pRigidBody->getCollisionGroup() & COL_ENGINE_STATIC_PROP) && !(pRigidBody->getExtendedFlags() & EXT_COL_ENGINE_BREAKABLE) )
			return true;

		return false;
	}
};


//*****************************************************************************
VuFirstMissileEntity::VuFirstMissileEntity(const VuJsonContainer &data, VuCarEntity *pCar):
	mData(data),
	mpLauncher(pCar),
	mTestCars(false),
	mpTarget(VUNULL),
	mCarVictimCount(0)
{
	// params
	mHomingRange = data["HomingRange"].asFloat();
	mCosTargetAcquisitionCone = VuCos(0.5f*VuDegreesToRadians(data["TargetAcquisitionCone"].asFloat()));
	mAltitute = data["Altitute"].asFloat();
	mSpeed = VuMphToMetersPerSecond(data["Speed"].asFloat());
	mCarCollisionRadius = data["CarCollisionRadius"].asFloat();

	const VuMatrix &carTransform = pCar->getModelMatrix();

	VuVector3 pos = carTransform.transform(pCar->getCollisionAabb().getCenter());

	mTransform = carTransform;
	if ( pCar->getSuspension()->getWheelContactCount() )
	{
		VuVector3 fwd = VuCross(pCar->getSuspension()->getAvgContactNormal(), carTransform.getAxisX());
		VuMathUtil::buildOrientationMatrix(fwd, carTransform.getAxisZ(), mTransform);
	}

	mLinearVelocity = mSpeed*mTransform.getAxisY();
	mTransform.setTrans(pos);

	mpCurSector = pCar->getDriver()->getCurrentSector();
}

//*****************************************************************************
void VuFirstMissileEntity::onGameInitialize()
{
	VuTickManager::IF()->registerHandler(this, &VuFirstMissileEntity::tickDecision, "Decision");

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
void VuFirstMissileEntity::onGameRelease()
{
	VuTickManager::IF()->unregisterHandlers(this);

	if ( mFlightPfx )
	{
		VuPfxManager::IF()->releaseEntity(mFlightPfx, true);
		mFlightPfx = VUNULL;
	}

	mStartSfx.release(VuAudioEvent::STOP_IMMEDIATE);
	mFlightSfx.release(VuAudioEvent::STOP_IMMEDIATE);
}

//*****************************************************************************
void VuFirstMissileEntity::tickDecision(float fdt)
{
	// look for target
	if ( mpTarget == VUNULL && mTestCars )
	{
		VuCarEntity *pTarget = VUNULL;

		// look for highest-placed car that's still racing and is ahead of launcher
		const VuCarStats &launcherStats = mpLauncher->getStats();
		int bestPlace = INT_MAX;
		for ( int i = 0; i < VuCarManager::IF()->getCarCount(); i++ )
		{
			VuCarEntity *pOtherCar = VuCarManager::IF()->getCar(i);
			const VuCarStats &otherStats = pOtherCar->getStats();
			if ( (otherStats.mPlace < launcherStats.mPlace) && (otherStats.mPlace < bestPlace) && !otherStats.mHasFinished )
			{
				pTarget = pOtherCar;
				bestPlace = otherStats.mPlace;
			}
		}

		if ( pTarget )
		{
			// check distance
			VuVector3 otherPos = pTarget->getModelMatrix().transform(pTarget->getCollisionAabb().getCenter());
			VuVector3 delta = otherPos - mTransform.getTrans();
			float distSquared = delta.magSquared();
			if ( distSquared < VuSquare(mHomingRange) )
			{
				// check target acquisition cone
				if ( VuDot(delta, mTransform.getAxisY()) > mCosTargetAcquisitionCone )
				{
					// check collision
					VuFirstMissileTargetRayTestResult rayTestResult;
					VuDynamicsRayTest::test(mTransform.getTrans(), otherPos, rayTestResult);

					if ( !rayTestResult.mbHasHit )
						mpTarget = pTarget;
				}
			}
		}
	}

	VuVector3 targetPos;
	if ( mpTarget )
	{
		// follow bread crumbs
		targetPos = mpTarget->getModelMatrix().transform(mpTarget->getCollisionAabb().getCenter());
	}
	else
	{
		// follow AI path
		if ( mpCurSector )
		{
			float traversalAmount = mpCurSector->traversalAmount(mTransform.getTrans());
			if ( traversalAmount > 1.0f )
				mpCurSector = mpCurSector->mpNextSectors[0];
		}

		if ( mpCurSector )
		{
			mpCurSector->closestPointAlongRacingLine(mTransform.getTrans(), targetPos);
			float distFromLine = VuDist(mTransform.getTrans(), targetPos);
			targetPos.mZ += mAltitute;
			targetPos += (4.0f*distFromLine + 1.0f)*mpCurSector->mUnitDir;
		}
		else
		{
			targetPos = mTransform.getTrans() + mTransform.getAxisY();
		}
	}
	
	VuVector3 prevPos = mTransform.getTrans();
	mLinearVelocity = mSpeed*(targetPos - prevPos).normal();
	VuVector3 curPos = mTransform.getTrans() + mLinearVelocity*fdt;

	VuMathUtil::buildOrientationMatrix(mLinearVelocity, mTransform.getAxisZ(), mTransform);
	mTransform.setTrans(curPos);

	mpTransformComponent->setWorldTransform(mTransform);

	// check if missile has hit other cars
	if ( VuDistSquared(prevPos, curPos) > FLT_MIN )
	{
		if ( !mTestCars )
		{
			// don't test against other cars until missile has traveled outside our collision radius
			VuAabb collisionAabb = mpLauncher->getCollisionAabb();
			collisionAabb.mMin -= VuVector3(mCarCollisionRadius);
			collisionAabb.mMax += VuVector3(mCarCollisionRadius);

			if ( !VuMathUtil::testIntersectionLineSegBox(prevPos, curPos, collisionAabb, mpLauncher->getModelMatrix()) )
				mTestCars = true;
		}

		if ( mTestCars )
		{
			for ( int i = 0; i < VuCarManager::IF()->getCarCount(); i++ )
			{
				VuCarEntity *pOtherCar = VuCarManager::IF()->getCar(i);

				if ( pOtherCar != mpLauncher )
				{
					VuAabb collisionAabb = pOtherCar->getCollisionAabb();
					collisionAabb.mMin -= VuVector3(mCarCollisionRadius);
					collisionAabb.mMax += VuVector3(mCarCollisionRadius);

					if ( VuMathUtil::testIntersectionLineSegBox(prevPos, curPos, collisionAabb, pOtherCar->getModelMatrix()) )
					{
						if ( !pOtherCar->isGhostly() )
						{
							bool wasCrashed = pOtherCar->hasCrashed();

							VuParams params;
							params.addVector3(mTransform.getTrans());
							params.addVector3(mLinearVelocity);
							params.addString(mData["CarEffect"].asCString());
							params.addEntity(mpLauncher);
							pOtherCar->handleEvent("OnHitByMissile", params);

							if ( pOtherCar->hasCrashed() && !wasCrashed )
							{
								mCarVictimCount++;
								if ( mCarVictimCount == 3 && mpLauncher->getDriver()->isHuman() )
									VuAchievementManager::IF()->unlock(38);

								if ( pOtherCar != mpTarget )
								{
									VuAudioUtil::playSfx(mData["InnocentBystanderSfx"].asCString(), mTransform.getTrans());
								}
							}
						}

						if ( pOtherCar == mpTarget )
						{
							VuVector3 impactPoint = pOtherCar->getModelMatrix().transform(collisionAabb.getCenter());
							mTransform.setTrans(impactPoint);
							explode();
						}
					}
				}
			}
		}
	}

	// check if missile has hit any breakable or dynamic props
	{
		VuFirstMissileFlightRayTestResult rayTestResult;
		VuDynamicsRayTest::test(prevPos, curPos, rayTestResult);

		if ( rayTestResult.mbHasHit )
		{
			if ( VuEntity *pEntity = rayTestResult.mpRigidBody->getEntity() )
			{
				VuParams params;
				params.addVector3(mTransform.getTrans());
				params.addVector3(mLinearVelocity);
				params.addString(mData["CarEffect"].asCString());
				params.addEntity(mpLauncher);
				pEntity->handleEvent("OnHitByMissile", params);
			}
		}
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
}

//*****************************************************************************
void VuFirstMissileEntity::explode()
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
