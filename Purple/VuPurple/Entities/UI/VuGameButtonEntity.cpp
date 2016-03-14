//*****************************************************************************
//
//  Copyright (c) 2009-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Game Button class
// 
//*****************************************************************************

#include "VuGameButtonEntity.h"
#include "VuPurple/Managers/VuSettingsManager.h"
#include "VuPurple/Managers/VuControlMethodManager.h"
#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Gfx/VuGameGfxUtil.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/UI/VuUI.h"
#include "VuEngine/UI/VuUIDrawUtil.h"
#include "VuEngine/UI/VuUIInputUtil.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Components/2dLayout/Vu2dLayoutComponent.h"
#include "VuEngine/Components/Transform/VuTransformComponent.h"
#include "VuEngine/Components/VuTransitionComponent.h"
#include "VuEngine/Properties/VuAssetProperty.h"
#include "VuEngine/Properties/VuFontEnumProperty.h"
#include "VuEngine/Assets/VuTextureAsset.h"
#include "VuEngine/Assets/VuFontAsset.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/Font/VuFontDraw.h"
#include "VuEngine/HAL/Gfx/VuTexture.h"
#include "VuEngine/DB/VuStringDB.h"

#include "VuPurple/Managers/VuBillingManager.h"
#include "VuEngine/Util/VuSpreadsheetQuery.h"
#include "VuEngine/Util/VuStringUtil.h"

#include "VuEngine/VuEngine.h"
//*****************************************************************************
// VuGameButtonBaseEntity
//*****************************************************************************

IMPLEMENT_RTTI(VuGameButtonBaseEntity, VuEntity);




/*
static VuStaticIntEnumProperty::Choice sButtonTypes[] =
{
	{ "Common", VuGameButtonBaseEntity::BTN_TYPE_COMMON},
	{ "BigGift", VuGameButtonBaseEntity::BTN_TYPE_BIG_GIFT },
	{ VUNULL }
};*/


//*****************************************************************************
VuGameButtonBaseEntity::VuGameButtonBaseEntity():
	mInitiallyHidden(false),
	mDisabled(false),
	mFlipImageX(false),
	mFlipImageY(false),
	mFlipIconX(false),
	mFlipIconY(false),
	mCanHaveFocus(true),
	mSwapIconDepth(false),
	mHidden(false),
	mNewTouch(false),
	mTouchDown(false),
	mTouchUp(false),
	mPressedState(NOT_PRESSED)

{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this, 120));
	addComponent(new Vu2dLayoutComponent(this, &VuGameButtonBaseEntity::drawLayout));
	addComponent(mpTransitionComponent = new VuTransitionComponent(this));

	// properties
	addProperty(new VuBoolProperty("Hidden", mInitiallyHidden));
	addProperty(new VuBoolProperty("Disabled", mDisabled));
	addProperty(new VuRectProperty("Image Rect", mImageRect));
	addProperty(new VuRectProperty("Touch Rect", mTouchRect));
	addProperty(new VuRectProperty("Text Rect", mTextRect));
	addProperty(new VuRectProperty("Icon Rect", mIconRect));

// modify by xlm,[/2014/12/2 ] 
	addProperty(new VuRectProperty("Rmb Rect", mRmbTextRect));
	addProperty(new VuStringProperty("RmbText Id", mRmbTextId));

	/*addProperty(new VuStaticIntEnumProperty("ButtonType", mButtonType, sButtonTypes));*/


	addProperty(new VuFontEnumProperty("Font", mFont));
	addProperty(new VuStringProperty("String ID", mStringID));
	addProperty(new VuStringProperty("Game Pad String ID", mGamePadStringID));
	mSelectedTexture.addProperties(getProperties(), "Selected Texture");
	mUnselectedTexture.addProperties(getProperties(), "Unselected Texture");
	addProperty(new VuBoolProperty("FlipImageX", mFlipImageX));
	addProperty(new VuBoolProperty("FlipImageY", mFlipImageY));
	addProperty(new VuBoolProperty("FlipIconX", mFlipIconX));
	addProperty(new VuBoolProperty("FlipIconY", mFlipIconY));
	ADD_UI_ANCHOR_PROPERTIES(getProperties(), mAnchor, "");
	addProperty(new VuBoolProperty("Can Have Focus", mCanHaveFocus));
	addProperty(new VuBoolProperty("Swap Icon Depth", mSwapIconDepth));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuGameButtonBaseEntity, SetFocus);
	ADD_SCRIPT_INPUT(mpScriptComponent, VuGameButtonBaseEntity, SetStringID, VuRetVal::Void, VuParamDecl(1, VuParams::String));
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuGameButtonBaseEntity, Show);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuGameButtonBaseEntity, Hide);
	ADD_SCRIPT_INPUT(mpScriptComponent, VuGameButtonBaseEntity, IsVisible, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuGameButtonBaseEntity, Enable);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuGameButtonBaseEntity, Disable);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, NavUp);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, NavDown);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, NavLeft);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, NavRight);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnFocus);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, Pressed);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, PressedFail);

	// event handlers
	REG_EVENT_HANDLER(VuGameButtonBaseEntity, OnUITick);
	REG_EVENT_HANDLER(VuGameButtonBaseEntity, OnUIGamePad);
	REG_EVENT_HANDLER(VuGameButtonBaseEntity, OnUITouch);
	REG_EVENT_HANDLER(VuGameButtonBaseEntity, OnUIDraw);

	// constants
	mPressedScale = 1.0f - 0.01f*VuGameUtil::IF()->constantDB()["UI"]["PressedButtonSizeReduction"].asFloat();
}

