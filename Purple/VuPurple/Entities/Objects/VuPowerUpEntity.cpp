//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  PowerUp entity
// 
//*****************************************************************************

#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarPowerUpController.h"
#include "VuPurple/Entities/Car/Parts/VuCarEffectController.h"
#include "VuPurple/Dynamics/VuCollisionTypes.h"
#include "VuPurple/PowerUps/VuPowerUpManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Entities/VuEntityRepository.h"
#include "VuEngine/Components/3dDraw/Vu3dDrawComponent.h"
#include "VuEngine/Components/3dLayout/Vu3dLayoutComponent.h"
#include "VuEngine/Components/Motion/VuMotionComponent.h"
#include "VuEngine/Components/RigidBody/VuRigidBodyComponent.h"
#include "VuEngine/Components/Transform/VuTransformComponent.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Properties/VuAudioProperty.h"
#include "VuEngine/Properties/VuBitFieldProperty.h"
#include "VuEngine/Properties/VuDBEntryProperty.h"
#include "VuEngine/HAL/Audio/VuAudio.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Pfx/VuPfx.h"
#include "VuEngine/Pfx/VuPfxManager.h"
#include "VuEngine/Pfx/VuPfxEntity.h"
#include "VuEngine/Pfx/VuPfx.h"
#include "VuEngine/Services/VuServiceManager.h"
#include "VuEngine/Services/VuPfxAttachService.h"
#include "VuEngine/Math/VuMathUtil.h"
#include "VuEngine/Math/VuRand.h"
#include "VuEngine/Util/VuAudioUtil.h"
#include "VuEngine/Dev/VuDevConfig.h"


class VuPowerUpEntity : public VuEntity, public VuMotionComponentIF, public VuRigidBodyContactCallback
{
	DECLARE_RTTI

public:
	VuPowerUpEntity();
	~VuPowerUpEntity();

	void				onGameInitialize();
	void				onGameRelease();

protected:
	// VuMotionComponentIF interface
	virtual void		onMotionUpdate();

	// VuRigidBodyContactCallback
	virtual bool		onRigidBodyContactAdded(VuContactPoint &cp);

	bool				enable();
	void				disable();

	void				draw(const VuGfxDrawParams &params);
	void				drawLayout(const Vu3dLayoutDrawParams &params);

	void				tickDecision(float fdt);
	void				tickBuild(float fdt);

	void				determineType();

	// components
	Vu3dDrawComponent		*mp3dDrawComponent;
	Vu3dLayoutComponent		*mp3dLayoutComponent;
	VuMotionComponent		*mpMotionComponent;
	VuRigidBodyComponent	*mpRigidBodyComponent;
	VuScriptComponent		*mpScriptComponent;

	// properties
	std::string				mStaticPfxName;
	std::string				mPopPfxName;
	std::string				mPopSfxName;
	std::string				mCollectPfxName;
	std::string				mCollectSfxName;
	float					mRespawnDelay;
	float					mDrawDist;
	float					mAngularFrequency;
	float					mDampingRatio;
	VUUINT32				mPowerUpMask;
	std::string				mForceCarEffect;

	bool					mEnabled;
	VuPfxSystemInstance		*mpStaticPfxSystem;
	VuWeakRef<VuCarEntity>	mCollector;
	float					mRespawnTimer;
	float					mScale;
	float					mScaleVelocity;
	bool					mSpringActive;
};

IMPLEMENT_RTTI(VuPowerUpEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuPowerUpEntity);


