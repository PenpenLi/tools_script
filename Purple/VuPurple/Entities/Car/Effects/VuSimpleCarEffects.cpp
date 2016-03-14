//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Simple car effects
// 
//*****************************************************************************

#include "VuCarEffect.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarEffectController.h"
#include "VuPurple/Entities/Car/Parts/VuCarSuspension.h"
#include "VuPurple/Entities/Car/Camera/VuCarCamera.h"
#include "VuPurple/PowerUps/VuPowerUpManager.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Managers/VuAchievementManager.h"
#include "VuEngine/Math/VuUnitConversion.h"
#include "VuEngine/Util/VuDataUtil.h"


//*****************************************************************************
// VuCarStartGlobalEffect
//*****************************************************************************

class VuCarStartGlobalEffect : public VuCarEffect
{
public:
	virtual void onStart(const VuJsonContainer &data, const char *effectName)
	{
		const std::string &carEffect = data["CarEffect"].asString();

		if ( VuPowerUpManager::IF()->getForcedPowerUp() && VuPowerUpManager::IF()->getForcedPowerUp()->mName == effectName )
		{
			mpCar->getEffectController()->applyEffect(carEffect.c_str(), mpCar);
		}
		else
		{
			for ( int i = 0; i < VuCarManager::IF()->getCarCount(); i++ )
			{
				VuCarEntity *pOpponentCar = VuCarManager::IF()->getCar(i);
				if ( mpCar != pOpponentCar )
				{
					float startDistance = data["StartDistance"].asFloat();

					if ( pOpponentCar->getStats().mDistFromStart - mpCar->getStats().mDistFromStart > startDistance )
						pOpponentCar->getEffectController()->applyEffect(carEffect.c_str(), mpCar);
				}
			}
		}
	}
};

IMPLEMENT_CAR_EFFECT(VuCarStartGlobalEffect)


//*****************************************************************************
// VuCarRagdollEffect
//*****************************************************************************

class VuCarRagdollEffect : public VuCarEffect
{
public:
	virtual void onApply(const VuJsonContainer &data)
	{
		mpCar->startCrash();
		if ( mpOriginator && mpOriginator->isDerivedFrom(VuCarEntity::msRTTI) )
		{
			VuCarEntity *pOtherCar = static_cast<VuCarEntity *>(mpOriginator);
			VuVector3 pos = mpCar->getModelMatrix().transform(mpCar->getCollisionAabb().getCenter());
			pOtherCar->addScore(data["Scoring"].asCString(), "Car", pos);
		}
	}
};

IMPLEMENT_CAR_EFFECT(VuCarRagdollEffect)


//*****************************************************************************
// VuCarChangeTiresEffect
//*****************************************************************************

class VuCarChangeTiresEffect : public VuCarEffect
{
public:
	virtual void onApply(const VuJsonContainer &data)
	{
		for ( int i = 0; i < 4; i++ )
			mpCar->getWheel(i).mCurScale = data["WheelScale"].asFloat();

		VuCarSuspension *pSuspension = mpCar->getSuspension();
		pSuspension->setSpringFactor(data["SpringFactor"].asFloat());
		pSuspension->setDampingFactor(data["DampingFactor"].asFloat());

		pSuspension->setBounceSfx(data["BounceSfx"].asCString(), data["BounceSfxThreshold"].asFloat());
	}
	virtual void onStop(const VuJsonContainer &data)
	{
		for ( int i = 0; i < 4; i++ )
			mpCar->getWheel(i).mCurScale = 1.0f;

		VuCarSuspension *pSuspension = mpCar->getSuspension();
		pSuspension->setSpringFactor(1.0f);
		pSuspension->setDampingFactor(1.0f);

		pSuspension->setBounceSfx("", 0.0f);
	}
};

IMPLEMENT_CAR_EFFECT(VuCarChangeTiresEffect)


//*****************************************************************************
// VuCarConfusionEffect
//*****************************************************************************

class VuCarConfusionEffect : public VuCarEffect
{
public:
	virtual void onStart(const VuJsonContainer &data, const char *effectName)
	{
		mpCar->getEffectController()->pushSubEffect(VuCarEffectController::CONFUSION);
	}
	virtual void onApply(const VuJsonContainer &data)
	{
		mStartTimer = data["StartDelay"].asFloat();
	}
	virtual void onStop(const VuJsonContainer &data)
	{
		mpCar->setYawControlFactor(1.0f);
		mpCar->getEffectController()->popSubEffect(VuCarEffectController::CONFUSION);
	}
	virtual void onTick(float fdt)
	{
		mStartTimer -= fdt;
		if ( mStartTimer <= 0.0f )
			mpCar->setYawControlFactor(-1.0f);
	}

	float	mStartTimer;
};

IMPLEMENT_CAR_EFFECT(VuCarConfusionEffect)


//*****************************************************************************
// VuLowGravityEffect
//*****************************************************************************

