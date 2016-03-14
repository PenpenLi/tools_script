//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  power-up list entity
// 
//*****************************************************************************

#include "VuPurple/Components/VuMultiBoingTransitionComponent.h"
#include "VuPurple/PowerUps/VuPowerUpManager.h"
#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Managers/VuControlMethodManager.h"
#include "VuPurple/Gfx/VuGameGfxUtil.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Components/2dLayout/Vu2dLayoutComponent.h"
#include "VuEngine/Properties/VuFontEnumProperty.h"
#include "VuEngine/Assets/VuTextureAsset.h"
#include "VuEngine/Managers/VuProfileManager.h"
#include "VuEngine/UI/VuUI.h"
#include "VuEngine/UI/VuUIDrawUtil.h"
#include "VuEngine/UI/VuUIInputUtil.h"
#include "VuEngine/UI/VuUIPropertyUtil.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/Font/VuFontDraw.h"
#include "VuEngine/DB/VuStringDB.h"
#include "VuEngine/Math/VuMathUtil.h"
#include "VuEngine/Util/VuStringUtil.h"


class VuPowerUpListEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuPowerUpListEntity();
	~VuPowerUpListEntity();

	virtual void					onGameInitialize();
	virtual void					onGameRelease();

protected:
	// scripting
	VuRetVal						SetFocus(const VuParams &params = VuParams());

	// event handlers
	void							OnUITick(const VuParams &params);
	void							OnUIGamePad(const VuParams &params);
	void							OnUITouch(const VuParams &params);
	void							OnUIDraw(const VuParams &params);

	void							drawLayout(bool bSelected);
	void							drawItems(float scaleStart, float scaleTarget);

	VuVector2						calcOffset(int index);
	void							select(int index);
	void							choose();

	// components
	VuScriptComponent				*mpScriptComponent;
	VuMultiBoingTransitionComponent	*mpTransitionComponent;

	// properties
	int								mColumnCount;
	VuVector2						mSpacing;
	VuUIRectProperties				mTouchRect;
	VuUIRectProperties				mIconRect;
	VuUIImageProperties				mSelectedImage;
	VuUIImageProperties				mUnselectedImage;
	VuUIRectProperties				mImageRect;
	VuUIRectProperties				mPriceRect;
	std::string						mPriceFont;
	VuUIStringFormatProperties		mPriceStringFormat;
	std::string						mPriceStringID;
	VuUIImageProperties				mOwnedImage;
	VuUIRectProperties				mOwnedImageRect;
	VuUIImageProperties				mNotOwnedImage;
	VuUIRectProperties				mNotOwnedImageRect;
	VuColor							mNotOwnedColor;
	float							mMaxDelay;
	float							mMinAngularFrequency;
	float							mMaxAngularFrequency;
	float							mDampingRatio;

	bool							mNewTouch;
	bool							mTouchDown;
	bool							mTouchUp;
	VuVector2						mTouchPos;

	enum ePressedState { NOT_PRESSED, PRESSED };
	ePressedState					mPressedState;
	float							mPressedScale;

	int								mSelectedIndex;
	std::string						mPriceText;

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

IMPLEMENT_RTTI(VuPowerUpListEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuPowerUpListEntity);


//*****************************************************************************
VuPowerUpListEntity::VuPowerUpListEntity():
	mColumnCount(5),
	mSpacing(0,0),
	mNotOwnedColor(160,160,160,255),
	mMaxDelay(0.3f),
	mMinAngularFrequency(2.0f),
	mMaxAngularFrequency(3.0f),
	mDampingRatio(0.5f),
	mNewTouch(false),
	mTouchDown(false),
	mTouchUp(false),
	mPressedState(NOT_PRESSED),
	mSelectedIndex(0)
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this, 120));
	addComponent(new Vu2dLayoutComponent(this, &VuPowerUpListEntity::drawLayout));
	addComponent(mpTransitionComponent = new VuMultiBoingTransitionComponent(this));

	// properties
	addProperty(new VuIntProperty("Column Count", mColumnCount));
	addProperty(new VuVector2Property("Spacing", mSpacing));
	addProperty(new VuRectProperty("Touch Rect", mTouchRect));
	addProperty(new VuRectProperty("Icon Rect", mIconRect));
	mSelectedImage.addProperties(getProperties(), "Selected Image");
	mUnselectedImage.addProperties(getProperties(), "Unselected Image");
	addProperty(new VuRectProperty("Image Rect", mImageRect));
	addProperty(new VuRectProperty("Price Rect", mPriceRect));
	addProperty(new VuFontEnumProperty("Price Font", mPriceFont));
	ADD_UI_STRING_FORMAT_PROPERTIES(getProperties(), mPriceStringFormat, "Price ");
	addProperty(new VuStringProperty("Price String ID", mPriceStringID));
	mOwnedImage.addProperties(getProperties(), "Owned Image");
	addProperty(new VuRectProperty("Owned Image Rect", mOwnedImageRect));
	mNotOwnedImage.addProperties(getProperties(), "Not Owned Image");
	addProperty(new VuRectProperty("No Owned Image Rect", mNotOwnedImageRect));
	addProperty(new VuColorProperty("No Owned Color", mNotOwnedColor));
	addProperty(new VuFloatProperty("Max Delay", mMaxDelay));
	addProperty(new VuFloatProperty("Min Angular Frequency", mMinAngularFrequency));
	addProperty(new VuFloatProperty("Max Angular Frequency", mMaxAngularFrequency));
	addProperty(new VuFloatProperty("Damping Ratio", mDampingRatio));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuPowerUpListEntity, SetFocus);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, NavUp);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, NavDown);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, NavLeft);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, NavRight);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnChosenOwned);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnChosenNotOwned);

	// event handlers
	REG_EVENT_HANDLER(VuPowerUpListEntity, OnUITick);
	REG_EVENT_HANDLER(VuPowerUpListEntity, OnUIGamePad);
	REG_EVENT_HANDLER(VuPowerUpListEntity, OnUITouch);
	REG_EVENT_HANDLER(VuPowerUpListEntity, OnUIDraw);

	// constants
	mPressedScale = 1.0f - 0.01f*VuGameUtil::IF()->constantDB()["UI"]["PressedButtonSizeReduction"].asFloat();
}

