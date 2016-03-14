//*****************************************************************************
//
//  Copyright (c) 2008-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Preview entity
// 
//*****************************************************************************

#include "VuPreviewEntity.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Components/3dDraw/Vu3dDrawComponent.h"
#include "VuEngine/Gfx/GfxScene/VuGfxStaticScene.h"
#include "VuEngine/Gfx/GfxScene/VuGfxAnimatedScene.h"
#include "VuEngine/Gfx/Camera/VuCamera.h"
#include "VuEngine/Gfx/GfxSort/VuGfxSort.h"
#include "VuEngine/Gfx/Font/VuFontDraw.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Animation/VuSkeleton.h"
#include "VuEngine/Animation/VuAnimation.h"
#include "VuEngine/Animation/VuAnimationControl.h"
#include "VuEngine/Animation/VuAnimatedSkeleton.h"
#include "VuEngine/Json/VuJsonContainer.h"
#include "VuEngine/Json/VuJsonReader.h"
#include "VuEngine/Dev/VuDev.h"


IMPLEMENT_RTTI(VuPreviewEntity, VuEntity);


//*****************************************************************************
VuPreviewEntity::VuPreviewEntity():
	mSceneType(INVALID),
	mpAnimatedSkeleton(VUNULL)
{
	// components
	addComponent(mp3dDrawComponent = new Vu3dDrawComponent(this));
	mp3dDrawComponent->setDrawMethod(this, &VuPreviewEntity::draw);
	mp3dDrawComponent->setDrawShadowMethod(this, &VuPreviewEntity::drawShadow);

	mp3dDrawComponent->show();

	// register callbacks/handlers
	VuTickManager::IF()->registerHandler(this, &VuPreviewEntity::tickBuild, "Build");
	VuKeyboard::IF()->addCallback(this);
}

//*****************************************************************************
VuPreviewEntity::~VuPreviewEntity()
{
	mp3dDrawComponent->hide();

	if ( mpAnimatedSkeleton )
		mpAnimatedSkeleton->removeRef();

	// unregister callbacks/handlers
	VuTickManager::IF()->unregisterHandlers(this);
	VuKeyboard::IF()->removeCallback(this);
}

//*****************************************************************************
bool VuPreviewEntity::loadScene(const VuJsonContainer &sceneData, const VuJsonContainer &auxData)
{
	// determine what type of scene to load
	if ( sceneData.hasMember("VuGfxScene") )
	{
		if ( !loadStaticScene(sceneData["VuGfxScene"]) )
			return false;
		mSceneType = STATIC_MODEL;
	}
	else if ( sceneData.hasMember("VuAnimatedModel") )
	{
		if ( !loadAnimatedScene(sceneData["VuAnimatedModel"]) )
			return false;
		mSceneType = ANIMATED_MODEL;
	}
	else if ( sceneData.hasMember("VuAnimation") )
	{
		if ( !loadAnimatedScene(auxData["VuAnimatedModel"]) )
			return false;
		if ( !loadAnimation(sceneData["VuAnimation"]) )
			return false;
		mSceneType = ANIMATION;
	}
	else
	{
		return VUERROR("Unknown scene type.");
	}

	return true;
}

//*****************************************************************************
bool VuPreviewEntity::loadStaticScene(const VuJsonContainer &data)
{
	// load model container
	VuGfxStaticScene *pGfxStaticScene = new VuGfxStaticScene;
	if ( !pGfxStaticScene->load(data) )
	{
		pGfxStaticScene->removeRef();
		return false;
	}

	// create static model instance
	mStaticModelInstance.setModel(pGfxStaticScene);
	pGfxStaticScene->removeRef();

	mp3dDrawComponent->updateVisibility(mStaticModelInstance.getAabb());

	return true;
}

//*****************************************************************************
bool VuPreviewEntity::loadAnimatedScene(const VuJsonContainer &data)
{
	// load skeleton
	VuSkeleton *pSkeleton = new VuSkeleton;
	if ( !pSkeleton->load(data["Skeleton"]) )
	{
		pSkeleton->removeRef();
		return false;
	}

	// load model container
	VuGfxAnimatedScene *pGfxAnimatedScene = new VuGfxAnimatedScene;
	if ( !pGfxAnimatedScene->load(data) )
	{
		pSkeleton->removeRef();
		pGfxAnimatedScene->removeRef();
		return false;
	}

	// create animated model instance
	mAnimatedModelInstance.setModel(pSkeleton, pGfxAnimatedScene);

	// create animated skeleton
	mpAnimatedSkeleton = new VuAnimatedSkeleton(pSkeleton);

	// clean up
	pSkeleton->removeRef();
	pGfxAnimatedScene->removeRef();

	mp3dDrawComponent->updateVisibility(mAnimatedModelInstance.getLocalAabb());

	return true;
}

