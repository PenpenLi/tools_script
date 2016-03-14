//*****************************************************************************
//
//  Copyright (c) 2011-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  CarChampRaceGame class
// 
//*****************************************************************************

#include "VuCarChampRaceGame.h"
#include "VuPurple/Ai/VuAiManager.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarEngine.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Managers/VuStatsManager.h"
#include "VuPurple/Managers/VuLeaderboardManager.h"
#include "VuPurple/Managers/VuAchievementManager.h"
#include "VuPurple/Managers/VuAnalyticsManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Dev/VuDevConfig.h"


IMPLEMENT_RTTI(VuCarChampRaceGame, VuGame);


//*****************************************************************************
VuCarChampRaceGame::VuCarChampRaceGame(VuProject *pProject):
	VuGame(pProject),
	mLastPlaceLastLap(false)
{
	// create FSM
	{
		VuFSM::VuState *pState;

		if ( !VuDevConfig::IF()->getParam("SkipIntro").asBool() )
		{
			pState = mGameFSM.addState("Intro");
			pState->setEnterMethod(this, &VuCarChampRaceGame::onIntroEnter);
			pState->setExitMethod(this, &VuCarChampRaceGame::onIntroExit);
			pState->setTickMethod(this, &VuCarChampRaceGame::onIntroTick);
		}

		pState = mGameFSM.addState("PreGame");
		pState->setEnterMethod(this, &VuCarChampRaceGame::onPreGameEnter);
		pState->setExitMethod(this, &VuCarChampRaceGame::onPreGameExit);
		pState->setTickMethod(this, &VuCarChampRaceGame::onPreGameTick);

		pState = mGameFSM.addState("Game");
		pState->setEnterMethod(this, &VuCarChampRaceGame::onGameEnter);
		pState->setTickMethod(this, &VuCarChampRaceGame::onGameTick);

		pState = mGameFSM.addState("PostGame");
		pState->setEnterMethod(this, &VuCarChampRaceGame::onPostGameEnter);
		pState->setExitMethod(this, &VuCarChampRaceGame::onPostGameExit);
		pState->setTickMethod(this, &VuCarChampRaceGame::onPostGameTick);

		pState = mGameFSM.addState("Exit");

		// add transitions
		mGameFSM.addTransition("Intro", "PreGame", "IntroDone");
		mGameFSM.addTransition("PreGame", "Game", "StartLightsOut");
		mGameFSM.addTransition("Game", "PostGame", "RacersFinished");
		mGameFSM.addTransition("", "Exit", "Exit");
	}
}

//*****************************************************************************
VuCarChampRaceGame::~VuCarChampRaceGame()
{
}

//*****************************************************************************
void VuCarChampRaceGame::onLoad(const VuJsonContainer &data)
{
	// load HUDs
	loadHUD("Intro", "HUDs/RaceIntroHUD");
	loadHUD("PreGame", "HUDs/RacePreGameHUD");
	loadHUD("Game", "HUDs/RaceGameHUD");
	loadHUD("PostGame", "HUDs/RaceCarChampPostGameHUD");
}

//*****************************************************************************
void VuCarChampRaceGame::onPreBegin()
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
void VuCarChampRaceGame::onPostBegin()
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
void VuCarChampRaceGame::onEnd()
{
	VuAiManager::IF()->onRaceEnd();
}

//*****************************************************************************
void VuCarChampRaceGame::onCarFinished(VuCarEntity *pCar)
{
	VuAiManager::IF()->removeFromRace(pCar);

	VuCarStats &stats = pCar->getStats();

	if ( pCar->getDriver()->isHuman() && !stats.mDNF )
	{
		// stats
		VuStatsManager::IF()->recordCarChampRaceResult(pCar);

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
	}
}

//*****************************************************************************
void VuCarChampRaceGame::onIntroEnter()
{
	useHUD("Intro");

	for ( int i = 0; i < mCars.size(); i++ )
		mCars[i]->pushPauseControlRequest();

	VuEventManager::IF()->broadcast("OnStartCinematicIntro");
}

//*****************************************************************************
void VuCarChampRaceGame::onIntroExit()
{
	for ( int i = 0; i < mCars.size(); i++ )
		mCars[i]->popPauseControlRequest();
}

//*****************************************************************************
void VuCarChampRaceGame::onIntroTick(float fdt)
{
	mGameFSM.setCondition("IntroDone", !VuCarManager::IF()->isCameraOverrideEnabled());
}

