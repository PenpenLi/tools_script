//*****************************************************************************
//
//  Copyright (c) 2007-2008 Ralf Knoesel
//  Copyright (c) 2008-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  AI Ray Test Classes
// 
//*****************************************************************************

#include "VuPurple/Powerups/VuPowerUpManager.h"
#include "VuPurple/Entities/Car/Driver/VuAiDriverEntity.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarPowerUpController.h"
#include "VuPurple/Entities/Car/Parts/VuCarEffectController.h"
#include "VuPurple/Entities/Car/Effects/VuCarEffect.h"
#include "VuPurple/Managers/VuCarManager.h"

#include "VuPurple/Track/VuTrackPlan.h"
#include "VuPurple/Track/VuTrackUtil.h"
#include "VuPurple/Track/VuTrackSector.h"
#include "VuPurple/Track/VuTrackManager.h"

#include "VuPurple/AI/VuAiTuningVariables.h"
#include "VuPurple/AI/VuAiManager.h"
#include "VuPurple/AI/VuAiInstance.h"
#include "VuPurple/AI/VuAiBehavior.h"
#include "VuPurple/AI/VuAiBrain.h"
#include "VuPurple/AI/VuAiUtils.h"

//*****************************************************************************
// VuAiBrain - Base Class of all Brains
//*****************************************************************************
void VuAiBrain::clearList(BehaviorList &list) 
{ 
	for (auto pBehavior : list) 
	{ 
		VuAiBehaviorFactory::IF()->destroy(pBehavior); 
	} 
	list.clear(); 
}

//*****************************************************************************
VuAiBehavior* VuAiBrain::addPossibleBehavior(const std::string& name)
{
	VuAiBehavior* pNewBehavior = VuAiBehaviorFactory::IF()->create(name);
	if (pNewBehavior)
	{
		pNewBehavior->init(mpAiInstance.get());

		mPossibleBehaviors.push_back(pNewBehavior);
	}

	return pNewBehavior;
}

//*****************************************************************************
void VuAiBrain::expireActiveBehaviors()
{
	BehaviorList::iterator it;

	for (it = mActiveBehaviors.begin(); it != mActiveBehaviors.end();)
	{
		if ((*it)->isFinished())
		{
			(*it)->stop();

//			VUPRINTF("Expired Behavior %s\n", (*it)->getName().c_str());

			// Deallocate the storage
			VuAiBehaviorFactory::IF()->destroy(*it);

			// Next element
			it = mActiveBehaviors.erase(it);
		}
		else
		{
			++it;
		}
	}
}

//*****************************************************************************
bool VuAiBrain::hasActiveBehavior(const std::string& name)
{
	for (auto behavior : mActiveBehaviors)
	{
		if (behavior->getName() == name)
		{
			return true;
		}
	}

	return false;
}

//*****************************************************************************
float VuAiBrain::sort()
{
	if (mPossibleBehaviors.size() > 1)
	{
		std::sort(mPossibleBehaviors.begin(), mPossibleBehaviors.end(), behaviorSortFunction);
	}

	return 0.0f;
}

//*****************************************************************************
VuAiBehavior* VuAiBrain::select()
{
	VuAiBehavior* pBestBehavior = VUNULL;

	while (mPossibleBehaviors.size() > 0)
	{
		pBestBehavior = mPossibleBehaviors.back();

		mPossibleBehaviors.pop_back();

		bool alreadyInActiveBehaviors = false;
		for (auto pActiveBehavior : mActiveBehaviors)
		{
			if (pBestBehavior->getName() == pActiveBehavior->getName())
			{
				alreadyInActiveBehaviors = true;
				break;
			}
		}

		if (alreadyInActiveBehaviors)
		{
			VuAiBehaviorFactory::IF()->destroy(pBestBehavior);
			pBestBehavior = VUNULL;
		}
		else
		{
			break;
		}
	}

	return pBestBehavior;
}

//*****************************************************************************
void VuAiBrain::tickBehaviors(float fdt)
{
	for (auto pBehavior : mActiveBehaviors)
	{
		pBehavior->tick(fdt);
	}
}

//*****************************************************************************
// Static
bool VuAiBrain::behaviorSortFunction(VuAiBehavior* pOne, VuAiBehavior* pTwo)
{
	VUASSERT(pOne != VUNULL && pTwo != VUNULL, "Illegal parameter passed on behavior sort.");

	if (pOne == VUNULL || pTwo == VUNULL)
	{
		return false;
	}

	// Best score sorts to the end of the list where we pop_back() when we get it
	if (pOne->score() < pTwo->score())
	{
		return true;
	}

	return false;
}


//*****************************************************************************
void VuAiBrain::think()
{
	// re-cache sensor input at some frequency
	refreshSensors();

	// Remove any ones from mActiveBehaviors that are finished now
	expireActiveBehaviors();

	VuAiManager::eRaceState raceState = VuAiManager::IF()->getRaceState();

	// If we're not racing, no reason to think about things
	if (raceState != VuAiManager::RACING)
	{
		return;
	}

	// See if we can beat our current behavior
	if (generatePossibleBehaviors() > 0)
	{
		sort();

		VuAiBehavior* pBestPossible = !mPossibleBehaviors.empty() ? mPossibleBehaviors.back() : VUNULL;
		VuAiBehavior* pBestActive = !mActiveBehaviors.empty() ? mActiveBehaviors.back() : VUNULL;

		float possibleScore = 0.0f;
		float activeScore = 0.0f;

		if (pBestPossible)
		{
			possibleScore = pBestPossible->score();
		}

		if (pBestActive)
		{
			// Decay score of active behaviors slightly
			activeScore = pBestActive->score() * 0.99f;
		}

		if (possibleScore > activeScore)
		{
			VuAiBehavior* pNew = select();

			// Zero scores are discarded
			if (pNew && pNew->score() > FLT_EPSILON)
			{
//				VUPRINTF("Info: Starting %s.\n", pNew->getName().c_str());

				mActiveBehaviors.push_back(pNew);

				pNew->startReacting();
			}
		}
	}
}

