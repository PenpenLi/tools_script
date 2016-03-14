//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  RedGfxComposer implementation class
// 
//*****************************************************************************

#include "VuPurpleGfxComposer.h"
#include "VuPurpleGfxComposerCommands.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Managers/VuVideoShareManager.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarHeadlights.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Util/VuScreenShotUtil.h"
#include "VuEngine/VuEngine.h"
#include "VuEngine/Managers/VuDrawManager.h"
#include "VuEngine/Managers/VuGfxSettingsManager.h"
#include "VuEngine/Managers/VuFoliageManager.h"
#include "VuEngine/Managers/VuLensWaterManager.h"
#include "VuEngine/Managers/VuConfigManager.h"
#include "VuEngine/Managers/VuDialogManager.h"
#include "VuEngine/Managers/VuMessageBoxManager.h"
#include "VuEngine/Components/3dDraw/Vu3dDrawManager.h"
#include "VuEngine/Gfx/GfxSort/VuGfxSort.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/GfxComposer/VuGfxComposerCommands.h"
#include "VuEngine/Gfx/GfxComposer/VuPSSM.h"
#include "VuEngine/HAL/Gfx/VuGfx.h"
#include "VuEngine/HAL/Gfx/VuRenderTarget.h"
#include "VuEngine/HAL/Gfx/VuShaderProgram.h"
#include "VuEngine/HAL/Gfx/VuPipelineState.h"
#include "VuEngine/HAL/Gfx/VuTexture.h"
#include "VuEngine/Water/VuWater.h"
#include "VuEngine/Water/VuWaterTexture.h"
#include "VuEngine/Dynamics/VuDynamics.h"
#include "VuEngine/Math/VuMathUtil.h"
#include "VuEngine/Json/VuJsonContainer.h"
#include "VuEngine/Util/VuImageUtil.h"
#include "VuEngine/Dev/VuDevMenu.h"
#include "VuEngine/Dev/VuDevProfile.h"
#include "VuEngine/Dev/VuDevConfig.h"


// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuPurpleGfxComposer, VuPurpleGfxComposer);


struct ScreenShotMetrics
{
	int mWidth, mHeight, mRefWidth, mRefHeight, mCount;
};
static ScreenShotMetrics sScreenShotMetrics[] =
{
	{    0,    0,    0,   0,  0 }, // current size
	{ 1280,  720,  640, 360,  1 }, //   921600 pixels
	{ 1920, 1080,  960, 540,  2 }, //  2073600 pixels
	{ 2560, 1440, 1280, 720,  4 }, //  3686400 pixels
	{ 3840, 2160, 1280, 720,  8 }, //  8294400 pixels
	{ 5120, 2880, 1280, 720, 16 }, // 14745600 pixels
	{ 7680, 4320, 1280, 720, 32 }, // 33177600 pixels
};

// choices
static VuDevMenu::IntEnumChoice sScreenShotChoices[] =
{
	{ "Current", 0},
	{ "1280x720", 1},
	{ "1920x1080", 2},
	{ "2560x1440", 3},
	{ "3840x2160", 4},
	{ "5120x2880", 5},
	{ "7680x4320", 6},
	{ VUNULL }
};
VU_COMPILE_TIME_ASSERT(sizeof(sScreenShotChoices)/sizeof(sScreenShotChoices[0]) - 1 == sizeof(sScreenShotMetrics)/sizeof(sScreenShotMetrics[0]));

// material extension
class VuPurpleGfxComposer::VuMatExt : public VuGfxSortMatExt
{
public:
	VuMatExt():
		mhHeadlightPosition(VUNULL),
		mhHeadlightDirection(VUNULL),
		mhHeadlightDiffuseColor(VUNULL),
		mhHeadlightRange(VUNULL)
	{}

	VuPSSM::VuMatExt	mMatExtPSSM;

	VUHANDLE			mhHeadlightPosition;
	VUHANDLE			mhHeadlightDirection;
	VUHANDLE			mhHeadlightDiffuseColor;
	VUHANDLE			mhHeadlightRange;
	VUINT				miSampWaterNormal;
};


//*****************************************************************************
VuPurpleGfxComposer::VuPurpleGfxComposer():
	mDisplayScale(1.0f),
	mShadowDetail(1.0f),
	mPSSMEnabled(true),
	mRadialBlurEnabled(true),
	mColorCorrectionEnabled(true),
	mDisablePresent(false),
	mRejectionScale(0.02f),
	mbDrawCollision(false),
	mReflectionRenderTargetScale(0.25f),
	mReflectionRejectionScale(0.05f),
	mbShowReflectionMap(false),
	mReflectionMapOffset(0.5f, 0.5f),
	mReflectionMapScale(0.5f, 0.5f),
	mpPSSM(VUNULL),
	mbShowShadowMaps(false),
	mScreenShotSize(0),
	mViewportCount(0),
	mSimIndex(0),
	mGfxIndex(1),
	mBlurRenderTargetWidth(0),
	mBlurRenderTargetHeight(0),
	mpBlurRenderTarget0(VUNULL),
	mpBlurRenderTarget1(VUNULL)
{
	VuGfxComposer::IF()->setGameInterface(this);

	VuDevMenu::IF()->addFloat("GfxComposer/Rejection Scale", mRejectionScale, 0.01f, 0, 1);
	VuDevMenu::IF()->addFloat("GfxComposer/Display Scale", mDisplayScale, 0.01f, 0.5f, 1.0f);
	VuDevMenu::IF()->addFloat("GfxComposer/Reflection Render Target Scale", mReflectionRenderTargetScale, 0.01f, 0.1f, 0.5f);
	VuDevMenu::IF()->addFloat("GfxComposer/Reflection Rejection Scale", mReflectionRejectionScale, 0.01f, 0, 1);
	VuDevMenu::IF()->addBool("GfxComposer/Show Reflection Map", mbShowReflectionMap);
	VuDevMenu::IF()->addBool("GfxComposer/Show Shadow Maps", mbShowShadowMaps);
	VuDevMenu::IF()->addIntEnum("GfxComposer/Screen Shot Size", mScreenShotSize, sScreenShotChoices);
	VuDevMenu::IF()->addBool("GfxComposer/Draw Collision", mbDrawCollision);

	VuDevConfig::IF()->getParam("DrawCollision").getValue(mbDrawCollision);

	mShadowSplitDistances[0] = 3.0f;
	mShadowSplitDistances[1] = 15.0f;
	mShadowSplitDistances[2] = 75.0f;
}

