//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuStubVideoShareManager
//
//*****************************************************************************

#include "VuPurple/Managers/VuVideoShareManager.h"
#include "VuEngine/Dev/VuDevConfig.h"


class VuStubVideoShareManager : public VuVideoShareManager
{
protected:
	virtual bool	isSupported_internal() { return VuDevConfig::IF()->getParam("VideoShareTest").asBool(); }
	virtual void	startRecording_internal() {}
	virtual void	stopRecording_internal() {}
	virtual void	pause_internal() {}
	virtual void	resume_internal() {}
	virtual void	beginDraw_internal() {}
	virtual void	endDraw_internal() {}
	virtual void	showShareUI_internal() {}
	virtual void	showWatchUI_internal() {}
};


// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuVideoShareManager, VuStubVideoShareManager);