//*****************************************************************************
VuPowerUpListEntity::~VuPowerUpListEntity()
{
}

//*****************************************************************************
void VuPowerUpListEntity::onGameInitialize()
{
	int index = 0;
	const std::string &powerUpName = VuProfileManager::IF()->dataRead()["Lists"]["PowerUp"].asString();
	for ( int i = 0; i < VuPowerUpManager::IF()->getPowerUpCount(); i++ )
		if ( powerUpName == VuPowerUpManager::IF()->getPowerUp(i)->mName )
			index = i;

	select(index);
}

//*****************************************************************************
void VuPowerUpListEntity::onGameRelease()
{
	const std::string &powerUpName = VuPowerUpManager::IF()->getPowerUp(mSelectedIndex)->mName;
	VuProfileManager::IF()->dataWrite()["Lists"]["PowerUp"].putValue(powerUpName);
}

//*****************************************************************************
VuRetVal VuPowerUpListEntity::SetFocus(const VuParams &params)
{
	VuUI::IF()->setFocus(this);

	return VuRetVal();
}

//*****************************************************************************
void VuPowerUpListEntity::OnUITick(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	float fdt = accessor.getFloat();

	mpTransitionComponent->tick(fdt);

	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	if ( mNewTouch )
	{
		for ( int index = 0; index < VuPowerUpManager::IF()->getPowerUpCount(); index++ )
		{
			VuRect touchRect = uiDrawParams.transform(mTouchRect + calcOffset(index));
			if ( touchRect.contains(mTouchPos) )
			{
				mPressedState = PRESSED;
				select(index);
				break;
			}
		}
	}

	if ( mPressedState == PRESSED )
	{
		if ( mTouchUp )
		{
			choose();
			mPressedState = NOT_PRESSED;
		}
		VuRect touchRect = uiDrawParams.transform(mTouchRect + calcOffset(mSelectedIndex));
		if ( !touchRect.contains(mTouchPos) )
		{
			mPressedState = NOT_PRESSED;
		}
	}

	if ( mTouchUp )
	{
		mTouchDown = false;
		mTouchUp = false;
	}
	mNewTouch = false;
}

