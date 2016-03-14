//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  HumanDriver class
// 
//*****************************************************************************

#include "VuHumanDriverEntity.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarSuspension.h"
#include "VuPurple/Entities/Car/Camera/VuCarCamera.h"
#include "VuPurple/Track/VuTrackManager.h"
#include "VuPurple/Track/VuTrackSector.h"
#include "VuPurple/Track/VuTrackUtil.h"
#include "VuPurple/Managers/VuSettingsManager.h"
#include "VuPurple/Managers/VuControlMethodManager.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Managers/VuGameServicesManager.h"
#include "VuPurple/Managers/VuAchievementManager.h"
#include "VuPurple/PowerUps/VuPowerUpManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuPurple/Entities/Car/Parts/VuCarEffectController.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Managers/VuInputManager.h"
#include "VuEngine/HAL/Accel/VuAccel.h"
#include "VuEngine/HAL/Touch/VuTouch.h"
#include "VuEngine/HAL/Gfx/VuGfx.h"
#include "VuEngine/HAL/GamePad/VuGamePad.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/Font/VuFontDraw.h"
#include "VuEngine/Water/VuFluidsObject.h"
#include "VuEngine/Math/VuMathUtil.h"
#include "VuEngine/DB/VuStringDB.h"
#include "VuEngine/Math/VuRand.h"
#include "VuEngine/Math/VuUnitConversion.h"
#include "VuEngine/Util/VuDataUtil.h"
#include "VuEngine/Util/VuStringUtil.h"
#include "VuEngine/Util/VuAudioUtil.h"
#include "VuEngine/Dev/VuDevConfig.h"


IMPLEMENT_RTTI(VuHumanDriver, VuDriverEntity);

#define HUD_TEXT_ALPHA_RATE 2.0f


//*****************************************************************************
VuHumanDriver::VuHumanDriver(VuCarEntity *pCar, int padIndex, bool anyPad) :
VuDriverEntity(pCar),
	mStartBoostPhase(BOOSTPHASE_NONE),
	mBoostAtStart(false),
	mBoostPressed(false),
	mPadIndex(padIndex),
	mAnyPad(anyPad),
	mpCurrentGameSector(VUNULL),
	mpCurrentRealSector(VUNULL),
	mWrongWayInertia(0.0f),
	mHeadingWrongWay(false),
	mSteeringExtent0(10.0f),
	mSteeringExtent1(30.0f),
	mAccelerometerCameraDamping(1.0f),
	mGamePadCameraTilt(0.0f),
	mWrongWaySpeedThreshold(0.0f),
	mWrongWayAngleThreshold(0.0f),
	mWrongWayInertiaTime(0.0f),
	mCameraRoll(0.0f),
	mPowerSlideDetection(false),
	mPowerSlideBrakeTimer(0.0f),
	mRecoverSpeedThreshold(0.0f),
	mReachedRecoverSpeedThreshold(false)
{
	if ( VuGameServicesManager::IF() )
	{
		mName = VuGameServicesManager::IF()->getGamerTag();
	}
	else
	{
		mName = VuStringDB::IF()->getString("Common_Player");
	}

	// read constants
	VuGameUtil::IF()->constantDB()["HumanDriver"]["AccelerometerSteeringExtents"][0].getValue(mSteeringExtent0);
	VuGameUtil::IF()->constantDB()["HumanDriver"]["AccelerometerSteeringExtents"][1].getValue(mSteeringExtent1);
	VuGameUtil::IF()->constantDB()["HumanDriver"]["AccelerometerCameraDamping"].getValue(mAccelerometerCameraDamping);

	VuGameUtil::IF()->constantDB()["HumanDriver"]["GamePadCameraTilt"].getValue(mGamePadCameraTilt);
	VuGameUtil::IF()->constantDB()["HumanDriver"]["RecoverSpeedThreshold"].getValue(mRecoverSpeedThreshold);

	if ( VuGameUtil::IF()->constantDB()["HUD"]["WrongWaySpeedThreshold"].getValue(mWrongWaySpeedThreshold) )
		mWrongWaySpeedThreshold = VuMphToMetersPerSecond(mWrongWaySpeedThreshold);
	if ( VuGameUtil::IF()->constantDB()["HUD"]["WrongWayAngleThreshold"].getValue(mWrongWayAngleThreshold) )
		mWrongWayAngleThreshold = VuCos(VuDegreesToRadians(mWrongWayAngleThreshold));
	VuGameUtil::IF()->constantDB()["HUD"]["WrongWayInertiaTime"].getValue(mWrongWayInertiaTime);

	REG_EVENT_HANDLER(VuHumanDriver, OnStartLightSequence);
}
	
