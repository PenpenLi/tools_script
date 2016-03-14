//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuCarFreezeEffect
// 
//*****************************************************************************

#include "VuCarEffect.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarEffectController.h"
#include "VuPurple/Entities/Car/Parts/VuCarSuspension.h"
#include "VuPurple/Entities/Car/Parts/VuCarChassis.h"
#include "VuEngine/Gfx/Model/VuStaticModelInstance.h"
#include "VuPurple/Managers/VuAchievementManager.h"


class VuCarFreezeEffect : public VuCarEffect
{
public:
	virtual void	onStart(const VuJsonContainer &data, const char *effectName);
	virtual void	onApply(const VuJsonContainer &data);
	virtual void	onStop(const VuJsonContainer &data);
	virtual void	onTick(float fdt);
	virtual void	onDraw(const VuGfxDrawParams &params);

	VuStaticModelInstance	mModelInstance;
};

IMPLEMENT_CAR_EFFECT(VuCarFreezeEffect)


//*****************************************************************************
void VuCarFreezeEffect::onStart(const VuJsonContainer &data, const char *effectName)
{
	mpCar->getEffectController()->pushSubEffect(VuCarEffectController::FREEZE);

	mpCar->getEffectController()->pushWheelSurfaceOverride("Ice");

	mpCar->getChassis()->pushLooseSteering();

	if ( mpCar->getEffectController()->isActive(VuCarEffectController::ON_FIRE) )
		if ( mpOriginator && mpOriginator->getDriver()->isHuman() )
			VuAchievementManager::IF()->unlock(41); // Freeze an opponent who's on fire
}

//*****************************************************************************
void VuCarFreezeEffect::onApply(const VuJsonContainer &data)
{
	mModelInstance.setModelAsset(data["ModelAsset"].asString());

	VuCarChassis::LooseSteeringParams params;
	params.mSteeringFactor = data["SteeringFactor"].asFloat();
	params.mSteeringErrorAmount = data["SteeringErrorAmount"].asFloat();
	params.mSteeringErrorInterval = data["SteeringErrorInterval"].asFloat();
	mpCar->getChassis()->setLooseSteeringParams(params);
}

//*****************************************************************************
void VuCarFreezeEffect::onStop(const VuJsonContainer &data)
{
	mpCar->getEffectController()->popSubEffect(VuCarEffectController::FREEZE);

	mpCar->getEffectController()->popWheelSurfaceOverride();

	mpCar->getChassis()->popLooseSteering();
}

//*****************************************************************************
void VuCarFreezeEffect::onTick(float fdt)
{
	if ( mpCar->shatterIce() )
	{
		mTimeRemaining = 0.0f;
		mpCar->startCrash();
	}
}

//*****************************************************************************
void VuCarFreezeEffect::onDraw(const VuGfxDrawParams &params)
{
	VuMatrix modelMat = mpCar->getModelMatrix();
	modelMat.translateLocal(mpCar->getFreezeEffectAabb().getCenter());

	VuVector3 scale = mpCar->getFreezeEffectAabb().getExtents()/mModelInstance.getAabb().getExtents();
	modelMat.scaleLocal(scale);

	mModelInstance.draw(modelMat, params);
}
