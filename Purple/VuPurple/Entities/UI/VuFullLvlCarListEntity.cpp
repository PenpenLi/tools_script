
//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Series List Entity
// 
//*****************************************************************************


#include "VuPurple/Entities/UI/VuHListEntity.h"
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
#include "VuPurple/Gfx/VuGameGfxUtil.h"
#include "VuEngine/UI/VuUI.h"
#include "VuEngine/Math/VuMathUtil.h"
#include "VuPurple/Managers/VuControlMethodManager.h"


class VuFullLvlCarListEntity:public VuHListEntity
{
	DECLARE_RTTI
public:
	VuFullLvlCarListEntity();
	~VuFullLvlCarListEntity();

	virtual void onGameInitialize();
	virtual void onGameRelease();

protected:
	virtual int					getItemCount();
	virtual void				getItemName(int index, std::string &itemName);
	virtual void				getItemImage(int index, std::string &itemImage);
	virtual bool				isItemLocked(int index);
	virtual void				getUnlockText(int index, std::string &text){ text = "locked"; }
	virtual void				onItemSelected(int index){}
	virtual void				onItemChosen(int index);
	virtual void				onItemLocked(int inex);
	virtual void				onDrawItem(int index, const VuVector2 &offset, float scale);

	virtual void				OnUIDraw(const VuParams &params);

	//virtual void				drawLayout(bool bSelected);

	void buildList();

	void drawList(float scaleStart, float scaleTarget);
	void setCurrentFullLvlCar(int index);

	

	//properties
	VuUIRectProperties mCarImageRect;
	VuUIRectProperties mCarTitleRect;
	VuUIRectProperties mCarSubTitleRect;
	VuUIRectProperties mLabelRect;
	VuUIRectProperties mSaleInfo1Rect;
	VuUIRectProperties mSaleInfo2Rect;
	VuUIRectProperties mCurrentPriceRect;
	VuUIRectProperties mBuyBtnRect;
	VuUIRectProperties mBuyTextRect;

	std::string			subTitleFont;
	std::string			priceFont1;
	std::string			priceFont2;
	std::string			priceFont3;
	std::string			btnTextFont;

	VuTextureAsset *mpBuyImage;
	VuTextureAsset *mpOwnedImage;
	VuTextureAsset *mpLabelImage;

	typedef std::map<std::string, VuTextureAsset*> CarTexture;
	CarTexture mCarTextures;

	VuGameManager::FullLvlCars mFullLvlCar;

	typedef struct myString
	{
		myString(std::string str1){
			str = str1;
		}
		std::string str;


		bool operator< (const myString &a)  const
		{
			VuGameManager::FullLvlCars mCars = VuGameManager::IF()->getFullLvlCars();
			return	mCars[str].mCurrentPrice <mCars[a.str].mCurrentPrice;
		}
	}MyString;
	std::vector<MyString> mFullLvlCarNames;
	//std::vector<std::string> mFullLvlCarNames;
};

IMPLEMENT_RTTI(VuFullLvlCarListEntity, VuHListEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuFullLvlCarListEntity);



VuFullLvlCarListEntity::VuFullLvlCarListEntity()
{
	addProperty(new VuRectProperty("Car Image Rect", mCarImageRect));
	addProperty(new VuRectProperty("CarTitle Rect", mCarTitleRect));
	addProperty(new VuRectProperty("Car SubTitle Rect", mCarSubTitleRect));
	addProperty(new VuRectProperty("Label Rect", mLabelRect));
	addProperty(new VuRectProperty("Sale Info1 Rect", mSaleInfo1Rect));
	addProperty(new VuRectProperty("Sale Info2 Rect", mSaleInfo2Rect));

	addProperty(new VuRectProperty("Current Price Rect", mCurrentPriceRect));

	addProperty(new VuRectProperty("Buy Rect", mBuyBtnRect));
	addProperty(new VuRectProperty("BuyText Rect", mBuyTextRect));

	addProperty(new VuFontEnumProperty("subTitle Font", subTitleFont));
	addProperty(new VuFontEnumProperty("price1 Font", priceFont1));
	addProperty(new VuFontEnumProperty("price2 Font", priceFont2));
	addProperty(new VuFontEnumProperty("price3 Font", priceFont3));
	addProperty(new VuFontEnumProperty("btnTextFont Font", btnTextFont));

	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, CarChosen);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, CarIsOwned);
	
	buildList();
}


