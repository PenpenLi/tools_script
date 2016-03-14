//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  RedGfxComposer implementation class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Gfx/GfxComposer/VuGfxComposer.h"
#include "VuEngine/HAL/Keyboard/VuKeyboard.h"
#include "VuEngine/Managers/VuViewportManager.h"

class VuCamera;
class VuRenderTarget;
class VuPSSM;
class VuGfxSettings;


class VuPurpleGfxComposer : public VuSystemComponent, VuGfxComposer::GameInterface, VuKeyboard::Callback
{
	DECLARE_SYSTEM_COMPONENT(VuPurpleGfxComposer)

protected:
	// called by game
	friend class VuPurple;
	virtual bool	init();
	virtual void	release();

public:
	VuPurpleGfxComposer();
	~VuPurpleGfxComposer();

	// VuGfxComposer::GameInterface
	virtual VuTexture		*getWaterReflectionTexture(int viewport);
	virtual VuTexture		*getDepthTexture();
	virtual VuVector2		getWaterReflectionMapOffset()	{ return mReflectionMapOffset; }
	virtual VuVector2		getWaterReflectionMapScale()	{ return mReflectionMapScale; }
	virtual VuGfxSortMatExt	*createMaterialExt(VuGfxSortMaterial *pMat);
	virtual void			setGlobalConstants(const VUHANDLE hMatExt, VuShaderProgram *pSP);
	virtual void			gatherInfo(const VuGfxDrawParams &params);
	virtual void			synchronizeGfx();

	// game-specific
	float				getDisplayScale()			{ return mDisplayScale; }
	bool				isRadialBlurEnabled()		{ return mRadialBlurEnabled; }
	bool				isColorCorrectionEnabled()	{ return mColorCorrectionEnabled; }
	bool				getDisablePresent()			{ return mDisablePresent; }

	void				setShadowSplitDistances(float split1, float split2, float split3) { mShadowSplitDistances[0] = split1; mShadowSplitDistances[1] = split2; mShadowSplitDistances[2] = split3; }

protected:
	class VuMatExt;

	// VuKeyboard::Callback
	virtual void		onKeyDown(VUUINT32 key);

	void				draw();
	void				submitSceneCommands(VuRenderTarget *pRenderTarget, const VuCamera &camera, const VuGfxSettings &gfxSettings, VUUINT32 zoneMask);
	void				submitReflectionCommands(VuRenderTarget *pReflectionRenderTarget, const VuCamera &camera, const VuGfxSettings &gfxSettings, float waterSurfaceZ, VUUINT32 zoneMask);
	void				submitShadowCommands(const VuCamera &camera, VUUINT32 zoneMask);

	void				drawReflectionMap(VuTexture *pReflectionTexture);
	void				drawShadowMaps();

	void				updateRenderTargets();
	void				destroyRenderTargets();

	void				updateBlurRenderTargets();
	void				destroyBlurRenderTargets();

	void				takeScreenShot();

	void				configDisplayScale(float value)   { mDisplayScale = value; }
	void				configShadowDetail(float value)   { mShadowDetail = value; }
	void				configDisablePresent(bool value)  { mDisablePresent = value; }
	void				configRadialBlur(bool value)      { mRadialBlurEnabled = value; }
	void				configColorCorrection(bool value) { mColorCorrectionEnabled = value; }
	void				configShaderLOD(int value);

	// tweaks
	float				mDisplayScale;
	float				mShadowDetail;
	bool				mPSSMEnabled;
	bool				mDisablePresent;
	bool				mRadialBlurEnabled;
	bool				mColorCorrectionEnabled;
	float				mRejectionScale;
	bool				mbDrawCollision;

	// reflection
	float				mReflectionRenderTargetScale;
	float				mReflectionRejectionScale;
	bool				mbShowReflectionMap;
	VuVector2			mReflectionMapOffset;
	VuVector2			mReflectionMapScale;

	VuPSSM				*mpPSSM;
	bool				mbShowShadowMaps;
	float				mShadowSplitDistances[3];

	int					mScreenShotSize;

	struct Viewport
	{
		Viewport();
		bool				mDisplayIsScaled;
		int					mCurRenderTargetWidth;
		int					mCurRenderTargetHeight;
		VuRenderTarget		*mpRenderTargets[2];
		VuRenderTarget		*mpReflectionRenderTarget;
	};
	Viewport			mViewports[VuViewportManager::MAX_VIEWPORTS];
	int					mViewportCount;

	struct VuHeadlight
	{
		VuVector3	mPosition;
		VuVector3	mDirection;
		VuVector3	mDiffuseColor;
		VuVector4	mRange;
	};
	VuHeadlight			mHeadlights[2][VuViewportManager::MAX_VIEWPORTS][2];
	int					mSimIndex;
	int					mGfxIndex;

	int					mBlurRenderTargetWidth;
	int					mBlurRenderTargetHeight;
	VuRenderTarget		*mpBlurRenderTarget0;
	VuRenderTarget		*mpBlurRenderTarget1;
};
