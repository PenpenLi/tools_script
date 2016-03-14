//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  AiTestGameMode class
// 
//*****************************************************************************
#include "VuAiTestGameMode.h"

#include "VuEngine/Entities/UI/VuUIScreenEntity.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Projects/VuProject.h"
#include "VuEngine/Managers/VuViewportManager.h"
#include "VuEngine/Managers/VuProfileManager.h"
#include "VuEngine/Managers/VuLensWaterManager.h"
#include "VuEngine/Assets/VuAssetFactory.h"
#include "VuEngine/Assets/VuProjectAsset.h"
#include "VuEngine/HAL/Audio/VuAudio.h"
#include "VuEngine/HAL/Gfx/VuGfx.h"
#include "VuEngine/Gfx/GfxSort/VuGfxSort.h"
#include "VuEngine/Dev/VuDevConfig.h"
#include "VuEngine/Assets/VuSpreadsheetAsset.h"
#include "VuEngine/Managers/VuConfigManager.h"

#include "VuPurple/AI/VuAiUtils.h"
#include "VuPurple/AI/VuAiManager.h"
#include "VuPurple/Entities/Car/Driver/VuAiDriverEntity.h"
#include "VuPurple/Entities/Car/Camera/VuCarCamera.h"
#include "VuPurple/Managers/VuProjectManager.h"
#include "VuPurple/Managers/VuFadeManager.h"
#include "VuPurple/Managers/VuAnalyticsManager.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Managers/VuCloudSaveManager.h"
#include "VuPurple/Managers/VuControlMethodManager.h"
#include "VuPurple/Gfx/GfxComposer/VuPurpleGfxComposer.h"
#include "VuPurple/Games/VuAiTestGame.h"
#include "VuPurple/Track/VuTrackManager.h"
#include "VuPurple/Track/VuTrackUtil.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuPurple/Util/VuAchievementUtil.h"


IMPLEMENT_RTTI(VuAiTestGameMode, VuGameMode);


#define LOADING_SCREEN "Screens/Loading"
#define MAX_LOADING_SCREEN_DURATION (5.0f) // seconds

static int sAiTestModeTrackIndex = 0;
static int sAiTestModeNumRaces = 0;
static int sAiTestModeOpponentListIndex = 0;
static int sAiTestModeOpponentListMax = 0;
static VuJsonContainer sOpponentData;

static int sAiTestModeEventIndex = 0;

