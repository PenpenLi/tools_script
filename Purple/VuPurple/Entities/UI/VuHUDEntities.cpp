//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  HUD class
// 
//*****************************************************************************

#include "VuHUDEntities.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Driver/VuHumanDriverEntity.h"
#include "VuPurple/Entities/Car/Camera/VuCarCamera.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Managers/VuControlMethodManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/UI/VuUI.h"
#include "VuEngine/Entities/UI/VuUITextBaseEntity.h"
#include "VuEngine/Entities/UI/VuUIImageEntity.h"
#include "VuEngine/UI/VuUIDrawUtil.h"
#include "VuEngine/Components/2dLayout/Vu2dLayoutComponent.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Properties/VuAudioProperty.h"
#include "VuEngine/Properties/VuFontEnumProperty.h"
#include "VuEngine/Assets/VuTextureAsset.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Managers/VuInputManager.h"
#include "VuEngine/Managers/VuDialogManager.h"
#include "VuEngine/HAL/Touch/VuTouch.h"
#include "VuEngine/HAL/Gfx/VuGfx.h"
#include "VuEngine/HAL/Gfx/VuTexture.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/Shaders/VuBasicShaders.h"
#include "VuEngine/Gfx/GfxSort/VuGfxSort.h"
#include "VuEngine/DB/VuStringDB.h"
#include "VuEngine/Memory/VuScratchPad.h"
#include "VuEngine/Math/VuMathUtil.h"
#include "VuEngine/Util/VuStringUtil.h"
#include "VuEngine/Util/VuAudioUtil.h"


IMPLEMENT_RTTI(VuHUDEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuHUDEntity);


//*****************************************************************************
VuHUDEntity::VuHUDEntity() : VuEntity(CAN_HAVE_CHILDREN), mInputUtil(this)
{
	// event handlers
	REG_EVENT_HANDLER(VuHUDEntity, DisableInput);
	REG_EVENT_HANDLER(VuHUDEntity, EnableInput);
}

//*****************************************************************************
void VuHUDEntity::onGameInitialize()
{
	mInputUtil.enable();
}

//*****************************************************************************
void VuHUDEntity::onGameRelease()
{
	mInputUtil.disable();
}

//*****************************************************************************
void VuHUDEntity::tick(float fdt, VUUINT32 padMask)
{
	if ( !VuMessageBoxManager::IF()->getActiveMessageBox() && !VuDialogManager::IF()->getActiveDialog() )
	{
		// input
		mInputUtil.tick(padMask);

		VuParams params;
		params.addFloat(fdt);
		handleEventRecursive("OnUITick", params);
	}
}

//*****************************************************************************
void VuHUDEntity::draw()
{
	VuGfxSort::IF()->setFullScreenLayer(VuGfxSort::FSL_HUD);
	VuGfxSort::IF()->setViewport(0);
	VuGfxSort::IF()->setReflectionLayer(0);
	VuGfxSort::IF()->setViewportLayer(VuGfxSort::VPL_HUD);

	// set up crop matrix and text scale
	VuGfxUtil::IF()->pushMatrix(VuUI::IF()->getCropMatrix());
	VuGfxUtil::IF()->pushTextScale(VuUI::IF()->getTextScale());

	// draw
	handleEventRecursive("OnUIDraw");

	// pop state
	VuGfxUtil::IF()->popTextScale();
	VuGfxUtil::IF()->popMatrix();
}


//*****************************************************************************
// HUD Text Element
//*****************************************************************************
class VuHUDTextElementEntity : public VuUITextBaseEntity
{
	DECLARE_RTTI

public:
	VuHUDTextElementEntity();

private:
	enum { TOTAL_TIME, LAP_TIME, BEST_LAP_TIME, LAP_NUM, LAP_DEN, PLACE_NUM, PLACE_DEN, WRONG_WAY, OUT_OF_BOUNDS, GAME_TIMER, SCORE, ELIMINATION_COUNT, REMATCH_TIMER };

