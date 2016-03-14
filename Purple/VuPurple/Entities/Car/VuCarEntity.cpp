//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Prop entity
// 
//*****************************************************************************

#include "VuCarEntity.h"
#include "Driver/VuAiDriverEntity.h"
#include "Driver/VuHumanDriverEntity.h"
#include "Camera/VuCarCamera.h"
#include "Parts/VuCarChassis.h"
#include "Parts/VuCarEngine.h"
#include "Parts/VuCarAnimController.h"
#include "Parts/VuCarPfxController.h"
#include "Parts/VuCarPowerUpController.h"
#include "Parts/VuCarEffectController.h"
#include "Parts/VuCarSuspension.h"
#include "Parts/VuCarHeadlights.h"
#include "Parts/VuCarShadow.h"
#include "VuPurple/Dynamics/VuCollisionTypes.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Managers/VuAchievementManager.h"
#include "VuPurple/Managers/VuOutOfBoundsManager.h"
#include "VuPurple/PowerUps/VuPowerUpManager.h"
#include "VuPurple/PowerUps/VuPowerUpUtil.h"
#include "VuPurple/Track/VuTrackManager.h"
#include "VuPurple/Track/VuTrackSector.h"
#include "VuPurple/Entities/Objects/VuAiWaypointEntity.h"
#include "VuPurple/Entities/Objects/VuBreakablePropEntity.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuPurple/Util/VuCarUtil.h"
#include "VuEngine/Components/3dDraw/Vu3dDrawComponent.h"
#include "VuEngine/Components/3dLayout/Vu3dLayoutComponent.h"
#include "VuEngine/Components/Instigator/VuInstigatorComponent.h"
#include "VuEngine/Properties/VuBasicProperty.h"
#include "VuEngine/Properties/VuEnumProperty.h"
#include "VuEngine/Properties/VuDBEntryProperty.h"
#include "VuEngine/Assets/VuAssetFactory.h"
#include "VuEngine/Assets/VuFluidsMeshAsset.h"
#include "VuEngine/Assets/VuTextureAsset.h"
#include "VuEngine/Assets/VuCollisionMeshAsset.h"
#include "VuEngine/Entities/Water/VuWaterSurfaceEntity.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Managers/VuTriggerManager.h"
#include "VuEngine/Managers/VuGfxSettingsManager.h"
#include "VuEngine/Managers/VuTireTrackManager.h"
#include "VuEngine/Managers/VuLensWaterManager.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/Model/VuAnimatedModelInstance.h"
#include "VuEngine/Gfx/Model/VuStaticModelInstance.h"
#include "VuEngine/Gfx/GfxScene/VuGfxStaticScene.h"
#include "VuEngine/Gfx/GfxSort/VuGfxSort.h"
#include "VuEngine/Gfx/Font/VuFontDraw.h"
#include "VuEngine/Gfx/Light/VuLightManager.h"
#include "VuEngine/HAL/Gfx/VuGfx.h"
#include "VuEngine/HAL/Gfx/VuTexture.h"
#include "VuEngine/Water/VuFluidsObject.h"
#include "VuEngine/Water/VuWater.h"
#include "VuEngine/Dynamics/Util/VuDynamicsRayTest.h"
#include "VuEngine/Math/VuUnitConversion.h"
#include "VuEngine/Math/VuQuaternion.h"
#include "VuEngine/Math/VuMathUtil.h"
#include "VuEngine/Math/VuRand.h"
#include "VuEngine/Util/VuAudioUtil.h"
#include "VuEngine/Util/VuSpreadsheetQuery.h"
#include "VuEngine/Dev/VuDev.h"
#include "VuEngine/Dev/VuDevMenu.h"
#include "VuEngine/Dev/VuDevConfig.h"
#include "VuEngine/Dev/VuDevUtil.h"


IMPLEMENT_RTTI(VuCarEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuCarEntity);


// constants
#define HUMAN_AI_COLLISION_ADVANTAGE 1.0f
#define ANCHOR_SPRING_COEFFICIENT 10.0f
#define ANCHOR_DAMPING_COEFFICIENT 10.0f

#define RADIAL_BLUR_INC_RATE 0.5f // per second
#define RADIAL_BLUR_DEC_RATE 1.0f // per second
#define SPEED_EFFECT_MIN_SPEED 80.0f // mph
#define SPEED_EFFECT_MIN_HEIGHT 5.0f // meters
#define SPEED_EFFECT_MAX_HEIGHT 10.0f // meters
#define SPEED_EFFECT_INC_RATE 2.0f // per second
#define SPEED_EFFECT_DEC_RATE 2.0f // per second

#define RECOVERY_GHOST_TIMER 3.0f
#define GHOST_MODE_FLASH_RATE 10.0f // per second

#define BOOST_METER_ALPHA_1 0.5f
#define BOOST_METER_ALPHA_2 1.0f
#define BOOST_METER_FLASH_RATE 2.0f // per second

#define KILL_PLANE -100.0f // meters

#define SHADOW_TEST_DIST 30.0f // m

#define POST_FINISH_ANCHOR_SPEED 1.0f // mph

#define POWER_SLIDE_STEERING_BLEND_TIME 0.25f // seconds

#define COS_NITRO_ANGULAR_THRESHOLD 0.5f

#define REVERSE_STEERING_THRESHOLD 1.0f // mph


// static variables
static bool sbDebugHUD = false;
static bool sbDebugDraw = false;
static bool sbDebugDrawFluids = true;
static bool sbDebugDrawCollision = false;
static bool sbDebugDrawPfxFluids = false;


class VuWallPenetrationRayTestResult : public VuDynamicsRayTest::VuClosestResult
{
public:
	virtual bool needsCollision(VuRigidBody *pRigidBody)
	{
		if ( pRigidBody->hasContactResponse() )
			if ( pRigidBody->getCollisionGroup() & COL_ENGINE_STATIC_PROP )
				return true;

		return false;
	}
};



