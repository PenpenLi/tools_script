//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  FrontEndGameMode class
// 
//*****************************************************************************

#include "VuFrontEndGameMode.h"
#include "VuPurple/Managers/VuProjectManager.h"
#include "VuPurple/Managers/VuFadeManager.h"
#include "VuPurple/Managers/VuCloudSaveManager.h"
#include "VuPurple/Gfx/GfxComposer/VuPurpleGfxComposer.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Entities/UI/VuUIScreenEntity.h"
#include "VuEngine/Projects/VuProject.h"
#include "VuEngine/Managers/VuViewportManager.h"
#include "VuEngine/Managers/VuProfileManager.h"
#include "VuEngine/Managers/VuDialogManager.h"
#include "VuEngine/Managers/VuTireTrackManager.h"
#include "VuEngine/Cloud/VuCloudManager.h"
#include "VuEngine/UI/VuUIUtil.h"
#include "VuEngine/Assets/VuAssetFactory.h"
#include "VuEngine/Gfx/GfxSort/VuGfxSort.h"
#include "VuEngine/Pfx/VuPfx.h"
#include "VuEngine/Math/VuRand.h"
#include "VuEngine/Json/VuJsonContainer.h"
#include "VuEngine/Json/VuJsonReader.h"
#include "VuEngine/Memory/VuScratchPad.h"
#include "VuEngine/Util/VuZLibUtil.h"
#include "VuEngine/Dev/VuDevConfig.h"


IMPLEMENT_RTTI(VuFrontEndGameMode, VuGameMode);


#define BACKGROUND_PROJECT "Screens/Background"


//*****************************************************************************
VuFrontEndGameMode::VuFrontEndGameMode():
	mpBackgroundProject(VUNULL),
	mpScreenProject(VUNULL),
	mpNewsScreenProject(VUNULL)
{
	// register event handlers
	REG_EVENT_HANDLER(VuFrontEndGameMode, OnStartActionGame);
	REG_EVENT_HANDLER(VuFrontEndGameMode, OnStartSplitScreenGame);
	REG_EVENT_HANDLER(VuFrontEndGameMode, OnSetScreen);
	REG_EVENT_HANDLER(VuFrontEndGameMode, OnPushScreen);
	REG_EVENT_HANDLER(VuFrontEndGameMode, OnPopScreen);
	REG_EVENT_HANDLER(VuFrontEndGameMode, OnSetScreenStack);
	REG_EVENT_HANDLER(VuFrontEndGameMode, OnPushNewsScreen);
	REG_EVENT_HANDLER(VuFrontEndGameMode, OnShutdownApp);

	// add keyboard callback
	VuKeyboard::IF()->addCallback(this);

	// create FSM
	{
		VuFSM::VuState *pState = mFSM.addState("Begin");
		pState->setEnterMethod(this, &VuFrontEndGameMode::onBeginEnter);

		pState = mFSM.addState("End");
		pState->setEnterMethod(this, &VuFrontEndGameMode::onEndEnter);
		pState->setTickMethod(this, &VuFrontEndGameMode::onEndTick);

		pState = mFSM.addState("Run");
		pState->setTickMethod(this, &VuFrontEndGameMode::onRunTick);

		pState = mFSM.addState("Exit");

		pState = mFSM.addState("FadeOut");
		pState->setEnterMethod(this, &VuFrontEndGameMode::onFadeOutEnter);
		pState->setTickMethod(this, &VuFrontEndGameMode::onFadeOutTick);

		pState = mFSM.addState("FadeTransition");
		pState->setEnterMethod(this, &VuFrontEndGameMode::loadNextScreen);

		pState = mFSM.addState("FadeIn");
		pState->setEnterMethod(this, &VuFrontEndGameMode::onFadeInEnter);
		pState->setTickMethod(this, &VuFrontEndGameMode::onFadeInTick);

		// add transitions
		mFSM.addTransition("Begin", "FadeIn", "");

		mFSM.addTransition("Run", "End", "NextGameModeSet");
		mFSM.addTransition("Run", "FadeOut", "NextScreenSet");
		mFSM.addTransition("Run", "FadeOut", "NewsScreenSet");

		mFSM.addTransition("FadeOut", "FadeTransition", "FadeOutComplete");

		mFSM.addTransition("FadeTransition", "FadeIn", "");

		mFSM.addTransition("FadeIn", "Run", "FadeInComplete");

		mFSM.addTransition("End", "Exit", "FadeManagerInactive");
	}
}

