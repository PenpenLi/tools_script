//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Horizontal List Entity
// 
//*****************************************************************************

#include "VuHListEntity.h"
#include "VuPurple/Components/VuMultiBoingTransitionComponent.h"
#include "VuPurple/Managers/VuControlMethodManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/UI/VuUI.h"
#include "VuEngine/UI/VuUIDrawUtil.h"
#include "VuEngine/UI/VuUIInputUtil.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Components/2dLayout/Vu2dLayoutComponent.h"
#include "VuEngine/Properties/VuFontEnumProperty.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Assets/VuTextureAsset.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/Font/VuFontDraw.h"
#include "VuEngine/DB/VuStringDB.h"
#include "VuEngine/Math/VuMathUtil.h"


IMPLEMENT_RTTI(VuHListEntity, VuEntity);


//*****************************************************************************
VuHListEntity::VuHListEntity():
	mBottomAnchorRatio(0.0f),
	mSpacing(540),
	mScrollLag(0.1f),
	mScrollResidualDamping(0.99f),
	mScrollThreshold(20),
	mDotSpacing(0,0),
	mMaxDelay(0.3f),
	mMinAngularFrequency(2.0f),
	mMaxAngularFrequency(3.0f),
	mDampingRatio(0.5f),
	mSelectedItem(0),
	mState(WAITING),
	mPressedItem(-1),
	mScrollPos(0),
	mScrollTargetPos(0),
	mScrollVel(0),
	mNewTouch(false),
	mTouchDown(false),
	mTouchUp(false)
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this, 120));
	addComponent(new Vu2dLayoutComponent(this, &VuHListEntity::drawLayout));
	addComponent(mpTransitionComponent = new VuMultiBoingTransitionComponent(this));

	// properties
	addProperty(new VuFloatProperty("Bottom Anchor Ratio", mBottomAnchorRatio));
	addProperty(new VuFloatProperty("Spacing", mSpacing));
	addProperty(new VuFloatProperty("Scroll Lag", mScrollLag));
	addProperty(new VuFloatProperty("Scroll Residual Damping", mScrollResidualDamping));
	addProperty(new VuFloatProperty("Scroll Threshold", mScrollThreshold));
	addProperty(new VuRectProperty("Touch Rect", mTouchRect));
	mBackgroundImage.addProperties(getProperties(), "Background Image");
	mBackgroundSelectedImage.addProperties(getProperties(), "Background Selected Image");
	mBackgroundPressedImage.addProperties(getProperties(), "Background Pressed Image");
	addProperty(new VuRectProperty("Background Rect", mBackgroundRect));
	mFrameImage.addProperties(getProperties(), "Frame Image");
	mFrameSelectedImage.addProperties(getProperties(), "Frame Selected Image");
	mFramePressedImage.addProperties(getProperties(), "Frame Pressed Image");
	addProperty(new VuRectProperty("Frame Rect", mFrameRect));
	addProperty(new VuRectProperty("Image Rect", mImageRect));
	addProperty(new VuRectProperty("Name Rect", mNameRect));
	addProperty(new VuFontEnumProperty("Name Font", mNameFont));
	ADD_UI_STRING_FORMAT_PROPERTIES(getProperties(), mNameStringFormat, "Name ");
	mLockImage.addProperties(getProperties(), "Lock Image");
	addProperty(new VuRectProperty("Lock Rect", mLockRect));
	addProperty(new VuRectProperty("Unlock Rect", mUnlockRect));
	addProperty(new VuFontEnumProperty("Unlock Font", mUnlockFont));
	ADD_UI_STRING_FORMAT_PROPERTIES(getProperties(), mUnlockStringFormat, "Unlock ");
	mMaskImage.addProperties(getProperties(), "Mask Image");
	addProperty(new VuRectProperty("Dot Rect", mDotRect));
	addProperty(new VuVector2Property("Dot Spacing", mDotSpacing));
	mDotOnImage.addProperties(getProperties(), "Dot On Image");
	mDotOffImage.addProperties(getProperties(), "Dot Off Image");
	addProperty(new VuFloatProperty("Max Delay", mMaxDelay));
	addProperty(new VuFloatProperty("Min Angular Frequency", mMinAngularFrequency));
	addProperty(new VuFloatProperty("Max Angular Frequency", mMaxAngularFrequency));
	addProperty(new VuFloatProperty("Damping Ratio", mDampingRatio));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuHListEntity, SetFocus);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, NavUp);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, NavDown);

	// event handlers
	REG_EVENT_HANDLER(VuHListEntity, OnUITick);
	REG_EVENT_HANDLER(VuHListEntity, OnUIGamePad);
	REG_EVENT_HANDLER(VuHListEntity, OnUITouch);
	REG_EVENT_HANDLER(VuHListEntity, OnUIDraw);
}

