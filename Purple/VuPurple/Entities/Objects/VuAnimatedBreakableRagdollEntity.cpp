//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Animated breakable ragdoll entity
// 
//*****************************************************************************

#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Dynamics/VuCollisionTypes.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Entities/Objects/VuAnimatedPropEntity.h"
#include "VuEngine/Components/Transform/VuTransformComponent.h"
#include "VuEngine/Components/RigidBody/VuRigidBodyComponent.h"
#include "VuEngine/Components/3dDraw/Vu3dDrawAnimatedModelComponent.h"
#include "VuEngine/Components/3dDraw/Vu3dDrawRagdollComponent.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Components/Attach/VuAttachComponent.h"
#include "VuEngine/Properties/VuAudioProperty.h"
#include "VuEngine/Properties/VuDBEntryProperty.h"
#include "VuEngine/Properties/VuRotation3dProperty.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Animation/VuAnimatedSkeleton.h"
#include "VuEngine/Math/VuUnitConversion.h"
#include "VuEngine/Math/VuRand.h"
#include "VuEngine/Math/VuMathUtil.h"
#include "VuEngine/Pfx/VuPfx.h"
#include "VuEngine/Pfx/VuPfxManager.h"
#include "VuEngine/Pfx/VuPfxEntity.h"
#include "VuEngine/HAL/Audio/VuAudio.h"


class VuAnimatedBreakableRagdollEntity : public VuAnimatedPropEntity, public VuRigidBodyContactCallback
{
	DECLARE_RTTI

public:
	VuAnimatedBreakableRagdollEntity();
	~VuAnimatedBreakableRagdollEntity();

	virtual void			onPostLoad();

	virtual void			onGameInitialize();
	virtual void			onGameRelease();

protected:
	// event handlers
	void					OnHitByMissile(const VuParams &params);
	void					OnExplosion(const VuParams &params);

	void					typeModified();

	virtual void			show();
	virtual void			hide();

	virtual void			tickDecision(float fdt);
	virtual void			tickAnim(float fdt);

	// VuRigidBodyContactCallback
	virtual bool			onRigidBodyContactAdded(VuContactPoint &cp);

	// scripting
	VuRetVal				Break(const VuParams &params);

	// components
	Vu3dDrawRagdollComponent	*mp3dDrawRagdollComponent;

	// properties
	std::string				mType;
	std::string				mPfxName;
	std::string				mSfxName;
	float					mFadeDelay;
	float					mFadeTime;
	VuVector3				mMinLinVel;
	VuVector3				mMaxLinVel;
	VuVector3				mMinAngVel;
	VuVector3				mMaxAngVel;
	bool					mThrowDriver;
	int						mStatsType;

	VuDBEntryProperty		*mpTypeProperty;
	float					mThresholdSpeed;
	float					mCamShakeMagnitude;
	float					mCamShakeDuration;
	float					mCamShakeFalloffTime;
	float					mCamShakeFrequency;

	bool					mbBreakNow;
	VuVector3				mCollisionPos;
	VuVector3				mCollisionVel;
	VuWeakRef<VuEntity>		mBreakerEntity;
	float					mTimer;
};

IMPLEMENT_RTTI(VuAnimatedBreakableRagdollEntity, VuAnimatedPropEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuAnimatedBreakableRagdollEntity);


//*****************************************************************************
VuAnimatedBreakableRagdollEntity::VuAnimatedBreakableRagdollEntity():
	mFadeDelay(2.0f),
	mFadeTime(1.0f),
	mMinLinVel(-5.0f,  5.0f,  0.0f),
	mMaxLinVel( 5.0f,  5.0f,  0.0f),
	mMinAngVel(-0.25f*VU_PI, -0.25f*VU_PI, 0.25f*VU_PI),
	mMaxAngVel( 0.25f*VU_PI,  0.25f*VU_PI, 0.25f*VU_PI),
	mThrowDriver(false),
	mStatsType(VuCarStats::NONE),
	mbBreakNow(false),
	mCollisionPos(0,0,0),
	mCollisionVel(0,0,0),
	mTimer(0.0f)
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
	addComponent(mp3dDrawRagdollComponent = new Vu3dDrawRagdollComponent(this));

	// properties
	addProperty(mpTypeProperty = new VuDBEntryProperty("Type", mType, "PropDB")) -> setWatcher(this, &VuAnimatedBreakableRagdollEntity::typeModified);
	addProperty(new VuStringProperty("Pfx Name", mPfxName));
	addProperty(new VuAudioEventNameProperty("Sfx Name", mSfxName));
	addProperty(new VuFloatProperty("Fade Delay", mFadeDelay));
	addProperty(new VuFloatProperty("Fade Time", mFadeTime));
	addProperty(new VuVector3Property("Min Lin Vel", mMinLinVel));
	addProperty(new VuVector3Property("Max Lin Vel", mMaxLinVel));
	addProperty(new VuRotation3dProperty("Min Ang Vel", mMinAngVel));
	addProperty(new VuRotation3dProperty("Max Ang Vel", mMaxAngVel));
	addProperty(new VuBoolProperty("Throw Driver", mThrowDriver));
	addProperty(new VuStaticIntEnumProperty("Stats Type", mStatsType, statsTypeChoices));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuAnimatedBreakableRagdollEntity, Break);
	ADD_SCRIPT_OUTPUT(mpScriptComponent, OnBreak, VuRetVal::Void, VuParamDecl(1, VuParams::Entity));

	mpRigidBodyComponent->setContactCallback(this);
	mpRigidBodyComponent->setExtendedFlags(mpRigidBodyComponent->getExtendedFlags() | EXT_COL_ENGINE_BREAKABLE | EXT_COL_ENGINE_DETECT_EXPLOSIONS);

	// register event handlers
	REG_EVENT_HANDLER(VuAnimatedBreakableRagdollEntity, OnHitByMissile);
	REG_EVENT_HANDLER(VuAnimatedBreakableRagdollEntity, OnExplosion);
}

//*****************************************************************************
VuAnimatedBreakableRagdollEntity::~VuAnimatedBreakableRagdollEntity()
{
}

//*****************************************************************************
void VuAnimatedBreakableRagdollEntity::onPostLoad()
{
	VuAnimatedPropEntity::onPostLoad();

	typeModified();
}

//*****************************************************************************
void VuAnimatedBreakableRagdollEntity::onGameInitialize()
{
	VuAnimatedPropEntity::onGameInitialize();

	mp3dDrawRagdollComponent->setModelInstance(&mp3dDrawAnimatedModelComponent->getModelInstance());

	// register phased ticks
	VuTickManager::IF()->registerHandler(this, &VuAnimatedBreakableRagdollEntity::tickDecision, "Decision");

	mpRigidBodyComponent->setCollisionFlags(mpRigidBodyComponent->getCollisionFlags() & (~btCollisionObject::CF_NO_CONTACT_RESPONSE));

	if ( mThrowDriver )
	{
		mpRigidBodyComponent->setExtendedFlags(mpRigidBodyComponent->getExtendedFlags() | EXT_COL_GAME_THROW_DRIVER);
	}

	mbBreakNow = false;
	mTimer = 0.0f;
}

//*****************************************************************************
void VuAnimatedBreakableRagdollEntity::onGameRelease()
{
	VuAnimatedPropEntity::onGameRelease();

	// unregister phased tick
	VuTickManager::IF()->unregisterHandlers(this);
}

//*****************************************************************************
void VuAnimatedBreakableRagdollEntity::OnHitByMissile(const VuParams &params)
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
void VuAnimatedBreakableRagdollEntity::OnExplosion(const VuParams &params)
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
			pCar->addScore("ShootingGallery", mType.c_str(), mp3dDrawAnimatedModelComponent->getAabb().getCenter());
		}
	}
}

//*****************************************************************************
void VuAnimatedBreakableRagdollEntity::typeModified()
{
	// get data from db
	const VuJsonContainer &data = mpTypeProperty->getEntryData();
	mThresholdSpeed = VuMphToMetersPerSecond(data["Threshold Speed"].asFloat());

	const VuJsonContainer &cameraShakeData = data["Camera Shake"];
	mCamShakeMagnitude = cameraShakeData["Magnitude"].asFloat();
	mCamShakeDuration = cameraShakeData["Duration"].asFloat();
	mCamShakeFalloffTime = cameraShakeData["Falloff Time"].asFloat();
	mCamShakeFrequency = cameraShakeData["Frequency"].asFloat();
}

