//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuCarRemoteControlEffect
// 
//*****************************************************************************

#include "VuCarEffect.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarEffectController.h"
#include "VuPurple/Entities/Car/Camera/VuCarCamera.h"
#include "VuPurple/Managers/VuAchievementManager.h"
#include "VuPurple/PowerUps/VuPowerUpUtil.h"
#include "VuEngine/Pfx/VuPfxManager.h"
#include "VuEngine/Pfx/VuPfxEntity.h"
#include "VuEngine/Pfx/VuPfx.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Gfx/Model/VuAnimatedModelInstance.h"
#include "VuEngine/Animation/VuSkeleton.h"
#include "VuEngine/Dynamics/VuRagdoll.h"
#include "VuEngine/Services/VuServiceManager.h"
#include "VuEngine/Services/VuPfxAttachService.h"


class VuCarRemoteControlEffect : public VuCarEffect
{
public:
	VuCarRemoteControlEffect();
	~VuCarRemoteControlEffect();

	virtual void	onStart(const VuJsonContainer &data, const char *effectName);
	virtual void	onApply(const VuJsonContainer &data);
	virtual void	onStop(const VuJsonContainer &data);
	virtual void	onDraw(const VuGfxDrawParams &params);
	void			tickPostDecision(float fdt);

	VUUINT32		mNitroActive;

	std::string				mRagdoll;
	VuAnimatedModelInstance	mModelInstance;
	VuEntity				*mpRagdollEntity;
	VuRagdoll				*mpRagdoll;

	std::string		mRagdollPfxName;
	VUUINT32		mRagdollPfx;
};

IMPLEMENT_CAR_EFFECT(VuCarRemoteControlEffect)


//*****************************************************************************
VuCarRemoteControlEffect::VuCarRemoteControlEffect():
	mNitroActive(0),
	mRagdollPfx(0)
{
	mpRagdollEntity = new VuEntity;
	mpRagdoll = new VuRagdoll;
}

//*****************************************************************************
VuCarRemoteControlEffect::~VuCarRemoteControlEffect()
{
	mpRagdollEntity->removeRef();
	delete mpRagdoll;
}

//*****************************************************************************
void VuCarRemoteControlEffect::onStart(const VuJsonContainer &data, const char *effectName)
{
	VuTickManager::IF()->registerHandler(this, &VuCarRemoteControlEffect::tickPostDecision, "PostDecision");
}

//*****************************************************************************
void VuCarRemoteControlEffect::onApply(const VuJsonContainer &data)
{
	mModelInstance.setModelAsset(data["ModelAsset"].asString());

	const std::string &ragdoll = data["Ragdoll"].asString();
	if ( ragdoll != mRagdoll )
	{
		mRagdoll = ragdoll;

		mpRagdoll->stopSimulation();

		const VuJsonContainer &ragdollData = VuGameUtil::IF()->ragdollDB()[ragdoll];
		VuRagdoll::Params ragdollParams;
		ragdollParams.mCollisionMask = 0;
		mpRagdoll->configure(mModelInstance.getSkeleton(), ragdollData, mpRagdollEntity, ragdollParams);

		VuVector3 carOffset = mpCar->getMountPoint(data["MountPoint"].asCString()).getTrans();
		carOffset -= mpCar->getCenterOfMass();

		mpRagdoll->attachTo(mpCar->getRigidBody(), carOffset, data["AttachedBody"].asCString());

		VuMatrix transform = mpCar->getModelMatrix();
		transform.translateLocal(carOffset);
		mpRagdoll->startSimulation(transform, mModelInstance.getSkeleton()->mpLocalPose, mpCar->getLinearVelocity(), mpCar->getAngularVelocity());
	}

	// pfx
	const std::string &ragdollPfxName = data["RagdollPfx"]["Name"].asString();
	if ( ragdollPfxName != mRagdollPfxName )
	{
		mRagdollPfxName = ragdollPfxName;

		if ( VuPfxEntity *pPfxEntity = VuPfxManager::IF()->getEntity(mRagdollPfx) )
			pPfxEntity->getSystemInstance()->stop(true);

		const VuJsonContainer &ragdollPfxData = data["RagdollPfx"];

		if ( ragdollPfxData.isObject() )
		{
			if ( (mRagdollPfx = VuPfxManager::IF()->createEntity(ragdollPfxData["Name"].asCString(), false)) )
			{
				if ( VuPfxEntity *pPfxEntity = VuPfxManager::IF()->getEntity(mRagdollPfx) )
				{
					if ( VuPfxAttachService *pService = VuServiceManager::IF()->createPfxService<VuPfxAttachService>() )
					{
						const std::string &bone = ragdollPfxData["Bone"].asString();

						int boneIndex = mModelInstance.getSkeleton()->getBoneIndex(bone.c_str());
						VuMatrix transform;
						VuPowerUpUtil::readTransform(ragdollPfxData, transform);
						pService->init(mpRagdollEntity, mRagdollPfx, transform, &mModelInstance, boneIndex);
					}

					pPfxEntity->getSystemInstance()->start();
				}
			}
		}
	}
}

//*****************************************************************************
void VuCarRemoteControlEffect::onStop(const VuJsonContainer &data)
{
	// pfx
	if ( VuPfxEntity *pPfxEntity = VuPfxManager::IF()->getEntity(mRagdollPfx) )
		pPfxEntity->getSystemInstance()->stop();

	mpRagdoll->stopSimulation();
	VuTickManager::IF()->unregisterHandler(this, "PostDecision");
}

//*****************************************************************************
void VuCarRemoteControlEffect::tickPostDecision(float fdt)
{
	if ( mpOriginator && mpOriginator->isDerivedFrom(VuCarEntity::msRTTI) )
	{
		VuCarEntity *pController = static_cast<VuCarEntity *>(mpOriginator);

		// did controller lose sight of me?
		VuVector3 myPos = mpCar->getModelMatrix().transform(mpCar->getCollisionAabb().getCenter());
		VuVector3 screenPos = pController->getCamera()->worldToScreen(myPos);

		if ( screenPos.mZ < 0.0f || screenPos.mX < 0.0f || screenPos.mX > 1.0f || screenPos.mY < 0.0f || screenPos.mY > 1.0f )
		{
			mTimeRemaining = 0.0f;
		}
		else
		{
			mpCar->setYawControl(pController->getYawControl());
			mpCar->setThrottleControl(pController->getThrottleControl());

			if ( mpCar->hasCrashed() && mNitroActive )
			{
				if ( pController->getDriver()->isHuman() )
					VuAchievementManager::IF()->unlock(37); // Use Remote Control to detonate a car under the Nitro effect
			}
		}
	}

	mNitroActive = mpCar->getEffectController()->isActive(VuCarEffectController::NITRO);

	mpRagdollEntity->getTransformComponent()->setWorldTransform(mpCar->getModelMatrix());

	mModelInstance.setPose(mpCar->getModelMatrix(), mpRagdoll);
	mModelInstance.finalizePose();
}

//*****************************************************************************
void VuCarRemoteControlEffect::onDraw(const VuGfxDrawParams &params)
{
	mModelInstance.draw(mpCar->getModelMatrix(), params);
}
