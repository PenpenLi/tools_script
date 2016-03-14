//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  TutorialPowerUpGame class
// 
//*****************************************************************************

#include "VuTutorialPowerUpGame.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarEngine.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Ai/VuAiManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Dev/VuDevConfig.h"


IMPLEMENT_RTTI(VuTutorialPowerUpGame, VuGame);


//*****************************************************************************
VuTutorialPowerUpGame::VuTutorialPowerUpGame(VuProject *pProject):
	VuGame(pProject)
{
	// create FSM
	{
		VuFSM::VuState *pState;

		if ( !VuDevConfig::IF()->getParam("SkipIntro").asBool() )
		{
			pState = mGameFSM.addState("Intro");
			pState->setEnterMethod(this, &VuTutorialPowerUpGame::onIntroEnter);
			pState->setExitMethod(this, &VuTutorialPowerUpGame::onIntroExit);
			pState->setTickMethod(this, &VuTutorialPowerUpGame::onIntroTick);
		}

		pState = mGameFSM.addState("PreGame");
		pState->setEnterMethod(this, &VuTutorialPowerUpGame::onPreGameEnter);
		pState->setExitMethod(this, &VuTutorialPowerUpGame::onPreGameExit);
		pState->setTickMethod(this, &VuTutorialPowerUpGame::onPreGameTick);

		pState = mGameFSM.addState("Game");
		pState->setEnterMethod(this, &VuTutorialPowerUpGame::onGameEnter);
		pState->setTickMethod(this, &VuTutorialPowerUpGame::onGameTick);

		pState = mGameFSM.addState("PostGame");
		pState->setEnterMethod(this, &VuTutorialPowerUpGame::onPostGameEnter);
		pState->setExitMethod(this, &VuTutorialPowerUpGame::onPostGameExit);
		pState->setTickMethod(this, &VuTutorialPowerUpGame::onPostGameTick);

		pState = mGameFSM.addState("Exit");

		// add transitions
		mGameFSM.addTransition("Intro", "PreGame", "IntroDone");
		mGameFSM.addTransition("PreGame", "Game", "StartLightsOut");
		mGameFSM.addTransition("Game", "PostGame", "GameFinished");
		mGameFSM.addTransition("", "Exit", "Exit");
	}

	// event handlers
	REG_EVENT_HANDLER(VuTutorialPowerUpGame, OnStartCarEffect);

	mCarSpawner.configure(VuGameUtil::IF()->constantDB()["Games"]["TutorialPowerUp"]);
}

//*****************************************************************************
VuTutorialPowerUpGame::~VuTutorialPowerUpGame()
{
}

//*****************************************************************************
void VuTutorialPowerUpGame::OnStartCarEffect(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);

	VuEntity *pVictimEntity = accessor.getEntity();
	const char *effectName = accessor.getString();
	VuEntity *pOriginatorEntity = accessor.getEntity();

	if ( strcmp(effectName, "HomingMissileVictim") == 0 )
	{
		VuCarEntity *pVictim = pVictimEntity->safeCast<VuCarEntity>();
		VuCarEntity *pOriginator = pOriginatorEntity->safeCast<VuCarEntity>();
		if ( pVictim && pOriginator )
		{
			if ( pVictim->getDriver()->isAi() && pOriginator->getDriver()->isHuman() )
			{
				mGameFSM.setCondition("GameFinished", true);
			}
		}
	}
}

//*****************************************************************************
void VuTutorialPowerUpGame::onLoad(const VuJsonContainer &data)
{
	const std::string &playerCar = data["Cars"][0]["Properties"]["Car"].asString();
	const std::string &playerDriver = data["Cars"][0]["Properties"]["Driver"].asString();
	const std::string &playerDecal = data["Cars"][0]["Properties"]["Decal"].asString();

	mCarSpawner.load(playerCar, playerDriver, playerDecal);

	// load HUDs
	loadHUD("Intro", "HUDs/TutorialPowerUpIntroHUD");
	loadHUD("PreGame", "HUDs/TutorialPowerUpPreGameHUD");
	loadHUD("Game", "HUDs/TutorialPowerUpGameHUD");
	loadHUD("PostGame", "HUDs/TutorialPowerUpPostGameHUD");
}

//*****************************************************************************
void VuTutorialPowerUpGame::onPreBegin()
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
void VuTutorialPowerUpGame::onPostBegin()
{
	VuAiManager::IF()->onGameStart();

	mCarSpawner.begin();

	// init stats
	if ( mCars.size() )
	{
		VuCarEntity *pCar = mCars[0];
		pCar->getStats().mPlace = 1;
	}
}

//*****************************************************************************
void VuTutorialPowerUpGame::onEnd()
{
	mCarSpawner.end();

	VuAiManager::IF()->onRaceEnd();
	VuAiManager::IF()->reset();
}

//*****************************************************************************
void VuTutorialPowerUpGame::onTick(float fdt)
{
	mCarSpawner.update(fdt);
}

//*****************************************************************************
void VuTutorialPowerUpGame::onIntroEnter()
{
	useHUD("Intro");

	for ( int i = 0; i < mCars.size(); i++ )
		mCars[i]->pushPauseControlRequest();

	VuEventManager::IF()->broadcast("OnStartCinematicIntro");
}

//*****************************************************************************
void VuTutorialPowerUpGame::onIntroExit()
{
	for ( int i = 0; i < mCars.size(); i++ )
		mCars[i]->popPauseControlRequest();
}

//*****************************************************************************
void VuTutorialPowerUpGame::onIntroTick(float fdt)
{
	mGameFSM.setCondition("IntroDone", !VuCarManager::IF()->isCameraOverrideEnabled());
}

//*****************************************************************************
void VuTutorialPowerUpGame::onPreGameEnter()
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
void VuTutorialPowerUpGame::onPreGameExit()
{
	for ( int i = 0; i < mCars.size(); i++ )
	{
		mCars[i]->popPauseControlRequest();
		mCars[i]->getEngine()->setRandomRev(false);
	}
}

//*****************************************************************************
void VuTutorialPowerUpGame::onPreGameTick(float fdt)
{
	updatePreGameTiming(mGameFSM.getTimeInState());
}

//*****************************************************************************
void VuTutorialPowerUpGame::onGameEnter()
{
	useHUD("Game");

	VuAiManager::IF()->onRaceStart();
}

//*****************************************************************************
void VuTutorialPowerUpGame::onGameTick(float fdt)
{
	updateTiming(fdt);
}

//*****************************************************************************
void VuTutorialPowerUpGame::onPostGameEnter()
{
	useHUD("PostGame");
	VuGameUtil::IF()->pushDisallowPauseMenu();

	// finish all cars
	for ( int i = 0; i < mCars.size(); i++ )
		setCarFinished(mCars[i]);
}

//*****************************************************************************
void VuTutorialPowerUpGame::onPostGameExit()
{
	VuGameUtil::IF()->popDisallowPauseMenu();
}

//*****************************************************************************
void VuTutorialPowerUpGame::onPostGameTick(float fdt)
{
	updateTiming(fdt);
}
