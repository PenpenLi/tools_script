//*****************************************************************************
//
//  Copyright (c) 2012-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Lava surface entity
// 
//*****************************************************************************

#include "VuPurple/Dynamics/VuCollisionTypes.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Water/VuWater.h"
#include "VuEngine/Components/Transform/VuTransformComponent.h"
#include "VuEngine/Components/3dDraw/Vu3dDrawComponent.h"
#include "VuEngine/Components/3dLayout/Vu3dLayoutComponent.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Components/Motion/VuMotionComponent.h"
#include "VuEngine/Properties/VuBasicProperty.h"
#include "VuEngine/Properties/VuEnumProperty.h"
#include "VuEngine/Properties/VuAssetProperty.h"
#include "VuEngine/Assets/VuAssetFactory.h"
#include "VuEngine/Assets/VuTextureAsset.h"
#include "VuEngine/Assets/VuCubeTextureAsset.h"
#include "VuEngine/Assets/VuWaterMapAsset.h"
#include "VuEngine/Managers/VuViewportManager.h"
#include "VuEngine/Gfx/Camera/VuCamera.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/Shaders/VuBasicShaders.h"
#include "VuEngine/HAL/Gfx/VuGfx.h"
#include "VuEngine/Math/VuMatrix.h"
#include "VuEngine/Math/VuMathUtil.h"


class VuLavaSurfaceEntity : public VuEntity, public VuMotionComponentIF
{
	DECLARE_RTTI

public:
	VuLavaSurfaceEntity();
	~VuLavaSurfaceEntity();

	virtual void		onPostLoad();
	virtual void		onGameInitialize();
	virtual void		onGameRelease();

protected:
	virtual void		surfaceModified();
	void				shaderModified();
	void				createSurfaceDesc(VuWaterSurfaceDesc &desc);
	void				createShaderDesc(VuWaterShaderDesc &desc);
	virtual void		drawLayout(const Vu3dLayoutDrawParams &params);
	virtual bool		collideLayout(const VuVector3 &v0, VuVector3 &v1);
	void				draw(const VuGfxDrawParams &params);

	// VuMotionComponentIF interface
	virtual void		onMotionUpdate();

	Vu3dDrawComponent		*mp3dDrawComponent;
	Vu3dLayoutComponent		*mp3dLayoutComponent;
	VuScriptComponent		*mpScriptComponent;
	VuMotionComponent		*mpMotionComponent;

	typedef VuAssetProperty<VuWaterMapAsset> WaterMapAsset;

	int					mSizeX;
	int					mSizeY;
	float				mMaxWaveDepth;
	float				mMaxWaveHeight;
	float				mDrawDist;
	std::string			mWaterMapAssetName;

	VuWaterShaderDesc	mShaderDesc;
	VuWaterSurface		*mpSurface;
	VuWaterShader		*mpShader;

	WaterMapAsset		*mpWaterMapAssetProperty;
};


IMPLEMENT_RTTI(VuLavaSurfaceEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuLavaSurfaceEntity);


// choices for size
static VuStaticIntEnumProperty::Choice sSizeChoices[] =
{
	{ "1", 1},
	{ "2", 2},
	{ "4", 4},
	{ "8", 8},
	{ "16", 16},
	{ "32", 32},
	{ "64", 64},
	{ "128", 128},
	{ "256", 256},
	{ "512", 512},
	{ "1024", 1024},
	{ "2048", 2048},
	{ "4096", 4096},
	{ "8192", 8192},
	{ VUNULL }
};

