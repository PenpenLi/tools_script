//*****************************************************************************
//
//  Copyright (c) 2009-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Track Plan Class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Containers/VuArray.h"
#include "VuEngine/Math/VuRand.h"
#include "VuEngine/Math/VuVector3.h"

class VuTrackSector;
class VuCarEntity;

#define MAX_PLAN_SECTOR_INFOS		16

enum eAiLane						{ LANE_LEFT, LANE_CENTER, LANE_RIGHT, LANE_MAX };

class VuRigidBody;

class VuPlanSectorInfo
{
public:
	VuPlanSectorInfo() : mCumulativeTurn(0.0f), mCumulativeTurnRatio(0.0f), mTurnAngleHere(0.0f), mBlockedLane(LANE_MAX), mShortCut(false) {}

	float mCumulativeTurn;
	float mCumulativeTurnRatio;
	float mTurnAngleHere;		// Positive to the left, negative to the right
	eAiLane mBlockedLane;
	bool mShortCut;
};

//*****************************************************************************
class VuTrackPlan
{
public:
	enum eTrackPlanType { PLAN_MAIN_TRACK, PLAN_RANDOM, PLAN_SHORTCUTS, PLAN_RANDOM_SHORTCUTS};

	VuTrackPlan(int maxDepth);
	virtual ~VuTrackPlan();

	bool							createPlan(VuCarEntity* pCar, const VuTrackSector *pStartingSector);
	bool							update();

	void							setPlanType(eTrackPlanType planType) { mPlanType = planType; }

	int								getStepCount() const { return mSectors.size(); }
	const VuTrackSector*			getSectorForStep(int step) const;
	const VuPlanSectorInfo*			getSectorInfoForStep(int step) const;

	bool							includes(const VuTrackSector *pSector) const;
	bool							hasSpeedHint();

	// "bendiness" factor of the track represented in the plan
	float							getTrackBendiness() { return mBendiness; }

	float							computeBendinessAt(int depth);
	float							computeBendinessAt(int start, int end);
	float							computeBendinessAtDist(float dist);
	float							computeBendinessAtTime(float lookAheadTime);
	float							computeCornerAngle(float lookAheadTime);

	float							getDistanceToNextShortcut();


protected:
	VuCarEntity*					getCar() { return mpCar; }

	const VuTrackSector*			onChooseNextSector(const VuTrackSector *pSector);

private:
	void							deallocatePlanInfo();

	bool							fillOutPlan();
	void							findObstacles();
	void							analyzePlan();

	VuPlanSectorInfo*				getPlanSectorInfo(int index);

	// Instance Vars
	//
	eTrackPlanType					mPlanType;
	float							mBendiness; // cumulative angle ratio * 100 to give values from 0.0-ish to 2.0-ish
	int								mMaxDepth;
	VuArray<const VuTrackSector*>	mSectors;

	VuCarEntity*					mpCar;

	// Storage for sector info from plan analysis
	VuArray<VuPlanSectorInfo*>		mPlanSectorInfos;
};
