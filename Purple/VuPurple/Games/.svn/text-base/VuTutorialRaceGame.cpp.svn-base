//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  TutorialRaceGame class
// 
//*****************************************************************************

#include "VuTutorialRaceGame.h"
#include "VuPurple/Ai/VuAiManager.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarEngine.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Dev/VuDevConfig.h"


IMPLEMENT_RTTI(VuTutorialRaceGame, VuGame);


//*****************************************************************************
VuTutorialRaceGame::VuTutorialRaceGame(VuProject *pProject):
	VuGame(pProject)
{
	// create FSM
	{
		VuFSM::VuState *pState;

		if ( !VuDevConfig::IF()->getParam("SkipIntro").asBool() )
		{
			pState = mGameFSM.addState("Intro");
			pState->setEnterMethod(this, &VuTutorialRaceGame::onIntroEnter);
			pState->setExitMethod(this, &VuTutorialRaceGame::onIntroExit);
			pState->setTickMethod(this, &VuTutorialRaceGame::onIntroTick);
		}

		pState = mGameFSM.addState("PreGame");
		pState->setEnterMethod(this, &VuTutorialRaceGame::onPreGameEnter);
		pState->setExitMethod(this, &VuTutorialRaceGame::onPreGameExit);
		pState->setTickMethod(this, &VuTutorialRaceGame::onPreGameTick);

		pState = mGameFSM.addState("Game");
		pState->setEnterMethod(this, &VuTutorialRaceGame::onGameEnter);
		pState->setTickMethod(this, &VuTutorialRaceGame::onGameTick);

		pState = mGameFSM.addState("PostGame");
		pState->setEnterMethod(this, &VuTutorialRaceGame::onPostGameEnter);
		pState->setExitMethod(this, &VuTutorialRaceGame::onPostGameExit);
		pState->setTickMethod(this, &VuTutorialRaceGame::onPostGameTick);

		pState = mGameFSM.addState("Exit");

		// add transitions
		mGameFSM.addTransition("Intro", "PreGame", "IntroDone");
		mGameFSM.addTransition("PreGame", "Game", "StartLightsOut");
		mGameFSM.addTransition("Game", "PostGame", "GameFinished");
		mGameFSM.addTransition("", "Exit", "Exit");
	}
}

//*****************************************************************************
VuTutorialRaceGame::~VuTutorialRaceGame()
{
}

//*****************************************************************************
void VuTutorialRaceGame::onLoad(const VuJsonContainer &data)
{
	// load HUDs
	loadHUD("Intro", "HUDs/TutorialRaceIntroHUD");
	loadHUD("PreGame", "HUDs/TutorialRacePreGameHUD");
	loadHUD("Game", "HUDs/TutorialRaceGameHUD");
	loadHUD("PostGame", "HUDs/TutorialRacePostGameHUD");
}

//*****************************************************************************
void VuTutorialRaceGame::onPreBegin()
{
	for ( int i = 0; i < mCars.size(); i++ )
	{
		VuCarEntity *pCar = mCars[i];

		// place on grid
		if ( i < VuCarManager::IF()->getGridPositionCount() )
		{
			VuTransformComponent *pGridTC = VuCarManager::IF()->getGridPosition(i)->getTransformComponent();
			VuMatrix xform;
			VuGameUtil::buildSafeMatrixForCar(pGridTC->getWorldPosition(), pGridTC->getWorldTransform().getAxisY(), pCar->getCollisionAabb(), xform, true);
			pCar->getTransformComponent()->setWorldTransform(xform);
		}
	}
}

//*****************************************************************************
void VuTutorialRaceGame::onPostBegin()
{
	if ( VuAiManager::IF()->canRace() )
	{
		for ( int i = 0; i < VuCarManager::IF()->getAiCarCount(); i++ )
			VuAiManager::IF()->addToRace(VuCarManager::IF()->getAiCar(i), 0.0f, 0.0f);

		VuAiManager::IF()->onGameStart();
	}

	// init race stats
	mPlacing.resize(mCars.size());

	for ( int i = 0; i < mCars.size(); i++ )
	{
		VuCarStats *pRaceStats = &mCars[i]->getStats();

		pRaceStats->mPlace = i + 1;
		mPlacing[i] = i;
	}
}

