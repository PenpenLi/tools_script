//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuCarBoostEffect
// 
//*****************************************************************************

#include "VuCarEffect.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarEffectController.h"
#include "VuPurple/Entities/Car/Parts/VuCarSuspension.h"
#include "VuPurple/Entities/Car/Parts/VuCarEngine.h"
#include "VuEngine/Math/VuUnitConversion.h"


class VuCarBoostEffect : public VuCarEffect
{
public:
	virtual void	onStart(const VuJsonContainer &data, const char *effectName);
	virtual void	onApply(const VuJsonContainer &data);
	virtual void	onStop(const VuJsonContainer &data);
	virtual void	onTick(float fdt);
};

IMPLEMENT_CAR_EFFECT(VuCarBoostEffect)


//*****************************************************************************
void VuCarBoostEffect::onStart(const VuJsonContainer &data, const char *effectName)
{
	mpCar->getEffectController()->pushSubEffect(VuCarEffectController::BOOST);
}

//*****************************************************************************
void VuCarBoostEffect::onApply(const VuJsonContainer &data)
{
	float speedMPH = data["Speed"].asFloat();
	float power = data["Power"].asFloat();
	float instantRatio = data["InstantRatio"].asFloat();

	VuCarEngine *pEngine = mpCar->getEngine();
	VuCarSuspension *pSuspension = mpCar->getSuspension();

	float boostedSpeedMPH = pEngine->getBaseMaxForwardSpeedMPH() + speedMPH;
	pEngine->setBoostedMaxForwardSpeedMPH(boostedSpeedMPH);
	pEngine->setBoostedPowerMultiplier(power);

	if ( !mpCar->getHasFinished() && pSuspension->getWheelContactCount() )
	{
		VuVector3 fwd = VuCross(pSuspension->getAvgContactNormal(), mpCar->getModelMatrix().getAxisX());
		float speed = VuDot(fwd, mpCar->getLinearVelocity());
		float boostedSpeed = VuMphToMetersPerSecond(boostedSpeedMPH);
		if ( speed < boostedSpeed )
		{
			float addedSpeed = instantRatio*(boostedSpeed - speed);

			VuVector3 fwd = VuCross(pSuspension->getAvgContactNormal(), mpCar->getModelMatrix().getAxisX());
			fwd.normalize();
			VuVector3 linVel = mpCar->getLinearVelocity() + addedSpeed*fwd;
			mpCar->getRigidBody()->setVuLinearVelocity(linVel);
		}
	}
}

//*****************************************************************************
void VuCarBoostEffect::onStop(const VuJsonContainer &data)
{
	mpCar->getEffectController()->popSubEffect(VuCarEffectController::BOOST);
}

//*****************************************************************************
void VuCarBoostEffect::onTick(float fdt)
{
	if ( mpCar->getHasFinished() || mpCar->getDriver()->isRagdollActive() )
		mTimeRemaining = 0.0f;
}
