//*****************************************************************************
//
//  Copyright (c) 2007-2008 Ralf Knoesel
//  Copyright (c) 2008-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  CarCamera class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Gfx/Camera/VuCamera.h"

class VuCarEntity;
class VuJsonContainer;


class VuCarCamera : public VuCamera
{
public:
	VuCarCamera(VuCarEntity *pCar);
	~VuCarCamera();

	void				setData(const VuJsonContainer &data);
	void				preDataModified() {}
	void				postDataModified() {}

	void				onTick(float fdt);
	void				snap();
	void				control(float roll, bool bReverse);

	void				beginShake(float magnitude, float duration, float falloffTime, float frequency, int priority = 0);
	void				stopShake();

	bool				hideCarForMode();

private:
	void				tickTilt(float fdt, VuVector3 &vPos, VuVector3 &vTarget, VuVector3 &vUp, float &fNearPlane);
	void				tickStandard(float fdt, VuVector3 &vPos, VuVector3 &vTarget, VuVector3 &vUp, float &fNearPlane);
	void				tickRagdoll(float fdt, VuVector3 &vPos, VuVector3 &vTarget, VuVector3 &vUp, float &fNearPlane);
	void				tickShake(float fdt, VuVector3 &vPos, VuVector3 &vTarget, VuVector3 &vUp);
	void				collide(VuVector3 &vPos, VuVector3 &vTarget, VuVector3 &vUp);

	enum { HISTORY_SIZE = 10 };
	VuCarEntity			*mpCar;
	int					mCarLinearVelocityHistoryIndex;
	VuVector3			mCarLinearVelocityHistory[HISTORY_SIZE];
	VuVector3			mCarLinearVelocity;

	// tuning params
	VuVector3			mTargetLocalOffset;
	float				mTargetWorldZOffset;
	float				mIdealDist;
	float				mIdealPitch;
	float				mLagFactor;
	float				mLagDist;
	float				mTiltStayBehindFactor;

	// state
	VuVector3			mIdealPosition;
	VuVector3			mPosition;
	bool				mbSnap;
	float				mRollControl;
	bool				mbReverseControl;
	VuVector3			mCarVelocity;
	int					mShakePriority;
	float				mShakeMagnitude;
	float				mShakeTotalTime;
	float				mShakeFalloffTime;
	float				mShakeFreq;
	VuVector3			mShakeAxis;
	float				mStayBehindFactor;
};