//*****************************************************************************
VuAiTestGameMode::VuAiTestGameMode() :
	mpLoadingScreenProject(VUNULL),
	mpGameProject(VUNULL),
	mpGame(VUNULL),
	mLoadingScreenDuration(MAX_LOADING_SCREEN_DURATION),
	mAiTestMode(false),
	mAllOpponents(false),
	mLogHotSpots(false),
	mAlwaysLogFrames(false),
	mLogsPerSecond(4.0f),
	mLastLogTime(0.0f),
	mNumOpponents(5)
{
	// register event handlers
	REG_EVENT_HANDLER(VuAiTestGameMode, OnEndActionGame);
	REG_EVENT_HANDLER(VuAiTestGameMode, OnRestartActionGame);
	REG_EVENT_HANDLER(VuAiTestGameMode, OnSkipLoadingScreen);

	// add keyboard callback
	VuKeyboard::IF()->addCallback(this);

	// create FSM
	{
		VuFSM::VuState *pState;

		if ( !VuGameUtil::IF()->dataRead()["DevRestart"].asBool() )
		{
			pState = mFSM.addState("LoadingScreen");
			pState->setEnterMethod(this, &VuAiTestGameMode::onLoadingScreenEnter);
			pState->setExitMethod(this, &VuAiTestGameMode::onLoadingScreenExit);
			pState->setTickMethod(this, &VuAiTestGameMode::onLoadingScreenTick);
			pState->setDrawMethod(this, &VuAiTestGameMode::onLoadingScreenDraw);
		}

		pState = mFSM.addState("LoadLevel");
		pState->setEnterMethod(this, &VuAiTestGameMode::onLoadLevelEnter);
		pState->setExitMethod(this, &VuAiTestGameMode::onLoadLevelExit);
		pState->setTickMethod(this, &VuAiTestGameMode::onLoadLevelTick);
		pState->setDrawMethod(this, &VuAiTestGameMode::onLoadLevelDraw);

		pState = mFSM.addState("Begin");
		pState->setEnterMethod(this, &VuAiTestGameMode::onBeginEnter);
		pState->setTickMethod(this, &VuAiTestGameMode::onBeginTick);
		pState->setDrawMethod(this, &VuAiTestGameMode::onGameDraw);

		pState = mFSM.addState("End");
		pState->setEnterMethod(this, &VuAiTestGameMode::onEndEnter);
		pState->setTickMethod(this, &VuAiTestGameMode::onEndTick);
		pState->setDrawMethod(this, &VuAiTestGameMode::onGameDraw);

		pState = mFSM.addState("Run");
		pState->setEnterMethod(this, &VuAiTestGameMode::onRunEnter);
		pState->setTickMethod(this, &VuAiTestGameMode::onRunTick);
		pState->setDrawMethod(this, &VuAiTestGameMode::onGameDraw);

		pState = mFSM.addState("Exit");

		// add transitions
		if ( !VuGameUtil::IF()->dataRead()["DevRestart"].asBool() )
		{
			mFSM.addTransition("LoadingScreen", "LoadLevel", "LoadingScreenShown");
		}

		mFSM.addTransition("LoadLevel", "Begin", "LoadLevelFinished");

		mFSM.addTransition("Begin", "Run", "FadeManagerInactive");

		mFSM.addTransition("Run", "Exit", "Restart");
		mFSM.addTransition("Run", "End", "NextGameModeSet");

		mFSM.addTransition("End", "Exit", "FadeManagerInactive");
	}

	// See if we're supposed to be in AITestMode
	if (VuDevConfig::IF()->hasParam("AITestMode"))
	{
		const VuJsonContainer& testModeData = VuDevConfig::IF()->getParam("AITestMode");

		mAiTestMode = true;

		mAllOpponents =		testModeData.hasMember("AllOpponents")	? testModeData["AllOpponents"].asBool() : false;
		mAllEvents =		testModeData.hasMember("AllEvents")		? testModeData["AllEvents"].asBool() : false;
		mNumOpponents =		testModeData.hasMember("NumOpponents")	? testModeData["NumOpponents"].asInt() : 5;
		mLogHotSpots =		testModeData.hasMember("LogHotSpots")	? testModeData["LogHotSpots"].asBool() : false;
		mAlwaysLogFrames =	testModeData.hasMember("AlwaysLogFrames") ? testModeData["AlwaysLogFrames"].asBool() : false;
		mLogsPerSecond =	testModeData.hasMember("LogsPerSecond") ? testModeData["LogsPerSecond"].asFloat() : 120.0f;
		mLogCars =			testModeData.hasMember("LogCars")		? testModeData["LogCars"].asBool() : false;

		mAiTestModeTracks.clear();

		if (mAllEvents)
		{
			mEventList.clear();

			const VuSpreadsheetAsset *pData = VuGameUtil::IF()->eventSpreadsheet();

			for (int i = 0; i < pData->getRowCount(); i++)
			{
				const VuFastContainer& data = pData->getRow(i);

				// Races only
				std::string eventType = data[2].asCString();
				if (eventType != "Race")
				{
					continue;
				}

				VuAiEventInfo eventInfo;

				eventInfo.mEvent = data[1].asCString();
				eventInfo.mType = data[2].asCString();
				eventInfo.mTrack = data[3].asCString();
				
				for (int j = 0; j < 5; j++)
				{
					const char *opponentName = data[14 + j].asCString();

					eventInfo.mOpponents[j] = opponentName;
				}

				VUPRINTF("%s, %s, %s, %s, %s, %s, %s, %s\n", eventInfo.mEvent.c_str(), eventInfo.mType.c_str(), eventInfo.mTrack.c_str(),
															eventInfo.mOpponents[0].c_str(),
															eventInfo.mOpponents[1].c_str(),
															eventInfo.mOpponents[2].c_str(),
															eventInfo.mOpponents[3].c_str(),
															eventInfo.mOpponents[4].c_str());
				mEventList.push_back(eventInfo);
			}
		}
		else
		{
			const VuJsonContainer& trackData = testModeData["Tracks"];
			for (int i = 0; i < trackData.size(); i++)
			{
				const std::string name = trackData[i].asString();
				mAiTestModeTracks.push_back(name);
			}
		}
	}

	// Build opponent lists if necessary
	if (sAiTestModeOpponentListMax == 0)
	{
		VUPRINTF("Building Opponent Groups:\n");

		if (mAllOpponents)
		{
			const VuSpreadsheetAsset *pData = VuGameUtil::IF()->opponentSpreadsheet();

			int counter = 0;
			for (int i = 0; i < pData->getRowCount(); i++)
			{
				const VuFastContainer& data = pData->getRow(i);
				std::string name = data[0].asCString();

				// Skip bosses and default opponents
				if (name.find("Default") != std::string::npos ||
					name.find("Boss") != std::string::npos)
				{
					continue;
				}

				VUPRINTF("%s ", name.c_str());

				sOpponentData[sAiTestModeOpponentListMax][counter].putValue(name);

				if (++counter >= mNumOpponents)
				{
					VUPRINTF("\n");
					counter = 0;
					sAiTestModeOpponentListMax++;
				}
			}
			VUPRINTF("\n");
		}
	}
}

