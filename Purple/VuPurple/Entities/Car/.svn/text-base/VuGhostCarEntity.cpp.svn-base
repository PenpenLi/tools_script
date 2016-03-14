//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Ghost Car entity
// 
//*****************************************************************************

#include "VuGhostCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarWheel.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Managers/VuCollisionManager.h"
#include "VuPurple/Track/VuTrackManager.h"
#include "VuPurple/Track/VuTrackSector.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuPurple/PowerUps/VuPowerUpUtil.h"
#include "VuEngine/Components/Transform/VuTransformComponent.h"
#include "VuEngine/Components/3dDraw/Vu3dDrawComponent.h"
#include "VuEngine/Assets/VuAssetFactory.h"
#include "VuEngine/Assets/VuAnimationAsset.h"
#include "VuEngine/Assets/VuCollisionMeshAsset.h"
#include "VuEngine/Assets/VuTextureAsset.h"
#include "VuEngine/Animation/VuSkeleton.h"
#include "VuEngine/Animation/VuAnimatedSkeleton.h"
#include "VuEngine/Animation/VuAnimationControl.h"
#include "VuEngine/Animation/VuAnimationTransform.h"
#include "VuEngine/Animation/VuAnimation.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/GfxScene/VuGfxStaticScene.h"
#include "VuEngine/Gfx/Shadow/VuBlobShadow.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Managers/VuTireTrackManager.h"
#include "VuEngine/Managers/VuGfxSettingsManager.h"
#include "VuEngine/Managers/VuConfigManager.h"
#include "VuEngine/Dynamics/Util/VuDynamicsRayTest.h"
#include "VuEngine/Water/VuWater.h"
#include "VuEngine/Water/VuFluidsObject.h"
#include "VuEngine/Math/VuCubicCurve.h"
#include "VuEngine/Pfx/VuPfx.h"
#include "VuEngine/Pfx/VuPfxPattern.h"
#include "VuEngine/Math/VuUnitConversion.h"
#include "VuEngine/Util/VuDataUtil.h"


#define SHADOW_TEST_DIST 10.0f // m
#define WHEEL_DRIVE_PFX_MIN_SPEED VuMphToMetersPerSecond(10.0f)


class VuGhostSuspensionRayTestResult : public VuDynamicsRayTest::VuClosestResult
{
public:
	VuGhostSuspensionRayTestResult(VUUINT32 mask) : mMask(mask) {}

	virtual bool needsCollision(VuRigidBody *pRigidBody)
	{
		if ( pRigidBody->getCollisionGroup() & mMask )
			return true;

		return false;
	}
	VUUINT32	mMask;
};


