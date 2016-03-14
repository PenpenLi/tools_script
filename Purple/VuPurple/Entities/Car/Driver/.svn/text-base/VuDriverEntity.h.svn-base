//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Driver base class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Animation/VuAnimationEventIF.h"
#include "VuEngine/Util/VuColor.h"
#include "VuEngine/Util/VuFSM.h"

class VuCarEntity;
class Vu3dDrawComponent;
class VuGfxDrawParams;
class VuGfxDrawShadowParams;
class VuAnimatedModelInstance;
class VuStaticModelInstance;
class VuSkeleton;
class VuRagdoll;
class VuPfxSystemInstance;
class VuTrackSector;
class VuRect;
class VuAnimatedSkeleton;
class VuAnimationControl;
class VuTextureAsset;
class VuAnimationAsset;
class VuCarEffect;


class VuDriverEntity : public VuEntity, public VuAnimationEventIF
{
	DECLARE_RTTI

protected:
	~VuDriverEntity();

public:
	VuDriverEntity(VuCarEntity *pCar);

	virtual bool				isHuman() = 0;
	virtual bool				isAi() = 0;
	virtual bool				isLocal() = 0;
	virtual bool				isRemote() = 0;
	virtual bool				isUi() = 0;

	virtual void				dataModified(const std::string &driverName);

	virtual void				onLoad(const VuJsonContainer &data) {}
	virtual void				onGameInitialize();
	virtual void				onGameRelease();
	virtual void				onTickDecision(float fdt);
	virtual void				onTickBuild(float fdt);
	virtual void				onRecover();
	virtual void				onTeleported() {}
	virtual void				onApplyForces(float fdt) {}

	virtual void				notifyOfEffect(const std::string& name, const VuCarEffect* pEffect) {}				// Called when a CarEffect is applied to this driver's car

	virtual const VuTrackSector	*getCurrentSector() const { return VUNULL; }
	virtual bool				headingWrongWay() const	{ return false; }

	virtual float				getAudioThrottleControl() const;
	virtual float				getAnimationThrottleControl() const;
	virtual float				getAnimationYawControl() const;

	void						setName(const std::string &name)	{ mName = name; }
	const char					*getName()							{ return mName.c_str(); }
	VuTextureAsset				*getPortraitImage()					{ return mpPortraitImage; }
	VuTextureAsset				*getAbilityImage()					{ return mpAbilityImage; }
	VuAnimatedModelInstance		*getAnimatedModelInstance()	const	{ return mpModelInstance; }
	VuSkeleton					*getSkeleton() const;

	virtual void				setFinished();
	void						onImpact(const VuVector3 &deltaV);
	void						onPassedCar(VuCarEntity *pOtherCar);
	void						onCollidedWithCar(VuCarEntity *pOtherCar);

	void						startRagdoll();
	bool						isRagdollActive();

	void						enableAutoDrive(float seconds) { setAutoDrive(true);  setAutoDriveDuration(seconds); }
	void						setAutoDrive(bool autoDriveOn) { mAutoDrive = autoDriveOn; }
	void						setAutoDriveDuration(float seconds) { mAutoDriveDuration = seconds; }
	bool						isAutoDrive() const { return mAutoDrive; }

	void						hide();

	VUUINT32					createEffectAnimation(const std::string &assetName);
	void						releaseEffectAnimation(VUUINT32 id);

	void						getRagdollCameraTarget(VuVector3 &target) const;
	void						getRagdollCameraVelocity(VuVector3 &vel) const;

protected:
	enum eAnim { ANIM_START, ANIM_WIN, ANIM_LOSE, ANIM_FINISH, ANIM_IMPACT_FORWARD, ANIM_HAPPY, ANIM_ANGRY, NUM_ANIMS };
	typedef std::vector<std::string> Anims;
	typedef std::vector<VuAnimationControl *> AnimControls;

	virtual void			loadData(const VuJsonContainer &data);
	virtual void			clear();

	void					draw(const VuGfxDrawParams &params);
	void					drawShadow(const VuGfxDrawShadowParams &params);
	void					drawPrefetch();

	// VuAnimationEventIF
	virtual void			onAnimationEvent(const std::string &type, const VuJsonContainer &params);

	// animation FSM
	void					createAnimFSM();
	void					animStartEnter() { startAnimation(ANIM_START, 0.0f); }
	void					animDriveEnter();
	void					animImpactForwardEnter() { startAnimation(ANIM_IMPACT_FORWARD, 0.1f); }
	void					animHappyEnter() { startAnimation(ANIM_HAPPY, 0.25f); mHappyTimer = mHappyRepeatTime; }
	void					animAngryEnter() { startAnimation(ANIM_ANGRY, 0.25f); mAngryTimer = mAngryRepeatTime; }
	void					animWinEnter() { startAnimation(ANIM_WIN, 0.25f); }
	void					animLoseEnter() { startAnimation(ANIM_LOSE, 0.25f); }
	void					animFinishEnter() { startAnimation(ANIM_FINISH, 0.25f); }
	void					animRagdollEnter();
	void					animRagdollExit();

	void					startAnimation(eAnim anim, float blendTime);
	void					stopAnimation();

	// components
	Vu3dDrawComponent		*mp3dDrawComponent;

	// db data
	std::string				mPortraitImageAssetName;
	std::string				mAbilityImageAssetName;
	std::string				mModelAssetName;
	std::string				mLod1ModelAssetName;
	std::string				mLod2ModelAssetName;
	float					mLod1Dist;
	float					mLod2Dist;
	float					mDrawDist;
	std::string				mTurnAnim;
	Anims					mAnims[NUM_ANIMS];
	float					mHappyRepeatTime;
	float					mAngryRepeatTime;

	VuCarEntity				*mpCar;
	VuAnimatedModelInstance	*mpModelInstance;
	VuAnimatedModelInstance	*mpLod1ModelInstance;
	VuStaticModelInstance	*mpLod2ModelInstance;
	std::string				mName;
	VuAnimatedSkeleton		*mpAnimatedSkeleton;
	VuAnimationControl		*mpTurnAnimControl;
	AnimControls			mAnimControls[NUM_ANIMS];
	VuAnimationControl		*mpCurAnimControl;
	float					mCurTurnPos;
	float					mCurTurnRate;
	float					mBlendRate;
	float					mAnimWeight;
	VuRagdoll				*mpRagdoll;
	VuPfxSystemInstance		*mpRagdollPfx;
	VuFSM					mAnimFSM;
	std::string				mRagdollType;
	std::string				mRagdollCameraTarget;
	std::string				mRagdollPfx;
	int						mRagdollCameraTargetBodyIndex;
	VuTextureAsset			*mpPortraitImage;
	VuTextureAsset			*mpAbilityImage;
	float					mAbilityChargeTime;
	float					mHappyTimer;
	float					mAngryTimer;
	bool					mAutoDrive;
	float					mAutoDriveDuration;

	struct VuEffectAnim
	{
		VUUINT32			mID;
		bool				mActive;
		VuAnimationControl	*mpAnimControl;
	};
	typedef std::vector<VuEffectAnim> EffectAnims;
	EffectAnims				mEffectAnims;
	VUUINT32				mNextEffectAnimId;

	VuVector3				mPrevCarVelocity;
	int						mPrevPlace;
};