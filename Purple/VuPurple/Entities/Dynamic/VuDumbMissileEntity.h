//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Missile PowerUp class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Dynamics/VuDynamics.h"
#include "VuEngine/HAL/Audio/VuAudioEvent.h"

class VuCarEntity;


class VuDumbMissileEntity : public VuEntity, public VuDynamicsStepCallback
{
	DECLARE_RTTI

public:
	VuDumbMissileEntity(const VuJsonContainer &data, VuCarEntity *pCar, const VuMatrix &transform);

	virtual void	onGameInitialize();
	virtual void	onGameRelease();

protected:
	void			tickDecision(float fdt);

	// VuDynamicsStepCallback
	virtual void	onDynamicsApplyForces(float fdt);
	virtual void	onDynamicsAdvanceEnvironment(float fdt, bool bSimStep);

	void			explode();

	const VuJsonContainer	&mData;

	VuCarEntity			*mpLauncher;
	bool				mTestCars;
	VuMatrix			mTransform;
	VuVector3			mLinearVelocity;
	VuVector3			mLastPosition;
	float				mDistance;
	VUUINT32			mCollidedHash;
	VuAudioEvent		mStartSfx;
	VUUINT32			mFlightPfx;
	VuAudioEvent		mFlightSfx;
	bool				mPlayRicochetSfx;

	float				mRange;
	float				mCosTargetAcquisitionCone;
	float				mAltitute;
	float				mSpeed;
	float				mGravity;
	float				mGroundSnapFactor;
	float				mCarCollisionRadius;
	bool				mRicochet;
};