//*****************************************************************************
VuLavaSurfaceEntity::VuLavaSurfaceEntity():
	mSizeX(256),
	mSizeY(256),
	mMaxWaveDepth(5),
	mMaxWaveHeight(5),
	mDrawDist(FLT_MAX),
	mpSurface(VUNULL),
	mpShader(VUNULL)
{
	mShaderDesc.mOverrideGlobalSettings = true;
	mShaderDesc.mProceduralReflection = false;
	mShaderDesc.mNormalMapEnabled = false;
	mShaderDesc.mReflectionCubeTextureAsset = "";

	// properties
	addProperty(new VuStaticIntEnumProperty("X Size", mSizeX, sSizeChoices))	->	setWatcher(this, &VuLavaSurfaceEntity::surfaceModified);
	addProperty(new VuStaticIntEnumProperty("Y Size", mSizeY, sSizeChoices))	->	setWatcher(this, &VuLavaSurfaceEntity::surfaceModified);
	addProperty(new VuFloatProperty("Max Wave Depth", mMaxWaveDepth))		->	setWatcher(this, &VuLavaSurfaceEntity::surfaceModified);
	addProperty(new VuFloatProperty("Max Wave Height", mMaxWaveHeight))		->	setWatcher(this, &VuLavaSurfaceEntity::surfaceModified);
	addProperty(new VuFloatProperty("Draw Distance", mDrawDist));
	addProperty(mpWaterMapAssetProperty = new VuAssetProperty<VuWaterMapAsset>("WaterMap", mWaterMapAssetName));

	addProperty(new VuColorProperty("AmbientColor", mShaderDesc.mAmbientColor))				->	setWatcher(this, &VuLavaSurfaceEntity::shaderModified);
	addProperty(new VuColorProperty("DiffuseColor", mShaderDesc.mDiffuseColor))				->	setWatcher(this, &VuLavaSurfaceEntity::shaderModified);
	addProperty(new VuBoolProperty("FogEnabled", mShaderDesc.mFogEnabled))					->	setWatcher(this, &VuLavaSurfaceEntity::shaderModified);
	addProperty(new VuFloatProperty("FoamTextureSize", mShaderDesc.mFoamTextureSize))		->	setWatcher(this, &VuLavaSurfaceEntity::shaderModified);
	addProperty(new VuAssetNameProperty(VuTextureAsset::msRTTI.mstrType, "FoamTextureAsset", mShaderDesc.mFoamTextureAsset))
		->	setWatcher(this, &VuLavaSurfaceEntity::shaderModified);
	addProperty(new VuColorProperty("DecalAmbientColor", mShaderDesc.mDecalAmbientColor))	->	setWatcher(this, &VuLavaSurfaceEntity::shaderModified);
	addProperty(new VuColorProperty("DecalDiffuseColor", mShaderDesc.mDecalDiffuseColor))	->	setWatcher(this, &VuLavaSurfaceEntity::shaderModified);
	addProperty(new VuAssetNameProperty(VuTextureAsset::msRTTI.mstrType, "DecalTextureAsset", mShaderDesc.mDecalTextureAsset))
		->	setWatcher(this, &VuLavaSurfaceEntity::shaderModified);

	// components
	addComponent(mp3dDrawComponent = new Vu3dDrawComponent(this));
	addComponent(mp3dLayoutComponent = new Vu3dLayoutComponent(this));
	addComponent(mpScriptComponent = new VuScriptComponent(this, 150, false));
	addComponent(mpMotionComponent = new VuMotionComponent(this, this));

	mp3dDrawComponent->setDrawMethod(this, &VuLavaSurfaceEntity::draw);
	mp3dLayoutComponent->setDrawMethod(this, &VuLavaSurfaceEntity::drawLayout);
	mp3dLayoutComponent->setCollideMethod(this, &VuLavaSurfaceEntity::collideLayout);

	// want to know when Transform is changed
	mpTransformComponent->setWatcher(&VuLavaSurfaceEntity::surfaceModified);

	// limit manipulation to translation and rotation about Z
	mpTransformComponent->setMask(VuTransformComponent::TRANS|VuTransformComponent::ROT_Z);

	surfaceModified();
}

//*****************************************************************************
VuLavaSurfaceEntity::~VuLavaSurfaceEntity()
{
	// destroy shader
	if ( mpShader )
	{
		mpShader->removeRef();
		mpShader = VUNULL;
	}
}

//*****************************************************************************
void VuLavaSurfaceEntity::onPostLoad()
{
	surfaceModified();
	shaderModified();
}

//*****************************************************************************
void VuLavaSurfaceEntity::onGameInitialize()
{
	// create surface
	VuWaterSurfaceDesc surfaceDesc;
	createSurfaceDesc(surfaceDesc);
	mpSurface = VuWater::IF()->createSurface(surfaceDesc, this);

	// start rendering
	mp3dDrawComponent->show();
}

//*****************************************************************************
void VuLavaSurfaceEntity::onGameRelease()
{
	// stop rendering
	mp3dDrawComponent->hide();

	// destroy surface
	mpSurface->removeRef();
	mpSurface = VUNULL;
}

//*****************************************************************************
void VuLavaSurfaceEntity::surfaceModified()
{
	// modify surface
	VuWaterSurfaceDesc desc;
	createSurfaceDesc(desc);
	if ( mpSurface )
		mpSurface->modify(desc);

	// update bounds
	VuAabb aabb;
	aabb.mMin = VuVector3(-0.5f*mSizeX, -0.5f*mSizeY, -mMaxWaveDepth);
	aabb.mMax = VuVector3( 0.5f*mSizeX,  0.5f*mSizeY,  mMaxWaveHeight);
	mp3dDrawComponent->updateVisibility(aabb, mpTransformComponent->getWorldTransform());

	mp3dLayoutComponent->setLocalBounds(aabb);
}

