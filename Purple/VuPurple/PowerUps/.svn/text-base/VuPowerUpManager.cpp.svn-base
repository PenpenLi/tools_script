//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  PowerUpManager class
// 
//*****************************************************************************

#include "VuPowerUpManager.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/Entities/Car/Driver/VuAiDriverEntity.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Assets/VuAssetFactory.h"
#include "VuEngine/Assets/VuSpreadsheetAsset.h"
#include "VuEngine/Assets/VuTextureAsset.h"
#include "VuEngine/Dev/VuDevConfig.h"


// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuPowerUpManager, VuPowerUpManager);

#define MAX_POWER_UP_GROUP_COUNT 16
#define MAX_POWER_UP_VARIATION_COUNT 32

//*****************************************************************************
VuPowerUpManager::VuPowerUpManager():
	mPowerUpsEnabled(true),
	mpForcePowerUp(VUNULL)
{
}

//*****************************************************************************
bool VuPowerUpManager::init()
{
	// load power ups from spreadsheet and db
	VuSpreadsheetAsset *pPowerUpSpreadsheetAsset = VuAssetFactory::IF()->createAsset<VuSpreadsheetAsset>("PowerUps");
	if ( pPowerUpSpreadsheetAsset == VUNULL )
		return false;

	const VuJsonContainer &groups = VuGameUtil::IF()->powerUpDB()["Groups"];

	int groupIndex = pPowerUpSpreadsheetAsset->getColumnIndex("Group");
	int powerUpIndex = pPowerUpSpreadsheetAsset->getColumnIndex("PowerUp");
	int priceIndex = pPowerUpSpreadsheetAsset->getColumnIndex("Price");

	for ( int iRow = 0; iRow < pPowerUpSpreadsheetAsset->getRowCount(); iRow++ )
	{
		const VuFastContainer &row = pPowerUpSpreadsheetAsset->getRow(iRow);

		const char *groupName = row[groupIndex].asCString();
		VuPowerUpGroup *pGroup = getGroupByName(groupName);
		if ( pGroup == VUNULL )
		{
			pGroup = createGroup(groupName, groups[groupName]);
			mGroups.push_back(pGroup);
		}

		const char *powerUpName = row[powerUpIndex].asCString();

		// create power-up
		VuPowerUp *pPowerUp = new VuPowerUp;
		pPowerUp->mName = powerUpName;
		pPowerUp->mHashedName = VuHash::fnv32String(pPowerUp->mName.c_str());
		pPowerUp->mPrice = row[priceIndex].asInt();
		pPowerUp->mpIconTextureAsset = VuAssetFactory::IF()->createAsset<VuTextureAsset>("UI/Powerup/" + pPowerUp->mName);

		mPowerUps.push_back(pPowerUp);
		pGroup->mPowerUps.push_back(pPowerUp);
	}

	VuAssetFactory::IF()->releaseAsset(pPowerUpSpreadsheetAsset);

	VUASSERT(getGroupCount() <= MAX_POWER_UP_GROUP_COUNT, "Exceeded max power-up group count!");

	int variationCount = 0;
	for ( Groups::const_iterator iter = mGroups.begin(); iter != mGroups.end(); iter++ )
		variationCount += (int)(*iter)->mVariations.size();
	VUASSERT(variationCount <= MAX_POWER_UP_VARIATION_COUNT, "Exceeded max power-up variation count!");

	if ( VuDevConfig::IF() && VuDevConfig::IF()->hasParam("ForcePowerUp") )
		mpForcePowerUp = getPowerUpByName(VuDevConfig::IF()->getParam("ForcePowerUp").asCString());

	return true;
}

//*****************************************************************************
void VuPowerUpManager::release()
{
	for ( Groups::iterator iter = mGroups.begin(); iter != mGroups.end(); iter++ )
		delete *iter;
	mGroups.clear();

	for ( PowerUps::iterator iter = mPowerUps.begin(); iter != mPowerUps.end(); iter++ )
	{
		VuAssetFactory::IF()->releaseAsset((*iter)->mpIconTextureAsset);
		delete *iter;
	}
	mPowerUps.clear();
}

//*****************************************************************************
VuPowerUpGroup *VuPowerUpManager::getGroupByName(const char *name)
{
	VUUINT32 hashedName = VuHash::fnv32String(name);

	for ( Groups::const_iterator iter = mGroups.begin(); iter != mGroups.end(); iter++ )
		if ( (*iter)->mHashedName == hashedName )
			return *iter;

	return VUNULL;
}

//*****************************************************************************
const VuPowerUp *VuPowerUpManager::choosePowerUp(VuCarEntity *pCar, VUUINT32 groupMask, int &charges)
{
	if ( mpForcePowerUp && pCar->getDriver()->isHuman() )
	{
		charges = 10;
		return mpForcePowerUp;
	}

	// calculate weights and total weight
	float weights[MAX_POWER_UP_VARIATION_COUNT];
	float totalWeight = 0.0f;
	{
		int groupIndex = 0;
		int weightIndex = 0;
		for ( Groups::const_iterator itGroup = mGroups.begin(); itGroup != mGroups.end(); itGroup++ )
		{
			if ( groupMask & (1<<groupIndex) )
			{
				VuPowerUpGroup *pGroup = *itGroup;
				for ( VuPowerUpGroup::Variations::const_iterator itVariation = pGroup->mVariations.begin(); itVariation != pGroup->mVariations.end(); itVariation++ )
				{
					float weight = itVariation->calcWeight(pCar);
					weights[weightIndex] = weight;
					totalWeight += weight;
					weightIndex++;
				}
			}
			groupIndex++;
		}
	}

	// calculate random weight
	float weight = VuRand::global().range(0.0f, totalWeight);

	// find powerup variation for weight
	{
		int groupIndex = 0;
		int weightIndex = 0;
		for ( Groups::const_iterator itGroup = mGroups.begin(); itGroup != mGroups.end(); itGroup++ )
		{
			if ( groupMask & (1<<groupIndex) )
			{
				VuPowerUpGroup *pGroup = *itGroup;
				for ( VuPowerUpGroup::Variations::const_iterator itVariation = pGroup->mVariations.begin(); itVariation != pGroup->mVariations.end(); itVariation++ )
				{
					if ( weight <= weights[weightIndex] )
					{
						charges = itVariation->mCharges;
						return choosePowerUpFromGroup(pCar, pGroup, charges);
					}
					weight -= weights[weightIndex];
					weightIndex++;
				}
			}
			groupIndex++;
		}
	}

	VUASSERT(0, "Impossible!");

	return VUNULL;
}

