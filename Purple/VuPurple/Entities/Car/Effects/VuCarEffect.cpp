//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuCarEffect class
// 
//*****************************************************************************

#include "VuCarEffect.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/Entities/Dynamic/VuShatterEntity.h"
#include "VuPurple/PowerUps/VuPowerUpUtil.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuEngine/Entities/VuEntityRepository.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Assets/VuTextureAsset.h"
#include "VuEngine/Pfx/VuPfxManager.h"
#include "VuEngine/Pfx/VuPfxEntity.h"
#include "VuEngine/Pfx/VuPfx.h"
#include "VuEngine/Gfx/GfxSort/VuGfxSort.h"
#include "VuEngine/Util/VuAudioUtil.h"


//*****************************************************************************
void VuCarEffect::start(const VuJsonContainer &data, const char *effectName)
{
	data["ReapplyType"].getValue(mReapplyType);

	// hit image
	const std::string &hitImage = data["HitNotification"].asString();
	if ( hitImage.length() && mpOriginator )
	{
		int viewport = 0;
		if ( VuCarManager::IF()->getViewport(mpCar, viewport) )
		{
			VuParams params;
			params.addInt(viewport);
			params.addAsset(mpOriginator->getDriver()->getPortraitImage());
			params.addString(hitImage.c_str());
			VuEventManager::IF()->broadcast("ShowHitNotification", params);
		}
	}

	// global event
	{
		VuParams params;
		params.addEntity(mpCar);
		params.addString(effectName);
		params.addEntity(mpOriginator);
		VuEventManager::IF()->broadcast("OnStartCarEffect", params);
	}
	
	onStart(data, effectName);
}

//*****************************************************************************
void VuCarEffect::apply(const VuJsonContainer &data)
{
	// sfx
	VuPowerUpUtil::createSfx(mpCar, data["StartSfx"].asCString());

	// pfx
	VuPowerUpUtil::createPfx(mpCar, data["StartPfx"], true);

	// sfx
	const std::string &loopingSfxName = data["LoopingSfx"].asString();
	if ( loopingSfxName != mLoopingSfxName )
	{
		mLoopingSfxName = loopingSfxName;

		mLoopingSfx.release(VuAudioEvent::STOP_IMMEDIATE);

		if ( mLoopingSfx.create(mLoopingSfxName.c_str()) )
		{
			mLoopingSfx.set3DAttributes(&mpCar->getModelPosition(), &mpCar->getLinearVelocity(), VUNULL);
			mLoopingSfx.start();
		}
	}

	// pfx
	const std::string &loopingPfxName = data["LoopingPfx"]["Name"].asString();
	if ( loopingPfxName != mLoopingPfxName )
	{
		mLoopingPfxName = loopingPfxName;

		if ( VuPfxEntity *pPfxEntity = VuPfxManager::IF()->getEntity(mLoopingPfx) )
			pPfxEntity->getSystemInstance()->stop(true);

		mLoopingPfx = VuPowerUpUtil::createPfx(mpCar, data["LoopingPfx"], false);
	}
	else
	{
		// handle case where effect is activated after looping pfx has been soft-killed
		if ( VuPfxEntity *pPfxEntity = VuPfxManager::IF()->getEntity(mLoopingPfx) )
		{
			if ( pPfxEntity->getSystemInstance()->getState() == VuPfxSystemInstance::STATE_STOPPING )
			{
				pPfxEntity->getSystemInstance()->stop(true);
				pPfxEntity->getSystemInstance()->start();
			}
		}
	}

	// splat pfx
	const std::string &splatPfxName = data["SplatPfx"].asString();
	if ( splatPfxName != mSplatPfxName )
	{
		mSplatPfxName = splatPfxName;

		if ( mpSplatPfx )
			VuPfx::IF()->releaseSystemInstance(mpSplatPfx);

		mpSplatPfx = VuPfx::IF()->createSystemInstance(splatPfxName.c_str());
		if ( mpSplatPfx )
			mpSplatPfx->start();

		mSplatPfxDist = data["SplatPfxDist"].asFloat();
	}
	else
	{
		if ( mpSplatPfx && mpSplatPfx->getState() == VuPfxSystemInstance::STATE_STOPPING )
		{
			mpSplatPfx->stop(true);
			mpSplatPfx->start();
		}
	}

	mSoftKillTime = data["SoftKillTime"].asFloat();

	if ( mReapplyType == "Stack" )
		mTimeRemaining += data["Duration"].asFloat();
	else
		mTimeRemaining = data["Duration"].asFloat();

	onApply(data);

	mpData = &data;
}

