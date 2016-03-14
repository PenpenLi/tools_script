//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuRewardWheelEntity
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Entities/VuEntity.h"

class VuRewardWheelEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuRewardWheelEntity();

	virtual void		onGameInitialize();
	virtual void		onGameRelease();

protected:
	// event handlers
	void				OnUITick(const VuParams &params);
	void				OnUIDraw(const VuParams &params);

	// scripting
	VuRetVal			Spin(const VuParams &params);
	VuRetVal			IsCompleted(const VuParams &params);

	void				drawLayout(bool bSelected);
	void				drawWheel(float alpha);
	void				drawNeedle(float alpha);

	int					getNormalizedSlice(int slice);
	bool				isSliceVisible(const VuVector2 &center, float innerRadius, float outerRadius, float minAngle, float maxAngle);
	void				drawRewardSlice(float angle, VuTexture *pTexture);
	void				drawBackgroundSlice(float angle, int curSlice);
	void				drawPegs(float angle);

	VuVector2			calcOffset();

	void				createData();
	bool				chooseTodaysCar(std::string &name);
	bool				chooseTodaysPowerUp(std::string &name);
	bool				isStatMaxed(const std::string &stat);

	void				giveReward(int index);

	// components
	VuScriptComponent		*mpScriptComponent;

	// properties
	float					mAngularOffset;
	float					mInnerRadius;
	float					mOuterRadius;
	float					mBackgroundRadius;
	float					mWedgeAngle;
	float					mSpaceAngle;
	float					mSubdivisionAngle;
	float					mRotationLag;
	float					mMaxAngSpeed;
	VuUIAnchorProperties	mAnchor;
	VuVector2				mTexCoord0;
	VuVector2				mTexCoord1;
	int						mSpinRevs;

	float					mTargetAngle;
	float					mCurAngle;
	float					mAngularVel;
	int						mCurWedge;

	enum eState { WAITING, SPINNING, DONE };
	eState					mState;

	VuUIImageProperties		mNeedleTexture;
	VuVector2				mNeedleSize;
	float					mNeedleRadius;
	float					mNeedleRotation;
	float					mNeedleAngularVelocity;
	VuVector2				mNeedlePivotOffset;
	
	VuUIImageProperties		mPegTexture;
	VuVector2				mPegSize;
	VuVector2				mPegAdjust;

	VuUIImageProperties		mWheelSlice1;
	VuUIImageProperties		mWheelSlice2;
	VuVector2				mWheelTexCoord0;
	VuVector2				mWheelTexCoord1;

	struct Reward
	{
		Reward() : mpImage(VUNULL), mAmount(0), mChance(100) {}
		std::string		mType;
		int				mChance;
		int				mAmount;
		std::string		mName;
		VuTextureAsset	*mpImage;
	};
	typedef std::deque<Reward> Rewards;
	Rewards					mRewards;
	int						mDaysSince2000;
};

