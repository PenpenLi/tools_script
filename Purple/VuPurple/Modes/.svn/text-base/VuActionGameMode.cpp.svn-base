//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  ActionGameMode class
// 
//*****************************************************************************

#include "VuActionGameMode.h"
#include "VuPauseMenu.h"
#include "VuPurple/Managers/VuProjectManager.h"
#include "VuPurple/Managers/VuFadeManager.h"
#include "VuPurple/Managers/VuAnalyticsManager.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Managers/VuCloudSaveManager.h"
#include "VuPurple/Managers/VuControlMethodManager.h"
#include "VuPurple/Gfx/GfxComposer/VuPurpleGfxComposer.h"
#include "VuPurple/Games/VuRaceGame.h"
#include "VuPurple/Games/VuBoostBlitzGame.h"
#include "VuPurple/Games/VuEliminationGame.h"
#include "VuPurple/Games/VuBossBattleGame.h"
#include "VuPurple/Games/VuShootingGalleryGame.h"
#include "VuPurple/Games/VuDemolitionDerbyGame.h"
#include "VuPurple/Games/VuFollowLeaderGame.h"
#include "VuPurple/Games/VuTutorialDriveGame.h"
#include "VuPurple/Games/VuTutorialPowerUpGame.h"
#include "VuPurple/Games/VuTutorialRaceGame.h"
#include "VuPurple/Games/VuDuelGame.h"
#include "VuPurple/Games/VuCarChampRaceGame.h"
#include "VuPurple/Track/VuTrackManager.h"
#include "VuPurple/Track/VuTrackUtil.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuPurple/Util/VuAchievementUtil.h"
#include "VuEngine/Entities/UI/VuUIScreenEntity.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Projects/VuProject.h"
#include "VuEngine/Managers/VuViewportManager.h"
#include "VuEngine/Managers/VuProfileManager.h"
#include "VuEngine/Managers/VuLensWaterManager.h"
#include "VuEngine/Assets/VuAssetFactory.h"
#include "VuEngine/Assets/VuProjectAsset.h"
#include "VuEngine/HAL/Audio/VuAudio.h"
#include "VuEngine/Gfx/GfxSort/VuGfxSort.h"
#include "VuEngine/Pfx/VuPfx.h"
#include "VuEngine/Dev/VuDevConfig.h"


IMPLEMENT_RTTI(VuActionGameMode, VuGameMode);


#define LOADING_SCREEN "Screens/Loading"
#define MIN_LOADING_TIME_BETWEEN_FRAMES 0.1 // seconds


//*****************************************************************************
VuActionGameMode::VuActionGameMode():
	mpLoadingScreenProject(VUNULL),
	mpGameProjectAsset(VUNULL),
	mpGameProject(VUNULL),
	mpGame(VUNULL),
	mLastDrawTime(0.0),
	mpPauseMenu(VUNULL)
{
	// register event handlers
	REG_EVENT_HANDLER(VuActionGameMode, OnEndActionGame);
	REG_EVENT_HANDLER(VuActionGameMode, OnRestartActionGame);
	REG_EVENT_HANDLER(VuActionGameMode, OnStartActionGame);
	REG_EVENT_HANDLER(VuActionGameMode, OnPauseActionGame);
	REG_EVENT_HANDLER(VuActionGameMode, OnResumeActionGame);
	REG_EVENT_HANDLER(VuActionGameMode, OnSkipLoadingScreen);

	// add keyboard callback
	VuKeyboard::IF()->addCallback(this);

	// create FSM
	{
		VuFSM::VuState *pState;

		pState = mFSM.addState("LoadLevel");
		pState->setEnterMethod(this, &VuActionGameMode::onLoadLevelEnter);
		pState->setExitMethod(this, &VuActionGameMode::onLoadLevelExit);
		pState->setTickMethod(this, &VuActionGameMode::onLoadLevelTick);
		pState->setDrawMethod(this, &VuActionGameMode::onLoadLevelDraw);

		pState = mFSM.addState("Begin");
		pState->setEnterMethod(this, &VuActionGameMode::onBeginEnter);
		pState->setTickMethod(this, &VuActionGameMode::onBeginTick);
		pState->setDrawMethod(this, &VuActionGameMode::onGameDraw);

		pState = mFSM.addState("End");
		pState->setEnterMethod(this, &VuActionGameMode::onEndEnter);
		pState->setTickMethod(this, &VuActionGameMode::onEndTick);
		pState->setDrawMethod(this, &VuActionGameMode::onGameDraw);

		pState = mFSM.addState("Run");
		pState->setEnterMethod(this, &VuActionGameMode::onRunEnter);
		pState->setTickMethod(this, &VuActionGameMode::onRunTick);
		pState->setDrawMethod(this, &VuActionGameMode::onGameDraw);

		pState = mFSM.addState("Exit");

		// add transitions
		mFSM.addTransition("LoadLevel", "Begin", "LoadLevelFinished");

		mFSM.addTransition("Begin", "Run", "FadeManagerInactive");

		mFSM.addTransition("Run", "Exit", "Restart");
		mFSM.addTransition("Run", "End", "NextGameModeSet");

		mFSM.addTransition("End", "Exit", "FadeManagerInactive");
	}
}

