//*****************************************************************************
//
//  Copyright (c) 2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuAndroidGameServicesManager
//
//*****************************************************************************

#include "VuAndroidGameServicesManager.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Assets/VuAssetFactory.h"


// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuGameServicesManager, VuAndroidGameServicesManager);

// static JAVA stuff
static JNIEnv		*s_jniEnv;
static jobject		s_helperObject;
static jmethodID	s_startSignIn;
static jmethodID	s_startSignOut;
static jmethodID	s_unlockAchievement;
static jmethodID	s_showAchievements;
static jmethodID	s_submitScore;


//*****************************************************************************
void VuAndroidGameServicesManager::bindJavaMethods(JNIEnv *jniEnv, jobject classLoaderObject, jmethodID findClassMethod)
{
	__android_log_print(ANDROID_LOG_DEBUG, "ofm",  "VuAndroidGameServicesManager::bindJavaMethods()\n");

	s_jniEnv = jniEnv;

	// get reference to helper class object
	jstring helperClassName = jniEnv->NewStringUTF("com/vectorunit/VuGameServicesHelper");
	jclass helperClass = (jclass)jniEnv->CallObjectMethod(classLoaderObject, findClassMethod, helperClassName);
	jniEnv->DeleteLocalRef(helperClassName);

	jmethodID getInstance = jniEnv->GetStaticMethodID(helperClass, "getInstance", "()Lcom/vectorunit/VuGameServicesHelper;");
	jobject helperObject = jniEnv->CallStaticObjectMethod(helperClass, getInstance);
	s_helperObject = jniEnv->NewGlobalRef(helperObject);

	// methods
	s_startSignIn = jniEnv->GetMethodID(helperClass, "startSignIn", "()V");
	s_startSignOut = jniEnv->GetMethodID(helperClass, "startSignOut", "()V");
	s_unlockAchievement = jniEnv->GetMethodID(helperClass, "unlockAchievement", "(Ljava/lang/String;)V");
	s_showAchievements = jniEnv->GetMethodID(helperClass, "showAchievements", "()V");
	s_submitScore = jniEnv->GetMethodID(helperClass, "submitScore", "(Ljava/lang/String;I)V");
}

//*****************************************************************************
VuAndroidGameServicesManager::VuAndroidGameServicesManager()
{
	// event handlers
	REG_EVENT_HANDLER(VuAndroidGameServicesManager, OnAndroidSignInSuccess);
	REG_EVENT_HANDLER(VuAndroidGameServicesManager, OnAndroidSignInFailure);
	REG_EVENT_HANDLER(VuAndroidGameServicesManager, OnAndroidSignOut);
	REG_EVENT_HANDLER(VuAndroidGameServicesManager, OnAndroidAddFriend);
	REG_EVENT_HANDLER(VuAndroidGameServicesManager, OnAndroidFinalizeFriends);
	REG_EVENT_HANDLER(VuAndroidGameServicesManager, OnAndroidAchievementState);

	if ( VuAssetFactory::IF()->getSku() == "GooglePlay" )
		mAuthType = "GoogleGames";
	else if ( VuAssetFactory::IF()->getSku() == "Amazon" )
		mAuthType = "GameCircle";
}

//*****************************************************************************
void VuAndroidGameServicesManager::OnAndroidSignInSuccess(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	const char *gamerID = accessor.getString();
	const char *gamerTag = accessor.getString();

	// If it's not the same user, and someone is signed in already, then just
	// sign them out with respect to OUR code.
	if (mGamerID != "" && 
		(gamerID != mGamerID || gamerTag != mGamerTag))
	{
		onSignOut();
	}

	onSignInSuccess(gamerID, gamerTag);
}

//*****************************************************************************
void VuAndroidGameServicesManager::OnAndroidSignInFailure(const VuParams &params)
{
	onSignInFailure();
}

//*****************************************************************************
void VuAndroidGameServicesManager::OnAndroidSignOut(const VuParams &params)
{
	onSignOut();
}

//*****************************************************************************
void VuAndroidGameServicesManager::OnAndroidAddFriend(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	const char *gamerID = accessor.getString();
	const char *gamerTag = accessor.getString();

	mTempFriends.resize(mTempFriends.size() + 1);
	FriendInfo &friendInfo = mTempFriends.back();

	friendInfo.mGamerID = gamerID;
	friendInfo.mGamerTag = gamerTag;
}

//*****************************************************************************
void VuAndroidGameServicesManager::OnAndroidFinalizeFriends(const VuParams &params)
{
	onFriendsRetrieved(mTempFriends);
	mTempFriends.clear();
}

//*****************************************************************************
void VuAndroidGameServicesManager::OnAndroidAchievementState(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	const char *strNativeId = accessor.getString();
	bool unlocked = accessor.getBool();

	for ( Achievements::iterator iter = mAchievements.begin(); iter != mAchievements.end(); iter++ )
		if ( iter->second.mNativeId == strNativeId )
			iter->second.mUnlocked = unlocked;
}

//*****************************************************************************
void VuAndroidGameServicesManager::startSignInAsync()
{
	s_jniEnv->CallVoidMethod(s_helperObject, s_startSignIn);
}

//*****************************************************************************
void VuAndroidGameServicesManager::startSignOutAsync()
{
	s_jniEnv->CallVoidMethod(s_helperObject, s_startSignOut);
}