VuFullLvlCarListEntity::~VuFullLvlCarListEntity()
{
	/*for (auto &carName : mFullLvlCarNames)
	{*/
		//FullLvlCar car = mFullLvlCar[carName];
		//VuAssetFactory::IF()->releaseAsset(car.mpCarImage);
	//}

	for (std::map<std::string, VuTextureAsset*>::iterator it = mCarTextures.begin(); it != mCarTextures.end(); it++)
	{
		VuTextureAsset *pTexture = it->second;
		if (pTexture)
		{
			VuAssetFactory::IF()->releaseAsset(pTexture);
		}
		
	}

	if (mpBuyImage)
	{
		VuAssetFactory::IF()->releaseAsset(mpBuyImage);
	}

	if (mpOwnedImage)
	{
		VuAssetFactory::IF()->releaseAsset(mpOwnedImage);
	}
	if (mpLabelImage)
	{
		VuAssetFactory::IF()->releaseAsset(mpLabelImage);
	}
}

void VuFullLvlCarListEntity::onGameInitialize()
{
	VuHListEntity::onGameInitialize();

	for (int i = 0; i < mFullLvlCarNames.size(); i++)
	{
		std::string &carName = mFullLvlCarNames[i].str;
		VuGameManager::FullLvlCar& car = mFullLvlCar[carName];
		car.isOwned = VuGameManager::IF()->getCar(car.mCarName).mIsOwned;
	}
}

void VuFullLvlCarListEntity::onGameRelease()
{
	VuHListEntity::onGameRelease();
}


int VuFullLvlCarListEntity::getItemCount()
{
	return mFullLvlCar.size();
}


void VuFullLvlCarListEntity::getItemName(int index, std::string &itemName)
{
	std::string &carName = mFullLvlCarNames[index].str;
	VuGameManager::FullLvlCar car = mFullLvlCar[carName];
	itemName = car.mCarTitle;
}

void VuFullLvlCarListEntity::getItemImage(int index, std::string &itemImage)
{
	std::string &carName = mFullLvlCarNames[index].str;
	VuGameManager::FullLvlCar car = mFullLvlCar[carName];
	itemImage = "";
	itemImage += car.mCarTitle;
}

bool VuFullLvlCarListEntity::isItemLocked(int index)
{
	return false;
}

void VuFullLvlCarListEntity::onItemChosen(int index)
{
	setCurrentFullLvlCar(index);
	std::string &carName = mFullLvlCarNames[index].str;
	VuGameManager::FullLvlCar car = mFullLvlCar[carName];
	if (car.isOwned == false)
	{
		mpScriptComponent->getPlug("CarChosen")->execute();
	}
	else
	{
		mpScriptComponent->getPlug("CarIsOwned")->execute();
	}
}


void VuFullLvlCarListEntity::setCurrentFullLvlCar(int index)
{
	std::string &carName = mFullLvlCarNames[index].str;
	VuGameManager::FullLvlCar car = mFullLvlCar[carName];
	VuGameManager::IF()->setPurchase_FullLvlCarName(car.mCarName);
}

void VuFullLvlCarListEntity::onItemLocked(int index)
{
	mpScriptComponent->getPlug("CarIsOwned");
}

void VuFullLvlCarListEntity::OnUIDraw(const VuParams &params)
{
	drawList(1.0,1.0);
}



