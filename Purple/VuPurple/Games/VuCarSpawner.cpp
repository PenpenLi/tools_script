//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuCarSpawner
// 
//*****************************************************************************

#include "VuCarSpawner.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/Entities/Car/Driver/VuAiDriverEntity.h"
#include "VuPurple/Entities/Objects/VuAiWaypointEntity.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/AI/VuAiManager.h"
#include "VuPurple/Track/VuTrackManager.h"
#include "VuPurple/Track/VuTrackSector.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Entities/VuEntityFactory.h"
#include "VuEngine/Assets/VuAssetFactory.h"
#include "VuEngine/Assets/VuAnimatedModelAsset.h"
#include "VuEngine/Assets/VuStaticModelAsset.h"
#include "VuEngine/Assets/VuFluidsMeshAsset.h"
#include "VuEngine/Assets/VuTextureAsset.h"


// constants
#define SPAWN_TIMER 1.0f


//*****************************************************************************
VuCarSpawner::VuCarSpawner():
	mSpawnCount(0),
	mSpawnTimer(0.0f)
{
}

//*****************************************************************************
VuCarSpawner::~VuCarSpawner()
{
	for ( int i = 0; i < (int)mPreloadedCars.size(); i++ )
		mPreloadedCars[i]->removeRef();

	for ( int i = 0; i < (int)mCarAssets.size(); i++ )
		VuAssetFactory::IF()->releaseAsset(mCarAssets[i]);
}

//*****************************************************************************
void VuCarSpawner::configure(const VuJsonContainer &data)
{
	mMaxCarCount = data["MaxCarCount"].asInt();
	mFrontSpawnDistance = data["FrontSpawnDistance"].asFloat();
	mBackKillDistance = data["BackKillDistance"].asFloat();
	mCarVarietyCount = data["CarVarietyCount"].asInt();
	mDriverVarietyCount = data["DriverVarietyCount"].asInt();

	mOptimalSpacing = (mFrontSpawnDistance + mBackKillDistance)/mMaxCarCount;
	mFrontKillDistance = mFrontSpawnDistance + mOptimalSpacing;

	mCarData["Ai"] = data["Ai"];
	mCarData["Properties"]["DriverType"].putValue("AI");
}

//*****************************************************************************
void VuCarSpawner::load(const std::string &playerCar, const std::string &playerDriver, const std::string &playerDecal)
{
	// gather car names (excluding player car name)
	Names carNames;
	const VuJsonContainer &carNameData = VuGameUtil::IF()->constantDB()["Names"]["Cars"];
	for ( int i = 0; i < carNameData.size(); i++ )
		carNames.push_back(carNameData[i].asString());

	// gather driver names (excluding player driver name)
	Names driverNames;
	const VuJsonContainer &driverNameData = VuGameUtil::IF()->constantDB()["Names"]["Drivers"];
	for ( int i = 0; i < driverNameData.size(); i++ )
		driverNames.push_back(driverNameData[i].asString());

	// gather color names
	for ( int i = 0; i < VuGameUtil::IF()->colorDB().size(); i++ )
		mColorNames.push_back(VuGameUtil::IF()->colorDB()[i]["Name"].asString());

	createNames(carNames, mCarNames, playerCar, mCarVarietyCount);
	createNames(driverNames, mDriverNames, playerDriver, mDriverVarietyCount);

	// preload cars
	for ( auto &carName : mCarNames )
	{
		mCarData["Properties"]["Car"].putValue(carName);
		mCarData["Properties"]["Driver"].putValue(playerDriver);
		mCarData["Properties"]["Decal"].putValue(playerDecal);

		VuCarEntity *pCar = VuEntityFactory::IF()->createEntity<VuCarEntity>();

		pCar->load(mCarData);
		pCar->postLoad();

		mPreloadedCars.push_back(pCar);
	}

	// load driver assets
	for ( auto &driverName : mDriverNames )
	{
		const VuJsonContainer &driverData = VuGameUtil::IF()->driverDB()[driverName];

		mCarAssets.push_back(VuAssetFactory::IF()->createAsset<VuTextureAsset>(driverData["PortraitImage"].asString()));
		mCarAssets.push_back(VuAssetFactory::IF()->createAsset<VuTextureAsset>(driverData["AbilityImage"].asString()));
		mCarAssets.push_back(VuAssetFactory::IF()->createAsset<VuAnimatedModelAsset>(driverData["Model Asset"].asString()));
		mCarAssets.push_back(VuAssetFactory::IF()->createAsset<VuAnimatedModelAsset>(driverData["LOD 1 Model Asset"].asString()));
		mCarAssets.push_back(VuAssetFactory::IF()->createAsset<VuStaticModelAsset>(driverData["LOD 2 Model Asset"].asString()));

		const VuJsonContainer &skinData = VuGameUtil::IF()->carSkinDB()[driverData["AiSkin"].asString()];
		std::string decalTexture;
		if ( VuGameUtil::IF()->getDecal(skinData["Decal"].asString(), decalTexture) )
			mCarAssets.push_back(VuAssetFactory::IF()->createAsset<VuTextureAsset>(decalTexture));
	}
}

