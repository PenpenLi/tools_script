//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuPlantEntity
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Dynamics/VuDynamics.h"

class VuCarEntity;
class VuMatrix;
class VuPlantRigidBody;


class VuPlantEntity : public VuEntity, public VuRigidBodyContactCallback
{
	DECLARE_RTTI

public:
	VuPlantEntity(VuCarEntity *pCar, const VuJsonContainer &data, const VuMatrix &transform);
	~VuPlantEntity();

	virtual void	onGameInitialize();
	virtual void	onGameRelease();

protected:
	// VuRigidBodyContactCallback
	virtual bool	onRigidBodyContactAdded(VuContactPoint &cp);

	void			tickDecision(float fdt);

	VuCarEntity				*mpLauncher;
	VuMatrix				mTransform;
	btCollisionShape		*mpShape;
	VuPlantRigidBody		*mpRigidBody;
	VuWeakRef<VuCarEntity>	mVictimEntity;
	float					mAge;
	VUUINT32				mPlantPfx;
	float					mLifeTime;
	float					mFadeTime;
	std::string				mCarEffect;
};
