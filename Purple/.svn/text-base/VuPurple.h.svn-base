//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Interface class to Purple application.
// 
//*****************************************************************************

#pragma once

#include "VuEngine/VuApplication.h"

class VuCmdLineArgs;


class VuPurple : public VuApplication
{
public:

	// obtaining the interface
	static VuPurple	*IF() { return &mPurple; }

	VuPurple();

	// init/release
	bool		initGame();
	bool		initEditor();
	bool		initLauncher();
	void		release();

	// does game want to exit?
	bool		exitToOS();

private:
	// sys components
	typedef std::list<VuSystemComponent *> SysComponents;
	SysComponents	mSysComponents;

	// the game
	static VuPurple	mPurple;
};
