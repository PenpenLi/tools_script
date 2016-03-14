//*****************************************************************************
//
//  Copyright (c) 2009-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  TrackManager class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/VuSystemComponent.h"
#include "VuEngine/Containers/VuArray.h"

class VuAiWaypointEntity;
class VuTrackSector;
class VuVector3;

class VuTrackManager : public VuSystemComponent
{
	DECLARE_SYSTEM_COMPONENT(VuTrackManager)

protected:
	friend class VuPurple;

	virtual bool init();
	virtual void release();

public:
	VuTrackManager();
	~VuTrackManager();

	VuTrackSector		*createSector(VuAiWaypointEntity *pWaypoint, VuAiWaypointEntity *pNextWaypoint, float branchProbability);
	void				releaseSector(VuTrackSector *pTrackSector);

	bool				buildTrack();
	void				reset();

	bool				isBuilt() const { return mIsBuilt; }
	bool				isCircuit() const;

	VuTrackSector		*getFirstSector() const;
	VuTrackSector		*getLastSector() const;

	float				getLength() const;

	int					getCheckPointCount() { return mCheckPoints.size(); }
	VuAiWaypointEntity	*getCheckPoint(int section) { return mCheckPoints[section]; }
	VuAiWaypointEntity	*getNextCheckPoint(int section) { return getNextCheckPoint(getCheckPoint(section)); }
	VuAiWaypointEntity	*getNextCheckPoint(const VuAiWaypointEntity *pCheckPoint);

	VuTrackSector		*findSectorFromPosition(const VuVector3 &pos) const;
	VuTrackSector		*findSectorFromPosition(const VuVector3 &pos, VUUINT32 section) const;
	VuTrackSector		*findSectorFromPosition(const VuVector3 &pos, VUUINT32 section, VUUINT32 nextSection) const;
	VuTrackSector		*findRecoverySector(float distanceFromStartOfLap, const VuVector3 &pos, VuVector3 &recoveryPos) const;

protected:
	void			buildBranchRecursive(VuAiWaypointEntity *pWaypoint, int section, float distanceFromStart);

	void			tickDecision(float fdt);

	void			debugRender();

	typedef VuArray<VuTrackSector *> TrackSectors;
	typedef VuArray<VuAiWaypointEntity *> CheckPoints;

	TrackSectors	mTrackSectors;
	VuTrackSector	*mpFirstSector;
	VuTrackSector	*mpLastSector;
	float			mTrackLength;

	CheckPoints		mCheckPoints;

	bool			mIsBuilt;
};
