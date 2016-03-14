//*****************************************************************************
//
//  Copyright (c) 2010-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  PreviewGameMode class
// 
//*****************************************************************************

#include "VuPreviewGameMode.h"
#include "VuPreviewEntity.h"
#include "VuPurple/Modes/VuGameModeManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Managers/VuViewportManager.h"
#include "VuEngine/Managers/VuInputManager.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/GfxSort/VuGfxSort.h"
#include "VuEngine/Gfx/Light/VuLightManager.h"
#include "VuEngine/HAL/Gfx/VuGfx.h"
#include "VuEngine/HAL/File/VuFile.h"
#include "VuEngine/Json/VuJsonContainer.h"
#include "VuEngine/Json/VuJsonReader.h"
#include "VuEngine/Json/VuJsonWriter.h"
#include "VuEngine/Util/VuDataUtil.h"
#include "VuEngine/Dev/VuDevMenu.h"


IMPLEMENT_RTTI(VuPreviewGameMode, VuGameMode);


#define SESSION_SETTINGS_FILE "Tools/Preview.user"


// options
class VuPreviewOptions
{
public:
	VuPreviewOptions();
	
	void	load();
	void	save() const;

	bool	mbGrid;
	bool	mbInstanceNames;
	bool	mbMeshNames;
	bool	mbMeshBounds;
	bool	mbMeshPartBounds;
	bool	mbBones;
	bool	mbBoneNames;
	bool	mbSceneInfo;
	bool	mbAnimationTimeLine;
	bool	mbTarget;
	bool	mbLightVector;
	float	mNearPlane;
	float	mFarPlane;
	VuColor	mBackgroundColor;
	VuColor	mGridColor;
	VuColor	mLineColor;
	VuColor	mTextColor;
	VuColor	mAmbientLightColor;
	VuColor	mDirectionalLightFrontColor;
	VuColor	mDirectionalLightBackColor;
	VuColor	mDirectionalLightSpecularColor;
};


//*****************************************************************************
VuPreviewGameMode::VuPreviewGameMode():
	mpOptions(VUNULL),
	mpPreviewEntity(VUNULL),
	mLightRotX(70.0f),
	mLightRotZ(-150.0f)
{
}

//*****************************************************************************
VuPreviewGameMode::~VuPreviewGameMode()
{
	delete mpOptions;
}

//*****************************************************************************
bool VuPreviewGameMode::enter(const std::string &prevMode)
{
	// create/load options
	mpOptions = new VuPreviewOptions;
	mpOptions->load();

	// set 1 viewport
	VuViewportManager::IF()->reset();
	VuViewportManager::IF()->setViewportCount( 1);

	// create preview entity
	mpPreviewEntity = new VuPreviewEntity;

	// load scene
	VuJsonReader reader;
	VuJsonContainer sceneData, auxData;
	if ( VuFile::IF()->exists("PreviewScene.json") )
		reader.loadFromFile(sceneData, "PreviewScene.json");
	if ( VuFile::IF()->exists("PreviewAux.json") )
		reader.loadFromFile(auxData, "PreviewAux.json");
	if ( !mpPreviewEntity->loadScene(sceneData, auxData) )
		return false;

	return true;
}

//*****************************************************************************
void VuPreviewGameMode::exit()
{
	// unregister callbacks/handlers
	VuTickManager::IF()->unregisterHandlers(this);

	// save/destroy options
	mpOptions->save();

	// reset viewports
	VuViewportManager::IF()->reset();

	// destroy preview entity
	mpPreviewEntity->removeRef();
}

