//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  SplitScreenGameMode class
// 
//*****************************************************************************

#include "VuSplitScreenGameMode.h"
#include "VuPauseMenu.h"
#include "VuPurple/Entities/Car/Driver/VuAiDriverEntity.h"
#include "VuPurple/Managers/VuProjectManager.h"
#include "VuPurple/Managers/VuFadeManager.h"
#include "VuPurple/Managers/VuAnalyticsManager.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Managers/VuCloudSaveManager.h"
#include "VuPurple/Games/VuSplitScreenRaceGame.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuPurple/Util/VuAchievementUtil.h"
#include "VuEngine/Entities/UI/VuUIScreenEntity.h"
#include "VuEngine/Projects/VuProject.h"
#include "VuEngine/Managers/VuViewportManager.h"
#include "VuEngine/Managers/VuProfileManager.h"
#include "VuEngine/Managers/VuConfigManager.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Assets/VuAssetFactory.h"
#include "VuEngine/Assets/VuSpreadsheetAsset.h"
#include "VuEngine/Assets/VuProjectAsset.h"
#include "VuEngine/Gfx/GfxSort/VuGfxSort.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Memory/VuScratchPad.h"
#include "VuEngine/Dev/VuDevConfig.h"


IMPLEMENT_RTTI(VuSplitScreenGameMode, VuGameMode);


#define LOADING_SCREEN "Screens/Loading"
#define PAUSE_SCREEN "Screens/PauseSplitScreen"
#define MIN_LOADING_TIME_BETWEEN_FRAMES 0.1 // seconds


//*****************************************************************************
VuSplitScreenGameMode::VuSplitScreenGameMode():
	mpLoadingScreenProject(VUNULL),
	mpGameProjectAsset(VUNULL),
	mpGameProject(VUNULL),
	mpGame(VUNULL),
	mLastDrawTime(0.0),
	mpPauseMenu(VUNULL),
	mNextEventIndex(0)
{
	// add keyboard callback
	VuKeyboard::IF()->addCallback(this);

	// register event handlers
	REG_EVENT_HANDLER(VuSplitScreenGameMode, OnEndActionGame);
	REG_EVENT_HANDLER(VuSplitScreenGameMode, OnPauseActionGame);
	REG_EVENT_HANDLER(VuSplitScreenGameMode, OnResumeActionGame);
	REG_EVENT_HANDLER(VuSplitScreenGameMode, OnGameRematch);

	// create FSM
	{
		VuFSM::VuState *pState;

		pState = mFSM.addState("LoadLevel");
		pState->setEnterMethod(this, &VuSplitScreenGameMode::onLoadLevelEnter);
		pState->setExitMethod(this, &VuSplitScreenGameMode::onLoadLevelExit);
		pState->setTickMethod(this, &VuSplitScreenGameMode::onLoadLevelTick);
		pState->setDrawMethod(this, &VuSplitScreenGameMode::onLoadLevelDraw);

		pState = mFSM.addState("Begin");
		pState->setEnterMethod(this, &VuSplitScreenGameMode::onBeginEnter);
		pState->setTickMethod(this, &VuSplitScreenGameMode::onBeginTick);
		pState->setDrawMethod(this, &VuSplitScreenGameMode::onGameDraw);

		pState = mFSM.addState("End");
		pState->setEnterMethod(this, &VuSplitScreenGameMode::onEndEnter);
		pState->setTickMethod(this, &VuSplitScreenGameMode::onEndTick);
		pState->setDrawMethod(this, &VuSplitScreenGameMode::onGameDraw);

		pState = mFSM.addState("Run");
		pState->setEnterMethod(this, &VuSplitScreenGameMode::onRunEnter);
		pState->setTickMethod(this, &VuSplitScreenGameMode::onRunTick);
		pState->setDrawMethod(this, &VuSplitScreenGameMode::onGameDraw);

		pState = mFSM.addState("Reset");
		pState->setEnterMethod(this, &VuSplitScreenGameMode::onResetEnter);
		pState->setExitMethod(this, &VuSplitScreenGameMode::onResetExit);
		pState->setTickMethod(this, &VuSplitScreenGameMode::onResetTick);
		pState->setDrawMethod(this, &VuSplitScreenGameMode::onGameDraw);

		pState = mFSM.addState("Exit");

		// add transitions
		mFSM.addTransition("LoadLevel", "Begin", "LoadLevelFinished");

		mFSM.addTransition("Begin", "Run", "FadeManagerInactive");

		mFSM.addTransition("Run", "End", "NextGameModeSet");
		mFSM.addTransition("Run", "Reset", "RematchRequested");

		mFSM.addTransition("End", "Exit", "FadeManagerInactive");

		mFSM.addTransition("Reset", "LoadLevel", "FadeManagerInactive");
	}
}

