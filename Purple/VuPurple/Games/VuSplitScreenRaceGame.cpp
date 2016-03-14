//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  RaceGame class
// 
//*****************************************************************************

#include "VuSplitScreenRaceGame.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarEngine.h"
#include "VuPurple/Entities/Car/Driver/VuHumanDriverEntity.h"
#include "VuPurple/Entities/Objects/VuAiWaypointEntity.h"
#include "VuPurple/Entities/UI/VuHUDEntities.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/DB/VuStringDB.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Managers/VuInputManager.h"


IMPLEMENT_RTTI(VuSplitScreenRaceGame, VuGame);


//*****************************************************************************
VuSplitScreenRaceGame::VuSplitScreenRaceGame(VuProject *pProject):
	VuGame(pProject)
{

	// create FSM
	{
		VuFSM::VuState *pState;

		pState = mGameFSM.addState("PreGame");
		pState->setEnterMethod(this, &VuSplitScreenRaceGame::onPreGameEnter);
		pState->setExitMethod(this, &VuSplitScreenRaceGame::onPreGameExit);
		pState->setTickMethod(this, &VuSplitScreenRaceGame::onPreGameTick);

		pState = mGameFSM.addState("Game");
		pState->setEnterMethod(this, &VuSplitScreenRaceGame::onGameEnter);
		pState->setTickMethod(this, &VuSplitScreenRaceGame::onGameTick);

		pState = mGameFSM.addState("PostGame");
		pState->setEnterMethod(this, &VuSplitScreenRaceGame::onPostGameEnter);
		pState->setExitMethod(this, &VuSplitScreenRaceGame::onPostGameExit);
		pState->setTickMethod(this, &VuSplitScreenRaceGame::onPostGameTick);

		pState = mGameFSM.addState("Exit");

		// add transitions
		mGameFSM.addTransition("PreGame", "Game", "StartLightsOut");
		mGameFSM.addTransition("Game", "PostGame", "RacersFinished & Expired");
		mGameFSM.addTransition("", "Exit", "Exit");
	}
}

//*****************************************************************************
VuSplitScreenRaceGame::~VuSplitScreenRaceGame()
{
}

//*****************************************************************************
void VuSplitScreenRaceGame::onLoad(const VuJsonContainer &data)
{
	mTimeRemaining = data["TimeLimit"].asFloat();

	// count humans
	int viewportCount = 0;
	const VuJsonContainer &cars = VuGameUtil::IF()->dataRead()["GameData"]["Cars"];
	for ( int i = 0; i < cars.size(); i++ )
		if ( cars[i]["Properties"]["DriverType"].asString() == "Human" )
			viewportCount++;

	// load HUDs
	char preGameHud[64], gameHud[64];
	VU_SPRINTF(preGameHud, sizeof(preGameHud), "HUDs/SplitScreen%dPreGameHUD", viewportCount);
	VU_SPRINTF(gameHud, sizeof(gameHud), "HUDs/SplitScreen%dGameHUD", viewportCount);
	loadHUD("PreGame", preGameHud);
	loadHUD("Game", gameHud);
	loadHUD("PostGame", "HUDs/SplitScreenPostGameHUD");
}

//*****************************************************************************
void VuSplitScreenRaceGame::onPreBegin()
{
	const VuJsonContainer &gameData = VuGameUtil::IF()->dataRead()["GameData"];

	for ( int i = 0; i < mCars.size(); i++ )
	{
		VuCarEntity *pCar = mCars[i];

		// place on grid
		int gridPos = gameData["Cars"][i]["GridPos"].asInt();
		if ( gridPos < VuCarManager::IF()->getGridPositionCount() )
		{
			VuTransformComponent *pGridTC = VuCarManager::IF()->getGridPosition(gridPos)->getTransformComponent();
			VuMatrix xform;
			VuGameUtil::buildSafeMatrixForCar(pGridTC->getWorldPosition(), pGridTC->getWorldTransform().getAxisY(), pCar->getCollisionAabb(), xform, true);
			pCar->getTransformComponent()->setWorldTransform(xform);
		}

		// set name
		if ( pCar->getDriver()->isHuman() )
		{
			VuHumanDriver *pHumanDriver = static_cast<VuHumanDriver *>(pCar->getDriver());

			char strName[32];
			VU_SPRINTF(strName, sizeof(strName), "%s %d", VuStringDB::IF()->getString("Common_Player").c_str(), pHumanDriver->getPadIndex() + 1);
			pCar->getDriver()->setName(strName);
		}
	}
}

//*****************************************************************************
void VuSplitScreenRaceGame::onPostBegin()
{
	// init race stats
	mPlacing.resize(mCars.size());
	mChampRanks.resize(mCars.size());

	for ( int i = 0; i < mCars.size(); i++ )
	{
		VuCarStats *pRaceStats = &mCars[i]->getStats();

		pRaceStats->mPlace = i + 1;
		pRaceStats->mChampRank = i + 1;
		mPlacing[i] = i;
		mChampRanks[i] = i;
	}
}

