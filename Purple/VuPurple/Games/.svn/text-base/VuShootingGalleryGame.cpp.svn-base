//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  ShootingGalleryGame class
// 
//*****************************************************************************

#include "VuShootingGalleryGame.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarEngine.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Managers/VuStatsManager.h"
#include "VuPurple/Managers/VuLeaderboardManager.h"
#include "VuPurple/Ai/VuAiManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Dev/VuDevConfig.h"


IMPLEMENT_RTTI(VuShootingGalleryGame, VuGame);


//*****************************************************************************
VuShootingGalleryGame::VuShootingGalleryGame(VuProject *pProject):
	VuGame(pProject),
	mTimeLimit(60)
{
	// create FSM
	{
		VuFSM::VuState *pState;

		if ( !VuDevConfig::IF()->getParam("SkipIntro").asBool() )
		{
			pState = mGameFSM.addState("Intro");
			pState->setEnterMethod(this, &VuShootingGalleryGame::onIntroEnter);
			pState->setExitMethod(this, &VuShootingGalleryGame::onIntroExit);
			pState->setTickMethod(this, &VuShootingGalleryGame::onIntroTick);
		}

		pState = mGameFSM.addState("PreGame");
		pState->setEnterMethod(this, &VuShootingGalleryGame::onPreGameEnter);
		pState->setExitMethod(this, &VuShootingGalleryGame::onPreGameExit);
		pState->setTickMethod(this, &VuShootingGalleryGame::onPreGameTick);

		pState = mGameFSM.addState("Game");
		pState->setEnterMethod(this, &VuShootingGalleryGame::onGameEnter);
		pState->setTickMethod(this, &VuShootingGalleryGame::onGameTick);

		pState = mGameFSM.addState("PostGame");
		pState->setEnterMethod(this, &VuShootingGalleryGame::onPostGameEnter);
		pState->setExitMethod(this, &VuShootingGalleryGame::onPostGameExit);

		pState = mGameFSM.addState("Exit");

		// add transitions
		mGameFSM.addTransition("Intro", "PreGame", "IntroDone");
		mGameFSM.addTransition("PreGame", "Game", "StartLightsOut");
		mGameFSM.addTransition("Game", "PostGame", "Finished");
		mGameFSM.addTransition("", "Exit", "Exit");
	}

	mCarSpawner.configure(VuGameUtil::IF()->constantDB()["Games"]["ShootingGallery"]);
}

//*****************************************************************************
VuShootingGalleryGame::~VuShootingGalleryGame()
{
}

//*****************************************************************************
void VuShootingGalleryGame::onLoad(const VuJsonContainer &data)
{
	const std::string &playerCar = data["Cars"][0]["Properties"]["Car"].asString();
	const std::string &playerDriver = data["Cars"][0]["Properties"]["Driver"].asString();
	const std::string &playerDecal = data["Cars"][0]["Properties"]["Decal"].asString();

	mCarSpawner.load(playerCar, playerDriver, playerDecal);

	data["TimeLimit"].getValue(mTimeLimit);

	// init stats
	for ( int i = 0; i < mCars.size(); i++ )
		mCars[i]->getStats().mGameTimer = mTimeLimit;

	// load HUDs
	loadHUD("Intro", "HUDs/ShootingGalleryIntroHUD");
	loadHUD("PreGame", "HUDs/ShootingGalleryPreGameHUD");
	loadHUD("Game", "HUDs/ShootingGalleryGameHUD");
	loadHUD("PostGame", mIsChallenge ? "HUDs/ShootingGalleryChallengePostGameHUD" : "HUDs/ShootingGalleryPostGameHUD");
}

//*****************************************************************************
void VuShootingGalleryGame::onPreBegin()
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
void VuShootingGalleryGame::onPostBegin()
{
	VuAiManager::IF()->onGameStart();

	mCarSpawner.begin();
}

//*****************************************************************************
void VuShootingGalleryGame::onEnd()
{
	mCarSpawner.end();

	VuAiManager::IF()->onRaceEnd();
	VuAiManager::IF()->reset();
}

//*****************************************************************************
void VuShootingGalleryGame::onCarFinished(VuCarEntity *pCar)
{
	VuCarStats &stats = pCar->getStats();

	if ( pCar->getDriver()->isHuman() && !stats.mDNF )
	{
		// stats
		VuStatsManager::IF()->recordShootingGalleryResult(mEventName, pCar);

		// leaderboard
		if ( !pCar->getHasCheated() )
			VuLeaderboardManager::IF()->submitScore(mEventName, stats.mScore);
	}
}

//*****************************************************************************
void VuShootingGalleryGame::onTick(float fdt)
{
	mCarSpawner.update(fdt);
}

//*****************************************************************************
void VuShootingGalleryGame::onIntroEnter()
{
	useHUD("Intro");

	for ( int i = 0; i < mCars.size(); i++ )
		mCars[i]->pushPauseControlRequest();

	VuEventManager::IF()->broadcast("OnStartCinematicIntro");
}

//*****************************************************************************
void VuShootingGalleryGame::onIntroExit()
{
	for ( int i = 0; i < mCars.size(); i++ )
		mCars[i]->popPauseControlRequest();
}

//*****************************************************************************
void VuShootingGalleryGame::onIntroTick(float fdt)
{
	mGameFSM.setCondition("IntroDone", !VuCarManager::IF()->isCameraOverrideEnabled());
}

//*****************************************************************************
void VuShootingGalleryGame::onPreGameEnter()
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
void VuShootingGalleryGame::onPreGameExit()
{
	for ( int i = 0; i < mCars.size(); i++ )
	{
		mCars[i]->popPauseControlRequest();
		mCars[i]->getEngine()->setRandomRev(false);
	}
}

//*****************************************************************************
void VuShootingGalleryGame::onPreGameTick(float fdt)
{
	updatePreGameTiming(mGameFSM.getTimeInState());
}

//*****************************************************************************
void VuShootingGalleryGame::onGameEnter()
{
	useHUD("Game");

	VuAiManager::IF()->onRaceStart();
}

//*****************************************************************************
void VuShootingGalleryGame::onGameTick(float fdt)
{
	updateTiming(fdt);

	if ( VuCarManager::IF()->getLocalHumanCarCount() )
	{
		VuCarEntity *pCar = VuCarManager::IF()->getLocalHumanCar(0);
		VuCarStats &stats = pCar->getStats();

		// game-specific timing
		stats.mGameTimer = VuCeilInt(VuMax(mTimeLimit - mGameFSM.getTimeInState(), 0.0f));
		if ( stats.mGameTimer <= 0 )
			setCarFinished(pCar);

		// placing
		const VuJsonContainer &criteria = VuGameUtil::IF()->getEventData()["Criteria"];

		for ( stats.mPlace = criteria.size() + 1; stats.mPlace > 1; stats.mPlace-- )
			if ( stats.mScore < criteria[stats.mPlace - 2].asInt() )
				break;

		// end condition
		mGameFSM.setCondition("Finished", pCar->getHasFinished());
	}
}

//*****************************************************************************
void VuShootingGalleryGame::onPostGameEnter()
{
	useHUD("PostGame");
	VuGameUtil::IF()->pushDisallowPauseMenu();
}

//*****************************************************************************
void VuShootingGalleryGame::onPostGameExit()
{
	VuGameUtil::IF()->popDisallowPauseMenu();
}
