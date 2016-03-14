//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  HUD OnScreenControl classes
// 
//*****************************************************************************

#include "VuPurple/Entities/Car/Driver/VuHumanDriverEntity.h"
#include "VuPurple/Managers/VuControlMethodManager.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/UI/VuUI.h"
#include "VuEngine/UI/VuUIDrawUtil.h"
#include "VuEngine/UI/VuUIPropertyUtil.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/2dLayout/Vu2dLayoutComponent.h"
#include "VuEngine/Properties/VuAssetProperty.h"
#include "VuEngine/Assets/VuTextureAsset.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/HAL/Touch/VuTouch.h"
#include "VuEngine/Managers/VuInputManager.h"


//*****************************************************************************
class VuInputButtonEnumProperty : public VuStringEnumProperty
{
public:
	VuInputButtonEnumProperty(const char *strName, std::string &pValue) : VuStringEnumProperty(strName, pValue) {}

	virtual int			getChoiceCount() const		{ return VuGameUtil::IF()->inputDB()["Buttons"].size(); }
	virtual const char	*getChoice(int index) const	{ return VuGameUtil::IF()->inputDB()["Buttons"][index]["Name"].asCString(); }
};

//*****************************************************************************
class VuInputAxisEnumProperty : public VuStringEnumProperty
{
public:
	VuInputAxisEnumProperty(const char *strName, std::string &pValue) : VuStringEnumProperty(strName, pValue) {}

	virtual int			getChoiceCount() const		{ return VuGameUtil::IF()->inputDB()["Axes"].size(); }
	virtual const char	*getChoice(int index) const	{ return VuGameUtil::IF()->inputDB()["Axes"][index]["Name"].asCString(); }
};


//*****************************************************************************
// OnScreenControl
//*****************************************************************************
class VuHUDOnScreenControlEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuHUDOnScreenControlEntity();

protected:
	// event handlers
	void				OnUITick(const VuParams &params);
	void				OnUIDraw(const VuParams &params);

	virtual void		onTickDown() = 0;
	virtual bool		isPressed() = 0;

	bool				isEnabled();

	void				drawControl(float alpha);
	void				drawLayout(bool bSelected);

	// components

	// properties
	VuUIRectProperties		mImageRect;
	VuUIRectProperties		mTouchRect;
	VuUIImageProperties		mImage;
	VuUIImageProperties		mPressedImage;
	bool					mFlipX;
	bool					mFlipY;
	VuUIAnchorProperties	mAnchor;
	bool					mEnableForTilt;
	bool					mEnableForTouchA;
	bool					mEnableForTouchB;
	bool					mEnableForGamePad;
	bool					mEnableForKeyboard;

	bool					mTouchDown;
	VuVector2				mTouchPos;
	int						mPadIndex;
};

IMPLEMENT_RTTI(VuHUDOnScreenControlEntity, VuEntity);

//*****************************************************************************
VuHUDOnScreenControlEntity::VuHUDOnScreenControlEntity():
	mImageRect(0,0,100,100),
	mTouchRect(0,0,100,100),
	mFlipX(false),
	mFlipY(false),
	mEnableForTilt(true),
	mEnableForTouchA(true),
	mEnableForTouchB(true),
	mEnableForGamePad(false),
	mEnableForKeyboard(false),
	mTouchDown(false),
	mPadIndex(0)
{
	// components
	addComponent(new Vu2dLayoutComponent(this, &VuHUDOnScreenControlEntity::drawLayout));

	// properties
	addProperty(new VuRectProperty("Image Rect", mImageRect));
	addProperty(new VuRectProperty("Touch Rect", mTouchRect));
	mImage.addProperties(getProperties(), "Texture Asset");
	mPressedImage.addProperties(getProperties(), "Pressed Texture Asset");
	addProperty(new VuBoolProperty("FlipX", mFlipX));
	addProperty(new VuBoolProperty("FlipY", mFlipY));
	ADD_UI_ANCHOR_PROPERTIES(getProperties(), mAnchor, "");
	addProperty(new VuBoolProperty("Enable for Tilt", mEnableForTilt));
	addProperty(new VuBoolProperty("Enable for TouchA", mEnableForTouchA));
	addProperty(new VuBoolProperty("Enable for TouchB", mEnableForTouchB));
	addProperty(new VuBoolProperty("Enable for GamePad", mEnableForGamePad));
	addProperty(new VuBoolProperty("Enable for Keyboard", mEnableForKeyboard));

	// event handlers
	REG_EVENT_HANDLER(VuHUDOnScreenControlEntity, OnUITick);
	REG_EVENT_HANDLER(VuHUDOnScreenControlEntity, OnUIDraw);
}

//*****************************************************************************
void VuHUDOnScreenControlEntity::OnUITick(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	float fdt = accessor.getFloat();

	if ( isEnabled() )
	{
		if ( VuCarEntity *pCar = VuCarManager::IF()->getCameraTargetForViewport(0) )
			if ( pCar->getDriver()->isHuman() )
				mPadIndex = static_cast<VuHumanDriver *>(pCar->getDriver())->getPadIndex();

		VuUIDrawParams uiDrawParams;
		VuUIDrawUtil::getParams(this, uiDrawParams);

		mTouchDown = false;
		for ( int i = 0; i < VuTouch::IF()->getTouchCount(); i++ )
		{
			VuVector2 touch;
			VuTouch::IF()->getTouch(i, touch);

			VuVector2 cropTouch = VuUI::IF()->getInvCropMatrix().transform(touch);
			VuRect touchRect = uiDrawParams.transform(mTouchRect);
			mAnchor.apply(touchRect, touchRect);
			mTouchDown |= touchRect.contains(cropTouch);
			mTouchPos = cropTouch;
		}

		if ( mTouchDown )
			onTickDown();
	}
}

