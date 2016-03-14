//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  UI Slider class
// 
//*****************************************************************************

#include "VuSliderEntity.h"
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
#include "VuEngine/HAL/Touch/VuTouch.h"
#include "VuEngine/HAL/Gfx/VuTexture.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Math/VuMathUtil.h"
#include "VuEngine/DB/VuStringDB.h"


IMPLEMENT_RTTI(VuSliderEntity, VuEntity);

//*****************************************************************************
VuSliderEntity::VuSliderEntity():
	mVisible(true),
	mBarDstDelta(0,0),
	mBarSrcDelta(0,0),
	mBarSrcOnDelta(0,0),
	mBarCount(0)
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));
	addComponent(new Vu2dLayoutComponent(this, &VuSliderEntity::drawLayout));

	// properties
	addProperty(new VuBoolProperty("Visible", mVisible));
	mFrameImage.addProperties(getProperties(), "Frame Image");
	mSelectedFrameImage.addProperties(getProperties(), "Selected Frame Image");
	addProperty(new VuRectProperty("Frame Rect", mFrameRect));
	addProperty(new VuRectProperty("Text Rect", mTextRect));
	addProperty(new VuFontEnumProperty("Font", mFont));
	ADD_UI_STRING_FORMAT_PROPERTIES(getProperties(), mStringFormat, "");
	addProperty(new VuStringProperty("String ID", mStringID));
	mBarImage.addProperties(getProperties(), "Bar Image");
	addProperty(new VuRectProperty("Bar Dst Rect", mBarDstRect));
	addProperty(new VuVector2Property("Bar Dst Delta", mBarDstDelta));
	addProperty(new VuRectProperty("Bar Src Rect", mBarSrcRect));
	addProperty(new VuVector2Property("Bar Src Delta", mBarSrcDelta));
	addProperty(new VuVector2Property("Bar Src On Delta", mBarSrcOnDelta));
	addProperty(new VuIntProperty("Bar Count", mBarCount));

	// event handlers
	REG_EVENT_HANDLER(VuSliderEntity, OnUITick);
	REG_EVENT_HANDLER(VuSliderEntity, OnUIGamePad);
	REG_EVENT_HANDLER(VuSliderEntity, OnUIDraw);

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuSliderEntity, SetFocus);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuSliderEntity, Show);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuSliderEntity, Hide);
	ADD_SCRIPT_INPUT(mpScriptComponent, VuSliderEntity, IsEnabled, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnSetValue);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, NavUp);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, NavDown);
}

//*****************************************************************************
int VuSliderEntity::getBars()
{
	return VuRound(mBarCount*getValue());
}

//*****************************************************************************
void VuSliderEntity::setBars(int bars)
{
	VuGameUtil::IF()->playSfx(VuGameUtil::SFX_UI_NAVIGATE);
	setValue((float)bars/mBarCount);
	mpScriptComponent->getPlug("OnSetValue")->execute();
}

//*****************************************************************************
VuRetVal VuSliderEntity::SetFocus(const VuParams &params)
{
	VuUI::IF()->setFocus(this);

	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuSliderEntity::Show(const VuParams &params)
{
	mVisible = true;
	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuSliderEntity::Hide(const VuParams &params)
{
	mVisible = false;

	return VuRetVal();
}

//*****************************************************************************
void VuSliderEntity::OnUITick(const VuParams &params)
{
	if ( mVisible && isEnabled() )
	{
		VuUIDrawParams uiDrawParams;
		VuUIDrawUtil::getParams(this, uiDrawParams);

		// remove overlap
		VuRect barDstRect = mBarDstRect;
		if ( VuAbs(mBarDstDelta.mX) > VuAbs(mBarDstDelta.mY) )
		{
			float overlap = barDstRect.mWidth - VuAbs(mBarDstDelta.mX);
			if ( overlap > 0.0f )
			{
				barDstRect.mX += 0.5f*overlap;
				barDstRect.mWidth -= overlap;
			}
		}
		else
		{
			float overlap = barDstRect.mHeight - VuAbs(mBarDstDelta.mY);
			if ( overlap > 0.0f )
			{
				barDstRect.mY += 0.5f*overlap;
				barDstRect.mHeight -= overlap;
			}
		}

		int bars = -1;
		for ( int iTouch = 0; iTouch < VuTouch::IF()->getTouchCount(); iTouch++ )
		{
			VuVector2 touch;
			VuTouch::IF()->getTouch(iTouch, touch);
			touch = VuUI::IF()->getInvCropMatrix().transform(touch);

			for ( int iBar = -1; iBar < mBarCount; iBar++ )
			{
				VuRect dstRect = uiDrawParams.transform(barDstRect + (float)iBar*mBarDstDelta);
				mAnchor.apply(dstRect, dstRect);

				if ( dstRect.contains(touch) )
				{
					bars = iBar + 1;
				}
			}
		}

		if ( bars >= 0 )
		{
			if ( bars != getBars() )
			{
				setBars(bars);
			}
		}
	}
}

//*****************************************************************************
void VuSliderEntity::OnUIGamePad(const VuParams &params)
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
				if ( getBars() > 0 )
					setBars(getBars() - 1);
			}
			else if ( channel == VuUIInputUtil::CHANNEL_RIGHT )
			{
				if ( getBars() < mBarCount )
					setBars(getBars() + 1);
			}
		}
	}
}

//*****************************************************************************
void VuSliderEntity::OnUIDraw(const VuParams &params)
{
	float alpha = 1.0f;

	if ( mVisible )
	{
		drawSlider(alpha);
	}
}

//*****************************************************************************
void VuSliderEntity::drawLayout(bool bSelected)
{
	if ( bSelected )
	{
		VuUIDrawParams uiDrawParams;
		VuUIDrawUtil::getParams(this, uiDrawParams);

		const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mFont);
		VuRect rect = uiDrawParams.transform(mTextRect);
		VuGfxUtil::IF()->drawRectangleOutline2d(uiDrawParams.mDepth, fontEntry.params().mColor, rect);
	}

	drawSlider(1.0f);
}

//*****************************************************************************
void VuSliderEntity::drawSlider(float alpha)
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

	// bar
	if ( VuTexture *pTexture = mBarImage.getTexture() )
	{
		for ( int iBar = 0; iBar < mBarCount; iBar++ )
		{
			VuRect dstRect = uiDrawParams.transform(mBarDstRect + (float)iBar*mBarDstDelta);
			mAnchor.apply(dstRect, dstRect);
			VuRect srcRect = mBarSrcRect + (float)iBar*mBarSrcDelta;
			if ( getBars() >= (iBar + 1) )
				srcRect += mBarSrcOnDelta;
			srcRect /= VuVector2(pTexture->getWidth(), pTexture->getHeight());
			VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth + 1*GFX_SORT_DEPTH_STEP, pTexture, color, dstRect, srcRect);
		}
	}

	// text
	const std::string &text = VuStringDB::IF()->getString(mStringID);
	if ( text.length() )
	{
		const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mFont);

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
