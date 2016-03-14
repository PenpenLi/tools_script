//*****************************************************************************
//
//  Copyright (c) 2009-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  CarPfxController class
// 
//*****************************************************************************

#include "VuCarPfxController.h"
#include "VuCarPowerUpController.h"
#include "VuCarEffectController.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarEngine.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Managers/VuCollisionManager.h"
#include "VuPurple/Pfx/Patterns/VuPfxCarPatterns.h"
#include "VuEngine/Pfx/VuPfx.h"
#include "VuEngine/Pfx/VuPfxEntity.h"
#include "VuEngine/Pfx/VuPfxManager.h"
#include "VuEngine/Pfx/VuPfxSystem.h"
#include "VuEngine/Pfx/VuPfxPattern.h"
#include "VuEngine/Pfx/VuPfxProcess.h"
#include "VuEngine/Water/VuFluidsObject.h"
#include "VuEngine/Gfx/VuGfxDrawParams.h"
#include "VuEngine/Gfx/Model/VuAnimatedModelInstance.h"
#include "VuEngine/Animation/VuSkeleton.h"
#include "VuEngine/Assets/VuFluidsMeshAsset.h"
#include "VuEngine/Managers/VuLensWaterManager.h"
#include "VuEngine/Math/VuUnitConversion.h"
#include "VuEngine/Json/VuJsonContainer.h"
#include "VuEngine/Util/VuDataUtil.h"


// constants

#define WHEEL_DRIVE_PFX_MIN_SPEED VuMphToMetersPerSecond(10.0f)
#define MAX_SPLASH_DROPLET_RATE 64.0f // droplets/sec


//*****************************************************************************
VuCarPfxController::VuCarPfxController(VuCarEntity *pCar):
	mpCar(pCar),
	mBackFireMaxDuration(1.0f),
	mpChassisEffect(VUNULL),
	mpBackFirePfxSystem(VUNULL),
	mBackFirePfxOn(false),
	mBackFireTimer(0.0f)
{
	mpFluidsObject = new VuFluidsObject;
}

//*****************************************************************************
VuCarPfxController::~VuCarPfxController()
{
	delete mpFluidsObject;
}

//*****************************************************************************
void VuCarPfxController::onGameInitialize()
{
	create();
}

//*****************************************************************************
void VuCarPfxController::onGameRelease()
{
	clear();
}

//*****************************************************************************
void VuCarPfxController::setData(const VuJsonContainer &data)
{
	VuDataUtil::getValue(data["ChassisEffect"]["Fluids Mesh Asset"], mFluidsMeshAssetName);
	VuDataUtil::getValue(data["ChassisEffect"]["Effect"], mChassisEffectName);
	VuDataUtil::getValue(data["BackFire"], mBackFirePfxName);
	VuDataUtil::getValue(data["BackFireBlue"], mBackFireBluePfxName);
	VuDataUtil::getValue(data["BackFireMaxDuration"], mBackFireMaxDuration);
	VuDataUtil::getValue(data["PowerSlide"], mPowerSlidePfxName);
}

//*****************************************************************************
void VuCarPfxController::preDataModified()
{
	if ( mpCar->isGameInitialized() )
		clear();
}

//*****************************************************************************
void VuCarPfxController::postDataModified()
{
	mpFluidsObject->setAsset(mFluidsMeshAssetName);

	if ( mpCar->isGameInitialized() )
		create();
}

//*****************************************************************************
void VuCarPfxController::applyTuneUp()
{
	if ( mpBackFirePfxSystem )
		VuPfx::IF()->releaseSystemInstance(mpBackFirePfxSystem);

	mpBackFirePfxSystem = VuPfx::IF()->createSystemInstance(mBackFireBluePfxName.c_str());
}

