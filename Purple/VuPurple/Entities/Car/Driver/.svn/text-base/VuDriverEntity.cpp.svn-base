//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Driver base class
// 
//*****************************************************************************

#include "VuDriverEntity.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Track/VuTrackSector.h"
#include "VuPurple/Pfx/Processes/VuPfxEmitRagdollSplash.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Util/VuGameUtil.h"

#include "VuEngine/Components/3dDraw/Vu3dDrawComponent.h"
#include "VuEngine/Assets/VuAssetFactory.h"
#include "VuEngine/Assets/VuAnimationAsset.h"
#include "VuEngine/Assets/VuTextureAsset.h"
#include "VuEngine/Animation/VuAnimatedSkeleton.h"
#include "VuEngine/Animation/VuAnimationControl.h"
#include "VuEngine/Animation/VuAnimation.h"
#include "VuEngine/Dynamics/VuRagdoll.h"
#include "VuEngine/Gfx/Model/VuAnimatedModelInstance.h"
#include "VuEngine/Gfx/Model/VuStaticModelInstance.h"
#include "VuEngine/Pfx/VuPfx.h"
#include "VuEngine/Pfx/VuPfxSystem.h"
#include "VuEngine/Pfx/VuPfxPattern.h"
#include "VuEngine/Pfx/VuPfxProcess.h"
#include "VuEngine/HAL/Gfx/VuGfx.h"
#include "VuEngine/Json/VuJsonContainer.h"
#include "VuEngine/Math/VuMathUtil.h"
#include "VuEngine/Math/VuRand.h"
#include "VuEngine/Math/VuUnitConversion.h"
#include "VuEngine/Water/VuWater.h"
#include "VuEngine/Water/VuFluidsObject.h"
#include "VuEngine/Util/VuDataUtil.h"
#include "VuEngine/Util/VuSpreadsheetQuery.h"
#include "VuEngine/Dev/VuDevMenu.h"
#include "VuEngine/Dev/VuDevUtil.h"

#include "VuPurple/AI/VuAIUtils.h"	// for auto-drive


IMPLEMENT_RTTI(VuDriverEntity, VuEntity);


// static variables
static bool sbDebug = false;
static bool sbDebugBones = true;
static bool sbDebugBoneNames = false;
static bool sbDebugRagdoll = true;

#define BLEND_TIME 0.25f // seconds
#define TURN_SMOOTH_TIME 0.1f
#define IMPACT_DELTA_V 10.0f // mph
#define PASSING_DISTANCE 10.0f // meters

#define DEBUG_BONE_SIZE 0.1f


//*****************************************************************************
VuDriverEntity::VuDriverEntity(VuCarEntity *pCar):
	mLod1Dist(FLT_MAX),
	mLod2Dist(FLT_MAX),
	mDrawDist(FLT_MAX),
	mAngryRepeatTime(0.0f),
	mHappyRepeatTime(0.0f),
	mpCar(pCar),
	mpAnimatedSkeleton(VUNULL),
	mpTurnAnimControl(VUNULL),
	mpCurAnimControl(VUNULL),
	mCurTurnPos(0.0f),
	mCurTurnRate(0.0f),
	mBlendRate(0.0f),
	mAnimWeight(0.0f),
	mpRagdollPfx(VUNULL),
	mRagdollType("Default"),
	mRagdollCameraTargetBodyIndex(0),
	mpPortraitImage(VUNULL),
	mpAbilityImage(VUNULL),
	mNextEffectAnimId(1),
	mAbilityChargeTime(FLT_MAX),
	mAngryTimer(0.0f),
	mHappyTimer(0.0f),
	mAutoDrive(false),
	mPrevCarVelocity(0,0,0),
	mPrevPlace(0)
{
	static VuDevBoolOnce sOnce;
	if ( sOnce && VuDevMenu::IF() )
	{
		VuDevMenu::IF()->addBool("Driver/Debug", sbDebug);
		VuDevMenu::IF()->addBool("Driver/DebugDrawBones", sbDebugBones);
		VuDevMenu::IF()->addBool("Driver/DebugDrawBoneNames", sbDebugBoneNames);
		VuDevMenu::IF()->addBool("Driver/DebugRagdoll", sbDebugRagdoll);
	}

	// components
	addComponent(mp3dDrawComponent = new Vu3dDrawComponent(this, false));

	mp3dDrawComponent->setDrawMethod(this, &VuDriverEntity::draw);
	mp3dDrawComponent->setDrawShadowMethod(this, &VuDriverEntity::drawShadow);
	mp3dDrawComponent->setDrawPrefetchMethod(this, &VuDriverEntity::drawPrefetch);

	// models
	mpModelInstance = new VuAnimatedModelInstance;
	mpLod1ModelInstance = new VuAnimatedModelInstance;
	mpLod2ModelInstance = new VuStaticModelInstance;

	mpModelInstance->enableTranslucentDepth(true);
	mpLod1ModelInstance->enableTranslucentDepth(true);
	mpLod2ModelInstance->enableTranslucentDepth(true);

	mpLod2ModelInstance->setRejectionScaleModifier(0.0f);

	// ragdoll
	mpRagdoll = new VuRagdoll;

	createAnimFSM();
}

