//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuAndroidCloudTuningManager
//
//*****************************************************************************

#include "VuAndroidCloudTuningManager.h"


// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuCloudTuningManager, VuAndroidCloudTuningManager);

// static JAVA stuff
static JNIEnv		*s_jniEnv;
static jobject		s_helperObject;
static jmethodID	s_wasGameConfigurationReceived;
static jmethodID	s_getGameConfigurationValue;


//*****************************************************************************
void VuAndroidCloudTuningManager::bindJavaMethods(JNIEnv *jniEnv, jobject classLoaderObject, jmethodID findClassMethod)
{
	__android_log_print(ANDROID_LOG_DEBUG, "CloudTuning",  "VuAndroidCloudTuningManager::bindJavaMethods()\n");

	s_jniEnv = jniEnv;

	// get reference to helper class object
	jstring helperClassName = jniEnv->NewStringUTF("com/vectorunit/VuCloudTuningHelper");
	jclass helperClass = (jclass)jniEnv->CallObjectMethod(classLoaderObject, findClassMethod, helperClassName);
	jniEnv->DeleteLocalRef(helperClassName);

	jmethodID getInstance = jniEnv->GetStaticMethodID(helperClass, "getInstance", "()Lcom/vectorunit/VuCloudTuningHelper;");
	jobject helperObject = jniEnv->CallStaticObjectMethod(helperClass, getInstance);
	s_helperObject = jniEnv->NewGlobalRef(helperObject);

	// methods
	s_wasGameConfigurationReceived = jniEnv->GetMethodID(helperClass, "wasGameConfigurationReceived", "()Z");
	s_getGameConfigurationValue = jniEnv->GetMethodID(helperClass, "getGameConfigurationValue", "(Ljava/lang/String;)Ljava/lang/String;");
}

//*****************************************************************************
bool VuAndroidCloudTuningManager::wasGameConfigurationReceived()
{
	return s_jniEnv->CallBooleanMethod(s_helperObject, s_wasGameConfigurationReceived);
}

//*****************************************************************************
void VuAndroidCloudTuningManager::getGameConfigurationValue(const std::string &key, std::string &value)
{
	jstring jKey = s_jniEnv->NewStringUTF(key.c_str());

	if ( jstring jValue = (jstring)s_jniEnv->CallObjectMethod(s_helperObject, s_getGameConfigurationValue, jKey) )
	{
		const char *strValue = s_jniEnv->GetStringUTFChars(jValue, 0);

		value = strValue;

		s_jniEnv->ReleaseStringUTFChars(jValue, strValue);  
		s_jniEnv->DeleteLocalRef(jValue);
	}

	s_jniEnv->DeleteLocalRef(jKey);
}