//*****************************************************************************
void VuHumanDriver::onGameInitialize()
{
	VuDriverEntity::onGameInitialize();

	mStartBoostPhase = BOOSTPHASE_NONE;
	mBoostAtStart = false;
	mBoostPressed = false;
}

//*****************************************************************************
void VuHumanDriver::onGameRelease()
{
	VuDriverEntity::onGameRelease();
}

//*****************************************************************************
void VuHumanDriver::onRecover()
{
	VuDriverEntity::onRecover();

	VuCarStats &stats = mpCar->getStats();

	VuAudioUtil::playSfx("Global/Vehicles/recover");

	mReachedRecoverSpeedThreshold = false;
}

//*****************************************************************************
void VuHumanDriver::onTickDecision(float fdt)
{
	VuDriverEntity::onTickDecision(fdt);

	if ( mAnyPad )
		mPadIndex = VuControlMethodManager::IF()->recentPadIndex();

	if ( fdt > 0.0f )
	{
		if ( mAutoDrive )
		{
			mCameraRoll = VuLerp(mCameraRoll, 0.0f, fdt);
			mpCar->getCamera()->control(mCameraRoll, false);
		}
		else
		{
			updateControls(fdt);
		}

		updateWrongWay(fdt);
	}


	// update recovery threshold
	if ( !mReachedRecoverSpeedThreshold )
	{
		float speedMPH = VuMetersPerSecondToMph(mpCar->getLinearVelocity().mag());
		if ( speedMPH > mRecoverSpeedThreshold )
			mReachedRecoverSpeedThreshold = true;
	}
	
	// update current game/real sector
	VuVector3 pos = mpCar->getModelMatrix().getTrans();

	if ( !mpCurrentGameSector || !mpCurrentGameSector->pointInside(pos) )
		mpCurrentGameSector = VuTrackManager::IF()->findSectorFromPosition(pos, mpCar->getStats().mCurrentSection);

	if ( !mpCurrentRealSector || !mpCurrentRealSector->pointInside(pos) )
		mpCurrentRealSector = VuTrackManager::IF()->findSectorFromPosition(pos);

	// update stats
	VuCarStats &stats = mpCar->getStats();
	bool wrongWay = false;
	bool outOfBounds = false;
	if ( mpCar->isOutOfBounds() )
		outOfBounds = true;
	else if ( headingWrongWay() )
		wrongWay = true;

	stats.mWrongWayAlpha = VuClamp(stats.mWrongWayAlpha + (wrongWay ? 1.0f : -1.0f)*HUD_TEXT_ALPHA_RATE*fdt, 0.0f, 1.0f);
	stats.mOutOfBoundsAlpha = VuClamp(stats.mOutOfBoundsAlpha + (outOfBounds ? 1.0f : -1.0f)*HUD_TEXT_ALPHA_RATE*fdt, 0.0f, 1.0f);

	stats.mUsedBrake |= (mpCar->getThrottleControl() < -FLT_EPSILON) & (!mpCar->isControlPaused()) & (!isAutoDrive());

	if (!mBoostPressed && mStartBoostPhase != BOOSTPHASE_NONE)
	{
		mBoostPressed = VuInputManager::IF()->getButtonWasPressed(mPadIndex, "PowerUp");

		if (mBoostPressed)
		{
			if (mStartBoostPhase == BOOSTPHASE_BOOSTABLE)
			{
				// Nailed it
				mBoostAtStart = true;
			}
		}
	}
}

