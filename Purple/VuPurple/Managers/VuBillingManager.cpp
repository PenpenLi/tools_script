//*****************************************************************************
//
//  Copyright (c) 2012-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  BillingManager class
// 
//*****************************************************************************

#include "VuBillingManager.h"
#include "VuPurple/Managers/VuCloudSaveManager.h"
#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Managers/VuAnalyticsManager.h"
#include "VuPurple/Managers/VuStatsManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuPurple/Util/VuGameFontMacros.h"
#include "VuEngine/DB/VuStringDB.h"
#include "VuEngine/Managers/VuProfileManager.h"
#include "VuEngine/Managers/VuMessageBoxManager.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Util/VuStringUtil.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuPurple/Util/CommonUtility.h"

//*****************************************************************************
void VuBillingItemProcessor::onMessageBoxClosed(VuMessageBox *pMessageBox)
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
VuBillingManager::VuBillingManager():
mBusy(false), mFirstChargeProcessor(NULL), m_billingType(LOCAL_MESSAGE), mBillingItemProcessor(NULL)
{
}

VuBillingManager::~VuBillingManager()
{
	if (mFirstChargeProcessor)
	{
		delete mFirstChargeProcessor;
		mFirstChargeProcessor = NULL;
	}
	if (mBillingItemProcessor)
	{
		delete mBillingItemProcessor;
		mBillingItemProcessor = NULL;
	}
}

//*****************************************************************************
bool VuBillingManager::init()
{
	if (mFirstChargeProcessor == NULL)
		mFirstChargeProcessor = new FirstChargeProcessor;
	if (mBillingItemProcessor == NULL)
		mBillingItemProcessor = new VuBillingItemProcessor;
	return true; 
}

//*****************************************************************************
void VuBillingManager::postInit()
{
	for ( int i = 0; i < VuGameUtil::IF()->storeDB().size(); i++ )
	{
		const VuJsonContainer &itemData = VuGameUtil::IF()->storeDB()[i];
		const std::string &itemName = itemData["Name"].asString();

		setPrice(itemName.c_str(), itemData["StubPrice"].asCString());

		int sc = itemData["StandardCurrency"].asInt();
		int pc = itemData["PremiumCurrency"].asInt();
		if ( sc + pc )
		{
			char strValue[16];
			VuStringUtil::integerFormat(sc + pc, strValue, sizeof(strValue));
			VuGameFontMacros::IF()->setMacro(itemName.c_str(), strValue);
		}
	}

	setOwned("StarterPack");
	//setOwned("starterPack");

}

void	VuBillingManager::startPurchaseInternal(const std::string& itemName,float needMoney)
{
	setCurrentClickItem(itemName);

	bool mode = NO_POP_MUTIDIALOG_PURCHASE;
	do
	{
		if (mode == true)
		{
			setBillingType(LOCAL_MESSAGE);
			startPurchase(itemName);
			break;
		}

		if (needMoney > 0)
		{
			VuMessageBoxParams messageBoxParams;
			messageBoxParams.mType = "Billing";
			messageBoxParams.mHeading = "test";
			messageBoxParams.mTextA = "test";
			messageBoxParams.mBody = "test";
			messageBoxParams.mPauseGame = true;

			//messageBoxParams.mUserData = (nCoinTotal << 16) + nDiamTotal;

			VuMessageBox *pMB = VuMessageBoxManager::IF()->create(messageBoxParams);
			mBillingItemProcessor->setItemName(itemName);

			pMB->setCallback(mBillingItemProcessor);
			pMB->removeRef();
		}
		else
		{
			startPurchase(itemName);
		}

	} while (0);

}


//*****************************************************************************
void VuBillingManager::startPurchase(const std::string &itemName)
{	
	if (isOwned(itemName.c_str()))
	{
		onPurchaseResult(itemName, "RESULT_ALREADY_OWNED");
	}
	else if ( !isBusy() )
	{
		mBusy = true;
		startPurchaseInternal(itemName);
	}
	/*if (VuTickManager::IF()->isPaused())
	{
		VuTickManager::IF()->popPauseRequest();
	}
*/
}

