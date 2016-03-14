//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  FrontEndGameMode class
// 
//*****************************************************************************

#pragma once

#include "VuFrontEndCamera.h"
#include "VuGameMode.h"
#include "VuEngine/Events/VuEventMap.h"
#include "VuEngine/HAL/Keyboard/VuKeyboard.h"
#include "VuEngine/Util/VuFSM.h"

class VuProject;
class VuUIScreenEntity;
class VuGameScreenEntity;
class VuJsonContainer;


class VuFrontEndGameMode : public VuGameMode, VuKeyboard::Callback
{
	DECLARE_RTTI
	DECLARE_EVENT_MAP

public:
	VuFrontEndGameMode();
	~VuFrontEndGameMode();

	virtual bool		enter(const std::string &prevMode);
	virtual void		exit();
	virtual const char	*tick(float fdt);
	virtual void		draw();

private:
	// events
	void				OnStartActionGame(const VuParams &params);
	void				OnStartSplitScreenGame(const VuParams &params);
	void				OnSetScreen(const VuParams &params);
	void				OnPushScreen(const VuParams &params);
	void				OnPopScreen(const VuParams &params);
	void				OnSetScreenStack(const VuParams &params);
	void				OnPushNewsScreen(const VuParams &params);
	void				OnShutdownApp(const VuParams &params);

	// VuKeyboard::Callback
	virtual void		onKeyDown(VUUINT32 key);

	void				loadBackgroundProject();
	void				unloadBackgroundProject();
	void				loadNextScreen();
	void				loadScreen(const std::string &screen);
	void				unloadScreen();
	void				unloadNewsScreen();
	VuUIScreenEntity	*getCurScreen();

	// states
	void				onBeginEnter();
	void				onEndEnter();
	void				onEndTick(float fdt);
	void				onRunTick(float fdt);
	void				onFadeOutEnter();
	void				onFadeOutTick(float fdt);
	void				onFadeInEnter();
	void				onFadeInTick(float fdt);

	VuFSM				mFSM;
	std::string			mNextGameMode;
	std::string			mNextEventType;
	std::string			mNextScreen;
	std::string			mCurScreen;
	VuProject			*mpBackgroundProject;
	VuProject			*mpScreenProject;
	VuProject			*mpNewsScreenProject;

	VuFrontEndCamera	mCamera;
};