//*****************************************************************************
VuActionGameMode::~VuActionGameMode()
{
	// remove keyboard callback
	VuKeyboard::IF()->removeCallback(this);
}

//*****************************************************************************
bool VuActionGameMode::enter(const std::string &prevMode)
{
	VuViewportManager::IF()->reset();
	VuViewportManager::IF()->setViewportCount(1);

	mpPauseMenu = new VuPauseMenu(false);

	mFSM.begin();

	VuGameUtil::IF()->dataWrite().removeMember("DevRestart");

	// make sure generic pfx is preloaded
	VuAssetFactory::IF()->preloadAsset("VuPfxAsset", "Generic");

	return true;
}

//*****************************************************************************
void VuActionGameMode::exit()
{
	mFSM.end();

	delete mpPauseMenu;
	mpPauseMenu = VUNULL;

	// stop music
	VuGameUtil::IF()->stopMusic();

	if ( mNextGameMode == "Action" && !VuGameUtil::IF()->dataRead()["DevRestart"].asBool() )
	{
		for ( const auto &iter : VuAssetFactory::IF()->getRepository() )
			VuAssetFactory::IF()->cacheAsset(iter.second);
	}

	// destroy game
	if ( mpGame )
	{
		mpGame->end();
		delete mpGame;
		mpGame = VUNULL;
	}

	// unload project
	if ( mpGameProject )
	{
		mpGameProject->gameRelease();
		VuProjectManager::IF()->unload(mpGameProject);
		mpGameProject = VUNULL;
	}

	VuViewportManager::IF()->reset();

	// if going back into action mode, reload DBs
	if ( VuGameUtil::IF()->dataRead()["DevRestart"].asBool() )
		VuGameUtil::IF()->reloadDBs();

	// recalculate achievements
	VuAchievementUtil::recalculateAchievements();

	VuPfx::IF()->checkForLeaks();
}

//*****************************************************************************
const char *VuActionGameMode::tick(float fdt)
{
	mFSM.setCondition("FadeManagerInactive", !VuFadeManager::IF()->isFading());
	mFSM.setCondition("NextGameModeSet", !mNextGameMode.empty());

	mFSM.evaluate();
	mFSM.tick(fdt);

	if ( mFSM.getCurState()->getName() == "Exit" )
		return mNextGameMode.c_str();

	return "";
}

//*****************************************************************************
void VuActionGameMode::draw()
{
	mFSM.draw();
}

//*****************************************************************************
void VuActionGameMode::OnEndActionGame(const VuParams &params)
{
	mNextGameMode = "FrontEnd";
}

//*****************************************************************************
void VuActionGameMode::OnRestartActionGame(const VuParams &params)
{
	mNextGameMode = "Action";
}

//*****************************************************************************
void VuActionGameMode::OnStartActionGame(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	std::string nextEventType = accessor.getString();

	VuJsonContainer &gameData = VuGameUtil::IF()->dataWrite()["GameData"];

	if ( nextEventType == "TutorialDriveEvent" )
		VuGameUtil::configureTutorialDriveEvent(gameData);
	else if ( nextEventType == "TutorialPowerUpEvent" )
		VuGameUtil::configureTutorialPowerUpEvent(gameData);
	else if ( nextEventType == "TutorialRaceEvent" )
		VuGameUtil::configureTutorialRaceEvent(gameData);
	else if ( nextEventType == "CarChampEvent" )
		VuGameUtil::configureCarChampEvent(gameData);

	gameData["EventType"].putValue(nextEventType);

	mNextGameMode = "Action";
}