//*****************************************************************************
void VuCarSpawner::begin()
{
	mTrackLength = VuTrackManager::IF()->getLength();
}

//*****************************************************************************
void VuCarSpawner::end()
{
	for ( int i = 0; i < (int)mActiveCars.size(); i++ )
	{
		VuAiManager::IF()->removeFromRace(mActiveCars[i]);
		mActiveCars[i]->gameRelease();
		mActiveCars[i]->removeRef();
	}
	mActiveCars.clear();
}

//*****************************************************************************
void VuCarSpawner::update(float fdt)
{
	VuCarEntity *pPlayerCar = VuCarManager::IF()->getCameraTargetForViewport(0);
	if ( VuCarManager::IF()->getLocalHumanCarCount() )
		pPlayerCar = VuCarManager::IF()->getLocalHumanCar(0);

	// kill cars
	for ( int iCar = 0; iCar < (int)mActiveCars.size(); iCar++ )
	{
		VuCarEntity *pCar = mActiveCars[iCar];

		float distFromPlayer = pCar->getStats().mDistFromPlayer;

		if ( distFromPlayer < -mBackKillDistance || distFromPlayer > mFrontKillDistance )
		{
			VuAiManager::IF()->removeFromRace(pCar);
			pCar->gameRelease();
			pCar->removeRef();
			mActiveCars.erase(std::find(mActiveCars.begin(), mActiveCars.end(), pCar));
			break;
		}
	}

	// spawn new cars
	if ( mSpawnTimer > 0.0f )
	{
		mSpawnTimer -= fdt;
	}
	else if ((int)mActiveCars.size() < mMaxCarCount && pPlayerCar->getStats().mDistFromStart > 0.0f)
	{
		// check if any cars are close to spawn point
		float maxDistFromPlayer = -FLT_MAX;
		for ( int i = 0; i < (int)mActiveCars.size(); i++ )
			maxDistFromPlayer = VuMax(maxDistFromPlayer, mActiveCars[i]->getStats().mDistFromPlayer);

		if ( maxDistFromPlayer < mFrontSpawnDistance - mOptimalSpacing )
		{
			VuVector3 spawnPos;
			const VuTrackSector *pSector;
			if ( calcPointAlongRacingLine(pPlayerCar, mFrontSpawnDistance, spawnPos, pSector) )
			{
				VuCarEntity *pCar = createCar();

				pCar->setIsSpawned(true);

				// initialize car
				VuMatrix xform;
				VuGameUtil::buildSafeMatrixForCar(spawnPos, pSector->mUnitDir, pCar->getCollisionAabb(), xform, true);

				pCar->getTransformComponent()->setWorldTransform(xform);
				pCar->gameInitialize();

				mActiveCars.push_back(pCar);

				// configure driver
				VuAiDriver *pDriver = static_cast<VuAiDriver *>(pCar->getDriver());
				mSpawnTimer = SPAWN_TIMER;

				VuAiManager::IF()->addToRace(pCar, 0.0f, 0.0f);
				pDriver->replan(pSector);
			}
		}
	}

	// update progress
	for ( int iCar = 0; iCar < (int)mActiveCars.size(); iCar++ )
	{
		VuCarEntity *pCar = mActiveCars[iCar];
		VuCarStats *pStats = &pCar->getStats();

		const VuTrackSector *pCurrentSector = pCar->getDriver()->getCurrentSector();

		if ( pCurrentSector )
		{
			pStats->mDistFromStartOfLap = pCurrentSector->mDistanceFromStart + (pCurrentSector->mLength*pCurrentSector->traversalAmount(pCar->getModelPosition()));

			float diff = pStats->mDistFromStartOfLap - pPlayerCar->getStats().mDistFromStartOfLap;

			// get diff into -trackLength/2 -> tracklength/2 range
			diff /= mTrackLength;
			diff += 0.5f;
			diff -= VuFloor(diff);
			diff -= 0.5f;
			diff *= mTrackLength;

			pStats->mDistFromPlayer = diff;
		}
	}
}