//*****************************************************************************
VuCarEntity::VuCarEntity():
	mDriverType("AI"),
	mpDriver(VUNULL),
	mPadIndex(-1),
	mCarStage(0),
	mAccelLevel(0),
	mSpeedLevel(0),
	mHandlingLevel(0),
	mToughLevel(0),
	mLod1Dist(FLT_MAX),
	mLod2Dist(FLT_MAX),
	mDrawDist(FLT_MAX),
	mAmbientColor(VuGameUtil::IF()->getAmbientColor()),
	mDiffuseColor(VuGameUtil::IF()->getDiffuseColor()),
	mMass(500.0f),
	mInertiaFactor(1.0f),
	mMaxSteeringAngle(0.0f),
	mDriverOffset(0,0,0),
	mInducedPowerSlideTractionFactor(1.0f),
	mInducedPowerSlideCoeff(0.0f),
	mInducedPowerSlideSteeringFactor(1.0f),
	mDurability(1.0f),
	mAiBuffAccel(1.0f),
	mAiBuffSpeed(1.0f),
	mAiBuffHandling(1.0f),
	mAiBuffTough(1.0f),
	mCollisionAabb(VuVector3(0,0,0)),
	mFreezeEffectAabb(VuVector3(0,0,0)),
	mCenterOfMass(0,0,0),
	mInertia(0,0,0),
	mAddedToDynamicsWorld(false),
	mCollisionMask(COL_EVERYTHING),
	mAttachedToDropShip(false),
	mDropShipRadialBlur(0),
	mShadowValue(1.0f),
	mDrawShadowValue(1.0f),
	mDrawColor(255,255,255),
	mPfxColor(1,1,1,1),
	mWaterSurfaceFlags(0),
	mAlpha(1.0f),
	mDrawColorOverride(false),
	mThrottleControl(0.0f),
	mYawControl(0.0f),
	mYawControlFactor(1.0f),
	mPowerUpControl(false),
	mExtraPowerUpControl1(false),
	mPowerSlideControl(false),
	mPauseControlRequestCount(0),
	mSimulatedAI(false),
	mHasCheated(false),
	mStartCrash(false),
	mHasCrashed(false),
	mShatterIce(false),
	mPlace(0),
	mOutOfBounds(false),
	mOutOfBoundsRecoverTime(0.0f),
	mOutOfBoundsTimer(0.0f),
	mCollidedThisFrame(false),
	mCollidedThisFrameWithGround(false),
	mCollidedWithHumanRelSpeed(0.0f),
	mCollisionRagdollThreshold(30.0f),
	mHollywoodMinLinVel(0,0,0),
	mHollywoodMaxLinVel(0,0,0),
	mHollywoodMinAngVel(0,0,0),
	mHollywoodMaxAngVel(0,0,0),
	mCrashRecoveryTime(2.0f),
	mUpsideDownRecoveryTime(2.0f),
	mStuckRecoveryTime(2.0f),
	mStuckSpeedThreshold(10.0f),
	mWaterRecoverySpeed(5.0f),
	mWrongWayRecoveryTime(5.0f),
	mShatterIceThreshold(30.0f),
	mAnchored(true),
	mAnchorPosRB(0,0,0),
	mRadialBlurAmount(0.0f),
	mLensWaterRadialSpreadAmount(0.0f),
	mSpeedEffectAmount(0.0f),
	mGhostModeTimer(0.0f),
	mGhostMode(false),
	mGhostHide(false),
	mHidden(false),
	mDisableCollisionsCounter(0),
	mWaterDeath(false),
	mCrashTimer(0.0f),
	mUpsideDownTimer(0.0f),
	mStuckTimer(0.0f),
	mWrongWayTimer(0.0f),
	mPowerSliding(false),
	mPowerSlideAngle(0.0f),
	mPowerSlideDuration(0.0f),
	mIsRacer(false),
	mIsSpawned(false),
	mIsCameraTarget(false),
	mIsTunedUp(false),
	//xlm modify   mPowerUpSlotCount(1),
	mPowerUpSlotCount(1),
	mDynamicLightGroupMask(0xffffffff)
{
	static VuDevBoolOnce sOnce;
	if ( sOnce && VuDevMenu::IF() )
	{
		VuDevMenu::IF()->addBool("Car/DebugHUD", sbDebugHUD);
		VuDevMenu::IF()->addBool("Car/DebugDraw", sbDebugDraw);
		VuDevMenu::IF()->addBool("Car/DebugDrawFluids", sbDebugDrawFluids);
		VuDevMenu::IF()->addBool("Car/DebugDrawCollision", sbDebugDrawCollision);
		VuDevMenu::IF()->addBool("Car/DebugDrawPfxFluids", sbDebugDrawPfxFluids);

		VuDevConfig::IF()->getParam("DebugHUD").getValue(sbDebugHUD);
	}

	// components
	addComponent(mp3dLayoutComponent = new Vu3dLayoutComponent(this));
	addComponent(mp3dDrawComponent = new Vu3dDrawComponent(this, false));
	addComponent(mpInstigatorComponent = new VuInstigatorComponent(this));

	// want to know when transform is changed
	mpTransformComponent->setWatcher(&VuCarEntity::transformModified);
	mpTransformComponent->setMask(VuTransformComponent::TRANS|VuTransformComponent::ROT);

	mp3dLayoutComponent->setDrawMethod(this, &VuCarEntity::drawLayout);
	mp3dDrawComponent->setDrawMethod(this, &VuCarEntity::draw);
	mp3dDrawComponent->setDrawShadowMethod(this, &VuCarEntity::drawShadow);
	mp3dDrawComponent->setDrawPrefetchMethod(this, &VuCarEntity::drawPrefetch);

	// properties
	addProperty(new VuStringProperty("DriverType", mDriverType));
	addProperty(new VuIntProperty("PadIndex", mPadIndex));
	addProperty(new VuStringProperty("NetGameId", mNetGameId));
	addProperty(new VuStringProperty("Car", mCarName));
	addProperty(new VuIntProperty("Stage", mCarStage));
	addProperty(new VuStringProperty("Decal", mDecalName));
	addProperty(new VuStringProperty("PaintColor", mPaintColorName));
	addProperty(new VuStringProperty("DecalColor", mDecalColorName));
	addProperty(new VuStringProperty("Driver", mDriverName));
	addProperty(new VuIntProperty("Accel", mAccelLevel));
	addProperty(new VuIntProperty("Speed", mSpeedLevel));
	addProperty(new VuIntProperty("Handling", mHandlingLevel));
	addProperty(new VuIntProperty("Tough", mToughLevel));
	addProperty(new VuIntProperty("PowerUpSlotCount", mPowerUpSlotCount));
	addProperty(new VuStringProperty("OpponentName", mOpponentName));

	// rigid body
	btRigidBody::btRigidBodyConstructionInfo info(0, this, VUNULL);
	mpRigidBody = new VuCarRigidBody(info, this, COL_GAME_CAR, mCollisionMask);
	mpRigidBody->setContactCallback(this);
	mpRigidBody->setActivationState(DISABLE_DEACTIVATION);

	// camera
	mpCamera = new VuCarCamera(this);

	// parts
	mpChassis = new VuCarChassis(this);
	mpEngine = new VuCarEngine(this);
	mpAnimController = new VuCarAnimController(this);
	mpPfxController = new VuCarPfxController(this);
	mpPowerUpController = new VuCarPowerUpController(this);
	mpEffectController = new VuCarEffectController(this);
	mpSuspension = new VuCarSuspension(this);
	mpHeadlights = new VuCarHeadlights(this);
	mpShadow = new VuCarShadow(this);

	// wheels
	for ( int i = 0; i < 4; i++ )
		mWheels[i].setWheel((VuCarWheel::eWheel)i);

	// models
	mpModelInstance = new VuStaticModelInstance;
	mpLod1ModelInstance = new VuStaticModelInstance;
	mpLod2ModelInstance = new VuStaticModelInstance;

	mpModelInstance->setMaterialSubstIF(&mSkin);
	mpLod1ModelInstance->setMaterialSubstIF(&mLod1Skin);
	mpLod2ModelInstance->setMaterialSubstIF(&mLod2Skin);

	mpModelInstance->enableTranslucentDepth(true);
	mpLod1ModelInstance->enableTranslucentDepth(true);
	mpLod2ModelInstance->enableTranslucentDepth(true);

	// constants
	{
		const VuJsonContainer &carConstants = VuGameUtil::IF()->constantDB()["Car"];
		VuDataUtil::getValue(carConstants["CollisionRagdollThreshold"], mCollisionRagdollThreshold);
		VuDataUtil::getValue(carConstants["HollywoodMinLinVel"], mHollywoodMinLinVel);
		VuDataUtil::getValue(carConstants["HollywoodMaxLinVel"], mHollywoodMaxLinVel);
		VuDataUtil::getValue(carConstants["HollywoodMinAngVel"], mHollywoodMinAngVel);
		VuDataUtil::getValue(carConstants["HollywoodMaxAngVel"], mHollywoodMaxAngVel);
		mCollisionRagdollThreshold = VuMphToMetersPerSecond(mCollisionRagdollThreshold);
		mHollywoodMinLinVel *= VuMphToMetersPerSecond(1.0f);
		mHollywoodMaxLinVel *= VuMphToMetersPerSecond(1.0f);
		mHollywoodMinAngVel = VuDegreesToRadians(mHollywoodMinAngVel);
		mHollywoodMaxAngVel = VuDegreesToRadians(mHollywoodMaxAngVel);

		VuDataUtil::getValue(carConstants["CrashRecoveryTime"], mCrashRecoveryTime);
		VuDataUtil::getValue(carConstants["UpsideDownRecoveryTime"], mUpsideDownRecoveryTime);
		VuDataUtil::getValue(carConstants["StuckRecoveryTime"], mStuckRecoveryTime);
		VuDataUtil::getValue(carConstants["StuckSpeedThreshold"], mStuckSpeedThreshold);
		VuDataUtil::getValue(carConstants["WaterRecoverySpeed"], mWaterRecoverySpeed);
		VuDataUtil::getValue(carConstants["WrongWayRecoveryTime"], mWrongWayRecoveryTime);
		mStuckSpeedThreshold = VuMphToMetersPerSecond(mStuckSpeedThreshold);
		mWaterRecoverySpeed = VuMphToMetersPerSecond(mWaterRecoverySpeed);
		VuDataUtil::getValue(carConstants["ShatterIceThreshold"], mShatterIceThreshold);
		mShatterIceThreshold = VuMphToMetersPerSecond(mShatterIceThreshold);

		const VuJsonContainer &powerUpConstants = VuGameUtil::IF()->constantDB()["PowerUps"];
	}

	// register event handlers
	REG_EVENT_HANDLER(VuCarEntity, OnCameraShake);
	REG_EVENT_HANDLER(VuCarEntity, OnHitByMissile);
	REG_EVENT_HANDLER(VuCarEntity, OnExplosion);
}

//*****************************************************************************
VuCarEntity::~VuCarEntity()
{
	clear();

	delete mpRigidBody;
	mpDriver->removeRef();
	delete mpCamera;
	delete mpChassis;
	delete mpEngine;
	delete mpAnimController;
	delete mpPfxController;
	delete mpPowerUpController;
	delete mpEffectController;
	delete mpSuspension;
	delete mpHeadlights;
	delete mpShadow;
	delete mpModelInstance;
	delete mpLod1ModelInstance;
	delete mpLod2ModelInstance;
}

//*****************************************************************************
void VuCarEntity::onLoad(const VuJsonContainer &data)
{
	// driver
	if ( mDriverType == "Human" )
	{
		if ( mPadIndex >= 0 )
			mpDriver = new VuHumanDriver(this, mPadIndex, false);
		else
			mpDriver = new VuHumanDriver(this, 0, true);
	}
	else if ( mDriverType == "AI" )
	{
		mpDriver = new VuAiDriver(this);
	}

	mpDriver->onLoad(data);

	dataModified();
	transformModified();
}

void					VuCarEntity::setPlace(int place)				
{ 
	mPlace = place; 
}
int						VuCarEntity::getPlace() const				
{ 
	
	return mPlace; 
}

//*****************************************************************************
void VuCarEntity::onGameInitialize()
{
	// register phased ticks
	VuTickManager::IF()->registerHandler(this, &VuCarEntity::tickDecision, "Decision");
	VuTickManager::IF()->registerHandler(this, &VuCarEntity::tickBuild, "Build");

	// show
	mp3dDrawComponent->show();

	addToDynamicsWorld();

	mpDriver->onGameInitialize();
	mpChassis->onGameInitialize();
	mpEngine->onGameInitialize();
	mpAnimController->onGameInitialize();
	mpPfxController->onGameInitialize();
	mpPowerUpController->onGameInitialize(mPowerUpSlotCount);
	mpEffectController->onGameInitialize();
	mpSuspension->onGameInitialize();
	mpHeadlights->onGameInitialize();
	mpShadow->onGameInitialize();

	for ( int i = 0; i < 4; i++ )
		mWheels[i].onGameInitialize();

	VuCarManager::IF()->addCar(this);

	// start instigating
	mpInstigatorComponent->enable();
}

//*****************************************************************************
void VuCarEntity::onGameRelease()
{
	// stop instigating
	mpInstigatorComponent->disable();

	VuCarManager::IF()->removeCar(this);

	// unregister phased ticks
	VuTickManager::IF()->unregisterHandlers(this);

	// hide
	mp3dDrawComponent->hide();

	removeFromDynamicsWorld();

	mpDriver->onGameRelease();
	mpChassis->onGameRelease();
	mpEngine->onGameRelease();
	mpAnimController->onGameRelease();
	mpPfxController->onGameRelease();
	mpPowerUpController->onGameRelease();
	mpEffectController->onGameRelease();
	mpSuspension->onGameRelease();
	mpHeadlights->onGameRelease();
	mpShadow->onGameRelease();

	for ( int i = 0; i < 4; i++ )
		mWheels[i].onGameRelease();
}

//*****************************************************************************
VuFluidsObject *VuCarEntity::getFluidsObject() const
{
	return mpChassis->getFluidsObject();
}

//*****************************************************************************
VuSkeleton *VuCarEntity::getSkeleton() const
{
	//return mpModelInstance->getSkeleton();
	return VUNULL;
}

