//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Game Gfx util
// 
//*****************************************************************************

#pragma once

#include "VuEngine/VuSystemComponent.h"
#include "VuEngine/Math/VuMatrix.h"
#include "VuEngine/Util/VuColor.h"
#include "VuEngine/Math/VuRect.h"

class VuGfxSortMaterial;
class VuTexture;


class VuGameGfxUtil : public VuSystemComponent
{
	DECLARE_SYSTEM_COMPONENT(VuGameGfxUtil)

protected:
	// called by game
	friend class VuPurple;
	bool	init();
	void	release();

public:
	VuGameGfxUtil();
	~VuGameGfxUtil();

	// lightning
	struct LightningParams
	{
		VuMatrix	mViewProjMat;
		VuVector3	mEyePosition;
		VuVector3	mSrcPos;
		VuVector3	mDstPos;
		float		mDisplacement;
		int			mBoltCount;
		int			mDetail;
		float		mWidth;
		VuColor		mColor;
		VUUINT32	mRandomSeed;
	};
	void	drawLightning(const LightningParams &params);

	// car icon drawing
	struct CarIconDrawParams
	{
		CarIconDrawParams() : mpIconTexture(VUNULL), mpMaskTexture(VUNULL) {}
		float		mDepth;
		VuTexture	*mpIconTexture;
		VuTexture	*mpMaskTexture;
		VuRect		mDstRect;
		VuRect		mSrcRect;
		VuColor		mColor;
		VuColor		mPaintColor;
	};
	void		drawCarIcon(const CarIconDrawParams &params);

	// gray scale drawing
	void		drawGrayScaleTexture2d(float depth, VuTexture *pTexture, const VuColor &color, const VuRect &dstRect = VuRect(0,0,1,1), const VuRect &srcRect = VuRect(0,0,1,1));

private:
	VuGfxSortMaterial	*mpCarIconMaterial;
	VuGfxSortMaterial	*mpGrayScaleMaterial;
};