//*****************************************************************************
void VuHumanDriver::onTeleported()
{
	mpCurrentGameSector = mpCurrentRealSector = VuTrackManager::IF()->findSectorFromPosition(mpCar->getModelPosition());

	VuCarStats &stats = mpCar->getStats();
	stats.mCurrentSection = mpCurrentGameSector->mSection;
	stats.mpNextCheckPoint = VuTrackManager::IF()->getNextCheckPoint(mpCurrentGameSector->mSection);
}

//*****************************************************************************
void VuHumanDriver::updateControls(float fdt)
{
	VuCarStats &stats = mpCar->getStats();

	float throttleControl = 0.0f, yawControl = 0.0f, cameraRoll = 0.0f;
	bool bPowerUp = false;

// modify by xlm,[/2015/4/1 ] 
	bool bExtraPowerUp = false;

	// auto accelerate
	bool bAutoAccelerate = true;
	if ( VuControlMethodManager::IF()->isGamePad() )
	{
		bAutoAccelerate = VuSettingsManager::IF()->getAutoThrottleGamePad();

		VuGamePad::eDeviceType deviceType = VuGamePad::IF()->getController(mPadIndex).mDeviceType;
		if ( deviceType == VuGamePad::DEVICE_SIMPLE_GAMEPAD || deviceType == VuGamePad::DEVICE_REMOTE_CONTROL )
			bAutoAccelerate = true;
	}
	else if ( VuControlMethodManager::IF()->isKeyboard() )
	{
		bAutoAccelerate = VuSettingsManager::IF()->getAutoThrottleKeyboard();
	}

	if ( bAutoAccelerate )
	{
		throttleControl = 1.0f - 2.0f*VuInputManager::IF()->getAxisValue(mPadIndex, "Brake");
	}
	else
	{
		throttleControl = VuInputManager::IF()->getAxisValue(mPadIndex, "Throttle") - VuInputManager::IF()->getAxisValue(mPadIndex, "Brake");
	}
	throttleControl = VuClamp(throttleControl, -1.0f, 1.0f);

	yawControl = VuInputManager::IF()->getAxisValue(mPadIndex, "Steering");
	yawControl = VuClamp(yawControl, -1.0f, 1.0f);
	yawControl *= VuAbs(yawControl);

	bPowerUp = VuInputManager::IF()->getButtonWasPressed(mPadIndex, "PowerUp");

	bExtraPowerUp = VuInputManager::IF()->getButtonWasPressed(mPadIndex, "PowerUpExtra1");




	cameraRoll = yawControl*VuDegreesToRadians(mGamePadCameraTilt);

	if ( VuControlMethodManager::IF()->isTilt() )
	{
		yawControl = 0.0f;
		cameraRoll = 0.0f;

		// accelerometer
		VuVector3 accelerometer;
		if ( VuAccel::IF()->getAccel(accelerometer) )
		{
			float sensitivity = VuSettingsManager::IF()->getSteeringSensitivity();
			float extent = VuDegreesToRadians(VuLerp(mSteeringExtent0, mSteeringExtent1, sensitivity));
			float accelAngle = VuASin(VuClamp(accelerometer.mX, -1.0f, 1.0f));

			yawControl = VuClamp(accelAngle/extent, -1.0f, 1.0f);

			cameraRoll = VuClamp(accelAngle, -extent, extent);
		}
	}

	if ( isRagdollActive() )
	{
		throttleControl = 0.0f;
		yawControl = 0.0f;
	}

	if ( bPowerUp && !mpCar->canUsePowerUp() )
		bPowerUp = false;

	//xlm
	if (bExtraPowerUp && !mpCar->canUsePowerUp())
	{
		bExtraPowerUp = false;
	}

	// power sliding?
	bool bPowerSlide = false;
	if ( mPowerSlideDetection )
	{
		bool bPowerSlideDown = VuInputManager::IF()->getButtonValue(mPadIndex, "PowerSlide");

		if ( bPowerSlideDown )
		{
			mPowerSlideBrakeTimer += fdt;
		}
		else
		{
			mPowerSlideDetection = false;
			if ( mPowerSlideBrakeTimer < 0.5f )
				bPowerSlide = true;
		}
	}
	else
	{
		mPowerSlideDetection = VuInputManager::IF()->getButtonWasPressed(mPadIndex, "PowerSlide");

		mPowerSlideBrakeTimer = 0.0f;
	}

	// control car
	mpCar->setThrottleControl(throttleControl);
	mpCar->setYawControl(yawControl);
	mpCar->setPowerUpControl(bPowerUp);
	mpCar->setPowerSlideControl(bPowerSlide);

	//xlm
	mpCar->setExtraPowerUpControl1(bExtraPowerUp);

	


	// control camera
	if ( mpCar->getHasFinished() || !VuSettingsManager::IF()->getCameraTilt() )
		mCameraRoll = VuLerp(mCameraRoll, 0.0f, fdt);
	else
		mCameraRoll = VuLerp(mCameraRoll, cameraRoll, VuMin(mAccelerometerCameraDamping*fdt, 1.0f));
	mpCar->getCamera()->control(mCameraRoll, false);

	// use ability
	if ( !stats.mCharacterAbilityUsed && (stats.mCharacterAbilityCharge >= 1.0f) )
	{
		if ( VuInputManager::IF()->getButtonWasPressed(mPadIndex, "UseAbility") && !mpCar->getPauseControlCount() && mpCar->canUsePowerUp() )
		{
			mpCar->getEffectController()->applyEffect(mpCar->getDriverName().c_str(), VUNULL);

			stats.mCharacterAbilityCharge = 0.0f;
			stats.mCharacterAbilityUsed = true;
		}
	}
}