//*****************************************************************************
const char *VuPreviewGameMode::tick(float fdt)
{
	mDebugCamera.tick(VuTickManager::IF()->getRealDeltaTime(), 0);
	tickLight(VuTickManager::IF()->getRealDeltaTime());

	// update ProjViewMatrix
	float aspectRatio = VuGameUtil::IF()->calcDisplayAspectRatio();
	float vertFov = VuGameUtil::IF()->calcCameraVertFov();
	mDebugCamera.setProjMatrixVert(vertFov, aspectRatio, mpOptions->mNearPlane, mpOptions->mFarPlane);

	VuViewportManager::IF()->setCamera(0, mDebugCamera);
	VuViewportManager::IF()->setUiCameraVert(mDebugCamera.getFovVert(), mDebugCamera.getAspectRatio(), mDebugCamera.getNearPlane(), mDebugCamera.getFarPlane());

	// ambient light
	{
		VuLightManager::IF()->ambientLight().mColor = mpOptions->mAmbientLightColor;
	}

	// diffuse light
	{
		VuLightManager::IF()->directionalLight().mFrontColor = mpOptions->mDirectionalLightFrontColor;
		VuLightManager::IF()->directionalLight().mBackColor = mpOptions->mDirectionalLightBackColor;
		VuLightManager::IF()->directionalLight().mSpecularColor = mpOptions->mDirectionalLightSpecularColor;
		VuLightManager::IF()->directionalLight().mDirection = calcLightDirection();
	}

	// set draw info params
	{
		VUUINT32 flags = 0;
		if ( mpOptions->mbInstanceNames )		flags |= VuGfxDrawInfoParams::INSTANCE_NAMES;
		if ( mpOptions->mbMeshNames )			flags |= VuGfxDrawInfoParams::MESH_NAMES;
		if ( mpOptions->mbMeshBounds )			flags |= VuGfxDrawInfoParams::MESH_BOUNDS;
		if ( mpOptions->mbMeshPartBounds )		flags |= VuGfxDrawInfoParams::MESH_PART_BOUNDS;
		if ( mpOptions->mbBones )				flags |= VuGfxDrawInfoParams::BONES;
		if ( mpOptions->mbBoneNames )			flags |= VuGfxDrawInfoParams::BONE_NAMES;
		if ( mpOptions->mbSceneInfo )			flags |= VuGfxDrawInfoParams::SCENE_INFO;
		if ( mpOptions->mbAnimationTimeLine )	flags |= VuGfxDrawInfoParams::ANIMATION_TIMELINE;
		mpPreviewEntity->setDrawInfoParams(flags, mpOptions->mTextColor, mpOptions->mLineColor);
	}

	return "";
}

//*****************************************************************************
void VuPreviewGameMode::draw()
{
	// clear command
	VuGfxSort::IF()->setFullScreenLayer(VuGfxSort::FSL_GAME);
	VuGfxSort::IF()->setViewport(0);
	VuGfxSort::IF()->setReflectionLayer(VuGfxSort::REFLECTION_OFF);
	VuGfxSort::IF()->setViewportLayer(VuGfxSort::VPL_DEPTH);

	VuGfxUtil::IF()->submitClearCommand(VUGFX_CLEAR_COLOR, mpOptions->mBackgroundColor, 1.0f);

	// draw grid?
	if ( mpOptions->mbGrid )
		drawGrid(mDebugCamera.getViewProjMatrix(), mpOptions->mGridColor);

	// draw the rest of the stuff w/o depth testing
	VuGfxSort::IF()->setFullScreenLayer(VuGfxSort::FSL_HUD);
	VuGfxSort::IF()->setViewport(0);
	VuGfxSort::IF()->setReflectionLayer(0);
	VuGfxSort::IF()->setViewportLayer(0);

	VuGfxUtil::IF()->submitClearCommand(VUGFX_CLEAR_DEPTH, VuColor(0,0,0), 1.0f);

	// draw target?
	if ( mpOptions->mbTarget )
	{
		VuVector3 vTarget = mDebugCamera.getTargetPosition();
		VuGfxUtil::IF()->drawLine3d(VuColor(255,   0,   0), vTarget, vTarget + VuVector3(1, 0, 0), mDebugCamera.getViewProjMatrix());
		VuGfxUtil::IF()->drawLine3d(VuColor(  0, 255,   0), vTarget, vTarget + VuVector3(0, 1, 0), mDebugCamera.getViewProjMatrix());
		VuGfxUtil::IF()->drawLine3d(VuColor(  0,   0, 255), vTarget, vTarget + VuVector3(0, 0, 1), mDebugCamera.getViewProjMatrix());
	}

	// draw light vector?
	if ( mpOptions->mbLightVector )
		drawLightVector(mDebugCamera, mpOptions->mDirectionalLightFrontColor);
}

//*****************************************************************************
void VuPreviewGameMode::drawGrid(const VuMatrix &mat, const VuColor &color)
{
	// calculate size of grid (based on scene aabb)
	float fHalfSize = 5.0f;
	fHalfSize = VuMax(fHalfSize, VuAbs(mpPreviewEntity->getAabb().mMin.mX));
	fHalfSize = VuMax(fHalfSize, VuAbs(mpPreviewEntity->getAabb().mMin.mY));
	fHalfSize = VuMax(fHalfSize, VuAbs(mpPreviewEntity->getAabb().mMax.mX));
	fHalfSize = VuMax(fHalfSize, VuAbs(mpPreviewEntity->getAabb().mMax.mY));

	int nSubDivs = 5;
	for ( int i = -(nSubDivs+1); i <= (nSubDivs+1); i++ )
	{
		float fT = (float)i/(float)(nSubDivs+1);
		VuGfxUtil::IF()->drawLine3d(color, fHalfSize*VuVector3(fT, -1, 0), fHalfSize*VuVector3(fT,  1, 0), mat);
		VuGfxUtil::IF()->drawLine3d(color, fHalfSize*VuVector3(-1, fT, 0), fHalfSize*VuVector3( 1, fT, 0), mat);
	}
}