//*****************************************************************************
VuSplitScreenGameMode::~VuSplitScreenGameMode()
{
	// remove keyboard callback
	VuKeyboard::IF()->removeCallback(this);
}

//*****************************************************************************
bool VuSplitScreenGameMode::enter(const std::string &prevMode)
{
	if ( !VuGameUtil::IF()->dataRead()["DevRestart"].asBool() )
	{
		if ( !VuGameUtil::IF()->dataRead().hasMember("GameData") )
			VuGameUtil::IF()->dataWrite()["GameData"] = VuDevConfig::IF()->getParam("ActionGameData");
		else
			configureNextEvent();
	}

	// count humans
	int viewportCount = 0;
	{
		const VuJsonContainer &cars = VuGameUtil::IF()->dataRead()["GameData"]["Cars"];
		for ( int i = 0; i < cars.size(); i++ )
			if ( cars[i]["Properties"]["DriverType"].asString() == "Human" )
				viewportCount++;
	}

	if ( viewportCount < 2 || viewportCount > 4 )
		return VUERROR("Invalid viewport count: %d", viewportCount);

	VuViewportManager::IF()->reset();
	VuViewportManager::IF()->setViewportCount(viewportCount);

	// reconfigure
	VuConfigManager::IF()->pushConfig();
	char strConfig[64];
	VU_SPRINTF(strConfig, sizeof(strConfig), "%sSplit%d", VuAssetFactory::IF()->getSku().c_str(), viewportCount);
	VuConfigManager::IF()->setConfig(strConfig);

	// initialize starting positions
	{
		VuJsonContainer &cars = VuGameUtil::IF()->dataWrite()["GameData"]["Cars"];
		for ( int i = 0; i < cars.size(); i++ )
			cars[i]["GridPos"].putValue(i);
	}

	mpPauseMenu = new VuPauseMenu(false);
	mpPauseMenu->setDefaultPauseMenu(PAUSE_SCREEN);

	mFSM.begin();

	VuGameUtil::IF()->dataWrite().removeMember("DevRestart");

	// initialize champ table
	{
		VuGameUtil::IF()->dataWrite()["ChampTable"].clear();
		const VuJsonContainer &cars = VuGameUtil::IF()->dataRead()["GameData"]["Cars"];
		for ( int i = 0; i < cars.size(); i++ )
			VuGameUtil::IF()->dataWrite()["ChampTable"][i] = cars[i]["Properties"];
	}

	return true;
}

//*****************************************************************************
void VuSplitScreenGameMode::exit()
{
	mFSM.end();

	delete mpPauseMenu;
	mpPauseMenu = VUNULL;

	reset();

	VuViewportManager::IF()->reset();

	VuConfigManager::IF()->popConfig();

	// if going back into action mode, reload DBs
	if ( VuGameUtil::IF()->dataRead()["DevRestart"].asBool() )
		VuGameUtil::IF()->reloadDBs();

	// recalculate achievements
	VuAchievementUtil::recalculateAchievements();
}

//*****************************************************************************
const char *VuSplitScreenGameMode::tick(float fdt)
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
void VuSplitScreenGameMode::draw()
{
	mFSM.draw();
}