//*****************************************************************************
VuPurpleGfxComposer::~VuPurpleGfxComposer()
{
	VuGfxComposer::IF()->setGameInterface(VUNULL);
}

//*****************************************************************************
bool VuPurpleGfxComposer::init()
{
	// create sub-systems
	mpPSSM = new VuPSSM();

	// start rendering
	VuDrawManager::IF()->registerHandler(this, &VuPurpleGfxComposer::draw);

	// add keyboard callback
	VuKeyboard::IF()->addCallback(this);

	// configuration
	mDisplayScale = VuConfigManager::IF()->getFloat("Composer/DisplayScale")->mValue;
	mShadowDetail = VuConfigManager::IF()->getFloat("Composer/ShadowDetail")->mValue;
	mDisablePresent = VuConfigManager::IF()->getBool("Composer/DisablePresent")->mValue;
	mRadialBlurEnabled = VuConfigManager::IF()->getBool("Effects/RadialBlur")->mValue;
	mColorCorrectionEnabled = VuConfigManager::IF()->getBool("Effects/ColorCorrection")->mValue;

	VuConfigManager::IF()->registerFloatHandler("Composer/DisplayScale", this, &VuPurpleGfxComposer::configDisplayScale);
	VuConfigManager::IF()->registerFloatHandler("Composer/ShadowDetail", this, &VuPurpleGfxComposer::configShadowDetail);
	VuConfigManager::IF()->registerBoolHandler("Composer/DisablePresent", this, &VuPurpleGfxComposer::configDisablePresent);
	VuConfigManager::IF()->registerBoolHandler("Effects/RadialBlur", this, &VuPurpleGfxComposer::configRadialBlur);
	VuConfigManager::IF()->registerBoolHandler("Effects/ColorCorrection", this, &VuPurpleGfxComposer::configColorCorrection);

	mPSSMEnabled = (VuGfxUtil::IF()->getShaderLOD() == 0);
	VuConfigManager::IF()->registerIntHandler("Gfx/ShaderLOD", this, &VuPurpleGfxComposer::configShaderLOD);

	return true;
}

//*****************************************************************************
void VuPurpleGfxComposer::release()
{
	// clean up
	VuConfigManager::IF()->unregisterFloatHandler("Composer/DisplayScale", this);
	VuConfigManager::IF()->unregisterFloatHandler("Composer/ShadowDetail", this);
	VuConfigManager::IF()->unregisterBoolHandler("Composer/DisablePresent", this);
	VuConfigManager::IF()->unregisterBoolHandler("Effects/RadialBlur", this);
	VuConfigManager::IF()->unregisterBoolHandler("Effects/ColorCorrection", this);
	VuConfigManager::IF()->unregisterIntHandler("Gfx/ShaderLOD", this);
	VuDrawManager::IF()->unregisterHandler(this);
	VuKeyboard::IF()->removeCallback(this);

	destroyRenderTargets();
	destroyBlurRenderTargets();

	delete mpPSSM;
	mpPSSM = VUNULL;
}

//*****************************************************************************
VuTexture *VuPurpleGfxComposer::getWaterReflectionTexture(int viewport)
{
	if ( mViewports[viewport].mpReflectionRenderTarget )
		return mViewports[viewport].mpReflectionRenderTarget->getColorTexture();

	return VUNULL;
}

//*****************************************************************************
VuTexture *VuPurpleGfxComposer::getDepthTexture()
{
	return VUNULL;
}

//*****************************************************************************
VuGfxSortMatExt *VuPurpleGfxComposer::createMaterialExt(VuGfxSortMaterial *pMat)
{
	VuMatExt *pMatExt = new VuMatExt;

	VuPSSM::resolveConstants(pMat->mpShaderProgram, &pMatExt->mMatExtPSSM);

	if ( pMat->mpShaderProgram->getConstantByName("gHeadlightPosition") )
	{
		pMatExt->mhHeadlightPosition = pMat->mpShaderProgram->getConstantByName("gHeadlightPosition");
		pMatExt->mhHeadlightDirection = pMat->mpShaderProgram->getConstantByName("gHeadlightDirection");
		pMatExt->mhHeadlightDiffuseColor = pMat->mpShaderProgram->getConstantByName("gHeadlightDiffuseColor");
		pMatExt->mhHeadlightRange = pMat->mpShaderProgram->getConstantByName("gHeadlightRange");
	}

	pMatExt->miSampWaterNormal = pMat->mpShaderProgram->getSamplerIndexByName("gWaterNormalTexture");

	return pMatExt;
}

