//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Ui Car entity
// 
//*****************************************************************************

#include "VuEngine/Gfx/Model/VuStaticModelInstance.h"
#include "VuEngine/Gfx/Model/VuAnimatedModelInstance.h"
#include "VuEngine/Gfx/Model/VuModelSkin.h"

class VuBlobShadow;


class VuUiCar
{
public:
	VuUiCar(bool dropShadowEnabled);
	~VuUiCar();

	void			setCar(const std::string &car, int stage);
	void			setSkin(const std::string &decal, const std::string &paintColor, const std::string &decalColor);
	void			setSkin(const std::string &carSkin);
	void			setDriver(const std::string &driver);
	void			setDriverAnim(const std::string &anim);

	void			setColor(const VuColor &color) { mCarColor = mDriverColor = color; }
	void			setCarColor(const VuColor &color) { mCarColor = color; }
	void			setDriverColor(const VuColor &color) { mDriverColor = color; }

	void			setTransform(const VuMatrix &transform) { mTransform = transform; }
	void			tickDecision(float fdt);
	void			tickAnim(float fdt);

	void			draw(const VuGfxDrawParams &params);
	void			drawShadow(const VuGfxDrawShadowParams &params);

	const VuAabb	&getAabb() { return mAabb; }

	void			setBlobShadowDirty() { mBlobShadowDirty = true; }

protected:
	void			configShaderLOD(int value);

	std::string			mCurCar;
	int					mCurStage;
	std::string			mCurDecal;
	std::string			mCurPaintColor;
	std::string			mCurDecalColor;
	std::string			mCurDriver;
	std::string			mCurDriverAnim;

	VuColor					mCarColor;
	VuColor					mDriverColor;
	VuStaticModelInstance	mChassis;
	VuAnimatedModelInstance	mSuspension;
	VuStaticModelInstance	mFrontWheel;
	VuStaticModelInstance	mRearWheel;
	VuModelSkin				mSkin;
	VuAnimatedModelInstance	mDriver;
	VuAnimatedSkeleton		*mpAnimatedSkeleton;
	VuBlobShadow			*mpBlobShadow;
	bool					mBlobShadowDirty;

	VuMatrix			mTransform;
	VuMatrix			mDriverTransform;
	VuMatrix			mWheelTransforms[4];
	VuAabb				mAabb;
	VuAabb				mCollisionAabb;
	float				mOffsetZ;
};
