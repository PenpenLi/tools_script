//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuAndroidVideoShareManager
//
//*****************************************************************************

#include <jni.h>
#include "VuPurple/Managers/VuVideoShareManager.h"


class VuAndroidVideoShareManager : public VuVideoShareManager
{
public:
	// this function MUST be called from the application's JNI_OnLoad,
	// from a function known to be called by JNI_OnLoad, or from a function
	// in a Java-called thread.
	static void		bindJavaMethods(JNIEnv *jniEnv, jobject classLoaderObject, jmethodID findClassMethod);

protected:
	virtual bool	isSupported_internal();
	virtual void	startRecording_internal();
	virtual void	stopRecording_internal();
	virtual void	pause_internal();
	virtual void	resume_internal();
	virtual void	beginDraw_internal();
	virtual void	endDraw_internal();
	virtual void	showShareUI_internal();
	virtual void	showWatchUI_internal();
};
