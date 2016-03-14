//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  SplitScreenGameMode class
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
class VuJsonContainer;
class VuPauseMenu;
class VuUIScreenEntity;


class VuSplitScreenGameMode : public VuGameMode, VuKeyboard::Callback
{
	DECLARE_RTTI
	DECLARE_EVENT_MAP

public:
	VuSplitScreenGameMode();
	~VuSplitScreenGameMode();

	virtual bool		enter(const std::string &prevMode);
	virtual void		exit();
	virtual const char	*tick(float fdt);
	virtual void		draw();

private:
	// VuKeyboard::Callback
	virtual void		onKeyDown(VUUINT32 key);

	void					reset();
	void					configureNextEvent();
	const VuJsonContainer	&getChampData();

	// events
	void				OnEndActionGame(const VuParams &params);
	void				OnPauseActionGame(const VuParams &params);
	void				OnResumeActionGame(const VuParams &params);
	void				OnGameRematch(const VuParams &params);

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
	void				onResetEnter();
	void				onResetExit();
	void				onResetTick(float fdt);

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

	int					mNextEventIndex;

	typedef std::pair<std::string, std::string> PreloadEntry;
	typedef std::deque<PreloadEntry> PreloadQueue;
	PreloadQueue		mPreloadQueue;
	VUUINT				mPreloadIndex;
};
