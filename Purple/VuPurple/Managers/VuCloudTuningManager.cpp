//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuCloudTuningManager
// 
//*****************************************************************************

#include "VuCloudTuningManager.h"
#include "VuEngine/Assets/VuAssetFactory.h"
#include "VuEngine/Assets/VuDBAsset.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Json/VuJsonReader.h"
#include "VuEngine/Dev/VuDevConfig.h"


#define CLOUD_TUNING_CHECK_DELAY 1.0f // seconds


//*****************************************************************************
VuCloudTuningManager::VuCloudTuningManager():
	mTimeOut(0.0f)
{
}

//*****************************************************************************
bool VuCloudTuningManager::init()
{
	mpTuningDB = VuAssetFactory::IF()->createAsset<VuDBAsset>("TuningDB");

	mTuningDB = mpTuningDB->getDB();

	setVariables();

	if ( !VuDevConfig::IF()->getParam("DisableCloudTuning").asBool() )
		VuTickManager::IF()->registerHandler(this, &VuCloudTuningManager::tickNetwork, "Network");

	return true;
}

//*****************************************************************************
void VuCloudTuningManager::release()
{
	VuAssetFactory::IF()->releaseAsset(mpTuningDB);

	VuTickManager::IF()->unregisterHandler(this, "Network");
}

//*****************************************************************************
void VuCloudTuningManager::tickNetwork(float fdt)
{
	mTimeOut -= fdt;
	 
	if ( mTimeOut <= 0.0f )
	{
		mTimeOut = CLOUD_TUNING_CHECK_DELAY;

		if ( wasGameConfigurationReceived() )
		{
			VuTickManager::IF()->unregisterHandler(this, "Network");

			VUPRINTF("Game Configuration Received:\n");

			std::string value;

			for ( int iKey = 0; iKey < mTuningDB.numMembers(); iKey++ )
			{
				const std::string &key = mTuningDB.getMemberKey(iKey);

				value.clear();
				getGameConfigurationValue(key, value);
				if ( value.length() )
				{
					VuJsonReader reader;
					VuJsonContainer data;
					if ( reader.loadFromString(data, value) )
					{
						mTuningDB[key] = data;
						VUPRINTF("\t%s = %s\n", key.c_str(), value.c_str());
					}
				}
			}

			setVariables();
		}
	}
}

//*****************************************************************************
void VuCloudTuningManager::setVariables()
{
	mVariables.mTokenRefillPrice = mTuningDB["TokenRefillPrice"].asInt();
	mVariables.mTokenRefillTime = mTuningDB["TokenRefillTime"].asInt();
	mVariables.mMaxTokens = mTuningDB["MaxTokens"].asInt();
}
