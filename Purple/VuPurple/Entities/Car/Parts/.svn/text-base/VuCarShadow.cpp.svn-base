//*****************************************************************************
//
//  Copyright (c) 2012-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Car Shadow
// 
//*****************************************************************************

#include "VuCarShadow.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Camera/VuCarCamera.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarHeadlights.h"
#include "VuPurple/Entities/Car/Parts/VuCarSuspension.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/Shadow/VuBlobShadow.h"
#include "VuEngine/Json/VuJsonContainer.h"
#include "VuEngine/Util/VuDataUtil.h"


// constants

#define DROP_SHADOW_TEXTURE_SIZE 256
#define DROP_SHADOW_ALPHA 0.5f


//*****************************************************************************
VuCarShadow::VuCarShadow(VuCarEntity *pCar):
	mpCar(pCar),
	mpBlobShadow(VUNULL),
	mpDropShadow(VUNULL),
	mBlobShadowVisible(false),
	mDropShadowVisible(false)
{
}

//*****************************************************************************
VuCarShadow::~VuCarShadow()
{
	delete mpBlobShadow;
	delete mpDropShadow;
}

//*****************************************************************************
void VuCarShadow::preDataModified()
{
	if ( !mpBlobShadow )
		mpBlobShadow = new VuBlobShadow;

	if ( !mpDropShadow && (mpCar->getDriver()->isUi() || mpCar->getIsRacer()) )
		mpDropShadow = new VuDropShadow(DROP_SHADOW_TEXTURE_SIZE);
}

//*****************************************************************************
void VuCarShadow::postDataModified()
{
	// update blob shadow
	{
		const VuJsonContainer &blobShadowData = VuGameUtil::IF()->constantDB()["Car"]["BlobShadow"];
		mpBlobShadow->setTexture(blobShadowData["Texture"].asString());
		blobShadowData["Alpha"].getValue(mpBlobShadow->params().mAlpha);
	}
}

//*****************************************************************************
void VuCarShadow::onTick(float fdt)
{
	mBlobShadowVisible = false;
	mDropShadowVisible = false;

	// determine if car is visible in any viewport
	const VuMatrix &transform = mpCar->getModelMatrix();
	const VuAabb &aabb = mpCar->getCollisionAabb();

	bool visible = false;
	float closestDistance = FLT_MAX;
	for ( int iViewport = 0; iViewport < VuViewportManager::IF()->getViewportCount(); iViewport++ )
	{
		const VuCamera &camera = VuViewportManager::IF()->getViewport(iViewport).mCamera;
		if ( camera.isAabbVisible(aabb, transform) )
		{
			visible = true;
			closestDistance = VuMin(closestDistance, VuDist(transform.getTrans(), camera.getEyePosition()));
		}
	}

	if ( visible )
	{
		const VuMatrix &transform = mpCar->getModelMatrix();
		const VuAabb &aabb = mpCar->getCollisionAabb();

		if ( closestDistance < mpBlobShadow->params().mDistanceFadeFar )
		{
			mpBlobShadow->calculate(transform, aabb);
			mBlobShadowVisible = true;
		}

		if ( mpDropShadow && !mpCar->getHeadlights()->getTurnedOn() && VuGameUtil::IF()->getCarDropShadows() && VuGfxUtil::IF()->getShaderLOD() > 0 )
		{
			if ( mpCar->getAlpha() + FLT_EPSILON > 0.5f )
			{
				if ( closestDistance < mpDropShadow->params().mDistanceFadeFar )
				{
					VuAabb modelAabb = mpCar->getModelInstance()->getAabb();
					modelAabb.addAabb(mpCar->getSuspension()->getAabb());
					for ( int iWheel = 0; iWheel < 4; iWheel++ )
						modelAabb.addAabb(mpCar->getWheel(iWheel).mpModelInstance->getAabb());

					mpDropShadow->params().mAlpha = mpCar->getShadowValue()*DROP_SHADOW_ALPHA;
					mpDropShadow->calculate(transform, modelAabb);
					mDropShadowVisible = true;
				}
			}
		}
	}
}

//*****************************************************************************
void VuCarShadow::draw(const VuGfxDrawParams &params)
{
	if ( !params.mbDrawReflection )
	{
		if ( mBlobShadowVisible )
			mpBlobShadow->draw(params);

		if ( mDropShadowVisible && VuCarManager::IF()->getCameraTargetForViewport(VuGfxSort::IF()->getViewport()) == mpCar )
			mpDropShadow->draw(params, mpCar);
	}
}
