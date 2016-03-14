//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Game-implementation of breakable prop entity
// 
//*****************************************************************************

#include "VuBreakablePropEntity.h"
#include "VuEngine/Components/3dDraw/VuVertexColorComponent.h"
#include "VuEngine/Components/3dDraw/VuDepthFogComponent.h"


class VuBreakableGamePropEntity : public VuBreakablePropEntity
{
	DECLARE_RTTI

public:
	VuBreakableGamePropEntity();

protected:
	// components
	VuVertexColorComponent	*mpVertexColorComponent;
	VuDepthFogComponent		*mpDepthFogComponent;
};


IMPLEMENT_RTTI(VuBreakableGamePropEntity, VuBreakablePropEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuBreakableGamePropEntity);


//*****************************************************************************
VuBreakableGamePropEntity::VuBreakableGamePropEntity()
{
	// components
	addComponent(mpVertexColorComponent = new VuVertexColorComponent(this));
	addComponent(mpDepthFogComponent = new VuDepthFogComponent(this));
}
