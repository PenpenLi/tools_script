//*****************************************************************************
//
//  Copyright (c) 2012-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Ui Car entities
// 
//*****************************************************************************

#include "VuUiCar.h"
#include "VuPurple/Entities/Car/Parts/VuCarWheel.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Assets/VuAssetFactory.h"
#include "VuEngine/Assets/VuAnimationAsset.h"
#include "VuEngine/Animation/VuSkeleton.h"
#include "VuEngine/Animation/VuAnimatedSkeleton.h"
#include "VuEngine/Animation/VuAnimationControl.h"
#include "VuEngine/Gfx/GfxScene/VuGfxStaticScene.h"
#include "VuEngine/Gfx/Shadow/VuBlobShadow.h"
#include "VuEngine/Managers/VuConfigManager.h"


//*****************************************************************************
VuUiCar::VuUiCar(bool dropShadowEnabled):
	mCurStage(0),
	mCarColor(255,255,255),
	mDriverColor(255,255,255),
	mpAnimatedSkeleton(VUNULL),
	mpBlobShadow(VUNULL),
	mBlobShadowDirty(true),
	mTransform(VuMatrix::identity()),
	mDriverTransform(VuMatrix::identity()),
	mAabb(VuAabb::zero()),
	mCollisionAabb(VuAabb::zero()),
	mOffsetZ(0)
{
	for ( int i = 0; i < 4; i++ )
		mWheelTransforms[i].loadIdentity();

	mChassis.setMaterialSubstIF(&mSkin);
	mChassis.enableTranslucentDepth(true);
	mSuspension.enableTranslucentDepth(true);
	mFrontWheel.enableTranslucentDepth(true);
	mRearWheel.enableTranslucentDepth(true);
	mDriver.enableTranslucentDepth(true);

	if ( dropShadowEnabled )
	{
		mpBlobShadow = new VuBlobShadow;

		const VuJsonContainer &blobShadowData = VuGameUtil::IF()->constantDB()["Car"]["BlobShadow"];
		mpBlobShadow->setTexture(blobShadowData["Texture"].asString());
		blobShadowData["Alpha"].getValue(mpBlobShadow->params().mAlpha);
	}

	VuConfigManager::IF()->registerIntHandler("Gfx/ShaderLOD", this, &VuUiCar::configShaderLOD);
}

//*****************************************************************************
VuUiCar::~VuUiCar()
{
	VU_SAFE_RELEASE(mpAnimatedSkeleton);

	delete mpBlobShadow;

	VuConfigManager::IF()->unregisterIntHandler("Gfx/ShaderLOD", this);
}

//*****************************************************************************
void VuUiCar::setCar(const std::string &car, int stage)
{
	if ( car != mCurCar || stage != mCurStage )
	{
		mCurCar = car;
		mCurStage = stage;

		mSkin.clear();
		mCurDecal.clear();
		mCurPaintColor.clear();
		mCurDecalColor.clear();

		const VuJsonContainer &carData = VuGameUtil::IF()->carDB()[mCurCar];
		const VuJsonContainer &stageData = carData["Stages"][mCurStage];
		const VuJsonContainer &frontWheelData = VuGameUtil::IF()->wheelDB()[carData["FrontWheels"].asString()];
		const VuJsonContainer &rearWheelData = VuGameUtil::IF()->wheelDB()[carData["RearWheels"].asString()];

		mChassis.setModelAsset(stageData["Model Asset"].asString());
		mSuspension.setModelAsset(carData["Suspension"]["Model Asset"].asString());
		mFrontWheel.setModelAsset(frontWheelData["Model Asset"].asString());
		mRearWheel.setModelAsset(rearWheelData["Model Asset"].asString());

		VuVector3 driverOffset(0,0,0);
		VuDataUtil::getValue(carData["Driver Offset"], driverOffset);
		mDriverTransform.setTrans(driverOffset);

		const VuJsonContainer &corners = carData["Suspension"]["Corners"];
		for ( int i = 0; i < 4; i++ )
		{
			mWheelTransforms[i].loadIdentity();

			if ( mSuspension.getSkeleton() )
			{
				int boneIndex = mSuspension.getSkeleton()->getBoneIndex(corners[i]["WheelBone"].asCString());
				if ( boneIndex >= 0 )
				{
					const VuVector3 &attachmentPointLS = mSuspension.getModelMatrices()[boneIndex].getTrans(); 

					mWheelTransforms[i].translateLocal(attachmentPointLS);
					if ( i == VuCarWheel::FRONT_LEFT || i == VuCarWheel::REAR_LEFT )
						mWheelTransforms[i].rotateZLocal(VU_PI);
				}
			}
		}

		mAabb = mAabb.zero();
		mAabb.addAabb(mChassis.getAabb());
		mAabb.addAabb(mSuspension.getLocalAabb());
		mAabb.addAabb(mFrontWheel.getAabb(), mWheelTransforms[0]);
		mAabb.addAabb(mFrontWheel.getAabb(), mWheelTransforms[1]);
		mAabb.addAabb(mRearWheel.getAabb(), mWheelTransforms[2]);
		mAabb.addAabb(mRearWheel.getAabb(), mWheelTransforms[3]);

		VuDataUtil::getValue(carData["Collision Aabb"], mCollisionAabb);

		mOffsetZ = carData["UI Z Offset"].asFloat();

		mBlobShadowDirty = true;
	}
}