//*****************************************************************************
VuDriverEntity::~VuDriverEntity()
{
	clear();

	delete mpModelInstance;
	delete mpLod1ModelInstance;
	delete mpLod2ModelInstance;

	delete mpRagdoll;
}

//*****************************************************************************
void VuDriverEntity::dataModified(const std::string &driverName)
{
	clear();

	loadData(VuGameUtil::IF()->driverDB()["Default"]);
	loadData(VuGameUtil::IF()->driverDB()[driverName]);

	mpPortraitImage = VuAssetFactory::IF()->createAsset<VuTextureAsset>(mPortraitImageAssetName, VuAssetFactory::OPTIONAL_ASSET);
	mpAbilityImage = VuAssetFactory::IF()->createAsset<VuTextureAsset>(mAbilityImageAssetName, VuAssetFactory::OPTIONAL_ASSET);

	// create model
	mpModelInstance->setModelAsset(mModelAssetName);
	mpLod1ModelInstance->setModelAsset(mLod1ModelAssetName);
	mpLod2ModelInstance->setModelAsset(mLod2ModelAssetName);

	// create animation
	if ( mpModelInstance->getSkeleton() )
	{
		mpAnimatedSkeleton = new VuAnimatedSkeleton(mpModelInstance->getSkeleton());

		if ( VuAssetFactory::IF()->doesAssetExist<VuAnimationAsset>(mTurnAnim) )
		{
			mpTurnAnimControl = new VuAnimationControl(mTurnAnim);
			if ( mpTurnAnimControl->getAnimation() )
			{
				mpTurnAnimControl->setWeight(0.0f);
				mpTurnAnimControl->setTimeFactor(0.0f);
				mpAnimatedSkeleton->addAnimationControl(mpTurnAnimControl);
			}
			else
			{
				mpTurnAnimControl->removeRef();
				mpTurnAnimControl = VUNULL;
			}
		}

		for ( int iAnim = 0; iAnim < NUM_ANIMS; iAnim++ )
		{
			for ( int index = 0; index < (int)mAnims[iAnim].size(); index++ )
			{
				if ( VuAssetFactory::IF()->doesAssetExist<VuAnimationAsset>(mAnims[iAnim][index]) )
				{
					VuAnimationControl *pAnimControl = new VuAnimationControl(mAnims[iAnim][index]);
					pAnimControl->setLooping(false);
					pAnimControl->setEventIF(this);
					if ( pAnimControl->getAnimation() )
						mAnimControls[iAnim].push_back(pAnimControl);
					else
						pAnimControl->removeRef();
				}
			}
		}
	}

	// configure ragdoll
	VuRagdoll::Params ragdollParams;
	ragdollParams.mWaterSimulation = true;
	mpRagdoll->configure(mpModelInstance->getSkeleton(), VuGameUtil::IF()->ragdollDB()[mRagdollType], this, ragdollParams);
	mRagdollCameraTargetBodyIndex = VuMax(mpRagdoll->getBodyIndex(mRagdollCameraTarget.c_str()), 0);

	// create ragdoll pfx
	if ( VuPfx::IF() )
	{
		if ( (mpRagdollPfx = VuPfx::IF()->createSystemInstance(mRagdollPfx.c_str())) )
		{
			// set custom pfx data
			VuPfxSystemInstance *pSystemInst = static_cast<VuPfxSystemInstance *>(mpRagdollPfx);
			for ( VuPfxPatternInstance *pPatternInst = pSystemInst->mPatterns.front(); pPatternInst; pPatternInst = pPatternInst->next() )
				for ( VuPfxProcessInstance *pProcessInst = pPatternInst->mProcesses.front(); pProcessInst; pProcessInst = pProcessInst->next() )
					if ( pProcessInst->mpParams->isDerivedFrom(VuPfxEmitRagdollSplashQuadFountain::msRTTI) )
						static_cast<VuPfxEmitRagdollSplashQuadFountainInstance *>(pProcessInst)->mpRagdoll = mpRagdoll;
		}
	}

	const VuSpreadsheetAsset *pSA = VuGameUtil::IF()->driverSpreadsheet();
	int rowIndex = VuSpreadsheetQuery::findFirstRow(pSA, VuSpreadsheetQuery::VuStringEqual("Driver", driverName.c_str()));

	VuFastDataUtil::getValue(pSA->getField(rowIndex, "Charge Time"), mAbilityChargeTime);
}

