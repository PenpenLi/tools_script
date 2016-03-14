//*****************************************************************************
//
//  Copyright (c) 2009-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Breakable Prop entity
// 
//*****************************************************************************

#include "VuBreakablePropEntity.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Dynamics/VuCollisionTypes.h"
#include "VuPurple/Util/VuCarUtil.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Components/3dDraw/Vu3dDrawBreakableModelComponent.h"
#include "VuEngine/Components/3dDraw/Vu3dDrawStaticModelComponent.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Components/Transform/VuTransformComponent.h"
#include "VuEngine/Components/RigidBody/VuRigidBodyComponent.h"
#include "VuEngine/Properties/VuAudioProperty.h"
#include "VuEngine/Properties/VuDBEntryProperty.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Math/VuUnitConversion.h"
#include "VuEngine/Pfx/VuPfx.h"
#include "VuEngine/Pfx/VuPfxManager.h"
#include "VuEngine/Pfx/VuPfxEntity.h"
#include "VuEngine/HAL/Audio/VuAudio.h"


IMPLEMENT_RTTI(VuBreakablePropEntity, VuPropEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuBreakablePropEntity);


//*****************************************************************************
VuBreakablePropEntity::VuBreakablePropEntity():
	mThrowDriver(false),
	mStatsType(VuCarStats::NONE),
	mbBreakNow(false),
	mCollisionPos(0,0,0),
	mCollisionVel(0,0,0)
{
	static VuStaticIntEnumProperty::Choice statsTypeChoices[] =
	{
		{ "None", VuCarStats::NONE },
		{ "Seagull", VuCarStats::SEAGULL },
		{ "Palm", VuCarStats::PALM },
		{ "DeathBat", VuCarStats::DEATH_BAT },
		{ "Penguin", VuCarStats::PENGUIN },
		{ "Crab", VuCarStats::CRAB },
		{ "LavaMonster", VuCarStats::LAVA_MONSTER },
		{ "Yeti", VuCarStats::YETI },
		{ VUNULL }
	};

	// components
	addComponent(mp3dDrawBreakableModelComponent = new Vu3dDrawBreakableModelComponent(this));

	// properties
	addProperty(mpTypeProperty = new VuDBEntryProperty("Type", mType, "PropDB")) -> setWatcher(this, &VuBreakablePropEntity::typeModified);
	addProperty(new VuStringProperty("Pfx Name", mPfxName));
	addProperty(new VuAudioEventNameProperty("Sfx Name", mSfxName));
	addProperty(new VuBoolProperty("Throw Driver", mThrowDriver));
	addProperty(new VuStaticIntEnumProperty("Stats Type", mStatsType, statsTypeChoices));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuBreakablePropEntity, Break);
	ADD_SCRIPT_OUTPUT(mpScriptComponent, OnBreak, VuRetVal::Void, VuParamDecl(1, VuParams::Entity));

	mpRigidBodyComponent->setContactCallback(this);
	mpRigidBodyComponent->setExtendedFlags(mpRigidBodyComponent->getExtendedFlags() | EXT_COL_ENGINE_BREAKABLE | EXT_COL_ENGINE_DETECT_EXPLOSIONS);

	// register event handlers
	REG_EVENT_HANDLER(VuBreakablePropEntity, OnExplosion);
	REG_EVENT_HANDLER(VuBreakablePropEntity, OnHitByMissile);
}

//*****************************************************************************
void VuBreakablePropEntity::onPostLoad()
{
	VuPropEntity::onPostLoad();

	typeModified();
}

