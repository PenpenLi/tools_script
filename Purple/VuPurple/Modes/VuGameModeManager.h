//*****************************************************************************
//
//  Copyright (c) 2007-2008 Ralf Knoesel
//  Copyright (c) 2008-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  GameModeManager class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/VuSystemComponent.h"


class VuGameModeManager : public VuSystemComponent
{
	DECLARE_SYSTEM_COMPONENT(VuGameModeManager)

protected:
	// called by game
	friend class VuPurple;
	virtual bool init() = 0;

public:
	virtual bool	exitToOS() = 0;
};
