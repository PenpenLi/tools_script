//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Android Advertising manager
//
//*****************************************************************************

#include "VuAndroidAdManager.h"
#include "VuEngine/Events/VuEventManager.h"


// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuAdManager, VuAndroidAdManager);

// static JAVA stuff
static JNIEnv		*s_jniEnv;
static jobject		s_helperObject;
static jmethodID	s_isReadyInterstitial;
static jmethodID	s_isReadyIncentivized;
static jmethodID	s_isReadyReward;
static jmethodID	s_showInterstitial;
static jmethodID	s_showIncentivized;
static jmethodID	s_showReward;
static jmethodID	s_checkForAds;


//*****************************************************************************
void VuAndroidAdManager::bindJavaMethods(JNIEnv *jniEnv, jobject classLoaderObject, jmethodID findClassMethod)
{
	__android_log_print(ANDROID_LOG_DEBUG, "Ad",  "VuAndroidAdManager::bindJavaMethods()\n");

	s_jniEnv = jniEnv;

	// get reference to helper class object
	jstring helperClassName = jniEnv->NewStringUTF("com/vectorunit/VuAdHelper");
	jclass helperClass = (jclass)jniEnv->CallObjectMethod(classLoaderObject, findClassMethod, helperClassName);
	jniEnv->DeleteLocalRef(helperClassName);

	jmethodID getInstance = jniEnv->GetStaticMethodID(helperClass, "getInstance", "()Lcom/vectorunit/VuAdHelper;");
	jobject helperObject = jniEnv->CallStaticObjectMethod(helperClass, getInstance);
	s_helperObject = jniEnv->NewGlobalRef(helperObject);

	// methods
	s_isReadyInterstitial = jniEnv->GetMethodID(helperClass, "isReadyInterstitial", "()Z");
	s_isReadyIncentivized = jniEnv->GetMethodID(helperClass, "isReadyIncentivized", "()Z");
	s_isReadyReward = jniEnv->GetMethodID(helperClass, "isReadyReward", "()Z");
	s_showInterstitial = jniEnv->GetMethodID(helperClass, "showInterstitial", "()V");
	s_showIncentivized = jniEnv->GetMethodID(helperClass, "showIncentivized", "()V");
	s_showReward = jniEnv->GetMethodID(helperClass, "showReward", "()V");
	s_checkForAds = jniEnv->GetMethodID(helperClass, "checkForAds", "()V");
}

//*****************************************************************************
VuAndroidAdManager::VuAndroidAdManager()
{
	// event handlers
	REG_EVENT_HANDLER(VuAndroidAdManager, OnAndroidAdFinished);
}

//*****************************************************************************
void VuAndroidAdManager::OnAndroidAdFinished(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	bool completed = accessor.getBool();

	onAdFinished(completed);
}

//*****************************************************************************
bool VuAndroidAdManager::isAdReadyInternal(int adUnit)
{
	bool ready = false;

	if ( adUnit == AD_UNIT_INTERSTITIAL )
		ready = s_jniEnv->CallBooleanMethod(s_helperObject, s_isReadyInterstitial);
	else if ( adUnit == AD_UNIT_INCENTIVIZED )
		ready = s_jniEnv->CallBooleanMethod(s_helperObject, s_isReadyIncentivized);
	else if ( adUnit == AD_UNIT_REWARD )
		ready = s_jniEnv->CallBooleanMethod(s_helperObject, s_isReadyReward);

	return ready;
}

//*****************************************************************************
void VuAndroidAdManager::showAdInternal(int adUnit)
{
	if ( adUnit == AD_UNIT_INTERSTITIAL )
		s_jniEnv->CallVoidMethod(s_helperObject, s_showInterstitial);
	else if ( adUnit == AD_UNIT_INCENTIVIZED )
		s_jniEnv->CallVoidMethod(s_helperObject, s_showIncentivized);
	else if ( adUnit == AD_UNIT_REWARD )
		s_jniEnv->CallVoidMethod(s_helperObject, s_showReward);
	else
		onAdFinished(false);
}

//*****************************************************************************
void VuAndroidAdManager::checkForAdsInternal()
{
	s_jniEnv->CallVoidMethod(s_helperObject, s_checkForAds);
}


extern "C"
{
	JNIEXPORT void JNICALL Java_com_vectorunit_VuAdHelper_onAdFinished(JNIEnv *env, jobject obj, jboolean jCompleted);
}

//*****************************************************************************
JNIEXPORT void JNICALL Java_com_vectorunit_VuAdHelper_onAdFinished(JNIEnv *env, jobject obj, jboolean jCompleted)
{
	VuParams params;
	params.addBool(jCompleted);
	VuEventManager::IF()->broadcastDelayed(0.0f, true, "OnAndroidAdFinished", params);
}
