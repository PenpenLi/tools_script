//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuCarDropBallsEffect
// 
//*****************************************************************************

#include "VuCarEffect.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarEffectController.h"
#include "VuPurple/Entities/Car/Parts/VuCarSuspension.h"
#include "VuPurple/Entities/Car/Parts/VuCarEngine.h"
#include "VuPurple/Entities/Dynamic/VuSpringBallEntity.h"
#include "VuEngine/Entities/VuEntityRepository.h"
#include "VuEngine/Math/VuUnitConversion.h"
#include "VuEngine/Math/VuRand.h"


class VuCarDropBallsEffect : public VuCarEffect
{
public:
	VuCarDropBallsEffect() : mDropBallsRemaining(0), mShootBallsRemaining(0) {}

	virtual void	onApply(const VuJsonContainer &data);
	virtual void	onTick(float fdt);

	void			createBall(float angularOffset, float ballSpeed);

	int				mDropBallsRemaining;
	float			mDropSpread;
	float			mDropSpeed;
	float			mDropLaunchRate;

	int				mShootBallsRemaining;
	float			mShootSpread;
	float			mShootSpeed;
	float			mShootLaunchRate;
};

IMPLEMENT_CAR_EFFECT(VuCarDropBallsEffect)


//*****************************************************************************
void VuCarDropBallsEffect::onApply(const VuJsonContainer &data)
{
	int dropCount = data["DropCount"].asInt();
	mDropSpread = VuDegreesToRadians(data["DropSpread"].asFloat());
	mDropSpeed = VuMphToMetersPerSecond(data["DropSpeed"].asFloat());

	int shootCount = data["ShootCount"].asInt();
	mShootSpread = VuDegreesToRadians(data["ShootSpread"].asFloat());
	mShootSpeed = VuMphToMetersPerSecond(data["ShootSpeed"].asFloat());

	if ( mReapplyType == "Stack" )
	{
		mDropBallsRemaining += dropCount;
		mShootBallsRemaining += shootCount;
	}
	else
	{
		mDropBallsRemaining = dropCount;
		mShootBallsRemaining = shootCount;
	}

	mDropLaunchRate = mDropBallsRemaining/mTimeRemaining;
	mShootLaunchRate = mShootBallsRemaining/mTimeRemaining;
}

//*****************************************************************************
void VuCarDropBallsEffect::onTick(float fdt)
{
	// don't drop more than 1 per frame
	int optimalDropBallsRemaining = VuMax(VuTruncate((mTimeRemaining - fdt)*mDropLaunchRate), 0);
	if ( mDropBallsRemaining > optimalDropBallsRemaining )
	{
		float angularOffset = VuRand::global().range(-0.5f*mDropSpread, 0.5f*mDropSpread);
		createBall(VU_PI + angularOffset, mDropSpeed);
		mDropBallsRemaining--;
	}

	// don't shoot more than 1 per frame
	int optimalShootBallsRemaining = VuMax(VuTruncate((mTimeRemaining - fdt)*mShootLaunchRate), 0);
	if ( mShootBallsRemaining > optimalShootBallsRemaining )
	{
		float angularOffset = VuRand::global().range(-0.5f*mShootSpread, 0.5f*mShootSpread);
		createBall(angularOffset, mShootSpeed);
		mShootBallsRemaining--;
	}
}

//*****************************************************************************
void VuCarDropBallsEffect::createBall(float angularOffset, float ballSpeed)
{
	// calculate launch transform
	VuMatrix launchTransform;
	{
		VuVector3 rotation;
		VuRand::global().randomOrientation(rotation);

		VuVector3 position;
		position = mpCar->getTransformComponent()->getWorldTransform().transform(mpCar->getCollisionAabb().getCenter());

		launchTransform.setEulerAngles(rotation);
		launchTransform.setTrans(position);
	}

	// calculate launch velocity
	VuVector3 launchVel;
	{
		VuMatrix carTransform = mpCar->getTransformComponent()->getWorldTransform();
		carTransform.rotateZLocal(angularOffset);
		launchVel = mpCar->getLinearVelocity() + ballSpeed*carTransform.getAxisY();
	}

	const VuJsonContainer &dropBallData = (*mpData)["BallData"];
	VuEntityRepository::IF()->addManagedEntity(new VuSpringBallEntity(dropBallData, mpCar, launchTransform, launchVel));
}
