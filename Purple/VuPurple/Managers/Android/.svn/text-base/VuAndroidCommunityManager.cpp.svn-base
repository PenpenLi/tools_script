//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Android Community manager
//
//*****************************************************************************

#include "VuAndroidCommunityManager.h"


// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuCommunityManager, VuAndroidCommunityManager);

// static JAVA stuff
static JNIEnv		*s_jniEnv;
static jobject		s_helperObject;
static jmethodID	s_showWebPage;
static jmethodID	s_showMoreGames;
static jmethodID	s_showTwitterPage;
static jmethodID	s_showFacebookPage;
static jmethodID	s_showGooglePlusPage;
static jmethodID	s_rateGame;


//*****************************************************************************
void VuAndroidCommunityManager::bindJavaMethods(JNIEnv *jniEnv, jobject classLoaderObject, jmethodID findClassMethod)
{
	__android_log_print(ANDROID_LOG_DEBUG, "ofm",  "VuAndroidCommunityManager::bindJavaMethods()\n");

	s_jniEnv = jniEnv;

	// get reference to helper class object
	jstring helperClassName = jniEnv->NewStringUTF("com/vectorunit/VuCommunityHelper");
	jclass helperClass = (jclass)jniEnv->CallObjectMethod(classLoaderObject, findClassMethod, helperClassName);
	jniEnv->DeleteLocalRef(helperClassName);

	jmethodID getInstance = jniEnv->GetStaticMethodID(helperClass, "getInstance", "()Lcom/vectorunit/VuCommunityHelper;");
	jobject helperObject = jniEnv->CallStaticObjectMethod(helperClass, getInstance);
	s_helperObject = jniEnv->NewGlobalRef(helperObject);

	// methods
	s_showWebPage = jniEnv->GetMethodID(helperClass, "showWebPage", "(Ljava/lang/String;)V");
	s_showMoreGames = jniEnv->GetMethodID(helperClass, "showMoreGames", "(Ljava/lang/String;Ljava/lang/String;)V");
	s_showTwitterPage = jniEnv->GetMethodID(helperClass, "showTwitterPage", "(Ljava/lang/String;)V");
	s_showFacebookPage = jniEnv->GetMethodID(helperClass, "showFacebookPage", "(Ljava/lang/String;Ljava/lang/String;)V");
	s_showGooglePlusPage = jniEnv->GetMethodID(helperClass, "showGooglePlusPage", "(Ljava/lang/String;)V");
	s_rateGame = jniEnv->GetMethodID(helperClass, "rateGame", "(Ljava/lang/String;)V");
}

//*****************************************************************************
void VuAndroidCommunityManager::showWebPage(const std::string &url)
{
    jstring jUrl = s_jniEnv->NewStringUTF(url.c_str());
	s_jniEnv->CallVoidMethod(s_helperObject, s_showWebPage, jUrl);
    s_jniEnv->DeleteLocalRef(jUrl);
}

//*****************************************************************************
void VuAndroidCommunityManager::showMoreGames(const std::string &packageName, const std::string &publisherName)
{
    jstring jPackageName = s_jniEnv->NewStringUTF(packageName.c_str());
    jstring jPublisherName = s_jniEnv->NewStringUTF(publisherName.c_str());
	s_jniEnv->CallVoidMethod(s_helperObject, s_showMoreGames, jPackageName, jPublisherName);
    s_jniEnv->DeleteLocalRef(jPackageName);
    s_jniEnv->DeleteLocalRef(jPublisherName);
}

//*****************************************************************************
void VuAndroidCommunityManager::showTwitterPage(const std::string &userName)
{
    jstring jUserName = s_jniEnv->NewStringUTF(userName.c_str());
	s_jniEnv->CallVoidMethod(s_helperObject, s_showTwitterPage, jUserName);
    s_jniEnv->DeleteLocalRef(jUserName);
}

//*****************************************************************************
void VuAndroidCommunityManager::showFacebookPage(const std::string &id, const std::string &userName)
{
    jstring jId = s_jniEnv->NewStringUTF(id.c_str());
    jstring jUserName = s_jniEnv->NewStringUTF(userName.c_str());
	s_jniEnv->CallVoidMethod(s_helperObject, s_showFacebookPage, jId, jUserName);
    s_jniEnv->DeleteLocalRef(jId);
    s_jniEnv->DeleteLocalRef(jUserName);
}

//*****************************************************************************
void VuAndroidCommunityManager::showGooglePlusPage(const std::string &id)
{
    jstring jId = s_jniEnv->NewStringUTF(id.c_str());
	s_jniEnv->CallVoidMethod(s_helperObject, s_showGooglePlusPage, jId);
    s_jniEnv->DeleteLocalRef(jId);
}

//*****************************************************************************
void VuAndroidCommunityManager::rateGame(const std::string &packageName)
{
    jstring jPackageName = s_jniEnv->NewStringUTF(packageName.c_str());
	s_jniEnv->CallVoidMethod(s_helperObject, s_rateGame, jPackageName);
    s_jniEnv->DeleteLocalRef(jPackageName);
}