//*****************************************************************************
VuGhostCarEntity::VuGhostCarEntity(const VuArray<VUBYTE> &ghostData):
	mpSuspensionAnimatedSkeleton(VUNULL),
	mpDriverAnimatedSkeleton(VUNULL),
	mpTurnAnimControl(VUNULL),
	mpBlobShadow(VUNULL),
	mpPortraitImage(VUNULL),
	mChassisLod1Dist(FLT_MAX),
	mChassisLod2Dist(FLT_MAX),
	mDriverLod1Dist(FLT_MAX),
	mDriverLod2Dist(FLT_MAX),
	mCollisionAabb(VuAabb::one()),
	mLinearVelocity(0,0,0),
	mDriverTransform(VuMatrix::identity()),
	mNearFadeDist(0.0f),
	mFarFadeDist(0.0f),
	mEndFadeOutTime(0.0f),
	mPlaybackActive(false),
	mPlaybackTime(0.0f),
	mPlaybackFrame(0),
	mPlaybackEndTime(0.0f),
	mPlaybackTimeFactor(1.0f),
	mGhostMode(false),
	mGhostModeTimer(0.0),
	mSteering(0.0f),
	mAlpha(1.0f),
	mShadowValue(1.0f),
	mDrawShadowValue(1.0f),
	mDrawColor(255,255,255),
	mPfxColor(1,1,1,1),
	mWaterSurfaceFlags(0),
	mMinRPM(0.0f),
	mMaxRPM(0.0f),
	mpFluidsObject(VUNULL),
	mpChassisEffect(VUNULL),
	mpBackFirePfxSystem(VUNULL),
	mpCarTrailEffect(VUNULL),
	mBackFirePfxFlag(false),
	mBackFirePfxOn(false),
	mCurrentSection(0),
	mpCurrentSector(VUNULL),
	mDistFromStartOfLap(0.0f)
{
	addComponent(mp3dDrawComponent = new Vu3dDrawComponent(this));

	mp3dDrawComponent->setDrawMethod(this, &VuGhostCarEntity::draw);
	mp3dDrawComponent->setDrawShadowMethod(this, &VuGhostCarEntity::drawShadow);

	VuBinaryDataReader ghostDataReader(ghostData);
	mInitialState.read(ghostDataReader);

	const VuJsonContainer &defaultCarData = VuGameUtil::IF()->carDB()["Default"];
	const VuJsonContainer &carData = VuGameUtil::IF()->carDB()[mInitialState.mCarName];
	const VuJsonContainer &stageData = carData["Stages"][mInitialState.mStage];
	
	VuDataUtil::getValue(defaultCarData["LOD 1 Distance"], mChassisLod1Dist);
	VuDataUtil::getValue(defaultCarData["LOD 2 Distance"], mChassisLod2Dist);
	VuDataUtil::getValue(defaultCarData["Max Steering Angle"], mMaxSteeringAngle);
	mMaxSteeringAngle = VuDegreesToRadians(mMaxSteeringAngle);

	// chassis
	{
		mChassis.setModelAsset(stageData["Model Asset"].asString());
		mChassis.enableTranslucentDepth(true);
		mChassis.setMaterialSubstIF(&mSkin);
		mChassis.setRejectionScaleModifier(0.0f);

		mChassisLod1.setModelAsset(stageData["LOD 1 Model Asset"].asString());
		mChassisLod1.enableTranslucentDepth(true);
		mChassisLod1.setMaterialSubstIF(&mSkinLod1);
		mChassisLod1.setRejectionScaleModifier(0.0f);

		mChassisLod2.setModelAsset(stageData["LOD 2 Model Asset"].asString());
		mChassisLod2.enableTranslucentDepth(true);
		mChassisLod2.setMaterialSubstIF(&mSkinLod2);
		mChassisLod2.setRejectionScaleModifier(0.0f);
	}

	// suspension
	{
		const VuJsonContainer &frontWheelData = VuGameUtil::IF()->wheelDB()[carData["FrontWheels"].asString()];
		const VuJsonContainer &rearWheelData = VuGameUtil::IF()->wheelDB()[carData["RearWheels"].asString()];
		const VuJsonContainer &suspensionData = carData["Suspension"];

		mSuspension.setModelAsset(suspensionData["Model Asset"].asString());
		mSuspension.enableTranslucentDepth(true);

		mCorners[VuCarWheel::FRONT_LEFT].mFront = true;
		mCorners[VuCarWheel::FRONT_RIGHT].mFront = true;
		mCorners[VuCarWheel::FRONT_LEFT].mLeft = true;
		mCorners[VuCarWheel::REAR_LEFT].mLeft = true;

		if ( mSuspension.getSkeleton() )
		{
			mpSuspensionAnimatedSkeleton = new VuAnimatedSkeleton(mSuspension.getSkeleton());

			for ( int iCorner = 0; iCorner < 4; iCorner++ )
			{
				Corner &corner = mCorners[iCorner];
				const VuJsonContainer &cornerData = suspensionData["Corners"][iCorner];
				const VuJsonContainer &wheelData = corner.mFront ? frontWheelData : rearWheelData;

				int boneIndex = mSuspension.getSkeleton()->getBoneIndex(cornerData["WheelBone"].asCString());
				if ( boneIndex >= 0 )
				{
					corner.mAttachmentPoint = mSuspension.getModelMatrices()[boneIndex].getTrans();

					const std::string &animationName = cornerData["Animation"].asString();

					if ( VuAssetFactory::IF()->doesAssetExist<VuAnimationAsset>(animationName) )
					{
						corner.mpAnimationControl = new VuAnimationControl(animationName);

						corner.mpAnimationControl->setLooping(false);
						corner.mpAnimationControl->setTimeFactor(0.0f);
						mpSuspensionAnimatedSkeleton->addAnimationControl(corner.mpAnimationControl);

						// determine limits
						corner.mpAnimationControl->setLocalTime(1.0f);
						mpSuspensionAnimatedSkeleton->build();
						corner.mUpperLimit = mpSuspensionAnimatedSkeleton->getLocalPose()[boneIndex].mTranslation.mZ - corner.mAttachmentPoint.mZ;

						corner.mpAnimationControl->setLocalTime(0.0f);
						mpSuspensionAnimatedSkeleton->build();
						corner.mLowerLimit = mpSuspensionAnimatedSkeleton->getLocalPose()[boneIndex].mTranslation.mZ - corner.mAttachmentPoint.mZ;

						corner.mWheelModel.setModelAsset(wheelData["Model Asset"].asString());
						corner.mWheelModel.enableTranslucentDepth(true);
						corner.mWheelModel.setRejectionScaleModifier(0.0f);

						VuDataUtil::getValue(wheelData["LOD 1 Distance"], corner.mLod1Dist);
						corner.mWheelModelLod1.setModelAsset(wheelData["LOD 1 Model Asset"].asString());
						corner.mWheelModelLod1.enableTranslucentDepth(true);
						corner.mWheelModelLod1.setRejectionScaleModifier(0.0f);

						corner.mWheelRadius = wheelData["Radius"].asFloat();

						corner.mBlurSkinData = wheelData["Blur Skin"];
						corner.mBlurSkin.build(corner.mWheelModel.getGfxStaticScene(), corner.mBlurSkinData);
						corner.mWheelModel.setMaterialSubstIF(&corner.mBlurSkin);

						corner.mTireTrackWidth = wheelData["Tire Track Width"].asFloat();
						corner.mTireTrackMinDist = wheelData["Tire Track Min Dist"].asFloat();
						corner.mTireTrackMinVel = wheelData["Tire Track Min Vel"].asFloat();
					}
				}

				wheelData["Water Pfx"].getValue(corner.mWaterPfxName);
			}
		}
	}

	// driver
	{
		const VuJsonContainer &defaultDriverData = VuGameUtil::IF()->driverDB()["Default"];
		const VuJsonContainer &driverData = VuGameUtil::IF()->driverDB()[mInitialState.mDriverName];

		VuDataUtil::getValue(defaultDriverData["LOD 1 Distance"], mDriverLod1Dist);
		VuDataUtil::getValue(defaultDriverData["LOD 2 Distance"], mDriverLod2Dist);

		mDriver.setModelAsset(driverData["Model Asset"].asString());
		mDriver.enableTranslucentDepth(true);

		mDriverLod1.setModelAsset(driverData["LOD 1 Model Asset"].asString());
		mDriverLod1.enableTranslucentDepth(true);

		mDriverLod2.setModelAsset(driverData["LOD 2 Model Asset"].asString());
		mDriverLod2.enableTranslucentDepth(true);

		VuVector3 driverOffset(0,0,0);
		VuDataUtil::getValue(carData["Driver Offset"], driverOffset);
		mDriverTransform.setTrans(driverOffset);

		if ( mDriver.getSkeleton() )
		{
			const std::string &turnAnim = VuGameUtil::IF()->driverDB()["Default"]["Animations"]["Turn"].asString();

			if ( VuAssetFactory::IF()->doesAssetExist<VuAnimationAsset>(turnAnim) )
			{
				mpDriverAnimatedSkeleton = new VuAnimatedSkeleton(mDriver.getSkeleton());
				mpTurnAnimControl = new VuAnimationControl(turnAnim);
				mpTurnAnimControl->setLooping(false);
				mpTurnAnimControl->setTimeFactor(0.0f);
				mpDriverAnimatedSkeleton->addAnimationControl(mpTurnAnimControl);
			}
		}

		mpPortraitImage = VuAssetFactory::IF()->createAsset<VuTextureAsset>(driverData["PortraitImage"].asString(), VuAssetFactory::OPTIONAL_ASSET);
	}

	// skin
	{
		std::string decalTexture;
		VuColor decalColor, paintColor;
		VuGameUtil::IF()->getDecal(mInitialState.mDecalName, decalTexture);
		VuGameUtil::IF()->getColor(mInitialState.mPaintColorName, paintColor);
		VuGameUtil::IF()->getColor(mInitialState.mDecalColorName, decalColor);

		VuDataUtil::putValue(mSkinData["Textures"]["DecalTexture"], decalTexture);
		VuDataUtil::putValue(mSkinData["Constants"]["PaintColor"], paintColor.toVector3());
		VuDataUtil::putValue(mSkinData["Constants"]["DecalColor"], decalColor.toVector3());

		mSkin.build(mChassis.getGfxStaticScene(), mSkinData);
		mSkinLod1.build(mChassisLod1.getGfxStaticScene(), mSkinData);
		mSkinLod2.build(mChassisLod1.getGfxStaticScene(), mSkinData);
	}

	// get collision info
	VuDataUtil::getValue(carData["Collision Aabb"], mCollisionAabb);

	// audio
	{
		const VuJsonContainer engineData = carData["Engine"];
		const VuJsonContainer engineStageData = stageData["Engine"];

		VuDataUtil::getValue(engineData["Audio"]["Run"], mAudioEventName);
		VuDataUtil::getValue(engineStageData["Min RPM"], mMinRPM);
		VuDataUtil::getValue(engineStageData["Max RPM"], mMaxRPM);
	}

	// pfx
	{
		const VuJsonContainer &defaultPfxData = defaultCarData["Pfx"];
		const VuJsonContainer &stagePfxData = stageData["Pfx"];

		mpFluidsObject = new VuFluidsObject;
		mpFluidsObject->setAsset(defaultPfxData["ChassisEffect"]["Fluids Mesh Asset"].asString());

		VuDataUtil::getValue(defaultPfxData["ChassisEffect"]["Effect"], mChassisEffectName);
		VuDataUtil::getValue(stagePfxData["BackFire"], mBackFirePfxName);
	}

	// blob shadow
	{
		mpBlobShadow = new VuBlobShadow;

		const VuJsonContainer &blobShadowData = VuGameUtil::IF()->constantDB()["Car"]["BlobShadow"];
		mpBlobShadow->setTexture(blobShadowData["Texture"].asString());
		blobShadowData["Alpha"].getValue(mpBlobShadow->params().mAlpha);
	}

	// process ghost data
	processGhostData(ghostDataReader);

	VuConfigManager::IF()->registerIntHandler("Gfx/ShaderLOD", this, &VuGhostCarEntity::configShaderLOD);
}

