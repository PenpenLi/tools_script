//*****************************************************************************
//
//  Copyright (c) 2007-2008 Ralf Knoesel
//  Copyright (c) 2008-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  AI Tuning Variables
// 
//*****************************************************************************

#include "VuEngine/Json/VuJsonContainer.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuPurple/Entities/Car/Driver/VuAiDriverEntity.h"
#include "VuPurple/AI/VuAiTuningVariables.h"


//*****************************************************************************
// VuAiGlobalConstants
//*****************************************************************************

void VuAiGlobalConstants::load()
{
	VuGameUtil::IF()->constantDB()["Ai"]["GroupSeparationDistance"].getValue(mPackSeparationDistance);
	VuGameUtil::IF()->constantDB()["Ai"]["EarlyRaceCutoffPercent"].getValue(mEarlyRaceCutoffPercent);
	VuGameUtil::IF()->constantDB()["Ai"]["MidRaceCutoffPercent"].getValue(mMidRaceCutoffPercent);
	VuGameUtil::IF()->constantDB()["Ai"]["DeathBatFrequency"].getValue(mDeathBatFrequency);
}

//*****************************************************************************
// VuAiTuningVariables
//*****************************************************************************

//*****************************************************************************
VuAiTuningVariables::VuAiTuningVariables()
{
	setDefaults();
}

//*****************************************************************************
VuAiTuningVariables::~VuAiTuningVariables()
{
	mBehaviorWeights.clear();

	if (mpRaceScriptEarly)
	{
		delete mpRaceScriptEarly;
	}
	if (mpRaceScriptMid)
	{
		delete mpRaceScriptMid;
	}
	if (mpRaceScriptLate)
	{
		delete mpRaceScriptLate;
	}
}

//*****************************************************************************
void VuAiTuningVariables::setDefaults()
{
	mAiBrainName = "null";
	mAiSkill = SKILL_NORMAL;
	mDesiredCarPack = PACK_NEAR;

	mAiBoostFrequency = AI_BOOST_FREQUENCY;
	mAiBoostBendiness = AI_MAX_BENDINESS_FOR_BOOST;
	mAiSpikedTiresFrequency = AI_SPIKED_TIRES_FREQUENCY;
	mAiSpikedTiresBendiness = AI_MIN_BENDINESS_FOR_SPIKED_TIRES;
	mAiPowerslideFrequency = AI_POWERSLIDE_FREQUENCY;
	mAiPowerslideBendiness = AI_MIN_BENDINESS_FOR_POWERSLIDE;
	mAiThrottleDownFrequency = AI_THROTTLE_DOWN_FREQUENCY;
	mAiThrottleDownBendiness = AI_THROTTLE_DOWN_BENDINESS;
	mAiCharAbilityFrequency = AI_CHAR_ABILITY_FREQUENCY;
	mAiBossPowerUpFrequency = AI_BOSS_POWERUP_FREQUENCY;

	mBuffPercent = AI_DEFAULT_BUFF_PERCENT;
	mAggressiveness = AI_DEFAULT_AGGRESSIVENESS;
	mAvoidance = AI_DEFAULT_AVOIDANCE;
	mReactionTime = AI_DEFAULT_REACTION_TIME;
	mMaximumLead = AI_DEFAULT_MAX_LEAD;

	mBehaviorWeights.clear();
	mPowerUpWeights.clear();

	mAccelerationMultiplier = 1.0f;
	mTopSpeedMultiplier = 1.0f;
	mHandlingMultiplier = 1.0f;
	mToughnessMultiplier = 1.0f;

	mAirSteerMultiplier = AI_DEFAULT_AIR_STEER_MULTIPLIER;
	mAirSteeringSpeedOverride = AI_DEFAULT_AIR_STEERING_OVERRIDE;

	mpRaceScriptEarly = VUNULL;
	mpRaceScriptMid = VUNULL;
	mpRaceScriptLate = VUNULL;
}

