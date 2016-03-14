//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  ActionGameMode class
// 
//*****************************************************************************

#pragma once

#include "VuGameMode.h"
#include "VuEngine/Events/VuEventMap.h"
#include "VuEngine/HAL/Keyboard/VuKeyboard.h"
#include "VuEngine/Util/VuFSM.h"

class VuProjectAsset;
class VuProject;
class VuGame;
class VuPauseMenu;
class VuUIScreenEntity;


class VuActionGameMode : public VuGameMode, VuKeyboard::Callback
{
	DECLARE_RTTI
	DECLARE_EVENT_MAP

public:
	VuActionGameMode();
	~VuActionGameMode();

	virtual bool		enter(const std::string &prevMode);
	virtual void		exit();
	virtual const char	*tick(float fdt);
	virtual void		draw();

private:
	// events
	void				OnEndActionGame(const VuParams &params);
	void				OnRestartActionGame(const VuParams &params);
	void				OnStartActionGame(const VuParams &params);
	void				OnPauseActionGame(const VuParams &params);
	void				OnResumeActionGame(const VuParams &params);
	void				OnSkipLoadingScreen(const VuParams &params);

	// VuKeyboard::Callback
	virtual void		onKeyDown(VUUINT32 key);

	void				onGameDraw();

	// states
	void				onLoadLevelEnter();
	void				onLoadLevelExit();
	void				onLoadLevelTick(float fdt);
	void				onLoadLevelDraw();
	void				onBeginEnter();
	void				onBeginTick(float fdt);
	void				onEndEnter();
	void				onEndTick(float fdt);
	void				onRunEnter();
	void				onRunTick(float fdt);

	VuUIScreenEntity	*getLoadingScreen();

	VuFSM				mFSM;
	VuProject			*mpLoadingScreenProject;
	VuProjectAsset		*mpGameProjectAsset;
	VuProject			*mpGameProject;
	VuGame				*mpGame;
	double				mLastDrawTime;
	std::string			mGameType;
	std::string			mNextGameMode;
	VuPauseMenu			*mpPauseMenu;

	typedef std::pair<std::string, std::string> PreloadEntry;
	typedef std::deque<PreloadEntry> PreloadQueue;
	PreloadQueue		mPreloadQueue;
	VUUINT				mPreloadIndex;
};
