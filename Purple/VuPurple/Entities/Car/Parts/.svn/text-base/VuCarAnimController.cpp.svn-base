//*****************************************************************************
//
//  Copyright (c) 2009-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  CarAnimController class
// 
//*****************************************************************************

#include "VuCarAnimController.h"
#include "VuCarEngine.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuEngine/Assets/VuAssetFactory.h"
#include "VuEngine/Assets/VuAnimationAsset.h"
#include "VuEngine/Animation/VuAnimatedSkeleton.h"
#include "VuEngine/Animation/VuAnimationControl.h"
#include "VuEngine/Animation/VuAnimation.h"
#include "VuEngine/Util/VuDataUtil.h"


//*****************************************************************************
VuCarAnimController::VuCarAnimController(VuCarEntity *pCar):
	mpCar(pCar),
	mpAnimatedSkeleton(VUNULL),
	mpThrustAnimControl(VUNULL),
	mpTurnAnimControl(VUNULL),
	mThrustRevRate(-1.0f),
	mThrustIdleRate(0.0f),
	mThrustFwdRate(1.0f),
	mThrustAccel(1.0f),
	mCurTurnPos(0.0f)
{
}

//*****************************************************************************
VuCarAnimController::~VuCarAnimController()
{
}

//*****************************************************************************
void VuCarAnimController::onGameInitialize()
{
	create();
}

//*****************************************************************************
void VuCarAnimController::onGameRelease()
{
	clear();
}

//*****************************************************************************
void VuCarAnimController::setData(const VuJsonContainer &data)
{
	VuDataUtil::getValue(data["ThrustAsset"], mThrustAnimAssetName);
	VuDataUtil::getValue(data["TurnAsset"], mTurnAnimAssetName);
	VuDataUtil::getValue(data["TransformAsset"], mTransformAnimAssetName);

	VuDataUtil::getValue(data["ThrustRevRate"], mThrustRevRate);
	VuDataUtil::getValue(data["ThrustIdleRate"], mThrustIdleRate);
	VuDataUtil::getValue(data["ThrustFwdRate"], mThrustFwdRate);
	VuDataUtil::getValue(data["ThrustAccel"], mThrustAccel);
}

//*****************************************************************************
void VuCarAnimController::preDataModified()
{
	if ( mpCar->isGameInitialized() )
		clear();
}

//*****************************************************************************
void VuCarAnimController::postDataModified()
{
	if ( mpCar->isGameInitialized() )
		create();
}

//*****************************************************************************
void VuCarAnimController::onTickDecision(float fdt)
{
	// update thrust animation
	if ( mpThrustAnimControl )
	{
		// update current thrust animation speed
		float throttle = mpCar->getDriver()->getAnimationThrottleControl();

		float fCurThrustAnimSpeed = mpThrustAnimControl->getTimeFactor();
		float fTargetThrustAnimSpeed = 0.0f;
		if ( throttle > 0 )
			fTargetThrustAnimSpeed = VuLerp(mThrustIdleRate, mThrustFwdRate, throttle);
		else
			fTargetThrustAnimSpeed = VuLerp(mThrustIdleRate, mThrustRevRate, -throttle);
		float fAccel = fTargetThrustAnimSpeed > fCurThrustAnimSpeed ? mThrustAccel : -mThrustAccel;
		fCurThrustAnimSpeed += fdt*fAccel;

		mpThrustAnimControl->setTimeFactor(fCurThrustAnimSpeed);
	}

	// update turn animation
	if ( mpTurnAnimControl )
	{
		// update current turn position
		float yawControl = mpCar->getDriver()->getAnimationYawControl();
		mpTurnAnimControl->setLocalTime((0.5f + 0.5f*yawControl)*mpTurnAnimControl->getAnimation()->getEndTime());
	}
}

//*****************************************************************************
void VuCarAnimController::onTickBuild(float fdt)
{
	if ( mpAnimatedSkeleton )
	{
		mpAnimatedSkeleton->advance(fdt);
		mpAnimatedSkeleton->build();
	}
}

//*****************************************************************************
void VuCarAnimController::create()
{
	if ( mpCar->getSkeleton() )
	{
		mpAnimatedSkeleton = new VuAnimatedSkeleton(mpCar->getSkeleton());

		if ( VuAssetFactory::IF()->doesAssetExist<VuAnimationAsset>(mThrustAnimAssetName) )
		{
			mpThrustAnimControl = new VuAnimationControl(mThrustAnimAssetName);
			if ( mpThrustAnimControl->getAnimation() )
			{
				mpAnimatedSkeleton->addAnimationControl(mpThrustAnimControl);
				mpThrustAnimControl->setTimeFactor(0.0f);
			}
			else
			{
				mpThrustAnimControl->removeRef();
				mpThrustAnimControl = VUNULL;
			}
		}

		if ( VuAssetFactory::IF()->doesAssetExist<VuAnimationAsset>(mTurnAnimAssetName) )
		{
			mpTurnAnimControl = new VuAnimationControl(mTurnAnimAssetName);
			if ( mpTurnAnimControl->getAnimation() )
			{
				mpAnimatedSkeleton->addAnimationControl(mpTurnAnimControl);
				mpTurnAnimControl->setTimeFactor(0.0f);
				mpTurnAnimControl->setLocalTime(0.5f*mpTurnAnimControl->getAnimation()->getEndTime());
			}
			else
			{
				mpTurnAnimControl->removeRef();
				mpTurnAnimControl = VUNULL;
			}
		}
	}
}

//*****************************************************************************
void VuCarAnimController::clear()
{
	if ( mpAnimatedSkeleton )
	{
		mpAnimatedSkeleton->removeRef();
		mpAnimatedSkeleton = VUNULL;
	}
	if ( mpThrustAnimControl )
	{
		mpThrustAnimControl->removeRef();
		mpThrustAnimControl = VUNULL;
	}
	if ( mpTurnAnimControl )
	{
		mpTurnAnimControl->removeRef();
		mpTurnAnimControl = VUNULL;
	}
}