//*****************************************************************************
void VuSplitScreenGameMode::onKeyDown(VUUINT32 key)
{
	if ( key == VUKEY_SPACE )
	{
		mNextGameMode = "SplitScreen";
		VuGameUtil::IF()->dataWrite()["DevRestart"].putValue(true);
	}
}

//*****************************************************************************
void VuSplitScreenGameMode::reset()
{
	VuGfxSort::IF()->flush();

	// stop music
	VuGameUtil::IF()->stopMusic();

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

	mFSM.clearAllConditions();
}

//*****************************************************************************
void VuSplitScreenGameMode::configureNextEvent()
{
	const VuJsonContainer &champData = getChampData();
	const VuJsonContainer &events = champData["Events"];
	const std::string &trackName = events[mNextEventIndex].asString();

	VuJsonContainer &gameData = VuGameUtil::IF()->dataWrite()["GameData"];
	
	gameData["Champ"] = champData["Name"];
	gameData["EventIndex"].putValue(mNextEventIndex);
	gameData["ProjectAsset"].putValue(trackName + "_Race");
	gameData["Track"].putValue(trackName);
	gameData["GameType"].putValue("Race");
	gameData["LapCount"] = champData["LapCount"];
	gameData["TimeLimit"] = champData["TimeLimit"];

	mNextEventIndex = (mNextEventIndex + 1)%events.size();
}

//*****************************************************************************
const VuJsonContainer &VuSplitScreenGameMode::getChampData()
{
	const std::string &champName = VuGameUtil::IF()->getChampName();
	const VuJsonContainer &champData = VuGameUtil::IF()->champDB()[champName];

	if ( champData.isNull() )
		return VuGameUtil::IF()->champDB()[VuGameUtil::IF()->constantDB()["Names"]["Champs"][0].asString()];

	return champData;
}

//*****************************************************************************
void VuSplitScreenGameMode::OnEndActionGame(const VuParams &params)
{
	mNextGameMode = "FrontEnd";
}

//*****************************************************************************
void VuSplitScreenGameMode::OnPauseActionGame(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	VUUINT32 padMask = (accessor.getNextType() == VuParams::Int) ? (1<<accessor.getInt()) : 0xff;

	mpPauseMenu->pause(PAUSE_SCREEN, true, padMask);
}

//*****************************************************************************
void VuSplitScreenGameMode::OnResumeActionGame(const VuParams &params)
{
	mpPauseMenu->unpause();
}

//*****************************************************************************
void VuSplitScreenGameMode::OnGameRematch(const VuParams &params)
{
	mFSM.setCondition("RematchRequested", true);
}

