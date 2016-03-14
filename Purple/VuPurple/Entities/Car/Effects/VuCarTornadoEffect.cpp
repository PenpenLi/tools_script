//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuCarTornadoEffect
// 
//*****************************************************************************

#include "VuCarEffect.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarEffectController.h"
#include "VuPurple/Entities/Car/Parts/VuCarSuspension.h"
#include "VuEngine/Dynamics/VuDynamics.h"
#include "VuEngine/Math/VuRand.h"


class VuCarTornadoEffect : public VuCarEffect
{
public:
	VuCarTornadoEffect() : mCurTime(0.0f), mCurRotation(0.0f), mEndTime(0.0f), mEndRotation(0.0f), mpConstraint(VUNULL) {}
	virtual void	onStart(const VuJsonContainer &data, const char *effectName);
	virtual void	onApply(const VuJsonContainer &data);
	virtual void	onStop(const VuJsonContainer &data);
	virtual void	onTick(float fdt);

	void			destroyConstraint();

	float					mCurTime;
	float					mCurRotation;
	float					mStartTime;
	float					mStartRotation;
	float					mEndTime;
	float					mEndRotation;
	btGeneric6DofConstraint	*mpConstraint;
};

IMPLEMENT_CAR_EFFECT(VuCarTornadoEffect)


//*****************************************************************************
void VuCarTornadoEffect::onStart(const VuJsonContainer &data, const char *effectName)
{
	if ( !mpCar->getEffectController()->isActive(VuCarEffectController::TORNADO) )
	{
		mpCar->getEffectController()->pushSubEffect(VuCarEffectController::TORNADO);
		mpCar->getEffectController()->pushTraction();

		mpConstraint = new btGeneric6DofConstraint(*mpCar->getRigidBody(), btTransform::getIdentity(), true);
		mpConstraint->setLinearLowerLimit(btVector3(-FLT_MAX, -FLT_MAX, -FLT_MAX));
		mpConstraint->setLinearUpperLimit(btVector3( FLT_MAX,  FLT_MAX,  FLT_MAX));
		mpConstraint->setAngularLowerLimit(btVector3(-FLT_MAX, -FLT_MAX, -FLT_MAX));
		mpConstraint->setAngularUpperLimit(btVector3( FLT_MAX,  FLT_MAX,  FLT_MAX));
		VuDynamics::IF()->addConstraint(mpConstraint, true);
	}
}

//*****************************************************************************
void VuCarTornadoEffect::onApply(const VuJsonContainer &data)
{
	if ( mpConstraint )
	{
		float effectDuration = data["Duration"].asFloat();
		int maxTurns = data["Turns"][data["Turns"].size() - 1].asInt();
		int turns = data["Turns"][VuRand::global().range(0, data["Turns"].size())].asInt();
		float rotation = turns*360.0f;
		float spinDuration = effectDuration*turns/maxTurns;

		mTimeRemaining -= (effectDuration - spinDuration);

		mStartTime = 0.0f;
		mStartRotation = 0.0f;

		mEndTime = spinDuration;
		mEndRotation = rotation;

		mpCar->getSuspension()->setTractionFactor(0.0f);
	}
}

//*****************************************************************************
void VuCarTornadoEffect::onStop(const VuJsonContainer &data)
{
	destroyConstraint();
}

//*****************************************************************************
void VuCarTornadoEffect::onTick(float fdt)
{
	if ( mpConstraint )
	{
		mCurTime += fdt;

		mCurTime = VuMin(mCurTime, mEndTime);

		float t = VuLinStep(mStartTime, mEndTime, mCurTime);
		t = VuMin(1.2f*t, 1.0f);
		mCurRotation = VuLerp(mStartRotation, mEndRotation, t);

		VuVector3 rotation(0.0f, 0.0f, VuDegreesToRadians(mCurRotation));

		mpConstraint->setAngularLowerLimit(VuDynamicsUtil::toBtVector3(rotation));
		mpConstraint->setAngularUpperLimit(VuDynamicsUtil::toBtVector3(rotation));

		if ( mCurTime >= mEndTime )
			destroyConstraint();
	}
}

//*****************************************************************************
void VuCarTornadoEffect::destroyConstraint()
{
	if ( mpConstraint )
	{
		mpCar->getEffectController()->popSubEffect(VuCarEffectController::TORNADO);
		mpCar->getEffectController()->popTraction();

		VuDynamics::IF()->removeConstraint(mpConstraint);
		delete mpConstraint;
		mpConstraint = VUNULL;
	}
}