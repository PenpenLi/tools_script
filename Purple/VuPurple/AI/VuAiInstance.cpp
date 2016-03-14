//*****************************************************************************
//
//  Copyright (c) 2007-2008 Ralf Knoesel
//  Copyright (c) 2008-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  AI Ray Test Classes
// 
//*****************************************************************************

#include "VuEngine/Json/VuJsonContainer.h"
#include "VuEngine/Dev/VuDevConfig.h"
#include "VuEngine/Dev/VuDevProfile.h"
#include "VuEngine/Math/VuRand.h"

#include "VuPurple/AI/VuAiInstance.h"
#include "VuPurple/Track/VuTrackManager.h"
#include "VuPurple/Track/VuTrackPlan.h"
#include "VuPurple/Track/VuTrackSector.h"
#include "VuPurple/Track/VuTrackUtil.h"
#include "VuPurple/Entities/Objects/VuAiWaypointEntity.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Driver/VuAiDriverEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarEngine.h"
#include "VuPurple/Entities/Car/Parts/VuCarChassis.h"
#include "VuPurple/Entities/Car/Parts/VuCarEffectController.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuPurple/PowerUps/VuPowerUpmanager.h"

#include "VuPurple/AI/VuAiUtils.h"
#include "VuPurple/AI/VuAiDriverHint.h"
#include "VuPurple/AI/VuAiTuningVariables.h"
#include "VuPurple/AI/VuAiBrain.h"
#include "VuPurple/AI/VuAiBehavior.h"
#include "VuPurple/AI/VuAiManager.h"

#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Managers/VuCarManager.h"

#define MAX_PATH_CORRECTION_DEPTH	8
#define MIN_CATCHUP_SPEED_HINT		VuMphToMetersPerSecond(100.0f)
#define MAX_CATCHUP_SPEED_HINT		VuMphToMetersPerSecond(200.0f)

//*****************************************************************************
VuAiInstance::VuAiInstance()
: mpDriver(VUNULL)
, mpCar(VUNULL)
, mpCompetitor(VUNULL)
//, mpCurrentSector(VUNULL)
, mTimeSpentWedged(0.0f)
, mSimulatedTraversalAmt(0.0f)
, mConfiguredTopSpeed(0.0f)
, mConfiguredTime(0.0f)
, mInitialLapCount(0)
, mpInitialCheckPoint(VUNULL)
, mCurrentSpeedHint(0.0f)
, mSpeedSampleCount(0)
, mCompetitorTimer(0.0f)
, mAverageSpeed(0.0f)
, mpBrain(VUNULL)
, mIsSkidding(false)
, mIsChallenge(false)
, mRaceCompletionPercent(0.0f)
, mCurRaceProgress(PROGRESS_EARLY_RACE)
, mCurLane(LANE_CENTER)
, mUsePowerUps(true)
, mLastBoostTime(0.0f)
, mLastSpikedTiresTime(0.0f)
, mLastPowerslideTime(0.0f)
, mLastThrottleDownTime(0.0f)
, mLastCharAbilityTime(0.0f)
, mLastPowerUpTime(0.0f)
{
	if (VuDevConfig::IF()->hasParam("AITestMode"))
	{
		const VuJsonContainer& testModeData = VuDevConfig::IF()->getParam("AITestMode");
		if (testModeData.hasMember("UsePowerUps"))
		{
			mUsePowerUps = testModeData["UsePowerUps"].asBool();
		}
	}
}

//*****************************************************************************
VuAiInstance::~VuAiInstance()
{
	for (int i = 0; i < PACK_MAX; i++)
	{
		mCarPacks[i].clear();
	}
}

