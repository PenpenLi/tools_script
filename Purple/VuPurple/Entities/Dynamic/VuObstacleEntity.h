//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Obstacle class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Dynamics/VuDynamics.h"
#include "VuEngine/Gfx/Model/VuStaticModelInstance.h"
#include "VuEngine/Gfx/Model/VuBreakableModelInstance.h"

class VuCarEntity;
class VuMatrix;
class Vu3dDrawComponent;
class VuObstacleRigidBody;
class VuGfxDrawParams;
class VuGfxDrawShadowParams;


class VuObstacleEntity : public VuEntity, public VuRigidBodyContactCallback
{
	DECLARE_RTTI

public:
	VuObstacleEntity(const VuJsonContainer &data, VuCarEntity *pCar, const VuMatrix &transform);
	~VuObstacleEntity();

	virtual void	onGameInitialize();
	virtual void	onGameRelease();

protected:
	// event handlers
	void			OnExplosion(const VuParams &params);
	void			OnHitByMissile(const VuParams &params);

	// VuRigidBodyContactCallback
	virtual bool	onRigidBodyContactAdded(VuContactPoint &cp);

	void			addToWorld();
	void			removeFromWorld();

	void			tickDecision(float fdt);

	void			draw(const VuGfxDrawParams &params);
	void			drawShadow(const VuGfxDrawShadowParams &params);

	Vu3dDrawComponent			*mp3dDrawComponent;

	const VuJsonContainer		&mData;
	VuCarEntity					*mpLauncher;
	VuMatrix					mTransform;
	VuStaticModelInstance		mStaticModelInstance;
	VuBreakableModelInstance	mBreakableModelInstance;
	btCollisionShape			*mpShape;
	btCollisionShape			*mpChildShape;
	VuObstacleRigidBody			*mpRigidBody;
	VuColor						mColor;
	bool						mAddedToWorld;
	std::string					mBreakPfx;
	std::string					mBreakSfx;
	float						mFadeDelay;
	float						mFadeTime;
	float						mDrawDistance;
	float						mSpeedPenalty;
	float						mCamShakeMagnitude;
	float						mCamShakeDuration;
	float						mCamShakeFalloffTime;
	float						mCamShakeFrequency;
	float						mAge;
	bool						mBreakNow;
	float						mBreakTimer;
	VuVector3					mCollisionVel;
	VuWeakRef<VuEntity>			mBreakerEntity;

	enum eState { STATE_WHOLE, STATE_BREAKING, STATE_DONE };
	eState						mState;
};
