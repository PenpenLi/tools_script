//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Series List Entity
// 
//*****************************************************************************

#include "VuHListEntity.h"
#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Properties/VuFontEnumProperty.h"
#include "VuEngine/Assets/VuSpreadsheetAsset.h"
#include "VuEngine/Assets/VuTextureAsset.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/Font/VuFontDraw.h"
#include "VuEngine/Managers/VuProfileManager.h"
#include "VuEngine/DB/VuStringDB.h"
#include "VuEngine/UI/VuUIDrawUtil.h"
#include "VuEngine/Util/VuStringUtil.h"
#include "VuEngine/Util/VuSpreadsheetQuery.h"
#include "VuEngine/Dev/VuDevConfig.h"


class VuSeriesListEntity : public VuHListEntity
{
	DECLARE_RTTI

public:
	VuSeriesListEntity();
	~VuSeriesListEntity();

	virtual void	onGameInitialize();
	virtual void	onGameRelease();

protected:
	virtual int					getItemCount();
	virtual void				getItemName(int index, std::string &itemName);
	virtual void				getItemImage(int index, std::string &itemImage);
	virtual bool				isItemLocked(int index);
	virtual void				getUnlockText(int index, std::string &text) { text = VuStringDB::IF()->getString(mLockedStringID); }
	virtual void				onItemSelected(int index) {}
	virtual void				onItemChosen(int index);
	virtual void				onItemLocked(int inex);
	virtual void				onDrawItem(int index, const VuVector2 &offset, float scale);

	void						setCurrentSeries(int index);
	void						buildList();

	// properties
	VuUIRectProperties			mBossRect;
	VuUIRectProperties			mBossCheckRect;
	VuUIRectProperties			mStageRect;
	VuUIRectProperties			mStageCheckRect;
	VuUIImageProperties			mCheckedImage;
	VuUIImageProperties			mUncheckedImage;
	VuUIRectProperties			mStarsImageRect;
	VuUIImageProperties			mStarsImage;
	VuUIRectProperties			mStarsTextRect;
	std::string					mStarsFont;
	VuUIStringFormatProperties	mStarsStringFormat;
	std::string					mLockedStringID;

	struct Series
	{
		Series() : mPreviousBossDefeated(true), mEligibleCarOwned(true), mNeedToSwitchCar(false), mNeedToDowngradeCar(false), mpPreviousBossImage(VUNULL), mDone(false)  {}
		std::string		mName;
		std::string		mBossBattle;
		std::string		mBossName;
		int				mStage;
		bool			mPreviousBossDefeated;
		bool			mEligibleCarOwned;
		bool			mNeedToSwitchCar;
		bool			mNeedToDowngradeCar;
		VuTextureAsset	*mpPreviousBossImage;
		VuTextureAsset	*mpStageImage;
		float			mDelay;
		float			mAngularFrequency;
		bool			mDone;
	};
	std::vector<Series>			mSeries;
};


IMPLEMENT_RTTI(VuSeriesListEntity, VuHListEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuSeriesListEntity);


//*****************************************************************************
VuSeriesListEntity::VuSeriesListEntity()
{
	// properties
	addProperty(new VuRectProperty("Boss Rect", mBossRect));
	addProperty(new VuRectProperty("Boss Check Rect", mBossCheckRect));
	addProperty(new VuRectProperty("Stage Rect", mStageRect));
	addProperty(new VuRectProperty("Stage Check Rect", mStageCheckRect));
	mCheckedImage.addProperties(getProperties(), "Checked Image");
	mUncheckedImage.addProperties(getProperties(), "Unchecked Image");
	addProperty(new VuRectProperty("Stars Image Rect", mStarsImageRect));
	mStarsImage.addProperties(getProperties(), "Stars Image");
	addProperty(new VuRectProperty("Stars Text Rect", mStarsTextRect));
	addProperty(new VuFontEnumProperty("Stars Font", mStarsFont));
	ADD_UI_STRING_FORMAT_PROPERTIES(getProperties(), mStarsStringFormat, "Stars ");
	addProperty(new VuStringProperty("Locked String ID", mLockedStringID));

	// scripting
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, SeriesChosen);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, LockedBoss);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, LockedStage);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, SwitchCar);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, DowngradeCar);

	// build series list
	buildList();
}

//*****************************************************************************
VuSeriesListEntity::~VuSeriesListEntity()
{
	for ( auto &series : mSeries )
	{
		VuAssetFactory::IF()->releaseAsset(series.mpPreviousBossImage);
		VuAssetFactory::IF()->releaseAsset(series.mpStageImage);
	}
}