//*****************************************************************************
void VuSplitScreenGameMode::onGameDraw()
{
	// draw lines on splits
	VuVector2 *verts = (VuVector2 *)VuScratchPad::get();
	int vertCount = 0;

	VuVector2 *pv = verts;
	#define ADD_LINE(v0, v1) { pv[0] = v0; pv[1] = v1; pv += 2; vertCount += 2; }

	if ( VuViewportManager::IF()->getViewportCount() == 2 )
	{
		ADD_LINE(VuViewportManager::IF()->getViewport(0).mUnsafeRect.getBottomLeft(), VuViewportManager::IF()->getViewport(0).mUnsafeRect.getBottomRight());
	}
	else if ( VuViewportManager::IF()->getViewportCount() == 3 )
	{
		ADD_LINE(VuViewportManager::IF()->getViewport(0).mUnsafeRect.getBottomLeft(), VuViewportManager::IF()->getViewport(0).mUnsafeRect.getBottomRight());
		ADD_LINE(VuViewportManager::IF()->getViewport(1).mUnsafeRect.getTopRight(), VuViewportManager::IF()->getViewport(1).mUnsafeRect.getBottomRight());
	}
	else if ( VuViewportManager::IF()->getViewportCount() == 4 )
	{
		ADD_LINE(VuViewportManager::IF()->getViewport(0).mUnsafeRect.getBottomLeft(), VuViewportManager::IF()->getViewport(1).mUnsafeRect.getBottomRight());
		ADD_LINE(VuViewportManager::IF()->getViewport(0).mUnsafeRect.getTopRight(), VuViewportManager::IF()->getViewport(2).mUnsafeRect.getBottomRight());
	}
	else if ( VuViewportManager::IF()->getViewportCount() == 5 )
	{
		ADD_LINE(VuViewportManager::IF()->getViewport(0).mUnsafeRect.getBottomLeft(), VuViewportManager::IF()->getViewport(1).mUnsafeRect.getBottomRight());
		ADD_LINE(VuViewportManager::IF()->getViewport(0).mUnsafeRect.getTopRight(), VuViewportManager::IF()->getViewport(0).mUnsafeRect.getBottomRight());
		ADD_LINE(VuViewportManager::IF()->getViewport(2).mUnsafeRect.getTopRight(), VuViewportManager::IF()->getViewport(2).mUnsafeRect.getBottomRight());
		ADD_LINE(VuViewportManager::IF()->getViewport(3).mUnsafeRect.getTopRight(), VuViewportManager::IF()->getViewport(3).mUnsafeRect.getBottomRight());
	}
	else if ( VuViewportManager::IF()->getViewportCount() == 6 )
	{
		ADD_LINE(VuViewportManager::IF()->getViewport(0).mUnsafeRect.getBottomLeft(), VuViewportManager::IF()->getViewport(2).mUnsafeRect.getBottomRight());
		ADD_LINE(VuViewportManager::IF()->getViewport(0).mUnsafeRect.getTopRight(), VuViewportManager::IF()->getViewport(3).mUnsafeRect.getBottomRight());
		ADD_LINE(VuViewportManager::IF()->getViewport(1).mUnsafeRect.getTopRight(), VuViewportManager::IF()->getViewport(4).mUnsafeRect.getBottomRight());
	}

	#undef ADD_LINE

	if ( vertCount )
	{
		VuGfxSort::IF()->setFullScreenLayer(VuGfxSort::FSL_HUD);
		VuGfxSort::IF()->setViewport(0);
		VuGfxSort::IF()->setReflectionLayer(0);
		VuGfxSort::IF()->setViewportLayer(VuGfxSort::VPL_HUD);

		VuGfxUtil::IF()->drawLines2d(1.0f, VUGFX_PT_LINELIST, VuColor(0,0,0), verts, vertCount);
	}

	if ( mpGame )
	{
		mpGame->draw();
	}

	mpPauseMenu->draw();
}

//*****************************************************************************
void VuSplitScreenGameMode::onLoadLevelEnter()
{
	mpLoadingScreenProject = VuProjectManager::IF()->load(LOADING_SCREEN);
	if ( mpLoadingScreenProject )
		mpLoadingScreenProject->gameInitialize();

	VuFadeManager::IF()->startFadeIn(0.0f);

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
void VuSplitScreenGameMode::onLoadLevelExit()
{
	if ( mpLoadingScreenProject )
	{
		mpLoadingScreenProject->gameRelease();
		VuProjectManager::IF()->unload(mpLoadingScreenProject);
		mpLoadingScreenProject = VUNULL;
	}

	VuFadeManager::IF()->startFadeOut(0.0f);

	const VuJsonContainer &gameData = VuGameUtil::IF()->dataRead()["GameData"];

	// load game project
	mpGameProject = VuProjectManager::IF()->load(mpGameProjectAsset);
	VuAssetFactory::IF()->releaseAsset(mpGameProjectAsset);
	mpGameProjectAsset = VUNULL;

	// create game
	const std::string &gameType = gameData["GameType"].asString();
	if ( gameType == "Race" )
		mpGame = new VuSplitScreenRaceGame(mpGameProject);

	// load game data
	if ( mpGame )
		mpGame->load(gameData);

	mpGameProject->gameInitialize();
	if ( mpGame )
		mpGame->begin();

	VuAssetFactory::IF()->clearAssetCache();
	mPreloadQueue.clear();

	// set points stats
	for ( int i = 0; i < VuCarManager::IF()->getCarCount(); i++ )
	{
		VuCarEntity *pCar = VuCarManager::IF()->getCar(i);
		VuCarStats *pStats = &pCar->getStats();

		pStats->mBasePoints = VuGameUtil::IF()->dataRead()["ChampTable"][i]["Points"].asInt();
	}

	VuProfileManager::IF()->save();
	VuCloudSaveManager::IF()->save();

	// start music
	VuGameUtil::IF()->startMusic(VuGameUtil::MUSIC_GAME);
}

//*****************************************************************************
void VuSplitScreenGameMode::onLoadLevelTick(float fdt)
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
	{
		pScreen->tick(fdt);
	}
}

