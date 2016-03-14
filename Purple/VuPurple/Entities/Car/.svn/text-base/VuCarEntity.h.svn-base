//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Car entity
// 
//*****************************************************************************

#pragma once

#include "VuCarStats.h"
#include "Parts/VuCarWheel.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/Transform/VuTransformComponent.h"
#include "VuEngine/Dynamics/VuDynamics.h"
#include "VuEngine/Dynamics/VuStridingMesh.h"
#include "VuEngine/Gfx/VuGfxSettings.h"
#include "VuEngine/Gfx/Model/VuModelSkin.h"
#include "VuEngine/Gfx/Shadow/VuDropShadow.h"
#include "VuEngine/Math/VuVector2.h"
#include "VuEngine/Util/VuColor.h"
#include "VuPurple/Util/VuBoing.h"


class Vu3dLayoutComponent;
class Vu3dDrawComponent;
class VuInstigatorComponent;
class Vu3dLayoutDrawParams;
class VuGfxDrawParams;
class VuGfxDrawShadowParams;
class VuCamera;
class VuCarRigidBody;
class VuDriverEntity;
class VuCarCamera;
class VuCarChassis;
class VuCarEngine;
class VuCarAnimController;
class VuCarPfxController;
class VuCarPowerUpController;
class VuCarEffectController;
class VuCarSuspension;
class VuCarHeadlights;
class VuCarShadow;
class VuFluidsObject;
class VuSkeleton;
class VuDBEntryProperty;
class VuAnimatedModelInstance;
class VuStaticModelInstance;
class VuDropShipEntity;


class VuCarRigidBody : public VuRigidBody
{
public:
	VuCarRigidBody(const btRigidBody::btRigidBodyConstructionInfo &info, VuEntity *pEntity, VUINT16 collisionGroup, VUINT16 collisionMask);
	virtual bool checkCollideWithOverride(const  btCollisionObject* co) const;
	void ensureCheckCollideWith() { m_checkCollideWith = true; }
};


class VuCarEntity : public VuEntity, public btMotionState, public VuDynamicsStepCallback, public VuRigidBodyContactCallback, public VuDropShadow::Callback
{
	DECLARE_RTTI

public:
	VuCarEntity();
	~VuCarEntity();

	virtual void			onLoad(const VuJsonContainer &data);
	virtual void			onGameInitialize();
	virtual void			onGameRelease();

	VuCarRigidBody			*getRigidBody() const			{ return mpRigidBody; }
	VuDriverEntity			*getDriver()					{ return mpDriver; }
	VuCarCamera				*getCamera() const				{ return mpCamera; }
	VuCarChassis			*getChassis() const				{ return mpChassis; }
	VuCarEngine				*getEngine() const				{ return mpEngine; }
	VuCarPfxController		*getPfxController() const		{ return mpPfxController; }
	VuCarPowerUpController	*getPowerUpController() const	
	{ 
		return mpPowerUpController;
	}
	VuCarEffectController	*getEffectController() const	{ return mpEffectController; }
	VuCarSuspension			*getSuspension() const			{ return mpSuspension; }
	VuCarHeadlights			*getHeadlights()				{ return mpHeadlights; }
	VuCarShadow				*getShadow()					{ return mpShadow; }
	VuCarWheel				&getWheel(int index)			{ return mWheels[index]; }
	VuStaticModelInstance	*getModelInstance()				{ return mpModelInstance; }
	VuFluidsObject			*getFluidsObject() const;
	VuSkeleton				*getSkeleton() const;

	const VuVector3			&getModelPosition() const	{ return mpTransformComponent->getWorldPosition(); }
	const VuVector3			&getModelRotation() const	{ return mpTransformComponent->getWorldRotation(); }
	const VuMatrix			&getModelMatrix() const		{ return mpTransformComponent->getWorldTransform(); }
	const VuMatrix			&getMountPoint(const char *name) const;

