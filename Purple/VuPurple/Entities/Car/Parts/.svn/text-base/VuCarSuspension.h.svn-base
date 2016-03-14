//*****************************************************************************
//
//  Copyright (c) 2012-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Car Suspension
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Math/VuVector3.h"

class VuCarEntity;
class VuJsonContainer;
class VuAnimatedModelInstance;
class VuStaticModelInstance;
class VuAnimatedSkeleton;
class VuAnimationControl;
class VuAabb;
class VuCamera;
class VuGfxDrawParams;
class VuGfxDrawShadowParams;
class VuColor;


class VuCarSuspension
{
public:
	VuCarSuspension(VuCarEntity *pCar);
	~VuCarSuspension();

	void					onGameInitialize() {}
	void					onGameRelease() {}

	void					setData(const VuJsonContainer &data);
	void					preDataModified();
	void					postDataModified();

	const VuAabb			&getAabb();
	int						getWheelContactCount()	{ return mWheelContactCount; }
	const VuVector3			&getAvgContactNormal()	{ return mAvgContactNormal; }

	void					onTickDecision(float fdt);
	void					onTickBuild(float fdt);
	void					onApplyForces(float fdt);

	void					drawDebug(const VuCamera &camera);
	void					draw(const VuGfxDrawParams &params, const VuColor &color, float dist, VUUINT32 dynamicLightGroupMask);
	void					drawPrefetch();
	void					drawShadow(const VuGfxDrawShadowParams &params, float dist);
	void					drawDropShadow(const VuGfxDrawShadowParams &params, float dist);

	void					setSpringFactor(float factor) { mSpringFactor = factor; }
	void					setDampingFactor(float factor) { mDampingFactor = factor; }
	void					setTractionFactor(float factor) { mTractionFactor = factor; }

	void					setInducedPowerSlideTractionFactor(float factor) { mInducedPowerSlideTractionFactor = factor; }
	void					setInducedPowerSlideCoeff(float coeff) { mInducedPowerSlideCoeff = coeff; }

	float					getTravel(int corner) { return mCorners[corner].mTravel; }

	void					setBounceSfx(const char *sfx, float threshold) { mBounceSfx = sfx; mBounceSfxThreshold = threshold; mBounceSfxTimer = 0.0f; }

private:
	struct VuCorner
	{
		VuCorner() : mpAnimationControl(VUNULL), mUpperLimit(0.0f), mLowerLimit(0.0f), mTravel(0.0f) {}
		std::string			mWheelBoneName;
		std::string			mAnimationName;
		VuAnimationControl	*mpAnimationControl;
		float				mUpperLimit;
		float				mLowerLimit;
		float				mTravel;
	};

	void					clear();

	VuCarEntity				*mpCar;

	std::string				mModelAssetName;
	float					mRolloverResistance;
	float					mWheelieResistance;
	float					mUpperSpringCoeff;
	float					mLowerSpringCoeff;
	float					mDampingCoeff;
	float					mPowerSlideCoeff;
	float					mVisualExtensionRate;
	VuCorner				mCorners[4];

	VuAnimatedModelInstance	*mpModelInstance;
	VuAnimatedSkeleton		*mpAnimatedSkeleton;

	int						mWheelContactCount;
	VuVector3				mAvgContactNormal;

	float					mBaseTractionFactor;

	float					mSpringFactor;
	float					mDampingFactor;
	float					mTractionFactor;

	float					mInducedPowerSlideTractionFactor;
	float					mInducedPowerSlideCoeff;

	std::string				mBounceSfx;
	float					mBounceSfxThreshold;
	float					mBounceSfxTimer;
};