//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  FollowLeaderGame class
// 
//*****************************************************************************

#include "VuFollowLeaderGame.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/VuGhostCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarEngine.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/Entities/Dynamic/VuBreadCrumbEntity.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Managers/VuStatsManager.h"
#include "VuPurple/Managers/VuLeaderboardManager.h"
#include "VuPurple/Managers/VuAchievementManager.h"
#include "VuPurple/Track/VuTrackManager.h"
#include "VuPurple/PowerUps/VuPowerUpManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuPurple/Util/VuGhostUtil.h"
#include "VuEngine/Entities/VuEntityRepository.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Assets/VuAssetFactory.h"
#include "VuEngine/Assets/VuGenericAsset.h"
#include "VuEngine/Util/VuFileUtil.h"
#include "VuEngine/Dev/VuDevConfig.h"
#include "VuEngine/Dev/VuDevHostComm.h"


IMPLEMENT_RTTI(VuFollowLeaderGame, VuGame);


//*****************************************************************************
VuFollowLeaderGame::VuFollowLeaderGame(VuProject *pProject):
	VuGame(pProject),
	mpRecorder(VUNULL),
	mpGhostCar(VUNULL),
	mBreadCrumbCount(50),
	mBreadCrumbDelay(5),
	mBreadCrumbsDropped(0),
	mTimeFactor(1.0f),
	mMaintainDistFactor(2.0f),
	mMaintainDistRange0(5.0f),
	mMaintainDistRange1(20.0f),
	mTimeOutTime(0),
	mCountdownTime(0),
	mTimeSinceLastCrumb(0)
{
	// event handlers
	REG_EVENT_HANDLER(VuFollowLeaderGame, OnRecovered);
	REG_EVENT_HANDLER(VuFollowLeaderGame, OnBreadCrumb);

	// create FSM
	{
		VuFSM::VuState *pState;

		if ( !VuDevConfig::IF()->getParam("SkipIntro").asBool() )
		{
			pState = mGameFSM.addState("Intro");
			pState->setEnterMethod(this, &VuFollowLeaderGame::onIntroEnter);
			pState->setExitMethod(this, &VuFollowLeaderGame::onIntroExit);
			pState->setTickMethod(this, &VuFollowLeaderGame::onIntroTick);
		}

		pState = mGameFSM.addState("PreGame");
		pState->setEnterMethod(this, &VuFollowLeaderGame::onPreGameEnter);
		pState->setExitMethod(this, &VuFollowLeaderGame::onPreGameExit);
		pState->setTickMethod(this, &VuFollowLeaderGame::onPreGameTick);

		pState = mGameFSM.addState("Game");
		pState->setEnterMethod(this, &VuFollowLeaderGame::onGameEnter);
		pState->setExitMethod(this, &VuFollowLeaderGame::onGameExit);
		pState->setTickMethod(this, &VuFollowLeaderGame::onGameTick);

		pState = mGameFSM.addState("PostGame");
		pState->setEnterMethod(this, &VuFollowLeaderGame::onPostGameEnter);
		pState->setExitMethod(this, &VuFollowLeaderGame::onPostGameExit);

		pState = mGameFSM.addState("Record");
		pState->setEnterMethod(this, &VuFollowLeaderGame::onRecordEnter);
		pState->setExitMethod(this, &VuFollowLeaderGame::onRecordExit);
		pState->setTickMethod(this, &VuFollowLeaderGame::onRecordTick);

		pState = mGameFSM.addState("Exit");

		// add transitions
		mGameFSM.addTransition("Intro", "PreGame", "IntroDone");
		mGameFSM.addTransition("PreGame", "Record", "StartLightsOut & Record");
		mGameFSM.addTransition("PreGame", "Game", "StartLightsOut");
		mGameFSM.addTransition("Game", "PostGame", "Finished");
		mGameFSM.addTransition("Record", "Exit", "Finished");
		mGameFSM.addTransition("", "Exit", "Exit");
	}

	VuPowerUpManager::IF()->setPowerUpsEnabled(false);
}

//*****************************************************************************
VuFollowLeaderGame::~VuFollowLeaderGame()
{
	VuPowerUpManager::IF()->setPowerUpsEnabled(true);
}

