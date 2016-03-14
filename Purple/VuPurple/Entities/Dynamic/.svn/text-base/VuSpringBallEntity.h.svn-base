//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Spring Ball class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Dynamics/VuDynamics.h"
#include "VuEngine/Gfx/Model/VuStaticModelInstance.h"

class VuCarEntity;
class VuPowerUp;
class VuMatrix;
class Vu3dDrawComponent;
class VuSpringBallRigidBody;
class VuGfxDrawParams;
class VuGfxDrawShadowParams;


class VuSpringBallEntity : public VuEntity, public VuRigidBodyContactCallback, btMotionState
{
	DECLARE_RTTI

public:
	VuSpringBallEntity(const VuJsonContainer &data, VuCarEntity *pCar, const VuMatrix &transform, const VuVector3 &launchVel);

	virtual void	onGameInitialize();
	virtual void	onGameRelease();

protected:
	// btMotionState
	virtual void	getWorldTransform(btTransform& worldTrans) const;
	virtual void	setWorldTransform(const btTransform& worldTrans);

	// VuRigidBodyContactCallback
	virtual bool	onRigidBodyContactAdded(VuContactPoint &cp);

	void			tickDecision(float fdt);

	void			draw(const VuGfxDrawParams &params);
	void			drawShadow(const VuGfxDrawShadowParams &params);

	Vu3dDrawComponent			*mp3dDrawComponent;

	const VuJsonContainer		&mData;
	VuCarEntity					*mpLauncher;
	VuMatrix					mLaunchTransform;
	VuVector3					mLaunchVelocity;
	VuStaticModelInstance		mModelInstance;
	btCollisionShape			*mpShape;
	VuSpringBallRigidBody		*mpRigidBody;
	float						mRadius;
	float						mMass;
	float						mLinearDamping;
	float						mDrawDistance;
	std::string					mCarEffect;
	float						mSelfCollisionTime;
	float						mLifeTime;
	float						mAge;
	bool						mSpringNow;
	VuWeakRef<VuCarEntity>		mBreakerEntity;
};