//*****************************************************************************
void VuHUDOnScreenControlEntity::OnUIDraw(const VuParams &params)
{
	float alpha = 1.0f;

	if ( isEnabled() )
	{
		drawControl(alpha);
	}
}

//*****************************************************************************
bool VuHUDOnScreenControlEntity::isEnabled()
{
	if ( VuCarManager::IF()->getLocalHumanCarCount() == 0 )
		return false;

	if ( !mEnableForTilt && VuControlMethodManager::IF()->isTilt() )
		return false;

	if ( !mEnableForTouchA && VuControlMethodManager::IF()->isTouchA() )
		return false;

	if ( !mEnableForTouchB && VuControlMethodManager::IF()->isTouchB() )
		return false;

	if ( !mEnableForGamePad && VuControlMethodManager::IF()->isGamePad() )
		return false;

	if ( !mEnableForKeyboard && VuControlMethodManager::IF()->isKeyboard() )
		return false;

	return true;
}

//*****************************************************************************
void VuHUDOnScreenControlEntity::drawControl(float alpha)
{
	bool pressed = isPressed();

	VuTexture *pTexture = mImage.getTexture();
	if ( pressed )
		pTexture = mPressedImage.getTexture();

	if ( pTexture )
	{
		VuUIDrawParams uiDrawParams;
		VuUIDrawUtil::getParams(this, uiDrawParams);

		VuRect imageRect = uiDrawParams.transform(mImageRect);
		if ( pressed && mImage.getTextureAsset() == VUNULL )
		{
			imageRect.mX = mTouchPos.mX - 0.5f*imageRect.mWidth;
			imageRect.mY = mTouchPos.mY - 0.5f*imageRect.mHeight;
		}
		else
		{
			mAnchor.apply(imageRect, imageRect);
		}

		VuRect srcRect(0,0,1,1);
		if ( mFlipX )
			srcRect.flipX();
		if ( mFlipY )
			srcRect.flipY();

		VuColor color(255,255,255, VuRound(255*alpha));
		VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth, pTexture, color, imageRect, srcRect);
	}
}

//*****************************************************************************
void VuHUDOnScreenControlEntity::drawLayout(bool bSelected)
{
	// draw rects
	if ( bSelected )
	{
		VuUIDrawParams uiDrawParams;
		VuUIDrawUtil::getParams(this, uiDrawParams);

		VuRect touchRect = uiDrawParams.transform(mTouchRect);
		mAnchor.apply(touchRect, touchRect);
		VuGfxUtil::IF()->drawRectangleOutline2d(uiDrawParams.mDepth, VuColor(255,255,255), touchRect);

		VuRect imageRect = uiDrawParams.transform(mImageRect);
		mAnchor.apply(imageRect, imageRect);
		VuGfxUtil::IF()->drawRectangleOutline2d(uiDrawParams.mDepth, VuColor(255,255,255), imageRect);
	}

	// draw
	drawControl(1.0f);
}


//*****************************************************************************
// OnScreenButton
//*****************************************************************************
class VuHUDOnScreenButtonEntity : public VuHUDOnScreenControlEntity
{
	DECLARE_RTTI

public:
	VuHUDOnScreenButtonEntity();

protected:
	virtual void		onTickDown();
	virtual bool		isPressed();

	// properties
	std::string			mButtonName;
};

IMPLEMENT_RTTI(VuHUDOnScreenButtonEntity, VuHUDOnScreenControlEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuHUDOnScreenButtonEntity);


//*****************************************************************************
VuHUDOnScreenButtonEntity::VuHUDOnScreenButtonEntity()
{
	addProperty(new VuInputButtonEnumProperty("Button Name", mButtonName));
}

//*****************************************************************************
void VuHUDOnScreenButtonEntity::onTickDown()
{
	VuInputManager::IF()->setOnScreenButton(mPadIndex, mButtonName.c_str());
}

//*****************************************************************************
bool VuHUDOnScreenButtonEntity::isPressed()
{
	if ( VuInputManager::IF() )
		return VuInputManager::IF()->getButtonValue(mPadIndex, mButtonName.c_str());

	return false;
}


//*****************************************************************************
// OnScreenAxis
//*****************************************************************************
class VuHUDOnScreenAxisEntity : public VuHUDOnScreenControlEntity
{
	DECLARE_RTTI

public:
	VuHUDOnScreenAxisEntity();

protected:
	virtual void		onTickDown();
	virtual bool		isPressed();

	// properties
	std::string			mAxisName;
	int					mDirection;
};

IMPLEMENT_RTTI(VuHUDOnScreenAxisEntity, VuHUDOnScreenControlEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuHUDOnScreenAxisEntity);


//*****************************************************************************
VuHUDOnScreenAxisEntity::VuHUDOnScreenAxisEntity():
	mDirection(1)
{
	addProperty(new VuInputAxisEnumProperty("Axis Name", mAxisName));

	static VuStaticIntEnumProperty::Choice choices[] = { { "positive", 1 }, { "negative", -1 }, { VUNULL } };
	addProperty(new VuStaticIntEnumProperty("Axis Direction", mDirection, choices));
}

//*****************************************************************************
void VuHUDOnScreenAxisEntity::onTickDown()
{
	VuInputManager::IF()->setOnScreenAxis(mPadIndex, mAxisName.c_str(), (float)mDirection);
}

//*****************************************************************************
bool VuHUDOnScreenAxisEntity::isPressed()
{
	if ( VuInputManager::IF() )
		return VuInputManager::IF()->getRawAxisValue(mPadIndex, mAxisName.c_str())*mDirection > 0;

	return false;
}




