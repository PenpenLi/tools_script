//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  BoostBlitzGame class
// 
//*****************************************************************************

#include "VuBoostBlitzGame.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarEngine.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/Entities/Car/VuGhostCarEntity.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Managers/VuStatsManager.h"
#include "VuPurple/Managers/VuLeaderboardManager.h"
#include "VuPurple/Track/VuTrackManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/HAL/File/VuFile.h"
#include "VuEngine/Managers/VuProfileManager.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Util/VuFileUtil.h"
#include "VuEngine/Util/VuZLibUtil.h"
#include "VuEngine/Dev/VuDevConfig.h"


IMPLEMENT_RTTI(VuBoostBlitzGame, VuGame);

static const VUUINT32 scBoostBlitzRecordingMagic = ('V'<<24)|('U'<<16)|('B'<<8)|('R');
static const VUUINT32 scBoostBlitzRecordingVersion = 1;

#define BOOST_BLITZ_RECORD_MAX_TIME 120.0f // seconds
#define BOOST_BLITZ_RECORD_TIME_STEP (1.0f/30.0f) // seconds
//#define BOOST_BLITZ_RECORD_TIME_STEP 0.3 // seconds


//*****************************************************************************
VuBoostBlitzGame::VuBoostBlitzGame(VuProject *pProject):
	VuGame(pProject),
	mpRecorder(VUNULL),
	mpGhostCar(VUNULL)
{
	// create FSM
	{
		VuFSM::VuState *pState;
		
		if ( !VuDevConfig::IF()->getParam("SkipIntro").asBool() )
		{
			pState = mGameFSM.addState("Intro");
			pState->setEnterMethod(this, &VuBoostBlitzGame::onIntroEnter);
			pState->setExitMethod(this, &VuBoostBlitzGame::onIntroExit);
			pState->setTickMethod(this, &VuBoostBlitzGame::onIntroTick);
		}

		pState = mGameFSM.addState("PreGame");
		pState->setEnterMethod(this, &VuBoostBlitzGame::onPreGameEnter);
		pState->setExitMethod(this, &VuBoostBlitzGame::onPreGameExit);
		pState->setTickMethod(this, &VuBoostBlitzGame::onPreGameTick);

		pState = mGameFSM.addState("Game");
		pState->setEnterMethod(this, &VuBoostBlitzGame::onGameEnter);
		pState->setExitMethod(this, &VuBoostBlitzGame::onGameExit);
		pState->setTickMethod(this, &VuBoostBlitzGame::onGameTick);

		pState = mGameFSM.addState("PostGame");
		pState->setEnterMethod(this, &VuBoostBlitzGame::onPostGameEnter);
		pState->setExitMethod(this, &VuBoostBlitzGame::onPostGameExit);

		pState = mGameFSM.addState("Exit");

		// transitions
		mGameFSM.addTransition("Intro", "PreGame", "IntroDone");
		mGameFSM.addTransition("PreGame", "Game", "StartLightsOut");
		mGameFSM.addTransition("Game", "PostGame", "Finished");
		mGameFSM.addTransition("", "Exit", "Exit");
	}
}

//*****************************************************************************
VuBoostBlitzGame::~VuBoostBlitzGame()
{
	delete mpRecorder;
}

//*****************************************************************************
void VuBoostBlitzGame::onLoad(const VuJsonContainer &data)
{
	// load HUDs
	loadHUD("Intro", "HUDs/BoostBlitzIntroHUD");
	loadHUD("PreGame", "HUDs/BoostBlitzPreGameHUD");
	loadHUD("Game", "HUDs/BoostBlitzGameHUD");
	loadHUD("PostGame", mIsChallenge ? "HUDs/BoostBlitzChallengePostGameHUD" : "HUDs/BoostBlitzPostGameHUD");

	// load ghost
	VuArray<VUBYTE> ghostData(0);
	if ( loadRecording(ghostData) )
	{
		mpGhostCar = new VuGhostCarEntity(ghostData);
		mpGhostCar->setShortName("GhostCar");
		mpGhostCar->load(VuGameUtil::IF()->constantDB()["Games"]["BoostBlitz"]["GhostCar"]);
		mpGhostCar->postLoad();
	}
}

