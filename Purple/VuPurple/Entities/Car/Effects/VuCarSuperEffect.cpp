//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuCarSuperEffect
// 
//*****************************************************************************

#include "VuCarEffect.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarEffectController.h"
#include "VuPurple/Entities/Car/Parts/VuCarSuspension.h"
#include "VuPurple/Entities/Car/Parts/VuCarEngine.h"
#include "VuPurple/Util/VuSiren.h"
#include "VuEngine/Managers/VuTickManager.h"


class VuCarSuperEffect : public VuCarEffect
{
public:
	VuCarSuperEffect() : mSirens(0) {}

	virtual void	onStart(const VuJsonContainer &data, const char *effectName);
	virtual void	onApply(const VuJsonContainer &data);
	virtual void	onStop(const VuJsonContainer &data);
	virtual void	onDraw(const VuGfxDrawParams &params);

	void			tickCorona(float fdt);

	typedef VuArray<VuSiren *> Sirens;
	Sirens			mSirens;
};

IMPLEMENT_CAR_EFFECT(VuCarSuperEffect)


//*****************************************************************************
void VuCarSuperEffect::onStart(const VuJsonContainer &data, const char *effectName)
{
	mpCar->getEffectController()->pushToughness(data["SmashCarEffect"].asCString());
	mpCar->getEffectController()->pushSubEffect(VuCarEffectController::BOOST);
	mpCar->getEffectController()->pushSubEffect(VuCarEffectController::SHIELD);
	mpCar->getEffectController()->pushTraction();

	const VuJsonContainer &sirenDatas = data["Sirens"];
	if ( sirenDatas.size() )
	{
		mSirens.resize(sirenDatas.size());
		for ( int i = 0; i < sirenDatas.size(); i++ )
		{
			const VuJsonContainer &sirenData = sirenDatas[i];

			VuSiren *pSiren = new VuSiren;

			pSiren->loadData(sirenData);
			pSiren->mTransform *= mpCar->getMountPoint(sirenData["Mount"].asCString());

			mSirens[i] = pSiren;
		}

		VuTickManager::IF()->registerHandler(this, &VuCarSuperEffect::tickCorona, "Corona");
	}
}

//*****************************************************************************
void VuCarSuperEffect::onApply(const VuJsonContainer &data)
{
	float speedMPH = data["Speed"].asFloat();
	float power = data["Power"].asFloat();
	float tractionFactor = data["TractionFactor"].asFloat();

	VuCarEngine *pEngine = mpCar->getEngine();

	float boostedSpeedMPH = pEngine->getBaseMaxForwardSpeedMPH() + speedMPH;
	pEngine->setBoostedMaxForwardSpeedMPH(boostedSpeedMPH);
	pEngine->setBoostedPowerMultiplier(power);

	mpCar->getSuspension()->setTractionFactor(tractionFactor);
}

//*****************************************************************************
void VuCarSuperEffect::onStop(const VuJsonContainer &data)
{
	mpCar->getEffectController()->popToughness();
	mpCar->getEffectController()->popSubEffect(VuCarEffectController::BOOST);
	mpCar->getEffectController()->popSubEffect(VuCarEffectController::SHIELD);
	mpCar->getEffectController()->popTraction();

	for ( int i = 0; i < (int)mSirens.size(); i++ )
		delete mSirens[i];
	mSirens.clear();

	VuTickManager::IF()->unregisterHandler(this, "Corona");
}

//*****************************************************************************
void VuCarSuperEffect::onDraw(const VuGfxDrawParams &params)
{
	for ( int i = 0; i < (int)mSirens.size(); i++ )
		mSirens[i]->draw(mpCar->getModelMatrix(), params);
}

//*****************************************************************************
void VuCarSuperEffect::tickCorona(float fdt)
{
	for ( int i = 0; i < (int)mSirens.size(); i++ )
	{
		mSirens[i]->updateVisibility(mpCar->getModelMatrix());
		mSirens[i]->tick(fdt);
	}
}
