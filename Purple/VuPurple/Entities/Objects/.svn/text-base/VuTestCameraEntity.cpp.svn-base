//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Front end camera entity
// 
//*****************************************************************************

#include "VuPurple/Gfx/GfxComposer/VuPurpleGfxComposer.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/3dLayout/Vu3dLayoutComponent.h"
#include "VuEngine/Components/Transform/VuTransformComponent.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Managers/VuViewportManager.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/Camera/VuCamera.h"
#include "VuEngine/HAL/Gfx/VuGfx.h"


class VuTestCameraEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuTestCameraEntity();

	virtual void		onPostLoad() { modified(); }
	virtual void		onGameInitialize();
	virtual void		onGameRelease();

private:
	void				tickViewports(float fdt);
	void				modified();
	void				drawLayout(const Vu3dLayoutDrawParams &params);

	// components
	Vu3dLayoutComponent		*mp3dLayoutComponent;

	// properties

	VuCamera			mCamera;
};


IMPLEMENT_RTTI(VuTestCameraEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuTestCameraEntity);


//*****************************************************************************
VuTestCameraEntity::VuTestCameraEntity()
{
	// components
	addComponent(mp3dLayoutComponent = new Vu3dLayoutComponent(this));

	mpTransformComponent->setWatcher(&VuTestCameraEntity::modified);
	mpTransformComponent->setMask(VuTransformComponent::TRANS|VuTransformComponent::ROT);
	mp3dLayoutComponent->setDrawMethod(this, &VuTestCameraEntity::drawLayout);
}

//*****************************************************************************
void VuTestCameraEntity::onGameInitialize()
{
	// register phased tick
	VuTickManager::IF()->registerHandler(this, &VuTestCameraEntity::tickViewports, "Viewports");
}

//*****************************************************************************
void VuTestCameraEntity::onGameRelease()
{
	// unregister phased tick
	VuTickManager::IF()->unregisterHandlers(this);
}

//*****************************************************************************
void VuTestCameraEntity::tickViewports(float fdt)
{
	// configure camera proj matrix
	float aspectRatio = VuGameUtil::IF()->calcDisplayAspectRatio();
	float vertFov = VuGameUtil::IF()->calcCameraVertFov();
	mCamera.setProjMatrixVert(vertFov, aspectRatio, 1.0f, 500.0f);

	VuViewportManager::IF()->setCamera(0, mCamera);
	VuViewportManager::IF()->setUiCameraVert(mCamera.getFovVert(), mCamera.getAspectRatio(), mCamera.getNearPlane(), mCamera.getFarPlane());
}

//*****************************************************************************
void VuTestCameraEntity::modified()
{
	const VuMatrix &mat = mpTransformComponent->getWorldTransform();
	VuVector3 vEye = mat.getTrans();
	VuVector3 vTarget = vEye + mat.getAxisY();
	VuVector3 vUp = mat.getAxisZ();

	mCamera.setViewMatrix(vEye, vTarget, vUp);
}

//*****************************************************************************
void VuTestCameraEntity::drawLayout(const Vu3dLayoutDrawParams &params)
{
	VuGfxUtil *pGfxUtil = VuGfxUtil::IF();

	VuMatrix mat = mpTransformComponent->getWorldTransform();
	VuVector3 vPos = mat.getTrans();
	VuVector3 vDir = -mat.getAxisZ();

	VuColor color(128, 255, 128);

	// draw center arrow
	{
		VuMatrix mat = mpTransformComponent->getWorldTransform();
		pGfxUtil->drawArrowLines(color, 10.0f, 1.0f, 1.0f, mat*params.mCamera.getViewProjMatrix());
	}
}