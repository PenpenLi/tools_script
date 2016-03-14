//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  CarEffectController class
// 
//*****************************************************************************

#include "VuCarEffectController.h"
#include "VuCarSuspension.h"

#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Effects/VuCarEffect.h"
#include "VuPurple/Dynamics/VuCollisionTypes.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"

//*****************************************************************************
// Macro used by registries to register entities from their init() method.
#define REGISTER_CAR_EFFECT(className) \
{ \
	extern VuCarEffect *Create##className(); \
	extern VuEntity *Create##className(const char *strType); \
	mEffectCreators[#className] = Create##className; \
}

//*****************************************************************************
VuCarEffectController::VuCarEffectController(VuCarEntity *pCar):
	mpCar(pCar),
	mWheelSurfaceOverrideCount(0)
{
	memset(mSubEffects, 0, sizeof(mSubEffects));

	// register effects
	REGISTER_CAR_EFFECT(VuCarShootMissileEffect);
	REGISTER_CAR_EFFECT(VuCarShootMissilesEffect);
	REGISTER_CAR_EFFECT(VuCarShootFirstMissileEffect);
	REGISTER_CAR_EFFECT(VuCarShootBestLineMissileEffect);
	REGISTER_CAR_EFFECT(VuCarScatterShotEffect);
	REGISTER_CAR_EFFECT(VuCarShootEarthStrikeEffect);
	REGISTER_CAR_EFFECT(VuCarShootHydraEffect);
	REGISTER_CAR_EFFECT(VuCarShootLightningEffect);
	REGISTER_CAR_EFFECT(VuCarDropOilSlickEffect);
	REGISTER_CAR_EFFECT(VuCarDropBreakableEffect);
	REGISTER_CAR_EFFECT(VuCarDropFakeEffect);
	REGISTER_CAR_EFFECT(VuCarDropSpringEffect);
	REGISTER_CAR_EFFECT(VuCarShieldEffect);
	REGISTER_CAR_EFFECT(VuCarToughnessEffect);
	REGISTER_CAR_EFFECT(VuCarStartGlobalEffect);
	REGISTER_CAR_EFFECT(VuCarSuperEffect);
	REGISTER_CAR_EFFECT(VuCarChangeTractionEffect);
	REGISTER_CAR_EFFECT(VuCarBoostEffect);
	REGISTER_CAR_EFFECT(VuCarDropBallsEffect);
	REGISTER_CAR_EFFECT(VuCarTranslocateEffect);
	REGISTER_CAR_EFFECT(VuCarPlantEffect);
	REGISTER_CAR_EFFECT(VuCarRagdollEffect);
	REGISTER_CAR_EFFECT(VuCarFreezeEffect);
	REGISTER_CAR_EFFECT(VuCarOilyTiresEffect);
	REGISTER_CAR_EFFECT(VuCarChangeTiresEffect);
	REGISTER_CAR_EFFECT(VuCarConfusionEffect);
	REGISTER_CAR_EFFECT(VuLowGravityEffect);
	REGISTER_CAR_EFFECT(VuCarNitroEffect);
	REGISTER_CAR_EFFECT(VuCarRemoteControlEffect);
	REGISTER_CAR_EFFECT(VuCarSplatEffect);
	REGISTER_CAR_EFFECT(VuCarAttachRagdollEffect);
	REGISTER_CAR_EFFECT(VuCarTornadoEffect);
	REGISTER_CAR_EFFECT(VuCarEarthquakeEffect);
	REGISTER_CAR_EFFECT(VuCarDiscoEffect);
	REGISTER_CAR_EFFECT(VuCarWrestlingVictimEffect);
	REGISTER_CAR_EFFECT(VuCarPlantVictimEffect);
	REGISTER_CAR_EFFECT(VuCarOnFireEffect);
	REGISTER_CAR_EFFECT(VuCarSpringEffect);
	REGISTER_CAR_EFFECT(VuCarBoostStartEffect);
	REGISTER_CAR_EFFECT(VuCarGhostEffect);
}

//*****************************************************************************
VuCarEffectController::~VuCarEffectController()
{
	for ( int i = 0; i < SUB_EFFECT_COUNT; i++ )
		VUASSERT(mSubEffects[i] == 0, "VuCarEffectController sub-effect leak");
}

//*****************************************************************************
void VuCarEffectController::onGameInitialize()
{
}

//*****************************************************************************
void VuCarEffectController::onGameRelease()
{
	stopAllEffects();
}

//*****************************************************************************
void VuCarEffectController::onTickDecision(float fdt)
{
	for ( ActiveEffects::iterator iter = mActiveEffects.begin(); iter != mActiveEffects.end(); )
	{
		iter->second->tick(fdt);
		iter->second->mTimeRemaining -= fdt;

		if ( iter->second->mTimeRemaining <= 0.0f )
		{
			iter->second->stop();
			delete iter->second;
			mActiveEffects.erase(iter++);
		}
		else
		{
			iter++;
		}
	}
}

//*****************************************************************************
void VuCarEffectController::onApplyForces(float fdt)
{
	for ( ActiveEffects::iterator iter = mActiveEffects.begin(); iter != mActiveEffects.end(); iter++ )
		iter->second->onApplyForces(fdt);
}

//*****************************************************************************
void VuCarEffectController::draw(const VuGfxDrawParams &params)
{
	for ( ActiveEffects::iterator iter = mActiveEffects.begin(); iter != mActiveEffects.end(); iter++ )
		iter->second->draw(params);
}

//*****************************************************************************
void VuCarEffectController::applyEffect(const char *effect, VuEntity *pOriginator)
{
	VUASSERT(VuDynamics::IF()->isPotentiallyBusy() == false, "applyEffect() called while dynamics thread is potentially busy!");

	const VuJsonContainer &data = VuGameUtil::IF()->carEffectDB()[effect];
	const std::string &type = data["Type"].asString();

	// blocked?
	if ( isActive(VuCarEffectController::SHIELD) && data["Blockable"].asBool() )
		return;

	// finished?
	if ( mpCar->getHasFinished() )
		return;

	VuCarEntity *pOriginatorCar = VUNULL;
	if ( pOriginator && pOriginator->isDerivedFrom(VuCarEntity::msRTTI) )
		pOriginatorCar = static_cast<VuCarEntity *>(pOriginator);

	VuCarEffect* pEffect = VUNULL;

	// already active?
	ActiveEffects::iterator itEffect = mActiveEffects.find(type);
	if ( itEffect != mActiveEffects.end() )
	{
		// refresh effect
		itEffect->second->mpOriginator = pOriginatorCar;
		if (itEffect->second->mReapplyType != "Ignore")
		{
			itEffect->second->apply(data);
			mpCar->getDriver()->notifyOfEffect(effect, pEffect);
		}

		pEffect = itEffect->second;
	}
	else
	{
		// find creator
		EffectCreators::iterator itCreator = mEffectCreators.find(type);
		if ( itCreator != mEffectCreators.end() )
		{
			// start new effect
			pEffect = itCreator->second();

			pEffect->mpCar = mpCar;
			pEffect->mpOriginator = pOriginatorCar;
			pEffect->start(data, effect);
			pEffect->apply(data);
			mActiveEffects[type] = pEffect;
			mpCar->getDriver()->notifyOfEffect(effect, pEffect);
		}
	}
}

//*****************************************************************************
void VuCarEffectController::stopAllEffects()
{
	for ( ActiveEffects::iterator iter = mActiveEffects.begin(); iter != mActiveEffects.end(); iter++ )
	{
		iter->second->stop();
		delete iter->second;
	}
	mActiveEffects.clear();

	VUASSERT(mWheelSurfaceOverrideCount == 0, "Car effect wheel surface leak");
}

//*****************************************************************************
void VuCarEffectController::pushWheelSurfaceOverride(const char *surfaceType)
{
	VUUINT8 surfaceID = VuDynamics::IF()->getSurfaceTypeID(surfaceType);
	for ( int i = 0; i < 4; i++ )
		mpCar->getWheel(i).mSurfaceTypeOverride = surfaceID;

	mWheelSurfaceOverrideCount++;
}

//*****************************************************************************
void VuCarEffectController::popWheelSurfaceOverride()
{
	VUASSERT(mWheelSurfaceOverrideCount > 0, "Car effect wheel surface leak");

	mWheelSurfaceOverrideCount--;
	if ( mWheelSurfaceOverrideCount == 0 )
	{
		for ( int i = 0; i < 4; i++ )
			mpCar->getWheel(i).mSurfaceTypeOverride = 255;
	}
}

//*****************************************************************************
void VuCarEffectController::pushToughness(const char *smashCarEffect)
{
	pushSubEffect(TOUGHNESS);

	VuRigidBody *pRigidBody = mpCar->getRigidBody();
	pRigidBody->setExtendedFlags(pRigidBody->getExtendedFlags() | EXT_COL_GAME_TOUGH);

	mToughnessSmashCarEffect = smashCarEffect;
}

//*****************************************************************************
void VuCarEffectController::popToughness()
{
	popSubEffect(TOUGHNESS);

	if ( !isActive(TOUGHNESS) )
	{
		VuRigidBody *pRigidBody = mpCar->getRigidBody();
		pRigidBody->setExtendedFlags(pRigidBody->getExtendedFlags() & (~EXT_COL_GAME_TOUGH));
	}
}

//*****************************************************************************
void VuCarEffectController::pushTraction()
{
	pushSubEffect(TRACTION);
}

//*****************************************************************************
void VuCarEffectController::popTraction()
{
	popSubEffect(TRACTION);

	if ( !isActive(TRACTION) )
	{
		mpCar->getSuspension()->setTractionFactor(1.0f);
	}
}
