//*****************************************************************************
//
//  Copyright (c) 2007-2008 Ralf Knoesel
//  Copyright (c) 2008-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  For tracking global use of powerups by AI
// 
//*****************************************************************************

#include "VuPurple/AI/VuAiPowerUpTracker.h"

#include "VuPurple/PowerUps/VuPowerUpManager.h"
#include "VuEngine/Util/VuHash.h"

//*****************************************************************************
VuAiPowerUpTracker::VuAiPowerUpTracker()
{

}

//*****************************************************************************
void VuAiPowerUpTracker::init()
{
	mAllPowerUps.clear();
	mAllPowerUps.reserve(32);

	mPowerUpMap.clear();

	buildFor("Throw");
	buildFor("Seek");
	buildFor("Global");
	buildFor("LongShot");
	buildFor("Defense");
	buildFor("Shield");
	buildFor("Boost");
	buildFor("SpikedTires");
}

//*****************************************************************************
void VuAiPowerUpTracker::raceStart()
{
	for (auto pInfo : mAllPowerUps)
	{
		pInfo->mUsageCount = 0;
	}
}

//*****************************************************************************
void VuAiPowerUpTracker::release()
{
	for (auto pInfo : mAllPowerUps)
	{
		delete pInfo;
	}
}

//*****************************************************************************
const char* VuAiPowerUpTracker::getBestPowerUpByGroup(const char* pGroupName)
{
	VUUINT32 groupNameHash = VuHash::fnv32String(pGroupName);

	TrackingList& list = mPowerUpMap[groupNameHash];

	TrackingItem* pLeastUsed = VUNULL;
	VUUINT32 leastUsedCount = list[0]->mUsageCount;

	for (auto pItem : list)
	{
		if (pItem->mUsageCount < leastUsedCount)
		{
			leastUsedCount = pItem->mUsageCount;
			pLeastUsed = pItem;
		}
	}

	if (pLeastUsed == VUNULL)
	{
		// Couldn't find one used less than the first one, must be all used
		// the same amount. Shuffle, select and repeat.
		std::random_shuffle(list.begin(), list.end());

		return list[0]->mpName;
	}
	else
	{
		return pLeastUsed->mpName;
	}
}

//*****************************************************************************
void VuAiPowerUpTracker::powerUpWasUsed(const char* pPowerUpName)
{
	VUUINT32 nameHash = VuHash::fnv32String(pPowerUpName);

	for (auto pInfo : mAllPowerUps)
	{
		if (pInfo->mPowerUpHashName == nameHash)
		{
			pInfo->mUsageCount++;
			break;
		}
	}
}

//*****************************************************************************
void VuAiPowerUpTracker::buildFor(const char* pGroupName)
{
	VUUINT32 groupNameHash = VuHash::fnv32String(pGroupName);

	std::vector<const char*> tempList;

	VuPowerUpGroup* pGroup = VuPowerUpManager::IF()->getGroupByName(pGroupName);
	
	if (!pGroup)
	{
		return;
	}

	// Get the list of powerup names
	pGroup->appendPowerUpNames(tempList);

	TrackingList groupTrackingList;

	for (auto pName : tempList)
	{
		VUUINT32 powerUpHashName = VuHash::fnv32String(pName);

		TrackingItem* pItem = new TrackingItem;
		if (pItem)
		{
			pItem->mPowerUpHashName = powerUpHashName;
			pItem->mUsageCount = 0;
			pItem->mpName = pName;
			mAllPowerUps.push_back(pItem);

			groupTrackingList.push_back(pItem);
		}
	}
	
	mPowerUpMap[groupNameHash] = groupTrackingList;
}