//*****************************************************************************
void VuTutorialRaceGame::onEnd()
{
	VuAiManager::IF()->onRaceEnd();
}

//*****************************************************************************
void VuTutorialRaceGame::onCarFinished(VuCarEntity *pCar)
{
	VuAiManager::IF()->removeFromRace(pCar);
}

//*****************************************************************************
void VuTutorialRaceGame::onIntroEnter()
{
	useHUD("Intro");

	for ( int i = 0; i < mCars.size(); i++ )
		mCars[i]->pushPauseControlRequest();

	VuEventManager::IF()->broadcast("OnStartCinematicIntro");
}

//*****************************************************************************
void VuTutorialRaceGame::onIntroExit()
{
	for ( int i = 0; i < mCars.size(); i++ )
		mCars[i]->popPauseControlRequest();
}

//*****************************************************************************
void VuTutorialRaceGame::onIntroTick(float fdt)
{
	mGameFSM.setCondition("IntroDone", !VuCarManager::IF()->isCameraOverrideEnabled());
}

//*****************************************************************************
void VuTutorialRaceGame::onPreGameEnter()
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
void VuTutorialRaceGame::onPreGameExit()
{
	for ( int i = 0; i < mCars.size(); i++ )
	{
		mCars[i]->popPauseControlRequest();
		mCars[i]->getEngine()->setRandomRev(false);
	}
}

//*****************************************************************************
void VuTutorialRaceGame::onPreGameTick(float fdt)
{
	updatePreGameTiming(mGameFSM.getTimeInState());
}

//*****************************************************************************
void VuTutorialRaceGame::onGameEnter()
{
	useHUD("Game");

	VuAiManager::IF()->onRaceStart();
}

//*****************************************************************************
void VuTutorialRaceGame::onGameTick(float fdt)
{
	updateTiming(fdt);
	updatePlacing();

	// did racer finish?
	bool bRacersFinished = true;
	for ( int i = 0; i < mCars.size(); i++ )
		if ( mCars[i]->getIsRacer() )
			bRacersFinished &= mCars[i]->getHasFinished();

	mGameFSM.setCondition("GameFinished", bRacersFinished);
}

//*****************************************************************************
void VuTutorialRaceGame::onPostGameEnter()
{
	useHUD("PostGame");
	VuGameUtil::IF()->pushDisallowPauseMenu();

	// finish all cars
	for ( int i = 0; i < mCars.size(); i++ )
		setCarFinished(mCars[i]);
}

//*****************************************************************************
void VuTutorialRaceGame::onPostGameExit()
{
	VuGameUtil::IF()->popDisallowPauseMenu();
}

//*****************************************************************************
void VuTutorialRaceGame::onPostGameTick(float fdt)
{
	updateTiming(fdt);
	updatePlacing();
}

//*****************************************************************************
void VuTutorialRaceGame::updatePlacing()
{
	std::sort(mPlacing.begin(), mPlacing.end(), VuPlacingComp(mCars));

	for ( int i = 0; i < mCars.size(); i++ )
	{
		int place = i + 1;
		mCars[mPlacing[i]]->getStats().mPlace = place;
	}
}

//*****************************************************************************
bool VuTutorialRaceGame::VuPlacingComp::operator()(int i0, int i1)
{
	bool bFinished0 = mCars[i0]->getHasFinished();
	bool bFinished1 = mCars[i1]->getHasFinished();
	const VuCarStats &stats0 = mCars[i0]->getStats();
	const VuCarStats &stats1 = mCars[i1]->getStats();

	if ( stats0.mDNF )
		return false;

	if ( stats1.mDNF )
		return true;

	// If they have both finished, which one finished in least total time
	if ( bFinished0 && bFinished1 )
		return stats0.mTotalTime < stats1.mTotalTime;

	// If only one has finished, which one
	if ( bFinished0 )
		return true;

	if ( bFinished1 )
		return false;

	// If neither have finished, which one has completed the most laps
	if ( stats0.mCurLap != stats1.mCurLap )
		return stats0.mCurLap > stats1.mCurLap;

	// If both have completed the same number of laps, which one has the least distance to go on their current lap?
	return stats0.mDistToEndOfLap < stats1.mDistToEndOfLap;
}