//*****************************************************************************
//
//  Copyright (c) 2013-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Spring Obstacle class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Dynamics/VuDynamics.h"
#include "VuEngine/Gfx/Model/VuAnimatedModelInstance.h"

class VuCarEntity;
class VuPowerUp;
class VuMatrix;
class Vu3dDrawComponent;
class VuSpringRigidBody;
class VuGfxDrawParams;
class VuGfxDrawShadowParams;
class VuAnimationControl;


class VuSpringObstacleEntity : public VuEntity, public VuRigidBodyContactCallback
{
	DECLARE_RTTI

public:
	VuSpringObstacleEntity(const VuJsonContainer &data, VuCarEntity *pCar, const VuMatrix &transform);
	~VuSpringObstacleEntity();

	virtual void	onGameInitialize();
	virtual void	onGameRelease();

protected:
	// VuRigidBodyContactCallback
	virtual bool	onRigidBodyContactAdded(VuContactPoint &cp);

	void			addToWorld();
	void			removeFromWorld();

	void			tickDecision(float fdt);
	void			tickBuild(float fdt);

	void			draw(const VuGfxDrawParams &params);
	void			drawShadow(const VuGfxDrawShadowParams &params);

	Vu3dDrawComponent			*mp3dDrawComponent;

	VuCarEntity					*mpLauncher;
	VuMatrix					mTransform;
	VuAnimatedModelInstance		mAnimatedModelInstance;
	VuAnimatedSkeleton			*mpAnimatedSkeleton;
	VuAnimationControl			*mpAnimationControl;
	btCollisionShape			*mpShape;
	btCollisionShape			*mpChildShape;
	VuSpringRigidBody			*mpRigidBody;
	VuColor						mColor;
	float						mDrawDistance;
	std::string					mCarEffect;
	std::string					mSpringSfx;
	float						mFadeStartTime;
	float						mFadeDuration;
	bool						mAddedToWorld;
	float						mAge;
	bool						mSpringNow;
	float						mSprungTimer;
	VuWeakRef<VuCarEntity>		mBreakerEntity;
	float						mAlpha;

	enum eState { STATE_WHOLE, STATE_ANIMATING, STATE_DONE };
	eState						mState;
};
