//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuCarDiscoEffect
// 
//*****************************************************************************

#include "VuCarEffect.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarChassis.h"


class VuCarDiscoEffect : public VuCarEffect
{
public:
	virtual void	onStart(const VuJsonContainer &data, const char *effectName);
	virtual void	onApply(const VuJsonContainer &data);
	virtual void	onStop(const VuJsonContainer &data);
	virtual void	onApplyForces(float fdt);

	float			mPhysicsDamping;

	VUUINT32		mAnimEffectId;
};

IMPLEMENT_CAR_EFFECT(VuCarDiscoEffect)


//*****************************************************************************
void VuCarDiscoEffect::onStart(const VuJsonContainer &data, const char *effectName)
{
	mpCar->getChassis()->pushLooseSteering();
	mAnimEffectId = mpCar->getDriver()->createEffectAnimation(data["Animation"].asString());
}

//*****************************************************************************
void VuCarDiscoEffect::onApply(const VuJsonContainer &data)
{
	mPhysicsDamping = data["PhysicsDamping"].asFloat();

	VuCarChassis::LooseSteeringParams params;
	params.mSteeringFactor = data["SteeringFactor"].asFloat();
	params.mSteeringErrorAmount = data["SteeringErrorAmount"].asFloat();
	params.mSteeringErrorInterval = data["SteeringErrorInterval"].asFloat();
	mpCar->getChassis()->setLooseSteeringParams(params);
}

//*****************************************************************************
void VuCarDiscoEffect::onStop(const VuJsonContainer &data)
{
	mpCar->getChassis()->popLooseSteering();
	mpCar->getDriver()->releaseEffectAnimation(mAnimEffectId);
}

//*****************************************************************************
void VuCarDiscoEffect::onApplyForces(float fdt)
{
	VuRigidBody *pRB = mpCar->getRigidBody();

	btVector3 linVel = pRB->getLinearVelocity();
	linVel *= btPow(btScalar(1) - mPhysicsDamping, fdt);

	pRB->setLinearVelocity(linVel);
}