//*****************************************************************************
// Brain Factory
//*****************************************************************************

IMPLEMENT_SYSTEM_COMPONENT(VuAiBrainFactory, VuAiBrainFactory);

VuAiBrainFactory::VuAiBrainFactory()
{
}

//*****************************************************************************
bool VuAiBrainFactory::init()
{
	REGISTER_BRAIN(Default);
	REGISTER_BRAIN(BossHula);
	REGISTER_BRAIN(BossBeachBro);
	REGISTER_BRAIN(BossDisco);
	REGISTER_BRAIN(BossLucha);
	REGISTER_BRAIN(BossTribal);
	REGISTER_BRAIN(BossRoller);
	REGISTER_BRAIN(BossAlien);
	REGISTER_BRAIN(BossBunny);
	REGISTER_BRAIN(Lazy);
	REGISTER_BRAIN(BossMcSkelly);

	return true;
}

//*****************************************************************************
void VuAiBrainFactory::registerBrain(const std::string& name, BrainCreateFunction function)
{
	mBrains[name] = function;
}

//*****************************************************************************
VuAiBrain* VuAiBrainFactory::create(const std::string& name)
{
	BrainCreatorMap::iterator itCreator = mBrains.find(name);
	if (itCreator != mBrains.end())
	{
		// start new effect
		BrainCreateFunction fn = itCreator->second;

		VuAiBrain *pBrain = fn();

		if (pBrain == VUNULL)
		{
			return VUNULL;
		}

		pBrain->setAiBrainName(name);

		return pBrain;
	}

	return VUNULL;
}

//*****************************************************************************
void VuAiBrainFactory::destroy(VuAiBrain* pBrain)
{
	if (pBrain)
	{
		delete pBrain;
	}
}

//*****************************************************************************
// VuAiBrainDefault
//*****************************************************************************

IMPLEMENT_BRAIN_CREATE(Default);

void VuAiBrainDefault::init(VuAiInstance* pInstance)
{
	VuAiBrain::init(pInstance);

	mPowerUpSelectionList.reserve(32);
}

//*****************************************************************************
void VuAiBrainDefault::refreshSensors()
{
	VuCarEntity* pCar = mpAiInstance->getCar();
	VUASSERT(pCar, "NULL Car Value from mpAiInstance");

	VuCarPowerUpController* pController = pCar->getPowerUpController();
	VUASSERT(pController, "Car has NULL PowerUpController");
}

//*****************************************************************************
void VuAiBrainDefault::generateForPowerUpsByGroup(const std::string& groupNames)
{
	if (mpAiInstance && !mpAiInstance->mUsePowerUps)
	{
		return;
	}

	mPowerUpSelectionList.clear();

	if (groupNames.find("Global") != std::string::npos) mPowerUpSelectionList.push_back(VuAiManager::IF()->getBestPowerUpByGroup("Global"));
	if (groupNames.find("Throw") != std::string::npos) mPowerUpSelectionList.push_back(VuAiManager::IF()->getBestPowerUpByGroup("Throw"));
	if (groupNames.find("Seek") != std::string::npos) mPowerUpSelectionList.push_back(VuAiManager::IF()->getBestPowerUpByGroup("Seek"));
	if (groupNames.find("Defense") != std::string::npos) mPowerUpSelectionList.push_back(VuAiManager::IF()->getBestPowerUpByGroup("Defense"));
	if (groupNames.find("LongShot") != std::string::npos) mPowerUpSelectionList.push_back(VuAiManager::IF()->getBestPowerUpByGroup("LongShot"));
	if (groupNames.find("Shield") != std::string::npos) mPowerUpSelectionList.push_back(VuAiManager::IF()->getBestPowerUpByGroup("Shield"));

	VuRand rand;

	int which = rand.range(0, (int)mPowerUpSelectionList.size() - 1);
	const VuPowerUp* pForcedPowerUp = VuPowerUpManager::IF()->getForcedPowerUp();
	if (pForcedPowerUp)
	{
		addPossibleBehavior(pForcedPowerUp->mName);
	}
	else
	{
		addPossibleBehavior(mPowerUpSelectionList[which]);
	}
}

