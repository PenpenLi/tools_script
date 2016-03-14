//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  UI CheckBox class
// 
//*****************************************************************************

#include "VuCheckBoxEntity.h"
#include "VuPurple/Managers/VuControlMethodManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/UI/VuUI.h"
#include "VuEngine/UI/VuUIDrawUtil.h"
#include "VuEngine/UI/VuUIInputUtil.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Components/2dLayout/Vu2dLayoutComponent.h"
#include "VuEngine/Properties/VuBasicProperty.h"
#include "VuEngine/Properties/VuFontEnumProperty.h"
#include "VuEngine/Assets/VuFontAsset.h"
#include "VuEngine/Assets/VuTextureAsset.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/Font/VuFontDraw.h"
#include "VuEngine/Gfx/GfxSort/VuGfxSort.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Math/VuMathUtil.h"
#include "VuEngine/DB/VuStringDB.h"


//*****************************************************************************
// CheckBoxBase Entity
//*****************************************************************************

IMPLEMENT_RTTI(VuCheckBoxBaseEntity, VuEntity);

//*****************************************************************************
VuCheckBoxBaseEntity::VuCheckBoxBaseEntity():
	mVisible(true)
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));
	addComponent(new Vu2dLayoutComponent(this, &VuCheckBoxBaseEntity::drawLayout));

	// properties
	addProperty(new VuBoolProperty("Visible", mVisible));
	mFrameImage.addProperties(getProperties(), "Frame Image");
	mSelectedFrameImage.addProperties(getProperties(), "Selected Frame Image");
	addProperty(new VuRectProperty("Frame Rect", mFrameRect));
	addProperty(new VuRectProperty("Touch Rect", mTouchRect));
	addProperty(new VuRectProperty("Text Rect", mTextRect));
	addProperty(new VuFontEnumProperty("Font", mFont));
	ADD_UI_STRING_FORMAT_PROPERTIES(getProperties(), mStringFormat, "");
	addProperty(new VuStringProperty("String ID", mStringID));
	mCheckedImage.addProperties(getProperties(), "Checked Image");
	mUncheckedImage.addProperties(getProperties(), "Unchecked Image");
	addProperty(new VuRectProperty("Check Rect", mCheckRect));
	ADD_UI_ANCHOR_PROPERTIES(getProperties(), mAnchor, "");

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuCheckBoxBaseEntity, SetFocus);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuCheckBoxBaseEntity, Show);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuCheckBoxBaseEntity, Hide);
	ADD_SCRIPT_INPUT(mpScriptComponent, VuCheckBoxBaseEntity, IsEnabled, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT(mpScriptComponent, VuCheckBoxBaseEntity, IsChecked, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnChecked);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnUnchecked);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, NavUp);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, NavDown);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, NavLeft);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, NavRight);

	// event handlers
	REG_EVENT_HANDLER(VuCheckBoxBaseEntity, OnUIGamePad);
	REG_EVENT_HANDLER(VuCheckBoxBaseEntity, OnUITouch);
	REG_EVENT_HANDLER(VuCheckBoxBaseEntity, OnUIDraw);
}

//*****************************************************************************
void VuCheckBoxBaseEntity::OnUIGamePad(const VuParams &params)
{
	if ( VuUI::IF()->getFocus() == this )
	{
		VuParams::VuAccessor accessor(params);
		int channel = accessor.getInt();
		bool down = accessor.getBool();
		int padIndex = accessor.getInt();

		if ( down )
		{
			if ( channel == VuUIInputUtil::CHANNEL_UP )
			{
				mpScriptComponent->getPlug("NavUp")->execute();

				if ( mpScriptComponent->getPlug("NavUp")->getNumConnections() )
					VuGameUtil::IF()->playSfx(VuGameUtil::SFX_UI_NAVIGATE);
			}
			else if ( channel == VuUIInputUtil::CHANNEL_DOWN )
			{
				mpScriptComponent->getPlug("NavDown")->execute();

				if ( mpScriptComponent->getPlug("NavDown")->getNumConnections() )
					VuGameUtil::IF()->playSfx(VuGameUtil::SFX_UI_NAVIGATE);
			}
			else if ( channel == VuUIInputUtil::CHANNEL_LEFT )
			{
				mpScriptComponent->getPlug("NavLeft")->execute();

				if ( mpScriptComponent->getPlug("NavLeft")->getNumConnections() )
					VuGameUtil::IF()->playSfx(VuGameUtil::SFX_UI_NAVIGATE);
			}
			else if ( channel == VuUIInputUtil::CHANNEL_RIGHT )
			{
				mpScriptComponent->getPlug("NavRight")->execute();

				if ( mpScriptComponent->getPlug("NavRight")->getNumConnections() )
					VuGameUtil::IF()->playSfx(VuGameUtil::SFX_UI_NAVIGATE);
			}
			else if ( channel == VuUIInputUtil::CHANNEL_SELECT )
			{
				toggleCheckState();
			}
		}
	}
}