//*****************************************************************************
VuGhostCarEntity::~VuGhostCarEntity()
{
	for ( Splines::iterator iter = mSplines.begin(); iter != mSplines.end(); iter++ )
		delete *iter;

	VU_SAFE_RELEASE(mpSuspensionAnimatedSkeleton);
	VU_SAFE_RELEASE(mpDriverAnimatedSkeleton);
	VU_SAFE_RELEASE(mpTurnAnimControl);

	for ( int i = 0; i < 4; i++ )
		VU_SAFE_RELEASE(mCorners[i].mpAnimationControl);

	delete mpFluidsObject;

	delete mpBlobShadow;

	VuAssetFactory::IF()->releaseAsset(mpPortraitImage);

	VuConfigManager::IF()->unregisterIntHandler("Gfx/ShaderLOD", this);
}

//*****************************************************************************
void VuGhostCarEntity::onLoad(const VuJsonContainer &data)
{
	data["NearFadeDist"].getValue(mNearFadeDist);
	data["FarFadeDist"].getValue(mFarFadeDist);
	data["EndFadeOutTime"].getValue(mEndFadeOutTime);

	const std::string& gameType = VuGameUtil::IF()->getGameType();

	const VuJsonContainer& ghostCar = VuGameUtil::IF()->constantDB()["Games"][gameType]["GhostCar"];
	if (ghostCar.hasMember("TrailPfx"))
	{
		mCarTrailEffectName = ghostCar["TrailPfx"]["Name"].asString();
		VuPowerUpUtil::readTransform(ghostCar["TrailPfx"], mCarTrailTransform);
	}
	else
	{
		mpCarTrailEffect = VUNULL;
		mCarTrailTransform = VuMatrix::identity();
	}
}