//*****************************************************************************
void VuAndroidGameServicesManager::setAchievementUnlocked(const char *nativeId)
{
    jstring jNativeId = s_jniEnv->NewStringUTF(nativeId);
	s_jniEnv->CallVoidMethod(s_helperObject, s_unlockAchievement, jNativeId);
    s_jniEnv->DeleteLocalRef(jNativeId);
}

//*****************************************************************************
void VuAndroidGameServicesManager::submitScoreAsync(const char *nativeId, int value)
{
    jstring jNativeId = s_jniEnv->NewStringUTF(nativeId);
	s_jniEnv->CallVoidMethod(s_helperObject, s_submitScore, jNativeId, value);
    s_jniEnv->DeleteLocalRef(jNativeId);
}

//*****************************************************************************
void VuAndroidGameServicesManager::showAchievements()
{
	s_jniEnv->CallVoidMethod(s_helperObject, s_showAchievements);
}


extern "C"
{
	JNIEXPORT void JNICALL Java_com_vectorunit_VuGameServicesHelper_onSignInSuccess(JNIEnv *env, jobject obj, jstring jGamerID, jstring jGamerTag);
	JNIEXPORT void JNICALL Java_com_vectorunit_VuGameServicesHelper_onSignInFailure(JNIEnv *env, jobject obj);
	JNIEXPORT void JNICALL Java_com_vectorunit_VuGameServicesHelper_onSignOut(JNIEnv *env, jobject obj);
	JNIEXPORT void JNICALL Java_com_vectorunit_VuGameServicesHelper_addFriend(JNIEnv *env, jobject obj, jstring jGamerID, jstring jGamerTag);
	JNIEXPORT void JNICALL Java_com_vectorunit_VuGameServicesHelper_finalizeFriends(JNIEnv *env, jobject obj);
	JNIEXPORT void JNICALL Java_com_vectorunit_VuGameServicesHelper_onAchievementState(JNIEnv *env, jobject obj, jstring jNativeId, jboolean jUnlocked);
}

//*****************************************************************************
JNIEXPORT void JNICALL Java_com_vectorunit_VuGameServicesHelper_onSignInSuccess(JNIEnv *env, jobject obj, jstring jGamerID, jstring jGamerTag)
{
	// get gamerID
	std::string gamerID;
	{
		const char *strGamerID = env->GetStringUTFChars(jGamerID, 0);
		gamerID = strGamerID;
		env->ReleaseStringUTFChars(jGamerID, strGamerID);
	}

	// get gamerTag
	std::string gamerTag;
	{
		const char *strGamerTag = env->GetStringUTFChars(jGamerTag, 0);
		gamerTag = strGamerTag;
		env->ReleaseStringUTFChars(jGamerTag, strGamerTag);
	}

	VuParams params;
	params.addString(gamerID.c_str());
	params.addString(gamerTag.c_str());
	VuEventManager::IF()->broadcastDelayed(0.0f, true, "OnAndroidSignInSuccess", params);
}

//*****************************************************************************
JNIEXPORT void JNICALL Java_com_vectorunit_VuGameServicesHelper_onSignInFailure(JNIEnv *env, jobject obj)
{
	VuEventManager::IF()->broadcastDelayed(0.0f, true, "OnAndroidSignInFailure");
}

//*****************************************************************************
JNIEXPORT void JNICALL Java_com_vectorunit_VuGameServicesHelper_onSignOut(JNIEnv *env, jobject obj)
{
	VuEventManager::IF()->broadcastDelayed(0.0f, true, "OnAndroidSignOut");
}

//*****************************************************************************
JNIEXPORT void JNICALL Java_com_vectorunit_VuGameServicesHelper_addFriend(JNIEnv *env, jobject obj, jstring jGamerID, jstring jGamerTag)
{
	// get gamerID
	std::string gamerID;
	{
		const char *strGamerID = env->GetStringUTFChars(jGamerID, 0);
		gamerID = strGamerID;
		env->ReleaseStringUTFChars(jGamerID, strGamerID);
	}

	// get gamerTag
	std::string gamerTag;
	{
		const char *strGamerTag = env->GetStringUTFChars(jGamerTag, 0);
		gamerTag = strGamerTag;
		env->ReleaseStringUTFChars(jGamerTag, strGamerTag);
	}

	VuParams params;
	params.addString(gamerID.c_str());
	params.addString(gamerTag.c_str());
	VuEventManager::IF()->broadcastDelayed(0.0f, true, "OnAndroidAddFriend", params);
}

//*****************************************************************************
JNIEXPORT void JNICALL Java_com_vectorunit_VuGameServicesHelper_finalizeFriends(JNIEnv *env, jobject obj)
{
	VuEventManager::IF()->broadcastDelayed(0.0f, true, "OnAndroidFinalizeFriends");
}

//*****************************************************************************
JNIEXPORT void JNICALL Java_com_vectorunit_VuGameServicesHelper_onAchievementState(JNIEnv *env, jobject obj, jstring jNativeId, jboolean jUnlocked)
{
	// get nativeId
	std::string nativeId;
	{
		const char *strNativeId = env->GetStringUTFChars(jNativeId, 0);
		nativeId = strNativeId;
		env->ReleaseStringUTFChars(jNativeId, strNativeId);
	}

	VuParams params;
	params.addString(nativeId.c_str());
	params.addBool(jUnlocked);
	VuEventManager::IF()->broadcastDelayed(0.0f, true, "OnAndroidAchievementState", params);
}