//*****************************************************************************
void VuPurpleGfxComposer::setGlobalConstants(const VUHANDLE hMatExt, VuShaderProgram *pSP)
{
	VuMatExt *pMatExt = static_cast<VuMatExt *>(hMatExt);

	if ( mPSSMEnabled )
	{
		mpPSSM->setConstants(pSP, &pMatExt->mMatExtPSSM);
	}

	if ( pMatExt->mhHeadlightPosition )
	{
		int viewportIndex = VuGfxSort::IF()->getRenderViewport();
		int reflectionIndex = VuGfxSort::IF()->getRenderReflectionLayer() - VuGfxSort::REFLECTION_ON;
		const VuHeadlight &headlight = mHeadlights[mGfxIndex][viewportIndex][reflectionIndex];

		pSP->setConstantVector3(pMatExt->mhHeadlightPosition, headlight.mPosition);
		pSP->setConstantVector3(pMatExt->mhHeadlightDirection, headlight.mDirection);
		pSP->setConstantVector3(pMatExt->mhHeadlightDiffuseColor, headlight.mDiffuseColor);
		pSP->setConstantVector4(pMatExt->mhHeadlightRange, headlight.mRange);
	}

	if ( pMatExt->miSampWaterNormal >= 0 )
		VuGfx::IF()->setTexture(pMatExt->miSampWaterNormal, VuWater::IF()->renderer()->getWaterTexture()->getActiveTexture());
}

//*****************************************************************************
void VuPurpleGfxComposer::gatherInfo(const VuGfxDrawParams &params)
{
	int viewportIndex = VuGfxSort::IF()->getViewport();
	int reflectionIndex = VuGfxSort::IF()->getReflectionLayer() - VuGfxSort::REFLECTION_ON;
	VUASSERT(reflectionIndex >= 0 && reflectionIndex < 2, "VuPurpleGfxComposer::gatherInfo() bad reflection index");
	VuHeadlight &headlight = mHeadlights[mSimIndex][viewportIndex][reflectionIndex];

	if ( VuCarEntity *pCar = VuCarManager::IF()->getCameraTargetForViewport(viewportIndex) )
	{
		VuCarHeadlights *pCarHeadlights = pCar->getHeadlights();
		const VuMatrix &modelMat = pCar->getModelMatrix();
		const VuMatrix &lightTransform = pCarHeadlights->getLightTransform();

		float minAngle = 0.5f*pCarHeadlights->getConeAngle();
		float maxAngle = 0.5f*pCarHeadlights->getConeAngle() + pCarHeadlights->getPenumbraAngle();

		headlight.mPosition = modelMat.transform(lightTransform.getTrans());
		headlight.mDirection = modelMat.transformNormal(lightTransform.getAxisY());
		headlight.mDiffuseColor = pCarHeadlights->getLightDiffuseColor();
		headlight.mRange.mX = pCarHeadlights->getFalloffRangeMin();
		headlight.mRange.mY = pCarHeadlights->getFalloffRangeMax() + FLT_EPSILON;
		headlight.mRange.mZ = VuCos(minAngle) + FLT_EPSILON;
		headlight.mRange.mW = VuCos(maxAngle);
	}
	else
	{
		memset(&headlight, 0, sizeof(headlight));
	}
}

//*****************************************************************************
void VuPurpleGfxComposer::synchronizeGfx()
{
	mSimIndex ^= 1;
	mGfxIndex ^= 1;
}

//*****************************************************************************
void VuPurpleGfxComposer::onKeyDown(VUUINT32 key)
{
	if ( key == VUKEY_T )
	{
		takeScreenShot();
	}
}

