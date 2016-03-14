//*****************************************************************************
//
//  Copyright (c) 2007-2008 Ralf Knoesel
//  Copyright (c) 2008-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  AI Instance Class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Math/VuMathUtil.h"
#include "VuEngine/Math/VuMatrix.h"
#include "VuEngine/Json/VuJsonContainer.h"
#include "VuEngine/Objects/VuRefObj.h"

#include "VuPurple/Track/VuTrackPlan.h"
#include "VuPurple/AI/VuAiTuningVariables.h"

#define AVERAGE_SPEED_COUNT		10

class VuAiDriver;
class VuCarEntity;
class VuAiWaypointEntity;
class VuTrackSector;
class VuAiBrain;
class VuAiTuningVariables;
class VuPlanObstacleInfo;

enum eAiCarPack;
enum eAiRaceProgress				{ PROGRESS_EARLY_RACE, PROGRESS_MID_RACE, PROGRESS_LATE_RACE };

static const float AI_LANE_WIDTH = 4.f;

class VuAiInstance : public VuRefObj
{
public:

	VuAiInstance();
	~VuAiInstance();

	void					init(VuAiDriver* pDriver, VuCarEntity* pCar);
	void					preConfigure(float finishTime, float topSpeedMPH);
	void					release();

	void					think();

	void					checkForSkidding();
	void					checkRaceProgress();
	void					tickDecision(float fdt);

	// Various Utilities for the AI
	const char*				filterPowerupUsage(const char* pName);
	void					usePowerup(const char* pName);
	void					computeAverageSpeed();
	void					applySpeedHint();
	bool					hasSpeedHint();
	void					doWedgeDetection(float fdt);
	float					updateRaceCompletionPercent();
	eAiCarPack				getCarPack(const VuCarEntity* pCar);
	eAiRaceProgress			getRaceProgress();
	float					getLeadOverRacer();

	eAiLane					getLane();

	VuAiTuningVariables*	getAiTuningVariables(bool getRawData = false);

	// Accessors
	VuAiDriver*				getAiDriver() { return mpDriver; }
	VuAiBrain*				getAiBrain() { return mpBrain; }
	VuCarEntity*			getCar() { return mpCar; }
	VuCarEntity*			getCompetitor() { return mpCompetitor; }
	
	
public:
//	VuMatrix				mPrevMatrix;
//	VuMatrix				mCurMatrix;

	VuAiDriver *			mpDriver;
	VuCarEntity *			mpCar;

	VuCarEntity *			mpCompetitor;
	float					mCompetitorTimer;	// How much longer we're competing against current competitor

	float					mTimeSpentWedged;	// To help with wedge detection

	int						mInitialLapCount;
	VuAiWaypointEntity *	mpInitialCheckPoint;

	float					mCurrentSpeedHint;
	float					mAverageSpeed;

	float					mConfiguredTime;
	float					mConfiguredTopSpeed;
	float					mSimulatedTraversalAmt;

	float					mSpeedSamples[AVERAGE_SPEED_COUNT];
	int						mSpeedSampleCount;

	eAiLane					mBlockedLane;

	VuAiBrain*				mpBrain;

	bool					mIsSkidding;
	bool					mIsChallenge;

	float					mRaceCompletionPercent;
	eAiRaceProgress			mCurRaceProgress;
	eAiLane					mCurLane;

	std::string				mGameType;

	std::list<VuCarEntity*>	mCarPacks[PACK_MAX];

	// Debug Options
	bool					mUsePowerUps;

public:
	// Variables used by the Brains
	float					mLastBoostTime;
	float					mLastSpikedTiresTime;
	float					mLastPowerslideTime;
	float					mLastThrottleDownTime;
	float					mLastCharAbilityTime;
	float					mLastPowerUpTime; // for bosses
};