//*****************************************************************************
void VuBillingManager::setPrice(const char *itemName, const char *price)
{
	mPrices[itemName] = price;

	char strMacro[64];
	VU_SPRINTF(strMacro, sizeof(strMacro), "PRICE_%s", itemName);

	VuGameFontMacros::IF()->setMacro(strMacro, price);
}

//*****************************************************************************
bool VuBillingManager::isOwned(const char *itemName)
{
	if (VuProfileManager::IF())
		return VuProfileManager::IF()->dataRead()["Billing"][itemName].asBool();
	return false;
}

//*****************************************************************************
void VuBillingManager::setOwned(const char *itemName)
{
	if (VuProfileManager::IF())
		VuProfileManager::IF()->dataWrite()["Billing"][itemName].putValue(true);
	
}

//*****************************************************************************
bool VuBillingManager::isUnlocked(const char *theThing)
{
	const VuJsonContainer &storeDB = VuGameUtil::IF()->storeDB();
	for ( int i = 0; i < storeDB.size(); i++ )
	{
		const VuJsonContainer &item = storeDB[i];
		if ( item[theThing].asBool() && isOwned(item["Name"].asCString()) )
			return true;
	}

	return false;
}

//*****************************************************************************
void VuBillingManager::logPurchaseAnalyticsEvent(const char *itemName)
{
	// analytics - What are the most popular IAPs?
	{
		VuJsonContainer variables;
		
		variables["Highest Event Played"].putValue(VuStatsManager::IF()->getHighestEventPlayedIndex());
		variables["Coins Earned"].putValue(VuGameManager::IF()->getCurrencyEarned(VuGameManager::CT_STANDARD));
		variables["Coins Spent"].putValue(VuGameManager::IF()->getCurrencySpent(VuGameManager::CT_STANDARD));
		variables["Gems Earned"].putValue(VuGameManager::IF()->getCurrencyEarned(VuGameManager::CT_PREMIUM));
		variables["Gems Spent"].putValue(VuGameManager::IF()->getCurrencySpent(VuGameManager::CT_PREMIUM));
		
		VuAnalyticsManager::IF()->logEvent("IAP Made", "IAP Name", itemName, variables);
	}
	
	// analytics - At what point in progression are players making each IAP?
	{
		VuJsonContainer variables;
		
		variables["Coins Earned"].putValue(VuGameManager::IF()->getCurrencyEarned(VuGameManager::CT_STANDARD));
		variables["Coins Spent"].putValue(VuGameManager::IF()->getCurrencySpent(VuGameManager::CT_STANDARD));
		variables["Gems Earned"].putValue(VuGameManager::IF()->getCurrencyEarned(VuGameManager::CT_PREMIUM));
		variables["Gems Spent"].putValue(VuGameManager::IF()->getCurrencySpent(VuGameManager::CT_PREMIUM));
		
		char str[256];
		VU_SPRINTF(str, sizeof(str), "IAP %s Purchased", itemName);
		VuAnalyticsManager::IF()->logEvent(str, "Highest Event Played", VuStatsManager::IF()->getHighestEventPlayedName().c_str(), variables);
	}
}

