//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Event List Entity
// 
//*****************************************************************************

#include "VuPurple/Components/VuMultiBoingTransitionComponent.h"
#include "VuPurple/Managers/VuStatsManager.h"
#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Managers/VuControlMethodManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/UI/VuUI.h"
#include "VuEngine/UI/VuUIDrawUtil.h"
#include "VuEngine/UI/VuUIInputUtil.h"
#include "VuEngine/UI/VuUIPropertyUtil.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Components/2dLayout/Vu2dLayoutComponent.h"
#include "VuEngine/Properties/VuFontEnumProperty.h"
#include "VuEngine/Assets/VuTextureAsset.h"
#include "VuEngine/Assets/VuFontAsset.h"
#include "VuEngine/Assets/VuSpreadsheetAsset.h"
#include "VuEngine/Assets/VuProjectAsset.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/Font/VuFontDraw.h"
#include "VuEngine/DB/VuStringDB.h"
#include "VuEngine/Math/VuMathUtil.h"
#include "VuEngine/Util/VuSpreadsheetQuery.h"
#include "VuEngine/Dev/VuDevConfig.h"
#include "../../VuEngine.h"


class VuEventListEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuEventListEntity();
	~VuEventListEntity();

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

	VuVector2						calcOffset(int eventIndex);
	bool							isLocked(int eventIndex);
	void							select(int eventIndex);
	void							choose();
	void							showTips(int eventIndex);

	void							buildList();

	int								numRows();
	int								numColumns();

	// components
	VuScriptComponent				*mpScriptComponent;
	VuMultiBoingTransitionComponent	*mpTransitionComponent;

	// properties
	int								mRowCount;
	VuVector2						mSpacing;
	VuUIRectProperties				mImageRect;
	VuUIRectProperties				mTouchRect;
	VuUIRectProperties				mTextRect;
	std::string						mFont;
	VuUIStringFormatProperties		mStringFormat;
	VuUIImageProperties				mSelectedTexture;
	VuUIImageProperties				mUnselectedTexture;
	VuUIImageProperties				mStarImage;
	VuUIImageProperties				mStarMissingImage;
	VuUIRectProperties				mStarRect;
	float							mStarSpacing;
	VuUIImageProperties				mLockImage;
	VuUIRectProperties				mLockRect;
	VuUIRectProperties				mGameTypeRect;
	VuUIRectProperties				mStarsReqRect;
	std::string						mStarsReqFont;
	VuUIStringFormatProperties		mStarsReqStringFormat;
	VuUIRectProperties				mBossImageRect;
	VuUIRectProperties				mBossBeatenRect;
	VuUIImageProperties				mBossBeatenImage;
	VuUIRectProperties				mBossUnbeatenRect;
	VuUIImageProperties				mBossUnbeatenImage;
	float							mMaxDelay;
	float							mMinAngularFrequency;
	float							mMaxAngularFrequency;
	float							mDampingRatio;
	VuUIImageProperties				mGemImage;
	VuUIImageProperties				mGemMissingImage;
	VuUIRectProperties				mGemRect;

	bool							mNewTouch;
	bool							mTouchDown;
	bool							mTouchUp;
	VuVector2						mTouchPos;

	enum ePressedState { NOT_PRESSED, PRESSED };
	ePressedState					mPressedState;
	float							mPressedScale;

	struct Event
	{
		Event() : mBestPlace(0), mIsBossBattle(false), mIsBossBeaten(false), mpBossImage(VUNULL), mDone(false) {}
		std::string		mName;
		int				mSpreadsheetRow;
		int				mStarsRequired;
		int				mBestPlace;
		VuTextureAsset	*mpGameTypeImage;
		bool			mIsBossBattle;
		bool			mIsBossBeaten;
		VuTextureAsset	*mpBossImage;
		float			mDelay;
		float			mAngularFrequency;
		bool			mDone;
	};
	std::vector<Event>		mEvents;

	std::string				mSeriesName;
	int						mSelectedEventIndex;
};


IMPLEMENT_RTTI(VuEventListEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuEventListEntity);


