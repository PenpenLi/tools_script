//*****************************************************************************
//
//  Copyright (c) 2008-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Preview entity
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/HAL/Keyboard/VuKeyboard.h"
#include "VuEngine/Gfx/Model/VuStaticModelInstance.h"
#include "VuEngine/Gfx/Model/VuAnimatedModelInstance.h"

class Vu3dDrawComponent;
class VuAnimatedSkeleton;


class VuPreviewEntity : public VuEntity, public VuKeyboard::Callback
{
	DECLARE_RTTI

public:
	VuPreviewEntity();
	~VuPreviewEntity();

	bool					loadScene(const VuJsonContainer &sceneData, const VuJsonContainer &auxData);
	VuAabb					getAabb();
	void					setDrawInfoParams(VUUINT32 flags, const VuColor &textColor, const VuColor &lineColor);

private:
	bool					loadStaticScene(const VuJsonContainer &data);
	bool					loadAnimatedScene(const VuJsonContainer &data);
	bool					loadAnimation(const VuJsonContainer &data);

	void					draw(const VuGfxDrawParams &params);
	void					drawShadow(const VuGfxDrawShadowParams &params);
	void					drawAnimationTimeLine();

	void					tickBuild(float fdt);

	// VuKeyboard::Callback
	virtual void			onKeyDown(VUUINT32 key);

	// components
	Vu3dDrawComponent		*mp3dDrawComponent;

	enum eSceneType { INVALID, STATIC_MODEL, ANIMATED_MODEL, ANIMATION };

	eSceneType				mSceneType;
	VuStaticModelInstance	mStaticModelInstance;
	VuAnimatedModelInstance	mAnimatedModelInstance;
	VuAnimatedSkeleton		*mpAnimatedSkeleton;
	VUUINT32				mDrawInfoFlags;
	VuColor					mDrawInfoTextColor;
	VuColor					mDrawInfoLineColor;
};
