//*****************************************************************************
//
//  Copyright (c) 2012-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Car Wheel
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Math/VuMatrix.h"
#include "VuEngine/Gfx/Model/VuModelSkin.h"

class VuCarEntity;
class VuJsonContainer;
class VuCamera;
class VuStaticModelInstance;
class VuGfxDrawParams;
class VuGfxDrawShadowParams;
class VuColor;
class VuPfxSystemInstance;
class VuTireTrack;


class VuCarWheel
{
public:
	VuCarWheel();
	~VuCarWheel();

	enum eWheel { FRONT_LEFT, FRONT_RIGHT, REAR_LEFT, REAR_RIGHT };

	void					onGameInitialize();
	void					onGameRelease();

	void					setWheel(eWheel wheel);
	void					create();
	void					clear();

	float					getRadius() { return mRadius*mCurScale; }

	void					setData(const VuJsonContainer &data);
	void					preDataModified();
	void					postDataModified();

	void					draw(const VuGfxDrawParams &params, const VuColor &color, float dist, VUUINT32 dynamicLightGroupMask);
	void					drawPrefetch();
	void					drawShadow(const VuGfxDrawShadowParams &params, float dist);
	void					drawDropShadow(const VuGfxDrawShadowParams &params, float dist);

	bool					sliding() { return mSideSlide | mEngineSlide | mBrakeSlide; }

	// data
	std::string				mWheelName;

	VuStaticModelInstance	*mpModelInstance;
	VuStaticModelInstance	*mpLod1ModelInstance;
	float					mLod1Dist;
	bool					mFlipModel;
	bool					mIsFrontWheel;
	VuVector3				mAttachmentPointLS;
	float					mLatFrictionCoeff;
	float					mLongFrictionCoeff;
	float					mBlurAngularVelocity;
	std::string				mWaterPfxName;
	float					mTireTrackWidth;
	float					mTireTrackMinDist;
	float					mTireTrackMinVel;
	VuPfxSystemInstance		*mpWaterPfx;
	VuTireTrack				*mpTireTrack;

	// input
	float					mSteeringAngle;
	float					mEngineForce;
	float					mBrakingForce;

	// output
	VuMatrix				mTransform;
	float					mCurRotation;
	float					mCurAngVel;
	bool					mSideSlide;
	bool					mEngineSlide;
	bool					mBrakeSlide;

	bool					mIsGameInitialized;
	bool					mVisualContact;
	VUUINT8					mVisualContactSurfaceType;
	VUUINT32				mVisualContactCollisionGroup;
	VUUINT32				mVisualContactExtendedFlags;
	VuVector3				mVisualContactPointWS;
	VuVector3				mVisualContactNormal;
	float					mVisualSuspensionDist;
	float					mVisualScaleZ;

	float					mVisualWaterHeight;
	bool					mSubmerged;

	float					mCurScale;
	float					mCurScaleX;

	VUUINT8					mSurfaceTypeOverride;

private:
	VuModelSkin				mBlurSkin;
	float					mRadius;
};