//*****************************************************************************
void VuSeriesListEntity::onGameInitialize()
{
	VuHListEntity::onGameInitialize();


	float scrollPos = VuProfileManager::IF()->dataRead()["Lists"]["SeriesScrollPos"].asFloat();
	scrollPos = VuClamp(scrollPos, calcScrollMin(), calcScrollMax());
	mScrollPos = mScrollTargetPos = scrollPos;

	const std::string &seriesName = VuProfileManager::IF()->dataRead()["Lists"]["Series"].asString();
	for ( int i = 0; i < (int)mSeries.size(); i++ )
		if ( seriesName == mSeries[i].mName )
			setSelectedItem(i, true);
}

//*****************************************************************************
void VuSeriesListEntity::onGameRelease()
{
	const std::string &seriesName = mSeries[getSelectedItem()].mName;
	VuProfileManager::IF()->dataWrite()["Lists"]["Series"].putValue(seriesName);
	VuProfileManager::IF()->dataWrite()["Lists"]["SeriesScrollPos"].putValue(mScrollTargetPos);

	VuHListEntity::onGameRelease();
}

//*****************************************************************************
int VuSeriesListEntity::getItemCount()
{
	return (int)mSeries.size();
}

//*****************************************************************************
void VuSeriesListEntity::getItemName(int index, std::string &itemName)
{
	itemName = mSeries[index].mName;
}

//*****************************************************************************
void VuSeriesListEntity::getItemImage(int index, std::string &itemImage)
{
	itemImage = "UI/SelectionIcons/";
	itemImage += mSeries[index].mName;
}

//*****************************************************************************
bool VuSeriesListEntity::isItemLocked(int index)
{
	//½âËø
	//return false;
	if ( VuDevConfig::IF() && VuDevConfig::IF()->getParam("UnlockAll").asBool() )
		return false;

	if ( !mSeries[index].mPreviousBossDefeated )
		return true;

	return false;
}

//*****************************************************************************
void VuSeriesListEntity::onItemChosen(int index)
{
	setCurrentSeries(index);

	if ( !mSeries[index].mEligibleCarOwned )
		mpScriptComponent->getPlug("LockedStage")->execute();
	else if ( mSeries[index].mNeedToSwitchCar )
		mpScriptComponent->getPlug("SwitchCar")->execute();
	else if ( mSeries[index].mNeedToDowngradeCar )
		mpScriptComponent->getPlug("DowngradeCar")->execute();
	else
		mpScriptComponent->getPlug("SeriesChosen")->execute();
}

//*****************************************************************************
void VuSeriesListEntity::onItemLocked(int index)
{
	setCurrentSeries(index);

	if ( !mSeries[index].mPreviousBossDefeated )
		mpScriptComponent->getPlug("LockedBoss")->execute();
}

//*****************************************************************************
void VuSeriesListEntity::onDrawItem(int index, const VuVector2 &offset, float scale)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	bool locked = isItemLocked(index);

	// draw previous boss image & checkbox
	if ( locked )
	{
		if ( mSeries[index].mpPreviousBossImage )
		{
			VuRect rect = mBossRect;
			rect.scale(mTouchRect.getCenter(), scale);
			VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth - 11*GFX_SORT_DEPTH_STEP, mSeries[index].mpPreviousBossImage->getTexture(), (rect + offset)*uiDrawParams.mInvAuthScale);
		}

		VuRect rect = mBossCheckRect;
		rect.scale(mTouchRect.getCenter(), scale);
		if ( mSeries[index].mPreviousBossDefeated )
			VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth - 11*GFX_SORT_DEPTH_STEP, mCheckedImage.getTexture(), (rect + offset)*uiDrawParams.mInvAuthScale);
		else
			VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth - 11*GFX_SORT_DEPTH_STEP, mUncheckedImage.getTexture(), (rect + offset)*uiDrawParams.mInvAuthScale);
	}
	
	// draw stage image & checkbox
	{
		if ( mSeries[index].mpStageImage )
		{
			VuRect rect = mStageRect;
			rect.scale(mTouchRect.getCenter(), scale);
			VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth - 11*GFX_SORT_DEPTH_STEP, mSeries[index].mpStageImage->getTexture(), (rect + offset)*uiDrawParams.mInvAuthScale);
		}

		VuRect rect = mStageCheckRect;
		rect.scale(mTouchRect.getCenter(), scale);
		if ( mSeries[index].mEligibleCarOwned )
			VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth - 11*GFX_SORT_DEPTH_STEP, mCheckedImage.getTexture(), (rect + offset)*uiDrawParams.mInvAuthScale);
		else
			VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth - 11*GFX_SORT_DEPTH_STEP, mUncheckedImage.getTexture(), (rect + offset)*uiDrawParams.mInvAuthScale);
	}

	// draw stars image
	if ( !locked )
	{
		VuRect rect = mStarsImageRect;
		rect.scale(mTouchRect.getCenter(), scale);
		VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth, mStarsImage.getTexture(), (rect + offset)*uiDrawParams.mInvAuthScale);
	}

	// draw stars text
	if ( !locked )
	{
		int earnedStars = VuGameUtil::IF()->calcEarnedSeriesStars(mSeries[index].mName.c_str());
		int maxStars = VuGameUtil::IF()->calcMaxSeriesStars(mSeries[index].mName.c_str());

		const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mStarsFont);
		VuFontDrawParams fdParams = fontEntry.params();

		VuRect rect = mStarsTextRect;
		rect.scale(mTouchRect.getCenter(), scale);
		rect = (rect + offset)*uiDrawParams.mInvAuthScale;
		fdParams.mSize *= scale;

		char str[64];
		VU_SPRINTF(str, sizeof(str), "%d/%d", earnedStars, maxStars);
		VuGfxUtil::IF()->fontDraw()->drawString(uiDrawParams.mDepth, fontEntry.font(), str, fdParams, rect, mStarsStringFormat);
	}
}