	virtual const char *getText();

	// properties
	int			mViewport;
	int			mType;

	std::string	mText;
};

IMPLEMENT_RTTI(VuHUDTextElementEntity, VuUITextBaseEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuHUDTextElementEntity);


//*****************************************************************************
VuHUDTextElementEntity::VuHUDTextElementEntity():
	mViewport(0),
	mType(TOTAL_TIME)
{
	// add properties
	addProperty(new VuIntProperty("Viewport", mViewport));
	static VuStaticIntEnumProperty::Choice typeChoices[] =
	{
		{ "Total Time", TOTAL_TIME },
		{ "Lap Time", LAP_TIME },
		{ "Best Lap Time", BEST_LAP_TIME },
		{ "Lap Numerator", LAP_NUM },
		{ "Lap Denominator", LAP_DEN },
		{ "Place Numerator", PLACE_NUM },
		{ "Place Denominator", PLACE_DEN },
		{ "Wrong Way", WRONG_WAY },
		{ "Out of Bounds", OUT_OF_BOUNDS },
		{ "Game Timer", GAME_TIMER },
		{ "Score", SCORE },
		{ "Elimination Count", ELIMINATION_COUNT },
		{ "Rematch Timer", REMATCH_TIMER },
		{ VUNULL }
	};
	addProperty(new VuStaticIntEnumProperty("Type", mType, typeChoices));
}

//*****************************************************************************
const char *VuHUDTextElementEntity::getText()
{
	float totalTime = 183.45f;
	float lapTime = 143.45f;
	float bestLapTime = 144.56f;
	float wrongWayAlpha = 1.0f;
	float outOfBoundsAlpha = 1.0f;
	int lap = 2, lapCount = 3;
	int place = 2, carCount = 8;
	int gameTimer = 65;
	int score = 1775;
	int eliminationCount = 8;
	int rematchTimer = 5;

	if ( VuCarManager::IF() )
	{
		if ( VuCarEntity *pCar = VuCarManager::IF()->getCameraTargetForViewport(mViewport) )
		{
			VuCarStats *pStats = &pCar->getStats();

			totalTime = (float)pStats->mTotalTime;
			lapTime = (float)pStats->mCurLapTime;
			bestLapTime = pStats->mBestLapTime;
			wrongWayAlpha = pStats->mWrongWayAlpha;
			outOfBoundsAlpha = pStats->mOutOfBoundsAlpha;
			lap = pStats->mCurLap;
			lapCount = pStats->mLapCount;
			place = pStats->mPlace;
			carCount = VuCarManager::IF()->getCarCount();
			gameTimer = pStats->mGameTimer;
			score = pStats->mScore;
			eliminationCount = pStats->mEliminationCount;
			rematchTimer = pStats->mRematchTimer;
		}
	}

	char str[256] = "";

	if ( mType == TOTAL_TIME )
	{
		VuStringUtil::timeFormatSeconds(VuStringUtil::MM_SS, totalTime, str, 256);
	}
	else if ( mType == LAP_TIME )
	{
		VuStringUtil::timeFormatSeconds(VuStringUtil::MM_SS, lapTime, str, 256);
	}
	else if ( mType == BEST_LAP_TIME )
	{
		if ( bestLapTime < FLT_MAX )
			VuStringUtil::timeFormatSeconds(VuStringUtil::MM_SS, bestLapTime, str, 256);
		else
			VU_STRCPY(str, sizeof(str), VuStringDB::IF()->getString("HUD_NoTime").c_str());
	}
	else if ( mType == LAP_NUM )
	{
		VU_SPRINTF(str, sizeof(str), "%d", VuClamp(lap, 1, lapCount));
	}
	else if ( mType == LAP_DEN )
	{
		VU_SPRINTF(str, sizeof(str), "/%d", lapCount);
	}
	else if ( mType == PLACE_NUM )
	{
		VU_SPRINTF(str, sizeof(str), "%d", place);
	}
	else if ( mType == PLACE_DEN )
	{
		VU_SPRINTF(str, sizeof(str), "/%d", carCount);
	}
	else if ( mType == WRONG_WAY )
	{
		if ( wrongWayAlpha > 0.0f )
			VU_STRCPY(str, sizeof(str), VuStringDB::IF()->getString("HUD_WrongWay").c_str());
		mAlpha = wrongWayAlpha;
	}
	else if ( mType == OUT_OF_BOUNDS )
	{
		if ( outOfBoundsAlpha > 0.0f )
			VU_STRCPY(str, sizeof(str), VuStringDB::IF()->getString("HUD_OutOfBounds").c_str());
		mAlpha = outOfBoundsAlpha;
	}
	else if ( mType == GAME_TIMER )
	{
		if ( gameTimer >= 0 )
			VuStringUtil::timeFormatSeconds(VuStringUtil::MM_SS, gameTimer, str, 256);
	}
	else if ( mType == SCORE )
	{
		VuStringUtil::integerFormat(score, str, 256);
	}
	else if ( mType == ELIMINATION_COUNT )
	{
		VU_SPRINTF(str, sizeof(str), "/%d", eliminationCount);
	}
	else if ( mType == REMATCH_TIMER )
	{
		if ( rematchTimer >= 0 )
			VuStringUtil::timeFormatSeconds(VuStringUtil::MM_SS, rematchTimer, str, 256);
	}

	mText = str;
	return mText.c_str();
}