//*****************************************************************************
// Used by non-boss AI to do powerup selection
//
void VuAiBrainDefault::generateForPowerUpsAllPossible()
{
	if (mpAiInstance && !mpAiInstance->mUsePowerUps)
	{
		return;
	}

	// Where is the AI's car relative to the racer/competitor?
	//
	eAiCarPack whichPack = mpAiInstance->getCarPack(mpAiInstance->getCar());

	mPowerUpSelectionList.clear();

	if (whichPack == PACK_AHEAD)
	{
		mPowerUpSelectionList.push_back(VuAiManager::IF()->getBestPowerUpByGroup("Global"));
		mPowerUpSelectionList.push_back(VuAiManager::IF()->getBestPowerUpByGroup("Defense"));
		mPowerUpSelectionList.push_back(VuAiManager::IF()->getBestPowerUpByGroup("Defense"));
		mPowerUpSelectionList.push_back(VuAiManager::IF()->getBestPowerUpByGroup("Defense"));
		mPowerUpSelectionList.push_back(VuAiManager::IF()->getBestPowerUpByGroup("Shield"));
	}
	else if (whichPack == PACK_BEHIND)
	{
		mPowerUpSelectionList.push_back(VuAiManager::IF()->getBestPowerUpByGroup("Throw"));
		mPowerUpSelectionList.push_back(VuAiManager::IF()->getBestPowerUpByGroup("Throw"));
		mPowerUpSelectionList.push_back(VuAiManager::IF()->getBestPowerUpByGroup("Throw"));
		mPowerUpSelectionList.push_back(VuAiManager::IF()->getBestPowerUpByGroup("Seek"));
		mPowerUpSelectionList.push_back(VuAiManager::IF()->getBestPowerUpByGroup("LongShot"));
		mPowerUpSelectionList.push_back(VuAiManager::IF()->getBestPowerUpByGroup("LongShot"));
		mPowerUpSelectionList.push_back(VuAiManager::IF()->getBestPowerUpByGroup("Global"));
	}
	else if (whichPack == PACK_NEAR)
	{
		mPowerUpSelectionList.push_back(VuAiManager::IF()->getBestPowerUpByGroup("Throw"));
		mPowerUpSelectionList.push_back(VuAiManager::IF()->getBestPowerUpByGroup("Seek"));
		mPowerUpSelectionList.push_back(VuAiManager::IF()->getBestPowerUpByGroup("Defense"));
		mPowerUpSelectionList.push_back(VuAiManager::IF()->getBestPowerUpByGroup("Defense"));
		mPowerUpSelectionList.push_back(VuAiManager::IF()->getBestPowerUpByGroup("Defense"));
		mPowerUpSelectionList.push_back(VuAiManager::IF()->getBestPowerUpByGroup("Global"));
		mPowerUpSelectionList.push_back(VuAiManager::IF()->getBestPowerUpByGroup("Shield"));
	}
	else
	{
		return;
	}

	VuRand rand;

	VuAiBehavior* pBehavior = VUNULL;

	int which = rand.range(0, (int)mPowerUpSelectionList.size() - 1);
	const VuPowerUp* pForcedPowerUp = VuPowerUpManager::IF()->getForcedPowerUp();
	if (pForcedPowerUp)
	{
		pBehavior = addPossibleBehavior(pForcedPowerUp->mName);
	}
	else
	{
		pBehavior = addPossibleBehavior(mPowerUpSelectionList[which]);

		VuAiManager::IF()->powerUpWasUsed(mPowerUpSelectionList[which]);
	}

	// For the powerup behavior, override scale up the reaction time based on skill
	// level. Higher skill means holding onto powerups longer.
	//
	if (pBehavior)
	{
		float reactionTime = mpAiInstance->getAiTuningVariables()->mReactionTime;

		// Want to randomize/extend powerup usage reaction time
		switch (mpAiInstance->getAiTuningVariables()->mAiSkill)
		{
		case SKILL_BAD:		reactionTime *= VuRand::global().range(2.0f, 8.0f); break;
		case SKILL_NORMAL:	reactionTime *= VuRand::global().range(4.0f, 8.0f); break;
		case SKILL_GOOD:	reactionTime *= VuRand::global().range(6.0f, 8.0f); break;
		}
//		VUPRINTF("Status: %s powerup %s reactionTime=%0.2f\n", mpAiInstance->getAiDriver()->getName(), pBehavior->getName().c_str(), reactionTime);

		pBehavior->overrideReactionTime(reactionTime);
	}
}

//*****************************************************************************
void VuAiBrainDefault::generateForPowerUps()
{
	if (mpAiInstance && !mpAiInstance->mUsePowerUps)
	{
		return;
	}

	const VuPowerUp* pPowerUp = mpAiInstance->getCar()->getPowerUpController()->getSlot(0).mpPowerUp;

	// If we can't use one yet, don't do anything
	if (!mpAiInstance->getCar()->canUsePowerUp() || pPowerUp == VUNULL)
	{
		return;
	}

	// Eat whatever powerup we had and determine what powerup we REALLY want to use
	mpAiInstance->getCar()->getPowerUpController()->consumeNextPowerUp();

	VuAiBrainDefault::generateForPowerUpsAllPossible();
}

//*****************************************************************************


void VuAiBrainDefault::generateForAvoidance()
{
	eAiLane obLane = mpAiInstance->mBlockedLane;
	eAiLane myLane = mpAiInstance->mCurLane;

	if (obLane == myLane)
	{
		eAiLane desiredLane = obLane;
		if (obLane == LANE_LEFT)
		{
			desiredLane = LANE_RIGHT;
		}
		else if (obLane == LANE_RIGHT)
		{
			desiredLane = LANE_LEFT;
		}
		else
		{
			VuRand rand;
			if (rand.range(0, 1) == 0)
			{
				desiredLane = LANE_LEFT;
			}
			else
			{
				desiredLane = LANE_RIGHT;
			}
		}

		VuAiBehavior* pBehavior = addPossibleBehavior("LaneChange");
		if (pBehavior)
		{
			pBehavior->getParams().addInt((int)desiredLane);
		}
	}
}

//*****************************************************************************
void VuAiBrainDefault::generateForDebuffs()
{
	if ( mpAiInstance->getCar()->getEffectController()->isActive(VuCarEffectController::CONFUSION) )
	{
		addPossibleBehavior("LoseControl");
	}
	
	const VuCarEffectController::ActiveEffects& effects = mpAiInstance->getCar()->getEffectController()->getActiveEffects();
	VuCarEffectController::ActiveEffects::const_iterator it = effects.find("VuCarSplatEffect");
	if (it != effects.end())
	{
		VuCarEffect* pEffect = it->second;
		if (pEffect)
		{
			if (pEffect->mSplatPfxName.find_first_of("Tiki") != std::string::npos)
			{
				if (!hasActiveBehavior("LoseControl"))
				{
					addPossibleBehavior("LoseControl");
				}
			}
		}
	}
}

