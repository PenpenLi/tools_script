//*****************************************************************************
//
//  Copyright (c) 2008-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Track Sector class
// 
//*****************************************************************************

#include "VuTrackSector.h"
#include "VuPurple/Entities/Objects/VuAiWaypointEntity.h"
#include "VuEngine/HAL/Gfx/VuGfxTypes.h"
#include "VuEngine/Math/VuMathUtil.h"
#include "VuEngine/Math/VuUnitConversion.h"
#include "VuEngine/Dev/VuDev.h"
#include "VuEngine/Util/VuColor.h"


//*****************************************************************************
VuTrackSector::	VuTrackSector(VuAiWaypointEntity *pWaypoint, VuAiWaypointEntity *pNextWaypoint, float branchProbability):
	mpWaypoint(pWaypoint),
	mpNextWaypoint(pNextWaypoint),
	mBranchProbability(branchProbability),
	mSection(0),
	mNextSectorCount(0),
	mPrevSectorCount(0),
	mNoAiOnBranch(false),
	mDistanceFromStart(0.0f)
{
	mEnterLeft = pWaypoint->getLeftEndPoint();
	mEnterRight = pWaypoint->getRightEndPoint();
	mEnterPoint = pWaypoint->getPosition();
	mExitLeft = pNextWaypoint->getLeftEndPoint();
	mExitRight = pNextWaypoint->getRightEndPoint();
	mExitPoint = pNextWaypoint->getPosition();

	mEnterSpeedHint = VuMphToMetersPerSecond(pWaypoint->mSpeedHintMPH);
	mExitSpeedHint = VuMphToMetersPerSecond(pNextWaypoint->mSpeedHintMPH);

	mAllowRecovery = pWaypoint->allowRecovery() && pNextWaypoint->allowRecovery();
	mNoAiOnBranch = (pWaypoint->noAiOnBranch() != 0);

	VuVector3 up = VuCross(mExitRight - mEnterRight, mEnterLeft - mEnterRight);
	up.normalize();

	VuVector3 enterNormal = VuCross(mEnterLeft - mEnterRight, up);
	enterNormal.normalize();

	VuVector3 rightNormal = VuCross(mEnterRight - mExitRight, up);
	rightNormal.normalize();

	VuVector3 leftNormal = VuCross(mExitLeft - mEnterLeft, up);
	leftNormal.normalize();

	VuVector3 exitNormal = VuCross(mExitRight - mExitLeft, up);
	exitNormal.normalize();

	mEnterPlane = VuMathUtil::planeFromNormalPoint(enterNormal, mEnterRight);
	mRightPlane = VuMathUtil::planeFromNormalPoint(rightNormal, mEnterRight);
	mLeftPlane = VuMathUtil::planeFromNormalPoint(leftNormal, mExitLeft);
	mExitPlane = VuMathUtil::planeFromNormalPoint(exitNormal, mExitLeft);

	mUnitDir = (mExitPoint - mEnterPoint);
	mLength = mUnitDir.normalize();
	mEnterWidth = VuDist(mEnterRight, mEnterLeft);
	mExitWidth = VuDist(mExitRight, mExitLeft);
}

//*****************************************************************************
float VuTrackSector::calcSpeedHint(float t, float currentSpeed) const
{
	float a = mEnterSpeedHint > 0.0f ? mEnterSpeedHint : currentSpeed;
	float b = mExitSpeedHint > 0.0f ? mExitSpeedHint : currentSpeed;

	return VuLerp(a, b, VuClamp(t, 0.0f, 1.0f));
}

//*****************************************************************************
bool VuTrackSector::pointInside(const VuVector3 &point) const
{
	float test = FLT_MAX;

	test = VuMin(test, VuMathUtil::distPointPlane(point, mEnterPlane));
	test = VuMin(test, VuMathUtil::distPointPlane(point, mRightPlane));
	test = VuMin(test, VuMathUtil::distPointPlane(point, mLeftPlane));
	test = VuMin(test, VuMathUtil::distPointPlane(point, mExitPlane));

	return test >= 0.0f;
}

