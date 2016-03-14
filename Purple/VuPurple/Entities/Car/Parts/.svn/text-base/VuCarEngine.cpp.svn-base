//*****************************************************************************
//
//  Copyright (c) 2007-2008 Ralf Knoesel
//  Copyright (c) 2008-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  CarEngine class
// 
//*****************************************************************************

#include "VuCarEngine.h"
#include "VuCarSuspension.h"
#include "VuCarEffectController.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarPowerUpController.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Assets/VuSpreadsheetAsset.h"
#include "VuEngine/Water/VuWater.h"
#include "VuEngine/Water/VuFluidsObject.h"
#include "VuEngine/Math/VuUnitConversion.h"
#include "VuEngine/Math/VuRand.h"
#include "VuEngine/Math/VuMathUtil.h"
#include "VuEngine/HAL/Audio/VuAudio.h"
#include "VuEngine/Json/VuJsonContainer.h"
#include "VuEngine/Util/VuDataUtil.h"
#include "VuEngine/Util/VuSpreadsheetQuery.h"
#include "VuEngine/Dev/VuDev.h"


// constants
#define RANDOM_REV_DELAY_MIN 1.0f
#define RANDOM_REV_DELAY_MAX 2.0f
#define RAMDOM_REV_ACCEL VuDegreesToRadians(-360.0f)
#define WHEEL_RADIUS 1.0f


//*****************************************************************************
VuCarEngine::VuCarEngine(VuCarEntity *pCar):
	mpCar(pCar),
	mBaseMaxForwardSpeed(1),
	mBoostedMaxForwardSpeed(1),
	mBoostedPowerMultiplier(1),
	mMaxReverseSpeed(1),
	mMaxBrakingForce(0.0f),
	mGearCount(1),
	mHeadroomRPM(0.0f),
	mMaxPower(0.0f),
	mMinRPM(0.0f),
	mMaxRPM(0.0f),
	mMaxTorque(0.0f),
	mRPM(0.0f),
	mLoad(0.0f),
	mCurEngineForce(0.0f),
	mCurBrakingForce(0.0f),
	mGear(0),
	mPrevGear(0),
	mAudioRPM(0.0f),
	mAudioLoad(0.0f),
	mUpShifting(false),
	mBackFiring(false),
	mRandomRev(false),
	mRandomRevTimer(0.0f),
	mRandomRevRPM(0.0f)
{
}

//*****************************************************************************
VuCarEngine::~VuCarEngine()
{
}

//*****************************************************************************
void VuCarEngine::onGameInitialize()
{
	startAudio();
}

//*****************************************************************************
void VuCarEngine::onGameRelease()
{
	stopAudio();
}

//*****************************************************************************
void VuCarEngine::setData(const VuJsonContainer &data)
{
	// get properties
	VuDataUtil::getValue(data["Max Reverse Speed"], mMaxReverseSpeed);
	VuDataUtil::getValue(data["Max Braking Force"], mMaxBrakingForce);
	VuDataUtil::getValue(data["Gear Count"], mGearCount);
	VuDataUtil::getValue(data["Headroom RPM"], mHeadroomRPM);
	VuDataUtil::getValue(data["Min RPM"], mMinRPM);
	VuDataUtil::getValue(data["Max RPM"], mMaxRPM);

	// audio
	VuDataUtil::getValue(data["Audio"]["Run"], mAudioEventName);
}

//*****************************************************************************
void VuCarEngine::postDataModified()
{
	configure(mpCar->getCarStage(), mpCar->getAccelLevel(), mpCar->getSpeedLevel());

	if ( mpCar->isGameInitialized() )
	{
		stopAudio();
		startAudio();
	}
}

