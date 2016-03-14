//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Translocate car effect
// 
//*****************************************************************************

#include "VuCarEffect.h"
#include "VuPurple/Entities/Car/Driver/VuAiDriverEntity.h"
#include "VuPurple/Entities/Car/Camera/VuCarCamera.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Managers/VuAchievementManager.h"
#include "VuPurple/PowerUps/VuPowerUpUtil.h"
#include "VuEngine/Dynamics/Util/VuDynamicsRayTest.h"
#include "VuEngine/Pfx/VuPfxManager.h"
#include "VuEngine/Pfx/VuPfxEntity.h"
#include "VuEngine/Pfx/VuPfx.h"


//*****************************************************************************
// VuCarTranslocateEffect
//*****************************************************************************

class VuCarTranslocateEffect : public VuCarEffect
{
public:
	VuCarTranslocateEffect() : mpTarget(VUNULL), mTargetPfx(0) {}

	virtual void	onStart(const VuJsonContainer &data, const char *effectName);
	virtual void	onStop(const VuJsonContainer &data);
	virtual void	onTick(float fdt);

	float			mRange;

	VuCarEntity		*mpTarget;
	VuAudioEvent	mTargetSfx;
	VUUINT32		mTargetPfx;
};

IMPLEMENT_CAR_EFFECT(VuCarTranslocateEffect)


class VuTranslocateRayTestResult : public VuDynamicsRayTest::VuClosestResult
{
public:
	virtual bool needsCollision(VuRigidBody *pRigidBody)
	{
		if ( !(pRigidBody->getCollisionGroup() & COL_ENGINE_STATIC_PROP) )
			return false;

		if ( pRigidBody->getExtendedFlags() & EXT_COL_ENGINE_BREAKABLE )
			return false;

		if ( pRigidBody->getExtendedFlags() & EXT_COL_ENGINE_NOT_CORONA )
			return false;

		return true;
	}
};


//*****************************************************************************
void VuCarTranslocateEffect::onStart(const VuJsonContainer &data, const char *effectName)
{
	mRange = data["Range"].asFloat();

	VuVector3 cameraPos = mpCar->getCamera()->getTransform().getTrans();

	// determine target
	float bestDist = mRange;
	for ( int i = 0; i < VuCarManager::IF()->getCarCount(); i++ )
	{
		VuCarEntity *pOtherCar = VuCarManager::IF()->getCar(i);
		if ( pOtherCar != mpCar )
		{
			// check distance
			VuVector3 otherPos = pOtherCar->getModelMatrix().transform(pOtherCar->getCollisionAabb().getCenter());
			VuVector3 delta = otherPos - cameraPos;
			float dist = delta.normalize();
			if ( dist < bestDist )
			{
				// is on screen?
				VuVector3 screenPos = mpCar->getCamera()->worldToScreen(otherPos);
				if ( screenPos.mZ > 0.0f && screenPos.mX > 0.0f && screenPos.mX < 1.0f && screenPos.mY > 0.0f && screenPos.mY < 1.0f )
				{
					// check line-of-sight from camera
					VuTranslocateRayTestResult rayTestResult;
					VuDynamicsRayTest::test(cameraPos, otherPos, rayTestResult);

					if ( !rayTestResult.mbHasHit )
					{
						mpTarget = pOtherCar;
						bestDist = dist;
					}
				}
			}
		}
	}

	if ( mpTarget )
	{
		if ( mTargetSfx.create(data["StartSfx"].asCString()) )
		{
			mTargetSfx.set3DAttributes(&mpTarget->getModelPosition(), &mpTarget->getLinearVelocity(), VUNULL);
			mTargetSfx.start();
		}

		mTargetPfx = VuPowerUpUtil::createPfx(mpTarget, data["LoopingPfx"], false);
	}
	else
	{
		mTimeRemaining = 0.0f;
	}
}

