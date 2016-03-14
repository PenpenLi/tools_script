//*****************************************************************************
//
//  Copyright (c) 2010-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Pfx Car Patterns
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Pfx/Patterns/VuPfxTrailPattern.h"
#include "VuEngine/Pfx/Patterns/VuPfxGeomPattern.h"

class VuCarEntity;


//*****************************************************************************
// Pfx Car Thrust Geom Pattern
//*****************************************************************************

class VuPfxCarThrustGeomPattern : public VuPfxGeomPattern
{
	DECLARE_RTTI
	DECLARE_PFX_PATTERN
};

class VuPfxCarThrustGeomPatternInstance : public VuPfxGeomPatternInstance
{
public:
	VuPfxCarThrustGeomPatternInstance() : mpCar(VUNULL) {}

	virtual void			tick(float fdt, bool ui);

	const VuCarEntity	*mpCar;
};