//*****************************************************************************
VuPowerUpEntity::VuPowerUpEntity():
	mRespawnDelay(1.0f),
	mDrawDist(FLT_MAX),
	mAngularFrequency(2.0f),
	mDampingRatio(0.25f),
	mPowerUpMask(~0),
	mEnabled(false),
	mpStaticPfxSystem(VUNULL),
	mRespawnTimer(0.0f),
	mScale(0.0f),
	mScaleVelocity(0.0f),
	mSpringActive(false)
{
	// components
	addComponent(mp3dDrawComponent = new Vu3dDrawComponent(this, false));
	addComponent(mp3dLayoutComponent = new Vu3dLayoutComponent(this));
	addComponent(mpMotionComponent = new VuMotionComponent(this, this));
	addComponent(mpRigidBodyComponent = new VuRigidBodyComponent(this));
	addComponent(mpScriptComponent = new VuScriptComponent(this, 100, false));

	mp3dDrawComponent->setDrawMethod(this, &VuPowerUpEntity::draw);
	mp3dLayoutComponent->setDrawMethod(this, &VuPowerUpEntity::drawLayout);

	// properties
	addProperty(new VuFloatProperty("Respawn Delay", mRespawnDelay));
	addProperty(new VuStringProperty("Static Pfx", mStaticPfxName));
	addProperty(new VuStringProperty("Pop Pfx", mPopPfxName));
	addProperty(new VuAudioEventNameProperty("Pop Sfx", mPopSfxName));
	addProperty(new VuStringProperty("Collect Pfx", mCollectPfxName));
	addProperty(new VuAudioEventNameProperty("Collect Sfx", mCollectSfxName));
	addProperty(new VuFloatProperty("Draw Distance", mDrawDist));
	addProperty(new VuFloatProperty("Angular Frequency", mAngularFrequency));
	addProperty(new VuFloatProperty("Damping Ratio", mDampingRatio));
	addProperty(new VuDBEntryProperty("Force Car Effect", mForceCarEffect, "CarEffectDB"));

	for ( int i = 0; i < VuPowerUpManager::IF()->getGroupCount(); i++ )
		addProperty(new VuBitFieldProperty(VuPowerUpManager::IF()->getGroup(i)->mName.c_str(), mPowerUpMask, 1<<i));
}

//*****************************************************************************
VuPowerUpEntity::~VuPowerUpEntity()
{
}

