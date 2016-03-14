//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuBreadCrumbEntity
// 
//*****************************************************************************

#include "VuBreadCrumbEntity.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Dynamics/VuCollisionTypes.h"
#include "VuPurple/Util/VuCollisionUtil.h"
#include "VuEngine/Entities/VuEntityRepository.h"
#include "VuEngine/Components/3dDraw/Vu3dDrawComponent.h"
#include "VuEngine/Components/Transform/VuTransformComponent.h"
#include "VuEngine/Dynamics/VuDynamics.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Pfx/VuPfx.h"
#include "VuEngine/Pfx/VuPfxManager.h"
#include "VuEngine/Pfx/VuPfxEntity.h"
#include "VuEngine/HAL/Audio/VuAudio.h"
#include "VuEngine/Services/VuServiceManager.h"
#include "VuEngine/Services/VuPfxAttachService.h"
#include "VuEngine/Math/VuMathUtil.h"
#include "VuEngine/Json/VuJsonContainer.h"
#include "VuEngine/Util/VuDataUtil.h"


IMPLEMENT_RTTI(VuBreadCrumbEntity, VuEntity);


//*****************************************************************************
VuBreadCrumbEntity::VuBreadCrumbEntity(const VuJsonContainer &gameData, const VuJsonContainer &eventData, const VuMatrix &transform, int index):
	mGameData(gameData),
	mEventData(eventData),
	mTransform(transform),
	mIndex(index),
	mpStaticPfxSystem(VUNULL),
	mCollisionOffset(0,0,0),
	mAngularFrequency(2.0f),
	mDampingRatio(0.25f),
	mAge(0.0f),
	mBreakNow(false),
	mCollisionVel(0,0,0),
	mScale(0.0f),
	mScaleVelocity(0.0f),
	mSpringActive(true)
{
	// components
	addComponent(mp3dDrawComponent = new Vu3dDrawComponent(this));

	mp3dDrawComponent->setDrawMethod(this, &VuBreadCrumbEntity::draw);

	// collision shape
	VuCollisionUtil::createCollisionShape(gameData, mpShape, mpChildShape);

	// rigid body
	btRigidBody::btRigidBodyConstructionInfo info(0.0f, VUNULL, mpShape);
	info.m_startWorldTransform = VuDynamicsUtil::toBtTransform(mTransform);
	mpRigidBody = new VuRigidBody(info, this, COL_GAME_POWERUP, COL_GAME_CAR);
	mpRigidBody->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	mpRigidBody->setContactCallback(this);

	// more params
	mLifeTime = gameData["LifeTime"].asFloat();
	mSoftKillTime = gameData["SoftKillTime"].asFloat();
	mDrawDistance = gameData["DrawDistance"].asFloat();
	VuDataUtil::getValue(gameData["CollisionOffset"], mCollisionOffset);
	VuDataUtil::getValue(gameData["AngularFrequency"], mAngularFrequency);
	VuDataUtil::getValue(gameData["DampingRatio"], mDampingRatio);
}

//****************************************************************************
VuBreadCrumbEntity::~VuBreadCrumbEntity()
{
	delete mpShape;
	delete mpChildShape;
	delete mpRigidBody;
}

//*****************************************************************************
void VuBreadCrumbEntity::onGameInitialize()
{
	mpTransformComponent->setWorldTransform(mTransform);

	if ( (mpStaticPfxSystem = VuPfx::IF()->createSystemInstance(mGameData["StaticPfx"].asCString())) )
	{
		mpStaticPfxSystem->setMatrix(mpTransformComponent->getWorldTransform());
		mpStaticPfxSystem->start();

		VuDynamics::IF()->addRigidBody(mpRigidBody);

		mp3dDrawComponent->show();

		VuTickManager::IF()->registerHandler(this, &VuBreadCrumbEntity::tickDecision, "Decision");
		VuTickManager::IF()->registerHandler(this, &VuBreadCrumbEntity::tickBuild, "Build");
	}
}

