//*****************************************************************************
//
//  Copyright (c) 2010-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  ForceAffector entity
// 
//*****************************************************************************

#include "VuPurple/Dynamics/VuCollisionTypes.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/Transform/VuTransformComponent.h"
#include "VuEngine/Components/3dLayout/Vu3dLayoutComponent.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Properties/VuBasicProperty.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/Camera/VuCamera.h"
#include "VuEngine/Dynamics/VuDynamics.h"
#include "VuEngine/Dev/VuDev.h"
#include "VuEngine/Dev/VuDevMenu.h"
#include "VuEngine/Dev/VuDevUtil.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"


class VuForceAffectorEntity : public VuEntity, VuDynamicsStepCallback
{
	DECLARE_RTTI

public:
	VuForceAffectorEntity();

	virtual void		onGameInitialize();
	virtual void		onGameRelease();

protected:
	// scripting
	VuRetVal			Activate(const VuParams &params);
	VuRetVal			Deactivate(const VuParams &params);

	// VuDynamicsStepCallback
	virtual void		onDynamicsApplyForces(float fdt) = 0;

	void				activate();
	void				deactivate();

	void				transformModified();

	void				tickBuild(float fdt);

	virtual void		drawLayout(const Vu3dLayoutDrawParams &params) = 0;
	virtual void		drawDebug() = 0;

	// components
	Vu3dLayoutComponent			*mp3dLayoutComponent;
	VuScriptComponent			*mpScriptComponent;

	// properties
	bool						mbInitiallyActive;
	float						mAcceleration;
	float						mMaxSpeed;

	btBoxShape					mCollisionShape;
	btPairCachingGhostObject	mGhostObject;
	bool						mbActive;
};


IMPLEMENT_RTTI(VuForceAffectorEntity, VuEntity);


// constants

#ifndef VURETAIL
	#define DEBUG_DRAWING_ENABLED 1
#endif

#ifdef DEBUG_DRAWING_ENABLED
	static bool sbDebugDraw = false;
#endif


//*****************************************************************************
VuForceAffectorEntity::VuForceAffectorEntity():
	mbInitiallyActive(true),
	mAcceleration(5),
	mMaxSpeed(20.0f),
	mCollisionShape(btVector3(1.0f, 1.0f, 1.0f)),
	mbActive(false)
{
#if DEBUG_DRAWING_ENABLED
	static VuDevBoolOnce sOnce;
	if(sOnce && VuDevMenu::IF())
	{
		VuDevMenu::IF()->addBool("Game/DrawForceAffectors", sbDebugDraw);
	}
#endif

	// components
	addComponent(mp3dLayoutComponent = new Vu3dLayoutComponent(this));
	addComponent(mpScriptComponent = new VuScriptComponent(this, 150, false));

	mpTransformComponent->setWatcher(&VuForceAffectorEntity::transformModified);
	mp3dLayoutComponent->setDrawMethod(this, &VuForceAffectorEntity::drawLayout);

	// properties
	addProperty(new VuBoolProperty("Initially Active", mbInitiallyActive));
	addProperty(new VuFloatProperty("Acceleration", mAcceleration));
	addProperty(new VuFloatProperty("Max Speed", mMaxSpeed));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuForceAffectorEntity, Activate);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuForceAffectorEntity, Deactivate);

	mGhostObject.setCollisionShape(&mCollisionShape);
	mGhostObject.setCollisionFlags(mGhostObject.getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
}

//*****************************************************************************
void VuForceAffectorEntity::onGameInitialize()
{
	// register phased ticks
#if DEBUG_DRAWING_ENABLED
	VuTickManager::IF()->registerHandler(this, &VuForceAffectorEntity::tickBuild, "Build");
#endif

	if ( mbInitiallyActive )
		Activate(VuParams());
}

//*****************************************************************************
void VuForceAffectorEntity::onGameRelease()
{
	// unregister phased tick
#if DEBUG_DRAWING_ENABLED
	VuTickManager::IF()->unregisterHandlers(this);
#endif

	deactivate();
}