//*****************************************************************************
void VuGameButtonBaseEntity::onGameInitialize()
{
	mHidden = mInitiallyHidden;
}

//*****************************************************************************
VuRetVal VuGameButtonBaseEntity::SetFocus(const VuParams &params)
{
	if ( mCanHaveFocus )
	{
		VuUI::IF()->setFocus(this);
		mpScriptComponent->getPlug("OnFocus")->execute();
	}

	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuGameButtonBaseEntity::SetStringID(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	mStringID = accessor.getString();

	return VuRetVal();
}

// modify by xlm,[/2014/12/2 ] 
VuRetVal VuGameButtonBaseEntity::SetRmbTextId(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	mRmbTextId = accessor.getString();

	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuGameButtonBaseEntity::Show(const VuParams &params)
{
	mHidden = false;

	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuGameButtonBaseEntity::Hide(const VuParams &params)
{
	mHidden = true;

	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuGameButtonBaseEntity::IsVisible(const VuParams &params)
{
	bool visible = isVisibleAndActive();

	return VuRetVal(visible);
}

//*****************************************************************************
VuRetVal VuGameButtonBaseEntity::Enable(const VuParams &params)
{
	mDisabled = false;

	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuGameButtonBaseEntity::Disable(const VuParams &params)
{
	mDisabled = true;

	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuGameButtonBaseEntity::IsEnabled(const VuParams &params)
{
	bool enabled = isEnabled();

	return VuRetVal(enabled);
}

//*****************************************************************************
void VuGameButtonBaseEntity::OnUITick(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	float fdt = accessor.getFloat();

	mpTransitionComponent->tick(fdt);

	if ( isVisibleAndActive() )
	{
		VuUIDrawParams uiDrawParams;
		VuUIDrawUtil::getParams(this, uiDrawParams);

		VuRect touchRect = uiDrawParams.transform(mTouchRect);
		mAnchor.apply(touchRect, touchRect);
		if ( mNewTouch )
		{
			if ( touchRect.contains(mTouchPos) )
			{
				SetFocus();
				mPressedState = PRESSED;
			}
		}
		if ( mPressedState == PRESSED )
		{
			if ( mTouchUp )
			{
				if ( isEnabled() )
					mpScriptComponent->getPlug("Pressed")->execute();
				else
					mpScriptComponent->getPlug("PressedFail")->execute();

				mPressedState = NOT_PRESSED;
			}
			if ( !touchRect.contains(mTouchPos) )
			{
				mPressedState = NOT_PRESSED;
			}
		}

		if ( mTouchUp )
		{
			mTouchDown = false;
			mTouchUp = false;
		}
		mNewTouch = false;
	}
}

//*****************************************************************************
void VuGameButtonBaseEntity::OnUIGamePad(const VuParams &params)
{
	if ( (VuUI::IF()->getFocus() == this) && isVisibleAndActive() )
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
				if ( isEnabled() )
					mpScriptComponent->getPlug("Pressed")->execute();
				else
					mpScriptComponent->getPlug("PressedFail")->execute();
			}
		}
	}
}

//*****************************************************************************
void VuGameButtonBaseEntity::OnUITouch(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	int action = accessor.getInt();
	VuVector2 touch = accessor.getVector2();
		
	if ( action == VuUIInputUtil::TOUCH_DOWN )
	{
		if ( isVisibleAndActive() && !mTouchDown )
		{
			mTouchDown = true;
			mNewTouch = true;
			mTouchPos = touch;
		}
	}
	else if ( action == VuUIInputUtil::TOUCH_UP )
	{
		mTouchUp = true;
	}
	else if ( action == VuUIInputUtil::TOUCH_MOVE )
	{
		mTouchPos = touch;
	}
}

//*****************************************************************************
void VuGameButtonBaseEntity::OnUIDraw(const VuParams &params)
{
	if ( isVisible() )
		drawButton(mpTransitionComponent->alpha());
}

//*****************************************************************************
void VuGameButtonBaseEntity::drawLayout(bool bSelected)
{
	if ( bSelected )
	{
		VuUIDrawParams uiDrawParams;
		VuUIDrawUtil::getParams(this, uiDrawParams);

		const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mFont);

		VuRect touchRect = uiDrawParams.transform(mTouchRect);
		mAnchor.apply(touchRect, touchRect);
		VuGfxUtil::IF()->drawRectangleOutline2d(uiDrawParams.mDepth, fontEntry.params().mColor, touchRect);
	}

	drawButton(1.0f);
}

//*****************************************************************************
void VuGameButtonBaseEntity::drawButton(float alpha)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	float depth = uiDrawParams.mDepth;
	VuColor color(255, 255, 255, VuRound(255*alpha));

	bool hasFocus = VuUI::IF()->getFocus() == this;

	// draw background
	VuColor backgroundColor = color;
	VuTexture *pImage = mUnselectedTexture.getTexture();
	//if ( hasFocus && VuControlMethodManager::IF()->isGamePadOrKeyboard() )
	if (hasFocus)
	{
		pImage = mSelectedTexture.getTexture();
		backgroundColor = VuGameUtil::IF()->calcHighlightColor(alpha);
	}

	if ( pImage )
	{
		VuRect dstRect = mImageRect;

		if ( mPressedState == PRESSED )
			dstRect.scale(mImageRect.getCenter(), mPressedScale);

		dstRect = uiDrawParams.transform(dstRect);
		mAnchor.apply(dstRect, dstRect);

		VuRect srcRect(0,0,1,1);
		if ( mFlipImageX )
			srcRect.flipX();
		if ( mFlipImageY )
			srcRect.flipY();

		if ( isDisabled() )
			VuGameGfxUtil::IF()->drawGrayScaleTexture2d(depth, pImage, backgroundColor, dstRect, srcRect);
		else
			VuGfxUtil::IF()->drawTexture2d(depth, pImage, backgroundColor, dstRect, srcRect);
	}
	depth -= GFX_SORT_DEPTH_STEP;

	// determine icon depts
	float iconDepth = depth;
	depth -= GFX_SORT_DEPTH_STEP;
	float secondaryIconDepth = depth;
	depth -= GFX_SORT_DEPTH_STEP;

	if ( mSwapIconDepth )
		VuSwap(iconDepth, secondaryIconDepth);

	// draw icon
	{
		VuRect dstRect = mIconRect;

		if ( mPressedState == PRESSED )
			dstRect.scale(mImageRect.getCenter(), mPressedScale);

		dstRect = uiDrawParams.transform(dstRect);
		mAnchor.apply(dstRect, dstRect);

		VuRect srcRect(0,0,1,1);
		if ( mFlipIconX )
			srcRect.flipX();
		if ( mFlipIconY )
			srcRect.flipY();

		drawIcon(iconDepth, color, dstRect, srcRect);
	}

	// draw secondary icon
	{
		drawSecondaryIcon(secondaryIconDepth, color, uiDrawParams);
	}

	// draw text
	const char *stringId = mStringID.c_str();
	if ( !mGamePadStringID.empty() && (VuControlMethodManager::IF()->getMethod() == VuControlMethodManager::METHOD_GAMEPAD) )
		stringId = mGamePadStringID.c_str();

	if ( stringId[0] )
	{
		const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mFont);
		const char *strText = VuStringDB::IF()->getStringSelf(stringId);

		VuFontDrawParams fdParams = fontEntry.params();
		VuRect dstRect = mTextRect;

		if ( mPressedState == PRESSED )
		{
			fdParams.mSize *= mPressedScale;
			dstRect.scale(mImageRect.getCenter(), mPressedScale);
		}
		fdParams.mSize *= 0.5f*(mpTransformComponent->getWorldScale().mX + mpTransformComponent->getWorldScale().mY);

		dstRect = uiDrawParams.transform(dstRect);
		mAnchor.apply(dstRect, dstRect);

		VUUINT32 flags = VUGFX_TEXT_DRAW_HCENTER | VUGFX_TEXT_DRAW_VCENTER;

		

		drawBaseText(depth, fontEntry.font(), strText, fdParams, dstRect, flags, alpha);

		//VuGfxUtil::IF()->fontDraw()->drawString(depth, fontEntry.font(), strText, fdParams, dstRect, flags, alpha);
	}

// modify by xlm,[/2014/12/2 ] draw rmb text
	if (mRmbTextId.empty())
	{
		depth -= GFX_SORT_DEPTH_STEP;
		return;
	}
		

	const char *rmbId = mRmbTextId.c_str();
	/*if (!mGamePadStringID.empty() && (VuControlMethodManager::IF()->getMethod() == VuControlMethodManager::METHOD_GAMEPAD))
		stringId = mGamePadStringID.c_str();
*/

	if (rmbId[0])
	{
		const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mFont);
		const char *strText = VuStringDB::IF()->getStringSelf(rmbId);

		VuFontDrawParams fdParams = fontEntry.params();
		VuRect dstRect = mRmbTextRect;

		if (mPressedState == PRESSED)
		{
			fdParams.mSize *= mPressedScale;
			dstRect.scale(mImageRect.getCenter(), mPressedScale);
		}
		fdParams.mSize *= 0.5f*(mpTransformComponent->getWorldScale().mX + mpTransformComponent->getWorldScale().mY);

		dstRect = uiDrawParams.transform(dstRect);
		mAnchor.apply(dstRect, dstRect);

		VUUINT32 flags = VUGFX_TEXT_DRAW_HCENTER | VUGFX_TEXT_DRAW_VCENTER;
		VuGfxUtil::IF()->fontDraw()->drawString(depth, fontEntry.font(), strText, fdParams, dstRect, flags, alpha);

		
	}

	depth -= GFX_SORT_DEPTH_STEP;
}

