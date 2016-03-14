//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuVideoShareManager
// 
//*****************************************************************************

#include "VuVideoShareManager.h"


//*****************************************************************************
VuVideoShareManager::VuVideoShareManager():
	mIsRecording(false),
	mHasRecording(false)
{
}

//*****************************************************************************
bool VuVideoShareManager::isSupported()
{
	if ( !VuSys::IF()->hasTouch() )
		return false;

	return isSupported_internal();
}

//*****************************************************************************
void VuVideoShareManager::startRecording()
{
	if ( !mIsRecording )
	{
		mIsRecording = true;
		startRecording_internal();
	}
}

//*****************************************************************************
void VuVideoShareManager::stopRecording()
{
	if ( mIsRecording )
	{
		mIsRecording = false;
		mHasRecording = true;
		stopRecording_internal();
	}
}

//*****************************************************************************
void VuVideoShareManager::resetRecording()
{
	if ( !mIsRecording )
		mHasRecording = false;
}

//*****************************************************************************
void VuVideoShareManager::pause()
{
	if ( mIsRecording )
		pause_internal();
}

//*****************************************************************************
void VuVideoShareManager::resume()
{
	if ( mIsRecording )
		resume_internal();
}

//*****************************************************************************
void VuVideoShareManager::beginDraw()
{
	beginDraw_internal();
}

//*****************************************************************************
void VuVideoShareManager::endDraw()
{
	endDraw_internal();
}

//*****************************************************************************
void VuVideoShareManager::showShareUI()
{
	if ( mHasRecording )
		showShareUI_internal();
}

//*****************************************************************************
void VuVideoShareManager::showWatchUI()
{
	showWatchUI_internal();
}
