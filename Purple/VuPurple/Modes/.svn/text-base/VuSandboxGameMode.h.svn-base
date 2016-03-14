//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  SandboxGameMode class
// 
//*****************************************************************************

#pragma once

#include "VuGameMode.h"
#include "VuEngine/HAL/Keyboard/VuKeyboard.h"

class VuProject;


class VuSandboxGameMode : public VuGameMode, VuKeyboard::Callback
{
	DECLARE_RTTI

public:
	VuSandboxGameMode();
	~VuSandboxGameMode();

	virtual bool		enter(const std::string &prevMode);
	virtual void		exit();
	virtual const char	*tick(float fdt);
	virtual void		draw();

private:
	// VuKeyboard::Callback
	void				onKeyDown(VUUINT32 key);

	void				tickViewports(float fdt);

	VuProject			*mpProject;
	std::string			mNextGameMode;
};
