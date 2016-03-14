//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Pause menu class
// 
//*****************************************************************************

#include "VuPauseMenu.h"
#include "VuPurple/Managers/VuProjectManager.h"
#include "VuPurple/Managers/VuControlMethodManager.h"
#include "VuPurple/Managers/VuVideoShareManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/UI/VuUI.h"
#include "VuEngine/UI/VuUIUtil.h"
#include "VuEngine/Entities/UI/VuUIScreenEntity.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Managers/VuInputManager.h"
#include "VuEngine/Managers/VuDialogManager.h"
#include "VuEngine/Managers/VuMessageBoxManager.h"
#include "VuEngine/Projects/VuProject.h"
#include "VuEngine/Assets/VuAssetFactory.h"
#include "VuEngine/Assets/VuTextureAsset.h"
#include "VuEngine/Assets/VuProjectAsset.h"
#include "VuEngine/HAL/Gfx/VuGfx.h"
#include "VuEngine/HAL/Audio/VuAudio.h"
#include "VuEngine/HAL/Touch/VuTouch.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/Font/VuFontDraw.h"
#include "VuEngine/Gfx/GfxSort/VuGfxSort.h"
#include "VuEngine/DB/VuStringDB.h"
#include "VuEngine/Util/VuDataUtil.h"


// constants
#define MIN_FADE_TIME 0.3f // seconds


//*****************************************************************************
VuPauseMenu::VuPauseMenu(bool netGame):
	mNetGame(netGame),
	mGamePaused(false),
	mFadeEnabled(true),
	mBackgroundAlphaValue(0.0f),
	mBackgroundColor(0,0,0,255),
	mDefaultScreen("Screens/Pause"),
	mpScreenProject(VUNULL),
	mPadMask(0xff)
{
	// register event handlers
	REG_EVENT_HANDLER(VuPauseMenu, OnSetScreen);
	REG_EVENT_HANDLER(VuPauseMenu, OnPushScreen);
	REG_EVENT_HANDLER(VuPauseMenu, OnPopScreen);
	REG_EVENT_HANDLER(VuPauseMenu, OnReloadScreen);
	REG_EVENT_HANDLER(VuPauseMenu, SetDefaultPauseMenu);

	// read constants
	VuDataUtil::getValue(VuGameUtil::IF()->constantDB()["UI"]["PauseMenuBackgroundColor"], mBackgroundColor);

	// create FSM
	VuFSM::VuState *pState = mFSM.addState("Unpaused");
	pState->setEnterMethod(this, &VuPauseMenu::onUnpausedEnter);
	pState->setExitMethod(this, &VuPauseMenu::onUnpausedExit);

	pState = mFSM.addState("Paused");
	pState->setTickMethod(this, &VuPauseMenu::onPausedTick);

	pState = mFSM.addState("Transition");
	pState->setEnterMethod(this, &VuPauseMenu::onTransition);

	pState = mFSM.addState("FadeIn");
	pState->setEnterMethod(this, &VuPauseMenu::onFadeInEnter);
	pState->setTickMethod(this, &VuPauseMenu::onFadeInTick);

	pState = mFSM.addState("FadeOut");
	pState->setEnterMethod(this, &VuPauseMenu::onFadeOutEnter);
	pState->setTickMethod(this, &VuPauseMenu::onFadeOutTick);

	// add transitions
	mFSM.addTransition("Unpaused", "Transition", "NextScreenSet");

	mFSM.addTransition("Paused", "FadeOut", "NextScreenSet");
	mFSM.addTransition("Paused", "FadeOut", "Unpause");

	mFSM.addTransition("Transition", "FadeIn", "ScreenLoaded");
	mFSM.addTransition("Transition", "Unpaused", "");

	mFSM.addTransition("FadeOut", "Transition", "FadeOutComplete");

	mFSM.addTransition("FadeIn", "Paused", "FadeInComplete");

	// begin FSM
	mFSM.begin();
}

//*****************************************************************************
VuPauseMenu::~VuPauseMenu()
{
	// end FSM
	mFSM.end();

	// end unpaused
	setGamePaused(false);
	unloadScreen();
}

//*****************************************************************************
void VuPauseMenu::pause(const char *initialScreen, bool fade, VUUINT32 padMask)
{
	if ( !isPaused() && VuGameUtil::IF()->isPauseMenuAllowed() )
	{
		if ( !VuAssetFactory::IF()->doesAssetExist<VuProjectAsset>(initialScreen) )
			initialScreen = mDefaultScreen.c_str();

		VuGameUtil::IF()->screenStackClear();
		mNextScreen = initialScreen;
		mFadeEnabled = fade;
		mPadMask = padMask;
	}
}

//*****************************************************************************
void VuPauseMenu::unpause()
{
	mFSM.setCondition("Unpause", true);
}

//*****************************************************************************
bool VuPauseMenu::isPaused()
{
	return mGamePaused;
}

//*****************************************************************************
void VuPauseMenu::tick(float fdt)
{
	// use real dt
	fdt = VuTickManager::IF()->getRealDeltaTime();

	// set conditions
	mFSM.setCondition("NextScreenSet", !mNextScreen.empty());
	mFSM.setCondition("ScreenLoaded", mpScreenProject != VUNULL);

	mFSM.evaluate();
	mFSM.tick(fdt);
}

//*****************************************************************************
void VuPauseMenu::draw()
{
	VuGfxSort::IF()->setFullScreenLayer(VuGfxSort::FSL_UI);
	VuGfxSort::IF()->setViewport(0);
	VuGfxSort::IF()->setReflectionLayer(0);
	VuGfxSort::IF()->setViewportLayer(VuGfxSort::VPL_UI);

	if ( isPaused() )
	{
		drawBackground();

		if ( VuUIScreenEntity *pScreen = getCurScreen() )
			pScreen->draw();
	}
}

