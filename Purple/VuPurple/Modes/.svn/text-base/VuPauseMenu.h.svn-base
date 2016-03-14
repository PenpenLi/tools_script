//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Pause menu class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Events/VuEventMap.h"
#include "VuEngine/Util/VuFSM.h"
#include "VuEngine/Util/VuColor.h"

class VuUIScreenEntity;
class VuProject;


class VuPauseMenu
{
	DECLARE_EVENT_MAP

public:
	VuPauseMenu(bool netGame);
	~VuPauseMenu();

	void				pause(const char *initialScreen, bool fade, VUUINT32 padMask);
	void				unpause();
	bool				isPaused();

	void				tick(float fdt);
	void				draw();

	void				setDefaultPauseMenu(const char *screen) { mDefaultScreen = screen; }

private:
	// events
	void				OnSetScreen(const VuParams &params);
	void				OnPushScreen(const VuParams &params);
	void				OnPopScreen(const VuParams &params);
	void				OnReloadScreen(const VuParams &params);
	void				SetDefaultPauseMenu(const VuParams &params);

	void				setGamePaused(bool paused);

	void				loadScreen(const std::string &screen);
	void				unloadScreen();
	VuUIScreenEntity	*getCurScreen();

	void				drawBackground();

	// FSM
	void				onUnpausedEnter();
	void				onUnpausedExit();
	void				onPausedTick(float fdt);
	void				onTransition();
	void				onFadeInEnter();
	void				onFadeInTick(float fdt);
	void				onFadeOutEnter();
	void				onFadeOutTick(float fdt);

	VuFSM				mFSM;
	bool				mNetGame;
	bool				mGamePaused;
	bool				mFadeEnabled;
	float				mBackgroundAlphaValue;
	VuColor				mBackgroundColor;
	std::string			mDefaultScreen;
	std::string			mNextScreen;
	std::string			mCurScreen;
	VuProject			*mpScreenProject;
	VUUINT32			mPadMask;
};
