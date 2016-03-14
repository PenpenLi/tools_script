//*****************************************************************************
//
//  Copyright (c) 2007-2008 Ralf Knoesel
//  Copyright (c) 2008-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  AI Behavior Classes
// 
//*****************************************************************************

#include "VuEngine/Math/VuRand.h"
#include "VuPurple/Entities/Car/Driver/VuAiDriverEntity.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarPowerUpController.h"
#include "VuPurple/Entities/Car/Parts/VuCarEffectController.h"
#include "VuPurple/Entities/Car/Parts/VuCarWheel.h"
#include "VuPurple/PowerUps/VuPowerUpmanager.h"
#include "VuPurple/Managers/VuCarManager.h"

#include "VuPurple/Track/VuTrackManager.h"
#include "VuPurple/Track/VuTrackSector.h"

#include "VuPurple/AI/VuAiInstance.h"
#include "VuPurple/AI/VuAiBehavior.h"
#include "VuPurple/AI/VuAiUtils.h"

//*****************************************************************************
void VuAiBehavior::init(VuAiInstance* pInstance) 
{ 
	mpAiInstance = pInstance; 
	mScore = 0; 
	mIsExpired = false; 
	mIsDone = false;  
	mCountdownTimer = 0.0f; 
	mTuningWeight = 1.0f;
	mTimeInBehavior = 0.0f;

	BehaviorWeights& behaviorWeights = pInstance->getAiTuningVariables()->mBehaviorWeights;
	BehaviorWeights::iterator behaviorIterator = behaviorWeights.find(getClassName());
	if (behaviorIterator != behaviorWeights.end())
	{
		mTuningWeight = behaviorIterator->second;
	}

	PowerUpWeights& powerUpWeights = pInstance->getAiTuningVariables()->mPowerUpWeights;
	PowerUpWeights::iterator powerUpIterator = powerUpWeights.find(getName());
	if (powerUpIterator != powerUpWeights.end())
	{
		mTuningWeight = powerUpIterator->second;
	}

	VuRand rand;
	mReactionTime = mpAiInstance->getAiTuningVariables()->mReactionTime;

	// Random range from 75% to 100% of entered ReactionTime
	mReactionTime = (mReactionTime / 2.0f) + (mReactionTime / 4.0f) + rand.range(0.0f, mReactionTime / 4.0f);
}

//*****************************************************************************
void VuAiBehavior::startReacting()
{
	mIsReacting = true;
}

//*****************************************************************************
bool VuAiBehavior::tick(float fdt)
{
	if (mIsReacting)
	{
		mReactionTime -= fdt;
		if (mReactionTime <= 0.0f)
		{
			mReactionTime = 0.0f;
			mIsReacting = false;

			// Actually start the behavior now
			start();
		}

		// Behavior hasn't expired yet
		return true;
	}

	mTimeInBehavior += fdt;

	if (mCountdownTimer > 0.0f)
	{
		mCountdownTimer -= fdt;
		if (mCountdownTimer <= FLT_EPSILON)
		{
			mCountdownTimer = 0.0f;
			
			mIsExpired = true;

			return false;
		}
	}

	return !mIsExpired;
}

//*****************************************************************************
bool VuAiBehavior::start()
{
	mIsDone = false;  
	mIsExpired = false; 
	mCountdownTimer = 0.0f;
	mTimeInBehavior = 0.0f;

	return true; 
}

//*****************************************************************************
// Behavior Factory
//*****************************************************************************

IMPLEMENT_SYSTEM_COMPONENT(VuAiBehaviorFactory, VuAiBehaviorFactory);

VuAiBehaviorFactory::VuAiBehaviorFactory()
{
}