//*****************************************************************************
VuHListEntity::~VuHListEntity()
{
	for ( TextureAssets::iterator iter = mTextureAssets.begin(); iter != mTextureAssets.end(); iter++ )
		VuAssetFactory::IF()->releaseAsset(iter->second);
}

//*****************************************************************************
void VuHListEntity::onGameInitialize()
{
}

//*****************************************************************************
void VuHListEntity::onGameRelease()
{
}

//*****************************************************************************
VuRetVal VuHListEntity::SetFocus(const VuParams &params)
{
	VuUI::IF()->setFocus(this);

	return VuRetVal();
}

//*****************************************************************************
void VuHListEntity::OnUITick(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	float fdt = accessor.getFloat();

	mpTransitionComponent->tick(fdt);

	if ( mNewTouch )
	{
		if ( handleNewTouch() )
			SetFocus();
	}

	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	mScrollTargetPos = VuClamp(mScrollTargetPos, calcScrollMin(), calcScrollMax());
	mScrollPos = VuMathUtil::smoothCD(mScrollPos, mScrollTargetPos, mScrollVel, mScrollLag, fdt);

	if ( mState == PRESSED )
	{
		if ( mTouchDown )
		{
			VuVector2 touchDelta = uiDrawParams.transformInv(mTouchPos) - uiDrawParams.transformInv(mInitialTouchDown);
			VuVector2 itemOffset = calcItemOffset(mPressedItem);

			if ( VuAbs(touchDelta.mX) > mScrollThreshold || !touched(mTouchRect + itemOffset) )
				mState = SCROLLING;
		}
		else
		{
			mState = WAITING;
			if ( mSelectedItem != mPressedItem )
				setSelectedItem(mPressedItem, false);

			// is item on screen?
			if ( VuAbs(mScrollTargetPos - mScrollPos) < 1.0f )
			{
				if ( isItemLocked(getSelectedItem()) )
				{
					onItemLocked(getSelectedItem());
					VuGameUtil::IF()->playSfx(VuGameUtil::SFX_UI_ALERT);
				}
				else
				{
					onItemChosen(getSelectedItem());
					VuGameUtil::IF()->playSfx(VuGameUtil::SFX_UI_SELECT);
				}
			}
		}
	}
	else if ( mState == SCROLLING )
	{
		if ( mTouchDown )
		{
			VuVector2 touchDelta = uiDrawParams.transformInv(mTouchPos) - uiDrawParams.transformInv(mInitialTouchDown);

			mScrollTargetPos = mScrollTouchPos - touchDelta.mX;
		}
		else
		{
			mState = WAITING;
			mScrollTargetPos = VuRound((mScrollTargetPos + mScrollVel*0.25f)/mSpacing)*mSpacing;
		}
	}
	else
	{
		mPressedItem = -1;
	}

	if ( mTouchUp )
	{
		mTouchDown = false;
		mTouchUp = false;
	}
	mNewTouch = false;
}