//*****************************************************************************
void VuBoostBlitzGame::onPreBegin()
{
	if ( mCars.size() )
	{
		VuCarEntity *pCar = mCars[0];

		// place on grid
		if ( const VuEntity *pStart = VuCarManager::IF()->getBoostBlitzStart() )
		{
			const VuMatrix &xform = pStart->getTransformComponent()->getWorldTransform();
			pCar->getTransformComponent()->setWorldTransform(xform);
		}
	}
}

//*****************************************************************************
void VuBoostBlitzGame::onEnd()
{
	// save ghost
	if ( VuCarManager::IF()->getLocalHumanCarCount() )
	{
		VuCarEntity *pCar = VuCarManager::IF()->getLocalHumanCar(0);
		VuCarStats &stats = pCar->getStats();

		if ( pCar->getHasFinished() && !stats.mDNF && !pCar->getHasCheated() )
		{
			if ( mpGhostCar == VUNULL || (stats.mTotalTime <= mpGhostCar->getPlaybackEndTime()) )
				saveRecording(mpRecorder->mData);
		}
	}

	if ( mpGhostCar )
	{
		mpGhostCar->gameRelease();
		mpGhostCar->removeRef();
		mpGhostCar = VUNULL;
	}
}

//*****************************************************************************
void VuBoostBlitzGame::onCarFinished(VuCarEntity *pCar)
{
	VuCarStats &stats = pCar->getStats();

	// record result
	if ( pCar->getDriver()->isHuman() && !stats.mDNF )
	{
		// stats
		VuStatsManager::IF()->recordBoostBlitzResult(mEventName, pCar);

		// leaderboard
		if ( !pCar->getHasCheated() )
		{
			VUINT64 lbValue = -VUINT64(stats.mTotalTime*1000);
			VuLeaderboardManager::IF()->submitScore(mEventName, lbValue);
		}
	}
}

//*****************************************************************************
void VuBoostBlitzGame::onIntroEnter()
{
	useHUD("Intro");

	for ( int i = 0; i < mCars.size(); i++ )
		mCars[i]->pushPauseControlRequest();

	VuEventManager::IF()->broadcast("OnStartCinematicIntro");
}

//*****************************************************************************
void VuBoostBlitzGame::onIntroExit()
{
	for ( int i = 0; i < mCars.size(); i++ )
		mCars[i]->popPauseControlRequest();
}

//*****************************************************************************
void VuBoostBlitzGame::onIntroTick(float fdt)
{
	mGameFSM.setCondition("IntroDone", !VuCarManager::IF()->isCameraOverrideEnabled());
}

//*****************************************************************************
void VuBoostBlitzGame::onPreGameEnter()
{
	useHUD("PreGame");

	for ( int i = 0; i < mCars.size(); i++ )
	{
		mCars[i]->pushPauseControlRequest();
		mCars[i]->getEngine()->setRandomRev(true);
	}

	VuEventManager::IF()->broadcast("OnPreGameEnter");
}

//*****************************************************************************
void VuBoostBlitzGame::onPreGameExit()
{
	for ( int i = 0; i < mCars.size(); i++ )
	{
		mCars[i]->popPauseControlRequest();
		mCars[i]->getEngine()->setRandomRev(false);
	}
}

//*****************************************************************************
void VuBoostBlitzGame::onPreGameTick(float fdt)
{
	updatePreGameTiming(mGameFSM.getTimeInState());
}