void VuGameButtonBaseEntity::drawBaseText(int nDepth, VuFont* pFont, const char *strText ,VuFontDrawParams& fontParam, VuRect& rect, VUUINT32 uFlag, float alpha)
{
	VuGfxUtil::IF()->fontDraw()->drawString(nDepth, pFont, strText, fontParam, rect, uFlag, alpha);
}

//*****************************************************************************
bool VuGameButtonBaseEntity::isVisibleAndActive()
{
	return isVisible() && (mpTransitionComponent->getState() == VuTransitionBaseComponent::STATE_ACTIVE);
}

//*****************************************************************************
// VuGameButtonEntity
//*****************************************************************************

class VuGameButtonEntity : public VuGameButtonBaseEntity
{
	DECLARE_RTTI

public:
	VuGameButtonEntity()
	{
		// properties
		mIconTexture.addProperties(getProperties(), "Icon Texture");
	}

protected:
	virtual void drawIcon(float depth, const VuColor &color, const VuRect &dstRect, const VuRect &srcRect)
	{
		if ( VuTexture *pIcon = mIconTexture.getTexture() )
		{
			if ( isDisabled() )
				VuGameGfxUtil::IF()->drawGrayScaleTexture2d(depth, pIcon, color, dstRect, srcRect);
			else
				VuGfxUtil::IF()->drawTexture2d(depth, pIcon, color, dstRect, srcRect);
		}
	}



	VuUIImageProperties		mIconTexture;
};

IMPLEMENT_RTTI(VuGameButtonEntity, VuGameButtonBaseEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuGameButtonEntity);