//*****************************************************************************
void VuAiInstance::init(VuAiDriver* pDriver, VuCarEntity* pCar)
{
	mpCar = pCar;
	mpDriver = pDriver;

	// Tell the driver what AI instance it has
	if (mpDriver)
	{
		mpDriver->setAiInstance(this);
		mpDriver->setSkill(getAiTuningVariables()->mAiSkill);

		if (getCar()->getCarStage() == 0)
		{
			// Class A cars never shortcut no matter what
			mpDriver->getPlan().setPlanType(VuTrackPlan::PLAN_MAIN_TRACK);
		}
		else
		{
			switch (mpDriver->getSkill())
			{
			case SKILL_BAD:	mpDriver->getPlan().setPlanType(VuTrackPlan::PLAN_MAIN_TRACK);	break;
			case SKILL_NORMAL: mpDriver->getPlan().setPlanType(VuTrackPlan::PLAN_RANDOM); break;
			case SKILL_GOOD: mpDriver->getPlan().setPlanType(VuTrackPlan::PLAN_RANDOM_SHORTCUTS);	break;
			}
		}
	}

	// Initialize the brain, giving it the ability to override track plan if necessary
	mpBrain = VuAiBrainFactory::IF()->create(getAiTuningVariables()->mAiBrainName.c_str());
	if (mpBrain)
	{
		mpBrain->init(this);
	}

	mCurRaceProgress = getRaceProgress();

	mIsChallenge = VuGameUtil::IF()->getEventData()["IsChallenge"].asBool();

	mGameType = VuGameUtil::IF()->getGameType();

	// FIXME: need to get this from the Driver, as it will be stored there shortly
	//
	std::string myOpponentName = "Default";

	// Now modify the performance characteristics based on OpponentDB data
	const VuJsonContainer& data = VuGameUtil::IF()->aiPersonalityDB()[getAiTuningVariables()->mAiPersonalityName];

	if (data != VuJsonContainer::null)
	{
		if (data.hasMember("BuffPercent") && getAiTuningVariables()->mBuffPercent != 100.0f)
		{
			float multiplier = (getAiTuningVariables()->mBuffPercent / 100.0f);

			mpCar->setAiBuffAccel(multiplier);
			mpCar->setAiBuffHandling(multiplier);
			mpCar->setAiBuffSpeed(multiplier);
			mpCar->setAiBuffTough(multiplier);
		}
		else if (data.hasMember("Performance"))
		{
			mpCar->setAiBuffAccel(getAiTuningVariables()->mAccelerationMultiplier);
			mpCar->setAiBuffHandling(getAiTuningVariables()->mHandlingMultiplier);
			mpCar->setAiBuffSpeed(getAiTuningVariables()->mTopSpeedMultiplier);
			mpCar->setAiBuffTough(getAiTuningVariables()->mToughnessMultiplier);
		}

		// Override Chassis air control value (0.0 to 1.0)
		mpCar->getChassis()->setAirControlMultiplier(getAiTuningVariables()->mAirSteerMultiplier);

		if (getAiTuningVariables()->mAirSteeringSpeedOverride > FLT_EPSILON)
		{
			mpCar->getChassis()->setAirSteeringSpeed(getAiTuningVariables()->mAirSteeringSpeedOverride);
		}

		// Never start with a timer set to expire at less than 10 seconds from now.
		float minimumDelay = 10.0f;

		mLastBoostTime = (float)VuSys::IF()->getTime() - VuRand::global().range(1.0f, getAiTuningVariables()->mAiBoostFrequency) + minimumDelay;
		mLastSpikedTiresTime = (float)VuSys::IF()->getTime() - VuRand::global().range(1.0f, getAiTuningVariables()->mAiSpikedTiresFrequency) + minimumDelay;
		mLastPowerslideTime = (float)VuSys::IF()->getTime() - VuRand::global().range(1.0f, getAiTuningVariables()->mAiPowerslideFrequency) + minimumDelay;
		mLastThrottleDownTime = (float)VuSys::IF()->getTime() - VuRand::global().range(1.0f, getAiTuningVariables()->mAiThrottleDownFrequency) + minimumDelay;
		mLastCharAbilityTime = (float)VuSys::IF()->getTime() + minimumDelay;
		mLastPowerUpTime = (float)VuSys::IF()->getTime() + minimumDelay;
	}
}

//*****************************************************************************
void VuAiInstance::release()
{
	if (mpBrain)
	{
		mpBrain->release();

		VuAiBrainFactory::IF()->destroy(mpBrain);
	}

	mpBrain = VUNULL;
}

//*****************************************************************************
void VuAiInstance::preConfigure(float finishTime, float topSpeedMPH)
{
	// if the finish time is 0, this car will be placed in the level
	// evenly spaced between the start of the track and the last car
	// that had a finish time specified for it, then a configured time
	// will be figured out from there for it automatically
	mConfiguredTime = finishTime;

	mConfiguredTopSpeed = mpCar->getMaxForwardSpeed();
	if (topSpeedMPH > 0.0f)
	{
		mConfiguredTopSpeed = VuMin(VuMphToMetersPerSecond(topSpeedMPH), mConfiguredTopSpeed);
	}

	mCurrentSpeedHint = mConfiguredTopSpeed;
}