//*****************************************************************************
void VuGhostCarEntity::onGameInitialize()
{
	VuMatrix matModel;
	matModel.setEulerAngles(mInitialState.mRotation);
	matModel.setTrans(mInitialState.mPosition);

	getTransformComponent()->setWorldTransform(mInitialState.mPosition, mInitialState.mRotation);

	mp3dDrawComponent->show();

	VuTickManager::IF()->registerHandler(this, &VuGhostCarEntity::tickDecision, "Decision");
	VuTickManager::IF()->registerHandler(this, &VuGhostCarEntity::tickBuild, "Build");

	for ( int iCorner = 0; iCorner < 4; iCorner++ )
	{
		Corner &corner = mCorners[iCorner];

		VuTireTrackParams tireTrackParams;
		if ( corner.mTireTrackWidth > 0.0f )	tireTrackParams.mWidth = corner.mTireTrackWidth;
		if ( corner.mTireTrackMinDist > 0.0f )	tireTrackParams.mMinDist = corner.mTireTrackMinDist;
		if ( corner.mTireTrackMinVel > 0.0f )	tireTrackParams.mMinVel = corner.mTireTrackMinVel;
		tireTrackParams.mOffsetX = corner.mWheelModel.getAabb().getCenter().mX;
		if ( corner.mLeft )
			tireTrackParams.mOffsetX *= -1.0f;
		corner.mpTireTrack = VuTireTrackManager::IF()->createTireTrack(tireTrackParams);

		corner.mpWaterPfx = VuPfx::IF()->createSystemInstance(corner.mWaterPfxName.c_str());
	}

	if ( mpFluidsObject->getFluidsMeshAsset() )
	{
		mpChassisEffect = VuPfx::IF()->createSystemInstance(mChassisEffectName.c_str());
		if ( mpChassisEffect )
		{
			// set custom pfx process data
			for ( VuPfxPatternInstance *pPatternInst = mpChassisEffect->mPatterns.front(); pPatternInst; pPatternInst = pPatternInst->next() )
			{
				for ( VuPfxProcessInstance *pProcessInst = pPatternInst->mProcesses.front(); pProcessInst; pProcessInst = pProcessInst->next() )
				{
					if ( pProcessInst->mpParams->isDerivedFrom(VuPfxEmitCarChassis::msRTTI) )
					{
						static_cast<VuPfxEmitCarChassisInstance *>(pProcessInst)->mpFluidsObject = mpFluidsObject;
						static_cast<VuPfxEmitCarChassisInstance *>(pProcessInst)->mpStateIF = this;
					}
				}
			}

			mpChassisEffect->start();
		}
	}

	mpCarTrailEffect = VuPfx::IF()->createSystemInstance(mCarTrailEffectName.c_str());
	if (mpCarTrailEffect)
	{
		mpCarTrailEffect->start();
	}

	mpBackFirePfxSystem = VuPfx::IF()->createSystemInstance(mBackFirePfxName.c_str());

	VuCarManager::IF()->addGhostCar(this);
}

//*****************************************************************************
void VuGhostCarEntity::onGameRelease()
{
	stopPlayback();

	VuCarManager::IF()->removeGhostCar(this);

	mp3dDrawComponent->hide();

	VuTickManager::IF()->unregisterHandlers(this);

	for ( int iCorner = 0; iCorner < 4; iCorner++ )
	{
		Corner &corner = mCorners[iCorner];
		if ( corner.mpTireTrack )
		{
			VuTireTrackManager::IF()->releaseTireTrack(corner.mpTireTrack);
			corner.mpTireTrack = VUNULL;
		}

		if ( corner.mpDrivePfx ) { VuPfx::IF()->releaseSystemInstance(corner.mpDrivePfx); corner.mpDrivePfx = VUNULL; }
		if ( corner.mpSlidePfx ) { VuPfx::IF()->releaseSystemInstance(corner.mpSlidePfx); corner.mpSlidePfx = VUNULL; }
		if ( corner.mpWaterPfx ) { VuPfx::IF()->releaseSystemInstance(corner.mpWaterPfx); corner.mpWaterPfx = VUNULL; }
	}

	if ( mpChassisEffect )
	{
		VuPfx::IF()->releaseSystemInstance(mpChassisEffect);
		mpChassisEffect = VUNULL;
	}

	if ( mpBackFirePfxSystem )
	{
		VuPfx::IF()->releaseSystemInstance(mpBackFirePfxSystem);
		mpBackFirePfxSystem = VUNULL;
	}

	if (mpCarTrailEffect)
	{
		VuPfx::IF()->releaseSystemInstance(mpCarTrailEffect);
		mpCarTrailEffect = VUNULL;
	}
}

//*****************************************************************************
void VuGhostCarEntity::startPlayback()
{
	if ( !mPlaybackActive )
	{
		mPlaybackActive = true;
		mPlaybackTime = 0.0f;
		mPlaybackFrame = 0;

		if ( mEngineAudioEvent.create(mAudioEventName.c_str()) )
		{
			const VuVector3 &pos = mpTransformComponent->getWorldPosition();
			mEngineAudioEvent.set3DAttributes(&pos, VUNULL, VUNULL);
			mEngineAudioEvent.start();
		}
	}
}

//*****************************************************************************
void VuGhostCarEntity::stopPlayback()
{
	if ( mPlaybackActive )
	{
		mPlaybackActive = false;

		mEngineAudioEvent.release();
	}
}

//*****************************************************************************
void VuGhostCarEntity::getPfxEmitCarChassisState(VuMatrix &transform, VuVector3 &linVel, VUUINT32 &waterSurfaceFlags)
{
	transform = mpTransformComponent->getWorldTransform();
	linVel = mLinearVelocity;
	waterSurfaceFlags = mWaterSurfaceFlags;
}

//*****************************************************************************
void VuGhostCarEntity::processGhostData(VuBinaryDataReader &ghostDataReader)
{
	while ( ghostDataReader.remaining() )
	{
		VUUINT8 commandType = *static_cast<const VUUINT8 *>(ghostDataReader.cur());

		if ( commandType == VuGhostUtil::FRAME_V3 )
		{
			mFrames.resize(mFrames.size() + 1);
			VuPlaybackFrame &frame = mFrames.back();
			frame.readV3(ghostDataReader);

			frame.mSplineIndex = 0;
			frame.mSplineOffset = 0;
			frame.mLinearVelocity.set(0.0f, 0.0f, 0.0f);
		}
		else if ( commandType == VuGhostUtil::FRAME_V2 )
		{
			mFrames.resize(mFrames.size() + 1);
			VuPlaybackFrame &frame = mFrames.back();
			frame.readV2(ghostDataReader);

			frame.mSplineIndex = 0;
			frame.mSplineOffset = 0;
			frame.mLinearVelocity.set(0.0f, 0.0f, 0.0f);
		}
	}

	// calculate velocities
	for ( int i = 1; i < (int)mFrames.size(); i++ )
	{
		VuPlaybackFrame &prevFrame = mFrames[i-1];
		VuPlaybackFrame &frame = mFrames[i];
		frame.mLinearVelocity = (frame.mPosition - prevFrame.mPosition)/(frame.mTime - prevFrame.mTime);
	}

	// build splines
	buildSplines();

	// determine end time
	mPlaybackEndTime = mFrames.size() ? mFrames.back().mTime : 0.0f;
}