//*****************************************************************************
// VuPromoteGameButtonEntity added by xlm
//*****************************************************************************
// modify by xlm,[/2015/4/16 ] 
class VuPromoteGameButtonEntity : public VuGameButtonEntity
{
	DECLARE_RTTI



public:
	VuPromoteGameButtonEntity() : mPromoteWeekDay(-1), mItemName("")
	{
		// properties
		//mIconTexture.addProperties(getProperties(), "Icon Texture");

		addProperty(new VuRectProperty("PromoteIconRect", mPromoteIconRect));

		mPromoteIcon.addProperties(getProperties(), "Promote Icon");
		addProperty(new VuIntProperty("PromoteWeekDay", mPromoteWeekDay));

		addProperty(new VuStringProperty("ItemName", mItemName));

		addProperty(new VuRectProperty("DoubleIconRect", mDoubleIconRect));

		mDoubleIcon.addProperties(getProperties(), "Double Icon");
	}

protected:

	virtual void							drawBaseText(int nDepth, VuFont* pFont, const char *strText, VuFontDrawParams& fontParam, VuRect& rect, VUUINT32 uFlag, float alpha)
	{
		const VuJsonContainer &itemData = VuDataUtil::findArrayMember(VuGameUtil::IF()->storeDB(), "Name", mItemName);

		int nPrice = 0;
		int nPlus = 0;

		char szPrice[20] = { 0 };
		char szPlus[20] = { 0 };
		if (mItemName.find("Standard") != std::string::npos)
		{
			nPrice = itemData["StandardCurrency"].asInt();
			nPlus = itemData["PlusCoin"].asInt();
		}
		else
		{
			nPrice = itemData["PremiumCurrency"].asInt();
			nPlus = itemData["PlusDiamond"].asInt();
		}

		sprintf(szPrice, "%d", nPrice);
		sprintf(szPlus, "%d", nPlus);

		std::string strContent(strText);
		VuStringUtil::replace(strContent, "[Text_Price]", szPrice);
		VuStringUtil::replace(strContent, "[Text_Plus]", szPlus);

		VuGfxUtil::IF()->fontDraw()->drawString(nDepth, pFont, strContent.c_str(), fontParam, rect, uFlag, alpha);


	}


	/*virtual void drawIcon(float depth, const VuColor &color, const VuRect &dstRect, const VuRect &srcRect)
	{
		if (VuTexture *pIcon = mIconTexture.getTexture())
		{
			if (isDisabled())
				VuGameGfxUtil::IF()->drawGrayScaleTexture2d(depth, pIcon, color, dstRect, srcRect);
			else
				VuGfxUtil::IF()->drawTexture2d(depth, pIcon, color, dstRect, srcRect);
		}

		if (VuGameManager::IF()->getWeekDay() == 6)
		{
			if (VuTexture* pPromoteIcon = mPromoteIcon.getTexture())
			{
				if (isDisabled())
					VuGameGfxUtil::IF()->drawGrayScaleTexture2d(depth, pPromoteIcon, color, dstRect, srcRect);
				else
					VuGfxUtil::IF()->drawTexture2d(depth, pPromoteIcon, color, dstRect, srcRect);
			}
		}

		
	}*/

	virtual void drawSecondaryIcon(float depth, const VuColor &color, const VuUIDrawParams &uiDrawParams)
	{
		if (VuEngine::IF()->editorMode())
		{
			if (VuTexture *pSecondaryIcon = mPromoteIcon.getTexture())
			{
				VuRect dstRect = mPromoteIconRect;

				if (mPressedState == PRESSED)
					dstRect.scale(mImageRect.getCenter(), mPressedScale);

				dstRect = uiDrawParams.transform(dstRect);
				mAnchor.apply(dstRect, dstRect);

				if (isDisabled())
					VuGameGfxUtil::IF()->drawGrayScaleTexture2d(depth, pSecondaryIcon, color, dstRect);
				else
					VuGfxUtil::IF()->drawTexture2d(depth, pSecondaryIcon, color, dstRect);
			}

			if (VuTexture *pDoubleIcon = mDoubleIcon.getTexture())
			{
				VuRect dstRect = mDoubleIconRect;

				if (mPressedState == PRESSED)
					dstRect.scale(mImageRect.getCenter(), mPressedScale);

				dstRect = uiDrawParams.transform(dstRect);
				mAnchor.apply(dstRect, dstRect);

				if (isDisabled())
					VuGameGfxUtil::IF()->drawGrayScaleTexture2d(depth, pDoubleIcon, color, dstRect);
				else
					VuGfxUtil::IF()->drawTexture2d(depth, pDoubleIcon, color, dstRect);
			}
		}
		else
		{
			if (VuGameManager::IF()->getWeekDay() == mPromoteWeekDay)
			{
				if (VuTexture *pSecondaryIcon = mPromoteIcon.getTexture())
				{
					VuRect dstRect = mPromoteIconRect;

					if (mPressedState == PRESSED)
						dstRect.scale(mImageRect.getCenter(), mPressedScale);

					dstRect = uiDrawParams.transform(dstRect);
					mAnchor.apply(dstRect, dstRect);

					if (isDisabled())
						VuGameGfxUtil::IF()->drawGrayScaleTexture2d(depth, pSecondaryIcon, color, dstRect);
					else
						VuGfxUtil::IF()->drawTexture2d(depth, pSecondaryIcon, color, dstRect);
				}
			}

			bool bPurchased = __isPurchasedCoinOrDiam();
			VuTexture *pDoubleIcon = mDoubleIcon.getTexture();
			if (!bPurchased && pDoubleIcon)
			{
				VuRect dstRect = mDoubleIconRect;

				if (mPressedState == PRESSED)
					dstRect.scale(mImageRect.getCenter(), mPressedScale);

				dstRect = uiDrawParams.transform(dstRect);
				mAnchor.apply(dstRect, dstRect);

				if (isDisabled())
					VuGameGfxUtil::IF()->drawGrayScaleTexture2d(depth, pDoubleIcon, color, dstRect);
				else
					VuGfxUtil::IF()->drawTexture2d(depth, pDoubleIcon, color, dstRect);
			}
			
		}
	

		
	}


protected:
	VuUIRectProperties				mPromoteIconRect;