//*****************************************************************************
void VuAiInstance::think()
{
	if (mpBrain)
	{
		mpBrain->think();
	}
}

//*****************************************************************************
void VuAiInstance::checkForSkidding()
{
	// See if we're skidding
	int tiresSkidding = 0;
	for (int wheelNum = VuCarWheel::REAR_LEFT; wheelNum <= VuCarWheel::REAR_RIGHT; wheelNum++)
	{
		const VuCarWheel& wheel = getCar()->getWheel(wheelNum);

		if (wheel.mBrakeSlide || wheel.mEngineSlide || wheel.mSideSlide)
		{
			tiresSkidding += 1;
		}
	}

	// If we're skidding, slow down even more until we've got traction
	if (tiresSkidding == 2)
	{
		mIsSkidding = true;
	}
	else
	{
		mIsSkidding = false;
	}
}

//*****************************************************************************
void VuAiInstance::checkRaceProgress()
{
	// Check to see race progress
	eAiRaceProgress newProgress = getRaceProgress();

	// See if race has progressed
	if (newProgress > mCurRaceProgress)
	{
		// Race has progressed, reload or re-init anything that's phase-specific
		//

		if (mpBrain)
		{
			// Load the new Brain for this race phase
			//
			std::string curBrainName = mpBrain->getAiBrainName();
			std::string newBrainName = getAiTuningVariables()->mAiBrainName;

			if (curBrainName != newBrainName)
			{
				// Deallocate the old brain
				VuAiBrainFactory::IF()->destroy(mpBrain);

				// Allocate the new brain
				mpBrain = VuAiBrainFactory::IF()->create(newBrainName);
				if (mpBrain)
				{
					mpBrain->init(this);
				}
			}
		}
		// See if anything else needs to change


		// Update progress tracking var
		mCurRaceProgress = newProgress;
	}
}

//*****************************************************************************
void VuAiInstance::tickDecision(float fdt)
{
	VU_PROFILE_SIM("AI");

	// Remaining meters of track
	updateRaceCompletionPercent();

	checkForSkidding();

	checkRaceProgress();

	mCurLane = getLane();

	if (mpBrain)
	{
		mpBrain->tickBehaviors(fdt);
	}

	// See if the car is stuck
	if (VuAiManager::IF()->isWedgeDetectionEnabled())
	{
		doWedgeDetection(fdt);
	}

	computeAverageSpeed();

	VuAiDriverHints hints;

	hints.mSpeed = mCurrentSpeedHint;
	
	getAiDriver()->updateHints(hints);

	applySpeedHint();

	// Execute thinking cycle on Brain/Behaviors
	// (re-evaluates and scores behaviors but does not tick them)
	//
	think();
}

//*****************************************************************************
// filterPowerupUsage
//
// Called when powerups are used by the AI
//
// This kind of blows, but we're close enough to ship date for me to prefer 
// filtered catch-all cases over over systemic changes to well-tested code 
//
const char* VuAiInstance::filterPowerupUsage(const char* pName)
{
	std::string filterName = "DeathBat";

	if (pName == filterName)
	{
		bool canDeathBatAgain = VuSys::IF()->getTime() > VuAiManager::IF()->getLastDeathBatTime() + VuAiManager::IF()->getGlobalConstants()->mDeathBatFrequency;

		VuCarEntity*	pLeadCar = VUNULL;
		VUINT			leadCarPlace = 999;

		for (int i = 0; i < VuCarManager::IF()->getCarCount(); i++)
		{
			VuCarEntity* pCar = VuCarManager::IF()->getCar(i);

			// Ignore finished cars
			if (pCar->getHasFinished())
				continue;

			if (pCar->getStats().mPlace < leadCarPlace)
			{
				pLeadCar = pCar;
				leadCarPlace = pLeadCar->getStats().mPlace;
			}
		}

		VuCarEntity* pHumanCar = VUNULL;
		if (VuCarManager::IF()->getLocalHumanCarCount() > 0)
		{
			pHumanCar = VuCarManager::IF()->getLocalHumanCar(0);
		}


		bool humanLeadCar = (pHumanCar != VUNULL && pHumanCar == pLeadCar);
		if (humanLeadCar && !canDeathBatAgain)
		{
			pName = "PoliceChase";

			return pName;
		}

		VuAiManager::IF()->setLastDeathBatTime(static_cast<float>(VuSys::IF()->getTime()));
	}

	return pName;
}