//*****************************************************************************
void VuAiTuningVariables::copyBaseValues(const VuAiTuningVariables* pVariables)
{
	if (pVariables == VUNULL)
	{
		return;
	}

	mAiBrainName = pVariables->mAiBrainName;
	mAiSkill = pVariables->mAiSkill;
	mDesiredCarPack = pVariables->mDesiredCarPack;

	mAiBoostFrequency = pVariables->mAiBoostFrequency;
	mAiBoostBendiness = pVariables->mAiBoostBendiness;
	mAiSpikedTiresFrequency = pVariables->mAiSpikedTiresFrequency;
	mAiSpikedTiresBendiness = pVariables->mAiSpikedTiresBendiness;
	mAiPowerslideFrequency = pVariables->mAiPowerslideFrequency;
	mAiPowerslideBendiness = pVariables->mAiPowerslideBendiness;
	mAiThrottleDownFrequency = pVariables->mAiThrottleDownFrequency;
	mAiThrottleDownBendiness = pVariables->mAiThrottleDownBendiness;
	mAiCharAbilityFrequency = pVariables->mAiCharAbilityFrequency;
	mAiBossPowerUpFrequency = pVariables->mAiBossPowerUpFrequency;

	mBuffPercent = pVariables->mBuffPercent;
	mAggressiveness = pVariables->mAggressiveness;
	mAvoidance = pVariables->mAvoidance;
	mReactionTime = pVariables->mReactionTime;
	mMaximumLead = pVariables->mMaximumLead;

	mBehaviorWeights = pVariables->mBehaviorWeights;
	mPowerUpWeights = pVariables->mPowerUpWeights;
	mAccelerationMultiplier = pVariables->mAccelerationMultiplier;
	mTopSpeedMultiplier = pVariables->mTopSpeedMultiplier;
	mHandlingMultiplier = pVariables->mHandlingMultiplier;
	mToughnessMultiplier = pVariables->mToughnessMultiplier;

	mAirSteerMultiplier = pVariables->mAirSteerMultiplier;
	mAirSteeringSpeedOverride = pVariables->mAirSteeringSpeedOverride;

	mpRaceScriptEarly = VUNULL;
	mpRaceScriptMid = VUNULL;
	mpRaceScriptLate = VUNULL;
}