	VuUIImageProperties				mPromoteIcon;

	VuUIRectProperties       mDoubleIconRect;
	VuUIImageProperties		 mDoubleIcon;

	int								mPromoteWeekDay;

	std::string						mItemName;
private:
	bool __isPurchasedCoinOrDiam()
	{
		bool bRet = false;
		bRet = VuBillingManager::IF()->isOwned("PurchasedDiam") || VuBillingManager::IF()->isOwned("PurchasedCoin");

		return bRet;
	}
};

IMPLEMENT_RTTI(VuPromoteGameButtonEntity, VuGameButtonEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuPromoteGameButtonEntity);





//*****************************************************************************
// VuDriverLockedButtonEntity  added by xlm
//*****************************************************************************

class VuDriverLockedButtonEntity : public VuGameButtonEntity
{
	DECLARE_RTTI

public:
	VuDriverLockedButtonEntity()
	{
		// properties
		
	}

protected:
	/*virtual void							OnUITick(const VuParams &params)  // modify by xlm,[/2014/12/22 ] 
	{

		VuParams::VuAccessor accessor(params);
		float fdt = accessor.getFloat();

		mpTransitionComponent->tick(fdt);

		if (isVisibleAndActive())
		{
			VuUIDrawParams uiDrawParams;
			VuUIDrawUtil::getParams(this, uiDrawParams);

			VuRect touchRect = uiDrawParams.transform(mTouchRect);
			mAnchor.apply(touchRect, touchRect);
			if (mNewTouch)
			{
				if (touchRect.contains(mTouchPos))
				{
					SetFocus();
					mPressedState = PRESSED;
				}
			}
			if (mPressedState == PRESSED)
			{
				if (mTouchUp)
				{
					if (isEnabled())
						mpScriptComponent->getPlug("Pressed")->execute();
					else
					{
						mpScriptComponent->getPlug("PressedFail")->execute();


					}
						

					mPressedState = NOT_PRESSED;
				}
				if (!touchRect.contains(mTouchPos))
				{
					mPressedState = NOT_PRESSED;
				}
			}

			if (mTouchUp)
			{
				mTouchDown = false;
				mTouchUp = false;
			}
			mNewTouch = false;
		}
	}
*/

	virtual void							drawBaseText(int nDepth, VuFont* pFont, const char *strText, VuFontDrawParams& fontParam, VuRect& rect, VUUINT32 uFlag, float alpha)
	{

		std::string strBaseText(strText);

		//获取当前的车手
		const std::string &selectedDriver = VuGameUtil::IF()->getSelectedDriver();
		//VuStringUtil::replace(strBaseText, "[DRIVER_UNLOCK_PRICE]", );

		const VuSpreadsheetAsset *pSA = VuGameUtil::IF()->driverSpreadsheet();
		int rowIndex = VuSpreadsheetQuery::findFirstRow(pSA, VuSpreadsheetQuery::VuStringEqual("Driver", selectedDriver.c_str()));
/*
		int nPrice = -1;
		VuFastDataUtil::getValue(pSA->getField(rowIndex, "UnlockPrice"), nPrice);*/
/*
		std::string strPrice;
		const VuFastContainer& ct = pSA->getField(rowIndex, "UnlockPrice");
		const char* ptemp = ct.asCString();

		int nInt = ct.asInt();*/

		int nPrice = -1;
		VuFastDataUtil::getValue(pSA->getField(rowIndex, "UnlockPrice"), nPrice);

		char szContent[20];
		sprintf(szContent, "%d", nPrice);

		VuStringUtil::replace(strBaseText, "[DRIVER_UNLOCK_PRICE]", szContent);

		VuGfxUtil::IF()->fontDraw()->drawString(nDepth, pFont, strBaseText.c_str(), fontParam, rect, uFlag, alpha);
		
	}
		
};

IMPLEMENT_RTTI(VuDriverLockedButtonEntity, VuGameButtonEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuDriverLockedButtonEntity);






//*****************************************************************************
// VuCarButtonEntity
//*****************************************************************************

