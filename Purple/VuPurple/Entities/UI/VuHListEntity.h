//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Horizontal List Entity
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/UI/VuUIPropertyUtil.h"

class VuScriptComponent;
class VuMultiBoingTransitionComponent;


class VuHListEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuHListEntity();
	~VuHListEntity();

	virtual void				onGameInitialize();
	virtual void				onGameRelease();

protected:
	virtual int					getItemCount() = 0;
	virtual void				getItemName(int index, std::string &itemName) = 0;
	virtual void				getItemImage(int index, std::string &itemImage) = 0;
	virtual bool				isItemLocked(int index) = 0;
	virtual void				getUnlockText(int index, std::string &text) = 0;
	virtual void				onItemSelected(int index) = 0;
	virtual void				onItemChosen(int index) = 0;
	virtual void				onItemLocked(int inex) = 0;
	virtual void				onDrawItem(int index, const VuVector2 &offset, float scale) = 0;

	// scripting
	VuRetVal					SetFocus(const VuParams &params = VuParams());

	// event handlers
	virtual void				OnUITick(const VuParams &params);
	void						OnUIGamePad(const VuParams &params);
	void						OnUITouch(const VuParams &params);
	virtual void				OnUIDraw(const VuParams &params);

	virtual void				drawLayout(bool bSelected);
	void						drawList(float scaleStart, float scaleTarget);

	bool						handleNewTouch();

	void						setSelectedItem(int index, bool snap);
	int							getSelectedItem() { return mSelectedItem; }

	bool						touched(const VuRect &rect);

	int							calcNumItemsOnScreen();
	VuVector2					calcPosition();
	VuVector2					calcItemOffset(int itemIndex);
	float						calcScrollMin();
	float						calcScrollMax();

	void						findItem(int index);

	VuTexture					*getTexture(const std::string &assetName);

	// components
	VuScriptComponent			*mpScriptComponent;
	VuMultiBoingTransitionComponent	*mpTransitionComponent;

	// properties
	float						mBottomAnchorRatio;
	float						mSpacing;
	float						mScrollLag;
	float						mScrollResidualDamping;
	float						mScrollThreshold;
	VuUIRectProperties			mTouchRect;
	VuUIImageProperties			mBackgroundImage;
	VuUIImageProperties			mBackgroundSelectedImage;
	VuUIImageProperties			mBackgroundPressedImage;
	VuUIRectProperties			mBackgroundRect;
	VuUIImageProperties			mFrameImage;
	VuUIImageProperties			mFrameSelectedImage;
	VuUIImageProperties			mFramePressedImage;
	VuUIRectProperties			mFrameRect;
	VuUIRectProperties			mImageRect;
	VuUIRectProperties			mNameRect;
	std::string					mNameFont;
	VuUIStringFormatProperties	mNameStringFormat;
	VuUIImageProperties			mLockImage;
	VuUIRectProperties			mLockRect;
	VuUIRectProperties			mUnlockRect;
	std::string					mUnlockFont;
	VuUIStringFormatProperties	mUnlockStringFormat;
	VuUIImageProperties			mMaskImage;
	VuUIRectProperties			mDotRect;
	VuVector2					mDotSpacing;
	VuUIImageProperties			mDotOnImage;
	VuUIImageProperties			mDotOffImage;
	float						mMaxDelay;
	float						mMinAngularFrequency;
	float						mMaxAngularFrequency;
	float						mDampingRatio;

	int							mSelectedItem;

	enum eState { WAITING, PRESSED, SCROLLING };
	eState						mState;
	int							mPressedItem;
	VuVector2					mInitialTouchDown;
	float						mScrollPos;
	float						mScrollTargetPos;
	float						mScrollVel;
	float						mScrollTouchPos;

	bool						mNewTouch;
	bool						mTouchDown;
	bool						mTouchUp;
	VuVector2					mTouchPos;

	typedef std::map<std::string, VuTextureAsset *> TextureAssets;
	TextureAssets				mTextureAssets;

	std::string					mTempText;

	struct Instance
	{
		Instance() : mDone(false) {}
		float	mDelay;
		float	mAngularFrequency;
		bool	mDone;
	};
	typedef std::map<int, Instance> Instances;
	Instances			mInstances;
	Instance			&getInstance(int index);
};
