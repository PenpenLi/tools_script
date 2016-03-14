//*****************************************************************************
//
//  Copyright (c) 2008-2008 Ralf Knoesel
//  Copyright (c) 2008-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  UnusedAssetsGameMode class
// 
//*****************************************************************************

#pragma once

#include "VuGameMode.h"
#include "VuEngine/Util/VuFSM.h"


class VuUnusedAssetsGameMode : public VuGameMode
{
	DECLARE_RTTI

public:
	virtual bool		enter(const std::string &prevMode);
	virtual void		exit();
	virtual const char	*tick(float fdt);

private:
	void		onJetSkisEnter();
	void		onJetSkisTick(float fdt);
	void		onRidersEnter();
	void		onRidersTick(float fdt);
	void		onProjectsEnter();
	void		onProjectsExit();
	void		onProjectsTick(float fdt);
	void		onResultEnter();
	void		onResultTick(float fdt);

	typedef std::queue<std::string> Queue;

	VuFSM		mFSM;
	Queue		mQueue;
	int			mUnusedAssetCount;
};
