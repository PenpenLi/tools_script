//*****************************************************************************
//
//  Copyright (c) 2007-2008 Ralf Knoesel
//  Copyright (c) 2008-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  AI Tuning Variables Classes
// 
//*****************************************************************************

#pragma once

// Global Config Variables
//static const float					AI_EARLY_RACE_CUTOFF_PERCENT		= 30.0f; // percent of race completion before which the progress is "Early Race"
//static const float					AI_MID_RACE_CUTOFF_PERCENT			= 70.0f; // percent of race completion before which the progress is "Mid Race" anything larger is "End Race"

// Config Variables
static const float					AI_BOOST_FREQUENCY					= 6.0f;
static const float					AI_MAX_BENDINESS_FOR_BOOST			= 0.7f;	// boosts if bendiness of road is less than this value. Range is 0.0f to roughly 2.0f
static const float					AI_SPIKED_TIRES_FREQUENCY			= 10.0f;
static const float					AI_MIN_BENDINESS_FOR_SPIKED_TIRES	= 1.0f;
static const float					AI_POWERSLIDE_FREQUENCY				= 10.0f;
static const float					AI_MIN_BENDINESS_FOR_POWERSLIDE		= 1.2f;

//static const float					AI_MIN_GROUP_SEPARATION_DISTANCE	= 20.0f; // meters of separation between cars to be "grouped"

static const float					AI_CHAR_ABILITY_FREQUENCY			= 45.0f; // seconds between eligibility for use of character ability after having used it
static const float					AI_BOSS_POWERUP_FREQUENCY			= 15.0f;	 // seconds between eligibility for using a powerup if we're a boss
static const float					AI_DEFAULT_REACTION_TIME			= 0.5f;  // seconds between deciding to do something and doing it
static const float					AI_DEFAULT_AGGRESSIVENESS			= 0.5f;  // value from 0.0 to 1.0 to describe how aggressive this driver is
static const float					AI_DEFAULT_AVOIDANCE				= 0.5f;	 // value from 0.0 to 1.0 to describe how much this AI will try to avoid obstacles
static const float					AI_DEFAULT_BUFF_PERCENT				= 100.0f;// For a global buff/debuff of Acceleration/TopSpeed/Handling/Toughness
static const float					AI_DEFAULT_MAX_LEAD					= 0.0f;  // 0 for no maximum lead, or meters of lead on player during races
static const float					AI_THROTTLE_DOWN_FREQUENCY			= 5.0f;
static const float					AI_THROTTLE_DOWN_BENDINESS			= 1.0f;
static const float					AI_DEFAULT_AIR_STEER_MULTIPLIER		= 0.5f;
static const float					AI_DEFAULT_AIR_STEERING_OVERRIDE	= 0.0f;

enum eSkill							{ SKILL_BAD, SKILL_NORMAL, SKILL_GOOD, SKILL_MAX };
enum eAiCarPack						{ PACK_BEHIND, PACK_NEAR, PACK_AHEAD, PACK_MAX };

typedef std::map<std::string, float> BehaviorWeights;
typedef std::map<std::string, float> PowerUpWeights;

class VuJsonContainer;

struct VuAiGlobalConstants
{
	void load();

	float mPackSeparationDistance;
	float mEarlyRaceCutoffPercent;
	float mMidRaceCutoffPercent;
	float mDeathBatFrequency;
};

class VuAiTuningVariables
{
public:
	VuAiTuningVariables();
	virtual ~VuAiTuningVariables();

	void			setDefaults();
	void			copyBaseValues(const VuAiTuningVariables* pVariables);

	void			onLoad(const VuJsonContainer &opponentData, const VuJsonContainer* pPersonalityData, const char* pPersonalityName);

	void			downTuneByPercent(VuAiTuningVariables* pVariables, float percent);


	std::string		mAiBrainName;
	std::string		mAiPersonalityName;
	eSkill			mAiSkill;
	eAiCarPack		mDesiredCarPack;

	float			mAiBoostFrequency;
	float			mAiBoostBendiness;
	float			mAiSpikedTiresFrequency;
	float			mAiSpikedTiresBendiness;
	float			mAiPowerslideFrequency;
	float			mAiPowerslideBendiness;
	float			mAiThrottleDownFrequency;
	float			mAiThrottleDownBendiness;
	float			mAiCharAbilityFrequency;
	float			mAiBossPowerUpFrequency;
	float			mBuffPercent;

	float			mAccelerationMultiplier;
	float			mTopSpeedMultiplier;
	float			mHandlingMultiplier;
	float			mToughnessMultiplier;

	float			mAirSteerMultiplier;
	float			mAirSteeringSpeedOverride;

	// Loaded from PersonalityDB
	float			mAggressiveness;
	float			mAvoidance;
	float			mReactionTime;
	float			mMaximumLead;
	BehaviorWeights	mBehaviorWeights;
	PowerUpWeights	mPowerUpWeights;

	VuAiTuningVariables*	mpRaceScriptEarly;
	VuAiTuningVariables*	mpRaceScriptMid;
	VuAiTuningVariables*	mpRaceScriptLate;
};