//*****************************************************************************
void VuPowerUpEntity::onGameInitialize()
{
	mpRigidBodyComponent->setCollisionGroup(COL_GAME_POWERUP);
	mpRigidBodyComponent->setCollisionMask(COL_GAME_CAR);
	mpRigidBodyComponent->createRigidBody();
	mpRigidBodyComponent->setCollisionFlags(mpRigidBodyComponent->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	mpRigidBodyComponent->setContactCallback(this);

	VuTickManager::IF()->registerHandler(this, &VuPowerUpEntity::tickDecision, "Decision");
	VuTickManager::IF()->registerHandler(this, &VuPowerUpEntity::tickBuild, "Build");

	enable();
}

//*****************************************************************************
void VuPowerUpEntity::onGameRelease()
{
	disable();

	mpRigidBodyComponent->destroyRigidBody();

	// unregister phased ticks
	VuTickManager::IF()->unregisterHandlers(this);
}

//*****************************************************************************
void VuPowerUpEntity::onMotionUpdate()
{
	mpTransformComponent->setWorldTransform(mpMotionComponent->getWorldTransform(), false);
	if ( mEnabled )
	{
		mpRigidBodyComponent->onMotionUpdate(mpMotionComponent);
		mpStaticPfxSystem->setMatrix(mpMotionComponent->getWorldTransform());
	}
}

//*****************************************************************************
bool VuPowerUpEntity::enable()
{
	if ( !mEnabled && VuPowerUpManager::IF()->getPowerUpsEnabled() )
	{
		if ( (mpStaticPfxSystem = VuPfx::IF()->createSystemInstance(mStaticPfxName.c_str())) )
		{
			mEnabled = true;

			mScale = 0.0f;
			mScaleVelocity = 0.0f;
			mSpringActive = true;

			mpStaticPfxSystem->setMatrix(mpTransformComponent->getWorldTransform());
			mpStaticPfxSystem->setScale(mScale);
			mpStaticPfxSystem->start();

			mpRigidBodyComponent->addToWorld();

			mp3dDrawComponent->show();
		}
	}

	return mEnabled;
}

//*****************************************************************************
void VuPowerUpEntity::disable()
{
	if ( mEnabled )
	{
		mEnabled = false;

		mpStaticPfxSystem->stop(true);
		VuPfx::IF()->releaseSystemInstance(mpStaticPfxSystem);
		mpStaticPfxSystem = VUNULL;

		mpRigidBodyComponent->removeFromWorld();

		mp3dDrawComponent->hide();
	}
}

//*****************************************************************************
bool VuPowerUpEntity::onRigidBodyContactAdded(VuContactPoint &cp)
{
	VuCarEntity *pCar = static_cast<VuCarEntity *>(cp.mpOtherBody->getEntity());

	if ( !pCar->isSpawned() )
		mCollector = pCar;

	return false;
}

//*****************************************************************************
void VuPowerUpEntity::draw(const VuGfxDrawParams &params)
{
	float distSquared = (mpTransformComponent->getWorldPosition() - params.mEyePos).magSquared();

	if ( distSquared < mDrawDist*mDrawDist )
	{
		if ( mpStaticPfxSystem )
			mpStaticPfxSystem->draw(params);
	}
}

//*****************************************************************************
void VuPowerUpEntity::drawLayout(const Vu3dLayoutDrawParams &params)
{
	mpRigidBodyComponent->draw(VuColor(255,255,0), params.mCamera);
}

//*****************************************************************************
void VuPowerUpEntity::tickDecision(float fdt)
{
	if ( mEnabled )
	{
		if ( VuCarEntity *pCar = mCollector.get() )
		{
			mCollector = VUNULL;

			// pfx
			if ( VUUINT32 hPfx = VuPfxManager::IF()->createEntity(mPopPfxName.c_str(), true) )
			{
				if ( VuPfxEntity *pPfxEntity = VuPfxManager::IF()->getEntity(hPfx) )
				{
					pPfxEntity->getSystemInstance()->setMatrix(mpTransformComponent->getWorldTransform());
					pPfxEntity->getSystemInstance()->start();
				}
			}

			// sfx
			if ( pCar->getIsRacer() )
				VuAudioUtil::playSfx(mPopSfxName.c_str(), pCar->getModelPosition());

			if ( mForceCarEffect.length() )
			{
				pCar->getEffectController()->applyEffect(mForceCarEffect.c_str(), VUNULL);
			}
			else
			{
				if ( pCar->getPowerUpController()->collectPowerUp(mPowerUpMask) )
				{
					if ( VUUINT32 hPfx = VuPfxManager::IF()->createEntity(mCollectPfxName.c_str(), true) )
					{
						if ( VuPfxEntity *pPfxEntity = VuPfxManager::IF()->getEntity(hPfx) )
						{
							if ( VuPfxAttachService *pService = VuServiceManager::IF()->createPfxService<VuPfxAttachService>() )
								pService->init(pCar, hPfx, VuMatrix::identity());

							pPfxEntity->getSystemInstance()->start();
						}
					}

					if ( pCar->getIsRacer() )
						VuAudioUtil::playSfx(mCollectSfxName.c_str(), pCar->getModelPosition());
				}
			}

			// start respawn
			disable();
			mRespawnTimer = mRespawnDelay;
		}

		// update scale
		if ( mSpringActive )
		{
			VuMathUtil::calcDampedSimpleHarmonicMotion(&mScale, &mScaleVelocity, 1.0f, fdt, mAngularFrequency*VU_2PI, mDampingRatio);
			if ( VuAbs(mScale - 1.0f) < 0.01f && VuAbs(mScaleVelocity) < 0.01f )
			{
				mSpringActive = false;
				mScale = 1.0f;
				mScaleVelocity = 0.0f;
			}
		}
	}
	else
	{
		mRespawnTimer -= fdt;
		if ( mRespawnTimer <= 0.0f )
			enable();
	}
}

//*****************************************************************************
void VuPowerUpEntity::tickBuild(float fdt)
{
	if ( mEnabled )
	{
		// update visibility
		if ( mpStaticPfxSystem )
		{
			mpStaticPfxSystem->setScale(mScale);
			mpStaticPfxSystem->tick(fdt, false);
			if ( mpStaticPfxSystem->getAabb().isValid() )
				mp3dDrawComponent->updateVisibility(mpStaticPfxSystem->getAabb());
		}
	}
}