//*****************************************************************************
VuRetVal VuForceAffectorEntity::Activate(const VuParams &params)
{
	activate();

	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuForceAffectorEntity::Deactivate(const VuParams &params)
{
	deactivate();

	return VuRetVal();
}

//*****************************************************************************
void VuForceAffectorEntity::activate()
{
	if ( !mbActive )
	{
		mCollisionShape.setLocalScaling(VuDynamicsUtil::toBtVector3(mpTransformComponent->getWorldScale()));
		mGhostObject.setWorldTransform(VuDynamicsUtil::toBtTransform(mpTransformComponent->getWorldTransform()));

		VuDynamics::IF()->getDynamicsWorld()->addCollisionObject(&mGhostObject, COL_GAME_FORCE_AFFECTOR, COL_ENGINE_DYNAMIC_PROP|COL_GAME_CAR);

		// register dynamics methods
		VuDynamics::IF()->registerStepCallback(this);

		mbActive = true;
	}
}

//*****************************************************************************
void VuForceAffectorEntity::deactivate()
{
	if ( mbActive )
	{
		VuDynamics::IF()->getDynamicsWorld()->removeCollisionObject(&mGhostObject);

		// unregister dynamics methods
		VuDynamics::IF()->unregisterStepCallback(this);

		mbActive = false;
	}
}

//*****************************************************************************
void VuForceAffectorEntity::transformModified()
{
	if ( mbActive )
	{
		mGhostObject.setWorldTransform(VuDynamicsUtil::toBtTransform(mpTransformComponent->getWorldTransform()));
		mCollisionShape.setLocalScaling(VuDynamicsUtil::toBtVector3(mpTransformComponent->getWorldScale()));
		VuDynamics::IF()->getDynamicsWorld()->updateSingleAabb(&mGhostObject);
	}
}

//*****************************************************************************
void VuForceAffectorEntity::tickBuild(float fdt)
{
#if DEBUG_DRAWING_ENABLED
	if ( mbActive && sbDebugDraw )
		drawDebug();
#endif
}


//*****************************************************************************
// VuRectangularDirectionalForceAffectorEntity
//*****************************************************************************

class VuRectangularDirectionalForceAffectorEntity : public VuForceAffectorEntity
{
	DECLARE_RTTI

public:
	VuRectangularDirectionalForceAffectorEntity();

private:
	// VuDynamicsStepCallback
	virtual void		onDynamicsApplyForces(float fdt);

	virtual void		drawLayout(const Vu3dLayoutDrawParams &params);
	virtual void		drawDebug();
};

IMPLEMENT_RTTI(VuRectangularDirectionalForceAffectorEntity, VuForceAffectorEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuRectangularDirectionalForceAffectorEntity);


//*****************************************************************************
VuRectangularDirectionalForceAffectorEntity::VuRectangularDirectionalForceAffectorEntity()
{
}

//*****************************************************************************
void VuRectangularDirectionalForceAffectorEntity::onDynamicsApplyForces(float fdt)
{
	if ( mGhostObject.getNumOverlappingObjects() == 0 )
		return;

	VuMatrix transform = VuDynamicsUtil::toVuMatrix(mGhostObject.getWorldTransform());

	VuMatrix invMat = transform;
	invMat.scaleLocal(VuDynamicsUtil::toVuVector3(mCollisionShape.getLocalScaling()));
	invMat.invert();

	int count = mGhostObject.getNumOverlappingObjects();
	for ( int i = 0; i < count; i++ )
	{
		btCollisionObject *pColObj = mGhostObject.getOverlappingObject(i);
		if ( pColObj->getInternalType() == btCollisionObject::CO_RIGID_BODY )
		{
			VuRigidBody *pRigidBody = static_cast<VuRigidBody *>(pColObj);

			// transform center of mass position into local space
			VuVector3 vPos = pRigidBody->getVuCenterOfMassPosition();
			vPos = invMat.transform(vPos);

			// test against box in local space
			if ( VuAbs(vPos.mX) <= 1.0f && VuAbs(vPos.mY) <= 1.0f && VuAbs(vPos.mZ) <= 1.0f )
			{
				// test speed
				if ( VuDot(transform.getAxisY(), pRigidBody->getVuLinearVelocity()) < mMaxSpeed )
				{
					// calculate force
					float acceleration = -mAcceleration*pRigidBody->getGravity().getZ();
					float force = acceleration/pRigidBody->getInvMass();
					VuVector3 vForce = force*transform.getAxisY();
					pRigidBody->applyCentralForce(VuDynamicsUtil::toBtVector3(vForce));
				}
			}
		}
	}
}

//*****************************************************************************
void VuRectangularDirectionalForceAffectorEntity::drawLayout(const Vu3dLayoutDrawParams &params)
{
	if ( params.mbSelected )
	{
		VuMatrix mat = mpTransformComponent->getWorldTransform();
		mat.scaleLocal(mpTransformComponent->getWorldScale());

		// draw arrow
		VuGfxUtil::IF()->drawArrowLines(VuColor(255, 128, 128), 1.0f, 0.25f, 0.25f, mat*params.mCamera.getViewProjMatrix());
		mat.rotateYLocal(VuDegreesToRadians(-90.0f));
		VuGfxUtil::IF()->drawArrowLines(VuColor(255, 128, 128), 1.0f, 0.25f, 0.25f, mat*params.mCamera.getViewProjMatrix());
	}
}

//*****************************************************************************
void VuRectangularDirectionalForceAffectorEntity::drawDebug()
{
	VuMatrix mat = mpTransformComponent->getWorldTransform();
	mat.scaleLocal(mpTransformComponent->getWorldScale());

	// draw arrow
	VuDev::IF()->drawArrow(VuColor(255, 128, 128), 1.0f, 0.25f, 0.25f, mat);
	mat.rotateYLocal(VuDegreesToRadians(-90.0f));
	VuDev::IF()->drawArrow(VuColor(255, 128, 128), 1.0f, 0.25f, 0.25f, mat);
}
