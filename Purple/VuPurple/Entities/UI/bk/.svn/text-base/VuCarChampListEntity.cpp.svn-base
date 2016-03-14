//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Car List Entity
// 
//*****************************************************************************

#include "VuPurple/Components/VuMultiBoingTransitionComponent.h"
#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Managers/VuStatsManager.h"
#include "VuPurple/Managers/VuControlMethodManager.h"
#include "VuPurple/Gfx/VuGameGfxUtil.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuPurple/Util/VuGameFontMacros.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Components/2dLayout/Vu2dLayoutComponent.h"
#include "VuEngine/Properties/VuFontEnumProperty.h"
#include "VuEngine/Assets/VuTextureAsset.h"
#include "VuEngine/Managers/VuProfileManager.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/Font/VuFontDraw.h"
#include "VuEngine/DB/VuStringDB.h"
#include "VuEngine/UI/VuUI.h"
#include "VuEngine/UI/VuUIDrawUtil.h"
#include "VuEngine/UI/VuUIInputUtil.h"
#include "VuEngine/UI/VuUIPropertyUtil.h"
#include "VuEngine/Math/VuMathUtil.h"
#include "VuEngine/Util/VuStringUtil.h"


class VuCarChampListEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuCarChampListEntity();
	~VuCarChampListEntity();

	virtual void	onGameInitialize();
	virtual void	onGameRelease();

protected:
	// scripting
	VuRetVal					SetFocus(const VuParams &params = VuParams());
	VuRetVal					LevelUpCar(const VuParams &params);
	VuRetVal					ClearChamp(const VuParams &params);

	// event handlers
	void						OnUITick(const VuParams &params);
	void						OnUIGamePad(const VuParams &params);
	void						OnUITouch(const VuParams &params);
	void						OnUIDraw(const VuParams &params);

	void						drawLayout(bool bSelected);
	void						drawList(float scaleStart, float scaleTarget);

	bool						handleNewTouch();

	int							getItemCount() { return (int)mItems.size(); }
	int							getButtonCount(int iItem);
	void						setSelectedItem(int index, bool snap);

	bool						touched(const VuRect &rect);

	int							calcNumItemsOnScreen();
	VuVector2					calcPosition();
	VuVector2					calcItemOffset(int itemIndex);
	float						calcScrollMin();
	float						calcScrollMax();

	void						findItem(int index);
	bool						isInProgress(int index);
	bool						isStageAvailable(int iItem, int iStage);

	void						onButtonPressed(int iItem, int iButton);
	void						onBuyButtonPressed(int iItem);
	void						onQuitButtonPressed(int iItem);
	void						onContinueButtonPressed(int iItem);
	void						onStageButtonPressed(int iItem, int iStage);

	// components
	VuScriptComponent				*mpScriptComponent;
	VuMultiBoingTransitionComponent	*mpTransitionComponent;

	struct StageProperties
	{
		StageProperties() : mOffsetY(0.0f), mScale(1.0f) {}
		VuUIImageProperties	mImage;
		float				mOffsetY;
		float				mScale;
	};

	// properties
	float						mSpacing;
	float						mScrollLag;
	float						mScrollResidualDamping;
	float						mScrollThreshold;
	VuUIRectProperties			mTouchRect;
	VuUIRectProperties			mBackRect;
	VuUIImageProperties			mBackImage;
	VuUIImageProperties			mBackCompImage;
	VuUIRectProperties			mCarImageRect;
	VuUIRectProperties			mCarNameRect;
	std::string					mCarNameFont;
	VuUIStringFormatProperties	mCarNameStringFormat;
	VuUIRectProperties			mLockImageRect;
	VuUIImageProperties			mLockImage;
	std::string					mLockStringId;
	VuUIRectProperties			mLockTextRect;
	std::string					mLockTextFont;
	VuUIStringFormatProperties	mLockTextStringFormat;
	VuUIRectProperties			mProgressImageRect;
	std::string					mProgressStringId;
	VuUIRectProperties			mProgressTextRect;
	std::string					mProgressTextFont;
	VuUIStringFormatProperties	mProgressTextStringFormat;
	VuUIRectProperties			mStageButtonRect;
	VuUIImageProperties			mStageButtonImage;
	VuUIImageProperties			mStageButtonSelImage;
	VuUIRectProperties			mStarRect;
	VuUIImageProperties			mStarOffImage;
	VuUIImageProperties			mStarOnImage;
	float						mStarOffsetX;
	VuUIRectProperties			mStageIconRect;
	StageProperties				mStageProperties[4];
	VuUIRectProperties			mWinImageRect;
	VuUIImageProperties			mWinImage;
	std::string					mWinStringId;
	VuUIRectProperties			mWinTextRect;
	std::string					mWinTextFont;
	VuUIStringFormatProperties	mWinTextStringFormat;
	VuUIRectProperties			mCompletionTextRect;
	std::string					mCompletionTextFont;
	VuUIStringFormatProperties	mCompletionTextStringFormat;
	VuUIRectProperties			mBuyButtonRect;
	VuUIImageProperties			mBuyButtonImage;
	VuUIImageProperties			mBuyButtonSelImage;
	std::string					mBuyButtonStringId;
	std::string					mBuyButtonFont;
	VuUIRectProperties			mContinueButtonRect;
	VuUIImageProperties			mContinueButtonImage;
	VuUIImageProperties			mContinueButtonSelImage;
	std::string					mContinueButtonStringId;
	std::string					mContinueButtonFont;
	VuUIRectProperties			mQuitButtonRect;
	VuUIImageProperties			mQuitButtonImage;
	VuUIImageProperties			mQuitButtonSelImage;
	std::string					mQuitButtonStringId;
	std::string					mQuitButtonFont;
	VuUIRectProperties			mDotRect;
	VuVector2					mDotSpacing;
	VuUIImageProperties			mDotOnImage;
	VuUIImageProperties			mDotOffImage;
	float						mMaxDelay;
	float						mMinAngularFrequency;
	float						mMaxAngularFrequency;
	float						mDampingRatio;
	VuUIImageProperties			mGemImage;
	VuUIImageProperties			mGemMissingImage;
	VuUIRectProperties			mGemRect;

	struct Item
	{
		Item() : mIsOwned(false), mDone(false), mPaintColor(255,255,255), mIsGold(false) {}
		std::string		mCarName;
		bool			mIsOwned;
		float			mDelay;
		float			mAngularFrequency;
		bool			mDone;
		VuTextureAsset	*mpIconTexture;
		VuTextureAsset	*mpMaskTexture;
		VuColor			mPaintColor;
		bool			mIsGold;
	};
	std::vector<Item>			mItems;

	int							mSelectedItem;
	int							mSelectedButton;
	int							mPressedItem;
	int							mPressedButton;

	enum eState { WAITING, PRESSED, SCROLLING };
	eState						mState;
	float						mPressedScale;

	VuVector2					mInitialTouchDown;
	float						mScrollPos;
	float						mScrollTargetPos;
	float						mScrollVel;
	float						mScrollTouchPos;

	bool						mNewTouch;
	bool						mTouchDown;
	bool						mTouchUp;
	VuVector2					mTouchPos;

	std::string					mTempText;
};


