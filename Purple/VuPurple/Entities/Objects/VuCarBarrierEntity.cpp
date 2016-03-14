//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Car barrier
// 
//*****************************************************************************

#include "VuPurple/Dynamics/VuCollisionTypes.h"
#include "VuEngine/Entities/Objects/VuPropEntity.h"
#include "VuEngine/Components/RigidBody/VuRigidBodyComponent.h"


class VuCarBarrierEntity : public VuPropEntity
{
	DECLARE_RTTI

public:
	virtual void	onGameInitialize();
};


IMPLEMENT_RTTI(VuCarBarrierEntity, VuPropEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuCarBarrierEntity);


//*****************************************************************************
void VuCarBarrierEntity::onGameInitialize()
{
	VuPropEntity::onGameInitialize();

	mpRigidBodyComponent->setCollisionGroup(COL_GAME_CAR_BARRIER);
	mpRigidBodyComponent->setCollisionMask(COL_GAME_CAR);
}