void VuFullLvlCarListEntity::drawList(float scaleStart, float scaleTarget)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	bool hasFocus = VuUI::IF()->getFocus() == this;

	VuVector2 position = calcPosition();

	int itemCount = getItemCount();
	for (int i = 0; i < itemCount; i++)
	{
		VuVector2 offset = position + calcItemOffset(i);

		Instance &inst = getInstance(i);

		// determine scale
		float scale = scaleStart, vel = 0.0f;
		//float age = VuMax(mpTransitionComponent->getTransitionTimer() - inst.mDelay, 0.0f);
		//VuMathUtil::calcDampedSimpleHarmonicMotion(&scale, &vel, scaleTarget, age, inst.mAngularFrequency*VU_2PI, mDampingRatio);

		if (scale < 0.0f)
			inst.mDone = true;
		if (inst.mDone)
			continue;

		// draw background
		{
			VuColor backgroundColor(255, 255, 255);

			VuTexture *pTexture = mBackgroundImage.getTexture();
			if (hasFocus)
			{
				if (i == mPressedItem)
				{
					pTexture = mBackgroundPressedImage.getTexture();
				}
				else if ((i == mSelectedItem) && VuControlMethodManager::IF()->isGamePadOrKeyboard())
				{
					pTexture = mBackgroundSelectedImage.getTexture();
					backgroundColor = VuGameUtil::IF()->calcHighlightColor(1.0f);
				}
			}

			if (pTexture)
			{
				VuRect rect = mBackgroundRect;
				//rect.scale(mTouchRect.getCenter(), scale);
				VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth + 7 * GFX_SORT_DEPTH_STEP, pTexture, backgroundColor, (rect + offset)*uiDrawParams.mInvAuthScale);
			}
		}

		// draw frame
		{
			VuTexture *pTexture = mFrameImage.getTexture();
			if (hasFocus)
			{
				if (i == mPressedItem)
					pTexture = mFramePressedImage.getTexture();
				else if ((i == mSelectedItem) && VuControlMethodManager::IF()->isGamePadOrKeyboard())
					pTexture = mFrameSelectedImage.getTexture();
			}

			if (pTexture)
			{
				VuRect rect = mFrameRect;
				//rect.scale(mTouchRect.getCenter(), scale);
				VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth - 7 * GFX_SORT_DEPTH_STEP, pTexture, (rect + offset)*uiDrawParams.mInvAuthScale);
			}
		}

		// draw image
		mTempText.clear();
		getItemImage(i, mTempText);
		if (VuTexture *pTexture = getTexture(mTempText))
		{
			VuRect rect = mImageRect;
			rect.scale(mTouchRect.getCenter(), scale);

			if (VuTexture *pMaskTexture = mMaskImage.getTexture())
				VuGfxUtil::IF()->drawMaskedTexture2d(uiDrawParams.mDepth + 6 * GFX_SORT_DEPTH_STEP, pTexture, pMaskTexture, VuColor(255, 255, 255), (rect + offset)*uiDrawParams.mInvAuthScale);
			else
				VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth + 6 * GFX_SORT_DEPTH_STEP, pTexture, (rect + offset)*uiDrawParams.mInvAuthScale);
		}

		if (isItemLocked(i))
		{
			VuRect rect = mLockRect;
			rect.scale(mTouchRect.getCenter(), scale);

			// draw lock image
			if (VuTexture *pTexture = mLockImage.getTexture())
			{
				VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth - 9 * GFX_SORT_DEPTH_STEP, pTexture, (rect + offset)*uiDrawParams.mInvAuthScale);
			}

			// draw unlock text
			{
				mTempText.clear();
				getUnlockText(i, mTempText);
				if (mTempText.length())
				{
					const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mUnlockFont);
					VuFontDrawParams fdParams = fontEntry.params();

					VuRect dstRect = mUnlockRect;
					dstRect.scale(mTouchRect.getCenter(), scale);
					dstRect = (dstRect + offset)*uiDrawParams.mInvAuthScale;
					fdParams.mSize *= scale;

					VuGfxUtil::IF()->fontDraw()->drawString(uiDrawParams.mDepth - 10 * GFX_SORT_DEPTH_STEP, fontEntry.font(), mTempText.c_str(), fdParams, dstRect, mUnlockStringFormat);
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
		if (itemCount >= 2 && mDotSpacing.magSquared() > FLT_EPSILON)
		{
			VuRect dstRect = mDotRect + (i - 0.5f*(itemCount - 1))*mDotSpacing;
			dstRect.scale(dstRect.getCenter(), scale);
			dstRect = uiDrawParams.transform(dstRect);

			VuVector2 imageCenter = (mImageRect.getCenter() + offset)*uiDrawParams.mInvAuthScale;
			imageCenter = VuGfxUtil::IF()->getMatrix().transform(imageCenter);
			if (imageCenter.mX >= 0.0f && imageCenter.mX <= 1.0f)
				VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth, mDotOnImage.getTexture(), dstRect);
			else
				VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth, mDotOffImage.getTexture(), dstRect);

		}

		// allow derived class to draw
		onDrawItem(i, offset, scale);
	}
}

