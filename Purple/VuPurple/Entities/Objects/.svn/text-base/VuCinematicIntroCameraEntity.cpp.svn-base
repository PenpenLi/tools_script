//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Cinematic Intro Camera Entity
// 
//*****************************************************************************

#include "VuPurple/Managers/VuFadeManager.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/Transform/VuTransformComponent.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Components/3dLayout/Vu3dLayoutComponent.h"
#include "VuEngine/Components/Motion/VuMotionComponent.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Managers/VuViewportManager.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/Camera/VuCamera.h"
#include "VuEngine/HAL/Gfx/VuGfx.h"


class VuCinematicIntroCameraEntity : public VuEntity, public VuMotionComponentIF
{
	DECLARE_RTTI

public:
	VuCinematicIntroCameraEntity();

	virtual void		onGameInitialize();
	virtual void		onGameRelease();

private:
	// VuMotionComponentIF interface
	virtual void		onMotionUpdate();

	// event handlers
	void				OnSkipIntro(const VuParams &params) { mSkipped = true; }

	// scripting
	VuRetVal			Activate(const VuParams &params = VuParams());

	void				tickDecision(float fdt);

	void				drawLayout(const Vu3dLayoutDrawParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;
	Vu3dLayoutComponent	*mp3dLayoutComponent;
	VuMotionComponent	*mpMotionComponent;

	// properties
	float				mTotalTime;
	float				mFadeTime;
	float				mNearPlane;
	float				mFarPlane;

	// references
	VuScriptRef			*mpTargetRef;

	bool				mActive;
	float				mTimer;
	bool				mFadingOut;
	VuCamera			mCamera;
	bool				mSkipped;
};


IMPLEMENT_RTTI(VuCinematicIntroCameraEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuCinematicIntroCameraEntity);


//*****************************************************************************
VuCinematicIntroCameraEntity::VuCinematicIntroCameraEntity():
	mTotalTime(5.0f),
	mFadeTime(0.5f),
	mActive(false),
	mSkipped(false),
	mNearPlane(1.0f),
	mFarPlane(500.0f)
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this, 150));
	addComponent(mp3dLayoutComponent = new Vu3dLayoutComponent(this));
	addComponent(mpMotionComponent = new VuMotionComponent(this, this));

	mpTransformComponent->setMask(VuTransformComponent::TRANS|VuTransformComponent::ROT);
	mp3dLayoutComponent->setDrawMethod(this, &VuCinematicIntroCameraEntity::drawLayout);

	// properties
	addProperty(new VuFloatProperty("Total Time", mTotalTime));
	addProperty(new VuFloatProperty("Fade Time", mFadeTime));
	addProperty(new VuFloatProperty("Near Plane", mNearPlane));
	addProperty(new VuFloatProperty("Far Plane", mFarPlane));

	// event handlers
	REG_EVENT_HANDLER(VuCinematicIntroCameraEntity, OnSkipIntro);

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuCinematicIntroCameraEntity, Activate);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnActivated);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnExpired);
	mpTargetRef = ADD_SCRIPT_REF(mpScriptComponent, Target, VuEntity::msRTTI);
}

//*****************************************************************************
void VuCinematicIntroCameraEntity::onGameInitialize()
{
}

//*****************************************************************************
void VuCinematicIntroCameraEntity::onGameRelease()
{
	VuTickManager::IF()->unregisterHandlers(this);
	VuCarManager::IF()->setCameraOverride(VUNULL);
}

//*****************************************************************************
void VuCinematicIntroCameraEntity::onMotionUpdate()
{
	mpTransformComponent->setWorldTransform(mpMotionComponent->getWorldTransform(), false);
}

//*****************************************************************************
VuRetVal VuCinematicIntroCameraEntity::Activate(const VuParams &params)
{
	if ( !mActive )
	{
		mActive = true;
		mTimer = 0.0f;
		mFadingOut = false;
		VuFadeManager::IF()->startFadeIn(mFadeTime);
		VuTickManager::IF()->registerHandler(this, &VuCinematicIntroCameraEntity::tickDecision, "Decision");
		mpScriptComponent->getPlug("OnActivated")->execute();

		VuCarManager::IF()->setCameraOverride(&mCamera);
	}

	return VuRetVal();
}

//*****************************************************************************
void VuCinematicIntroCameraEntity::tickDecision(float fdt)
{
	float aspectRatio = VuGameUtil::IF()->calcDisplayAspectRatio();
	float vertFov = VuGameUtil::IF()->calcCameraVertFov();
	mCamera.setProjMatrixVert(vertFov, aspectRatio, mNearPlane, mFarPlane);

	const VuMatrix &mat = mpTransformComponent->getWorldTransform();
	VuVector3 vEye = mat.getTrans();
	VuVector3 vTarget = vEye + mat.getAxisY();
	VuVector3 vUp = mat.getAxisZ();
	if ( VuEntity *pTargetEntity = mpTargetRef->getRefEntity() )
		vTarget = pTargetEntity->getTransformComponent()->getWorldPosition();
	mCamera.setViewMatrix(vEye, vTarget, vUp);

	VuCarManager::IF()->setCameraOverride(&mCamera);

	mTimer += fdt;
	if ( mFadingOut )
	{
		if ( !VuFadeManager::IF()->isFading() )
		{
			mActive = false;
			VuTickManager::IF()->unregisterHandler(this, "Decision");
			VuCarManager::IF()->setCameraOverride(VUNULL);
			VuFadeManager::IF()->startFadeIn();

			if ( !mSkipped )
			{
				mpScriptComponent->getPlug("OnExpired")->execute();
			}
		}
	}
	else if ( mTimer > mFadeTime )
	{
		if ( mSkipped || (mTimer > mTotalTime - mFadeTime) )
		{
			VuFadeManager::IF()->startFadeOut(mFadeTime);
			mFadingOut = true;
		}
	}
}

//*****************************************************************************
void VuCinematicIntroCameraEntity::drawLayout(const Vu3dLayoutDrawParams &params)
{
	VuGfxUtil *pGfxUtil = VuGfxUtil::IF();

	VuMatrix mat = mpTransformComponent->getWorldTransform();
	VuVector3 vPos = mat.getTrans();
	VuVector3 vDir = -mat.getAxisZ();

	VuColor color(128, 255, 128);

	if ( VuEntity *pTargetEntity = mpTargetRef->getRefEntity() )
	{
		// draw line to target
		pGfxUtil->drawLine3d(color, mpTransformComponent->getWorldPosition(), pTargetEntity->getTransformComponent()->getWorldPosition(), params.mCamera.getViewMatrix());
	}
	else
	{
		// draw center arrow
		VuMatrix mat = mpTransformComponent->getWorldTransform();
		pGfxUtil->drawArrowLines(color, 10.0f, 1.0f, 1.0f, mat*params.mCamera.getViewProjMatrix());
	}
}