//*****************************************************************************
// HUD Button
//*****************************************************************************
class VuHUDButtonEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuHUDButtonEntity();

protected:
	// scripting
	VuRetVal			Show(const VuParams &params) { mVisible = true; return VuRetVal(); }
	VuRetVal			Hide(const VuParams &params) { mVisible = false; return VuRetVal(); }

	// event handlers
	virtual void		OnUITick(const VuParams &params);
	virtual void		OnUITouch(const VuParams &params);
	virtual void		OnUIDraw(const VuParams &params);

	virtual void		onTick(float fdt) {}
	virtual void		onPressed() = 0;
	virtual bool		isActive();

	virtual void		drawButton(float alpha);
	void				drawLayout(bool bSelected);

	// components
	VuScriptComponent	*mpScriptComponent;

	// properties
	bool					mVisible;
	VuRect					mImageRect;
	VuUIImageProperties		mImage;
	VuUIImageProperties		mPressedImage;
	VuRect					mTouchRect;
	VuUIAnchorProperties	mAnchor;
	bool					mEnableForTilt;
	bool					mEnableForTouchA;
	bool					mEnableForTouchB;
	bool					mEnableForGamePad;
	bool					mEnableForKeyboard;

	bool					mWasPressed;

	enum ePressedState { NOT_PRESSED, PRESSED };
	ePressedState			mPressedState;
	VuVector2				mTouchPos;
};

IMPLEMENT_RTTI(VuHUDButtonEntity, VuEntity);

