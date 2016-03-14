//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuOilSlickEntity
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Dynamics/VuDynamics.h"
#include "VuEngine/Math/VuVector3.h"

class VuCarEntity;
class Vu3dDrawComponent;
class VuOilSlickRigidBody;
class VuGfxDrawParams;
class VuMaterialAsset;


class VuOilSlickEntity : public VuEntity, public VuRigidBodyContactCallback
{
	DECLARE_RTTI

public:
	static VuOilSlickEntity	*drop(const VuJsonContainer &data, VuCarEntity *pCar);

	VuOilSlickEntity(const VuJsonContainer &data, VuCarEntity *pCar, const VuMatrix &transform, float radius, const std::deque<VuVector3> &verts);
	~VuOilSlickEntity();

	virtual void	onGameInitialize();
	virtual void	onGameRelease();

protected:
	// VuRigidBodyContactCallback
	virtual bool	onRigidBodyContactAdded(VuContactPoint &cp);

	void			addToWorld();
	void			removeFromWorld();

	void			tickDecision(float fdt);

	void			draw(const VuGfxDrawParams &params);

	class VuOilSlickVertex
	{
	public:
		float		mXyz[3];
		float		mUv[2];
		VUUINT32	mColor;
	};
	typedef std::vector<VuOilSlickVertex> Verts;

	Vu3dDrawComponent	*mp3dDrawComponent;

	VuCarEntity			*mpLauncher;
	VuMatrix			mTransform;
	float				mRadius;
	VuOilSlickRigidBody	*mpRigidBody;
	VuMaterialAsset		*mpMaterialAsset;
	float				mDrawDist;
	float				mAge;
	std::string			mCarEffect;
	Verts				mVerts;

	bool					mAddedToWorld;
	bool					mRunOver;
	VuWeakRef<VuCarEntity>	mRunOverEntity;
};