//*****************************************************************************
void VuCarPfxController::onTickBuild(float fdt)
{
	bool bIsCameraTarget = mpCar->getIsCameraTarget();
	VuCarEffectController *pEffectController = mpCar->getEffectController();

	// backfire
	if ( mpCar->getEngine()->isBackFiring() )
		mBackFireTimer += fdt;
	else
		mBackFireTimer = 0.0f;
	int engineBackFire = mpCar->getEngine()->isBackFiring() && (mBackFireTimer < mBackFireMaxDuration);
	engineBackFire |= pEffectController->isActive(VuCarEffectController::BOOST);

	if ( mpBackFirePfxSystem )
	{
		if ( !mBackFirePfxOn )
		{
			if ( engineBackFire )
			{
				mBackFirePfxOn = true;
				mpBackFirePfxSystem->start();
			}
		}
		else
		{
			if ( !engineBackFire )
			{
				mBackFirePfxOn = false;
				mpBackFirePfxSystem->stop();
			}
		}
	}

	mAabb.reset();

	if ( mpChassisEffect )
	{
		mpChassisEffect->setPosition(mpCar->getModelPosition());
		mpChassisEffect->setColor(mpCar->getPfxColor());
		mpChassisEffect->tick(fdt, false);
		if ( mpChassisEffect->getAabb().isValid() )
			mAabb.addAabb(mpChassisEffect->getAabb());
	}

	if ( mpBackFirePfxSystem )
	{
		mpBackFirePfxSystem->setMatrix(mpCar->getModelMatrix());
		mpBackFirePfxSystem->tick(fdt, false);
		if ( mpBackFirePfxSystem->getAabb().isValid() )
			mAabb.addAabb(mpBackFirePfxSystem->getAabb());
	}

	// wheel effects
	tickWheelEffects(fdt);
}

//*****************************************************************************
void VuCarPfxController::draw(const VuGfxDrawParams &params)
{
	if ( !params.mbDrawReflection )
	{
		if ( mpChassisEffect )
			mpChassisEffect->draw(params);

		if ( mpBackFirePfxSystem )
			mpBackFirePfxSystem->draw(params);

		for ( int i = 0; i < 4; i++ )
		{
			VuCarWheel &wheel = mpCar->getWheel(i);
			VuPfxWheel &pfxWheel = mPfxWheels[i];

			if ( pfxWheel.mpDriveSystem )
				pfxWheel.mpDriveSystem->draw(params);
			if ( pfxWheel.mpSlideSystem )
				pfxWheel.mpSlideSystem->draw(params);
			if ( pfxWheel.mpPowerSlideSystem )
				pfxWheel.mpPowerSlideSystem->draw(params);
			if ( wheel.mpWaterPfx )
				wheel.mpWaterPfx->draw(params);
		}
	}
}

//*****************************************************************************
void VuCarPfxController::clear()
{
	if ( mpChassisEffect )
	{
		VuPfx::IF()->releaseSystemInstance(mpChassisEffect);
		mpChassisEffect = VUNULL;
	}

	if ( mpBackFirePfxSystem )
	{
		VuPfx::IF()->releaseSystemInstance(mpBackFirePfxSystem);
		mpBackFirePfxSystem = VUNULL;
	}

	for ( int i = 0; i < 4; i++ )
	{
		VuPfxWheel &pfxWheel = mPfxWheels[i];

		if ( pfxWheel.mpDriveSystem )		{ VuPfx::IF()->releaseSystemInstance(pfxWheel.mpDriveSystem); pfxWheel.mpDriveSystem = VUNULL; }
		if ( pfxWheel.mpSlideSystem )		{ VuPfx::IF()->releaseSystemInstance(pfxWheel.mpSlideSystem); pfxWheel.mpSlideSystem = VUNULL; }
		if ( pfxWheel.mpPowerSlideSystem )	{ VuPfx::IF()->releaseSystemInstance(pfxWheel.mpPowerSlideSystem); pfxWheel.mpPowerSlideSystem = VUNULL; }
	}
}

//*****************************************************************************
void VuCarPfxController::getPfxEmitCarChassisState(VuMatrix &transform, VuVector3 &linVel, VUUINT32 &waterSurfaceFlags)
{
	transform = mpCar->getModelMatrix();
	linVel = mpCar->getLinearVelocity();
	waterSurfaceFlags = mpCar->getWaterSurfaceFlags();
}