//*****************************************************************************
VuHUDButtonEntity::VuHUDButtonEntity():
	mVisible(true),
	mImageRect(0,0,100,100),
	mTouchRect(0,0,100,100),
	mEnableForTilt(true),
	mEnableForTouchA(true),
	mEnableForTouchB(true),
	mEnableForGamePad(false),
	mEnableForKeyboard(false),
	mWasPressed(false),
	mPressedState(NOT_PRESSED)
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this, 150, false));
	addComponent(new Vu2dLayoutComponent(this, &VuHUDButtonEntity::drawLayout));

	// properties
	addProperty(new VuBoolProperty("Initially Visible", mVisible));
	addProperty(new VuRectProperty("Image Rect", mImageRect));
	addProperty(new VuRectProperty("Touch Rect", mTouchRect));
	mImage.addProperties(getProperties(), "Texture Asset");
	mPressedImage.addProperties(getProperties(), "Pressed Texture Asset");
	ADD_UI_ANCHOR_PROPERTIES(getProperties(), mAnchor, "");
	addProperty(new VuBoolProperty("Enable for Tilt", mEnableForTilt));
	addProperty(new VuBoolProperty("Enable for TouchA", mEnableForTouchA));
	addProperty(new VuBoolProperty("Enable for TouchB", mEnableForTouchB));
	addProperty(new VuBoolProperty("Enable for GamePad", mEnableForGamePad));
	addProperty(new VuBoolProperty("Enable for Keyboard", mEnableForKeyboard));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuHUDButtonEntity, Show);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuHUDButtonEntity, Hide);

	// event handlers
	REG_EVENT_HANDLER(VuHUDButtonEntity, OnUITick);
	REG_EVENT_HANDLER(VuHUDButtonEntity, OnUITouch);
	REG_EVENT_HANDLER(VuHUDButtonEntity, OnUIDraw);
}

//*****************************************************************************
void VuHUDButtonEntity::OnUITick(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	float fdt = accessor.getFloat();

	if ( isActive() )
	{
		if ( mWasPressed )
			onPressed();

		if ( mPressedState == PRESSED )
		{
			VuUIDrawParams uiDrawParams;
			VuUIDrawUtil::getParams(this, uiDrawParams);

			VuRect touchRect = uiDrawParams.transform(mTouchRect);
			mAnchor.apply(touchRect, touchRect);

			bool touched = false;
			for ( int i = 0; i < VuTouch::IF()->getTouchCount(); i++ )
			{
				VuVector2 touchPos;
				VuTouch::IF()->getTouch(i, touchPos);
				touchPos = VuUI::IF()->getInvCropMatrix().transform(touchPos);
				touched |= touchRect.contains(touchPos);
			}

			if ( !touched )
				mPressedState = NOT_PRESSED;
		}

		mWasPressed = false;
	}

	onTick(fdt);
}

//*****************************************************************************
void VuHUDButtonEntity::OnUITouch(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	int action = accessor.getInt();
	VuVector2 touch = accessor.getVector2();

	if ( action == VuUIInputUtil::TOUCH_DOWN )
	{
		if ( isActive() && mPressedState == NOT_PRESSED )
		{
			VuUIDrawParams uiDrawParams;
			VuUIDrawUtil::getParams(this, uiDrawParams);

			VuRect touchRect = uiDrawParams.transform(mTouchRect);
			mAnchor.apply(touchRect, touchRect);

			if ( touchRect.contains(touch) )
			{
				mPressedState = PRESSED;
				mTouchPos = touch;
			}
		}
	}
	else if ( action == VuUIInputUtil::TOUCH_UP )
	{
		if ( isActive() && mPressedState == PRESSED )
		{
			VuUIDrawParams uiDrawParams;
			VuUIDrawUtil::getParams(this, uiDrawParams);

			VuRect touchRect = uiDrawParams.transform(mTouchRect);
			mAnchor.apply(touchRect, touchRect);

			if ( touchRect.contains(touch) )
			{
				mPressedState = NOT_PRESSED;
				mWasPressed = true;
			}
		}
	}
	else if ( action == VuUIInputUtil::TOUCH_MOVE )
	{
		mTouchPos = touch;
	}
}

//*****************************************************************************
void VuHUDButtonEntity::OnUIDraw(const VuParams &params)
{
	float alpha = 1.0f;

	if ( isActive() )
	{
		drawButton(alpha);
	}
}

//*****************************************************************************
bool VuHUDButtonEntity::isActive()
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

	return mVisible;
}

