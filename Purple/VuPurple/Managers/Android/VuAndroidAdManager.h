//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Android Advertising manager
//
//*****************************************************************************

#include <jni.h>
#include "VuPurple/Managers/VuAdManager.h"
#include "VuEngine/Events/VuEventMap.h"


class VuAndroidAdManager : public VuAdManager
{
	DECLARE_EVENT_MAP

public:
	// this function MUST be called from the application's JNI_OnLoad,
	// from a function known to be called by JNI_OnLoad, or from a function
	// in a Java-called thread.
	static void		bindJavaMethods(JNIEnv *jniEnv, jobject classLoaderObject, jmethodID findClassMethod);

	// platform-specific functionality
	static VuAndroidAdManager *IF() { return static_cast<VuAndroidAdManager *>(VuAdManager::IF()); }

	VuAndroidAdManager();

protected:
	// event handlers
	void			OnAndroidAdFinished(const VuParams &params);

	virtual bool	isAdReadyInternal(int adUnit);
	virtual void	showAdInternal(int adUnit);
	virtual void	checkForAdsInternal();
};