class VuLowGravityEffect : public VuCarEffect
{
public:
	virtual void onStart(const VuJsonContainer &data, const char *effectName)
	{
		mpCar->getEffectController()->pushSubEffect(VuCarEffectController::LOW_GRAVITY);
	}
	virtual void onApply(const VuJsonContainer &data)
	{
		mpCar->getRigidBody()->setGravity(VuDynamicsUtil::toBtVector3(data["GravityFactor"].asFloat()*VuDynamics::IF()->getGravity()));

		VuVector3 linVel = mpCar->getLinearVelocity();
		linVel.mZ += VuMphToMetersPerSecond(data["VerticalSpeed"].asFloat());
		mpCar->getRigidBody()->setVuLinearVelocity(linVel);
	}
	virtual void onStop(const VuJsonContainer &data)
	{
		mpCar->getRigidBody()->setGravity(VuDynamicsUtil::toBtVector3(VuDynamics::IF()->getGravity()));
		mpCar->getEffectController()->popSubEffect(VuCarEffectController::LOW_GRAVITY);
	}
};

IMPLEMENT_CAR_EFFECT(VuLowGravityEffect)


//*****************************************************************************
// VuCarChangeTractionEffect
//*****************************************************************************

class VuCarChangeTractionEffect : public VuCarEffect
{
public:
	virtual void onStart(const VuJsonContainer &data, const char *effectName)
	{
		mpCar->getEffectController()->pushTraction();
	}
	virtual void onApply(const VuJsonContainer &data)
	{
		float tractionFactor = data["TractionFactor"].asFloat();
		mpCar->getSuspension()->setTractionFactor(tractionFactor);

		float wheelScaleX = 1.0f;
		data["WheelScaleX"].getValue(wheelScaleX);
		for ( int i = 0; i < 4; i++ )
			mpCar->getWheel(i).mCurScaleX = wheelScaleX;
	}
	virtual void onStop(const VuJsonContainer &data)
	{
		mpCar->getEffectController()->popTraction();

		for ( int i = 0; i < 4; i++ )
			mpCar->getWheel(i).mCurScaleX = 1.0f;
	}
};

IMPLEMENT_CAR_EFFECT(VuCarChangeTractionEffect)


//*****************************************************************************
// VuCarShieldEffect
//*****************************************************************************

class VuCarShieldEffect : public VuCarEffect
{
public:
	virtual void onStart(const VuJsonContainer &data, const char *effectName)
	{
		mpCar->getEffectController()->pushSubEffect(VuCarEffectController::SHIELD);

		// removable effects
		const VuJsonContainer &removableEffects = data["RemoveEffects"];
		for ( const auto &effect : mpCar->getEffectController()->getActiveEffects() )
		{
			const std::string &effectType = (*effect.second->mpData)["Type"].asString();
			for ( int i = 0; i < removableEffects.size(); i++ )
			{
				if ( removableEffects[i].asString() == effectType )
				{
					effect.second->mTimeRemaining = 0.0f;
					break;
				}
			}
		}

		// stop any camera shaking
		mpCar->getCamera()->stopShake();
	}
	virtual void onStop(const VuJsonContainer &data)
	{
		mpCar->getEffectController()->popSubEffect(VuCarEffectController::SHIELD);
	}
};

IMPLEMENT_CAR_EFFECT(VuCarShieldEffect)


//*****************************************************************************
// VuCarToughnessEffect
//*****************************************************************************

class VuCarToughnessEffect : public VuCarEffect
{
public:
	virtual void onStart(const VuJsonContainer &data, const char *effectName)
	{
		mpCar->getEffectController()->pushToughness(data["SmashCarEffect"].asCString());
	}
	virtual void onStop(const VuJsonContainer &data)
	{
		mpCar->getEffectController()->popToughness();
	}
};

IMPLEMENT_CAR_EFFECT(VuCarToughnessEffect)


//*****************************************************************************
// VuCarOnFireEffect
//*****************************************************************************

class VuCarOnFireEffect : public VuCarEffect
{
public:
	virtual void onStart(const VuJsonContainer &data, const char *effectName)
	{
		mpCar->getEffectController()->pushSubEffect(VuCarEffectController::ON_FIRE);
		mAnimEffectId = mpCar->getDriver()->createEffectAnimation(data["Animation"].asString());

		if ( mpCar->getEffectController()->isActive(VuCarEffectController::FREEZE) )
			if ( mpOriginator && mpOriginator->getDriver()->isHuman() )
				VuAchievementManager::IF()->unlock(40); // Set fire to an opponent who's frozen
	}
	virtual void onApply(const VuJsonContainer &data)
	{
		mPhysicsDamping = data["PhysicsDamping"].asFloat();
	}
	virtual void onStop(const VuJsonContainer &data)
	{
		mpCar->getEffectController()->popSubEffect(VuCarEffectController::ON_FIRE);
		mpCar->getDriver()->releaseEffectAnimation(mAnimEffectId);
	}
	virtual void onApplyForces(float fdt)
	{
		VuRigidBody *pRB = mpCar->getRigidBody();

		btVector3 linVel = pRB->getLinearVelocity();
		linVel *= btPow(btScalar(1) - mPhysicsDamping, fdt);

		pRB->setLinearVelocity(linVel);
	}
	float		mPhysicsDamping;
	VUUINT32	mAnimEffectId;
};