//*****************************************************************************
void VuHUDButtonEntity::drawButton(float alpha)
{
	VuUIImageProperties *pImage = &mImage;
	if ( mPressedState == PRESSED )
		pImage = &mPressedImage;

	if ( VuTexture *pTexture = pImage->getTexture() )
	{
		VuUIDrawParams uiDrawParams;
		VuUIDrawUtil::getParams(this, uiDrawParams);

		VuRect imageRect = uiDrawParams.transform(mImageRect);
		if ( mPressedState == PRESSED && mImage.getTextureAsset() == VUNULL )
		{
			imageRect.mX = mTouchPos.mX - 0.5f*imageRect.mWidth;
			imageRect.mY = mTouchPos.mY - 0.5f*imageRect.mHeight;
		}
		else
		{
			mAnchor.apply(imageRect, imageRect);
		}

		VuColor color(255,255,255, VuRound(255*alpha));
		VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth, pTexture, color, imageRect);
	}
}

//*****************************************************************************
void VuHUDButtonEntity::drawLayout(bool bSelected)
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
	drawButton(1.0f);
}


//*****************************************************************************
// PauseButton
//*****************************************************************************
class VuHUDPauseButtonEntity : public VuHUDButtonEntity
{
	DECLARE_RTTI

private:
	virtual void		onTick(float fdt);
	virtual void		onPressed();
};

IMPLEMENT_RTTI(VuHUDPauseButtonEntity, VuHUDButtonEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuHUDPauseButtonEntity);


//*****************************************************************************
void VuHUDPauseButtonEntity::onTick(float fdt)
{
	if ( VuTouch::IF()->getFocusPriority() == 0 )
	{
		if ( VuCarEntity *pCar = VuCarManager::IF()->getCameraTargetForViewport(0) )
		{
			VuDriverEntity *pDriver = pCar->getDriver();
			if ( pDriver->isHuman() )
			{
				VuHumanDriver *pHumanDriver = static_cast<VuHumanDriver *>(pDriver);
				if ( VuInputManager::IF()->getButtonWasPressed(pHumanDriver->getPadIndex(), "Pause") )
				{
					VuGameUtil::IF()->playSfx(VuGameUtil::SFX_UI_SELECT);
					VuEventManager::IF()->broadcast("OnPauseActionGame");
				}
			}
		}
	}
}

//*****************************************************************************
void VuHUDPauseButtonEntity::onPressed()
{
	VuGameUtil::IF()->playSfx(VuGameUtil::SFX_UI_SELECT);
	VuEventManager::IF()->broadcast("OnPauseActionGame");
}


//*****************************************************************************
// Countdown Timer
//*****************************************************************************
class VuHUDCountdownTimerEntity : public VuUITextBaseEntity
{
	DECLARE_RTTI

public:
	VuHUDCountdownTimerEntity();

private:
	// event handlers
	void				OnUITick(const VuParams &params);

	virtual const char	*getText();

	// properties
	int					mViewport;
	int					mAlertTime;
	std::string			mAlertAudioEventName;

	int					mPrevTimer;
	std::string			mText;
};

IMPLEMENT_RTTI(VuHUDCountdownTimerEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuHUDCountdownTimerEntity);


//*****************************************************************************
VuHUDCountdownTimerEntity::VuHUDCountdownTimerEntity():
	mViewport(0),
	mAlertTime(5),
	mPrevTimer(0)
{
	// properties
	addProperty(new VuIntProperty("Viewport", mViewport));
	addProperty(new VuIntProperty("Alert Time", mAlertTime));
	addProperty(new VuAudioEventNameProperty("Alert SFX", mAlertAudioEventName));

	// scripting
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnAppear);

	// event handlers
	REG_EVENT_HANDLER(VuHUDCountdownTimerEntity, OnUITick);
}

//*****************************************************************************
void VuHUDCountdownTimerEntity::OnUITick(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	float fdt = accessor.getFloat();

	if ( VuCarEntity *pCar = VuCarManager::IF()->getCameraTargetForViewport(mViewport) )
	{
		VuCarStats *pStats = &pCar->getStats();

		if ( pStats->mGameTimer <= mAlertTime )
		{
			if ( pStats->mGameTimer != mPrevTimer )
				VuAudioUtil::playSfx(mAlertAudioEventName.c_str());

			if ( mPrevTimer > mAlertTime )
				mpScriptComponent->getPlug("OnAppear")->execute();
		}

		mPrevTimer = pStats->mGameTimer;
	}
}

