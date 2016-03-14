//*****************************************************************************
//
//  Copyright (c) 2009-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  CarPfxController class
// 
//*****************************************************************************

#pragma once

#include "VuPurple/Pfx/Processes/VuPfxEmitCarChassis.h"
#include "VuEngine/Math/VuAabb.h"
#include "VuEngine/Math/VuMatrix.h"

class VuCarEntity;
class VuJsonContainer;
class VuGfxDrawParams;
class VuPfxSystemInstance;
class VuFluidsObject;


class VuCarPfxController : public VuPfxEmitCarChassisInstance::StateIF
{
public:
	VuCarPfxController(VuCarEntity *pCar);
	~VuCarPfxController();

	void					onGameInitialize();
	void					onGameRelease();

	void					setData(const VuJsonContainer &data);
	void					preDataModified();
	void					postDataModified();

	void					applyTuneUp();

	void					onTickBuild(float fdt);
	void					draw(const VuGfxDrawParams &params);

	const VuAabb			&getAabb()					{ return mAabb; }
	VuFluidsObject			*getFluidsObject() const	{ return mpFluidsObject; }
	bool					isBackFirePfxOn()			{ return mBackFirePfxOn; }

private:
	struct VuPfxWheel
	{
		VuPfxWheel();
		VUUINT8				mSurfaceType;
		const char			*mDrivePfxName;
		const char			*mSlidePfxName;
		VuPfxSystemInstance	*mpDriveSystem;
		VuPfxSystemInstance	*mpSlideSystem;
		VuPfxSystemInstance	*mpPowerSlideSystem;
	};

	// VuPfxEmitCarChassisInstance::StateIF
	virtual void			getPfxEmitCarChassisState(VuMatrix &transform, VuVector3 &linVel, VUUINT32 &waterSurfaceFlags);

	void					create();
	void					clear();
	void					tickWheelEffects(float fdt);

	std::string				mFluidsMeshAssetName;
	std::string				mChassisEffectName;
	std::string				mBackFirePfxName;
	std::string				mBackFireBluePfxName;
	std::string				mPowerSlidePfxName;
	float					mBackFireMaxDuration;

	VuCarEntity				*mpCar;
	VuFluidsObject			*mpFluidsObject;
	VuPfxSystemInstance		*mpChassisEffect;
	VuPfxSystemInstance		*mpBackFirePfxSystem;
	VuPfxWheel				mPfxWheels[4];
	VuAabb					mAabb;
	bool					mBackFirePfxOn;

	float					mBackFireTimer;
};