//*****************************************************************************
const VuMatrix &VuCarEntity::getMountPoint(const char *name) const
{
	MountPoints::const_iterator iter = mMountPoints.find(name);
	if ( iter != mMountPoints.end() )
		return iter->second;

	return VuMatrix::identity();
}

//*****************************************************************************
void VuCarEntity::getDriverTransform(VuMatrix &transform) const
{
	transform = mpTransformComponent->getWorldTransform();
	transform.translateLocal(mDriverOffset);
}

//*****************************************************************************
float VuCarEntity::getMaxForwardSpeed() const
{
	float fMaxForwardSpeed = mpEngine->getBaseMaxForwardSpeedMPH();

	return VuMphToMetersPerSecond(fMaxForwardSpeed);
}

//*****************************************************************************
float VuCarEntity::getThrottleControl() const
{
	if ( getHasFinished() )
	{
		float fRatio = -VuDot(getLinearVelocity(), getModelMatrix().getAxisY());
		fRatio /= getMaxForwardSpeed();
		fRatio = VuClamp(fRatio, -1.0f, 1.0f);
		return fRatio;
	}

	if ( isControlPaused() )
		return 0;

	return mThrottleControl;
}

//*****************************************************************************
float VuCarEntity::getBreakablePenalty() const
{
	if ( mpEffectController->isActive(VuCarEffectController::TOUGHNESS) )
		return 0.0f;

	float penalty = 1.0f/getDurability();
	return penalty;
}

//*****************************************************************************
bool VuCarEntity::isControlPaused() const
{
	return (mPauseControlRequestCount > 0) && (mpDriver && !mpDriver->isAutoDrive()); 
}

//*****************************************************************************
void VuCarEntity::startSimulatedAI()
{
	if ( !mSimulatedAI )
	{
		mSimulatedAI = true;
		removeFromDynamicsWorld();
	}
}

//*****************************************************************************
void VuCarEntity::stopSimulatedAI()
{
	if ( mSimulatedAI )
	{
		mSimulatedAI = false;
		addToDynamicsWorld();
	}
}

//*****************************************************************************
bool VuCarEntity::canUsePowerUp()
{
	if ( mHasCrashed )
		return false;

	if ( mWaterDeath )
		return false;

	return true;
}

//*****************************************************************************
bool VuCarEntity::shouldRecover()
{
	if ( mCrashTimer > mCrashRecoveryTime )
		return true;

	if ( mUpsideDownTimer > mUpsideDownRecoveryTime )
		return true;

	if ( mStuckTimer > mStuckRecoveryTime )
		return true;

	if ( mWaterDeath )
		return true;

	if ( mOutOfBounds && (mOutOfBoundsTimer > mOutOfBoundsRecoverTime) )
		return true;

	if ( mWrongWayTimer > mWrongWayRecoveryTime )
		return true;

	return false;
}

//*****************************************************************************
void VuCarEntity::recover()
{
	VuVector3 recoveryPos;
	const VuTrackSector *pSector = VuTrackManager::IF()->findRecoverySector(getStats().mDistFromStartOfLap, getModelPosition(), recoveryPos);
	if ( pSector )
	{
		VuMatrix mat;
		VuGameUtil::buildSafeMatrixForCar(recoveryPos, pSector->mUnitDir, getCollisionAabb(), mat, true);

		// For other interested parties
		{
			VuParams params;
			params.addEntity(this);
			params.addVector3(getModelPosition());
			params.addVector3(mat.getTrans());
			VuEventManager::IF()->broadcastDelayed(0.0f, true, "OnRecovered", params);
		}

		recoverTo(mat.getTrans(), mat.getEulerAngles(), true, false);

		mpDriver->onRecover();
	}
}

//*****************************************************************************
void VuCarEntity::recoverTo(const VuVector3 &position, const VuVector3 &rotation, bool bGhost, bool bIgnoreTriggers)
{
	// start ghost mode
	if ( bGhost )
	{
		startGhostMode(RECOVERY_GHOST_TIMER);
	}

	// end death
	mWaterDeath = false;
	mHasCrashed = false;

	// end powerups
	mpEffectController->stopAllEffects();

	// stop power sliding
	stopPowerSliding();

	// calc matrix
	VuMatrix mat;
	mat.setEulerAngles(rotation);
	mat.setTrans(position);

	// set transform
	mpTransformComponent->setWorldTransform(mat, false);

	// synchronize rigid body
	VuMatrix matRB = mat;
	matRB.translateLocal(mCenterOfMass);
	mpRigidBody->setLinearVelocity(VuDynamicsUtil::toBtVector3(VuVector3(0,0,0)));
	mpRigidBody->setAngularVelocity(VuDynamicsUtil::toBtVector3(VuVector3(0,0,0)));
	mpRigidBody->setCenterOfMassTransform(VuDynamicsUtil::toBtTransform(matRB));

	// snap the camera
	mpCamera->snap();

	// ignore triggers?
	if ( bIgnoreTriggers )
	{
		mpInstigatorComponent->snap();
	}

	// update anchor position
	if ( mAnchored )
	{
		mAnchorPosRB = matRB.getTrans();
	}

	mpDriver->onTeleported();

	mCollidedThisFrame = false;
	mCollidedThisFrameWithGround = false;

	mStats.mRecoverCount++;
}

//*****************************************************************************
void VuCarEntity::startGhostMode(float duration)
{
	mGhostModeTimer = VuMax(mGhostModeTimer, duration);
	if ( !mGhostMode )
	{
		mGhostMode = true;
		pushDisableCollisions();
	}
}

//*****************************************************************************
void VuCarEntity::hide()
{
	// end powerups
	mpEffectController->stopAllEffects();

	mpEngine->hide();
	mpDriver->hide();
	mp3dDrawComponent->hide();
	removeFromDynamicsWorld();
	mHidden = true;
}

//*****************************************************************************
bool VuCarEntity::isGhostly()
{
	return isInGhostMode() || getEffectController()->isActive(VuCarEffectController::GHOST);
}

//*****************************************************************************
void VuCarEntity::applyTuneUp()
{
	if ( isTunedUp() )
		return;

	mIsTunedUp = true;

	mpPfxController->applyTuneUp();
	mpEngine->applyTuneUp();

	VuAchievementManager::IF()->unlock(12); // Hire a mechanic (use a tuneup)
}

//*****************************************************************************
void VuCarEntity::pushDisableCollisions()
{
	if ( mDisableCollisionsCounter == 0 )
		mpRigidBody->setCollisionMask(mCollisionMask ^ (COL_GAME_CAR|COL_GAME_OBSTACLE|COL_GAME_PLANT|COL_GAME_OIL_SLICK|COL_GAME_SPRING));

	mDisableCollisionsCounter++;
}

//*****************************************************************************
void VuCarEntity::popDisableCollisions()
{
	mDisableCollisionsCounter--;

	if ( mDisableCollisionsCounter == 0 )
		mpRigidBody->setCollisionMask(mCollisionMask);

	VUASSERT(mDisableCollisionsCounter >= 0, "VuCarEntity::popDisableCollisions() mismatch");
}