//*****************************************************************************
void VuBreakablePropEntity::onGameInitialize()
{
	VuPropEntity::onGameInitialize();

	mbBreakNow = false;
	mBreakerEntity = VUNULL;

	// register phased ticks
	VuTickManager::IF()->registerHandler(this, &VuBreakablePropEntity::tickDecision, "Decision");
	VuTickManager::IF()->registerHandler(this, &VuBreakablePropEntity::tickBuild, "Build");

	if ( mSpeedPenalty < FLT_EPSILON )
		mpRigidBodyComponent->setCollisionFlags(mpRigidBodyComponent->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	else
		mpRigidBodyComponent->setCollisionFlags(mpRigidBodyComponent->getCollisionFlags() & (~btCollisionObject::CF_NO_CONTACT_RESPONSE));

	if ( mThrowDriver )
	{
		mpRigidBodyComponent->setExtendedFlags(mpRigidBodyComponent->getExtendedFlags() | EXT_COL_GAME_THROW_DRIVER);
	}
}

//*****************************************************************************
void VuBreakablePropEntity::onGameRelease()
{
	VuPropEntity::onGameRelease();

	// unregister phased tick
	VuTickManager::IF()->unregisterHandlers(this);
}

//*****************************************************************************
void VuBreakablePropEntity::typeModified()
{
	// get data from db
	const VuJsonContainer &data = mpTypeProperty->getEntryData();
	mThresholdNormal = data["Threshold Normal"].asBool();
	mThresholdSpeed = VuMphToMetersPerSecond(data["Threshold Speed"].asFloat());
	mSpeedPenalty = data["Speed Penalty"].asFloat();

	const VuJsonContainer &cameraShakeData = data["Camera Shake"];
	mCamShakeMagnitude = cameraShakeData["Magnitude"].asFloat();
	mCamShakeDuration = cameraShakeData["Duration"].asFloat();
	mCamShakeFalloffTime = cameraShakeData["Falloff Time"].asFloat();
	mCamShakeFrequency = cameraShakeData["Frequency"].asFloat();

	if ( mThresholdSpeed > FLT_EPSILON )
	{
		mpRigidBodyComponent->setExtendedFlags(mpRigidBodyComponent->getExtendedFlags() | EXT_COL_GAME_MASSIVE_BREAKABLE);
		mpRigidBodyComponent->setExtendedFlags(mpRigidBodyComponent->getExtendedFlags() & (~EXT_COL_GAME_MASSLESS_BREAKABLE));
	}
	else
	{
		mpRigidBodyComponent->setExtendedFlags(mpRigidBodyComponent->getExtendedFlags() | EXT_COL_GAME_MASSLESS_BREAKABLE);
		mpRigidBodyComponent->setExtendedFlags(mpRigidBodyComponent->getExtendedFlags() & (~EXT_COL_GAME_MASSIVE_BREAKABLE));
	}
}

//*****************************************************************************
void VuBreakablePropEntity::OnExplosion(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	VuVector3 explosionPos = accessor.getVector3();
	float dist = accessor.getFloat();
	const VuJsonContainer &data = *static_cast<const VuJsonContainer *>(accessor.getPointer());
	VuEntity *pOriginator = accessor.getEntity();

	if ( !mbBreakNow && dist < data["BreakableDist"].asFloat() )
	{
		mbBreakNow = true;

		mCollisionVel = mpTransformComponent->getWorldPosition() - explosionPos;
		mCollisionVel.normalize();
		mCollisionVel *= VuMphToMetersPerSecond(data["BreakableSpeed"].asFloat());

		if ( pOriginator->isDerivedFrom(VuCarEntity::msRTTI) )
		{
			VuCarEntity *pCar = static_cast<VuCarEntity *>(pOriginator);
			pCar->addScore("ShootingGallery", mType.c_str(), mp3dDrawStaticModelComponent->getAabb().getCenter());
		}
	}
}

//*****************************************************************************
void VuBreakablePropEntity::OnHitByMissile(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	VuVector3 missilePos = accessor.getVector3();
	VuVector3 missileVel = accessor.getVector3();
	const char *carEffect = accessor.getString();
	VuEntity *pOriginator = accessor.getEntity();

	if ( !mbBreakNow )
	{
		mbBreakNow = true;
		mCollisionVel = missileVel;

		if ( pOriginator->isDerivedFrom(VuCarEntity::msRTTI) )
		{
			VuCarEntity *pCar = static_cast<VuCarEntity *>(pOriginator);
			pCar->addScore("ShootingGallery", mType.c_str(), missilePos);
		}
	}
}

//*****************************************************************************
bool VuBreakablePropEntity::onRigidBodyContactAdded(VuContactPoint &cp)
{
	// check if we should break
	if ( cp.mpOtherBody->hasContactResponse() && !mbBreakNow )
	{
		VuRigidBody *pRigidBody = mpRigidBodyComponent->getRigidBody();

		VUUINT32 otherExtendedFlags = cp.mpOtherBody->getExtendedFlags();

		// compute relative velocity
		VuVector3 vRelV = pRigidBody->getVuLinearVelocity() - cp.mpOtherBody->getVuLinearVelocity();
		float fRelVel = mThresholdNormal ? VuAbs(VuDot(cp.mNorWorld, vRelV)) : vRelV.mag();
		if ( (fRelVel > mThresholdSpeed) || (otherExtendedFlags & EXT_COL_GAME_TOUGH) )
		{
			VuEntity *pOtherEntity = cp.mpOtherBody->getEntity();
			VuCarEntity *pCar = (pOtherEntity && pOtherEntity->isDerivedFrom(VuCarEntity::msRTTI)) ? static_cast<VuCarEntity *>(pOtherEntity) : VUNULL;

			mpRigidBodyComponent->setCollisionFlags(mpRigidBodyComponent->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
			mCollisionPos = cp.mPosWorld;
			mCollisionVel = cp.mpOtherBody->getVuLinearVelocity();
			mBreakerEntity = pOtherEntity;
			mbBreakNow = true;

			if ( pCar )
			{
				float penalty = mSpeedPenalty*pCar->getBreakablePenalty();
				VuVector3 linVel = pCar->getRigidBody()->getVuLinearVelocity();
				linVel *= (1.0f - penalty);
				pCar->getRigidBody()->setVuLinearVelocity(linVel);
			}
		}
		else if ( otherExtendedFlags & EXT_COL_ENGINE_EXPLODABLE )
		{
			mpRigidBodyComponent->setCollisionFlags(mpRigidBodyComponent->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
			mCollisionPos = cp.mPosWorld;
			mCollisionVel = cp.mpOtherBody->getVuLinearVelocity();
			mBreakerEntity = cp.mpOtherBody->getEntity();
			mbBreakNow = true;
		}
	}

	return !mbBreakNow;
}

//*****************************************************************************
VuRetVal VuBreakablePropEntity::Break(const VuParams &params)
{
	mBreakerEntity = VUNULL;
	mbBreakNow = true;

	return VuRetVal();
}

//*****************************************************************************
void VuBreakablePropEntity::tickDecision(float fdt)
{
	mp3dDrawBreakableModelComponent->tickDecision(fdt);

	if ( mbBreakNow )
	{
		if ( mp3dDrawBreakableModelComponent->isWhole() )
		{
			hide();
			mp3dDrawBreakableModelComponent->startBreak(mpTransformComponent->getWorldTransform(), mCollisionVel, mp3dDrawStaticModelComponent->getColor());

			// script notification
			{
				VuParams params;
                if (mBreakerEntity != VUNULL)
                {
                    params.addEntity(mBreakerEntity.get());
                }
				mpScriptComponent->getPlug("OnBreak")->execute(params);
			}

			if ( mBreakerEntity )
			{
				// camera shake
				{
					VuParams params;
					params.addFloat(mCamShakeMagnitude);
					params.addFloat(mCamShakeDuration);
					params.addFloat(mCamShakeFalloffTime);
					params.addFloat(mCamShakeFrequency);
					mBreakerEntity->handleEvent("OnCameraShake", params);
				}

				// stats
				if ( mBreakerEntity->isDerivedFrom(VuCarEntity::msRTTI) )
				{
					VuCarEntity *pCar = static_cast<VuCarEntity *>(mBreakerEntity.get());
					pCar->addScore("DemolitionDerby", mType.c_str(), mCollisionPos);

					VuCarStats &stats = pCar->getStats();
					stats.mSmashedStuff[mStatsType]++;
				}
			}

			// pfx
			if ( VUUINT32 hPfx = VuPfxManager::IF()->createEntity(mPfxName.c_str(), true) )
			{
				if ( VuPfxEntity *pPfxEntity = VuPfxManager::IF()->getEntity(hPfx) )
				{
					pPfxEntity->getSystemInstance()->setMatrix(mpTransformComponent->getWorldTransform());
					pPfxEntity->getSystemInstance()->setLinearVelocity(mCollisionVel);
					pPfxEntity->getSystemInstance()->start();
				}
			}

			// sfx
			#if !VU_DISABLE_AUDIO
			if ( mSfxName.length() )
			{
				FMOD::Event *pEvent;
				if ( VuAudio::IF()->eventSystem()->getEvent(mSfxName.c_str(), FMOD_EVENT_NONBLOCKING, &pEvent) == FMOD_OK )
				{
					FMOD_VECTOR pos = VuAudio::toFmodVector(mpTransformComponent->getWorldPosition());
					pEvent->set3DAttributes(&pos, VUNULL, VUNULL);
					pEvent->start();
				}
			}
			#endif
		}

		mbBreakNow = false;
		mBreakerEntity = VUNULL;
	}
}

//*****************************************************************************
void VuBreakablePropEntity::tickBuild(float fdt)
{
	mp3dDrawBreakableModelComponent->tickBuild(fdt);
}