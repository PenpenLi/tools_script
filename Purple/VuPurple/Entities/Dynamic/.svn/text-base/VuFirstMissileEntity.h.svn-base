//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Missile PowerUp class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Dynamics/VuDynamics.h"
#include "VuEngine/HAL/Audio/VuAudioEvent.h"

class VuCarEntity;
class VuTrackSector;


class VuFirstMissileEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuFirstMissileEntity(const VuJsonContainer &data, VuCarEntity *pCar);

	virtual void	onGameInitialize();
	virtual void	onGameRelease();

protected:
	void			tickDecision(float fdt);

	void			explode();

	const VuJsonContainer	&mData;

	VuCarEntity			*mpLauncher;
	bool				mTestCars;
	VuMatrix			mTransform;
	VuVector3			mLinearVelocity;
	VuAudioEvent		mStartSfx;
	VUUINT32			mFlightPfx;
	VuAudioEvent		mFlightSfx;
	VuCarEntity			*mpTarget;
	const VuTrackSector	*mpCurSector;
	int					mCarVictimCount;

	float				mHomingRange;
	float				mCosTargetAcquisitionCone;
	float				mAltitute;
	float				mSpeed;
	float				mCarCollisionRadius;
};
