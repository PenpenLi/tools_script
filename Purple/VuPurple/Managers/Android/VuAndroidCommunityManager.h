//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Android Community manager
//
//*****************************************************************************

#include <jni.h>
#include "VuPurple/Managers/VuCommunityManager.h"


class VuAndroidCommunityManager : public VuCommunityManager
{
public:
	// this function MUST be called from the application's JNI_OnLoad,
	// from a function known to be called by JNI_OnLoad, or from a function
	// in a Java-called thread.
	static void			bindJavaMethods(JNIEnv *jniEnv, jobject classLoaderObject, jmethodID findClassMethod);

	virtual void		showWebPage(const std::string &url);
	virtual void		showMoreGames(const std::string &packageName, const std::string &publisherName);
	virtual void		showTwitterPage(const std::string &userName);
	virtual void		showFacebookPage(const std::string &id, const std::string &userName);
	virtual void		showGooglePlusPage(const std::string &id);
	virtual void		rateGame(const std::string &packageName);
};