//*****************************************************************************
VuAiTestGameMode::~VuAiTestGameMode()
{
	// remove keyboard callback
	VuKeyboard::IF()->removeCallback(this);
}

//*****************************************************************************
bool VuAiTestGameMode::enter(const std::string &prevMode)
{
	VuViewportManager::IF()->reset();
	VuViewportManager::IF()->setViewportCount(1);

	if ( !VuGameUtil::IF()->dataRead()["DevRestart"].asBool() )
	{
		mpLoadingScreenProject = VuProjectManager::IF()->load(LOADING_SCREEN);
		mLoadingScreenDuration = 0.0f;
	}

	mFSM.begin();

	VuGameUtil::IF()->dataWrite().removeMember("DevRestart");

	VuAiUtils::connectToLogServer();

	return true;
}

//*****************************************************************************
void VuAiTestGameMode::exit()
{
	VuAiUtils::disconnectFromLogServer();

	mFSM.end();

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

	// unload loading screen
	if ( mpLoadingScreenProject )
	{
		VuProjectManager::IF()->unload(mpLoadingScreenProject);
		mpLoadingScreenProject = VUNULL;
	}

	VuViewportManager::IF()->reset();

	// if going back into action mode, reload DBs
	if ( VuGameUtil::IF()->dataRead()["DevRestart"].asBool() )
		VuGameUtil::IF()->reloadDBs();
}

//*****************************************************************************
const char *VuAiTestGameMode::tick(float fdt)
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
void VuAiTestGameMode::draw()
{
	mFSM.draw();
}

//*****************************************************************************
void VuAiTestGameMode::OnEndActionGame(const VuParams &params)
{
	mNextGameMode = "AiTest";
}

//*****************************************************************************
void VuAiTestGameMode::OnRestartActionGame(const VuParams &params)
{
	mNextGameMode = "AiTest";
}

//*****************************************************************************
void VuAiTestGameMode::OnSkipLoadingScreen(const VuParams &params)
{
	mFSM.setCondition("LoadLevelFinished", true);
}

//*****************************************************************************
void VuAiTestGameMode::onKeyDown(VUUINT32 key)
{
	if (key == VUKEY_SPACE)
	{
		mNextGameMode = "AiTest";
		mFSM.pulseCondition("Restart");
		VuGameUtil::IF()->dataWrite()["DevRestart"].putValue(true);
	}
	//else if (key == VUKEY_F1)
	//{
	//	// Crash
	//	int* pInt = VUNULL;
	//	*pInt = 0;
	//}

}

//*****************************************************************************
void VuAiTestGameMode::onGameDraw()
{
	if ( mpGame )
	{
		mpGame->draw();
	}
}

//*****************************************************************************
void VuAiTestGameMode::onLoadingScreenEnter()
{
	if ( mpLoadingScreenProject )
		mpLoadingScreenProject->gameInitialize();

	VuFadeManager::IF()->startFadeIn(0.0f);
}

//*****************************************************************************
void VuAiTestGameMode::onLoadingScreenExit()
{
}

//*****************************************************************************
void VuAiTestGameMode::onLoadingScreenTick(float fdt)
{
	mFSM.pulseCondition("LoadingScreenShown");
}

//*****************************************************************************
void VuAiTestGameMode::onLoadingScreenDraw()
{
	if ( getLoadingScreen() )
	{
		getLoadingScreen()->draw();
	}
}