//*****************************************************************************
bool VuAiBehaviorFactory::init()
{
	//				 _PowerUp Name,_PowerUp Class_)
	REGISTER_BEHAVIOR(UsePowerUp,	UsePowerUp);

	REGISTER_BEHAVIOR(Firework,		PowerUpThrow);
	REGISTER_BEHAVIOR(Fireball,		PowerUpThrow);
	REGISTER_BEHAVIOR(Scattershot,	PowerUpThrow);
	REGISTER_BEHAVIOR(FreezeRay,	PowerUpThrow);
	REGISTER_BEHAVIOR(EarthStrike,	PowerUpThrow);
	REGISTER_BEHAVIOR(HomingMissile,PowerUpSeek);
	REGISTER_BEHAVIOR(Tornado,		PowerUpSeek);
	REGISTER_BEHAVIOR(Lightning,	PowerUpSeek);
	REGISTER_BEHAVIOR(RemoteControl,PowerUpSeek);
	REGISTER_BEHAVIOR(OilSlick,		PowerUpDropped);
	REGISTER_BEHAVIOR(ChickenCrate, PowerUpDropped);
	REGISTER_BEHAVIOR(MysteryCrate, PowerUpDropped);
	REGISTER_BEHAVIOR(BasicShield,	PowerUpShield);
	REGISTER_BEHAVIOR(Toughness,	PowerUpToughness);
	REGISTER_BEHAVIOR(Fake,			PowerUpDropped);
	REGISTER_BEHAVIOR(Spring,		PowerUpDropped);
	REGISTER_BEHAVIOR(LowGravity,	PowerUpGlobal);
	REGISTER_BEHAVIOR(BigTires,		PowerUpGlobal);
	REGISTER_BEHAVIOR(Confusion,	PowerUpGlobal);
	REGISTER_BEHAVIOR(NitroCar,		PowerUpGlobal);
	REGISTER_BEHAVIOR(BallChain,	PowerUpGlobal);
	REGISTER_BEHAVIOR(DeathBat,		PowerUpLongShot);
	REGISTER_BEHAVIOR(PoliceChase,	PowerUpLongShot);
	REGISTER_BEHAVIOR(Earthquake,	PowerUpLongShot);

	// Used by AI outside of the PowerUps
	REGISTER_BEHAVIOR(SpikedTires,	SpikedTires);
	REGISTER_BEHAVIOR(Boost,		Boost);
	REGISTER_BEHAVIOR(PowerSlide,	PowerSlide);

	// Character Abilities
	REGISTER_BEHAVIOR(CharAbility,	CharAbility);

	// Driving Control Behaviors
	REGISTER_BEHAVIOR(DriveLazy, 	DriveLazy);
	REGISTER_BEHAVIOR(LoseControl,	LoseControl);
	REGISTER_BEHAVIOR(BrakeForCorner, BrakeForCorner);
	REGISTER_BEHAVIOR(LaneChange,	LaneChange);
	REGISTER_BEHAVIOR(GetInFrontOfRacer, GetInFrontOfRacer);
	REGISTER_BEHAVIOR(GetInBackOfRacer, GetInBackOfRacer);
	REGISTER_BEHAVIOR(SlowDownForPack, SlowDownForPack);
	REGISTER_BEHAVIOR(SpeedUpForPack, SpeedUpForPack);
	REGISTER_BEHAVIOR(MaintainMaxLead, MaintainMaxLead);
	REGISTER_BEHAVIOR(HandlingBoost, HandlingBoost);

	return true;
}

//*****************************************************************************
VuAiBehavior* VuAiBehaviorFactory::create(const std::string& name)
{
	BehaviorCreateFunction pFunction = mBehaviors[name];

	if (pFunction == VUNULL)
	{
		return VUNULL;
	}

	VuAiBehavior* pNewBehavior = (*pFunction)();
	if (pNewBehavior)
	{
		pNewBehavior->setName(name.c_str());
	}

	// Add to tracking list for leak checking?

	return pNewBehavior;
}

//*****************************************************************************
void VuAiBehaviorFactory::destroy(VuAiBehavior* pBehavior)
{
	// May want to add tracking lists for leak checks/dangling behavior issues
	// if that becomes a problem

	if (pBehavior)
	{
		delete pBehavior;
	}
}

//*****************************************************************************
void VuAiBehaviorFactory::registerBehavior(const std::string& name, BehaviorCreateFunction function)
{
	mBehaviors[name] = function;
}

//*****************************************************************************
// UsePowerUp
//*****************************************************************************

IMPLEMENT_BEHAVIOR_CREATE(UsePowerUp);

//*****************************************************************************
float VuAiBehaviorUsePowerUp::score()
{
	mScore = mTuningWeight * 1.0f;
	return mScore;
}

//*****************************************************************************
bool VuAiBehaviorUsePowerUp::start()
{
	VuAiBehavior::start();

	mpAiInstance->usePowerup(mName.c_str());

	// Three seconds before using another powerup
	mCountdownTimer = 3.0f;

	return true;
}

//*****************************************************************************
// Boost
//*****************************************************************************

IMPLEMENT_BEHAVIOR_CREATE(Boost);

//*****************************************************************************
float VuAiBehaviorBoost::score()
{
	mScore = mTuningWeight * 1.0f;

	return mScore;
}

