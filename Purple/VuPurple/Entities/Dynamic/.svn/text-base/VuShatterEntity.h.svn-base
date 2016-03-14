//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuShatterEntity
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Gfx/Model/VuBreakableModelInstance.h"

class VuMatrix;
class Vu3dDrawComponent;
class VuGfxDrawParams;
class VuGfxDrawShadowParams;


class VuShatterEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuShatterEntity(const VuJsonContainer &data, const VuMatrix &transform, const VuVector3 &linVel, const VuColor &color);

	virtual void	onGameInitialize();
	virtual void	onGameRelease();

protected:
	void			tickBuild(float fdt);

	void			draw(const VuGfxDrawParams &params);

	Vu3dDrawComponent			*mp3dDrawComponent;

	VuMatrix					mTransform;
	VuBreakableModelInstance	mBreakableModelInstance;
	float						mFadeDelay;
	float						mFadeTime;
	float						mDrawDistance;
	VuColor						mColor;

	float						mAge;
};