//*****************************************************************************
bool VuPreviewEntity::loadAnimation(const VuJsonContainer &data)
{
	// load animation
	VuAnimation *pAnimation = new VuAnimation;
	if ( !pAnimation->load(data, false) )
	{
		pAnimation->removeRef();
		return false;
	}

	// create animation control
	VuAnimationControl *pAnimationControl = new VuAnimationControl(pAnimation);
	if ( mpAnimatedSkeleton )
		mpAnimatedSkeleton->addAnimationControl(pAnimationControl);

	// clean up
	pAnimationControl->removeRef();
	pAnimation->removeRef();

	return true;
}

//*****************************************************************************
VuAabb VuPreviewEntity::getAabb()
{
	if ( mSceneType == STATIC_MODEL )
	{
		return mStaticModelInstance.getAabb();
	}
	else if ( mSceneType == ANIMATED_MODEL || mSceneType == ANIMATION )
	{
		return VuAabb(mAnimatedModelInstance.getLocalAabb(), mAnimatedModelInstance.getRootTransform());
	}

	return VuAabb::zero();
}

//*****************************************************************************
void VuPreviewEntity::setDrawInfoParams(VUUINT32 flags, const VuColor &textColor, const VuColor &lineColor)
{
	mDrawInfoFlags = flags;
	mDrawInfoTextColor = textColor;
	mDrawInfoLineColor = lineColor;
}

//*****************************************************************************
void VuPreviewEntity::draw(const VuGfxDrawParams &params)
{
	VuGfxDrawInfoParams infoParams(params.mCamera);
	infoParams.mFlags = mDrawInfoFlags;
	infoParams.mDevTextColor = mDrawInfoTextColor;
	infoParams.mDevLineColor = mDrawInfoLineColor;

	if ( mSceneType == STATIC_MODEL )
	{
		mStaticModelInstance.draw(VuMatrix::identity(), params);
		mStaticModelInstance.drawInfo(VuMatrix::identity(), infoParams);
	}
	else if ( mSceneType == ANIMATED_MODEL || mSceneType == ANIMATION )
	{
		mAnimatedModelInstance.draw(VuMatrix::identity(), params);
		mAnimatedModelInstance.drawInfo(VuMatrix::identity(), infoParams);
	}

	if ( mSceneType == ANIMATION && (mDrawInfoFlags & VuGfxDrawInfoParams::ANIMATION_TIMELINE) )
	{
		drawAnimationTimeLine();
	}
}

//*****************************************************************************
void VuPreviewEntity::drawShadow(const VuGfxDrawShadowParams &params)
{
	if ( mSceneType == STATIC_MODEL )
	{
		mStaticModelInstance.drawShadow(VuMatrix::identity(), params);
	}
	else if ( mSceneType == ANIMATED_MODEL || mSceneType == ANIMATION )
	{
		mAnimatedModelInstance.drawShadow(VuMatrix::identity(), params);
	}
}

