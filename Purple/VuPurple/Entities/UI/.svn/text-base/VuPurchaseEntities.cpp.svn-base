//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  purchase entities
// 
//*****************************************************************************

#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Managers/VuCloudTuningManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Assets/VuSpreadsheetAsset.h"
#include "VuEngine/Dev/VuDevConfig.h"


#include "VuPurple/Managers/VuBillingManager.h"

#include "VuEngine/Properties/VuBasicProperty.h"
#include "VuEngine/Properties/VuStringProperty.h"

#include "VuEngine/Managers/VuProfileManager.h"

#include "VuPurple/Managers/VuCloudSaveManager.h"

#include "VuEngine/Util/VuSpreadsheetQuery.h"
#include "VuPurple/Managers/VuAnalyticsManager.h"
#include "VuEngine/Util/VuStringUtil.h"
#include "VuPurple/Managers/VuGameManager.h"

//*****************************************************************************
// PurchaseCar
//*****************************************************************************

class VuPurchaseCarEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuPurchaseCarEntity();

protected:
	VuRetVal			IsOwned(const VuParams &params);
	VuRetVal			CanAfford(const VuParams &params);
	VuRetVal			Purchase(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuPurchaseCarEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuPurchaseCarEntity);

//*****************************************************************************
VuPurchaseCarEntity::VuPurchaseCarEntity()
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	// scripting
	ADD_SCRIPT_INPUT(mpScriptComponent, VuPurchaseCarEntity, IsOwned, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT(mpScriptComponent, VuPurchaseCarEntity, CanAfford, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuPurchaseCarEntity, Purchase);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnSuccess);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnFailure);
}

//*****************************************************************************
VuRetVal VuPurchaseCarEntity::IsOwned(const VuParams &params)
{
	const std::string &selectedCar = VuGameUtil::IF()->getSelectedCar();
	bool isOwned = VuGameManager::IF()->getCar(selectedCar).mIsOwned;

	return VuRetVal(isOwned);
}

//*****************************************************************************
VuRetVal VuPurchaseCarEntity::CanAfford(const VuParams &params)
{
	bool canAfford = false;

	const std::string &selectedCar = VuGameUtil::IF()->getSelectedCar();
	int priceType = VuGameUtil::IF()->getCarPriceType(selectedCar);
	int price = VuGameUtil::IF()->getCarPrice(selectedCar);
	if (priceType == 1) // 金币
	{
		if (VuGameManager::IF()->getCurrency(VuGameManager::CT_STANDARD) >= price)
			canAfford = true;
	}
	else if (priceType == 3) //钻石
	{
		if (VuGameManager::IF()->getCurrency(VuGameManager::CT_PREMIUM) >= price)
			canAfford = true;
	}
	else  // 人民币；
	{
		;
	}

	return VuRetVal(canAfford);
}

//*****************************************************************************
VuRetVal VuPurchaseCarEntity::Purchase(const VuParams &params)
{
	const std::string &selectedCar = VuGameUtil::IF()->getSelectedCar();

	if (VuGameManager::IF()->purchaseCar(selectedCar))
	{
		mpScriptComponent->getPlug("OnSuccess")->execute();
		VuAnalyticsManager::IF()->logEvent("Buy car", selectedCar.c_str(), 1);
	}	
	else
		mpScriptComponent->getPlug("OnFailure")->execute();

	return VuRetVal();
}


//*****************************************************************************
// IsRmbPurchaseChar， 主要用来判断是不是需要rmb才能购买的车 xlm 2014/11/29
//*****************************************************************************
class VuIsRmbPurchaseCarEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuIsRmbPurchaseCarEntity();

protected:
	/*!
 	* \author: xlm 2014/11/29  
 	* \brief: is a game coin item or rmb item
 	*/
	//VuRetVal			IsRmbPurchase(const VuParams &params); //return a bool value to the VuIfEntity
	
	VuRetVal CheckCarPurchaseType(const VuParams &params);
	// components
	VuScriptComponent	*mpScriptComponent;
};


IMPLEMENT_RTTI(VuIsRmbPurchaseCarEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuIsRmbPurchaseCarEntity);


//*****************************************************************************
VuIsRmbPurchaseCarEntity::VuIsRmbPurchaseCarEntity()
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	// scripting
	/*ADD_SCRIPT_INPUT(mpScriptComponent, VuIsRmbPurchaseCarEntity, IsRmbPurchase, VuRetVal::Bool, VuParamDecl());*/
	ADD_SCRIPT_INPUT(mpScriptComponent, VuIsRmbPurchaseCarEntity, CheckCarPurchaseType, VuRetVal::Void, VuParamDecl());
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, RMB);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, Diamond);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, Coin);
}

//*****************************************************************************
//VuRetVal VuIsRmbPurchaseCarEntity::IsRmbPurchase(const VuParams &params)
//{
//	bool isRmbItem = false;
//
//	const std::string &selectedCar = VuGameUtil::IF()->getSelectedCar();
//
//	//
//	int priceType = VuGameUtil::IF()->getCarPriceType(selectedCar);
//
//	if (priceType == 2)  //must use rmb to purchase
//	{
//		isRmbItem = true;
//	}
//	else   //use game coin to purchase
//		isRmbItem = false;
//	return VuRetVal(isRmbItem);
//}


VuRetVal VuIsRmbPurchaseCarEntity::CheckCarPurchaseType(const VuParams &params)
{
	const std::string &selectedCar = VuGameUtil::IF()->getSelectedCar();
	int priceType = VuGameUtil::IF()->getCarPriceType(selectedCar);
	if (priceType == 1)
	{
		mpScriptComponent->getPlug("Coin")->execute(params);
	}
	else if (priceType == 2)
	{
		mpScriptComponent->getPlug("RMB")->execute(params);
	}
	else if (priceType == 3)
	{
		mpScriptComponent->getPlug("Diamond")->execute(params);
	}

	return VuRetVal();
}


