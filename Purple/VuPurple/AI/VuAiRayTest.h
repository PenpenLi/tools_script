//*****************************************************************************
//
//  Copyright (c) 2007-2008 Ralf Knoesel
//  Copyright (c) 2008-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  AI Raytest Classes
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Dynamics/Util/VuDynamicsRayTest.h"

class VuRigidBody;

class VuAiRayTestResult : public VuDynamicsRayTest::VuClosestResult
{
public:
	VuAiRayTestResult(VuRigidBody *pIgnore, VUUINT32 collisionMask, VUUINT32 extRejectMask);

	virtual bool needsCollision(VuRigidBody *pRigidBody);

private:
	VuRigidBody *mpIgnore;
	VUUINT32	mCollisionMask;
	VUUINT32	mExtRejectMask;
};