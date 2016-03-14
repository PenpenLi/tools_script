//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Android Community manager
//
//*****************************************************************************

#pragma once

#include <jni.h>
#include "VuPurple/Managers/VuSkyGiftManager.h"

/*
#include <jni.h>
*/


class VuAndroidSkyGiftManager : public VuSkyGiftManager
{
public:
	// this function MUST be called from the application's JNI_OnLoad,
	// from a function known to be called by JNI_OnLoad, or from a function
	// in a Java-called thread.
	static void			bindJavaMethods(JNIEnv *jniEnv,jobject classLoaderObject, jmethodID findClassMethod);
	
	virtual void	showGiftUI( bool bshow = true);
};

extern "C"
{
	JNIEXPORT void JNICALL Java_com_skycn_android_request_TWGiftCallbackNative_giftResponse
		(JNIEnv * jniEnv, jobject jz, jstring str);
}