//*****************************************************************************
VuFrontEndGameMode::~VuFrontEndGameMode()
{
	// remove keyboard callback
	VuKeyboard::IF()->removeCallback(this);
}

//*****************************************************************************
bool VuFrontEndGameMode::enter(const std::string &prevMode)
{	VuGameUtil::IF()->dataWrite().removeMember("GameData");

	VuViewportManager::IF()->reset();
	VuViewportManager::IF()->setViewportCount(1);

	VuGameUtil::IF()->setFrontEndCameraTransitionTime(0.0f);

	// load background
	loadBackgroundProject();

	// load initial screen
	VuGameUtil::IF()->screenStackClear();

	if ( VuDevConfig::IF()->hasParam("InitialScreen" ) )
	{
		VuGameUtil::IF()->screenStackPush(VuDevConfig::IF()->getParam("InitialScreen").asString());
	}
	else
	{
		std::string screenStackName = "Title";
		VuGameUtil::IF()->dataRead()["InitialScreenStack"].getValue(screenStackName);
		const VuJsonContainer &screenStack = VuGameUtil::IF()->constantDB()["ScreenStacks"][screenStackName];
		for ( int i = 0; i < screenStack.size(); i++ )
			VuGameUtil::IF()->screenStackPush(screenStack[i].asString());
	}

	VuGameUtil::IF()->dataWrite()["InitialScreenStack"].clear();

	loadScreen(VuGameUtil::IF()->screenStackTop());
	VuGameUtil::IF()->screenStackPop();

	// start music
	VuGameUtil::IF()->startMusic(VuGameUtil::MUSIC_MENU);

	mFSM.begin();

	VuProfileManager::IF()->save();
	VuCloudSaveManager::IF()->save();

	VuGameUtil::IF()->setShadowSplitDistances("FrontEnd");

	// make sure generic pfx is preloaded
	VuAssetFactory::IF()->preloadAsset("VuPfxAsset", "Generic");

	return true;
}

//*****************************************************************************
void VuFrontEndGameMode::exit()
{
	mFSM.end();

	// stop music
	VuGameUtil::IF()->stopMusic();

	// unload screen
	unloadScreen();
	unloadNewsScreen();

	// unload background
	unloadBackgroundProject();

	VuViewportManager::IF()->reset();
	VuTireTrackManager::IF()->reset();

	VuGameUtil::IF()->setShadowSplitDistances("Default");

	// clear results
	VuGameUtil::IF()->dataWrite().removeMember("Results");

	if ( mNextGameMode == "Action" )
	{
		VuJsonContainer &gameData = VuGameUtil::IF()->dataWrite()["GameData"];

		if ( mNextEventType == "AttractEvent" )
			VuGameUtil::configureAttractEvent(gameData);
		else if ( mNextEventType == "DemoEvent" )
			VuGameUtil::configureDemoEvent(gameData);
		else if ( mNextEventType == "SinglePlayerEvent" )
			VuGameUtil::configureSinglePlayerEvent(gameData);
		else if ( mNextEventType == "ChallengeEvent" )
			VuGameUtil::configureChallengeEvent(gameData);
		else if ( mNextEventType == "TutorialDriveEvent" )
			VuGameUtil::configureTutorialDriveEvent(gameData);
		else if ( mNextEventType == "TutorialPowerUpEvent" )
			VuGameUtil::configureTutorialPowerUpEvent(gameData);
		else if ( mNextEventType == "TutorialRaceEvent" )
			VuGameUtil::configureTutorialRaceEvent(gameData);
		else if ( mNextEventType == "DuelEvent" )
			VuGameUtil::configureDuelEvent(gameData);
		else if (mNextEventType == "CarChampEvent")
			VuGameUtil::configureCarChampEvent(gameData);

		gameData["EventType"].putValue(mNextEventType);
	}
	else if ( mNextGameMode == "SplitScreen" )
	{
		VuJsonContainer &gameData = VuGameUtil::IF()->dataWrite()["GameData"];
		gameData.clear();

		VuGameUtil::configureSplitScreenEvent(gameData);
	}

	VuPfx::IF()->checkForLeaks();
}

