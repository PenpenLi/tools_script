//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuCarWrestlingVictimEffect
// 
//*****************************************************************************

#include "VuCarEffect.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarEffectController.h"
#include "VuPurple/Entities/Car/Parts/VuCarSuspension.h"
#include "VuEngine/Dynamics/VuDynamics.h"
#include "VuEngine/Math/VuUnitConversion.h"


class VuCarWrestlingVictimEffect : public VuCarEffect
{
public:
	VuCarWrestlingVictimEffect() : mCurTime(0.0f), mCurRotation(0.0f), mEndTime(0.0f), mEndRotation(0.0f) {}
	virtual void	onStart(const VuJsonContainer &data, const char *effectName);
	virtual void	onApply(const VuJsonContainer &data);
	virtual void	onStop(const VuJsonContainer &data);
	virtual void	onTick(float fdt);

	bool					mSpinEnabled;
	float					mCurTime;
	float					mCurRotation;
	float					mEndTime;
	float					mEndRotation;
	btGeneric6DofConstraint	*mpConstraint;
};

IMPLEMENT_CAR_EFFECT(VuCarWrestlingVictimEffect)


//*****************************************************************************
void VuCarWrestlingVictimEffect::onStart(const VuJsonContainer &data, const char *effectName)
{
	mSpinEnabled = !mpCar->getEffectController()->isActive(VuCarEffectController::TORNADO);

	if ( mSpinEnabled )
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
void VuCarWrestlingVictimEffect::onApply(const VuJsonContainer &data)
{
	if ( mSpinEnabled )
	{
		float duration = data["Duration"].asFloat();
		float rotation = data["Rotation"].asFloat();

		mEndTime = duration;
		mEndRotation = rotation;

		mpCar->getSuspension()->setTractionFactor(0.0f);
	}

	float verticalSpeed = data["VerticalSpeed"].asFloat();
	verticalSpeed = VuMphToMetersPerSecond(verticalSpeed);

	VuVector3 linVel = mpCar->getLinearVelocity();
	linVel.mZ = verticalSpeed;
	mpCar->getRigidBody()->setVuLinearVelocity(linVel);
}

//*****************************************************************************
void VuCarWrestlingVictimEffect::onStop(const VuJsonContainer &data)
{
	if ( mSpinEnabled )
	{
		mpCar->getEffectController()->popSubEffect(VuCarEffectController::TORNADO);
		mpCar->getEffectController()->popTraction();

		VuDynamics::IF()->removeConstraint(mpConstraint);
		delete mpConstraint;
	}
}

//*****************************************************************************
void VuCarWrestlingVictimEffect::onTick(float fdt)
{
	if ( mSpinEnabled )
	{
		mCurTime += fdt;

		float t = mCurTime/mEndTime;
		t = VuMin(1.2f*t, 1.0f);
		mCurRotation = mEndRotation*t;

		VuVector3 rotation(0.0f, 0.0f, VuDegreesToRadians(mCurRotation));

		mpConstraint->setAngularLowerLimit(VuDynamicsUtil::toBtVector3(rotation));
		mpConstraint->setAngularUpperLimit(VuDynamicsUtil::toBtVector3(rotation));
	}
}