void VuFullLvlCarListEntity::onDrawItem(int index, const VuVector2 &offset, float scale)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	bool locked = isItemLocked(index);

	//draw car image
	std::string &carName = mFullLvlCarNames[index].str;

	VuGameManager::FullLvlCar &car = mFullLvlCar[carName];


	VuRect dstRect = mCarImageRect;
	VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth - 11 * GFX_SORT_DEPTH_STEP, mCarTextures[carName]->getTexture(), (dstRect + offset) *uiDrawParams.mInvAuthScale);

	//drawlabel
	if (mpLabelImage)
	{
		VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth - 12 * GFX_SORT_DEPTH_STEP, mpLabelImage->getTexture(), (mLabelRect + offset)*uiDrawParams.mInvAuthScale);
	}

	char tmpStr[64];

	const VuFontDB::VuEntry &subTitleFontEntry = VuFontDB::IF()->getFont(subTitleFont);
	const VuFontDB::VuEntry &price1FontEntry = VuFontDB::IF()->getFont(priceFont1);
	const VuFontDB::VuEntry &price2FontEntry = VuFontDB::IF()->getFont(priceFont2);
	const VuFontDB::VuEntry &price3fontEntry = VuFontDB::IF()->getFont(priceFont3);
	const VuFontDB::VuEntry &btnTextFontEntry = VuFontDB::IF()->getFont(btnTextFont);
	// draw Buy Button
	if (!car.isOwned)
	{
		VuRect dstRect = mBuyBtnRect;
		//dstRect.scale(mTouchRect.getCenter(), scale);
		VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth - 11 * GFX_SORT_DEPTH_STEP, mpBuyImage->getTexture(), (dstRect + offset) *uiDrawParams.mInvAuthScale);

		VU_SPRINTF(tmpStr, sizeof(tmpStr), "%s", VuStringDB::IF()->getString("Paint_BuyVideo").c_str());
		VuGfxUtil::IF()->fontDraw()->drawString(uiDrawParams.mDepth - 12 * GFX_SORT_DEPTH_STEP, btnTextFontEntry.font(),
			tmpStr, btnTextFontEntry.params(), (mBuyTextRect + offset)*uiDrawParams.mInvAuthScale, mNameStringFormat);
	}
	else
	{
		VuRect dstRect = mBuyBtnRect;
		//dstRect.scale(mTouchRect.getCenter(), scale);
		VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth - 11 * GFX_SORT_DEPTH_STEP, mpOwnedImage->getTexture(), (dstRect + offset) *uiDrawParams.mInvAuthScale);

		VU_SPRINTF(tmpStr, sizeof(tmpStr), "%s", VuStringDB::IF()->getString("IsOwned").c_str());
		VuGfxUtil::IF()->fontDraw()->drawString(uiDrawParams.mDepth - 12 * GFX_SORT_DEPTH_STEP, btnTextFontEntry.font(),
			tmpStr, btnTextFontEntry.params(), (mBuyTextRect + offset)*uiDrawParams.mInvAuthScale, mNameStringFormat);
	}

	//draw subtitle
	VuGfxUtil::IF()->fontDraw()->drawString(uiDrawParams.mDepth - 10 * GFX_SORT_DEPTH_STEP, subTitleFontEntry.font(),
		car.mCarSubTitle.c_str(), subTitleFontEntry.params(), (mCarSubTitleRect + offset)*uiDrawParams.mInvAuthScale, mNameStringFormat);
	
	//draw priceTitle
	VU_SPRINTF(tmpStr, sizeof(tmpStr), "%s%d%s", VuStringDB::IF()->getString("DisCountPrice").c_str(), car.mCurrentPrice, VuStringDB::IF()->getString("PriceUnit").c_str());

	VuGfxUtil::IF()->fontDraw()->drawString(uiDrawParams.mDepth - 13 * GFX_SORT_DEPTH_STEP, price1FontEntry.font(),
		tmpStr, price1FontEntry.params(), (mCurrentPriceRect + offset)*uiDrawParams.mInvAuthScale, mNameStringFormat);

	VU_SPRINTF(tmpStr, sizeof(tmpStr), "%d%s", car.mDiscountInfo, VuStringDB::IF()->getString("Discount").c_str());
	VuGfxUtil::IF()->fontDraw()->drawString(uiDrawParams.mDepth - 13 * GFX_SORT_DEPTH_STEP, price2FontEntry.font(),
		tmpStr, price2FontEntry.params(), (mSaleInfo1Rect + offset)*uiDrawParams.mInvAuthScale, mNameStringFormat);

	VU_SPRINTF(tmpStr, sizeof(tmpStr), "%s%d%s", VuStringDB::IF()->getString("ForSale").c_str(), car.mDiscountPrice, VuStringDB::IF()->getString("PriceUnit").c_str());
	VuGfxUtil::IF()->fontDraw()->drawString(uiDrawParams.mDepth - 13 * GFX_SORT_DEPTH_STEP, price3fontEntry.font(),
		tmpStr, price3fontEntry.params(), (mSaleInfo2Rect + offset)*uiDrawParams.mInvAuthScale, mNameStringFormat);
}

