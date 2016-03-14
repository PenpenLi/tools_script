//*****************************************************************************
//
//  Copyright (c) 2008-2008 Ralf Knoesel
//  Copyright (c) 2008-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  AssetGameMode class
// 
//*****************************************************************************

#include "VuAssetGameMode.h"
#include "VuPurple/Managers/VuFadeManager.h"
#include "VuEngine/VuEngine.h"
#include "VuEngine/Assets/VuAssetFactory.h"
#include "VuEngine/Managers/VuViewportManager.h"
#include "VuEngine/Json/VuJsonContainer.h"
#include "VuEngine/Json/VuJsonReader.h"
#include "VuEngine/Util/VuCmdLineArgs.h"
#include "VuEngine/Util/VuFilterExpression.h"
#include "VuEngine/Dev/VuDevConsole.h"
#include "VuEngine/Dev/VuDevConfig.h"


IMPLEMENT_RTTI(VuAssetGameMode, VuGameMode);


//*****************************************************************************
bool VuAssetGameMode::enter(const std::string &prevMode)
{
	// instantly fade out
	VuFadeManager::IF()->startFadeOut(0.0f);

	VUPRINTF("\n");

	VuDevConsole::IF()->show(true);
	VuViewportManager::IF()->reset();
	VuViewportManager::IF()->setViewportCount(1);
	mStartTime = VuSys::IF()->getTime();

	// dev mode?
	const VuJsonContainer &devPackageInfo = VuAssetFactory::IF()->getDevPackageInfo();
	if ( devPackageInfo.isNull() )
		return VUERROR("No dev package info available!");

	// determine sku
	VuEngine::IF()->cmdLineArgs().getValue("Sku", mSku);
	if ( mSku.empty() )
		return VUERROR("No SKU specified!");

	// determine version
	if ( !devPackageInfo["Version"].getValue(mVersion) )
		return VUERROR("No 'Version' specified in dev package!");

	// load asset names for asset DB
	if ( !loadAssetNames() )
		return false;

	VUPRINTF("Processing assets...\n");

	return true;
}

//*****************************************************************************
void VuAssetGameMode::exit()
{
	VUPRINTF("Total time: %.2fs\n", (float)(VuSys::IF()->getTime() - mStartTime));
	VuViewportManager::IF()->reset();
	VuDevConsole::IF()->show(false);
}

//*****************************************************************************
bool VuAssetGameMode::enumNextAsset(std::string &strType, std::string &strName)
{
	while ( true )
	{
		// get current type
		if ( mCurAssetType >= (int)mAssetTypes.size() )
			return false;
		AssetTypes::const_iterator itTypes = mAssetTypes.begin();
		for ( int i = 0; i < mCurAssetType; i++ )
			itTypes++;

		// get current name
		strType = itTypes->first;
		const AssetNames &assetNames = itTypes->second;
		if ( mCurAssetName < (int)assetNames.size() )
		{
			strName = assetNames[mCurAssetName];
			mCurAssetName++;
			mCurAssetCount++;
			return true;
		}

		// next type
		mCurAssetType++;
		mCurAssetName = 0;
	}
}

//*****************************************************************************
bool VuAssetGameMode::loadAssetNames()
{
	// load TOC
	VuJsonContainer dbData;
	VuJsonReader reader;
	if ( !reader.loadFromFile(dbData, VuAssetFactory::IF()->getDevPackageInfo()["Assets"].asString()) )
		return VUWARNING(reader.getLastError().c_str());

	// load asset data for each type
	for ( int iType = 0; iType < dbData.numMembers(); iType++ )
	{
		const std::string &typeName = dbData.getMemberKey(iType);
		const std::string &fileName = dbData[typeName].asString();

		VuJsonContainer assetData;
		if ( !reader.loadFromFile(assetData, fileName) )
			return VUWARNING(reader.getLastError().c_str());

		AssetType assetType;
		assetType.first = typeName;
		for ( int iAsset = 0; iAsset < assetData.numMembers(); iAsset++ )
		{
			const std::string &assetName = assetData.getMemberKey(iAsset);
			const VuJsonContainer &assetInfo = assetData[assetName];

			// account for filter
			bool filterResult = true;
			if ( assetInfo["Filter"].isString() )
			{
				VuFilterExpression filter;
				filter.addVariable("sku", mSku.c_str());
				if ( !filter.evaluate(assetInfo["Filter"].asCString()) )
					return VUWARNING(filter.getLastError());
				filterResult = filter.result();
			}

			if ( filterResult)
			{
				assetType.second.push_back(assetName);
			}
		}

		mAssetTypes.push_back(assetType);

		mTotalAssetCount += (int)assetType.second.size();
	}

	// sort DBs
	mAssetTypes.sort(assetTypeComp);

	return true;
}

//*****************************************************************************
bool VuAssetGameMode::assetTypeComp(const AssetType &assetType0, const AssetType &assetType1)
{
	int pri0 = VuAssetFactory::IF()->getAssetTypePriority(assetType0.first);
	int pri1 = VuAssetFactory::IF()->getAssetTypePriority(assetType1.first);

	return pri0 < pri1;
}