//*****************************************************************************
const char *VuHUDCountdownTimerEntity::getText()
{
	int gameTimer = mAlertTime;
	if ( VuCarManager::IF() )
		if ( VuCarEntity *pCar = VuCarManager::IF()->getCameraTargetForViewport(mViewport) )
			gameTimer = pCar->getStats().mGameTimer;

	char str[32] = "";
	if ( gameTimer <= mAlertTime )
		VuStringUtil::timeFormatSeconds(VuStringUtil::MM_SS, gameTimer, str, sizeof(str));
	mText = str;

	return mText.c_str();
}


//*****************************************************************************
// Eliminated Name
//*****************************************************************************
class VuHUDEliminatedNameEntity : public VuUITextBaseEntity
{
	DECLARE_RTTI

public:
	VuHUDEliminatedNameEntity();

private:
	// event handlers
	void				OnUITick(const VuParams &params);
	void				OnDriverWasEliminated(const VuParams &params);

	virtual const char	*getText() { return mText.c_str(); }
	virtual void		drawLayout(bool bSelected);
	virtual void		drawText(const char *text, float alpha);

	// properties
	float				mDuration;
	float				mFadeTime;

	float				mTimer;
	std::string			mText;
};

IMPLEMENT_RTTI(VuHUDEliminatedNameEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuHUDEliminatedNameEntity);

//*****************************************************************************
VuHUDEliminatedNameEntity::VuHUDEliminatedNameEntity():
	mDuration(3.0f),
	mFadeTime(0.5f),
	mTimer(0.0f)
{
	// properties
	addProperty(new VuFloatProperty("Duration", mDuration));
	addProperty(new VuFloatProperty("Fade Time", mFadeTime));

	// scripting
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnDriverEliminated);

	// event handlers
	REG_EVENT_HANDLER(VuHUDEliminatedNameEntity, OnUITick);
	REG_EVENT_HANDLER(VuHUDEliminatedNameEntity, OnDriverWasEliminated);
}

//*****************************************************************************
void VuHUDEliminatedNameEntity::OnUITick(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	float fdt = accessor.getFloat();

	mTimer -= fdt;
}

//*****************************************************************************
void VuHUDEliminatedNameEntity::OnDriverWasEliminated(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	VuEntity *pEntity = accessor.getEntity();

	if ( pEntity && pEntity->isDerivedFrom(VuCarEntity::msRTTI) )
	{
		VuCarEntity *pCar = static_cast<VuCarEntity *>(pEntity);

		mTimer = mDuration;

		mText = VuStringDB::IF()->getString("Driver_Eliminated");
		VuStringUtil::replace(mText, "[[DRIVER_NAME]]", pCar->getDriver()->getName());

		mpScriptComponent->getPlug("OnDriverEliminated")->execute();
	}
}

//*****************************************************************************
void VuHUDEliminatedNameEntity::drawLayout(bool bSelected)
{
	mText = VuStringDB::IF()->getString("Driver_Eliminated");
	mTimer = 0.5f*mDuration;

	VuUITextBaseEntity::drawLayout(bSelected);
}

//*****************************************************************************
void VuHUDEliminatedNameEntity::drawText(const char *text, float alpha)
{
	if ( mTimer > 0.0f )
	{
		float timeFromStart = mDuration - mTimer;
		if ( timeFromStart < mFadeTime )
			alpha *= timeFromStart/mFadeTime;
		if ( mTimer < mFadeTime )
			alpha *= mTimer/mFadeTime;

		VuUITextBaseEntity::drawText(text, alpha);
	}
}