//*****************************************************************************
void VuActionGameMode::OnPauseActionGame(const VuParams &params)
{
	const char *screen = "";
	bool fade = true;

	VuParams::VuAccessor accessor(params);
	if ( accessor.getNextType() == VuParams::String )
		screen = accessor.getString();
	if ( accessor.getNextType() == VuParams::Bool )
		fade = accessor.getBool();

	mpPauseMenu->pause(screen, fade, 0xff);
}

//*****************************************************************************
void VuActionGameMode::OnResumeActionGame(const VuParams &params)
{
	mpPauseMenu->unpause();
}

//*****************************************************************************
void VuActionGameMode::OnSkipLoadingScreen(const VuParams &params)
{
	mFSM.setCondition("LoadLevelFinished", true);
}

//*****************************************************************************
void VuActionGameMode::onKeyDown(VUUINT32 key)
{
	if ( key == VUKEY_SPACE )
	{
		mNextGameMode = "Action";
		mFSM.pulseCondition("Restart");
		VuGameUtil::IF()->dataWrite()["DevRestart"].putValue(true);
	}
}

//*****************************************************************************
void VuActionGameMode::onGameDraw()
{
	if ( mpGame )
	{
		mpGame->draw();
	}

	mpPauseMenu->draw();
}

//*****************************************************************************
void VuActionGameMode::onLoadLevelEnter()
{
	mpLoadingScreenProject = VuProjectManager::IF()->load(LOADING_SCREEN);
	if ( mpLoadingScreenProject )
		mpLoadingScreenProject->gameInitialize();

	VuFadeManager::IF()->startFadeIn(0.0f);

	if ( !VuGameUtil::IF()->dataRead().hasMember("GameData") )
		VuGameUtil::IF()->dataWrite()["GameData"] = VuDevConfig::IF()->getParam("ActionGameData");
	const VuJsonContainer &gameData = VuGameUtil::IF()->dataRead()["GameData"];

	// load project asset
	mpGameProjectAsset = VuAssetFactory::IF()->createAsset<VuProjectAsset>(gameData["ProjectAsset"].asString());

	// build preload queue
	int assetCount = mpGameProjectAsset->getAssetCount();
	std::string assetType, assetName;
	for ( int i = 0; i < assetCount; i++ )
	{
		mpGameProjectAsset->getAssetInfo(i, assetType, assetName);
		if ( !VuAssetFactory::IF()->findAsset(assetType.c_str(), assetName.c_str()) )
			mPreloadQueue.push_back(PreloadEntry(assetType, assetName));
	}

	mPreloadIndex = 0;
}

//*****************************************************************************
void VuActionGameMode::onLoadLevelExit()
{
	if ( mpLoadingScreenProject )
	{
		mpLoadingScreenProject->gameRelease();
		VuProjectManager::IF()->unload(mpLoadingScreenProject);
		mpLoadingScreenProject = VUNULL;
	}

	VuFadeManager::IF()->startFadeOut(0.0f);

	if ( !VuGameUtil::IF()->dataRead().hasMember("GameData") )
		VuGameUtil::IF()->dataWrite()["GameData"] = VuDevConfig::IF()->getParam("ActionGameData");
	const VuJsonContainer &gameData = VuGameUtil::IF()->dataRead()["GameData"];

	// load game project
	mpGameProject = VuProjectManager::IF()->load(mpGameProjectAsset);
	VuAssetFactory::IF()->releaseAsset(mpGameProjectAsset);
	mpGameProjectAsset = VUNULL;

	// create game
	const std::string &gameType = gameData["GameType"].asString();
	if ( gameType == "Race" )
		mpGame = new VuRaceGame(mpGameProject);
	else if ( gameType == "BoostBlitz" )
		mpGame = new VuBoostBlitzGame(mpGameProject);
	else if ( gameType == "Elimination" )
		mpGame = new VuEliminationGame(mpGameProject);
	else if ( gameType == "ShootingGallery" )
		mpGame = new VuShootingGalleryGame(mpGameProject);
	else if ( gameType == "DemolitionDerby" )
		mpGame = new VuDemolitionDerbyGame(mpGameProject);
	else if ( gameType == "FollowLeader" )
		mpGame = new VuFollowLeaderGame(mpGameProject);
	else if ( gameType == "BossBattle" )
		mpGame = new VuBossBattleGame(mpGameProject);
	else if ( gameType == "TutorialDrive" )
		mpGame = new VuTutorialDriveGame(mpGameProject);
	else if ( gameType == "TutorialPowerUp" )
		mpGame = new VuTutorialPowerUpGame(mpGameProject);
	else if ( gameType == "TutorialRace" )
		mpGame = new VuTutorialRaceGame(mpGameProject);
	else if ( gameType == "Duel" )
		mpGame = new VuDuelGame(mpGameProject);
	else if (gameType == "CarChampRace")
		mpGame = new VuCarChampRaceGame(mpGameProject);

	// load game data
	if ( mpGame )
		mpGame->load(gameData);

	mpGameProject->gameInitialize();
	if ( mpGame )
		mpGame->begin();

	VuAssetFactory::IF()->clearAssetCache();
	mPreloadQueue.clear();

	VuProfileManager::IF()->save();
	VuCloudSaveManager::IF()->save();

	// start music
	VuGameUtil::IF()->startMusic(VuGameUtil::MUSIC_GAME);
}