//*****************************************************************************
void VuPurpleGfxComposer::draw()
{
	VuGfxSettings gfxSettings;

	// update resources if viewports have changed
	updateRenderTargets();
	updateBlurRenderTargets();

	// blur enabled?
	bool blurEnabled = false;
	VuRenderTarget *pUiRenderTarget = VUNULL;
	if ( VuMessageBoxManager::IF()->getActiveMessageBox() || VuDialogManager::IF()->getActiveDialog() || VuGameUtil::IF()->getPauseMenuShown() )
	{
		blurEnabled = true;
		pUiRenderTarget = mpBlurRenderTarget0;
	}

	VuGfxSort::IF()->setScreen(0);
	VuGfxSort::IF()->setFullScreenLayer(0);
	VuGfxSort::IF()->setViewport(0);
	VuGfxSort::IF()->setReflectionLayer(0);
	VuGfxSort::IF()->setViewportLayer(0);

	VuGfxComposerSceneCommands::submitBeginEndScene(VUNULL);
	if ( VuViewportManager::IF()->getViewportCount() == 0 )
	{
		VuSetRenderTargetParams params(VUNULL);
		params.mColorLoadAction = VuSetRenderTargetParams::LoadActionClear;
		params.mDepthLoadAction = VuSetRenderTargetParams::LoadActionClear;
		VuGfxUtil::IF()->submitSetRenderTargetCommand(params, 1);
	}

	VuGfxComposerPostProcessCommands::CopyMultiParams copyMultiParams;

	for ( int iViewport = 0; iViewport < VuViewportManager::IF()->getViewportCount(); iViewport++ )
	{
		VuGfxSort::IF()->setFullScreenLayer(VuGfxSort::FSL_GAME);
		VuGfxSort::IF()->setViewport(iViewport);
		VuGfxSort::IF()->setReflectionLayer(0);
		VuGfxSort::IF()->setViewportLayer(0);

		const VuViewportManager::VuViewport &viewport = VuViewportManager::IF()->getViewport(iViewport);
		const VuCamera &camera = viewport.mCamera;

		VUUINT32 zoneMask = Vu3dDrawManager::IF()->calcZoneMask(camera.getEyePosition());

		// handle gfx settings
		VuGfxSettingsManager::IF()->getSettings(camera.getEyePosition(), gfxSettings);
		VuGfxSort::IF()->submitGfxSettings(gfxSettings);

		// determine effects
		bool bRadialBlur = mRadialBlurEnabled && viewport.mRadialBlurAmount > 0.0f;
		bool bLensWater = VuLensWaterManager::IF()->isEnabled() && VuLensWaterManager::IF()->isActive(iViewport);
		bool bColorCorrection = mColorCorrectionEnabled && !gfxSettings.hasDefaultColorCorrection();
		bool bEffectsActive = bRadialBlur || bLensWater|| bColorCorrection || mViewports[iViewport].mDisplayIsScaled;

		// compose reflection map
		submitReflectionCommands(mViewports[iViewport].mpReflectionRenderTarget, camera, gfxSettings, viewport.mCurWaterSurfaceZ, zoneMask);

		// submit shadow commands
		submitShadowCommands(camera, zoneMask);

		// submit scene commands
		submitSceneCommands(bEffectsActive ? mViewports[iViewport].mpRenderTargets[0] : pUiRenderTarget, camera, gfxSettings, zoneMask);

		drawReflectionMap(getWaterReflectionTexture(iViewport));
		drawShadowMaps();

		// submit framebuffer effects commands
		VuGfxSort::IF()->setFullScreenLayer(VuGfxSort::FSL_EFFECTS);
		VuGfxSort::IF()->setReflectionLayer(0);
		VuGfxSort::IF()->setViewportLayer(0);

		// post process
		if ( mViewports[iViewport].mpRenderTargets[0] )
		{
			VuRenderTarget *pCurRenderTarget = mViewports[iViewport]. mpRenderTargets[0];
			VuRenderTarget *pNextRenderTarget =  mViewports[iViewport].mpRenderTargets[1];

			// radial blur
			if ( bRadialBlur )
			{
				if ( !bLensWater && !bColorCorrection && !mViewports[iViewport].mDisplayIsScaled )
					pNextRenderTarget = pUiRenderTarget;

				VuGfxSort::IF()->setViewportLayer(0);
				VuGfxComposerPostProcessCommands::radialBlur(pCurRenderTarget->getColorTexture(), pNextRenderTarget, viewport.mRadialBlurAmount);
				VuSwap(pCurRenderTarget, pNextRenderTarget);
			}

			// lens water
			if ( bLensWater )
			{
				if ( !bColorCorrection && !mViewports[iViewport].mDisplayIsScaled )
					pNextRenderTarget = pUiRenderTarget;

				VuGfxSort::IF()->setViewportLayer(1);
				VuLensWaterManager::IF()->submit(iViewport, pCurRenderTarget->getColorTexture(), pNextRenderTarget);
				VuSwap(pCurRenderTarget, pNextRenderTarget);
			}

			// color correction
			if ( bColorCorrection )
			{
				if ( !mViewports[iViewport].mDisplayIsScaled )
					pNextRenderTarget = pUiRenderTarget;

				VuGfxSort::IF()->setViewportLayer(2);
				VuGfxComposerPostProcessCommands::colorCorrection(pCurRenderTarget->getColorTexture(), pNextRenderTarget, gfxSettings.mContrast, gfxSettings.mTint, gfxSettings.mGammaMin, gfxSettings.mGammaMax, gfxSettings.mGammaCurve);
				VuSwap(pCurRenderTarget, pNextRenderTarget);
			}

			// stretch-copy
			if ( mViewports[iViewport].mDisplayIsScaled && !mDisablePresent )
				copyMultiParams.add(pCurRenderTarget->getColorTexture(), viewport.mRect);
		}
	}

	// stretch-copy
	if ( copyMultiParams.mCount )
	{
		VuGfxSort::IF()->setViewportLayer(VuGfxSort::VPL_END);
		VuGfxComposerPostProcessCommands::copyMulti(pUiRenderTarget, copyMultiParams);
	}

	VuGfxSort::IF()->setViewport(0);
	VuGfxSort::IF()->setReflectionLayer(0);
	VuGfxSort::IF()->setViewportLayer(0);
	VuGfxSort::IF()->setFullScreenLayer(VuGfxSort::FSL_HUD);

	// clear depth buffer for ui (for ui geometry pfx)
	VuGfxUtil::IF()->submitClearCommand(VUGFX_CLEAR_DEPTH, VuColor(0,0,0), 1.0f);

	// handle safe zone
	VuGfxUtil::IF()->submitSetViewportCommand(VuViewportManager::IF()->getSafeZoneRect(), 1);

	// submit ui camera
	VuGfxSort::IF()->submitCamera(VuViewportManager::IF()->getUiCamera(), 2);

	// submit blur command
	if ( blurEnabled )
	{
		if ( VuGameUtil::IF()->getPauseMenuShown() )
		{
			VuGfxSort::IF()->setFullScreenLayer(VuGfxSort::FSL_UI);
			VuGfxComposerPostProcessCommands::blur(mpBlurRenderTarget0, mpBlurRenderTarget1, VuGameUtil::IF()->getPauseMenuFadeValue(), 0);
			VuGfxUtil::IF()->submitClearCommand(VUGFX_CLEAR_DEPTH, VuColor(0,0,0), 1.0f, 1);
			VuGfxUtil::IF()->submitSetViewportCommand(VuViewportManager::IF()->getSafeZoneRect(), 2);
		}
		if ( VuDialogManager::IF()->getActiveDialog() )
		{
			VuGfxSort::IF()->setFullScreenLayer(VuGfxSort::FSL_DIALOG);
			VuGfxComposerPostProcessCommands::blur(mpBlurRenderTarget0, mpBlurRenderTarget1, VuDialogManager::IF()->getFadeValue(), 0);
			VuGfxUtil::IF()->submitClearCommand(VUGFX_CLEAR_DEPTH, VuColor(0,0,0), 1.0f, 1);
			VuGfxUtil::IF()->submitSetViewportCommand(VuViewportManager::IF()->getSafeZoneRect(), 2);
		}
		if ( VuMessageBoxManager::IF()->getActiveMessageBox() )
		{
			VuGfxSort::IF()->setFullScreenLayer(VuGfxSort::FSL_MESSAGE_BOX);
			VuGfxComposerPostProcessCommands::blur(mpBlurRenderTarget0, mpBlurRenderTarget1, VuMessageBoxManager::IF()->getFadeValue(), 0);
			VuGfxUtil::IF()->submitClearCommand(VUGFX_CLEAR_DEPTH, VuColor(0,0,0), 1.0f, 1);
			VuGfxUtil::IF()->submitSetViewportCommand(VuViewportManager::IF()->getSafeZoneRect(), 2);
		}
		VuGfxComposerPostProcessCommands::copy(mpBlurRenderTarget0->getColorTexture(), VUNULL, 1);
	}

	// submit video share begin/end
	if ( VuVideoShareManager::IF()->isSupported() )
	{
		VuGfxSort::IF()->setFullScreenLayer(0);
		VuPurpleGfxComposerCommands::submitVideoShareBegin();

		VuGfxSort::IF()->setFullScreenLayer(VuGfxSort::FSL_END);
		VuPurpleGfxComposerCommands::submitVideoShareEnd();
	}
}

