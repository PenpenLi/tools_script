//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Fake PowerUp class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Dynamics/VuDynamics.h"

class VuCarEntity;
class VuFakePowerUp;
class VuMatrix;
class VuPfxSystemInstance;
class Vu3dDrawComponent;
class VuFakePowerUpRigidBody;
class VuGfxDrawParams;


class VuFakePowerUpEntity : public VuEntity, public VuRigidBodyContactCallback
{
	DECLARE_RTTI

public:
	VuFakePowerUpEntity(const VuJsonContainer &data, VuCarEntity *pCar, const VuMatrix &transform);
	~VuFakePowerUpEntity();

	virtual void	onGameInitialize();
	virtual void	onGameRelease();

protected:
	// VuRigidBodyContactCallback
	virtual bool	onRigidBodyContactAdded(VuContactPoint &cp);

	void			tickDecision(float fdt);
	void			tickBuild(float fdt);

	void			draw(const VuGfxDrawParams &params);

	Vu3dDrawComponent		*mp3dDrawComponent;

	const VuJsonContainer	&mData;
	VuCarEntity				*mpLauncher;
	VuMatrix				mTransform;
	VuPfxSystemInstance		*mpStaticPfxSystem;
	btCollisionShape		*mpShape;
	btCollisionShape		*mpChildShape;
	VuFakePowerUpRigidBody	*mpRigidBody;
	std::string				mStaticPfx;
	std::string				mBreakPfx;
	std::string				mBreakSfx;
	float					mDrawDistance;
	float					mAge;
	bool					mBreakNow;
	VuVector3				mCollisionVel;
	VuWeakRef<VuCarEntity>	mBreakerEntity;
};
