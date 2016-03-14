//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Billboard entity
// 
//*****************************************************************************

#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/Transform/VuTransformComponent.h"
#include "VuEngine/Components/3dLayout/Vu3dLayoutComponent.h"
#include "VuEngine/Components/3dDraw/Vu3dDrawComponent.h"
#include "VuEngine/Properties/VuAssetProperty.h"
#include "VuEngine/Assets/VuStaticModelAsset.h"
#include "VuEngine/Gfx/Model/VuStaticModelInstance.h"
#include "VuEngine/Gfx/Camera/VuCamera.h"


class VuBillboardEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuBillboardEntity();

	virtual void			onPostLoad();
	virtual void			onGameInitialize();
	virtual void			onGameRelease();

private:
	void					drawLayout(const Vu3dLayoutDrawParams &params);
	void					draw(const VuGfxDrawParams &params);
	void					transformModified();
	void					modelModified();

	// components
	Vu3dLayoutComponent		*mp3dLayoutComponent;
	Vu3dDrawComponent		*mp3dDrawComponent;

	// properties
	std::string				mModelAssetName;
	VuColor					mColor;
	float					mDrawDist;

	VuStaticModelInstance	mModelInstance;
};

IMPLEMENT_RTTI(VuBillboardEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuBillboardEntity);


//*****************************************************************************
VuBillboardEntity::VuBillboardEntity():
	mColor(255,255,255),
	mDrawDist(FLT_MAX)
{
	// components
	addComponent(mp3dLayoutComponent = new Vu3dLayoutComponent(this));
	addComponent(mp3dDrawComponent = new Vu3dDrawComponent(this));

	// want to know when transform is changed
	mpTransformComponent->setWatcher(&VuBillboardEntity::transformModified);

	mp3dLayoutComponent->setDrawMethod(this, &VuBillboardEntity::drawLayout);
	mp3dDrawComponent->setDrawMethod(this, &VuBillboardEntity::draw);

	// properties
	addProperty(new VuAssetNameProperty(VuStaticModelAsset::msRTTI.mstrType, "Model Asset", mModelAssetName))
		->setWatcher(this, &VuBillboardEntity::modelModified);

	addProperty(new VuColorProperty("Color", mColor));
	addProperty(new VuFloatProperty("Draw Distance", mDrawDist));
}

//*****************************************************************************
void VuBillboardEntity::onPostLoad()
{
	modelModified();
	transformModified();
}

//*****************************************************************************
void VuBillboardEntity::onGameInitialize()
{
	mp3dDrawComponent->show();
}

//*****************************************************************************
void VuBillboardEntity::onGameRelease()
{
	mp3dDrawComponent->hide();
}

//*****************************************************************************
void VuBillboardEntity::drawLayout(const Vu3dLayoutDrawParams &params)
{
	// check draw dist
	VuVector3 vPos = mpTransformComponent->getWorldPosition();
	float distSquared = (vPos - params.mCamera.getEyePosition()).magSquared();
	if ( distSquared > mDrawDist*mDrawDist )
		return;

	// build camera-facing transform
	VuMatrix mat;
	mat.loadIdentity();
	mat.setAxisX(-params.mCamera.getTransform().getAxisX());
	mat.setAxisY(-params.mCamera.getTransform().getAxisY());
	mat.setAxisZ(params.mCamera.getTransform().getAxisZ());
	mat.setTrans(vPos);
	mat.scaleLocal(mpTransformComponent->getWorldScale());

	mModelInstance.setColor(mColor);
	mModelInstance.draw(mat, VuGfxDrawParams(params.mCamera));
}

//*****************************************************************************
void VuBillboardEntity::draw(const VuGfxDrawParams &params)
{
	// check draw dist
	VuVector3 vPos = mpTransformComponent->getWorldPosition();
	float distSquared = (vPos - params.mEyePos).magSquared();
	if ( distSquared > mDrawDist*mDrawDist )
		return;

	// build camera-facing transform
	VuMatrix mat;
	mat.loadIdentity();
	mat.setAxisX(-params.mCamera.getTransform().getAxisX());
	mat.setAxisY(-params.mCamera.getTransform().getAxisY());
	mat.setAxisZ(params.mCamera.getTransform().getAxisZ());
	mat.setTrans(vPos);
	mat.scaleLocal(mpTransformComponent->getWorldScale());

	mModelInstance.setColor(mColor);
	mModelInstance.draw(mat, params);
}

//*****************************************************************************
void VuBillboardEntity::transformModified()
{
	VuMatrix mat = mpTransformComponent->getWorldTransform();
	mat.scaleLocal(mpTransformComponent->getWorldScale());

	mp3dDrawComponent->updateVisibility(mModelInstance.getAabb(), mat);
}

//*****************************************************************************
void VuBillboardEntity::modelModified()
{
	mModelInstance.setModelAsset(mModelAssetName);

	mp3dLayoutComponent->setLocalBounds(mModelInstance.getAabb());
}