//*****************************************************************************
int VuAiBrainDefault::generatePossibleBehaviors(eInclusionFlags flags)
{
	clearList(mPossibleBehaviors);

	// Create Behaviors based on what power ups we have or like to use
	//
	if (flags & INCLUDE_POWERUPS)
	{
		generateForPowerUps();
	}

	if (flags & INCLUDE_AVOIDANCE)
	{
		//	generateForAvoidance();
	}

	if (flags & INCLUDE_DEBUFFS)
	{
		generateForDebuffs();
	}

	// See if we should use Boost
	bool canBoostAgain = (VuSys::IF()->getTime() > mpAiInstance->mLastBoostTime + mpAiInstance->getAiTuningVariables()->mAiBoostFrequency);

	if (canBoostAgain && mpAiInstance->hasSpeedHint())
	{
		canBoostAgain = false;
	}

	eAiRaceProgress progress = mpAiInstance->getRaceProgress();

	eSkill skill = mpAiInstance->getAiTuningVariables()->mAiSkill;

	VuCarEntity* pHumanCar = VUNULL;
	if (VuCarManager::IF()->getLocalHumanCarCount() > 0)
	{
		pHumanCar = VuCarManager::IF()->getLocalHumanCar(0);
	}

	float distanceToHuman = VuAiUtils::raceDistanceBetween(mpAiInstance->getCar(), pHumanCar);

	// Get the current bendiness value from the planned TrackPlan
	VuTrackPlan& plan = mpAiInstance->getAiDriver()->getPlan();
	float bendiness = plan.getTrackBendiness();

	if (skill > SKILL_BAD)
	{
		bool canBrakeForCornerAgain = (VuSys::IF()->getTime() > mpAiInstance->mLastThrottleDownTime + mpAiInstance->getAiTuningVariables()->mAiThrottleDownFrequency);

		float immediateBendiness = plan.computeBendinessAtTime(3.0f); // seconds of lookahead at current speed
		
		// If we're not skilled, slow down a little when bendy road is ahead
		if ((flags & INCLUDE_CORNER_BRAKING) && canBrakeForCornerAgain && (immediateBendiness > mpAiInstance->getAiTuningVariables()->mAiThrottleDownBendiness))
		{
			addPossibleBehavior("BrakeForCorner");
			mpAiInstance->mLastThrottleDownTime = (float)VuSys::IF()->getTime();
		}
	}

	if (skill >= SKILL_NORMAL)
	{
		//
		// Skill is Greater than BAD
		//
		bool farBehind = false;


		if (distanceToHuman > 100.0f)
		{
			farBehind = true;
		}

		// Check for use of Boost
		if ((flags & INCLUDE_BOOST) &&
			canBoostAgain &&
			(bendiness < mpAiInstance->getAiTuningVariables()->mAiBoostBendiness) &&
			farBehind)			// don't boost in the first part of the race
		{
			if (!mpAiInstance->getCar()->getEffectController()->isActive(VuCarEffectController::BOOST))
			{
				VUPRINTF("BOOST!\n");
				addPossibleBehavior("Boost");
				mpAiInstance->mLastBoostTime = (float)VuSys::IF()->getTime();
			}
		}

		// Check for use of Spiked Tires (extra traction)
		bool canUseSpikedTiresAgain = (VuSys::IF()->getTime() > mpAiInstance->mLastSpikedTiresTime + mpAiInstance->getAiTuningVariables()->mAiSpikedTiresFrequency);
		if ((flags & INCLUDE_SPIKED_TIRES) &&
			canUseSpikedTiresAgain && 
			bendiness > mpAiInstance->getAiTuningVariables()->mAiSpikedTiresBendiness)
		{
			if (!mpAiInstance->getCar()->getEffectController()->isActive(VuCarEffectController::TRACTION))
			{
				addPossibleBehavior("HandlingBoost");

				mpAiInstance->mLastSpikedTiresTime = (float)VuSys::IF()->getTime();
			}
		}

		if (skill >= SKILL_GOOD)
		{
			// Check for Powerslide
			bool canUsePowerSlideAgain = (VuSys::IF()->getTime() > mpAiInstance->mLastPowerslideTime + mpAiInstance->getAiTuningVariables()->mAiPowerslideFrequency);

			if ((flags & INCLUDE_POWERSLIDE) && canUsePowerSlideAgain)
			{
				float localBendiness = plan.computeBendinessAtDist(30.0f); // meters of lookahead
				if (localBendiness > mpAiInstance->getAiTuningVariables()->mAiPowerslideBendiness)
				{
					addPossibleBehavior("PowerSlide");
					mpAiInstance->mLastPowerslideTime = (float)VuSys::IF()->getTime();
				}
			}
		}
	}

	if (flags & INCLUDE_RACE_POSITIONING)
	{
		eAiCarPack myPack = mpAiInstance->getCarPack(mpAiInstance->getCar());
		eAiCarPack desiredPack = mpAiInstance->getAiTuningVariables()->mDesiredCarPack;

		if (desiredPack < myPack)
		{
			int stage = mpAiInstance->getCar()->getCarStage();
			if (stage < 2)
			{
				addPossibleBehavior("SlowDownForPack");
			}
		}
		else if (desiredPack > myPack)
		{
			if (distanceToHuman > 60)
			{
				addPossibleBehavior("SpeedUpForPack");
			}
		}
	}

	// If we want to maintain a max lead, and we're in 3rd place or worse
	// and we have a MaximumLead set
	int myPlace = mpAiInstance->getCar()->getStats().mPlace;

	if ((flags & INCLUDE_MAX_LEAD) &&
		(myPlace >= 3) &&
		(mpAiInstance->getAiTuningVariables()->mMaximumLead > FLT_EPSILON))
	{

		float lead = mpAiInstance->getLeadOverRacer();

		if (lead > mpAiInstance->getAiTuningVariables()->mMaximumLead)
		{
			// Slow down!
			addPossibleBehavior("MaintainMaxLead");
		}

	}

	return (int)mPossibleBehaviors.size();
}