/*人民币购买汽车*/
//*****************************************************************************
// 人民币购买汽车
//*****************************************************************************
class VuRMBPurchaseCarProcessor :public VuMessageBox::Callback
{
public:
	virtual void onMessageBoxClosed(VuMessageBox *pMessageBox);
	std::string getItemName()const { return m_itemName; }
	void setItemName(std::string itemName) { m_itemName = itemName; }

protected:
	std::string m_itemName;
};

void VuRMBPurchaseCarProcessor::onMessageBoxClosed(VuMessageBox *pMessageBox)
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




class VuRMBPurchaseCarEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuRMBPurchaseCarEntity();
	~VuRMBPurchaseCarEntity();
	virtual void		onGameInitialize();
	virtual void		onGameRelease();

protected:
	// scripting
	VuRetVal			StartPurchase(const VuParams &params);

	VuRetVal			IsOwned(const VuParams &params);

	//modify by liudan  判断 用户是否已经购买 该fullLvlCar 对应的车；
	VuRetVal			IsOwnCurCar(const VuParams &params);

	// event handlers
	void				OnPurchaseMade(const VuParams &params);

	VuRetVal			StartFullLvlCarPurchase(const VuParams &params);
	VuRetVal			StartCurFuLvlCarPurchase(const VuParams &params);
	// components
	VuScriptComponent	*mpScriptComponent;

	// properties
	std::string			mItemName; //当前正在购买的车的商品名称

    //modify by liudan   2015/7/16  17:29
	std::string			mFullLvlCar; //满级车辆的名称
	VuRMBPurchaseCarProcessor* m_processor;
};

IMPLEMENT_RTTI(VuRMBPurchaseCarEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuRMBPurchaseCarEntity);


//*****************************************************************************
VuRMBPurchaseCarEntity::VuRMBPurchaseCarEntity()
{
	m_processor = new VuRMBPurchaseCarProcessor;
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));
/*

	const std::string &selectedCar = VuGameUtil::IF()->getSelectedCar();

	std::string itemName = selectedCar;*/
	// properties
	addProperty(new VuStringProperty("FullLvl Car", mFullLvlCar));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuRMBPurchaseCarEntity, StartPurchase);
	
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuRMBPurchaseCarEntity, StartFullLvlCarPurchase);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuRMBPurchaseCarEntity, StartCurFuLvlCarPurchase);
	ADD_SCRIPT_INPUT(mpScriptComponent, VuRMBPurchaseCarEntity, IsOwned, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT(mpScriptComponent, VuRMBPurchaseCarEntity, IsOwnCurCar, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnSuccess);
}
//*****************************************************************************
VuRMBPurchaseCarEntity::~VuRMBPurchaseCarEntity()
{
	if (m_processor)
	{
		delete m_processor;
		m_processor = NULL;
	}
}

//*****************************************************************************
void VuRMBPurchaseCarEntity::onGameInitialize()
{
	// event handlers
	REG_EVENT_HANDLER(VuRMBPurchaseCarEntity, OnPurchaseMade);
}

//*****************************************************************************
void VuRMBPurchaseCarEntity::onGameRelease()
{
	mEventMap.unregisterHandlers();
}

//*****************************************************************************
VuRetVal VuRMBPurchaseCarEntity::StartPurchase(const VuParams &params)
{
	/*
	#if defined VUANDROID
	if ( !VuBillingManager::IF()->isBusy() )
	{
	VuBillingManager::IF()->startPurchase(mItemName);
	}

	#elif defined VUIOS

	#elif defined VUWIN32 || defined VUWINSTORE || defined VUWINPHONE
	if (!VuBillingManager::IF()->isBusy())
	{
	VuBillingManager::IF()->startPurchase(mItemName);
	}

	#endif

	return VuRetVal();*/

	//原版


	if (!VuBillingManager::IF()->isBusy())
	{
		
	

		const std::string &selectedCar = VuGameUtil::IF()->getSelectedCar();
		mItemName = selectedCar;
		//VuBillingManager::IF()->setCurrentClickItem(mItemName);

		const VuJsonContainer &itemData = VuDataUtil::findArrayMember(VuGameUtil::IF()->storeDB(), "Name", mItemName);
		std::string strNeedMoney = itemData["StubPrice"].asString();
		float needMoney = VuStringUtil::readFloat(strNeedMoney.c_str());
		VuBillingManager::IF()->startPurchaseInternal(mItemName, needMoney);


		//VuMessageBoxParams messageBoxParams;
		//messageBoxParams.mType = "Billing";
		//messageBoxParams.mHeading = "test";
		//messageBoxParams.mTextA = "test";
		//messageBoxParams.mBody = "test";
		//messageBoxParams.mPauseGame = false;

		////messageBoxParams.mUserData = (nCoinTotal << 16) + nDiamTotal;

		//VuMessageBox *pMB = VuMessageBoxManager::IF()->create(messageBoxParams);
		//m_processor->setItemName(mItemName);
		//pMB->setCallback(m_processor);
		//pMB->removeRef();
	}

	return VuRetVal();
}

VuRetVal VuRMBPurchaseCarEntity::StartFullLvlCarPurchase(const VuParams &params)
{
	if (!VuBillingManager::IF()->isBusy())
	{
		const std::string &selectedCar = VuGameUtil::IF()->getSelectedCar();
		mItemName = "FullLvl" + selectedCar;

		VuBillingManager::IF()->setCurrentClickItem(mItemName);
		VuMessageBoxParams messageBoxParams;
		messageBoxParams.mType = "Billing";
		messageBoxParams.mHeading = "test";
		messageBoxParams.mTextA = "test";
		messageBoxParams.mBody = "test";
		messageBoxParams.mPauseGame = false;

		//messageBoxParams.mUserData = (nCoinTotal << 16) + nDiamTotal;

		VuMessageBox *pMB = VuMessageBoxManager::IF()->create(messageBoxParams);
		m_processor->setItemName(mItemName);
		pMB->setCallback(m_processor);
		pMB->removeRef();
		//VuBillingManager::IF()->startPurchase(mItemName);
	}

	return VuRetVal();
}