//*****************************************************************************
void VuHumanDriver::updateWrongWay(float fdt)
{
	// state
	bool headingWrongWay = false;
	if ( mpCurrentGameSector && !mpCar->getHasFinished() && !isRagdollActive() )
	{
		const VuVector3 &sectorDir = mpCurrentGameSector->mUnitDir;
		const VuVector3 &linVel = mpCar->getLinearVelocity();
		float speed = linVel.mag();

		if ( speed > mWrongWaySpeedThreshold )
		{
			float cosAngle = VuDot(linVel, sectorDir)/speed;
			if ( cosAngle < mWrongWayAngleThreshold )
			{
				cosAngle = VuDot(mpCar->getModelMatrix().getAxisY(), sectorDir);
				if ( cosAngle < 0.0f )
					headingWrongWay = true;
			}
		}
	}

	// inertia
	if ( headingWrongWay )
		mWrongWayInertia += fdt;
	else
		mWrongWayInertia -= fdt;
	mWrongWayInertia = VuClamp(mWrongWayInertia, 0.0f, mWrongWayInertiaTime);

	if ( mHeadingWrongWay )
	{
		if ( mWrongWayInertia <= 0 )
			mHeadingWrongWay = false;
	}
	else
	{
		if ( mWrongWayInertia >= mWrongWayInertiaTime )
			mHeadingWrongWay = true;
	}
}

//*****************************************************************************
void VuHumanDriver::OnStartLightSequence(const VuParams& params)
{
	VuParams::VuAccessor accessor(params);

	const char* pEventType = accessor.getString();
	if (pEventType)
	{
		if (!strcmp(pEventType, "3"))
		{
			mStartBoostPhase = BOOSTPHASE_PREBOOST;
		}
		else if (!strcmp(pEventType, "Boostable"))
		{
			// Check for a PowerUp button press and apply boost
			mStartBoostPhase = BOOSTPHASE_BOOSTABLE;
		}
		else if (!strcmp(pEventType, "Start"))
		{
			// Too late!
			mStartBoostPhase = BOOSTPHASE_POSTBOOST;
		}
		else if (!strcmp(pEventType, "BoostPoint"))
		{
			if (mBoostAtStart)
			{
				// Must be holding throttle to get Boost
				if (mpCar->getRawThrottleControl() > 0.5f)
				{
					mpCar->getEffectController()->applyEffect("BoostStart", mpCar);
					VuAchievementManager::IF()->unlock(44); // Do a boost start
				}

				mBoostAtStart = false;
			}

			mBoostPressed = false;
			mStartBoostPhase = BOOSTPHASE_NONE;
		}
		else
		{
			mStartBoostPhase = BOOSTPHASE_NONE;
		}
	}
}