//*****************************************************************************
void VuPowerUpListEntity::OnUIGamePad(const VuParams &params)
{
	if ( VuUI::IF()->getFocus() == this )
	{
		VuParams::VuAccessor accessor(params);
		int channel = accessor.getInt();
		bool down = accessor.getBool();
		int padIndex = accessor.getInt();

		if ( down )
		{
			if ( channel == VuUIInputUtil::CHANNEL_UP )
			{
				int newIndex = mSelectedIndex - mColumnCount;
				if ( newIndex >= 0 )
				{
					select(newIndex);
					VuGameUtil::IF()->playSfx(VuGameUtil::SFX_UI_NAVIGATE);
				}
				else if ( mpScriptComponent->getPlug("NavUp")->getNumConnections() )
				{
					mpScriptComponent->getPlug("NavUp")->execute();
					VuGameUtil::IF()->playSfx(VuGameUtil::SFX_UI_NAVIGATE);
				}
			}
			else if ( channel == VuUIInputUtil::CHANNEL_DOWN )
			{
				int newIndex = mSelectedIndex + mColumnCount;
				if ( newIndex < VuPowerUpManager::IF()->getPowerUpCount() )
				{
					select(newIndex);
					VuGameUtil::IF()->playSfx(VuGameUtil::SFX_UI_NAVIGATE);
				}
				else if ( mpScriptComponent->getPlug("NavDown")->getNumConnections() )
				{
					mpScriptComponent->getPlug("NavDown")->execute();
					VuGameUtil::IF()->playSfx(VuGameUtil::SFX_UI_NAVIGATE);
				}
			}
			else if ( channel == VuUIInputUtil::CHANNEL_LEFT )
			{
				int newIndex = mSelectedIndex - 1;
				if ( newIndex >= 0 )
				{
					select(mSelectedIndex - 1);
					VuGameUtil::IF()->playSfx(VuGameUtil::SFX_UI_NAVIGATE);
				}
				else if ( mpScriptComponent->getPlug("NavLeft")->getNumConnections() )
				{
					mpScriptComponent->getPlug("NavLeft")->execute();
					VuGameUtil::IF()->playSfx(VuGameUtil::SFX_UI_NAVIGATE);
				}
			}
			else if ( channel == VuUIInputUtil::CHANNEL_RIGHT )
			{
				int newIndex = mSelectedIndex + 1;
				if ( newIndex < VuPowerUpManager::IF()->getPowerUpCount() )
				{
					select(mSelectedIndex + 1);
					VuGameUtil::IF()->playSfx(VuGameUtil::SFX_UI_NAVIGATE);
				}
				else if ( mpScriptComponent->getPlug("NavRight")->getNumConnections() )
				{
					mpScriptComponent->getPlug("NavRight")->execute();
					VuGameUtil::IF()->playSfx(VuGameUtil::SFX_UI_NAVIGATE);
				}
			}
			else if ( channel == VuUIInputUtil::CHANNEL_SELECT )
			{
				choose();
			}
		}
	}
}

//*****************************************************************************
void VuPowerUpListEntity::OnUITouch(const VuParams &params)
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
void VuPowerUpListEntity::OnUIDraw(const VuParams &params)
{
	float scaleStart = 0.0f;
	float scaleTarget = 1.0f;
	if ( mpTransitionComponent->getState() == VuTransitionComponent::STATE_TRANS_OUT )
	{
		scaleStart = 1.0f;
		scaleTarget = 0.0f;
	}

	drawItems(scaleStart, scaleTarget);
}

//*****************************************************************************
void VuPowerUpListEntity::drawLayout(bool bSelected)
{
	if ( bSelected )
	{
		VuUIDrawParams uiDrawParams;
		VuUIDrawUtil::getParams(this, uiDrawParams);

		for ( int index = 0; index < VuPowerUpManager::IF()->getPowerUpCount(); index++ )
		{
			VuRect touchRect = uiDrawParams.transform(mTouchRect + calcOffset(index));
			VuGfxUtil::IF()->drawRectangleOutline2d(uiDrawParams.mDepth, VuColor(255,255,255), touchRect);
		}
	}

	drawItems(1.0f, 1.0f);
}