//*****************************************************************************
void VuCheckBoxBaseEntity::OnUITouch(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	int action = accessor.getInt();
	VuVector2 touch = accessor.getVector2();

	if ( action == VuUIInputUtil::TOUCH_DOWN )
	{
		if ( mVisible && isEnabled() )
		{
			VuUIDrawParams uiDrawParams;
			VuUIDrawUtil::getParams(this, uiDrawParams);

			VuRect touchRect = uiDrawParams.transform(mTouchRect);
			mAnchor.apply(touchRect, touchRect);

			if ( touchRect.contains(touch) )
			{
				toggleCheckState();
			}
		}
	}
}

//*****************************************************************************
void VuCheckBoxBaseEntity::OnUIDraw(const VuParams &params)
{
	float alpha = 1.0f;

	if ( mVisible )
	{
		drawCheckBox(alpha);
	}
}

//*****************************************************************************
VuRetVal VuCheckBoxBaseEntity::SetFocus(const VuParams &params)
{
	VuUI::IF()->setFocus(this);

	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuCheckBoxBaseEntity::Show(const VuParams &params)
{
	mVisible = true;
	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuCheckBoxBaseEntity::Hide(const VuParams &params)
{
	mVisible = false;

	return VuRetVal();
}

//*****************************************************************************
void VuCheckBoxBaseEntity::drawLayout(bool bSelected)
{
	if ( bSelected )
	{
		VuUIDrawParams uiDrawParams;
		VuUIDrawUtil::getParams(this, uiDrawParams);

		const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mFont);
		VuRect rect = uiDrawParams.transform(mTextRect);
		VuGfxUtil::IF()->drawRectangleOutline2d(uiDrawParams.mDepth, fontEntry.params().mColor, rect);
	}
	drawCheckBox(1.0f);
}

//*****************************************************************************
void VuCheckBoxBaseEntity::drawCheckBox(float alpha)
{
	if ( !isEnabled() )
		alpha *= 0.5f;

	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	VuColor color(255,255,255, VuRound(255*alpha));

	bool hasFocus = VuUI::IF()->getFocus() == this;

	// frame
	VuColor frameColor = color;
	VuTexture *pFrameTexture = mFrameImage.getTexture();
	if ( hasFocus && VuControlMethodManager::IF()->isGamePadOrKeyboard() )
	{
		pFrameTexture = mSelectedFrameImage.getTexture();
		frameColor = VuGameUtil::IF()->calcHighlightColor(alpha);
	}

	if ( pFrameTexture )
	{
		VuRect rect = uiDrawParams.transform(mFrameRect);
		mAnchor.apply(rect, rect);
		VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth + 2*GFX_SORT_DEPTH_STEP, pFrameTexture, frameColor, rect);
	}

	// check
	if ( VuTexture *pTexture = isChecked() ? mCheckedImage.getTexture() : mUncheckedImage.getTexture() )
	{
		VuRect rect = uiDrawParams.transform(mCheckRect);
		mAnchor.apply(rect, rect);
		VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth + 1*GFX_SORT_DEPTH_STEP, pTexture, color, rect);
	}

	// text
	{
		const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mFont);

		const std::string &text = VuStringDB::IF()->getString(mStringID);
		if ( text.length() )
		{
			VuRect rect = uiDrawParams.transform(mTextRect);
			mAnchor.apply(rect, rect);

			VuGfxUtil::IF()->fontDraw()->drawString(
				uiDrawParams.mDepth + 0*GFX_SORT_DEPTH_STEP,
				fontEntry.font(),
				text.c_str(),
				fontEntry.params(),
				rect,
				mStringFormat,
				alpha
			);
		}
	}
}

//*****************************************************************************
void VuCheckBoxBaseEntity::toggleCheckState()
{
	if ( isChecked() )
	{
		VuGameUtil::IF()->playSfx(VuGameUtil::SFX_UI_BACK);
		setChecked(false);
		mpScriptComponent->getPlug("OnUnchecked")->execute();
	}
	else
	{
		VuGameUtil::IF()->playSfx(VuGameUtil::SFX_UI_SELECT);
		setChecked(true);
		mpScriptComponent->getPlug("OnChecked")->execute();
	}
}


//*****************************************************************************
// CheckBox Entity
//*****************************************************************************
class VuCheckBoxEntity : public VuCheckBoxBaseEntity
{
	DECLARE_RTTI

public:
	VuCheckBoxEntity();

private:
	// scripting
	VuRetVal		SetChecked(const VuParams &params) { mChecked = true; return VuRetVal(); }
	VuRetVal		setUnchecked(const VuParams &params) { mChecked = false; return VuRetVal(); }

	virtual bool	isChecked()					{ return mChecked; }
	virtual void	setChecked(bool checked)	{ mChecked = checked; }

	// properties
	bool			mChecked;
};

IMPLEMENT_RTTI(VuCheckBoxEntity, VuCheckBoxBaseEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuCheckBoxEntity);


//*****************************************************************************
VuCheckBoxEntity::VuCheckBoxEntity():
	mChecked(false)
{
	// properties
	addProperty(new VuBoolProperty("Checked", mChecked));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuCheckBoxEntity, SetChecked);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuCheckBoxEntity, setUnchecked);
}
