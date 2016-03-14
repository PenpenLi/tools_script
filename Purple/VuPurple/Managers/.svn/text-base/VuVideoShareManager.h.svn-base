//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuVideoShareManager
// 
//*****************************************************************************

#pragma once

#include "VuEngine/VuSystemComponent.h"


class VuVideoShareManager : public VuSystemComponent
{
	DECLARE_SYSTEM_COMPONENT(VuVideoShareManager)

public:
	VuVideoShareManager();

	virtual bool init() { return true; }

	bool	isSupported();

	void	startRecording();
	void	stopRecording();
	bool	isRecording() { return mIsRecording; }
	void	resetRecording();
	bool	hasRecording() { return mHasRecording; }

	void	pause();
	void	resume();

	void	beginDraw();
	void	endDraw();

	void	showShareUI();
	void	showWatchUI();

protected:
	virtual bool	isSupported_internal() = 0;
	virtual void	startRecording_internal() = 0;
	virtual void	stopRecording_internal() = 0;
	virtual void	pause_internal() = 0;
	virtual void	resume_internal() = 0;
	virtual void	beginDraw_internal() = 0;
	virtual void	endDraw_internal() = 0;
	virtual void	showShareUI_internal() = 0;
	virtual void	showWatchUI_internal() = 0;

	bool	mIsRecording;
	bool	mHasRecording;
};
