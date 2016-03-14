//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuPlantEntity
// 
//*****************************************************************************

#include "VuPlantEntity.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarEffectController.h"
#include "VuPurple/Dynamics/VuCollisionTypes.h"
#include "VuEngine/Json/VuJsonContainer.h"
#include "VuEngine/Components/Transform/VuTransformComponent.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Entities/VuEntityRepository.h"
#include "VuEngine/Pfx/VuPfxManager.h"
#include "VuEngine/Pfx/VuPfxEntity.h"
#include "VuEngine/Pfx/VuPfx.h"


IMPLEMENT_RTTI(VuPlantEntity, VuEntity);


class VuPlantRigidBody : public VuRigidBody
{
public:
	VuPlantRigidBody(const btRigidBody::btRigidBodyConstructionInfo &info, VuEntity *pEntity, VUINT16 collisionGroup, VUINT16 collisionMask):
		VuRigidBody(info, pEntity, collisionGroup, collisionMask),
		mpIgnoreBody(VUNULL)
	{
		m_checkCollideWith = true;
	}

	virtual bool checkCollideWithOverride(const  btCollisionObject* co) const
	{
		const VuRigidBody *pOtherBody = static_cast<const VuRigidBody *>(co);
		if ( pOtherBody == mpIgnoreBody )
			return false;

		return true;
	}

	VuRigidBody	*mpIgnoreBody;
};


//*****************************************************************************
VuPlantEntity::VuPlantEntity(VuCarEntity *pCar, const VuJsonContainer &data, const VuMatrix &transform):
	mpLauncher(pCar),
	mTransform(transform),
	mAge(0.0f)
{
	// params
	float radius = data["Radius"].asFloat();
	mLifeTime = data["LifeTime"].asFloat();
	mFadeTime = data["FadeTime"].asFloat();
	mCarEffect = data["CarEffect"].asString();

	// shape
	mpShape = new btSphereShape(radius);

	// rigid body
	VuMatrix rigidBodyTransform = mTransform;
	rigidBodyTransform.translateLocal(VuVector3(0,0,radius));
	btRigidBody::btRigidBodyConstructionInfo info(0.0f, VUNULL, mpShape);
	info.m_startWorldTransform = VuDynamicsUtil::toBtTransform(rigidBodyTransform);
	mpRigidBody = new VuPlantRigidBody(info, this, COL_GAME_PLANT, COL_GAME_CAR);
	mpRigidBody->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	mpRigidBody->setContactCallback(this);
	mpRigidBody->mpIgnoreBody = pCar->getRigidBody();

	// pfx
	if ( (mPlantPfx = VuPfxManager::IF()->createEntity(data["ItemPfx"].asCString(), false)) )
	{
		if ( VuPfxEntity *pPfxEntity = VuPfxManager::IF()->getEntity(mPlantPfx) )
		{
			pPfxEntity->getSystemInstance()->setMatrix(mTransform);
			pPfxEntity->getSystemInstance()->start();
		}
	}
}

//*****************************************************************************
VuPlantEntity::~VuPlantEntity()
{
	delete mpRigidBody;
	delete mpShape;
}

//*****************************************************************************
void VuPlantEntity::onGameInitialize()
{
	mpTransformComponent->setWorldTransform(mTransform);

	VuDynamics::IF()->addRigidBody(mpRigidBody);

	// register phased ticks
	VuTickManager::IF()->registerHandler(this, &VuPlantEntity::tickDecision, "Decision");
}

//*****************************************************************************
void VuPlantEntity::onGameRelease()
{
	VuDynamics::IF()->removeRigidBody(mpRigidBody);

	// unregister phased tick
	VuTickManager::IF()->unregisterHandler(this, "Decision");
}

//*****************************************************************************
bool VuPlantEntity::onRigidBodyContactAdded(VuContactPoint &cp)
{
	if ( VuEntity *pEntity = cp.mpOtherBody->getEntity() )
	{
		if ( pEntity->isDerivedFrom(VuCarEntity::msRTTI) )
		{
			VuCarEntity *pCar = static_cast<VuCarEntity *>(cp.mpOtherBody->getEntity());
			mVictimEntity = pCar;
		}
	}

	return false;
}

//*****************************************************************************
void VuPlantEntity::tickDecision(float fdt)
{
	if ( mVictimEntity )
	{
		mVictimEntity->getEffectController()->applyEffect(mCarEffect.c_str(), mpLauncher);
		mVictimEntity = VUNULL;
	}

	mAge += fdt;
	if ( mAge > mLifeTime - mFadeTime )
	{
		if ( VuPfxEntity *pPfxEntity = VuPfxManager::IF()->getEntity(mPlantPfx) )
			pPfxEntity->getSystemInstance()->stop();

		if ( mAge > mLifeTime )
			VuEntityRepository::IF()->removeManagedEntity(this);
	}
}