//*****************************************************************************
float VuTrackSector::distanceFromSector(const VuVector3 &point) const
{
	if(pointInside(point))
	{
		return 0.0f;
	}

	float d1 = VuMathUtil::distPointLineSeg(point, mExitLeft, mExitRight);
	float d2 = VuMathUtil::distPointLineSeg(point, mExitRight, mEnterRight);
	float d3 = VuMathUtil::distPointLineSeg(point, mEnterRight, mEnterLeft);
	float d4 = VuMathUtil::distPointLineSeg(point, mEnterLeft, mExitLeft);

	return VuMin(d1, VuMin(d2, VuMin(d3, d4)));
/*
	return VuMathUtil::distPointLineSeg(point, mEnterPoint, mExitPoint);
*/
}

//*****************************************************************************
void VuTrackSector::closestPointInSector(const VuVector3 &point, VuVector3& closest) const
{
	if (pointInside(point))
	{
		closest = point;
		return;
	}

	VuVector3 p[4];

	VuMathUtil::closestPointLineSeg(point, mExitLeft, mExitRight, p[0]);
	VuMathUtil::closestPointLineSeg(point, mExitRight, mEnterRight, p[1]);
	VuMathUtil::closestPointLineSeg(point, mEnterRight, mEnterLeft, p[2]);
	VuMathUtil::closestPointLineSeg(point, mEnterLeft, mExitLeft, p[3]);

	float closestDist = FLT_MAX;

	for (int i = 0; i < 4; i++)
	{
		float dist = VuDist2d(p[i], point);
		if (dist < closestDist)
		{
			closestDist = dist;
			closest = p[i];
		}
	}
}

//*****************************************************************************
void VuTrackSector::closestPointAlongRacingLine(const VuVector3 &point, VuVector3 &closest) const
{
	VuMathUtil::closestPointLineSeg(point, mEnterPoint, mExitPoint, closest);
}

//*****************************************************************************
void VuTrackSector::pointAlongRacingLine(float t, VuVector3 &point) const
{
	point = VuLerp(mEnterPoint, mExitPoint, t);
}

//*****************************************************************************
float VuTrackSector::traversalAmount(const VuVector3 &point) const
{
	float enterDistance = VuMathUtil::distPointPlane(point, mEnterPlane);
	float exitDistance = VuMathUtil::distPointPlane(point, mExitPlane);

	float t = enterDistance / (enterDistance + exitDistance);

	return t;
}