//*****************************************************************************
void VuPowerUpListEntity::drawItems(float scaleStart, float scaleTarget)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	bool hasFocus = VuUI::IF()->getFocus() == this;

	for ( int index = 0; index < VuPowerUpManager::IF()->getPowerUpCount(); index++ )
	{
		const VuPowerUp *pPowerUp = VuPowerUpManager::IF()->getPowerUp(index);

		Instance &inst = getInstance(index);

		// is the power-up owned?
		bool owned;
		if ( VuGameManager::IF () )
			owned = VuGameManager::IF()->isPowerUpOwned(pPowerUp->mName);
		else
			owned = index%3 ? true : false;

		// determine scale
		float scale = scaleStart, vel = 0.0f;
		float age = VuMax(mpTransitionComponent->getTransitionTimer() - inst.mDelay, 0.0f);
		VuMathUtil::calcDampedSimpleHarmonicMotion(&scale, &vel, scaleTarget, age, inst.mAngularFrequency*VU_2PI, mDampingRatio);
		if ( (index == mSelectedIndex) && (mPressedState == PRESSED) )
			scale *= mPressedScale;

		if ( scale < 0.0f )
			inst.mDone = true;
		if ( inst.mDone )
			continue;

		VuVector2 offset = calcOffset(index);
		float depth = uiDrawParams.mDepth;

		// draw background
		{
			VuRect dstRect = mImageRect;
			dstRect.scale(mImageRect.getCenter(), scale);
			dstRect = uiDrawParams.transform(dstRect + offset);

			if ( (index == mSelectedIndex) && hasFocus && VuControlMethodManager::IF()->isGamePadOrKeyboard() )
			{
				VuColor color = VuGameUtil::IF()->calcHighlightColor(1.0f);

				if ( owned )
					VuGfxUtil::IF()->drawTexture2d(depth, mSelectedImage.getTexture(), color, dstRect);
				else
					VuGameGfxUtil::IF()->drawGrayScaleTexture2d(depth, mSelectedImage.getTexture(), color, dstRect);
			}
			else
			{
				if ( owned )
					VuGfxUtil::IF()->drawTexture2d(depth, mUnselectedImage.getTexture(), dstRect);
				else
					VuGameGfxUtil::IF()->drawGrayScaleTexture2d(depth, mUnselectedImage.getTexture(), mNotOwnedColor, dstRect);
			}

			depth -= GFX_SORT_DEPTH_STEP;
		}

		// draw icon
		if ( VuTextureAsset *pTextureAsset = pPowerUp->mpIconTextureAsset )
		{
			VuRect dstRect = mIconRect;
			dstRect.scale(mImageRect.getCenter(), scale);
			dstRect = uiDrawParams.transform(dstRect + offset);

			if ( owned )
				VuGfxUtil::IF()->drawTexture2d(depth, pTextureAsset->getTexture(), dstRect);
			else
				VuGameGfxUtil::IF()->drawGrayScaleTexture2d(depth, pTextureAsset->getTexture(), mNotOwnedColor, dstRect);

			depth -= GFX_SORT_DEPTH_STEP;
		}

		// owned / not owned image
		if ( owned )
		{
			if ( VuTexture *pTexture = mOwnedImage.getTexture() )
			{
				VuRect dstRect = mOwnedImageRect;
				dstRect.scale(mImageRect.getCenter(), scale);
				dstRect = uiDrawParams.transform(dstRect + offset);

				VuGfxUtil::IF()->drawTexture2d(depth, pTexture, dstRect);

				depth -= GFX_SORT_DEPTH_STEP;
			}
		}
		else
		{
			if ( VuTexture *pTexture = mNotOwnedImage.getTexture() )
			{
				VuRect dstRect = mNotOwnedImageRect;
				dstRect.scale(mImageRect.getCenter(), scale);
				dstRect = uiDrawParams.transform(dstRect + offset);

				VuGfxUtil::IF()->drawTexture2d(depth, pTexture, dstRect);

				depth -= GFX_SORT_DEPTH_STEP;
			}
		}

		// draw price
		if ( !owned )
		{
			char strPrice[16];
			VuStringUtil::integerFormat(pPowerUp->mPrice, strPrice, sizeof(strPrice));

			mPriceText = VuStringDB::IF()->getString(mPriceStringID);
			VuStringUtil::replace(mPriceText, "[[POWER_UP_PRICE]]", strPrice);

			const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mPriceFont);

			VuFontDrawParams fdParams = fontEntry.params();
			fdParams.mSize *= scale;
			VuRect dstRect = mPriceRect;
			dstRect.scale(mImageRect.getCenter(), scale);
			dstRect = uiDrawParams.transform(dstRect + offset);

			VuGfxUtil::IF()->fontDraw()->drawString(depth, fontEntry.font(), mPriceText.c_str(), fdParams, dstRect, mPriceStringFormat);
	
			depth -= GFX_SORT_DEPTH_STEP;
		}
	}
}

//*****************************************************************************
VuVector2 VuPowerUpListEntity::calcOffset(int index)
{
	VuVector2 offset = mSpacing*VuVector2(index%mColumnCount, index/mColumnCount);

	return offset;
}

//*****************************************************************************
void VuPowerUpListEntity::select(int index)
{
	mSelectedIndex = index;

	const VuPowerUp *pPowerUp = VuPowerUpManager::IF()->getPowerUp(mSelectedIndex);
	VuGameUtil::IF()->dataWrite()["SelectedPowerUp"].putValue(pPowerUp->mName);
}

//*****************************************************************************
void VuPowerUpListEntity::choose()
{
	const VuPowerUp *pPowerUp = VuPowerUpManager::IF()->getPowerUp(mSelectedIndex);

	if ( VuGameManager::IF()->isPowerUpOwned(pPowerUp->mName) )
		mpScriptComponent->getPlug("OnChosenOwned")->execute();
	else
		mpScriptComponent->getPlug("OnChosenNotOwned")->execute();
}

//*****************************************************************************
VuPowerUpListEntity::Instance &VuPowerUpListEntity::getInstance(int index)
{
	Instances::iterator iter = mInstances.find(index);
	if ( iter != mInstances.end() )
		return iter->second;

	Instance &inst = mInstances[index];

	inst.mDelay = VuRand::global().range(0.0f, mMaxDelay);
	inst.mAngularFrequency = VuRand::global().range(mMinAngularFrequency, mMaxAngularFrequency);

	return inst;
}