//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuHUDCharacterAbilityEntity
// 
//*****************************************************************************

#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuEngine/Components/2dLayout/Vu2dLayoutComponent.h"
#include "VuEngine/Assets/VuTextureAsset.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/Shaders/VuBasicShaders.h"
#include "VuEngine/Pfx/VuPfx.h"
#include "VuEngine/HAL/Gfx/VuGfx.h"
#include "VuEngine/UI/VuUI.h"
#include "VuEngine/UI/VuUIPropertyUtil.h"
#include "VuEngine/UI/VuUIDrawUtil.h"
#include "VuEngine/Memory/VuScratchPad.h"


class VuHUDCharacterAbilityEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuHUDCharacterAbilityEntity();

	virtual void		onGameInitialize();
	virtual void		onGameRelease();

protected:
	// event handlers
	void				OnUIDraw(const VuParams &params);

	void				tickBuild(float fdt);
	void				calcPfxTransform(VuMatrix &transform);

	void				drawLayout(bool bSelected);
	void				draw(VuTextureAsset *pDriverImage, bool used, float charge, float alpha);
	void				drawMeter(float charge, float alpha, float depth);

	// properties
	int						mViewport;
	float					mInnerRadius;
	float					mOuterRadius;
	int						mSegmentCount;
	VuColor					mBackColor;
	VuColor					mFillColor;
	VuColor					mUsedColor;
	VuUIImageProperties		mFrameImage;
	VuUIRectProperties		mFrameRect;
	VuUIRectProperties		mDriverRect;
	VuUIAnchorProperties	mAnchor;
	std::string				mPfxName;
	VuVector2				mPfxPos;
	float					mPfxDist;

	VuPfxSystemInstance		*mpPfxSystemInstance;
};

IMPLEMENT_RTTI(VuHUDCharacterAbilityEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuHUDCharacterAbilityEntity);


//*****************************************************************************
VuHUDCharacterAbilityEntity::VuHUDCharacterAbilityEntity():
	mViewport(0),
	mInnerRadius(90),
	mOuterRadius(100),
	mSegmentCount(32),
	mBackColor(0,0,0),
	mFillColor(255,255,255),
	mUsedColor(128,128,128),
	mPfxPos(0,0),
	mPfxDist(10.0f),
	mpPfxSystemInstance(VUNULL)
{
	// components
	addComponent(new Vu2dLayoutComponent(this, &VuHUDCharacterAbilityEntity::drawLayout));

	// properties
	addProperty(new VuIntProperty("Viewport", mViewport));
	addProperty(new VuFloatProperty("Inner Radius", mInnerRadius));
	addProperty(new VuFloatProperty("Outer Radius", mOuterRadius));
	addProperty(new VuIntProperty("Segment Count", mSegmentCount));
	addProperty(new VuColorProperty("Back Color", mBackColor));
	addProperty(new VuColorProperty("Fill Color", mFillColor));
	addProperty(new VuColorProperty("Used Color", mUsedColor));
	mFrameImage.addProperties(getProperties(), "Frame Image");
	addProperty(new VuRectProperty("Frame Rect", mFrameRect));
	addProperty(new VuRectProperty("Driver Rect", mDriverRect));
	ADD_UI_ANCHOR_PROPERTIES(getProperties(), mAnchor, "");
	addProperty(new VuStringProperty("Pfx Name", mPfxName));
	addProperty(new VuVector2Property("Pfx Pos", mPfxPos));
	addProperty(new VuFloatProperty("Pfx Dist", mPfxDist));

	// event handlers
	REG_EVENT_HANDLER(VuHUDCharacterAbilityEntity, OnUIDraw);
}

//*****************************************************************************
void VuHUDCharacterAbilityEntity::onGameInitialize()
{
	VuTickManager::IF()->registerHandler(this, &VuHUDCharacterAbilityEntity::tickBuild, "Build");

	mpPfxSystemInstance = VuPfx::IF()->createSystemInstance(mPfxName.c_str());
}

