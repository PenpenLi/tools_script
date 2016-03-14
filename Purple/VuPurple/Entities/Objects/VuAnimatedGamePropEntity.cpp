//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Game-implementation of animated prop entity
// 
//*****************************************************************************

#include "VuEngine/Entities/Objects/VuAnimatedPropEntity.h"
#include "VuPurple/Util/VuCollisionPropertyUtil.h"


class VuAnimatedGamePropEntity : public VuAnimatedPropEntity
{
	DECLARE_RTTI

public:
	VuAnimatedGamePropEntity();
};


IMPLEMENT_RTTI(VuAnimatedGamePropEntity, VuAnimatedPropEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuAnimatedGamePropEntity);


//*****************************************************************************
VuAnimatedGamePropEntity::VuAnimatedGamePropEntity()
{
	// properties
	VuCollisionPropertyUtil::addCollisionGroupProperty(getProperties(), mCollisionGroup);
	VuCollisionPropertyUtil::addCollisionMaskProperty(getProperties(), mCollisionMask);
}