//*****************************************************************************
void VuGhostCarEntity::buildSplines()
{
	int frameCount = (int)mFrames.size();
	int splineIndex = 0;
	int splineOffset = 0;

	std::deque<int> splineSizes;
	splineSizes.push_back(0);

	// assign spline and spline index
	for ( int iFrame = 0; iFrame < frameCount; iFrame++ )
	{
		VuPlaybackFrame &frame = mFrames[iFrame];
		frame.mSplineIndex = splineIndex;
		frame.mSplineOffset = splineOffset;

		splineOffset++;
		splineSizes.back()++;

		if ( iFrame + 1 < frameCount )
		{
			VuPlaybackFrame &nextFrame = mFrames[iFrame+1];
			if ( !(frame.mFlags & VuGhostUtil::FLAG_GHOST) && (nextFrame.mFlags & VuGhostUtil::FLAG_GHOST) )
			{
				splineIndex++;
				splineOffset = 0;
				splineSizes.push_back(0);
			}
		}
	}

	// create splines
	for ( int i = 0; i < (int)splineSizes.size(); i++ )
	{
		VuCubicPosCurve *pSpline = new VuCubicPosCurve;
		pSpline->reserve(splineSizes[i]);
		mSplines.push_back(pSpline);
	}

	// build splines
	for ( int iFrame = 0; iFrame < frameCount; iFrame++ )
	{
		VuPlaybackFrame &frame = mFrames[iFrame];

		VuCubicPosCurve *pSpline = mSplines[frame.mSplineIndex];

		pSpline->addControlPoint(frame.mPosition, frame.mTime);
		if ( pSpline->getControlPointCount() == splineSizes[frame.mSplineIndex] )
			pSpline->build(VuVector3(0,0,0), frame.mLinearVelocity);
	}
}

