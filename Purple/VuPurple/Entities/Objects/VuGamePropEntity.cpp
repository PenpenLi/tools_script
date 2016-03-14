//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Game-implementation of prop entity
// 
//*****************************************************************************

#include "VuEngine/Entities/Objects/VuPropEntity.h"
#include "VuEngine/Components/3dDraw/VuVertexColorComponent.h"
#include "VuEngine/Components/3dDraw/VuDepthFogComponent.h"


class VuGamePropEntity : public VuPropEntity
{
	DECLARE_RTTI

public:
	VuGamePropEntity();

protected:
	// components
	VuVertexColorComponent	*mpVertexColorComponent;
	VuDepthFogComponent		*mpDepthFogComponent;
};


IMPLEMENT_RTTI(VuGamePropEntity, VuPropEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuGamePropEntity);


//*****************************************************************************
VuGamePropEntity::VuGamePropEntity()
{
	// components
	addComponent(mpVertexColorComponent = new VuVertexColorComponent(this));
	addComponent(mpDepthFogComponent = new VuDepthFogComponent(this));
}