//*****************************************************************************
bool VuAiBehaviorBoost::start()
{
	VuAiBehavior::start();

	// See if we're currently boosting
	if (!mpAiInstance->getCar()->getEffectController()->isActive(VuCarEffectController::BOOST))
	{
		mpAiInstance->getCar()->getEffectController()->applyEffect("Boost", mpAiInstance->getCar());

		// TODO:
		// As a nod to fairness, this eats the current powerup
		// VuCarPowerUpController::Slot slot = mpAiInstance->getCar()->eatPowerUp();

		// 2 second delay before letting this Behavior expire
		mCountdownTimer = 1.0f;
	}
	else
	{
		mCountdownTimer = 0.1f;
	}

	return true;
}

//*****************************************************************************
// SpikedTires
//*****************************************************************************

IMPLEMENT_BEHAVIOR_CREATE(SpikedTires);

//*****************************************************************************
float VuAiBehaviorSpikedTires::score()
{
	mScore = mTuningWeight * 1.0f;
	
	return mScore;
}

//*****************************************************************************
bool VuAiBehaviorSpikedTires::start()
{
	VuAiBehavior::start();

	mpAiInstance->usePowerup(mName.c_str());

	mCountdownTimer = 3.0f;

	return true;
}

//*****************************************************************************
// SpikedTires
//*****************************************************************************

IMPLEMENT_BEHAVIOR_CREATE(PowerSlide);

//*****************************************************************************
float VuAiBehaviorPowerSlide::score()
{
	mScore = mTuningWeight * 1.0f;

	return mScore;
}

//*****************************************************************************
bool VuAiBehaviorPowerSlide::start()
{
	VuAiBehavior::start();

	mpAiInstance->getCar()->setPowerSlideControl(true);

	mCountdownTimer = 4.0f;

	return true;
}

//*****************************************************************************
// PowerUpThrow
//*****************************************************************************

IMPLEMENT_BEHAVIOR_CREATE(PowerUpThrow);

//*****************************************************************************
float VuAiBehaviorPowerUpThrow::score()
{
	mScore = mpAiInstance->getAiTuningVariables()->mAggressiveness * mTuningWeight * 1.0f;

	return mScore;
}

//*****************************************************************************
bool VuAiBehaviorPowerUpThrow::start()
{
	VuAiBehavior::start();

	mpAiInstance->usePowerup(mName.c_str());

	mCountdownTimer = 0.1f;

	return true;
}

//*****************************************************************************
bool VuAiBehaviorPowerUpThrow::tick(float fdt)
{
	// Required for timer maintenance
	if (!VuAiBehavior::tick(fdt))
	{
		return false;
	}

	if (mIsReacting)
	{
		VuCarEntity* pHim = VUNULL;

		// See if any AI cars are in front by a little bit (shootable with unguided)
		//
		for (int i = 0; i < VuCarManager::IF()->getAiCarCount(); i++)
		{
			pHim = VuCarManager::IF()->getAiCar(i);
			if (pHim)
			{
				float dist = VuAiUtils::raceDistanceBetween(mpAiInstance->getCar(), pHim);
				if (dist > 0.0f && dist < 80.0f)
				{
					mReactionTime = 0.0f;
					mIsReacting = false;

					// Actually start the behavior now
					start();

					return true;
				}
			}
		}
	}

	return true;
}

//*****************************************************************************
// VuAiBehaviorPowerUpSeek
//*****************************************************************************

IMPLEMENT_BEHAVIOR_CREATE(PowerUpSeek);

//*****************************************************************************
float VuAiBehaviorPowerUpSeek::score()
{
	mScore = mpAiInstance->getAiTuningVariables()->mAggressiveness * mTuningWeight * 1.0f;

	return mScore;
}

//*****************************************************************************
bool VuAiBehaviorPowerUpSeek::start()
{
	VuAiBehavior::start();

	mpAiInstance->usePowerup(mName.c_str());

	mCountdownTimer = 4.0f;

	return true;
}

//*****************************************************************************
bool VuAiBehaviorPowerUpSeek::tick(float fdt)
{
	// Required for timer maintenance
	if (!VuAiBehavior::tick(fdt))
	{
		return false;
	}

	if (mIsReacting)
	{
		VuCarEntity* pHim = VUNULL;

		// See if any AI cars are in front by a little bit (shootable with unguided)
		//
		for (int i = 0; i < VuCarManager::IF()->getAiCarCount(); i++)
		{
			pHim = VuCarManager::IF()->getAiCar(i);
			if (pHim)
			{
				float dist = VuAiUtils::raceDistanceBetween(mpAiInstance->getCar(), pHim);
				if (dist > 0.0f && dist < 200.0f)
				{
					mReactionTime = 0.0f;
					mIsReacting = false;

					// Actually start the behavior now
					start();

					return true;
				}
			}
		}
	}

	return true;
}

