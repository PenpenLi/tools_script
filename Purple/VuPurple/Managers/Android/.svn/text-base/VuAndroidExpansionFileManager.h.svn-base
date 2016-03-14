//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Android ExpansionFile manager
//
//*****************************************************************************

#include <jni.h>
#include "VuPurple/Managers/VuExpansionFileManager.h"
#include "VuEngine/Events/VuEventMap.h"
#include "VuEngine/HAL/File/Android/VuAndroidFile.h"


class VuAndroidExpansionFileManager : public VuExpansionFileManager, public VuAndroidFile::ExpansionFileIF
{
	DECLARE_EVENT_MAP

public:
	// this function MUST be called from the application's JNI_OnLoad,
	// from a function known to be called by JNI_OnLoad, or from a function
	// in a Java-called thread.
	static void		bindJavaMethods(JNIEnv *jniEnv, jobject classLoaderObject, jmethodID findClassMethod);

	VuAndroidExpansionFileManager();

	virtual bool	init();

	virtual void	startDownloadInternal();

	// platform-specific functionality
	static VuAndroidExpansionFileManager *IF() { return static_cast<VuAndroidExpansionFileManager *>(VuExpansionFileManager::IF()); }

protected:
	// VuFile::ExpansionFileIF
	virtual VUHANDLE	open();
	virtual void		close(VUHANDLE hFile);
	virtual int			read(VUHANDLE hFile, void *pData, int size);
	virtual bool		seek(VUHANDLE hFile, int pos);

	// event handlers
	void			OnAndroidDownloadProgress(const VuParams &params);
	void			OnAndroidDownloadResult(const VuParams &params);
};
