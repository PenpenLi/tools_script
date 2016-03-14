//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Utilities for collision properties
// 
//*****************************************************************************

#include "VuCollisionPropertyUtil.h"
#include "VuPurple/Dynamics/VuCollisionTypes.h"
#include "VuEngine/Properties/VuProperties.h"
#include "VuEngine/Properties/VuEnumProperty.h"
#include "VuEngine/Properties/VuBitFieldProperty.h"


//*****************************************************************************
void VuCollisionPropertyUtil::addCollisionGroupProperty(VuProperties &properties, int &collisionGroup)
{
	static VuStaticIntEnumProperty::Choice sGroupChoices[] =
	{
		{ "Static Prop", COL_ENGINE_STATIC_PROP},
		{ "Dynamic Prop", COL_ENGINE_DYNAMIC_PROP},
		{ "Corona Occluder", COL_ENGINE_CORONA_OCCLUDER},
		{ "Ragdoll", COL_ENGINE_RAGDOLL},
		{ "Car", COL_GAME_CAR},
		{ "Force Affector", COL_GAME_FORCE_AFFECTOR},
		{ VUNULL }
	};
	properties.add(new VuStaticIntEnumProperty("Collision Group", collisionGroup, sGroupChoices));
}

//*****************************************************************************
void VuCollisionPropertyUtil::addCollisionMaskProperty(VuProperties &properties, VUUINT32 &collisionMask)
{
	properties.add(new VuBitFieldProperty("Collide Static Props", collisionMask, COL_ENGINE_STATIC_PROP));
	properties.add(new VuBitFieldProperty("Collide Dynamic Props", collisionMask, COL_ENGINE_DYNAMIC_PROP));
	properties.add(new VuBitFieldProperty("Collide Corona Occluders", collisionMask, COL_ENGINE_CORONA_OCCLUDER));
	properties.add(new VuBitFieldProperty("Collide Ragdolls", collisionMask, COL_ENGINE_RAGDOLL));
	properties.add(new VuBitFieldProperty("Collide Car", collisionMask, COL_GAME_CAR));
	properties.add(new VuBitFieldProperty("Collide Force Affectors", collisionMask, COL_GAME_FORCE_AFFECTOR));
}
