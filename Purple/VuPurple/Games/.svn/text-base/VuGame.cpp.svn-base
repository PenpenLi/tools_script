//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Game class
// 
//*****************************************************************************

#include "VuGame.h"
#include "VuPurple/Track/VuTrackManager.h"
#include "VuPurple/Track/VuTrackSector.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/Entities/Objects/VuAiWaypointEntity.h"
#include "VuPurple/Entities/UI/VuHUDEntities.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Managers/VuCollisionManager.h"
#include "VuPurple/Managers/VuProjectManager.h"
#include "VuPurple/Managers/VuVideoShareManager.h"
#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Managers/VuAnalyticsManager.h"
#include "VuPurple/PowerUps/VuPowerUpManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/UI/VuUI.h"
#include "VuEngine/Entities/VuEntityRepository.h"
#include "VuEngine/Entities/VuEntityFactory.h"
#include "VuEngine/Managers/VuViewportManager.h"
#include "VuEngine/Managers/VuLensWaterManager.h"
#include "VuEngine/Managers/VuTireTrackManager.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Projects/VuProject.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Pfx/VuPfxManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuPurple/Managers/VuRankManager.h"
#include "VuPurple/Managers/VuPackManager.h"


IMPLEMENT_RTTI_BASE(VuGame);


//*****************************************************************************
VuGame::VuGame(VuProject *pProject):
	mpProject(pProject),
	mTrackLength(0.0f),
	mLapCount(99),
	mIsChallenge(false),
	mPreGameTime(0.0),
	mStartLightsOut(false)
{
	// event handlers
	REG_EVENT_HANDLER(VuGame, FinishGame);
}

//*****************************************************************************
VuGame::~VuGame()
{
	for ( int i = 0; i < mCars.size(); i++ )
		mCars[i]->removeRef();

	useHUD("");

	for ( auto &entry : mHUDs )
		VuProjectManager::IF()->unload(entry.second);
}

//*****************************************************************************
void VuGame::load(const VuJsonContainer &data)
{
	int numHumans = data["Cars"].size();
	int numOpponents = VuMin(data["Opponents"].size(), getNumOpponents());

	// create ai cars
	{
		const VuJsonContainer &opponentData = data["Opponents"];

		for ( int i = 0; i < numOpponents; i++ )
		{
			bool isRacer = (numHumans == 0) && (i + 1 == opponentData.size());

			char strName[64];
			VU_SPRINTF(strName, sizeof(strName), "AiCar%02d", i+1);

			VuCarEntity *pCar = VuEntityFactory::IF()->createEntity<VuCarEntity>();
			pCar->setShortName(strName);
			pCar->setIsRacer(isRacer);

			if ( opponentData[i].isString() )
			{
				const std::string &opponentName = opponentData[i].asString();

				VuJsonContainer carData;
				VuGameUtil::IF()->buildOpponentData(opponentName, carData);

				pCar->load(carData);
			}
			else
			{
				pCar->load(opponentData[i]);
			}

			pCar->postLoad();

			mCars.push_back(pCar);
		}
	}

	// configure human cars
	{
		const VuJsonContainer &carData = data["Cars"];

		for ( int i = 0; i < carData.size(); i++ )
		{
			char strName[64];
			VU_SPRINTF(strName, sizeof(strName), "HumanCar%02d", i+1);

			VuCarEntity *pCar = VuEntityFactory::IF()->createEntity<VuCarEntity>();
			pCar->setShortName(strName);
			pCar->setIsRacer(true);
			pCar->load(carData[i]);
			pCar->postLoad();

			mCars.push_back(pCar);
		}
	}

	// info
	data["SeriesName"].getValue(mSeriesName);
	data["EventName"].getValue(mEventName);
	data["LapCount"].getValue(mLapCount);
	data["Track"].getValue(mTrackName);
	data["GameType"].getValue(mGameType);
	data["IsChallenge"].getValue(mIsChallenge);

	onLoad(data);
}