class VuCarButtonEntity : public VuGameButtonBaseEntity
{
	DECLARE_RTTI

public:
	VuCarButtonEntity() : mpIconTexture(VUNULL), mpMaskTexture(VUNULL), mPaintColor(255,255,255)
	{
		mSecondaryIconTexture.addProperties(getProperties(), "Secondary Icon Texture");
		addProperty(new VuRectProperty("Secondary Icon Rect", mSecondaryIconRect));

		ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuCarButtonEntity, doResetCarTexture);
	}

	VuRetVal doResetCarTexture(const VuParams &params)
	{
		const std::string &name = VuGameManager::IF()->getCurCarName();
		mpIconTexture = VuAssetFactory::IF()->createAsset<VuTextureAsset>("UI/Icon/Car_" + name, VuAssetFactory::OPTIONAL_ASSET);
		mpMaskTexture = VuAssetFactory::IF()->createAsset<VuTextureAsset>("UI/Icon/Car_" + name + "_m", VuAssetFactory::OPTIONAL_ASSET);

		const VuJsonContainer &colorData = VuDataUtil::findArrayMember(VuGameUtil::IF()->colorDB(), "Name", VuGameManager::IF()->getCurCar().mPaintColor);
		VuDataUtil::getValue(colorData["Color"], mPaintColor);
		return VuRetVal();
	}

	virtual void onGameInitialize()
	{
		VuGameButtonBaseEntity::onGameInitialize();

		const std::string &name = VuGameManager::IF()->getCurCarName();
		mpIconTexture = VuAssetFactory::IF()->createAsset<VuTextureAsset>("UI/Icon/Car_" + name, VuAssetFactory::OPTIONAL_ASSET);
		mpMaskTexture = VuAssetFactory::IF()->createAsset<VuTextureAsset>("UI/Icon/Car_" + name + "_m", VuAssetFactory::OPTIONAL_ASSET);

		const VuJsonContainer &colorData = VuDataUtil::findArrayMember(VuGameUtil::IF()->colorDB(), "Name", VuGameManager::IF()->getCurCar().mPaintColor);
		VuDataUtil::getValue(colorData["Color"], mPaintColor);
	}

	virtual void onGameRelease()
	{
		VuAssetFactory::IF()->releaseAsset(mpIconTexture);
		VuAssetFactory::IF()->releaseAsset(mpMaskTexture);
	}

protected:
	virtual void drawIcon(float depth, const VuColor &color, const VuRect &dstRect, const VuRect &srcRect)
	{
		if ( mpIconTexture && mpMaskTexture )
		{
			VuGameGfxUtil::CarIconDrawParams params;

			params.mDepth = depth;
			params.mpIconTexture = mpIconTexture->getTexture();
			params.mpMaskTexture = mpMaskTexture->getTexture();
			params.mDstRect = dstRect;
			params.mSrcRect = srcRect;
			params.mColor = color;
			params.mPaintColor = mPaintColor;

			VuGameGfxUtil::IF()->drawCarIcon(params);
		}
	}

	virtual void drawSecondaryIcon(float depth, const VuColor &color, const VuUIDrawParams &uiDrawParams)
	{
		if ( VuTexture *pSecondaryIcon = mSecondaryIconTexture.getTexture() )
		{
			VuRect dstRect = mSecondaryIconRect;

			if ( mPressedState == PRESSED )
				dstRect.scale(mImageRect.getCenter(), mPressedScale);

			dstRect = uiDrawParams.transform(dstRect);
			mAnchor.apply(dstRect, dstRect);

			if ( isDisabled() )
				VuGameGfxUtil::IF()->drawGrayScaleTexture2d(depth, pSecondaryIcon, color, dstRect);
			else
				VuGfxUtil::IF()->drawTexture2d(depth, pSecondaryIcon, color, dstRect);
		}
	}

	VuUIImageProperties		mSecondaryIconTexture;
	VuUIRectProperties		mSecondaryIconRect;

	VuTextureAsset	*mpIconTexture;
	VuTextureAsset	*mpMaskTexture;
	VuColor			mPaintColor;
};

IMPLEMENT_RTTI(VuCarButtonEntity, VuGameButtonBaseEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuCarButtonEntity);


//*****************************************************************************
// VuDriverButtonEntity
//*****************************************************************************

class VuDriverButtonEntity : public VuGameButtonBaseEntity
{
	DECLARE_RTTI

public:
	VuDriverButtonEntity() : mpIconTexture(VUNULL)
	{
		mSecondaryIconTexture.addProperties(getProperties(), "Secondary Icon Texture");
		addProperty(new VuRectProperty("Secondary Icon Rect", mSecondaryIconRect));
	}

	virtual void onGameInitialize()
	{
		VuGameButtonBaseEntity::onGameInitialize();

		const std::string &name = VuGameManager::IF()->getCurDriverName();
		mpIconTexture = VuAssetFactory::IF()->createAsset<VuTextureAsset>("UI/Icon/Driver_" + name, VuAssetFactory::OPTIONAL_ASSET);
	}

	virtual void onGameRelease()
	{
		VuAssetFactory::IF()->releaseAsset(mpIconTexture);
	}

protected:
	virtual void drawIcon(float depth, const VuColor &color, const VuRect &dstRect, const VuRect &srcRect)
	{
		if ( mpIconTexture )
			VuGfxUtil::IF()->drawTexture2d(depth, mpIconTexture->getTexture(), color, dstRect, srcRect);
	}

	virtual void drawSecondaryIcon(float depth, const VuColor &color, const VuUIDrawParams &uiDrawParams)
	{
		if ( VuTexture *pSecondaryIcon = mSecondaryIconTexture.getTexture() )
		{
			VuRect dstRect = mSecondaryIconRect;

			if ( mPressedState == PRESSED )
				dstRect.scale(mImageRect.getCenter(), mPressedScale);

			dstRect = uiDrawParams.transform(dstRect);
			mAnchor.apply(dstRect, dstRect);

			if ( isDisabled() )
				VuGameGfxUtil::IF()->drawGrayScaleTexture2d(depth, pSecondaryIcon, color, dstRect);
			else
				VuGfxUtil::IF()->drawTexture2d(depth, pSecondaryIcon, color, dstRect);
		}
	}

	VuUIImageProperties		mSecondaryIconTexture;
	VuUIRectProperties		mSecondaryIconRect;

	VuTextureAsset	*mpIconTexture;
};

IMPLEMENT_RTTI(VuDriverButtonEntity, VuGameButtonBaseEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuDriverButtonEntity);









// modify by xlm,[/2014/12/22 ] 
//*****************************************************************************
// VuGiftButtonEntity
//*****************************************************************************

