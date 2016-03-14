//*****************************************************************************
//
//  Copyright (c) 2007-2008 Ralf Knoesel
//  Copyright (c) 2008-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  AiManager class
// 
//*****************************************************************************

#include "VuEngine/Components/Transform/VuTransformComponent.h"
#include "VuEngine/Containers/VuObjectArray.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Managers/VuViewportManager.h"
#include "VuEngine/Json/VuJsonWriter.h"
#include "VuEngine/Json/VuJsonReader.h"
#include "VuEngine/Util/VuColor.h"
#include "VuEngine/Util/VuDataUtil.h"

#include "VuEngine/Dev/VuDev.h"
#include "VuEngine/Dev/VuDevConfig.h"
#include "VuEngine/Dev/VuDevStat.h"
#include "VuEngine/Dev/VuDevMenu.h"
#include "VuEngine/Dev/VuDevUtil.h"

#include "VuPurple/Track/VuTrackManager.h"
#include "VuPurple/Track/VuTrackPlan.h"
#include "VuPurple/Track/VuTrackSector.h"
#include "VuPurple/Track/VuTrackUtil.h"
#include "VuPurple/Entities/Objects/VuAiWaypointEntity.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Driver/VuAiDriverEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarEngine.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuPurple/PowerUps/VuPowerUpManager.h"
#include "VuPurple/AI/VuAiTuningVariables.h"
#include "VuPurple/AI/VuAiBrain.h"
#include "VuPurple/AI/VuAiBehavior.h"
#include "VuPurple/AI/VuAiManager.h"
#include "VuPurple/AI/VuAiDriverHint.h"

// For testing car effects on various AI situations
#include "VuPurple/Entities/Car/Parts/VuCarEffectController.h"

#define STATPAGE_NAME			"AI"
#define DEBUG_EVENTS_FILENAME	"aidebug.json"
#define COMPETE_TIME			(5.0f)
#define COMPETE_MAX_DISTSQ		(25.0f * 25.0f)

//*****************************************************************************
float VuAiManager::smMaxSpeed = 150.0f;

float VuAiManager::smMinSimRadius = 300.0f;
float VuAiManager::smMaxSimRadius = 350.0f;

bool VuAiManager::smDebugWaypoints = false;
bool VuAiManager::smForceDrive = false;
float VuAiManager::smWedgedDeltaThreshold = 2.0f;
float VuAiManager::smTimeSpentWedgedThreshold = 10.0f;

// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuAiManager, VuAiManager);

//*****************************************************************************
VuAiManager::VuAiManager()
: mRaceState(UNINITIALIZED)
, mRecordDebugEvents(false)
, mRenderDebugEvents(false)
, mLoadDebugEvents(false)
, mpFirstSector(VUNULL)
, mpLastSector(VUNULL)
, mWedgeDetectionEnabled(false)
, mLastDeathBatTime(0.0f)
{
}

//*****************************************************************************
VuAiManager::~VuAiManager()
{
	reset();
}