//*****************************************************************************
void VuGame::begin()
{
	VuKeyboard::IF()->addCallback(this);

	onPreBegin();

	VuTrackManager::IF()->buildTrack();
	if ( VuTrackManager::IF()->isBuilt() )
		mTrackLength = VuTrackManager::IF()->getLength();

	for ( int i = 0; i < mCars.size(); i++ )
	{
		VuCarEntity *pCar = mCars[i];

		pCar->gameInitialize();
	}

	// init stats
	for ( int i = 0; i < mCars.size(); i++ )
	{
		VuCarEntity *pCar = mCars[i];
		VuCarStats *pStats = &pCar->getStats();

		pStats->mCurLap = 1;
		pStats->mLapCount = mLapCount;
		if ( VuTrackManager::IF()->getCheckPointCount() > 1 )
			pStats->mpNextCheckPoint = VuTrackManager::IF()->getCheckPoint(1);
		pStats->mPrevPosition = pCar->getModelPosition();
		pStats->mCurrentSection = 0;

		// check for stage cheat
		if ( !mIsChallenge && pCar->getDriver()->isHuman() )
		{
			int seriesStage = VuGameUtil::IF()->seriesDB()[mSeriesName]["Stage"].asInt();
			if ( pCar->getCarStage() > seriesStage )
				pCar->setHasCheated();
		}
	}

	onPostBegin();

	mGameFSM.begin();

	VuAnalyticsManager::IF()->logEvent("levelbegin", VuGameUtil::IF()->getEventName().c_str());

#ifdef ENABLE_VU_GAME_METRICS
	mGameMetrics.onGameStart(this);
#endif
}

//*****************************************************************************
void VuGame::end()
{
#ifdef ENABLE_VU_GAME_METRICS
	mGameMetrics.onGameEnd();
#endif
	VuCarEntity *pCar = VuCarManager::IF()->getCurrLocalHumanCar();
	if (pCar && pCar->getHasFinished())
	{
		int nPlace = pCar->getStats().mPlace;
		if (nPlace == 1)
		{
			VuAnalyticsManager::IF()->logEvent("levelfinish", VuGameUtil::IF()->getEventName().c_str());
		}
		else
			VuAnalyticsManager::IF()->logEvent("levelfail", VuGameUtil::IF()->getEventName().c_str());


	}
	else
		VuAnalyticsManager::IF()->logEvent("levelcancel", VuGameUtil::IF()->getEventName().c_str());




	if ( mIsChallenge && mStartLightsOut )
	{
		if ( VuCarManager::IF()->getLocalHumanCarCount() )
		{
			VuCarEntity *pCar = VuCarManager::IF()->getLocalHumanCar(0);
			VuCarStats &stats = pCar->getStats();

			if ( !pCar->getHasCheated() )
			{
				bool completed = pCar->getHasFinished() && (stats.mPlace == 1);

				VuJsonContainer variables;
				variables["Completed"].putValue(completed);
				variables["Used Tune-Up"].putValue(pCar->isTunedUp());
				variables["Crossed Finish Line"].putValue(pCar->getHasFinished());

				char strConfig[256];
				VU_SPRINTF(strConfig, sizeof(strConfig), "%s_%s_%d", mTrackName.c_str(), mGameType.c_str(), pCar->getCarStage());
				VuAnalyticsManager::IF()->logEvent("DailyChallengePlayed", "Challenge Config", strConfig, variables);
			}
		}
	}

	mGameFSM.end();

	onEnd();

	for ( int i = 0; i < mCars.size(); i++ )
		mCars[i]->gameRelease();

	VuEntityRepository::IF()->resetManagedEntities();
	VuPfxManager::IF()->killAllEntities();
	VuTrackManager::IF()->reset();
	VuCollisionManager::IF()->reset();
	VuViewportManager::IF()->resetEffects();
	VuLensWaterManager::IF()->reset();
	VuTireTrackManager::IF()->reset();
	VuVideoShareManager::IF()->stopRecording();
	VuVideoShareManager::IF()->resetRecording();

	VuKeyboard::IF()->removeCallback(this);

}

//*****************************************************************************
bool VuGame::tick(float fdt)
{
	updateDistances(fdt);
	updateCheckPoints();

	onTick(fdt);

	mGameFSM.evaluate();
	mGameFSM.tick(fdt);

	if ( VuHUDEntity *pHUD = getCurHUDEntity() )
		pHUD->tick(fdt);

#ifdef ENABLE_VU_GAME_METRICS
	mGameMetrics.onGameTick(fdt);
#endif

	return mGameFSM.getCurState()->getName() != "Exit";
}

