//*****************************************************************************
//
//  Copyright (c) 2012-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Car Shadow
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Gfx/Shadow/VuDropShadow.h"

class VuCarEntity;
class VuJsonContainer;
class VuGfxDrawParams;
class VuBlobShadow;


class VuCarShadow
{
public:
	VuCarShadow(VuCarEntity *pCar);
	~VuCarShadow();

	void					onGameInitialize() {}
	void					onGameRelease() {}

	void					preDataModified();
	void					postDataModified();

	void					onTick(float fdt);
	void					draw(const VuGfxDrawParams &params);

private:
	VuCarEntity				*mpCar;

	VuBlobShadow			*mpBlobShadow;
	VuDropShadow			*mpDropShadow;
	bool					mBlobShadowVisible;
	bool					mDropShadowVisible;
};