//*****************************************************************************
void VuCarEntity::attachToDropShip(VuDropShipEntity *pDropShip)
{
	if ( !mAttachedToDropShip )
	{
		mAttachedToDropShip = true;
		mAnchored = false;

		mpRigidBody->setMassProps(0, VuDynamicsUtil::toBtVector3(mInertia));
		mpRigidBody->setCollisionFlags(mpRigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		mpRigidBody->setCollisionMask(COL_NOTHING);
		mpRigidBody->setContactCallback(VUNULL);
		VuDynamics::IF()->unregisterStepCallback(this);
	}
}

//*****************************************************************************
void VuCarEntity::detachFromDropShip(float releaseSpeed)
{
	if ( mAttachedToDropShip )
	{
		mAttachedToDropShip = false;

		VuVector3 linVel = releaseSpeed*mpRigidBody->getVuLinearVelocity().normal();
		mpRigidBody->setVuLinearVelocity(linVel);
		mpRigidBody->setVuAngularVelocity(VuVector3(0,0,0));

		mpRigidBody->setCollisionFlags(mpRigidBody->getCollisionFlags() & (~btCollisionObject::CF_KINEMATIC_OBJECT));
		mpRigidBody->setCollisionMask(mCollisionMask);
		mpRigidBody->setMassProps(mMass, VuDynamicsUtil::toBtVector3(mInertia));
		mpRigidBody->setContactCallback(this);
		VuDynamics::IF()->registerStepCallback(this);
	}
}

//*****************************************************************************
void VuCarEntity::updateDropShip(const VuMatrix &transform, const VuVector3 &linVel, const VuVector3 &angVel, float radialBlur)
{
	if ( mAttachedToDropShip )
	{
		mpTransformComponent->setWorldTransform(transform, false);

		VuMatrix matRB = mpTransformComponent->getWorldTransform();
		matRB.translateLocal(mCenterOfMass);

		// synchronize rigid body
		mpRigidBody->setVuLinearVelocity(linVel);
		mpRigidBody->setVuAngularVelocity(angVel);
		mpRigidBody->setCenterOfMassTransform(VuDynamicsUtil::toBtTransform(matRB));

		mDropShipRadialBlur = radialBlur;
	}
}

//*****************************************************************************
void VuCarEntity::changeDriver(const std::string &driverName)
{
	if ( driverName != mDriverName )
	{
		mDriverName = driverName;

		mpDriver->onGameRelease();
		mpDriver->dataModified(driverName);
		mpDriver->onGameInitialize();
	}
}

//*****************************************************************************
void VuCarEntity::changeSkin(const std::string &decal, const std::string &paintColor, const std::string &decalColor)
{
	if ( decal != mDecalName || paintColor != mPaintColorName || decalColor != mDecalColorName)
	{
		mDecalName = decal;
		mPaintColorName = paintColor;
		mDecalColorName = decalColor;

		updateSkin();
	}
}

//*****************************************************************************
void VuCarEntity::updateSkin()
{
	VuJsonContainer skinData;

	std::string decalTexture;
	if ( VuGameUtil::IF()->getDecal(mDecalName, decalTexture) )
		VuDataUtil::putValue(skinData["Textures"]["DecalTexture"], decalTexture);

	VuColor decalColor;
	if ( VuGameUtil::IF()->getColor(mDecalColorName, decalColor) )
		VuDataUtil::putValue(skinData["Constants"]["DecalColor"], decalColor.toVector3());

	VuColor paintColor;
	if ( VuGameUtil::IF()->getColor(mPaintColorName, paintColor) )
		VuDataUtil::putValue(skinData["Constants"]["PaintColor"], paintColor.toVector3());

	mSkin.build(mpModelInstance->getGfxStaticScene(), skinData);
	mLod1Skin.build(mpLod1ModelInstance->getGfxStaticScene(), skinData);
	mLod2Skin.build(mpLod2ModelInstance->getGfxStaticScene(), skinData);
}

//*****************************************************************************
void VuCarEntity::showScorePopup(int score, const VuVector3 &worldPos)
{
	// find an unused slot
	int slot;
	for (slot = 0; slot < (int)mScorePopups.size(); slot++ )
		if ( mScorePopups[slot].mBoing.isDone() )
			break;

	if ( slot == mScorePopups.size() )
	{
		mScorePopups.resize(mScorePopups.size() + 1);
		mScorePopups.back().mBoing.load(VuGameUtil::IF()->constantDB()["HUD"]["ScoreBoing"]);
	}

	VuVector3 screenPos = getCamera()->worldToScreen(worldPos);

	ScorePopup &popup = mScorePopups[slot];
	popup.mScore = score;
	popup.mScreenPos.mX = VuClamp(screenPos.mX, 0.0f, 1.0f);
	popup.mScreenPos.mY = VuClamp(screenPos.mY, 0.0f, 1.0f);
	popup.mBoing.reset();
}

//*****************************************************************************
void VuCarEntity::addScore(const char *gameType, const char *objectType, const VuVector3 &pos)
{
	if ( (VuGameUtil::IF()->getGameType() == gameType) && !getHasFinished() )
	{
		int score = VuGameUtil::IF()->constantDB()["Games"][gameType]["Scoring"][objectType].asInt();
		mStats.mScore += score;

		showScorePopup(score, pos);
	}
}

//*****************************************************************************
void VuCarEntity::getWorldTransform(btTransform& worldTrans) const
{
	VuMatrix matRB = mpTransformComponent->getWorldTransform();
	matRB.translateLocal(mCenterOfMass);

	worldTrans = VuDynamicsUtil::toBtTransform(matRB);
}

//*****************************************************************************
void VuCarEntity::setWorldTransform(const btTransform& worldTrans)
{
	VuMatrix matModel = VuDynamicsUtil::toVuMatrix(worldTrans);
	matModel.translateLocal(-mCenterOfMass);

	// make sure humans don't penetrate walls
	if ( mpDriver->isHuman() && mpDriver->isLocal() )
	{
		VuVector3 prevPos = mpTransformComponent->getWorldTransform().transform(getCollisionAabb().getCenter());
		VuVector3 newPos = matModel.transform(getCollisionAabb().getCenter());

		VuVector3 frameDelta = newPos - prevPos;
		if ( frameDelta.magSquared() > VuSquare(getCollisionAabb().getExtents().mX) )
		{
			VuWallPenetrationRayTestResult result;
			VuDynamicsRayTest::test(prevPos, newPos, result);
			if ( result.mbHasHit )
			{
				matModel.setTrans(prevPos);

				VuVector3 linVel = getLinearVelocity();
				linVel -= result.mHitNormal*VuDot(result.mHitNormal, linVel);
				mpRigidBody->setVuLinearVelocity(linVel);

				VuMatrix matRB = matModel;
				matRB.translateLocal(mCenterOfMass);
				mpRigidBody->setCenterOfMassTransform(VuDynamicsUtil::toBtTransform(matRB));
			}
		}
	}

	mpTransformComponent->setWorldTransform(matModel, false);
}

//*****************************************************************************
void VuCarEntity::onDynamicsAdvanceEnvironment(float fdt, bool bSimStep)
{
	if ( bSimStep )
	{
		mCollidedThisFrame = false;
		mCollidedThisFrameWithGround = false;
	}
}

//*****************************************************************************
void VuCarEntity::onDynamicsApplyForces(float fdt)
{
	if ( mpRigidBody->getActivationState() != ISLAND_SLEEPING )
	{
		mpChassis->onApplyForces(fdt);
		mpEngine->onApplyForces(fdt);
		mpSuspension->onApplyForces(fdt);
		mpEffectController->onApplyForces(fdt);

		// anchor forces
		if ( mAnchored )
			applyAnchorForces(fdt);
	}
	else
	{
		// stop wheels from rotating
		for ( int iWheel = 0; iWheel < 4; iWheel++ )
			mWheels[iWheel].mCurAngVel = 0.0f;
	}
}

//*****************************************************************************
bool VuCarEntity::onRigidBodyContactAdded(VuContactPoint &cp)
{
	VUUINT32 otherCollisionGroup = cp.mpOtherBody->getCollisionGroup();
	VUUINT32 otherExtendedFlags = cp.mpOtherBody->getExtendedFlags();

	// compute relative velocity
	VuVector3 vRelV = mpRigidBody->getVuLinearVelocity() - cp.mpOtherBody->getVuLinearVelocity();
	float fRelVel = VuDot(cp.mNorWorld, vRelV);

	if ( otherCollisionGroup & COL_GAME_CAR )
	{
		// collision tuning
		if ( otherExtendedFlags & EXT_COL_GAME_RACER )
		{
			if ( getDriver()->isAi() )
			{
				float fImpulse = VuDynamicsUtil::collisionImpulse(mpRigidBody, cp.mpOtherBody, cp.mPosWorld, cp.mNorWorld);
				VuVector3 vImpulse = (HUMAN_AI_COLLISION_ADVANTAGE*fImpulse)*cp.mNorWorld;
				VuDynamicsUtil::applyImpulseWorld(*mpRigidBody, vImpulse, cp.mPosWorld);
				mCollidedWithHumanRelSpeed = fRelVel;
			}
		}

		// handle toughness
		VuCarEntity *pOtherCar = static_cast<VuCarEntity *>(cp.mpOtherBody->getEntity());
		if ( pOtherCar->getEffectController()->isActive(VuCarEffectController::TOUGHNESS) )
		{
			if ( !mStartCrash && !mHasCrashed )
			{
				mpSmasher = pOtherCar;
			}
		}

		mpCollider = pOtherCar;
	}

	if ( otherExtendedFlags & EXT_COL_GAME_THROW_DRIVER )
	{
		if ( VuAbs(fRelVel) > mCollisionRagdollThreshold )
			mStartCrash = true;
	}

	if ( mpEffectController->isActive(VuCarEffectController::NITRO) )
	{
		const VuVector3 &up = VuDynamicsUtil::toVuVector3(mpRigidBody->getWorldTransform().getBasis().getColumn(2));
		if ( VuDot(cp.mNorWorld, up) < COS_NITRO_ANGULAR_THRESHOLD )
		{
			if ( otherCollisionGroup & (COL_ENGINE_STATIC_PROP|COL_ENGINE_DYNAMIC_PROP|COL_GAME_CAR|COL_GAME_OBSTACLE|COL_GAME_CAR_BARRIER) )
			{
				mStartCrash = true;
			}
		}
	}

	if ( mpEffectController->isActive(VuCarEffectController::FREEZE) )
	{
		const VuVector3 &up = VuDynamicsUtil::toVuVector3(mpRigidBody->getWorldTransform().getBasis().getColumn(2));
		if ( VuDot(cp.mNorWorld, up) < COS_NITRO_ANGULAR_THRESHOLD )
		{
			if ( otherCollisionGroup & (COL_ENGINE_STATIC_PROP|COL_ENGINE_DYNAMIC_PROP|COL_GAME_CAR|COL_GAME_OBSTACLE|COL_GAME_CAR_BARRIER) )
			{
				if ( !(otherExtendedFlags & EXT_COL_GAME_MASSLESS_BREAKABLE) )
				{
					if ( (-fRelVel) > mShatterIceThreshold )
						mShatterIce = true;
				}
			}
		}
	}

	mCollidedThisFrame = true;

	if ( otherCollisionGroup & COL_ENGINE_STATIC_PROP )
		if ( !(otherExtendedFlags & (EXT_COL_ENGINE_BREAKABLE | EXT_COL_ENGINE_EXPLODABLE)) )
			mCollidedThisFrameWithGround = true;

	return true;
}

//*****************************************************************************
void VuCarEntity::onDropShadowDraw(const VuGfxDrawShadowParams &params)
{
	const VuMatrix &matModel = mpTransformComponent->getWorldTransform();
	float dist = (matModel.getTrans() - params.mEyePos).mag();

	if ( dist > mDrawDist )
		return;

	// body
	if ( dist > mLod2Dist )
		mpLod2ModelInstance->drawDropShadow(matModel, params);
	else if ( dist > mLod1Dist )
		mpLod1ModelInstance->drawDropShadow(matModel, params);
	else
		mpModelInstance->drawDropShadow(matModel, params);

	// wheels
	for ( int iWheel = 0; iWheel < 4; iWheel++ )
		mWheels[iWheel].drawDropShadow(params, dist);

	// suspension
	mpSuspension->drawDropShadow(params, dist);
}

//*****************************************************************************
void VuCarEntity::OnCameraShake(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);

	float magnitude = accessor.getFloat();
	float duration = accessor.getFloat();
	float falloffTime = accessor.getFloat();
	float frequency = accessor.getFloat();

	if ( !mpEffectController->isActive(VuCarEffectController::TOUGHNESS) )
		mpCamera->beginShake(magnitude, duration, falloffTime, frequency);
}

//*****************************************************************************
void VuCarEntity::OnHitByMissile(const VuParams &params)
{
	if ( mHasCrashed )
		return;

	if ( mGhostMode )
		return;

	VuParams::VuAccessor accessor(params);
	VuVector3 missilePos = accessor.getVector3();
	VuVector3 linVel = accessor.getVector3();
	const char *carEffect = accessor.getString();
	VuEntity *pOriginator = accessor.getEntity();

	mpEffectController->applyEffect(carEffect, pOriginator);
}