//*****************************************************************************
// VuAiBrainLazy
//*****************************************************************************

IMPLEMENT_BRAIN_CREATE(Lazy);

int VuAiBrainLazy::generatePossibleBehaviors(eInclusionFlags flags)
{
	addPossibleBehavior("DriveLazy");

	return (int)mPossibleBehaviors.size();
}

//*****************************************************************************
// VuAiBrainBossHula
//
// Augments the Default brain, rather than the Base brain.
//*****************************************************************************

IMPLEMENT_BRAIN_CREATE(BossHula);

//*****************************************************************************
void VuAiBrainBossHula::generateForCharacterAbility()
{
	VuCarEntity* pCompetitor = mpAiInstance->getCompetitor();
	
	bool abilityTimerExpired = (VuSys::IF()->getTime() > mpAiInstance->mLastCharAbilityTime +
		mpAiInstance->getAiTuningVariables()->mAiCharAbilityFrequency);

	bool ableToUseAbility = false;
	
	if (mpAiInstance->getRaceProgress() > PROGRESS_EARLY_RACE)
		ableToUseAbility = true;
	if (mpAiInstance->mRaceCompletionPercent > 1.0f &&
		mpAiInstance->mRaceCompletionPercent < 3.0f)
	{
		ableToUseAbility = true;
	}

	if (abilityTimerExpired && 
			ableToUseAbility &&
			!VuAiUtils::inFrontOfMe(mpAiInstance->getCar(), pCompetitor))
	{
		addPossibleBehavior("CharAbility");
		mpAiInstance->mLastCharAbilityTime = (float)VuSys::IF()->getTime();
	}
}

int VuAiBrainBossHula::generatePossibleBehaviors(eInclusionFlags flags)
{
	// We'll do our own powerup management for the boss, so don't use the 
	// default one
	//
	flags &= ~INCLUDE_POWERUPS;

	VuAiBrainDefault::generatePossibleBehaviors(flags);

	generateForCharacterAbility();

	if (VuSys::IF()->getTime() > mpAiInstance->mLastPowerUpTime + mpAiInstance->getAiTuningVariables()->mAiBossPowerUpFrequency)
	{
		if (VuAiUtils::inFrontOfMe(mpAiInstance->getCar(), mpAiInstance->getCompetitor()))
		{
			generateForPowerUpsByGroup("Throw,Seek");
		}
		else
		{
			generateForPowerUpsByGroup("Defense");
		}
		mpAiInstance->mLastPowerUpTime = (float)VuSys::IF()->getTime() + VuRand::global().range(-1.0f, 1.0f);
	}

	return (int)mPossibleBehaviors.size();
}


//*****************************************************************************
// VuAiBrainBossBeachBro
//
// Augments the Default brain, rather than the Base brain.
//*****************************************************************************

IMPLEMENT_BRAIN_CREATE(BossBeachBro);

//*****************************************************************************
void VuAiBrainBossBeachBro::generateForCharacterAbility()
{
	VuCarEntity* pCompetitor = mpAiInstance->getCompetitor();

	bool canUseAbilityAgain = (VuSys::IF()->getTime() > mpAiInstance->mLastCharAbilityTime +
		mpAiInstance->getAiTuningVariables()->mAiCharAbilityFrequency);

	if (canUseAbilityAgain &&
		mpAiInstance->getRaceProgress() > PROGRESS_EARLY_RACE &&
		!VuAiUtils::inFrontOfMe(mpAiInstance->getCar(), pCompetitor))
	{
		addPossibleBehavior("CharAbility");
		mpAiInstance->mLastCharAbilityTime = (float)VuSys::IF()->getTime();
	}
}

int VuAiBrainBossBeachBro::generatePossibleBehaviors(eInclusionFlags flags)
{
	VuAiBrainDefault::generatePossibleBehaviors(flags);

	generateForCharacterAbility();

	if (VuSys::IF()->getTime() > mpAiInstance->mLastPowerUpTime + mpAiInstance->getAiTuningVariables()->mAiBossPowerUpFrequency)
	{
		if (VuAiUtils::inFrontOfMe(mpAiInstance->getCar(), mpAiInstance->getCompetitor()))
		{
			generateForPowerUpsByGroup("Throw,Seek,LongShot");
		}
		else
		{
			generateForPowerUpsByGroup("Defense");
		}
		mpAiInstance->mLastPowerUpTime = (float)VuSys::IF()->getTime() + VuRand::global().range(-1.0f, 1.0f);
	}

	return (int)mPossibleBehaviors.size();
}


//*****************************************************************************
// VuAiBrainBossDisco
//
// Augments the Default brain, rather than the Base brain.
//*****************************************************************************

IMPLEMENT_BRAIN_CREATE(BossDisco);