//*****************************************************************************
VuEventListEntity::VuEventListEntity():
	mRowCount(3),
	mSpacing(0,0),
	mImageRect(0,0,0,0),
	mTouchRect(0,0,0,0),
	mTextRect(0,0,0,0),
	mStarSpacing(50.0f),
	mMaxDelay(0.3f),
	mMinAngularFrequency(2.0f),
	mMaxAngularFrequency(3.0f),
	mDampingRatio(0.5f),
	mNewTouch(false),
	mTouchDown(false),
	mTouchUp(false),
	mPressedState(NOT_PRESSED),
	mSelectedEventIndex(0)
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this, 120));
	addComponent(new Vu2dLayoutComponent(this, &VuEventListEntity::drawLayout));
	addComponent(mpTransitionComponent = new VuMultiBoingTransitionComponent(this));

	// properties
	addProperty(new VuIntProperty("Row Count", mRowCount));
	addProperty(new VuVector2Property("Spacing", mSpacing));
	addProperty(new VuRectProperty("Image Rect", mImageRect));
	addProperty(new VuRectProperty("Touch Rect", mTouchRect));
	addProperty(new VuRectProperty("Text Rect", mTextRect));
	addProperty(new VuFontEnumProperty("Font", mFont));
	ADD_UI_STRING_FORMAT_PROPERTIES(getProperties(), mStringFormat, "");
	mSelectedTexture.addProperties(getProperties(), "Selected Texture");
	mUnselectedTexture.addProperties(getProperties(), "Unselected Texture");
	mStarImage.addProperties(getProperties(), "Star Image");
	mStarMissingImage.addProperties(getProperties(), "Star Missing Image");
	addProperty(new VuRectProperty("Star Rect", mStarRect));
	addProperty(new VuFloatProperty("Star Spacing", mStarSpacing));
	mLockImage.addProperties(getProperties(), "Lock Image");
	addProperty(new VuRectProperty("Lock Rect", mLockRect));
	addProperty(new VuRectProperty("Game Type Rect", mGameTypeRect));
	addProperty(new VuRectProperty("Stars Req Rect", mStarsReqRect));
	addProperty(new VuFontEnumProperty("Stars Req Font", mStarsReqFont));
	ADD_UI_STRING_FORMAT_PROPERTIES(getProperties(), mStarsReqStringFormat, "Stars Req ");
	addProperty(new VuRectProperty("Boss Image Rect", mBossImageRect));
	addProperty(new VuRectProperty("Boss Beaten Rect", mBossBeatenRect));
	mBossBeatenImage.addProperties(getProperties(), "Boss Beaten Image");
	addProperty(new VuRectProperty("Boss Unbeaten Rect", mBossUnbeatenRect));
	mBossUnbeatenImage.addProperties(getProperties(), "Boss Unbeaten Image");
	addProperty(new VuFloatProperty("Max Delay", mMaxDelay));
	addProperty(new VuFloatProperty("Min Angular Frequency", mMinAngularFrequency));
	addProperty(new VuFloatProperty("Max Angular Frequency", mMaxAngularFrequency));
	addProperty(new VuFloatProperty("Damping Ratio", mDampingRatio));
	mGemImage.addProperties(getProperties(), "Gem Image");
	mGemMissingImage.addProperties(getProperties(), "Gem Missing Image");
	addProperty(new VuRectProperty("Gem Rect", mGemRect));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuEventListEntity, SetFocus);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, NavUp);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, NavDown);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, NavLeft);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, NavRight);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnChosen);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnLocked);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent,OnShowNoEnoughStarsTips);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnShowHasSameRolesWithBossDriver);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnBossBeaten);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnBossChosen);

	// event handlers
	REG_EVENT_HANDLER(VuEventListEntity, OnUITick);
	REG_EVENT_HANDLER(VuEventListEntity, OnUIGamePad);
	REG_EVENT_HANDLER(VuEventListEntity, OnUITouch);
	REG_EVENT_HANDLER(VuEventListEntity, OnUIDraw);

	// build event list
	buildList();

	// constants
	mPressedScale = 1.0f - 0.01f*VuGameUtil::IF()->constantDB()["UI"]["PressedButtonSizeReduction"].asFloat();
}

//*****************************************************************************
VuEventListEntity::~VuEventListEntity()
{
	for ( int i = 0; i < (int)mEvents.size(); i++ )
	{
		VuAssetFactory::IF()->releaseAsset(mEvents[i].mpGameTypeImage);
		VuAssetFactory::IF()->releaseAsset(mEvents[i].mpBossImage);
	}
}

//*****************************************************************************
void VuEventListEntity::onGameInitialize()
{
	int eventIndex = 0;
	const std::string &eventName = VuProfileManager::IF()->dataRead()["Lists"]["Event"][mSeriesName].asString();
	for ( int i = 0; i < (int)mEvents.size(); i++ )
		if ( eventName == mEvents[i].mName )
			eventIndex = i;

	select(eventIndex);
}

