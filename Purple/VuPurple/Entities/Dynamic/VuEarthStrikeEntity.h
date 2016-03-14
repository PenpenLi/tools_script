//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuEarthStrikeEntity
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Dynamics/VuDynamics.h"
#include "VuEngine/HAL/Audio/VuAudioEvent.h"

class VuCarEntity;
class VuPowerUp;
class VuTireTrack;


class VuEarthStrikeEntity : public VuEntity, public VuDynamicsStepCallback
{
	DECLARE_RTTI

public:
	VuEarthStrikeEntity(const VuJsonContainer &data, VuCarEntity *pCar, const VuMatrix &transform);

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
	VuMatrix			mTransform;
	VuVector3			mLinearVelocity;
	VuVector3			mLastPosition;
	float				mDistance;
	bool				mWallCollision;
	VuAudioEvent		mStartSfx;
	VUUINT32			mFlightPfx;
	VuAudioEvent		mFlightSfx;
	VuTireTrack			*mpTireTrack;

	float				mRange;
	float				mCosTargetAcquisitionCone;
	float				mAltitute;
	float				mSpeed;
	float				mGravity;
	float				mGroundSnapFactor;
	float				mCollisionRadius;
	std::string			mCarEffect;
};
