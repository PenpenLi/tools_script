//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  PreloadGameMode class
// 
//*****************************************************************************

#pragma once

#include "VuGameMode.h"

class VuUIScreenEntity;
class VuProject;
class VuDBAsset;


class VuPreloadGameMode : public VuGameMode
{
	DECLARE_RTTI

public:
	VuPreloadGameMode();
	~VuPreloadGameMode();

	virtual bool		enter(const std::string &prevMode);
	virtual void		exit();
	virtual const char	*tick(float fdt);
	virtual void		draw();

private:

	void				addToQueue(const std::string &assetType, const std::string &assetName);
	VuUIScreenEntity	*getLoadingScreen();

	VuProject			*mpLoadingScreenProject;

	typedef std::pair<std::string, std::string> PreloadEntry;
	typedef std::deque<PreloadEntry> PreloadQueue;
	PreloadQueue		mPreloadQueue;
	VUUINT				mPreloadIndex;
	double				mLastDrawTime;

	typedef std::set<VUUINT32> PreloadHashes;
	PreloadHashes		mPreloadHashes;
};
