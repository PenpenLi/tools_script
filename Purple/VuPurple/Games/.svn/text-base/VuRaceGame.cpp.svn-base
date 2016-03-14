//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  RaceGame class
// 
//*****************************************************************************

#include "VuRaceGame.h"
#include "VuPurple/Ai/VuAiManager.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarEngine.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Managers/VuStatsManager.h"
#include "VuPurple/Managers/VuLeaderboardManager.h"
#include "VuPurple/Managers/VuAchievementManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Dev/VuDevConfig.h"
#include "VuEngine/Managers/VuDialogManager.h"
#include "VuPurple/Managers/VuGiftDialogManager.h"


IMPLEMENT_RTTI(VuRaceGame, VuGame);


//*****************************************************************************
VuRaceGame::VuRaceGame(VuProject *pProject):
	VuGame(pProject),
	mLastPlaceLastLap(false)
{
	// create FSM
	{
		VuFSM::VuState *pState;

		if ( !VuDevConfig::IF()->getParam("SkipIntro").asBool() )
		{
			pState = mGameFSM.addState("Intro");
			pState->setEnterMethod(this, &VuRaceGame::onIntroEnter);
			pState->setExitMethod(this, &VuRaceGame::onIntroExit);
			pState->setTickMethod(this, &VuRaceGame::onIntroTick);
		}

		pState = mGameFSM.addState("PreGame");
		pState->setEnterMethod(this, &VuRaceGame::onPreGameEnter);
		pState->setExitMethod(this, &VuRaceGame::onPreGameExit);
		pState->setTickMethod(this, &VuRaceGame::onPreGameTick);

		pState = mGameFSM.addState("Game");
		pState->setEnterMethod(this, &VuRaceGame::onGameEnter);
		pState->setTickMethod(this, &VuRaceGame::onGameTick);

		pState = mGameFSM.addState("PostGame");
		pState->setEnterMethod(this, &VuRaceGame::onPostGameEnter);
		pState->setExitMethod(this, &VuRaceGame::onPostGameExit);
		pState->setTickMethod(this, &VuRaceGame::onPostGameTick);

		pState = mGameFSM.addState("Exit");

		// add transitions
		mGameFSM.addTransition("Intro", "PreGame", "IntroDone");
		mGameFSM.addTransition("PreGame", "Game", "StartLightsOut");
		mGameFSM.addTransition("Game", "PostGame", "RacersFinished");
		mGameFSM.addTransition("", "Exit", "Exit");
	}
}

//*****************************************************************************
VuRaceGame::~VuRaceGame()
{
}

//*****************************************************************************
void VuRaceGame::onLoad(const VuJsonContainer &data)
{
	// load HUDs
	loadHUD("Intro", "HUDs/RaceIntroHUD");
	loadHUD("PreGame", "HUDs/RacePreGameHUD");
	loadHUD("Game", "HUDs/RaceGameHUD");
	loadHUD("PostGame", mIsChallenge ? "HUDs/RaceChallengePostGameHUD" : "HUDs/RacePostGameHUD");
}

//*****************************************************************************
void VuRaceGame::onPreBegin()
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
void VuRaceGame::onPostBegin()
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
void VuRaceGame::onEnd()
{
	VuAiManager::IF()->onRaceEnd();
}

//*****************************************************************************
void VuRaceGame::onCarFinished(VuCarEntity *pCar)
{
	VuAiManager::IF()->removeFromRace(pCar);

	VuCarStats &stats = pCar->getStats();

	if ( pCar->getDriver()->isHuman() && !stats.mDNF )
	{
		// stats
		VuStatsManager::IF()->recordRaceResult(mEventName, pCar);

		// achievements
		if ( stats.mPlace == 1 )
		{
			if ( stats.mUsedBrake == false && pCar->getCarStage() >= 3 )
				VuAchievementManager::IF()->unlock(25); // Win a 1000hp Race without using the brake

			if ( stats.mRecoverCount == 0 && pCar->getCarStage() >= 3 )
				VuAchievementManager::IF()->unlock(26); // Win a 1000hp Race without resetting

			if ( stats.mPowerUpsUsedCount == 0 )
				VuAchievementManager::IF()->unlock(27); // Win a Race without using a Powerup

			if ( mLastPlaceLastLap && stats.mLapCount > 1 )
				VuAchievementManager::IF()->unlock(28); // Go from last to first place on the last lap of a Race
		}

		// leaderboard
		if ( !pCar->getHasCheated() )
		{
			VUINT64 lbValue = -VUINT64(stats.mTotalTime*1000);
			VuLeaderboardManager::IF()->submitScore(mEventName, lbValue);
		}
	}
}

//*****************************************************************************
void VuRaceGame::onIntroEnter()
{
	useHUD("Intro");

	for ( int i = 0; i < mCars.size(); i++ )
		mCars[i]->pushPauseControlRequest();

	VuEventManager::IF()->broadcast("OnStartCinematicIntro");
}

//*****************************************************************************
void VuRaceGame::onIntroExit()
{
	for ( int i = 0; i < mCars.size(); i++ )
		mCars[i]->popPauseControlRequest();
}

//*****************************************************************************
void VuRaceGame::onIntroTick(float fdt)
{
	mGameFSM.setCondition("IntroDone", !VuCarManager::IF()->isCameraOverrideEnabled());
}

//*****************************************************************************
void VuRaceGame::onPreGameEnter()
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
void VuRaceGame::onPreGameExit()
{
	for ( int i = 0; i < mCars.size(); i++ )
	{
		mCars[i]->popPauseControlRequest();
		mCars[i]->getEngine()->setRandomRev(false);
	}
}

//*****************************************************************************
void VuRaceGame::onPreGameTick(float fdt)
{
	updatePreGameTiming(mGameFSM.getTimeInState());
}

//*****************************************************************************
void VuRaceGame::onGameEnter()
{
	useHUD("Game");

	VuAiManager::IF()->onRaceStart();
}

//*****************************************************************************
void VuRaceGame::onGameTick(float fdt)
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
void VuRaceGame::onPostGameEnter()
{
	useHUD("PostGame");
	VuGameUtil::IF()->pushDisallowPauseMenu();
	
}

//*****************************************************************************
void VuRaceGame::onPostGameExit()
{
	VuGameUtil::IF()->popDisallowPauseMenu();
}

//*****************************************************************************
void VuRaceGame::onPostGameTick(float fdt)
{
	updateTiming(fdt);
	updatePlacing();
}

//*****************************************************************************
void VuRaceGame::updatePlacing()
{
	std::sort(mPlacing.begin(), mPlacing.end(), VuPlacingComp(mCars));

	for ( int i = 0; i < mCars.size(); i++ )
	{
		int place = i + 1;
		mCars[mPlacing[i]]->getStats().mPlace = place;
	}

	if ( VuCarManager::IF()->getLocalHumanCarCount() )
	{
		VuCarEntity *pCar = VuCarManager::IF()->getLocalHumanCar(0);
		VuCarStats &stats = pCar->getStats();
		if ( (stats.mCurLap == stats.mLapCount) && (stats.mPlace == mCars.size()) && (mCars.size() > 5) )
			mLastPlaceLastLap = true;
	}
}

//*****************************************************************************
bool VuRaceGame::VuPlacingComp::operator()(int i0, int i1)
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