//*****************************************************************************
void VuCarEntity::OnExplosion(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	VuVector3 explosionPos = accessor.getVector3();
	float dist = accessor.getFloat();
	const VuJsonContainer &data = *static_cast<const VuJsonContainer *>(accessor.getPointer());
	VuEntity *pOriginator = accessor.getEntity();

	// determine scale based on distance
	float scale = 1.0f - VuLinStep(data["InnerRadius"].asFloat(), data["OuterRadius"].asFloat(), dist);

	// camera shake
	float camShakeMagnitude = data["CamShakeMagnitude"].asFloat();
	float camShakeDuration = data["CamShakeDuration"].asFloat();
	float camShakeFalloffTime = data["CamShakeFalloffTime"].asFloat();
	float camShakeFrequency = data["CamShakeFrequency"].asFloat();
	getCamera()->beginShake(camShakeMagnitude*scale, camShakeDuration, camShakeFalloffTime, camShakeFrequency);

	int viewport;
	if ( VuCarManager::IF()->getViewport(this, viewport) )
		VuLensWaterManager::IF()->addDroplets(viewport, scale*data["LensSplashAmount"].asFloat());

	if ( dist < data["CarEffectDist"].asFloat() )
	{
		if ( pOriginator != this || data["CarEffectSelf"].asBool() )
			mpEffectController->applyEffect(data["CarEffect"].asCString(), pOriginator);
	}
}

//*****************************************************************************
void VuCarEntity::drawLayout(const Vu3dLayoutDrawParams &params)
{
	if ( params.mbDrawCollision )
	{
		VuGfxUtil::IF()->drawAabbSolid(VuColor(255,255,255), mCollisionAabb, mpTransformComponent->getWorldTransform(), params.mCamera.getViewProjMatrix());
	}
	else
	{
		mpModelInstance->draw(mpTransformComponent->getWorldTransform(), VuGfxDrawParams(params.mCamera));
	}
}

//*****************************************************************************
void VuCarEntity::draw(const VuGfxDrawParams &params)
{
	if ( sbDebugHUD 
		&& this == VuCarManager::IF()->getCameraTargetForViewport(0) 
		&& !params.mbDrawReflection)
//		&& getIsRacer() )
		drawDebugHUD();

	if ( sbDebugDraw )
	{
		drawDebug(params.mCamera);
	}
	else
	{
		if ( mGhostMode && mGhostHide )
			return;

		const VuMatrix &matModel = mpTransformComponent->getWorldTransform();

		float dist = (matModel.getTrans() - params.mEyePos).mag();
		if ( dist > mDrawDist )
			return;

		VuColor color = mDrawColor;
		color.mA = VuRound(255*mAlpha);
	
		if ( params.mbDrawReflection )
		{
			mpLod2ModelInstance->setColor(color);
			mpLod2ModelInstance->setDynamicLightGroupMask(mDynamicLightGroupMask);
			mpLod2ModelInstance->draw(matModel, params);
		}
		else
		{
			if ( dist > mLod2Dist )
			{
				mpLod2ModelInstance->setColor(color);
				mpLod2ModelInstance->setDynamicLightGroupMask(mDynamicLightGroupMask);
				mpLod2ModelInstance->draw(matModel, params);
			}
			else if ( dist > mLod1Dist )
			{
				mpLod1ModelInstance->setColor(color);
				mpLod1ModelInstance->setDynamicLightGroupMask(mDynamicLightGroupMask);
				mpLod1ModelInstance->draw(matModel, params);
			}
			else
			{
				mpModelInstance->setColor(color);
				mpModelInstance->setDynamicLightGroupMask(mDynamicLightGroupMask);
				mpModelInstance->draw(matModel, params);
			}

			// draw wheels
			for ( int iWheel = 0; iWheel < 4; iWheel++ )
				mWheels[iWheel].draw(params, color, dist, mDynamicLightGroupMask);

			mpPfxController->draw(params);
			mpEffectController->draw(params);
			mpSuspension->draw(params, color, dist, mDynamicLightGroupMask);
			mpHeadlights->draw(params);
			mpShadow->draw(params);
		}
	}
}

//*****************************************************************************
void VuCarEntity::drawShadow(const VuGfxDrawShadowParams &params)
{
	if ( !sbDebugDraw )
	{
		if ( params.mbDrawReflection )
			return;

		if ( mGhostMode && mGhostHide )
			return;

		if ( mAlpha + FLT_EPSILON < 0.5f )
			return;

		const VuMatrix &matModel = mpTransformComponent->getWorldTransform();

		float dist = (matModel.getTrans() - params.mEyePos).mag();
		if ( dist > mDrawDist )
			return;

		if ( dist > mLod2Dist )
		{
			mpLod2ModelInstance->drawShadow(matModel, params);
		}
		else if ( dist > mLod1Dist )
		{
			mpLod1ModelInstance->drawShadow(matModel, params);
		}
		else
		{
			mpModelInstance->drawShadow(matModel, params);
		}

		// draw wheels
		for ( int iWheel = 0; iWheel < 4; iWheel++ )
			mWheels[iWheel].drawShadow(params, dist);

		mpSuspension->drawShadow(params, dist);
	}
}

//*****************************************************************************
void VuCarEntity::drawPrefetch()
{
	mpLod2ModelInstance->drawPrefetch();
	mpLod1ModelInstance->drawPrefetch();
	mpModelInstance->drawPrefetch();

	for ( int iWheel = 0; iWheel < 4; iWheel++ )
		mWheels[iWheel].drawPrefetch();

	mpSuspension->drawPrefetch();
	mpHeadlights->drawPrefetch();
}

//*****************************************************************************
void VuCarEntity::drawDebugHUD()
{
	// save state
	VUUINT fsl = VuGfxSort::IF()->getFullScreenLayer();
	VUUINT vpl = VuGfxSort::IF()->getViewportLayer();

	// set state
	VuGfxSort::IF()->setFullScreenLayer(VuGfxSort::FSL_HUD);
	VuGfxSort::IF()->setViewportLayer(VuGfxSort::VPL_HUD);

	float speed = VuMetersPerSecondToMph(getLinearVelocity().mag());

	VuFontDrawParams fdParams;
	fdParams.mSize = 25.0f;
	fdParams.mWeight = 120;
	fdParams.mOutlineWeight = 1.0f;
	VuRect rect(0.4f, 0.0f, 0.0f, 0.0f);

	char str[256];
	VU_SPRINTF(str, sizeof(str), "Speed: %d MPH\nGear: %d\nRPM: %d", VuRound(speed), mpEngine->getGear() + 1, VuRound(mpEngine->getAudioRPM()));

	VuGfxUtil::IF()->fontDraw()->drawString(0.0f, VuDev::IF()->getFont(), str, fdParams, rect, 0);

	// restore state
	VuGfxSort::IF()->setFullScreenLayer(fsl);
	VuGfxSort::IF()->setViewportLayer(vpl);
}

//*****************************************************************************
void VuCarEntity::drawDebug(const VuCamera &camera)
{
	VuMatrix matModel = mpTransformComponent->getWorldTransform();

	VuVector3 vCarCOM = matModel.transform(mCenterOfMass);
	float fCarMass = 1.0f/mpRigidBody->getInvMass();
	VuVector3 vSize(0.2f, 0.2f, 0.2f);

	// draw center of mass
	{
		VuMatrix mat = matModel;
		mat.translateLocal(mCenterOfMass);
		VuGfxUtil::IF()->drawAabbLines(VuColor(255,64,64), VuAabb(-vSize, vSize), mat*camera.getViewProjMatrix());
		VuDev::IF()->printf(mat.getTrans(), VUGFX_TEXT_DRAW_HCENTER|VUGFX_TEXT_DRAW_VCENTER, VuColor(255,255,255), "%d", (int)(fCarMass));
	}

	// draw hydrodynamic center
	{
		VuMatrix mat = matModel;
		mat.translateLocal(mpChassis->getHydrodynamicCenter());
		VuGfxUtil::IF()->drawAabbLines(VuColor(64,64,255), VuAabb(-vSize, vSize), mat*camera.getViewProjMatrix());
	}

	// draw collision aabb
	VuGfxUtil::IF()->drawAabbLines(VuColor(128,128,128), mCollisionAabb, matModel*camera.getViewProjMatrix());

	if ( sbDebugDrawFluids )
	{
		if ( mpChassis->getFluidsObject()->getFluidsMeshAsset() )
			mpChassis->getFluidsObject()->getFluidsMeshAsset()->draw(VuColor(255,255,255), matModel);
	}

	VuGfxUtil::IF()->drawAabbSolid(VuColor(255,255,255), mCollisionAabb, matModel, camera.getViewProjMatrix());

	if ( sbDebugDrawPfxFluids )
	{
		if ( mpPfxController->getFluidsObject()->getFluidsMeshAsset() )
			mpPfxController->getFluidsObject()->getFluidsMeshAsset()->draw(VuColor(255,255,255), matModel);
	}

	mpSuspension->drawDebug(camera);

	// wheels
	for ( int iWheel = 0; iWheel < 4; iWheel++ )
	{
		const VuCarWheel &wheel = mWheels[iWheel];

		VuColor color = wheel.mVisualContact ? VuColor(0,0,255) : VuColor(255,0,255);

		VuVector3 wheelPos = wheel.mTransform.getTrans();
		VuVector3 axle = wheel.mTransform.getAxisX();
		VuVector3 contactPoint = wheel.mVisualContactPointWS;

		VuGfxUtil::IF()->drawLine3d(color, wheelPos, wheelPos + axle, camera.getViewProjMatrix());
		if ( wheel.mVisualContact )
			VuGfxUtil::IF()->drawLine3d(color, wheelPos, wheel.mVisualContactPointWS, camera.getViewProjMatrix());
	}
}