//*****************************************************************************
void VuBillingManager::onPurchaseResult(const std::string &itemName, const std::string &result)
{
	std::string bodyText;
	if ( result == "RESULT_OK" || result == "RESULT_ALREADY_OWNED" )
	{
		//bodyText = "Store_MB_Success";
		if ( result == "RESULT_ALREADY_OWNED" )
			bodyText = "Store_MB_AlreadyOwned";
		if (getCurrentBillingType() == WEIXIN)
			VuRequestOrderManager::IF()->startSuccessVerification(itemName);
		else
		{
			processItem(itemName.c_str());
			{
				VuParams params;
				params.addString(itemName.c_str());
				VuEventManager::IF()->broadcast("OnPurchaseMade", params);
			}
		}
			
	}
	else if ( result == "RESULT_USER_CANCELED" )
	{
		//bodyText = "Store_MB_UserCanceled";
	}
	else if ( result == "RESULT_SERVICE_UNAVAILABLE" )
	{
		bodyText = "Store_MB_ServiceUnavailable";
	}
	else if ( result == "RESULT_BILLING_UNAVAILABLE" )
	{
		bodyText = "Store_MB_BillingUnavailable";
	}
	else if ( result == "RESULT_ITEM_UNAVAILABLE" )
	{
		bodyText = "Store_MB_ItemUnavailable";
	}
	else if ( result == "RESULT_DEVELOPER_ERROR" )
	{
		bodyText = "Store_MB_DeveloperError";
	}
	else if ( result == "RESULT_DEFERRED" )
	{
		bodyText = "Store_MB_Deferred";
	}
	else
	{
		bodyText = "Store_MB_Error";
	}

	if (bodyText.length())
	{
		bodyText = VuStringDB::IF()->getString(bodyText);
		std::string itemText = VuStringDB::IF()->getString("StoreItem_" + itemName);
		VuStringUtil::replace(bodyText, "[ITEM_NAME]", itemText.c_str());

		VuMessageBoxParams messageBoxParams;
		messageBoxParams.mType = "SimpleA";
		messageBoxParams.mHeading = "Store_MB_Heading";
		messageBoxParams.mTextA = "Common_OK";
		messageBoxParams.mBody = bodyText;
		messageBoxParams.mPauseGame = true;

		VuMessageBox *pMB = VuMessageBoxManager::IF()->create(messageBoxParams);
		pMB->removeRef();
	}

	if (VuTickManager::IF()->isPaused())
	{
		//VuTickManager::IF()->popPauseRequest();
	}

	mBusy = false;
}