//*****************************************************************************
void VuDriverEntity::onGameInitialize()
{
	mp3dDrawComponent->show();
	mAnimFSM.begin();
}

//*****************************************************************************
void VuDriverEntity::onGameRelease()
{
	mp3dDrawComponent->hide();
	mAnimFSM.end();

	for ( EffectAnims::iterator iter = mEffectAnims.begin(); iter != mEffectAnims.end(); iter++ )
	{
		mpAnimatedSkeleton->removeAnimationControl(iter->mpAnimControl);
		iter->mpAnimControl->removeRef();
	}
	mEffectAnims.clear();
}

//*****************************************************************************
void VuDriverEntity::onTickDecision(float fdt)
{
	// timers
	mHappyTimer -= fdt;
	mAngryTimer -= fdt;

	// FSM conditions
	mAnimFSM.setCondition("IntroDone", !VuCarManager::IF()->isCameraOverrideEnabled());
	mAnimFSM.setCondition("HappyTimerExp", mHappyTimer <= 0.0f);
	mAnimFSM.setCondition("AngryTimerExp", mAngryTimer <= 0.0f);

	mAnimFSM.evaluate();
	mAnimFSM.tick(fdt);

	// update effect anims
	float totalEffectWeight = 0.0f;
	for ( EffectAnims::iterator iter = mEffectAnims.begin(); iter != mEffectAnims.end(); )
	{
		// effect anim blending
		float weight = iter->mpAnimControl->getWeight();
		if ( iter->mActive )
		{
			weight += fdt/BLEND_TIME;
			weight = VuMin(weight, 1.0f);
		}
		else
		{
			weight -= fdt/BLEND_TIME;
			weight = VuMax(weight, 0.0f);
		}
		iter->mpAnimControl->setWeight(weight);

		totalEffectWeight += weight;

		// remove effect anims that have finished blending out
		if ( !iter->mActive && weight < FLT_EPSILON )
		{
			mpAnimatedSkeleton->removeAnimationControl(iter->mpAnimControl);
			iter->mpAnimControl->removeRef();
			iter = mEffectAnims.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	// calculate anim weight
	float turnWeight = 1.0f - totalEffectWeight;

	if ( mpCurAnimControl )
	{
		mAnimWeight = VuMin(mAnimWeight + fdt*mBlendRate, 1.0f);
		mpCurAnimControl->setWeight(turnWeight*mAnimWeight);
		turnWeight = turnWeight*(1.0f - mAnimWeight);
	}

	// update current turn position
	mCurTurnPos = VuMathUtil::smoothCD(mCurTurnPos, getAnimationYawControl(), mCurTurnRate, TURN_SMOOTH_TIME, fdt);
	float turnAnimFactor = 0.5f + 0.5f*mCurTurnPos;

	// update turn animations
	if ( mpTurnAnimControl )
	{
		mpTurnAnimControl->setWeight(turnWeight);
		mpTurnAnimControl->setLocalTime(turnAnimFactor*mpTurnAnimControl->getAnimation()->getEndTime());
	}

	VuCarStats &stats = mpCar->getStats();
	if ( mpCar->getPauseControlCount() == 0 && !stats.mCharacterAbilityUsed )
	{
		stats.mCharacterAbilityCharge += fdt/mAbilityChargeTime;
		stats.mCharacterAbilityCharge = VuMin(stats.mCharacterAbilityCharge, 1.0f);
	}

	// check for large impact
	if ( mpCar->getCollidedThisFrame() )
		onImpact(mpCar->getLinearVelocity() - mPrevCarVelocity);
	mPrevCarVelocity = mpCar->getLinearVelocity();

	// did I pass somebody?
	int curPlace = mpCar->getStats().mPlace;
	if ( curPlace == mPrevPlace - 1 )
	{
		// find car I just passed
		for ( int i = 0; i < VuCarManager::IF()->getCarCount(); i++ )
		{
			VuCarEntity *pOtherCar = VuCarManager::IF()->getCar(i);
			if ( pOtherCar->getStats().mPlace == mPrevPlace )
			{
				if ( VuDist(pOtherCar->getModelPosition(), mpCar->getModelPosition()) < PASSING_DISTANCE )
				{
					onPassedCar(pOtherCar);
					break;
				}
			}
		}
	}
	mPrevPlace = curPlace;

	if (mAutoDrive)
	{
		if (mAutoDriveDuration > FLT_EPSILON)
		{
			// Do some steering along AI waypoints
			//
			VuMatrix modelMatrix = mpCar->getModelMatrix();
			modelMatrix.translateLocal(mpCar->getCollisionAabb().getCenter());
			float d = VuDot(modelMatrix.getAxisY(), mpCar->getLinearVelocity());
			float sign = d >= 0.0f ? 1.0f : -1.0f;
			float currentSpeed = mpCar->getLinearVelocity().mag() * sign;

			const VuVector3 &currentPosition = modelMatrix.getTrans();
			const VuVector3 &currentDirection = modelMatrix.getAxisY();

			VuVector3 newGoalPosition;
			VuVector3 newGoalDirection;

			VuAiUtils::determineGoalPos(mpCar, currentPosition, newGoalPosition, newGoalDirection);

			float goalSpeed = mpCar->getMaxForwardSpeed();

			float yawControl = 0.0f;
			float throttleControl = 0.0f;

			VuAiUtils::determineControls(mpCar, currentPosition, currentDirection, newGoalPosition, currentSpeed, goalSpeed, fdt, yawControl, throttleControl);

			float yawValue = (mpCar->getRawYawControl() + yawControl) * 0.5f;
			float throttleValue = (mpCar->getRawThrottleControl() + throttleControl) * 0.5f;

			// Set the actual controls on the car
			mpCar->setYawControl(yawValue);
			mpCar->setThrottleControl(throttleValue);

			mAutoDriveDuration -= fdt;
		}
		else
		{
			mpCar->setThrottleControl(0.0f);
		}
	}
}

//*****************************************************************************
void VuDriverEntity::onTickBuild(float fdt)
{
	VuMatrix transform;
	mpCar->getDriverTransform(transform);
	mpTransformComponent->setWorldTransform(transform);

	if ( mpRagdoll->isActive() )
	{
		mpModelInstance->setPose(transform, mpRagdoll);
	}
	else
	{
		// update animation
		if ( mpAnimatedSkeleton )
		{
			mpAnimatedSkeleton->advance(fdt);
			mpAnimatedSkeleton->build();
		}

		mpModelInstance->setPose(mpAnimatedSkeleton);
	}

	if ( mpLod1ModelInstance->getGfxAnimatedScene() )
		mpLod1ModelInstance->copyPose(mpModelInstance);

	mpModelInstance->finalizePose();
	mpLod1ModelInstance->finalizePose();

	VuAabb aabb(mpModelInstance->getLocalAabb(), transform);

	if ( mpRagdoll->isActive() && mpRagdollPfx )
	{
		mpRagdollPfx->tick(fdt, false);
		aabb.addAabb(mpRagdollPfx->getAabb());
	}

	// update visibility
	mp3dDrawComponent->updateVisibility(aabb);
}

//*****************************************************************************
void VuDriverEntity::onRecover()
{
	mAnimFSM.pulseCondition("Recovered");
}

//*****************************************************************************
float VuDriverEntity::getAudioThrottleControl() const
{
	return mpCar->getThrottleControl();
}

//*****************************************************************************
float VuDriverEntity::getAnimationThrottleControl() const
{
	return mpCar->getThrottleControl();
}

//*****************************************************************************
float VuDriverEntity::getAnimationYawControl() const
{
	return mpCar->getRawYawControl();
}

//*****************************************************************************
VuSkeleton *VuDriverEntity::getSkeleton() const
{
	return mpModelInstance->getSkeleton();
}

//*****************************************************************************
void VuDriverEntity::setFinished()
{
	const VuCarStats &stats = mpCar->getStats();
	if ( stats.mDNF || stats.mPlace > 3 )
		mAnimFSM.pulseCondition("DriverLost");
	else if ( stats.mPlace == 1 )
		mAnimFSM.pulseCondition("DriverWon");
	else
		mAnimFSM.pulseCondition("DriverFinished");

	// At end of race, enable auto driving
	enableAutoDrive(5.0f);
}

//*****************************************************************************
void VuDriverEntity::onImpact(const VuVector3 &deltaV)
{
	if ( deltaV.mag() > VuMphToMetersPerSecond(IMPACT_DELTA_V) )
	{
		float dot = VuDot(mpCar->getModelMatrix().getAxisY(), deltaV.normal());
		if ( dot < 0.7071f )
		{
			mAnimFSM.pulseCondition("CollisionFront");
		}
	}
}

//*****************************************************************************
void VuDriverEntity::onPassedCar(VuCarEntity *pOtherCar)
{
	mAnimFSM.pulseCondition("PassedCar");
}

//*****************************************************************************
void VuDriverEntity::onCollidedWithCar(VuCarEntity *pOtherCar)
{
	mAnimFSM.pulseCondition("CollidedWithCar");
}

//*****************************************************************************
void VuDriverEntity::startRagdoll()
{
	mAnimFSM.pulseCondition("StartRagdoll");
}

//*****************************************************************************
bool VuDriverEntity::isRagdollActive()
{
	return mpRagdoll->isActive();
}

//*****************************************************************************
void VuDriverEntity::hide()
{
	mpRagdoll->stopSimulation();
	if ( mpRagdollPfx )
		mpRagdollPfx->stop();

	mp3dDrawComponent->hide();
}

//*****************************************************************************
VUUINT32 VuDriverEntity::createEffectAnimation(const std::string &assetName)
{
	for ( EffectAnims::iterator iter = mEffectAnims.begin(); iter != mEffectAnims.end(); iter++ )
		iter->mActive = false;

	VuEffectAnim effectAnim;
	effectAnim.mID = mNextEffectAnimId++;
	effectAnim.mActive = true;
	effectAnim.mpAnimControl = new VuAnimationControl(assetName);
	effectAnim.mpAnimControl->setWeight(0.0f);

	mEffectAnims.push_back(effectAnim);

	mpAnimatedSkeleton->addAnimationControl(effectAnim.mpAnimControl);

	return effectAnim.mID;
}

//*****************************************************************************
void VuDriverEntity::releaseEffectAnimation(VUUINT32 id)
{
	for ( EffectAnims::iterator iter = mEffectAnims.begin(); iter != mEffectAnims.end(); iter++ )
		if ( iter->mID == id )
			iter->mActive = false;
}

//*****************************************************************************
void VuDriverEntity::getRagdollCameraTarget(VuVector3 &target) const
{
	target = mpRagdoll->getBody(mRagdollCameraTargetBodyIndex).mWorldTransform.getTrans();
}

//*****************************************************************************
void VuDriverEntity::getRagdollCameraVelocity(VuVector3 &vel) const
{
	vel = mpRagdoll->getBody(mRagdollCameraTargetBodyIndex).mpRigidBody->getVuLinearVelocity();
}

//*****************************************************************************
void VuDriverEntity::loadData(const VuJsonContainer &data)
{
	VuDataUtil::getValue(data["PortraitImage"], mPortraitImageAssetName);
	VuDataUtil::getValue(data["AbilityImage"], mAbilityImageAssetName);
	VuDataUtil::getValue(data["Model Asset"], mModelAssetName);
	VuDataUtil::getValue(data["LOD 1 Model Asset"], mLod1ModelAssetName);
	VuDataUtil::getValue(data["LOD 2 Model Asset"], mLod2ModelAssetName);
	VuDataUtil::getValue(data["LOD 1 Distance"], mLod1Dist);
	VuDataUtil::getValue(data["LOD 2 Distance"], mLod2Dist);
	VuDataUtil::getValue(data["Draw Distance"], mDrawDist);

	// animations
	const VuJsonContainer &animations = data["Animations"];
	VuDataUtil::getValue(animations["Turn"], mTurnAnim);
	for ( int i = 0; i < animations["Start"].size(); i++ )
		mAnims[ANIM_START].push_back(animations["Start"][i].asString());
	for ( int i = 0; i < animations["Win"].size(); i++ )
		mAnims[ANIM_WIN].push_back(animations["Win"][i].asString());
	for ( int i = 0; i < animations["Lose"].size(); i++ )
		mAnims[ANIM_LOSE].push_back(animations["Lose"][i].asString());
	for ( int i = 0; i < animations["Finish"].size(); i++ )
		mAnims[ANIM_FINISH].push_back(animations["Finish"][i].asString());
	for ( int i = 0; i < animations["ImpactForward"].size(); i++ )
		mAnims[ANIM_IMPACT_FORWARD].push_back(animations["ImpactForward"][i].asString());
	for ( int i = 0; i < animations["Happy"].size(); i++ )
		mAnims[ANIM_HAPPY].push_back(animations["Happy"][i].asString());
	for ( int i = 0; i < animations["Angry"].size(); i++ )
		mAnims[ANIM_ANGRY].push_back(animations["Angry"][i].asString());

	// ragdoll
	const VuJsonContainer &ragdoll = data["Ragdoll"];
	VuDataUtil::getValue(ragdoll["Type"], mRagdollType);
	VuDataUtil::getValue(ragdoll["Camera Target"], mRagdollCameraTarget);
	VuDataUtil::getValue(ragdoll["Splash Pfx"], mRagdollPfx);

	// timers
	const VuJsonContainer &timers = data["Timers"];
	VuDataUtil::getValue(timers["Happy"], mHappyRepeatTime);
	VuDataUtil::getValue(timers["Angry"], mAngryRepeatTime);
}

//*****************************************************************************
void VuDriverEntity::clear()
{
	VuAssetFactory::IF()->releaseAsset(mpPortraitImage);
	VuAssetFactory::IF()->releaseAsset(mpAbilityImage);
	mpPortraitImage = VUNULL;
	mpAbilityImage = VUNULL;

	if ( mpAnimatedSkeleton )
	{
		mpAnimatedSkeleton->removeRef();
		mpAnimatedSkeleton = VUNULL;
	}
	if ( mpTurnAnimControl )
	{
		mpTurnAnimControl->removeRef();
		mpTurnAnimControl = VUNULL;
	}
	for ( int iAnim = 0; iAnim < NUM_ANIMS; iAnim++ )
	{
		for ( int index = 0; index < (int)mAnimControls[iAnim].size(); index++ )
			mAnimControls[iAnim][index]->removeRef();
		mAnimControls[iAnim].clear();
	}
	if ( mpRagdollPfx )
	{
		VuPfx::IF()->releaseSystemInstance(mpRagdollPfx);
		mpRagdollPfx = VUNULL;
	}
}

//*****************************************************************************
void VuDriverEntity::draw(const VuGfxDrawParams &params)
{
	VuMatrix transform;
	mpCar->getDriverTransform(transform);

	if ( sbDebug )
	{
		VuGfxDrawInfoParams infoParams(params.mCamera);

		if ( sbDebugBones )			infoParams.mFlags |= VuGfxDrawInfoParams::BONES;
		if ( sbDebugBoneNames )		infoParams.mFlags |= VuGfxDrawInfoParams::BONE_NAMES;

		infoParams.mBoneSize = DEBUG_BONE_SIZE;

		mpModelInstance->drawInfo(transform, infoParams);

		if ( sbDebugRagdoll )
			mpRagdoll->drawDebugBodies(params.mCamera, VuColor(255,255,255));
	}
	else
	{
		if ( params.mbDrawReflection )
			return;

		VuColor color = mpCar->getDrawColor();
		color.mA = VuRound(255*mpCar->getAlpha());

		if ( mpCar->isInGhostMode() && mpCar->getGhostHide() )
			return;

		VuVector3 pos = transform.transform(mpModelInstance->getLocalAabb().getCenter());
		float dist = (pos - params.mEyePos).mag();

		if ( dist > mDrawDist )
			return;

		if ( dist > mLod2Dist && !mpRagdoll->isActive() )
		{
			mpLod2ModelInstance->setColor(color);
			mpLod2ModelInstance->setDynamicLightGroupMask(mpCar->getDynamicLightGroupMask());
			mpLod2ModelInstance->draw(transform, params);
		}
		else if ( dist > mLod1Dist )
		{
			mpLod1ModelInstance->setColor(color);
			mpLod1ModelInstance->setDynamicLightGroupMask(mpCar->getDynamicLightGroupMask());
			mpLod1ModelInstance->draw(transform, params);
		}
		else
		{
			mpModelInstance->setColor(color);
			mpModelInstance->setDynamicLightGroupMask(mpCar->getDynamicLightGroupMask());
			mpModelInstance->draw(transform, params);
		}

		if ( mpRagdoll->isActive() && mpRagdollPfx )
			mpRagdollPfx->draw(params);
	}
}

//*****************************************************************************
void VuDriverEntity::drawShadow(const VuGfxDrawShadowParams &params)
{
	if ( !sbDebug )
	{
		if ( params.mbDrawReflection )
			return;

		if ( mpCar->isInGhostMode() && mpCar->getGhostHide() )
			return;

		if ( mpCar->getAlpha() + FLT_EPSILON < 0.5f )
			return;

		VuMatrix transform;
		mpCar->getDriverTransform(transform);

		VuVector3 pos = transform.transform(mpModelInstance->getLocalAabb().getCenter());
		float distSquared = (pos - params.mEyePos).magSquared();
		if ( distSquared > mLod2Dist*mLod2Dist && !mpRagdoll->isActive() )
		{
			mpLod2ModelInstance->drawShadow(transform, params);
		}
		else if ( distSquared > mLod1Dist*mLod1Dist )
		{
			mpLod1ModelInstance->drawShadow(transform, params);
		}
		else
		{
			mpModelInstance->drawShadow(transform, params);
		}
	}
}

//*****************************************************************************
void VuDriverEntity::drawPrefetch()
{
	mpLod2ModelInstance->drawPrefetch();
	mpLod1ModelInstance->drawPrefetch();
	mpModelInstance->drawPrefetch();
}

//*****************************************************************************
void VuDriverEntity::onAnimationEvent(const std::string &type, const VuJsonContainer &params)
{
	if ( type == "AnimDone" )
		mAnimFSM.pulseCondition("AnimDone");
}

//*****************************************************************************
void VuDriverEntity::createAnimFSM()
{
	VuFSM::VuState *pState;

	pState = mAnimFSM.addState("Intro");

	pState = mAnimFSM.addState("Start");
		pState->setEnterMethod(this, &VuDriverEntity::animStartEnter);

	pState = mAnimFSM.addState("Drive");
		pState->setEnterMethod(this, &VuDriverEntity::animDriveEnter);

	pState = mAnimFSM.addState("ImpactForward");
		pState->setEnterMethod(this, &VuDriverEntity::animImpactForwardEnter);

	pState = mAnimFSM.addState("Happy");
		pState->setEnterMethod(this, &VuDriverEntity::animHappyEnter);

	pState = mAnimFSM.addState("Angry");
		pState->setEnterMethod(this, &VuDriverEntity::animAngryEnter);

	pState = mAnimFSM.addState("Win");
		pState->setEnterMethod(this, &VuDriverEntity::animWinEnter);

	pState = mAnimFSM.addState("Lose");
		pState->setEnterMethod(this, &VuDriverEntity::animLoseEnter);

	pState = mAnimFSM.addState("Finish");
		pState->setEnterMethod(this, &VuDriverEntity::animFinishEnter);

	pState = mAnimFSM.addState("Ragdoll");
		pState->setEnterMethod(this, &VuDriverEntity::animRagdollEnter);
		pState->setExitMethod(this, &VuDriverEntity::animRagdollExit);

	// add transitions
	mAnimFSM.addTransition("", "Ragdoll", "StartRagdoll");
	mAnimFSM.addTransition("Intro", "Start", "IntroDone");
	mAnimFSM.addTransition("Start", "Drive", "AnimDone");
	mAnimFSM.addTransition("Drive", "Win", "DriverWon");
	mAnimFSM.addTransition("Drive", "Lose", "DriverLost");
	mAnimFSM.addTransition("Drive", "Finish", "DriverFinished");
	mAnimFSM.addTransition("Drive", "ImpactForward", "CollisionFront");
	mAnimFSM.addTransition("Drive", "Happy", "PassedCar & HappyTimerExp");
	mAnimFSM.addTransition("Drive", "Angry", "CollidedWithCar & AngryTimerExp");
	mAnimFSM.addTransition("ImpactForward", "Drive", "AnimDone");
	mAnimFSM.addTransition("Happy", "Drive", "AnimDone");
	mAnimFSM.addTransition("Angry", "Drive", "AnimDone");
	mAnimFSM.addTransition("Win", "Drive", "AnimDone");
	mAnimFSM.addTransition("Lose", "Drive", "AnimDone");
	mAnimFSM.addTransition("Finish", "Drive", "AnimDone");
	mAnimFSM.addTransition("Ragdoll", "Drive", "Recovered");
}

//*****************************************************************************
void VuDriverEntity::animDriveEnter()
{
	stopAnimation();

	mCurTurnPos = 0.0f;
	mCurTurnRate = 0.0f;
}

//*****************************************************************************
void VuDriverEntity::animRagdollEnter()
{
	VuMatrix transform;
	mpCar->getDriverTransform(transform);

	VuVector3 linVel = mpCar->getLinearVelocity();
	linVel.mZ += VuMphToMetersPerSecond(10.0f);

	VuVector3 angVel;
	angVel.mX = VuRand::global().range(-VU_2PI, VU_2PI);
	angVel.mY = VuRand::global().range(-VU_2PI, VU_2PI);
	angVel.mZ = VuRand::global().range(-VU_2PI, VU_2PI);
	angVel = transform.transformNormal(angVel);

	mpRagdoll->startSimulation(transform, mpAnimatedSkeleton->getLocalPose(), linVel, angVel);
	if ( mpRagdollPfx )
		mpRagdollPfx->start();
}

//*****************************************************************************
void VuDriverEntity::animRagdollExit()
{
	mpRagdoll->stopSimulation();
	if ( mpRagdollPfx )
		mpRagdollPfx->stop();
}

//*****************************************************************************
void VuDriverEntity::startAnimation(eAnim anim, float blendTime)
{
	AnimControls &animControls = mAnimControls[anim];

	if ( animControls.size() )
	{
		mpCurAnimControl = animControls[VuRand::global().range(0, (int)animControls.size())];
		mpCurAnimControl->setLocalTime(0.0f);
		mpAnimatedSkeleton->addAnimationControl(mpCurAnimControl);

		if ( blendTime > FLT_EPSILON )
		{
			mAnimWeight = 0.0f;
			mBlendRate = 1.0f/blendTime;
		}
		else
		{
			mAnimWeight = 1.0f;
			mBlendRate = 0.0f;
		}
	}
	else
	{
		mAnimFSM.pulseCondition("AnimDone");
	}
}

//*****************************************************************************
void VuDriverEntity::stopAnimation()
{
	if ( mpCurAnimControl )
	{
		mpAnimatedSkeleton->removeAnimationControl(mpCurAnimControl);
		mpCurAnimControl = VUNULL;
	}
}