//*****************************************************************************
void VuSplitScreenGameMode::onLoadLevelDraw()
{
	if ( getLoadingScreen() )
	{
		getLoadingScreen()->draw();
	}

	mLastDrawTime = VuSys::IF()->getTime();
}

//*****************************************************************************
void VuSplitScreenGameMode::onBeginEnter()
{
	VuFadeManager::IF()->startFadeIn();
	VuFadeManager::IF()->skipFrames(3);
}

//*****************************************************************************
void VuSplitScreenGameMode::onBeginTick(float fdt)
{
	if ( mpGame && !mpGame->tick(fdt) )
		mNextGameMode = "FrontEnd";
}

//*****************************************************************************
void VuSplitScreenGameMode::onEndEnter()
{
	VuFadeManager::IF()->startFadeOut();
}

//*****************************************************************************
void VuSplitScreenGameMode::onEndTick(float fdt)
{
	VuGameUtil::IF()->setMusicVolume(1.0f - VuFadeManager::IF()->getFadeAmount());
}

//*****************************************************************************
void VuSplitScreenGameMode::onRunEnter()
{
}

//*****************************************************************************
void VuSplitScreenGameMode::onRunTick(float fdt)
{
	if ( mpGame && !mpGame->tick(fdt) )
		mNextGameMode = "FrontEnd";

	mpPauseMenu->tick(fdt);

	// update points
	VuJsonContainer &champTable = VuGameUtil::IF()->dataWrite()["ChampTable"];
	for ( int i = 0; i < VuCarManager::IF()->getCarCount(); i++ )
	{
		VuCarEntity *pCar = VuCarManager::IF()->getCar(i);
		VuCarStats *pStats = &pCar->getStats();

		champTable[i]["Points"].putValue(pStats->mPoints);
		champTable[i]["Rank"].putValue(pStats->mChampRank);
	}
}

//*****************************************************************************
void VuSplitScreenGameMode::onResetEnter()
{
	VuFadeManager::IF()->startFadeOut();
}

//*****************************************************************************
void VuSplitScreenGameMode::onResetExit()
{
	reset();
	configureNextEvent();

	// update starting positions
	{
		VuJsonContainer &cars = VuGameUtil::IF()->dataWrite()["GameData"]["Cars"];
		for ( int i = 0; i < cars.size(); i++ )
		{
			int gridPos = cars[i]["GridPos"].asInt();
			gridPos = (gridPos + 1)%cars.size();
			cars[i]["GridPos"].putValue(gridPos);
		}
	}
}

//*****************************************************************************
void VuSplitScreenGameMode::onResetTick(float fdt)
{
	VuGameUtil::IF()->setMusicVolume(1.0f - VuFadeManager::IF()->getFadeAmount());
}

//*****************************************************************************
VuUIScreenEntity *VuSplitScreenGameMode::getLoadingScreen()
{
	if ( mpLoadingScreenProject && mpLoadingScreenProject->getRootEntity()->isDerivedFrom(VuUIScreenEntity::msRTTI) )
		return static_cast<VuUIScreenEntity *>(mpLoadingScreenProject->getRootEntity());

	return VUNULL;
}