//*****************************************************************************
// VuAiBehaviorPowerUpDropped
//*****************************************************************************

IMPLEMENT_BEHAVIOR_CREATE(PowerUpDropped);

//*****************************************************************************
float VuAiBehaviorPowerUpDropped::score()
{
	mScore = mTuningWeight * 1.0f;
	
	return mScore;
}

//*****************************************************************************
bool VuAiBehaviorPowerUpDropped::start()
{
	VuAiBehavior::start();

	mpAiInstance->usePowerup(mName.c_str());

	mCountdownTimer = 4.0f;

	return true;
}

//*****************************************************************************
// VuAiBehaviorPowerUpGlobal
//*****************************************************************************

IMPLEMENT_BEHAVIOR_CREATE(PowerUpGlobal);

//*****************************************************************************
float VuAiBehaviorPowerUpGlobal::score()
{
	mScore = mpAiInstance->getAiTuningVariables()->mAggressiveness * mTuningWeight * 1.0f;
	return mScore;
}

//*****************************************************************************
bool VuAiBehaviorPowerUpGlobal::start()
{
	VuAiBehavior::start();

	mpAiInstance->usePowerup(mName.c_str());

	mCountdownTimer = 4.0f;

	return true;
}

//*****************************************************************************
// VuAiBehaviorPowerUpLongShot
//*****************************************************************************

IMPLEMENT_BEHAVIOR_CREATE(PowerUpLongShot);

//*****************************************************************************
float VuAiBehaviorPowerUpLongShot::score()
{
	mScore = mpAiInstance->getAiTuningVariables()->mAggressiveness * mTuningWeight * 1.0f;
	return mScore;
}

//*****************************************************************************
bool VuAiBehaviorPowerUpLongShot::start()
{
	VuAiBehavior::start();

	mpAiInstance->usePowerup(mName.c_str());

	mCountdownTimer = 4.0f;

	return true;
}

//*****************************************************************************
// VuAiBehaviorPowerUpShield
//*****************************************************************************

IMPLEMENT_BEHAVIOR_CREATE(PowerUpShield);

//*****************************************************************************
float VuAiBehaviorPowerUpShield::score()
{
	mScore = mTuningWeight * 1.0f;

	return mScore;
}

//*****************************************************************************
bool VuAiBehaviorPowerUpShield::start()
{
	VuAiBehavior::start();

	mpAiInstance->usePowerup(mName.c_str());

	mCountdownTimer = 4.0f;

	return true;
}

//*****************************************************************************
bool VuAiBehaviorPowerUpShield::tick(float fdt)
{
	// Required for timer maintenance
	if (!VuAiBehavior::tick(fdt))
	{
		return false;
	}

	if (mIsReacting)
	{
		VuCarEntity* pHim = VUNULL;

		// See if any AI cars are in back by a little bit
		//
		for (int i = 0; i < VuCarManager::IF()->getAiCarCount(); i++)
		{
			pHim = VuCarManager::IF()->getAiCar(i);
			if (pHim)
			{
				float dist = VuAiUtils::raceDistanceBetween(mpAiInstance->getCar(), pHim);
				if (dist < 0.0f && dist > -60.0f)
				{
					mReactionTime = 0.0f;
					mIsReacting = false;

					// Actually start the behavior now
					start();

					return true;
				}
			}
		}
	}

	return true;
}

//*****************************************************************************
// VuAiBehaviorPowerUpToughness
//*****************************************************************************

IMPLEMENT_BEHAVIOR_CREATE(PowerUpToughness);

//*****************************************************************************
float VuAiBehaviorPowerUpToughness::score()
{
	mScore = mTuningWeight * 1.0f;
	
	return mScore;
}

//*****************************************************************************
bool VuAiBehaviorPowerUpToughness::start()
{
	VuAiBehavior::start();

	mpAiInstance->usePowerup(mName.c_str());

	mCountdownTimer = 4.0f;

	return true;
}
//*****************************************************************************

IMPLEMENT_BEHAVIOR_CREATE(CharAbility);

//*****************************************************************************
float VuAiBehaviorCharAbility::score()
{
	mScore = mTuningWeight * 2.0f;

	return mScore;
}

bool  VuAiBehaviorCharAbility::start()
{
	VuAiBehavior::start();

	mpAiInstance->getCar()->getEffectController()->applyEffect(mpAiInstance->getCar()->getDriverName().c_str(), VUNULL);

	mCountdownTimer = 5.0f;

	return true;
}

bool VuAiBehaviorCharAbility::tick(float fdt)
{
	// Required for timer maintenance
	if (!VuAiBehavior::tick(fdt))
	{
		return false;
	}

	return true;
}

