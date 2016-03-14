//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Out of Bounds Entities
// 
//*****************************************************************************

#include "VuPurple/Managers/VuOutOfBoundsManager.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/Transform/VuTransformComponent.h"
#include "VuEngine/Components/3dLayout/Vu3dLayoutComponent.h"
#include "VuEngine/Properties/VuBasicProperty.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/Camera/VuCamera.h"


//*****************************************************************************
// Base Entity
//*****************************************************************************
class VuOutOfBoundsBaseEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuOutOfBoundsBaseEntity();

protected:
	virtual void		drawLayout(const Vu3dLayoutDrawParams &params) = 0;

	// components
	Vu3dLayoutComponent	*mp3dLayoutComponent;

	// properties
	float				mRecoverTime;
};

IMPLEMENT_RTTI(VuOutOfBoundsBaseEntity, VuEntity);

//*****************************************************************************
VuOutOfBoundsBaseEntity::VuOutOfBoundsBaseEntity():
	mRecoverTime(1.0f)
{
	// components
	addComponent(mp3dLayoutComponent = new Vu3dLayoutComponent(this));

	mp3dLayoutComponent->setDrawMethod(this, &VuOutOfBoundsBaseEntity::drawLayout);

	// properties
	addProperty(new VuFloatProperty("Recover Time", mRecoverTime));
}


//*****************************************************************************
// Box Entity
//*****************************************************************************
class VuOutOfBoundsBoxEntity : public VuOutOfBoundsBaseEntity, VuOutOfBoundsManager::VuOutOfBoundsIF
{
	DECLARE_RTTI

public:
	virtual void		onGameInitialize();
	virtual void		onGameRelease();

protected:
	// VuOutOfBoundsManager::VuOutOfBoundsIF
	virtual bool		isOutOfBounds(const VuVector3 &position) const;

	virtual void		drawLayout(const Vu3dLayoutDrawParams &params);
};

IMPLEMENT_RTTI(VuOutOfBoundsBoxEntity, VuOutOfBoundsBaseEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuOutOfBoundsBoxEntity);

//*****************************************************************************
void VuOutOfBoundsBoxEntity::onGameInitialize()
{
	VuOutOfBoundsManager::IF()->addZone(this, mpTransformComponent->getWorldPosition(), mpTransformComponent->getWorldScale().mag(), mRecoverTime);
}

//*****************************************************************************
void VuOutOfBoundsBoxEntity::onGameRelease()
{
	VuOutOfBoundsManager::IF()->removeZone(this);
}

//*****************************************************************************
bool VuOutOfBoundsBoxEntity::isOutOfBounds(const VuVector3 &position) const
{
	const VuMatrix &mat = mpTransformComponent->getWorldTransform();
	const VuVector3 extents = mpTransformComponent->getWorldScale();

	VuVector3 relPos = position - mat.getTrans();
	float maxDist = VuAbs(VuDot(relPos, mat.getAxisX())) - extents.mX;
	maxDist = VuMax(maxDist, VuAbs(VuDot(relPos, mat.getAxisY())) - extents.mY);
	maxDist = VuMax(maxDist, VuAbs(VuDot(relPos, mat.getAxisZ())) - extents.mZ);

	return maxDist < 0.0f;
}

//*****************************************************************************
void VuOutOfBoundsBoxEntity::drawLayout(const Vu3dLayoutDrawParams &params)
{
	if ( params.mbSelected )
	{
		VuMatrix mat = mpTransformComponent->getWorldTransform();
		mat.scaleLocal(mpTransformComponent->getWorldScale());

		VuGfxUtil::IF()->drawAabbSolid(VuColor(128,255,128,128), mp3dLayoutComponent->getLocalBounds(), mat, params.mCamera.getViewProjMatrix());
	}
}