//*****************************************************************************
void VuBreadCrumbEntity::onGameRelease()
{
	if ( mpStaticPfxSystem )
	{
		VuPfx::IF()->releaseSystemInstance(mpStaticPfxSystem);
		mpStaticPfxSystem = VUNULL;

		VuDynamics::IF()->removeRigidBody(mpRigidBody);

		mp3dDrawComponent->hide();

		VuTickManager::IF()->unregisterHandlers(this);
	}
}

//*****************************************************************************
bool VuBreadCrumbEntity::onRigidBodyContactAdded(VuContactPoint &cp)
{
	// check if we should break
	if ( !mBreakNow )
	{
		mCollisionPos = cp.mPosWorld;
		mCollisionVel = cp.mpOtherBody->getVuLinearVelocity();
		mpBreakerEntity = static_cast<VuCarEntity *>(cp.mpOtherBody->getEntity());
		mBreakNow = true;
	}

	return false;
}

//*****************************************************************************
void VuBreadCrumbEntity::tickDecision(float fdt)
{
	mAge += fdt;

	if ( mAge > mLifeTime - mSoftKillTime )
		mpStaticPfxSystem->stop();

	if ( mBreakNow )
	{
		// pfx
		if ( VUUINT32 hPfx = VuPfxManager::IF()->createEntity(mGameData["BreakPfx"].asCString(), true) )
		{
			if ( VuPfxEntity *pPfxEntity = VuPfxManager::IF()->getEntity(hPfx) )
			{
				if ( VuPfxAttachService *pService = VuServiceManager::IF()->createPfxService<VuPfxAttachService>() )
					pService->init(mpBreakerEntity, hPfx, VuMatrix::identity());

				pPfxEntity->getSystemInstance()->start();
			}
		}

		// sfx
		#if !VU_DISABLE_AUDIO
		FMOD::Event *pEvent;
		if ( VuAudio::IF()->eventSystem()->getEvent(mGameData["BreakSfx"].asCString(), FMOD_EVENT_NONBLOCKING, &pEvent) == FMOD_OK )
		{
			FMOD_VECTOR pos = VuAudio::toFmodVector(mpTransformComponent->getWorldPosition());
			pEvent->set3DAttributes(&pos, VUNULL, VUNULL);
			pEvent->start();
		}
		#endif

		// scoring
		if ( !mpBreakerEntity->getHasFinished() )
			mpBreakerEntity->getStats().mScore++;

		mBreakNow = false;
		mpBreakerEntity = VUNULL;

		VuEntityRepository::IF()->removeManagedEntity(this);

		VuEventManager::IF()->broadcast("OnBreadCrumb");
	}
	else if ( mAge > mLifeTime )
	{
		VuEntityRepository::IF()->removeManagedEntity(this);
	}
}

//*****************************************************************************
void VuBreadCrumbEntity::tickBuild(float fdt)
{
	// update scale
	if ( mSpringActive )
	{
		VuMathUtil::calcDampedSimpleHarmonicMotion(&mScale, &mScaleVelocity, 1.0f, fdt, mAngularFrequency*VU_2PI, mDampingRatio);
		if ( VuAbs(mScale - 1.0f) < 0.01f && VuAbs(mScaleVelocity) < 0.01f )
		{
			mSpringActive = false;
			mScale = 1.0f;
			mScaleVelocity = 0.0f;
		}

		mpStaticPfxSystem->setScale(mScale);
	}

	mpStaticPfxSystem->tick(fdt, false);

	if ( mpStaticPfxSystem->getAabb().isValid() )
		mp3dDrawComponent->updateVisibility(mpStaticPfxSystem->getAabb());
}

//*****************************************************************************
void VuBreadCrumbEntity::draw(const VuGfxDrawParams &params)
{
	if ( params.mbDrawReflection )
		return;

	float distSquared = (mTransform.getTrans() - params.mEyePos).magSquared();
	if ( distSquared > VuSquare(mDrawDistance) )
		return;

	if ( mpStaticPfxSystem )
		mpStaticPfxSystem->draw(params);
}
