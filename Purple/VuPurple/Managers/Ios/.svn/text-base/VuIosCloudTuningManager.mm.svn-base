//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuIosCloudTuningManager
//
//*****************************************************************************

#import <FuseApi.h>

#include "VuIosCloudTuningManager.h"


// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuCloudTuningManager, VuIosCloudTuningManager);

// static variables
bool VuIosCloudTuningManager::sGameConfigurationReceived = false;


//*****************************************************************************
void VuIosCloudTuningManager::getGameConfigurationValue(const std::string &key, std::string &value)
{
	NSString* keyString = [NSString stringWithUTF8String:key.c_str()];
	NSString* valueString = [FuseAPI getGameConfigurationValue:keyString];
	
	if ( const char *strValue = [valueString UTF8String] )
		value = strValue;
}
