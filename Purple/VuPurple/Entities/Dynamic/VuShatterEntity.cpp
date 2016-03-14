//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuShatterEntity
// 
//*****************************************************************************

#include "VuShatterEntity.h"
#include "VuEngine/Components/Transform/VuTransformComponent.h"
#include "VuEngine/Components/3dDraw/Vu3dDrawComponent.h"
#include "VuEngine/Entities/VuEntityRepository.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Json/VuJsonContainer.h"
#include "VuEngine/Util/VuDataUtil.h"


IMPLEMENT_RTTI(VuShatterEntity, VuEntity);


//*****************************************************************************
VuShatterEntity::VuShatterEntity(const VuJsonContainer &data, const VuMatrix &transform, const VuVector3 &linVel, const VuColor &color):
	mTransform(transform),
	mColor(color),
	mAge(0.0f)
{
	// components
	addComponent(mp3dDrawComponent = new Vu3dDrawComponent(this));

	mp3dDrawComponent->setDrawMethod(this, &VuShatterEntity::draw);

	mBreakableModelInstance.setModelAsset(data["Model"].asString());

	VuDataUtil::getValue(data["MinLinVel"], mBreakableModelInstance.mMinPieceLinVel);
	VuDataUtil::getValue(data["MaxLinVel"], mBreakableModelInstance.mMaxPieceLinVel);
	VuDataUtil::getValue(data["MinAngVel"], mBreakableModelInstance.mMinPieceAngVel);
	VuDataUtil::getValue(data["MaxAngVel"], mBreakableModelInstance.mMaxPieceAngVel);
	VuDataUtil::getValue(data["MinVelDamping"], mBreakableModelInstance.mMinVelocityDamping);
	VuDataUtil::getValue(data["MaxVelDamping"], mBreakableModelInstance.mMaxVelocityDamping);

	mBreakableModelInstance.mMinPieceAngVel = VuDegreesToRadians(mBreakableModelInstance.mMinPieceAngVel);
	mBreakableModelInstance.mMaxPieceAngVel = VuDegreesToRadians(mBreakableModelInstance.mMaxPieceAngVel);

	mBreakableModelInstance.initializePieces(transform, linVel);

	// more params
	mFadeDelay = data["FadeDelay"].asFloat();
	mFadeTime = data["FadeTime"].asFloat();
	mDrawDistance = data["DrawDistance"].asFloat();
}

//*****************************************************************************
void VuShatterEntity::onGameInitialize()
{
	mpTransformComponent->setWorldTransform(mTransform);

	mp3dDrawComponent->updateVisibility(mBreakableModelInstance.getAabb(), mTransform);
	mp3dDrawComponent->show();

	// register phased ticks
	VuTickManager::IF()->registerHandler(this, &VuShatterEntity::tickBuild, "Build");
}

//*****************************************************************************
void VuShatterEntity::onGameRelease()
{
	mp3dDrawComponent->hide();

	// unregister phased tick
	VuTickManager::IF()->unregisterHandlers(this);
}

//*****************************************************************************
void VuShatterEntity::tickBuild(float fdt)
{
	mAge += fdt;

	VuAabb aabb;
	mBreakableModelInstance.updatePieces(fdt, aabb);
	if ( aabb.isValid() )
		mp3dDrawComponent->updateVisibility(aabb);

	if ( mAge > mFadeDelay + mFadeTime )
	{
		mp3dDrawComponent->hide();
		VuEntityRepository::IF()->removeManagedEntity(this);
	}
}

//*****************************************************************************
void VuShatterEntity::draw(const VuGfxDrawParams &params)
{
	if ( params.mbDrawReflection )
		return;

	float distSquared = (mTransform.getTrans() - params.mEyePos).magSquared();
	if ( distSquared > VuSquare(mDrawDistance) )
		return;

	float alpha = 1.0f - VuLinStep(mFadeDelay, mFadeDelay + mFadeTime, mAge);
	mColor.mA = (VUUINT8)VuRound(alpha*255.0f);

	mBreakableModelInstance.setColor(mColor);
	mBreakableModelInstance.drawPieces(params);
}