//*****************************************************************************
void VuHListEntity::OnUIGamePad(const VuParams &params)
{
	if ( VuUI::IF()->getFocus() == this )
	{
		VuParams::VuAccessor accessor(params);
		int channel = accessor.getInt();
		bool down = accessor.getBool();
		int padIndex = accessor.getInt();

		if ( down )
		{
			int maxIndex = getItemCount() - 1;

			if ( channel == VuUIInputUtil::CHANNEL_UP )
			{
				mpScriptComponent->getPlug("NavUp")->execute();

				if ( mpScriptComponent->getPlug("NavUp")->getNumConnections() )
					VuGameUtil::IF()->playSfx(VuGameUtil::SFX_UI_NAVIGATE);
			}
			else if ( channel == VuUIInputUtil::CHANNEL_DOWN )
			{
				mpScriptComponent->getPlug("NavDown")->execute();

				if ( mpScriptComponent->getPlug("NavDown")->getNumConnections() )
					VuGameUtil::IF()->playSfx(VuGameUtil::SFX_UI_NAVIGATE);
			}
			else if ( channel == VuUIInputUtil::CHANNEL_LEFT )
			{
				if ( maxIndex > 0 )
				{
					int prevItem = getSelectedItem() - 1;
					if ( prevItem >= 0 )
					{
						setSelectedItem(prevItem, false);
						findItem(prevItem);
						VuGameUtil::IF()->playSfx(VuGameUtil::SFX_UI_NAVIGATE);
					}
				}
			}
			else if ( channel == VuUIInputUtil::CHANNEL_RIGHT )
			{
				if ( maxIndex > 0 )
				{
					int nextItem = getSelectedItem() + 1;
					if ( nextItem < getItemCount() )
					{
						setSelectedItem(nextItem, false);
						findItem(nextItem);
						VuGameUtil::IF()->playSfx(VuGameUtil::SFX_UI_NAVIGATE);
					}
				}
			}
			else if ( channel == VuUIInputUtil::CHANNEL_SELECT )
			{
				if ( isItemLocked(getSelectedItem()) )
				{
					onItemLocked(getSelectedItem());
					VuGameUtil::IF()->playSfx(VuGameUtil::SFX_UI_ALERT);
				}
				else
				{
					onItemChosen(getSelectedItem());
					VuGameUtil::IF()->playSfx(VuGameUtil::SFX_UI_SELECT);
				}
			}
		}
	}
}

//*****************************************************************************
void VuHListEntity::OnUITouch(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	int action = accessor.getInt();
	VuVector2 touch = accessor.getVector2();
		
	if ( action == VuUIInputUtil::TOUCH_DOWN )
	{
		if ( !mTouchDown )
		{
			mTouchDown = true;
			mNewTouch = true;
			mTouchPos = touch;
		}
	}
	else if ( action == VuUIInputUtil::TOUCH_UP )
	{
		mTouchUp = true;
	}
	else if ( action == VuUIInputUtil::TOUCH_MOVE )
	{
		mTouchPos = touch;
	}
}

//*****************************************************************************
void VuHListEntity::OnUIDraw(const VuParams &params)
{
	float scaleStart = 0.0f;
	float scaleTarget = 1.0f;
	if ( mpTransitionComponent->getState() == VuTransitionComponent::STATE_TRANS_OUT )
	{
		scaleStart = 1.0f;
		scaleTarget = 0.0f;
	}

	drawList(scaleStart, scaleTarget);
}

//*****************************************************************************
void VuHListEntity::drawLayout(bool bSelected)
{
	if ( bSelected )
	{
		VuUIDrawParams uiDrawParams;
		VuUIDrawUtil::getParams(this, uiDrawParams);

		VuVector2 position = calcPosition();

		VuGfxUtil::IF()->drawRectangleOutline2d(uiDrawParams.mDepth, VuColor(255,255,255), (mTouchRect + position)*uiDrawParams.mInvAuthScale);

		// name
		{
			const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mNameFont);
			VuGfxUtil::IF()->drawRectangleOutline2d(uiDrawParams.mDepth, fontEntry.params().mColor, (mNameRect + position)*uiDrawParams.mInvAuthScale);
		}
	
		// unlock text
		{
			const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mUnlockFont);
			VuGfxUtil::IF()->drawRectangleOutline2d(uiDrawParams.mDepth, fontEntry.params().mColor, (mUnlockRect + position)*uiDrawParams.mInvAuthScale);
		}
	}

	drawList(1.0f, 1.0f);
}