//*****************************************************************************
void VuSplitScreenRaceGame::checkPauseInput()
{
	if ( VuTouch::IF()->getFocusPriority() == 0 )
	{
		for ( int i = 0; i < VuCarManager::IF()->getLocalHumanCarCount(); i++ )
		{
			VuHumanDriver *pHumanDriver = static_cast<VuHumanDriver *>(VuCarManager::IF()->getLocalHumanCar(i)->getDriver());
			int padIndex = pHumanDriver->getPadIndex();
			if ( VuInputManager::IF()->getButtonWasPressed(padIndex, "Pause") )
			{
				VuGameUtil::IF()->playSfx(VuGameUtil::SFX_UI_SELECT);

				VuParams params;
				params.addInt(padIndex);
				VuEventManager::IF()->broadcast("OnPauseActionGame", params);

				break;
			}
		}
	}
}

//*****************************************************************************
void VuSplitScreenRaceGame::onPreGameEnter()
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
void VuSplitScreenRaceGame::onPreGameExit()
{
	for ( int i = 0; i < mCars.size(); i++ )
	{
		mCars[i]->popPauseControlRequest();
		mCars[i]->getEngine()->setRandomRev(false);
	}
}

//*****************************************************************************
void VuSplitScreenRaceGame::onPreGameTick(float fdt)
{
	updatePreGameTiming(mGameFSM.getTimeInState());

	checkPauseInput();
}

//*****************************************************************************
void VuSplitScreenRaceGame::onGameEnter()
{
	useHUD("Game");

	mPostGameDelay = VuGameUtil::IF()->constantDB()["Game"]["SplitScreenPostGameDelay"].asFloat();
}

//*****************************************************************************
void VuSplitScreenRaceGame::onGameTick(float fdt)
{
	updateTiming(fdt);
	updatePlacing();

	// did racer finish?
	bool allRacersFinished = true;
	bool anyRacerFinished = false;
	for ( int i = 0; i < mCars.size(); i++ )
	{
		if ( mCars[i]->getIsRacer() )
		{
			allRacersFinished &= mCars[i]->getHasFinished();
			anyRacerFinished |= mCars[i]->getHasFinished();
		}
	}

	mGameFSM.setCondition("RacersFinished", allRacersFinished);

	if ( allRacersFinished )
	{
		mPostGameDelay -= fdt;
		mGameFSM.setCondition("Expired", mPostGameDelay <= 0.0f);
	}

	checkPauseInput();

	// update timers & handle DNF
	if ( anyRacerFinished )
	{
		mTimeRemaining -= fdt;
		for ( int i = 0; i < mCars.size(); i++ )
		{
			VuCarEntity *pCar = mCars[i];
			VuCarStats &stats = pCar->getStats();

			if ( !pCar->getHasFinished() )
			{
				stats.mGameTimer = VuCeilInt(VuMax(mTimeRemaining, 0.0f));

				if ( stats.mGameTimer <= 0 )
				{
					stats.mDNF = true;
					setCarFinished(pCar);
				}
			}
		}
	}
}

//*****************************************************************************
void VuSplitScreenRaceGame::onPostGameEnter()
{
	useHUD("PostGame");
	VuGameUtil::IF()->pushDisallowPauseMenu();
}

//*****************************************************************************
void VuSplitScreenRaceGame::onPostGameExit()
{
	VuGameUtil::IF()->popDisallowPauseMenu();
}

//*****************************************************************************
void VuSplitScreenRaceGame::onPostGameTick(float fdt)
{
	updateTiming(fdt);
	updatePlacing();
}

//*****************************************************************************
void VuSplitScreenRaceGame::updatePlacing()
{
	std::sort(mPlacing.begin(), mPlacing.end(), VuPlacingComp(mCars));
	std::sort(mChampRanks.begin(), mChampRanks.end(), VuChampRankComp(mCars));

	for ( int i = 0; i < mCars.size(); i++ )
	{
		mCars[mPlacing[i]]->getStats().mPlace = i + 1;
		mCars[mChampRanks[i]]->getStats().mChampRank = i + 1;
	}

	// update points
	for ( int i = 0; i < VuCarManager::IF()->getCarCount(); i++ )
	{
		VuCarEntity *pCar = VuCarManager::IF()->getCar(i);
		VuCarStats *pStats = &pCar->getStats();

		pStats->mPoints = pStats->mBasePoints;
		if ( pStats->mHasFinished && !pStats->mDNF )
			pStats->mPoints += VuGameUtil::IF()->constantDB()["PointsTable"][pStats->mPlace - 1].asInt();
	}
}

//*****************************************************************************
bool VuSplitScreenRaceGame::VuPlacingComp::operator()(int i0, int i1)
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

//*****************************************************************************
bool VuSplitScreenRaceGame::VuChampRankComp::operator()(int i0, int i1)
{
	const VuCarStats &stats0 = mCars[i0]->getStats();
	const VuCarStats &stats1 = mCars[i1]->getStats();

	return stats0.mPoints > stats1.mPoints;
}