//*****************************************************************************
const char *VuFrontEndGameMode::tick(float fdt)
{
	mFSM.setCondition("FadeManagerInactive", !VuFadeManager::IF()->isFading());
	mFSM.setCondition("NextGameModeSet", !mNextGameMode.empty());
	mFSM.setCondition("NextScreenSet", !mNextScreen.empty());
	mFSM.setCondition("NewsScreenSet", mpNewsScreenProject != VUNULL);

	mFSM.evaluate();
	mFSM.tick(fdt);

	if ( mFSM.getCurState()->getName() == "Exit" )
		return mNextGameMode.c_str();

	return "";
}

//*****************************************************************************
void VuFrontEndGameMode::draw()
{
	mFSM.draw();

	if ( VuUIScreenEntity *pScreen = getCurScreen() )
		pScreen->draw();
}

//*****************************************************************************
void VuFrontEndGameMode::OnStartActionGame(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	mNextEventType = accessor.getString();

	mNextGameMode = "Action";
}

//*****************************************************************************
void VuFrontEndGameMode::OnStartSplitScreenGame(const VuParams &params)
{
	mNextGameMode = "SplitScreen";
}

//*****************************************************************************
void VuFrontEndGameMode::OnShutdownApp(const VuParams &params)
{
	mNextGameMode = "Shutdown";
}

//*****************************************************************************
void VuFrontEndGameMode::OnSetScreen(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);

	mNextScreen = accessor.getString();
}

//*****************************************************************************
void VuFrontEndGameMode::OnPushScreen(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);

	mNextScreen = accessor.getString();
	VuGameUtil::IF()->screenStackPush(mCurScreen);
}

//*****************************************************************************
void VuFrontEndGameMode::OnPopScreen(const VuParams &params)
{
	if ( VuGameUtil::IF()->screenStackSize() )
	{
		mNextScreen = VuGameUtil::IF()->screenStackTop();
		VuGameUtil::IF()->screenStackPop();
	}
}

//*****************************************************************************
void VuFrontEndGameMode::OnSetScreenStack(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	const char *screenStackName = accessor.getString();

	VuGameUtil::IF()->screenStackClear();

	const VuJsonContainer &screenStack = VuGameUtil::IF()->constantDB()["ScreenStacks"][screenStackName];
	for ( int i = 0; i < screenStack.size(); i++ )
		VuGameUtil::IF()->screenStackPush(screenStack[i].asString());

	mNextScreen = VuGameUtil::IF()->screenStackTop();
	VuGameUtil::IF()->screenStackPop();
}

//*****************************************************************************
void VuFrontEndGameMode::OnPushNewsScreen(const VuParams &params)
{
	if ( mpNewsScreenProject == VUNULL )
	{
		VuArray<VUBYTE> newsData;
		VuCloudManager::IF()->getNewsData(newsData);
		if ( newsData.size() )
		{
			VUUINT32 size = VuScratchPad::SIZE - 1;
			if ( VuZLibUtil::gzipUncompressFromMemory(VuScratchPad::get(), &size, &newsData[0], newsData.size()) )
			{
				char *strDoc = static_cast<char*>(VuScratchPad::get());
				strDoc[size] = '\0';

				// check for and skip UTF-8 header
				if ( (VUUINT8)strDoc[0] == 0xef && (VUUINT8)strDoc[1] == 0xbb && (VUUINT8)strDoc[2] == 0xbf )
					strDoc += 3;

				VuJsonContainer doc;
				VuJsonReader reader;
				if ( reader.loadFromString(doc, strDoc) )
				{
					mpNewsScreenProject = new VuProject;
					VuGfxSort::IF()->flush();
					if ( mpNewsScreenProject->load(doc, "News") )
					{
						VuGameUtil::IF()->screenStackPush(mCurScreen);
					}
					else
					{
						mpNewsScreenProject->removeRef();
						mpNewsScreenProject = VUNULL;
					}
				}
			}
		}
	}
}

