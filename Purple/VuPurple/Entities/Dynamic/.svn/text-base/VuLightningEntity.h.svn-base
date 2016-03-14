//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuLightningEntity
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Entities/VuEntity.h"

class VuCarEntity;
class Vu3dDrawComponent;
class VuGfxDrawParams;

class VuLightningEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuLightningEntity(const VuJsonContainer &data, VuCarEntity *pCar);

	virtual void	onGameInitialize();
	virtual void	onGameRelease();

protected:
	void			tickDecision(float fdt);
	void			draw(const VuGfxDrawParams &params);

	const VuJsonContainer	&mData;

	Vu3dDrawComponent	*mp3dDrawComponent;
	VuCarEntity			*mpLauncher;
	VuCarEntity			*mpTargetCar;

	float				mRange;
	float				mCosTargetAcquisitionCone;
	float				mDisplacement;
	int					mBoltCount;
	int					mDetail;
	float				mLifeTime;
	float				mWidth;
	VuColor				mColor;

	VuVector3			mSrcPos;
	VuVector3			mDstPos;

	float				mAge;
	VUUINT32			mRandomSeed;
};