//*****************************************************************************
void VuAiInstance::usePowerup(const char* pName)
{
	pName = filterPowerupUsage(pName);

	const VuPowerUp* pPowerUp = VuPowerUpManager::IF()->getPowerUpByName(pName);

	if (pPowerUp)
	{
		VuAiUtils::reportEvent(this, "UsedPowerUp", pName);

		getCar()->getEffectController()->applyEffect(pName, VUNULL);

		VUPRINTF("Status: %s in %s (%s) used PowerUp '%s'.\n", getCar()->getDriverName().c_str(), getCar()->getCarName().c_str(), getCar()->getOpponentName().c_str(), pName);
	}
}

//*****************************************************************************
void VuAiInstance::computeAverageSpeed()
{
	// Track the average speed of the racer on the track
	float avgSpeed;

	mSpeedSamples[mSpeedSampleCount % AVERAGE_SPEED_COUNT] = mpDriver->getCurrentSpeed();
	mSpeedSampleCount++;

	avgSpeed = 0.0f;
	int sampleCount = VuMin(mSpeedSampleCount, AVERAGE_SPEED_COUNT);
	for (int iSample = 0; iSample < sampleCount; iSample++)
	{
		avgSpeed += mSpeedSamples[iSample];
	}

	avgSpeed /= sampleCount;

	mAverageSpeed =  avgSpeed;
}

//*****************************************************************************
void VuAiInstance::applySpeedHint()
{
	// By default, the speed hint is the configured top speed...
	mCurrentSpeedHint = mConfiguredTopSpeed;

	VuVector3 vPos = VuVector3(0.0f, 0.0f, 0.0f);
	VuVector3 vCurPos = getCar()->getModelPosition();

	if (getAiDriver()->getCurrentSector())
	{
		// See if the Driver is ahead of or behind the pace needed to hit the 
		// configured time
		if (mConfiguredTime > 0.0f)
		{
			float distanceLeftEstimate = VuTrackUtil::estimatedTrackLeft(vCurPos, getAiDriver()->getCurrentSector(), VuAiManager::IF()->getLastRaceSector());
			float timeLeftEstimate = distanceLeftEstimate / mAverageSpeed;

			if (VuAiManager::IF()->getRaceTimeElapsed() > mConfiguredTime)
			{
				mCurrentSpeedHint = MAX_CATCHUP_SPEED_HINT;
			}
			else if (VuAiManager::IF()->getRaceTimeElapsed() + timeLeftEstimate > mConfiguredTime)
			{
				mCurrentSpeedHint = VuClamp(distanceLeftEstimate / (mConfiguredTime - VuAiManager::IF()->getRaceTimeElapsed()), MIN_CATCHUP_SPEED_HINT, MAX_CATCHUP_SPEED_HINT);
			}
		}

		// The track may hint at a speed, if it does, and it is lower than 
		// our calculated speed hint, use that instead
		float trackSpeedHint = getAiDriver()->getCurrentSector()->calcSpeedHint(getAiDriver()->getCurrentSector()->traversalAmount(vCurPos), mCurrentSpeedHint);

		if (trackSpeedHint < mCurrentSpeedHint)
		{
			mCurrentSpeedHint = VuMin(mConfiguredTopSpeed, trackSpeedHint);
		}
	}
}

bool VuAiInstance::hasSpeedHint()
{
	if (mCurrentSpeedHint != 0 && mCurrentSpeedHint != mConfiguredTopSpeed)
	{
		return true;
	}

	return mpDriver->getPlan().hasSpeedHint();
}


