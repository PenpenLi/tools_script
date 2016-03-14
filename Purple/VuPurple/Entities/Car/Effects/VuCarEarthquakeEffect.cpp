//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuCarEarthquakeEffect
// 
//*****************************************************************************

#include "VuCarEffect.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarSuspension.h"
#include "VuPurple/Entities/Car/Camera/VuCarCamera.h"
#include "VuEngine/Math/VuUnitConversion.h"
#include "VuEngine/Math/VuRand.h"


class VuCarEarthquakeEffect : public VuCarEffect
{
public:
	virtual void	onApply(const VuJsonContainer &data);
	virtual void	onStop(const VuJsonContainer &data);
	virtual void	onApplyForces(float fdt);

	float			mImpulseFrequency;
	float			mMaxImpulse;
	VuRand			mRand;
};

IMPLEMENT_CAR_EFFECT(VuCarEarthquakeEffect)


//*****************************************************************************
void VuCarEarthquakeEffect::onApply(const VuJsonContainer &data)
{
	float magnitude = data["CamShakeMagnitude"].asFloat();
	float falloffTime = data["CamShakeFalloffTime"].asFloat();
	float frequency = data["CamShakeFrequency"].asFloat();
	int priority = data["CamShakePriority"].asInt();

	mImpulseFrequency = data["ImpulseFrequency"].asFloat();
	mMaxImpulse = VuMphToMetersPerSecond(data["MaxImpulse"].asFloat());

	mpCar->getCamera()->beginShake(magnitude, mTimeRemaining, falloffTime, frequency, priority);
}

//*****************************************************************************
void VuCarEarthquakeEffect::onStop(const VuJsonContainer &data)
{
	// if effect was forced to stop, stop shaking... otherwise let it fade out by itself
	if ( mTimeRemaining > 0.0f )
	{
		mpCar->getCamera()->stopShake();
	}
}

//*****************************************************************************
void VuCarEarthquakeEffect::onApplyForces(float fdt)
{
	// impulse?
	if ( mRand.rand() < fdt*mImpulseFrequency )
	{
		if ( mpCar->getSuspension()->getWheelContactCount() )
		{
			VuRigidBody &rb = *mpCar->getRigidBody();

			VuVector3 impulse(mMaxImpulse*mRand.range(-1.0f, 1.0f), 0.0f, 0.0f);
			impulse = mpCar->getModelMatrix().transformNormal(impulse);

			// apply impulse
			VuVector3 linVel = rb.getVuLinearVelocity();
			linVel += impulse;
			rb.setVuLinearVelocity(linVel);
		}
	}
}