//*****************************************************************************
void VuSeriesListEntity::setCurrentSeries(int index)
{
	VuJsonContainer &eventData = VuGameUtil::IF()->dataWrite()["EventData"];
	eventData["SeriesName"].putValue(mSeries[index].mName);
}

//*****************************************************************************
void VuSeriesListEntity::buildList()
{
	const VuSpreadsheetAsset *pSA = VuGameUtil::IF()->eventSpreadsheet();
	int seriesColumn = pSA->getColumnIndex("Series");
	int eventColumn = pSA->getColumnIndex("Event");
	for ( int iRow = 0; iRow < pSA->getRowCount(); iRow++ )
	{
		const char *seriesName = pSA->getField(iRow, seriesColumn).asCString();
		const char *eventName = pSA->getField(iRow, eventColumn).asCString();

		VUASSERT(seriesName[0], "Invalid series in sp event spreadsheet!");

		// already added?
		bool exists = false;
		for ( int i = 0; i < (int)mSeries.size(); i++ )
			if ( mSeries[i].mName == seriesName )
				exists = true;

		if ( !exists )
		{
			mSeries.resize(mSeries.size() + 1);
			Series &series = mSeries.back();
			series.mName = seriesName;
			series.mStage = VuGameUtil::IF()->seriesDB()[seriesName]["Stage"].asInt();
		}
		else
		{
			Series &series = mSeries.back();
			series.mBossBattle = eventName;
		}
	}

	// determine highest car stage
	int currentCarStage = 0;
	int highestCarStage = 0;
	if ( VuGameManager::IF() )
	{
		currentCarStage = VuGameManager::IF()->getCurCar().mStage;

		const VuJsonContainer &carNames = VuGameUtil::IF()->constantDB()["Names"]["Cars"];
		for ( int i = 0; i < carNames.size(); i++ )
			highestCarStage = VuMax(highestCarStage, VuGameManager::IF()->getCar(carNames[i].asString()).mStage);
	}

	for ( int i = 0; i < (int)mSeries.size(); i++ )
	{
		Series &series = mSeries[i];


		//modify by xlm
		/*series.mEligibleCarOwned = highestCarStage >= series.mStage;*/
		series.mEligibleCarOwned = true;
		//tony.hu disable hp check for battle
		series.mNeedToDowngradeCar = false;//currentCarStage > series.mStage;
		series.mNeedToSwitchCar = false;// (highestCarStage >= series.mStage) && (currentCarStage < series.mStage);
		
		series.mBossName = VuGameUtil::IF()->getSeriesBossName(series.mName);

		if (VuDevConfig::IF() && VuDevConfig::IF()->getParam("UnlockAll").asBool())
		{
			series.mEligibleCarOwned = true;
		}

		if ( i >= 1 )
		{
			Series &previousSeries = mSeries[i-1];

			// previous boss image
			const std::string &previousBossImageName = VuGameUtil::IF()->driverDB()[previousSeries.mBossName]["PortraitImage"].asString();
			series.mpPreviousBossImage = VuAssetFactory::IF()->createAsset<VuTextureAsset>(previousBossImageName, VuAssetFactory::OPTIONAL_ASSET);
			
			// previous boss defeated?
			if ( VuGameManager::IF() )
				series.mPreviousBossDefeated = VuGameManager::IF()->getDriver(previousSeries.mBossName).mIsBeaten;
		}

		// stage image
		int stage = VuGameUtil::IF()->seriesDB()[series.mName]["Stage"].asInt();
		char strStageImage[64];
		VU_SPRINTF(strStageImage, sizeof(strStageImage), "UI/Icon/Stage%d", stage);
		series.mpStageImage = VuAssetFactory::IF()->createAsset<VuTextureAsset>(strStageImage, VuAssetFactory::OPTIONAL_ASSET);
	}
}