//*****************************************************************************
void VuAiInstance::doWedgeDetection(float fdt)
{
	VuTransformComponent *pCarTransform = mpCar->getTransformComponent();

	const VuCarEntity *pCar = mpCar;

	VuVector3 vPos = VuVector3(0, pCar->getCollisionAabb().mMax.mY, pCar->getCollisionAabb().getCenter().mZ);
	VuVector3 vPrevPos = getAiDriver()->getPreviousModelMatrix().transform(vPos);
	VuVector3 vCurPos = pCar->getModelMatrix().transform(vPos);

	// "Wedged" detection
	{
		VuVector3 vDeltaPos = (vCurPos - vPrevPos);

		if (vDeltaPos.magSquared() < (fdt * VuAiManager::IF()->getWedgedDeltaThreshold() * VuAiManager::IF()->getWedgedDeltaThreshold()))
		{
			mTimeSpentWedged += fdt;
		}
		else
		{
			mTimeSpentWedged = 0.0f;
		}

		if (mTimeSpentWedged > VuAiManager::IF()->getTimeSpentWedgedThreshold())
		{
			if (VuAiManager::IF()->isRecordDebugEvents())
			{
				VuVector3 debugPos = pCar->getModelMatrix().getTrans();
				VuJsonContainer &debugEvent = VuAiManager::IF()->addDebugEvent(mpDriver->getName(), "Wedged", &debugPos);
			}

			VuVector3 pos;

			if (getAiDriver()->getCurrentSector())
			{
				getAiDriver()->getCurrentSector()->closestPointAlongRacingLine(vCurPos, pos);
			}
			else
			{
				return;
			}

			const VuVector3 &fwd = getAiDriver()->getCurrentSector()->mUnitDir;

			VuMatrix newMat;

			VuGameUtil::buildSafeMatrixForCar(pos, fwd, mpCar->getCollisionAabb(), newMat, true);

			pCarTransform->setWorldTransform(newMat);
			mTimeSpentWedged = 0.0f;

			mpDriver->replan(getAiDriver()->getCurrentSector());
		}
	}
}

//*****************************************************************************
float VuAiInstance::updateRaceCompletionPercent()
{
	VuCarStats *pStats = &getCar()->getStats();

	float totalLength = VuTrackManager::IF()->getLength() * pStats->mLapCount;
	float remainingTrack = pStats->mDistFromStart;

	mRaceCompletionPercent = (remainingTrack / totalLength) * 100.0f;

	mRaceCompletionPercent = VuClamp(mRaceCompletionPercent, 0.0f, 100.0f);

//	VUPRINTF("Completion: %%%.2f\n", mRaceCompletionPercent);

	return mRaceCompletionPercent;
}

//*****************************************************************************
eAiCarPack VuAiInstance::getCarPack(const VuCarEntity* pCar)
{
	for (int i = 0; i < PACK_MAX; i++)
	{
		if (mCarPacks[i].end() != std::find(mCarPacks[i].begin(), mCarPacks[i].end(), pCar))
		{
			return (eAiCarPack)i;
		}
	}

	return PACK_MAX;
}

//*****************************************************************************
eAiRaceProgress VuAiInstance::getRaceProgress()
{
	if (mRaceCompletionPercent < VuAiManager::IF()->getGlobalConstants()->mEarlyRaceCutoffPercent)
	{
		return PROGRESS_EARLY_RACE;
	}
	else if (mRaceCompletionPercent < VuAiManager::IF()->getGlobalConstants()->mMidRaceCutoffPercent)
	{
		return PROGRESS_MID_RACE;
	}
	else
	{
		return PROGRESS_LATE_RACE;
	}
}

//*****************************************************************************
float VuAiInstance::getLeadOverRacer()
{
	// The human/racer car is always the last car.
	VuCarEntity *pRacerCar = VuCarManager::IF()->getCar(VuCarManager::IF()->getCarCount() - 1);
	if (!pRacerCar)
	{
		return 0.0f;
	}

	float racerDist = pRacerCar->getStats().mDistFromStart;
	float myDist = getCar()->getStats().mDistFromStart;

	float difference = myDist - racerDist;

	return difference;
}

//*****************************************************************************

eAiLane VuAiInstance::getLane()
{
	return LANE_CENTER;

// Not used in BBR
//	return VuAiUtils::getLaneFromPos(getCar()->getModelMatrix().getTrans());
}

//*****************************************************************************
VuAiTuningVariables* VuAiInstance::getAiTuningVariables(bool getRawData) 
{
	return mpDriver->getAiTuningVariables(getRawData); 
}