//*****************************************************************************
void VuPauseMenu::OnSetScreen(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);

	mNextScreen = accessor.getString();
}

//*****************************************************************************
void VuPauseMenu::OnPushScreen(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);

	mNextScreen = accessor.getString();
	VuGameUtil::IF()->screenStackPush(mCurScreen);
}

//*****************************************************************************
void VuPauseMenu::OnPopScreen(const VuParams &params)
{
	if ( VuGameUtil::IF()->screenStackSize() )
	{
		mNextScreen = VuGameUtil::IF()->screenStackTop();
		VuGameUtil::IF()->screenStackPop();
	}
}

//*****************************************************************************
void VuPauseMenu::OnReloadScreen(const VuParams &params)
{
	mNextScreen = mCurScreen;
}

//*****************************************************************************
void VuPauseMenu::SetDefaultPauseMenu(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	mDefaultScreen = accessor.getString();
}

//*****************************************************************************
void VuPauseMenu::setGamePaused(bool paused)
{
	if ( paused != mGamePaused )
	{
		if ( !mNetGame )
		{
			if ( paused )
			{
				VuTickManager::IF()->pushPauseRequest();
				VuGameUtil::IF()->setPauseMenuShown(mFadeEnabled);
				VuVideoShareManager::IF()->pause();
			}
			else
			{
				VuTickManager::IF()->popPauseRequest();
				VuGameUtil::IF()->setPauseMenuShown(false);
				VuVideoShareManager::IF()->resume();
			}
		}

		if ( paused )
			VuAudio::IF()->pushCategoryPause("game");
		else
			VuAudio::IF()->popCategoryPause("game");

		mGamePaused = paused;
	}
}

//*****************************************************************************
void VuPauseMenu::loadScreen(const std::string &screen)
{
	mCurScreen = screen;

	if ( (mpScreenProject = VuProjectManager::IF()->load(screen)) )
		mpScreenProject->gameInitialize();
}

//*****************************************************************************
void VuPauseMenu::unloadScreen()
{
	VuGameUtil::IF()->setLastScreen(mCurScreen);

	mCurScreen = "";

	if ( mpScreenProject )
	{
		mpScreenProject->gameRelease();
		VuProjectManager::IF()->unload(mpScreenProject);
		mpScreenProject = VUNULL;
	}
}

//*****************************************************************************
VuUIScreenEntity *VuPauseMenu::getCurScreen()
{
	if ( mpScreenProject && mpScreenProject->getRootEntity()->isDerivedFrom(VuUIScreenEntity::msRTTI) )
		return static_cast<VuUIScreenEntity *>(mpScreenProject->getRootEntity());

	return VUNULL;
}

//*****************************************************************************
void VuPauseMenu::drawBackground()
{
	VuColor color = mBackgroundColor;
	color.mA = VuRound(color.mA*mBackgroundAlphaValue);
	if ( color.mA )
		VuGfxUtil::IF()->drawFilledRectangle2d(1.0f, color);
}

//*****************************************************************************
void VuPauseMenu::onUnpausedEnter()
{
	setGamePaused(false);
}

//*****************************************************************************
void VuPauseMenu::onUnpausedExit()
{
	setGamePaused(true);
	mFSM.setCondition("Unpause", false);
}

//*****************************************************************************
void VuPauseMenu::onPausedTick(float fdt)
{
	if ( VuUIScreenEntity *pScreen = getCurScreen() )
	{
		if ( !VuMessageBoxManager::IF()->getActiveMessageBox() && !VuDialogManager::IF()->getActiveDialog() )
		{
			pScreen->tick(fdt, mPadMask);
		}
	}
}

//*****************************************************************************
void VuPauseMenu::onTransition()
{
	unloadScreen();
	if ( !mNextScreen.empty() )
		loadScreen(mNextScreen);
	mNextScreen = "";
}

//*****************************************************************************
void VuPauseMenu::onFadeInEnter()
{
	VuUIUtil::startTransitionIn(getCurScreen());
}

//*****************************************************************************
void VuPauseMenu::onFadeInTick(float fdt)
{
	bool transitionDone = VuUIUtil::tickTransition(getCurScreen(), fdt);

	mBackgroundAlphaValue = VuClamp(mFSM.getTimeInState()/MIN_FADE_TIME, mBackgroundAlphaValue, 1.0f);

	if ( transitionDone && mBackgroundAlphaValue >= 1.0f )
		mFSM.pulseCondition("FadeInComplete");

	VuGameUtil::IF()->setPauseMenuFadeAmount(mBackgroundAlphaValue);
}

//*****************************************************************************
void VuPauseMenu::onFadeOutEnter()
{
	VuUIUtil::startTransitionOut(getCurScreen());
}

//*****************************************************************************
void VuPauseMenu::onFadeOutTick(float fdt)
{
	bool transitionDone = VuUIUtil::tickTransition(getCurScreen(), fdt);

	if ( mNextScreen.empty() )
	{
		mBackgroundAlphaValue = VuMax(1.0f - mFSM.getTimeInState()/MIN_FADE_TIME, 0.0f);
		if ( transitionDone && mBackgroundAlphaValue <= 0.0f )
			mFSM.pulseCondition("FadeOutComplete");
	}
	else
	{
		if ( transitionDone )
			mFSM.pulseCondition("FadeOutComplete");
	}

	VuGameUtil::IF()->setPauseMenuFadeAmount(mBackgroundAlphaValue);
}
