//*****************************************************************************
//
//  Copyright (c) 2009-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  AssetConvertGameMode class
// 
//*****************************************************************************

#include "VuAssetConvertGameMode.h"
#include "VuEngine/Assets/VuAssetFactory.h"
#include "VuEngine/Json/VuJsonReader.h"
#include "VuEngine/Json/VuJsonWriter.h"
#include "VuEngine/Util/VuDataUtil.h"


IMPLEMENT_RTTI(VuAssetConvertGameMode, VuAssetGameMode);


//*****************************************************************************
bool VuAssetConvertGameMode::enter(const std::string &prevMode)
{
	if ( !VuAssetGameMode::enter(prevMode) )
		return false;

	VUPRINTF("Converting all assets...\n");

	return true;
}

//*****************************************************************************
void VuAssetConvertGameMode::exit()
{
	VuAssetGameMode::exit();
}

//*****************************************************************************
const char *VuAssetConvertGameMode::tick(float fdt)
{
	std::string strType, strName;
	while ( enumNextAsset(strType, strName) )
	{
		// convert asset
		if ( strType == "VuTextureAsset" )
		{
			// do something with this file
			if ( 1 )
			{
				VUPRINTF("Converted '%s'\n", strName.c_str());

				return "";
			}
			else
			{
				VUPRINTF("\nERROR: Failed to convert '%s/%s'\n", strType.c_str(), strName.c_str());
				return "Result";
			}
		}
	}

	return "Result"; // next game mode
}