IMPLEMENT_RTTI(VuCarChampListEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuCarChampListEntity);


//*****************************************************************************
VuCarChampListEntity::VuCarChampListEntity():
	mSpacing(540),
	mScrollLag(0.1f),
	mScrollResidualDamping(0.99f),
	mScrollThreshold(20),
	mStarOffsetX(0.0f),
	mDotSpacing(0,0),
	mMaxDelay(0.3f),
	mMinAngularFrequency(2.0f),
	mMaxAngularFrequency(3.0f),
	mDampingRatio(0.5f),
	mSelectedItem(0),
	mSelectedButton(0),
	mPressedItem(-1),
	mPressedButton(-1),
	mState(WAITING),
	mScrollPos(0),
	mScrollTargetPos(0),
	mScrollVel(0),
	mNewTouch(false),
	mTouchDown(false),
	mTouchUp(false)
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this, 120));
	addComponent(new Vu2dLayoutComponent(this, &VuCarChampListEntity::drawLayout));
	addComponent(mpTransitionComponent = new VuMultiBoingTransitionComponent(this));

	// properties
	addProperty(new VuFloatProperty("Spacing", mSpacing));
	addProperty(new VuFloatProperty("Scroll Lag", mScrollLag));
	addProperty(new VuFloatProperty("Scroll Residual Damping", mScrollResidualDamping));
	addProperty(new VuFloatProperty("Scroll Threshold", mScrollThreshold));
	addProperty(new VuRectProperty("Touch Rect", mTouchRect));
	addProperty(new VuRectProperty("Back Rect", mBackRect));
	mBackImage.addProperties(getProperties(), "Back Image");
	mBackCompImage.addProperties(getProperties(), "Back Comp Image");
	addProperty(new VuRectProperty("Car Image Rect", mCarImageRect));
	addProperty(new VuRectProperty("Car Name Rect", mCarNameRect));
	addProperty(new VuFontEnumProperty("Car Name Font", mCarNameFont));
	ADD_UI_STRING_FORMAT_PROPERTIES(getProperties(), mCarNameStringFormat, "Car Name ");
	addProperty(new VuRectProperty("Lock Image Rect", mLockImageRect));
	mLockImage.addProperties(getProperties(), "Lock Image");
	addProperty(new VuStringProperty("Lock String ID", mLockStringId));
	addProperty(new VuRectProperty("Lock Text Rect", mLockTextRect));
	addProperty(new VuFontEnumProperty("Lock Text Font", mLockTextFont));
	ADD_UI_STRING_FORMAT_PROPERTIES(getProperties(), mLockTextStringFormat, "Lock Text ");
	addProperty(new VuRectProperty("Progress Image Rect", mProgressImageRect));
	addProperty(new VuStringProperty("Progress String ID", mProgressStringId));
	addProperty(new VuRectProperty("Progress Text Rect", mProgressTextRect));
	addProperty(new VuFontEnumProperty("Progress Text Font", mProgressTextFont));
	ADD_UI_STRING_FORMAT_PROPERTIES(getProperties(), mProgressTextStringFormat, "Progress Text ");
	addProperty(new VuRectProperty("Stage Button Rect", mStageButtonRect));
	mStageButtonImage.addProperties(getProperties(), "Stage Button Image");
	mStageButtonSelImage.addProperties(getProperties(), "Stage Button Sel Image");
	addProperty(new VuRectProperty("Star Rect", mStarRect));
	mStarOffImage.addProperties(getProperties(), "Star Off Image");
	mStarOnImage.addProperties(getProperties(), "Star On Image");
	addProperty(new VuFloatProperty("Star Offset X", mStarOffsetX));
	addProperty(new VuRectProperty("Stage Icon Rect", mStageIconRect));
	mStageProperties[0].mImage.addProperties(getProperties(), "Stage 0 Image");
	addProperty(new VuFloatProperty("Stage 0 Offset Y", mStageProperties[0].mOffsetY));
	addProperty(new VuFloatProperty("Stage 0 Scale", mStageProperties[0].mScale));
	mStageProperties[1].mImage.addProperties(getProperties(), "Stage 1 Image");
	addProperty(new VuFloatProperty("Stage 1 Offset Y", mStageProperties[1].mOffsetY));
	addProperty(new VuFloatProperty("Stage 1 Scale", mStageProperties[1].mScale));
	mStageProperties[2].mImage.addProperties(getProperties(), "Stage 2 Image");
	addProperty(new VuFloatProperty("Stage 2 Offset Y", mStageProperties[2].mOffsetY));
	addProperty(new VuFloatProperty("Stage 2 Scale", mStageProperties[2].mScale));
	mStageProperties[3].mImage.addProperties(getProperties(), "Stage 3 Image");
	addProperty(new VuFloatProperty("Stage 3 Offset Y", mStageProperties[3].mOffsetY));
	addProperty(new VuFloatProperty("Stage 3 Scale", mStageProperties[3].mScale));
	addProperty(new VuRectProperty("Win Image Rect", mWinImageRect));
	mWinImage.addProperties(getProperties(), "Win Image");
	addProperty(new VuStringProperty("Win String ID", mWinStringId));
	addProperty(new VuRectProperty("Win Text Rect", mWinTextRect));
	addProperty(new VuFontEnumProperty("Win Text Font", mWinTextFont));
	ADD_UI_STRING_FORMAT_PROPERTIES(getProperties(), mWinTextStringFormat, "Win Text ");
	addProperty(new VuRectProperty("Completion Text Rect", mCompletionTextRect));
	addProperty(new VuFontEnumProperty("Completion Text Font", mCompletionTextFont));
	ADD_UI_STRING_FORMAT_PROPERTIES(getProperties(), mCompletionTextStringFormat, "Completion Text ");
	addProperty(new VuRectProperty("Buy Button Rect", mBuyButtonRect));
	mBuyButtonImage.addProperties(getProperties(), "Buy Button Image");
	mBuyButtonSelImage.addProperties(getProperties(), "Buy Button Sel Image");
	addProperty(new VuStringProperty("Buy Button String ID", mBuyButtonStringId));
	addProperty(new VuFontEnumProperty("Buy Button Font", mBuyButtonFont));
	addProperty(new VuRectProperty("Continue Button Rect", mContinueButtonRect));
	mContinueButtonImage.addProperties(getProperties(), "Continue Button Image");
	mContinueButtonSelImage.addProperties(getProperties(), "Continue Button Sel Image");
	addProperty(new VuStringProperty("Continue Button String ID", mContinueButtonStringId));
	addProperty(new VuFontEnumProperty("Continue Button Font", mContinueButtonFont));
	addProperty(new VuRectProperty("Quit Button Rect", mQuitButtonRect));
	mQuitButtonImage.addProperties(getProperties(), "Quit Button Image");
	mQuitButtonSelImage.addProperties(getProperties(), "Quit Button Sel Image");
	addProperty(new VuStringProperty("Quit Button String ID", mQuitButtonStringId));
	addProperty(new VuFontEnumProperty("Quit Button Font", mQuitButtonFont));
	addProperty(new VuRectProperty("Dot Rect", mDotRect));
	addProperty(new VuVector2Property("Dot Spacing", mDotSpacing));
	mDotOnImage.addProperties(getProperties(), "Dot On Image");
	mDotOffImage.addProperties(getProperties(), "Dot Off Image");
	addProperty(new VuFloatProperty("Max Delay", mMaxDelay));
	addProperty(new VuFloatProperty("Min Angular Frequency", mMinAngularFrequency));
	addProperty(new VuFloatProperty("Max Angular Frequency", mMaxAngularFrequency));
	addProperty(new VuFloatProperty("Damping Ratio", mDampingRatio));
	mGemImage.addProperties(getProperties(), "Gem Image");
	mGemMissingImage.addProperties(getProperties(), "Gem Missing Image");
	addProperty(new VuRectProperty("Gem Rect", mGemRect));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuCarChampListEntity, SetFocus);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuCarChampListEntity, LevelUpCar);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuCarChampListEntity, ClearChamp);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, NavUp);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, NavDown);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, PurchaseSuccess);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, PurchaseFailure);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, LevelUpSuccess);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, LevelUpFailure);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, StageLocked);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, NeedLevelUp);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, NeedUpgrade);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, CarChampChosen);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, QuitChamp);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, ContinueChamp);

	// event handlers
	REG_EVENT_HANDLER(VuCarChampListEntity, OnUITick);
	REG_EVENT_HANDLER(VuCarChampListEntity, OnUIGamePad);
	REG_EVENT_HANDLER(VuCarChampListEntity, OnUITouch);
	REG_EVENT_HANDLER(VuCarChampListEntity, OnUIDraw);

	// build item list
	const VuJsonContainer &cars = VuGameUtil::IF()->constantDB()["Names"]["Cars"];
	for ( int i = 0; i < cars.size(); i++ )
	{
		mItems.resize(mItems.size() + 1);
		Item &item = mItems.back();

		item.mCarName = cars[i].asString();
		item.mDelay = VuRand::global().range(0.0f, mMaxDelay);
		item.mAngularFrequency = VuRand::global().range(mMinAngularFrequency, mMaxAngularFrequency);

		if ( VuGameManager::IF() )
		{
			const VuGameManager::Car &car = VuGameManager::IF()->getCar(item.mCarName);

			item.mIsOwned = car.mIsOwned;
			item.mIsGold = car.mIsGold;

			const VuJsonContainer &colorData = VuDataUtil::findArrayMember(VuGameUtil::IF()->colorDB(), "Name", VuGameManager::IF()->getCar(item.mCarName).mPaintColor);
			VuDataUtil::getValue(colorData["Color"], item.mPaintColor);
		}

		if ( item.mIsGold )
		{
			item.mpIconTexture = VuAssetFactory::IF()->createAsset<VuTextureAsset>("UI/Icon/Car_" + item.mCarName + "_gold", VuAssetFactory::OPTIONAL_ASSET);
			item.mpMaskTexture = VUNULL;
		}
		else
		{
			item.mpIconTexture = VuAssetFactory::IF()->createAsset<VuTextureAsset>("UI/Icon/Car_" + item.mCarName, VuAssetFactory::OPTIONAL_ASSET);
			item.mpMaskTexture = VuAssetFactory::IF()->createAsset<VuTextureAsset>("UI/Icon/Car_" + item.mCarName + "_m", VuAssetFactory::OPTIONAL_ASSET);
		}
	}

	// constants
	mPressedScale = 1.0f - 0.01f*VuGameUtil::IF()->constantDB()["UI"]["PressedButtonSizeReduction"].asFloat();
}