class VuBigGiftButtonEntity : public VuGameButtonBaseEntity
{
	DECLARE_RTTI

public:
	VuBigGiftButtonEntity()
	{
		// properties
		mIconUnRmbUserTexture.addProperties(getProperties(), "UnRmb Icon Texture");
		mIconRmbUserTexture.addProperties(getProperties(), "Rmb Icon Texture");

		ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuBigGiftButtonEntity, doStart);

	}

protected:

	VuRetVal						doStart(const VuParams &params)
	{
		if (__isRmb() && __isGainedGift())
		{
			mHidden = true;
		}
		else
			mHidden = false;
		return VuRetVal();
	}

	virtual void drawIcon(float depth, const VuColor &color, const VuRect &dstRect, const VuRect &srcRect)
	{
		VuTexture* pIcon = NULL;

		if (__isRmb())
		{
			if (!__isGainedGift())
			{
				pIcon = mIconRmbUserTexture.getTexture();

			}
			else
			{
				mHidden = true;
				return;
			}
		}
		else //非rmb玩家
		{
			pIcon = mIconUnRmbUserTexture.getTexture();
		}

		if (pIcon)
			VuGfxUtil::IF()->drawTexture2d(depth, pIcon, color, dstRect, srcRect);

		/*if (VuTexture *pIcon = mIconTexture.getTexture())
		{
			if (isDisabled())
				VuGameGfxUtil::IF()->drawGrayScaleTexture2d(depth, pIcon, color, dstRect, srcRect);
			else
				VuGfxUtil::IF()->drawTexture2d(depth, pIcon, color, dstRect, srcRect);
		}*/
	}

private:
	bool __isRmb()
	{
		bool bRet = false;
		bRet = VuBillingManager::IF()->isOwned("RMBUser");

		return bRet;
	}

	bool __isGainedGift()
	{
		bool bRet = false;
		bRet = VuBillingManager::IF()->isOwned("BigGift");

		return bRet;
	}



	VuUIImageProperties		mIconUnRmbUserTexture;  //非rmb玩家图片
	VuUIImageProperties		mIconRmbUserTexture;   //rmb玩家图片

};

IMPLEMENT_RTTI(VuBigGiftButtonEntity, VuGameButtonBaseEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuBigGiftButtonEntity);
















// modify by xlm,[/2014/12/22 ] 
//*****************************************************************************
// VuFirstChargeButtonEntity
//*****************************************************************************

class VuFirstChargeButtonEntity : public VuGameButtonBaseEntity
{
	DECLARE_RTTI

public:
	VuFirstChargeButtonEntity()
	{
		// properties
		mIconUnRmbUserTexture.addProperties(getProperties(), "UnCharged Icon");
		mIconRmbUserTexture.addProperties(getProperties(), "Charged Icon");

		//ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuFirstChargeButtonEntity, doStart);

	}

protected:

	/*VuRetVal						doStart(const VuParams &params)
	{
		if (__isRmb() && __isGainedGift())
		{
			mHidden = true;
		}
		else
			mHidden = false;
		return VuRetVal();
	}*/

	virtual void drawIcon(float depth, const VuColor &color, const VuRect &dstRect, const VuRect &srcRect)
	{
		VuTexture* pIcon = NULL;

		bool bPurchased = __isPurchasedCoinOrDiam();

		if (bPurchased)
		{
			/*if (!__isGainedGift())
			{
				pIcon = mIconRmbUserTexture.getTexture();

			}
			else
			{
				mHidden = true;
				return;
			}e
*/

			pIcon = mIconRmbUserTexture.getTexture();
		}
		else //非rmb玩家
		{
			pIcon = mIconUnRmbUserTexture.getTexture();
		}


		if (pIcon)
		{
			if (bPurchased)
			{
				VuGfxUtil::IF()->drawTexture2d(depth, pIcon, color, dstRect, srcRect);

			}
			else
			{
				VuRect tmpRect(dstRect.mX, dstRect.mY - dstRect.mHeight/ 4.0, dstRect.mWidth * 4 / 3.0, dstRect.mHeight * 4 / 3.0);
				VuGfxUtil::IF()->drawTexture2d(depth, pIcon, color, tmpRect, srcRect);

			}

		}
			//VuGfxUtil::IF()->drawTexture2d(depth, pIcon, color, dstRect, srcRect);

		/*if (VuTexture *pIcon = mIconTexture.getTexture())
		{
		if (isDisabled())
		VuGameGfxUtil::IF()->drawGrayScaleTexture2d(depth, pIcon, color, dstRect, srcRect);
		else
		VuGfxUtil::IF()->drawTexture2d(depth, pIcon, color, dstRect, srcRect);
		}*/
	}

private:
	bool __isPurchasedCoinOrDiam()
	{
		bool bRet = false;
		bRet = VuBillingManager::IF()->isOwned("PurchasedDiam") || VuBillingManager::IF()->isOwned("PurchasedCoin");

		return bRet;
	}

	VuUIImageProperties		mIconUnRmbUserTexture;  //非rmb玩家图片
	VuUIImageProperties		mIconRmbUserTexture;   //rmb玩家图片

};

IMPLEMENT_RTTI(VuFirstChargeButtonEntity, VuGameButtonBaseEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuFirstChargeButtonEntity);


// modify by xlm,[/2015/2/6 ] 

