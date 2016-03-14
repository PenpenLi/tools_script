//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  ExpansionFileManager class
// 
//*****************************************************************************

#include "VuExpansionFileManager.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Assets/VuAssetFactory.h"
#include "VuEngine/HAL/File/VuFile.h"
#include "VuEngine/Dev/VuDevConfig.h"


//*****************************************************************************
bool VuExpansionFileManager::init()
{
	mTesting = VuDevConfig::IF()->getParam("ExpansionFileTest").asBool();
	mTestMsg = VuDevConfig::IF()->getParam("ExpansionFileTestMsg").asString();

	return true;
}

//*****************************************************************************
bool VuExpansionFileManager::openExpansionFile()
{
	if ( mTesting )
		return false;

	if ( !VuAssetFactory::IF()->isPackFileOpen() )
		return true;
	return VuAssetFactory::IF()->openExpansionFile();
}

//*****************************************************************************
void VuExpansionFileManager::startDownload(VuListener *pListener)
{
	mpListener = pListener;
	startDownloadInternal();
	mpListener->onDownloadProgress(0.0f);
}

//*****************************************************************************
void VuExpansionFileManager::onDownloadResult(std::string errorMsg)
{
	if ( mTesting )
	{
		errorMsg = mTestMsg;
	}
	else if ( errorMsg.empty() )
	{
		if ( !VuAssetFactory::IF()->openExpansionFile() )
			errorMsg = "ExpansionFail_Corrupt";
	}

	if ( errorMsg.empty() )
	{
		mpListener->onDownloadSuccess();
		mpListener = VUNULL;
	}
	else
	{
		VuMessageBoxParams messageBoxParams;
		messageBoxParams.mType = "SimpleA";
		messageBoxParams.mHeading = "ExpansionFail_Header";
		messageBoxParams.mTextA = "Common_OK";
		messageBoxParams.mBody = errorMsg;

		VuMessageBox *pMB = VuMessageBoxManager::IF()->create(messageBoxParams);
		pMB->setCallback(this);
		pMB->removeRef();
	}
}

//*****************************************************************************
void VuExpansionFileManager::onDownloadProgress(float progress)
{
	mpListener->onDownloadProgress(progress);
}

//*****************************************************************************
void VuExpansionFileManager::onMessageBoxClosed(VuMessageBox *pMessageBox)
{
	mpListener->onDownloadFailure();
	mpListener = VUNULL;
}
