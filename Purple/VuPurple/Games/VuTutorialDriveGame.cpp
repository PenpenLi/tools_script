//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  TutorialDriveGame class
// 
//*****************************************************************************

#include "VuTutorialDriveGame.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarEngine.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Dev/VuDevConfig.h"


IMPLEMENT_RTTI(VuTutorialDriveGame, VuGame);


//*****************************************************************************
VuTutorialDriveGame::VuTutorialDriveGame(VuProject *pProject):
	VuGame(pProject)
{
	// create FSM
	{
		VuFSM::VuState *pState;

		if ( !VuDevConfig::IF()->getParam("SkipIntro").asBool() )
		{
			pState = mGameFSM.addState("Intro");
			pState->setEnterMethod(this, &VuTutorialDriveGame::onIntroEnter);
			pState->setExitMethod(this, &VuTutorialDriveGame::onIntroExit);
			pState->setTickMethod(this, &VuTutorialDriveGame::onIntroTick);
		}

		pState = mGameFSM.addState("PreGame");
		pState->setEnterMethod(this, &VuTutorialDriveGame::onPreGameEnter);
		pState->setExitMethod(this, &VuTutorialDriveGame::onPreGameExit);
		pState->setTickMethod(this, &VuTutorialDriveGame::onPreGameTick);

		pState = mGameFSM.addState("Game");
		pState->setEnterMethod(this, &VuTutorialDriveGame::onGameEnter);
		pState->setTickMethod(this, &VuTutorialDriveGame::onGameTick);

		pState = mGameFSM.addState("PostGame");
		pState->setEnterMethod(this, &VuTutorialDriveGame::onPostGameEnter);
		pState->setExitMethod(this, &VuTutorialDriveGame::onPostGameExit);
		pState->setTickMethod(this, &VuTutorialDriveGame::onPostGameTick);

		pState = mGameFSM.addState("Exit");

		// add transitions
		mGameFSM.addTransition("Intro", "PreGame", "IntroDone");
		mGameFSM.addTransition("PreGame", "Game", "StartLightsOut");
		mGameFSM.addTransition("Game", "PostGame", "GameFinished");
		mGameFSM.addTransition("", "Exit", "Exit");
	}
}

//*****************************************************************************
VuTutorialDriveGame::~VuTutorialDriveGame()
{
}

//*****************************************************************************
void VuTutorialDriveGame::onLoad(const VuJsonContainer &data)
{
	// load HUDs
	loadHUD("Intro", "HUDs/TutorialDriveIntroHUD");
	loadHUD("PreGame", "HUDs/TutorialDrivePreGameHUD");
	loadHUD("Game", "HUDs/TutorialDriveGameHUD");
	loadHUD("PostGame", "HUDs/TutorialDrivePostGameHUD");
}

//*****************************************************************************
void VuTutorialDriveGame::onPreBegin()
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
void VuTutorialDriveGame::onPostBegin()
{
	// init stats
	if ( mCars.size() )
	{
		VuCarEntity *pCar = mCars[0];
		pCar->getStats().mPlace = 1;
	}
}

//*****************************************************************************
void VuTutorialDriveGame::onIntroEnter()
{
	useHUD("Intro");

	for ( int i = 0; i < mCars.size(); i++ )
		mCars[i]->pushPauseControlRequest();

	VuEventManager::IF()->broadcast("OnStartCinematicIntro");
}

//*****************************************************************************
void VuTutorialDriveGame::onIntroExit()
{
	for ( int i = 0; i < mCars.size(); i++ )
		mCars[i]->popPauseControlRequest();
}

//*****************************************************************************
void VuTutorialDriveGame::onIntroTick(float fdt)
{
	mGameFSM.setCondition("IntroDone", !VuCarManager::IF()->isCameraOverrideEnabled());
}

//*****************************************************************************
void VuTutorialDriveGame::onPreGameEnter()
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
void VuTutorialDriveGame::onPreGameExit()
{
	for ( int i = 0; i < mCars.size(); i++ )
	{
		mCars[i]->popPauseControlRequest();
		mCars[i]->getEngine()->setRandomRev(false);
	}
}

//*****************************************************************************
void VuTutorialDriveGame::onPreGameTick(float fdt)
{
	updatePreGameTiming(mGameFSM.getTimeInState());
}

//*****************************************************************************
void VuTutorialDriveGame::onGameEnter()
{
	useHUD("Game");
}

//*****************************************************************************
void VuTutorialDriveGame::onGameTick(float fdt)
{
	updateTiming(fdt);

	// did racer finish?
	bool bRacersFinished = true;
	for ( int i = 0; i < mCars.size(); i++ )
		if ( mCars[i]->getIsRacer() )
			bRacersFinished &= mCars[i]->getHasFinished();

	mGameFSM.setCondition("GameFinished", bRacersFinished);
}

//*****************************************************************************
void VuTutorialDriveGame::onPostGameEnter()
{
	useHUD("PostGame");
	VuGameUtil::IF()->pushDisallowPauseMenu();

	// finish all cars
	for ( int i = 0; i < mCars.size(); i++ )
		setCarFinished(mCars[i]);
}

//*****************************************************************************
void VuTutorialDriveGame::onPostGameExit()
{
	VuGameUtil::IF()->popDisallowPauseMenu();
}

//*****************************************************************************
void VuTutorialDriveGame::onPostGameTick(float fdt)
{
	updateTiming(fdt);
}