//*****************************************************************************
void VuCarEngine::configure(int carStage, int accelLevel, int speedLevel)
{
	// acceleration data
	mMaxPower = VuGameUtil::IF()->interpolateCarLevel(mpCar->getCarName(), carStage, accelLevel, "Max Power");

	// top speed data
	mBaseMaxForwardSpeed = VuGameUtil::IF()->interpolateCarLevel(mpCar->getCarName(), carStage, speedLevel, "Max Speed");

	mRPM = mMinRPM;
	mMaxTorque = VuFtlbToNm(mMaxPower*5252.0f/mMaxRPM);

	// calculate ratios
	mRatios.resize(mGearCount);
	for ( int i = 0; i < mGearCount; i++ )
	{
		float topSpeed = VuMphToMetersPerSecond(mBaseMaxForwardSpeed)*(i + 1)/mGearCount;
		mRatios[i] = mMaxRPM*VU_2PI*WHEEL_RADIUS/(60.0f*topSpeed);
	}
}

//*****************************************************************************
void VuCarEngine::applyTuneUp()
{
	int carStage = mpCar->getCarStage();
	int accelLevel = mpCar->getAccelLevel();
	int speedLevel = mpCar->getSpeedLevel();

	// apply tune-up
	if ( carStage == 3 )
	{
		int numLevels = VuGameUtil::IF()->getNumCarLevels(carStage);
		accelLevel = numLevels;
		speedLevel = numLevels;
	}
	else
	{
		carStage++;
	}

	configure(carStage, accelLevel, speedLevel);
}

//*****************************************************************************
void VuCarEngine::onTick(float fdt)
{
	// update RPM/load/gear
	if ( mRandomRev )
	{
		updateRandomRevParams(fdt);
	}
	else if ( mpCar->getSuspension()->getWheelContactCount() == 0 )
	{
		updateAirborneParams(fdt);
	}
	else
	{
		updateParams(fdt);
	}

	// update audio
	{
		float audioLoad = mLoad;

		// handle upshift
		if ( mGear > mPrevGear )
			mUpShifting = true;
		mPrevGear = mGear;

		if ( mUpShifting )
		{
			audioLoad = 0.0f;
			if ( mAudioRPM <= mRPM )
				mUpShifting = false;
		}

		// backfiring?
		if ( mBackFiring )
		{
			if ( audioLoad > 0.1f )
				mBackFiring = false;
		}
		else
		{
			mBackFiring = mUpShifting;
			if ( audioLoad < 0.1f && mRPM < mAudioRPM )
				mBackFiring = true;
		}

		mAudioRPM = VuMathUtil::chase(mAudioRPM, mRPM, 2.0f*(mMaxRPM - mMinRPM)*fdt);
		mAudioLoad = VuMathUtil::chase(mAudioLoad, audioLoad, 2.0f*fdt);

		mEngineAudioEvent.setParameterValue("rpm", mAudioRPM);
		mEngineAudioEvent.setParameterValue("load", mAudioLoad);

		const VuVector3 &pos = mpCar->getModelPosition();
		const VuVector3 &vel = mpCar->getLinearVelocity();
		mEngineAudioEvent.set3DAttributes(&pos, &vel, VUNULL);
	}
}

//*****************************************************************************
void VuCarEngine::hide()
{
	mEngineAudioEvent.release();
}

//*****************************************************************************
void VuCarEngine::updateRandomRevParams(float fdt)
{
	mRandomRevTimer -= fdt;
	if ( mRandomRevTimer <= 0 )
	{
		mRandomRevTimer = VuRand::global().range(RANDOM_REV_DELAY_MIN, RANDOM_REV_DELAY_MAX);
		if ( mRandomRevRPM == 0.0f ) // first time?
		{
			mRandomRevRPM = mMinRPM;
		}
		else
		{
			mLoad = 1.0f;
			mBackFiring = false;
			mRandomRevRPM = mMaxRPM;
		}
	}

	if ( mRandomRevRPM > mRPM )
	{
		mRPM = VuMin(mRPM + 2.0f*(mMaxRPM - mMinRPM)*fdt, mRandomRevRPM);
		if ( mRPM >= mRandomRevRPM )
		{
			mRandomRevRPM = mMinRPM;
			mLoad = 0.0f;
			mBackFiring = true;
		}
	}
	else
	{
		mRPM = VuMax(mRPM - 1.0f*(mMaxRPM - mMinRPM)*fdt, mRandomRevRPM);
	}

	mCurEngineForce = 0.0f;
	mCurBrakingForce = 0.0f;
}