//*****************************************************************************
void VuHListEntity::drawList(float scaleStart, float scaleTarget)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	bool hasFocus = VuUI::IF()->getFocus() == this;

	VuVector2 position = calcPosition();

	int itemCount = getItemCount();
	for ( int i = 0; i < itemCount; i++ )
	{
		VuVector2 offset = position + calcItemOffset(i);

		Instance &inst = getInstance(i);

		// determine scale
		float scale = scaleStart, vel = 0.0f;
		float age = VuMax(mpTransitionComponent->getTransitionTimer() - inst.mDelay, 0.0f);
		VuMathUtil::calcDampedSimpleHarmonicMotion(&scale, &vel, scaleTarget, age, inst.mAngularFrequency*VU_2PI, mDampingRatio);

		if ( scale < 0.0f )
			inst.mDone = true;
		if ( inst.mDone )
			continue;

		// draw background
		{
			VuColor backgroundColor(255,255,255);

			VuTexture *pTexture = mBackgroundImage.getTexture();
			if ( hasFocus )
			{
				if ( i == mPressedItem )
				{
					pTexture = mBackgroundPressedImage.getTexture();
				}
				else if ( (i == mSelectedItem) && VuControlMethodManager::IF()->isGamePadOrKeyboard() )
				{
					pTexture = mBackgroundSelectedImage.getTexture();
					backgroundColor = VuGameUtil::IF()->calcHighlightColor(1.0f);
				}
			}

			if ( pTexture )
			{
				VuRect rect = mBackgroundRect;
				rect.scale(mTouchRect.getCenter(), scale);
				VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth + 7*GFX_SORT_DEPTH_STEP, pTexture, backgroundColor, (rect + offset)*uiDrawParams.mInvAuthScale);
			}
		}

		// draw frame
		{
			VuTexture *pTexture = mFrameImage.getTexture();
			if ( hasFocus )
			{
				if ( i == mPressedItem )
					pTexture = mFramePressedImage.getTexture();
				else if ( (i == mSelectedItem) && VuControlMethodManager::IF()->isGamePadOrKeyboard() )
					pTexture = mFrameSelectedImage.getTexture();
			}

			if ( pTexture )
			{
				VuRect rect = mFrameRect;
				rect.scale(mTouchRect.getCenter(), scale);
				VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth - 7*GFX_SORT_DEPTH_STEP, pTexture, (rect + offset)*uiDrawParams.mInvAuthScale);
			}
		}

		// draw image
		mTempText.clear();
		getItemImage(i, mTempText);
		if ( VuTexture *pTexture = getTexture(mTempText) )
		{
			VuRect rect = mImageRect;
			rect.scale(mTouchRect.getCenter(), scale);

			if ( VuTexture *pMaskTexture = mMaskImage.getTexture() )
				VuGfxUtil::IF()->drawMaskedTexture2d(uiDrawParams.mDepth + 6*GFX_SORT_DEPTH_STEP, pTexture, pMaskTexture, VuColor(255,255,255), (rect + offset)*uiDrawParams.mInvAuthScale);
			else
				VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth + 6*GFX_SORT_DEPTH_STEP, pTexture, (rect + offset)*uiDrawParams.mInvAuthScale);
		}

		if ( isItemLocked(i) )
		{
			VuRect rect = mLockRect;
			rect.scale(mTouchRect.getCenter(), scale);

			// draw lock image
			if ( VuTexture *pTexture = mLockImage.getTexture() )
			{
				VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth - 9*GFX_SORT_DEPTH_STEP, pTexture, (rect + offset)*uiDrawParams.mInvAuthScale);
			}

			// draw unlock text
			{
				mTempText.clear();
				getUnlockText(i, mTempText);
				if ( mTempText.length() )
				{
					const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mUnlockFont);
					VuFontDrawParams fdParams = fontEntry.params();

					VuRect dstRect = mUnlockRect;
					dstRect.scale(mTouchRect.getCenter(), scale);
					dstRect = (dstRect + offset)*uiDrawParams.mInvAuthScale;
					fdParams.mSize *= scale;

					VuGfxUtil::IF()->fontDraw()->drawString(uiDrawParams.mDepth - 10*GFX_SORT_DEPTH_STEP, fontEntry.font(), mTempText.c_str(), fdParams, dstRect, mUnlockStringFormat);
				}
			}
		}

		// draw name
		{
			const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mNameFont);
			VuFontDrawParams fdParams = fontEntry.params();

			mTempText.clear();
			getItemName(i, mTempText);
			const std::string &strText = VuStringDB::IF()->getString(mTempText);

			VuRect dstRect = mNameRect;
			dstRect.scale(mTouchRect.getCenter(), scale);
			dstRect = (dstRect + offset)*uiDrawParams.mInvAuthScale;
			fdParams.mSize *= scale;

			VuGfxUtil::IF()->fontDraw()->drawString(uiDrawParams.mDepth, fontEntry.font(), strText.c_str(), fdParams, dstRect, mNameStringFormat);
		}

		// draw dot
		if ( itemCount >= 2 && mDotSpacing.magSquared() > FLT_EPSILON )
		{
			VuRect dstRect = mDotRect + (i - 0.5f*(itemCount - 1))*mDotSpacing;
			dstRect.scale(dstRect.getCenter(), scale);
			dstRect = uiDrawParams.transform(dstRect);

			VuVector2 imageCenter = (mImageRect.getCenter() + offset)*uiDrawParams.mInvAuthScale;
			imageCenter = VuGfxUtil::IF()->getMatrix().transform(imageCenter);
			if ( imageCenter.mX >= 0.0f && imageCenter.mX <= 1.0f )
				VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth, mDotOnImage.getTexture(), dstRect);
			else
				VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth, mDotOffImage.getTexture(), dstRect);

		}

		// allow derived class to draw
		onDrawItem(i, offset, scale);
	}
}

