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


class VuCheckBoxBaseEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuCheckBoxBaseEntity();

protected:
	virtual bool		isChecked() = 0;
	virtual void		setChecked(bool checked) = 0;
	virtual bool		isEnabled() { return true; }
	VuRetVal			IsChecked(const VuParams &params) { return VuRetVal(isChecked()); }

	// event handlers
	void				OnUITick(const VuParams &params);
	void				OnUIGamePad(const VuParams &params);
	void				OnUITouch(const VuParams &params);
	void				OnUIDraw(const VuParams &params);

	// scripting
	VuRetVal			SetFocus(const VuParams &params = VuParams());
	VuRetVal			Show(const VuParams &params);
	VuRetVal			Hide(const VuParams &params);
	VuRetVal			IsEnabled(const VuParams &params) { return VuRetVal(isEnabled()); }

	void				drawLayout(bool bSelected);
	void				drawCheckBox(float alpha);

	void				toggleCheckState();

	// components
	VuScriptComponent				*mpScriptComponent;

	// properties
	bool							mVisible;
	VuUIImageProperties				mFrameImage;
	VuUIImageProperties				mSelectedFrameImage;
	VuUIRectProperties				mFrameRect;
	VuUIRectProperties				mTouchRect;
	VuUIRectProperties				mTextRect;
	std::string						mFont;
	VuUIStringFormatProperties		mStringFormat;
	std::string						mStringID;
	VuUIImageProperties				mCheckedImage;
	VuUIImageProperties				mUncheckedImage;
	VuUIRectProperties				mCheckRect;
	VuUIAnchorProperties			mAnchor;
};