//*****************************************************************************
void VuCarTranslocateEffect::onStop(const VuJsonContainer &data)
{
	// stop target sfx/pfx
	mTargetSfx.release(VuAudioEvent::STOP_IMMEDIATE);

	if ( VuPfxEntity *pPfxEntity = VuPfxManager::IF()->getEntity(mTargetPfx) )
		pPfxEntity->getSystemInstance()->stop();
	mTargetPfx = 0;

	// translocate!
	if ( mpTarget )
	{
		if ( mpCar->getDriver()->isHuman() && mpTarget->getStats().mDistToEndOfLap < 100.0f && mpTarget->getStats().mLapCount == mpTarget->getStats().mCurLap)
			VuAchievementManager::IF()->unlock(46); // Use the Alien to teleport into 1st place within 100m of the finish line

		// handle physical state
		VuMatrix carTransform = mpCar->getModelMatrix();
		VuVector3 carLinVel = mpCar->getLinearVelocity();
		VuVector3 carAngVel = mpCar->getAngularVelocity();

		VuMatrix targetTransform = mpTarget->getModelMatrix();
		VuVector3 targetLinVel = mpTarget->getLinearVelocity();
		VuVector3 targetAngVel = mpTarget->getAngularVelocity();

		mpCar->getTransformComponent()->setWorldTransform(targetTransform);
		mpCar->getRigidBody()->setVuLinearVelocity(targetLinVel);
		mpCar->getRigidBody()->setVuAngularVelocity(targetAngVel);

		mpTarget->getTransformComponent()->setWorldTransform(carTransform);
		mpTarget->getRigidBody()->setVuLinearVelocity(carLinVel);
		mpTarget->getRigidBody()->setVuAngularVelocity(carAngVel);

		// handle stats
		VuCarStats &carStats = mpCar->getStats();
		VuCarStats &targetStats = mpTarget->getStats();

		VuSwap(carStats.mCurLap, targetStats.mCurLap);
		VuSwap(carStats.mPrevPosition, targetStats.mPrevPosition);
		
		mpCar->getDriver()->onTeleported();
		mpTarget->getDriver()->onTeleported();

		// success sfx/pfx
		VuPowerUpUtil::createSfx(mpCar, data["SuccessSfx"].asCString());
		VuPowerUpUtil::createSfx(mpTarget, data["SuccessSfx"].asCString());

		VuPowerUpUtil::createPfx(mpCar, data["SuccessPfx"], true);
		VuPowerUpUtil::createPfx(mpTarget, data["SuccessPfx"], true);
	}
	else
	{
		// fail sfx on alien
		VuPowerUpUtil::createSfx(mpCar, data["FailSfx"].asCString());
	}
}

//*****************************************************************************
void VuCarTranslocateEffect::onTick(float fdt)
{
	if ( mpTarget )
	{
		bool lostTarget = false;

		if ( mpCar->getHasFinished() || mpTarget->getHasFinished() )
		{
			lostTarget = true;
			VUPRINTF("Translocate target lost due to finishing\n");
		}
		else if ( mpCar->getDriver()->isRagdollActive() || mpTarget->getDriver()->isRagdollActive() )
		{
			lostTarget = true;
			VUPRINTF("Translocate target lost due to ragdoll\n");
		}
		else
		{
			// check distance
			VuVector3 cameraPos = mpCar->getCamera()->getTransform().getTrans();
			VuVector3 targetPos = mpTarget->getModelMatrix().transform(mpTarget->getCollisionAabb().getCenter());
			//VuVector3 delta = targetPos - cameraPos;
			//float dist = delta.normalize();
			//if ( dist > mRange )
			//{
			//	lostTarget = true;
			//	VUPRINTF("Translocate target lost due to distance\n");
			//}
			//else
			{
				// is on screen?
				VuVector3 screenPos = mpCar->getCamera()->worldToScreen(targetPos);
				if ( screenPos.mZ < 0.0f || screenPos.mX < 0.0f || screenPos.mX > 1.0f || screenPos.mY < 0.0f || screenPos.mY > 1.0f )
				{
					lostTarget = true;
					VUPRINTF("Translocate target lost due to off screen\n");
				}
				//else
				//{
				//	// check line-of-sight
				//	VuTranslocateRayTestResult rayTestResult;
				//	VuDynamicsRayTest::test(cameraPos, targetPos, rayTestResult);
				//	if ( rayTestResult.mbHasHit )
				//	{
				//		lostTarget = true;
				//		VUPRINTF("Translocate target lost due to ray cast\n");
				//	}
				//}
			}
		}

		if ( lostTarget )
		{
			// play fail sfx on target
			VuPowerUpUtil::createSfx(mpTarget, (*mpData)["FailSfx"].asCString());
			mpTarget = VUNULL;
		}
	}

	if ( mpTarget == VUNULL)
		mTimeRemaining = 0.0f;
}
