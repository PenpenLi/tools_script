//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuCarOilyTiresEffect
// 
//*****************************************************************************

#include "VuCarEffect.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarEffectController.h"
#include "VuPurple/Entities/Car/Parts/VuCarChassis.h"


class VuCarOilyTiresEffect : public VuCarEffect
{
public:
	virtual void	onStart(const VuJsonContainer &data, const char *effectName);
	virtual void	onApply(const VuJsonContainer &data);
	virtual void	onStop(const VuJsonContainer &data);
};

IMPLEMENT_CAR_EFFECT(VuCarOilyTiresEffect)


//*****************************************************************************
void VuCarOilyTiresEffect::onStart(const VuJsonContainer &data, const char *effectName)
{
	mpCar->getEffectController()->pushSubEffect(VuCarEffectController::OILY_TIRES);
	mpCar->getEffectController()->pushWheelSurfaceOverride("Oil");

	mpCar->getChassis()->pushLooseSteering();
}

//*****************************************************************************
void VuCarOilyTiresEffect::onApply(const VuJsonContainer &data)
{
	VuCarChassis::LooseSteeringParams params;
	params.mSteeringFactor = data["SteeringFactor"].asFloat();
	params.mSteeringErrorAmount = data["SteeringErrorAmount"].asFloat();
	params.mSteeringErrorInterval = data["SteeringErrorInterval"].asFloat();
	mpCar->getChassis()->setLooseSteeringParams(params);
}

//*****************************************************************************
void VuCarOilyTiresEffect::onStop(const VuJsonContainer &data)
{
	mpCar->getEffectController()->popSubEffect(VuCarEffectController::OILY_TIRES);
	mpCar->getEffectController()->popWheelSurfaceOverride();

	mpCar->getChassis()->popLooseSteering();
}