VuRetVal	VuRMBPurchaseCarEntity::StartCurFuLvlCarPurchase(const VuParams &params)
{
	if (!VuBillingManager::IF()->isBusy())
	{
		mItemName = "FullLvl" + VuGameManager::IF()->getPurchaseFullLvlCarName();
		
		VuBillingManager::IF()->setCurrentClickItem(mItemName);

		VuMessageBoxParams messageBoxParams;
		messageBoxParams.mType = "Billing";
		messageBoxParams.mHeading = "test";
		messageBoxParams.mTextA = "test";
		messageBoxParams.mBody = "test";
		messageBoxParams.mPauseGame = false;

		//messageBoxParams.mUserData = (nCoinTotal << 16) + nDiamTotal;

		VuMessageBox *pMB = VuMessageBoxManager::IF()->create(messageBoxParams);
		m_processor->setItemName(mItemName);
		pMB->setCallback(m_processor);
		pMB->removeRef();		
		//VuBillingManager::IF()->startPurchase(mItemName);
	}

	return VuRetVal();
}


//*****************************************************************************
VuRetVal VuRMBPurchaseCarEntity::IsOwned(const VuParams &params)
{
	bool isOwned = VuBillingManager::IF()->isOwned(mItemName.c_str());

	return VuRetVal(isOwned);
}

VuRetVal VuRMBPurchaseCarEntity::IsOwnCurCar(const VuParams &params)
{
	bool isOwned = VuGameManager::IF()->getCar(mFullLvlCar).mIsOwned;

	return VuRetVal(isOwned);
}


//*****************************************************************************
void VuRMBPurchaseCarEntity::OnPurchaseMade(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	const char *itemName = accessor.getString();

	if (mItemName.compare(itemName) == 0)
	{
		mpScriptComponent->getPlug("OnSuccess")->execute();
		VuAnalyticsManager::IF()->logEvent("Buy car", itemName, 1);
	}
}












//*****************************************************************************
// PurchaseUpgradedCar
//*****************************************************************************

class VuPurchaseUpgradedCarEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuPurchaseUpgradedCarEntity();

protected:
	VuRetVal			MakesSense(const VuParams &params);
	VuRetVal			CanAfford(const VuParams &params);
	VuRetVal			Purchase(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuPurchaseUpgradedCarEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuPurchaseUpgradedCarEntity);

//*****************************************************************************
VuPurchaseUpgradedCarEntity::VuPurchaseUpgradedCarEntity()
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	// scripting
	ADD_SCRIPT_INPUT(mpScriptComponent, VuPurchaseUpgradedCarEntity, MakesSense, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT(mpScriptComponent, VuPurchaseUpgradedCarEntity, CanAfford, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuPurchaseUpgradedCarEntity, Purchase);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnSuccess);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnFailure);
}

//*****************************************************************************
VuRetVal VuPurchaseUpgradedCarEntity::MakesSense(const VuParams &params)
{
	const std::string &selectedCar = VuGameUtil::IF()->getSelectedCar();

	bool makesSense = false;

	const VuGameManager::Car &car = VuGameManager::IF()->getCar(selectedCar);
	if ( car.mIsOwned && (car.mStage == 0) && (VuGameManager::IF()->getUnlockedStage() > 0) )
		makesSense = true;

	return VuRetVal(makesSense);
}

//*****************************************************************************
VuRetVal VuPurchaseUpgradedCarEntity::CanAfford(const VuParams &params)
{
	bool canAfford = false;

	const std::string &selectedCar = VuGameUtil::IF()->getSelectedCar();

	int price = VuGameUtil::IF()->getUpgradedCarPrice(selectedCar);

	if ( VuGameManager::IF()->getCurrency(VuGameManager::CT_STANDARD) >= price )
		canAfford = true;

	return VuRetVal(canAfford);
}

//*****************************************************************************
VuRetVal VuPurchaseUpgradedCarEntity::Purchase(const VuParams &params)
{
	const std::string &selectedCar = VuGameUtil::IF()->getSelectedCar();

	if (VuGameManager::IF()->purchaseUpgradedCar(selectedCar))
	{
		VuAnalyticsManager::IF()->logEvent("Buy car", selectedCar.c_str(), 1);
		mpScriptComponent->getPlug("OnSuccess")->execute();
	}
	else
		mpScriptComponent->getPlug("OnFailure")->execute();

	return VuRetVal();
}




//*****************************************************************************
// PurchaseUpgrade
//*****************************************************************************

class VuPurchaseUpgradeEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuPurchaseUpgradeEntity();

protected:
	VuRetVal			IsMaxed(const VuParams &params);
	VuRetVal			CanAfford(const VuParams &params);
	VuRetVal			Purchase(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;

	// properties
	std::string			mStat;
};

IMPLEMENT_RTTI(VuPurchaseUpgradeEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuPurchaseUpgradeEntity);

//*****************************************************************************
VuPurchaseUpgradeEntity::VuPurchaseUpgradeEntity()
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	// properties
	static VuStaticStringEnumProperty::Choice sStatChoices[] = { "Accel", "Speed", "Handling", "Tough", VUNULL };
	addProperty(new VuStaticStringEnumProperty("Stat", mStat, sStatChoices));

	// scripting
	ADD_SCRIPT_INPUT(mpScriptComponent, VuPurchaseUpgradeEntity, IsMaxed, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT(mpScriptComponent, VuPurchaseUpgradeEntity, CanAfford, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuPurchaseUpgradeEntity, Purchase);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnSuccess);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnFailure);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnMaxed);
}

