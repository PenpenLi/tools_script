//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Android Analytics manager
//
//*****************************************************************************

#include "VuAndroidAnalyticsManager.h"
#include "VuEngine/Json/VuJsonContainer.h"
#include "VuEngine/Json/VuJsonWriter.h"


// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuAnalyticsManager, VuAndroidAnalyticsManager);

// static JAVA stuff
static JNIEnv		*s_jniEnv;
static jobject		s_helperObject;
static jmethodID	s_startVariables;
static jmethodID	s_addIntVariable;
static jmethodID	s_addFloatVariable;
static jmethodID	s_logEvent;
static jmethodID	s_logZhimengEvent;


static jmethodID	s_setLevel;
static jmethodID	s_setCurrency;
static jmethodID	s_displayNotifications;

//*****************************************************************************
void VuAndroidAnalyticsManager::bindJavaMethods(JNIEnv *jniEnv, jobject classLoaderObject, jmethodID findClassMethod)
{
	__android_log_print(ANDROID_LOG_DEBUG, "ofm",  "VuAndroidAnalyticsManager::bindJavaMethods()\n");

	s_jniEnv = jniEnv;

	// get reference to helper class object
	jstring helperClassName = jniEnv->NewStringUTF("com/vectorunit/VuAnalyticsHelper");
	jclass helperClass = (jclass)jniEnv->CallObjectMethod(classLoaderObject, findClassMethod, helperClassName);
	jniEnv->DeleteLocalRef(helperClassName);

	jmethodID getInstance = jniEnv->GetStaticMethodID(helperClass, "getInstance", "()Lcom/vectorunit/VuAnalyticsHelper;");
	jobject helperObject = jniEnv->CallStaticObjectMethod(helperClass, getInstance);
	s_helperObject = jniEnv->NewGlobalRef(helperObject);

	// methods
	s_startVariables = jniEnv->GetMethodID(helperClass, "startVariables", "()V");
	s_addIntVariable = jniEnv->GetMethodID(helperClass, "addIntVariable", "(Ljava/lang/String;I)V");
	s_addFloatVariable = jniEnv->GetMethodID(helperClass, "addFloatVariable", "(Ljava/lang/String;F)V");
	s_logEvent = jniEnv->GetMethodID(helperClass, "logEvent", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
	s_logZhimengEvent = jniEnv->GetMethodID(helperClass, "record", "(Ljava/lang/String;Ljava/lang/String;I)V");
	s_setLevel = jniEnv->GetMethodID(helperClass, "setLevel", "(I)V");
	s_setCurrency = jniEnv->GetMethodID(helperClass, "setCurrency", "(II)V");
	s_displayNotifications = jniEnv->GetMethodID(helperClass, "displayNotifications", "()V");


}

//*****************************************************************************
bool VuAndroidAnalyticsManager::init()
{
	if ( !VuAnalyticsManager::init() )
		return false;

	return true;
}

//*****************************************************************************
void VuAndroidAnalyticsManager::logEventInternal(const char *eventName, const char *paramName, const char *paramValue, const VuJsonContainer &variables)
{
	JNIEnv *jniEnv = s_jniEnv;

	// start variables
	jniEnv->CallVoidMethod(s_helperObject, s_startVariables);

	// add variables
	for ( int i = 0; i < variables.numMembers(); i++ )
	{
		const std::string &key = variables.getMemberKey(i);
		const VuJsonContainer &value = variables[key];

		jstring jKey = jniEnv->NewStringUTF(key.c_str());

		if ( value.isInt() )
		{
			jniEnv->CallVoidMethod(s_helperObject, s_addIntVariable, jKey, value.asInt());
		}
		else if ( value.isFloat() )
		{
			jniEnv->CallVoidMethod(s_helperObject, s_addFloatVariable, jKey, value.asFloat());
		}
		else if ( value.isBool() )
		{
			jniEnv->CallVoidMethod(s_helperObject, s_addIntVariable, jKey, (int)value.asBool());
		}

		jniEnv->DeleteLocalRef(jKey);
	}

	// log event
    jstring jEventName = jniEnv->NewStringUTF(eventName);
    jstring jParamName = jniEnv->NewStringUTF(paramName);
    jstring jParamValue = jniEnv->NewStringUTF(paramValue);

	jniEnv->CallVoidMethod(s_helperObject, s_logEvent, jEventName, jParamName, jParamValue);

    jniEnv->DeleteLocalRef(jParamValue);
    jniEnv->DeleteLocalRef(jParamName);
    jniEnv->DeleteLocalRef(jEventName);
}
//*****************************************************************************
void	VuAndroidAnalyticsManager::logEventInternal(const char* eventName, const char* paramKey, int value)
{
	__android_log_print(ANDROID_LOG_DEBUG, "ofm", "VuAndroidAnalyticsManager::logEventInternal()\n");
	JNIEnv *jniEnv = s_jniEnv;

	jstring jEventName = jniEnv->NewStringUTF(eventName);
	jstring jParamKey = jniEnv->NewStringUTF(paramKey);
	
	jniEnv->CallVoidMethod(s_helperObject, s_logZhimengEvent, jEventName, jParamKey, value);
}

//*****************************************************************************
void VuAndroidAnalyticsManager::setLevelInternal(int level)
{
	s_jniEnv->CallVoidMethod(s_helperObject, s_setLevel, level);
}

//*****************************************************************************
void VuAndroidAnalyticsManager::setCurrencyInternal(int type, int balance)
{
	s_jniEnv->CallVoidMethod(s_helperObject, s_setCurrency, type, balance);
}

//*****************************************************************************
void VuAndroidAnalyticsManager::displayNotificationsInternal()
{
	s_jniEnv->CallVoidMethod(s_helperObject, s_displayNotifications);
}