//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuBreadCrumbEntity
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Dynamics/VuDynamicsCallbacks.h"
#include "VuEngine/Math/VuMatrix.h"

class VuCarEntity;
class VuPfxSystemInstance;
class Vu3dDrawComponent;
class VuGfxDrawParams;
class btCollisionShape;


class VuBreadCrumbEntity : public VuEntity, public VuRigidBodyContactCallback
{
	DECLARE_RTTI

public:
	VuBreadCrumbEntity(const VuJsonContainer &gameData, const VuJsonContainer &eventData, const VuMatrix &transform, int index);
	~VuBreadCrumbEntity();

	virtual void	onGameInitialize();
	virtual void	onGameRelease();

protected:
	// VuRigidBodyContactCallback
	virtual bool	onRigidBodyContactAdded(VuContactPoint &cp);

	void			tickDecision(float fdt);
	void			tickBuild(float fdt);

	void			draw(const VuGfxDrawParams &params);

	Vu3dDrawComponent		*mp3dDrawComponent;

	const VuJsonContainer	&mGameData;
	const VuJsonContainer	&mEventData;
	VuMatrix				mTransform;
	int						mIndex;
	VuPfxSystemInstance		*mpStaticPfxSystem;
	btCollisionShape		*mpShape;
	btCollisionShape		*mpChildShape;
	VuRigidBody				*mpRigidBody;

	float					mLifeTime;
	float					mSoftKillTime;
	float					mDrawDistance;
	VuVector3				mCollisionOffset;
	float					mAngularFrequency;
	float					mDampingRatio;

	float					mAge;
	bool					mBreakNow;
	VuVector3				mCollisionPos;
	VuVector3				mCollisionVel;
	VuCarEntity				*mpBreakerEntity;
	float					mScale;
	float					mScaleVelocity;
	bool					mSpringActive;
};