//*****************************************************************************
void VuCarEngine::updateAirborneParams(float fdt)
{
	float throttle = VuMax(mpCar->getThrottleControl(), 0.0f);

	mRPM = VuLerp(mMinRPM, 1.05f*mMaxRPM, throttle);
	mLoad = 0.5f*throttle;
	mCurEngineForce = 0.0f;
	mCurBrakingForce = 0.0f;
}

//*****************************************************************************
void VuCarEngine::updateParams(float fdt)
{
	float fForwardSpeed = VuDot(mpCar->getLinearVelocity(), mpCar->getModelMatrix().getAxisY());

	// determine rpm and gear
	if ( fForwardSpeed >= 0.0f )
	{
		while ( 1 )
		{
			mRPM = 60.0f*fForwardSpeed*mRatios[mGear]/VU_2PI*WHEEL_RADIUS;
			if ( mRPM > mMaxRPM )
			{
				if ( mGear == mRatios.size() - 1 )
					break;
				mGear++;
			}
			else if ( mGear == 0 )
			{
				break;
			}
			else
			{
				float rpmInLowerGear = 60.0f*fForwardSpeed*mRatios[mGear - 1]/VU_2PI*WHEEL_RADIUS;
				if ( rpmInLowerGear + mHeadroomRPM > mMaxRPM )
					break;
				mGear--;
			}
		}
	}
	else
	{
		mRPM = -60.0f*fForwardSpeed*mRatios[mGear]/VU_2PI*WHEEL_RADIUS;
	}
	mRPM = VuMax(mRPM, mMinRPM);
	if ( mRPM > mMaxRPM )
		mRPM = 0.5f*(mRPM + mMaxRPM);

	// determine max engine torque
	float engineTorque = 0.0f;

	if ( mpCar->getEffectController()->isActive(VuCarEffectController::BOOST) )
	{
		if ( fForwardSpeed < VuMphToMetersPerSecond(mpCar->getAiBuffSpeed()*mBoostedMaxForwardSpeed) )
			engineTorque = mpCar->getAiBuffAccel()*mBoostedPowerMultiplier*mMaxTorque;
	}
	else
	{
		if ( fForwardSpeed < VuMphToMetersPerSecond(mpCar->getAiBuffSpeed()*mBaseMaxForwardSpeed) )
			engineTorque = mpCar->getAiBuffAccel()*mMaxTorque;
	}

	// calculate engine force (at wheel)
	float throttle = mpCar->getThrottleControl();
	float engineForce = engineTorque*mRatios[mGear]/WHEEL_RADIUS;
	mCurEngineForce = throttle*engineForce;

	mCurBrakingForce = VuMax(-throttle, 0.0f)*mMaxBrakingForce;

	// reverse top speed
	if ( mCurEngineForce < 0.0f )
	{
		if ( fForwardSpeed < -VuMphToMetersPerSecond(mMaxReverseSpeed) )
			mCurEngineForce = 0.0f;
	}

	// determine load
	mLoad = VuMax(throttle, 0.0f);
}

//*****************************************************************************
void VuCarEngine::onApplyForces(float fdt)
{
	if ( mRandomRev )
	{
		if ( mRandomRevRPM > mRPM )
		{
			VuVector3 accel(0.0f, RAMDOM_REV_ACCEL, 0.0f);
			accel = mpCar->getModelMatrix().transformNormal(accel);

			btMatrix3x3 inertiaTensor = mpCar->getRigidBody()->getInvInertiaTensorWorld().inverse();
			mpCar->getRigidBody()->applyTorque(VuDynamicsUtil::toBtVector3(accel)*inertiaTensor);
		}
	}
}

//*****************************************************************************
void VuCarEngine::startAudio()
{
	if ( mEngineAudioEvent.create(mAudioEventName.c_str()) )
	{
		const VuVector3 &pos = mpCar->getModelPosition();
		mEngineAudioEvent.set3DAttributes(&pos, VUNULL, VUNULL);
		mEngineAudioEvent.start();
	}
}

//*****************************************************************************
void VuCarEngine::stopAudio()
{
	mEngineAudioEvent.release();
}