//*****************************************************************************
void VuCarPfxController::create()
{
	if ( mpFluidsObject->getFluidsMeshAsset() )
	{
		if ( (mpChassisEffect = VuPfx::IF()->createSystemInstance(mChassisEffectName.c_str())) )
		{
			// set custom pfx process data
			for ( VuPfxPatternInstance *pPatternInst = mpChassisEffect->mPatterns.front(); pPatternInst; pPatternInst = pPatternInst->next() )
			{
				for ( VuPfxProcessInstance *pProcessInst = pPatternInst->mProcesses.front(); pProcessInst; pProcessInst = pProcessInst->next() )
				{
					if ( pProcessInst->mpParams->isDerivedFrom(VuPfxEmitCarChassis::msRTTI) )
					{
						static_cast<VuPfxEmitCarChassisInstance *>(pProcessInst)->mpFluidsObject = mpFluidsObject;
						static_cast<VuPfxEmitCarChassisInstance *>(pProcessInst)->mpStateIF = this;
					}
				}
			}

			mpChassisEffect->start();
		}
	}

	mpBackFirePfxSystem = VuPfx::IF()->createSystemInstance(mBackFirePfxName.c_str());

	for ( int i = 0; i < 4; i++ )
		mPfxWheels[i].mpPowerSlideSystem = VuPfx::IF()->createSystemInstance(mPowerSlidePfxName.c_str());
}

