//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  TitleSequenceGameMode class
// 
//*****************************************************************************

#pragma once

#include "VuGameMode.h"
#include "VuEngine/Events/VuEventMap.h"
#include "VuEngine/Util/VuFSM.h"

class VuUIScreenEntity;
class VuProject;


class VuTitleSequenceGameMode : public VuGameMode
{
	DECLARE_RTTI
	DECLARE_EVENT_MAP

public:
	VuTitleSequenceGameMode();
	~VuTitleSequenceGameMode();

	virtual bool		enter(const std::string &prevMode);
	virtual void		exit();
	virtual const char	*tick(float fdt);
	virtual void		draw();

private:
	// events
	void				OnNextScreen(const VuParams &params);

	void				loadScreen(const std::string &screen);
	void				unloadScreen();
	VuUIScreenEntity	*getCurScreen();

	// states
	void				onLoadNextScreenEnter();
	void				onFadeInEnter();
	void				onWaitTick(float fdt);
	void				onFadeOutEnter();

	typedef std::queue<std::string> ScreenQueue;

	VuFSM				mFSM;
	VuProject			*mpScreenProject;
	ScreenQueue			mScreenQueue;
	float				mFadeTime;
	float				mWaitTime;
	bool				mTouched;
};
