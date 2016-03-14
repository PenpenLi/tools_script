//*****************************************************************************
//
//  Copyright (c) 2007-2008 Ralf Knoesel
//  Copyright (c) 2008-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  CarBody class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Water/VuWaterWakeWave.h"
#include "VuEngine/HAL/Audio/VuAudioEvent.h"

class VuCarEntity;
class VuFluidsObject;
class VuWaterFlatWakeWave;
class VuWaterFlatWakeWaveDesc;
class VuJsonContainer;
class VuMatrix;
class VuCamera;


class VuCarChassis
{
public:
	VuCarChassis(VuCarEntity *pCar);
	~VuCarChassis();

	void					onGameInitialize() {}
	void					onGameRelease() {}

	void					setData(const VuJsonContainer &data);
	void					preDataModified() {}
	void					postDataModified();

	void					onTick(float fdt);
	void					onApplyForces(float fdt);

	const VuVector3			&getAerodynamicCenter()		{ return mAerodynamicCenter; }
	const VuVector3			&getHydrodynamicCenter()	{ return mHydrodynamicCenter; }
	VuFluidsObject			*getFluidsObject() const	{ return mpFluidsObject; }

	struct LooseSteeringParams
	{
		LooseSteeringParams() : mSteeringFactor(1.0f), mSteeringErrorAmount(0.5f), mSteeringErrorInterval(2.0f) {}
		float	mSteeringFactor;
		float	mSteeringErrorAmount;
		float	mSteeringErrorInterval;
	};
	void					pushLooseSteering();
	void					popLooseSteering();
	void					setLooseSteeringParams(const LooseSteeringParams &params) { mLooseSteeringParams = params; }
	void					setInducedPowerSlideSteeringFactor(float factor) { mInducedPowerSlideSteeringFactor = factor; }

	void					setAirControlMultiplier(float value) { mAirborneControlCoeff = value; }
	float					getAirControlMultiplier() { return mAirborneControlCoeff; }

	void					setAirSteeringSpeed(float value) { mAirSteeringSpeed = value; }
	float					getAirSteeringSpeed() { return mAirSteeringSpeed; }

private:
	void					advanceWake();
	void					calculateWakeParams(VuWaterWakeWaveParams &params);
	void					calculateFluidsForces(float fdt, const VuMatrix &matModel, VuVector3 &vTotalForce, VuVector3 &vTotalTorque);
	void					calculateDragForces(float fdt, const VuMatrix &matModel, VuVector3 &vTotalForce, VuVector3 &vTotalTorque);
	void					calculateLiftForces(float fdt, const VuMatrix &matModel, VuVector3 &vTotalForce, VuVector3 &vTotalTorque);
	void					calculateWaterSelfRightingForces(float fdt, const VuMatrix &matModel, VuVector3 &vTotalForce, VuVector3 &vTotalTorque);
	void					calculateStabilityForces(float fdt, const VuMatrix &matModel, VuVector3 &vTotalForce, VuVector3 &vTotalTorque);
	void					calculatePowerSlideForces(float fdt, const VuMatrix &matModel, VuVector3 &vTotalForce, VuVector3 &vTotalTorque);
	void					calculateAirborneCorrectionForces(float fdt, const VuMatrix &matModel, VuVector3 &vTotalForce, VuVector3 &vTotalTorque);
	void					applySteering(float fdt, const VuMatrix &matModel);

	VuCarEntity				*mpCar;

	std::string				mFluidsMeshAssetName;
	VuVector3				mHydrodynamicCenter;
	VuVector3				mAerodynamicCenter;
	VuVector3				mStability;
	float					mWaterDragDensity;
	float					mWaterBuoyancyDensity;
	float					mAirDensity;
	float					mDensityRatio;
	float					mDensityRatioSpeed;
	float					mLinVelSpeed0;
	float					mLinVelFactor0;
	float					mLinVelSpeed1;
	float					mLinVelFactor1;
	float					mLatSkinFrictionCoeff;
	float					mLongSkinFrictionCoeff;
	float					mPowerSlideCoeff;
	float					mDragCoeff;
	float					mSlowSteeringBoatSpeed;
	float					mSlowSteeringSpeed;
	float					mFastSteeringBoatSpeed;
	float					mFastSteeringSpeed;
	float					mAirSteeringSpeed;
	float					mAeroLift;
	float					mAirborneSpringCoeff;
	float					mAirborneDampingCoeff;
	float					mAirborneControlCoeff;
	VuVector3				mWakePos;
	float					mAudioBounceThreshold;
	float					mAudioBigSplashThreshold;
	std::string				mBigSplashPFX;
	std::string				mLavaSplashPFX;
	std::string				mLavaBurnPFX;

	float					mWaterSelfRightingTimeThreshold;
	float					mWaterSelfRightingBaseAccel;
	float					mWaterSelfRightingTimer;

	float					mAirborneTimer;

	bool					mbBigSplash;

	// fluids object
	VuFluidsObject			*mpFluidsObject;

	// wake
	VuWaterWakeWaveDesc		mWakeWaveDesc;
	VuWaterFlatWakeWave		*mpCurFlatWakeWave;

	// loose steering
	LooseSteeringParams		mLooseSteeringParams;
	float					mLooseSteeringTimer;
	int						mLooseSteeringActive;

	// steering factor
	float					mInducedPowerSlideSteeringFactor;

	// audio
	VuAudioEvent			mSplashAudioEvent;
	VuAudioEvent			mBigSplashAudioEvent;
	float					mLastSubmergedVolume;
};