//*****************************************************************************
bool VuHListEntity::handleNewTouch()
{
	int itemCount = getItemCount();
	VuRect overallTouchRect = VuRect::bounds(mTouchRect + calcItemOffset(0), mTouchRect + calcItemOffset(itemCount - 1));

	if ( touched(overallTouchRect) )
	{
		mInitialTouchDown = mTouchPos;
		mScrollTouchPos = mScrollTargetPos;

		for ( int iItem = 0; iItem < itemCount; iItem++ )
		{
			VuVector2 itemOffset = calcItemOffset(iItem);

			if ( touched(mTouchRect + itemOffset) )
			{
				mPressedItem = iItem;
				mState = PRESSED;
				return true;
			}
		}

		mState = SCROLLING;
		return true;
	}

	return false;
}

//*****************************************************************************
void VuHListEntity::setSelectedItem(int index, bool snap)
{
	mSelectedItem = index;

	findItem(index);

	if ( snap  )
	{
		mScrollPos = mScrollTargetPos;
		mScrollVel = 0.0f;
	}

	onItemSelected(index);
}

//*****************************************************************************
bool VuHListEntity::touched(const VuRect &rect)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	VuRect touchRect = (rect + calcPosition())*uiDrawParams.mInvAuthScale;

	return touchRect.contains(mTouchPos);
}

//*****************************************************************************
int VuHListEntity::calcNumItemsOnScreen()
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	const VuMatrix &cropMatrix = VuUI::IF()->getCropMatrix();
	float screenWidth = 1.0f/(uiDrawParams.mInvAuthScale.mX*cropMatrix.mX.mX);

	int numItems = VuTruncate((screenWidth - mTouchRect.mWidth)/mSpacing) + 1;
	numItems = VuMin(numItems, getItemCount());

	return numItems;
}

