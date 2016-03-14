//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Best Line Effect entity
// 
//*****************************************************************************

#include "VuBestLineEffectEntity.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarSuspension.h"
#include "VuPurple/Entities/Car/Parts/VuCarEffectController.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Track/VuTrackSector.h"
#include "VuEngine/Entities/VuEntityRepository.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Pfx/VuPfxManager.h"
#include "VuEngine/Pfx/VuPfxEntity.h"
#include "VuEngine/Pfx/VuPfx.h"
#include "VuEngine/Math/VuMathUtil.h"
#include "VuEngine/Math/VuUnitConversion.h"
#include "VuEngine/Util/VuAudioUtil.h"


IMPLEMENT_RTTI(VuBestLineEffectEntity, VuEntity);


//*****************************************************************************
VuBestLineEffectEntity::VuBestLineEffectEntity(const VuJsonContainer &data, VuCarEntity *pCar):
	mData(data),
	mpLauncher(pCar),
	mFading(false),
	mAge(0.0f)
{
	// params
	mAltitute = data["Altitute"].asFloat();
	mSpeed = VuMphToMetersPerSecond(data["Speed"].asFloat());
	mLifeTime = data["LifeTime"].asFloat();
	mFadeTime = data["FadeTime"].asFloat();
	mCarApplyRadius = data["CarApplyRadius"].asFloat();

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
void VuBestLineEffectEntity::onGameInitialize()
{
	VuTickManager::IF()->registerHandler(this, &VuBestLineEffectEntity::tickDecision, "Decision");

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
void VuBestLineEffectEntity::onGameRelease()
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
void VuBestLineEffectEntity::tickDecision(float fdt)
{
	// follow AI path
	if ( mpCurSector )
	{
		float traversalAmount = mpCurSector->traversalAmount(mTransform.getTrans());
		if ( traversalAmount > 1.0f )
			mpCurSector = mpCurSector->mpNextSectors[0];
	}

	VuVector3 targetPos;
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
	
	VuVector3 prevPos = mTransform.getTrans();
	mLinearVelocity = mSpeed*(targetPos - prevPos).normal();
	VuVector3 curPos = mTransform.getTrans() + mLinearVelocity*fdt;

	VuMathUtil::buildOrientationMatrix(mLinearVelocity, mTransform.getAxisZ(), mTransform);
	mTransform.setTrans(curPos);

	mpTransformComponent->setWorldTransform(mTransform);

	if ( VuPfxEntity *pPfxEntity = VuPfxManager::IF()->getEntity(mFlightPfx) )
		pPfxEntity->getSystemInstance()->setMatrix(mpTransformComponent->getWorldTransform());

	if ( mStartSfx.active() )
		mStartSfx.set3DAttributes(&mpTransformComponent->getWorldPosition(), &mLinearVelocity, VUNULL);

	if ( mFlightSfx.active() )
		mFlightSfx.set3DAttributes(&mpTransformComponent->getWorldPosition(), &mLinearVelocity, VUNULL);

	// check for cars
	for ( int i = 0; i < VuCarManager::IF()->getCarCount(); i++ )
	{
		VuCarEntity *pOtherCar = VuCarManager::IF()->getCar(i);

		if ( (pOtherCar != mpLauncher) && (mCarsAffected.find(pOtherCar) == mCarsAffected.end()) )
		{
			// check distance
			VuVector3 otherPos = pOtherCar->getModelMatrix().transform(pOtherCar->getCollisionAabb().getCenter());
			VuVector3 delta = otherPos - mTransform.getTrans();
			float distSquared = delta.magSquared();
			if ( distSquared < VuSquare(mCarApplyRadius) )
			{
				pOtherCar->getEffectController()->applyEffect(mData["CarEffect"].asCString(), mpLauncher);
				mCarsAffected.insert(pOtherCar);
			}
		}
	}

	mAge += fdt;

	if ( mFading )
	{
		mFadeTimer += fdt;
		if ( mFadeTimer > mFadeTime )
		{
			VuEntityRepository::IF()->removeManagedEntity(this);
		}
		else
		{
			if ( mFlightSfx.active() )
				mFlightSfx.setVolume(1.0f - mFadeTimer/mFadeTime);
		}
	}
	else
	{
		if ( mAge > mLifeTime )
		{
			mFading = true;
			mFadeTimer = 0.0f;

			if ( VuPfxEntity *pPfxEntity = VuPfxManager::IF()->getEntity(mFlightPfx) )
				pPfxEntity->getSystemInstance()->stop();
		}
	}
}
