//*****************************************************************************
//
//  Copyright (c) 2012-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Game Text entity
// 
//*****************************************************************************

#pragma once

#include "VuEngine/UI/VuUIPropertyUtil.h"
#include "VuEngine/Entities/VuEntity.h"

class VuScriptComponent;
class VuTransitionComponent;


class VuGameTextBaseEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuGameTextBaseEntity();

	virtual void					onGameInitialize();

	virtual const char				*getText() = 0;

protected:
	// event handlers
	virtual void					OnUITick(const VuParams &params);
	virtual void					OnUIDraw(const VuParams &params);

	VuRetVal						Show(const VuParams &params)		{ mVisible = true; return VuRetVal(); }
	VuRetVal						Hide(const VuParams &params)		{ mVisible = false; return VuRetVal(); }
	VuRetVal						SetAlpha(const VuParams &params);

	virtual void					drawLayout(bool bSelected);
	virtual void					drawText(const char *text, float alpha);

	VuVector2						measureString();

	// components
	VuScriptComponent				*mpScriptComponent;
	VuTransitionComponent			*mpTransitionComponent;

	// properties
	bool							mInitiallyVisible;
	VuUIRectProperties				mRect;
	std::string						mFont;
	VuUIStringFormatProperties		mStringFormat;
	VuUIAnchorProperties			mAnchor;

	VuVector2						mOffset; // may be used by derived classes to offset text
	float							mAlpha;
	bool							mVisible;
};

