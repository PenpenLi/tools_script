//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuBroadcastEntity
// 
//  by zouchao
//*****************************************************************************
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Components/VuTransitionComponent.h"
#include "VuEngine/UI/VuUIAnchor.h"
#include "VuEngine/Assets/VuTextureAsset.h"
#include "VuEngine/UI/VuUIPropertyUtil.h"
#include "VuEngine/Components/2dLayout/Vu2dLayoutComponent.h"
#include "VuEngine/Events/VuEventMap.h"
#include "VuEngine/UI/VuUIDrawUtil.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/UI/VuUIInputUtil.h"
#include "VuEngine/Util/VuImageLoader.h"
#include "VuEngine/HAL/Gfx/VuGfx.h"
#include "VuEngine/HAL/Gfx/VuTexture.h"
#include "VuEngine/Util/VuImageUtil.h"
//#include <fstream>
#include "VuEngine/Managers/VuInputManager.h"
#include "VuEngine/Math/VuMathUtil.h"
#include "VuBroadcastManager.h"
#include "VuEngine/DB/VuFontDB.h"
#include "VuEngine/Gfx/Font/VuFontDraw.h"
#include "VuEngine/Util/VuUtf8.h"
#include "VuPurple/Gfx/VuGameGfxUtil.h"

class VuBroadcastEntity : public VuEntity
{
	DECLARE_RTTI
public:
	VuBroadcastEntity();
	~VuBroadcastEntity();
protected:
	// components
	VuScriptComponent		*mpScriptComponent;
	VuTransitionComponent	*mpTransitionComponent;

	VuRetVal		Hide(const VuParams &params)		{ mVisible = false; return VuRetVal(); }
	// properties
	VuUIAnchor							mAnchor;
	VuUIRectProperties				mRect;
	VuUIRectProperties				mImageRect;
	VuUIRectProperties				mContentBgRect;
	VuUIRectProperties				mClipRect;
	float										mSpacing;
	VuUIRectProperties				mItemRect;
	VuUIImageProperties			mBackgroundTexture;
	VuUIImageProperties			mImageTexture;
	VuUIImageProperties			mContentBg;

	//scroll view
	float										mScrollLag;
	float										mScrollResidualDamping;
	VuUIRectProperties				mScrollBarRect;
	VuColor								mScrollBarColor;
	VuColor								mScrollBarBackColor;
	float										mNextPrevScrollSpeed;
	
	float										mBackgroundSpeed;
	float										mExtraScrollMax;
	VuUIImageProperties			mItemBackgroundTexture;
	enum eState { IDLE, SCROLLING };
	eState									mState;
	VuVector2							mInitialTouchDown;
	float										mScrollPos;
	float										mScrollTargetPos;
	float										mScrollVel;
	float										mScrollResidualVel;
	float										mScrollTouchPos;
	float										mAlpha;
	bool										mVisible;
	bool										mNewTouch;
	bool										mTouchDown;
	bool										mTouchUp;
	VuVector2							mTouchPos;
	int										mTouchIndex;
	int										mPreTouchIndex;
	enum ePressedState { NOT_PRESSED, PRESSED };
	ePressedState					mPressedState;
protected:
	void				OnUITick(const VuParams &params);
	void				OnUITouch(const VuParams &params);
	void				OnUIDraw(const VuParams &params);
	void				drawLayout(bool bSelected);

	void				drawRect(const VuRect &rect);
	void				drawBroadcast(float alpha);
	float				calcScrollMax();
	float				calcScrollSize();
protected:
	VuTexture*								mTexture;
	int											mItemSize;
	std::vector<VuTexture*> mImageTextures;
	std::vector<VuTexture*> mImageNameTextures;
};

IMPLEMENT_RTTI(VuBroadcastEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuBroadcastEntity);