	void					getDriverTransform(VuMatrix &transform) const;

	const VuVector3			&getLinearVelocity() const	{ return mpRigidBody->getVuLinearVelocity(); }
	const VuVector3			&getAngularVelocity() const	{ return mpRigidBody->getVuAngularVelocity(); }

	float					getMaxForwardSpeed() const;
	bool					isOutOfBounds() const			{ return mOutOfBounds; }

	void					setHasFinished()				{ mStats.mHasFinished = true; }
	bool					getHasFinished() const			{ return mStats.mHasFinished; }
	void					setHasCheated()					{ mHasCheated = true; }
	bool					getHasCheated() const			{ return mHasCheated; }
	void					setPlace(int place); //			{ mPlace = place; }
	int						getPlace() const; //			{ return mPlace; }
	bool					getCollidedThisFrame() const	{ return mCollidedThisFrame; }
	bool					getCollidedThisFrameWithGround() const { return mCollidedThisFrameWithGround; }

	float					getMass() const					{ return mMass; }
	const VuVector3			&getCenterOfMass() const		{ return mCenterOfMass; }
	const VuAabb			&getCollisionAabb() const		{ return mCollisionAabb; }
	const VuAabb			&getFreezeEffectAabb() const	{ return mFreezeEffectAabb; }
	float					getShadowValue() const			{ return mShadowValue; }
	float					getDrawShadowValue() const		{ return mDrawShadowValue; }
	const VuColor			&getDrawColor() const			{ return mDrawColor; }
	const VuVector4			&getPfxColor() const			{ return mPfxColor; }
	VUUINT32				getWaterSurfaceFlags() const	{ return mWaterSurfaceFlags; }

	void					setOverrideDrawColor(const VuColor &color) { mDrawColorOverride = true; mDrawOverrideColor = color; }
	void					clearOverrideDrawColor()                   { mDrawColorOverride = false; }

	float					getBreakablePenalty() const;

	bool					isControlPaused() const;
	void					setThrottleControl(float fThrottle)	{ mThrottleControl = fThrottle; }
	void					setYawControl(float fYaw)			{ mYawControl = fYaw; }
	void					setYawControlFactor(float factor)	{ mYawControlFactor = factor; }
	void					setPowerUpControl(bool bPowerUp)	{ mPowerUpControl = bPowerUp; }
	bool					getPowerUpControl() const			{ return isControlPaused() ? false : mPowerUpControl; }

	//xlm
	void					setExtraPowerUpControl1(bool bPowerUp)	{
		mExtraPowerUpControl1 = bPowerUp;
	}
	bool					getExtraPowerUpControl1() const { return isControlPaused() ? false : mExtraPowerUpControl1; }

	void					setPowerSlideControl(bool bPowerSlide) { mPowerSlideControl = bPowerSlide; }
	bool					getPowerSlideControl() const		{ return isControlPaused() ? false : mPowerSlideControl; }

	float					getThrottleControl() const;
	float					getYawControl() const				{ return isControlPaused() ? 0 : mYawControl*mYawControlFactor; }
	float					getRawThrottleControl() const		{ return mThrottleControl; }
	float					getRawYawControl() const			{ return mYawControl; }

	void					startCrash()						{ mStartCrash = true; }
	bool					hasCrashed()						{ return mStartCrash | mHasCrashed; }
	bool					canRecover() const					{ return !isControlPaused() && !mGhostMode && !mIsSpawned; }
	float					getCrashTimer() const				{ return mCrashTimer; }

	bool					shatterIce() const				{ return mShatterIce; }

	void					pushPauseControlRequest()		{ mPauseControlRequestCount++; }
	void					popPauseControlRequest()		{ mPauseControlRequestCount--; VUASSERT(mPauseControlRequestCount >= 0, "mismatch"); }
	int						getPauseControlCount() const	{ return mPauseControlRequestCount; }

	void					startSimulatedAI();
	void					stopSimulatedAI();