//*****************************************************************************
bool VuAiManager::init()
{
	VuTickManager::IF()->registerHandler(this, &VuAiManager::tickDecision, "Decision");

	// Read DevConfig settings
	VuJsonContainer config = VuDevConfig::IF()->getParam("AIConfig");

	VuDataUtil::getValue(config["DebugWaypoints"], smDebugWaypoints);
	VuDataUtil::getValue(config["ForceDrive"], smForceDrive);
	VuDataUtil::getValue(config["MinSimulationDistance"], smMinSimRadius);
	VuDataUtil::getValue(config["MaxSimulationDistance"], smMaxSimRadius);
	VuDataUtil::getValue(config["RecordDebugEvents"], mRecordDebugEvents);
	VuDataUtil::getValue(config["RenderDebugEvents"], mRenderDebugEvents);
	VuDataUtil::getValue(config["LoadDebugEvents"], mLoadDebugEvents);

	static VuDevBoolOnce sOnce;

	if (sOnce)
	{
		VuDevMenu::IF()->addFloat("AI/Tuning/Max Speed", smMaxSpeed, 10.0f, 0.0f);
		VuDevMenu::IF()->addBool("AI/Debug/Waypoints", smDebugWaypoints);
		VuDevMenu::IF()->addFloat("AI/Debug/Wedged Delta Threshold", smWedgedDeltaThreshold);
		VuDevMenu::IF()->addFloat("AI/Debug/Wedged Time Threshold", smTimeSpentWedgedThreshold);

		VuDevStat::IF()->addPage(STATPAGE_NAME, VuRect(50, 10, 40, 80));
	}

	mRaceState = INITIALIZED;

	if (mLoadDebugEvents)
	{
		VuJsonReader reader;

		reader.loadFromFile(mDebugEvents, "aidebug.json");
	}

	// Load global constants for AI's out of the ConstantDB
	mAiGlobalConstants.load();

	mPowerUpTracker.init();

	return true;
}

//*****************************************************************************
void VuAiManager::release()
{
	mRaceState = UNINITIALIZED;

	mPowerUpTracker.release();

	reset();

	VuTickManager::IF()->unregisterHandlers(this);
}

//*****************************************************************************
void VuAiManager::onGameStart()
{
	mRaceState = STAGING;

	mpFirstSector = VuTrackManager::IF()->getFirstSector();
	mpLastSector = VuTrackManager::IF()->getLastSector();

	doCarPlacement();

	mPowerUpTracker.raceStart();

	mLastDeathBatTime = 0.0f;
}

//*****************************************************************************
void VuAiManager::onRaceStart()
{
	mRaceState = RACING;
	mRaceTimeElapsed = 0.0f;

	enableWedgeDetection(true);
}

//*****************************************************************************
void VuAiManager::onRaceEnd()
{
	mRaceState = ENDED;

	enableWedgeDetection(false);

	writeDebugEvents();

	reset();
}

//*****************************************************************************
void VuAiManager::reset()
{
	mpFirstSector = VUNULL;
	mpLastSector = VUNULL;

	for(int iObject = 0; iObject < mAIObjects.size(); iObject++)
	{
		mAIObjects[iObject]->mpDriver->setAiInstance(VUNULL);
		mAIObjects[iObject]->release();

		mAIObjects[iObject]->removeRef();
	}

	mAIObjects.clear();

	mRaceState = INITIALIZED;
}

//*****************************************************************************
bool VuAiManager::canRace()
{
	// Currently only need a valid track available to be able to race
	return VuTrackManager::IF()->isBuilt();
}

//*****************************************************************************
void VuAiManager::addToRace(VuCarEntity *pCar, float finishTime, float topSpeedMPH)
{
	VUASSERT(pCar->getDriver()->getRTTI().isDerivedFrom(VuAiDriver::msRTTI), "Car added to AIManager is not driven by race AI");

	VuAiDriver *pDriver = static_cast<VuAiDriver *>(pCar->getDriver());
	VUASSERT(pDriver, "Unable to get Driver from Car");

	VuAiInstance *pNewObject = new VuAiInstance;
	VUASSERT(pNewObject, "Failed to allocate AIObject");

	// Set up the AiInstance with a car and driver, and let it create a brain
	pNewObject->init(pDriver, pCar);

	// If there are specific pre-configuration parameters for that car, do that
	pNewObject->preConfigure(finishTime, topSpeedMPH);

	// Add to list of AI Instances
	mAIObjects.push_back(pNewObject);
}

//*****************************************************************************
void VuAiManager::removeFromRace(VuCarEntity *pCar)
{
	for(int iObject = 0; iObject < mAIObjects.size(); iObject++)
	{
		VuAiInstance *pObject = mAIObjects[iObject];

		if(pObject->mpCar == pCar)
		{
			pObject->mpDriver->setAiInstance(VUNULL);
			pObject->release();

			mAIObjects.remove(pObject);

			pObject->removeRef();

			break;
		}
	}
}

