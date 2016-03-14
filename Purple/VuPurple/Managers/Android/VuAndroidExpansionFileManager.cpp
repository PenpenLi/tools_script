//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Android ExpansionFile manager
//
//*****************************************************************************

#include "VuAndroidExpansionFileManager.h"
#include "VuEngine/Events/VuEventManager.h"


// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuExpansionFileManager, VuAndroidExpansionFileManager);

// static JAVA stuff
static JNIEnv		*s_jniEnv;
static jobject		s_helperObject;
static jmethodID	s_openFile;
static jmethodID	s_closeFile;
static jmethodID	s_readFile;
static jmethodID	s_seekFile;
static jmethodID	s_startDownload;


//*****************************************************************************
void VuAndroidExpansionFileManager::bindJavaMethods(JNIEnv *jniEnv, jobject classLoaderObject, jmethodID findClassMethod)
{
	__android_log_print(ANDROID_LOG_DEBUG, "ofm",  "VuAndroidExpansionFileManager::bindJavaMethods()\n");

	s_jniEnv = jniEnv;

	// get reference to helper class object
	jstring helperClassName = jniEnv->NewStringUTF("com/vectorunit/VuExpansionFileHelper");
	jclass helperClass = (jclass)jniEnv->CallObjectMethod(classLoaderObject, findClassMethod, helperClassName);
	jniEnv->DeleteLocalRef(helperClassName);

	jmethodID getInstance = jniEnv->GetStaticMethodID(helperClass, "getInstance", "()Lcom/vectorunit/VuExpansionFileHelper;");
	jobject helperObject = jniEnv->CallStaticObjectMethod(helperClass, getInstance);
	s_helperObject = jniEnv->NewGlobalRef(helperObject);

	// methods
	s_openFile = jniEnv->GetMethodID(helperClass, "openFile", "()Z");
	s_closeFile = jniEnv->GetMethodID(helperClass, "closeFile", "()V");
	s_readFile = jniEnv->GetMethodID(helperClass, "readFile", "([B)I");
	s_seekFile = jniEnv->GetMethodID(helperClass, "seekFile", "(I)Z");
	s_startDownload = jniEnv->GetMethodID(helperClass, "startDownload", "()V");
}

//*****************************************************************************
VuAndroidExpansionFileManager::VuAndroidExpansionFileManager()
{
	// event handlers
	REG_EVENT_HANDLER(VuAndroidExpansionFileManager, OnAndroidDownloadProgress);
	REG_EVENT_HANDLER(VuAndroidExpansionFileManager, OnAndroidDownloadResult);
}

//*****************************************************************************
bool VuAndroidExpansionFileManager::init()
{
	if ( !VuExpansionFileManager::init() )
		return false;

	VuAndroidFile::IF()->setExpansionFileIF(this);

	return true;
}

//*****************************************************************************
void VuAndroidExpansionFileManager::startDownloadInternal()
{
	s_jniEnv->CallVoidMethod(s_helperObject, s_startDownload);
}

//*****************************************************************************
VUHANDLE VuAndroidExpansionFileManager::open()
{
	if ( s_jniEnv->CallBooleanMethod(s_helperObject, s_openFile) )
		return (VUHANDLE)1;

	return VUNULL;
}

//*****************************************************************************
void VuAndroidExpansionFileManager::close(VUHANDLE hFile)
{
	s_jniEnv->CallVoidMethod(s_helperObject, s_closeFile);
}

//*****************************************************************************
int VuAndroidExpansionFileManager::read(VUHANDLE hFile, void *pData, int size)
{
	if ( size <= 0 )
		return 0;

	jbyteArray jData = s_jniEnv->NewByteArray(size);

	int bytesRead = s_jniEnv->CallIntMethod(s_helperObject, s_readFile, jData);

	if ( bytesRead > 0 )
	{
		jbyte *ptr = s_jniEnv->GetByteArrayElements(jData, NULL);
		memcpy(pData, ptr, VuMin(bytesRead, size));
		s_jniEnv->ReleaseByteArrayElements(jData, ptr, 0);
	}

	s_jniEnv->DeleteLocalRef(jData);

	return bytesRead;
}

//*****************************************************************************
bool VuAndroidExpansionFileManager::seek(VUHANDLE hFile, int pos)
{
	return s_jniEnv->CallBooleanMethod(s_helperObject, s_seekFile, pos);
}

//*****************************************************************************
void VuAndroidExpansionFileManager::OnAndroidDownloadProgress(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	float progress = accessor.getFloat();

	onDownloadProgress(progress);
}
//*****************************************************************************
void VuAndroidExpansionFileManager::OnAndroidDownloadResult(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	const char *errorMsg = accessor.getString();

	onDownloadResult(errorMsg);
}


extern "C"
{
	JNIEXPORT void JNICALL Java_com_vectorunit_VuExpansionFileHelper_onDownloadProgress(JNIEnv *env, jobject obj, jlong curBytes, jlong totalBytes);
	JNIEXPORT void JNICALL Java_com_vectorunit_VuExpansionFileHelper_onDownloadResult(JNIEnv *env, jobject obj, jstring jErrorMsg);
}

//*****************************************************************************
JNIEXPORT void JNICALL Java_com_vectorunit_VuExpansionFileHelper_onDownloadProgress(JNIEnv *env, jobject obj, jlong curBytes, jlong totalBytes)
{
	float progress = (float)curBytes/totalBytes;

	VuParams params;
	params.addFloat(progress);
	VuEventManager::IF()->broadcastDelayed(0.0f, true, "OnAndroidDownloadProgress", params);
}

//*****************************************************************************
JNIEXPORT void JNICALL Java_com_vectorunit_VuExpansionFileHelper_onDownloadResult(JNIEnv *env, jobject obj, jstring jErrorMsg)
{
	// get errorMsg
	std::string errorMsg;
	{
		const char *strErrorMsg = env->GetStringUTFChars(jErrorMsg, 0);
		errorMsg = strErrorMsg;
		env->ReleaseStringUTFChars(jErrorMsg, strErrorMsg);
	}

	VuParams params;
	params.addString(errorMsg.c_str());
	VuEventManager::IF()->broadcastDelayed(0.0f, true, "OnAndroidDownloadResult", params);
}