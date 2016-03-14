//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  EliminationGame class
// 
//*****************************************************************************

#include "VuEliminationGame.h"
#include "VuPurple/Ai/VuAiManager.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarEngine.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Managers/VuStatsManager.h"
#include "VuPurple/Managers/VuLeaderboardManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Dev/VuDevConfig.h"


IMPLEMENT_RTTI(VuEliminationGame, VuGame);


//*****************************************************************************
VuEliminationGame::VuEliminationGame(VuProject *pProject):
	VuGame(pProject),
	mEliminationTime(10),
	mEliminationTimer(0.0f)
{
	// create FSM
	{
		VuFSM::VuState *pState;
	
		if ( !VuDevConfig::IF()->getParam("SkipIntro").asBool() )
		{
			pState = mGameFSM.addState("Intro");
			pState->setEnterMethod(this, &VuEliminationGame::onIntroEnter);
			pState->setExitMethod(this, &VuEliminationGame::onIntroExit);
			pState->setTickMethod(this, &VuEliminationGame::onIntroTick);
		}

		pState = mGameFSM.addState("PreGame");
		pState->setEnterMethod(this, &VuEliminationGame::onPreGameEnter);
		pState->setExitMethod(this, &VuEliminationGame::onPreGameExit);
		pState->setTickMethod(this, &VuEliminationGame::onPreGameTick);

		pState = mGameFSM.addState("Game");
		pState->setEnterMethod(this, &VuEliminationGame::onGameEnter);
		pState->setTickMethod(this, &VuEliminationGame::onGameTick);

		pState = mGameFSM.addState("PostGame");
		pState->setEnterMethod(this, &VuEliminationGame::onPostGameEnter);
		pState->setExitMethod(this, &VuEliminationGame::onPostGameExit);
		pState->setTickMethod(this, &VuEliminationGame::onPostGameTick);

		pState = mGameFSM.addState("Exit");

		// add transitions
		mGameFSM.addTransition("Intro", "PreGame", "IntroDone");
		mGameFSM.addTransition("PreGame", "Game", "StartLightsOut");
		mGameFSM.addTransition("Game", "PostGame", "OneCarRemaining");
		mGameFSM.addTransition("Game", "PostGame", "RacersFinished");
		mGameFSM.addTransition("", "Exit", "Exit");
	}

	VuGameUtil::IF()->constantDB()["Games"]["Elimination"]["EliminationTime"].getValue(mEliminationTime);
	mEliminationTimer = (float)mEliminationTime;
}

//*****************************************************************************
VuEliminationGame::~VuEliminationGame()
{
}

//*****************************************************************************
void VuEliminationGame::onKeyDown(VUUINT32 key)
{
	if ( key == VUKEY_F )
	{
		bool playerWins = VuKeyboard::IF()->isKeyDown(VUKEY_SHIFT);

		for ( int i = 0; i < VuCarManager::IF()->getCarCount(); i++ )
		{
			VuCarEntity *pCar = VuCarManager::IF()->getCar(i);
			VuCarStats *pStats = &pCar->getStats();

			setCarFinished(pCar);
			pCar->setHasCheated();
			pStats->mBestLapTime = VuMin(pStats->mBestLapTime, (float)pStats->mCurLapTime);
			pStats->mCurLap = VuMax(pStats->mCurLap, 2);

			if ( playerWins == pCar->getDriver()->isHuman() )
				pStats->mTotalTime += 1.0f;
		}
	}
	else
	{
		VuGame::onKeyDown(key);
	}
}

//*****************************************************************************
void VuEliminationGame::onLoad(const VuJsonContainer &data)
{
	// load HUDs
	loadHUD("Intro", "HUDs/EliminationIntroHUD");
	loadHUD("PreGame", "HUDs/EliminationPreGameHUD");
	loadHUD("Game", "HUDs/EliminationGameHUD");
	loadHUD("PostGame", mIsChallenge ? "HUDs/EliminationChallengePostGameHUD" : "HUDs/EliminationPostGameHUD");
}

//*****************************************************************************
void VuEliminationGame::onPreBegin()
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
void VuEliminationGame::onPostBegin()
{
	if ( VuAiManager::IF()->canRace() )
	{
		for ( int i = 0; i < VuCarManager::IF()->getAiCarCount(); i++ )
			VuAiManager::IF()->addToRace(VuCarManager::IF()->getAiCar(i), 0.0f, 0.0f);

		VuAiManager::IF()->onGameStart();
	}

	// init elimination stats
	mPlacing.resize(mCars.size());

	for ( int i = 0; i < mCars.size(); i++ )
	{
		VuCarStats *pStats = &mCars[i]->getStats();

		pStats->mPlace = i + 1;
		mPlacing[i] = i;

		pStats->mGameTimer = mEliminationTime;
		pStats->mEliminationCount = VuCarManager::IF()->getCarCount();
	}
}

//*****************************************************************************
void VuEliminationGame::onEnd()
{
	VuAiManager::IF()->onRaceEnd();
	VuAiManager::IF()->reset();
}

//*****************************************************************************
void VuEliminationGame::onCarFinished(VuCarEntity *pCar)
{
	VuAiManager::IF()->removeFromRace(pCar);

	VuCarStats &stats = pCar->getStats();

	// record result
	if ( pCar->getDriver()->isHuman() && !stats.mDNF )
	{
		// stats
		VuStatsManager::IF()->recordEliminationResult(mEventName, pCar);

		// leaderboard
		if ( !pCar->getHasCheated() )
		{
			VUINT64 lbValue = VUINT64(stats.mDistanceDriven);
			VuLeaderboardManager::IF()->submitScore(mEventName, lbValue);
		}
	}
}

