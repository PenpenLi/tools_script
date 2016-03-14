//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Utilities for collision
// 
//*****************************************************************************

#include "VuCollisionUtil.h"
#include "VuEngine/Dynamics/VuDynamics.h"
#include "VuEngine/Json/VuJsonContainer.h"
#include "VuEngine/Util/VuDataUtil.h"


//*****************************************************************************
bool VuCollisionUtil::createCollisionShape(const VuJsonContainer &params, btCollisionShape *&pShape, btCollisionShape *&pChildShape)
{
	pShape = VUNULL;
	pChildShape = VUNULL;

	if ( params.hasMember("CollisionBox") )
	{
		VuVector3 size(1,1,1);
		VuDataUtil::getValue(params["CollisionBox"], size);
		pShape = new btBoxShape(VuDynamicsUtil::toBtVector3(0.5f*size));
	}
	else if ( params.hasMember("CollisionSphere") )
	{
		float radius = 1;
		VuDataUtil::getValue(params["CollisionSphere"]["Radius"], radius);
		pShape = new btSphereShape(radius);
	}
	else if ( params.hasMember("CollisionCylinderZ") )
	{
		float radius = 1, height = 1;
		VuDataUtil::getValue(params["CollisionCylinderZ"]["Radius"], radius);
		VuDataUtil::getValue(params["CollisionCylinderZ"]["Height"], height);
		pShape = new btCylinderShapeZ(btVector3(radius, radius, 0.5f*height));
	}
	else
	{
		return false;
	}

	if ( params.hasMember("CollisionOffset") )
	{
		VuVector3 offset(0,0,0);
		VuDataUtil::getValue(params["CollisionOffset"], offset);

		pChildShape = pShape;
		btCompoundShape *pCompoundShape = new btCompoundShape;
		pShape = pCompoundShape;

		btTransform xform;
		xform.setIdentity();
		xform.setOrigin(VuDynamicsUtil::toBtVector3(offset));
		pCompoundShape->addChildShape(xform, pChildShape);
	}

	return true;
}