//*****************************************************************************
void VuLavaSurfaceEntity::shaderModified()
{
	if ( mpShader )
	{
		mpShader->removeRef();
		mpShader = VUNULL;
	}

	if ( VuWater::IF() )
	{
		VuWaterShaderDesc shaderDesc;
		createShaderDesc(shaderDesc);
		mpShader = VuWater::IF()->createShader(shaderDesc);
	}
}

//*****************************************************************************
void VuLavaSurfaceEntity::createSurfaceDesc(VuWaterSurfaceDesc &desc)
{
	memset(&desc, 0, sizeof(desc));
	desc.mPos = mpTransformComponent->getWorldPosition();
	desc.mRotZ = mpTransformComponent->getWorldRotation().mZ;
	desc.mPowSizeX = VuRound(VuLog((float)mSizeX)/VuLog(2.0f));
	desc.mPowSizeY = VuRound(VuLog((float)mSizeY)/VuLog(2.0f));
	desc.mMaxWaveDepth = mMaxWaveDepth;
	desc.mMaxWaveHeight = mMaxWaveHeight;
	desc.mpWaterMapAsset = mpWaterMapAssetProperty->getAsset();
	desc.mProceduralReflection = mShaderDesc.mProceduralReflection;
	desc.mFlags = WATER_SURFACE_LAVA;
}

//*****************************************************************************
void VuLavaSurfaceEntity::createShaderDesc(VuWaterShaderDesc &desc)
{
	desc = mShaderDesc;

	if ( desc.mFogEnabled )
		desc.mShaderNameOverride = "Water/Lava/Fog";
	else
		desc.mShaderNameOverride = "Water/Lava/Basic";
}

//*****************************************************************************
void VuLavaSurfaceEntity::drawLayout(const Vu3dLayoutDrawParams &params)
{
	float x0 = -0.5f*mSizeX;
	float y0 = -0.5f*mSizeY;
	float x1 = 0.5f*mSizeX;
	float y1 = 0.5f*mSizeY;

	// draw surface
	{
		VuVector3 verts[4];
		verts[0] = VuVector3(x0, y0, 0.0f);
		verts[1] = VuVector3(x1, y0, 0.0f);
		verts[2] = VuVector3(x0, y1, 0.0f);
		verts[3] = VuVector3(x1, y1, 0.0f);

		VuGfxUtil::IF()->drawTriangleStrip(mShaderDesc.mDiffuseColor, verts, 4, mpTransformComponent->getWorldTransform()*params.mCamera.getViewProjMatrix());
	}
}

//*****************************************************************************
bool VuLavaSurfaceEntity::collideLayout(const VuVector3 &v0, VuVector3 &v1)
{
	float x0 = -0.5f*mSizeX;
	float y0 = -0.5f*mSizeY;
	float x1 = 0.5f*mSizeX;
	float y1 = 0.5f*mSizeY;

	VuVector3 verts[4];
	verts[0] = VuVector3(x0, y0, 0.0f);
	verts[1] = VuVector3(x1, y0, 0.0f);
	verts[2] = VuVector3(x1, y1, 0.0f);
	verts[3] = VuVector3(x0, y1, 0.0f);
	for ( int i = 0; i < 4; i++ )
		verts[i] = mpTransformComponent->getWorldTransform().transform(verts[i]);

	bool hit = false;
	hit |= VuMathUtil::triangleLineSegIntersection(verts[0], verts[1], verts[2], v0, v1, v1);
	hit |= VuMathUtil::triangleLineSegIntersection(verts[2], verts[3], verts[0], v0, v1, v1);

	return hit;
}

//*****************************************************************************
void VuLavaSurfaceEntity::draw(const VuGfxDrawParams &params)
{
	float distSquared = (mp3dDrawComponent->getAabb().getCenter() - params.mEyePos).magSquared();
	if ( distSquared < mDrawDist*mDrawDist )
	{
		VuWaterRendererParams waterParams(mpSurface, mpShader, &params.mCamera);
		VuWater::IF()->renderer()->submit(waterParams);
	}
}

//*****************************************************************************
void VuLavaSurfaceEntity::onMotionUpdate()
{
	mpTransformComponent->setWorldTransform(mpMotionComponent->getWorldTransform());
}