//*****************************************************************************
void VuBillingManager::processItem(const char *itemName)
{

	//modify by liudan
	//如果字符串包含 FullLvl_
	const char* tmp = "FullLvl";

	bool bFullLvlCar= false ;
	std::string str1(itemName);
	std::string tmpCar;

	int iPos = str1.find(tmp);
	if (iPos != std::string::npos)
	{
		bFullLvlCar = true;
		tmpCar = str1.substr(iPos + strlen(tmp));

	}

	if (bFullLvlCar)
	{
		if (isOwned(tmpCar.c_str()))
			return;
		const VuJsonContainer &itemData = VuDataUtil::findArrayMember(VuGameUtil::IF()->storeDB(), "Name", itemName);

		const VuJsonContainer &cars = itemData["Cars"];
		for (int i = 0; i < cars.size(); i++)
			VuGameManager::IF()->giveFullLvlCar(cars[i].asString());


		VuProfileManager::IF()->save();
		VuCloudSaveManager::IF()->save();
		return;
	}
	//end by liudan


	if (isOwned(itemName))
		return;

	bool bCoinPromote = false;
	bool bDiamondPromote = false;
	std::string strItemName(itemName);
	int nWeekDay = VuGameManager::IF()->getWeekDay();
	if (nWeekDay == 6 && strItemName.find_first_of("StandardCurrency") != std::string::npos)
	{
		bCoinPromote = true;
	}

	if (nWeekDay == 0 &&strItemName.find_first_of("PremiumCurrency") != std::string::npos)
	{
		bDiamondPromote = true;
	}


	const VuJsonContainer &itemData = VuDataUtil::findArrayMember(VuGameUtil::IF()->storeDB(), "Name", itemName);
	

	int standardCurrency = itemData["StandardCurrency"].asInt();
	if (bCoinPromote)
	{
		VuGameManager::IF()->addCurrencyPurchased(VuGameManager::CT_STANDARD, standardCurrency*1.5);
	}
	else
		VuGameManager::IF()->addCurrencyPurchased(VuGameManager::CT_STANDARD, standardCurrency);

	int nPlusCoin = itemData["PlusCoin"].asInt();
	VuGameManager::IF()->addCurrencyPurchased(VuGameManager::CT_STANDARD, nPlusCoin);


	int premiumCurrency = itemData["PremiumCurrency"].asInt();
	if (bDiamondPromote)
	{
		VuGameManager::IF()->addCurrencyPurchased(VuGameManager::CT_PREMIUM, premiumCurrency*1.5);

	}
	else
		VuGameManager::IF()->addCurrencyPurchased(VuGameManager::CT_PREMIUM, premiumCurrency);

	int nPlusDiam = itemData["PlusDiamond"].asInt();
	VuGameManager::IF()->addCurrencyPurchased(VuGameManager::CT_PREMIUM, nPlusDiam);

	const VuJsonContainer &cars = itemData["Cars"];
	for (int i = 0; i < cars.size(); i++)
		VuGameManager::IF()->giveCar(cars[i].asString());

	const VuJsonContainer &powerUps = itemData["PowerUps"];
	for (int i = 0; i < powerUps.size(); i++)
		VuGameManager::IF()->givePowerUp(powerUps[i].asString());

	const VuJsonContainer &drivers = itemData["Drivers"];
	for (int i = 0; i < drivers.size(); i++)
		VuGameManager::IF()->purchaseDriverFromPack(drivers[i].asString());

	if (itemData["Type"].asString() == "Managed")
		setOwned(itemName);

	int nTokens = itemData["Tokens"].asInt();
	VuGameManager::IF()->addTokenNoLimit(nTokens);

	int nTuneUps = itemData["TuneUps"].asInt();
	VuGameManager::IF()->addTuneUps(nTuneUps);

	VuGameManager::IF()->setIapMade();

	if (!isOwned("RMBUser"))
	{
		setOwned("RMBUser");
		//弹出对话框，发送奖励
	}

	std::string tempItem = itemName;

	bool bPurchasedCoin = VuBillingManager::IF()->isOwned("PurchasedCoin");
	bool bPurchasedDiam = VuBillingManager::IF()->isOwned("PurchasedDiam");

	bool bRet = bPurchasedCoin || bPurchasedDiam;

	std::string subString = tempItem.substr(0, tempItem.length() - 1);
	 
	bool isCoinItem = (subString == "StandardCurrency");// (tempItem.find_first_of("StandardCurrency") != std::string::npos);
	bool isDiamItem = (subString == "PremiumCurrency");  //(tempItem.find_first_of("PremiumCurrency") != std::string::npos);

	if (isCoinItem && !bPurchasedCoin)
	{
		setOwned("PurchasedCoin");
	}

	if (isDiamItem && !bPurchasedDiam)
	{
		setOwned("PurchasedDiam");
	}

	if ((isCoinItem || isDiamItem) && !bRet)
	{
		std::string bodyText = VuStringDB::IF()->getString("First_Charge_Body");

		int nCoinTotal = standardCurrency + nPlusCoin;
		int nDiamTotal = premiumCurrency + nPlusDiam;
		char  strTemp[20] = { 0 };

		if (bCoinPromote)
		{
			sprintf(strTemp, "%d", int (nCoinTotal * 2 + standardCurrency*0.5));
			VuStringUtil::replace(bodyText, "[EXTRA_COIN]", strTemp);
		}
		else
		{
			sprintf(strTemp, "%d", nCoinTotal * 2);
			VuStringUtil::replace(bodyText, "[EXTRA_COIN]", strTemp);
		}

		if (bDiamondPromote)
		{
			sprintf(strTemp, "%d", int(nDiamTotal * 2 + premiumCurrency*0.5));
			VuStringUtil::replace(bodyText, "[EXTRA_DIAMOND]", strTemp);

		}
		else
		{
			sprintf(strTemp, "%d", nDiamTotal * 2);
			VuStringUtil::replace(bodyText, "[EXTRA_DIAMOND]", strTemp);


		}
/*
		sprintf(strTemp, "%d", nCoinTotal*2);
		VuStringUtil::replace(bodyText, "[EXTRA_COIN]", strTemp);
		sprintf(strTemp, "%d", nDiamTotal*2);
		VuStringUtil::replace(bodyText, "[EXTRA_DIAMOND]", strTemp);*/

		VuMessageBoxParams messageBoxParams;
		messageBoxParams.mType = "SimpleA";
		messageBoxParams.mHeading = "First_Charge_Header";
		messageBoxParams.mTextA = "Common_OK";
		messageBoxParams.mBody = bodyText;
		messageBoxParams.mPauseGame = true;

		messageBoxParams.mUserData = (nCoinTotal << 12) + nDiamTotal;

		VuMessageBox *pMB = VuMessageBoxManager::IF()->create(messageBoxParams);
		pMB->setCallback(mFirstChargeProcessor);
		pMB->removeRef();

	}
	else if ((isCoinItem || isDiamItem) && bRet)
	{
		std::string bodyText;
		if (isCoinItem)
		{
			bodyText = VuStringDB::IF()->getString("Buy_Coin_Tip_Body");
		}
		else
			bodyText = VuStringDB::IF()->getString("Buy_Diam_Tip_Body");
		

		int nCoinTotal = standardCurrency + nPlusCoin;
		int nDiamTotal = premiumCurrency + nPlusDiam;
		char  strTemp[20] = { 0 };


		if (bCoinPromote)
		{
			sprintf(strTemp, "%d", int (nCoinTotal  + standardCurrency*0.5));
			VuStringUtil::replace(bodyText, "[EXTRA_COIN]", strTemp);
		}
		else
		{
			sprintf(strTemp, "%d", nCoinTotal );
			VuStringUtil::replace(bodyText, "[EXTRA_COIN]", strTemp);
		}

		if (bDiamondPromote)
		{
			sprintf(strTemp, "%d", (int)(nDiamTotal  + premiumCurrency*0.5));
			VuStringUtil::replace(bodyText, "[EXTRA_DIAMOND]", strTemp);

		}
		else
		{
			sprintf(strTemp, "%d", nDiamTotal );
			VuStringUtil::replace(bodyText, "[EXTRA_DIAMOND]", strTemp);
		}

		sprintf(strTemp, "%d", nCoinTotal);
		VuStringUtil::replace(bodyText, "[EXTRA_COIN]", strTemp);
		sprintf(strTemp, "%d", nDiamTotal);
		VuStringUtil::replace(bodyText, "[EXTRA_DIAMOND]", strTemp);


		VuMessageBoxParams messageBoxParams;
		messageBoxParams.mType = "SimpleA";
		messageBoxParams.mHeading = "Buy_Coin_Diam_Header";
		messageBoxParams.mTextA = "Common_OK";
		messageBoxParams.mBody = bodyText;
		messageBoxParams.mPauseGame = true;

		VuMessageBox *pMB = VuMessageBoxManager::IF()->create(messageBoxParams);
		pMB->removeRef();
	}
	

	VuProfileManager::IF()->save();
	VuCloudSaveManager::IF()->save();
}

