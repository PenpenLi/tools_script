//*****************************************************************************
//
//  Copyright (c) 2009-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  TrackManager class
// 
//*****************************************************************************

#include "VuTrackManager.h"

#include "VuPurple/Entities/Objects/VuAiWaypointEntity.h"
#include "VuTrackSector.h"
#include "VuTrackUtil.h"
#include "VuEngine/Containers/VuStack.h"
#include "VuEngine/Dev/VuDevConfig.h"
#include "VuEngine/Dev/VuDevMenu.h"
#include "VuEngine/Dev/VuDevUtil.h"
#include "VuEngine/Json/VuJsonContainer.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Util/VuColor.h"
#include <stdlib.h>

// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuTrackManager, VuTrackManager);

static bool sDebugDraw = false;


//*****************************************************************************
static int CompareCheckPoints(const void *p1, const void *p2)
{
	const VuAiWaypointEntity *pCP1 = *(const VuAiWaypointEntity **)p1;
	const VuAiWaypointEntity *pCP2 = *(const VuAiWaypointEntity **)p2;

	float dist1 = pCP1->mpPrimarySector[0].mDistanceFromStart;
	float dist2 = pCP2->mpPrimarySector[0].mDistanceFromStart;

	if ( dist1 < dist2 ) return -1;
	if ( dist1 > dist2 ) return 1;

	return 0;
}


//*****************************************************************************
VuTrackManager::VuTrackManager():
	mpFirstSector(VUNULL),
	mpLastSector(VUNULL),
	mTrackLength(0.0f),
	mIsBuilt(false)
{
}

//*****************************************************************************
VuTrackManager::~VuTrackManager()
{
	reset();
}

//*****************************************************************************
bool VuTrackManager::init()
{
	VuTickManager::IF()->registerHandler(this, &VuTrackManager::tickDecision, "Decision");

	sDebugDraw = VuDevConfig::IF()->getParam("DrawTrack").asBool();

	static VuDevBoolOnce sbOnce;

	if(sbOnce)
	{
		VuDevMenu::IF()->addBool("Track/Draw", sDebugDraw);
	}

	return true;
}

//*****************************************************************************
void VuTrackManager::release()
{
	VuTickManager::IF()->unregisterHandlers(this);
}

//*****************************************************************************
VuTrackSector *VuTrackManager::createSector(VuAiWaypointEntity *pWaypoint, VuAiWaypointEntity *pNextWaypoint, float branchProbability)
{
	VuTrackSector *pTrackSector = new VuTrackSector(pWaypoint, pNextWaypoint, branchProbability);
	mTrackSectors.push_back(pTrackSector);

	if ( pWaypoint->isCheckPoint() )
		mCheckPoints.push_back(pWaypoint);

	return pTrackSector;
}

//*****************************************************************************
void VuTrackManager::releaseSector(VuTrackSector *pTrackSector)
{
	VuAiWaypointEntity *pWaypoint = pTrackSector->mpWaypoint;
	if ( pWaypoint->isCheckPoint() )
		mCheckPoints.removeSwap(pWaypoint);

	mTrackSectors.removeSwap(pTrackSector);
	delete pTrackSector;
}

//*****************************************************************************
bool VuTrackManager::buildTrack()
{
	VUASSERT(!isBuilt(), "Track aleady built!");

	// find first sector (start)
	mpFirstSector = VUNULL;
	for ( int i = 0; i < mTrackSectors.size(); i++ )
	{
		VuTrackSector *pSector = mTrackSectors[i];
		VuAiWaypointEntity *pWaypoint = pSector->mpWaypoint;

		if ( pWaypoint->isStart() )
		{
			VUASSERT(mpFirstSector == VUNULL, "Multiple or branching start waypoints!");
			mpFirstSector = pSector;
		}
	}
	VUASSERT(mpFirstSector, "Start not found!");

	// recursively build out branches
	buildBranchRecursive(mpFirstSector->mpWaypoint, -1, 0.0f);

	// TODO: right now this only calculates one route along the track, we 
	// might need to calculate alternative route lengths?
	VuTrackSector *pSector = mpFirstSector;
	do
	{
		mpLastSector = pSector;
		mTrackLength += pSector->mLength;

		if ( pSector->mNextSectorCount )
			pSector = pSector->mpNextSectors[0];
		else
			pSector = VUNULL;

	// in a point to point race we will come to a NULL sector, in a circuit 
	// race we will come back to the first sector
	} while(pSector && pSector != mpFirstSector);

	// sort checkpoints
	VUASSERT(mCheckPoints.size(), "");
	qsort(&mCheckPoints[0], mCheckPoints.size(), sizeof(mCheckPoints[0]), CompareCheckPoints);

	
	mIsBuilt = true;

	return true;
}