//*****************************************************************************
void VuPreviewGameMode::drawLightVector(const VuCamera &camera, const VuColor &color)
{
	// calculate radius of cylinder (on distance from eye to target)
	float fRadius = 0.1f*(camera.getTargetPosition() - camera.getEyePosition()).mag();

	// build rotation matrix
	VuMatrix mat;
	mat.loadIdentity();
	mat.rotateZLocal(VuDegreesToRadians(-mLightRotZ));
	mat.rotateXLocal(VuDegreesToRadians(-mLightRotX));
	mat.rotateXLocal(VuDegreesToRadians(90.0f));
	mat.translate(camera.getTargetPosition());
	mat = mat*camera.getViewProjMatrix();

	VuVector3 vDir = VuLightManager::IF()->directionalLight().mDirection;
	VuGfxUtil::IF()->drawCylinderLines(mpOptions->mDirectionalLightFrontColor, 1000.0f, fRadius, 8, mat) ;
}

//*****************************************************************************
void VuPreviewGameMode::tickLight(float fdt)
{
	// read pad 0
	mLightRotX += 90.0f*fdt*VuInputManager::IF()->getAxisValue(0, "PreviewLightRotX");
	mLightRotZ += 90.0f*fdt*VuInputManager::IF()->getAxisValue(0, "PreviewLightRotZ");

	mLightRotX = VuMin(VuMax(mLightRotX, -90.0f), 90.0f);
	while ( mLightRotZ <= -180.0f )	mLightRotZ += 360.0f;
	while ( mLightRotZ > 180.0f )	mLightRotZ -= 360.0f;
}

//*****************************************************************************
VuVector3 VuPreviewGameMode::calcLightDirection()
{
	// build rotation matrix
	VuMatrix mat;
	mat.loadIdentity();
	mat.rotateZLocal(VuDegreesToRadians(-mLightRotZ));
	mat.rotateXLocal(VuDegreesToRadians(-mLightRotX));

	return mat.transformNormal(VuVector3(0,1,0));
}

//*****************************************************************************
VuPreviewOptions::VuPreviewOptions():
	mbGrid(true),
	mbInstanceNames(false),
	mbMeshNames(false),
	mbMeshBounds(false),
	mbMeshPartBounds(false),
	mbBones(false),
	mbBoneNames(false),
	mbSceneInfo(false),
	mbAnimationTimeLine(true),
	mbTarget(false),
	mbLightVector(false),
	mNearPlane(1.0f),
	mFarPlane(250.0f),
	mBackgroundColor(160,160,160),
	mGridColor(128,128,128),
	mLineColor(255,255,64),
	mTextColor(255,255,255),
	mAmbientLightColor(32,32,32),
	mDirectionalLightFrontColor(255,255,255),
	mDirectionalLightBackColor(64,64,64),
	mDirectionalLightSpecularColor(255,255,255)
{
	// add to dev menu
	VuDevMenu::IF()->addBool("Options/Grid", mbGrid);
	VuDevMenu::IF()->addBool("Options/Instance Names", mbInstanceNames);
	VuDevMenu::IF()->addBool("Options/Mesh Names", mbMeshNames);
	VuDevMenu::IF()->addBool("Options/Mesh Bounds", mbMeshBounds);
	VuDevMenu::IF()->addBool("Options/Mesh Part Bounds", mbMeshPartBounds);
	VuDevMenu::IF()->addBool("Options/Bones", mbBones);
	VuDevMenu::IF()->addBool("Options/Bone Names", mbBoneNames);
	VuDevMenu::IF()->addBool("Options/Scene Info", mbSceneInfo);
	VuDevMenu::IF()->addBool("Options/Animation Time Line", mbAnimationTimeLine);
	VuDevMenu::IF()->addBool("Options/Target", mbTarget);
	VuDevMenu::IF()->addBool("Options/Light Vector", mbLightVector);
	VuDevMenu::IF()->addFloat("Options/Near Plane", mNearPlane, 0.1f, 0.1f, 9.9f);
	VuDevMenu::IF()->addFloat("Options/Far Plane", mFarPlane, 10, 250, 1000);

	VuDevMenu::IF()->addColor3("Options/BackgroundColor", mBackgroundColor);
	VuDevMenu::IF()->addColor3("Options/GridColor", mGridColor);
	VuDevMenu::IF()->addColor3("Options/LineColor", mLineColor);
	VuDevMenu::IF()->addColor3("Options/TextColor", mTextColor);
	VuDevMenu::IF()->addColor3("Options/AmbientLightColor", mAmbientLightColor);
	VuDevMenu::IF()->addColor3("Options/DirectionalLightFrontColor", mDirectionalLightFrontColor);
	VuDevMenu::IF()->addColor3("Options/DirectionalLightBackColor", mDirectionalLightBackColor);
	VuDevMenu::IF()->addColor3("Options/DirectionalLightSpecularColor", mDirectionalLightSpecularColor);
}