//*****************************************************************************
void VuFollowLeaderGame::onLoad(const VuJsonContainer &data)
{
	VuGenericAsset *pRecording = VuAssetFactory::IF()->createAsset<VuGenericAsset>("Recordings/" + mEventName, VuAssetFactory::OPTIONAL_ASSET);
	if ( pRecording )
	{
		mpGhostCar = new VuGhostCarEntity(pRecording->data());
		mpGhostCar->setShortName("GhostCar");
		mpGhostCar->load(VuGameUtil::IF()->constantDB()["Games"]["FollowLeader"]["GhostCar"]);
		mpGhostCar->postLoad();

		VuAssetFactory::IF()->releaseAsset(pRecording);

		const VuJsonContainer &eventData = VuGameUtil::IF()->getEventData();
		VuDataUtil::getValue(eventData["BreadCrumbCount"], mBreadCrumbCount);

		VuDataUtil::getValue(eventData["TimeFactor"], mTimeFactor);
		mpGhostCar->setTimeFactor(mTimeFactor);

		// time-out
		const VuJsonContainer &timeOutData = VuGameUtil::IF()->constantDB()["Games"]["FollowLeader"]["TimeOut"];
		VuDataUtil::getValue(timeOutData["TotalTime"], mTimeOutTime);
		VuDataUtil::getValue(timeOutData["CountdownTime"], mCountdownTime);
		VuDataUtil::getValue(timeOutData["BreadCrumbDelay"], mBreadCrumbDelay);

		// maintain distance
		const VuJsonContainer &maintainDistanceData = VuGameUtil::IF()->constantDB()["Games"]["FollowLeader"]["MaintainDistance"];
		VuDataUtil::getValue(maintainDistanceData["Factor"], mMaintainDistFactor);
		VuDataUtil::getValue(maintainDistanceData["Range"][0], mMaintainDistRange0);
		VuDataUtil::getValue(maintainDistanceData["Range"][1], mMaintainDistRange1);
	}
	else
	{
		mGameFSM.setCondition("Record", true);
	}

	// load HUDs
	loadHUD("Intro", "HUDs/FollowLeaderIntroHUD");
	loadHUD("PreGame", "HUDs/FollowLeaderPreGameHUD");
	loadHUD("Game", "HUDs/FollowLeaderGameHUD");
	loadHUD("PostGame", mIsChallenge ? "HUDs/FollowLeaderChallengePostGameHUD" : "HUDs/FollowLeaderPostGameHUD");
}

//*****************************************************************************
void VuFollowLeaderGame::onPreBegin()
{
	if ( mCars.size() )
	{
		VuCarEntity *pCar = mCars[0];

		int gridPosition = mpGhostCar ? 4 : 0;

		// place on grid
		if ( const VuEntity *pStart = VuCarManager::IF()->getGridPosition(gridPosition) )
		{
			VuTransformComponent *pGridTC = pStart->getTransformComponent();
			VuMatrix xform;
			VuGameUtil::buildSafeMatrixForCar(pGridTC->getWorldPosition(), pGridTC->getWorldTransform().getAxisY(), pCar->getCollisionAabb(), xform, true);
			pCar->getTransformComponent()->setWorldTransform(xform);
		}
	}

	if ( mpGhostCar )
		mpGhostCar->gameInitialize();
}

//*****************************************************************************
void VuFollowLeaderGame::onEnd()
{
	if ( mpGhostCar )
	{
		mpGhostCar->gameRelease();
		mpGhostCar->removeRef();
		mpGhostCar = VUNULL;
	}
}

//*****************************************************************************
void VuFollowLeaderGame::onCarFinished(VuCarEntity *pCar)
{
	VuCarStats &stats = pCar->getStats();

	// record result
	if ( pCar->getDriver()->isHuman() && !stats.mDNF )
	{
		// stats
		VuStatsManager::IF()->recordFollowLeaderResult(mEventName, pCar);

		// achievements
		if ( stats.mPlace == 1 )
		{
			if ( stats.mScore == mBreadCrumbCount )
				VuAchievementManager::IF()->unlock(34); // Collect every breadcrumb in a Follow the Leader event
		}

		// leaderboard
		if ( !pCar->getHasCheated() )
			VuLeaderboardManager::IF()->submitScore(mEventName, stats.mScore);
	}
}

//*****************************************************************************
void VuFollowLeaderGame::OnRecovered(const VuParams& params)
{
	VuParams::VuAccessor data(params);

	VuEntity* pEntity = data.getEntity();
	if ( pEntity && pEntity->isDerivedFrom(VuCarEntity::msRTTI) )
	{
		VuCarEntity* pCar = static_cast<VuCarEntity*>(pEntity);
		if ( pCar->getDriver()->isHuman() )
		{
			setCarFinished(pCar);
		}
	}
}

//*****************************************************************************
void VuFollowLeaderGame::OnBreadCrumb(const VuParams& params)
{
	mTimeSinceLastCrumb = 0.0f;
}

//*****************************************************************************
void VuFollowLeaderGame::onIntroEnter()
{
	useHUD("Intro");

	for ( int i = 0; i < mCars.size(); i++ )
		mCars[i]->pushPauseControlRequest();

	VuEventManager::IF()->broadcast("OnStartCinematicIntro");
}

//*****************************************************************************
void VuFollowLeaderGame::onIntroExit()
{
	for ( int i = 0; i < mCars.size(); i++ )
		mCars[i]->popPauseControlRequest();
}