IMPLEMENT_CAR_EFFECT(VuCarOnFireEffect)


//*****************************************************************************
//  VuCarPlantVictimEffect
//*****************************************************************************

class VuCarPlantVictimEffect : public VuCarEffect
{
public:
	virtual void onApply(const VuJsonContainer &data)
	{
		mPhysicsDamping = data["PhysicsDamping"].asFloat();
		mPhysicsEndTime = data["Duration"].asFloat() - data["PhysicsDuration"].asFloat();
	}

	virtual void onApplyForces(float fdt)
	{
		if ( mTimeRemaining > mPhysicsEndTime )
		{
			VuRigidBody *pRB = mpCar->getRigidBody();

			btVector3 linVel = pRB->getLinearVelocity();
			linVel *= btPow(btScalar(1) - mPhysicsDamping, fdt);

			pRB->setLinearVelocity(linVel);
		}
	}

	float				mPhysicsDamping;
	float				mPhysicsEndTime;
};

IMPLEMENT_CAR_EFFECT(VuCarPlantVictimEffect)


//*****************************************************************************
//  VuCarSplatEffect
//*****************************************************************************

class VuCarSplatEffect : public VuCarEffect
{
public:
	virtual void onApply(const VuJsonContainer &data)
	{
		mFinishLineAchievement = data["FinishLineAchievement"].asInt();
	}

	virtual void onTick(float fdt)
	{
		if ( mFinishLineAchievement && mpCar->getHasFinished() && mpCar->getDriver()->isHuman() && (mpCar->getStats().mPlace == 1) )
			VuAchievementManager::IF()->unlock(mFinishLineAchievement); // Win with something in your face
	}

	int		mFinishLineAchievement;
};

IMPLEMENT_CAR_EFFECT(VuCarSplatEffect)


//*****************************************************************************
//  VuCarSpringEffect
//*****************************************************************************

class VuCarSpringEffect : public VuCarEffect
{
public:
	virtual void onApply(const VuJsonContainer &data)
	{
		float verticalSpeed = VuMphToMetersPerSecond(data["VerticalSpeed"].asFloat());

		VuVector3 linVel = mpCar->getLinearVelocity();
		if ( linVel.mZ < verticalSpeed )
		{
			linVel.mZ = verticalSpeed;
			mpCar->getRigidBody()->setVuLinearVelocity(linVel);
		}
	}
};

IMPLEMENT_CAR_EFFECT(VuCarSpringEffect)


//*****************************************************************************
//  VuCarBoostStartEffect
//*****************************************************************************

class VuCarBoostStartEffect : public VuCarEffect
{
public:
	virtual void onApply(const VuJsonContainer &data)
	{
		float angle = VuDegreesToRadians(data["Angle"].asFloat());
		float speedXY = data["Amount"].asFloat()*mpCar->getMaxForwardSpeed();
		float speedZ = speedXY*VuTan(angle);

		VuVector3 linVel = speedXY*mpCar->getModelMatrix().getAxisY() + speedZ*mpCar->getModelMatrix().getAxisZ();
		mpCar->getRigidBody()->setVuLinearVelocity(mpCar->getLinearVelocity() + linVel );
	}
};

IMPLEMENT_CAR_EFFECT(VuCarBoostStartEffect)


//*****************************************************************************
// VuCarGhostEffect
//*****************************************************************************

class VuCarGhostEffect : public VuCarShieldEffect
{
public:
	virtual void onStart(const VuJsonContainer &data, const char *effectName)
	{
		VuCarShieldEffect::onStart(data, effectName);

		mpCar->getEffectController()->pushSubEffect(VuCarEffectController::GHOST);
		mpCar->pushDisableCollisions();

		float alpha = 1.0f;
		data["CarAlpha"].getValue(alpha);
		mpCar->setAlpha(alpha);

		VuColor overrideColor;
		if ( VuDataUtil::getValue(data["OverrideColor"], overrideColor) )
			mpCar->setOverrideDrawColor(overrideColor);
	}
	virtual void onStop(const VuJsonContainer &data)
	{
		mpCar->setAlpha(1.0f);

		mpCar->getEffectController()->popSubEffect(VuCarEffectController::GHOST);
		mpCar->popDisableCollisions();

		mpCar->clearOverrideDrawColor();

		VuCarShieldEffect::onStop(data);
	}
};

IMPLEMENT_CAR_EFFECT(VuCarGhostEffect)