//*****************************************************************************
// Driving Control Behaviors
//*****************************************************************************
//

IMPLEMENT_BEHAVIOR_CREATE(DriveLazy);

//*****************************************************************************
float VuAiBehaviorDriveLazy::score()
{
	mScore = mTuningWeight * 1.0f;
	return mScore;
}

//*****************************************************************************
bool  VuAiBehaviorDriveLazy::start()
{
	VuAiBehavior::start();

	// Set speed controls? Randomized?

	// Drive at this speed for this long
	mCountdownTimer = 4.0f;

	mpAiInstance->getAiDriver()->setGoalLineToFollow(VuAiDriver::GOAL_LINE_AI);

	return true;
}

//*****************************************************************************
bool VuAiBehaviorDriveLazy::tick(float fdt)
{
	// Required for timer maintenance
	if (!VuAiBehavior::tick(fdt))
	{
		return false;
	}

	float goalSpeed = mpAiInstance->getCar()->getMaxForwardSpeed() * 0.5f;

	mpAiInstance->getAiDriver()->addControlModifierGoalSpeed(goalSpeed);

	const VuTrackSector* pNextSector = mpAiInstance->getAiDriver()->getPlan().getSectorForStep(1);

	if (pNextSector)
	{
		VuVector3 pos = pNextSector->mEnterPoint * 0.7f + pNextSector->mExitPoint * 0.3f;

		mpAiInstance->getAiDriver()->addControlModifierGoalPosition(pos);
	}

	return true;
}

//*****************************************************************************
// LoseControl
//*****************************************************************************

IMPLEMENT_BEHAVIOR_CREATE(LoseControl);

//*****************************************************************************
float VuAiBehaviorLoseControl::score()
{
	mScore = mTuningWeight * 10.0f;
	return mScore;
}

//*****************************************************************************
bool  VuAiBehaviorLoseControl::start()
{
	VuAiBehavior::start();

	// Swerve for this long
	mCountdownTimer = 4.0f;

	return true;
}

//*****************************************************************************
bool VuAiBehaviorLoseControl::tick(float fdt)
{
	// Required for timer maintenance
	if (!VuAiBehavior::tick(fdt))
	{
		return false;
	}


	float scaleFactor = 0.5f;

	switch (mpAiInstance->getAiTuningVariables()->mAiSkill)
	{
	case SKILL_BAD: scaleFactor = 0.75f;
	case SKILL_NORMAL: scaleFactor = 0.5f;
	case SKILL_GOOD: scaleFactor = 0.25f;
	}

	float throttle = 0.8f;
	float yaw = VuRand::global().range(-1.0f, 1.0f) * scaleFactor;

	mpAiInstance->getAiDriver()->addControlModifierDirectStickControl(yaw, throttle);

	return true;
}

//*****************************************************************************
// BrakeForCorner
//*****************************************************************************

IMPLEMENT_BEHAVIOR_CREATE(BrakeForCorner);

//*****************************************************************************
float VuAiBehaviorBrakeForCorner::score()
{
	mScore = mTuningWeight * 1.1f;
	return mScore;
}

//*****************************************************************************
bool  VuAiBehaviorBrakeForCorner::start()
{
	VuAiBehavior::start();

	// Throttle Down for this long
	mCountdownTimer = 3.0f;

	// Temporarily increase handling while processing this behavior
	float buffValue = (float)mpAiInstance->getAiTuningVariables()->mAiSkill + 1.0f;

	mpAiInstance->getCar()->setAiBuffHandling(1.25f);

	return true;
}

//*****************************************************************************
bool VuAiBehaviorBrakeForCorner::tick(float fdt)
{
	// Required for timer maintenance
	if (!VuAiBehavior::tick(fdt))
	{
		return false;
	}

	float immediateBendiness = mpAiInstance->getAiDriver()->getPlan().computeBendinessAtTime(3.0f); // seconds of lookahead at current speed

	// If we're not skilled, slow down a little when bendy road is ahead
	if (immediateBendiness < (mpAiInstance->getAiTuningVariables()->mAiThrottleDownBendiness * 0.9f))
	{
		// Expire this behavior
		return false;
	}

	float velocity = mpAiInstance->getCar()->getLinearVelocity().mag();
	float maxSpeed = mpAiInstance->getCar()->getMaxForwardSpeed();

	float velocityRatio = VuMin(1.0f, velocity / maxSpeed);

	// Cut speed down to help make corner
	float goalSpeed = maxSpeed;

	// Only cut speed on corners for higher performance cars
	if (mpAiInstance->getCar()->getCarStage() > 0)
	{
		if (velocityRatio > 0.9f)
		{
			float multiplier = VuMax((1.0f - velocityRatio), 0.8f);

			goalSpeed *= multiplier;
		}

		// If we're skidding, slow down even more until we've got traction
		if (mpAiInstance->mIsSkidding)
		{
			goalSpeed = 0.90f * velocity;
		}
	}

	mpAiInstance->getAiDriver()->addControlModifierGoalSpeed(goalSpeed);
	
	return true;
}