//*****************************************************************************
// Ok, this is inefficient, but it's rarely used
//*****************************************************************************
bool VuTrackSector::overlaps(const VuTrackSector *pOtherSector) const
{
	float test;

	test = -FLT_MAX;
	test = VuMax(test, VuMathUtil::distPointPlane(pOtherSector->mEnterLeft, mLeftPlane));
	test = VuMax(test, VuMathUtil::distPointPlane(pOtherSector->mEnterRight, mLeftPlane));
	test = VuMax(test, VuMathUtil::distPointPlane(pOtherSector->mExitLeft, mLeftPlane));
	test = VuMax(test, VuMathUtil::distPointPlane(pOtherSector->mExitRight, mLeftPlane));

	if ( test <= 0 )
		return false;

	test = -FLT_MAX;
	test = VuMax(test, VuMathUtil::distPointPlane(pOtherSector->mEnterLeft, mRightPlane));
	test = VuMax(test, VuMathUtil::distPointPlane(pOtherSector->mEnterRight, mRightPlane));
	test = VuMax(test, VuMathUtil::distPointPlane(pOtherSector->mExitLeft, mRightPlane));
	test = VuMax(test, VuMathUtil::distPointPlane(pOtherSector->mExitRight, mRightPlane));

	if ( test <= 0 )
		return false;

	test = -FLT_MAX;
	test = VuMax(test, VuMathUtil::distPointPlane(pOtherSector->mEnterLeft, mEnterPlane));
	test = VuMax(test, VuMathUtil::distPointPlane(pOtherSector->mEnterRight, mEnterPlane));
	test = VuMax(test, VuMathUtil::distPointPlane(pOtherSector->mExitLeft, mEnterPlane));
	test = VuMax(test, VuMathUtil::distPointPlane(pOtherSector->mExitRight, mEnterPlane));

	if ( test <= 0 )
		return false;

	test = -FLT_MAX;
	test = VuMax(test, VuMathUtil::distPointPlane(pOtherSector->mEnterLeft, mExitPlane));
	test = VuMax(test, VuMathUtil::distPointPlane(pOtherSector->mEnterRight, mExitPlane));
	test = VuMax(test, VuMathUtil::distPointPlane(pOtherSector->mExitLeft, mExitPlane));
	test = VuMax(test, VuMathUtil::distPointPlane(pOtherSector->mExitRight, mExitPlane));

	if ( test <= 0 )
		return false;

	test = -FLT_MAX;
	test = VuMax(test, VuMathUtil::distPointPlane(mEnterLeft, pOtherSector->mLeftPlane));
	test = VuMax(test, VuMathUtil::distPointPlane(mEnterRight, pOtherSector->mLeftPlane));
	test = VuMax(test, VuMathUtil::distPointPlane(mExitLeft, pOtherSector->mLeftPlane));
	test = VuMax(test, VuMathUtil::distPointPlane(mExitRight, pOtherSector->mLeftPlane));

	if ( test <= 0 )
		return false;

	test = -FLT_MAX;
	test = VuMax(test, VuMathUtil::distPointPlane(mEnterLeft, pOtherSector->mRightPlane));
	test = VuMax(test, VuMathUtil::distPointPlane(mEnterRight, pOtherSector->mRightPlane));
	test = VuMax(test, VuMathUtil::distPointPlane(mExitLeft, pOtherSector->mRightPlane));
	test = VuMax(test, VuMathUtil::distPointPlane(mExitRight, pOtherSector->mRightPlane));

	if ( test <= 0 )
		return false;

	test = -FLT_MAX;
	test = VuMax(test, VuMathUtil::distPointPlane(mEnterLeft, pOtherSector->mEnterPlane));
	test = VuMax(test, VuMathUtil::distPointPlane(mEnterRight, pOtherSector->mEnterPlane));
	test = VuMax(test, VuMathUtil::distPointPlane(mExitLeft, pOtherSector->mEnterPlane));
	test = VuMax(test, VuMathUtil::distPointPlane(mExitRight, pOtherSector->mEnterPlane));

	if ( test <= 0 )
		return false;

	test = -FLT_MAX;
	test = VuMax(test, VuMathUtil::distPointPlane(mEnterLeft, pOtherSector->mExitPlane));
	test = VuMax(test, VuMathUtil::distPointPlane(mEnterRight, pOtherSector->mExitPlane));
	test = VuMax(test, VuMathUtil::distPointPlane(mExitLeft, pOtherSector->mExitPlane));
	test = VuMax(test, VuMathUtil::distPointPlane(mExitRight, pOtherSector->mExitPlane));

	if ( test <= 0 )
		return false;

	return true;
}

//*****************************************************************************
void VuTrackSector::connect()
{
	if ( mpNextWaypoint->mpPrimarySector )
	{
		addNext(mpNextWaypoint->mpPrimarySector);
		mpNextWaypoint->mpPrimarySector->addPrev(this);
	}

	if ( mpNextWaypoint->mpAlternateSector )
	{
		addNext(mpNextWaypoint->mpAlternateSector);
		mpNextWaypoint->mpAlternateSector->addPrev(this);
	}
}

//*****************************************************************************
void VuTrackSector::debugRender(const VuColor &color) const
{
	VuDev::IF()->drawLine(mExitLeft, mExitRight, color);
	VuDev::IF()->drawLine(mExitRight, mEnterRight, color);
	VuDev::IF()->drawLine(mEnterRight, mEnterLeft, color);
	VuDev::IF()->drawLine(mEnterLeft, mExitLeft, color);

	VuDev::IF()->drawLine(mEnterPoint, mExitPoint, color);

	VuVector3 pt = mEnterPoint + (mUnitDir * mLength * 0.5f);

//	VuDev::IF()->drawLine(mEnterPointProLine, mExitPointProLine, VuColor(100, 255, 100));
//	VuDev::IF()->drawLine(mEnterPointShortcutLine, mExitPointShortcutLine, VuColor(255, 100, 100));

	VuDev::IF()->printf(pt, VUGFX_TEXT_DRAW_HCENTER|VUGFX_TEXT_DRAW_VCENTER, color, "%d", mSection);
}