//*****************************************************************************
VuVector2 VuHListEntity::calcPosition()
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	const VuMatrix &cropMatrix = VuUI::IF()->getCropMatrix();

	VuVector2 position = uiDrawParams.mPosition;

	position *= uiDrawParams.mInvAuthScale;
	position.mX -= cropMatrix.mT.mX/cropMatrix.mX.mX;
	position.mY += mBottomAnchorRatio*(1.0f - cropMatrix.mT.mY - cropMatrix.mY.mY)/cropMatrix.mY.mY;
	position /= uiDrawParams.mInvAuthScale;

	// are we scrolling?
	float screenWidth = 1.0f/(uiDrawParams.mInvAuthScale.mX*cropMatrix.mX.mX);
	float listWidth = mTouchRect.mWidth + (getItemCount() - 1)*mSpacing;
	if ( screenWidth > listWidth )
	{
		position.mX += 0.5f*(screenWidth - listWidth);
	}
	else
	{
		int numItemsOnScreen = calcNumItemsOnScreen();
		float width = mTouchRect.mWidth + (numItemsOnScreen - 1)*mSpacing;
		position.mX += 0.5f*(screenWidth - width) - mTouchRect.mX;
	}

	return position;
}

//*****************************************************************************
VuVector2 VuHListEntity::calcItemOffset(int itemIndex)
{
	return VuVector2(itemIndex*mSpacing - mScrollPos, 0.0f);
}

//*****************************************************************************
float VuHListEntity::calcScrollMin()
{
	return 0.0f;
}

//*****************************************************************************
float VuHListEntity::calcScrollMax()
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	const VuMatrix &cropMatrix = VuUI::IF()->getCropMatrix();

	float screenWidth = 1.0f/(uiDrawParams.mInvAuthScale.mX*cropMatrix.mX.mX);

	float scrollMax = mTouchRect.mWidth + (getItemCount() - 1)*mSpacing;
	scrollMax -= screenWidth;

	int numItemsOnScreen = calcNumItemsOnScreen();
	float width = mTouchRect.mWidth + (numItemsOnScreen - 1)*mSpacing;
	scrollMax += (screenWidth - width);

	scrollMax = VuMax(scrollMax, calcScrollMin());

	return scrollMax;
}

//*****************************************************************************
void VuHListEntity::findItem(int index)
{
	int numItemsOnScreen = calcNumItemsOnScreen();

	int targetIndex = VuRound(mScrollTargetPos/mSpacing);

	if ( index < targetIndex )
		targetIndex = index;
	else if ( index >= targetIndex + numItemsOnScreen )
		targetIndex = index - numItemsOnScreen + 1;

	mScrollTargetPos = targetIndex*mSpacing;
}

//*****************************************************************************
VuTexture *VuHListEntity::getTexture(const std::string &assetName)
{
	VuTextureAsset *pTextureAsset = VUNULL;

	TextureAssets::iterator iter = mTextureAssets.find(assetName);
	if ( iter != mTextureAssets.end() )
	{
		pTextureAsset = iter->second;
	}
	else if ( VuAssetFactory::IF()->doesAssetExist<VuTextureAsset>(assetName) )
	{
		pTextureAsset = VuAssetFactory::IF()->createAsset<VuTextureAsset>(assetName);
		mTextureAssets[assetName] = pTextureAsset;
	}

	return pTextureAsset ? pTextureAsset->getTexture() : VUNULL;
}

//*****************************************************************************
VuHListEntity::Instance &VuHListEntity::getInstance(int index)
{
	Instances::iterator iter = mInstances.find(index);
	if ( iter != mInstances.end() )
		return iter->second;

	Instance &inst = mInstances[index];

	inst.mDelay = VuRand::global().range(0.0f, mMaxDelay);
	inst.mAngularFrequency = VuRand::global().range(mMinAngularFrequency, mMaxAngularFrequency);

	return inst;
}