//*****************************************************************************
bool VuTrackManager::isCircuit() const
{
	VUASSERT(isBuilt(), "Track not built");

	return mpLastSector->mNextSectorCount > 0;
}

//*****************************************************************************
void VuTrackManager::reset()
{
	mpFirstSector = VUNULL;
	mpLastSector = VUNULL;
	mTrackLength = 0.0f;

	mIsBuilt = false;
}

//*****************************************************************************
VuTrackSector *VuTrackManager::getFirstSector() const
{
	VUASSERT(isBuilt(), "getFirstSector called on uninitialized track");

	return mpFirstSector;
}

//*****************************************************************************
VuTrackSector *VuTrackManager::getLastSector() const
{
	VUASSERT(isBuilt(), "getLastSector called on uninitialized track");

	return mpLastSector;
}

//*****************************************************************************
float VuTrackManager::getLength() const
{
	VUASSERT(isBuilt(), "getLength called on uninitialized track");

	return mTrackLength;
}

//*****************************************************************************
VuAiWaypointEntity *VuTrackManager::getNextCheckPoint(const VuAiWaypointEntity *pCheckPoint)
{
	for ( int iCheckPoint = 0; iCheckPoint < mCheckPoints.size(); iCheckPoint++ )
	{
		if ( mCheckPoints[iCheckPoint] == pCheckPoint )
		{
			int iNextCheckPoint = iCheckPoint + 1;

			if ( isCircuit() )
				iNextCheckPoint %= mCheckPoints.size();

			if ( iNextCheckPoint >= mCheckPoints.size() )
				return VUNULL;

			return mCheckPoints[iNextCheckPoint];
		}
	}

	return VUNULL;
}

//*****************************************************************************
VuTrackSector *VuTrackManager::findSectorFromPosition(const VuVector3 &pos) const
{
	VUASSERT(isBuilt(), "findSectorFromPosition called on uninitialized track");

	float closestDistance = FLT_MAX;
	VuTrackSector *pClosestSector = VUNULL;

	for ( int iSector = 0; iSector < mTrackSectors.size(); iSector++ )
	{
		VuTrackSector *pSector = mTrackSectors[iSector];

		float distanceFromSector = pSector->distanceFromSector(pos);
		if ( distanceFromSector < closestDistance )
		{
			closestDistance = distanceFromSector;
			pClosestSector = pSector;
		}
	}

	return pClosestSector;
}

//*****************************************************************************
VuTrackSector *VuTrackManager::findSectorFromPosition(const VuVector3 &pos, VUUINT32 section) const
{
	VUASSERT(isBuilt(), "findSectorFromPosition called on uninitialized track");

	float closestDistance = FLT_MAX;
	VuTrackSector *pClosestSector = VUNULL;

	for ( int iSector = 0; iSector < mTrackSectors.size(); iSector++ )
	{
		VuTrackSector *pSector = mTrackSectors[iSector];

		if ( pSector->mSection == section )
		{
			float distanceFromSector = pSector->distanceFromSector(pos);
			if ( distanceFromSector < closestDistance )
			{
				closestDistance = distanceFromSector;
				pClosestSector = pSector;
			}
		}
	}

	return pClosestSector;
}

//*****************************************************************************
VuTrackSector *VuTrackManager::findSectorFromPosition(const VuVector3 &pos, VUUINT32 section, VUUINT32 nextSection) const
{
	VUASSERT(isBuilt(), "findSectorFromPosition called on uninitialized track");

	float closestDistance = FLT_MAX;
	VuTrackSector *pClosestSector = VUNULL;

	for ( int iSector = 0; iSector < mTrackSectors.size(); iSector++ )
	{
		VuTrackSector *pSector = mTrackSectors[iSector];

		if ( (pSector->mSection == section) || (pSector->mSection == nextSection) )
		{
			float distanceFromSector = pSector->distanceFromSector(pos);
			if ( distanceFromSector < closestDistance )
			{
				closestDistance = distanceFromSector;
				pClosestSector = pSector;
			}
		}
	}

	return pClosestSector;
}