VuBroadcastEntity::VuBroadcastEntity():
mSpacing(0),
mScrollLag(0.1f),
mScrollResidualDamping(0.99f),
mScrollBarRect(0, 0, 0, 0),
mScrollBarColor(255, 255, 255),
mScrollBarBackColor(0, 0, 0),
mNextPrevScrollSpeed(360.0f),
mBackgroundSpeed(1.0f),
mExtraScrollMax(0.0f),
mState(IDLE),
mScrollPos(0),
mScrollTargetPos(0),
mScrollVel(0),
mScrollResidualVel(0),
mAlpha(1.0f),
mVisible(true),
mNewTouch(false),
mTouchDown(false),
mTouchUp(false),
mPressedState(NOT_PRESSED),
mTouchIndex(0),
mPreTouchIndex(-1),
mItemSize(0)
{
	//components
	addComponent(mpScriptComponent = new VuScriptComponent(this, 150, false));
	addComponent(new Vu2dLayoutComponent(this, &VuBroadcastEntity::drawLayout));
	addComponent(mpTransitionComponent = new VuTransitionComponent(this));

	//properties
	ADD_UI_ANCHOR_PROPERTIES(getProperties(), mAnchor, "");
	addProperty(new VuRectProperty("Rect", mRect));
	addProperty(new VuFloatProperty("ScrollLag", mScrollLag));
	addProperty(new VuFloatProperty("ScrollResidualDamping", mScrollResidualDamping));
	addProperty(new VuRectProperty("ScrollBarRect", mScrollBarRect));
	addProperty(new VuColorProperty("ScrollBarColor", mScrollBarColor));
	addProperty(new VuColorProperty("ScrollBarBackColor", mScrollBarBackColor));
	addProperty(new VuFloatProperty("NextPrevScrollSpeed", mNextPrevScrollSpeed));
	addProperty(new VuFloatProperty("BackgroundSpeed", mBackgroundSpeed));
	addProperty(new VuFloatProperty("ExtraScrollMax", mExtraScrollMax));
	addProperty(new VuRectProperty("ItemRect", mItemRect));
	addProperty(new VuRectProperty("Clip Rect", mClipRect));
	addProperty(new VuFloatProperty("Spacing", mSpacing));
	addProperty(new VuRectProperty("ImageRect", mImageRect));
	addProperty(new VuRectProperty("ContentBgRect", mContentBgRect));

	REG_EVENT_HANDLER(VuBroadcastEntity, OnUITick);
	REG_EVENT_HANDLER(VuBroadcastEntity, OnUITouch);
	REG_EVENT_HANDLER(VuBroadcastEntity, OnUIDraw);

	mBackgroundTexture.addProperties(getProperties(), "BackgroundTexture");
	mItemBackgroundTexture.addProperties(getProperties(), "ItemBackgroundTexture");
	mImageTexture.addProperties(getProperties(), "ImageTexture");
	mContentBg.addProperties(getProperties(), "ContentBg");
	

	/*std::fstream binary_file("d:\\test2.png", std::ios::binary | std::ios::in);
	binary_file.seekg(0, std::ios::end);
	int n = binary_file.tellg();
	char *s = NULL;
	s = new char[n + 1];
	binary_file.seekg(0, std::ios::beg);
	binary_file.read(s,n);
	s[n + 1] = '\0';*/
	//VuImageLoader loader;
	if (VuBroadcastManager::IF())
	{
		//if (!VuBroadcastManager::IF()->isDataOK())
			//return;
		mItemSize = VuBroadcastManager::IF()->getBroadcastData().size();
		VuBroadcastManager::BroadcastItems items = VuBroadcastManager::IF()->getBroadcastData();	
	
		VuTexture* tempTexture;
		for (int i = 0; i < mItemSize; i++)
		{
			VuImageLoader loader1, loader2;
			VuTextureState state;
			state.mAddressU = VUGFX_ADDRESS_CLAMP;
			state.mAddressV = VUGFX_ADDRESS_CLAMP;
			state.mMipFilter = VUGFX_TEXF_NONE;
			VuArray<VUBYTE> rgba;

			loader1.load(items[i].mImage, items[i].imageSize);

#if defined VUANDROID 
			loader1.flipImg();
#endif

			if (!VuImageUtil::convertToRGBA(loader1, rgba))
				return;


			tempTexture = VuGfx::IF()->createTexture(loader1.getWidth(), loader1.getHeight(), 0, VUGFX_FORMAT_A8R8G8B8, state);
			tempTexture->setData(0, &rgba.begin(), rgba.size());

			mImageTextures.push_back(tempTexture);

			loader2.load(items[i].mNameImage, items[i].nameImageSize);

#if defined VUANDROID 
			loader2.flipImg();
#endif

			if (!VuImageUtil::convertToRGBA(loader2, rgba))
				return;

			tempTexture = VuGfx::IF()->createTexture(loader2.getWidth(), loader2.getHeight(), 0, VUGFX_FORMAT_A8R8G8B8, state);
			tempTexture->setData(0, &rgba.begin(), rgba.size());

			mImageNameTextures.push_back(tempTexture);
		}
	}
	//else
	//{
	//	loader.load((VUBYTE *)(s), n + 1);
	//}
		
	//VuArray<VUBYTE> rgba;
	//if (!VuImageUtil::convertToRGBA(loader, rgba))
		//return;

	/*	VuTextureState state;
		state.mAddressU = VUGFX_ADDRESS_CLAMP;
		state.mAddressV = VUGFX_ADDRESS_CLAMP;
		state.mMipFilter = VUGFX_TEXF_NONE;

		mTexture = VuGfx::IF()->createTexture(loader.getWidth(), loader.getHeight(), 0, VUGFX_FORMAT_A8R8G8B8, state);

		mTexture->setData(0, &rgba.begin(), rgba.size());*/
}

