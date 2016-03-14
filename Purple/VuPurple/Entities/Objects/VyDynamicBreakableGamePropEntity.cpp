//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Game-implementation of dynamic breakable prop entity
// 
//*****************************************************************************

#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Dynamics/VuCollisionTypes.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/Transform/VuTransformComponent.h"
#include "VuEngine/Components/RigidBody/VuRigidBodyComponent.h"
#include "VuEngine/Components/3dDraw/Vu3dDrawStaticModelComponent.h"
#include "VuEngine/Components/3dDraw/Vu3dDrawBreakableModelComponent.h"
#include "VuEngine/Components/3dLayout/Vu3dLayoutComponent.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Properties/VuBasicProperty.h"
#include "VuEngine/Properties/VuAudioProperty.h"
#include "VuEngine/Properties/VuDBEntryProperty.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Math/VuUnitConversion.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/HAL/Audio/VuAudio.h"
#include "VuEngine/Pfx/VuPfx.h"
#include "VuEngine/Pfx/VuPfxManager.h"
#include "VuEngine/Pfx/VuPfxEntity.h"


// constants

#define REPEAT_TIME 1.0f // seconds
#define SHADOW_TEST_DIST 10.0f // m
#define TOUGH_CAR_COLLISION_ADVANTAGE 2.0f


class VuDynamicBreakableGamePropEntity : public VuEntity, btMotionState, VuRigidBodyContactCallback
{
	DECLARE_RTTI
public:
	VuDynamicBreakableGamePropEntity();

	virtual void	onPostLoad();
	virtual void	onGameInitialize();
	virtual void	onGameRelease();

protected:
	// btMotionState
	virtual void	getWorldTransform(btTransform& worldTrans) const;
	virtual void	setWorldTransform(const btTransform& worldTrans);

	// scripting
	VuRetVal		Show(const VuParams &params)	{ show(); return VuRetVal(); }
	VuRetVal		Hide(const VuParams &params)	{ hide(); return VuRetVal(); }

	void			show();
	void			hide();

	void			drawLayout(const Vu3dLayoutDrawParams &params);
	bool			collideLayout(const VuVector3 &v0, VuVector3 &v1);
	void			transformModified();
	void			massModified();
	void			typeModified();

	// event handlers
	void			OnDeactivate(const VuParams &params) { hide(); }
	void			OnExplosion(const VuParams &params);
	void			OnHitByMissile(const VuParams &params);

	// VuRigidBodyContactCallback
	virtual bool	onRigidBodyContactAdded(VuContactPoint &cp);

	void			tickDecision(float fdt);
	void			tickBuild(float fdt);

	// components
	Vu3dLayoutComponent				*mp3dLayoutComponent;
	VuScriptComponent				*mpScriptComponent;
	Vu3dDrawStaticModelComponent	*mp3dDrawStaticModelComponent;
	VuRigidBodyComponent			*mpRigidBodyComponent;
	Vu3dDrawBreakableModelComponent	*mp3dDrawBreakableModelComponent;

	// properties
	bool			mbInitiallyVisible;
	float			mMass;
	VuVector3		mCenterOfMass;
	std::string		mType;
	std::string		mPfxName;
	std::string		mSfxName;
	bool			mIsTough;
	bool			mCollisionLighting;

	VuDBEntryProperty	*mpTypeProperty;
	bool				mThresholdNormal;
	float				mThresholdSpeed;
	float				mSpeedPenalty;
	float				mCamShakeMagnitude;
	float				mCamShakeDuration;
	float				mCamShakeFalloffTime;
	float				mCamShakeFrequency;

	bool				mbVisible;
	bool				mbBreakNow;
	bool				mbDeactivateNow;
	VuVector3			mCollisionPos;
	VuVector3			mCollisionVel;
	VuWeakRef<VuEntity>	mBreakerEntity;
};


IMPLEMENT_RTTI(VuDynamicBreakableGamePropEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuDynamicBreakableGamePropEntity);


//*****************************************************************************
VuDynamicBreakableGamePropEntity::VuDynamicBreakableGamePropEntity():
	mbInitiallyVisible(true),
	mMass(100.0f),
	mCenterOfMass(0,0,0),
	mbVisible(false),
	mIsTough(false),
	mCollisionLighting(false),
	mbBreakNow(false),
	mbDeactivateNow(false),
	mCollisionPos(0,0,0),
	mCollisionVel(0,0,0)
{
	// components
	addComponent(mp3dLayoutComponent = new Vu3dLayoutComponent(this));
	addComponent(mpScriptComponent = new VuScriptComponent(this, 150, false));
	addComponent(mp3dDrawStaticModelComponent = new Vu3dDrawStaticModelComponent(this));
	addComponent(mp3dDrawBreakableModelComponent = new Vu3dDrawBreakableModelComponent(this));
	addComponent(mpRigidBodyComponent = new VuRigidBodyComponent(this));

	mp3dLayoutComponent->setDrawMethod(this, &VuDynamicBreakableGamePropEntity::drawLayout);
	mp3dLayoutComponent->setCollideMethod(this, &VuDynamicBreakableGamePropEntity::collideLayout);

	// want to know when transform is changed
	mpTransformComponent->setWatcher(&VuDynamicBreakableGamePropEntity::transformModified);

	// properties
	addProperty(new VuBoolProperty("Initially Visible", mbInitiallyVisible));
	addProperty(new VuFloatProperty("Mass", mMass)) -> setWatcher(this, &VuDynamicBreakableGamePropEntity::massModified);
	addProperty(new VuVector3Property("Center of Mass", mCenterOfMass)) -> setWatcher(this, &VuDynamicBreakableGamePropEntity::massModified);
	addProperty(mpTypeProperty = new VuDBEntryProperty("Type", mType, "PropDB")) -> setWatcher(this, &VuDynamicBreakableGamePropEntity::typeModified);
	addProperty(new VuStringProperty("Pfx Name", mPfxName));
	addProperty(new VuAudioEventNameProperty("Sfx Name", mSfxName));
	addProperty(new VuBoolProperty("Is Tough", mIsTough));
	addProperty(new VuBoolProperty("Collision Lighting", mCollisionLighting));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuDynamicBreakableGamePropEntity, Show);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuDynamicBreakableGamePropEntity, Hide);
	ADD_SCRIPT_OUTPUT(mpScriptComponent, OnBreak, VuRetVal::Void, VuParamDecl(1, VuParams::Entity));

	// event handlers
	REG_EVENT_HANDLER(VuDynamicBreakableGamePropEntity, OnDeactivate);
	REG_EVENT_HANDLER(VuDynamicBreakableGamePropEntity, OnExplosion);
	REG_EVENT_HANDLER(VuDynamicBreakableGamePropEntity, OnHitByMissile);
}

//*****************************************************************************
void VuDynamicBreakableGamePropEntity::onPostLoad()
{
	transformModified();
	massModified();
	typeModified();
}

//*****************************************************************************
void VuDynamicBreakableGamePropEntity::onGameInitialize()
{
	mpRigidBodyComponent->setMass(mMass);
	mpRigidBodyComponent->setMotionState(this);
	mpRigidBodyComponent->setCollisionGroup(COL_ENGINE_DYNAMIC_PROP);
	mpRigidBodyComponent->setCollisionMask(COL_EVERYTHING);
	mpRigidBodyComponent->setContactCallback(this);
	mpRigidBodyComponent->setExtendedFlags(mpRigidBodyComponent->getExtendedFlags() | EXT_COL_ENGINE_BREAKABLE | EXT_COL_ENGINE_DETECT_EXPLOSIONS);

	if ( mIsTough )
		mpRigidBodyComponent->setExtendedFlags(mpRigidBodyComponent->getExtendedFlags() | EXT_COL_GAME_TOUGH);

	mpRigidBodyComponent->createRigidBody();
	VUASSERT(mpRigidBodyComponent->getRigidBody(), "Dynamic Prop without collision!");

	if ( mbInitiallyVisible )
		show();

	// register phased ticks
	VuTickManager::IF()->registerHandler(this, &VuDynamicBreakableGamePropEntity::tickDecision, "Decision");
	VuTickManager::IF()->registerHandler(this, &VuDynamicBreakableGamePropEntity::tickBuild, "Build");
}