//*****************************************************************************
void VuAnimatedBreakableRagdollEntity::show()
{
	VuAnimatedPropEntity::show();
}

//*****************************************************************************
void VuAnimatedBreakableRagdollEntity::hide()
{
	VuAnimatedPropEntity::hide();

	mp3dDrawRagdollComponent->stopSimulation();
	mp3dDrawRagdollComponent->hide();
}

//*****************************************************************************
void VuAnimatedBreakableRagdollEntity::tickDecision(float fdt)
{
	if ( mbBreakNow )
	{
		hide();

		VuVector3 linVel = mCollisionVel;
		VuVector3 angVel(0,0,0);
		if ( mBreakerEntity )
		{
			VuMatrix mat;
			VuMathUtil::buildOrientationMatrix(mCollisionVel, VuVector3(0,0,1), mat);

			VuVector3 randLinVel;
			randLinVel.mX = VuLerp(mMinLinVel.mX, mMaxLinVel.mX, VuRand::global().rand());
			randLinVel.mY = VuLerp(mMinLinVel.mY, mMaxLinVel.mY, VuRand::global().rand());
			randLinVel.mZ = VuLerp(mMinLinVel.mZ, mMaxLinVel.mZ, VuRand::global().rand());
			linVel += mat.transformNormal(randLinVel);

			angVel.mX = VuLerp(mMinAngVel.mX, mMaxAngVel.mX, VuRand::global().rand());
			angVel.mY = VuLerp(mMinAngVel.mY, mMaxAngVel.mY, VuRand::global().rand());
			angVel.mZ = VuLerp(mMinAngVel.mZ, mMaxAngVel.mZ, VuRand::global().rand());
			angVel = mpTransformComponent->getWorldTransform().transformNormal(angVel);
		}

		mp3dDrawRagdollComponent->startSimulation(mp3dDrawAnimatedModelComponent->getAnimatedSkeleton()->getLocalPose(), linVel, angVel);
		mp3dDrawRagdollComponent->show();

		mTimer = mFadeTime + mFadeDelay;

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

			// script notification
			{
				VuParams params;
				params.addEntity(mBreakerEntity.get());
				mpScriptComponent->getPlug("OnBreak")->execute(params);
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

		mbBreakNow = false;
		mBreakerEntity = VUNULL;
	}

	if ( mTimer > 0.0f )
	{
		float fAlpha = VuMin(mTimer/mFadeTime, 1.0f);

		VuColor color = mp3dDrawAnimatedModelComponent->getModelInstance().getColor();
		color.mA = (VUUINT8)VuRound(fAlpha*255.0f);
		mp3dDrawAnimatedModelComponent->getModelInstance().setColor(color);

		mTimer -= fdt;
		if ( mTimer < 0.0f )
			hide();
	}
}

//*****************************************************************************
void VuAnimatedBreakableRagdollEntity::tickAnim(float fdt)
{
	if ( mp3dDrawRagdollComponent->isShown() )
	{
		mpAttachComponent->update(mpTransformComponent->getWorldTransform());
	}
	else
	{
		VuAnimatedPropEntity::tickAnim(fdt);
	}
}

//*****************************************************************************
bool VuAnimatedBreakableRagdollEntity::onRigidBodyContactAdded(VuContactPoint &cp)
{
	// check if we should break
	if ( cp.mpOtherBody->hasContactResponse() && !mbBreakNow )
	{
		VuRigidBody *pRigidBody = mpRigidBodyComponent->getRigidBody();

		VUUINT32 otherExtendedFlags = cp.mpOtherBody->getExtendedFlags();

		// compute relative velocity
		VuVector3 vRelV = pRigidBody->getVuLinearVelocity() - cp.mpOtherBody->getVuLinearVelocity();
		float fRelVel = VuAbs(VuDot(cp.mNorWorld, vRelV));
		if ( (fRelVel > mThresholdSpeed) || (otherExtendedFlags & EXT_COL_GAME_TOUGH))
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
VuRetVal VuAnimatedBreakableRagdollEntity::Break(const VuParams &params)
{
	mBreakerEntity = VUNULL;
	mbBreakNow = true;

	return VuRetVal();
}