//*****************************************************************************
void VuPreviewOptions::load()
{
	if ( !VuFile::IF()->exists(SESSION_SETTINGS_FILE) )
		return;

	VuJsonContainer options;
	VuJsonReader reader;
	if ( !reader.loadFromFile(options, SESSION_SETTINGS_FILE) )
		VUERROR("Unable to load session settings:\n%s", reader.getLastError().c_str());

	VuDataUtil::getValue(options["Grid"], mbGrid);
	VuDataUtil::getValue(options["InstanceNames"], mbInstanceNames);
	VuDataUtil::getValue(options["MeshNames"], mbMeshNames);
	VuDataUtil::getValue(options["MeshBounds"], mbMeshBounds);
	VuDataUtil::getValue(options["MeshPartBounds"], mbMeshPartBounds);
	VuDataUtil::getValue(options["Bones"], mbBones);
	VuDataUtil::getValue(options["BoneNames"], mbBoneNames);
	VuDataUtil::getValue(options["SceneInfo"], mbSceneInfo);
	VuDataUtil::getValue(options["AnimationTimeLine"], mbAnimationTimeLine);
	VuDataUtil::getValue(options["Target"], mbTarget);
	VuDataUtil::getValue(options["LightVector"], mbLightVector);

	VuDataUtil::getValue(options["NearPlane"], mNearPlane);
	VuDataUtil::getValue(options["FarPlane"], mFarPlane);

	VuDataUtil::getValue(options["BackgroundColor"], mBackgroundColor);
	VuDataUtil::getValue(options["GridColor"], mGridColor);
	VuDataUtil::getValue(options["LineColor"], mLineColor);
	VuDataUtil::getValue(options["TextColor"], mTextColor);
	VuDataUtil::getValue(options["AmbientLightColor"], mAmbientLightColor);
	VuDataUtil::getValue(options["DirectionalLightFrontColor"], mDirectionalLightFrontColor);
	VuDataUtil::getValue(options["DirectionalLightBackColor"], mDirectionalLightBackColor);
	VuDataUtil::getValue(options["DirectionalLightSpecularColor"], mDirectionalLightSpecularColor);
}

//*****************************************************************************
void VuPreviewOptions::save() const
{
	VuJsonContainer options;

	VuDataUtil::putValue(options["Grid"], mbGrid);
	VuDataUtil::putValue(options["InstanceNames"], mbInstanceNames);
	VuDataUtil::putValue(options["MeshNames"], mbMeshNames);
	VuDataUtil::putValue(options["MeshBounds"], mbMeshBounds);
	VuDataUtil::putValue(options["MeshPartBounds"], mbMeshPartBounds);
	VuDataUtil::putValue(options["Bones"], mbBones);
	VuDataUtil::putValue(options["BoneNames"], mbBoneNames);
	VuDataUtil::putValue(options["SceneInfo"], mbSceneInfo);
	VuDataUtil::putValue(options["AnimationTimeLine"], mbAnimationTimeLine);
	VuDataUtil::putValue(options["Target"], mbTarget);
	VuDataUtil::putValue(options["LightVector"], mbLightVector);

	VuDataUtil::putValue(options["NearPlane"], mNearPlane);
	VuDataUtil::putValue(options["FarPlane"], mFarPlane);

	VuDataUtil::putValue(options["BackgroundColor"], mBackgroundColor);
	VuDataUtil::putValue(options["GridColor"], mGridColor);
	VuDataUtil::putValue(options["LineColor"], mLineColor);
	VuDataUtil::putValue(options["TextColor"], mTextColor);
	VuDataUtil::putValue(options["AmbientLightColor"], mAmbientLightColor);
	VuDataUtil::putValue(options["DirectionalLightFrontColor"], mDirectionalLightFrontColor);
	VuDataUtil::putValue(options["DirectionalLightBackColor"], mDirectionalLightBackColor);
	VuDataUtil::putValue(options["DirectionalLightSpecularColor"], mDirectionalLightSpecularColor);

	VuJsonWriter writer;
	if ( !writer.saveToFile(options, SESSION_SETTINGS_FILE) )
		VUWARNING("Unable to save session settings.");
}
