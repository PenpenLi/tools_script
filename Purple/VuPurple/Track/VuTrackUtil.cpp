//*****************************************************************************
//
//  Copyright (c) 2008-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Track related utilities
// 
//*****************************************************************************

#include "VuTrackUtil.h"
#include "VuTrackPlan.h"
#include "VuTrackSector.h"
#include "VuTrackManager.h"
#include "VuPurple/Entities/Objects/VuAiWaypointEntity.h"
#include "VuEngine/Math/VuVector2.h"
#include "VuEngine/Math/VuVector3.h"
#include "VuEngine/Math/VuMathUtil.h"
#include "VuEngine/Math/VuMatrix.h"


//*****************************************************************************
float VuTrackUtil::estimatedTrackLeft(const VuVector3 &position, const VuTrackSector *pSector, const VuTrackSector *pStartFinishSector)
{
	VUASSERT(pSector, "NULL sector passed to estimatedTrackLeft");
	VUASSERT(pStartFinishSector, "NULL sector passed to estimatedTrackLeft");

	if(!pSector)
	{
		return 0.0f;
	}

	// How far are we along the current sector?
	float t = pSector->traversalAmount(position);
	
	float distanceLeft = (1.0f - t) * pSector->mLength;

	while(pSector->mNextSectorCount)
	{
		pSector = pSector->mpNextSectors[0];

		if(pSector == pStartFinishSector)
		{
			break;
		}

		distanceLeft += pSector->mLength;
	}

	return distanceLeft;
}

//*****************************************************************************
bool VuTrackUtil::getTransformAlongTrack(float dist, VuMatrix &transform)
{
	VuTrackSector *pLastSector = VuTrackManager::IF()->getLastSector();
	VuTrackSector *pSector = VuTrackManager::IF()->getFirstSector();
	while ( pSector )
	{
		if ( dist < pSector->mLength )
		{
			VuVector3 pos = VuLerp(pSector->mEnterPoint, pSector->mExitPoint, dist/pSector->mLength);
			VuVector3 fwd = VuVector3(pSector->mUnitDir.mX, pSector->mUnitDir.mY, 0.0f).normal();
			VuVector3 up = VuVector3(0.0f, 0.0f, 1.0f);
			VuVector3 right = VuCross(fwd, up);

			transform.loadIdentity();
			transform.setAxisX(right);
			transform.setAxisY(fwd);
			transform.setAxisZ(up);
			transform.setTrans(pos);

			return true;
		}

		dist -= pSector->mLength;

		pSector = (pSector == pLastSector) ? VUNULL : pSector->mpNextSectors[0];
	}

	return false;
}