//*****************************************************************************
void VuHUDCharacterAbilityEntity::onGameRelease()
{
	if ( mpPfxSystemInstance )
	{
		VuPfx::IF()->releaseSystemInstance(mpPfxSystemInstance);
		mpPfxSystemInstance = VUNULL;
	}

	VuTickManager::IF()->unregisterHandler(this, "Build");
}

//*****************************************************************************
void VuHUDCharacterAbilityEntity::OnUIDraw(const VuParams &params)
{
	float alpha = 1.0f;

	if ( VuCarEntity *pCar = VuCarManager::IF()->getCameraTargetForViewport(mViewport) )
	{
		VuCarStats &stats = pCar->getStats();

		draw(pCar->getDriver()->getAbilityImage(), stats.mCharacterAbilityUsed, stats.mCharacterAbilityCharge, alpha);

		if ( mpPfxSystemInstance && (mpPfxSystemInstance->getState() != VuPfxSystemInstance::STATE_STOPPED) )
		{
			const VuCamera &camera = VuViewportManager::IF()->getUiCamera();
			mpPfxSystemInstance->draw(VuGfxDrawParams(camera));
		}
	}
}

//*****************************************************************************
void VuHUDCharacterAbilityEntity::tickBuild(float fdt)
{
	if ( mpPfxSystemInstance )
	{
		if ( VuCarEntity *pCar = VuCarManager::IF()->getCameraTargetForViewport(mViewport) )
		{
			VuCarStats &stats = pCar->getStats();
			if ( !stats.mCharacterAbilityUsed && stats.mCharacterAbilityCharge >= 1.0f )
			{
				if ( mpPfxSystemInstance->getState() != VuPfxSystemInstance::STATE_ALIVE )
					mpPfxSystemInstance->start();
			}
			else
			{
				mpPfxSystemInstance->stop();
			}
		}

		if ( mpPfxSystemInstance->getState() != VuPfxSystemInstance::STATE_STOPPED )
		{
			VuMatrix transform;
			calcPfxTransform(transform);
			mpPfxSystemInstance->setMatrix(transform);

			mpPfxSystemInstance->tick(fdt, true);
		}
	}
}

//*****************************************************************************
void VuHUDCharacterAbilityEntity::calcPfxTransform(VuMatrix &transform)
{
	const VuCamera &camera = VuViewportManager::IF()->getUiCamera();

	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	VuVector2 screenPos = uiDrawParams.transform(mPfxPos);
	mAnchor.apply(screenPos, screenPos);
	screenPos = VuUI::IF()->getCropMatrix().transform(screenPos);
	float dist = (mPfxDist - camera.getNearPlane())/(camera.getFarPlane() - camera.getNearPlane());
	VuVector3 worldPos = camera.screenToWorld(VuVector3(screenPos.mX, screenPos.mY, dist));

	transform = camera.getTransform();
	transform.setTrans(worldPos);
}

//*****************************************************************************
void VuHUDCharacterAbilityEntity::drawLayout(bool bSelected)
{
	draw(VUNULL, false, 0.8f, 1.0f);

	if ( bSelected )
	{
		VuUIDrawParams uiDrawParams;
		VuUIDrawUtil::getParams(this, uiDrawParams);

		// draw rect
		VuRect rect = uiDrawParams.transform(mDriverRect);
		VuGfxUtil::IF()->drawRectangleOutline2d(0.0f, VuColor(255,255,255), rect);

		// draw pfx pos
		VuGfxUtil::IF()->drawLine2d(uiDrawParams.mDepth, VuColor(255,255,255), uiDrawParams.transform(mPfxPos - VuVector2(10,0)), uiDrawParams.transform(mPfxPos + VuVector2(10,0)));
		VuGfxUtil::IF()->drawLine2d(uiDrawParams.mDepth, VuColor(255,255,255), uiDrawParams.transform(mPfxPos - VuVector2(0,10)), uiDrawParams.transform(mPfxPos + VuVector2(0,10)));
	}
}

