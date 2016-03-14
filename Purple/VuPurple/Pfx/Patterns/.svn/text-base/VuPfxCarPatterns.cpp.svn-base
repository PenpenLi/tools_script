//*****************************************************************************
//
//  Copyright (c) 2010-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Pfx Car Patterns
// 
//*****************************************************************************

#include "VuPfxCarPatterns.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuEngine/Pfx/Particles/VuPfxTrailParticle.h"
#include "VuEngine/Pfx/Particles/VuPfxGeomParticle.h"


//*****************************************************************************
// Pfx Car Thrust Geom Pattern
//*****************************************************************************

IMPLEMENT_RTTI(VuPfxCarThrustGeomPattern, VuPfxGeomPattern);
IMPLEMENT_PFX_PATTERN_REGISTRATION(VuPfxCarThrustGeomPattern, VuPfxGeomParticle);


//*****************************************************************************
void VuPfxCarThrustGeomPatternInstance::tick(float fdt, bool ui)
{
	VuPfxGeomPatternInstance::tick(fdt, ui);

	float fThrottle = mpCar ? mpCar->getThrottleControl() : 1.0f;
	float fAlpha = VuMax(fThrottle, 0.0f);
	for ( VuPfxParticle *p = mParticles.front(); p; p = p->next() )
		p->mColor.mW = fAlpha;
}
