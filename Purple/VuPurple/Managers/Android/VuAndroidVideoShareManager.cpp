//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuAndroidVideoShareManager
//
//*****************************************************************************

#include "VuAndroidVideoShareManager.h"


// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuVideoShareManager, VuAndroidVideoShareManager);


// static JAVA stuff
static JNIEnv		*s_jniEnv;
static jobject		s_helperObject;
static jmethodID	s_isSupported;
static jmethodID	s_startRecording;
static jmethodID	s_stopRecording;
static jmethodID	s_pause;
static jmethodID	s_resume;
static jmethodID	s_beginDraw;
static jmethodID	s_endDraw;
static jmethodID	s_showShareUI;
static jmethodID	s_showWatchUI;


//*****************************************************************************
void VuAndroidVideoShareManager::bindJavaMethods(JNIEnv *jniEnv, jobject classLoaderObject, jmethodID findClassMethod)
{
	__android_log_print(ANDROID_LOG_DEBUG, "VideoShare",  "VuAndroidVideoShareManager::bindJavaMethods()\n");

	s_jniEnv = jniEnv;

	// get reference to helper class object
	jstring helperClassName = jniEnv->NewStringUTF("com/vectorunit/VuVideoShareHelper");
	jclass helperClass = (jclass)jniEnv->CallObjectMethod(classLoaderObject, findClassMethod, helperClassName);
	jniEnv->DeleteLocalRef(helperClassName);

	jmethodID getInstance = jniEnv->GetStaticMethodID(helperClass, "getInstance", "()Lcom/vectorunit/VuVideoShareHelper;");
	jobject helperObject = jniEnv->CallStaticObjectMethod(helperClass, getInstance);
	s_helperObject = jniEnv->NewGlobalRef(helperObject);

	// methods
	s_isSupported = jniEnv->GetMethodID(helperClass, "isSupported", "()Z");
	s_startRecording = jniEnv->GetMethodID(helperClass, "startRecording", "()V");
	s_stopRecording = jniEnv->GetMethodID(helperClass, "stopRecording", "()V");
	s_pause = jniEnv->GetMethodID(helperClass, "pause", "()V");
	s_resume = jniEnv->GetMethodID(helperClass, "resume", "()V");
	s_beginDraw = jniEnv->GetMethodID(helperClass, "beginDraw", "()V");
	s_endDraw = jniEnv->GetMethodID(helperClass, "endDraw", "()V");
	s_showShareUI = jniEnv->GetMethodID(helperClass, "showShareUI", "()V");
	s_showWatchUI = jniEnv->GetMethodID(helperClass, "showWatchUI", "()V");
}

//*****************************************************************************
bool VuAndroidVideoShareManager::isSupported_internal()
{
	return s_jniEnv->CallBooleanMethod(s_helperObject, s_isSupported);
}

//*****************************************************************************
void VuAndroidVideoShareManager::startRecording_internal()
{
	s_jniEnv->CallVoidMethod(s_helperObject, s_startRecording);
}

//*****************************************************************************
void VuAndroidVideoShareManager::stopRecording_internal()
{
	s_jniEnv->CallVoidMethod(s_helperObject, s_stopRecording);
}

//*****************************************************************************
void VuAndroidVideoShareManager::pause_internal()
{
	s_jniEnv->CallVoidMethod(s_helperObject, s_pause);
}

//*****************************************************************************
void VuAndroidVideoShareManager::resume_internal()
{
	s_jniEnv->CallVoidMethod(s_helperObject, s_resume);
}

//*****************************************************************************
void VuAndroidVideoShareManager::beginDraw_internal()
{
	s_jniEnv->CallVoidMethod(s_helperObject, s_beginDraw);
}

//*****************************************************************************
void VuAndroidVideoShareManager::endDraw_internal()
{
	s_jniEnv->CallVoidMethod(s_helperObject, s_endDraw);
}

//*****************************************************************************
void VuAndroidVideoShareManager::showShareUI_internal()
{
	s_jniEnv->CallVoidMethod(s_helperObject, s_showShareUI);
}

//*****************************************************************************
void VuAndroidVideoShareManager::showWatchUI_internal()
{
	s_jniEnv->CallVoidMethod(s_helperObject, s_showWatchUI);
}