//*****************************************************************************
void VuAiTestGameMode::onLoadLevelEnter()
{
	mLoadLevelStartTime = (float)VuSys::IF()->getTime();

	if ( !VuGameUtil::IF()->dataRead().hasMember("GameData") )
		VuGameUtil::IF()->dataWrite()["GameData"] = VuDevConfig::IF()->getParam("ActionGameData");

	VuJsonContainer &gameData = VuGameUtil::IF()->dataWrite()["GameData"];

	// load game project
	const std::string &projectAsset = gameData["ProjectAsset"].asString();

	if (mAiTestMode)
	{
		if (mAllEvents)
		{
			VuAiEventInfo& event = mEventList[sAiTestModeEventIndex];
			const std::string &projectAsset = event.mTrack + "_Race";

			gameData["ProjectAsset"].putValue(projectAsset);
			gameData["Track"].putValue(event.mTrack);
			gameData["EventName"].putValue(event.mEvent);

			mpGameProject = VuProjectManager::IF()->load(projectAsset);
		}
		else
		{
			const std::string &track = mAiTestModeTracks[sAiTestModeTrackIndex];
			const std::string &projectAsset = track + "_Race";

			gameData["ProjectAsset"].putValue(projectAsset);
			gameData["Track"].putValue(track);

			mpGameProject = VuProjectManager::IF()->load(projectAsset);
		}
	}
	else
	{
		mpGameProject = VuProjectManager::IF()->load(projectAsset);
	}
	VUASSERT(mpGameProject, "Unable to load game project!");

	mpGame = new VuAiTestGame(mpGameProject);

	if (mAllEvents)
	{
		// Poke in useful Opponent data
		VuJsonContainer& data = gameData["Opponents"];

		VuAiEventInfo& event = mEventList[sAiTestModeEventIndex];
		for (int i = 0; i < 5; i++)
		{
			std::string& opponentName = event.mOpponents[i];

			data[i].putValue(opponentName);
		}
	}
	else if (mAllOpponents)
	{
		// Poke in useful Opponent data
		VuJsonContainer& data = gameData["Opponents"];

		data = sOpponentData[sAiTestModeOpponentListIndex];
	}

	// load game data
	if ( mpGame )
		mpGame->load(gameData);

	VuAssetFactory::IF()->clearAssetCache();
}

//*****************************************************************************
void VuAiTestGameMode::onLoadLevelExit()
{
	if ( mpLoadingScreenProject )
	{
		mpLoadingScreenProject->gameRelease();
		VuFadeManager::IF()->startFadeOut(0.0f);
	}

	mpGameProject->gameInitialize();
	if ( mpGame )
		mpGame->begin();

	VuProfileManager::IF()->save();
	VuCloudSaveManager::IF()->save();

	// start music
	VuGameUtil::IF()->startMusic(VuGameUtil::MUSIC_GAME);
}

//*****************************************************************************
void VuAiTestGameMode::onLoadLevelTick(float fdt)
{
	if ( VuUIScreenEntity *pScreen = getLoadingScreen() )
	{
		pScreen->tick(fdt);

		if ( VuSys::IF()->getTime() - mLoadLevelStartTime > mLoadingScreenDuration )
			mFSM.pulseCondition("LoadLevelFinished");
	}
	else
	{
		mFSM.pulseCondition("LoadLevelFinished");
	}
}

//*****************************************************************************
void VuAiTestGameMode::onLoadLevelDraw()
{
	if ( VuUIScreenEntity *pScreen = getLoadingScreen() )
	{
		pScreen->draw();
	}
}

//*****************************************************************************
void VuAiTestGameMode::onBeginEnter()
{
	if ( VuFadeManager::IF()->getFadeAmount() > 0.0f )
	{
		VuFadeManager::IF()->startFadeIn();
		VuFadeManager::IF()->skipFrames(3);
	}
}

//*****************************************************************************
void VuAiTestGameMode::onBeginTick(float fdt)
{
	if ( mpGame && !mpGame->tick(fdt) )
		mNextGameMode = "AiTest";
}

//*****************************************************************************
void VuAiTestGameMode::onEndEnter()
{
	// Write end of race stats
	int num = VuAiManager::IF()->getNumInstances();
	for (int i = 0; i < num; i++)
	{
		VuAiInstance* pInstance = VuAiManager::IF()->getInstance(i);

		VuCarStats& stats = pInstance->getCar()->getStats();

		char tempStr[64];
		VU_SPRINTF(tempStr, 64, "%d", stats.mPlace);

		VuAiUtils::reportEvent(pInstance, "FinishPlace", tempStr);
	}

	VuFadeManager::IF()->startFadeOut();

	if (mAllEvents)
	{
		if (++sAiTestModeEventIndex >= (int)mEventList.size())
		{
			sAiTestModeEventIndex = 0;
		}
	}
	else if (++sAiTestModeTrackIndex >= (int)mAiTestModeTracks.size())
	{
		sAiTestModeTrackIndex = 0;
		if (++sAiTestModeOpponentListIndex >= sAiTestModeOpponentListMax)
		{
			sAiTestModeOpponentListIndex = 0;
		}
	}

	sAiTestModeNumRaces++;
}