//*****************************************************************************
VuCarChampListEntity::~VuCarChampListEntity()
{
	for ( const auto &item : mItems )
	{
		VuAssetFactory::IF()->releaseAsset(item.mpIconTexture);
		VuAssetFactory::IF()->releaseAsset(item.mpMaskTexture);
	}
}

//*****************************************************************************
void VuCarChampListEntity::onGameInitialize()
{
	float scrollPos = VuProfileManager::IF()->dataRead()["Lists"]["CarChampScrollPos"].asFloat();
	scrollPos = VuClamp(scrollPos, calcScrollMin(), calcScrollMax());
	mScrollPos = mScrollTargetPos = scrollPos;

	const std::string &carName = VuProfileManager::IF()->dataRead()["Lists"]["CarChampCarName"].asString();
	for ( int i = 0; i < (int)mItems.size(); i++ )
		if ( carName == mItems[i].mCarName )
			setSelectedItem(i, true);
}

//*****************************************************************************
void VuCarChampListEntity::onGameRelease()
{
	const std::string &carName = mItems[mSelectedItem].mCarName;
	VuProfileManager::IF()->dataWrite()["Lists"]["CarChampCarName"].putValue(carName);
	VuProfileManager::IF()->dataWrite()["Lists"]["CarChampScrollPos"].putValue(mScrollTargetPos);
}

//*****************************************************************************
VuRetVal VuCarChampListEntity::SetFocus(const VuParams &params)
{
	VuUI::IF()->setFocus(this);

	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuCarChampListEntity::LevelUpCar(const VuParams &params)
{
	Item &item = mItems[mSelectedItem];

	int newStage = VuGameManager::IF()->getCar(item.mCarName).mStage;
	if ( VuGameManager::IF()->purchaseCarLevelUp(item.mCarName, newStage + 1) )
	{
		mpScriptComponent->getPlug("LevelUpSuccess")->execute();
	}
	else
	{
		mpScriptComponent->getPlug("LevelUpFailure")->execute();
	}

	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuCarChampListEntity::ClearChamp(const VuParams &params)
{
	Item &item = mItems[mSelectedItem];

	VuJsonContainer &carChampData = VuProfileManager::IF()->dataWrite()["CarChampData"][item.mCarName];
	carChampData.clear();

	return VuRetVal();
}

//*****************************************************************************
void VuCarChampListEntity::OnUITick(const VuParams &params)
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
				onButtonPressed(mPressedItem, mPressedButton);
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
		mPressedButton = -1;
	}

	if ( mTouchUp )
	{
		mTouchDown = false;
		mTouchUp = false;
	}
	mNewTouch = false;
}