//*****************************************************************************
bool VuAiBehaviorBrakeForCorner::stop()
{
	// Restore buff value
	mpAiInstance->getCar()->setAiBuffHandling(mpAiInstance->getAiTuningVariables()->mHandlingMultiplier);

	return VuAiBehavior::stop();
}

//*****************************************************************************
// LaneChange
//*****************************************************************************

IMPLEMENT_BEHAVIOR_CREATE(LaneChange);

//*****************************************************************************
float VuAiBehaviorLaneChange::score()
{
	mScore = mTuningWeight * 1.1f * mpAiInstance->getAiTuningVariables()->mAvoidance;
	return mScore;
}

//*****************************************************************************
bool  VuAiBehaviorLaneChange::start()
{
	VuAiBehavior::start();

	mCountdownTimer = 0.25f;

	return true;
}

//*****************************************************************************
bool VuAiBehaviorLaneChange::tick(float fdt)
{
	// Required for timer maintenance
	if (!VuAiBehavior::tick(fdt))
	{
		return false;
	}

	VuParams::VuAccessor paramData(mParams);

	eAiLane desiredLane = static_cast<eAiLane>(paramData.getInt());

	float yaw = mpAiInstance->getCar()->getYawControl();
	if (desiredLane < mpAiInstance->mCurLane)
	{
		yaw = -1.0f;
	}
	else if (desiredLane > mpAiInstance->mCurLane)
	{
		yaw = 1.0f;
	}

	float throttle = mpAiInstance->getCar()->getThrottleControl();

	mpAiInstance->getAiDriver()->addControlModifierDirectStickControl(yaw, throttle);

	return true;
}


//*****************************************************************************
// GetInFrontOfRacer
//*****************************************************************************

IMPLEMENT_BEHAVIOR_CREATE(GetInFrontOfRacer);

//*****************************************************************************
float VuAiBehaviorGetInFrontOfRacer::score()
{
	mScore = mTuningWeight * 1.1f * mpAiInstance->getAiTuningVariables()->mAggressiveness;
	return mScore;
}

//*****************************************************************************
bool  VuAiBehaviorGetInFrontOfRacer::start()
{
	VuAiBehavior::start();

	mCountdownTimer = 10.00f;

	return true;
}

//*****************************************************************************
bool VuAiBehaviorGetInFrontOfRacer::tick(float fdt)
{
	// Required for timer maintenance
	if (!VuAiBehavior::tick(fdt))
	{
		return false;
	}

	if (!mpAiInstance->mpCompetitor)
	{
		return true;
	}

	float dist = VuAiUtils::raceDistanceBetween(mpAiInstance->getCar(), mpAiInstance->mpCompetitor);

	float metersInFront = 5.0f;
	if (dist < metersInFront)
	{
		mpAiInstance->getCar()->setAiBuffHandling(10.0f);
		mpAiInstance->getCar()->setAiBuffSpeed(10.0f);
		VUPRINTF("FSpeed: %#.2f\n", mpAiInstance->getCar()->getLinearVelocity().mag());
	}
	else
	{
		float buffPercent = mpAiInstance->getAiTuningVariables()->mBuffPercent;

		if (buffPercent != 100.0f)
		{
			mpAiInstance->getCar()->setAiBuffHandling(buffPercent / 100.0f);
			mpAiInstance->getCar()->setAiBuffSpeed(buffPercent / 100.0f);
		}
		else
		{
			mpAiInstance->getCar()->setAiBuffHandling(mpAiInstance->getAiTuningVariables()->mHandlingMultiplier);
			mpAiInstance->getCar()->setAiBuffSpeed(mpAiInstance->getAiTuningVariables()->mTopSpeedMultiplier);
		}

		mCountdownTimer = 0.0f;
	}


	return true;
}

//*****************************************************************************
bool VuAiBehaviorGetInFrontOfRacer::stop()
{
	// Restore buff value
	float buffPercent = mpAiInstance->getAiTuningVariables()->mBuffPercent;

	if (buffPercent != 100.0f)
	{
		mpAiInstance->getCar()->setAiBuffHandling(buffPercent / 100.0f);
		mpAiInstance->getCar()->setAiBuffSpeed(buffPercent / 100.0f);
	}
	else
	{
		mpAiInstance->getCar()->setAiBuffHandling(mpAiInstance->getAiTuningVariables()->mHandlingMultiplier);
		mpAiInstance->getCar()->setAiBuffSpeed(mpAiInstance->getAiTuningVariables()->mTopSpeedMultiplier);
	}

	return VuAiBehavior::stop();
}


