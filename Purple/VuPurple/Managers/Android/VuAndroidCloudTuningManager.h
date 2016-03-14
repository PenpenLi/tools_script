//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuAndroidCloudTuningManager
//
//*****************************************************************************

#pragma once

#include <jni.h>
#include "VuPurple/Managers/VuCloudTuningManager.h"


class VuAndroidCloudTuningManager : public VuCloudTuningManager
{
public:
	// this function MUST be called from the application's JNI_OnLoad,
	// from a function known to be called by JNI_OnLoad, or from a function
	// in a Java-called thread.
	static void		bindJavaMethods(JNIEnv *jniEnv, jobject classLoaderObject, jmethodID findClassMethod);

protected:
	virtual bool	wasGameConfigurationReceived();
	virtual void	getGameConfigurationValue(const std::string &key, std::string &value);
};