//*****************************************************************************
VuRetVal VuPurchaseUpgradeEntity::IsMaxed(const VuParams &params)
{
	bool isMaxed = false;

	const VuGameManager::Car &car = VuGameManager::IF()->getCurCar();

	int level = car.getStat(mStat.c_str());
	if ( level >= car.getMaxLevel() )
		isMaxed = true;

	return VuRetVal(isMaxed);
}

//*****************************************************************************
VuRetVal VuPurchaseUpgradeEntity::CanAfford(const VuParams &params)
{
	bool canAfford = false;

	const VuGameManager::Car &car = VuGameManager::IF()->getCurCar();

	int nextLevel = car.getStat(mStat.c_str()) + 1;

	int priceSC = VuGameUtil::IF()->getCarUpgradePrice(car.mName, car.mStage, nextLevel, mStat);
	if ( VuGameManager::IF()->getCurrency(VuGameManager::CT_STANDARD) >= priceSC )
		canAfford = true;

	return VuRetVal(canAfford);
}

//*****************************************************************************
VuRetVal VuPurchaseUpgradeEntity::Purchase(const VuParams &params)
{
	if ( VuGameManager::IF()->purchaseCarUpgrade(mStat) )
	{
		mpScriptComponent->getPlug("OnSuccess")->execute();

		int level = VuGameManager::IF()->getCurCar().getStat(mStat.c_str());
		if ( level >= VuGameManager::IF()->getCurCar().getMaxLevel() )
			mpScriptComponent->getPlug("OnMaxed")->execute();
	}
	else
	{
		mpScriptComponent->getPlug("OnFailure")->execute();
	}

	return VuRetVal();
}


//*****************************************************************************
// GiveStage
//*****************************************************************************

class VuGiveStageEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuGiveStageEntity();

protected:
	VuRetVal			IsAllowed(const VuParams &params) { return VuRetVal(isAllowed()); }

// modify by xlm,[/2015/4/30 ] 
	VuRetVal			IsSeriesAllowed(const VuParams &params);
	VuRetVal			IsMaxed(const VuParams &params) { return VuRetVal(isMaxed()); }
	VuRetVal			IsLocked(const VuParams &params) { return VuRetVal(isLocked()); }
	VuRetVal			PartialStats(const VuParams &params) { return VuRetVal(partialStats()); }
	VuRetVal			Give(const VuParams &params);

	bool				isAllowed();
	bool				isMaxed();
	bool				isLocked();
	bool				partialStats();

	bool				_isSeriesAllowed();

	std::string  selectCarName;

	//modify by liudan
	void OnGenericEvent(const VuParams &params);


	// components
	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuGiveStageEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuGiveStageEntity);

//*****************************************************************************
VuGiveStageEntity::VuGiveStageEntity()
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	// scripting
	ADD_SCRIPT_INPUT(mpScriptComponent, VuGiveStageEntity, IsAllowed, VuRetVal::Bool, VuParamDecl());
// modify by xlm,[/2015/4/30 ] 
	ADD_SCRIPT_INPUT(mpScriptComponent, VuGiveStageEntity, IsSeriesAllowed, VuRetVal::Bool, VuParamDecl());

	ADD_SCRIPT_INPUT(mpScriptComponent, VuGiveStageEntity, IsMaxed, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT(mpScriptComponent, VuGiveStageEntity, IsLocked, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT(mpScriptComponent, VuGiveStageEntity, PartialStats, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuGiveStageEntity, Give);

	REG_EVENT_HANDLER(VuGiveStageEntity, OnGenericEvent);
	selectCarName = "";
}

//*****************************************************************************
VuRetVal VuGiveStageEntity::Give(const VuParams &params)
{
	VuGameManager::IF()->giveCarStage();

	return VuRetVal();
}

void VuGiveStageEntity::OnGenericEvent(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	const char *itemName = accessor.getString();

	std::string itemStr = itemName;
	//char *item = itemName;
	//int size = item.size();
	//char *tmpStr = "_";
	//char* buf = strstr(item, tmpStr);
	char *tmp = "_";
	int iPos = itemStr.find_last_of(tmp);
	int iSize = itemStr.size();
	if (iPos < iSize)
	{
		selectCarName = "";
		for (int i = iPos+1; i < iSize; i++)
		{
			selectCarName = selectCarName + itemStr[i];
		}
		int size = selectCarName.size();
	}
}

// modify by xlm,[/2015/4/30 ] 
VuRetVal			VuGiveStageEntity::IsSeriesAllowed(const VuParams &params)
{
	return VuRetVal(_isSeriesAllowed());
}

bool				VuGiveStageEntity::_isSeriesAllowed()
{
	if (isLocked())
		return false;

	/*if (partialStats())
		return false;*/

	return true;
}

//*****************************************************************************
bool VuGiveStageEntity::isAllowed()
{
	if ( isMaxed() )
		return false;

	if ( isLocked() )
		return false;

	if ( partialStats() )
		return false;

	return true;
}

//*****************************************************************************
bool VuGiveStageEntity::isMaxed()
{
	bool ret = false;

	if (selectCarName == "")
	{
		 ret = VuGameManager::IF()->getCurCar().mStage >= VuGameUtil::IF()->carDB()[VuGameManager::IF()->getCurCar().mName]["skycn_maxStage"].asInt();
	}
	else
	{
		const VuGameManager::Car &car =  VuGameManager::IF()->getCar(selectCarName);
		int iCarStage = car.mStage;
		int iMaxStage = VuGameUtil::IF()->carDB()[car.mName]["skycn_maxStage"].asInt();
		ret = (iCarStage >= iMaxStage);
	}
	
	//tony.hu SKCN_CAR 2014.12.26
	/*int iCurStage = VuGameManager::IF()->getCurCar().mStage;
	int iMaxStage = VuGameUtil::IF()->carDB()[VuGameManager::IF()->getCurCar().mName]["skycn_maxStage"].asInt();

	bool ret = VuGameManager::IF()->getCurCar().mStage >= VuGameUtil::IF()->carDB()[VuGameManager::IF()->getCurCar().mName]["skycn_maxStage"].asInt();*/
	//VUPRINTF("tony.hu isMaxed=%s\n", ret?"true":"false");
	return ret;
	//return VuGameManager::IF()->getCurCar().mStage >= 3;

	

	//modify by xlm, 去除所有的限制
	//return false;
}

//*****************************************************************************
bool VuGiveStageEntity::isLocked()
{
	if ( VuDevConfig::IF()->getParam("UnlockAll").asBool() )
		return false;
//SKYCN_EASY_LEVELUP tony.hu 2014.12.25 disable this check to have a easy level up
	return false;
	//return VuGameManager::IF()->getCurCar().mStage >= VuGameManager::IF()->getUnlockedStage();
}

//*****************************************************************************
bool VuGiveStageEntity::partialStats()
{

	//modify xlm, 汽车不再有1000hp限制

	const VuGameManager::Car &car = VuGameManager::IF()->getCurCar();

	return !car.areStatsMaxed();
	//return false;
}


//*****************************************************************************
// PurchaseDriver
//*****************************************************************************

class VuPurchaseDriverEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuPurchaseDriverEntity();

protected:
	VuRetVal			IsOwned(const VuParams &params);
	VuRetVal			IsDuelable(const VuParams &params);
	VuRetVal			CanAfford(const VuParams &params);
	VuRetVal			Purchase(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuPurchaseDriverEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuPurchaseDriverEntity);

//*****************************************************************************
VuPurchaseDriverEntity::VuPurchaseDriverEntity()
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	// scripting
	ADD_SCRIPT_INPUT(mpScriptComponent, VuPurchaseDriverEntity, IsOwned, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT(mpScriptComponent, VuPurchaseDriverEntity, IsDuelable, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT(mpScriptComponent, VuPurchaseDriverEntity, CanAfford, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuPurchaseDriverEntity, Purchase);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnSuccess);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnFailure);
}

//*****************************************************************************
VuRetVal VuPurchaseDriverEntity::IsOwned(const VuParams &params)
{
	const std::string &selectedDriver = VuGameUtil::IF()->getSelectedDriver();
	bool isOwned = VuGameManager::IF()->getDriver(selectedDriver).isOnTeam();

	return VuRetVal(isOwned);
}

//*****************************************************************************
VuRetVal VuPurchaseDriverEntity::IsDuelable(const VuParams &params)
{
	const std::string &selectedDriver = VuGameUtil::IF()->getSelectedDriver();

	bool isDuelable = VuGameUtil::IF()->driverDB()[selectedDriver]["Duelable"].asBool();

	return VuRetVal(isDuelable);
}

//*****************************************************************************
VuRetVal VuPurchaseDriverEntity::CanAfford(const VuParams &params)
{
	const std::string &selectedDriver = VuGameUtil::IF()->getSelectedDriver();

	int price = VuGameUtil::IF()->getDriverPrice(selectedDriver);


	/*const VuSpreadsheetAsset *pSA = VuGameUtil::IF()->driverSpreadsheet();
	int rowIndex = VuSpreadsheetQuery::findFirstRow(pSA, VuSpreadsheetQuery::VuStringEqual("Driver", selectedDriver.c_str()));
	int nPrice = -1;
	VuFastDataUtil::getValue(pSA->getField(rowIndex, "UnlockPrice"), nPrice);*/

	int nNowPC = VuGameManager::IF()->getCurrency(VuGameManager::CT_PREMIUM);
	if (nNowPC >= price)
	{
		return VuRetVal(true);
	}



	return VuRetVal(false);
}

//*****************************************************************************
VuRetVal VuPurchaseDriverEntity::Purchase(const VuParams &params)
{
	//减少当前的pc数量

	const std::string &selectedDriver = VuGameUtil::IF()->getSelectedDriver();

	if (VuGameManager::IF()->purchaseDriver(selectedDriver))
	{
		mpScriptComponent->getPlug("OnSuccess")->execute();
		VuAnalyticsManager::IF()->logEvent("Buy Driver", selectedDriver.c_str(), 1);
	}	
	else
		mpScriptComponent->getPlug("OnFailure")->execute();

	return VuRetVal();
}


//*****************************************************************************
// PurchasePowerUp
//*****************************************************************************

class VuPurchasePowerUpEntity : public VuEntity{
	DECLARE_RTTI

public:
	VuPurchasePowerUpEntity();

protected:
	VuRetVal			CanAfford(const VuParams &params);
	VuRetVal			Purchase(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuPurchasePowerUpEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuPurchasePowerUpEntity);

//*****************************************************************************
VuPurchasePowerUpEntity::VuPurchasePowerUpEntity()
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	// scripting
	ADD_SCRIPT_INPUT(mpScriptComponent, VuPurchasePowerUpEntity, CanAfford, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuPurchasePowerUpEntity, Purchase);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnSuccess);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnFailure);
}

//*****************************************************************************
VuRetVal VuPurchasePowerUpEntity::CanAfford(const VuParams &params)
{
	bool canAfford = false;

	const std::string &powerUp = VuGameUtil::IF()->dataRead()["SelectedPowerUp"].asString();

	int price = VuGameUtil::IF()->getPowerUpPrice(powerUp);

	if ( price > 0 )
		if ( VuGameManager::IF()->getCurrency(VuGameManager::CT_PREMIUM) >= price )
			canAfford = true;

	return VuRetVal(canAfford);
}

//*****************************************************************************
VuRetVal VuPurchasePowerUpEntity::Purchase(const VuParams &params)
{
	const std::string &powerUp = VuGameUtil::IF()->dataRead()["SelectedPowerUp"].asString();
	
	if (VuGameManager::IF()->purchasePowerUp(powerUp))
	{
		mpScriptComponent->getPlug("OnSuccess")->execute();
		VuAnalyticsManager::IF()->logEvent("Buy Item", powerUp.c_str(), 1);
	}
	else
		mpScriptComponent->getPlug("OnFailure")->execute();

	return VuRetVal();
}


//*****************************************************************************
// PurchaseTuneUps
//*****************************************************************************

class VuPurchaseTuneUpsEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuPurchaseTuneUpsEntity();

protected:
	VuRetVal			CanAfford(const VuParams &params);
	VuRetVal			Purchase(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;

	// properties
	int					mCount;
	int					mPricePC;
};

IMPLEMENT_RTTI(VuPurchaseTuneUpsEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuPurchaseTuneUpsEntity);

//*****************************************************************************
VuPurchaseTuneUpsEntity::VuPurchaseTuneUpsEntity():
	mCount(1),
	mPricePC(1)
{
	// properties
	addProperty(new VuIntProperty("Count", mCount));
	addProperty(new VuIntProperty("Price PC", mPricePC));

	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	// scripting
	ADD_SCRIPT_INPUT(mpScriptComponent, VuPurchaseTuneUpsEntity, CanAfford, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuPurchaseTuneUpsEntity, Purchase);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnSuccess);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnFailure);
}

//*****************************************************************************
VuRetVal VuPurchaseTuneUpsEntity::CanAfford(const VuParams &params)
{
	bool canAfford = false;

	if ( VuGameManager::IF()->getCurrency(VuGameManager::CT_PREMIUM) >= mPricePC )
		canAfford = true;

	return VuRetVal(canAfford);
}

//*****************************************************************************
VuRetVal VuPurchaseTuneUpsEntity::Purchase(const VuParams &params)
{
	if ( VuGameManager::IF()->purchaseTuneUps(mCount, mPricePC) )
		mpScriptComponent->getPlug("OnSuccess")->execute();
	else
		mpScriptComponent->getPlug("OnFailure")->execute();

	return VuRetVal();
}


//*****************************************************************************
// PurchaseTokenRefill
//*****************************************************************************

class VuPurchaseTokenRefillEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuPurchaseTokenRefillEntity();

protected:
	VuRetVal			CanAfford(const VuParams &params);
	VuRetVal			Purchase(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuPurchaseTokenRefillEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuPurchaseTokenRefillEntity);

//*****************************************************************************
VuPurchaseTokenRefillEntity::VuPurchaseTokenRefillEntity()
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	// scripting
	ADD_SCRIPT_INPUT(mpScriptComponent, VuPurchaseTokenRefillEntity, CanAfford, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuPurchaseTokenRefillEntity, Purchase);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnSuccess);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnFailure);
}

//*****************************************************************************
VuRetVal VuPurchaseTokenRefillEntity::CanAfford(const VuParams &params)
{
	bool canAfford = false;

	int pricePC = VuCloudTuningManager::IF()->variables().mTokenRefillPrice;

	if ( VuGameManager::IF()->getCurrency(VuGameManager::CT_PREMIUM) >= pricePC )
		canAfford = true;

	return VuRetVal(canAfford);
}

//*****************************************************************************
VuRetVal VuPurchaseTokenRefillEntity::Purchase(const VuParams &params)
{
	
	if ( VuGameManager::IF()->purchaseTokenRefill() )
		mpScriptComponent->getPlug("OnSuccess")->execute();
	else
		mpScriptComponent->getPlug("OnFailure")->execute();

	return VuRetVal();
}







class VuPurchaseTokenRefillExEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuPurchaseTokenRefillExEntity();

protected:
	VuRetVal			CanAfford(const VuParams &params);
	VuRetVal			Purchase(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;

	int					mPrice;
	int					mCount;
};

IMPLEMENT_RTTI(VuPurchaseTokenRefillExEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuPurchaseTokenRefillExEntity);

//*****************************************************************************
VuPurchaseTokenRefillExEntity::VuPurchaseTokenRefillExEntity()
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	addProperty(new VuIntProperty("price", mPrice));
	addProperty(new VuIntProperty("count", mCount));


	// scripting
	ADD_SCRIPT_INPUT(mpScriptComponent, VuPurchaseTokenRefillExEntity, CanAfford, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuPurchaseTokenRefillExEntity, Purchase);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnSuccess);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnFailure);
}

//*****************************************************************************
VuRetVal VuPurchaseTokenRefillExEntity::CanAfford(const VuParams &params)
{
	bool canAfford = false;

	//int pricePC = VuCloudTuningManager::IF()->variables().mTokenRefillPrice;

	if (VuGameManager::IF()->getCurrency(VuGameManager::CT_PREMIUM) >= mPrice)
		canAfford = true;

	return VuRetVal(canAfford);
}

//*****************************************************************************
VuRetVal VuPurchaseTokenRefillExEntity::Purchase(const VuParams &params)
{
	if (VuGameManager::IF()->PurchaseTokenRefill(mCount, mPrice))
		mpScriptComponent->getPlug("OnSuccess")->execute();
	else
		mpScriptComponent->getPlug("OnFailure")->execute();

	return VuRetVal();
}





//*****************************************************************************
// VuPurchaseInfoEntity
//*****************************************************************************

class VuPurchaseInfoEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuPurchaseInfoEntity();

protected:
	VuRetVal			BoughtCar(const VuParams &params) { return VuRetVal(VuGameManager::IF()->numCarsBought() > 0); }
	VuRetVal			BoughtUpgrade(const VuParams &params) { return VuRetVal(VuGameManager::IF()->numUpgradesBought() > 0); }
	VuRetVal			BoughtDriver(const VuParams &params) { return VuRetVal(VuGameManager::IF()->numDriversBought() > 0); }
	VuRetVal			BoughtPowerUp(const VuParams &params) { return VuRetVal(VuGameManager::IF()->numPowerUpsBought() > 0); }
	VuRetVal			BoughtTuneUp(const VuParams &params) { return VuRetVal(VuGameManager::IF()->numTuneUpsBought() > 0); }

	// components
	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuPurchaseInfoEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuPurchaseInfoEntity);

//*****************************************************************************
VuPurchaseInfoEntity::VuPurchaseInfoEntity()
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	// scripting
	ADD_SCRIPT_INPUT(mpScriptComponent, VuPurchaseInfoEntity, BoughtCar, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT(mpScriptComponent, VuPurchaseInfoEntity, BoughtUpgrade, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT(mpScriptComponent, VuPurchaseInfoEntity, BoughtDriver, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT(mpScriptComponent, VuPurchaseInfoEntity, BoughtPowerUp, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT(mpScriptComponent, VuPurchaseInfoEntity, BoughtTuneUp, VuRetVal::Bool, VuParamDecl());
}



































//*****************************************************************************
// PurchaseTuneUps
//*****************************************************************************

class VuPurchaseNonRmbItemEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuPurchaseNonRmbItemEntity();

protected:
	VuRetVal			CanAfford(const VuParams &params);
	virtual VuRetVal			Purchase(const VuParams &params);

	bool				_CheckCanAfford();

	virtual void				_ProcessItem();

	// components
	VuScriptComponent	*mpScriptComponent;

	// properties
	int					mCount;
	int					mBuyType; //0 免费， 1 金币购买    2 钻石购买    3 rmb购买
	int					mPricePC;
	std::string			mItemName;  //商品名称
};

IMPLEMENT_RTTI(VuPurchaseNonRmbItemEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuPurchaseNonRmbItemEntity);

//*****************************************************************************
VuPurchaseNonRmbItemEntity::VuPurchaseNonRmbItemEntity() :
mCount(1),
mPricePC(1),
mBuyType(0)
{
	// properties
	addProperty(new VuIntProperty("Count", mCount));
	addProperty(new VuIntProperty("Price PC", mPricePC));
	addProperty(new VuStringProperty("ItemName", mItemName));


	// properties
	static VuStaticIntEnumProperty::Choice sNonRmbChoiseChoices[] =
	{
		{ "free", 0 },
		{ "coin", 1 }, { "diam", 2 }
	};
	addProperty(new VuStaticIntEnumProperty("BuyType", mBuyType, sNonRmbChoiseChoices));

	



	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	// scripting
	ADD_SCRIPT_INPUT(mpScriptComponent, VuPurchaseNonRmbItemEntity, CanAfford, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuPurchaseNonRmbItemEntity, Purchase);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnSuccess);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnFailure);
}

//*****************************************************************************
VuRetVal VuPurchaseNonRmbItemEntity::CanAfford(const VuParams &params)
{
	/*bool canAfford = false;

	if (mBuyType == 0)
	{
		canAfford = true;
	}
	else if (mBuyType == 1)
	{
		if (VuGameManager::IF()->getCurrency(VuGameManager::CT_STANDARD) >= mPricePC)
			canAfford = true;
	}
	else if (mBuyType == 2)
	{
		if (VuGameManager::IF()->getCurrency(VuGameManager::CT_PREMIUM) >= mPricePC)
			canAfford = true;
	}*/

	/*if (VuGameManager::IF()->getCurrency(VuGameManager::CT_PREMIUM) >= mPricePC)
		canAfford = true;*/

	return VuRetVal(_CheckCanAfford());
}

bool				VuPurchaseNonRmbItemEntity::_CheckCanAfford()
{
	bool canAfford = false;

	if (mBuyType == 0)
	{
		canAfford = true;
	}
	else if (mBuyType == 1)
	{
		if (VuGameManager::IF()->getCurrency(VuGameManager::CT_STANDARD) >= mPricePC)
			canAfford = true;
	}
	else if (mBuyType == 2)
	{
		if (VuGameManager::IF()->getCurrency(VuGameManager::CT_PREMIUM) >= mPricePC)
			canAfford = true;
	}

	/*if (VuGameManager::IF()->getCurrency(VuGameManager::CT_PREMIUM) >= mPricePC)
	canAfford = true;*/

	return canAfford;
}


void				VuPurchaseNonRmbItemEntity::_ProcessItem()
{


	if (VuBillingManager::IF()->isOwned(mItemName.c_str()))
		return;



	const VuJsonContainer &itemData = VuDataUtil::findArrayMember(VuGameUtil::IF()->storeDB(), "Name", mItemName);

	int standardCurrency = itemData["StandardCurrency"].asInt();
	VuGameManager::IF()->addCurrencyPurchased(VuGameManager::CT_STANDARD, standardCurrency);

	int nPlusCoin = itemData["PlusCoin"].asInt();
	VuGameManager::IF()->addCurrencyPurchased(VuGameManager::CT_STANDARD, nPlusCoin);


	int premiumCurrency = itemData["PremiumCurrency"].asInt();
	VuGameManager::IF()->addCurrencyPurchased(VuGameManager::CT_PREMIUM, premiumCurrency);

	int nPlusDiam = itemData["PlusDiamond"].asInt();
	VuGameManager::IF()->addCurrencyPurchased(VuGameManager::CT_PREMIUM, nPlusDiam);

	const VuJsonContainer &cars = itemData["Cars"];
	for (int i = 0; i < cars.size(); i++)
		VuGameManager::IF()->giveCar(cars[i].asString());

	const VuJsonContainer &powerUps = itemData["PowerUps"];
	for (int i = 0; i < powerUps.size(); i++)
		VuGameManager::IF()->givePowerUp(powerUps[i].asString());



	if (itemData["Type"].asString() == "Managed")
		VuBillingManager::IF()->setOwned(mItemName.c_str());

	int nTokens = itemData["Tokens"].asInt();
	VuGameManager::IF()->addTokenNoLimit(nTokens);

	int nTuneUps = itemData["TuneUps"].asInt();
	VuGameManager::IF()->addTuneUps(nTuneUps);

	VuProfileManager::IF()->save();
	VuCloudSaveManager::IF()->save();


}


//*****************************************************************************
VuRetVal VuPurchaseNonRmbItemEntity::Purchase(const VuParams &params)
{

	bool bBuySuccess = false;

	if (!_CheckCanAfford())
	{
		mpScriptComponent->getPlug("OnFailure")->execute();
	}
	else
	{
		int nPC = 0;
		int nSC = 0;

		if (mBuyType == 1)
		{
			nSC = mPricePC;

		}
		else if (mBuyType == 2)
		{
			nPC = mPricePC;
		}

		VuGameManager::IF()->spendCurrency(nSC, nPC);
		_ProcessItem();
		mpScriptComponent->getPlug("OnSuccess")->execute();
	}




	/*if (VuGameManager::IF()->purchaseTuneUps(mCount, mPricePC))
		mpScriptComponent->getPlug("OnSuccess")->execute();
	else
		mpScriptComponent->getPlug("OnFailure")->execute();*/

	return VuRetVal();
}









//*****************************************************************************
// PurchaseTuneUps
//*****************************************************************************

class VuReceiveLoginItemEntity : public VuPurchaseNonRmbItemEntity
{
	DECLARE_RTTI

public:
	VuReceiveLoginItemEntity();

protected:

	virtual VuRetVal			Purchase(const VuParams &params);


	virtual void				_ProcessItem();

private:
	int						m_nReceiveDay;

	// components
	//VuScriptComponent	*mpScriptComponent;

};

IMPLEMENT_RTTI(VuReceiveLoginItemEntity, VuPurchaseNonRmbItemEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuReceiveLoginItemEntity);

VuReceiveLoginItemEntity::VuReceiveLoginItemEntity() : VuPurchaseNonRmbItemEntity(), m_nReceiveDay(0)
{

}


VuRetVal			VuReceiveLoginItemEntity::Purchase(const VuParams &params)
{


	if (!VuGameManager::IF())
	{
		mpScriptComponent->getPlug("OnFailure")->execute();
		return VuRetVal();
	}
	m_nReceiveDay = VuGameManager::IF()->getAvailableLoginDay();
	if (m_nReceiveDay == 0)
	{
		mpScriptComponent->getPlug("OnFailure")->execute();
		return VuRetVal();
	}
/*
	char szItemName[64] = { 0 };
	sprintf(szItemName, "Login%dDay", nReceiveDay);

	const VuJsonContainer &itemData = VuDataUtil::findArrayMember(VuGameUtil::IF()->storeDB(), "Name", szItemName);

	int standardCurrency = itemData["StandardCurrency"].asInt();*/
	//VuGameManager::IF()->addCurrencyPurchased(VuGameManager::CT_STANDARD, standardCurrency);

	bool bBuySuccess = false;

	if (!_CheckCanAfford())
	{
		mpScriptComponent->getPlug("OnFailure")->execute();
	}
	else
	{

		/*int nPC = 0;
		int nSC = 0;

		if (mBuyType == 1)
		{
			nSC = mPricePC;


		}
		else if (mBuyType == 2)
		{
			nSC = mPricePC;
		}

		VuGameManager::IF()->spendCurrency(nSC, nPC);*/

		//保存已经领取的天数
		VuGameManager::IF()->addReceivedDays();

		_ProcessItem();


		mpScriptComponent->getPlug("OnSuccess")->execute();
	}




	/*if (VuGameManager::IF()->purchaseTuneUps(mCount, mPricePC))
	mpScriptComponent->getPlug("OnSuccess")->execute();
	else
	mpScriptComponent->getPlug("OnFailure")->execute();*/

	return VuRetVal();
}



void				VuReceiveLoginItemEntity::_ProcessItem()
{

	char szItemName[64] = { 0 };
	sprintf(szItemName, "Login%dDay", m_nReceiveDay);

	const VuJsonContainer &itemData = VuDataUtil::findArrayMember(VuGameUtil::IF()->storeDB(), "Name", szItemName);

	//const VuJsonContainer &itemData = VuDataUtil::findArrayMember(VuGameUtil::IF()->storeDB(), "Name", mItemName);

	int standardCurrency = itemData["StandardCurrency"].asInt();
	VuGameManager::IF()->addCurrencyPurchased(VuGameManager::CT_STANDARD, standardCurrency);

	int nPlusCoin = itemData["PlusCoin"].asInt();
	VuGameManager::IF()->addCurrencyPurchased(VuGameManager::CT_STANDARD, nPlusCoin);


	int premiumCurrency = itemData["PremiumCurrency"].asInt();
	VuGameManager::IF()->addCurrencyPurchased(VuGameManager::CT_PREMIUM, premiumCurrency);

	int nPlusDiam = itemData["PlusDiamond"].asInt();
	VuGameManager::IF()->addCurrencyPurchased(VuGameManager::CT_PREMIUM, nPlusDiam);

	const VuJsonContainer &cars = itemData["Cars"];
	for (int i = 0; i < cars.size(); i++)
		VuGameManager::IF()->giveCar(cars[i].asString());

	const VuJsonContainer &powerUps = itemData["PowerUps"];
	for (int i = 0; i < powerUps.size(); i++)
		VuGameManager::IF()->givePowerUp(powerUps[i].asString());



	if (itemData["Type"].asString() == "Managed")
		VuBillingManager::IF()->setOwned(mItemName.c_str());

	int nTokens = itemData["Tokens"].asInt();
	VuGameManager::IF()->addTokenNoLimit(nTokens);

	int nTuneUps = itemData["TuneUps"].asInt();
	VuGameManager::IF()->addTuneUps(nTuneUps);

	VuProfileManager::IF()->save();
	VuCloudSaveManager::IF()->save();


}