//*****************************************************************************
void VuCarChampListEntity::OnUIGamePad(const VuParams &params)
{
	if ( VuUI::IF()->getFocus() == this )
	{
		VuParams::VuAccessor accessor(params);
		int channel = accessor.getInt();
		bool down = accessor.getBool();
		int padIndex = accessor.getInt();

		if ( down )
		{
			int maxItemIndex = getItemCount() - 1;
			int maxButtonIndex = getButtonCount(mSelectedItem) - 1;

			if ( channel == VuUIInputUtil::CHANNEL_UP )
			{
				if ( mSelectedButton < maxButtonIndex )
				{
					mSelectedButton++;
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
				if ( mSelectedButton > 0 )
				{
					mSelectedButton--;
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
				if ( maxItemIndex > 0 )
				{
					int prevItem = mSelectedItem - 1;
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
				if ( maxItemIndex > 0 )
				{
					int nextItem = mSelectedItem + 1;
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
				onButtonPressed(mSelectedItem, mSelectedButton);
			}
		}
	}
}

//*****************************************************************************
void VuCarChampListEntity::OnUITouch(const VuParams &params)
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
void VuCarChampListEntity::OnUIDraw(const VuParams &params)
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
void VuCarChampListEntity::drawLayout(bool bSelected)
{
	if ( bSelected )
	{
		VuUIDrawParams uiDrawParams;
		VuUIDrawUtil::getParams(this, uiDrawParams);

		VuVector2 position = calcPosition();

		VuGfxUtil::IF()->drawRectangleOutline2d(uiDrawParams.mDepth, VuColor(255,255,255), (mTouchRect + position)*uiDrawParams.mInvAuthScale);

		// name
		{
			const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mCarNameFont);
			VuGfxUtil::IF()->drawRectangleOutline2d(uiDrawParams.mDepth, fontEntry.params().mColor, (mCarNameRect + position)*uiDrawParams.mInvAuthScale);
		}
	
		// lock text
		{
			const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mLockTextFont);
			VuGfxUtil::IF()->drawRectangleOutline2d(uiDrawParams.mDepth, fontEntry.params().mColor, (mLockTextRect + position)*uiDrawParams.mInvAuthScale);
		}

		// progress text
		{
			const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mProgressTextFont);
			VuGfxUtil::IF()->drawRectangleOutline2d(uiDrawParams.mDepth, fontEntry.params().mColor, (mProgressTextRect + position)*uiDrawParams.mInvAuthScale);
		}

		// win text
		{
			const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mWinTextFont);
			VuGfxUtil::IF()->drawRectangleOutline2d(uiDrawParams.mDepth, fontEntry.params().mColor, (mWinTextRect + position)*uiDrawParams.mInvAuthScale);
		}

		// completion text
		{
			const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mCompletionTextFont);
			VuGfxUtil::IF()->drawRectangleOutline2d(uiDrawParams.mDepth, fontEntry.params().mColor, (mCompletionTextRect + position)*uiDrawParams.mInvAuthScale);
		}
	}

	drawList(1.0f, 1.0f);
}