//*****************************************************************************
void VuAiBrainBossDisco::generateForCharacterAbility()
{
	VuCarEntity* pCompetitor = mpAiInstance->getCompetitor();

	bool canUseAbilityAgain = (VuSys::IF()->getTime() > mpAiInstance->mLastCharAbilityTime +
		mpAiInstance->getAiTuningVariables()->mAiCharAbilityFrequency);

	if (canUseAbilityAgain)
	{
		VuAiBehavior* pNewBehavior = VUNULL;
		if (VuAiUtils::inFrontOfMe(mpAiInstance->getCar(), pCompetitor))
		{
			float lead = mpAiInstance->getLeadOverRacer();

			if (lead < -60.0f)
			{
				pNewBehavior = addPossibleBehavior("CharAbility");
			}
			else if (lead < -30.0f)
			{
				pNewBehavior = addPossibleBehavior("EarthStrike");
			}
			else if (lead < -15.0)
			{
				pNewBehavior = addPossibleBehavior("Scattershot");
			}
			else if (lead < -7.5f)
			{
				pNewBehavior = addPossibleBehavior("Firework");
			}
		}
		else
		{
			// If we're in front, leave a trail of madness
			//
			VUUINT32 choice = VuRand::global().range(0, 6);
			switch (choice)
			{
			case 0: pNewBehavior = addPossibleBehavior("OilSlick"); break;
			case 1: pNewBehavior = addPossibleBehavior("ChickenCrate"); break;
			case 2: pNewBehavior = addPossibleBehavior("MysteryCrate"); break;
			case 3: pNewBehavior = addPossibleBehavior("BasicShield"); break;
			case 4:	pNewBehavior = addPossibleBehavior("Fake"); break;
			case 5:	pNewBehavior = addPossibleBehavior("Spring"); break;
			}
		}

		if (pNewBehavior)
		{
			// Slam the tuning weight so we always pick these behaviors
			pNewBehavior->overrideTuningWeight(6.0f);
		}

		mpAiInstance->mLastCharAbilityTime = (float)VuSys::IF()->getTime();
	}
}

int VuAiBrainBossDisco::generatePossibleBehaviors(eInclusionFlags flags)
{
	VuAiBrainDefault::generatePossibleBehaviors(flags);

	generateForCharacterAbility();

	return (int)mPossibleBehaviors.size();
}


//*****************************************************************************
// VuAiBrainBossLucha
//
// Augments the Default brain, rather than the Base brain.
//*****************************************************************************

IMPLEMENT_BRAIN_CREATE(BossLucha);

void VuAiBrainBossLucha::init(VuAiInstance* pInstance)
{
	VuAiBrainBossDefault::init(pInstance);

	// Override the default (Random) track plan with a shortcutting plan for 
	// this boss.
	pInstance->getAiDriver()->getPlan().setPlanType(VuTrackPlan::PLAN_SHORTCUTS);
}

//*****************************************************************************
void VuAiBrainBossLucha::generateForCharacterAbility()
{
	VuCarEntity* pCompetitor = mpAiInstance->getCompetitor();

	bool abilityTimerExpired = (VuSys::IF()->getTime() > mpAiInstance->mLastCharAbilityTime +
		mpAiInstance->getAiTuningVariables()->mAiCharAbilityFrequency);

	bool useAbilityAtRaceStart = false;
	if (mpAiInstance->mRaceCompletionPercent > 0.1f &&
		mpAiInstance->mRaceCompletionPercent < 2.0f)
	{
		useAbilityAtRaceStart = true;
	}

	float diff = VuAiUtils::raceDistanceBetween(mpAiInstance->getCar(), pCompetitor);

	bool nearOrBehind = false;
	if (diff < 5.0f)
	{
		nearOrBehind = true;
	}

	if (useAbilityAtRaceStart ||
		(abilityTimerExpired 
			&& mpAiInstance->getRaceProgress() > PROGRESS_EARLY_RACE 
			&& nearOrBehind))
	{
		addPossibleBehavior("CharAbility");

		mpAiInstance->mLastCharAbilityTime = (float)VuSys::IF()->getTime();
	}
}

int VuAiBrainBossLucha::generatePossibleBehaviors(eInclusionFlags flags)
{
	VuAiBrainDefault::generatePossibleBehaviors(flags);

	generateForCharacterAbility();

	if (VuSys::IF()->getTime() > mpAiInstance->mLastPowerUpTime + mpAiInstance->getAiTuningVariables()->mAiBossPowerUpFrequency)
	{
		if (VuAiUtils::inFrontOfMe(mpAiInstance->getCar(), mpAiInstance->getCompetitor()))
		{
			generateForPowerUpsByGroup("Throw,Seek,LongShot");
		}
		else
		{
			generateForPowerUpsByGroup("Defense,Shield");
		}
		mpAiInstance->mLastPowerUpTime = (float)VuSys::IF()->getTime() + VuRand::global().range(-1.0f, 1.0f);
	}

	return (int)mPossibleBehaviors.size();
}


//*****************************************************************************
// VuAiBrainBossTribal
//
// Augments the Default brain, rather than the Base brain.
//*****************************************************************************

IMPLEMENT_BRAIN_CREATE(BossTribal);

//*****************************************************************************
void VuAiBrainBossTribal::init(VuAiInstance* pInstance)
{
	VuAiBrainBossDefault::init(pInstance);

	// Override the default (Random) track plan with a shortcutting plan for 
	// this boss.
	pInstance->getAiDriver()->getPlan().setPlanType(VuTrackPlan::PLAN_SHORTCUTS);
}

//*****************************************************************************
void VuAiBrainBossTribal::generateForCharacterAbility()
{
	VuCarEntity* pCompetitor = mpAiInstance->getCompetitor();

	bool canUseAbilityAgain = (VuSys::IF()->getTime() > mpAiInstance->mLastCharAbilityTime +
		mpAiInstance->getAiTuningVariables()->mAiCharAbilityFrequency);

	if (canUseAbilityAgain)
	{
		VuAiBehavior* pNewBehavior = VUNULL;
		if (VuAiUtils::inFrontOfMe(mpAiInstance->getCar(), pCompetitor))
		{
			float lead = mpAiInstance->getLeadOverRacer();

			if (lead < -150.0f)
			{
				pNewBehavior = addPossibleBehavior("DeathBat");
			}
			else if (lead < -80.0f)
			{
				pNewBehavior = addPossibleBehavior("PoliceChase");
			}
			else if (lead < -15.0)
			{
				pNewBehavior = addPossibleBehavior("CharAbility");
			}
			else if (lead < -7.5f)
			{
				pNewBehavior = addPossibleBehavior("HomingMissile");
			}
		}
		else
		{
			if (VuRand::global().range(0, 2) % 2 == 0)
			{
				pNewBehavior = addPossibleBehavior("ChickenCrate");
			}
			else
			{
				pNewBehavior = addPossibleBehavior("MysteryCrate");
			}
		}

		if (pNewBehavior)
		{
			// Slam the tuning weight so we always pick these behaviors
			pNewBehavior->overrideTuningWeight(6.0f);
		}

		mpAiInstance->mLastCharAbilityTime = (float)VuSys::IF()->getTime();
	}
}

