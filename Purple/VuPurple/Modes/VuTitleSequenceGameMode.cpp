//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  TitleSequenceGameMode class
// 
//*****************************************************************************

#include "VuTitleSequenceGameMode.h"
#include "VuPurple/Managers/VuFadeManager.h"
#include "VuPurple/Managers/VuProjectManager.h"
#include "VuPurple/Gfx/GfxComposer/VuPurpleGfxComposer.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Entities/UI/VuUIScreenEntity.h"
#include "VuEngine/Projects/VuProject.h"
#include "VuEngine/Assets/VuAssetFactory.h"
#include "VuEngine/Gfx/GfxSort/VuGfxSort.h"
#include "VuEngine/Managers/VuDialogManager.h"
#include "VuEngine/Managers/VuMessageBoxManager.h"


IMPLEMENT_RTTI(VuTitleSequenceGameMode, VuGameMode);


//*****************************************************************************
VuTitleSequenceGameMode::VuTitleSequenceGameMode():
	mpScreenProject(VUNULL),
	mTouched(false)
{
	// create FSM
	{
		VuFSM::VuState *pState;

		pState = mFSM.addState("LoadNextScreen");
		pState->setEnterMethod(this, &VuTitleSequenceGameMode::onLoadNextScreenEnter);

		pState = mFSM.addState("FadeIn");
		pState->setEnterMethod(this, &VuTitleSequenceGameMode::onFadeInEnter);

		pState = mFSM.addState("Wait");
		pState->setTickMethod(this, &VuTitleSequenceGameMode::onWaitTick);

		pState = mFSM.addState("FadeOut");
		pState->setEnterMethod(this, &VuTitleSequenceGameMode::onFadeOutEnter);

		pState = mFSM.addState("Exit");

		// add transitions
		mFSM.addTransition("LoadNextScreen", "FadeIn", "ScreenLoaded");
		mFSM.addTransition("LoadNextScreen", "Exit", "");

		mFSM.addTransition("FadeIn", "Wait", "FadeManagerInactive");

		mFSM.addTransition("Wait", "FadeOut", "Expired");
		mFSM.addTransition("Wait", "FadeOut", "Touched");

		mFSM.addTransition("FadeOut", "LoadNextScreen", "FadeManagerInactive");
	}

	// read constants
	{
		const VuJsonContainer &titleSequence = VuGameUtil::IF()->constantDB()["TitleSequence"];
		const VuJsonContainer &sku = titleSequence["SkuOverride"][VuAssetFactory::IF()->getSku()];

		const VuJsonContainer &screens = sku.hasMember("Screens") ? sku["Screens"] : titleSequence["Screens"];
		for ( int i = 0; i < screens.size(); i++ )
			mScreenQueue.push(screens[i].asString());

		mFadeTime = titleSequence["FadeTime"].asFloat();
		mWaitTime = titleSequence["WaitTime"].asFloat();
	}

	// register event handlers
	REG_EVENT_HANDLER(VuTitleSequenceGameMode, OnNextScreen);
}

//*****************************************************************************
VuTitleSequenceGameMode::~VuTitleSequenceGameMode()
{
}

//*****************************************************************************
bool VuTitleSequenceGameMode::enter(const std::string &prevMode)
{
	// start music
	VuGameUtil::IF()->startMusic(VuGameUtil::MUSIC_MENU);

	mFSM.begin();

	return true;
}

//*****************************************************************************
void VuTitleSequenceGameMode::exit()
{
	mFSM.end();
	unloadScreen();
}

//*****************************************************************************
const char *VuTitleSequenceGameMode::tick(float fdt)
{
	mFSM.setCondition("FadeManagerInactive", !VuFadeManager::IF()->isFading());
	mFSM.setCondition("ScreenLoaded", mpScreenProject != VUNULL);

	mFSM.evaluate();
	mFSM.tick(fdt);

	if ( mFSM.getCurState()->getName() == "Exit" )
		return "Preload";

	return "";
}

//*****************************************************************************
void VuTitleSequenceGameMode::draw()
{
	mFSM.draw();

	if ( VuUIScreenEntity *pScreen = getCurScreen() )
	{
		pScreen->draw();
	}
}

//*****************************************************************************
void VuTitleSequenceGameMode::OnNextScreen(const VuParams &params)
{
	if ( !mTouched )
	{
		mTouched = true;
	}
}

//*****************************************************************************
void VuTitleSequenceGameMode::loadScreen(const std::string &screen)
{
	if ( (mpScreenProject = VuProjectManager::IF()->load(screen)) )
		mpScreenProject->gameInitialize();
}

//*****************************************************************************
void VuTitleSequenceGameMode::unloadScreen()
{
	if ( mpScreenProject )
	{
		mpScreenProject->gameRelease();
		VuProjectManager::IF()->unload(mpScreenProject);
		mpScreenProject = VUNULL;
	}
}

//*****************************************************************************
VuUIScreenEntity *VuTitleSequenceGameMode::getCurScreen()
{
	if ( mpScreenProject && mpScreenProject->getRootEntity()->isDerivedFrom(VuUIScreenEntity::msRTTI) )
		return static_cast<VuUIScreenEntity *>(mpScreenProject->getRootEntity());

	return VUNULL;
}

//*****************************************************************************
void VuTitleSequenceGameMode::onLoadNextScreenEnter()
{
	unloadScreen();

	if ( mScreenQueue.size() )
	{
		loadScreen(mScreenQueue.front());
		mScreenQueue.pop();
	}
}

//*****************************************************************************
void VuTitleSequenceGameMode::onFadeInEnter()
{
	mTouched = false;

	VuFadeManager::IF()->startFadeIn(mFadeTime);
}

//*****************************************************************************
void VuTitleSequenceGameMode::onWaitTick(float fdt)
{
	if ( VuUIScreenEntity *pScreen = getCurScreen() )
		if ( !VuMessageBoxManager::IF()->getActiveMessageBox() && !VuDialogManager::IF()->getActiveDialog() )
			pScreen->tick(fdt);

	if ( mFSM.getTimeInState() >= mWaitTime )
		mFSM.pulseCondition("Expired");

	mFSM.setCondition("Touched", mTouched);
}

//*****************************************************************************
void VuTitleSequenceGameMode::onFadeOutEnter()
{
	VuFadeManager::IF()->startFadeOut(mFadeTime);
}