//*****************************************************************************
void VuPurpleGfxComposer::submitSceneCommands(VuRenderTarget *pRenderTarget, const VuCamera &camera, const VuGfxSettings &gfxSettings, VUUINT32 zoneMask)
{
	VU_PROFILE_SIM("Scene");

	VuGfxSort::IF()->setReflectionLayer(VuGfxSort::REFLECTION_OFF);

	// submit commands
	VuGfxComposerSceneCommands::submitClear(pRenderTarget);

	// draw scene
	VuGfxSort::IF()->setViewportLayer(VuGfxSort::VPL_WORLD);
	if ( mbDrawCollision )
	{
		VuDynamics::IF()->drawCollision(camera);
	}
	else
	{
		VuGfxDrawParams params(camera);
		params.mRejectionScale = mRejectionScale;
		params.mbDrawReflection = false;
		params.mZoneMask = zoneMask;
		Vu3dDrawManager::IF()->draw(params);
		VuFoliageManager::IF()->draw();
	}

	// submit camera
	VuGfxSort::IF()->submitCamera(camera);
}

//*****************************************************************************
void VuPurpleGfxComposer::submitReflectionCommands(VuRenderTarget *pReflectionRenderTarget, const VuCamera &camera, const VuGfxSettings &gfxSettings, float waterSurfaceZ, VUUINT32 zoneMask)
{
	if ( VuWater::IF()->getProdecuralReflectionsEnabled() )
	{
		VU_PROFILE_SIM("Reflection");

		VuGfxSort::IF()->setReflectionLayer(VuGfxSort::REFLECTION_ON);

		// calculate water reflection plane
		VuVector4 reflectionPlane = VuMathUtil::planeFromNormalPoint(VuVector3(0,0,1), VuVector3(0,0,waterSurfaceZ));

		// create reflected camera
		VuCamera reflectionCamera = camera;
		{
			VuVector3 vEye = camera.getEyePosition();
			VuVector3 vTarget = camera.getTargetPosition();
			VuVector3 vUp = camera.getTransform().getAxisZ();

			vEye.mZ = waterSurfaceZ - (vEye.mZ - waterSurfaceZ);
			vTarget.mZ = waterSurfaceZ - (vTarget.mZ - waterSurfaceZ);
			vUp = VuVector3(-vUp.mX, -vUp.mY, vUp.mZ);

			reflectionCamera.setViewMatrix(vEye, vTarget, vUp);
		}

		// transform reflection plane into view space
		VuMatrix mat = reflectionCamera.getViewProjMatrix();
		mat.invert();
		mat.transpose();
		VuVector4 plane = mat.transform(reflectionPlane);

		// submit clip plane commands
		VuGfxComposerSceneCommands::submitReflectionClip(plane);

		// submit scene commands
		VuGfxComposerSceneCommands::submitClear(pReflectionRenderTarget);

		// draw scene
		VuGfxSort::IF()->setViewportLayer(VuGfxSort::VPL_WORLD);
		if ( mbDrawCollision )
		{
			VuDynamics::IF()->drawCollision(camera);
		}
		else
		{
			VuGfxDrawParams params(reflectionCamera);
			params.mRejectionScale = mReflectionRejectionScale;
			params.mbDrawReflection = true;
			params.mReflectionPlane = reflectionPlane;
			params.mZoneMask = zoneMask;
			Vu3dDrawManager::IF()->draw(params);
			VuFoliageManager::IF()->draw();
		}

		// submit resolve command
		VuGfxSort::IF()->setViewportLayer(VuGfxSort::VPL_END);

		// submit camera
		VuGfxSort::IF()->submitCamera(reflectionCamera);

		VuGfxSort::IF()->setReflectionLayer(VuGfxSort::REFLECTION_OFF);
	}
}

