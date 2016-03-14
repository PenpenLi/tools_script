//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Car Stats
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Math/VuVector3.h"

class VuAiWaypointEntity;


// stats
class VuCarStats
{
public:
	VuCarStats() :
		mHasFinished(false),
		mCurLap(0),
		mCurLapTime(0.0),
		mTotalTime(0.0f),
		mBestLapTime(FLT_MAX),
		mDistToEndOfLap(0),
		mDistFromStartOfLap(0),
		mDistFromStart(0),
		mDistBehindLeader(0),
		mDistFromPlayer(0),
		mPlace(0),
		mChampRank(0),
		mLapCount(0),
		mCurrentSection(0),
		mpNextCheckPoint(VUNULL),
		mPrevPosition(0,0,0),
		mWrongWayAlpha(0.0f),
		mOutOfBoundsAlpha(0.0f),
		mGameTimer(-1),
		mScore(0),
		mEliminationCount(0),
		mRematchTimer(-1),
		mBasePoints(0),
		mPoints(0),
		mDNF(false),
		mDistanceDriven(0.0),
		mCharacterAbilityCharge(0.0f),
		mCharacterAbilityUsed(false),
		mUsedBrake(false),
		mRecoverCount(0),
		mPowerUpsUsedCount(0),
		mCurJumpDistance(0),
		mMaxJumpDistance(0)
	{
		memset(mSmashedStuff, 0, sizeof(mSmashedStuff));
	}

	bool				mHasFinished;
	int					mCurLap;
	double				mCurLapTime;
	double				mTotalTime;
	float				mBestLapTime;
	float				mDistToEndOfLap;
	float				mDistFromStartOfLap;
	float				mDistFromStart;
	float				mDistBehindLeader;
	float				mDistFromPlayer;

	int					mPlace;
	int					mChampRank;
	int					mLapCount;
	int					mCurrentSection;
	VuAiWaypointEntity	*mpNextCheckPoint;
	VuVector3			mPrevPosition;
	float				mWrongWayAlpha;
	float				mOutOfBoundsAlpha;
	int					mGameTimer;
	int					mScore;
	int					mEliminationCount;
	int					mRematchTimer;
	int					mBasePoints;
	int					mPoints;
	bool				mDNF;
	double				mDistanceDriven;
	float				mCharacterAbilityCharge;
	bool				mCharacterAbilityUsed;
	bool				mUsedBrake;
	int					mRecoverCount;
	int					mPowerUpsUsedCount;
	double				mCurJumpDistance;
	float				mMaxJumpDistance;

	enum { NONE, SEAGULL, PALM, DEATH_BAT, PENGUIN, CRAB, LAVA_MONSTER, YETI, NUM_TYPES };
	int					mSmashedStuff[NUM_TYPES];
};