//*****************************************************************************
void VuCarPfxController::tickWheelEffects(float fdt)
{
	bool playDrivePfx = mpCar->getLinearVelocity().mag() > WHEEL_DRIVE_PFX_MIN_SPEED;
	int wheelSplashPfxCount = 0;

	int overWater = mpCar->getWaterSurfaceFlags() & WATER_SURFACE_WATER;

	for ( int i = 0; i < 4; i++ )
	{
		VuCarWheel &wheel = mpCar->getWheel(i);
		VuPfxWheel &pfxWheel = mPfxWheels[i];

		float wheelRadius = wheel.getRadius();

		if ( wheel.mVisualContact )
		{
			if ( wheel.mVisualContactSurfaceType != pfxWheel.mSurfaceType )
			{
				const char *wheelDrivePfxName = VuCollisionManager::IF()->getWheelDrivePfxName(wheel.mVisualContactSurfaceType);
				if ( wheelDrivePfxName && (wheelDrivePfxName != pfxWheel.mDrivePfxName) )
				{
					if ( pfxWheel.mpDriveSystem )
						VuPfx::IF()->releaseSystemInstance(pfxWheel.mpDriveSystem);
					pfxWheel.mpDriveSystem = VuPfx::IF()->createSystemInstance(wheelDrivePfxName);

					pfxWheel.mDrivePfxName = wheelDrivePfxName;
				}

				const char *wheelSlidePfxName = VuCollisionManager::IF()->getWheelSlidePfxName(wheel.mVisualContactSurfaceType);
				if ( wheelSlidePfxName && (wheelSlidePfxName != pfxWheel.mSlidePfxName) )
				{
					if ( pfxWheel.mpSlideSystem )
						VuPfx::IF()->releaseSystemInstance(pfxWheel.mpSlideSystem);
					pfxWheel.mpSlideSystem = VuPfx::IF()->createSystemInstance(wheelSlidePfxName);

					pfxWheel.mSlidePfxName = wheelSlidePfxName;
				}

				pfxWheel.mSurfaceType = wheel.mVisualContactSurfaceType;
			}
		}

		// update
		VuMatrix transform = mpCar->getModelMatrix();
		transform.translateLocal(wheel.mAttachmentPointLS + VuVector3(0.0f, 0.0f, wheel.mVisualSuspensionDist - wheelRadius));
		transform.rotateZLocal(wheel.mSteeringAngle);

		VuVector3 linVel = VuDynamicsUtil::pointVelocityWorld(*mpCar->getRigidBody(), transform.getTrans());

		if ( pfxWheel.mpDriveSystem )
		{
			if ( playDrivePfx && wheel.mVisualContact && !wheel.sliding() )
			{
				if ( pfxWheel.mpDriveSystem->getState() != VuPfxSystemInstance::STATE_ALIVE )
					pfxWheel.mpDriveSystem->start();
			}
			else
			{
				if ( pfxWheel.mpDriveSystem->getState() == VuPfxSystemInstance::STATE_ALIVE )
					pfxWheel.mpDriveSystem->stop();
			}

			pfxWheel.mpDriveSystem->setMatrix(transform);
			pfxWheel.mpDriveSystem->setLinearVelocity(linVel);
			pfxWheel.mpDriveSystem->setColor(mpCar->getPfxColor());
			pfxWheel.mpDriveSystem->tick(fdt, false);

			if ( pfxWheel.mpDriveSystem->getAabb().isValid() )
				mAabb.addAabb(pfxWheel.mpDriveSystem->getAabb());
		}

		if ( pfxWheel.mpSlideSystem )
		{
			if ( (wheel.sliding()) && !wheel.mSubmerged )
			{
				if ( pfxWheel.mpSlideSystem->getState() != VuPfxSystemInstance::STATE_ALIVE )
					pfxWheel.mpSlideSystem->start();
			}
			else
			{
				if ( pfxWheel.mpSlideSystem->getState() == VuPfxSystemInstance::STATE_ALIVE )
					pfxWheel.mpSlideSystem->stop();
			}

			pfxWheel.mpSlideSystem->setMatrix(transform);
			pfxWheel.mpSlideSystem->setLinearVelocity(linVel);
			pfxWheel.mpSlideSystem->setColor(mpCar->getPfxColor());
			pfxWheel.mpSlideSystem->tick(fdt, false);

			if ( pfxWheel.mpSlideSystem->getAabb().isValid() )
				mAabb.addAabb(pfxWheel.mpSlideSystem->getAabb());
		}

		if ( pfxWheel.mpPowerSlideSystem )
		{
			if ( mpCar->isPowerSliding() )
			{
				if ( pfxWheel.mpPowerSlideSystem->getState() != VuPfxSystemInstance::STATE_ALIVE )
					pfxWheel.mpPowerSlideSystem->start();
			}
			else
			{
				if ( pfxWheel.mpPowerSlideSystem->getState() == VuPfxSystemInstance::STATE_ALIVE )
					pfxWheel.mpPowerSlideSystem->stop();
			}

			pfxWheel.mpPowerSlideSystem->setMatrix(transform);
			pfxWheel.mpPowerSlideSystem->setLinearVelocity(linVel);
			pfxWheel.mpPowerSlideSystem->setColor(mpCar->getPfxColor());
			pfxWheel.mpPowerSlideSystem->tick(fdt, false);

			if ( pfxWheel.mpPowerSlideSystem->getAabb().isValid() )
				mAabb.addAabb(pfxWheel.mpPowerSlideSystem->getAabb());
		}

		if ( wheel.mpWaterPfx )
		{
			if ( wheel.mSubmerged && overWater && (wheel.mTransform.getTrans().mZ + wheelRadius) > wheel.mVisualWaterHeight )
			{
				if ( wheel.mpWaterPfx->getState() != VuPfxSystemInstance::STATE_ALIVE )
					wheel.mpWaterPfx->start();

				wheelSplashPfxCount++;
			}
			else
			{
				if ( wheel.mpWaterPfx->getState() == VuPfxSystemInstance::STATE_ALIVE )
					wheel.mpWaterPfx->stop();
			}

			transform.mT.mZ = wheel.mVisualWaterHeight;

			wheel.mpWaterPfx->setMatrix(transform);
			wheel.mpWaterPfx->setLinearVelocity(linVel);
			wheel.mpWaterPfx->setColor(mpCar->getPfxColor());
			wheel.mpWaterPfx->tick(fdt, false);

			if ( wheel.mpWaterPfx->getAabb().isValid() )
				mAabb.addAabb(wheel.mpWaterPfx->getAabb());
		}
	}

	// lens water
	if ( wheelSplashPfxCount )
	{
		int viewport;
		if ( VuCarManager::IF()->getViewport(mpCar, viewport) )
		{
			float speedMin = VuMphToMetersPerSecond(20.0f);
			float speedMax = VuMphToMetersPerSecond(40.0f);
			float amount = (mpCar->getLinearVelocity().mag() - speedMin)/(speedMax - speedMin);
			amount = VuClamp(amount, 0.0f, 1.0f);

			amount *= 0.25f*wheelSplashPfxCount;

			VuLensWaterManager::IF()->addDroplets(viewport, amount*fdt*MAX_SPLASH_DROPLET_RATE);
		}
	}
}

//*****************************************************************************
VuCarPfxController::VuPfxWheel::VuPfxWheel():
	mSurfaceType(0xff),
	mDrivePfxName(VUNULL),
	mSlidePfxName(VUNULL),
	mpDriveSystem(VUNULL),
	mpSlideSystem(VUNULL),
	mpPowerSlideSystem(VUNULL)
{
}