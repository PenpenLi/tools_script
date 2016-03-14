//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuCarAttachRagdollEffect
// 
//*****************************************************************************

#include "VuCarEffect.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarSuspension.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Gfx/Model/VuAnimatedModelInstance.h"
#include "VuEngine/Animation/VuSkeleton.h"
#include "VuEngine/Dynamics/VuRagdoll.h"


class VuCarAttachRagdollEffect : public VuCarEffect
{
public:
	VuCarAttachRagdollEffect();
	~VuCarAttachRagdollEffect();
	virtual void	onApply(const VuJsonContainer &data);
	virtual void	onStop(const VuJsonContainer &data);
	virtual void	onTick(float fdt);
	virtual void	onDraw(const VuGfxDrawParams &params);

	std::string				mRagdoll;
	VuAnimatedModelInstance	mModelInstance;
	VuEntity				*mpRagdollEntity;
	VuRagdoll				*mpRagdoll;
};

IMPLEMENT_CAR_EFFECT(VuCarAttachRagdollEffect)


//*****************************************************************************
VuCarAttachRagdollEffect::VuCarAttachRagdollEffect()
{
	mpRagdollEntity = new VuEntity;
	mpRagdoll = new VuRagdoll;
}

//*****************************************************************************
VuCarAttachRagdollEffect::~VuCarAttachRagdollEffect()
{
	mpRagdollEntity->removeRef();
	delete mpRagdoll;
}

//*****************************************************************************
void VuCarAttachRagdollEffect::onApply(const VuJsonContainer &data)
{
	mModelInstance.setModelAsset(data["ModelAsset"].asString());

	const std::string &ragdoll = data["Ragdoll"].asString();
	if ( ragdoll != mRagdoll )
	{
		mRagdoll = ragdoll;

		mpRagdoll->stopSimulation();

		const VuJsonContainer &ragdollData = VuGameUtil::IF()->ragdollDB()[ragdoll];
		VuRagdoll::Params ragdollParams;
		mpRagdoll->configure(mModelInstance.getSkeleton(), ragdollData, mpRagdollEntity, ragdollParams);

		VuVector3 carOffset = mpCar->getMountPoint(data["MountPoint"].asCString()).getTrans();
		carOffset -= mpCar->getCenterOfMass();

		mpRagdoll->attachTo(mpCar->getRigidBody(), carOffset, data["AttachedBody"].asCString());

		VuMatrix transform = mpCar->getModelMatrix();
		transform.translateLocal(carOffset);
		mpRagdoll->startSimulation(transform, mModelInstance.getSkeleton()->mpLocalPose, mpCar->getLinearVelocity(), mpCar->getAngularVelocity());
	}
}

//*****************************************************************************
void VuCarAttachRagdollEffect::onStop(const VuJsonContainer &data)
{
	mpRagdoll->stopSimulation();
}

//*****************************************************************************
void VuCarAttachRagdollEffect::onTick(float fdt)
{
	mModelInstance.setPose(mpCar->getModelMatrix(), mpRagdoll);
	mModelInstance.finalizePose();
}

//*****************************************************************************
void VuCarAttachRagdollEffect::onDraw(const VuGfxDrawParams &params)
{
	mModelInstance.draw(mpCar->getModelMatrix(), params);
}