//*****************************************************************************
void VuCarEffect::stop()
{
	const VuJsonContainer &data = *mpData;

	onStop(data);

	// sfx
	mLoopingSfx.release(VuAudioEvent::STOP_IMMEDIATE);

	VuAudioUtil::playSfx(data["EndSfx"].asCString(), mpCar->getModelPosition());

	// pfx
	if ( VuPfxEntity *pPfxEntity = VuPfxManager::IF()->getEntity(mLoopingPfx) )
		pPfxEntity->getSystemInstance()->stop(mpCar->isInGhostMode());

	VuPowerUpUtil::createPfx(mpCar, data["EndPfx"], true);

	// splat pfx
	if ( mpSplatPfx )
	{
		VuPfx::IF()->releaseSystemInstance(mpSplatPfx);
		mpSplatPfx = VUNULL;
	}

	// shatter
	if ( data.hasMember("ShatterData") )
		VuEntityRepository::IF()->addManagedEntity(new VuShatterEntity(data["ShatterData"], mpCar->getModelMatrix(), mpCar->getLinearVelocity(), mpCar->getDrawColor()));
}

//*****************************************************************************
void VuCarEffect::tick(float fdt)
{
	if ( mLoopingSfx.active() )
		mLoopingSfx.set3DAttributes(&mpCar->getModelPosition(), &mpCar->getLinearVelocity(), VUNULL);

	if ( mTimeRemaining < mSoftKillTime )
	{
		if ( VuPfxEntity *pPfxEntity = VuPfxManager::IF()->getEntity(mLoopingPfx) )
			pPfxEntity->getSystemInstance()->stop();
	}

	// splat pfx
	if ( mpSplatPfx )
	{
		int viewport = 0;
		mSplatPfxVisible = VuCarManager::IF()->getViewport(mpCar, viewport);

		const VuCamera &camera = VuViewportManager::IF()->getUiCamera();
		const VuRect &rect = VuViewportManager::IF()->getViewport(viewport).mUnsafeRect;
		VuVector2 screenPos = rect.getCenter();
		float dist = (mSplatPfxDist - camera.getNearPlane())/(camera.getFarPlane() - camera.getNearPlane());
		VuVector3 worldPos = camera.screenToWorld(VuVector3(screenPos.mX, screenPos.mY, dist));

		VuVector3 scale(rect.getHeight(), 1.0f, rect.getHeight());

		VuMatrix transform = camera.getTransform();
		transform.setTrans(worldPos);
		transform.scaleLocal(scale);

		mpSplatPfx->setMatrix(transform);
		mpSplatPfx->tick(fdt, true);

		if ( mTimeRemaining < mSoftKillTime )
			mpSplatPfx->stop();
	}

	onTick(fdt);
}

//*****************************************************************************
void VuCarEffect::draw(const VuGfxDrawParams &params)
{
	// splat pfx
	if ( mpSplatPfx && mSplatPfxVisible )
	{
		int fsl = VuGfxSort::IF()->getFullScreenLayer();
		int vp = VuGfxSort::IF()->getViewport();
		int rl = VuGfxSort::IF()->getReflectionLayer();
		int vpl = VuGfxSort::IF()->getViewportLayer();

		VuGfxSort::IF()->setFullScreenLayer(VuGfxSort::FSL_HUD);
		VuGfxSort::IF()->setViewport(0);
		VuGfxSort::IF()->setReflectionLayer(0);
		VuGfxSort::IF()->setViewportLayer(VuGfxSort::VPL_HUD);

		const VuCamera &camera = VuViewportManager::IF()->getUiCamera();
		mpSplatPfx->draw(VuGfxDrawParams(camera));

		VuGfxSort::IF()->setFullScreenLayer(fsl);
		VuGfxSort::IF()->setViewport(vp);
		VuGfxSort::IF()->setReflectionLayer(rl);
		VuGfxSort::IF()->setViewportLayer(vpl);
	}

	onDraw(params);
}
