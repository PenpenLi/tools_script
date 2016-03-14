//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  DemolitionDerbyGame class
// 
//*****************************************************************************

#include "VuDemolitionDerbyGame.h"
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


IMPLEMENT_RTTI(VuDemolitionDerbyGame, VuGame);


//*****************************************************************************
VuDemolitionDerbyGame::VuDemolitionDerbyGame(VuProject *pProject):
	VuGame(pProject),
	mTimeLimit(60)
{
	// create FSM
	{
		VuFSM::VuState *pState;

		if ( !VuDevConfig::IF()->getParam("SkipIntro").asBool() )
		{
			pState = mGameFSM.addState("Intro");
			pState->setEnterMethod(this, &VuDemolitionDerbyGame::onIntroEnter);
			pState->setExitMethod(this, &VuDemolitionDerbyGame::onIntroExit);
			pState->setTickMethod(this, &VuDemolitionDerbyGame::onIntroTick);
		}

		pState = mGameFSM.addState("PreGame");
		pState->setEnterMethod(this, &VuDemolitionDerbyGame::onPreGameEnter);
		pState->setExitMethod(this, &VuDemolitionDerbyGame::onPreGameExit);
		pState->setTickMethod(this, &VuDemolitionDerbyGame::onPreGameTick);

		pState = mGameFSM.addState("Game");
		pState->setEnterMethod(this, &VuDemolitionDerbyGame::onGameEnter);
		pState->setTickMethod(this, &VuDemolitionDerbyGame::onGameTick);

		pState = mGameFSM.addState("PostGame");
		pState->setEnterMethod(this, &VuDemolitionDerbyGame::onPostGameEnter);
		pState->setExitMethod(this, &VuDemolitionDerbyGame::onPostGameExit);

		pState = mGameFSM.addState("Exit");

		// add transitions
		mGameFSM.addTransition("Intro", "PreGame", "IntroDone");
		mGameFSM.addTransition("PreGame", "Game", "StartLightsOut");
		mGameFSM.addTransition("Game", "PostGame", "Finished");
		mGameFSM.addTransition("", "Exit", "Exit");
	}

	mCarSpawner.configure(VuGameUtil::IF()->constantDB()["Games"]["DemolitionDerby"]);
}

//*****************************************************************************
VuDemolitionDerbyGame::~VuDemolitionDerbyGame()
{
}

//*****************************************************************************
void VuDemolitionDerbyGame::onLoad(const VuJsonContainer &data)
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
	loadHUD("Intro", "HUDs/DemolitionDerbyIntroHUD");
	loadHUD("PreGame", "HUDs/DemolitionDerbyPreGameHUD");
	loadHUD("Game", "HUDs/DemolitionDerbyGameHUD");
	loadHUD("PostGame", mIsChallenge ? "HUDs/DemolitionDerbyChallengePostGameHUD" : "HUDs/DemolitionDerbyPostGameHUD");
}

//*****************************************************************************
void VuDemolitionDerbyGame::onPreBegin()
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
void VuDemolitionDerbyGame::onPostBegin()
{
	VuAiManager::IF()->onGameStart();

	mCarSpawner.begin();
}

//*****************************************************************************
void VuDemolitionDerbyGame::onEnd()
{
	mCarSpawner.end();

	VuAiManager::IF()->onRaceEnd();
	VuAiManager::IF()->reset();
}

//*****************************************************************************
void VuDemolitionDerbyGame::onCarFinished(VuCarEntity *pCar)
{
	VuCarStats &stats = pCar->getStats();

	// record result
	if ( pCar->getDriver()->isHuman() && !stats.mDNF )
	{
		// stats
		VuStatsManager::IF()->recordDemolitionDerbyResult(mEventName, pCar);

		// leaderboard
		if ( !pCar->getHasCheated() )
			VuLeaderboardManager::IF()->submitScore(mEventName, stats.mScore);
	}
}

//*****************************************************************************
void VuDemolitionDerbyGame::onTick(float fdt)
{
	mCarSpawner.update(fdt);
}

//*****************************************************************************
void VuDemolitionDerbyGame::onIntroEnter()
{
	useHUD("Intro");

	for ( int i = 0; i < mCars.size(); i++ )
		mCars[i]->pushPauseControlRequest();

	VuEventManager::IF()->broadcast("OnStartCinematicIntro");
}

//*****************************************************************************
void VuDemolitionDerbyGame::onIntroExit()
{
	for ( int i = 0; i < mCars.size(); i++ )
		mCars[i]->popPauseControlRequest();
}

//*****************************************************************************
void VuDemolitionDerbyGame::onIntroTick(float fdt)
{
	mGameFSM.setCondition("IntroDone", !VuCarManager::IF()->isCameraOverrideEnabled());
}

//*****************************************************************************
void VuDemolitionDerbyGame::onPreGameEnter()
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
void VuDemolitionDerbyGame::onPreGameExit()
{
	for ( int i = 0; i < mCars.size(); i++ )
	{
		mCars[i]->popPauseControlRequest();
		mCars[i]->getEngine()->setRandomRev(false);
	}
}

//*****************************************************************************
void VuDemolitionDerbyGame::onPreGameTick(float fdt)
{
	updatePreGameTiming(mGameFSM.getTimeInState());
}

//*****************************************************************************
void VuDemolitionDerbyGame::onGameEnter()
{
	useHUD("Game");

	VuAiManager::IF()->onRaceStart();
}

//*****************************************************************************
void VuDemolitionDerbyGame::onGameTick(float fdt)
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
void VuDemolitionDerbyGame::onPostGameEnter()
{
	useHUD("PostGame");
	VuGameUtil::IF()->pushDisallowPauseMenu();
}

//*****************************************************************************
void VuDemolitionDerbyGame::onPostGameExit()
{
	VuGameUtil::IF()->popDisallowPauseMenu();
}
