//*****************************************************************************
//
//  Copyright (c) 2007-2008 Ralf Knoesel
//  Copyright (c) 2008-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  AI Ray Test Classes
// 
//*****************************************************************************

#include "VuAiRayTest.h"
#include "VuEngine/Dynamics/VuRigidBody.h"


//*****************************************************************************
VuAiRayTestResult::VuAiRayTestResult(VuRigidBody *pIgnore, VUUINT32 collisionMask, VUUINT32 extRejectMask):
	mpIgnore(pIgnore),
	mCollisionMask(collisionMask),
	mExtRejectMask(extRejectMask)
{
}

//*****************************************************************************
bool VuAiRayTestResult::needsCollision(VuRigidBody *pRigidBody)
{
	if ( pRigidBody != mpIgnore )
	{
		if ( pRigidBody->getCollisionGroup() & mCollisionMask )
		{
			if ( !(pRigidBody->getExtendedFlags() & mExtRejectMask) )
				return true;
		}
	}

	return false;
}