//*****************************************************************************
void VuAiTuningVariables::onLoad(const VuJsonContainer &opponentData, const VuJsonContainer* pPersonalityData, const char* pPersonalityName)
{
	// Personality Record name
	if (pPersonalityName == VUNULL)
	{
		if (opponentData.hasMember("AiPersonality")) mAiPersonalityName = opponentData["AiPersonality"].asString();
	}
	else
	{
		mAiPersonalityName = pPersonalityName;
	}

	const VuJsonContainer* pOpponentData = &opponentData;

	// Original call will pass in NULL for Personality Data, so we can look it up by name in the DB
	if (!pPersonalityData)
	{
		pPersonalityData = &VuGameUtil::IF()->aiPersonalityDB()[mAiPersonalityName];
	}
	else
	{
		// Recursive calls for filling out RaceScript parameters will use the AiPersonalityDB as the OpponentDB to
		// allow race-script overrides of both Personality AND OpponentDB info on a given 
		// 
		pOpponentData = pPersonalityData;
	}

	// Read the Brain requested
	if (pOpponentData->hasMember("AiBrain")) mAiBrainName = (*pOpponentData)["AiBrain"].asString();

	// Read the AI Skill Rating (0-2)
	if (pOpponentData->hasMember("AiSkill")) mAiSkill = (eSkill)(*pOpponentData)["AiSkill"].asInt();

	// Where in the race pack does this AI hope to be for this part of the race?
	if (pPersonalityData->hasMember("DesiredCarPack"))
	{
		std::string positionName = (*pPersonalityData)["DesiredCarPack"].asString().c_str();

		if (positionName == "Ahead") mDesiredCarPack = PACK_AHEAD;
		else if (positionName == "Behind") mDesiredCarPack = PACK_BEHIND;
		else if (positionName == "Near") mDesiredCarPack = PACK_NEAR;
	}

	// PersonalityDB overrides of OpponentDB Data
	if (pPersonalityData->hasMember("AiBrain"))	mAiBrainName = (*pPersonalityData)["AiBrain"].asString();
	if (pPersonalityData->hasMember("AiSkill")) mAiSkill = (eSkill)(*pPersonalityData)["AiSkill"].asInt();

	// Personality Frequencies and Track Measures
	if (pPersonalityData->hasMember("BoostFrequency")) mAiBoostFrequency = (*pPersonalityData)["BoostFrequency"].asFloat();
	if (pPersonalityData->hasMember("BoostBendiness")) mAiBoostBendiness = (*pPersonalityData)["BoostBendiness"].asFloat();
	if (pPersonalityData->hasMember("SpikesFrequency")) mAiSpikedTiresFrequency = (*pPersonalityData)["SpikesFrequency"].asFloat();
	if (pPersonalityData->hasMember("SpikesBendiness")) mAiSpikedTiresBendiness = (*pPersonalityData)["SpikesBendiness"].asFloat();
	if (pPersonalityData->hasMember("PowerslideFrequency")) mAiPowerslideFrequency = (*pPersonalityData)["PowerslideFrequency"].asFloat();
	if (pPersonalityData->hasMember("PowerslideBendiness")) mAiPowerslideBendiness = (*pPersonalityData)["PowerslideBendiness"].asFloat();
	if (pPersonalityData->hasMember("ThrottleDownFrequency")) mAiThrottleDownFrequency = (*pPersonalityData)["ThrottleDownFrequency"].asFloat();
	if (pPersonalityData->hasMember("ThrottleDownBendiness")) mAiThrottleDownBendiness = (*pPersonalityData)["ThrottleDownBendiness"].asFloat();
	if (pPersonalityData->hasMember("AbilityFrequency")) mAiCharAbilityFrequency = (*pPersonalityData)["AbilityFrequency"].asFloat();
	if (pPersonalityData->hasMember("BossPowerUpFrequency")) mAiBossPowerUpFrequency = (*pPersonalityData)["BossPowerUpFrequency"].asFloat();

	if (pPersonalityData->hasMember("Aggro")) mAggressiveness = (*pPersonalityData)["Aggro"].asFloat();
	if (pPersonalityData->hasMember("Avoidance")) mAvoidance = (*pPersonalityData)["Avoidance"].asFloat();
	if (pPersonalityData->hasMember("ReactionTime")) mReactionTime = (*pPersonalityData)["ReactionTime"].asFloat();
	if (pPersonalityData->hasMember("MaximumLead")) mMaximumLead = (*pPersonalityData)["MaximumLead"].asFloat();

	if (pPersonalityData->hasMember("BehaviorWeights"))
	{
		const VuJsonContainer& behaviorWeights = (*pPersonalityData)["BehaviorWeights"];
		for (int i = 0; i < behaviorWeights.size(); i++)
		{
			const VuJsonContainer& tuple = behaviorWeights[i];
			float value = 0.0f;

			std::string behaviorName = tuple.getMemberKey(0);
			value = tuple[behaviorName].asFloat();

			// [] operator will do replace
			mBehaviorWeights[behaviorName] = value;
		}
	}

	if (pPersonalityData->hasMember("AirSteerMultiplier")) mAirSteerMultiplier = (*pPersonalityData)["AirSteerMultiplier"].asFloat();
	if (pPersonalityData->hasMember("AirSteeringSpeedOverride")) mAirSteeringSpeedOverride = (*pPersonalityData)["AirSteeringSpeedOverride"].asFloat();
	
	if (pPersonalityData->hasMember("PowerUpWeights"))
	{
		const VuJsonContainer& powerUpWeights = (*pPersonalityData)["PowerUpWeights"];
		for (int i = 0; i < powerUpWeights.size(); i++)
		{
			const VuJsonContainer& tuple = powerUpWeights[i];
			float value = 0.0f;

			std::string powerUpName = tuple.getMemberKey(0);
			value = tuple[powerUpName].asFloat();

			// [] operator will do replace
			mPowerUpWeights[powerUpName] = value;
		}
	}
	if (pPersonalityData->hasMember("BuffPercent")) mBuffPercent = (*pPersonalityData)["BuffPercent"].asFloat();

	if (pPersonalityData->hasMember("Performance"))
	{
		const VuJsonContainer& perfData = (*pPersonalityData)["Performance"];

		if (perfData.hasMember("Acceleration")) mAccelerationMultiplier = perfData["Acceleration"].asFloat();
		if (perfData.hasMember("TopSpeed")) mTopSpeedMultiplier = perfData["TopSpeed"].asFloat();
		if (perfData.hasMember("Handling")) mHandlingMultiplier = perfData["Handling"].asFloat();
		if (perfData.hasMember("Toughness")) mToughnessMultiplier = perfData["Toughness"].asFloat();
	}

	if (pPersonalityData->hasMember("RaceScript"))
	{
		const VuJsonContainer& raceScript = (*pPersonalityData)["RaceScript"];
		
		if (raceScript.hasMember("Early"))
		{
			mpRaceScriptEarly = new VuAiTuningVariables();
			mpRaceScriptEarly->copyBaseValues(this);
			mpRaceScriptEarly->onLoad(opponentData, &raceScript["Early"], VUNULL);
		}

		if (raceScript.hasMember("Mid"))
		{
			mpRaceScriptMid = new VuAiTuningVariables();
			mpRaceScriptMid->copyBaseValues(this);
			mpRaceScriptMid->onLoad(opponentData, &raceScript["Mid"], VUNULL);
		}

		if (raceScript.hasMember("Late"))
		{
			mpRaceScriptLate = new VuAiTuningVariables();
			mpRaceScriptLate->copyBaseValues(this);
			mpRaceScriptLate->onLoad(opponentData, &raceScript["Late"], VUNULL);
		}
	}
}

//*****************************************************************************
void VuAiTuningVariables::downTuneByPercent(VuAiTuningVariables* pVariables, float percent)
{
	// Reduce performance variables by specified percent
	if (pVariables)
	{
		float scalar = (100.0f - percent) / 100.0f;

		pVariables->mAccelerationMultiplier = VuClamp(pVariables->mAccelerationMultiplier * scalar, 0.5f, 5.0f);
		pVariables->mTopSpeedMultiplier = VuClamp(pVariables->mTopSpeedMultiplier * scalar, 0.5f, 5.0f);
		pVariables->mHandlingMultiplier = VuClamp(pVariables->mHandlingMultiplier * scalar, 0.5f, 5.0f);
		pVariables->mToughnessMultiplier = VuClamp(pVariables->mToughnessMultiplier * scalar, 0.5f, 5.0f);

		downTuneByPercent(pVariables->mpRaceScriptEarly, percent);
		downTuneByPercent(pVariables->mpRaceScriptMid, percent);
		downTuneByPercent(pVariables->mpRaceScriptLate, percent);
	}
}

