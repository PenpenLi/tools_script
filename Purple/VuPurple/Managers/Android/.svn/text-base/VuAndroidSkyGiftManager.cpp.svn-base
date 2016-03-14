//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Android Community manager
//
//*****************************************************************************

#include "VuAndroidSkyGiftManager.h"


// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuSkyGiftManager, VuAndroidSkyGiftManager);

// static JAVA stuff
static JNIEnv		*s_jniEnv;
static jobject		s_helperObject;
static jmethodID	s_showGiftUI;
static jmethodID    s_GiftRequest;
//static jobject		s_helperObject;

jclass  s_helperClass;
//*****************************************************************************
void VuAndroidSkyGiftManager::bindJavaMethods(JNIEnv *jniEnv,jobject classLoaderObject, jmethodID findClassMethod)
{
	//__android_log_print(ANDROID_LOG_DEBUG, "ofm",  "VuAndroidSkyGiftManager::bindJavaMethods()\n");

	//__android_log_print(ANDROID_LOG_DEBUG, "ofm", "VuAndroidSkyGiftManager::bindJavaMethods()\n");


	s_jniEnv = jniEnv;
	
	// get reference to helper class object
	jstring helperClassName = jniEnv->NewStringUTF("com/skycn/android/request/TWGiftHelper");
	                                                
	jclass helperClass = (jclass)jniEnv->CallObjectMethod(classLoaderObject, findClassMethod, helperClassName);
	s_helperClass = (jclass)jniEnv->NewGlobalRef(helperClass);
	jniEnv->DeleteLocalRef(helperClassName);

	jmethodID getInstance = jniEnv->GetStaticMethodID(helperClass, "getInstance", "()Lcom/skycn/android/request/TWGiftHelper;");
	jobject helperObject = jniEnv->CallStaticObjectMethod(helperClass, getInstance);
	s_helperObject = (jobject)jniEnv->NewGlobalRef(helperObject);
	if( NULL == s_helperClass )
	{
	//__android_log_print(ANDROID_LOG_DEBUG, "Gift",  "Gift::giftResquest s_helperClass is null \n");
	//return;
	}
	
	//jclass helperClass22 = (jclass)s_jniEnv->FindClass( "com/skycn/android/request/TWGiftHelper" );
	
	//if( NULL == helperClass22 )
	{
	//__android_log_print(ANDROID_LOG_DEBUG, "Gift",  "Gift::giftResquest helperClass22 is null \n");
	//return;
	}
	//s_jniEnv->DeleteLocalRef(helperClassName);

	s_GiftRequest = s_jniEnv->GetMethodID(s_helperClass, "Start", "()V");
	
	if( NULL == s_GiftRequest )
	{
	//__android_log_print(ANDROID_LOG_DEBUG, "Gift",  "Gift::giftResquest s_GiftRequest is null \n");
	return;
	}
//__android_log_print(ANDROID_LOG_DEBUG, "Gift",  "Gift::giftResquest  is ok \n");
	// methods
	//s_wasGameConfigurationReceived = jniEnv->GetMethodID(helperClass, "wasGameConfigurationReceived", "()Z");
	//s_getGameConfigurationValue = jniEnv->GetMethodID(helperClass, "getGameConfigurationValue", "(Ljava/lang/String;)Ljava/lang/String;");
}

//*****************************************************************************
void VuAndroidSkyGiftManager::showGiftUI(bool bshow /*= true*/)
{
   // __android_log_print(ANDROID_LOG_DEBUG, "Gift",  "Gift::giftResquest 1()\n");
   // jstring helperClassName = s_jniEnv->NewStringUTF();
   if( NULL == s_jniEnv )
	{
	//__android_log_print(ANDROID_LOG_DEBUG, "Gift",  "Gift::giftResquest jniEnv is null \n");
	return;
	}
	
	
	if( NULL == s_helperClass )
	{
	//__android_log_print(ANDROID_LOG_DEBUG, "Gift",  "Gift::giftResquest s_helperClass1 is null \n");
	//return;
	}
	
	s_jniEnv->CallVoidMethod(s_helperObject, s_GiftRequest);

}
//*****************************************************************************
extern "C"
{
	char* jstringTostring(JNIEnv* env, jstring jstr)  
	{  
		char* rtn = NULL;  
		jclass clsstring = env->FindClass("java/lang/String");  
		jstring strencode = env->NewStringUTF("utf-8");  
		jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");  
		
		
		if(  clsstring	== NULL ){
			return "";
		}
		if(  mid	== NULL ){
			return "";
		}

		jbyteArray barr = (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);  

		jsize alen = env->GetArrayLength(barr);  
		jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);  
		if (alen > 0)  
		{  
			rtn = (char*)malloc(alen + 1);  
			memcpy(rtn, ba, alen);  
			rtn[alen] = 0;  
		}  
		env->ReleaseByteArrayElements(barr, ba, 0);  
		if( rtn != NULL ){
		//__android_log_print(ANDROID_LOG_DEBUG, "Gift",  "%s",rtn );
		}
		return rtn;  
	} 
JNIEXPORT void JNICALL Java_com_skycn_android_request_TWGiftCallbackNative_giftResponse
  (JNIEnv * jniEnv, jobject jz, jstring str)
	//void Java_com_skycn_android_request_TWGiftCallbackNative_giftResponse( JNIEnv* jniEnv, jstring str, jobject jz )
	{
		//__android_log_print(ANDROID_LOG_DEBUG, "Gift",  "Gift::giftResponse()\n");
		char* pStr = jstringTostring(jniEnv,str);
		if( pStr == NULL) 
			return;
		std::string tempStr = pStr;	
		VuAndroidSkyGiftManager::IF()->onGiftCallback( tempStr );
		free( pStr );
		pStr = NULL;
		
		//__android_log_print(ANDROID_LOG_DEBUG, "Gift",  "");		
	}
}