//*****************************************************************************
void VuPurpleGfxComposer::submitShadowCommands(const VuCamera &camera, VUUINT32 zoneMask)
{
	if ( mPSSMEnabled )
	{
		VU_PROFILE_SIM("Shadows");

		mpPSSM->setRejectionScale(mRejectionScale);
		mpPSSM->submitCommands(camera, zoneMask, false);
	}
}

//*****************************************************************************
void VuPurpleGfxComposer::drawReflectionMap(VuTexture *pReflectionTexture)
{
	if ( mbShowReflectionMap )
	{
		VuGfxSort::IF()->setViewportLayer(VuGfxSort::VPL_UI);
		
		VuRect rect = VuViewportManager::IF()->getViewport(0).mRect;
		
		float fAspect = rect.mWidth/rect.mHeight;
		
		float fSizeX = 0.2f;
		float fSizeY = fSizeX*fAspect;
		
		float fStartX = 1 - fSizeX;
		float fStartY = 1 - fSizeY;
		
		VuGfxUtil::IF()->drawTexture2d(0, pReflectionTexture, VuRect(fStartX, fStartY, fSizeX, fSizeY), VuRect(0,0,1,1), VuGfxSort::TRANS_UI_OPAQUE);
	}
	else if ( pReflectionTexture )
	{
		// crazy iOS issue: if render target is not used, it is 'lost'
		VuGfxSort::IF()->setViewportLayer(VuGfxSort::VPL_UI);
		VuGfxUtil::IF()->drawTexture2d(0, pReflectionTexture, VuRect(0,0,0,0));
	}
}

//*****************************************************************************
void VuPurpleGfxComposer::drawShadowMaps()
{
	if ( mbShowShadowMaps && mPSSMEnabled && mpPSSM )
	{
		VuGfxSort::IF()->setViewportLayer(VuGfxSort::VPL_UI);

		VuRect rect = VuViewportManager::IF()->getViewport(0).mRect;

		float fAspect = rect.mWidth/rect.mHeight;

		float fSizeX = 0.2f;
		float fSizeY = fSizeX*fAspect;

		for ( int i = 0; i < mpPSSM->getTextureCount(); i++ )
		{
			float fOffsetX = 1.1f*fSizeX*i;
			float fStartX = fOffsetX;
			float fStartY = 1 - fSizeY;

			VuGfxUtil::IF()->drawTexture2d(0, mpPSSM->getTexture(i), VuRect(fStartX, fStartY, fSizeX, fSizeY), VuRect(0,0,1,1), VuGfxSort::TRANS_UI_OPAQUE);
		}
	}
}