//*****************************************************************************
void VuCarEntity::tickDecision(float fdt)
{
	VuVector3 pos = getModelMatrix().transform(getCollisionAabb().getCenter());

	// update water surface flags
	const VuWaterSurface *pWaterSurface = VuWater::IF()->getSurface(pos, true);
	if (pWaterSurface)
		mWaterSurfaceFlags = pWaterSurface->getDesc().mFlags;

	// update gfx settings
	VuGfxSettingsManager::IF()->getSettings(pos, mGfxSettings);

	// update shadow value
	if (!VuDynamicsUtil::getShadowValue(pos, VuVector3(0.0f, 0.0f, -SHADOW_TEST_DIST), mShadowValue))
		if (pWaterSurface)
			mShadowValue = (float)pWaterSurface->getShadow(pos)*(1.0f / 255.0f);
	mDrawShadowValue = (VuGfxUtil::IF()->getShaderLOD() == 0) ? 1.0f : mShadowValue;
	mDrawColor = mDrawColorOverride ? mDrawOverrideColor : VuLerp(mAmbientColor, mDiffuseColor, mDrawShadowValue);
	mPfxColor = VuLerp(mGfxSettings.mPfxAmbientColor.toVector4(), mGfxSettings.mPfxDiffuseColor.toVector4(), mShadowValue);

	// suspension/wheels
	mpSuspension->onTickDecision(fdt);
	updateWheels(fdt);

	// update power ups
	mpPowerUpController->onTickDecision(fdt);

	// update effects
	mpEffectController->onTickDecision(fdt);

	updateGhostMode(fdt);
	mOutOfBounds = VuOutOfBoundsManager::IF()->isOutOfBounds(getModelPosition(), mOutOfBoundsRecoverTime);

	mpDriver->onTickDecision(fdt);

	// update power sliding
	updatePowerSliding(fdt);

	// chassis/engine/headlights/shadow
	mpChassis->onTick(fdt);
	mpEngine->onTick(fdt);
	mpHeadlights->onTick(fdt);
	mpShadow->onTick(fdt);

	// update animation
	mpAnimController->onTickDecision(fdt);

	mpCamera->onTick(fdt);

	// update anchor
	if ( mAnchored && getThrottleControl() > 0.0f )
		mAnchored = false;

	// handle deactivation when finished
	if ( getHasFinished() && getLinearVelocity().mag2d() < VuMphToMetersPerSecond(POST_FINISH_ANCHOR_SPEED) )
	{
		if ( mpRigidBody->getActivationState() != ISLAND_SLEEPING )
			mpRigidBody->forceActivationState(WANTS_DEACTIVATION);
	}

	// update recover
	if ( canRecover() && shouldRecover() )
		recover();

	// fell out of world?
	if ( getModelPosition().mZ < KILL_PLANE )
		recover();

	// smashed?
	if ( mpSmasher.get() )
	{
		mpEffectController->applyEffect(mpSmasher->getEffectController()->getToughnessSmashCarEffect().c_str(), mpSmasher.get());
		mpSmasher = VUNULL;
	}

	// collided?
	if ( mpCollider.get() )
	{
		mpDriver->onCollidedWithCar(mpCollider.get());
		mpCollider = VUNULL;
	}

	// start ragdoll?
	if ( mStartCrash )
	{
		if ( !mHasCrashed )
		{
			mpDriver->startRagdoll();

			onStartRagdoll();

			// hollywood
			VuVector3 vLinVelKick;
			vLinVelKick.mX = VuRand::global().range(mHollywoodMinLinVel.mX, mHollywoodMaxLinVel.mX);
			vLinVelKick.mY = VuRand::global().range(mHollywoodMinLinVel.mY, mHollywoodMaxLinVel.mY);
			vLinVelKick.mZ = VuRand::global().range(mHollywoodMinLinVel.mZ, mHollywoodMaxLinVel.mZ);
			vLinVelKick = getModelMatrix().transformNormal(vLinVelKick);
			mpRigidBody->setLinearVelocity(mpRigidBody->getLinearVelocity() + VuDynamicsUtil::toBtVector3(vLinVelKick));

			VuVector3 vAngVelKick;
			vAngVelKick.mX = VuRand::global().range(mHollywoodMinAngVel.mX, mHollywoodMaxAngVel.mX);
			vAngVelKick.mY = VuRand::global().range(mHollywoodMinAngVel.mY, mHollywoodMaxAngVel.mY);
			vAngVelKick.mZ = VuRand::global().range(mHollywoodMinAngVel.mZ, mHollywoodMaxAngVel.mZ);
			vAngVelKick = getModelMatrix().transformNormal(vAngVelKick);
			mpRigidBody->setAngularVelocity(mpRigidBody->getAngularVelocity() + VuDynamicsUtil::toBtVector3(vAngVelKick));
		}

		mStartCrash = false;
		mHasCrashed = true;
	}
	else if ( mCollidedWithHumanRelSpeed < 0.0f )
	{
		onCollidedWithHuman(mCollidedWithHumanRelSpeed);
	}

	mCollidedWithHumanRelSpeed = 0.0f;

	// update ragdoll timer
	if ( mHasCrashed )
		mCrashTimer += fdt;
	else
		mCrashTimer = 0.0f;

	// update out of bounds timer
	if ( mOutOfBounds )
		mOutOfBoundsTimer += fdt;
	else
		mOutOfBoundsTimer = 0.0f;

	updateWaterDeath(fdt);

	// update upside-down timer
	if ( getModelMatrix().getAxisZ().mZ < 0.5f )
		mUpsideDownTimer += fdt;
	else
		mUpsideDownTimer = 0.0f;

	// stuck upside-down timer
	if ( getThrottleControl() > 0.9f && getLinearVelocity().magSquared() < VuSquare(mStuckSpeedThreshold) )
		mStuckTimer += fdt;
	else
		mStuckTimer = 0.0f;

	// wrong-way timer
	if ( mpDriver->headingWrongWay() )
		mWrongWayTimer += fdt;
	else
		mWrongWayTimer = 0.0f;

	float horzFrameDist = getLinearVelocity().mag2d()*fdt;

	// update jump distance
	if ( mpSuspension->getWheelContactCount() || getFluidsObject()->isSubmerged() || mCollidedThisFrameWithGround )
	{
		if ( (mStats.mCurJumpDistance > 400.0f) && mpDriver->isHuman() )
			VuAchievementManager::IF()->unlock(42); // Jump 400 meters

		mStats.mMaxJumpDistance = VuMax(mStats.mMaxJumpDistance, (float)mStats.mCurJumpDistance);
		mStats.mCurJumpDistance = 0.0f;
	}
	else
	{
		mStats.mCurJumpDistance += horzFrameDist;
	}

	mShatterIce = false;

	for ( auto &scorePopup : mScorePopups )
		if ( !scorePopup.mBoing.isDone() )
			scorePopup.mBoing.tick(fdt);

	// make sure that we always 'check collide with'... (disabled when last constraint is removed)
	mpRigidBody->ensureCheckCollideWith();
}

//*****************************************************************************
void VuCarEntity::tickBuild(float fdt)
{
	// update driver
	mpDriver->onTickBuild(fdt);

	// update animation
	mpAnimController->onTickBuild(fdt);

	// update driver
	mpSuspension->onTickBuild(fdt);

	// update pfx
	mpPfxController->onTickBuild(fdt);

	// update visibility
	{
		const VuMatrix &modelMat = mpTransformComponent->getWorldTransform();
		VuAabb aabb(mpModelInstance->getAabb(), modelMat);
		if ( mpPfxController->getAabb().isValid() )
			aabb.addAabb(mpPfxController->getAabb());
		aabb.addAabb(mpSuspension->getAabb(), modelMat);
		aabb.addAabb(mpHeadlights->getAabb(), modelMat);
		for ( int iWheel = 0; iWheel < 4; iWheel++ )
		{
			VuCarWheel &wheel = mWheels[iWheel];
			aabb.addAabb(wheel.mpModelInstance->getAabb(), wheel.mTransform);
		}
		mp3dDrawComponent->updateVisibility(aabb);
	}

	// update effects
	updateSpeedEffect(fdt);

	// calculate dynamic light group mask (don't be affected by my own headlight)
	int viewport;
	if (VuCarManager::IF()->getViewport(this, viewport))
	{
		VUUINT32 myLightGroup = 1 << (viewport + 1);
		mDynamicLightGroupMask = ~myLightGroup;
	}
	else
	{
		mDynamicLightGroupMask = 0xffffffff;
	}
}

//*****************************************************************************
void VuCarEntity::updateGhostMode(float fdt)
{
	if ( mGhostMode )
	{
		int binary = VuRound(mGhostModeTimer*GHOST_MODE_FLASH_RATE)%2;
		mGhostHide = binary ? true : false;

		mGhostModeTimer -= fdt;
		if ( mGhostModeTimer < 0 )
		{
			popDisableCollisions();
			mGhostMode = false;
			mGhostModeTimer = 0.0f;
		}
	}
}