//*****************************************************************************
void VuUiCar::setSkin(const std::string &decal, const std::string &paintColor, const std::string &decalColor)
{
	if ( decal != mCurDecal || paintColor != mCurPaintColor || decalColor != mCurDecalColor )
	{
		mCurDecal = decal;
		mCurPaintColor = paintColor;
		mCurDecalColor = decalColor;

		std::string decalTexture;
		VuColor decalColor, paintColor;
		VuGameUtil::IF()->getDecal(mCurDecal, decalTexture);
		VuGameUtil::IF()->getColor(mCurPaintColor, paintColor);
		VuGameUtil::IF()->getColor(mCurDecalColor, decalColor);

		VuJsonContainer skinData;
		VuDataUtil::putValue(skinData["Textures"]["DecalTexture"], decalTexture);
		VuDataUtil::putValue(skinData["Constants"]["PaintColor"], paintColor.toVector3());
		VuDataUtil::putValue(skinData["Constants"]["DecalColor"], decalColor.toVector3());

		mSkin.build(mChassis.getGfxStaticScene(), skinData);
	}
}

//*****************************************************************************
void VuUiCar::setSkin(const std::string &carSkin)
{
	const VuJsonContainer &skinData = VuGameUtil::IF()->carSkinDB()[carSkin];

	setSkin(skinData["Decal"].asString(), skinData["PaintColor"].asString(), skinData["DecalColor"].asString());
}

//*****************************************************************************
void VuUiCar::setDriver(const std::string &driver)
{
	if ( driver != mCurDriver )
	{
		mCurDriver = driver;

		VU_SAFE_RELEASE(mpAnimatedSkeleton);
		mCurDriverAnim.clear();

		const VuJsonContainer &driverData = VuGameUtil::IF()->driverDB()[mCurDriver];

		mDriver.setModelAsset(driverData["Model Asset"].asString());
	}
}

//*****************************************************************************
void VuUiCar::setDriverAnim(const std::string &anim)
{
	if ( anim != mCurDriverAnim )
	{
		mCurDriverAnim = anim;

		VU_SAFE_RELEASE(mpAnimatedSkeleton);

		if ( mDriver.getSkeleton() )
		{
			if ( VuAssetFactory::IF()->doesAssetExist<VuAnimationAsset>(mCurDriverAnim) )
			{
				mpAnimatedSkeleton = new VuAnimatedSkeleton(mDriver.getSkeleton());
				VuAnimationControl *pAnimControl = new VuAnimationControl(mCurDriverAnim);
				mpAnimatedSkeleton->addAnimationControl(pAnimControl);
				pAnimControl->removeRef();
			}
		}
	}
}

//*****************************************************************************
void VuUiCar::tickDecision(float fdt)
{
	if ( mBlobShadowDirty && mpBlobShadow )
	{
		VuMatrix transform = mTransform;
		transform.translateLocal(VuVector3(0.0f,0.0f,mOffsetZ));
		mpBlobShadow->calculate(transform, mCollisionAabb);
		mBlobShadowDirty = false;
	}
}

//*****************************************************************************
void VuUiCar::tickAnim(float fdt)
{
	if ( mpAnimatedSkeleton )
	{
		mpAnimatedSkeleton->advance(fdt);
		mpAnimatedSkeleton->build();

		mDriver.setPose(mpAnimatedSkeleton);
		mDriver.finalizePose();
	}
}

//*****************************************************************************
void VuUiCar::draw(const VuGfxDrawParams &params)
{
	VuMatrix transform = mTransform;
	transform.translateLocal(VuVector3(0.0f,0.0f,mOffsetZ));

	if ( mCarColor.mA )
	{
		mChassis.setColor(mCarColor);
		mChassis.draw(transform, params);

		mSuspension.setColor(mCarColor);
		mSuspension.draw(transform, params);

		mFrontWheel.setColor(mCarColor);
		mFrontWheel.draw(mWheelTransforms[0]*transform, params);
		mFrontWheel.draw(mWheelTransforms[1]*transform, params);

		mRearWheel.setColor(mCarColor);
		mRearWheel.draw(mWheelTransforms[2]*transform, params);
		mRearWheel.draw(mWheelTransforms[3]*transform, params);

		if ( mCarColor.mA == 255 )
		{
			if ( mpBlobShadow && mChassis.getGfxStaticScene() )
				mpBlobShadow->draw(params);
		}
	}

	if ( mDriverColor.mA )
	{
		mDriver.setColor(mDriverColor);
		mDriver.draw(mDriverTransform*transform, params);
	}
}

//*****************************************************************************
void VuUiCar::drawShadow(const VuGfxDrawShadowParams &params)
{
	VuMatrix transform = mTransform;
	transform.translateLocal(VuVector3(0.0f,0.0f,mOffsetZ));

	if ( mCarColor.mA == 255 )
	{
		mChassis.drawShadow(transform, params);
		mSuspension.drawShadow(transform, params);

		mFrontWheel.drawShadow(mWheelTransforms[0]*transform, params);
		mFrontWheel.drawShadow(mWheelTransforms[1]*transform, params);
		mRearWheel.drawShadow(mWheelTransforms[2]*transform, params);
		mRearWheel.drawShadow(mWheelTransforms[3]*transform, params);
	}

	if ( mDriverColor.mA == 255 )
	{
		mDriver.drawShadow(mDriverTransform*transform, params);
	}
}

//*****************************************************************************
void VuUiCar::configShaderLOD(int value)
{
	// update skin
	std::string decal = mCurDecal;
	std::string paintColor = mCurPaintColor;
	std::string decalColor = mCurDecalColor;

	mCurDecal.clear();
	mCurPaintColor.clear();
	mCurDecalColor.clear();

	setSkin(decal, paintColor, decalColor);
}