//*****************************************************************************
void VuPurpleGfxComposer::updateRenderTargets()
{
	int displayWidth, displayHeight;
	VuGfx::IF()->getDisplaySize(VUNULL, displayWidth, displayHeight);

	// determine if update is needed
	bool bUpdate = false;
	if ( VuViewportManager::IF()->getViewportCount() != mViewportCount )
		bUpdate = true;

	for ( int iViewport = 0; iViewport < VuViewportManager::IF()->getViewportCount() && !bUpdate; iViewport++ )
	{
		const VuViewportManager::VuViewport &viewport = VuViewportManager::IF()->getViewport(iViewport);

		int newWidth = VuRound(displayWidth*mDisplayScale*viewport.mRect.mWidth);
		int newHeight = VuRound(displayHeight*mDisplayScale*viewport.mRect.mHeight);

		// calculate current and new render target counts
		int curRenderTargetCount = 0;
		if ( mViewports[iViewport].mpRenderTargets[0] ) curRenderTargetCount++;
		if ( mViewports[iViewport].mpRenderTargets[1] ) curRenderTargetCount++;

		int newRenderTargetCount = 0;
		if ( newWidth != displayWidth || newHeight != displayHeight )
			newRenderTargetCount++;
		if ( mRadialBlurEnabled )
			newRenderTargetCount++;
		if ( VuLensWaterManager::IF()->isEnabled() )
			newRenderTargetCount++;
		if ( mColorCorrectionEnabled )
			newRenderTargetCount++;
		newRenderTargetCount = VuMin(newRenderTargetCount, 2);

		// reflections enabled?
		bool curReflectionsEnabled = mViewports[iViewport].mpReflectionRenderTarget ? true : false;

		// check if update is needed
		if ( mViewports[iViewport].mCurRenderTargetWidth != newWidth || mViewports[iViewport].mCurRenderTargetHeight != newHeight )
			bUpdate = true;
		if ( curRenderTargetCount != newRenderTargetCount )
			bUpdate = true;
		if ( curReflectionsEnabled != VuWater::IF()->getProdecuralReflectionsEnabled() )
			bUpdate = true;
	}

	mViewportCount = VuViewportManager::IF()->getViewportCount();

	if ( bUpdate )
	{
		destroyRenderTargets();

		for ( int iViewport = 0; iViewport < mViewportCount; iViewport++ )
		{
			const VuViewportManager::VuViewport &viewport = VuViewportManager::IF()->getViewport(iViewport);

			int newWidth = VuRound(displayWidth*mDisplayScale*viewport.mRect.mWidth);
			int newHeight = VuRound(displayHeight*mDisplayScale*viewport.mRect.mHeight);

			int newRenderTargetCount = 0;
			if ( newWidth != displayWidth || newHeight != displayHeight )
				newRenderTargetCount++;
			if ( mRadialBlurEnabled )
				newRenderTargetCount++;
			if ( VuLensWaterManager::IF()->isEnabled() )
				newRenderTargetCount++;
			if ( mColorCorrectionEnabled )
				newRenderTargetCount++;
			newRenderTargetCount = VuMin(newRenderTargetCount, 2);

			if ( newRenderTargetCount >= 1 )
				mViewports[iViewport].mpRenderTargets[0] = VuGfx::IF()->createRenderTarget(newWidth, newHeight);
			if ( newRenderTargetCount >= 2 )
				mViewports[iViewport].mpRenderTargets[1] = VuGfx::IF()->createRenderTarget(newWidth, newHeight);

			if ( VuWater::IF()->getProdecuralReflectionsEnabled() )
			{
				int refWidth = VuRound(mReflectionRenderTargetScale*newWidth);
				int refHeight = VuRound(mReflectionRenderTargetScale*newHeight);
				mViewports[iViewport].mpReflectionRenderTarget = VuGfx::IF()->createRenderTarget(refWidth, refHeight);
			}

			mViewports[iViewport].mCurRenderTargetWidth = newWidth;
			mViewports[iViewport].mCurRenderTargetHeight = newHeight;

			mViewports[iViewport].mDisplayIsScaled = ( newWidth != displayWidth || newHeight != displayHeight );
		}
	}

	// update lens water
	VuLensWaterManager::IF()->setViewportCount(mViewportCount);
	for ( int i = 0; i < mViewportCount; i++ )
		VuLensWaterManager::IF()->updateTextureSize(i, mViewports[i].mCurRenderTargetWidth >> 1, mViewports[i].mCurRenderTargetHeight >> 1);

	// update PSSM
	int pssmSize = VuRound(mShadowDetail*mDisplayScale*VuSqrt((float)(displayWidth*displayHeight)));
	pssmSize = (pssmSize/32)*32; // round to nearest factor of 32
	mpPSSM->setCount(4);
	mpPSSM->setSize(pssmSize);
	mpPSSM->setSplitPositions(mShadowSplitDistances[0], mShadowSplitDistances[1], mShadowSplitDistances[2]);
}

//*****************************************************************************
void VuPurpleGfxComposer::destroyRenderTargets()
{
	VuGfxSort::IF()->flush();

	for ( int iViewport = 0; iViewport < VuViewportManager::MAX_VIEWPORTS; iViewport++ )
	{
		for ( int i = 0; i < 2; i++ )
		{
			if ( mViewports[iViewport].mpRenderTargets[i] )
			{
				mViewports[iViewport].mpRenderTargets[i]->removeRef();
				mViewports[iViewport].mpRenderTargets[i] = VUNULL;
			}
		}

		if ( mViewports[iViewport].mpReflectionRenderTarget )
		{
			mViewports[iViewport].mpReflectionRenderTarget->removeRef();
			mViewports[iViewport].mpReflectionRenderTarget = VUNULL;
		}

		mViewports[iViewport].mCurRenderTargetWidth = 0;
		mViewports[iViewport].mCurRenderTargetHeight = 0;
		mViewports[iViewport].mDisplayIsScaled = false;
	}
}

//*****************************************************************************
void VuPurpleGfxComposer::updateBlurRenderTargets()
{
	int displayWidth, displayHeight;
	VuGfx::IF()->getDisplaySize(VUNULL, displayWidth, displayHeight);

	int blurRenderTargetWidth = displayWidth/4;
	int blurRenderTargetHeight = displayHeight/4;
	if ( blurRenderTargetWidth != mBlurRenderTargetWidth || blurRenderTargetHeight != mBlurRenderTargetHeight )
	{
		destroyBlurRenderTargets();

		mpBlurRenderTarget0 = VuGfx::IF()->createRenderTarget(blurRenderTargetWidth, blurRenderTargetHeight);
		mpBlurRenderTarget1 = VuGfx::IF()->createRenderTarget(blurRenderTargetWidth, blurRenderTargetHeight);

		mBlurRenderTargetWidth = blurRenderTargetWidth;
		mBlurRenderTargetHeight = blurRenderTargetHeight;
	}
}

//*****************************************************************************
void VuPurpleGfxComposer::destroyBlurRenderTargets()
{
	VuGfxSort::IF()->flush();

	if ( mpBlurRenderTarget0 ) mpBlurRenderTarget0->removeRef();
	if ( mpBlurRenderTarget1 ) mpBlurRenderTarget1->removeRef();
	mpBlurRenderTarget0 = VUNULL;
	mpBlurRenderTarget1 = VUNULL;
}

