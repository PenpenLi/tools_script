//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  GameWhirlpool entity
// 
//*****************************************************************************

#include "VuPurple/Dynamics/VuCollisionTypes.h"
#include "VuEngine/Entities/Water/VuWhirlpoolWaveEntity.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/Transform/VuTransformComponent.h"
#include "VuEngine/Components/3dLayout/Vu3dLayoutComponent.h"
#include "VuEngine/Properties/VuBasicProperty.h"
#include "VuEngine/Dynamics/VuDynamics.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"


class VuGameWhirlpoolEntity : public VuWhirlpoolWaveEntity, VuDynamicsStepCallback
{
	DECLARE_RTTI

public:
	VuGameWhirlpoolEntity();

	virtual void			onGameInitialize();
	virtual void			onGameRelease();

private:
	virtual void			modified();

	// VuDynamicsStepCallback
	virtual void			onDynamicsApplyForces(float fdt);

	float					mHeight;
	float					mAcceleration;

	// force affector
	btBoxShape					mCollisionShape;
	btPairCachingGhostObject	mGhostObject;
};


IMPLEMENT_RTTI(VuGameWhirlpoolEntity, VuWhirlpoolWaveEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuGameWhirlpoolEntity);


//*****************************************************************************
VuGameWhirlpoolEntity::VuGameWhirlpoolEntity():
	mHeight(10),
	mAcceleration(10.0f),
	mCollisionShape(btVector3(1.0f, 1.0f, 1.0f))
{
	// properties
	addProperty(new VuFloatProperty("Height", mHeight))					->	setWatcher(this, &VuGameWhirlpoolEntity::modified);
	addProperty(new VuFloatProperty("Acceleration", mAcceleration));

	mGhostObject.setCollisionShape(&mCollisionShape);
	mGhostObject.setCollisionFlags(mGhostObject.getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

	modified();
}

//*****************************************************************************
void VuGameWhirlpoolEntity::onGameInitialize()
{
	VuWhirlpoolWaveEntity::onGameInitialize();

	VuDynamics::IF()->getDynamicsWorld()->addCollisionObject(&mGhostObject, COL_GAME_FORCE_AFFECTOR, COL_ENGINE_DYNAMIC_PROP|COL_GAME_CAR);

	// register dynamics methods
	VuDynamics::IF()->registerStepCallback(this);
}

//*****************************************************************************
void VuGameWhirlpoolEntity::onGameRelease()
{
	VuWhirlpoolWaveEntity::onGameRelease();

	VuDynamics::IF()->getDynamicsWorld()->removeCollisionObject(&mGhostObject);

	// unregister dynamics methods
	VuDynamics::IF()->unregisterStepCallback(this);
}

//*****************************************************************************
void VuGameWhirlpoolEntity::modified()
{
	VuWhirlpoolWaveEntity::modified();

	mCollisionShape.setLocalScaling(btVector3(mOuterRadius, mOuterRadius, 0.5f*(mHeight + mDepth)));
	VuMatrix transform = mpTransformComponent->getWorldTransform();
	transform.translateLocal(VuVector3(0.0f, 0.0f, 0.5f*(mHeight - mDepth)));
	mGhostObject.setWorldTransform(VuDynamicsUtil::toBtTransform(transform));

	mp3dLayoutComponent->setLocalBounds(VuAabb(VuVector3(-mOuterRadius, -mOuterRadius, -mDepth), VuVector3(mOuterRadius, mOuterRadius, mHeight)));
}

//*****************************************************************************
void VuGameWhirlpoolEntity::onDynamicsApplyForces(float fdt)
{
	int count = mGhostObject.getNumOverlappingObjects();
	for ( int i = 0; i < count; i++ )
	{
		btCollisionObject *pColObj = mGhostObject.getOverlappingObject(i);
		if ( pColObj->getInternalType() == btCollisionObject::CO_RIGID_BODY )
		{
			VuRigidBody *pRigidBody = static_cast<VuRigidBody *>(pColObj);

			// calculate relative position
			VuVector3 pos = pRigidBody->getVuCenterOfMassPosition() - mpTransformComponent->getWorldPosition();

			// test against bounds
			float distXY = pos.mag2d();
			if ( distXY > mInnerRadius && distXY < mOuterRadius && pos.mZ > -mDepth && pos.mZ < mHeight )
			{
				VuVector2 normalizedPos = VuVector2(pos.mX, pos.mY)/distXY;

				// angular component
				VuVector3 angVel = mAngularSpeed*VuVector3(-normalizedPos.mY, normalizedPos.mX, 0);

				// linear component
				float u = (distXY - mInnerRadius)/(mOuterRadius - mInnerRadius);
				float du_ddist = 1/(mOuterRadius - mInnerRadius);
				float magnitude = VuSqrt(u);
				float dmagnitude_ddist = 0.5f*du_ddist/magnitude;
				float dheight_ddist = dmagnitude_ddist*mDepth;
				VuVector3 linComp = VuVector3(-normalizedPos.mX, -normalizedPos.mY, -dheight_ddist).normal();
				VuVector3 linVel = mLinearSpeed*linComp;

				float vel_magnitude = 1 - magnitude;
				VuVector3 vel = vel_magnitude*(angVel + linVel);

				VuVector3 normalizedVel = vel;
				float speed = normalizedVel.normalize();

				// test speed
				if ( VuDot(vel.normal(), pRigidBody->getVuLinearVelocity()) < speed )
				{
					// calculate force
					float fForce = mAcceleration/pRigidBody->getInvMass();
					VuVector3 vForce = fForce*normalizedVel;
					pRigidBody->applyCentralForce(VuDynamicsUtil::toBtVector3(vForce));
				}
			}
		}
	}
}
