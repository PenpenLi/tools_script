//*****************************************************************************
//
//  Copyright (c) 2012-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Game Image entity
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/UI/VuUIPropertyUtil.h"

class VuScriptComponent;
class VuTransitionComponent;


class VuGameImageBaseEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuGameImageBaseEntity();

	virtual void		onGameInitialize();

	virtual VuTexture	*getTexture() = 0;

protected:
	// event handlers
	virtual void					OnUITick(const VuParams &params);
	virtual void					OnUIDraw(const VuParams &params);

	VuRetVal						Show(const VuParams &params)		{ mVisible = true; return VuRetVal(); }
	VuRetVal						Hide(const VuParams &params)		{ mVisible = false; return VuRetVal(); }

	virtual void					drawLayout(bool bSelected)	{ drawImage(1.0f); }
	virtual void					drawImage(float alpha);

	// components
	VuScriptComponent				*mpScriptComponent;
	VuTransitionComponent			*mpTransitionComponent;

	// properties
	bool							mInitiallyVisible;
	VuColor							mColor;
	VuUIRectProperties				mDstRect;
	VuUIRectProperties				mSrcRect;
	float							mRotation;
	bool							mFlipX;
	bool							mFlipY;
	VuUIAnchorProperties			mAnchor;

	float							mAlpha;
	bool							mVisible;
};