//*****************************************************************************
void VuPurpleGfxComposer::takeScreenShot()
{
	VuGfxSort::IF()->flush();

	// determine screen shot metrics
	ScreenShotMetrics metrics;
	int displayWidth, displayHeight;
	VuGfx::IF()->getDisplaySize(VUNULL, displayWidth, displayHeight);
	metrics.mWidth = displayWidth;
	metrics.mHeight = displayHeight;
	metrics.mRefWidth = VuRound(mReflectionRenderTargetScale*metrics.mWidth);
	metrics.mRefHeight = VuRound(mReflectionRenderTargetScale*metrics.mHeight);
	metrics.mCount = 1;

	if ( mScreenShotSize > 0 )
	{
		metrics = sScreenShotMetrics[mScreenShotSize];
	}

	// create new render target
	destroyRenderTargets();
	VuRenderTarget *pRenderTargets[2];
	pRenderTargets[0] = VuGfx::IF()->createRenderTarget(metrics.mWidth, metrics.mHeight/metrics.mCount);
	pRenderTargets[1] = VuGfx::IF()->createRenderTarget(metrics.mWidth, metrics.mHeight/metrics.mCount);
	mViewports[0].mpReflectionRenderTarget = VuGfx::IF()->createRenderTarget(metrics.mRefWidth, metrics.mRefHeight);

	// start screenshot
	VuScreenShotWriter screenShotWriter(metrics.mWidth, metrics.mHeight);

	for ( int iy = 0; iy < metrics.mCount; iy++ )
	{
		VuWater::IF()->renderer()->kick();

		VuGfxSort::IF()->setFullScreenLayer(VuGfxSort::FSL_GAME);
		VuGfxSort::IF()->setViewport(0);

		const VuViewportManager::VuViewport &viewport = VuViewportManager::IF()->getViewport(0);

		VuCamera camera = viewport.mCamera;
		camera.setProjMatrixVert(camera.getFovVert(), (float)metrics.mWidth/metrics.mHeight, camera.getNearPlane(), camera.getFarPlane());
		VuCamera screenShotCamera = camera;
		screenShotCamera.screenShotShear(0, iy, 1, metrics.mCount);

		VUUINT32 zoneMask = Vu3dDrawManager::IF()->calcZoneMask(camera.getEyePosition());

		mReflectionMapOffset = VuVector2(0.5f, (0.5f + metrics.mCount - iy - 1)/metrics.mCount);
		mReflectionMapScale = VuVector2(0.5f, 0.5f/metrics.mCount);

		// handle gfx settings
		VuGfxSettings gfxSettings;
		VuGfxSettingsManager::IF()->getSettings(camera.getEyePosition(), gfxSettings);
		VuGfxSort::IF()->submitGfxSettings(gfxSettings);

		// compose reflection map
		submitReflectionCommands(mViewports[0].mpReflectionRenderTarget, camera, gfxSettings, viewport.mCurWaterSurfaceZ, zoneMask);

		// submit shadow commands
		submitShadowCommands(screenShotCamera, zoneMask);

		// submit scene commands
		submitSceneCommands(pRenderTargets[0], screenShotCamera, gfxSettings, zoneMask);

		// submit framebuffer effects commands
		VuGfxSort::IF()->setFullScreenLayer(VuGfxSort::FSL_EFFECTS);
		VuGfxSort::IF()->setReflectionLayer(0);
		VuGfxSort::IF()->setViewportLayer(0);

		VuRenderTarget *pCurRenderTarget = pRenderTargets[0];
		VuRenderTarget *pNextRenderTarget =  pRenderTargets[1];

		// color correction
		bool bColorCorrection = mColorCorrectionEnabled && !gfxSettings.hasDefaultColorCorrection();
		if ( bColorCorrection )
		{
			VuGfxSort::IF()->setFullScreenLayer(VuGfxSort::FSL_EFFECTS);
			VuGfxSort::IF()->setViewportLayer(2);
			VuGfxComposerPostProcessCommands::colorCorrection(pCurRenderTarget->getColorTexture(), pNextRenderTarget, gfxSettings.mContrast, gfxSettings.mTint, gfxSettings.mGammaMin, gfxSettings.mGammaMax, gfxSettings.mGammaCurve);
			VuSwap(pCurRenderTarget, pNextRenderTarget);
		}

		// copy to framebuffer
		VuGfxSort::IF()->setFullScreenLayer(VuGfxSort::FSL_END);
		VuGfxComposerPostProcessCommands::copy(pCurRenderTarget->getColorTexture(), VUNULL);

		VuGfxSort::IF()->draw();
		VuGfxSort::IF()->flush();

		VuArray<VUBYTE> rgb(0);
		pCurRenderTarget->readPixels(rgb);
		if ( rgb.size() )
		{
			VuImageUtil::swapRB(&rgb[0], rgb.size()/3, 3);
			screenShotWriter.write(&rgb[0], rgb.size());
		}
	}

	// cleanup
	pRenderTargets[0]->removeRef();
	pRenderTargets[1]->removeRef();
	destroyRenderTargets();
	updateRenderTargets();

	mReflectionMapOffset = VuVector2(0.5f, 0.5f);
	mReflectionMapScale = VuVector2(0.5f, 0.5f);
}

//*****************************************************************************
void VuPurpleGfxComposer::configShaderLOD(int value)
{
	mPSSMEnabled = (value == 0);
}

//*****************************************************************************
VuPurpleGfxComposer::Viewport::Viewport():
	mDisplayIsScaled(false),
	mCurRenderTargetWidth(0),
	mCurRenderTargetHeight(0),
	mpReflectionRenderTarget(VUNULL)
{
	memset(&mpRenderTargets, 0, sizeof(mpRenderTargets));
}