//*****************************************************************************
void VuPreviewEntity::drawAnimationTimeLine()
{
	if ( !mpAnimatedSkeleton || !mpAnimatedSkeleton->getAnimationControlCount() )
		return;

	VuAnimationControl *pAC = mpAnimatedSkeleton->getAnimationControl(0);
	const VuAnimation *pAnim = pAC->getAnimation();
	if ( pAnim->getFrameCount() <= 1 )
		return;

	// colors
	VuColor gray(192,192,192);
	VuColor green(64, 255, 64);

	// font draw params
	VuFontDraw *pFontDraw = VuGfxUtil::IF()->fontDraw();
	VuFontDrawParams fdParams;
	fdParams.mSize = 30;
	fdParams.mColor = mDrawInfoTextColor;

	// set FSL
	int prevFSL = VuGfxSort::IF()->getFullScreenLayer();
	VuGfxSort::IF()->setFullScreenLayer(VuGfxSort::FSL_HUD);

	// background
	VuRect rect(0.05f, 0.85f, 0.9f, 0.10f);
	VuGfxUtil::IF()->drawFilledRectangle2d(FLT_EPSILON, VuColor(0, 0, 0, 128), rect);

	// time line
	VuVector2 begin(0.1f, 0.885f), end(0.9f, 0.885f);
	VuGfxUtil::IF()->drawLine2d(0, gray, begin, end);

	// frame markers
	for ( int iFrame = 0; iFrame < pAnim->getFrameCount(); iFrame++ )
	{
		VuVector2 center = VuLerp(begin, end, (float)iFrame/(pAnim->getFrameCount() - 1));
		float size = 0.01f;
		if ( iFrame%10 == 0 )
			size = 0.025f;
		else if ( iFrame%5 == 0 )
			size = 0.015f;
		VuGfxUtil::IF()->drawLine2d(0, gray, center - VuVector2(0.0f, size), center + VuVector2(0.0f, size));
	}

	// current time marker
	float curPos = VuClamp(pAC->getLocalTime()/pAnim->getEndTime(), 0.0f, 1.0f);
	VuVector2 current = VuLerp(begin, end, curPos);
	VuGfxUtil::IF()->drawLine2d(0, green, current - VuVector2(0.0f, 0.025f), current + VuVector2(0.0f, 0.025f));

	// frame #s
	char str[256];
	VuRect textRect;

	textRect = VuRect(begin.mX - 0.01f, begin.mY, 0.0f, 0.0f);
	pFontDraw->drawString(0, VuDev::IF()->getFont(), "1", fdParams, textRect, VUGFX_TEXT_DRAW_VCENTER|VUGFX_TEXT_DRAW_RIGHT);

	textRect = VuRect(end.mX + 0.01f, end.mY, 0.0f, 0.0f);
	VU_SPRINTF(str, sizeof(str), "%d", pAnim->getFrameCount());
	pFontDraw->drawString(0, VuDev::IF()->getFont(), str, fdParams, textRect, VUGFX_TEXT_DRAW_VCENTER|VUGFX_TEXT_DRAW_LEFT);

	textRect = VuRect(current.mX, current.mY + 0.025f, 0.0f, 0.0f);
	VU_SPRINTF(str, sizeof(str), "%.2f", 1 + pAnim->getFrameCount()*pAC->getLocalTime()/pAnim->getTotalTime());
	pFontDraw->drawString(0, VuDev::IF()->getFont(), str, fdParams, textRect, VUGFX_TEXT_DRAW_HCENTER);

	// restore FSL
	VuGfxSort::IF()->setFullScreenLayer(prevFSL);
}

//*****************************************************************************
void VuPreviewEntity::tickBuild(float fdt)
{
	if ( mpAnimatedSkeleton )
	{
		mpAnimatedSkeleton->advance(fdt);
		mpAnimatedSkeleton->build();
		mAnimatedModelInstance.setPose(mpAnimatedSkeleton);
		mAnimatedModelInstance.finalizePose();
	}
}

//*****************************************************************************
void VuPreviewEntity::onKeyDown(VUUINT32 key)
{
	if ( VuKeyboard::IF()->isKeyDown(VUKEY_SHIFT) )
	{
		if ( mpAnimatedSkeleton && mpAnimatedSkeleton->getAnimationControlCount() )
		{
			VuAnimationControl *pAC = mpAnimatedSkeleton->getAnimationControl(0);
			const VuAnimation *pAnim = pAC->getAnimation();
			int frame = VuRound(pAnim->getFrameCount()*pAC->getLocalTime()/pAnim->getTotalTime());
			if ( key == VUKEY_UP )
			{
				pAC->setLocalTime(0);
			}
			else if ( key == VUKEY_DOWN )
			{
				pAC->setLocalTime(pAnim->getEndTime());
			}
			if ( VuTickManager::IF()->isPaused() )
			{
				if ( key == VUKEY_LEFT )
				{
					frame = (frame - 1 + pAnim->getFrameCount())%pAnim->getFrameCount();
					pAC->setLocalTime(pAnim->getTotalTime()*frame/pAnim->getFrameCount());
				}
				else if ( key == VUKEY_RIGHT )
				{
					frame = (frame + 1)%pAnim->getFrameCount();
					pAC->setLocalTime(pAnim->getTotalTime()*frame/pAnim->getFrameCount());
				}
			}
			else
			{
				if ( key == VUKEY_LEFT )
				{
					pAC->setTimeFactor(-1.0f);
				}
				else if ( key == VUKEY_RIGHT )
				{
					pAC->setTimeFactor(1.0f);
				}
			}
		}
	}
}
