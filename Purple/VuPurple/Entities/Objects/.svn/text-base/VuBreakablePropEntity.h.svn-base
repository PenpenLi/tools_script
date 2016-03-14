//*****************************************************************************
//
//  Copyright (c) 2009-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Breakable Prop entity
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Entities/Objects/VuPropEntity.h"
#include "VuEngine/Dynamics/VuDynamicsCallbacks.h"

class Vu3dDrawBreakableModelComponent;
class VuDBEntryProperty;


class VuBreakablePropEntity : public VuPropEntity, public VuRigidBodyContactCallback
{
	DECLARE_RTTI
public:
	VuBreakablePropEntity();

	virtual void			onPostLoad();
	virtual void			onGameInitialize();
	virtual void			onGameRelease();

protected:
	void					typeModified();

	// event handlers
	void					OnExplosion(const VuParams &params);
	void					OnHitByMissile(const VuParams &params);

	// VuRigidBodyContactCallback
	virtual bool			onRigidBodyContactAdded(VuContactPoint &cp);

	// scripting
	VuRetVal				Break(const VuParams &params);

	virtual void			tickDecision(float fdt);
	virtual void			tickBuild(float fdt);

	// components
	Vu3dDrawBreakableModelComponent	*mp3dDrawBreakableModelComponent;

	// properties
	std::string				mType;
	std::string				mPfxName;
	std::string				mSfxName;
	bool					mThrowDriver;
	int						mStatsType;

	VuDBEntryProperty		*mpTypeProperty;
	bool					mThresholdNormal;
	float					mThresholdSpeed;
	float					mSpeedPenalty;
	float					mCamShakeMagnitude;
	float					mCamShakeDuration;
	float					mCamShakeFalloffTime;
	float					mCamShakeFrequency;

	bool					mbBreakNow;
	VuVector3				mCollisionPos;
	VuVector3				mCollisionVel;
	VuWeakRef<VuEntity>		mBreakerEntity;
};