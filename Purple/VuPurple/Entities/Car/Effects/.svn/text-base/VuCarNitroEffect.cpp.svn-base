//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuCarNitroEffect
// 
//*****************************************************************************

#include "VuCarEffect.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarEffectController.h"
#include "VuPurple/PowerUps/VuPowerUpUtil.h"
#include "VuEngine/Managers/VuExplosionManager.h"
#include "VuEngine/Pfx/VuPfxManager.h"
#include "VuEngine/Pfx/VuPfxEntity.h"
#include "VuEngine/Pfx/VuPfx.h"


class VuCarNitroEffect : public VuCarEffect
{
public:
	VuCarNitroEffect() : mExploded(false) {}
	virtual void	onStart(const VuJsonContainer &data, const char *effectName);
	virtual void	onStop(const VuJsonContainer &data);
	virtual void	onTick(float fdt);
	bool			mExploded;
};

IMPLEMENT_CAR_EFFECT(VuCarNitroEffect)


//*****************************************************************************
void VuCarNitroEffect::onStart(const VuJsonContainer &data, const char *effectName)
{
	mpCar->getEffectController()->pushSubEffect(VuCarEffectController::NITRO);
}

//*****************************************************************************
void VuCarNitroEffect::onStop(const VuJsonContainer &data)
{
	mpCar->getEffectController()->popSubEffect(VuCarEffectController::NITRO);

	if ( mExploded )
	{
		// hard kill looping pfx
		if (VuPfxEntity *pPfxEntity = VuPfxManager::IF()->getEntity(mLoopingPfx))
			pPfxEntity->getSystemInstance()->stop(true);

		VuExplosionManager::IF()->createExplosion(mpCar->getModelPosition(), data["Explosion"].asCString(), mpCar);
	}
}

//*****************************************************************************
void VuCarNitroEffect::onTick(float fdt)
{
	if ( mpCar->getDriver()->isRagdollActive() )
	{
		mExploded = true;
		mTimeRemaining = 0.0f;
	}
}