//*****************************************************************************
void VuGame::draw()
{
	if ( VuCarManager::IF()->isDevHudEnabled() )
	{
		// set up crop matrix and text scale
		VuGfxUtil::IF()->pushMatrix(VuUI::IF()->getCropMatrix());
		VuGfxUtil::IF()->pushTextScale(VuUI::IF()->getTextScale());

		mGameFSM.draw();

		if ( VuHUDEntity *pHUD = getCurHUDEntity() )
			pHUD->draw();

		// pop state
		VuGfxUtil::IF()->popTextScale();
		VuGfxUtil::IF()->popMatrix();
	}
}

//*****************************************************************************
void VuGame::FinishGame(const VuParams &params)
{
	mGameFSM.setCondition("GameFinished", true);
}

//*****************************************************************************
void VuGame::onKeyDown(VUUINT32 key)
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

			if ( playerWins && !pCar->getDriver()->isHuman() )
			{
				pStats->mTotalTime += 1.0f;
			}

			if ( playerWins && pCar->getDriver()->isHuman() )
			{
				const VuJsonContainer &criteria = VuGameUtil::IF()->getEventData()["Criteria"];
				pStats->mScore = criteria[0].asInt();
			}
		}
	}
}

//*****************************************************************************
void VuGame::updatePreGameTiming(float preGameTime)
{
	const VuJsonContainer &sequence = VuGameUtil::IF()->constantDB()["StartLightSequence"];

	float startTime = 0.0f;
	for ( int i = 0; i < sequence.size(); i++ )
	{
		const VuJsonContainer &entry = sequence[i];
		float time = entry["Time"].asFloat();
		if ( mPreGameTime < time && preGameTime >= time )
		{
			VuParams params;
			params.addString(entry["Event"].asCString());
			VuEventManager::IF()->broadcast("OnStartLightSequence", params);
		}

		startTime = VuMax(startTime, time);
	}

	mPreGameTime = preGameTime;

	if ( mPreGameTime >= startTime )
	{
		mGameFSM.setCondition("StartLightsOut", true);
		mStartLightsOut = true;
	}
}

//*****************************************************************************
void VuGame::updateTiming(float fdt)
{
	// update stats
	for ( int i = 0; i < mCars.size(); i++ )
	{
		VuCarEntity *pCar = mCars[i];
		VuCarStats *pStats = &pCar->getStats();

		// timing
		if ( !pCar->getHasFinished() )
		{
			pStats->mTotalTime += fdt;
			if ( pStats->mCurLap >= 1 )
				pStats->mCurLapTime += fdt;
		}
	}

	float leaderDistToEnd = FLT_MAX;

	float leaderDistFromStart = FLT_MIN;
	for ( int i = 0; i < mCars.size(); i++ )
		leaderDistFromStart = VuMax(leaderDistFromStart, mCars[i]->getStats().mDistFromStart);

	for ( int i = 0; i < mCars.size(); i++ )
		mCars[i]->getStats().mDistBehindLeader = leaderDistFromStart - mCars[i]->getStats().mDistFromStart;
}

//*****************************************************************************
void VuGame::updateDistances(float fdt)
{
	// update stats
	for ( int i = 0; i < mCars.size(); i++ )
	{
		VuCarEntity *pCar = mCars[i];
		VuCarStats *pStats = &pCar->getStats();

		if ( const VuTrackSector *pCurrentSector = pCar->getDriver()->getCurrentSector() )
		{
			VuVector3 curPos = pCar->getModelMatrix().getTrans();

			pStats->mDistFromStartOfLap = pCurrentSector->mDistanceFromStart + (pCurrentSector->mLength*pCurrentSector->traversalAmount(curPos));
			pStats->mDistToEndOfLap = mTrackLength - pStats->mDistFromStartOfLap;

			pStats->mDistFromStart = VuMax(pStats->mCurLap, 1)*mTrackLength - pStats->mDistToEndOfLap;

			// total distance traveled
			if ( !pCar->getHasFinished() )
			{
				pStats->mDistanceDriven += pCar->getLinearVelocity().mag2d()*fdt;
				pStats->mDistanceDriven = VuMin((float)pStats->mDistanceDriven, 1.5f*VuTrackManager::IF()->getLength()*mLapCount);
			}
		}
	}
}

