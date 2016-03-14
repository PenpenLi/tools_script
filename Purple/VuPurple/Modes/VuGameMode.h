//*****************************************************************************
//
//  Copyright (c) 2007-2008 Ralf Knoesel
//  Copyright (c) 2008-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  GameMode class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Objects/VuRTTI.h"

class VuGameMode
{
	DECLARE_RTTI

public:
	virtual ~VuGameMode() {}

	virtual bool		enter(const std::string &prevMode) = 0;
	virtual void		exit() = 0;
	virtual const char	*tick(float fdt) = 0;
	virtual void		draw() {}
};