VuBroadcastEntity::~VuBroadcastEntity()
{
	for (int i = 0; i < mItemSize; i++)
	{
		if (mImageTextures[i])
		{
			mImageTextures[i]->removeRef();
			mImageTextures[i] = NULL;
		}
		if (mImageNameTextures[i])
		{
			mImageNameTextures[i]->removeRef();
			mImageNameTextures[i] = NULL;
		}
	}

	mImageNameTextures.clear();
	mImageTextures.clear();
}

void	VuBroadcastEntity::OnUITick(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	float fdt = accessor.getFloat();
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	mpTransitionComponent->tick(fdt);

	int padIndex = 0;
	if (VuInputManager::IF()->getButtonValue(padIndex, "Up"))
		mScrollTargetPos -= mNextPrevScrollSpeed*fdt;
	if (VuInputManager::IF()->getButtonValue(padIndex, "Down"))
		mScrollTargetPos += mNextPrevScrollSpeed*fdt;

	if (mState != SCROLLING)
	{
		mScrollResidualVel *= VuPow(1.0f - mScrollResidualDamping, fdt);
		mScrollTargetPos += mScrollResidualVel* fdt;
		mScrollTargetPos = VuClamp(mScrollTargetPos, 0.0f, calcScrollMax());
	}
	mScrollPos = VuMathUtil::smoothCD(mScrollPos, mScrollTargetPos, mScrollVel, mScrollLag, fdt);

	if (mNewTouch)
	{
		VuVector2 offset(0.0f, -mScrollPos);
		for (int i = 0; i < mItemSize; i++)
		{
			VuRect dstRect = uiDrawParams.transform(mItemRect + offset);
			if (dstRect.contains(mTouchPos))
			{
				mPreTouchIndex = i;
				mPressedState = PRESSED;
				break;
			}
			offset.mY += mSpacing;
		}	
	}

	if (mPressedState == PRESSED)
	{
		if (mTouchUp == true)
		{
			VuVector2 offset(0.0f, -mScrollPos);
			for (int i = 0; i < mItemSize; i++)
			{
				VuRect dstRect = uiDrawParams.transform(mItemRect + offset);
				if (dstRect.contains(mTouchPos))
				{
					if (i == mPreTouchIndex)
					{
						mTouchIndex = i;
						break;
					}
				}
				offset.mY += mSpacing;
			}
			mPressedState = NOT_PRESSED;
		}
	}
	if (mTouchUp)
	{
		mTouchDown = false;
		mTouchUp = false;
	}
	mNewTouch = false;
}

