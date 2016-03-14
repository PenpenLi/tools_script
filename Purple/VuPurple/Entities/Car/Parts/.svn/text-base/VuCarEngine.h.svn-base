//*****************************************************************************
//
//  Copyright (c) 2007-2008 Ralf Knoesel
//  Copyright (c) 2008-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  CarEngine class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Containers/VuArray.h"
#include "VuEngine/HAL/Audio/VuAudioEvent.h"

class VuCarEntity;
class VuJsonContainer;
class VuMatrix;
class VuPfxSystemInstance;

namespace FMOD
{
	class Event;
	class EventParameter;
}


class VuCarEngine
{
public:
	VuCarEngine(VuCarEntity *pCar);
	~VuCarEngine();

	void				onGameInitialize();
	void				onGameRelease();

	void				setData(const VuJsonContainer &data);
	void				preDataModified() {}
	void				postDataModified();

	void				configure(int carStage, int accelLevel, int speedLevel);
	void				applyTuneUp();

	void				onTick(float fdt);
	void				onApplyForces(float fdt);

	void				setBoostedMaxForwardSpeedMPH(float speedMPH) { mBoostedMaxForwardSpeed = speedMPH; }
	void				setBoostedPowerMultiplier(float mult) { mBoostedPowerMultiplier = mult; }

	float				getBaseMaxForwardSpeedMPH() const { return mBaseMaxForwardSpeed; }
	float				getBoostedMaxForwardSpeedMPH() const { return mBoostedMaxForwardSpeed; }

	int					getGear() const				{ return mGear; }
	float				getAudioRPM() const			{ return mAudioRPM; }
	float				getAudioLoad() const		{ return mAudioLoad; }
	float				getCurEngineForce() const	{ return mCurEngineForce; }
	float				getCurBrakingForce() const	{ return mCurBrakingForce; }
	float				getMaxBrakingForce() const	{ return mMaxBrakingForce; }
	bool				isBackFiring() const		{ return mBackFiring; }
	float				getMinRPM() const			{ return mMinRPM; }
	float				getMaxRPM() const			{ return mMaxRPM; }

	void				hide();

	void				setRandomRev(bool enable) { mRandomRev = enable; }

private:
	void				updateRandomRevParams(float fdt);
	void				updateAirborneParams(float fdt);
	void				updateParams(float fdt);

	void				startAudio();
	void				stopAudio();

	typedef VuArray<float> Ratios;

	VuCarEntity			*mpCar;

	float				mBaseMaxForwardSpeed;
	float				mBoostedMaxForwardSpeed;
	float				mBoostedPowerMultiplier;
	float				mMaxReverseSpeed;
	float				mMaxBrakingForce;
	int					mGearCount;
	float				mHeadroomRPM;
	float				mMaxPower;
	float				mMinRPM;
	float				mMaxRPM;
	std::string			mAudioEventName;

	Ratios				mRatios;
	float				mMaxTorque;
	float				mRPM;
	float				mLoad;
	float				mCurEngineForce;
	float				mCurBrakingForce;
	int					mGear;

	int					mPrevGear;
	float				mAudioRPM;
	float				mAudioLoad;
	bool				mUpShifting;
	bool				mBackFiring;
	bool				mRandomRev;
	float				mRandomRevTimer;
	float				mRandomRevRPM;

	// audio
	VuAudioEvent		mEngineAudioEvent;
};