void VuFullLvlCarListEntity::buildList()
{
	mpBuyImage = VuAssetFactory::IF()->createAsset<VuTextureAsset>("UI/FullLvlCar/fullLvl_btn", VuAssetFactory::OPTIONAL_ASSET);
	mpOwnedImage = VuAssetFactory::IF()->createAsset<VuTextureAsset>("UI/FullLvlCar/fullLvl_btn_gray", VuAssetFactory::OPTIONAL_ASSET);
	mpLabelImage = VuAssetFactory::IF()->createAsset<VuTextureAsset>("UI/FullLvlCar/fullLvl_label", VuAssetFactory::OPTIONAL_ASSET);

	if (VuGameManager::IF())
	{
		mFullLvlCar = VuGameManager::IF()->getFullLvlCars();

		std::vector<std::string> mTmpCars = VuGameManager::IF()->getFullLvlCarNames();
		mFullLvlCarNames.clear();
		
		for (auto &carName : mTmpCars)
		{
			mFullLvlCarNames.push_back(myString(carName));
		}

		std::sort(mFullLvlCarNames.begin(), mFullLvlCarNames.end());



		for (auto &carName : mFullLvlCarNames)
		{
			VuTextureAsset* &pCarTextrue = mCarTextures[carName.str];
			pCarTextrue = VuAssetFactory::IF()->createAsset<VuTextureAsset>("UI/FullLvlCar/fullLvl_" + carName.str, VuAssetFactory::OPTIONAL_ASSET);
		}
		
	}
	else
	{
		char *mTmpCars[5] = { "Euro", "MonsterTruck", "Rally", "Moon", "Muscle" };

		char *mTmpCarNames[5] = { "Car_Euro", "Car_MonsterTruck", "Car_Rally", "Car_Moon", "Car_Muscle" };

		for (int i = 0; i < 5; i++)
		{
			mFullLvlCarNames.push_back(myString(mTmpCars[i]));

			

			VuGameManager::FullLvlCar &fulCar = mFullLvlCar[std::string(mTmpCars[i])];
			fulCar.mCarName = mTmpCars[i];
			fulCar.mCarTitle = mTmpCarNames[i];
			fulCar.mCarSubTitle = std::string(VuStringDB::IF()->getString("FullLvl"));
			//fulCar.mpCarImage = VuAssetFactory::IF()->createAsset<VuTextureAsset>("UI/FullLvlCar/fullLvl_" + fulCar.mCarName, VuAssetFactory::OPTIONAL_ASSET);

			VuTextureAsset* &pCarTextrue = mCarTextures[mTmpCars[i]];
			pCarTextrue = VuAssetFactory::IF()->createAsset<VuTextureAsset>("UI/FullLvlCar/fullLvl_" + fulCar.mCarName, VuAssetFactory::OPTIONAL_ASSET);


			if (i == 0)
			{
				fulCar.mCurrentPrice = 25;
				fulCar.mDiscountInfo = 4;
				fulCar.mDiscountPrice = 35;
			}
			else if (i == 1)
			{
				fulCar.mCurrentPrice = 30;
				fulCar.mDiscountInfo = 4;
				fulCar.mDiscountPrice = 50;
			}
			else if (i == 2)
			{
				fulCar.mCurrentPrice = 38;
				fulCar.mDiscountInfo = 4;
				fulCar.mDiscountPrice = 62;
			}
			else if (i == 3)
			{
				fulCar.mCurrentPrice = 68;
				fulCar.mDiscountInfo = 3;
				fulCar.mDiscountPrice = 152;
			}
			else
			{
				fulCar.mCurrentPrice = 60;
				fulCar.mDiscountInfo = 3;
				fulCar.mDiscountPrice = 140;
			}
		}
	}
	
}