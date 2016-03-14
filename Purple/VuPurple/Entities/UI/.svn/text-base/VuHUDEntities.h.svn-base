//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  HUD classes
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/UI/VuUIInputUtil.h"


class VuHUDEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuHUDEntity();

	virtual void	onGameInitialize();
	virtual void	onGameRelease();

	virtual void	tick(float fdt, VUUINT32 padMask = 0xf);
	virtual void	draw();

private:
	// event handlers
	void			DisableInput(const VuParams &params) { mInputUtil.disable(); }
	void			EnableInput(const VuParams &params) { mInputUtil.enable(); }

	VuUIInputUtil	mInputUtil;
};