//*****************************************************************************
void VuHUDCharacterAbilityEntity::draw(VuTextureAsset *pDriverImage, bool used, float charge, float alpha)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	float depth = uiDrawParams.mDepth;

	// meter
	drawMeter(charge, alpha, depth);
	depth -= GFX_SORT_DEPTH_STEP;
	VuColor color(255,255,255);
	color.mA = VuRound(255*alpha);

	if ( used )
		color *= mUsedColor;

	// frame
	{
		VuRect rect = uiDrawParams.transform(mFrameRect);
		mAnchor.apply(rect, rect);
		VuGfxUtil::IF()->drawTexture2d(depth, mFrameImage.getTexture(), color, rect);
	}
	depth -= GFX_SORT_DEPTH_STEP;

	// driver
	if ( pDriverImage )
	{
		VuRect rect = uiDrawParams.transform(mDriverRect);
		mAnchor.apply(rect, rect);
		VuGfxUtil::IF()->drawTexture2d(depth, pDriverImage->getTexture(), color, rect);
	}
	depth -= GFX_SORT_DEPTH_STEP;
}

//*****************************************************************************
void VuHUDCharacterAbilityEntity::drawMeter(float charge, float alpha, float depth)
{
	struct DrawData
	{
		static void callback(void *data)
		{
			DrawData *pData = static_cast<DrawData *>(data);

			// draw filled part
			pData->draw(0.0f, pData->mCharge, pData->mFillColor);

			// draw unfilled part
			pData->draw(pData->mCharge, 1.0f, pData->mBackColor);
		}

		void draw(float start, float end, const VuColor &color)
		{
			if ( end > start )
			{
				VuVertex2dXyz *verts = (VuVertex2dXyz *)VuScratchPad::get(VuScratchPad::GRAPHICS);
				VuVertex2dXyz *pv = verts;

				int segmentCount = VuTruncate((end - start)*mSegmentCount) + 1;

				for ( int i = 0; i <= segmentCount; i++ )
				{
					float angle = VuLerp(start, end, (float)i/segmentCount)*VU_2PI;

					float sinAngle, cosAngle;
					VuSinCosEst(angle, sinAngle, cosAngle);

					pv->mXyz[0] = sinAngle;
					pv->mXyz[1] = -cosAngle;
					pv->mXyz[2] = mDepth;
					pv++;

					pv->mXyz[0] = mInnerRatio*sinAngle;
					pv->mXyz[1] = -mInnerRatio*cosAngle;
					pv->mXyz[2] = mDepth;
					pv++;
				}

				VuGfxUtil::IF()->basicShaders()->set2dXyzConstants(mTransform, color);
				VuGfx::IF()->drawPrimitiveUP(VUGFX_PT_TRIANGLESTRIP, segmentCount*2, verts);
			}
		}

		VuMatrix	mTransform;
		float		mDepth;
		float		mInnerRatio;
		int			mSegmentCount;
		VuColor		mBackColor;
		VuColor		mFillColor;
		float		mCharge;
	};

	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	VuVector2 pos = uiDrawParams.transform(VuVector2(0,0));
	mAnchor.apply(pos, pos);

	VuVector2 scale = mOuterRadius*uiDrawParams.mLocalScale*uiDrawParams.mInvAuthScale;

	VuMatrix transform = VuGfxUtil::IF()->getMatrix();
	transform.translateLocal(VuVector3(pos.mX, pos.mY, 0.0f));
	transform.scaleLocal(VuVector3(scale.mX, scale.mY, 1.0f));

	DrawData *pData = static_cast<DrawData *>(VuGfxSort::IF()->allocateCommandMemory(sizeof(DrawData)));
	pData->mTransform = transform;
	pData->mDepth = uiDrawParams.mDepth;
	pData->mInnerRatio = mInnerRadius/mOuterRadius;
	pData->mSegmentCount = mSegmentCount;
	pData->mBackColor = mBackColor;
	pData->mFillColor = mFillColor;
	pData->mCharge = charge;

	VuGfxSort::IF()->submitDrawCommand<true>(VuGfxSort::TRANS_UI_MODULATE, VuGfxUtil::IF()->basicShaders()->get2dXyzMaterial(VuBasicShaders::FLV_MODULATED), VUNULL, &DrawData::callback, depth);
}