//*****************************************************************************
void VuEventListEntity::onGameRelease()
{
	const std::string &eventName = mEvents[mSelectedEventIndex].mName;
	VuProfileManager::IF()->dataWrite()["Lists"]["Event"][mSeriesName].putValue(eventName);
}

//*****************************************************************************
VuRetVal VuEventListEntity::SetFocus(const VuParams &params)
{
	VuUI::IF()->setFocus(this);

	return VuRetVal();
}

//*****************************************************************************
void VuEventListEntity::OnUITick(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	float fdt = accessor.getFloat();

	mpTransitionComponent->tick(fdt);

	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	if ( mNewTouch )
	{
		for ( int eventIndex = 0; eventIndex < (int)mEvents.size(); eventIndex++ )
		{
			VuRect touchRect = uiDrawParams.transform(mTouchRect + calcOffset(eventIndex));
			if ( touchRect.contains(mTouchPos) )
			{
				SetFocus();
				mPressedState = PRESSED;
				select(eventIndex);
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
		VuRect touchRect = uiDrawParams.transform(mTouchRect + calcOffset(mSelectedEventIndex));
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
void VuEventListEntity::OnUIGamePad(const VuParams &params)
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
				int newIndex = mSelectedEventIndex - numColumns();
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
				int newIndex = mSelectedEventIndex + numColumns();
				if ( newIndex < (int)mEvents.size() )
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
				int newIndex = mSelectedEventIndex - 1;
				if ( newIndex >= 0 )
				{
					select(mSelectedEventIndex - 1);
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
				int newIndex = mSelectedEventIndex + 1;
				if ( newIndex < (int)mEvents.size() )
				{
					select(mSelectedEventIndex + 1);
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
void VuEventListEntity::OnUITouch(const VuParams &params)
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
void VuEventListEntity::OnUIDraw(const VuParams &params)
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
void VuEventListEntity::drawLayout(bool bSelected)
{
	if ( bSelected )
	{
		VuUIDrawParams uiDrawParams;
		VuUIDrawUtil::getParams(this, uiDrawParams);

		for ( int eventIndex = 0; eventIndex < (int)mEvents.size(); eventIndex++ )
		{
			VuRect touchRect = uiDrawParams.transform(mTouchRect + calcOffset(eventIndex));
			VuGfxUtil::IF()->drawRectangleOutline2d(uiDrawParams.mDepth, VuColor(255,255,255), touchRect);
		}
	}

	drawItems(1.0f, 1.0f);
}

//*****************************************************************************
void VuEventListEntity::drawItems(float scaleStart, float scaleTarget)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	bool hasFocus = VuUI::IF()->getFocus() == this;

	for ( int eventIndex = 0; eventIndex < (int)mEvents.size(); eventIndex++ )
	{
		Event &event = mEvents[eventIndex];

		// determine scale
		float scale = scaleStart, vel = 0.0f;
		float age = VuMax(mpTransitionComponent->getTransitionTimer() - event.mDelay, 0.0f);
		VuMathUtil::calcDampedSimpleHarmonicMotion(&scale, &vel, scaleTarget, age, event.mAngularFrequency*VU_2PI, mDampingRatio);
		if ( (eventIndex == mSelectedEventIndex) && (mPressedState == PRESSED) )
			scale *= mPressedScale;

		if ( scale < 0.0f )
			event.mDone = true;
		if ( event.mDone )
			continue;

		VuVector2 offset = calcOffset(eventIndex);
		float depth = uiDrawParams.mDepth;

		// draw background
		{
			VuColor backgroundColor(255,255,255);

			VuTexture *pTexture = mUnselectedTexture.getTexture();
			if ( eventIndex == mSelectedEventIndex )
			{
				if ( hasFocus && VuControlMethodManager::IF()->isGamePadOrKeyboard() )
				{
					pTexture = mSelectedTexture.getTexture();
					backgroundColor = VuGameUtil::IF()->calcHighlightColor(1.0f);
				}
			}

			if ( pTexture )
			{
				VuRect dstRect = mImageRect;
				dstRect.scale(mImageRect.getCenter(), scale);
				dstRect = uiDrawParams.transform(dstRect + offset);

				VuGfxUtil::IF()->drawTexture2d(depth, pTexture, backgroundColor, dstRect);

				depth -= GFX_SORT_DEPTH_STEP;
			}
		}

		// draw game type
		if ( VuTextureAsset *pTextureAsset = event.mpGameTypeImage )
		{
			VuRect dstRect = mGameTypeRect;
			dstRect.scale(mImageRect.getCenter(), scale);
			dstRect = uiDrawParams.transform(dstRect + offset);

			VuGfxUtil::IF()->drawTexture2d(depth, pTextureAsset->getTexture(), dstRect);

			depth -= GFX_SORT_DEPTH_STEP;
		}

		if ( !isLocked(eventIndex) && !event.mIsBossBattle )
		{
			// stars
			{
				VuRect rect = mStarRect;
				for ( int i = 0; i < 3; i++ )
				{
					VuTexture *pTexture = (event.mBestPlace > 0 && event.mBestPlace <= 3 - i) ? mStarImage.getTexture() : mStarMissingImage.getTexture();
					if ( pTexture )
					{
						VuRect dstRect = rect;
						dstRect.scale(mImageRect.getCenter(), scale);
						dstRect = uiDrawParams.transform(dstRect + offset);
						VuGfxUtil::IF()->drawTexture2d(depth, pTexture, dstRect);
					}
					rect += VuVector2(mStarSpacing, 0.0f);
				}
				depth -= GFX_SORT_DEPTH_STEP;
			}

			// gem
			{
				VuTexture *pTexture = (event.mBestPlace == 1) ? mGemImage.getTexture() : mGemMissingImage.getTexture();
				if ( pTexture )
				{
					VuRect dstRect = mGemRect;
					dstRect.scale(mImageRect.getCenter(), scale);
					dstRect = uiDrawParams.transform(dstRect + offset);
					VuGfxUtil::IF()->drawTexture2d(depth, pTexture, dstRect);
				}
				depth -= GFX_SORT_DEPTH_STEP;
			}
		}

		// draw boss image
		if ( event.mIsBossBattle )
		{
			// draw boss image
			if ( event.mpBossImage )
			{
				VuRect dstRect = mBossImageRect;
				dstRect.scale(mImageRect.getCenter(), scale);
				dstRect = uiDrawParams.transform(dstRect + offset);
				VuGfxUtil::IF()->drawTexture2d(depth, event.mpBossImage->getTexture(), dstRect);
			}
			depth -= GFX_SORT_DEPTH_STEP;

			if ( event.mIsBossBeaten )
			{
				VuRect dstRect = mBossBeatenRect;
				dstRect.scale(mImageRect.getCenter(), scale);
				dstRect = uiDrawParams.transform(dstRect + offset);
				VuGfxUtil::IF()->drawTexture2d(depth, mBossBeatenImage.getTexture(), dstRect);
			}
			else
			{
				VuRect dstRect = mBossUnbeatenRect;
				dstRect.scale(mImageRect.getCenter(), scale);
				dstRect = uiDrawParams.transform(dstRect + offset);
				VuGfxUtil::IF()->drawTexture2d(depth, mBossUnbeatenImage.getTexture(), dstRect);
			}
			depth -= GFX_SORT_DEPTH_STEP;
		}

		// draw index
		if ( !event.mIsBossBattle )
		{
			const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mFont);
			char strIndex[16];
			VU_SPRINTF(strIndex, sizeof(strIndex), "%d", eventIndex + 1);

			VuFontDrawParams fdParams = fontEntry.params();
			VuRect dstRect = mTextRect;
			fdParams.mSize *= scale;
			dstRect.scale(mImageRect.getCenter(), scale);
			dstRect = uiDrawParams.transform(dstRect + offset);

			VuGfxUtil::IF()->fontDraw()->drawString(depth, fontEntry.font(), strIndex, fdParams, dstRect, mStringFormat);
	
			depth -= GFX_SORT_DEPTH_STEP;
		}

		if ( isLocked(eventIndex) )
		{
			// draw lock image
			if ( VuTexture *pTexture = mLockImage.getTexture() )
			{
				VuRect dstRect = mLockRect;
				dstRect.scale(mImageRect.getCenter(), scale);
				dstRect = uiDrawParams.transform(dstRect + offset);
				VuGfxUtil::IF()->drawTexture2d(depth, pTexture, dstRect);

				depth -= GFX_SORT_DEPTH_STEP;
			}

			// draw required stars
			{
				const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mStarsReqFont);

				char strReqStars[16];
				VU_SPRINTF(strReqStars, sizeof(strReqStars), "%d", event.mStarsRequired);

				VuFontDrawParams fdParams = fontEntry.params();
				VuRect dstRect = mStarsReqRect;
				fdParams.mSize *= scale;
				dstRect.scale(mImageRect.getCenter(), scale);
				dstRect = uiDrawParams.transform(dstRect + offset);

				VuGfxUtil::IF()->fontDraw()->drawString(depth, fontEntry.font(), strReqStars, fdParams, dstRect, mStarsReqStringFormat);
	
				depth -= GFX_SORT_DEPTH_STEP;
			}
		}
	}
}

//*****************************************************************************
VuVector2 VuEventListEntity::calcOffset(int eventIndex)
{
	int columnCount = numColumns();
	int rowCount = numRows();
	VuVector2 offset = mSpacing*VuVector2(eventIndex%columnCount, eventIndex/columnCount);
	VuVector2 size = mSpacing*VuVector2(columnCount-1, rowCount-1) + mTouchRect.getSize();
	offset -= 0.5f*size;

	return offset;
}

//*****************************************************************************
bool VuEventListEntity::isLocked(int eventIndex)
{
	//½âËø
	//return false;
	if ( VuDevConfig::IF() && VuDevConfig::IF()->getParam("UnlockAll").asBool() )
		return false;

	int stars = VuGameUtil::IF()->calcEarnedSeriesStars(mSeriesName.c_str());

	if (mEvents[eventIndex].mIsBossBattle == true)
	{
		if (!VuEngine::IF()->editorMode())
		{
			std::string curDriveName = VuGameManager::IF()->getCurDriverName();

			const Event &event = mEvents[eventIndex];

			const VuSpreadsheetAsset *pSA = VuGameUtil::IF()->eventSpreadsheet();
			const VuFastContainer &row = pSA->getRow(event.mSpreadsheetRow);
			std::string bossName = row[pSA->getColumnIndex("Opponents")].asCString();

			int nIndex = bossName.find(curDriveName);
			if (nIndex != std::string::npos)
				return true;
		}
	}

	return mEvents[eventIndex].mStarsRequired > stars;
}

//*****************************************************************************
void VuEventListEntity::select(int eventIndex)
{
	mSelectedEventIndex = eventIndex;

	const Event &event = mEvents[mSelectedEventIndex];
	VuJsonContainer &eventData = VuGameUtil::IF()->dataWrite()["EventData"];

	eventData["EventName"].putValue(event.mName);
}

void	VuEventListEntity::showTips(int eventIndex)
{
	//first judge if have enough stars
	int stars = VuGameUtil::IF()->calcEarnedSeriesStars(mSeriesName.c_str());
	if (mEvents[eventIndex].mStarsRequired > stars)
	{
		mpScriptComponent->getPlug("OnShowNoEnoughStarsTips")->execute();
		return;
	}

	//secod judge if have the driver same with the boss
	if (mEvents[eventIndex].mIsBossBattle == true)
	{
		std::string curDriveName = VuGameManager::IF()->getCurDriverName();

		const Event &event = mEvents[eventIndex];
		const VuSpreadsheetAsset *pSA = VuGameUtil::IF()->eventSpreadsheet();
		const VuFastContainer &row = pSA->getRow(event.mSpreadsheetRow);
		std::string bossName = row[pSA->getColumnIndex("Opponents")].asCString();

		int nIndex = bossName.find(curDriveName);
		if (nIndex != std::string::npos)
			mpScriptComponent->getPlug("OnShowHasSameRolesWithBossDriver")->execute();
	}	
}

//*****************************************************************************
void VuEventListEntity::choose()
{
	const Event &event = mEvents[mSelectedEventIndex];

	if ( isLocked(mSelectedEventIndex) )
	{
		mpScriptComponent->getPlug("OnLocked")->execute();
		showTips(mSelectedEventIndex);
	}
	else if ( event.mIsBossBattle && event.mIsBossBeaten )
	{
		mpScriptComponent->getPlug("OnBossBeaten")->execute();
	}
	else
	{
		const VuSpreadsheetAsset *pSA = VuGameUtil::IF()->eventSpreadsheet();

		const Event &event = mEvents[mSelectedEventIndex];
		const VuFastContainer &row = pSA->getRow(event.mSpreadsheetRow);
		std::string gameType = row[pSA->getColumnIndex("Type")].asCString();
		std::string trackName = row[pSA->getColumnIndex("Track")].asCString();
		std::string projectAsset = trackName + "_" + gameType;
		if ( !VuAssetFactory::IF()->doesAssetExist<VuProjectAsset>(projectAsset) )
			projectAsset = trackName + "_Race";

		VuJsonContainer &eventData = VuGameUtil::IF()->dataWrite()["EventData"];

		eventData["SeriesName"].putValue(mSeriesName);
		eventData["EventName"].putValue(event.mName);
		eventData["GameType"].putValue(gameType);
		eventData["ProjectAsset"].putValue(projectAsset);
		eventData["Track"].putValue(trackName);
		eventData["LapCount"].putValue(row[pSA->getColumnIndex("Laps")].asInt());
		eventData["TimeLimit"].putValue(row[pSA->getColumnIndex("Time")].asInt());
		eventData["IsChallenge"].putValue(false);

		eventData["Opponents"].clear();
		int firstOpponent = pSA->getColumnIndex("Opponents");
		if ( firstOpponent >= 0 )
		{
			for ( int i = firstOpponent; i < row.size(); i++ )
			{
				const char *opponent = row[i].asCString();
				if ( opponent[0] != '\0' )
					eventData["Opponents"].append().putValue(opponent);
			}
		}

		if ( event.mIsBossBattle )
			mpScriptComponent->getPlug("OnBossChosen")->execute();
		else
			mpScriptComponent->getPlug("OnChosen")->execute();
	}
}

//*****************************************************************************
void VuEventListEntity::buildList()
{

	const VuSpreadsheetAsset *pSA = VuGameUtil::IF()->eventSpreadsheet();
	int eventColumn = pSA->getColumnIndex("Event");
	int starsColumn = pSA->getColumnIndex("Stars");
	int typeColumn = pSA->getColumnIndex("Type");
	mSeriesName = VuProfileManager::IF() ? VuProfileManager::IF()->dataRead()["Lists"]["Series"].asString() : pSA->getField(0, "Series").asCString();

	VuSpreadsheetQuery::VuStringEqual expression("Series", mSeriesName.c_str());
	int rowIndex = VuSpreadsheetQuery::findFirstRow(pSA, expression);
	while ( rowIndex < pSA->getRowCount() )
	{
		mEvents.resize(mEvents.size() + 1);
		Event &event = mEvents.back();

		event.mName = pSA->getField(rowIndex, eventColumn).asCString();
		event.mSpreadsheetRow = rowIndex;
		event.mStarsRequired = pSA->getField(rowIndex, starsColumn).asInt();

		if ( VuStatsManager::IF() )
			VuStatsManager::IF()->getBestPlace(event.mName.c_str(), event.mBestPlace);

		std::string gameType = pSA->getField(rowIndex, typeColumn).asCString();
		std::string gameTypeImage = "UI/SelectionIcons/Event_" + gameType;
		event.mpGameTypeImage = VuAssetFactory::IF()->createAsset<VuTextureAsset>(gameTypeImage, VuAssetFactory::OPTIONAL_ASSET);

		if ( gameType == "BossBattle" )
		{
			event.mIsBossBattle = true;

			const std::string &bossName = VuGameUtil::IF()->getSeriesBossName(mSeriesName);
			if ( VuGameManager::IF() )
				event.mIsBossBeaten = VuGameManager::IF()->getDriver(bossName).mIsBeaten;

			if (VuDevConfig::IF() && VuDevConfig::IF()->getParam("UnlockAll").asBool())
			{
				event.mIsBossBeaten = false;
			}

			const std::string &bossImageName = VuGameUtil::IF()->driverDB()[bossName]["PortraitImage"].asString();
			event.mpBossImage = VuAssetFactory::IF()->createAsset<VuTextureAsset>(bossImageName, VuAssetFactory::OPTIONAL_ASSET);
		}

		event.mDelay = VuRand::global().range(0.0f, mMaxDelay);
		event.mAngularFrequency = VuRand::global().range(mMinAngularFrequency, mMaxAngularFrequency);

		rowIndex = VuSpreadsheetQuery::findNextRow(pSA, expression, rowIndex);
	}
}

//*****************************************************************************
int VuEventListEntity::numRows()
{
	int numColumns = (int)mEvents.size()/mRowCount;
	return VuMin(mRowCount, numColumns);
}

//*****************************************************************************
int VuEventListEntity::numColumns()
{
	int numColumns = (int)mEvents.size()/numRows();
	return numColumns;
}