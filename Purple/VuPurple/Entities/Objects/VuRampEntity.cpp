//*****************************************************************************
//
//  Copyright (c) 2009-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Ramp entity
// 
//*****************************************************************************

#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Dynamics/VuCollisionTypes.h"
#include "VuEngine/Entities/Objects/VuPropEntity.h"
#include "VuEngine/Components/3dDraw/VuVertexColorComponent.h"
#include "VuEngine/Components/3dDraw/VuDepthFogComponent.h"
#include "VuEngine/Components/RigidBody/VuRigidBodyComponent.h"
#include "VuEngine/Dynamics/VuDynamicsCallbacks.h"


class VuRampEntity : public VuPropEntity, public VuRigidBodyContactCallback
{
	DECLARE_RTTI

public:
	VuRampEntity();

	virtual void		onGameInitialize();

private:
	// AkRigidBodyContactCallback
	virtual bool		onRigidBodyContactAdded(VuContactPoint &cp);

	// components
	VuVertexColorComponent	*mpVertexColorComponent;
	VuDepthFogComponent		*mpDepthFogComponent;
};


IMPLEMENT_RTTI(VuRampEntity, VuPropEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuRampEntity);


//*****************************************************************************
VuRampEntity::VuRampEntity()
{
	// components
	addComponent(mpVertexColorComponent = new VuVertexColorComponent(this));
	addComponent(mpDepthFogComponent = new VuDepthFogComponent(this));

	mpRigidBodyComponent->setContactCallback(this);
}

//*****************************************************************************
void VuRampEntity::onGameInitialize()
{
	VuPropEntity::onGameInitialize();

	mpRigidBodyComponent->setExtendedFlags(mpRigidBodyComponent->getExtendedFlags() | EXT_COL_GAME_RAMP);
}

//*****************************************************************************
bool VuRampEntity::onRigidBodyContactAdded(VuContactPoint &cp)
{
	if ( VuEntity *pOtherEntity = cp.mpOtherBody->getEntity() )
	{
		if ( pOtherEntity->isDerivedFrom(VuCarEntity::msRTTI) )
		{
			VuCarEntity *pCar = static_cast<VuCarEntity *>(pOtherEntity);

			// boat's normal facing down?
			if ( cp.mNorWorld.mZ < -0.9f )
			{
				VuVector3 boatVel = pCar->getLinearVelocity();

				// determine lateral velocity
				VuVector3 latVel = boatVel + cp.mNorWorld*VuDot(cp.mNorWorld, boatVel);
				VuVector3 norVel = boatVel - latVel;

				if ( latVel.magSquared() > norVel.magSquared() )
				{
					cp.mCombinedFriction = -norVel.mag()/latVel.mag();
				}
			}
		}
	}

	return true;
}