//*****************************************************************************
void VuAiTestGameMode::onEndTick(float fdt)
{
	VuGameUtil::IF()->setMusicVolume(1.0f - VuFadeManager::IF()->getFadeAmount());
}

//*****************************************************************************
void VuAiTestGameMode::onRunEnter()
{
}

//*****************************************************************************
void VuAiTestGameMode::onRunTick(float fdt)
{
	if (mAiTestMode)
	{
		if (mpGame->allCarsFinished())
		{
			mNextGameMode = "AiTest";
			mFSM.setCondition("NextGameModeSet", true);
			return;
		}
	}

	if (mLogCars && VuSys::IF()->getTime() > mLastLogTime + 1.0f / mLogsPerSecond)
	{
		for (int i = 0; i < VuAiManager::IF()->getNumInstances(); i++)
		{
			VuAiInstance* pInstance = VuAiManager::IF()->getInstance(i);
			if (pInstance)
			{
				if (mAllEvents)
				{
					VuAiUtils::reportCarStatsEvent(pInstance, mEventList[sAiTestModeEventIndex].mEvent);
				}
				else
				{
					VuAiUtils::reportCarStatsEvent(pInstance, VuGameUtil::IF()->getTrack());
				}
			}
		}

		if (!mLogHotSpots)
		{
			mLastLogTime = (float)VuSys::IF()->getTime();
		}
	}

	if (mLogHotSpots)
	{
		VuCarEntity* pCar = VuCarManager::IF()->getCameraTargetForViewport(0);
		VuCarCamera* pCamera = pCar->getCamera();

		VuAiTestLogItem newItem;

		newItem.mFrameTime = fdt;
		newItem.mDrawCalls = VuGfx::IF()->getPrevDrawCallCount();
		newItem.mPrimitiveCount = VuGfx::IF()->getPrevPrimitiveCount();
		newItem.mCameraPos = pCamera->getEyePosition();
		newItem.mCameraDir = (pCamera->getTargetPosition() - pCamera->getEyePosition()).normal();

		mLog.push_back(newItem);

		if (mLog.size() > LOG_HISTORY_SIZE)
		{
			VuAiTestLogItem firstItem = mLog.front();
			mLog.pop_front();

			VuAiTestLog tempLog = mLog;

			tempLog.sort([](const VuAiTestLogItem& item1, const VuAiTestLogItem& item2)
			{
				if (item1.mFrameTime < item2.mFrameTime) return true;
				else return false;
			}
			);

			// Remove the largest and smallest samples from the list
			tempLog.pop_front();
			tempLog.pop_back();

			// Average the frame times
			float frameTimeSum = 0.0f;
			int counter = 0;

			for (auto& item : tempLog)
			{
				frameTimeSum += item.mFrameTime;
				counter++;
			}

			float avgFrameTime = frameTimeSum / (float)counter;
			if (avgFrameTime < FLT_EPSILON)
			{
				avgFrameTime = FLT_EPSILON;
			}

			float avgFrameRate = 1.0f / avgFrameTime;

			if ((mAlwaysLogFrames && VuSys::IF()->getTime() > mLastLogTime + 1.0f / mLogsPerSecond) 
					|| 
				(!mAlwaysLogFrames && avgFrameRate < VuConfigManager::IF()->getInt("Perf/MinimumFrameRate")->mValue))
			{
				VuAiUtils::reportHotSpotEvent(avgFrameRate, newItem.mDrawCalls, newItem.mPrimitiveCount, newItem.mCameraPos, newItem.mCameraDir);
				mLastLogTime = (float)VuSys::IF()->getTime();
			}
		}
	}

	if ( mpGame && !mpGame->tick(fdt) )
		mNextGameMode = "AiTest";
}

//*****************************************************************************
VuUIScreenEntity *VuAiTestGameMode::getLoadingScreen()
{
	if ( mpLoadingScreenProject && mpLoadingScreenProject->getRootEntity()->isDerivedFrom(VuUIScreenEntity::msRTTI) )
		return static_cast<VuUIScreenEntity *>(mpLoadingScreenProject->getRootEntity());

	return VUNULL;
}