	float					getRadialBlurAmount() const				{ return mRadialBlurAmount; }
	float					getLensWaterRadialSpreadAmount() const	{ return mLensWaterRadialSpreadAmount; }
	float					getSpeedEffectAmount() const			{ return mSpeedEffectAmount; }

	const VuGfxSettings		&getGfxSettings() const			{ return mGfxSettings; }

	bool					canUsePowerUp();
	bool					shouldRecover();
	void					recover();
	void					recoverTo(const VuVector3 &position, const VuVector3 &rotation, bool bGhost, bool bIgnoreTriggers);
	void					startGhostMode(float duration);
	bool					isInGhostMode()	{ return mGhostMode; }
	bool					getGhostHide() { return mGhostHide; }
	void					hide();
	bool					isHidden() { return mHidden; }
	bool					isDrowning() { return mWaterDeath; }
	bool					isPowerSliding() { return mPowerSliding; }
	bool					isGhostly();

	void					setIsRacer(bool isRacer) { mIsRacer = isRacer; }
	bool					getIsRacer() { return mIsRacer; }

	void					setIsSpawned(bool isSpawned) { mIsSpawned = isSpawned; }
	bool					isSpawned() { return mIsSpawned; }

	void					setIsCameraTarget(bool isCameraTarget) { mIsCameraTarget = isCameraTarget; }
	bool					getIsCameraTarget() { return mIsCameraTarget; }

	void					applyTuneUp();
	bool					isTunedUp() { return mIsTunedUp; }

	void					pushDisableCollisions();
	void					popDisableCollisions();

	void					attachToDropShip(VuDropShipEntity *pDropShip);
	void					detachFromDropShip(float releaseSpeed);
	void					updateDropShip(const VuMatrix &transform, const VuVector3 &linVel, const VuVector3 &angVel, float radialBlur);
	bool					isAttachedToDropShip() { return mAttachedToDropShip; }

	void					changeDriver(const std::string &driver);
	void					changeSkin(const std::string &decal, const std::string &paintColor, const std::string &decalColor);

	VuCarStats				&getStats()			{ return mStats; }
	const std::string		&getNetGameId()		{ return mNetGameId; }
	const std::string		&getCarName()		{ return mCarName; }
	int						getCarStage()		{ return mCarStage; }

	const std::string		&getDecalName()			{ return mDecalName; }
	const std::string		&getDecalColorName()	{ return mDecalColorName; }
	const std::string		&getPaintColorName()	{ return mPaintColorName; }
	const std::string		&getDriverName()		{ return mDriverName; }

	const std::string		&getOpponentName()		{ return mOpponentName; }

	int						getAccelLevel() { return mAccelLevel; }
	int						getSpeedLevel() { return mSpeedLevel; }
	int						getHandlingLevel() { return mHandlingLevel; }
	int						getToughLevel() { return mToughLevel; }

	void					updateSkin();

	VUUINT32				getDynamicLightGroupMask() const { return mDynamicLightGroupMask; }

	float					getDurability() const { return mDurability*mAiBuffTough; }

	// ai buffs
	void					setAiBuffAccel(float value) { mAiBuffAccel = value; }
	void					setAiBuffSpeed(float value) { mAiBuffSpeed = value; }
	void					setAiBuffHandling(float value) { mAiBuffHandling = value; }
	void					setAiBuffTough(float value) { mAiBuffTough = value; }

	float					getAiBuffAccel() const { return mAiBuffAccel; }
	float					getAiBuffSpeed() const { return mAiBuffSpeed; }
	float					getAiBuffHandling() const { return mAiBuffHandling; }
	float					getAiBuffTough() const { return mAiBuffTough; }

	struct ScorePopup
	{
		VuBoing		mBoing;
		int			mScore;
		VuVector2	mScreenPos;
	};
	typedef std::vector<ScorePopup> ScorePopups;
	void					showScorePopup(int score, const VuVector3 &worldPos);
	const ScorePopups		&getScorePopups() const { return mScorePopups; }