//*****************************************************************************
void VuCarEntity::updateWheels(float fdt)
{
	float curEngineForce = mpEngine->getCurEngineForce();
	float curBrakingForce = mpEngine->getCurBrakingForce();

	if ( getHasFinished() )
	{
		curEngineForce = 0.0f;
		curBrakingForce = mpEngine->getMaxBrakingForce();
	}

	for ( int i = 0; i < 4; i++ )
	{
		VuCarWheel &wheel = mWheels[i];

		// steering
		if ( wheel.mIsFrontWheel )
		{
			float maxSteeringAngle = VuDegreesToRadians(mMaxSteeringAngle);
			wheel.mSteeringAngle = -maxSteeringAngle*getYawControl();

			float fForwardSpeed = VuDot(getLinearVelocity(), getModelMatrix().getAxisY());
			float ratio = VuClamp(fForwardSpeed/REVERSE_STEERING_THRESHOLD, -1.0f, 1.0f);
			wheel.mSteeringAngle *= ratio;

			if ( mPowerSliding )
			{
				float steeringAngle = VuClamp(mPowerSlideAngle, -maxSteeringAngle, maxSteeringAngle);
				wheel.mSteeringAngle = VuLerp(wheel.mSteeringAngle, steeringAngle, VuMin(mPowerSlideDuration/POWER_SLIDE_STEERING_BLEND_TIME, 1.0f));
			}
		}

		// forces
		wheel.mEngineForce = curEngineForce/4;
		wheel.mBrakingForce = curBrakingForce/4;

		VuVector3 wheelScale(wheel.mCurScale);
		wheelScale.mX *= wheel.mCurScaleX;

		// update transform
		wheel.mTransform = mpTransformComponent->getWorldTransform();
		wheel.mTransform.translateLocal(wheel.mAttachmentPointLS + VuVector3(0.0f, 0.0f, wheel.mVisualSuspensionDist));
		wheel.mTransform.rotateZLocal(wheel.mSteeringAngle);
		wheel.mTransform.scaleLocal(VuVector3(1.0f, 1.0f, wheel.mVisualScaleZ));
		wheel.mTransform.rotateXLocal(wheel.mCurRotation);
		wheel.mTransform.scaleLocal(wheelScale);
		if ( wheel.mFlipModel )
			wheel.mTransform.rotateZLocal(VU_PI);
	}

	// water heights
	{
		VuWaterPhysicsVertex waterVerts[4];

		VuAabb aabb;
		for ( int i = 0; i < 4; i++ )
		{
			waterVerts[i].mPosition = mWheels[i].mTransform.getTrans();
			aabb.addPoint(waterVerts[i].mPosition);
		}

		VuWaterSurfaceDataParams wsdParams(VuWaterSurfaceDataParams::VT_PHYSICS);

		wsdParams.mVertCount = 4;
		wsdParams.mBoundingAabb = aabb;
		wsdParams.mBoundingCenter = aabb.getCenter();
		wsdParams.mBoundingRadius = aabb.getExtents().mag();
		wsdParams.mpPhysicsVertex = waterVerts;
		wsdParams.mStride = sizeof(waterVerts[0]);

		VuWater::IF()->getSurfaceData(wsdParams);

		for ( int i = 0; i < 4; i++ )
		{
			VuCarWheel &wheel = mWheels[i];

			wheel.mVisualWaterHeight = waterVerts[i].mHeight;
			wheel.mSubmerged = (waterVerts[i].mPosition.mZ - wheel.getRadius()) < waterVerts[i].mHeight;
		}
	}

	// update tire tracks
	for ( int i = 0; i < 4; i++ )
	{
		VuCarWheel &wheel = mWheels[i];

		bool contact = wheel.mVisualContact && !(wheel.mVisualContactExtendedFlags & EXT_COL_ENGINE_BREAKABLE);
		bool skid = wheel.mSideSlide | wheel.mEngineSlide | wheel.mBrakeSlide;
		VuVector3 vel = VuDynamicsUtil::pointVelocityWorld(*mpRigidBody, wheel.mVisualContactPointWS);
		VuTireTrackManager::IF()->updateTireTrack(wheel.mpTireTrack, contact, skid, wheel.mVisualContactSurfaceType, wheel.mVisualContactPointWS, wheel.mVisualContactNormal, vel, mShadowValue, wheel.mCurScaleX);
	}
}

//*****************************************************************************
void VuCarEntity::updateSpeedEffect(float fdt)
{
	// radial blur
	{
		// calculate desired radial blur amount
		float amount = 0.0f;
		if ( mAttachedToDropShip )
		{
			amount = mDropShipRadialBlur;
		}
		else if ( mpEffectController->isActive(VuCarEffectController::BOOST) )
		{
			float speedMin = VuMphToMetersPerSecond(20.0f);
			float speedMax = VuMphToMetersPerSecond(40.0f);
			amount = (getLinearVelocity().mag() - speedMin)/(speedMax - speedMin);
			amount = VuClamp(amount, 0.0f, 0.25f);
		}

		// smooth over time
		if ( amount > mRadialBlurAmount )
			mRadialBlurAmount += fdt*RADIAL_BLUR_INC_RATE;
		else
			mRadialBlurAmount -= fdt*RADIAL_BLUR_DEC_RATE;
		mRadialBlurAmount = VuClamp(mRadialBlurAmount, 0.0f, 1.0f);
	}

	// lens water
	{
		mLensWaterRadialSpreadAmount = mpCamera->getListenerVelocity().mag()/getMaxForwardSpeed() + 3.0f*mRadialBlurAmount;
	}

	// speed
	{
		float amount = (getFluidsObject()->getMinWaterHeight() - SPEED_EFFECT_MIN_HEIGHT)/(SPEED_EFFECT_MAX_HEIGHT - SPEED_EFFECT_MIN_HEIGHT);
		amount = VuClamp(amount, 0.0f, 1.0f);
		float fMinSpeed = VuMphToMetersPerSecond(SPEED_EFFECT_MIN_SPEED);
		amount = VuSelect(getLinearVelocity().magSquared() - fMinSpeed*fMinSpeed, amount, 0.0f);

		// smooth over time
		mSpeedEffectAmount += fdt*VuSelect(mSpeedEffectAmount - amount, -SPEED_EFFECT_DEC_RATE, SPEED_EFFECT_INC_RATE);
		mSpeedEffectAmount = VuClamp(mSpeedEffectAmount, 0.0f, 1.0f);
	}
}

//*****************************************************************************
void VuCarEntity::updateWaterDeath(float fdt)
{
	if ( getFluidsObject()->isSubmerged() && mpSuspension->getWheelContactCount() == 0 && !mGhostMode )
	{
		if ( getLinearVelocity().mag() < mWaterRecoverySpeed )
		{
			mWaterDeath = true;
		}

		if ( getWaterSurfaceFlags() & WATER_SURFACE_LAVA )
		{
			if ( !getEffectController()->isActive(VuCarEffectController::ON_FIRE) )
			{
				getEffectController()->applyEffect("LavaBurn", VUNULL);
			}
		}
	}
}

//*****************************************************************************
void VuCarEntity::updatePowerSliding(float fdt)
{
	if ( mPowerSliding ) 
	{
		mPowerSlideAngle = calcPowerSlideAngle();
		mPowerSlideDuration += fdt;

		bool stop = false;

		if ( mPowerSlideDirection*getYawControl() < -0.5f )
			stop = true;

		if ( getLinearVelocity().mag() < VuMphToMetersPerSecond(50.0f) )
			stop = true;

		if ( mpSuspension->getWheelContactCount() )
			if ( VuAbs(getYawControl()) < 0.5f )
				if ( mPowerSlideDirection*mPowerSlideAngle < VuDegreesToRadians(10.0f) )
					stop = true;

		if ( stop )
			stopPowerSliding();
	}
	else
	{
		if ( getPowerSlideControl() )
		{
			if ( mpSuspension->getWheelContactCount() )
			{
				if ( VuAbs(getYawControl()) > 0.5f )
				{
					const VuVector3 &linVel = getLinearVelocity();
					if ( linVel.mag() > VuMphToMetersPerSecond(50.0f) )
					{
						startPowerSliding();
					}
				}
			}
		}
	}
}

//*****************************************************************************
void VuCarEntity::startPowerSliding()
{
	if ( !mPowerSliding )
	{
		mPowerSliding = true;
		mPowerSlideDirection = getYawControl() > 0.5f ? 1.0f : -1.0f;
		mPowerSlideAngle = calcPowerSlideAngle();
		mPowerSlideDuration = 0.0f;

		mpSuspension->setInducedPowerSlideTractionFactor(mInducedPowerSlideTractionFactor);
		mpSuspension->setInducedPowerSlideCoeff(mInducedPowerSlideCoeff);
		mpChassis->setInducedPowerSlideSteeringFactor(mInducedPowerSlideSteeringFactor);
	}
}

//*****************************************************************************
void VuCarEntity::stopPowerSliding()
{
	if ( mPowerSliding )
	{
		mPowerSliding = false;

		mpSuspension->setInducedPowerSlideTractionFactor(1.0f);
		mpSuspension->setInducedPowerSlideCoeff(0.0f);
		mpChassis->setInducedPowerSlideSteeringFactor(1.0f);
	}
}

//*****************************************************************************
void VuCarEntity::setData(const VuJsonContainer &data)
{
	VuDataUtil::getValue(data["Model Asset"], mModelAssetName);
	VuDataUtil::getValue(data["LOD 1 Model Asset"], mLod1ModelAssetName);
	VuDataUtil::getValue(data["LOD 2 Model Asset"], mLod2ModelAssetName);
	VuDataUtil::getValue(data["LOD 1 Distance"], mLod1Dist);
	VuDataUtil::getValue(data["LOD 2 Distance"], mLod2Dist);
	VuDataUtil::getValue(data["Draw Distance"], mDrawDist);
	VuDataUtil::getValue(data["Surface Type"], mSurfaceType);
	VuDataUtil::getValue(data["Mass"], mMass);
	VuDataUtil::getValue(data["Inertia Factor"], mInertiaFactor);
	VuDataUtil::getValue(data["Max Steering Angle"], mMaxSteeringAngle);
	VuDataUtil::getValue(data["Shadow Texture Asset"], mShadowTextureAssetName);
	VuDataUtil::getValue(data["Driver Offset"], mDriverOffset);
	VuDataUtil::getValue(data["Power Slide Traction Factor"], mInducedPowerSlideTractionFactor);
	VuDataUtil::getValue(data["Power Slide Coeff"], mInducedPowerSlideCoeff);
	VuDataUtil::getValue(data["Power Slide Steering Factor"], mInducedPowerSlideSteeringFactor);
	VuDataUtil::getValue(data["Collision Aabb"], mCollisionAabb);
	VuDataUtil::getValue(data["Freeze Effect Aabb"], mFreezeEffectAabb);

	// parts
	mpCamera->setData(data["Camera"]);
	mpChassis->setData(data["Chassis"]);
	mpEngine->setData(data["Engine"]);
	mpAnimController->setData(data["Animation"]);
	mpPfxController->setData(data["Pfx"]);
	mpSuspension->setData(data["Suspension"]);
	mpHeadlights->setData(data["Headlights"]);

	// wheels
	mWheels[VuCarWheel::FRONT_LEFT].setData(data["FrontWheels"]);
	mWheels[VuCarWheel::FRONT_RIGHT].setData(data["FrontWheels"]);
	mWheels[VuCarWheel::REAR_LEFT].setData(data["RearWheels"]);
	mWheels[VuCarWheel::REAR_RIGHT].setData(data["RearWheels"]);

	// mount points
	const VuJsonContainer &mountPoints = data["Mount Points"];
	for ( int i = 0; i < mountPoints.numMembers(); i++ )
	{
		const std::string &name = mountPoints.getMemberKey(i);
		VuPowerUpUtil::readTransform(mountPoints[name], mMountPoints[name]);
	}
}