//*****************************************************************************
void VuFollowLeaderGame::onIntroTick(float fdt)
{
	mGameFSM.setCondition("IntroDone", !VuCarManager::IF()->isCameraOverrideEnabled());
}

//*****************************************************************************
void VuFollowLeaderGame::onPreGameEnter()
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
void VuFollowLeaderGame::onPreGameExit()
{
	for ( int i = 0; i < mCars.size(); i++ )
	{
		mCars[i]->popPauseControlRequest();
		mCars[i]->getEngine()->setRandomRev(false);
	}
}

//*****************************************************************************
void VuFollowLeaderGame::onPreGameTick(float fdt)
{
	updatePreGameTiming(mGameFSM.getTimeInState());
}

//*****************************************************************************
void VuFollowLeaderGame::onGameEnter()
{
	useHUD("Game");

	mpGhostCar->startPlayback();
}

//*****************************************************************************
void VuFollowLeaderGame::onGameExit()
{
}

//*****************************************************************************
void VuFollowLeaderGame::onGameTick(float fdt)
{
	updateTiming(fdt);

	// finished?
	if ( VuCarManager::IF()->getLocalHumanCarCount() )
	{
		VuCarEntity *pCar = VuCarManager::IF()->getLocalHumanCar(0);
		VuCarStats &stats = pCar->getStats();

		// placing
		const VuJsonContainer &criteria = VuGameUtil::IF()->getEventData()["Criteria"];

		for ( stats.mPlace = criteria.size() + 1; stats.mPlace > 1; stats.mPlace-- )
			if ( stats.mScore < criteria[stats.mPlace - 2].asInt() )
				break;

		// bread crumb timer
		if ( mTimeOutTime > 0.0f )
		{
			float timeRemaining = mTimeOutTime - mTimeSinceLastCrumb;
			if ( timeRemaining <= 0.0f )
				setCarFinished(pCar);

			if ( timeRemaining < mCountdownTime )
				stats.mGameTimer = VuTruncate(VuCeil(timeRemaining));
			else
				stats.mGameTimer = -1;
		}
		mTimeSinceLastCrumb += fdt;

		// end condition
		mGameFSM.setCondition("Finished", pCar->getHasFinished());
	}

	// drop bread crumbs
	float t = (mpGhostCar->getPlaybackTime() - mBreadCrumbDelay)/(mpGhostCar->getPlaybackEndTime() - mBreadCrumbDelay);
	t = VuClamp(t, 0.0f, 1.0f);
	int targetCount = VuRound(mBreadCrumbCount*t);
	if ( targetCount > mBreadCrumbsDropped )
	{
		const VuJsonContainer &breadCrumbData = VuGameUtil::IF()->constantDB()["Games"]["FollowLeader"]["BreadCrumbs"];
		const VuMatrix &transform = mpGhostCar->getTransformComponent()->getWorldTransform();
		VuEntityRepository::IF()->addManagedEntity(new VuBreadCrumbEntity(breadCrumbData, VuGameUtil::IF()->eventDB()[mEventName], transform, mBreadCrumbsDropped));

		mBreadCrumbsDropped++;
	}

	// time factor
	if ( VuCarManager::IF()->getLocalHumanCarCount() )
	{
		VuCarEntity *pCar = VuCarManager::IF()->getLocalHumanCar(0);

		float dist = VuDist(pCar->getModelPosition(), mpGhostCar->getTransformComponent()->getWorldPosition());
		float factor = VuLerp(mMaintainDistFactor, 1.0f, VuLinStep(mMaintainDistRange0, mMaintainDistRange1, dist));
		mpGhostCar->setTimeFactor(mTimeFactor*factor);
	}
}

//*****************************************************************************
void VuFollowLeaderGame::onPostGameEnter()
{
	useHUD("PostGame");
	VuGameUtil::IF()->pushDisallowPauseMenu();
}

//*****************************************************************************
void VuFollowLeaderGame::onPostGameExit()
{
	VuGameUtil::IF()->popDisallowPauseMenu();
}

//*****************************************************************************
void VuFollowLeaderGame::onRecordEnter()
{
	useHUD("");

	mpRecorder = new VuGhostRecorder(mCars[0]);
	mpRecorder->startRecord(FLT_MAX, 0.0f);
}

//*****************************************************************************
void VuFollowLeaderGame::onRecordExit()
{
	mpRecorder->endRecord();

	VuFileUtil::saveFile("recording.bin", &mpRecorder->mData[0], mpRecorder->mData.size());

	delete mpRecorder;
	mpRecorder = VUNULL;
}

//*****************************************************************************
void VuFollowLeaderGame::onRecordTick(float fdt)
{
	updateTiming(fdt);

	// finished?
	mGameFSM.setCondition("Finished", mCars[0]->getHasFinished());
}