//*****************************************************************************
void VuBoostBlitzGame::onGameEnter()
{
	useHUD("Game");

	mpRecorder = new VuGhostRecorder(mCars[0]);
	mpRecorder->startRecord(BOOST_BLITZ_RECORD_MAX_TIME, BOOST_BLITZ_RECORD_TIME_STEP);

	if ( mpGhostCar )
	{
		mpGhostCar->gameInitialize();
		mpGhostCar->startPlayback();
	}
}

//*****************************************************************************
void VuBoostBlitzGame::onGameExit()
{
	mpRecorder->endRecord();
}

//*****************************************************************************
void VuBoostBlitzGame::onGameTick(float fdt)
{
	updateTiming(fdt);

	if ( VuCarManager::IF()->getLocalHumanCarCount() )
	{
		VuCarEntity *pCar = VuCarManager::IF()->getLocalHumanCar(0);
		VuCarStats &stats = pCar->getStats();

		// placing
		const VuJsonContainer &criteria = VuGameUtil::IF()->getEventData()["Criteria"];

		for ( stats.mPlace = criteria.size() + 1; stats.mPlace > 1; stats.mPlace-- )
			if ( VuTruncate(float(100*stats.mTotalTime)) > VuTruncate(100*criteria[stats.mPlace - 2].asFloat()) )
				break;

		// end condition
		mGameFSM.setCondition("Finished", pCar->getHasFinished());
	}
}

//*****************************************************************************
void VuBoostBlitzGame::onPostGameEnter()
{
	useHUD("PostGame");
	VuGameUtil::IF()->pushDisallowPauseMenu();
}

//*****************************************************************************
void VuBoostBlitzGame::onPostGameExit()
{
	VuGameUtil::IF()->popDisallowPauseMenu();
}

//*****************************************************************************
void VuBoostBlitzGame::saveRecording(const VuArray<VUBYTE> &ghostData)
{
	if ( mEventName.length() )
	{
		std::string path;
		VuProfileManager::IF()->getPath(path);
		VuFileUtil::VuRootPathPushPop rootPathPushPop(path);

		std::string fileName = "BoostBlitz_" + mEventName;
		if ( VUHANDLE hFile = VuFile::IF()->open(fileName, VuFile::MODE_WRITE) )
		{
			VUUINT32 uncompressedSize = ghostData.size();

			VuFile::IF()->write(hFile, &scBoostBlitzRecordingMagic, sizeof(scBoostBlitzRecordingMagic));
			VuFile::IF()->write(hFile, &scBoostBlitzRecordingVersion, sizeof(scBoostBlitzRecordingVersion));
			VuFile::IF()->write(hFile, &uncompressedSize, sizeof(uncompressedSize));

			VuZLibUtil::compressToFile(hFile, &ghostData[0], ghostData.size());

			VuFile::IF()->close(hFile);
		}
	}
}

//*****************************************************************************
bool VuBoostBlitzGame::loadRecording(VuArray<VUBYTE> &ghostData)
{
	bool success = false;

	if ( mEventName.length() )
	{
		// load
		std::string path;
		VuProfileManager::IF()->getPath(path);
		VuFileUtil::VuRootPathPushPop rootPathPushPop(path);

		std::string fileName = "BoostBlitz_" + mEventName;
		if ( VUHANDLE hFile = VuFile::IF()->open(fileName, VuFile::MODE_READ) )
		{
			VUUINT32 magic = 0, version = 0, uncompressedSize = 0;
			VuFile::IF()->read(hFile, &magic, sizeof(magic));
			VuFile::IF()->read(hFile, &version, sizeof(version));
			VuFile::IF()->read(hFile, &uncompressedSize, sizeof(uncompressedSize));

			if ( (scBoostBlitzRecordingMagic == magic) && (scBoostBlitzRecordingVersion == version) )
			{
				ghostData.resize(uncompressedSize);
				if ( VuZLibUtil::uncompressFromFile(hFile, VuFile::IF()->size(hFile) - 12, &ghostData[0], &uncompressedSize) )
					success = true;
			}

			VuFile::IF()->close(hFile);
		}
	}

	return success;
}