//*****************************************************************************
// GetInBackOfRacer
//*****************************************************************************

IMPLEMENT_BEHAVIOR_CREATE(GetInBackOfRacer);

//*****************************************************************************
float VuAiBehaviorGetInBackOfRacer::score()
{
	mScore = mTuningWeight * 1.1f * mpAiInstance->getAiTuningVariables()->mAggressiveness;
	return mScore;
}

//*****************************************************************************
bool  VuAiBehaviorGetInBackOfRacer::start()
{
	VuAiBehavior::start();

	mCountdownTimer = 10.00f;

	return true;
}

//*****************************************************************************
bool VuAiBehaviorGetInBackOfRacer::tick(float fdt)
{
	// Required for timer maintenance
	if (!VuAiBehavior::tick(fdt))
	{
		return false;
	}
	
	if (!mpAiInstance->mpCompetitor)
	{
		return true;
	}

	float dist = VuAiUtils::raceDistanceBetween(mpAiInstance->getCar(), mpAiInstance->mpCompetitor);
	
	float metersBehind = 5.0f;
	if (dist > -1.0f * metersBehind)
	{
		mpAiInstance->getCar()->setAiBuffSpeed(0.01f);
		VUPRINTF("BSpeed: %#.2f\n", mpAiInstance->getCar()->getLinearVelocity().mag());
	}
	else
	{
		mpAiInstance->getCar()->setAiBuffSpeed(mpAiInstance->getAiTuningVariables()->mTopSpeedMultiplier);
		this->mCountdownTimer = 0.0f;
	}


	return true;
}

//*****************************************************************************
bool VuAiBehaviorGetInBackOfRacer::stop()
{
	// Restore buff value
	float buffPercent = mpAiInstance->getAiTuningVariables()->mBuffPercent;

	if (buffPercent != 100.0f)
	{
		mpAiInstance->getCar()->setAiBuffSpeed(buffPercent / 100.0f);
	}
	else
	{
		mpAiInstance->getCar()->setAiBuffSpeed(mpAiInstance->getAiTuningVariables()->mTopSpeedMultiplier);
	}

	return VuAiBehavior::stop();
}



//*****************************************************************************
// SlowDownForPack
//*****************************************************************************

IMPLEMENT_BEHAVIOR_CREATE(SlowDownForPack);

//*****************************************************************************
float VuAiBehaviorSlowDownForPack::score()
{
	mScore = mTuningWeight * 1.1f;
	return mScore;
}

//*****************************************************************************
bool  VuAiBehaviorSlowDownForPack::start()
{
	VuAiBehavior::start();

	float duration = 5.0f;
	float debuff = 0.8f;

	switch (mpAiInstance->getCar()->getCarStage())
	{
	case 0:		// Class A
		duration = 1.0f;
		debuff = 0.9f;
		break;
	case 1:		// Class B
		duration = 2.0f;
		debuff = 0.8f;
		break;
	case 2:		// Class C
		duration = 3.0f;
		debuff = 0.8f;
		break;
	case 3:		// Class D
		duration = 4.0f;
		debuff = 0.8f;
		break;
	}

	mCountdownTimer = duration;
	mpAiInstance->getCar()->setAiBuffSpeed(debuff);

	return true;
}

//*****************************************************************************
bool VuAiBehaviorSlowDownForPack::tick(float fdt)
{
	// Required for timer maintenance
	if (!VuAiBehavior::tick(fdt))
	{
		return false;
	}

	return true;
}

//*****************************************************************************
bool VuAiBehaviorSlowDownForPack::stop()
{
	// Restore buff value
	float buffPercent = mpAiInstance->getAiTuningVariables()->mBuffPercent;

	if (buffPercent != 100.0f)
	{
		mpAiInstance->getCar()->setAiBuffSpeed(buffPercent / 100.0f);
	}
	else
	{
		mpAiInstance->getCar()->setAiBuffSpeed(mpAiInstance->getAiTuningVariables()->mTopSpeedMultiplier);
	}

	return VuAiBehavior::stop();
}



//*****************************************************************************
// SpeedUpForPack
//*****************************************************************************

IMPLEMENT_BEHAVIOR_CREATE(SpeedUpForPack);