//*****************************************************************************
void VuCarSpawner::createNames(const Names &srcNames, Names &dstNames, const std::string &ignoreName, int count)
{
	// randomize names
	std::vector<int> shuffleArray;
	shuffleArray.resize(srcNames.size());
	VuRand::global().createShuffleArray((int)shuffleArray.size(), &shuffleArray[0]);

	for ( int i = 0; i < (int)shuffleArray.size(); i++ )
		dstNames.push_back(srcNames[shuffleArray[i]]);

	// ignoreName
	Names::iterator iter = std::find(dstNames.begin(), dstNames.end(), ignoreName);
	if ( iter != dstNames.end() )
		dstNames.erase(iter);

	// shrink if necessary
	if ( count && ((int)dstNames.size() > count) )
		dstNames.resize(count);
}

//*****************************************************************************
VUUINT32 VuCarSpawner::calcPointAlongRacingLine(VuCarEntity *pCar, float distance, VuVector3 &pos, const VuTrackSector *&pSector)
{
	pSector = pCar->getDriver()->getCurrentSector();
	VUASSERT(pSector, "VuAiManager::distFromStart() missing sector");

	VuVector3 curPos = pCar->getModelPosition();

	float t = pSector->traversalAmount(curPos);
	float distToEndOfSector = (1.0f - t)*pSector->mLength;
	if ( distance <= distToEndOfSector )
	{
		pSector->pointAlongRacingLine(t + distance/pSector->mLength, pos);
		return pSector->mpWaypoint->allowRecovery();
	}
	else
	{
		distance -= distToEndOfSector;

		while ( pSector->mNextSectorCount )
		{
			pSector = pSector->mpNextSectors[0];
			if ( distance < pSector->mLength )
			{
				pSector->pointAlongRacingLine(distance/pSector->mLength, pos);
				return pSector->mpWaypoint->allowRecovery();
			}
			distance -= pSector->mLength;
		}
	}

	return 0;
}

//*****************************************************************************
VuCarEntity *VuCarSpawner::createCar()
{
	int preAssetsLoadedCount = VuAssetFactory::IF()->getAssetsLoadedCount();

	const std::string &carName = mCarNames[VuRand::global().range(0, (int)mCarNames.size())];
	const std::string &driverName = mDriverNames[VuRand::global().range(0, (int)mDriverNames.size())];
	const VuJsonContainer &skinData = VuGameUtil::IF()->carSkinDB()[VuGameUtil::IF()->driverDB()[driverName]["AiSkin"].asString()];

	VuJsonContainer &properties = mCarData["Properties"];

	properties["Car"].putValue(carName);
	properties["Driver"].putValue(driverName);
	properties["Decal"] = skinData["Decal"];
	properties["PaintColor"] = skinData["PaintColor"];
	properties["DecalColor"] = skinData["DecalColor"];

	VuCarEntity *pCar = VuEntityFactory::IF()->createEntity<VuCarEntity>();

	char name[64];
	VU_SPRINTF(name, sizeof(name), "SpawnedCar%02d", ++mSpawnCount);
	pCar->setShortName(name);

	pCar->load(mCarData);
	pCar->postLoad();

	int postAssetsLoadedCount = VuAssetFactory::IF()->getAssetsLoadedCount();
	VUASSERT(preAssetsLoadedCount == postAssetsLoadedCount, "In-game asset load from car spawner!");

	return pCar;
}