//*****************************************************************************
void VuGhostCarEntity::tickDecision(float fdt)
{
	if ( mPlaybackActive )
	{
		mPlaybackTime = mPlaybackTime + fdt*mPlaybackTimeFactor;
		if ( mPlaybackTime >= mPlaybackEndTime )
		{
			mPlaybackTime = mPlaybackEndTime;
			stopPlayback();
		}
	}

	// advance frame
	while ( (mPlaybackFrame + 1 < (int)mFrames.size()) && (mPlaybackTime > mFrames[mPlaybackFrame + 1].mTime) )
	{
		mPlaybackFrame++;

		const VuPlaybackFrame &curFrame = mFrames[mPlaybackFrame];

		// handle flags
		VUUINT16 flags = curFrame.mFlags;

		if ( flags & VuGhostUtil::FLAG_GHOST )
		{
			mGhostMode = true;
		}
		else
		{
			mGhostMode = false;
			mGhostModeTimer = 0.0f;
		}

		mCorners[0].mSliding = (flags & VuGhostUtil::FLAG_WHEEL0_SLIDING) == VuGhostUtil::FLAG_WHEEL0_SLIDING;
		mCorners[1].mSliding = (flags & VuGhostUtil::FLAG_WHEEL1_SLIDING) == VuGhostUtil::FLAG_WHEEL1_SLIDING;
		mCorners[2].mSliding = (flags & VuGhostUtil::FLAG_WHEEL2_SLIDING) == VuGhostUtil::FLAG_WHEEL2_SLIDING;
		mCorners[3].mSliding = (flags & VuGhostUtil::FLAG_WHEEL3_SLIDING) == VuGhostUtil::FLAG_WHEEL3_SLIDING;

		mBackFirePfxFlag = (flags & VuGhostUtil::FLAG_BACKFIRE_PFX) == VuGhostUtil::FLAG_BACKFIRE_PFX;
	}

	if ( mPlaybackFrame + 1 < (int)mFrames.size() )
	{
		// interpolate
		const VuPlaybackFrame &frame0 = mFrames[mPlaybackFrame];
		const VuPlaybackFrame &frame1 = mFrames[mPlaybackFrame + 1];
		float ratio = (mPlaybackTime - frame0.mTime)/(frame1.mTime - frame0.mTime);
		ratio = VuMax(ratio, 0.0f);

		// transform
		VuVector3 pos, rot;

		if ( frame0.mSplineIndex != frame1.mSplineIndex )
		{
			pos = frame1.mPosition;
			rot = frame1.mRotation;
			mLinearVelocity = VuVector3(0,0,0);
		}
		else
		{
			mSplines[frame0.mSplineIndex]->interpolate(frame0.mSplineOffset, frame1.mSplineOffset, ratio, pos);
			rot = VuSlerp(frame0.mRotation, frame1.mRotation, ratio);
			mLinearVelocity = VuLerp(frame0.mLinearVelocity, frame1.mLinearVelocity, ratio);
		}

		getTransformComponent()->setWorldTransform(pos, rot, false);

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
		mDrawColor = VuLerp(VuGameUtil::IF()->getAmbientColor(), VuGameUtil::IF()->getDiffuseColor(), mDrawShadowValue);
		mPfxColor = VuLerp(mGfxSettings.mPfxAmbientColor.toVector4(), mGfxSettings.mPfxDiffuseColor.toVector4(), mShadowValue);

		// steering
		mSteering = VuLerp(frame0.mSteering, frame1.mSteering, ratio);
		if ( mpTurnAnimControl )
		{
			float turnAnimFactor = 0.5f + 0.5f*mSteering;
			mpTurnAnimControl->setLocalTime(turnAnimFactor*mpTurnAnimControl->getAnimation()->getEndTime());
		}

		// suspension
		for ( int iCorner = 0; iCorner < 4; iCorner++ )
		{
			Corner &corner = mCorners[iCorner];

			// cast ray
			VuVector3 upperTestPos = corner.mAttachmentPoint;
			upperTestPos.mZ += corner.mUpperLimit;
			upperTestPos = mpTransformComponent->getWorldTransform().transform(upperTestPos);

			VuVector3 lowerTestPos = corner.mAttachmentPoint;
			lowerTestPos.mZ += corner.mLowerLimit - corner.mWheelRadius;
			lowerTestPos = mpTransformComponent->getWorldTransform().transform(lowerTestPos);

			VuGhostSuspensionRayTestResult rayTestResult(COL_ENGINE_STATIC_PROP);
			VuDynamicsRayTest::test(upperTestPos, lowerTestPos, rayTestResult);

			corner.mContact = rayTestResult.mbHasHit;

			corner.mSuspensionDist = VuLerp(corner.mUpperLimit, corner.mLowerLimit - corner.mWheelRadius, rayTestResult.mHitFraction) + corner.mWheelRadius;
			corner.mSuspensionDist = VuClamp(corner.mSuspensionDist, corner.mLowerLimit, corner.mUpperLimit);

			// calculate wheel transform
			corner.mWheelTransform = mpTransformComponent->getWorldTransform();
			corner.mWheelTransform.translateLocal(corner.mAttachmentPoint + VuVector3(0.0f, 0.0f, corner.mSuspensionDist));
			if ( corner.mFront )
			{
				corner.mSteeringAngle = -mMaxSteeringAngle*mSteering;
				corner.mWheelTransform.rotateZLocal(corner.mSteeringAngle);
			}
			if ( corner.mLeft )
				corner.mWheelTransform.rotateZLocal(VU_PI);

			// update animation
			if ( corner.mpAnimationControl )
			{
				float travel = (corner.mSuspensionDist - corner.mLowerLimit)/(corner.mUpperLimit - corner.mLowerLimit);
				float animTime = travel*corner.mpAnimationControl->getAnimation()->getEndTime();
				corner.mpAnimationControl->setLocalTime(animTime);
			}

			// update tire tracks
			if ( corner.mpTireTrack )
			{
				VuVector3 contactPointWS(0,0,0);
				VuVector3 contactNormal(0,0,1);

				if ( corner.mContact )
				{
					if ( VuCollisionMeshAsset *pAsset = static_cast<VuCollisionMeshAsset *>(rayTestResult.mpRigidBody->getCollisionShape()->getUserPointer()) )
						corner.mSurfaceType = pAsset->getTriangleMaterial(rayTestResult.mTriangleIndex).mSurfaceTypeID;
					else
						corner.mSurfaceType = rayTestResult.mpRigidBody->getSurfaceType();

					contactPointWS = VuLerp(upperTestPos, lowerTestPos, rayTestResult.mHitFraction);
					contactNormal = rayTestResult.mHitNormal;
				}

				float shadowValue = 1.0f;
				VuTireTrackManager::IF()->updateTireTrack(corner.mpTireTrack, corner.mContact , corner.mSliding, corner.mSurfaceType, contactPointWS, contactNormal, mLinearVelocity, mShadowValue, 1.0f);
			}
		}

		// water heights
		{
			VuWaterPhysicsVertex waterVerts[4];

			VuAabb aabb;
			for ( int i = 0; i < 4; i++ )
			{
				waterVerts[i].mPosition = mCorners[i].mWheelTransform.getTrans();
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
				Corner &corner = mCorners[i];

				corner.mVisualWaterHeight = waterVerts[i].mHeight;
				corner.mSubmerged = (waterVerts[i].mPosition.mZ - corner.mWheelRadius) < waterVerts[i].mHeight;
			}
		}

		// audio
		if ( mEngineAudioEvent.active() )
		{
			float audioRPM = VuLerp(frame0.mAudioRPM, frame1.mAudioRPM, ratio);
			float audioLoad = VuLerp(frame0.mAudioLoad, frame1.mAudioLoad, ratio);

			audioRPM = VuLerp(mMinRPM, mMaxRPM, audioRPM);

			mEngineAudioEvent.setParameterValue("rpm", audioRPM);
			mEngineAudioEvent.setParameterValue("load", audioLoad);

			mEngineAudioEvent.set3DAttributes(&pos, &mLinearVelocity, VUNULL);
		}

		if ( mpBlobShadow )
			mpBlobShadow->calculate(getTransformComponent()->getWorldTransform(), mCollisionAabb);
	}

	// gameplay
	if ( VuTrackManager::IF()->isBuilt() )
	{
		VuVector3 pos = getTransformComponent()->getWorldPosition();
		if ( !mpCurrentSector || !mpCurrentSector->pointInside(pos) )
		{
			int nextSection = (mCurrentSection + 1)%VuTrackManager::IF()->getCheckPointCount();
			mpCurrentSector = VuTrackManager::IF()->findSectorFromPosition(pos, mCurrentSection, nextSection);
			mCurrentSection = mpCurrentSector->mSection;
		}

		if ( mpCurrentSector )
		{
			mDistFromStartOfLap = mpCurrentSector->mDistanceFromStart + (mpCurrentSector->mLength*mpCurrentSector->traversalAmount(pos));
		}
	}
}

