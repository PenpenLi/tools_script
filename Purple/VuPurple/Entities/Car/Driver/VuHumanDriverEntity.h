//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  HumanDriver class
// 
//*****************************************************************************

#pragma once

#include "VuDriverEntity.h"
#include "VuEngine/Math/VuRect.h"
#include "VuEngine/Containers/VuArray.h"
#include "VuEngine/Util/VuStringFormat.h"

class VuTextureAsset;


class VuHumanDriver : public VuDriverEntity
{
	DECLARE_RTTI

public:
	VuHumanDriver(VuCarEntity *pCar, int padIndex, bool anyPad);

	virtual bool				isHuman()	{ return true; }
	virtual bool				isAi()		{ return false; }
	virtual bool				isLocal()	{ return true; }
	virtual bool				isRemote()	{ return false; }
	virtual bool				isUi()		{ return false; }

	virtual void				onGameInitialize();
	virtual void				onGameRelease();
	virtual void				onRecover();

	virtual void				onTickDecision(float fdt);
	virtual void				onTeleported();

	virtual const VuTrackSector	*getCurrentSector() const { return mpCurrentGameSector; }
	virtual bool				headingWrongWay() const	{ return mHeadingWrongWay; }

	int							getPadIndex() { return mPadIndex; }

private:
	typedef VuArray<VuVector2> TouchPoints;

	enum eBoostPhase { BOOSTPHASE_NONE, BOOSTPHASE_PREBOOST, BOOSTPHASE_BOOSTABLE, BOOSTPHASE_POSTBOOST };

	void				updateControls(float fdt);
	void				updateWrongWay(float fdt);

	void				OnStartLightSequence(const VuParams& params);
	
	eBoostPhase			mStartBoostPhase;
	bool				mBoostAtStart;
	bool				mBoostPressed;

	int					mPadIndex;
	bool				mAnyPad;
	const VuTrackSector	*mpCurrentGameSector;
	const VuTrackSector	*mpCurrentRealSector;
	float				mWrongWayInertia;
	bool				mHeadingWrongWay;
	float				mSteeringExtent0;
	float				mSteeringExtent1;
	float				mAccelerometerCameraDamping;
	float				mGamePadCameraTilt;
	float				mWrongWaySpeedThreshold;
	float				mWrongWayAngleThreshold;
	float				mWrongWayInertiaTime;
	float				mCameraRoll;
	bool				mPowerSlideDetection;
	float				mPowerSlideBrakeTimer;
	float				mRecoverSpeedThreshold;
	bool				mReachedRecoverSpeedThreshold;
};