//*****************************************************************************
//
//  Copyright (c) 2012-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Car Headlights
// 
//*****************************************************************************

#include "VuCarHeadlights.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuEngine/Gfx/Light/VuLightManager.h"
#include "VuEngine/Json/VuJsonContainer.h"
#include "VuEngine/Util/VuDataUtil.h"


//*****************************************************************************
VuCarHeadlights::VuCarHeadlights(VuCarEntity *pCar):
	mpCar(pCar),
	mPosOffset(0.0f, 0.0f, 0.0f),
	mRotOffset(0.0f, 0.0f, 0.0f),
	mFalloffRangeMin(0.0f),
	mFalloffRangeMax(0.0f),
	mConeAngle(0.0f),
	mPenumbraAngle(0.0f),
	mDiffuseColor(255,255,255),
	mSwitchTime(1.0f),
	mLightTransform(VuMatrix::identity()),
	mTurnedOn(false),
	mBrightness(0.0f),
	mLightDiffuseColor(0.0f, 0.0f, 0.0f)
{
	mpModelInstance = new VuStaticModelInstance;
	mpDynamicLight = new VuDynamicLight;
}

//*****************************************************************************
VuCarHeadlights::~VuCarHeadlights()
{
	clear();

	delete mpModelInstance;
	delete mpDynamicLight;
}

//*****************************************************************************
void VuCarHeadlights::setData(const VuJsonContainer &data)
{
	// calculate offset matrix
	VuDataUtil::getValue(data["Model Asset"], mModelAssetName);
	VuDataUtil::getValue(data["PosOffset"], mPosOffset);
	VuDataUtil::getValue(data["RotOffset"], mRotOffset);
	VuDataUtil::getValue(data["Falloff Range Min"], mFalloffRangeMin);
	VuDataUtil::getValue(data["Falloff Range Max"], mFalloffRangeMax);
	VuDataUtil::getValue(data["Cone Angle"], mConeAngle);
	VuDataUtil::getValue(data["Penumbra Angle"], mPenumbraAngle);
	VuDataUtil::getValue(data["Diffuse Color"], mDiffuseColor);
	VuDataUtil::getValue(data["Switch Time"], mSwitchTime);
}

//*****************************************************************************
void VuCarHeadlights::preDataModified()
{
	clear();
}

//*****************************************************************************
void VuCarHeadlights::postDataModified()
{
	mpModelInstance->setModelAsset(mModelAssetName);

	mRotOffset *= VuDegreesToRadians(1.0f);
	mLightTransform.setEulerAngles(mRotOffset);
	mLightTransform.setTrans(mPosOffset);

	mConeAngle = VuDegreesToRadians(mConeAngle);
	mPenumbraAngle = VuDegreesToRadians(mPenumbraAngle);

	mpDynamicLight->mDiffuseColor = mDiffuseColor;
	mpDynamicLight->mFalloffRangeMin = mFalloffRangeMin;
	mpDynamicLight->mFalloffRangeMax = mFalloffRangeMax;
	mpDynamicLight->mConeAngle = mConeAngle;
	mpDynamicLight->mPenumbraAngle = mPenumbraAngle;
}

//*****************************************************************************
const VuAabb &VuCarHeadlights::getAabb()
{
	if ( mBrightness > 0.0f )
		return mpModelInstance->getAabb();

	return VuAabb::zero();
}

//*****************************************************************************
void VuCarHeadlights::onTick(float fdt)
{
	if ( mSwitchTime > FLT_EPSILON )
	{
		if ( mTurnedOn )
			mBrightness = VuMin(mBrightness + fdt/mSwitchTime, 1.0f);
		else
			mBrightness = VuMax(mBrightness - fdt/mSwitchTime, 0.0f);
	}
	else
	{
		mBrightness = mTurnedOn;
	}

	mDiffuseColor.toVector3(mLightDiffuseColor);
	mLightDiffuseColor *= mBrightness;

	updateDynamicLight();
}

//*****************************************************************************
void VuCarHeadlights::draw(const VuGfxDrawParams &params)
{
	if ( mBrightness > 0.0f )
	{
		const VuMatrix &modelMat = mpCar->getModelMatrix();

		VuColor color(mLightDiffuseColor);
		mpModelInstance->setColor(color);
		mpModelInstance->draw(modelMat, params);
	}
}

//*****************************************************************************
void VuCarHeadlights::drawPrefetch()
{
	mpModelInstance->drawPrefetch();
}

//*****************************************************************************
void VuCarHeadlights::clear()
{
}

//*****************************************************************************
void VuCarHeadlights::updateDynamicLight()
{
	int viewport;
	if (VuCarManager::IF()->getViewport(mpCar, viewport) && (mBrightness > 0.0f))
	{
		const VuMatrix &modelMat = mpCar->getModelMatrix();

		mpDynamicLight->mPosition = modelMat.transform(mLightTransform.getTrans());
		mpDynamicLight->mDirection = modelMat.transformNormal(mLightTransform.getAxisY());
		mpDynamicLight->mFactor = mBrightness;
		mpDynamicLight->mViewportMask = 1 << viewport;
		mpDynamicLight->mGroup = 1 << (viewport + 1);

		mpDynamicLight->update();
		mpDynamicLight->turnOn();
	}
	else
	{
		mpDynamicLight->turnOff();
	}
}