//*****************************************************************************
void VuFrontEndGameMode::onKeyDown(VUUINT32 key)
{
	if ( key == VUKEY_SPACE )
	{
		unloadBackgroundProject();
		loadBackgroundProject();
	}
}

//*****************************************************************************
void VuFrontEndGameMode::loadBackgroundProject()
{
	if ( (mpBackgroundProject = VuProjectManager::IF()->load(BACKGROUND_PROJECT)) )
		mpBackgroundProject->gameInitialize();
}

//*****************************************************************************
void VuFrontEndGameMode::unloadBackgroundProject()
{
	if ( mpBackgroundProject )
	{
		mpBackgroundProject->gameRelease();
		VuProjectManager::IF()->unload(mpBackgroundProject);
		mpBackgroundProject = VUNULL;
	}
}

//*****************************************************************************
void VuFrontEndGameMode::loadNextScreen()
{
	unloadScreen();
	if ( mpNewsScreenProject )
	{
		mpScreenProject = mpNewsScreenProject;
		mpNewsScreenProject = VUNULL;
		mpScreenProject->gameInitialize();
	}
	else
	{
		loadScreen(mNextScreen);
		mNextScreen = "";
	}
}

//*****************************************************************************
void VuFrontEndGameMode::loadScreen(const std::string &screen)
{
	mCurScreen = screen;

	if ( (mpScreenProject = VuProjectManager::IF()->load(screen)) )
		mpScreenProject->gameInitialize();
}

//*****************************************************************************
void VuFrontEndGameMode::unloadScreen()
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
void VuFrontEndGameMode::unloadNewsScreen()
{
	if ( mpNewsScreenProject )
	{
		VuGfxSort::IF()->flush();
		mpNewsScreenProject->removeRef();
		mpNewsScreenProject = VUNULL;
	}
}

//*****************************************************************************
VuUIScreenEntity *VuFrontEndGameMode::getCurScreen()
{
	if ( mpScreenProject && mpScreenProject->getRootEntity()->isDerivedFrom(VuUIScreenEntity::msRTTI) )
		return static_cast<VuUIScreenEntity *>(mpScreenProject->getRootEntity());

	return VUNULL;
}

//*****************************************************************************
void VuFrontEndGameMode::onBeginEnter()
{
	VuFadeManager::IF()->startFadeIn();
}

//*****************************************************************************
void VuFrontEndGameMode::onEndEnter()
{
	VuFadeManager::IF()->startFadeOut();
}

//*****************************************************************************
void VuFrontEndGameMode::onEndTick(float fdt)
{
	VuGameUtil::IF()->setMusicVolume(1.0f - VuFadeManager::IF()->getFadeAmount());
}

//*****************************************************************************
void VuFrontEndGameMode::onRunTick(float fdt)
{
	if ( VuUIScreenEntity *pScreen = getCurScreen() )
		if ( !VuMessageBoxManager::IF()->getActiveMessageBox() && !VuDialogManager::IF()->getActiveDialog() )
			pScreen->tick(fdt);
}

//*****************************************************************************
void VuFrontEndGameMode::onFadeOutEnter()
{
	VuUIUtil::startTransitionOut(getCurScreen());
}

//*****************************************************************************
void VuFrontEndGameMode::onFadeOutTick(float fdt)
{
	if ( VuUIUtil::tickTransition(getCurScreen(), fdt) )
		mFSM.pulseCondition("FadeOutComplete");
}

//*****************************************************************************
void VuFrontEndGameMode::onFadeInEnter()
{
	VuUIUtil::startTransitionIn(getCurScreen());
}

//*****************************************************************************
void VuFrontEndGameMode::onFadeInTick(float fdt)
{
	if ( VuUIUtil::tickTransition(getCurScreen(), fdt) )
		mFSM.pulseCondition("FadeInComplete");
}