//*****************************************************************************
VuTrackSector *VuTrackManager::findRecoverySector(float distanceFromStartOfLap, const VuVector3 &pos, VuVector3 &recoveryPos) const
{
	VUASSERT(isBuilt(), "findRecoverySectorFromPosition called on uninitialized track");

	distanceFromStartOfLap = VuMax(distanceFromStartOfLap, 0.0f);

	// find sectors that encompass the distance from start of lap
	VuTrackSector *pBestSector = VUNULL;
	float bestDistanceFromRacingLine = FLT_MAX;
	float bestDistanceAlongRacingLine = FLT_MAX;
	for ( int iSector = 0; iSector < mTrackSectors.size(); iSector++ )
	{
		VuTrackSector *pSector = mTrackSectors[iSector];

		// sector must be allowed to recover
		if ( pSector->mAllowRecovery )
		{
			// sector start must be before recovery distance
			if ( pSector->mDistanceFromStart <= distanceFromStartOfLap )
			{
				float distanceFromStartOfSector = distanceFromStartOfLap - pSector->mDistanceFromStart;
				if ( distanceFromStartOfSector < pSector->mLength )
				{
					// recovery distance falls within sector
					VuVector3 posAlongRacingLine;
					pSector->pointAlongRacingLine(distanceFromStartOfSector/pSector->mLength, posAlongRacingLine);
					float distanceFromRacingLine = VuDist(pos, posAlongRacingLine);
					if ( distanceFromRacingLine < bestDistanceFromRacingLine )
					{
						pBestSector = pSector;
						bestDistanceFromRacingLine = distanceFromRacingLine;
						bestDistanceAlongRacingLine = 0.0f;
						recoveryPos = posAlongRacingLine;
					}
				}
				else
				{
					// recovery distance falls outside sector
					float distanceAlongRacingLine = distanceFromStartOfSector - pSector->mLength;
					if ( distanceAlongRacingLine < bestDistanceAlongRacingLine )
					{
						pBestSector = pSector;
						bestDistanceAlongRacingLine = distanceAlongRacingLine;
						recoveryPos = pSector->mExitPoint;
					}
				}
			}
		}
	}

	// this can happen if the first sector is marked as 'not recoverable'
	if ( pBestSector == VUNULL )
	{
		pBestSector = getFirstSector();
		recoveryPos = pBestSector->mEnterPoint;
	}

	return pBestSector;
}

//*****************************************************************************
void VuTrackManager::buildBranchRecursive(VuAiWaypointEntity *pWaypoint, int section, float distanceFromStart)
{
	if ( pWaypoint->isCheckPoint() )
		section++;

	VuTrackSector *pPrimarySector = pWaypoint->mpPrimarySector;

	// dead end?
	if ( !pPrimarySector )
		return;

	// already built?
	if ( pPrimarySector->mNextSectorCount )
		return;

	// primary branch
	{
		pPrimarySector->mSection = section;
		pPrimarySector->mDistanceFromStart = distanceFromStart;
		pPrimarySector->connect();
		buildBranchRecursive(pWaypoint->mpNextPrimaryWaypoint, section, distanceFromStart + pPrimarySector->mLength);
	}

	// alternate branch
	if ( VuTrackSector *pAlternateSector = pWaypoint->mpAlternateSector )
	{
		if (pWaypoint->isCheckPoint())
		{
			VUPRINTF("Error: Waypoint %s is a CheckPoint and has a Branch. This is illegal. Madness will now ensue.\n");
			VUASSERT(0, "Checkpoint has track branches! This will cause problems. Please add another Waypoint to do the branching.");
		}
		
		pAlternateSector->mSection = section;
		pAlternateSector->mDistanceFromStart = distanceFromStart;
		pAlternateSector->connect();
		buildBranchRecursive(pWaypoint->mpNextAlternateWaypoint, section, distanceFromStart + pAlternateSector->mLength);
	}
}

//*****************************************************************************
void VuTrackManager::tickDecision(float fdt)
{
	if ( sDebugDraw )
		debugRender();
}

//*****************************************************************************
void VuTrackManager::debugRender()
{
	for ( int iSector = 0; iSector < mTrackSectors.size(); iSector++ )
	{
		const VuTrackSector *pSector = mTrackSectors[iSector];

		VuColor color;
		if ( pSector->mAllowRecovery )
			color.set(255,255,255);
		else
			color.set(255,0,0);

		pSector->debugRender(color);
	}
}