//*****************************************************************************
void VuCarChampListEntity::drawList(float scaleStart, float scaleTarget)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	bool hasFocus = VuUI::IF()->getFocus() == this;

	VuVector2 position = calcPosition();

	int itemCount = getItemCount();
	for ( int iItem = 0; iItem < itemCount; iItem++ )
	{
		Item &item = mItems[iItem];
		VuVector2 offset = position + calcItemOffset(iItem);
		float depth = uiDrawParams.mDepth;

		// determine scale
		float scale = scaleStart, vel = 0.0f;
		float age = VuMax(mpTransitionComponent->getTransitionTimer() - item.mDelay, 0.0f);
		VuMathUtil::calcDampedSimpleHarmonicMotion(&scale, &vel, scaleTarget, age, item.mAngularFrequency*VU_2PI, mDampingRatio);

		if ( scale < 0.0f )
			item.mDone = true;
		if ( item.mDone )
			continue;

		// calculation completion
		int completionPercentage = 0;
		if ( VuStatsManager::IF() )
		{
			int totalStars = 0;
			for ( int i = 0; i < 4; i++ )
			{
				int bestPlace;
				if ( VuStatsManager::IF()->getCarChampBestPlace(item.mCarName.c_str(), i, bestPlace) )
					totalStars += VuMax(4 - bestPlace, 0);
			}
			completionPercentage = 100*totalStars/12;
		}

		// selected?
		bool selected = false;
		if ( hasFocus && (iItem == mSelectedItem) && VuControlMethodManager::IF()->isGamePadOrKeyboard() )
			selected = true;

		// draw background
		{
			VuColor color(255,255,255);

			VuRect rect = mBackRect;
			rect.scale(mTouchRect.getCenter(), scale);

			VuTexture *pTexture = mBackImage.getTexture();
			if ( completionPercentage == 100 )
				pTexture = mBackCompImage.getTexture();

			if ( pTexture )
			{
				if ( item.mIsOwned )
					VuGfxUtil::IF()->drawTexture2d(depth, pTexture, color, (rect + offset)*uiDrawParams.mInvAuthScale);
				else
					VuGameGfxUtil::IF()->drawGrayScaleTexture2d(depth, pTexture, color, (rect + offset)*uiDrawParams.mInvAuthScale);
			}

			depth -= GFX_SORT_DEPTH_STEP;
		}

		// draw car image
		if ( item.mpIconTexture && item.mpMaskTexture )
		{
			VuGameGfxUtil::CarIconDrawParams params;

			params.mDepth = depth;
			params.mpIconTexture = item.mpIconTexture->getTexture();
			params.mpMaskTexture = item.mpMaskTexture->getTexture();
			params.mDstRect = mCarImageRect;
			params.mSrcRect = VuRect(0,0,1,1);
			params.mColor = VuColor(255,255,255);
			params.mPaintColor = item.mPaintColor;

			params.mDstRect.scale(mTouchRect.getCenter(), scale);
			params.mDstRect = (params.mDstRect + offset)*uiDrawParams.mInvAuthScale;

			VuGameGfxUtil::IF()->drawCarIcon(params);

			depth -= GFX_SORT_DEPTH_STEP;
		}
		else if ( item.mpIconTexture )
		{
			VuRect rect = mCarImageRect;
			rect.scale(mTouchRect.getCenter(), scale);
			VuGfxUtil::IF()->drawTexture2d(depth, item.mpIconTexture->getTexture(), (rect + offset)*uiDrawParams.mInvAuthScale);

			depth -= GFX_SORT_DEPTH_STEP;
		}

		// draw car name text
		{
			char str[64];
			VU_SPRINTF(str, sizeof(str), "Car_%s", item.mCarName.c_str());
			const char *text = VuStringDB::IF()->getString(str).c_str();

			const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mCarNameFont);
			VuFontDrawParams fdParams = fontEntry.params();

			VuRect dstRect = mCarNameRect;
			dstRect.scale(mTouchRect.getCenter(), scale);
			dstRect = (dstRect + offset)*uiDrawParams.mInvAuthScale;
			fdParams.mSize *= scale;

			if ((mCarNameStringFormat.mShrinkToFit) && dstRect.mWidth > 0.0f)
			{
				VuUIDrawUtil::shrinkToFit(str, uiDrawParams, fontEntry, fdParams, dstRect);
			}

			VuGfxUtil::IF()->fontDraw()->drawString(depth, fontEntry.font(), text, fdParams, dstRect, mCarNameStringFormat);

			depth -= GFX_SORT_DEPTH_STEP;
		}

		if ( !item.mIsOwned )
		{
			// draw lock image
			if ( VuTexture *pTexture = mLockImage.getTexture() )
			{
				VuRect rect = mLockImageRect;
				rect.scale(mTouchRect.getCenter(), scale);

				VuGfxUtil::IF()->drawTexture2d(depth, pTexture, (rect + offset)*uiDrawParams.mInvAuthScale);

				depth -= GFX_SORT_DEPTH_STEP;
			}

			// draw unlock text
			{
				const char *text = VuStringDB::IF()->getString(mLockStringId).c_str();

				const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mLockTextFont);
				VuFontDrawParams fdParams = fontEntry.params();

				VuRect dstRect = mLockTextRect;
				dstRect.scale(mTouchRect.getCenter(), scale);
				dstRect = (dstRect + offset)*uiDrawParams.mInvAuthScale;
				fdParams.mSize *= scale;

				if ((mLockTextStringFormat.mShrinkToFit) && dstRect.mWidth > 0.0f)
				{
					VuUIDrawUtil::shrinkToFit(text, uiDrawParams, fontEntry, fdParams, dstRect);
				}

				VuGfxUtil::IF()->fontDraw()->drawString(depth, fontEntry.font(), text, fdParams, dstRect, mLockTextStringFormat);

				depth -= GFX_SORT_DEPTH_STEP;
			}

			// draw buy button image
			{
				VuColor color(255,255,255);

				VuTexture *pImage = mBuyButtonImage.getTexture();
				if ( selected && (0 == mSelectedButton) )
				{
					pImage = mBuyButtonSelImage.getTexture();
					color = VuGameUtil::IF()->calcHighlightColor(1.0f);
				}
			
				if ( pImage )
				{
					VuRect rect = mBuyButtonRect;

					if ( mState == PRESSED && iItem == mPressedItem && (0 == mPressedButton) )
						rect.scale(mBuyButtonRect.getCenter(), mPressedScale);

					rect.scale(mTouchRect.getCenter(), scale);

					VuGfxUtil::IF()->drawTexture2d(depth, pImage, color, (rect + offset)*uiDrawParams.mInvAuthScale);

					depth -= GFX_SORT_DEPTH_STEP;
				}
			}

			// draw buy button text
			{
				char strPrice[16];
				VuStringUtil::integerFormat(VuGameUtil::IF()->getCarPrice(item.mCarName), strPrice, sizeof(strPrice));
				mTempText = VuStringDB::IF()->getString(mBuyButtonStringId).c_str();
				VuStringUtil::replace(mTempText, "[PRICE]", strPrice);

				const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mBuyButtonFont);
				VuFontDrawParams fdParams = fontEntry.params();

				VuRect dstRect = mBuyButtonRect;
				if ( mState == PRESSED && iItem == mPressedItem && (0 == mPressedButton) )
				{
					fdParams.mSize *= mPressedScale;
					dstRect.scale(mBuyButtonRect.getCenter(), mPressedScale);
				}
				dstRect.scale(mTouchRect.getCenter(), scale);
				dstRect = (dstRect + offset)*uiDrawParams.mInvAuthScale;
				fdParams.mSize *= scale;

				VuGfxUtil::IF()->fontDraw()->drawString(depth, fontEntry.font(), mTempText.c_str(), fdParams, dstRect, VUGFX_TEXT_DRAW_HCENTER|VUGFX_TEXT_DRAW_VCENTER);

				depth -= GFX_SORT_DEPTH_STEP;
			}
		}
		else if ( isInProgress(iItem) )
		{
			// draw in-progress image
			{
				const VuJsonContainer &carChampData = VuProfileManager::IF()->dataRead()["CarChampData"][item.mCarName];
				int stage = carChampData["Stage"].asInt();

				VuRect rect = mProgressImageRect;
				rect.scale(mTouchRect.getCenter(), scale);

				VuGfxUtil::IF()->drawTexture2d(depth, mStageProperties[stage].mImage.getTexture(), (rect + offset)*uiDrawParams.mInvAuthScale);

				depth -= GFX_SORT_DEPTH_STEP;
			}

			// draw in-progress text
			{
				const char *text = VuStringDB::IF()->getString(mProgressStringId).c_str();

				const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mProgressTextFont);
				VuFontDrawParams fdParams = fontEntry.params();

				VuRect dstRect = mProgressTextRect;
				dstRect.scale(mTouchRect.getCenter(), scale);
				dstRect = (dstRect + offset)*uiDrawParams.mInvAuthScale;
				fdParams.mSize *= scale;

				if ((mProgressTextStringFormat.mShrinkToFit) && dstRect.mWidth > 0.0f)
				{
					VuUIDrawUtil::shrinkToFit(text, uiDrawParams, fontEntry, fdParams, dstRect);
				}

				VuGfxUtil::IF()->fontDraw()->drawString(depth, fontEntry.font(), text, fdParams, dstRect, mProgressTextStringFormat);

				depth -= GFX_SORT_DEPTH_STEP;
			}

			// draw continue button image
			{
				VuColor color(255,255,255);

				VuTexture *pImage = mContinueButtonImage.getTexture();
				if ( selected && (1 == mSelectedButton) )
				{
					pImage = mContinueButtonSelImage.getTexture();
					color = VuGameUtil::IF()->calcHighlightColor(1.0f);
				}
			
				if ( pImage )
				{
					VuRect rect = mContinueButtonRect;

					if ( mState == PRESSED && iItem == mPressedItem && (1 == mPressedButton) )
						rect.scale(mContinueButtonRect.getCenter(), mPressedScale);

					rect.scale(mTouchRect.getCenter(), scale);

					VuGfxUtil::IF()->drawTexture2d(depth, pImage, color, (rect + offset)*uiDrawParams.mInvAuthScale);

					depth -= GFX_SORT_DEPTH_STEP;
				}
			}

			// draw continue button text
			{
				char strPrice[16];
				VuStringUtil::integerFormat(VuGameUtil::IF()->getCarPrice(item.mCarName), strPrice, sizeof(strPrice));
				mTempText = VuStringDB::IF()->getString(mContinueButtonStringId).c_str();
				VuStringUtil::replace(mTempText, "[PRICE]", strPrice);

				const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mContinueButtonFont);
				VuFontDrawParams fdParams = fontEntry.params();

				VuRect dstRect = mContinueButtonRect;
				if ( mState == PRESSED && iItem == mPressedItem && (1 == mPressedButton) )
				{
					fdParams.mSize *= mPressedScale;
					dstRect.scale(mContinueButtonRect.getCenter(), mPressedScale);
				}
				dstRect.scale(mTouchRect.getCenter(), scale);
				dstRect = (dstRect + offset)*uiDrawParams.mInvAuthScale;
				fdParams.mSize *= scale;

				VuGfxUtil::IF()->fontDraw()->drawString(depth, fontEntry.font(), mTempText.c_str(), fdParams, dstRect, VUGFX_TEXT_DRAW_HCENTER|VUGFX_TEXT_DRAW_VCENTER);

				depth -= GFX_SORT_DEPTH_STEP;
			}

			// draw quit button image
			{
				VuColor color(255,255,255);

				VuTexture *pImage = mQuitButtonImage.getTexture();
				if ( selected && (0 == mSelectedButton) )
				{
					pImage = mQuitButtonSelImage.getTexture();
					color = VuGameUtil::IF()->calcHighlightColor(1.0f);
				}
			
				if ( pImage )
				{
					VuRect rect = mQuitButtonRect;

					if ( mState == PRESSED && iItem == mPressedItem && (0 == mPressedButton) )
						rect.scale(mQuitButtonRect.getCenter(), mPressedScale);

					rect.scale(mTouchRect.getCenter(), scale);

					VuGfxUtil::IF()->drawTexture2d(depth, pImage, color, (rect + offset)*uiDrawParams.mInvAuthScale);

					depth -= GFX_SORT_DEPTH_STEP;
				}
			}

			// draw quit button text
			{
				char strPrice[16];
				VuStringUtil::integerFormat(VuGameUtil::IF()->getCarPrice(item.mCarName), strPrice, sizeof(strPrice));
				mTempText = VuStringDB::IF()->getString(mQuitButtonStringId).c_str();
				VuStringUtil::replace(mTempText, "[PRICE]", strPrice);

				const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mQuitButtonFont);
				VuFontDrawParams fdParams = fontEntry.params();

				VuRect dstRect = mQuitButtonRect;
				if ( mState == PRESSED && iItem == mPressedItem && (0 == mPressedButton) )
				{
					fdParams.mSize *= mPressedScale;
					dstRect.scale(mQuitButtonRect.getCenter(), mPressedScale);
				}
				dstRect.scale(mTouchRect.getCenter(), scale);
				dstRect = (dstRect + offset)*uiDrawParams.mInvAuthScale;
				fdParams.mSize *= scale;

				VuGfxUtil::IF()->fontDraw()->drawString(depth, fontEntry.font(), mTempText.c_str(), fdParams, dstRect, VUGFX_TEXT_DRAW_HCENTER|VUGFX_TEXT_DRAW_VCENTER);

				depth -= GFX_SORT_DEPTH_STEP;
			}
		}
		else
		{
			if ( completionPercentage == 100 )
			{
				// draw win image
				if ( VuTexture *pTexture = mWinImage.getTexture() )
				{
					VuRect rect = mWinImageRect;
					rect.scale(mTouchRect.getCenter(), scale);

					VuGfxUtil::IF()->drawTexture2d(depth, pTexture, (rect + offset)*uiDrawParams.mInvAuthScale);

					depth -= GFX_SORT_DEPTH_STEP;
				}

				// draw win text
				{
					const char *text = VuStringDB::IF()->getString(mWinStringId).c_str();

					const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mWinTextFont);
					VuFontDrawParams fdParams = fontEntry.params();

					VuRect dstRect = mWinTextRect;
					dstRect.scale(mTouchRect.getCenter(), scale);
					dstRect = (dstRect + offset)*uiDrawParams.mInvAuthScale;
					fdParams.mSize *= scale;

					if ((mWinTextStringFormat.mShrinkToFit) && dstRect.mWidth > 0.0f)
					{
						VuUIDrawUtil::shrinkToFit(text, uiDrawParams, fontEntry, fdParams, dstRect);
					}

					VuGfxUtil::IF()->fontDraw()->drawString(depth, fontEntry.font(), text, fdParams, dstRect, mWinTextStringFormat);

					depth -= GFX_SORT_DEPTH_STEP;
				}
			}
			else
			{
				// draw completion % text
				char text[32];
				VU_SPRINTF(text, sizeof(text), "%d%%", completionPercentage);

				const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mCompletionTextFont);
				VuFontDrawParams fdParams = fontEntry.params();

				VuRect dstRect = mCompletionTextRect;
				dstRect.scale(mTouchRect.getCenter(), scale);
				dstRect = (dstRect + offset)*uiDrawParams.mInvAuthScale;
				fdParams.mSize *= scale;

				if ((mCompletionTextStringFormat.mShrinkToFit) && dstRect.mWidth > 0.0f)
				{
					VuUIDrawUtil::shrinkToFit(text, uiDrawParams, fontEntry, fdParams, dstRect);
				}

				VuGfxUtil::IF()->fontDraw()->drawString(depth, fontEntry.font(), text, fdParams, dstRect, mCompletionTextStringFormat);

				depth -= GFX_SORT_DEPTH_STEP;
			}

			// stages
			for ( int iStage = 0; iStage < 4; iStage++ )
			{
				// available?
				bool available = isStageAvailable(iItem, iStage);

				// num stars
				int numStars = 0;
				if ( VuStatsManager::IF() )
				{
					int bestPlace;
					if ( VuStatsManager::IF()->getCarChampBestPlace(item.mCarName.c_str(), iStage, bestPlace) )
						numStars = VuMax(4 - bestPlace, 0);
				}

				// draw stage button
				{
					VuColor color(255,255,255);

					VuTexture *pImage = mStageButtonImage.getTexture();
					if ( selected && (iStage == mSelectedButton) )
					{
						pImage = mStageButtonSelImage.getTexture();
						color = VuGameUtil::IF()->calcHighlightColor(1.0f);
					}

					if ( pImage )
					{
						VuRect rect = mStageButtonRect;

						if ( mState == PRESSED && iItem == mPressedItem && iStage == mPressedButton )
							rect.scale(mStageButtonRect.getCenter(), mPressedScale);

						rect.scale(mStageButtonRect.getCenter(), mStageProperties[iStage].mScale);
						rect.mY += mStageProperties[iStage].mOffsetY;

						rect.scale(mTouchRect.getCenter(), scale);
						rect = (rect + offset)*uiDrawParams.mInvAuthScale;

						if ( available )
							VuGfxUtil::IF()->drawTexture2d(depth, pImage, color, rect);
						else
							VuGameGfxUtil::IF()->drawGrayScaleTexture2d(depth, pImage, color, rect);
					}

					depth -= GFX_SORT_DEPTH_STEP;
				}

				// draw stage icon
				{
					VuRect rect = mStageIconRect;

					if ( mState == PRESSED && iItem == mPressedItem && iStage == mPressedButton)
						rect.scale(mStageButtonRect.getCenter(), mPressedScale);

					rect.scale(mStageButtonRect.getCenter(), mStageProperties[iStage].mScale);
					rect.mY += mStageProperties[iStage].mOffsetY;

					rect.scale(mTouchRect.getCenter(), scale);
					rect = (rect + offset)*uiDrawParams.mInvAuthScale;

					VuGfxUtil::IF()->drawTexture2d(depth, mStageProperties[iStage].mImage.getTexture(), rect);

					depth -= GFX_SORT_DEPTH_STEP;
				}

				// draw stars
				for ( int iStar = 0; iStar < 3; iStar++ )
				{
					VuTexture *pImage = numStars > iStar ? mStarOnImage.getTexture() : mStarOffImage.getTexture();

					if ( pImage )
					{
						VuRect rect = mStarRect;
						rect.mX += iStar*mStarOffsetX;

						if ( mState == PRESSED && iItem == mPressedItem && iStage == mPressedButton)
							rect.scale(mStageButtonRect.getCenter(), mPressedScale);

						rect.scale(mStageButtonRect.getCenter(), mStageProperties[iStage].mScale);
						rect.mY += mStageProperties[iStage].mOffsetY;

						rect.scale(mTouchRect.getCenter(), scale);
						rect = (rect + offset)*uiDrawParams.mInvAuthScale;

						if ( available )
							VuGfxUtil::IF()->drawTexture2d(depth, pImage, rect);
						else
							VuGameGfxUtil::IF()->drawGrayScaleTexture2d(depth, pImage, VuColor(255,255,255), rect);

						depth -= GFX_SORT_DEPTH_STEP;
					}
				}

				// draw gem
				if ( !VuGameUtil::IF()->isPaidMode() )
				{
					VuTexture *pImage = mGemMissingImage.getTexture();
					if ( VuStatsManager::IF() )
					{
						int bestPlace = 0;
						VuStatsManager::IF()->getCarChampBestPlace(item.mCarName.c_str(), iStage, bestPlace);
						if ( bestPlace == 1 )
							pImage = mGemImage.getTexture();
					}

					if ( pImage )
					{
						VuRect rect = mGemRect;

						if ( mState == PRESSED && iItem == mPressedItem && iStage == mPressedButton)
							rect.scale(mStageButtonRect.getCenter(), mPressedScale);

						rect.scale(mStageButtonRect.getCenter(), mStageProperties[iStage].mScale);
						rect.mY += mStageProperties[iStage].mOffsetY;

						rect.scale(mTouchRect.getCenter(), scale);
						rect = (rect + offset)*uiDrawParams.mInvAuthScale;

						if ( available )
							VuGfxUtil::IF()->drawTexture2d(depth, pImage, rect);
						else
							VuGameGfxUtil::IF()->drawGrayScaleTexture2d(depth, pImage, VuColor(255,255,255), rect);
					}
					depth -= GFX_SORT_DEPTH_STEP;
				}
			}
		}

		// draw dot
		if ( itemCount >= 2 && mDotSpacing.magSquared() > FLT_EPSILON )
		{
			VuRect dstRect = mDotRect + (iItem - 0.5f*(itemCount - 1))*mDotSpacing;
			dstRect.scale(dstRect.getCenter(), scale);
			dstRect = uiDrawParams.transform(dstRect);

			VuVector2 imageCenter = (mBackRect.getCenter() + offset)*uiDrawParams.mInvAuthScale;
			imageCenter = VuGfxUtil::IF()->getMatrix().transform(imageCenter);
			if ( imageCenter.mX >= 0.0f && imageCenter.mX <= 1.0f )
				VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth, mDotOnImage.getTexture(), dstRect);
			else
				VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth, mDotOffImage.getTexture(), dstRect);
		}
	}
}