//*****************************************************************************
void VuAiManager::getStartingStats(const VuCarEntity *pCar, int &lapCount, VuAiWaypointEntity **ppNextCheckPoint) const
{
	VUASSERT(ppNextCheckPoint, "");

	for(int iObject = 0; iObject < mAIObjects.size(); iObject++)
	{
		const VuAiInstance *pObject = mAIObjects[iObject];

		if(pCar == pObject->mpCar)
		{
			lapCount = pObject->mInitialLapCount;
			*ppNextCheckPoint = pObject->mpInitialCheckPoint;

			return;
		}
	}

	VUASSERT(0, "Starting stats asked for unrecognized Car");
}

//*****************************************************************************
bool VuAiManager::isFinishedTraversingSector(const VuVector3 &prevPos, const VuVector3 &curPos, const VuTrackSector *pSector) const
{	if(pSector->traversalAmount(prevPos) < 1.0f &&
	   pSector->traversalAmount(curPos) >= 1.0f)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//*****************************************************************************
void VuAiManager::powerUpWasUsed(const char* pName)
{
	mPowerUpTracker.powerUpWasUsed(pName);
}

//*****************************************************************************
const char* VuAiManager::getBestPowerUpByGroup(const char* pGroupName)
{
	return mPowerUpTracker.getBestPowerUpByGroup(pGroupName);
}

//*****************************************************************************
void VuAiManager::downTuneAiCar(VuCarEntity* pCar, int numLosses)
{
	// Adjust the tuning of the AI car downwards based on the number of losses
	// to that boss
	if (!pCar->getDriver()->isAi())
	{
		return;
	}

	// Search for the car
	VuAiInstance* pInstance = VUNULL;

	for (int i = 0; i < mAIObjects.size(); i++)
	{
		if (mAIObjects[i] && mAIObjects[i]->getCar() == pCar)
		{
			pInstance = mAIObjects[i];
		}
	}

	if (pInstance == VUNULL)
	{
		return;
	}

	// Here we want the pointer to the root of the tuning variable structure
	// rather than a race-progress specific pointer, which is the normal way
	// to access the tuning variables
	//
	bool getRawData = true;
	VuAiTuningVariables* pTuning = pInstance->getAiTuningVariables(getRawData);
	if (pTuning == VUNULL)
	{
		return;
	}

	float downTuneRatio = VuLinStep(0.0f, 5.0f, (float)numLosses);
	float downTunePercent = VuLerp(0.0f, 50.0f, downTuneRatio);
	pTuning->downTuneByPercent(pTuning, downTunePercent);

	pCar->setAiBuffAccel(pInstance->getAiTuningVariables()->mAccelerationMultiplier);
	pCar->setAiBuffHandling(pInstance->getAiTuningVariables()->mHandlingMultiplier);
	pCar->setAiBuffSpeed(pInstance->getAiTuningVariables()->mTopSpeedMultiplier);
	pCar->setAiBuffTough(pInstance->getAiTuningVariables()->mToughnessMultiplier);
}

//*****************************************************************************
void VuAiManager::tickDecision(float fdt)
{
	switch(mRaceState)
	{
	case UNINITIALIZED:
	case INITIALIZED:
		break;

	// on STAGING, the game is loaded and we have a track but we haven't started
	// the actual race yet
	case STAGING:
		break;

	// We've started the actual race and are racing!
	case RACING:
		tickRacing(fdt);
		break;

	// Race ended
	case ENDED:
		break;

	default:
		VUASSERT(0, "case not handled in switch statement");
		break;
	}

	updateStatPage();

	if(smDebugWaypoints)
	{
		drawTrack();
	}
}

//*****************************************************************************
void VuAiManager::moveCarTo(VuCarEntity *pCar, const VuMatrix &matrix)
{
	for(int iObject = 0; iObject < mAIObjects.size(); iObject++)
	{
		VuAiInstance *pObject = mAIObjects[iObject];

		if(pObject->mpCar == pCar)
		{
			VuTrackSector* pNearestSector = VuTrackManager::IF()->findSectorFromPosition(matrix.getTrans());
			pObject->mpDriver->replan(pNearestSector);
		}
	}
}

//*****************************************************************************
void VuAiManager::adjustCompetition(float fdt)
{
	// Assume the human is the last car in the list in case we can't find a "racer"
	VuCarEntity *pHumanCar = VuCarManager::IF()->getCar(VuCarManager::IF()->getCarCount() - 1);

	// Compare vs. the first human player
	//
	for (int i = 0; i < VuCarManager::IF()->getCarCount(); i++)
	{
		VuCarEntity* pCar = VuCarManager::IF()->getCar(i);
		if (pCar->getIsRacer())
		{
			pHumanCar = pCar;
			break;
		}
	}

	// Determine ideal cars to target in terms of field position
	for (int i = 0; i < mAIObjects.size(); i++)
	{
		VuAiInstance* pInstance = mAIObjects[i];

		// Don't compete if we're low skill
		if (pInstance->getAiDriver()->getSkill() <= SKILL_BAD)
		{
			continue;
		}

		
		VuCarEntity* pMyCar = pInstance->getCar();
		
		// If we're a certain distance behind the player, don't compete
		// with each other, only compete with him. If we're near or in 
		// front of the human player, feel free to compete with anyone
		//
		if (pHumanCar)
		{
			float distToRacer = pHumanCar->getStats().mDistFromStart - pMyCar->getStats().mDistFromStart;

			// Target Human Only distance is 100m
			if (distToRacer > 100.0f)
			{
				pInstance->mpCompetitor = pHumanCar;
				pInstance->getAiDriver()->setTargetOpponent(pHumanCar);
				pInstance->mCompetitorTimer = COMPETE_TIME;
			}
		}

		eAiCarPack myPack = pInstance->getCarPack(pMyCar);
		eAiCarPack myDesiredPack = pInstance->getAiTuningVariables()->mDesiredCarPack;

		VuCarEntity* pCompeteCar = pInstance->mpCompetitor;
		if (pCompeteCar != VUNULL)
		{
			pInstance->mCompetitorTimer -= fdt;
			if (pInstance->mCompetitorTimer <= 0.0f)
			{
				pInstance->mpCompetitor = VUNULL;
				pInstance->getAiDriver()->setTargetOpponent(VUNULL);
				pCompeteCar = VUNULL;
			}
		}

		if (pCompeteCar == VUNULL)
		{
			if (myPack == myDesiredPack)
			{
				// Compete with the player since he's nearby
				VuCarEntity *pCar = pHumanCar;

				pInstance->mpCompetitor = pCar;
				pInstance->getAiDriver()->setTargetOpponent(pCar);
				pInstance->mCompetitorTimer = COMPETE_TIME;

				continue;
			}
			else
			{
				VuCarEntity* pCar = VUNULL;

				// Lose ground - compete with head of desired pack
				if (pInstance->mCarPacks[myDesiredPack].size() > 0)
				{
					pCar = pInstance->mCarPacks[myDesiredPack].front();
					pInstance->mCarPacks[myDesiredPack].pop_front();
				}
				else
				{
					pCar = pHumanCar;
				}

				if (pCar)
				{
					pInstance->mpCompetitor = pCar;
					pInstance->getAiDriver()->setTargetOpponent(pCar);
					pInstance->mCompetitorTimer = COMPETE_TIME;

					continue;
				}
			}
		}
	}
}


//*****************************************************************************
void VuAiManager::findCompetitor(float fdt)
{
	// Choose competitive pairs. This matches an AI Car up with a human player
	// and causes the AI to be a bit more competitive with that player for a 
	// while.
	for (int iPlayer = 0; iPlayer < VuCarManager::IF()->getLocalHumanCarCount(); iPlayer++)
	{
		VuCarEntity *pCar = VuCarManager::IF()->getLocalHumanCar(iPlayer);

		VuAiDriver *pLastCompetitor = VUNULL;
		bool bIsAssigned = false;
		bool bReassign = false;

		for (int iObject = 0; iObject < mAIObjects.size(); iObject++)
		{
			VuAiInstance *pObject = mAIObjects[iObject];

			VuAiDriver *pDriver = pObject->mpDriver;

			if (pObject->mpCompetitor == pCar)
			{
				bIsAssigned = true;

				pLastCompetitor = pDriver;

				pObject->mCompetitorTimer -= fdt;

				if (pObject->mCompetitorTimer <= 0.0f)
				{
					pObject->mpCompetitor = VUNULL;
					pDriver->setTargetOpponent(VUNULL);

					bReassign = true;
				}
				else
				{
					bReassign = false;
					/*
					VuDev::IF()->printf(pObject->mCurMatrix.getTrans(),
					VUGFX_TEXT_DRAW_NOCLIP,
					VuColor(255, 255, 255),
					"Competitor %0.1f",
					pObject->mCompetitorTimer);
					*/
				}
			}
		}

		if (!bIsAssigned || bReassign)
		{
			assignCompetitor(pCar, pLastCompetitor);
		}
	}
}

//*****************************************************************************
void VuAiManager::assignCompetitor(VuCarEntity *pCar, VuAiDriver *pLastCompetitor)
{
	// Find closest cars
	int newCompetitorIndex = -1;
	float minDistSq = FLT_MAX;

	for (int iObject = 0; iObject < mAIObjects.size(); iObject++)
	{
		VuAiDriver *pDriver = mAIObjects[iObject]->mpDriver;

		if (pDriver->getSkill() >= SKILL_NORMAL)
		{
			// Ignore the last car we competed against
			if (pDriver != pLastCompetitor)
			{
				VuVector3 diff = mAIObjects[iObject]->getCar()->getModelMatrix().getTrans() - pCar->getTransformComponent()->getWorldPosition();

				float distSq = diff.magSquared();

				if (distSq < COMPETE_MAX_DISTSQ && distSq < minDistSq)
				{
					newCompetitorIndex = iObject;
					minDistSq = distSq;
				}
			}
		}
	}

	// assign the competitor as our new driving target
	if (newCompetitorIndex >= 0 && newCompetitorIndex < mAIObjects.size())
	{
		VuAiDriver *pDriver = mAIObjects[newCompetitorIndex]->mpDriver;

		// Targets for drivers are assigned by the manager
		pDriver->setTargetOpponent(pCar);

		mAIObjects[newCompetitorIndex]->mpCompetitor = pCar;
		mAIObjects[newCompetitorIndex]->mCompetitorTimer = COMPETE_TIME;
	}
}

//*****************************************************************************
void VuAiManager::tickRacing(float fdt)
{
	mRaceTimeElapsed += fdt;

	for(int aiObjectIndex = 0; aiObjectIndex < (int)mAIObjects.size(); aiObjectIndex++)
	{
		VuAiInstance *pObject = mAIObjects[aiObjectIndex];

		// Doesn't need to happen every tick
		determineCarPacks(pObject);

		if(mRenderDebugEvents)
		{
			renderDebugEvents(pObject->mpDriver->getName());
		}
	}

	// Find someone to compete with
	adjustCompetition(fdt);
}

//*****************************************************************************
float VuAiManager::getMaxSpeedMPH() const
{
	return smMaxSpeed;
}

//*****************************************************************************
void VuAiManager::notifyBranchChanges(int sectorCount, const VuTrackSector **apSectors)
{
	// Find any AI objects that have one or more of these sectors in their plans.
	for(int iObject = 0; iObject < mAIObjects.size(); iObject++)
	{
		VuAiDriver *pDriver = mAIObjects[iObject]->mpDriver;

		const VuTrackPlan &plan = pDriver->getPlan();

		for(int iSector = 0; iSector < sectorCount; iSector++)
		{
			const VuTrackSector *pChangedSector = apSectors[iSector];

			// If the plan includes the sector that changed...
			if(plan.includes(pChangedSector))
			{
				const VuTrackSector *pCurrentSector = plan.getSectorForStep(0);

				// If the changed sector is one of the future steps of the plan (not the current step)...
				if(pCurrentSector != pChangedSector)
				{
					// If the changed sector was closed...
					if(pChangedSector->mBranchProbability <= 0.0f)
					{
						VUPRINTF("AI Driver %s replanning due to branch change.\n", pDriver->getName());

						// Come up with a new plan
						pDriver->replan(pCurrentSector);

						// break after replanning because we only need to replan once 
						// if any of the changed sectors were in our old plan
						break;
					}
				}
			}
		}
	}
}

//*****************************************************************************
void VuAiManager::doCarPlacement()
{
	for(int iObject = 0; iObject < mAIObjects.size(); iObject++)
	{
		VuAiInstance *pObject = mAIObjects[iObject];

		const VuMatrix &matrix = pObject->mpCar->getModelMatrix();

		// Use the placed sector (mpFirstSector) in the race, not the "current" sector, which 
		// the driver has already updated to be the first sector of the plan
		pObject->mpDriver->replan(mpFirstSector);
	}
}

//*****************************************************************************
void VuAiManager::enableWedgeDetection(bool enable) 
{ 
	mWedgeDetectionEnabled = enable; 
}

//*****************************************************************************
// Static
bool VuAiManager::carDistSortFunction(VuCarEntity* pOne, VuCarEntity* pTwo)
{
	VUASSERT(pOne != VUNULL && pTwo != VUNULL, "Illegal parameter passed on carDistSortFunction().");

	if (pOne == VUNULL || pTwo == VUNULL)
	{
		return false;
	}

	// Highest distance sorts to the top of the list
	if (pOne->getStats().mDistFromStart > pTwo->getStats().mDistFromStart)
	{
		return true;
	}

	return false;
}

//*****************************************************************************
void VuAiManager::determineCarPacks(VuAiInstance* pMyInstance)
{
	if (mAIObjects.size() <= 0)
	{
		return;
	}

	if (VuCarManager::IF()->getCarCount() <= 0)
	{
		return;
	}

	// Assume the human is the last car in the list in case we can't find a "racer"
	VuCarEntity *pHumanCar = VuCarManager::IF()->getCar(VuCarManager::IF()->getCarCount() - 1);

	// Compare vs. the first human player
	//
	for (int i = 0; i < VuCarManager::IF()->getCarCount(); i++)
	{
		VuCarEntity* pCar = VuCarManager::IF()->getCar(i);
		if (pCar->getIsRacer())
		{
			pHumanCar = pCar;
			break;
		}
	}

	for (int i = 0; i < PACK_MAX; i++)
	{
		pMyInstance->mCarPacks[i].clear();
	}

	// Based on race positions, determine which eCarPack they sort into
	// relative to the first human player
	for (int i = 0; i < mAIObjects.size(); i++)
	{
		VuAiInstance* pInstance = mAIObjects[i];
		if (!pInstance)
			continue;

		VuCarEntity* pOtherCar = pInstance->getCar();
		if (!pOtherCar)
			continue;

		// Positive, the human is in front of me, negative, behind me
		float diff = pHumanCar->getStats().mDistFromStart - pOtherCar->getStats().mDistFromStart;

		if (diff > getGlobalConstants()->mPackSeparationDistance)
		{
			// Other car is behind human
			pMyInstance->mCarPacks[PACK_BEHIND].push_back(pOtherCar);
		}
		else if (diff < (-1.0f * getGlobalConstants()->mPackSeparationDistance))
		{
			// Human is front of us by some significant amount
			pMyInstance->mCarPacks[PACK_AHEAD].push_back(pOtherCar);
		}
		else
		{
			// Near us (between sep distance behind and sep distance ahead)
			pMyInstance->mCarPacks[PACK_NEAR].push_back(pOtherCar);
		}
	}

	// Sort each by distance
	for (int i = 0; i < PACK_MAX; i++)
	{
		pMyInstance->mCarPacks[i].sort(carDistSortFunction);
	}
}

//***************************************************************************
// Debug Functions
//*****************************************************************************
void VuAiManager::updateStatPage() const
{
	const char* aProgress[] = { "Early", "Mid  ", "Late " };
	const char* aLane[] = { "Left  ", "Center", "Right ", "None  " };
	const char* aSkill[] = { "Bad   ", "Normal", "Good  ", "Great ", "Balls " };
	const char* aPack[] = { "Behind", "Near  ", "Ahead ", "None  " };

	if(VuDevStatPage *pPage = VuDevStat::IF()->getCurPage())
	{
		if(pPage->getName() == STATPAGE_NAME)
		{
			pPage->clear();

			for(int i = 0; i < (int)mAIObjects.size(); i++)
			{
				VuAiInstance *pObject = mAIObjects[i];

				eAiCarPack carPack = pObject->getCarPack(pObject->getCar());

				pPage->printf("Driver : %s\n", pObject->mpDriver->getName());
				pPage->printf("%0.1f%% Pk: %s Rp: %s Cl: %s Bl: %s\n", pObject->mRaceCompletionPercent, aPack[carPack], aProgress[pObject->mCurRaceProgress], aLane[pObject->mCurLane], aLane[pObject->mBlockedLane]);

				pPage->printf("Bendiness: %#.2f\n", pObject->getAiDriver()->getPlan().computeBendinessAtTime(3.0f));

				//if(pObject->mpCurrentSector)
				//{
				//	pPage->printf("Sector : %d\n", pObject->mpCurrentSector->mpWaypoint->getShortName().c_str());
				//}

				pObject->mpDriver->updateStatPage(pPage);
				pPage->printf("-----\n");
			}
		}
	}
}

//*****************************************************************************
void VuAiManager::drawTrack() const
{

}

//*****************************************************************************
VuJsonContainer &VuAiManager::addDebugEvent(const char *pName, const char *pEventName, const VuVector3* pPos)
{
	VuJsonContainer &debugEvent = mDebugEvents[pName].append();

	debugEvent["Event"].putValue(pEventName);
	debugEvent["Time"].putValue(mRaceTimeElapsed);

	if (pPos)
	{
		debugEvent["Position"][0].putValue(pPos->mX);
		debugEvent["Position"][1].putValue(pPos->mY);
		debugEvent["Position"][2].putValue(pPos->mZ);
	}

	return debugEvent;
}

//*****************************************************************************
void VuAiManager::writeDebugEvents()
{
	if(mRecordDebugEvents)
	{
		VuJsonWriter writer;

		writer.saveToFile(mDebugEvents, DEBUG_EVENTS_FILENAME);
	}
}

//*****************************************************************************
void VuAiManager::renderDebugEvents(const char *pName)
{
	const VuJsonContainer &debugEvents = mDebugEvents[pName];

	for(int iEvent = 0; iEvent < debugEvents.size(); iEvent++)
	{
		const VuJsonContainer &debugEvent = debugEvents[iEvent];

		if(debugEvent["Event"].asString() == "SectorExit")
		{
			VuVector3 pos;

			pos.mX = debugEvent["Position"][0].asFloat();
			pos.mY = debugEvent["Position"][1].asFloat();
			pos.mZ = debugEvent["Position"][2].asFloat();

			VuDev::IF()->drawLine(pos, pos + VuVector3(0.0f, 0.0f, 1.0f), VuColor(255, 255, 255));
		}
	}
}
