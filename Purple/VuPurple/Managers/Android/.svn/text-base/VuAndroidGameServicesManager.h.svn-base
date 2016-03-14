//*****************************************************************************
//
//  Copyright (c) 2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuAndroidGameServicesManager
//
//*****************************************************************************

#pragma once

#include <jni.h>
#include "VuPurple/Managers/VuGameServicesManager.h"
#include "VuEngine/Events/VuEventMap.h"


class VuAndroidGameServicesManager : public VuGameServicesManager
{
	DECLARE_EVENT_MAP

public:
	// this function MUST be called from the application's JNI_OnLoad,
	// from a function known to be called by JNI_OnLoad, or from a function
	// in a Java-called thread.
	static void		bindJavaMethods(JNIEnv *jniEnv, jobject classLoaderObject, jmethodID findClassMethod);

	VuAndroidGameServicesManager();

	// platform-specific functionality
	static VuAndroidGameServicesManager *IF() { return static_cast<VuAndroidGameServicesManager *>(VuGameServicesManager::IF()); }

protected:
	// event handlers
	void			OnAndroidSignInSuccess(const VuParams &params);
	void			OnAndroidSignInFailure(const VuParams &params);
	void			OnAndroidSignOut(const VuParams &params);
	void			OnAndroidAddFriend(const VuParams &params);
	void			OnAndroidFinalizeFriends(const VuParams &params);
	void			OnAndroidAchievementState(const VuParams &params);

	virtual void	startSignInAsync();
	virtual void	startSignOutAsync();
	virtual void	setAchievementUnlocked(const char *nativeId);
	virtual void	submitScoreAsync(const char *nativeId, int value);
	virtual void	showAchievements();

	Friends			mTempFriends;
};