//*****************************************************************************
void VuCarChampRaceGame::onPreGameEnter()
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
void VuCarChampRaceGame::onPreGameExit()
{
	for ( int i = 0; i < mCars.size(); i++ )
	{
		mCars[i]->popPauseControlRequest();
		mCars[i]->getEngine()->setRandomRev(false);
	}
}

//*****************************************************************************
void VuCarChampRaceGame::onPreGameTick(float fdt)
{
	updatePreGameTiming(mGameFSM.getTimeInState());
}

//*****************************************************************************
void VuCarChampRaceGame::onGameEnter()
{
	useHUD("Game");

	VuAiManager::IF()->onRaceStart();
}

//*****************************************************************************
void VuCarChampRaceGame::onGameTick(float fdt)
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
void VuCarChampRaceGame::onPostGameEnter()
{
	// finish all ai cars
	for ( int i = 0; i < VuCarManager::IF()->getAiCarCount(); i++ )
		setCarFinished(VuCarManager::IF()->getAiCar(i));

	const std::string &carName = VuGameUtil::IF()->dataRead()["CarChamp"]["Car"].asString();
	VuJsonContainer &carChampData = VuProfileManager::IF()->dataWrite()["CarChampData"][carName];
	VuJsonContainer &races = carChampData["Races"];

	int eventNumber = 0;
	int raceCount = races.size();
	for ( int i = 0; i < raceCount; i++ )
		if ( races[i].hasMember("Place") )
			eventNumber++;

	VuCarEntity *pCar = VuCarManager::IF()->getLocalHumanCar(0);
	VuCarStats &stats = pCar->getStats();

	VuJsonContainer &raceData = races[eventNumber];
	raceData["Place"].putValue(stats.mPlace);

	// player points
	if ( !stats.mDNF )
	{
		int points = carChampData["Points"].asInt();
		points += VuGameUtil::IF()->constantDB()["PointsTable"][stats.mPlace - 1].asInt();
		carChampData["Points"].putValue(points);
	}

	// opponent points
	for ( int i = 0; i < VuCarManager::IF()->getAiCarCount(); i++ )
	{
		VuCarEntity *pAiCar = VuCarManager::IF()->getAiCar(i);
		VuCarStats &aiStats = pAiCar->getStats();

		if ( !aiStats.mDNF )
		{
			VuJsonContainer &opponentData = carChampData["Opponents"][i];

			int points = opponentData["Points"].asInt();
			points += VuGameUtil::IF()->constantDB()["PointsTable"][aiStats.mPlace - 1].asInt();
			opponentData["Points"].putValue(points);
		}
	}

	// car champ completed?
	if ( eventNumber == raceCount - 1 )
	{
		// record result
		VuGameUtil::CarChampTable table;
		VuGameUtil::IF()->createCarChampTable(carName, table);

		// previous championship place
		int prevChampPlace = 0;
		VuStatsManager::IF()->getCarChampBestPlace(carName.c_str(), pCar->getCarStage(), prevChampPlace);

		// determine championship place
		int champPlace = 6;
		for ( int i = 0; i < (int)table.size(); i++ )
			if ( table[i].mPlayer )
				champPlace = i + 1;

		// record stats
		VuStatsManager::IF()->recordCarChampResult(carName.c_str(), pCar->getCarStage(), champPlace);

		// first win?
		if ( champPlace == 1 )
		{
			bool firstWin = prevChampPlace != 1;
			carChampData["FirstWin"].putValue(firstWin);

			// Analytics - Which gold cars are earned?
			if ( firstWin && pCar->getCarStage() == 3 )
			{
				VuAnalyticsManager::IF()->logEvent("Gold Car Earned", "Car Name", carName.c_str(), VuJsonContainer::null);
			}
		}

		// Analytics - Championship Progression
		if ( prevChampPlace == 0 )
		{
			// analytics
			VuJsonContainer variables;
			variables["Place"].putValue(champPlace);

			char str[256];
			VU_SPRINTF(str, sizeof(str), "%s_Stage%d", carName.c_str(), pCar->getCarStage());
			VuAnalyticsManager::IF()->logEvent("Championship Finished First Time", "Championship", str, variables);
		}
	}

	useHUD("PostGame");
	VuGameUtil::IF()->pushDisallowPauseMenu();
}

//*****************************************************************************
void VuCarChampRaceGame::onPostGameExit()
{
	VuGameUtil::IF()->popDisallowPauseMenu();
}

//*****************************************************************************
void VuCarChampRaceGame::onPostGameTick(float fdt)
{
	updateTiming(fdt);
	updatePlacing();
}

//*****************************************************************************
void VuCarChampRaceGame::updatePlacing()
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
bool VuCarChampRaceGame::VuPlacingComp::operator()(int i0, int i1)
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