//*****************************************************************************
void VuGhostCarEntity::tickBuild(float fdt)
{
	if ( mpSuspensionAnimatedSkeleton )
	{
		mpSuspensionAnimatedSkeleton->advance(fdt);
		mpSuspensionAnimatedSkeleton->build();

		mSuspension.setPose(mpSuspensionAnimatedSkeleton);
		mSuspension.finalizePose();
	}

	if ( mpDriverAnimatedSkeleton )
	{
		mpDriverAnimatedSkeleton->advance(fdt);
		mpDriverAnimatedSkeleton->build();

		mDriver.setPose(mpDriverAnimatedSkeleton);
		mDriver.finalizePose();

		if ( mDriverLod1.getGfxAnimatedScene() )
		{
			mDriverLod1.copyPose(&mDriver);
			mDriverLod1.finalizePose();
		}
	}

	// calculate alpha
	if ( VuCarEntity *pPlayerCar = VuCarManager::IF()->getCameraTargetForViewport(0) )
	{
		float humanDist = VuDist(pPlayerCar->getModelPosition(), mpTransformComponent->getWorldPosition());
		float distAlpha = VuLinStep(mNearFadeDist, mFarFadeDist, humanDist);

		float endAlpha = 1.0f - VuLinStep(mPlaybackEndTime - mEndFadeOutTime, mPlaybackEndTime, mPlaybackTime);

		mAlpha = distAlpha*endAlpha;
	}

	mAabb.reset();

	// update visibility
	{
		const VuMatrix &transform = mpTransformComponent->getWorldTransform();
		mAabb.addAabb(mChassis.getAabb(), transform);
		mAabb.addAabb(mSuspension.getLocalAabb(), transform);
		mAabb.addAabb(mDriver.getLocalAabb(), mDriverTransform*transform);
		for ( int i = 0; i < 4; i++ )
			mAabb.addAabb(mCorners[i].mWheelModel.getAabb(), mCorners[i].mWheelTransform);
	}

	// pfx
	if ( mpChassisEffect )
	{
		mpChassisEffect->setPosition(mpTransformComponent->getWorldPosition());
		mpChassisEffect->setColor(mPfxColor);
		mpChassisEffect->tick(fdt, false);
		if ( mpChassisEffect->getAabb().isValid() )
			mAabb.addAabb(mpChassisEffect->getAabb());
	}

	if ( mpBackFirePfxSystem )
	{
		if ( !mBackFirePfxOn )
		{
			if ( mBackFirePfxFlag )
			{
				mBackFirePfxOn = true;
				mpBackFirePfxSystem->start();
			}
		}
		else
		{
			if ( !mBackFirePfxFlag )
			{
				mBackFirePfxOn = false;
				mpBackFirePfxSystem->stop();
			}
		}

		mpBackFirePfxSystem->setMatrix(mpTransformComponent->getWorldTransform());
		mpBackFirePfxSystem->tick(fdt, false);
		if ( mpBackFirePfxSystem->getAabb().isValid() )
			mAabb.addAabb(mpBackFirePfxSystem->getAabb());
	}

	if (mpCarTrailEffect)
	{
		mpCarTrailEffect->setMatrix(mpTransformComponent->getWorldTransform() * mCarTrailTransform);
		mpCarTrailEffect->tick(fdt, false);
		if (mpCarTrailEffect->getAabb().isValid())
		{
			mAabb.addAabb(mpCarTrailEffect->getAabb());
		}
	}

	bool playDrivePfx = mLinearVelocity.mag() > WHEEL_DRIVE_PFX_MIN_SPEED;
	int overWater = mWaterSurfaceFlags & WATER_SURFACE_WATER;

	for ( int i = 0; i < 4; i++ )
	{
		Corner &corner = mCorners[i];

		if ( corner.mContact )
		{
			if ( corner.mSurfaceType != corner.mPfxSurfaceType )
			{
				const char *wheelDrivePfxName = VuCollisionManager::IF()->getWheelDrivePfxName(corner.mSurfaceType);
				if ( wheelDrivePfxName && (wheelDrivePfxName != corner.mDrivePfxName) )
				{
					if ( corner.mpDrivePfx )
						VuPfx::IF()->releaseSystemInstance(corner.mpDrivePfx);
					corner.mpDrivePfx = VuPfx::IF()->createSystemInstance(wheelDrivePfxName);

					corner.mDrivePfxName = wheelDrivePfxName;
				}

				const char *wheelSlidePfxName = VuCollisionManager::IF()->getWheelSlidePfxName(corner.mSurfaceType);
				if ( wheelSlidePfxName && (wheelSlidePfxName != corner.mSlidePfxName) )
				{
					if ( corner.mpSlidePfx )
						VuPfx::IF()->releaseSystemInstance(corner.mpSlidePfx);
					corner.mpSlidePfx = VuPfx::IF()->createSystemInstance(wheelSlidePfxName);

					corner.mSlidePfxName = wheelSlidePfxName;
				}

				corner.mPfxSurfaceType = corner.mSurfaceType;
			}
		}

		// update
		VuMatrix transform = mpTransformComponent->getWorldTransform();
		transform.translateLocal(corner.mAttachmentPoint + VuVector3(0.0f, 0.0f, corner.mSuspensionDist - corner.mWheelRadius));
		transform.rotateZLocal(corner.mSteeringAngle);

		if ( corner.mpDrivePfx )
		{
			if ( playDrivePfx && corner.mContact && !corner.mSliding )
			{
				if ( corner.mpDrivePfx->getState() != VuPfxSystemInstance::STATE_ALIVE )
					corner.mpDrivePfx->start();
			}
			else
			{
				if ( corner.mpDrivePfx->getState() == VuPfxSystemInstance::STATE_ALIVE )
					corner.mpDrivePfx->stop();
			}

			corner.mpDrivePfx->setMatrix(transform);
			corner.mpDrivePfx->setLinearVelocity(mLinearVelocity);
			corner.mpDrivePfx->setColor(mPfxColor);
			corner.mpDrivePfx->tick(fdt, false);

			if ( corner.mpDrivePfx->getAabb().isValid() )
				mAabb.addAabb(corner.mpDrivePfx->getAabb());
		}

		if ( corner.mpSlidePfx )
		{
			if ( corner.mSliding && !corner.mSubmerged )
			{
				if ( corner.mpSlidePfx->getState() != VuPfxSystemInstance::STATE_ALIVE )
					corner.mpSlidePfx->start();
			}
			else
			{
				if ( corner.mpSlidePfx->getState() == VuPfxSystemInstance::STATE_ALIVE )
					corner.mpSlidePfx->stop();
			}

			corner.mpSlidePfx->setMatrix(transform);
			corner.mpSlidePfx->setLinearVelocity(mLinearVelocity);
			corner.mpSlidePfx->setColor(mPfxColor);
			corner.mpSlidePfx->tick(fdt, false);

			if ( corner.mpSlidePfx->getAabb().isValid() )
				mAabb.addAabb(corner.mpSlidePfx->getAabb());
		}

		if ( corner.mpWaterPfx )
		{
			if ( corner.mSubmerged && overWater && (transform.getTrans().mZ + 2.0f*corner.mWheelRadius) > corner.mVisualWaterHeight )
			{
				if ( corner.mpWaterPfx->getState() != VuPfxSystemInstance::STATE_ALIVE )
					corner.mpWaterPfx->start();
			}
			else
			{
				if ( corner.mpWaterPfx->getState() == VuPfxSystemInstance::STATE_ALIVE )
					corner.mpWaterPfx->stop();
			}

			transform.mT.mZ = corner.mVisualWaterHeight;

			corner.mpWaterPfx->setMatrix(transform);
			corner.mpWaterPfx->setLinearVelocity(mLinearVelocity);
			corner.mpWaterPfx->setColor(mPfxColor);
			corner.mpWaterPfx->tick(fdt, false);

			if ( corner.mpWaterPfx->getAabb().isValid() )
				mAabb.addAabb(corner.mpWaterPfx->getAabb());
		}
	}

	mp3dDrawComponent->updateVisibility(mAabb);
}

