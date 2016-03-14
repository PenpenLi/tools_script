//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Best Line Effect entity
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Dynamics/VuDynamics.h"
#include "VuEngine/HAL/Audio/VuAudioEvent.h"

class VuCarEntity;
class VuPowerUp;
class VuTrackSector;


class VuBestLineEffectEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuBestLineEffectEntity(const VuJsonContainer &data, VuCarEntity *pCar);

	virtual void	onGameInitialize();
	virtual void	onGameRelease();

protected:
	void			tickDecision(float fdt);

	const VuJsonContainer	&mData;

	VuCarEntity			*mpLauncher;
	VuMatrix			mTransform;
	VuVector3			mLinearVelocity;
	VuAudioEvent		mStartSfx;
	VUUINT32			mFlightPfx;
	VuAudioEvent		mFlightSfx;
	const VuTrackSector	*mpCurSector;
	bool				mFading;
	float				mFadeTimer;
	float				mAge;

	float				mAltitute;
	float				mSpeed;
	float				mLifeTime;
	float				mFadeTime;
	float				mCarApplyRadius;

	typedef std::set<VuCarEntity *> CarsAffected;
	CarsAffected		mCarsAffected;
};
