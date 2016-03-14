//*****************************************************************************
//
//  Copyright (c) 2012-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Car Wheel
// 
//*****************************************************************************

#include "VuCarWheel.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Gfx/Model/VuStaticModelInstance.h"
#include "VuEngine/Gfx/GfxScene/VuGfxStaticScene.h"
#include "VuEngine/Pfx/VuPfx.h"
#include "VuEngine/Managers/VuTireTrackManager.h"


//*****************************************************************************
VuCarWheel::VuCarWheel():
	mLod1Dist(FLT_MAX),
	mFlipModel(false),
	mIsFrontWheel(false),
	mAttachmentPointLS(0.0f, 0.0f, 0.0f),
	mRadius(0.0f),
	mLatFrictionCoeff(0.0f),
	mLongFrictionCoeff(0.0f),
	mBlurAngularVelocity(0.0f),
	mTireTrackWidth(0.0f),
	mTireTrackMinDist(0.0f),
	mTireTrackMinVel(0.0f),
	mpWaterPfx(VUNULL),
	mpTireTrack(VUNULL),
	mSteeringAngle(0.0f),
	mEngineForce(0.0f),
	mBrakingForce(0.0f),
	mTransform(VuMatrix::identity()),
	mCurRotation(0.0f),
	mCurAngVel(0.0f),
	mSideSlide(false),
	mBrakeSlide(false),
	mEngineSlide(false),
	mIsGameInitialized(false),
	mVisualContact(false),
	mVisualSuspensionDist(0.0f),
	mVisualScaleZ(1.0f),
	mVisualWaterHeight(-1e9),
	mSubmerged(false),
	mCurScale(1.0f),
	mCurScaleX(1.0f),
	mSurfaceTypeOverride(255)
{
	mpModelInstance = new VuStaticModelInstance;
	mpLod1ModelInstance = new VuStaticModelInstance;

	mpModelInstance->setRejectionScaleModifier(0.0f);
	mpLod1ModelInstance->setRejectionScaleModifier(0.0f);
}

//*****************************************************************************
VuCarWheel::~VuCarWheel()
{
	delete mpModelInstance;
	delete mpLod1ModelInstance;
}

//*****************************************************************************
void VuCarWheel::onGameInitialize()
{
	mIsGameInitialized = true;

	create();
}

//*****************************************************************************
void VuCarWheel::onGameRelease()
{
	mIsGameInitialized = false;

	clear();
}

//*****************************************************************************
void VuCarWheel::setWheel(eWheel wheel)
{
	mFlipModel = (wheel == FRONT_LEFT || wheel == REAR_LEFT);
	mIsFrontWheel = (wheel == FRONT_LEFT || wheel == FRONT_RIGHT);
}

//*****************************************************************************
void VuCarWheel::create()
{
	mpWaterPfx = VuPfx::IF()->createSystemInstance(mWaterPfxName.c_str());

	VuTireTrackParams tireTrackParams;
	if ( mTireTrackWidth > 0.0f )	tireTrackParams.mWidth = mTireTrackWidth;
	if ( mTireTrackMinDist > 0.0f )	tireTrackParams.mMinDist = mTireTrackMinDist;
	if ( mTireTrackMinVel > 0.0f )	tireTrackParams.mMinVel = mTireTrackMinVel;
	tireTrackParams.mOffsetX = mpModelInstance->getAabb().getCenter().mX;
	if ( mFlipModel )
		tireTrackParams.mOffsetX *= -1.0f;
	mpTireTrack = VuTireTrackManager::IF()->createTireTrack(tireTrackParams);

	mTireTrackWidth = tireTrackParams.mWidth;
	mTireTrackMinDist = tireTrackParams.mMinDist;
	mTireTrackMinVel = tireTrackParams.mMinVel;
}

//*****************************************************************************
void VuCarWheel::clear()
{
	if ( mpWaterPfx )
	{
		VuPfx::IF()->releaseSystemInstance(mpWaterPfx);
		mpWaterPfx = VUNULL;
	}

	if ( mpTireTrack )
	{
		VuTireTrackManager::IF()->releaseTireTrack(mpTireTrack);
		mpTireTrack = VUNULL;
	}
}

//*****************************************************************************
void VuCarWheel::setData(const VuJsonContainer &data)
{
	data.getValue(mWheelName);
}

//*****************************************************************************
void VuCarWheel::preDataModified()
{
	clear();
}

//*****************************************************************************
void VuCarWheel::postDataModified()
{
	const VuJsonContainer &data = VuGameUtil::IF()->wheelDB()[mWheelName];

	std::string modelAsset;
	std::string lod1ModelAsset;

	VuDataUtil::getValue(data["Model Asset"], modelAsset);
	VuDataUtil::getValue(data["LOD 1 Model Asset"], lod1ModelAsset);
	VuDataUtil::getValue(data["LOD 1 Distance"], mLod1Dist);
	VuDataUtil::getValue(data["Radius"], mRadius);
	VuDataUtil::getValue(data["Lat Friction Coeff"], mLatFrictionCoeff);
	VuDataUtil::getValue(data["Long Friction Coeff"], mLongFrictionCoeff);
	VuDataUtil::getValue(data["Blur Angular Velocity"], mBlurAngularVelocity);
	VuDataUtil::getValue(data["Water Pfx"], mWaterPfxName);
	VuDataUtil::getValue(data["Tire Track Width"], mTireTrackWidth);
	VuDataUtil::getValue(data["Tire Track Min Dist"], mTireTrackMinDist);
	VuDataUtil::getValue(data["Tire Track Min Vel"], mTireTrackMinVel);

	mBlurAngularVelocity = VuDegreesToRadians(mBlurAngularVelocity);

	mpModelInstance->setModelAsset(modelAsset);
	mpLod1ModelInstance->setModelAsset(lod1ModelAsset);

	mBlurSkin.build(mpModelInstance->getGfxStaticScene(), data["Blur Skin"]);

	if ( mIsGameInitialized )
		create();
}

//*****************************************************************************
void VuCarWheel::draw(const VuGfxDrawParams &params, const VuColor &color, float dist, VUUINT32 dynamicLightGroupMask)
{
	if ( dist > mLod1Dist )
	{
		mpLod1ModelInstance->setColor(color);
		mpLod1ModelInstance->setDynamicLightGroupMask(dynamicLightGroupMask);
		mpLod1ModelInstance->draw(mTransform, params);
	}
	else
	{
		mpModelInstance->setColor(color);
		mpModelInstance->setDynamicLightGroupMask(dynamicLightGroupMask);
		mpModelInstance->setMaterialSubstIF(VuAbs(mCurAngVel) > mBlurAngularVelocity ? &mBlurSkin : VUNULL);
		mpModelInstance->draw(mTransform, params);
	}
}

//*****************************************************************************
void VuCarWheel::drawPrefetch()
{
	mpLod1ModelInstance->drawPrefetch();
	mpModelInstance->drawPrefetch();
}

//*****************************************************************************
void VuCarWheel::drawShadow(const VuGfxDrawShadowParams &params, float dist)
{
	if ( dist > mLod1Dist )
		mpLod1ModelInstance->drawShadow(mTransform, params);
	else
		mpModelInstance->drawShadow(mTransform, params);
}

//*****************************************************************************
void VuCarWheel::drawDropShadow(const VuGfxDrawShadowParams &params, float dist)
{
	if ( dist > mLod1Dist )
		mpLod1ModelInstance->drawDropShadow(mTransform, params);
	else
		mpModelInstance->drawDropShadow(mTransform, params);
}