//*****************************************************************************
// VuLoginRewardButtonEntity
//*****************************************************************************
class VuLoginRewardButtonEntity : public VuGameButtonBaseEntity
{
	DECLARE_RTTI

public:
	VuLoginRewardButtonEntity() : mDayNo(1)
	{
		// properties
		mUnReceivedIcon.addProperties(getProperties(), "UnReceivedIcon");
		mReceivedIcon.addProperties(getProperties(), "ReceivedIcon");


		mSecondaryIconTexture.addProperties(getProperties(), "Secondary Icon Texture");
		addProperty(new VuRectProperty("Secondary Icon Rect", mSecondaryIconRect));

		//ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuFirstChargeButtonEntity, doStart);

		addProperty(new VuIntProperty("DayNo", mDayNo));

	}

protected:

	/*VuRetVal						doStart(const VuParams &params)
	{
	if (__isRmb() && __isGainedGift())
	{
	mHidden = true;
	}
	else
	mHidden = false;
	return VuRetVal();
	}*/

	virtual void drawIcon(float depth, const VuColor &color, const VuRect &dstRect, const VuRect &srcRect)
	{
		//VuTexture* pIcon = NULL;
		VuTexture* pUnReceivedTexture = mUnReceivedIcon.getTexture();
		VuTexture* pReceivedTexture = mReceivedIcon.getTexture();
	
		/*bool bPurchased = __isPurchasedCoinOrDiam();*/
		int nState = __getCurrentState();
		if (nState == 1)
		{
			/*if (pUnReceivedTexture)
				VuGfxUtil::IF()->drawTexture2d(depth, pUnReceivedTexture, color, dstRect, srcRect);

			/ *VuRect tmpRect(dstRect.mX, dstRect.mY - dstRect.mHeight / 4.0, dstRect.mWidth * 4 / 3.0, dstRect.mHeight * 4 / 3.0);* /
			if (pReceivedTexture)			
			VuGfxUtil::IF()->drawTexture2d(depth - 20, pReceivedTexture, color, dstRect, srcRect);*/

			std::string bodyText = VuStringDB::IF()->getString("Received_Login_Text");

			const char *stringId = bodyText.c_str();
			if (!mGamePadStringID.empty() && (VuControlMethodManager::IF()->getMethod() == VuControlMethodManager::METHOD_GAMEPAD))
				stringId = mGamePadStringID.c_str();

			if (stringId[0])
			{
				const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mFont);
				const char *strText = VuStringDB::IF()->getStringSelf(stringId);

				VuFontDrawParams fdParams = fontEntry.params();
				VuRect dstRect = mTextRect;

				if (mPressedState == PRESSED)
				{
					fdParams.mSize *= mPressedScale;
					dstRect.scale(mImageRect.getCenter(), mPressedScale);
				}
				fdParams.mSize *= 0.5f*(mpTransformComponent->getWorldScale().mX + mpTransformComponent->getWorldScale().mY);

				VuUIDrawParams uiDrawParams;
				VuUIDrawUtil::getParams(this, uiDrawParams);
				dstRect = uiDrawParams.transform(dstRect);
				mAnchor.apply(dstRect, dstRect);

				VUUINT32 flags = VUGFX_TEXT_DRAW_HCENTER | VUGFX_TEXT_DRAW_VCENTER;
				VuGfxUtil::IF()->fontDraw()->drawString(depth, fontEntry.font(), strText, fdParams, dstRect, flags, 1.0f);
			}







			mDisabled = true;

		}
		else if (nState == 2)
		{
			/*if (pUnReceivedTexture)
				VuGfxUtil::IF()->drawTexture2d(depth, pUnReceivedTexture, color, dstRect, srcRect);*/
			/*drawSecondaryIcon();
*/
			mDisabled = false;
			VuTexture* pSecondTexture = mSecondaryIconTexture.getTexture();
			if (pSecondTexture)
			{
				VuRect dstRect = mSecondaryIconRect;
				if (mPressedState == PRESSED)
					dstRect.scale(mImageRect.getCenter(), mPressedScale);
				VuUIDrawParams uiDrawParams;
				VuUIDrawUtil::getParams(this, uiDrawParams);
				dstRect = uiDrawParams.transform(dstRect);
				mAnchor.apply(dstRect, dstRect);
				if (isDisabled())
					VuGameGfxUtil::IF()->drawGrayScaleTexture2d(depth, pSecondTexture, color, dstRect);
				else
					VuGfxUtil::IF()->drawTexture2d(depth, pSecondTexture, color, dstRect);
			}
		}
		else if (nState == 3)
		{
			mDisabled = false;
			/*mDisabled = true;
			if (pUnReceivedTexture)
				VuGfxUtil::IF()->drawTexture2d(depth, pUnReceivedTexture, color, dstRect, srcRect);*/
		}

	}

private:
	/*
	@ Author:	xlm(2015-2-6 10:22)
	@ Parameter:
	@ Returns:
	@ Description:1 表示已经领取  2 表示可领取  3表示未领取（不能领取）
	*/
	int __getCurrentState()
	{
		if (VuGameManager::IF())
			return VuGameManager::IF()->getCurrDayState(mDayNo);
		return 3;
	}

	/*bool __isPurchasedCoinOrDiam()
	{
		bool bRet = false;
		bRet = VuBillingManager::IF()->isOwned("PurchasedDiam") || VuBillingManager::IF()->isOwned("PurchasedCoin");

		return bRet;
	}
*/


	VuUIImageProperties		mUnReceivedIcon;  //非rmb玩家图片
	VuUIImageProperties		mReceivedIcon;   //rmb玩家图片


	VuUIImageProperties		mSecondaryIconTexture;
	VuUIRectProperties		mSecondaryIconRect;

	int						mDayNo;


	

};

IMPLEMENT_RTTI(VuLoginRewardButtonEntity, VuGameButtonBaseEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuLoginRewardButtonEntity);

