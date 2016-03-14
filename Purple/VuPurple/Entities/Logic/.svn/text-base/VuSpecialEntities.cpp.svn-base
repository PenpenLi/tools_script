//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Special entities
// 
//*****************************************************************************

#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Managers/VuCloudTuningManager.h"
#include "VuPurple/Managers/VuBillingManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuPurple/Util/VuGameFontMacros.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Components/2dLayout/Vu2dLayoutComponent.h"
#include "VuEngine/Components/VuTransitionComponent.h"
#include "VuEngine/Properties/VuDBEntryProperty.h"
#include "VuEngine/Assets/VuTextureAsset.h"
#include "VuEngine/DB/VuStringDB.h"
#include "VuEngine/UI/VuUIPropertyUtil.h"
#include "VuEngine/UI/VuUIDrawUtil.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Util/VuStringUtil.h"


//*****************************************************************************
// VuCloudSpecialEntity
//*****************************************************************************
class VuCloudSpecialEntity : public VuEntity
{
	DECLARE_RTTI
public:
	VuCloudSpecialEntity();

private:
	VuRetVal			Begin(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;

	// properties
	std::string			mSpecial;
	std::string			mDuration;
};

IMPLEMENT_RTTI(VuCloudSpecialEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuCloudSpecialEntity);


//*****************************************************************************
VuCloudSpecialEntity::VuCloudSpecialEntity()
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	// properties
	addProperty(new VuDBEntryProperty("Special", mSpecial, "SpecialDB"));
	addProperty(new VuDBEntryProperty("Duration", mDuration, "TuningDB"));

	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuCloudSpecialEntity, Begin);
}

//*****************************************************************************
VuRetVal VuCloudSpecialEntity::Begin(const VuParams &params)
{
	float duration = VuCloudTuningManager::IF()->tuningDB()[mDuration].asFloat();

	VuGameManager::IF()->beginSpecial(mSpecial, duration);

	return VuRetVal();
}


//*****************************************************************************
// VuSpecialTimerEntity
//*****************************************************************************
class VuSpecialTimerEntity : public VuEntity
{
	DECLARE_RTTI
public:
	VuSpecialTimerEntity();

	virtual void		onGameInitialize();

private:
	// event handlers
	void				OnUITick(const VuParams &params);

	void				updateMacros();

	// properties
	std::string			mSpecial;
};

IMPLEMENT_RTTI(VuSpecialTimerEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuSpecialTimerEntity);


//*****************************************************************************
VuSpecialTimerEntity::VuSpecialTimerEntity()
{
	// properties
	addProperty(new VuDBEntryProperty("Special", mSpecial, "SpecialDB"));

	REG_EVENT_HANDLER(VuSpecialTimerEntity, OnUITick);
}

//*****************************************************************************
void VuSpecialTimerEntity::onGameInitialize()
{
	updateMacros();
}

//*****************************************************************************
void VuSpecialTimerEntity::OnUITick(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	float fdt = accessor.getFloat();

	updateMacros();
}

//*****************************************************************************
void VuSpecialTimerEntity::updateMacros()
{
	VUINT64 timeRemaining = 0;

	VuGameManager::Specials::const_iterator iter = VuGameManager::IF()->activeSpecials().find(mSpecial);
	if ( iter != VuGameManager::IF()->activeSpecials().end() )
	{
		VUINT64 currentTime = VuTimeUtil::calcSecondsSince2000();
		if ( currentTime < iter->second.mExpirationTime )
			timeRemaining = iter->second.mExpirationTime - currentTime;
	}

	char str[256];
	VuStringUtil::timeFormatSeconds(VuStringUtil::HH_MM_SS, (int)timeRemaining, str, sizeof(str));
	VuGameFontMacros::IF()->setMacro("SPECIAL_TIME_REMAINING", str);
}


//*****************************************************************************
// VuCheckSpecialsEntity
//*****************************************************************************
class VuCheckSpecialsEntity : public VuEntity
{
	DECLARE_RTTI
public:
	VuCheckSpecialsEntity();

private:
	VuRetVal			AreAvailable(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuCheckSpecialsEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuCheckSpecialsEntity);


//*****************************************************************************
VuCheckSpecialsEntity::VuCheckSpecialsEntity()
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	ADD_SCRIPT_INPUT(mpScriptComponent, VuCheckSpecialsEntity, AreAvailable, VuRetVal::Bool, VuParamDecl());
}

//*****************************************************************************
VuRetVal VuCheckSpecialsEntity::AreAvailable(const VuParams &params)
{
	bool areAvailable = false;

	if ( VuGameManager::IF()->activeSpecials().size() )
	{
		VUINT64 currentTime = VuTimeUtil::calcSecondsSince2000();

		for ( const auto &iter : VuGameManager::IF()->activeSpecials() )
			if ( currentTime < iter.second.mExpirationTime )
				areAvailable = true;
	}

	return VuRetVal(areAvailable);
}


//*****************************************************************************
// VuCheckSpecialEntity
//*****************************************************************************
class VuCheckSpecialEntity : public VuEntity
{
	DECLARE_RTTI
public:
	VuCheckSpecialEntity();

private:
	VuRetVal			IsAvailable(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;

	// properties
	std::string			mSpecial;
};

IMPLEMENT_RTTI(VuCheckSpecialEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuCheckSpecialEntity);


//*****************************************************************************
VuCheckSpecialEntity::VuCheckSpecialEntity()
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	// properties
	addProperty(new VuDBEntryProperty("Special", mSpecial, "SpecialDB"));

	ADD_SCRIPT_INPUT(mpScriptComponent, VuCheckSpecialEntity, IsAvailable, VuRetVal::Bool, VuParamDecl());
}

//*****************************************************************************
VuRetVal VuCheckSpecialEntity::IsAvailable(const VuParams &params)
{
	VuGameManager::Specials::const_iterator iter = VuGameManager::IF()->activeSpecials().find(mSpecial);
	if ( iter != VuGameManager::IF()->activeSpecials().end() )
	{
		VUINT64 currentTime = VuTimeUtil::calcSecondsSince2000();
		if ( currentTime < iter->second.mExpirationTime )
			return VuRetVal(true);
	}

	const VuJsonContainer &permanentSpecials = VuGameUtil::IF()->constantDB()["PermanentSpecials"];
	for ( int i = 0; i < permanentSpecials.size(); i++ )
		if ( permanentSpecials[i].asString() == mSpecial )
			return VuRetVal(true);

	return VuRetVal(false);
}


class VuSpecialsProcessor :public VuMessageBox::Callback
{
public:
	virtual void onMessageBoxClosed(VuMessageBox *pMessageBox);
	void setItemName(std::string itemName) { m_itemName = itemName; }
protected:
	std::string m_itemName;
};

void VuSpecialsProcessor::onMessageBoxClosed(VuMessageBox *pMessageBox)
{
	if (pMessageBox->getResult() != "OnA")
	{
		if (VuBillingManager::IF()->getCurrentBillingType() == VuBillingManager::WEIXIN)
		{
			VuRequestOrderManager::IF()->setDelegate(VuBillingManager::IF());
			VuRequestOrderManager::IF()->startGetOrderId(m_itemName);
		}
		else
		{
			VuBillingManager::IF()->startPurchase(m_itemName);
		}
	}
		
}

//*****************************************************************************
// VuSpecialsEntity
//*****************************************************************************
class VuSpecialsEntity : public VuEntity
{
	DECLARE_RTTI
public:
	VuSpecialsEntity();
	~VuSpecialsEntity();
	virtual void		onGameInitialize();
	virtual void		onGameRelease();

private:
	// scripting
	VuRetVal			Show(const VuParams &params) { mHidden = false; return VuRetVal(); }
	VuRetVal			Hide(const VuParams &params) { mHidden = true; return VuRetVal(); }
	VuRetVal			AreAvailable(const VuParams &params) { return VuRetVal(mSpecials.size() > 0); }
	VuRetVal			HavePrev(const VuParams &params) { return VuRetVal(mCurIndex > 0); }
	VuRetVal			HaveNext(const VuParams &params) { return VuRetVal(mCurIndex + 1 < (int)mSpecials.size()); }
	VuRetVal			SetPrev(const VuParams &params) { mCurIndex = VuMax(mCurIndex - 1, 0); return VuRetVal(); }
	VuRetVal			SetNext(const VuParams &params) { mCurIndex = VuMin(mCurIndex + 1, (int)mSpecials.size() - 1); return VuRetVal(); }
	VuRetVal			StartPurchase(const VuParams &params);

	// event handlers
	void				OnUITick(const VuParams &params);
	void				OnUIDraw(const VuParams &params);
	void				OnPurchaseMade(const VuParams &params);

	void				updateMacros();

	void				drawLayout(bool bSelected);
	void				drawDots(const VuUIDrawParams &uiDrawParams, int index, int count, const VuColor &color);

	// components
	VuScriptComponent		*mpScriptComponent;
	VuTransitionComponent	*mpTransitionComponent;

	// properties
	bool				mInitiallyHidden;
	VuUIRectProperties	mImageRect;
	VuUIRectProperties	mDescImageRect;
	VuUIRectProperties	mDotRect;
	VuVector2			mDotSpacing;
	VuUIImageProperties	mDotOnImage;
	VuUIImageProperties	mDotOffImage;

	struct Special
	{
		std::string		mName;
		std::string		mStoreItemName;
		VUINT64			mExpirationTime;
		VuTextureAsset	*mpImage;
		VuTextureAsset  *mpDescImage;
	};
	typedef std::deque<Special> Specials;

	Specials			mSpecials;
	int					mCurIndex;
	bool				mHidden;
	VuSpecialsProcessor* m_specialProcessor;
};

IMPLEMENT_RTTI(VuSpecialsEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuSpecialsEntity);


//*****************************************************************************
VuSpecialsEntity::VuSpecialsEntity():
	mInitiallyHidden(false),
	mDotSpacing(0,0),
	mCurIndex(0),
	mHidden(false)
{

	//init processors
	m_specialProcessor = new VuSpecialsProcessor;
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));
	addComponent(new Vu2dLayoutComponent(this, &VuSpecialsEntity::drawLayout));
	addComponent(mpTransitionComponent = new VuTransitionComponent(this));

	// properties
	addProperty(new VuBoolProperty("Hidden", mInitiallyHidden));
	addProperty(new VuRectProperty("Image Rect", mImageRect));
	addProperty(new VuRectProperty("DescImage Rect", mDescImageRect));
	addProperty(new VuRectProperty("Dot Rect", mDotRect));
	addProperty(new VuVector2Property("Dot Spacing", mDotSpacing));
	mDotOnImage.addProperties(getProperties(), "Dot On Image");
	mDotOffImage.addProperties(getProperties(), "Dot Off Image");

	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuSpecialsEntity, Show);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuSpecialsEntity, Hide);
	ADD_SCRIPT_INPUT(mpScriptComponent, VuSpecialsEntity, AreAvailable, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT(mpScriptComponent, VuSpecialsEntity, HavePrev, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT(mpScriptComponent, VuSpecialsEntity, HaveNext, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuSpecialsEntity, SetPrev);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuSpecialsEntity, SetNext);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuSpecialsEntity, StartPurchase);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnSuccess);
}
//*****************************************************************************
VuSpecialsEntity::~VuSpecialsEntity()
{
	if (m_specialProcessor != NULL)
	{
		delete m_specialProcessor;
		m_specialProcessor = NULL;
	}
}
//*****************************************************************************
void VuSpecialsEntity::onGameInitialize()
{
	// add active specials
	VUINT64 currentTime = VuTimeUtil::calcSecondsSince2000();
	for ( const auto &iter : VuGameManager::IF()->activeSpecials() )
	{
		if ( currentTime < iter.second.mExpirationTime )
		{
			const VuJsonContainer &specialData = VuGameUtil::IF()->specialDB()[iter.first];

			mSpecials.resize(mSpecials.size() + 1);
			Special &special = mSpecials.back();

			special.mName = iter.first;
			special.mStoreItemName = specialData["StoreItem"].asString();
			special.mExpirationTime = iter.second.mExpirationTime;
			special.mpImage = VuAssetFactory::IF()->createAsset<VuTextureAsset>(specialData["Image"].asString(), VuAssetFactory::OPTIONAL_ASSET);
			
		}
	}

	// add permanent specials
	const VuJsonContainer &permanentSpecials = VuGameUtil::IF()->constantDB()["PermanentSpecials"];
	for ( int i = 0; i < permanentSpecials.size(); i++ )
	{
		const std::string &specialName = permanentSpecials[i].asString();
		const VuJsonContainer &specialData = VuGameUtil::IF()->specialDB()[specialName];

		const std::string &storeItemName = specialData["StoreItem"].asString();
		const std::string &theThing = specialData["HideIfUnlocked"].asString();

		if ( !VuBillingManager::IF()->isOwned(storeItemName.c_str()) )
		{
			if ( theThing.empty() || !VuBillingManager::IF()->isUnlocked(theThing.c_str()) )
			{
				mSpecials.resize(mSpecials.size() + 1);
				Special &special = mSpecials.back();

				special.mName = specialName;
				special.mStoreItemName = storeItemName;
				special.mExpirationTime = 0;
				special.mpImage = VuAssetFactory::IF()->createAsset<VuTextureAsset>(specialData["Image"].asString(), VuAssetFactory::OPTIONAL_ASSET);
				special.mpDescImage = VuAssetFactory::IF()->createAsset<VuTextureAsset>(specialData["DescImage"].asString(), VuAssetFactory::OPTIONAL_ASSET);
			}
		}
	}

	if ( mSpecials.size() )
	{
		// event handlers
		REG_EVENT_HANDLER(VuSpecialsEntity, OnUITick);
		REG_EVENT_HANDLER(VuSpecialsEntity, OnUIDraw);
		REG_EVENT_HANDLER(VuSpecialsEntity, OnPurchaseMade);
	}

	mHidden = mInitiallyHidden;

	updateMacros();
}

//*****************************************************************************
void VuSpecialsEntity::onGameRelease()
{
	mEventMap.unregisterHandlers();

	for (const auto &special : mSpecials)
	{
		VuAssetFactory::IF()->releaseAsset(special.mpImage);
		VuAssetFactory::IF()->releaseAsset(special.mpDescImage);
	}
		
	mSpecials.clear();
}

//*****************************************************************************
VuRetVal VuSpecialsEntity::StartPurchase(const VuParams &params)
{
	if (!VuBillingManager::IF()->isBusy())
	{
		const VuJsonContainer &itemData = VuDataUtil::findArrayMember(VuGameUtil::IF()->storeDB(), "Name", mSpecials[mCurIndex].mStoreItemName);
		std::string strNeedMoney = itemData["StubPrice"].asString();
		float needMoney = VuStringUtil::readFloat(strNeedMoney.c_str());
		VuBillingManager::IF()->startPurchaseInternal(mSpecials[mCurIndex].mStoreItemName, needMoney);



		/*VuMessageBoxParams messageBoxParams;
		messageBoxParams.mType = "Billing";
		messageBoxParams.mHeading = "test";
		messageBoxParams.mTextA = "test";
		messageBoxParams.mBody = "test";
		messageBoxParams.mPauseGame = false;

		VuBillingManager::IF()->setCurrentClickItem(mSpecials[mCurIndex].mStoreItemName);
		m_specialProcessor->setItemName(mSpecials[mCurIndex].mStoreItemName);
		VuMessageBox *pMB = VuMessageBoxManager::IF()->create(messageBoxParams);

		pMB->setCallback(m_specialProcessor);
		pMB->removeRef();*/
	}

	return VuRetVal();
}

//*****************************************************************************
void VuSpecialsEntity::OnUITick(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	float fdt = accessor.getFloat();

	mpTransitionComponent->tick(fdt);

	updateMacros();
}

//*****************************************************************************
void VuSpecialsEntity::OnUIDraw(const VuParams &params)
{
	if ( !mHidden )
	{
		VuUIDrawParams uiDrawParams;
		VuUIDrawUtil::getParams(this, uiDrawParams);

		float alpha = mpTransitionComponent->alpha();
		VuColor color(255,255,255,VuRound(255*alpha));

		if ( VuTextureAsset *pImage = mSpecials[mCurIndex].mpImage )
		{
			VuRect dstRect = uiDrawParams.transform(mImageRect);
			VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth, pImage->getTexture(), color, dstRect);
		}

		if (VuTextureAsset *pImage1 = mSpecials[mCurIndex].mpDescImage)
		{
			VuRect dstRect = uiDrawParams.transform(mDescImageRect);
			VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth, pImage1->getTexture(), color, dstRect);
		}

		drawDots(uiDrawParams, mCurIndex, (int)mSpecials.size(), color);
	}
}

//*****************************************************************************
void VuSpecialsEntity::OnPurchaseMade(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	const char *itemName = accessor.getString();

	for ( const auto &special : mSpecials )
		if ( special.mStoreItemName.compare(itemName) == 0 )
			mpScriptComponent->getPlug("OnSuccess")->execute();
}

//*****************************************************************************
void VuSpecialsEntity::updateMacros()
{
	if ( mSpecials.size() )
	{
		const Special &special = mSpecials[mCurIndex];

		char str[256];

		VU_SPRINTF(str, sizeof(str), "StoreItem_%s", special.mStoreItemName.c_str());
		VuGameFontMacros::IF()->setMacro("SPECIAL_NAME", VuStringDB::IF()->getString(str).c_str());

		VU_SPRINTF(str, sizeof(str), "StoreItem_%s_Desc", special.mStoreItemName.c_str());
		VuGameFontMacros::IF()->setMacro("SPECIAL_DESC", VuStringDB::IF()->getString(str).c_str());

		VU_SPRINTF(str, sizeof(str), "Special_%s_Timer", special.mStoreItemName.c_str());
		VuGameFontMacros::IF()->setMacro("SPECIAL_TIMER", VuStringDB::IF()->getString(str).c_str());

		VuGameFontMacros::IF()->setMacro("SPECIAL_PRICE", VuBillingManager::IF()->getPrice(special.mStoreItemName).c_str());

		if ( special.mExpirationTime )
		{
			VUINT64 currentTime = VuTimeUtil::calcSecondsSince2000();
			VUINT64 timeRemaining = special.mExpirationTime - currentTime;
			if ( timeRemaining < 0 )
				timeRemaining = 0;
			VuStringUtil::timeFormatSeconds(VuStringUtil::HH_MM_SS, (int)timeRemaining, str, sizeof(str));
			VuGameFontMacros::IF()->setMacro("SPECIAL_TIME_REMAINING", str);
		}
		else
		{
			VuGameFontMacros::IF()->setMacro("SPECIAL_TIME_REMAINING", "");
		}
	}
}

//*****************************************************************************
void VuSpecialsEntity::drawLayout(bool bSelected)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	VuRect dstRect = uiDrawParams.transform(mImageRect);
	VuGfxUtil::IF()->drawRectangleOutline2d(0.0f, VuColor(255,255,255), dstRect);

	VuRect dstRect1 = uiDrawParams.transform(mDescImageRect);
	VuGfxUtil::IF()->drawRectangleOutline2d(0.0f, VuColor(255, 255, 255), dstRect1);

	drawDots(uiDrawParams, 0, 4, VuColor(255,255,255));
}

//*****************************************************************************
void VuSpecialsEntity::drawDots(const VuUIDrawParams &uiDrawParams, int index, int count, const VuColor &color)
{
	if ( count >= 2 && mDotSpacing.magSquared() > FLT_EPSILON )
	{
		for ( int i = 0; i < count; i++ )
		{
			VuRect dstRect = mDotRect + (i - 0.5f*(count - 1))*mDotSpacing;
			dstRect = uiDrawParams.transform(dstRect);

			if ( i == index )
				VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth, mDotOnImage.getTexture(), color, dstRect);
			else
				VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth, mDotOffImage.getTexture(), color, dstRect);
		}
	}
}