//*****************************************************************************
void VuGame::updateCheckPoints()
{
	for ( int i = 0; i < mCars.size(); i++ )
	{
		VuCarEntity *pCar = mCars[i];
		VuCarStats *pStats = &pCar->getStats();

		VuVector3 curPosition = pCar->getModelPosition();

		if ( !pCar->getHasFinished() )
		{
			if ( pStats->mpNextCheckPoint )
			{
				if ( pStats->mpNextCheckPoint->testIntersection(pStats->mPrevPosition, curPosition, pCar->getCollisionAabb().getExtents().mX) )
				{
					VuAiWaypointEntity *pCurCP = pStats->mpNextCheckPoint;

					pStats->mCurrentSection = pCurCP->mpPrimarySector->mSection;
					pStats->mpNextCheckPoint = VuTrackManager::IF()->getNextCheckPoint(pCurCP);

					onCheckpointCrossed(pCar, pCurCP);

					if ( pCurCP->isFinish() )
					{
						if ( pStats->mCurLap >= 1 )
						{
							pStats->mBestLapTime = VuMin(pStats->mBestLapTime, (float)pStats->mCurLapTime);
							pStats->mCurLapTime = 0.0f;
						}

						if ( pStats->mCurLap == mLapCount )
						{
							setCarFinished(pCar);
						}

						pStats->mCurLap++;

						 // fixed placing bug
						pStats->mDistToEndOfLap = mTrackLength;
						pStats->mDistFromStartOfLap = 0.0f;
					}
				} // crossed plane
			}
		}

		// update prev position
		pStats->mPrevPosition = curPosition;
	}
}

//*****************************************************************************
void VuGame::setCarFinished(VuCarEntity *pCar)
{
	if ( !pCar->getHasFinished() )
	{
		pCar->setHasFinished();
		pCar->pushPauseControlRequest();

		pCar->getDriver()->setFinished();

		pCar->getStats().mGameTimer = -1;

		onCarFinished(pCar);
		if (pCar->getDriver()->isHuman())
		{
			if (!VuGameManager::IF()->getUid().empty() && (!VuGameManager::IF()->getPlayerName().empty()))
			{
				VuCarStats stats = pCar->getStats();
				VUINT totalDistance = stats.mDistanceDriven;
				VuRankManager::IF()->submitMilestone(totalDistance);
			}
		}
		VuParams params;
		params.addEntity(pCar);
		VuEventManager::IF()->broadcast("OnCarFinished", params);
	}
}

//*****************************************************************************
void VuGame::loadHUD(const char *name, const char *assetName)
{
	VuProject *pProject = VuProjectManager::IF()->load(assetName);
	if ( pProject && !pProject->getRootEntity()->isDerivedFrom(VuHUDEntity::msRTTI) )
	{
		VuProjectManager::IF()->unload(pProject);
		pProject = VUNULL;
	}

	if ( pProject )
		mHUDs[name] = pProject;
}

//*****************************************************************************
void VuGame::useHUD(const char *name)
{
	if ( VuProject *pProject = getCurHUDProject() )
		pProject->gameRelease();

	mCurHUD = name;

	if ( VuProject *pProject = getCurHUDProject() )
		pProject->gameInitialize();
}

//*****************************************************************************
VuHUDEntity *VuGame::getCurHUDEntity()
{
	if ( VuProject *pProject = getCurHUDProject() )
		return static_cast<VuHUDEntity *>(pProject->getRootEntity());

	return VUNULL;
}

//*****************************************************************************
VuProject *VuGame::getCurHUDProject()
{
	auto entry = mHUDs.find(mCurHUD);
	if ( entry != mHUDs.end() )
		return entry->second;

	return VUNULL;
}

//*****************************************************************************
bool VuGame::allCarsFinished()
{
	for (int i = 0; i < mCars.size(); i++)
	{
		const VuCarEntity* pCar = mCars[i];
		if (pCar)
		{
			if (!pCar->getHasFinished())
			{
				return false;
			}
		}
	}

	return true;
}