//*****************************************************************************
const VuPowerUp *VuPowerUpManager::choosePowerUpFromGroup(VuCarEntity *pCar, VuPowerUpGroup *pGroup, int &charges)
{
	if ( VuCarManager::IF()->getLocalHumanCarCount() >= 2 )
	{
		// for split-screen mode, do simple random lookup
		int powerUpIndex = VuRand::global().range(0, (int)pGroup->mPowerUps.size());
		return pGroup->mPowerUps[powerUpIndex];
	}
	else if ( pCar->getDriver()->isAi() )
	{
		// AI will use Powerups directly, so nothing to do here!
	}
	else if ( pCar->getDriver()->isLocal() && pCar->getDriver()->isHuman() )
	{
		// local human drivers randomly choose from their owned inventory
		VUASSERT(pGroup->mPowerUps.size() <= 16, "Too many power-ups in group!");
		const VuPowerUp *powerUps[16];
		int powerUpCount = 0;

		for ( auto pPowerUp : pGroup->mPowerUps )
			if ( VuGameManager::IF()->isPowerUpOwned(pPowerUp->mName) )
				powerUps[powerUpCount++] = pPowerUp;

		if ( powerUpCount )
		{
			const VuPowerUp *pPowerUp = powerUps[VuRand::global().range(0, powerUpCount)];
			return pPowerUp;
		}
	}

	// use default power-up
	return pGroup->getDefaultPowerUp();
}

//*****************************************************************************
const VuPowerUp *VuPowerUpManager::getPowerUpByName(const char *name)
{
	VUUINT32 hashedName = VuHash::fnv32String(name);

	for ( PowerUps::const_iterator iter = mPowerUps.begin(); iter != mPowerUps.end(); iter++ )
		if ( (*iter)->mHashedName == hashedName )
			return *iter;

	return VUNULL;
}

//*****************************************************************************
float VuPowerUpGroup::VuVariation::calcWeight(VuCarEntity *pCar) const
{
	float weight = 1.0f;

	switch ( mWeightMethod )
	{
		case WM_Fixed :
		{
			weight = mFixedWeight;
			break;
		}
		case WM_DistBehindLeader :
		{
			const VuCarStats &stats = pCar->getStats();
			weight = VuMathUtil::interpolateLinearCurve(stats.mDistBehindLeader, &mLinearWeightCurve[0], (int)mLinearWeightCurve.size());
			break;
		}
	}

	return weight;
}

//*****************************************************************************
VuPowerUp *VuPowerUpGroup::getPowerUp(const char *name)
{
	for ( PowerUps::iterator iter = mPowerUps.begin(); iter != mPowerUps.end(); iter++ )
	{
		VuPowerUp *pPowerUp = *iter;
		if ( pPowerUp->mName == name)
			return pPowerUp;
	}

	return VUNULL;
}

//*****************************************************************************
void VuPowerUpGroup::appendPowerUpNames(std::vector<const char*>& list)
{
	for (PowerUps::iterator iter = mPowerUps.begin(); iter != mPowerUps.end(); iter++)
	{
		list.push_back((*iter)->mName.c_str());
	}
}

//*****************************************************************************
VuPowerUpGroup *VuPowerUpManager::createGroup(const char *groupName, const VuJsonContainer &groupData)
{
	VuPowerUpGroup *pGroup = new VuPowerUpGroup;
	pGroup->mName = groupName;
	pGroup->mHashedName = VuHash::fnv32String(groupName);

	// load variations
	const VuJsonContainer &variations = groupData["Variations"];
	pGroup->mVariations.resize(variations.size());
	for ( int i = 0; i < variations.size(); i++ )
	{
		const VuJsonContainer &data = variations[i];
		VuPowerUpGroup::VuVariation &variation = pGroup->mVariations[i];

		data["Charges"].getValue(variation.mCharges);
		if ( data["FixedWeight"].getValue(variation.mFixedWeight) )
		{
			variation.mWeightMethod = VuPowerUpGroup::VuVariation::WM_Fixed;
		}
		else if ( data.hasMember("DistBehindLeaderWeight") )
		{
			variation.mWeightMethod = VuPowerUpGroup::VuVariation::WM_DistBehindLeader;

			const VuJsonContainer &weightData = data["DistBehindLeaderWeight"];
			const VuJsonContainer &dists = weightData["Dist"];
			const VuJsonContainer &weights = weightData["Weight"];
			int count = VuMax(dists.size(), weights.size());
			count = VuMax(count, 2);

			variation.mLinearWeightCurve.resize(count);
			for ( int i = 0; i < count; i++ )
			{
				variation.mLinearWeightCurve[i].mX = dists[i].asFloat();
				variation.mLinearWeightCurve[i].mY = weights[i].asFloat();
			}
		}
	}

	return pGroup;
}