//*****************************************************************************
void VuEliminationGame::onIntroEnter()
{
	useHUD("Intro");

	for ( int i = 0; i < mCars.size(); i++ )
		mCars[i]->pushPauseControlRequest();

	VuEventManager::IF()->broadcast("OnStartCinematicIntro");
}

//*****************************************************************************
void VuEliminationGame::onIntroExit()
{
	for ( int i = 0; i < mCars.size(); i++ )
		mCars[i]->popPauseControlRequest();
}

//*****************************************************************************
void VuEliminationGame::onIntroTick(float fdt)
{
	mGameFSM.setCondition("IntroDone", !VuCarManager::IF()->isCameraOverrideEnabled());
}

//*****************************************************************************
void VuEliminationGame::onPreGameEnter()
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
void VuEliminationGame::onPreGameExit()
{
	for ( int i = 0; i < mCars.size(); i++ )
	{
		mCars[i]->popPauseControlRequest();
		mCars[i]->getEngine()->setRandomRev(false);
	}
}

//*****************************************************************************
void VuEliminationGame::onPreGameTick(float fdt)
{
	updatePreGameTiming(mGameFSM.getTimeInState());
}

//*****************************************************************************
void VuEliminationGame::onGameEnter()
{
	useHUD("Game");

	VuAiManager::IF()->onRaceStart();
}

//*****************************************************************************
void VuEliminationGame::onGameTick(float fdt)
{
	updateTiming(fdt);
	updatePlacing();

	// elimination timeout
	mEliminationTimer -= fdt;
	if ( mEliminationTimer <= 0.0f )
	{
		mEliminationTimer += mEliminationTime;

		// hide previously eliminated racers
		for ( int i = 0; i < mCars.size(); i++ )
			if ( mCars[i]->getHasFinished() )
				mCars[i]->hide();

		// eliminate last racer
		for ( int i = (int)mPlacing.size() - 1; i >= 0; i-- )
		{
			VuCarEntity *pCar = mCars[mPlacing[i]];
			if ( !pCar->getHasFinished() )
			{
				setCarFinished(pCar);

				VuParams params;
				params.addEntity(pCar);
				VuEventManager::IF()->broadcast("OnDriverWasEliminated", params);

				break;
			}
		}
	}

	// calculate elimination count
	int eliminationCount = 0;
	for ( int i = 0; i < mCars.size(); i++ )
		eliminationCount += !mCars[i]->getHasFinished();

	// update stats
	for ( int i = 0; i < mCars.size(); i++ )
	{
		VuCarEntity *pCar = mCars[i];
		VuCarStats *pStats = &pCar->getStats();

		pStats->mGameTimer = pCar->getHasFinished() ? 0 : VuCeilInt(mEliminationTimer);
		pStats->mEliminationCount = eliminationCount;
	}

	// one jet ski remaining?
	mGameFSM.setCondition("OneCarRemaining", eliminationCount == 1);

	// did racers finish?
	bool bRacersFinished = true;
	for ( int i = 0; i < mCars.size(); i++ )
		if ( mCars[i]->getIsRacer() )
			bRacersFinished &= mCars[i]->getHasFinished();

	mGameFSM.setCondition("RacersFinished", bRacersFinished);
}

//*****************************************************************************
void VuEliminationGame::onPostGameEnter()
{
	useHUD("PostGame");

	// make sure all humans have finished
	for ( int i = 0; i < VuCarManager::IF()->getLocalHumanCarCount(); i++ )
		setCarFinished(VuCarManager::IF()->getLocalHumanCar(i));

	VuGameUtil::IF()->pushDisallowPauseMenu();
}

//*****************************************************************************
void VuEliminationGame::onPostGameExit()
{
	VuGameUtil::IF()->popDisallowPauseMenu();
}

//*****************************************************************************
void VuEliminationGame::onPostGameTick(float fdt)
{
	updateTiming(fdt);
	updatePlacing();
}

//*****************************************************************************
void VuEliminationGame::updatePlacing()
{
	std::sort(mPlacing.begin(), mPlacing.end(), VuPlacingComp(mCars));

	for ( int i = 0; i < mCars.size(); i++ )
	{
		int place = i + 1;
		mCars[mPlacing[i]]->getStats().mPlace = place;
	}
}

//*****************************************************************************
bool VuEliminationGame::VuPlacingComp::operator()(int i0, int i1)
{
	bool bFinished0 = mCars[i0]->getHasFinished();
	bool bFinished1 = mCars[i1]->getHasFinished();
	const VuCarStats &stats0 = mCars[i0]->getStats();
	const VuCarStats &stats1 = mCars[i1]->getStats();

	if ( stats0.mDNF )
		return false;

	if ( stats1.mDNF )
		return true;

	// If they have both finished, which one lasted longer
	if ( bFinished0 && bFinished1 )
		return stats0.mTotalTime > stats1.mTotalTime;

	// If only one has finished, which one
	if ( bFinished0 )
		return false;

	if ( bFinished1 )
		return true;

	// If neither have finished, which one has completed the most laps
	if ( stats0.mCurLap != stats1.mCurLap )
		return stats0.mCurLap > stats1.mCurLap;

	// If both have completed the same number of laps, which one has the least distance to go on their current lap?
	return stats0.mDistToEndOfLap < stats1.mDistToEndOfLap;
}