//*****************************************************************************
void VuGhostCarEntity::draw(const VuGfxDrawParams &params)
{
	const VuMatrix &transform = mpTransformComponent->getWorldTransform();


	VuColor color(255,255,255);
	color.mA = (VUUINT8)VuRound(color.mA*mAlpha);
	if ( color.mA == 0 )
		return;

	float dist = (transform.getTrans() - params.mEyePos).mag();

	// draw chassis
	if ( dist > mChassisLod2Dist )
	{
		mChassisLod2.setColor(color);
		mChassisLod2.draw(transform, params);
	}
	else if ( dist > mChassisLod1Dist )
	{
		mChassisLod1.setColor(color);
		mChassisLod1.draw(transform, params);
	}
	else
	{
		mChassis.setColor(color);
		mChassis.draw(transform, params);
	}

	// draw suspension
	mSuspension.setColor(color);
	mSuspension.draw(transform, params);

	// draw wheels
	for ( int i = 0; i < 4; i++ )
	{
		Corner &corner = mCorners[i];
		if ( dist > corner.mLod1Dist )
		{
			corner.mWheelModelLod1.setColor(color);
			corner.mWheelModelLod1.draw(corner.mWheelTransform, params);
		}
		else
		{
			corner.mWheelModel.setColor(color);
			corner.mWheelModel.draw(corner.mWheelTransform, params);
		}
	}

	// draw driver
	if ( dist > mDriverLod2Dist )
	{
		mDriverLod2.setColor(color);
		mDriverLod2.draw(mDriverTransform*transform, params);
		mDriverLod2.setRejectionScaleModifier(0.0f);
	}
	else if ( dist > mDriverLod1Dist )
	{
		mDriverLod1.setColor(color);
		mDriverLod1.draw(mDriverTransform*transform, params);
	}
	else
	{
		mDriver.setColor(color);
		mDriver.draw(mDriverTransform*transform, params);
	}

	// pfx
	if ( !params.mbDrawReflection )
	{
		if ( mpChassisEffect )
			mpChassisEffect->draw(params);

		if ( mpBackFirePfxSystem )
			mpBackFirePfxSystem->draw(params);

		if (mpCarTrailEffect)
		{
			mpCarTrailEffect->draw(params);
		}

		for ( int i = 0; i < 4; i++ )
		{
			Corner &corner = mCorners[i];

			if ( corner.mpDrivePfx )
				corner.mpDrivePfx->draw(params);
			if ( corner.mpSlidePfx )
				corner.mpSlidePfx->draw(params);
			if ( corner.mpWaterPfx )
				corner.mpWaterPfx->draw(params);
		}
	}

	// blob shadow
	if ( color.mA == 255 )
	{
		if ( mpBlobShadow && mChassis.getGfxStaticScene() )
			mpBlobShadow->draw(params);
	}
}

//*****************************************************************************
void VuGhostCarEntity::drawShadow(const VuGfxDrawShadowParams &params)
{
	if ( mAlpha < 0.5f )
		return;

	const VuMatrix &transform = mpTransformComponent->getWorldTransform();

	float dist = (transform.getTrans() - params.mEyePos).mag();

	// draw chassis
	if ( dist > mChassisLod2Dist )
		mChassisLod2.drawShadow(transform, params);
	else if ( dist > mChassisLod1Dist )
		mChassisLod1.drawShadow(transform, params);
	else
		mChassis.drawShadow(transform, params);

	// draw suspension
	mSuspension.drawShadow(transform, params);

	// draw wheels
	for ( int i = 0; i < 4; i++ )
	{
		Corner &corner = mCorners[i];
		if ( dist > corner.mLod1Dist )
			corner.mWheelModel.drawShadow(corner.mWheelTransform, params);
		else
			corner.mWheelModelLod1.drawShadow(corner.mWheelTransform, params);
	}

	// draw driver
	if ( dist > mDriverLod2Dist )
		mDriverLod2.drawShadow(mDriverTransform*transform, params);
	else if ( dist > mDriverLod1Dist )
		mDriverLod1.drawShadow(mDriverTransform*transform, params);
	else
		mDriver.drawShadow(mDriverTransform*transform, params);
}

//*****************************************************************************
void VuGhostCarEntity::configShaderLOD(int value)
{
	// update skins
	for ( int iCorner = 0; iCorner < 4; iCorner++ )
	{
		Corner &corner = mCorners[iCorner];
		corner.mBlurSkin.build(corner.mWheelModel.getGfxStaticScene(), corner.mBlurSkinData);
	}

	mSkin.build(mChassis.getGfxStaticScene(), mSkinData);
	mSkinLod1.build(mChassisLod1.getGfxStaticScene(), mSkinData);
	mSkinLod2.build(mChassisLod1.getGfxStaticScene(), mSkinData);
}