//*****************************************************************************
void VuDynamicBreakableGamePropEntity::onGameRelease()
{
	// unregister phased ticks
	VuTickManager::IF()->unregisterHandlers(this);

	hide();

	mpRigidBodyComponent->destroyRigidBody();
}

//*****************************************************************************
void VuDynamicBreakableGamePropEntity::getWorldTransform(btTransform& worldTrans) const
{
	VuMatrix matRB = mpTransformComponent->getWorldTransform();
	matRB.translateLocal(mpRigidBodyComponent->getCenterOfMass());

	worldTrans = VuDynamicsUtil::toBtTransform(matRB);
}

//*****************************************************************************
void VuDynamicBreakableGamePropEntity::setWorldTransform(const btTransform& worldTrans)
{
	VuMatrix matModel = VuDynamicsUtil::toVuMatrix(worldTrans);
	matModel.translateLocal(-mpRigidBodyComponent->getCenterOfMass());

	mpTransformComponent->setWorldTransform(matModel, false);
}

//*****************************************************************************
void VuDynamicBreakableGamePropEntity::show()
{
	if ( !mbVisible )
	{
		mbVisible = true;

		mp3dDrawStaticModelComponent->show();

		mpRigidBodyComponent->addToWorld();
	}
}

//*****************************************************************************
void VuDynamicBreakableGamePropEntity::hide()
{
	if ( mbVisible )
	{
		mbVisible = false;

		mp3dDrawStaticModelComponent->hide();

		mpRigidBodyComponent->removeFromWorld();
	}
}

//*****************************************************************************
void VuDynamicBreakableGamePropEntity::drawLayout(const Vu3dLayoutDrawParams &params)
{
	if ( params.mbDrawCollision )
	{
		mpRigidBodyComponent->draw(VuColor(255,255,255), params.mCamera);
	}
	else
	{
		mp3dDrawStaticModelComponent->drawLayout(params);
	}
}

//*****************************************************************************
bool VuDynamicBreakableGamePropEntity::collideLayout(const VuVector3 &v0, VuVector3 &v1)
{
	return mp3dDrawStaticModelComponent->collideLayout(v0, v1);
}

//*****************************************************************************
void VuDynamicBreakableGamePropEntity::transformModified()
{
	VuMatrix mat = mpTransformComponent->getWorldTransform();
	mat.scaleLocal(mpTransformComponent->getWorldScale());
	mp3dDrawStaticModelComponent->updateVisibility(mat);

	mpRigidBodyComponent->transformModified(mpTransformComponent->getWorldTransform());
	mpRigidBodyComponent->scaleModified(mpTransformComponent->getWorldScale());
}

//*****************************************************************************
void VuDynamicBreakableGamePropEntity::massModified()
{
	mpRigidBodyComponent->setMass(mMass);
	mpRigidBodyComponent->setCenterOfMass(mCenterOfMass);
}