	void					addScore(const char *gameType, const char *objectType, const VuVector3 &pos);

	void					setAlpha(float alpha) { mAlpha = alpha; }
	float					getAlpha() { return mAlpha; }

protected:
	// btMotionState
	virtual void			getWorldTransform(btTransform& worldTrans) const;
	virtual void			setWorldTransform(const btTransform& worldTrans);

	// VuDynamicsStepCallback
	virtual void			onDynamicsAdvanceEnvironment(float fdt, bool bSimStep);
	virtual void			onDynamicsApplyForces(float fdt);

	// VuRigidBodyContactCallback
	virtual bool			onRigidBodyContactAdded(VuContactPoint &cp);

	// VuDropShadow::Callback
	virtual void			onDropShadowDraw(const VuGfxDrawShadowParams &params);

	// event handlers
	void					OnCameraShake(const VuParams &params);
	void					OnHitByMissile(const VuParams &params);
	void					OnExplosion(const VuParams &params);

	void					drawLayout(const Vu3dLayoutDrawParams &params);
	virtual void			draw(const VuGfxDrawParams &params);
	void					drawShadow(const VuGfxDrawShadowParams &params);
	void					drawPrefetch();
	void					drawDebugHUD();
	void					drawDebug(const VuCamera &camera);

	virtual void			tickDecision(float fdt);
	virtual void			tickBuild(float fdt);

	void					updateGhostMode(float fdt);
	void					updateWheels(float fdt);
	void					updateSpeedEffect(float fdt);
	void					updateWaterDeath(float fdt);

	void					updatePowerSliding(float fdt);
	void					startPowerSliding();
	void					stopPowerSliding();

	virtual void			setData(const VuJsonContainer &data);
	void					dataModified();
	void					transformModified();
	virtual void			clear();

	void					addToDynamicsWorld();
	void					removeFromDynamicsWorld();

	void					applyAnchorForces(float fdt);
	
	float					calcPowerSlideAngle();

	// notifications for derived classes
	virtual void			onStartRagdoll() {}
	virtual void			onCollidedWithHuman(float relSpeed) {}

	// components
	Vu3dLayoutComponent		*mp3dLayoutComponent;
	Vu3dDrawComponent		*mp3dDrawComponent;
	VuInstigatorComponent	*mpInstigatorComponent;

	// properties
	std::string				mDriverType;
	int						mPadIndex;
	std::string				mNetGameId;
	std::string				mCarName;
	int						mCarStage;
	std::string				mDecalName;
	std::string				mDecalColorName;
	std::string				mPaintColorName;
	std::string				mDriverName;
	int						mAccelLevel;
	int						mSpeedLevel;
	int						mHandlingLevel;
	int						mToughLevel;
	std::string				mOpponentName;

	// db data
	std::string				mModelAssetName;
	std::string				mLod1ModelAssetName;
	std::string				mLod2ModelAssetName;
	float					mLod1Dist;
	float					mLod2Dist;
	float					mDrawDist;
	VuColor					mAmbientColor;
	VuColor					mDiffuseColor;
	std::string				mSurfaceType;
	float					mMass;
	float					mInertiaFactor;
	float					mMaxSteeringAngle;
	std::string				mShadowTextureAssetName;
	VuVector3				mDriverOffset;
	float					mInducedPowerSlideTractionFactor;
	float					mInducedPowerSlideCoeff;
	float					mInducedPowerSlideSteeringFactor;

	float					mDurability;
	float					mAiBuffAccel;
	float					mAiBuffSpeed;
	float					mAiBuffHandling;
	float					mAiBuffTough;