//*****************************************************************************

int VuAiBrainBossTribal::generatePossibleBehaviors(eInclusionFlags flags)
{
	VuAiBrainDefault::generatePossibleBehaviors(flags);

	generateForCharacterAbility();

	if (VuSys::IF()->getTime() > mpAiInstance->mLastPowerUpTime + mpAiInstance->getAiTuningVariables()->mAiBossPowerUpFrequency)
	{
		if (VuAiUtils::inFrontOfMe(mpAiInstance->getCar(), mpAiInstance->getCompetitor()))
		{
			generateForPowerUpsByGroup("LongShot");
		}
		else
		{
			generateForPowerUpsByGroup("Shield");
		}
		mpAiInstance->mLastPowerUpTime = (float)VuSys::IF()->getTime() + VuRand::global().range(-1.0f, 1.0f);
	}

	return (int)mPossibleBehaviors.size();
}


//*****************************************************************************
// VuAiBrainBossRoller
//
// Augments the Default brain, rather than the Base brain.
//*****************************************************************************

IMPLEMENT_BRAIN_CREATE(BossRoller);

//*****************************************************************************
void VuAiBrainBossRoller::init(VuAiInstance* pInstance)
{
	VuAiBrainBossDefault::init(pInstance);

	// Override the default (Random) track plan with a shortcutting plan for 
	// this boss.
	pInstance->getAiDriver()->getPlan().setPlanType(VuTrackPlan::PLAN_SHORTCUTS);
}

//*****************************************************************************
void VuAiBrainBossRoller::generateForCharacterAbility()
{
	VuCarEntity* pCompetitor = mpAiInstance->getCompetitor();

	bool abilityTimerExpired = (VuSys::IF()->getTime() > mpAiInstance->mLastCharAbilityTime +
		mpAiInstance->getAiTuningVariables()->mAiCharAbilityFrequency);

	float diff = VuAiUtils::raceDistanceBetween(mpAiInstance->getCar(), pCompetitor);

	bool nearOrBehind = false;
	if (diff < 5.0f)
	{
		nearOrBehind = true;
	}

	if (abilityTimerExpired
		&& mpAiInstance->getRaceProgress() > PROGRESS_EARLY_RACE
		&& nearOrBehind)
	{
		addPossibleBehavior("CharAbility");

		mpAiInstance->mLastCharAbilityTime = (float)VuSys::IF()->getTime();
	}
}

//*****************************************************************************
int VuAiBrainBossRoller::generatePossibleBehaviors(eInclusionFlags flags)
{
	VuAiBrainDefault::generatePossibleBehaviors(flags);

	generateForCharacterAbility();

	if (VuSys::IF()->getTime() > mpAiInstance->mLastPowerUpTime + mpAiInstance->getAiTuningVariables()->mAiBossPowerUpFrequency)
	{
		if (VuAiUtils::inFrontOfMe(mpAiInstance->getCar(), mpAiInstance->getCompetitor()))
		{
			generateForPowerUpsByGroup("Throw,Seek,LongShot");
		}
		else
		{
			generateForPowerUpsByGroup("Defense");
		}
		mpAiInstance->mLastPowerUpTime = (float)VuSys::IF()->getTime() + VuRand::global().range(-1.0f, 1.0f);
	}

	return (int)mPossibleBehaviors.size();
}


//*****************************************************************************
// VuAiBrainBossAlien
//
// Augments the Default brain, rather than the Base brain.
//*****************************************************************************

IMPLEMENT_BRAIN_CREATE(BossAlien);

//*****************************************************************************
void VuAiBrainBossAlien::generateForCharacterAbility()
{
	VuCarEntity* pCompetitor = mpAiInstance->getCompetitor();

	bool canUseAbilityAgain = (VuSys::IF()->getTime() > mpAiInstance->mLastCharAbilityTime +
		mpAiInstance->getAiTuningVariables()->mAiCharAbilityFrequency);

	if (canUseAbilityAgain)
	{
		VuAiBehavior* pNewBehavior = VUNULL;
		if (VuAiUtils::inFrontOfMe(mpAiInstance->getCar(), pCompetitor))
		{
			float lead = mpAiInstance->getLeadOverRacer();

			if (lead < -70.0f)
			{
				pNewBehavior = addPossibleBehavior("PoliceChase");
			}
			else if (lead < -30.0f)
			{
				pNewBehavior = addPossibleBehavior("Toughness");
			}
			else if (lead < -2.5f)
			{
				pNewBehavior = addPossibleBehavior("CharAbility");
			}
		}
		else
		{
			// If we're in front, leave a trail of madness
			//
			pNewBehavior = addPossibleBehavior("Spring");
		}

		if (pNewBehavior)
		{
			// Slam the tuning weight so we always pick these behaviors
			pNewBehavior->overrideTuningWeight(8.0f);
		}

		mpAiInstance->mLastCharAbilityTime = (float)VuSys::IF()->getTime();
	}
}

