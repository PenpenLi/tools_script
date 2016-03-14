//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  UI List class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/UI/VuUIPropertyUtil.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Properties/VuAssetProperty.h"

class VuScriptComponent;
class VuTexture;
class VuTextureAsset;
class VuScriptPlug;


class VuSliderEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuSliderEntity();

protected:
	virtual float		getValue() = 0;
	virtual void		setValue(float value) = 0;
	virtual bool		isEnabled() { return true; }

	int					getBars();
	void				setBars(int bars);

	// scripting
	VuRetVal			SetFocus(const VuParams &params = VuParams());
	VuRetVal			Show(const VuParams &params);
	VuRetVal			Hide(const VuParams &params);
	VuRetVal			IsEnabled(const VuParams &params) { return VuRetVal(isEnabled()); }

	// event handlers
	void				OnUITick(const VuParams &params);
	void				OnUIGamePad(const VuParams &params);
	void				OnUIDraw(const VuParams &params);

	void				drawLayout(bool bSelected);
	void				drawSlider(float alpha);

	// components
	VuScriptComponent				*mpScriptComponent;

	// properties
	bool							mVisible;
	VuUIImageProperties				mFrameImage;
	VuUIImageProperties				mSelectedFrameImage;
	VuUIRectProperties				mFrameRect;
	VuUIRectProperties				mTextRect;
	std::string						mFont;
	VuUIStringFormatProperties		mStringFormat;
	std::string						mStringID;
	VuUIImageProperties				mBarImage;
	VuUIRectProperties				mBarDstRect;
	VuVector2						mBarDstDelta;
	VuUIRectProperties				mBarSrcRect;
	VuVector2						mBarSrcDelta;
	VuVector2						mBarSrcOnDelta;
	int								mBarCount;
	VuUIAnchorProperties			mAnchor;
};
