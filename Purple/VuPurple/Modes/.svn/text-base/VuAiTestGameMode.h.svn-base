//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  AiTestGameMode class
// 
//*****************************************************************************

#pragma once

#include "VuGameMode.h"

#include "VuEngine/Events/VuEventMap.h"
#include "VuEngine/HAL/Keyboard/VuKeyboard.h"
#include "VuEngine/Util/VuFSM.h"

class VuProject;
class VuGame;
class VuUIScreenEntity;

const int LOG_HISTORY_SIZE = 8;

class VuAiTestGameMode : public VuGameMode, VuKeyboard::Callback
{
	DECLARE_RTTI
	DECLARE_EVENT_MAP

public:
	VuAiTestGameMode();
	~VuAiTestGameMode();

	virtual bool		enter(const std::string &prevMode);
	virtual void		exit();
	virtual const char	*tick(float fdt);
	virtual void		draw();

private:
	// events
	void				OnEndActionGame(const VuParams &params);
	void				OnRestartActionGame(const VuParams &params);
	void				OnSkipLoadingScreen(const VuParams &params);

	// VuKeyboard::Callback
	virtual void		onKeyDown(VUUINT32 key);

	void				onGameDraw();

	// states
	void				onLoadingScreenEnter();
	void				onLoadingScreenExit();
	void				onLoadingScreenTick(float fdt);
	void				onLoadingScreenDraw();
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
	VuProject			*mpGameProject;
	VuGame				*mpGame;
	std::string			mNextGameMode;
	float				mLoadLevelStartTime;
	float				mLoadingScreenDuration;
	
	// Debug
	struct VuAiTestLogItem
	{
		float		mFrameTime;
		VUUINT32	mDrawCalls;
		VUUINT32	mPrimitiveCount;
		VuVector3	mCameraPos;
		VuVector3	mCameraDir;
	};

	typedef std::list<VuAiTestLogItem> VuAiTestLog;

	VuAiTestLog					mLog;

	struct VuAiEventInfo
	{
		std::string mEvent;
		std::string mType;
		std::string mTrack;
		std::string mOpponents[5];
	};

	std::vector<VuAiEventInfo>	mEventList;

	bool						mAiTestMode;
	bool						mAllOpponents;
	bool						mAllEvents;
	bool						mLogHotSpots;
	bool						mAlwaysLogFrames;
	bool						mLogCars;
	std::vector<std::string>	mAiTestModeTracks;
	int							mNumOpponents;
	float						mLogsPerSecond;
	float						mLastLogTime;
};
