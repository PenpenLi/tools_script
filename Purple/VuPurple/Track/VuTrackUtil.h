//*****************************************************************************
//
//  Copyright (c) 2008-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Track related utilities
// 
//*****************************************************************************

#pragma once

class VuTrackPlan;
class VuTrackSector;
class VuAiWaypointEntity;
class VuVector2;
class VuVector3;
class VuMatrix;

namespace VuTrackUtil
{
	float				estimatedTrackLeft(const VuVector3 &position, const VuTrackSector *pSector, const VuTrackSector *pStartFinishSector);
	bool				getTransformAlongTrack(float dist, VuMatrix &transform);
}