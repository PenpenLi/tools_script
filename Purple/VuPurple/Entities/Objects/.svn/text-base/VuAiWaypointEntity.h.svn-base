//*****************************************************************************
//
//  Copyright (c) 2009-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  AIWaypoint entity
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/Transform/VuTransformComponent.h"

class Vu3dLayoutComponent;
class VuScriptComponent;
class Vu3dLayoutDrawParams;
class VuTrackSector;


class VuAiWaypointEntity : public VuEntity
{
	DECLARE_RTTI

public:
	// flags
	enum
	{
		IS_START       = 1<<0,
		IS_CHECKPOINT  = 1<<1,
		IS_FINISH      = 1<<2,
		ALLOW_RECOVERY = 1<<3,
		IS_NOAI_BRANCH = 1<<4,
	};

	VuAiWaypointEntity();

	virtual void		onGameInitialize();
	virtual void		onGameRelease();

	VuVector3			getPosition() const;
	VuVector3			getRightEndPoint() const;
	VuVector3			getLeftEndPoint() const;
	
	float				getWidth() const { return mpTransformComponent->getWorldScale().mX; }
	float				getHeight() const { return mpTransformComponent->getWorldScale().mY; }
	
	bool				testIntersection(const VuVector3 &prevPos, const VuVector3 &nextPos, float radius);

	VuRetVal			UpdateBranchProbabilities(const VuParams &params);

	VUUINT32			allowRecovery() { return mFlags & ALLOW_RECOVERY; }
	VUUINT32			isStart() { return mFlags & IS_START; }
	VUUINT32			isFinish() { return mFlags & IS_FINISH; }
	VUUINT32			isCheckPoint() { return mFlags & (IS_START|IS_CHECKPOINT|IS_FINISH); }
	VUUINT32			noAiOnBranch() { return mFlags & IS_NOAI_BRANCH; }

	// properties
	float				mLeftRightRatio;
	float				mSpeedHintMPH;
	VUUINT32			mFlags;
	float				mPrimaryBranchProbability;
	float				mAlternateBranchProbability;
	VuVector2			mCheckPointMinExtents;
	VuVector2			mCheckPointMaxExtents;

	VuAiWaypointEntity	*mpNextPrimaryWaypoint;
	VuAiWaypointEntity	*mpNextAlternateWaypoint;
	VuTrackSector		*mpPrimarySector;
	VuTrackSector		*mpAlternateSector;

protected:
	void				setBranchProbabilities(float a, float b);
	void				normalizeProbabilities(float &a, float &b);

	void				createSectors();
	void				releaseSectors();

	void				drawLayout(const Vu3dLayoutDrawParams &params);

	// components
	Vu3dLayoutComponent	*mp3dLayoutComponent;	
	VuScriptComponent	*mpScriptComponent;
};
