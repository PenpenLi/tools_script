//*****************************************************************************
//
//  Copyright (c) 2012-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Car Headlights
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Math/VuMatrix.h"
#include "VuEngine/Util/VuColor.h"

class VuCarEntity;
class VuJsonContainer;
class VuStaticModelInstance;
class VuAabb;
class VuGfxDrawParams;
class VuDynamicLight;


class VuCarHeadlights
{
public:
	VuCarHeadlights(VuCarEntity *pCar);
	~VuCarHeadlights();

	void					onGameInitialize() {}
	void					onGameRelease() {}

	void					setData(const VuJsonContainer &data);
	void					preDataModified();
	void					postDataModified();

	const VuAabb			&getAabb();
	float					getFalloffRangeMin()	{ return mFalloffRangeMin; }
	float					getFalloffRangeMax()	{ return mFalloffRangeMax; }
	float					getConeAngle()			{ return mConeAngle; }
	float					getPenumbraAngle()		{ return mPenumbraAngle; }
	const VuMatrix			&getLightTransform()	{ return mLightTransform; }
	const VuVector3			&getLightDiffuseColor()	{ return mLightDiffuseColor; }

	void					setTurnedOn(bool on) { mTurnedOn = on; }
	bool					getTurnedOn()        { return mTurnedOn; }

	void					onTick(float fdt);
	void					draw(const VuGfxDrawParams &params);
	void					drawPrefetch();

private:
	void					clear();

	void					updateDynamicLight();

	VuCarEntity				*mpCar;

	// data
	std::string				mModelAssetName;
	VuVector3				mPosOffset;
	VuVector3				mRotOffset;
	float					mFalloffRangeMin;
	float					mFalloffRangeMax;
	float					mConeAngle;
	float					mPenumbraAngle;
	VuColor					mDiffuseColor;
	float					mSwitchTime;

	VuMatrix				mLightTransform;
	VuStaticModelInstance	*mpModelInstance;
	VuDynamicLight			*mpDynamicLight;

	bool					mTurnedOn;
	float					mBrightness;
	VuVector3				mLightDiffuseColor;
};
