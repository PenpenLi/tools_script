//*****************************************************************************
//
//  Copyright (c) 2009-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  CarAnimController class
// 
//*****************************************************************************

#pragma once

class VuCarEntity;
class VuJsonContainer;
class VuAnimatedSkeleton;
class VuAnimationControl;


class VuCarAnimController
{
public:
	VuCarAnimController(VuCarEntity *pCar);
	~VuCarAnimController();

	void						onGameInitialize();
	void						onGameRelease();

	void						setData(const VuJsonContainer &data);
	void						preDataModified();
	void						postDataModified();

	void						onTickDecision(float fdt);
	void						onTickBuild(float fdt);

	const VuAnimatedSkeleton	*getAnimatedSkeleton() const	{ return mpAnimatedSkeleton; }

private:
	void				create();
	void				clear();

	VuCarEntity		*mpCar;

	VuAnimatedSkeleton	*mpAnimatedSkeleton;
	VuAnimationControl	*mpThrustAnimControl;
	VuAnimationControl	*mpTurnAnimControl;

	std::string			mThrustAnimAssetName;
	std::string			mTurnAnimAssetName;
	std::string			mTransformAnimAssetName;

	float				mThrustRevRate;
	float				mThrustIdleRate;
	float				mThrustFwdRate;
	float				mThrustAccel;

	float				mCurTurnPos;
};