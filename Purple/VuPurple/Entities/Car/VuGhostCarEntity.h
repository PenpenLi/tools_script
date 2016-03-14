//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Ghost Car entity
// 
//*****************************************************************************

#pragma once

#include "VuPurple/Pfx/Processes/VuPfxEmitCarChassis.h"
#include "VuPurple/Util/VuGhostUtil.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Gfx/Model/VuStaticModelInstance.h"
#include "VuEngine/Gfx/Model/VuAnimatedModelInstance.h"
#include "VuEngine/Gfx/Model/VuModelSkin.h"
#include "VuEngine/Gfx/VuGfxSettings.h"
#include "VuEngine/HAL/Audio/VuAudioEvent.h"

class Vu3dDrawComponent;
class VuCubicPosCurve;
class VuAnimationControl;
class VuTireTrack;
class VuFluidsObject;
class VuPfxSystemInstance;
class VuBlobShadow;
class VuTextureAsset;
class VuTrackSector;


class VuGhostCarEntity : public VuEntity, public VuPfxEmitCarChassisInstance::StateIF
{
public:
	VuGhostCarEntity(const VuArray<VUBYTE> &ghostData);
	~VuGhostCarEntity();

	virtual void	onLoad(const VuJsonContainer &data);

	virtual void	onGameInitialize();
	virtual void	onGameRelease();

	void			startPlayback();
	void			stopPlayback();

	void			setTimeFactor(float factor) { mPlaybackTimeFactor = factor; }

	float			getPlaybackTime() { return mPlaybackTime; }
	float			getPlaybackEndTime() { return mPlaybackEndTime; }
	const VuAabb	&getCollisionAabb() { return mCollisionAabb; }
	bool			getIsPlaybackActive() { return mPlaybackActive; }

	VuTextureAsset	*getPortraitImage() { return mpPortraitImage; }

	float			getDistFromStartOfLap() { return mDistFromStartOfLap; }

protected:
	// VuPfxEmitCarChassisInstance::StateIF
	virtual void		getPfxEmitCarChassisState(VuMatrix &transform, VuVector3 &linVel, VUUINT32 &waterSurfaceFlags);

	void				processGhostData(VuBinaryDataReader &ghostDataReader);
	void				buildSplines();

	void				tickDecision(float fdt);
	void				tickBuild(float fdt);

	void				draw(const VuGfxDrawParams &params);
	void				drawShadow(const VuGfxDrawShadowParams &params);

	void				configShaderLOD(int value);

	VuGhostUtil::InitialState	mInitialState;

	Vu3dDrawComponent		*mp3dDrawComponent;

	VuStaticModelInstance	mChassis;
	VuStaticModelInstance	mChassisLod1;
	VuStaticModelInstance	mChassisLod2;
	VuAnimatedModelInstance	mSuspension;
	VuAnimatedSkeleton		*mpSuspensionAnimatedSkeleton;
	VuJsonContainer			mSkinData;
	VuModelSkin				mSkin;
	VuModelSkin				mSkinLod1;
	VuModelSkin				mSkinLod2;
	VuAnimatedModelInstance	mDriver;
	VuAnimatedModelInstance	mDriverLod1;
	VuStaticModelInstance	mDriverLod2;
	VuAnimatedSkeleton		*mpDriverAnimatedSkeleton;
	VuAnimationControl		*mpTurnAnimControl;
	VuBlobShadow			*mpBlobShadow;
	VuTextureAsset			*mpPortraitImage;

	float				mChassisLod1Dist;
	float				mChassisLod2Dist;
	float				mDriverLod1Dist;
	float				mDriverLod2Dist;
	float				mMaxSteeringAngle;
	VuAabb				mCollisionAabb;

	VuAabb				mAabb;
	VuVector3			mLinearVelocity;
	VuMatrix			mDriverTransform;