float VuBroadcastEntity::calcScrollMax()
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	VuRect rect = uiDrawParams.transform(mRect);
	mAnchor.apply(rect, rect);
	rect = uiDrawParams.transformInv(rect);

	float scrollMax = calcScrollSize() - rect.mHeight;
	scrollMax = VuMax(scrollMax, 0.0f);
	return scrollMax;
}

float VuBroadcastEntity::calcScrollSize()
{
	return mItemSize*mSpacing + mExtraScrollMax;
}

void	VuBroadcastEntity::OnUITouch(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	int action = accessor.getInt();
	VuVector2 touch = accessor.getVector2();

	if (action == VuUIInputUtil::TOUCH_DOWN)
	{
		VuUIDrawParams uiDrawParams;
		VuUIDrawUtil::getParams(this, uiDrawParams);
		VuRect clipRect = uiDrawParams.transform(mClipRect);
		mAnchor.apply(clipRect, clipRect);

		if (clipRect.contains(touch))
		{
			mTouchDown = true;
			mNewTouch = true;
			mTouchPos = touch;
			mState = SCROLLING;
			mInitialTouchDown = touch;
			mScrollTouchPos = mScrollTargetPos;
		}
	}
	else if (action == VuUIInputUtil::TOUCH_UP)
	{
		mTouchUp = true;
		mState = IDLE;
		mScrollResidualVel = mScrollVel;
	}
	else if (action == VuUIInputUtil::TOUCH_MOVE)
	{
		mTouchPos = touch;
		if (mState == SCROLLING)
		{
			VuUIDrawParams uiDrawParams;
			VuUIDrawUtil::getParams(this, uiDrawParams);

			VuVector2 touchDelta = uiDrawParams.transformInv(touch) - uiDrawParams.transformInv(mInitialTouchDown);

			mScrollTargetPos = mScrollTouchPos - touchDelta.mY;
			if (mScrollTargetPos > 0)
			{
				int i = 0;
				int j = i;
			}
			mScrollTargetPos = VuClamp(mScrollTargetPos, 0.0f, calcScrollMax());
		}
	}
}

void	VuBroadcastEntity::OnUIDraw(const VuParams &params)
{
	//if (!VuBroadcastManager::IF()->isDataOK())
		//return;
	drawBroadcast(1.0);
}

void	VuBroadcastEntity::drawLayout(bool bSelected)
{
	if (bSelected)
	{
		drawRect(mRect);
		drawRect(mClipRect);
		drawRect(mImageRect);
		drawRect(mContentBgRect);
	}
	
	drawBroadcast(1.0);
	//drawRect(mRect);

	//VuUIDrawParams uiDrawParams;
	//VuUIDrawUtil::getParams(this, uiDrawParams);

	//if (VuTexture *pTexture = mBackgroundTexture.getTexture())
	//{
	//	VuColor color(255, 255, 255);
	//	color.mA = VuRound(color.mA*mAlpha);

	//	VuRect dstRect = uiDrawParams.transform(mRect);
	//	mAnchor.apply(dstRect, dstRect);
	//	VuRect srcRect(0, 0, 1, 1);
	//	//srcRect.mY += mBackgroundSpeed* mScrollPos / mRect.mHeight;
	//	VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth + 2 * GFX_SORT_DEPTH_STEP, pTexture, color, dstRect, srcRect);
	//}
}

void	VuBroadcastEntity::drawRect(const VuRect &rect)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);
	VuRect dstRect = uiDrawParams.transform(rect);
	VuGfxUtil::IF()->drawRectangleOutline2d(0.0f, VuColor(255, 255, 255), dstRect);
}