//*****************************************************************************
void VuDynamicBreakableGamePropEntity::typeModified()
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
void VuDynamicBreakableGamePropEntity::OnExplosion(const VuParams &params)
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
void VuDynamicBreakableGamePropEntity::OnHitByMissile(const VuParams &params)
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
bool VuDynamicBreakableGamePropEntity::onRigidBodyContactAdded(VuContactPoint &cp)
{
	// check if we should break
	if ( cp.mpOtherBody->hasContactResponse() && !mbBreakNow )
	{
		VuRigidBody *pRigidBody = mpRigidBodyComponent->getRigidBody();

		VUUINT32 otherExtendedFlags = cp.mpOtherBody->getExtendedFlags();

		if ( otherExtendedFlags & EXT_COL_ENGINE_EXPLODABLE )
		{
			mpRigidBodyComponent->setCollisionFlags(mpRigidBodyComponent->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
			mCollisionPos = cp.mPosWorld;
			mCollisionVel = cp.mpOtherBody->getVuLinearVelocity();
			mBreakerEntity = cp.mpOtherBody->getEntity();
			mbBreakNow = true;
		}
		else
		{
			VuEntity *pOtherEntity = cp.mpOtherBody->getEntity();
			if ( pOtherEntity && pOtherEntity->isDerivedFrom(VuCarEntity::msRTTI) )
			{
				VuCarEntity *pCar = static_cast<VuCarEntity *>(pOtherEntity);

				// compute relative velocity
				VuVector3 vRelV = pRigidBody->getVuLinearVelocity() - cp.mpOtherBody->getVuLinearVelocity();
				float fRelVel = mThresholdNormal ? VuAbs(VuDot(cp.mNorWorld, vRelV)) : vRelV.mag();
				if ( fRelVel > mThresholdSpeed )
				{
					mpRigidBodyComponent->setCollisionFlags(mpRigidBodyComponent->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
					mCollisionPos = cp.mPosWorld;
					mCollisionVel = cp.mpOtherBody->getVuLinearVelocity();
					mBreakerEntity = pOtherEntity;
					mbBreakNow = true;

					float penalty = mSpeedPenalty*pCar->getBreakablePenalty();
					VuVector3 linVel = pCar->getRigidBody()->getVuLinearVelocity();
					linVel *= (1.0f - penalty);
					pCar->getRigidBody()->setVuLinearVelocity(linVel);
				}
			}
		}
	}

	VUINT16 otherCollisionGroup = cp.mpOtherBody->getCollisionGroup();
	if ( otherCollisionGroup & COL_GAME_DEACTIVATION )
		mbDeactivateNow = true;

	return !mbBreakNow;
}

//*****************************************************************************
void VuDynamicBreakableGamePropEntity::tickDecision(float fdt)
{
	mp3dDrawBreakableModelComponent->tickDecision(fdt);

	if ( mbDeactivateNow )
	{
		hide();
		mbDeactivateNow = false;
	}

	if ( mbBreakNow )
	{
		if ( mp3dDrawBreakableModelComponent->isWhole() )
		{
			hide();
			mp3dDrawBreakableModelComponent->startBreak(mpTransformComponent->getWorldTransform(), mCollisionVel, mp3dDrawStaticModelComponent->getColor());

			// script notification
			{
				VuParams params;
                if (mBreakerEntity)
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

	// collision lighting
	if ( mCollisionLighting & mbVisible )
	{
		float shadowValue;
		if ( VuDynamicsUtil::getShadowValue(mpTransformComponent->getWorldPosition(), VuVector3(0.0f, 0.0f, -SHADOW_TEST_DIST), shadowValue) )
			mp3dDrawStaticModelComponent->setShadowValue(shadowValue);
	}

	if (mbVisible
		&& mpTransformComponent
		&& mpTransformComponent->getWorldTransform().getTrans().mZ < -1000.0f)
	{
		VUPRINTF("VuDynamicBreakableGamePropEntity %08lx falling out of world.\n", (VUUINT32)this);
		hide();
	}

}

//*****************************************************************************
void VuDynamicBreakableGamePropEntity::tickBuild(float fdt)
{
	mp3dDrawBreakableModelComponent->tickBuild(fdt);

	// update visibility
	{
		VuMatrix mat = mpTransformComponent->getWorldTransform();
		mat.scaleLocal(mpTransformComponent->getWorldScale());
		mp3dDrawStaticModelComponent->updateVisibility(mat);
	}
}
