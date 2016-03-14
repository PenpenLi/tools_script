//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Android Analytics manager
//
//*****************************************************************************

#include <jni.h>
#include "VuPurple/Managers/VuAnalyticsManager.h"


class VuAndroidAnalyticsManager : public VuAnalyticsManager
{
public:
	// this function MUST be called from the application's JNI_OnLoad,
	// from a function known to be called by JNI_OnLoad, or from a function
	// in a Java-called thread.
	static void			bindJavaMethods(JNIEnv *jniEnv, jobject classLoaderObject, jmethodID findClassMethod);

	virtual bool		init();

	virtual void		logEventInternal(const char *eventName, const char *paramName, const char *paramValue, const VuJsonContainer &variables);
	virtual void		logEventInternal(const char* eventName, const char* paramKey, int value);
	virtual void		setLevelInternal(int level);
	virtual void		setCurrencyInternal(int type, int balance);
	virtual void		displayNotificationsInternal();
};