//*****************************************************************************
float VuAiBehaviorSpeedUpForPack::score()
{
	mScore = mTuningWeight * 1.1f;


	return mScore;
}

//*****************************************************************************
bool  VuAiBehaviorSpeedUpForPack::start()
{
	VuAiBehavior::start();

	float duration = 5.0f;
	float buff = 1.2f;

	switch (mpAiInstance->getCar()->getCarStage())
	{
	case 0:		// Class A
		duration = 1.0f;
		buff = 1.2f;
		break;
	case 1:		// Class B
		duration = 2.0f;
		buff = 1.3f;
		break;
	case 2:		// Class C
		duration = 3.0f;
		buff = 1.5f;
		break;
	case 3:		// Class D
		duration = 4.0f;
		buff = 2.0f;
		break;
	}

	VUPRINTF("SPEEDUP!\n");
	mCountdownTimer = duration;
	mpAiInstance->getCar()->setAiBuffSpeed(buff);

	return true;
}

//*****************************************************************************
bool VuAiBehaviorSpeedUpForPack::tick(float fdt)
{
	// Required for timer maintenance
	if (!VuAiBehavior::tick(fdt))
	{
		return false;
	}

	return true;
}

//*****************************************************************************
bool VuAiBehaviorSpeedUpForPack::stop()
{
	// Restore buff value
	float buffPercent = mpAiInstance->getAiTuningVariables()->mBuffPercent;

	if (buffPercent != 100.0f)
	{
		mpAiInstance->getCar()->setAiBuffSpeed(buffPercent / 100.0f);
	}
	else
	{
		mpAiInstance->getCar()->setAiBuffSpeed(mpAiInstance->getAiTuningVariables()->mTopSpeedMultiplier);
	}

	return VuAiBehavior::stop();
}


//*****************************************************************************
// MaintainMaxLead
//*****************************************************************************

IMPLEMENT_BEHAVIOR_CREATE(MaintainMaxLead);

//*****************************************************************************
float VuAiBehaviorMaintainMaxLead::score()
{
	mScore = mTuningWeight * 4.0f;
	return mScore;
}

//*****************************************************************************
bool  VuAiBehaviorMaintainMaxLead::start()
{
	VuAiBehavior::start();

	float duration = 5.0f;
	float debuff = 0.5f;

	mCountdownTimer = duration;
	mpAiInstance->getCar()->setAiBuffSpeed(debuff);

	return true;
}

//*****************************************************************************
bool VuAiBehaviorMaintainMaxLead::tick(float fdt)
{
	// Required for timer maintenance
	if (!VuAiBehavior::tick(fdt))
	{
		return false;
	}

	// Abort early if we're behind our maximum lead distance, minus 20%
	float lead = mpAiInstance->getLeadOverRacer();
	if (lead < mpAiInstance->getAiTuningVariables()->mMaximumLead * 0.80f)
	{
		return false;
	}

	return true;
}

//*****************************************************************************
bool VuAiBehaviorMaintainMaxLead::stop()
{
	// Restore buff value
	float buffPercent = mpAiInstance->getAiTuningVariables()->mBuffPercent;

	if (buffPercent != 100.0f)
	{
		mpAiInstance->getCar()->setAiBuffSpeed(buffPercent / 100.0f);
	}
	else
	{
		mpAiInstance->getCar()->setAiBuffSpeed(mpAiInstance->getAiTuningVariables()->mTopSpeedMultiplier);
	}

	return VuAiBehavior::stop();
}


//*****************************************************************************
// HandlingBoost
//*****************************************************************************

IMPLEMENT_BEHAVIOR_CREATE(HandlingBoost);

//*****************************************************************************
float VuAiBehaviorHandlingBoost::score()
{
	mScore = mTuningWeight * 4.0f;
	return mScore;
}

//*****************************************************************************
bool  VuAiBehaviorHandlingBoost::start()
{
	VuAiBehavior::start();

	float duration = 10.0f;
	float buff = mpAiInstance->getAiTuningVariables()->mHandlingMultiplier * 2.0f;

	mCountdownTimer = duration;

	mpAiInstance->getCar()->setAiBuffHandling(buff);

	return true;
}

//*****************************************************************************
bool VuAiBehaviorHandlingBoost::tick(float fdt)
{
	// Required for timer maintenance
	if (!VuAiBehavior::tick(fdt))
	{
		return false;
	}

	return true;
}

//*****************************************************************************
bool VuAiBehaviorHandlingBoost::stop()
{
	// Restore buff value
	mpAiInstance->getCar()->setAiBuffHandling(mpAiInstance->getAiTuningVariables()->mHandlingMultiplier);

	return VuAiBehavior::stop();
}

