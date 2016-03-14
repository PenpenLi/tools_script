//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Front end camera entity
// 
//*****************************************************************************

#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Components/3dLayout/Vu3dLayoutComponent.h"
#include "VuEngine/Components/Transform/VuTransformComponent.h"
#include "VuEngine/Properties/VuNotifyProperty.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/Camera/VuCamera.h"
#include "VuEngine/HAL/Gfx/VuGfx.h"


class VuFrontEndCameraEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuFrontEndCameraEntity();

	virtual void		onGameInitialize();

private:
	// scripting
	VuRetVal			Activate(const VuParams &params = VuParams());

	void				drawLayout(const Vu3dLayoutDrawParams &params);
	void				test() { Activate(); }

	// components
	VuScriptComponent	*mpScriptComponent;
	Vu3dLayoutComponent	*mp3dLayoutComponent;

	// properties
	bool				mInitiallyActive;

	// references
	VuScriptRef			*mpTargetRef;
};


IMPLEMENT_RTTI(VuFrontEndCameraEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuFrontEndCameraEntity);


//*****************************************************************************
VuFrontEndCameraEntity::VuFrontEndCameraEntity():
	mInitiallyActive(true)
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this, 150));
	addComponent(mp3dLayoutComponent = new Vu3dLayoutComponent(this));

	mpTransformComponent->setMask(VuTransformComponent::TRANS|VuTransformComponent::ROT);
	mp3dLayoutComponent->setDrawMethod(this, &VuFrontEndCameraEntity::drawLayout);

	// properties
	addProperty(new VuBoolProperty("Initially Active", mInitiallyActive));
	addProperty(new VuNotifyProperty("Test")) -> setWatcher(this, &VuFrontEndCameraEntity::test);

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuFrontEndCameraEntity, Activate);
	mpTargetRef = ADD_SCRIPT_REF(mpScriptComponent, Target, VuEntity::msRTTI);
}

//*****************************************************************************
void VuFrontEndCameraEntity::onGameInitialize()
{
	if ( mInitiallyActive )
		Activate();
}

//*****************************************************************************
VuRetVal VuFrontEndCameraEntity::Activate(const VuParams &params)
{
	const VuMatrix &mat = mpTransformComponent->getWorldTransform();
	VuVector3 vEye = mat.getTrans();
	VuVector3 vTarget = vEye + mat.getAxisY();
	VuVector3 vUp = mat.getAxisZ();

	if ( VuEntity *pTargetEntity = mpTargetRef->getRefEntity() )
		vTarget = pTargetEntity->getTransformComponent()->getWorldPosition();

	VuParams outParams;
	outParams.addVector3(vEye);
	outParams.addVector3(vTarget);
	outParams.addVector3(vUp);
	VuEventManager::IF()->broadcast("OnSetFrontEndCamera", outParams);

	return VuRetVal();
}

//*****************************************************************************
void VuFrontEndCameraEntity::drawLayout(const Vu3dLayoutDrawParams &params)
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
