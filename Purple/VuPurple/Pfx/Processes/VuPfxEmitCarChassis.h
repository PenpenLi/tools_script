//*****************************************************************************
//
//  Copyright (c) 2008-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Pfx Car Chassis Emitter
// 
//*****************************************************************************

#include "VuEngine/Pfx/VuPfxProcess.h"
#include "VuEngine/Util/VuColor.h"

class VuMatrix;
class VuFluidsObject;


class VuPfxEmitCarChassis : public VuPfxProcess
{
	DECLARE_RTTI
	DECLARE_PFX_PROCESS

public:
	VuPfxEmitCarChassis();

	float		mMinLifespan;
	float		mMaxLifespan;
	float		mMinScale;
	float		mMaxScale;
	float		mMinRotation;
	float		mMaxRotation;
	float		mMinAngularVelocity;
	float		mMaxAngularVelocity;
	float		mMinSpawnCarSpeed;
	float		mMaxSpawnCarSpeed;
	float		mMaxSpawnPerSecond;
	VuColor		mDiffuseColor;
	float		mLateralVelocityMultiplier;
	float		mVerticalVelocityMultiplier;
	float		mMinDirStretch;
	float		mMaxDirStretch;
	float		mSpawnDistance;
};

class VuPfxEmitCarChassisInstance : public VuPfxProcessInstance
{
public:
	VuPfxEmitCarChassisInstance() : mpStateIF(VUNULL), mpFluidsObject(VUNULL), mSpawnAccum(0) {}

	virtual void		start();
	virtual void		tick(float fdt, bool ui);

	class StateIF
	{
	public:
		virtual void	getPfxEmitCarChassisState(VuMatrix &transform, VuVector3 &linVel, VUUINT32 &waterSurfaceFlags) = 0;
	};
	StateIF				*mpStateIF;
	VuFluidsObject		*mpFluidsObject;
	float				mSpawnAccum;
};
