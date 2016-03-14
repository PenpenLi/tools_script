//*****************************************************************************
//
//  Copyright (c) 2008-2008 Ralf Knoesel
//  Copyright (c) 2008-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  ResultGameMode class
// 
//*****************************************************************************

#pragma once

#include "VuGameMode.h"


class VuResultGameMode : public VuGameMode
{
	DECLARE_RTTI

public:
	virtual bool		enter(const std::string &prevMode);
	virtual void		exit();
	virtual const char	*tick(float fdt) { return ""; }
};
