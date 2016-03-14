//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  BossBattleGame class
// 
//*****************************************************************************

#include "VuBossBattleGame.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarEngine.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/Ai/VuAiManager.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Managers/VuStatsManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Dev/VuDevConfig.h"


IMPLEMENT_RTTI(VuBossBattleGame, VuGame);


//*****************************************************************************
VuBossBattleGame::VuBossBattleGame(VuProject *pProject):
	VuGame(pProject)
{
	// create FSM
	{
		VuFSM::VuState *pState;

		if ( !VuDevConfig::IF()->getParam("SkipIntro").asBool() )
		{
			pState = mGameFSM.addState("Intro");
			pState->setEnterMethod(this, &VuBossBattleGame::onIntroEnter);
			pState->setExitMethod(this, &VuBossBattleGame::onIntroExit);
			pState->setTickMethod(this, &VuBossBattleGame::onIntroTick);
		}

		pState = mGameFSM.addState("PreGame");
		pState->setEnterMethod(this, &VuBossBattleGame::onPreGameEnter);
		pState->setExitMethod(this, &VuBossBattleGame::onPreGameExit);
		pState->setTickMethod(this, &VuBossBattleGame::onPreGameTick);

		pState = mGameFSM.addState("Game");
		pState->setEnterMethod(this, &VuBossBattleGame::onGameEnter);
		pState->setTickMethod(this, &VuBossBattleGame::onGameTick);

		pState = mGameFSM.addState("PostGame");
		pState->setEnterMethod(this, &VuBossBattleGame::onPostGameEnter);
		pState->setExitMethod(this, &VuBossBattleGame::onPostGameExit);
		pState->setTickMethod(this, &VuBossBattleGame::onPostGameTick);

		pState = mGameFSM.addState("Exit");

		// add transitions
		mGameFSM.addTransition("Intro", "PreGame", "IntroDone");
		mGameFSM.addTransition("PreGame", "Game", "StartLightsOut");
		mGameFSM.addTransition("Game", "PostGame", "RacersFinished");
		mGameFSM.addTransition("", "Exit", "Exit");
	}
}

//*****************************************************************************
VuBossBattleGame::~VuBossBattleGame()
{
}

//*****************************************************************************
void VuBossBattleGame::onLoad(const VuJsonContainer &data)
{
	// load HUDs
	loadHUD("Intro", "HUDs/BossBattleIntroHUD");
	loadHUD("PreGame", "HUDs/BossBattlePreGameHUD");
	loadHUD("Game", "HUDs/BossBattleGameHUD");
	loadHUD("PostGame", "HUDs/BossBattlePostGameHUD");
}

//*****************************************************************************
void VuBossBattleGame::onPreBegin()
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
void VuBossBattleGame::onPostBegin()
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

	// Now check to see how many times the player has lost to this boss
	if (VuCarManager::IF()->getAiCarCount() > 0)
	{
		VuCarEntity* pCar = VuCarManager::IF()->getAiCar(0);
		if (pCar)
		{
			const std::string& driverName = pCar->getDriverName();

			// Only adjust difficulty if the driver hasn't been 
			// beaten
			if (!VuGameManager::IF()->getDriverBeaten(driverName))
			{
				int lossCount = VuGameManager::IF()->numBossLosses(driverName);

				if (lossCount > 0)
				{
					VuAiManager::IF()->downTuneAiCar(pCar, lossCount);
				}
			}
		}
	}
}

//*****************************************************************************
void VuBossBattleGame::onEnd()
{
	VuAiManager::IF()->onRaceEnd();
	VuAiManager::IF()->reset();
}

//*****************************************************************************
void VuBossBattleGame::onCarFinished(VuCarEntity *pCar)
{
	VuCarStats &stats = pCar->getStats();

	// record result
	if ( pCar->getDriver()->isHuman() && !stats.mDNF )
	{
		VuStatsManager::IF()->recordBossBattleResult(mEventName, pCar);

		if ( VuCarManager::IF()->getAiCarCount() )
		{
			VuCarEntity *pAiCar = VuCarManager::IF()->getAiCar(0);

			if ( stats.mPlace == 1 )
			{
				VuGameManager::IF()->setDriverBeaten(pAiCar->getDriverName());
				
				const std::string &nextSeries = VuGameUtil::IF()->getNextSeriesName(mSeriesName);
				int stage = VuGameUtil::IF()->seriesDB()[nextSeries]["Stage"].asInt();
				VuGameManager::IF()->unlockStage(stage);

				VuProfileManager::IF()->dataWrite()["Lists"]["Series"].putValue(nextSeries);
			}
			else
			{
				VuGameManager::IF()->lostToBoss(pAiCar->getDriverName());
			}
		}
	}

	VuAiManager::IF()->removeFromRace(pCar);
}

//*****************************************************************************
void VuBossBattleGame::onIntroEnter()
{
	useHUD("Intro");

	for ( int i = 0; i < mCars.size(); i++ )
		mCars[i]->pushPauseControlRequest();

	VuEventManager::IF()->broadcast("OnStartCinematicIntro");
}

//*****************************************************************************
void VuBossBattleGame::onIntroExit()
{
	for ( int i = 0; i < mCars.size(); i++ )
		mCars[i]->popPauseControlRequest();
}

//*****************************************************************************
void VuBossBattleGame::onIntroTick(float fdt)
{
	mGameFSM.setCondition("IntroDone", !VuCarManager::IF()->isCameraOverrideEnabled());
}

//*****************************************************************************
void VuBossBattleGame::onPreGameEnter()
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
void VuBossBattleGame::onPreGameExit()
{
	for ( int i = 0; i < mCars.size(); i++ )
	{
		mCars[i]->popPauseControlRequest();
		mCars[i]->getEngine()->setRandomRev(false);
	}
}

//*****************************************************************************
void VuBossBattleGame::onPreGameTick(float fdt)
{
	updatePreGameTiming(mGameFSM.getTimeInState());
}

//*****************************************************************************
void VuBossBattleGame::onGameEnter()
{
	useHUD("Game");

	VuAiManager::IF()->onRaceStart();
}

//*****************************************************************************
void VuBossBattleGame::onGameTick(float fdt)
{
	updateTiming(fdt);
	updatePlacing();

	// did racer finish?
	bool bRacersFinished = true;
	for ( int i = 0; i < mCars.size(); i++ )
		if ( mCars[i]->getIsRacer() )
			bRacersFinished &= mCars[i]->getHasFinished();

	mGameFSM.setCondition("RacersFinished", bRacersFinished);
}

//*****************************************************************************
void VuBossBattleGame::onPostGameEnter()
{
	useHUD("PostGame");
	VuGameUtil::IF()->pushDisallowPauseMenu();
}

//*****************************************************************************
void VuBossBattleGame::onPostGameExit()
{
	VuGameUtil::IF()->popDisallowPauseMenu();
}

//*****************************************************************************
void VuBossBattleGame::onPostGameTick(float fdt)
{
	updateTiming(fdt);
	updatePlacing();
}

//*****************************************************************************
void VuBossBattleGame::updatePlacing()
{
	std::sort(mPlacing.begin(), mPlacing.end(), VuPlacingComp(mCars));

	for ( int i = 0; i < mCars.size(); i++ )
	{
		int place = i + 1;
		mCars[mPlacing[i]]->getStats().mPlace = place;
	}
}

//*****************************************************************************
bool VuBossBattleGame::VuPlacingComp::operator()(int i0, int i1)
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