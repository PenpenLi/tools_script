//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuLightningEntity
// 
//*****************************************************************************

#include "VuLightningEntity.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Gfx/VuGameGfxUtil.h"
#include "VuPurple/Dynamics/VuCollisionTypes.h"
#include "VuEngine/Entities/VuEntityRepository.h"
#include "VuEngine/Components/3dDraw/Vu3dDrawComponent.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Dynamics/Util/VuDynamicsRayTest.h"
#include "VuEngine/Json/VuJsonContainer.h"
#include "VuEngine/Util/VuDataUtil.h"


IMPLEMENT_RTTI(VuLightningEntity, VuEntity);


class VuLightningRayTestResult : public VuDynamicsRayTest::VuClosestResult
{
public:
	virtual bool needsCollision(VuRigidBody *pRigidBody)
	{
		if ( pRigidBody->getCollisionGroup() & (COL_ENGINE_STATIC_PROP|COL_ENGINE_DYNAMIC_PROP|COL_GAME_OBSTACLE) )
			return true;

		return false;
	}
};


//*****************************************************************************
VuLightningEntity::VuLightningEntity(const VuJsonContainer &data, VuCarEntity *pCar):
	mData(data),
	mpLauncher(pCar),
	mpTargetCar(VUNULL),
	mAge(0),
	mRandomSeed(0),
	mColor(255,255,255)
{
	// components
	addComponent(mp3dDrawComponent = new Vu3dDrawComponent(this, false));

	mp3dDrawComponent->setDrawMethod(this, &VuLightningEntity::draw);

	// more params
	mRange = data["Range"].asFloat();
	mCosTargetAcquisitionCone = VuCos(0.5f*VuDegreesToRadians(data["TargetAcquisitionCone"].asFloat()));
	mDisplacement = data["Displacement"].asFloat();
	mBoltCount = data["BoltCount"].asInt();
	mDetail = data["Detail"].asInt();
	mLifeTime = data["LifeTime"].asFloat();
	mWidth = data["Width"].asFloat();
	VuDataUtil::getValue(data["Color"], mColor);
}

//*****************************************************************************
void VuLightningEntity::onGameInitialize()
{
	// determine target car
	VuVector3 launcherPos = mpLauncher->getModelMatrix().transform(mpLauncher->getCollisionAabb().getCenter());
	float bestDist = mRange;
	for ( int i = 0; i < VuCarManager::IF()->getCarCount(); i++ )
	{
		VuCarEntity *pOtherCar = VuCarManager::IF()->getCar(i);
		if ( pOtherCar != mpLauncher )
		{
			if ( !pOtherCar->isGhostly() && !pOtherCar->hasCrashed() )
			{
				// check distance
				VuVector3 otherPos = pOtherCar->getModelMatrix().transform(pOtherCar->getCollisionAabb().getCenter());
				VuVector3 delta = otherPos - launcherPos;
				float dist = delta.normalize();
				if ( dist < bestDist )
				{
					// check target acquisition cone
					if ( VuDot(delta, mpLauncher->getModelMatrix().getAxisY()) > mCosTargetAcquisitionCone )
					{
						mpTargetCar = pOtherCar;
						bestDist = dist;
					}
				}
			}
		}
	}

	if ( mpTargetCar )
	{
		VuVector3 targetPos = mpTargetCar->getModelMatrix().transform(mpTargetCar->getCollisionAabb().getCenter());
		VuParams params;
		params.addVector3(targetPos);
		params.addVector3(VuVector3(0,0,0));
		params.addString(mData["CarEffect"].asCString());
		params.addEntity(mpLauncher);
		mpTargetCar->handleEvent("OnHitByMissile", params);
	}

	VuTickManager::IF()->registerHandler(this, &VuLightningEntity::tickDecision, "Decision");
	mp3dDrawComponent->show();
}

//*****************************************************************************
void VuLightningEntity::onGameRelease()
{
	mp3dDrawComponent->hide();
	VuTickManager::IF()->unregisterHandlers(this);
}

//*****************************************************************************
void VuLightningEntity::tickDecision(float fdt)
{
	mAge += fdt;

	// check for end conditions
	bool done = false;

	if ( mAge > mLifeTime )
		done = true;
	if ( mpLauncher->getDriver()->isRagdollActive() )
		done = true;
	if ( mpLauncher->isInGhostMode() )
		done = true;

	if ( done )
	{
		VuEntityRepository::IF()->removeManagedEntity(this);
		return;
	}

	// perform raycast
	mSrcPos = mpLauncher->getModelMatrix().transform(mpLauncher->getCollisionAabb().getCenter());

	if ( mpTargetCar )
		mDstPos = mpTargetCar->getModelMatrix().transform(mpTargetCar->getCollisionAabb().getCenter());
	else
		mDstPos = mSrcPos + mRange*mpLauncher->getModelMatrix().getAxisY();

	VuLightningRayTestResult rayTestResult;
	VuDynamicsRayTest::test(mSrcPos, mDstPos, rayTestResult);

	if ( rayTestResult.mbHasHit )
	{
		mDstPos = VuLerp(mSrcPos, mDstPos, rayTestResult.mHitFraction);

		const VuRigidBody *pRigidBody = rayTestResult.mpRigidBody;
		if ( VuEntity *pEntity = pRigidBody->getEntity() )
		{
			VuParams params;
			params.addVector3(mDstPos);
			params.addVector3(VuVector3(0,0,0));
			params.addString(mData["CarEffect"].asCString());
			params.addEntity(mpLauncher);
			pEntity->handleEvent("OnHitByMissile", params);
		}
	}

	// update aabb
	VuVector3 extents(mDisplacement);

	VuAabb aabb(mSrcPos);
	aabb.addPoint(mDstPos);
	aabb.mMin -= extents;
	aabb.mMax += extents;

	mp3dDrawComponent->updateVisibility(aabb);

	// update random number seed
	if ( fdt > 0.0f )
	{
		VUUINT64 perf = VuSys::IF()->getPerfCounter();
		mRandomSeed = (perf & 0xffffffff) + (perf >> 32);
	}
}

//*****************************************************************************
void VuLightningEntity::draw(const VuGfxDrawParams &params)
{
	VuGameGfxUtil::LightningParams lightningParams;

	lightningParams.mViewProjMat = params.mCamera.getViewProjMatrix();
	lightningParams.mEyePosition = params.mCamera.getEyePosition();
	lightningParams.mSrcPos = mSrcPos;
	lightningParams.mDstPos = mDstPos;
	lightningParams.mDisplacement = mDisplacement;
	lightningParams.mBoltCount = mBoltCount;
	lightningParams.mDetail = mDetail;
	lightningParams.mWidth = mWidth;
	lightningParams.mColor = mColor;
	lightningParams.mRandomSeed = mRandomSeed;

	VuGameGfxUtil::IF()->drawLightning(lightningParams);
}