	struct Corner
	{
		Corner() : mLod1Dist(FLT_MAX), mWheelRadius(0), mAttachmentPoint(0,0,0), mUpperLimit(0), mLowerLimit(0), mSuspensionDist(0.0f), mSteeringAngle(0.0f),
			mWheelTransform(VuMatrix::identity()), mpAnimationControl(VUNULL), mFront(false), mLeft(false),
			mpTireTrack(VUNULL), mContact(false), mSliding(false), mSurfaceType(0), mPfxSurfaceType(0),
			mDrivePfxName(VUNULL), mSlidePfxName(VUNULL), mpDrivePfx(VUNULL), mpSlidePfx(VUNULL), mpWaterPfx(VUNULL),
			mVisualWaterHeight(-1e9), mSubmerged(false)
		{}

		VuStaticModelInstance	mWheelModel;
		VuStaticModelInstance	mWheelModelLod1;
		VuJsonContainer			mBlurSkinData;
		VuModelSkin				mBlurSkin;
		float					mLod1Dist;
		float					mWheelRadius;
		VuVector3				mAttachmentPoint;
		float					mUpperLimit;
		float					mLowerLimit;
		float					mSuspensionDist;
		float					mSteeringAngle;
		VuMatrix				mWheelTransform;
		VuAnimationControl		*mpAnimationControl;
		bool					mFront;
		bool					mLeft;
		VuTireTrack				*mpTireTrack;
		float					mTireTrackWidth;
		float					mTireTrackMinDist;
		float					mTireTrackMinVel;
		bool					mContact;
		bool					mSliding;
		VUUINT8					mSurfaceType;
		VUUINT8					mPfxSurfaceType;
		const char				*mDrivePfxName;
		const char				*mSlidePfxName;
		VuPfxSystemInstance		*mpDrivePfx;
		VuPfxSystemInstance		*mpSlidePfx;
		std::string				mWaterPfxName;
		VuPfxSystemInstance		*mpWaterPfx;
		float					mVisualWaterHeight;
		bool					mSubmerged;
	};
	Corner				mCorners[4];

	// data
	float				mNearFadeDist;
	float				mFarFadeDist;
	float				mEndFadeOutTime;

	bool				mPlaybackActive;
	float				mPlaybackTime;
	int					mPlaybackFrame;
	float				mPlaybackEndTime;
	float				mPlaybackTimeFactor;

	struct VuPlaybackFrame : public VuGhostUtil::Frame
	{
		int			mSplineIndex;
		int			mSplineOffset;
		VuVector3	mLinearVelocity;
	};
	typedef std::deque<VuPlaybackFrame> PlaybackFrames;
	typedef std::vector<VuCubicPosCurve *> Splines;
	PlaybackFrames		mFrames;
	Splines				mSplines;

	bool				mGhostMode;
	float				mGhostModeTimer;

	float				mSteering;
	float				mAlpha;
	float				mShadowValue;
	float				mDrawShadowValue;
	VuColor				mDrawColor;
	VuVector4			mPfxColor;
	VUUINT32			mWaterSurfaceFlags;

	VuGfxSettings		mGfxSettings;

	// audio
	std::string			mAudioEventName;
	float				mMinRPM;
	float				mMaxRPM;
	VuAudioEvent		mEngineAudioEvent;

	// pfx
	VuFluidsObject		*mpFluidsObject;
	VuPfxSystemInstance	*mpChassisEffect;
	VuPfxSystemInstance	*mpBackFirePfxSystem;
	VuPfxSystemInstance	*mpCarTrailEffect;

	std::string			mChassisEffectName;
	std::string			mBackFirePfxName;
	std::string			mCarTrailEffectName;
	bool				mBackFirePfxFlag;
	bool				mBackFirePfxOn;
	VuMatrix			mCarTrailTransform;

	// gameplay
	int					mCurrentSection;
	const VuTrackSector	*mpCurrentSector;
	float				mDistFromStartOfLap;
};