	VuCarRigidBody			*mpRigidBody;
	VuDriverEntity			*mpDriver;
	VuCarCamera				*mpCamera;
	VuCarChassis			*mpChassis;
	VuCarEngine				*mpEngine;
	VuCarAnimController		*mpAnimController;
	VuCarPfxController		*mpPfxController;
	VuCarPowerUpController	*mpPowerUpController;
	VuCarEffectController	*mpEffectController;
	VuCarSuspension			*mpSuspension;
	VuCarHeadlights			*mpHeadlights;
	VuCarShadow				*mpShadow;
	VuCarWheel				mWheels[4];



	VuAabb					mCollisionAabb;
	VuAabb					mFreezeEffectAabb;
	VuVector3				mCenterOfMass;
	VuVector3				mInertia;
	bool					mAddedToDynamicsWorld;
	VUUINT16				mCollisionMask;
	bool					mAttachedToDropShip;
	float					mDropShipRadialBlur;

	VuStaticModelInstance	*mpModelInstance;
	VuStaticModelInstance	*mpLod1ModelInstance;
	VuStaticModelInstance	*mpLod2ModelInstance;
	float					mShadowValue;
	float					mDrawShadowValue;
	VuColor					mDrawColor;
	VuVector4				mPfxColor;
	VUUINT32				mWaterSurfaceFlags;
	float					mAlpha;
	bool					mDrawColorOverride;
	VuColor					mDrawOverrideColor;

	VuModelSkin				mSkin;
	VuModelSkin				mLod1Skin;
	VuModelSkin				mLod2Skin;

	float					mThrottleControl;
	float					mYawControl;
	float					mYawControlFactor;
	bool					mPowerUpControl;
	bool					mPowerSlideControl;
	int						mPauseControlRequestCount;

	//xlm
	bool					mExtraPowerUpControl1;

	bool					mSimulatedAI;
	bool					mHasCheated;
	bool					mStartCrash;
	bool					mHasCrashed;
	VuWeakRef<VuCarEntity>	mpSmasher;
	VuWeakRef<VuCarEntity>	mpCollider;
	bool					mShatterIce;
	int						mPlace;

	bool					mOutOfBounds;
	float					mOutOfBoundsRecoverTime;
	float					mOutOfBoundsTimer;
	bool					mCollidedThisFrame;
	bool					mCollidedThisFrameWithGround;
	float					mCollidedWithHumanRelSpeed;
	float					mCollisionRagdollThreshold;
	VuVector3				mHollywoodMinLinVel;
	VuVector3				mHollywoodMaxLinVel;
	VuVector3				mHollywoodMinAngVel;
	VuVector3				mHollywoodMaxAngVel;
	float					mCrashRecoveryTime;
	float					mUpsideDownRecoveryTime;
	float					mStuckRecoveryTime;
	float					mStuckSpeedThreshold;
	float					mWaterRecoverySpeed;
	float					mWrongWayRecoveryTime;
	float					mShatterIceThreshold;

	bool					mAnchored;
	VuVector3				mAnchorPosRB;

	float					mRadialBlurAmount;
	float					mLensWaterRadialSpreadAmount;
	float					mSpeedEffectAmount;

	VuGfxSettings			mGfxSettings;

	float					mGhostModeTimer;
	bool					mGhostMode;
	bool					mGhostHide;
	bool					mHidden;
	int						mDisableCollisionsCounter;

	bool					mWaterDeath;

	float					mCrashTimer;
	float					mUpsideDownTimer;
	float					mStuckTimer;
	float					mWrongWayTimer;

	bool					mPowerSliding;
	float					mPowerSlideDirection;
	float					mPowerSlideAngle;
	float					mPowerSlideDuration;

	bool					mIsRacer;
	bool					mIsSpawned;
	bool					mIsCameraTarget;
	bool					mIsTunedUp;

	VuCarStats				mStats;

	int						mPowerUpSlotCount;
	VUUINT32				mDynamicLightGroupMask;

	ScorePopups				mScorePopups;

	typedef std::map<std::string, VuMatrix> MountPoints;
	MountPoints				mMountPoints;
};