void	VuBillingManager::setBillingType(BILLING_TYPE type)
{
	m_billingType = type;
}

VuBillingManager::BILLING_TYPE VuBillingManager::getCurrentBillingType() const
{
	return m_billingType;
}

void VuBillingManager::setCurrentClickItem(std::string itemName)
{
	m_currentClickItem = itemName;
}

bool VuBillingManager::isCurrentBigPayment()
{
	bool result = false;
	
	const VuJsonContainer &itemData = VuDataUtil::findArrayMember(VuGameUtil::IF()->storeDB(), "Name", m_currentClickItem);

	std::string strNeedMoney = itemData["StubPrice"].asString();
	
	int needMoney = VuStringUtil::readInt(strNeedMoney.c_str());
	
	result = (needMoney >= 30) ? true : false;
	return result;
}

void VuBillingManager::onGetOrderId(std::string strOrderId,std::string strOutTradeNo,std::string purchaseItem)
{
	VuGameManager::IF()->addNeedProcessedOrder(strOrderId, strOutTradeNo,purchaseItem);
	startPurchase(purchaseItem);
}

void VuBillingManager::onSuccessVerification(std::string itemName)
{
	processItem(itemName.c_str());

	{
		VuParams params;
		params.addString(itemName.c_str());
		VuEventManager::IF()->broadcast("OnPurchaseMade", params);
	}
}

void FirstChargeProcessor::onMessageBoxClosed(VuMessageBox *pMessageBox)
{
	if (pMessageBox->getResult() == "OnA")
	{
		int nValues = pMessageBox->params().mUserData;
		int nDiam = nValues & 0x0FFF;
		int nCoin = nValues >> 12;



		VuGameManager::IF()->addCurrencyPurchased(VuGameManager::CT_STANDARD, nCoin);

		VuGameManager::IF()->addCurrencyPurchased(VuGameManager::CT_PREMIUM, nDiam);

		VuProfileManager::IF()->save();
		VuCloudSaveManager::IF()->save();

	}
}

		
	