int VuAiBrainBossAlien::generatePossibleBehaviors(eInclusionFlags flags)
{
	VuAiBrainDefault::generatePossibleBehaviors(flags);

	generateForCharacterAbility();

	if (VuSys::IF()->getTime() > mpAiInstance->mLastPowerUpTime + mpAiInstance->getAiTuningVariables()->mAiBossPowerUpFrequency)
	{
		if (VuAiUtils::inFrontOfMe(mpAiInstance->getCar(), mpAiInstance->getCompetitor()))
		{
			generateForPowerUpsByGroup("Throw,Seek,LongShot");
		}
		else
		{
			generateForPowerUpsByGroup("Defense");
		}
		mpAiInstance->mLastPowerUpTime = (float)VuSys::IF()->getTime() + VuRand::global().range(-1.0f, 1.0f);
	}

	return (int)mPossibleBehaviors.size();
}


//*****************************************************************************
// VuAiBrainBossBunny
//
// Augments the Default brain, rather than the Base brain.
//*****************************************************************************

IMPLEMENT_BRAIN_CREATE(BossBunny);

//*****************************************************************************
void VuAiBrainBossBunny::init(VuAiInstance* pInstance)
{
	VuAiBrainBossDefault::init(pInstance);

	// Override the default (Random) track plan with a shortcutting plan for 
	// this boss.
	pInstance->getAiDriver()->getPlan().setPlanType(VuTrackPlan::PLAN_MAIN_TRACK);
}

//*****************************************************************************
void VuAiBrainBossBunny::generateForCharacterAbility()
{
	VuCarEntity* pCompetitor = mpAiInstance->getCompetitor();

	bool canUseAbilityAgain = (VuSys::IF()->getTime() > mpAiInstance->mLastCharAbilityTime +
		mpAiInstance->getAiTuningVariables()->mAiCharAbilityFrequency);

	if (canUseAbilityAgain)
	{
		VuAiBehavior* pNewBehavior = VUNULL;
		if (VuAiUtils::inFrontOfMe(mpAiInstance->getCar(), pCompetitor))
		{
			float lead = mpAiInstance->getLeadOverRacer();

			if (lead < -100.0f)
			{
				pNewBehavior = addPossibleBehavior("DeathBat");
			}
			else if (lead < -60.0f)
			{
				pNewBehavior = addPossibleBehavior("PoliceChase");
			}
			else if (lead < 10.0f)
			{
				pNewBehavior = addPossibleBehavior("CharAbility");
			}
		}

		if (pNewBehavior)
		{
			// Slam the tuning weight so we always pick these behaviors
			pNewBehavior->overrideTuningWeight(8.0f);
		}

		mpAiInstance->mLastCharAbilityTime = (float)VuSys::IF()->getTime();
	}
}

//*****************************************************************************
int VuAiBrainBossBunny::generatePossibleBehaviors(eInclusionFlags flags)
{
	// Bunny needs to chill right at the start of the race.
	//
	if (mpAiInstance->mRaceCompletionPercent < 2.0f)
	{
		return (int)mPossibleBehaviors.size();
	}

	VuAiBrainDefault::generatePossibleBehaviors(flags);

	generateForCharacterAbility();

	if (VuSys::IF()->getTime() > mpAiInstance->mLastPowerUpTime + mpAiInstance->getAiTuningVariables()->mAiBossPowerUpFrequency)
	{
		if (VuAiUtils::inFrontOfMe(mpAiInstance->getCar(), mpAiInstance->getCompetitor()))
		{
			generateForPowerUpsByGroup("Throw,Seek,LongShot,Global");
		}
		else
		{
			generateForPowerUpsByGroup("Shield,Global,Defense");
		}
		mpAiInstance->mLastPowerUpTime = (float)VuSys::IF()->getTime() + VuRand::global().range(-1.0f, 1.0f);
	}

	return (int)mPossibleBehaviors.size();
}


//*****************************************************************************
// VuAiBrainBossMcSkelly
//
// Augments the Default brain, rather than the Base brain.
//*****************************************************************************

IMPLEMENT_BRAIN_CREATE(BossMcSkelly);

//*****************************************************************************
void VuAiBrainBossMcSkelly::generateForCharacterAbility()
{
	VuCarEntity* pCompetitor = mpAiInstance->getCompetitor();

	bool canUseAbilityAgain = (VuSys::IF()->getTime() > mpAiInstance->mLastCharAbilityTime +
		mpAiInstance->getAiTuningVariables()->mAiCharAbilityFrequency);

	if (canUseAbilityAgain &&
		mpAiInstance->getRaceProgress() > PROGRESS_EARLY_RACE &&
		!VuAiUtils::inFrontOfMe(mpAiInstance->getCar(), pCompetitor))
	{
		addPossibleBehavior("CharAbility");
		mpAiInstance->mLastCharAbilityTime = (float)VuSys::IF()->getTime();
	}
}

int VuAiBrainBossMcSkelly::generatePossibleBehaviors(eInclusionFlags flags)
{
	VuAiBrainDefault::generatePossibleBehaviors(flags);

	generateForCharacterAbility();

	if (VuSys::IF()->getTime() > mpAiInstance->mLastPowerUpTime + mpAiInstance->getAiTuningVariables()->mAiBossPowerUpFrequency)
	{
		if (VuAiUtils::inFrontOfMe(mpAiInstance->getCar(), mpAiInstance->getCompetitor()))
		{
			generateForPowerUpsByGroup("Throw,Seek,LongShot");
		}
		else
		{
			generateForPowerUpsByGroup("Defense");
		}
		mpAiInstance->mLastPowerUpTime = (float)VuSys::IF()->getTime() + VuRand::global().range(-1.0f, 1.0f);
	}

	return (int)mPossibleBehaviors.size();
}

