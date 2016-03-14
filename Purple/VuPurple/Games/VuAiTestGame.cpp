//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  RaceGame class
// 
//*****************************************************************************

#include "VuAiTestGame.h"
#include "VuPurple/Ai/VuAiManager.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Dev/VuDevConfig.h"
#include "VuPurple/AI/VuAiUtils.h"

IMPLEMENT_RTTI(VuAiTestGame, VuGame);


//*****************************************************************************
VuAiTestGame::VuAiTestGame(VuProject *pProject):
	VuGame(pProject)
{
	// create FSM
	{
		VuFSM::VuState *pState;

		pState = mGameFSM.addState("PreGame");
		pState->setEnterMethod(this, &VuAiTestGame::onPreGameEnter);
		pState->setExitMethod(this, &VuAiTestGame::onPreGameExit);
		pState->setTickMethod(this, &VuAiTestGame::onPreGameTick);
		pState = mGameFSM.addState("Game");
		pState->setEnterMethod(this, &VuAiTestGame::onGameEnter);
		pState->setTickMethod(this, &VuAiTestGame::onGameTick);

		pState = mGameFSM.addState("Exit");

		// add transitions
		mGameFSM.addTransition("PreGame", "Game", "StartLightsOut");
		mGameFSM.addTransition("Game", "Exit", "RacersFinished");
		mGameFSM.addTransition("", "Exit", "Exit");
	}

	// load HUDs
	loadHUD("PreGame", "HUDs/RacePreGameHUD");
	loadHUD("Game", "HUDs/RaceGameHUD");

	mNumOpponents = 5;

	if (VuDevConfig::IF()->hasParam("AITestMode"))
	{
		const VuJsonContainer& testModeData = VuDevConfig::IF()->getParam("AITestMode");
		if (testModeData.hasMember("NumOpponents"))
		{
			mNumOpponents = testModeData["NumOpponents"].asInt();
		}
	}
}

//*****************************************************************************
VuAiTestGame::~VuAiTestGame()
{
}

//*****************************************************************************
void VuAiTestGame::onLoad(const VuJsonContainer &data)
{
	
}

//*****************************************************************************
void VuAiTestGame::onPreBegin()
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
void VuAiTestGame::onPostBegin()
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
void VuAiTestGame::onEnd()
{
	VuAiManager::IF()->onRaceEnd();
}

//*****************************************************************************
void VuAiTestGame::onCarFinished(VuCarEntity *pCar)
{
	VuAiManager::IF()->removeFromRace(pCar);
}

//*****************************************************************************
void VuAiTestGame::onPreGameEnter()
{
	useHUD("PreGame");

	for ( int i = 0; i < mCars.size(); i++ )
		mCars[i]->pushPauseControlRequest();

	VuEventManager::IF()->broadcast("OnPreGameEnter");
}

//*****************************************************************************
void VuAiTestGame::onPreGameExit()
{
	for ( int i = 0; i < mCars.size(); i++ )
		mCars[i]->popPauseControlRequest();
}

//*****************************************************************************
void VuAiTestGame::onPreGameTick(float fdt)
{
	updatePreGameTiming(mGameFSM.getTimeInState());
}

//*****************************************************************************
void VuAiTestGame::onGameEnter()
{
	useHUD("Game");

	VuAiManager::IF()->onRaceStart();
}

//*****************************************************************************
void VuAiTestGame::onGameTick(float fdt)
{
	updateTiming(fdt);
	updatePlacing();

	// did racer finish?
	bool carsFinished = true;
	bool anyFinished = false;
	for (int i = 0; i < mCars.size(); i++)
	{
		carsFinished &= mCars[i]->getHasFinished();
		anyFinished |= mCars[i]->getHasFinished();
	}

	if (mNumOpponents == 1 && anyFinished)
	{
		carsFinished = true;
	}

	mGameFSM.setCondition("RacersFinished", carsFinished);

	// Bail if we're in the race state for twice as long as we expect.
	if (mGameFSM.getTimeInState() > 3.0f * 60.0f)
	{
		mGameFSM.setCondition("RacersFinished", true);

		char s[64]; 
		VU_SPRINTF(s, 64, "%d", mLapCount);
		VuAiUtils::reportEvent("TimedOut", s);
	}
}

//*****************************************************************************
void VuAiTestGame::updatePlacing()
{
	std::sort(mPlacing.begin(), mPlacing.end(), VuPlacingComp(mCars));

	for ( int i = 0; i < mCars.size(); i++ )
	{
		int place = i + 1;
		mCars[mPlacing[i]]->getStats().mPlace = place;
	}
}

//*****************************************************************************
bool VuAiTestGame::VuPlacingComp::operator()(int i0, int i1)
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