//*****************************************************************************
void VuActionGameMode::onLoadLevelTick(float fdt)
{
	do
	{
		if ( mPreloadIndex >= mPreloadQueue.size() )
		{
			mFSM.pulseCondition("LoadLevelFinished");
			break;
		}

		const PreloadEntry &entry = mPreloadQueue[mPreloadIndex];
		if ( VuAsset *pAsset = VuAssetFactory::IF()->createAsset(entry.first, entry.second, VuAssetFactory::OPTIONAL_ASSET) )
		{
			VuAssetFactory::IF()->cacheAsset(pAsset);
			VuAssetFactory::IF()->releaseAsset(pAsset);
		}

		mPreloadIndex++;
	}
	while ( VuSys::IF()->getTime() - mLastDrawTime < MIN_LOADING_TIME_BETWEEN_FRAMES );

	float progress = (mPreloadQueue.size() > 0) ? (float)mPreloadIndex/mPreloadQueue.size() : 1.0f;

	VuParams params;
	params.addFloat(progress);
	VuEventManager::IF()->broadcast("OnProgressUpdate", params);

	if ( VuUIScreenEntity *pScreen = getLoadingScreen() )
		pScreen->tick(fdt);
}

//*****************************************************************************
void VuActionGameMode::onLoadLevelDraw()
{
	if ( VuUIScreenEntity *pScreen = getLoadingScreen() )
	{
		pScreen->draw();
	}

	mLastDrawTime = VuSys::IF()->getTime();
}

//*****************************************************************************
void VuActionGameMode::onBeginEnter()
{
	if ( VuFadeManager::IF()->getFadeAmount() > 0.0f )
	{
		VuFadeManager::IF()->startFadeIn();
		VuFadeManager::IF()->skipFrames(3);
	}
}

//*****************************************************************************
void VuActionGameMode::onBeginTick(float fdt)
{
	if ( mpGame && !mpGame->tick(fdt) )
		mNextGameMode = "FrontEnd";
}

//*****************************************************************************
void VuActionGameMode::onEndEnter()
{
	VuFadeManager::IF()->startFadeOut();
}

//*****************************************************************************
void VuActionGameMode::onEndTick(float fdt)
{
	VuGameUtil::IF()->setMusicVolume(1.0f - VuFadeManager::IF()->getFadeAmount());
}

//*****************************************************************************
void VuActionGameMode::onRunEnter()
{
}

//*****************************************************************************
void VuActionGameMode::onRunTick(float fdt)
{
	if ( mpGame && !mpGame->tick(fdt) )
		mNextGameMode = "FrontEnd";

	mpPauseMenu->tick(fdt);
}

//*****************************************************************************
VuUIScreenEntity *VuActionGameMode::getLoadingScreen()
{
	if ( mpLoadingScreenProject && mpLoadingScreenProject->getRootEntity()->isDerivedFrom(VuUIScreenEntity::msRTTI) )
		return static_cast<VuUIScreenEntity *>(mpLoadingScreenProject->getRootEntity());

	return VUNULL;
}