//*****************************************************************************
void VuCarEntity::dataModified()
{
	VuGfxSort::IF()->flush();

	clear();

	// notify parts
	mpCamera->preDataModified();
	mpChassis->preDataModified();
	mpEngine->preDataModified();
	mpAnimController->preDataModified();
	mpPfxController->preDataModified();
	mpSuspension->preDataModified();
	mpHeadlights->preDataModified();
	mpShadow->preDataModified();

	// notify wheels
	mWheels[VuCarWheel::FRONT_LEFT].preDataModified();
	mWheels[VuCarWheel::FRONT_RIGHT].preDataModified();
	mWheels[VuCarWheel::REAR_LEFT].preDataModified();
	mWheels[VuCarWheel::REAR_RIGHT].preDataModified();

	// set data
	const VuJsonContainer &defaultData = VuGameUtil::IF()->carDB()["Default"];
	const VuJsonContainer &carData = VuGameUtil::IF()->carDB()[mCarName];
	const VuJsonContainer &stageData = carData["Stages"][mCarStage];

	setData(defaultData);
	setData(carData);
	setData(stageData);

	// toughness
	mDurability = VuGameUtil::IF()->interpolateCarLevel(getCarName(), getCarStage(), getToughLevel(), "Durability");

	// create model
	mpModelInstance->setModelAsset(mModelAssetName);
	mpLod1ModelInstance->setModelAsset(mLod1ModelAssetName);
	mpLod2ModelInstance->setModelAsset(mLod2ModelAssetName);

	// create collision shape
	btCollisionShape *pCollisionShape = new btBoxShape(VuDynamicsUtil::toBtVector3(mCollisionAabb.getExtents()));
	mCenterOfMass = mCollisionAabb.getCenter();

	// calculate inertia
	VuVector3 vInertia;
	{
		VuVector3 vSize = mCollisionAabb.getSize();
		if ( vSize.mX < FLT_EPSILON )
		{
			VuAabb fluidsAabb;
			mpChassis->getFluidsObject()->getAabb(fluidsAabb);
			vSize = fluidsAabb.getSize();
		}

		vInertia.mX = (vSize.mY*vSize.mY + vSize.mZ*vSize.mZ)*mMass/12.0f;
		vInertia.mY = (vSize.mX*vSize.mX + vSize.mZ*vSize.mZ)*mMass/12.0f;
		vInertia.mZ = (vSize.mX*vSize.mX + vSize.mY*vSize.mY)*mMass/12.0f;
	}
	mInertia = mInertiaFactor*vInertia;

	// update rigid body
	mpRigidBody->setMassProps(mMass, VuDynamicsUtil::toBtVector3(mInertia));
	mpRigidBody->setCollisionShape(pCollisionShape);
	if ( VuDynamics::IF() )
	{
		mpRigidBody->setSurfaceType(mSurfaceType.c_str());
		mpRigidBody->setGravity(VuDynamicsUtil::toBtVector3(VuDynamics::IF()->getGravity()));
	}
	VUUINT32 extendedFlags = EXT_COL_ENGINE_DETECT_EXPLOSIONS | EXT_COL_ENGINE_REACT_TO_EXPLOSIONS;
	if ( mpDriver->isHuman() ) extendedFlags |= EXT_COL_GAME_HUMAN_DRIVER;
	if ( mpDriver->isAi() )    extendedFlags |= EXT_COL_GAME_AI_DRIVER;
	if ( getIsRacer() )        extendedFlags |= EXT_COL_GAME_RACER;
	mpRigidBody->setExtendedFlags(extendedFlags);
	//if ( getDriver()->isHuman() )
	//{
	//	mpRigidBody->setCcdSweptSphereRadius(mCollisionAabb.getExtents().mZ);
	//	mpRigidBody->setCcdMotionThreshold(mCollisionAabb.getExtents().mZ);
	//}

	// update driver
	mpDriver->dataModified(mDriverName);

	// update parts
	mpCamera->postDataModified();
	mpChassis->postDataModified();
	mpEngine->postDataModified();
	mpAnimController->postDataModified();
	mpPfxController->postDataModified();
	mpSuspension->postDataModified();
	mpHeadlights->postDataModified();
	mpShadow->postDataModified();

	// update wheels
	mWheels[VuCarWheel::FRONT_LEFT].postDataModified();
	mWheels[VuCarWheel::FRONT_RIGHT].postDataModified();
	mWheels[VuCarWheel::REAR_LEFT].postDataModified();
	mWheels[VuCarWheel::REAR_RIGHT].postDataModified();

	// update instigator
	{
		VUUINT32 mask = VuTriggerManager::getTypeMask("Any Jet Ski");
		if ( mpDriver->isLocal() && getIsRacer() ) mask |= VuTriggerManager::getTypeMask("Local Racer");
		if ( getIsRacer() ) mask |= VuTriggerManager::getTypeMask("Any Racer");
		mpInstigatorComponent->setMask(mask);
	}

	mpInstigatorComponent->setRadius(mCollisionAabb.getExtents().mX);
	mpInstigatorComponent->setOffset(mCollisionAabb.getCenter());

	// update skin
	updateSkin();
}

//*****************************************************************************
void VuCarEntity::transformModified()
{
	VuMatrix matRB = mpTransformComponent->getWorldTransform();
	matRB.translateLocal(mCenterOfMass);

	// synchronize rigid body
	mpRigidBody->setLinearVelocity(VuDynamicsUtil::toBtVector3(VuVector3(0,0,0)));
	mpRigidBody->setAngularVelocity(VuDynamicsUtil::toBtVector3(VuVector3(0,0,0)));
	mpRigidBody->setCenterOfMassTransform(VuDynamicsUtil::toBtTransform(matRB));

	// snap
	mpCamera->snap();
	mpInstigatorComponent->snap();

	// update anchor position
	if ( mAnchored )
	{
		mAnchorPosRB = matRB.getTrans();
	}
}

//*****************************************************************************
void VuCarEntity::clear()
{
	// clear rigid body
	mMass = 0;
	mCenterOfMass = VuVector3(0,0,0);

	// remove old collision shape
	if ( mpRigidBody->getCollisionShape() )
	{
		delete mpRigidBody->getCollisionShape();
		mpRigidBody->setCollisionShape(VUNULL);
	}
}

//*****************************************************************************
void VuCarEntity::removeFromDynamicsWorld()
{
	if ( mAddedToDynamicsWorld )
	{
		// remove from dynamics world
		VuDynamics::IF()->removeRigidBody(mpRigidBody);

		// unregister dynamics methods
		VuDynamics::IF()->unregisterStepCallback(this);

		mAddedToDynamicsWorld = false;
	}
}

//*****************************************************************************
void VuCarEntity::addToDynamicsWorld()
{
	if ( !mAddedToDynamicsWorld )
	{
		// add to dynamics world
		VuDynamics::IF()->addRigidBody(mpRigidBody);

		// register dynamics methods
		VuDynamics::IF()->registerStepCallback(this);

		mAddedToDynamicsWorld = true;
	}
}

//*****************************************************************************
void VuCarEntity::applyAnchorForces(float fdt)
{
	VuMatrix xform = mpRigidBody->getVuCenterOfMassTransform();
	VuVector3 rot = xform.getEulerAngles();

	// anchor force
	VuVector3 delta = mAnchorPosRB - xform.getTrans();
	VuVector3 linVel = getLinearVelocity();
	VuVector3 anchorForce = ANCHOR_SPRING_COEFFICIENT*mMass*VuVector3(delta.mX, delta.mY, 0);
	anchorForce -= ANCHOR_DAMPING_COEFFICIENT*mMass*VuVector3(linVel.mX, linVel.mY, 0);
	VuDynamicsUtil::applyCentralForce(*mpRigidBody, anchorForce);

	// stop wheels from rotating
	for ( int iWheel = 0; iWheel < 4; iWheel++ )
		mWheels[iWheel].mCurAngVel = 0.0f;
}

//*****************************************************************************
float VuCarEntity::calcPowerSlideAngle()
{
	if ( mpSuspension->getWheelContactCount() )
	{
		const VuVector3 &vNormal = mpSuspension->getAvgContactNormal();

		// determine velocity on ground plane
		VuVector3 vVel = getLinearVelocity();
		vVel -= vNormal*VuDot(vVel, vNormal);

		// determine heading on ground plane
		VuVector3 vHeading = getModelMatrix().getAxisY();
		vHeading -= vNormal*VuDot(vHeading, vNormal);

		if ( vVel.magSquared() > FLT_EPSILON && vHeading.magSquared() > FLT_EPSILON )
		{
			vVel.normalize();
			vHeading.normalize();

			float cosAngle = VuDot(vVel, vHeading);
			float sign = VuSelect(VuDot(VuCross(vHeading, vVel), vNormal), 1.0f, -1.0f);

			return sign*VuACos(VuMin(cosAngle, 1.0f));
		}
	}

	return 0.0f;
}

//*****************************************************************************
VuCarRigidBody::VuCarRigidBody(const btRigidBody::btRigidBodyConstructionInfo &info, VuEntity *pEntity, VUINT16 collisionGroup, VUINT16 collisionMask):
	VuRigidBody(info, pEntity, collisionGroup, collisionMask)
{
	m_checkCollideWith = true;
}

//*****************************************************************************
bool VuCarRigidBody::checkCollideWithOverride(const  btCollisionObject* co) const
{
	if ( co->getInternalType() == btCollisionObject::CO_RIGID_BODY )
	{
		const VuRigidBody *pOtherBody = static_cast<const VuRigidBody *>(co);
		if ( pOtherBody->getCollisionGroup() & COL_ENGINE_RAGDOLL )
		{
			VuCarEntity *pMyCar = static_cast<VuCarEntity *>(getEntity());
			if ( pMyCar->getDriver() == pOtherBody->getEntity() )
				return false;
		}
	}

	return true;
}