void	VuBroadcastEntity::drawBroadcast(float alpha)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);
	
	/*drawRect(mRect);
	drawRect(mClipRect);
	drawRect(mImageRect);
	drawRect(mContentBgRect);*/
	VuUIRectProperties itemImage;
	itemImage.set(mItemRect.mX +20, mItemRect.mY + 10, mItemRect.getWidth() - 40, mItemRect.getHeight() - 20);

	VuVector2 offset(0.0f, -mScrollPos);
	for (int i = 0; i < mItemSize; i++)
	{
		//drawRect(mItemRect + offset);
		VuTexture *pTexture = mItemBackgroundTexture.getTexture();
		if (pTexture)
		{
			VuColor color(255, 255, 255);
			color.mA = VuRound(color.mA*mAlpha);
			if (color.mA)
			{
				VuUIDrawParams uiDrawParams;
				VuUIDrawUtil::getParams(this, uiDrawParams);

				VuRect clipRect = uiDrawParams.transform(mClipRect);
				mAnchor.apply(clipRect, clipRect);

				VuRect dstRect = uiDrawParams.transform(mItemRect + offset);
				mAnchor.apply(dstRect, dstRect);
				
				VuRect dstItemImageRect = uiDrawParams.transform(itemImage + offset);
				mAnchor.apply(dstItemImageRect, dstItemImageRect);

				if (dstRect.intersects(clipRect))
				{
					VuRect srcRect(0, 0, 1, 1);
					VuRect dstRectOrig = dstRect;
					dstRect = VuRect::intersection(clipRect, dstRect);
					srcRect.mWidth = dstRect.mWidth / dstRectOrig.mWidth;
					srcRect.mHeight = dstRect.mHeight / dstRectOrig.mHeight;

					if (dstRectOrig.getLeft() < clipRect.getLeft())
						srcRect.mX = 1.0f - srcRect.mWidth;
					if (dstRectOrig.getTop() < clipRect.getTop())
						srcRect.mY = 1.0f - srcRect.mHeight;

					if (i == mTouchIndex)
						VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth + 0.5 * GFX_SORT_DEPTH_STEP, pTexture, color, dstRect, srcRect);
					else
						VuGameGfxUtil::IF()->drawGrayScaleTexture2d(uiDrawParams.mDepth + 0.5 * GFX_SORT_DEPTH_STEP, pTexture, color, dstRect, srcRect);
					//if (VuBroadcastManager::IF()->isDataOK())
					//{
						VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth + 0.001 * GFX_SORT_DEPTH_STEP, mImageNameTextures[i], color, dstItemImageRect, srcRect);
					//}
				}
			}
		}
		offset.mY += mSpacing;
	}

	if (VuTexture *pTexture = mBackgroundTexture.getTexture())
	{
		VuColor color(255, 255, 255);
		color.mA = VuRound(color.mA*mAlpha);

		VuRect dstRect = uiDrawParams.transform(mRect);
		mAnchor.apply(dstRect, dstRect);
		VuRect srcRect(0, 0, 1, 1);
		//srcRect.mY += mBackgroundSpeed* mScrollPos / mRect.mHeight;
		VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth + 2 * GFX_SORT_DEPTH_STEP, pTexture, color, dstRect, srcRect);
	}

	if (VuTexture *pTexture = mContentBg.getTexture())
	{
		VuColor color(255, 255, 255);
		color.mA = VuRound(color.mA*mAlpha);

		VuRect dstRect = uiDrawParams.transform(mContentBgRect);
		mAnchor.apply(dstRect, dstRect);
		VuRect srcRect(0, 0, 1, 1);
		//srcRect.mY += mBackgroundSpeed* mScrollPos / mRect.mHeight;
		VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth + 2 * GFX_SORT_DEPTH_STEP, pTexture, color, dstRect, srcRect);
	}

	if (VuBroadcastManager::IF())
	{
		VuTexture* pTexture;
		//if (VuBroadcastManager::IF()->isDataOK())
		//{
			pTexture = mImageTextures[mTouchIndex];
		//}
		//else
		//{
			//pTexture = mImageTexture.getTexture();
		//}
		
		VuColor color(255, 255, 255);
		color.mA = VuRound(color.mA*mAlpha);

		VuRect dstRect = uiDrawParams.transform(mImageRect);
		mAnchor.apply(dstRect, dstRect);
		VuRect srcRect(0, 0, 1, 1);
		//srcRect.mY += mBackgroundSpeed* mScrollPos / mRect.mHeight;
		VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth + 0.5 * GFX_SORT_DEPTH_STEP, pTexture, color, dstRect, srcRect);
	}
}