//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuCarPlantEffect
// 
//*****************************************************************************

#include "VuCarEffect.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarEngine.h"
#include "VuPurple/Entities/Car/Parts/VuCarSuspension.h"
#include "VuPurple/Entities/Car/Parts/VuCarEffectController.h"
#include "VuPurple/Entities/Dynamic/VuPlantEntity.h"
#include "VuEngine/Entities/VuEntityRepository.h"
#include "VuEngine/Dynamics/Util/VuDynamicsRayTest.h"
#include "VuEngine/Math/VuMathUtil.h"


class VuCarPlantEffect : public VuCarEffect
{
public:
	VuCarPlantEffect() : mItemsRemaining(0), mLastSpot(0,0,0) {}

	virtual void	onStart(const VuJsonContainer &data, const char *effectName);
	virtual void	onApply(const VuJsonContainer &data);
	virtual void	onStop(const VuJsonContainer &data);
	virtual void	onTick(float fdt);

	void			dropItem();

	int				mItemsRemaining;
	float			mMaxHeight;
	float			mMinSpacing;

	float			mPlantRate;
	VuVector3		mLastSpot;
};

IMPLEMENT_CAR_EFFECT(VuCarPlantEffect)


//*****************************************************************************
class VuPlantRayTestResult : public VuDynamicsRayTest::VuClosestResult
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
void VuCarPlantEffect::onStart(const VuJsonContainer &data, const char *effectName)
{
	mpCar->getEffectController()->pushSubEffect(VuCarEffectController::BOOST);
	mpCar->getEffectController()->pushTraction();
}

//*****************************************************************************
void VuCarPlantEffect::onApply(const VuJsonContainer &data)
{
	float speedMPH = data["Speed"].asFloat();
	float power = data["Power"].asFloat();
	float tractionFactor = data["TractionFactor"].asFloat();

	VuCarEngine *pEngine = mpCar->getEngine();

	float boostedSpeedMPH = pEngine->getBaseMaxForwardSpeedMPH() + speedMPH;
	pEngine->setBoostedMaxForwardSpeedMPH(boostedSpeedMPH);
	pEngine->setBoostedPowerMultiplier(power);

	mpCar->getSuspension()->setTractionFactor(tractionFactor);

	int maxCount = data["MaxCount"].asInt();
	mMaxHeight = data["MaxHeight"].asFloat();
	mMinSpacing = 2.0f*data["Radius"].asFloat();

	if ( mReapplyType == "Stack" )
		mItemsRemaining += maxCount;
	else
		mItemsRemaining = maxCount;

	mPlantRate = mItemsRemaining/mTimeRemaining;
}

//*****************************************************************************
void VuCarPlantEffect::onStop(const VuJsonContainer &data)
{
	mpCar->getEffectController()->popSubEffect(VuCarEffectController::BOOST);
	mpCar->getEffectController()->popTraction();
}

//*****************************************************************************
void VuCarPlantEffect::onTick(float fdt)
{
	int optimalItemsRemaining = VuMax(VuTruncate((mTimeRemaining - fdt)*mPlantRate), 0);

	// don't plant more than 1 per frame
	if ( mItemsRemaining > optimalItemsRemaining )
	{
		mItemsRemaining--;
		dropItem();
	}
}

//*****************************************************************************
void VuCarPlantEffect::dropItem()
{
	// first do a line test straight down
	VuPlantRayTestResult rayTestResult;
	VuVector3 pos = mpCar->getModelMatrix().transform(mpCar->getCollisionAabb().getCenter());
	VuVector3 ray(0.0f, 0.0f, -mMaxHeight);
	VuDynamicsRayTest::test(pos, pos + ray, rayTestResult);
	if ( rayTestResult.mbHasHit )
	{
		pos = pos + ray*rayTestResult.mHitFraction;

		if ( VuDist(pos, mLastSpot) > mMinSpacing )
		{
			VuMatrix transform;
			VuMathUtil::buildOrientationMatrixUp(mpCar->getModelMatrix().getAxisY(), rayTestResult.mHitNormal, transform);
			transform.setTrans(pos);

			VuEntityRepository::IF()->addManagedEntity(new VuPlantEntity(mpCar, *mpData, transform));

			mLastSpot = transform.getTrans();
		}
	}
}
