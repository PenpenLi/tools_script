//*****************************************************************************
//
//  Copyright (c) 2008-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Track sector class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Math/VuVector3.h"
#include "VuEngine/Math/VuVector4.h"

class VuAiWaypointEntity;
class VuColor;

class VuTrackSector
{
public:

	VuTrackSector(VuAiWaypointEntity *pWaypoint, VuAiWaypointEntity *pNextWaypoint, float branchProbability);

	float					calcSpeedHint(float t, float currentSpeed) const;

	bool					pointInside(const VuVector3 &point) const;

	float					distanceFromSector(const VuVector3 &point) const;
	void					closestPointInSector(const VuVector3 &point, VuVector3& closest) const;
	
	// Racing Line
	void					closestPointAlongRacingLine(const VuVector3 &point, VuVector3 &closest) const;
	void					pointAlongRacingLine(float t, VuVector3 &point) const;

	float					traversalAmount(const VuVector3 &point) const;

	bool					overlaps(const VuTrackSector *pOtherSector) const;

	// called by VuTrackSectorManager
	void					connect();
	void					addNext(VuTrackSector *pNextSector) { VUASSERT(mNextSectorCount < 2, "too many connections!"); mpNextSectors[mNextSectorCount++] = pNextSector; }
	void					addPrev(VuTrackSector *pPrevSector) { VUASSERT(mPrevSectorCount < 2, "too many connections!"); mpPrevSectors[mPrevSectorCount++] = pPrevSector; }

	void					debugRender(const VuColor &color) const;

	VuVector3				mEnterLeft;
	VuVector3				mEnterRight;
	VuVector3				mExitLeft;
	VuVector3				mExitRight;
	VuVector4				mEnterPlane;
	VuVector4				mRightPlane;
	VuVector4				mLeftPlane;
	VuVector4				mExitPlane;
	VuVector3				mEnterPoint;
	VuVector3				mExitPoint;
	VuVector3				mUnitDir;
	
	float					mLength;
	float					mEnterSpeedHint;
	float					mExitSpeedHint;
	float					mEnterWidth;
	float					mExitWidth;
	
	bool					mAllowRecovery;
	bool					mNoAiOnBranch;

	float					mBranchProbability;

	VUUINT32				mSection;

	VuAiWaypointEntity		*mpWaypoint;
	VuAiWaypointEntity		*mpNextWaypoint;

	int						mNextSectorCount;
	int						mPrevSectorCount;
	VuTrackSector			*mpNextSectors[2];
	VuTrackSector			*mpPrevSectors[2];

	float					mDistanceFromStart;
};