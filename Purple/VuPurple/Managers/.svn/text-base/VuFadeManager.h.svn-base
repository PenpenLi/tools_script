//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Fade Manager
// 
//*****************************************************************************

#pragma once

#include "VuEngine/VuSystemComponent.h"


class VuFadeManager : public VuSystemComponent
{
	DECLARE_SYSTEM_COMPONENT(VuFadeManager)

protected:
	// called by game
	friend class VuPurple;
	virtual bool init();
	virtual void release();

public:
	VuFadeManager();

	void	startFadeIn(float duration = -1.0f);
	void	startFadeOut(float duration = -1.0f);

	bool	isFading()            { return mFadeRate != 0; }
	float	getFadeAmount()       { return mFadeAmount; }
	void	skipFrames(int count) { mSkipFrameCount += count; }

private:
	void	tick(float fdt);
	void	draw();

	float	mDefaultDuration;
	float	mFadeAmount;
	float	mFadeRate;
	int		mSkipFrameCount;
};