//*****************************************************************************
bool VuCarChampListEntity::handleNewTouch()
{
	int itemCount = getItemCount();
	VuRect overallTouchRect = VuRect::bounds(mTouchRect + calcItemOffset(0), mTouchRect + calcItemOffset(itemCount - 1));

	if ( touched(overallTouchRect) )
	{
		mInitialTouchDown = mTouchPos;
		mScrollTouchPos = mScrollTargetPos;

		for ( int iItem = 0; iItem < itemCount; iItem++ )
		{
			Item &item = mItems[iItem];
			VuVector2 itemOffset = calcItemOffset(iItem);

			if ( !item.mIsOwned )
			{
				if ( touched(mBuyButtonRect + itemOffset) )
				{
					mState = PRESSED;
					mPressedItem = iItem;
					mPressedButton = 0;
					return true;
				}
			}
			else if ( isInProgress(iItem) )
			{
				if ( touched(mContinueButtonRect + itemOffset) )
				{
					mState = PRESSED;
					mPressedItem = iItem;
					mPressedButton = 1;
					return true;
				}
				else if ( touched(mQuitButtonRect + itemOffset) )
				{
					mState = PRESSED;
					mPressedItem = iItem;
					mPressedButton = 0;
					return true;
				}
			}
			else
			{
				for ( int iStage = 0; iStage < 4; iStage++ )
				{
					VuRect rect = mStageButtonRect;

					rect.scale(mStageButtonRect.getCenter(), mStageProperties[iStage].mScale);
					rect.mY += mStageProperties[iStage].mOffsetY;

					if ( touched(rect + itemOffset) )
					{
						mState = PRESSED;
						mPressedItem = iItem;
						mPressedButton = iStage;
						return true;
					}
				}
			}

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
int VuCarChampListEntity::getButtonCount(int iItem)
{
	if ( !mItems[iItem].mIsOwned )
		return 1;

	if ( isInProgress(iItem) )
		return 2;

	return 4;
}

//*****************************************************************************
void VuCarChampListEntity::setSelectedItem(int index, bool snap)
{
	mSelectedItem = index;

	findItem(index);

	if ( snap  )
	{
		mScrollPos = mScrollTargetPos;
		mScrollVel = 0.0f;
	}

	// select appropriate button
	if ( !mItems[index].mIsOwned )
	{
		mSelectedButton = 0;
	}
	else if ( isInProgress(index) )
	{
		mSelectedButton = 1;
	}
	else
	{
		mSelectedButton = 0;
		for ( int iStage = 1; iStage < 4; iStage++ )
			if ( isStageAvailable(index, iStage) )
				mSelectedButton = iStage;
	}
}

//*****************************************************************************
bool VuCarChampListEntity::touched(const VuRect &rect)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	VuRect touchRect = (rect + calcPosition())*uiDrawParams.mInvAuthScale;

	return touchRect.contains(mTouchPos);
}

//*****************************************************************************
int VuCarChampListEntity::calcNumItemsOnScreen()
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
VuVector2 VuCarChampListEntity::calcPosition()
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	const VuMatrix &cropMatrix = VuUI::IF()->getCropMatrix();

	VuVector2 position = uiDrawParams.mPosition;

	position *= uiDrawParams.mInvAuthScale;
	position.mX -= cropMatrix.mT.mX/cropMatrix.mX.mX;
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
VuVector2 VuCarChampListEntity::calcItemOffset(int itemIndex)
{
	return VuVector2(itemIndex*mSpacing - mScrollPos, 0.0f);
}

//*****************************************************************************
float VuCarChampListEntity::calcScrollMin()
{
	return 0.0f;
}

//*****************************************************************************
float VuCarChampListEntity::calcScrollMax()
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
void VuCarChampListEntity::findItem(int index)
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
bool VuCarChampListEntity::isInProgress(int index)
{
	bool isInProgress = false;

	if ( VuProfileManager::IF() )
	{
		const Item &item = mItems[index];
		const VuJsonContainer &carChampData = VuProfileManager::IF()->dataRead()["CarChampData"][item.mCarName];
		const VuJsonContainer &races = carChampData["Races"];

		if ( races.size() && !races[races.size() - 1].hasMember("Place") )
			isInProgress = true;
	}

	return isInProgress;
}

//*****************************************************************************
bool VuCarChampListEntity::isStageAvailable(int iItem, int iStage)
{
	if ( iStage >= 1 && VuStatsManager::IF() )
	{
		int bestPlace = 0;
		VuStatsManager::IF()->getCarChampBestPlace(mItems[iItem].mCarName.c_str(), iStage - 1, bestPlace);
		if ( bestPlace != 1 )
			return false;
	}

	return true;
}

//*****************************************************************************
void VuCarChampListEntity::onButtonPressed(int iItem, int iButton)
{
	const Item &item = mItems[iItem];

	if ( !item.mIsOwned )
	{
		if ( iButton == 0 )
			onBuyButtonPressed(iItem);
	}
	else if ( isInProgress(iItem) )
	{
		if ( iButton == 0 )
			onQuitButtonPressed(iItem);
		if ( iButton == 1 )
			onContinueButtonPressed(iItem);
	}
	else
	{
		if ( iButton >= 0 && iButton < 4 )
			onStageButtonPressed(iItem, iButton);
	}
}

//*****************************************************************************
void VuCarChampListEntity::onBuyButtonPressed(int iItem)
{
	Item &item = mItems[iItem];

	if ( VuGameManager::IF()->purchaseCar(item.mCarName) )
	{
		item.mIsOwned = true;
		mpScriptComponent->getPlug("PurchaseSuccess")->execute();
	}
	else
	{
		mpScriptComponent->getPlug("PurchaseFailure")->execute();
	}
}

//*****************************************************************************
void VuCarChampListEntity::onQuitButtonPressed(int iItem)
{
	mpScriptComponent->getPlug("QuitChamp")->execute();
}

//*****************************************************************************
void VuCarChampListEntity::onContinueButtonPressed(int iItem)
{
	Item &item = mItems[iItem];

	VuGameUtil::IF()->dataWrite()["CarChamp"]["Car"].putValue(item.mCarName);

	const VuJsonContainer &carChampData = VuProfileManager::IF()->dataRead()["CarChampData"][item.mCarName];
	VuGameUtil::IF()->dataWrite()["CarChamp"]["Stage"] = carChampData["Stage"];
	VuGameUtil::IF()->dataWrite()["CarChamp"]["Driver"] = carChampData["Driver"];

	mpScriptComponent->getPlug("ContinueChamp")->execute();
}

//*****************************************************************************
void VuCarChampListEntity::onStageButtonPressed(int iItem, int iStage)
{
	const Item &item = mItems[iItem];

	bool available = isStageAvailable(mSelectedItem, iStage);

	if ( available )
	{
		const VuGameManager::Car &car = VuGameManager::IF()->getCar(item.mCarName);

		if ( car.mStage >= iStage )
		{
			VuGameUtil::IF()->dataWrite()["CarChamp"]["Car"].putValue(item.mCarName);
			VuGameUtil::IF()->dataWrite()["CarChamp"]["Stage"].putValue(iStage);

			mpScriptComponent->getPlug("CarChampChosen")->execute();
		}
		else
		{
			char strPrice[32];
			VuStringUtil::integerFormat(VuGameUtil::IF()->getCarLevelUpPrice(item.mCarName, car.mStage + 1), strPrice, sizeof(strPrice));
			VuGameFontMacros::IF()->setMacro("CARCHAMP_LEVELUP_PRICE", strPrice);

			if ( car.areStatsMaxed() )
				mpScriptComponent->getPlug("NeedLevelUp")->execute();
			else
				mpScriptComponent->getPlug("NeedUpgrade")->execute();
		}
	}
	else
	{
		mpScriptComponent->getPlug("StageLocked")->execute();
	}
}
