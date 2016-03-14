//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  GameManager class
// 
//*****************************************************************************

#include "VuGameManager.h"
#include "VuPurple/Managers/VuStatsManager.h"
#include "VuPurple/Managers/VuCloudSaveManager.h"
#include "VuPurple/Managers/VuCloudTuningManager.h"
#include "VuPurple/Managers/VuAnalyticsManager.h"
#include "VuPurple/Managers/VuBillingManager.h"
#include "VuPurple/PowerUps/VuPowerUpManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuPurple/Util/VuAchievementUtil.h"
#include "VuEngine/Managers/VuProfileManager.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Managers/VuToastManager.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Assets/VuSpreadsheetAsset.h"
#include "VuEngine/Util/VuSpreadsheetQuery.h"
#include "VuEngine/Dev/VuDevConfig.h"

#include "VuPurple/Managers/VuNetManager.h"
#include "VuPurple/Protocol/GameProtoId.h"
#include "VuPurple/Protocol/GameProtoOld.h"

#include "Vupurple/Util/CommonUtility.h"
#include "VuEngine/DB/VuStringDB.h"
#include <math.h>


#if defined VUANDROID || defined VUIOS

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#include "VuEngine/Util/VuLog.h"

#endif
#include "../../Util/VuStringUtil.h"



// constants
#define UPDATE_INTERVAL 1.0 // seconds
#define SUSPICIOUS_SC_AMOUNT 1000000
#define SUSPICIOUS_PC_AMOUNT 5000

#define MAX_64_BIT  ((1<<64)-1) 

#define GAME_VERSION "1.9.2"
#define GAME_PRIMARY_VERSION "1.0.6"


// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuGameManager, VuGameManager);


class VuTuneUpToast : public VuToast
{
	DECLARE_RTTI
};
IMPLEMENT_RTTI(VuTuneUpToast, VuToast);


//*****************************************************************************
VuGameManager::VuGameManager():
	mbroadcastClicked(false),
	mUpdateTimer(0.0f),
	mTokenTimer(0),
	m_nReceivedDays(0),
	m_bShowImmediateButtonTip(false),
	m_llCurrServerTime(0),
	m_llLastServerTime(0),
	mWeekDay(-1),
	mGameStartTime(0),
	m_llDailyRewardLastTime(0),
	m_lastCarChampChallengeTime(0),
	mGetNameTimes(3),
	mEnterMainMenuTimes(0),
	mBroadCastVersion(0),
	mBroadCastSize(0),
	mIsBroadcastDataOK(false),
	mIs1_8VersionFirstLogin(true)
{
	// defaults
	setDefaults();

	// event handlers
	REG_EVENT_HANDLER(VuGameManager, OnSaveProfile);
	REG_EVENT_HANDLER(VuGameManager, OnCloudDataPostSync);
	REG_EVENT_HANDLER(VuGameManager, OnPurchaseMade);

	time(&mGameStartTime);
}

time_t	VuGameManager::getGameStartTime()
{
	return mGameStartTime;
}


//*****************************************************************************
bool VuGameManager::init()
{
	VuToastManager::IF()->registerToastType(VuTuneUpToast::msRTTI, "Screens/TuneUpToast");

	VuTickManager::IF()->registerHandler(this, &VuGameManager::tickDecision, "Decision");

	return true;
}

//*****************************************************************************
void VuGameManager::release()
{
	VuTickManager::IF()->unregisterHandler(this, "Decision");
}

//*****************************************************************************
void VuGameManager::tickDecision(float fdt)
{
	fdt = VuTickManager::IF()->getRealDeltaTime();

	mUpdateTimer += fdt;
	if ( mUpdateTimer > UPDATE_INTERVAL )
	{
		mUpdateTimer = 0.0f;

		VUINT64 currentTime = VuTimeUtil::calcSecondsSince2000();

		// disable ads gives infinite tickets
/*
		if ( VuBillingManager::IF()->isUnlocked("DisableAds") )
			mTokens = VuCloudTuningManager::IF()->variables().mMaxTokens;*/

		// handle token refill timer
		if ( mTokens < VuCloudTuningManager::IF()->variables().mMaxTokens )
		{
			// bug fix: Guard against high ticket timer
			if ( mTokenTimerStart > currentTime )
				mTokenTimerStart = currentTime;

			int tokenRefillTime = VuCloudTuningManager::IF()->variables().mTokenRefillTime;

			mTokenTimer = (int)(mTokenTimerStart + tokenRefillTime - currentTime);
			if ( mTokenTimer <= 0 )
			{
				mTokenTimerStart += tokenRefillTime;
				mTokenTimer += tokenRefillTime;
				addToken();
			}
		}


// modify by xlm,[/2014/12/12 ] 
		// clamp to max tokens (in case cloud tuning reduced them)
		//mTokens = VuMin(mTokens, VuCloudTuningManager::IF()->variables().mMaxTokens);

		// specials
		for ( Specials::iterator iter = mActiveSpecials.begin(); iter != mActiveSpecials.end(); )
		{
			bool erase = false;

			if ( !iter->second.mOneShot )
			{
				if ( currentTime > iter->second.mExpirationTime )
					erase = true;

				if ( VuBillingManager::IF()->isOwned(iter->second.mStoreItemName.c_str()) )
					erase = true;
			}

			if ( erase )
				mActiveSpecials.erase(iter++);
			else
				iter++;
		}
		
		// check for piracy
		if ( getCurrency(CT_STANDARD) >= SUSPICIOUS_SC_AMOUNT )
			setFlaggedAsPirate();
		if ( getCurrency(CT_PREMIUM) >= SUSPICIOUS_PC_AMOUNT )
			setFlaggedAsPirate();
	}
}

//*****************************************************************************
bool VuGameManager::spendCurrency(int sc, int pc)
{
	if ( canAfford(sc, pc) )
	{
		mCurrencies[CT_STANDARD].mAmountSpent += sc;
		mCurrencies[CT_PREMIUM].mAmountSpent += pc;

		return true;
	}

	return false;
}

//*****************************************************************************
bool VuGameManager::useTuneUp()
{
	if ( mTuneUps > 0 )
	{
		VuToastManager::IF()->showToast(new VuTuneUpToast);

		mTuneUps--;
		return true;
	}

	return false;
}

//*****************************************************************************
bool VuGameManager::purchaseTuneUps(int count, int pricePC)
{
	if ( pricePC > getCurrency(CT_PREMIUM) )
		return false;

	mCurrencies[CT_PREMIUM].mAmountSpent += pricePC;

	mTuneUps += count;

	mNumTuneUpsBought += count;

	VuProfileManager::IF()->save();
	VuCloudSaveManager::IF()->save();

	return true;
}

//*****************************************************************************
int VuGameManager::getTokens()
{
	return VuBillingManager::IF()->isUnlocked("DisableAds") ? VuCloudTuningManager::IF()->variables().mMaxTokens : mTokens;

	//return mTokens;
}

//*****************************************************************************
bool VuGameManager::useToken()
{
	// disable ads gives infinite tickets
	if ( VuBillingManager::IF()->isUnlocked("DisableAds") )
		return true;

	if ( mTokens > 0 )
	{
		if ( mTokens == VuCloudTuningManager::IF()->variables().mMaxTokens )
			mTokenTimerStart = VuTimeUtil::calcSecondsSince2000();

		mTokens--;

		VuParams params;
		params.addInt(-1);
		VuEventManager::IF()->broadcast("OnTokenUsed", params);

		// analytics - When are players exhausting their tickets?
		if ( (mTokens == 0) && !mTokensEverExhausted )
		{
			mTokensEverExhausted = true;

			VuJsonContainer variables;
			variables["IAP Made"].putValue(getIapMade());
			VuAnalyticsManager::IF()->logEvent("Tickets Exhausted", "Highest Event Played", VuStatsManager::IF()->getHighestEventPlayedName().c_str(), variables);
		}

		return true;
	}

	return false;
}

//*****************************************************************************
void VuGameManager::addToken()
{
	if (!VuBillingManager::IF()->isUnlocked("DisableAds"))
	{
		mTokens++;

		VuParams params;
		params.addInt(1);
		VuEventManager::IF()->broadcast("OnTokenAdded", params);
	}
	/*if ( mTokens < VuCloudTuningManager::IF()->variables().mMaxTokens )
	{
		mTokens++;

		VuParams params;
		params.addInt(1);
		VuEventManager::IF()->broadcast("OnTokenAdded", params);
	}*/
}

void VuGameManager::addTokenNoLimit(int nCount)
{
	mTokens += nCount;

	VuParams params;
	params.addInt(nCount);
	VuEventManager::IF()->broadcast("OnTokenAdded", params);
}

//*****************************************************************************
void VuGameManager::refillTokens()
{
/*
	VuParams params;
	params.addInt(VuCloudTuningManager::IF()->variables().mMaxTokens - mTokens);
	VuEventManager::IF()->broadcast("OnTokenRefill", params);

	mTokens = VuCloudTuningManager::IF()->variables().mMaxTokens*/;


// modify by xlm,[/2014/12/26 ] 

	if (mTokens < VuCloudTuningManager::IF()->variables().mMaxTokens)
	{
		VuParams params;
		params.addInt(VuCloudTuningManager::IF()->variables().mMaxTokens - mTokens);
		VuEventManager::IF()->broadcast("OnTokenRefill", params);

		mTokens = VuCloudTuningManager::IF()->variables().mMaxTokens;

	}

}

//*****************************************************************************
bool VuGameManager::purchaseTokenRefill()
{
// modify by xlm,[/2014/12/10 ] 去掉限制
	/*if ( getTokens() >= VuCloudTuningManager::IF()->variables().mMaxTokens )
		return false;*/

	//int pricePC = VuCloudTuningManager::IF()->variables().mTokenRefillPrice;
	int pricePC = 5;



	if ( pricePC > getCurrency(CT_PREMIUM) )
		return false;

	mCurrencies[CT_PREMIUM].mAmountSpent += pricePC;

	//refillTokens();
	addTokenNoLimit(1);


	VuProfileManager::IF()->save();
	VuCloudSaveManager::IF()->save();

	return true;
}

bool VuGameManager::PurchaseTokenRefill(int nCount, int nPrice)
{
	

	if (nPrice > getCurrency(CT_PREMIUM))
		return false;

	mCurrencies[CT_PREMIUM].mAmountSpent += nPrice;

	//refillTokens();
	addTokenNoLimit(nCount);


	VuProfileManager::IF()->save();
	VuCloudSaveManager::IF()->save();

	return true;
}

//*****************************************************************************
int VuGameManager::getHighestCarStage()
{
	int highestStage = 0;

	for ( const auto &car : mCars )
		highestStage = VuMax(highestStage, car.second.mStage);

	return highestStage;
}

//*****************************************************************************
void VuGameManager::setCurCar(const std::string &carName)
{
	if ( getCar(carName).mIsOwned )
	{
		mCurCarName = carName;
	}
}

//*****************************************************************************
bool VuGameManager::purchaseCar(const std::string &carName)
{
	Car &car = mCars[carName];
	if ( car.mIsOwned )
		return false;

	int priceType = VuGameUtil::IF()->getCarPriceType(carName);
	int price = VuGameUtil::IF()->getCarPrice(carName);
	if (priceType == 1) //金币
	{
		if (price > getCurrency(CT_STANDARD))
			return false;
		mCurrencies[CT_STANDARD].mAmountSpent += price;
	}
	else if (priceType == 3) //钻石
	{
		if (price > getCurrency(CT_PREMIUM))
			return false;
		mCurrencies[CT_PREMIUM].mAmountSpent += price;
	}
	else
	{
		return false;
	}

	

	

	car.mIsOwned = true;

	mNumCarsBought++;

	// analytics - What are the most popular cars?
	{
		VuJsonContainer variables;

		variables["Highest Event Played"].putValue(VuStatsManager::IF()->getHighestEventPlayedIndex());
		variables["Coins Earned"].putValue(getCurrencyEarned(VuGameManager::CT_STANDARD));
		variables["Coins Spent"].putValue(getCurrencySpent(VuGameManager::CT_STANDARD));
		variables["Gems Earned"].putValue(getCurrencyEarned(VuGameManager::CT_PREMIUM));
		variables["Gems Spent"].putValue(getCurrencySpent(VuGameManager::CT_PREMIUM));
		variables["IAP Made"].putValue(getIapMade());

		VuAnalyticsManager::IF()->logEvent("Car Purchased", "Car Name", carName.c_str(), variables);
	}

	// analytics - When are players purchasing each car?
	{
		VuJsonContainer variables;

		variables["Number of Cars Bought"].putValue(mNumCarsBought);
		variables["Coins Earned"].putValue(getCurrencyEarned(VuGameManager::CT_STANDARD));
		variables["Coins Spent"].putValue(getCurrencySpent(VuGameManager::CT_STANDARD));
		variables["Gems Earned"].putValue(getCurrencyEarned(VuGameManager::CT_PREMIUM));
		variables["Gems Spent"].putValue(getCurrencySpent(VuGameManager::CT_PREMIUM));
		variables["IAP Made"].putValue(getIapMade());

		char str[256];
		VU_SPRINTF(str, sizeof(str), "Car %s Purchased", carName.c_str());
		VuAnalyticsManager::IF()->logEvent(str, "Highest Event Played", VuStatsManager::IF()->getHighestEventPlayedName().c_str(), variables);
	}

	VuProfileManager::IF()->save();
	VuCloudSaveManager::IF()->save();

	VuAchievementUtil::recalculateAchievements();

	return true;
}

//*****************************************************************************
bool VuGameManager::purchaseUpgradedCar(const std::string &carName)
{
	Car &car = mCars[carName];

	if ( !car.mIsOwned )
		return false;
	
	if ( car.mStage != 0 )
		return false;
	
	if ( getUnlockedStage() <= 0 )
		return false;

	int price = VuGameUtil::IF()->getUpgradedCarPrice(carName);

	if ( price > getCurrency(CT_STANDARD) )
		return false;

	mCurrencies[CT_STANDARD].mAmountSpent += price;

	car.mIsOwned = true;
	car.mStage = getUnlockedStage();

	mNumCarsBought++;

	// analytics - What are the most popular cars?
	{
		VuJsonContainer variables;

		variables["Highest Event Played"].putValue(VuStatsManager::IF()->getHighestEventPlayedIndex());
		variables["Coins Earned"].putValue(getCurrencyEarned(VuGameManager::CT_STANDARD));
		variables["Coins Spent"].putValue(getCurrencySpent(VuGameManager::CT_STANDARD));
		variables["Gems Earned"].putValue(getCurrencyEarned(VuGameManager::CT_PREMIUM));
		variables["Gems Spent"].putValue(getCurrencySpent(VuGameManager::CT_PREMIUM));
		variables["IAP Made"].putValue(getIapMade());

		VuAnalyticsManager::IF()->logEvent("Car Purchased", "Car Name", carName.c_str(), variables);
	}

	// analytics - When are players purchasing each car?
	{
		VuJsonContainer variables;

		variables["Number of Cars Bought"].putValue(mNumCarsBought);
		variables["Coins Earned"].putValue(getCurrencyEarned(VuGameManager::CT_STANDARD));
		variables["Coins Spent"].putValue(getCurrencySpent(VuGameManager::CT_STANDARD));
		variables["Gems Earned"].putValue(getCurrencyEarned(VuGameManager::CT_PREMIUM));
		variables["Gems Spent"].putValue(getCurrencySpent(VuGameManager::CT_PREMIUM));
		variables["IAP Made"].putValue(getIapMade());

		char str[256];
		VU_SPRINTF(str, sizeof(str), "Car %s Purchased", carName.c_str());
		VuAnalyticsManager::IF()->logEvent(str, "Highest Event Played", VuStatsManager::IF()->getHighestEventPlayedName().c_str(), variables);
	}

	VuProfileManager::IF()->save();
	VuCloudSaveManager::IF()->save();

	VuAchievementUtil::recalculateAchievements();

	return true;
}

//*****************************************************************************
bool VuGameManager::purchaseCarUpgrade(const std::string &statName)
{
	Car &car = mCars[mCurCarName];
	if ( !car.mIsOwned )
		return false;

	int curLevel = car.getStat(statName.c_str());
	if ( curLevel >= car.getMaxLevel() )
		return false;

	int nextLevel = curLevel + 1;

	int priceSC = VuGameUtil::IF()->getCarUpgradePrice(car.mName, car.mStage, nextLevel, statName);
	if ( priceSC > VuGameManager::IF()->getCurrency(CT_STANDARD) )
		return false;

	mCurrencies[CT_STANDARD].mAmountSpent += priceSC;

	car.setStat(statName.c_str(), nextLevel);

	mNumUpgradesBought++;

	VuProfileManager::IF()->save();
	VuCloudSaveManager::IF()->save();

	return true;
}

//*****************************************************************************
bool VuGameManager::purchaseCarLevelUp(const std::string &carName, int newStage)
{
	Car &car = mCars[carName];
	if ( car.mIsOwned && car.mStage >= newStage )
		return true;

	int priceSC = VuGameUtil::IF()->getCarLevelUpPrice(carName, newStage);
	if ( priceSC > VuGameManager::IF()->getCurrency(CT_STANDARD) )
		return false;

	mCurrencies[CT_STANDARD].mAmountSpent += priceSC;

	car.mStage = newStage;
	car.mAccel = 0;
	car.mSpeed = 0;
	car.mHandling = 0;
	car.mTough = 0;

	if ( !car.mIsOwned )
	{
		car.mIsOwned = true;

		mNumCarsBought++;

		// analytics - What are the most popular cars?
		{
			VuJsonContainer variables;

			variables["Highest Event Played"].putValue(VuStatsManager::IF()->getHighestEventPlayedIndex());
			variables["Coins Earned"].putValue(getCurrencyEarned(VuGameManager::CT_STANDARD));
			variables["Coins Spent"].putValue(getCurrencySpent(VuGameManager::CT_STANDARD));
			variables["Gems Earned"].putValue(getCurrencyEarned(VuGameManager::CT_PREMIUM));
			variables["Gems Spent"].putValue(getCurrencySpent(VuGameManager::CT_PREMIUM));
			variables["IAP Made"].putValue(getIapMade());

			VuAnalyticsManager::IF()->logEvent("Car Purchased", "Car Name", carName.c_str(), variables);
		}

		// analytics - When are players purchasing each car?
		{
			VuJsonContainer variables;

			variables["Number of Cars Bought"].putValue(mNumCarsBought);
			variables["Coins Earned"].putValue(getCurrencyEarned(VuGameManager::CT_STANDARD));
			variables["Coins Spent"].putValue(getCurrencySpent(VuGameManager::CT_STANDARD));
			variables["Gems Earned"].putValue(getCurrencyEarned(VuGameManager::CT_PREMIUM));
			variables["Gems Spent"].putValue(getCurrencySpent(VuGameManager::CT_PREMIUM));
			variables["IAP Made"].putValue(getIapMade());

			char str[256];
			VU_SPRINTF(str, sizeof(str), "Car %s Purchased", carName.c_str());
			VuAnalyticsManager::IF()->logEvent(str, "Highest Event Played", VuStatsManager::IF()->getHighestEventPlayedName().c_str(), variables);
		}
	}

	VuProfileManager::IF()->save();
	VuCloudSaveManager::IF()->save();

	VuAchievementUtil::recalculateAchievements();

	return true;
}

//*****************************************************************************
bool VuGameManager::giveCarStage()
{
	Car &car = mCars[mCurCarName];
	if ( !car.mIsOwned )
		return false;

	if ( car.mStage >= 3 )
		return false;

	car.mStage++;
	car.mAccel = 0;
	car.mSpeed = 0;
	car.mHandling = 0;
	car.mTough = 0;

	VuProfileManager::IF()->save();
	VuCloudSaveManager::IF()->save();

	return true;
}

//*****************************************************************************
bool VuGameManager::giveCar(const std::string &carName)
{
	Car &car = mCars[carName];
	if ( car.mIsOwned )
		return false;

	car.mIsOwned = true;
	VuAnalyticsManager::IF()->logEvent("Buy car", carName.c_str(), 1);
	VuAchievementUtil::recalculateAchievements();

	return true;
}


bool VuGameManager::giveFullLvlCar(const std::string &carName)
{
	Car &car = mCars[carName];
	if (car.mIsOwned)
		return false;

	car.mIsOwned = true;
	int maxStage = VuGameUtil::IF()->carDB()[carName]["skycn_maxStage"].asInt();
	car.mStage = VuGameUtil::IF()->carDB()[carName]["skycn_maxStage"].asInt();
	int maxLevel = car.getMaxLevel();
	car.mAccel = maxLevel;
	car.mSpeed = maxLevel;
	car.mHandling = maxLevel;
	car.mTough = maxLevel;

	VuAchievementUtil::recalculateAchievements();

	return true;
}
//*****************************************************************************
bool VuGameManager::giveCarUpgrade(const std::string &statName)
{
	Car &car = mCars[mCurCarName];
	if ( !car.mIsOwned )
		return false;

	int curLevel = car.getStat(statName.c_str());
	if ( curLevel >= car.getMaxLevel() )
		return false;

	int nextLevel = curLevel + 1;

	car.setStat(statName.c_str(), nextLevel);

	return true;
}

//*****************************************************************************
void VuGameManager::paintCar(const std::string &decal, const std::string &decalColor, const std::string &paintColor)
{
	Car &car = mCars[mCurCarName];

	car.mDecal = decal;
	car.mDecalColor = decalColor;
	car.mPaintColor = paintColor;
}

//*****************************************************************************
void VuGameManager::setCurDriver(const std::string &driverName)
{
	if ( getDriver(driverName).isOnTeam() )
		mCurDriverName = driverName;
}

//*****************************************************************************
void VuGameManager::setDriverBeaten(const std::string &driverName)
{
	if ( !mDrivers[driverName].mIsBeaten )
	{
		mDrivers[driverName].mIsBeaten = true;

		if (!mDrivers[driverName].mIsPurchased)
			mDrivers[driverName].mIsInteresting = true;
		//mDrivers[driverName].mIsInteresting = true;

		// analytics - How many times does it take to beat a boss?
		{
			VuJsonContainer variables;
			variables["Num Losses"].putValue(mDrivers[driverName].mNumBossLosses);
			VuAnalyticsManager::IF()->logEvent("Boss Beaten", "Boss Name", driverName.c_str(), variables);
		}
	}
}

void VuGameManager::setDriverBeatenInDuelEvent(const std::string &driverName)
{
	Driver &driver = mDrivers[driverName];
	if (driver.isOnTeam())
		return;

	driver.mIsPurchased = true;
}

//*****************************************************************************
bool VuGameManager::getDriverBeaten(const std::string &driverName)
{
	Drivers::iterator it = mDrivers.find(driverName);
	if (it == mDrivers.end())
		return false;

	return it->second.mIsBeaten;
}


//*****************************************************************************
bool VuGameManager::purchaseDriver(const std::string &driverName)
{
	Driver &driver = mDrivers[driverName];
	if ( driver.isOnTeam() )
		return false;

	int price = VuGameUtil::IF()->getDriverPrice(driverName);

	if ( price > getCurrency(CT_PREMIUM) )
		return false;

	mCurrencies[CT_PREMIUM].mAmountSpent += price;

	driver.mIsPurchased = true;

	

	VuProfileManager::IF()->save();
	VuCloudSaveManager::IF()->save();

	VuAchievementUtil::recalculateAchievements();

	return true;
}

//*****************************************************************************
bool VuGameManager::purchaseDriverFromPack(const std::string &driverName)
{
	Driver &driver = mDrivers[driverName];
	if (driver.isOnTeam())
		return false;

	driver.mIsPurchased = true;

	VuProfileManager::IF()->save();
	VuCloudSaveManager::IF()->save();

	VuAchievementUtil::recalculateAchievements();

	return true;
}

//*****************************************************************************
void VuGameManager::lostToBoss(const std::string& bossName)
{
	mDrivers[bossName].mNumBossLosses++;
}

//*****************************************************************************
int VuGameManager::numBossLosses(const std::string& bossName)
{
	Drivers::iterator it = mDrivers.find(bossName);
	if (it == mDrivers.end())
		return 0;

	return it->second.mNumBossLosses;
}

//*****************************************************************************
void VuGameManager::clearDriverInteresting(const std::string &driverName)
{
	Drivers::iterator it = mDrivers.find(driverName);
	if ( it != mDrivers.end() )
		it->second.mIsInteresting = false;
}

//*****************************************************************************
bool VuGameManager::isDriverInteresting(const std::string &driverName)
{
	Drivers::iterator it = mDrivers.find(driverName);
	if ( it != mDrivers.end() )
		return it->second.mIsInteresting;

	return false;
}

//*****************************************************************************
bool VuGameManager::purchasePowerUp(const std::string &powerUpName)
{
	if ( isPowerUpOwned(powerUpName) )
		return false;

	int price = VuGameUtil::IF()->getPowerUpPrice(powerUpName);
	if ( price > getCurrency(CT_PREMIUM) )
		return false;

	mCurrencies[CT_PREMIUM].mAmountSpent += price;

	mOwnedPowerUps.insert(powerUpName);

	mNumPowerUpsBought++;

	// analytics - What are the most popular power-up?
	{
		VuJsonContainer variables;

		variables["Highest Event Played"].putValue(VuStatsManager::IF()->getHighestEventPlayedIndex());
		variables["Coins Earned"].putValue(getCurrencyEarned(VuGameManager::CT_STANDARD));
		variables["Coins Spent"].putValue(getCurrencySpent(VuGameManager::CT_STANDARD));
		variables["Gems Earned"].putValue(getCurrencyEarned(VuGameManager::CT_PREMIUM));
		variables["Gems Spent"].putValue(getCurrencySpent(VuGameManager::CT_PREMIUM));
		variables["IAP Made"].putValue(getIapMade());

		VuAnalyticsManager::IF()->logEvent("Power-Up Purchased", "Power-Up Name", powerUpName.c_str(), variables);
	}

	// analytics - When are players purchasing each power-up?
	{
		VuJsonContainer variables;

		variables["Number of Power-Ups Bought"].putValue(mNumPowerUpsBought);
		variables["Coins Earned"].putValue(getCurrencyEarned(VuGameManager::CT_STANDARD));
		variables["Coins Spent"].putValue(getCurrencySpent(VuGameManager::CT_STANDARD));
		variables["Gems Earned"].putValue(getCurrencyEarned(VuGameManager::CT_PREMIUM));
		variables["Gems Spent"].putValue(getCurrencySpent(VuGameManager::CT_PREMIUM));
		variables["IAP Made"].putValue(getIapMade());

		char str[256];
		VU_SPRINTF(str, sizeof(str), "Power-Up %s Purchased", powerUpName.c_str());
		VuAnalyticsManager::IF()->logEvent(str, "Highest Event Played", VuStatsManager::IF()->getHighestEventPlayedName().c_str(), variables);
	}

	VuProfileManager::IF()->save();
	VuCloudSaveManager::IF()->save();

	VuAchievementUtil::recalculateAchievements();

	return true;
}

//*****************************************************************************
bool VuGameManager::givePowerUp(const std::string &powerUpName)
{
	if ( isPowerUpOwned(powerUpName) )
		return false;

	mOwnedPowerUps.insert(powerUpName);

	VuAchievementUtil::recalculateAchievements();

	return true;
}

//*****************************************************************************
void VuGameManager::beginSpecial(const std::string &name, float duration)
{
	if ( duration <= 0.0f )
		return;

	// check if the special is currently active
	if ( mActiveSpecials.find(name) != mActiveSpecials.end() )
		return;

	const VuJsonContainer &data = VuGameUtil::IF()->specialDB()[name];
	if ( !data.isObject() )
		return;

	const std::string &storeItemName = data["StoreItem"].asString();
	if ( VuBillingManager::IF()->isOwned(storeItemName.c_str()) )
		return;

	Special &special = mActiveSpecials[name];

	VUINT64 currentTime = VuTimeUtil::calcSecondsSince2000();

	special.mExpirationTime = VuTimeUtil::calcSecondsSince2000() + VuRound(duration*3600);
	special.mStoreItemName = storeItemName;
	special.mOneShot = data["OneShot"].asBool();
}

//*****************************************************************************
bool VuGameManager::isConfirmedPirate()
{
	if ( getCurrency(CT_STANDARD) >= SUSPICIOUS_SC_AMOUNT )
		return true;

	if ( getCurrency(CT_PREMIUM) >= SUSPICIOUS_PC_AMOUNT )
		return true;

	return false;
}

//*****************************************************************************
void VuGameManager::OnSaveProfile(const VuParams &params)
{
	save();

	VuAnalyticsManager::IF()->setCurrency(1, getCurrency(CT_STANDARD));
	VuAnalyticsManager::IF()->setCurrency(2, getCurrency(CT_PREMIUM));
}

//*****************************************************************************
void VuGameManager::OnCloudDataPostSync(const VuParams &params)
{
	load();

	VuEventManager::IF()->broadcast("OnGameManagerPostSync");

	VuAchievementUtil::recalculateAchievements();
}

//*****************************************************************************
void VuGameManager::OnPurchaseMade(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	const char *itemName = accessor.getString();

	// specials
	for ( Specials::iterator iter = mActiveSpecials.begin(); iter != mActiveSpecials.end(); )
	{
		if ( iter->second.mStoreItemName.compare(itemName) == 0 )
			mActiveSpecials.erase(iter++);
		else
			iter++;
	}
}

//*****************************************************************************
void VuGameManager::setDefaults()
{
	for ( int i = 0; i < CURRENCY_TYPE_COUNT; i++ )
		mCurrencies[i].setDefaults();

	mCurrencies[CT_STANDARD].mAmountEarned = 800;
	mCars.clear();
	mCurCarName.clear();

	//modify by zc[2015/11/3]
	m_GiftDialogStates.clear();
	m_GiftDialogButtonStates.clear();
	//modify by zc [2015/8/9]
	m_Orders.clear();

	// gather cars
	const VuGameUtil::Names &carNames = VuGameUtil::IF()->getCarNames();
	mFullLvlCarNames.clear();
	for (VUUINT i = 0; i < carNames.size(); i++)
	{
		const std::string &carName = carNames[i];

		const VuJsonContainer &carData = VuGameUtil::IF()->carDB()[carName];

		Car &car = mCars[carName];
		car.mName = carName;

		const VuJsonContainer &factoryPaintJob = VuGameUtil::IF()->carSkinDB()[carName];
		car.mDecal = factoryPaintJob["Decal"].asCString();
		car.mPaintColor = factoryPaintJob["PaintColor"].asCString();
		car.mDecalColor = factoryPaintJob["DecalColor"].asCString();

		// cars that cost 0 are owned
		int price = VuGameUtil::IF()->getCarPrice(carName);

		if ( price == 0 )
		{
			if ( mCurCarName.empty() )
				mCurCarName = carName;

			car.mIsFree = true;
			car.mIsOwned = true;
		}

		int curFulPrice = VuGameUtil::IF()->getFullLevelCarCurPrice(carName);
		int primaryFulPrice = VuGameUtil::IF()->getFullLevelCarPrimaryPrice(carName);
		if (curFulPrice > 0 && primaryFulPrice)
		{
			mFullLvlCarNames.push_back(carName);
			FullLvlCar &fulCar = mFullLvlCar[carName];
			fulCar.mCarName = carName;
			fulCar.mCarTitle = "Car_"+carName;
			fulCar.mCarSubTitle = std::string(VuStringDB::IF()->getString("FullLvl"));
			fulCar.mCurrentPrice = curFulPrice;
			fulCar.mDiscountPrice = (primaryFulPrice - curFulPrice) > 0 ? (primaryFulPrice - curFulPrice) : 0;
			fulCar.mDiscountInfo =  int (curFulPrice * 10.0f / primaryFulPrice + 0.5);
		}
	}

	mDrivers.clear();
	mCurDriverName.clear();

	// gather drivers
	const VuGameUtil::Names &driverNames = VuGameUtil::IF()->getDriverNames();
	for (VUUINT i = 0; i < driverNames.size(); i++)
	{
		const std::string &driverName = driverNames[i];

		Driver &driver = mDrivers[driverName];
		driver.mName = driverName;

		if ( mCurDriverName.empty() )
		{
			mCurDriverName = driverName;
			driver.mIsDefault = true;
		}
	}

	mUsedOffers.clear();

	// owned power-ups
	mOwnedPowerUps.clear();
	for ( int i = 0; i < VuPowerUpManager::IF()->getPowerUpCount(); i++ )
	{
		const VuPowerUp *pPowerUp = VuPowerUpManager::IF()->getPowerUp(i);
		if ( pPowerUp->mPrice == 0 )
			mOwnedPowerUps.insert(pPowerUp->mName);
	}

	mTuneUps = VuGameUtil::IF()->constantDB()["Game"]["InitialTuneUps"].asInt();
	mTokens = VuCloudTuningManager::IF()->variables().mMaxTokens;
	mTokenTimerStart = 0;
	mTokensEverExhausted = false;

	for ( int i = 0; i < CALENDAR_COUNT; i++ )
		mCalendars[i].clear();

	mUnlockedStage = 0;
	mIapMade = false;

	mNumCarsBought = 0;
	mNumUpgradesBought = 0;
	mNumDriversBought = 0;
	mNumPowerUpsBought = 0;
	mNumTuneUpsBought = 0;

	mGameVersion = GAME_PRIMARY_VERSION;

	mFlaggedAsPirate = false;
	mBroadCastVersion = 0;
	mBroadCastSize = 0;
	mIsBroadcastDataOK = false;
	mActiveSpecials.clear();

	mGameVersion = GAME_PRIMARY_VERSION;
	bVerNeedChange = false;
}

//*****************************************************************************
void VuGameManager::load()
{
	setDefaults();

 	const VuJsonContainer &gameData = VuProfileManager::IF()->dataRead()["Game"];

	mCurrencies[CT_STANDARD].load(gameData["SC"]);
	mCurrencies[CT_PREMIUM].load(gameData["PC"]);

	gameData["GameVersion"].getValue(mGameVersion);
	if (strcmp(mGameVersion.c_str(), GAME_VERSION) != 0)
	{
		bVerNeedChange = true;
		mGameVersion = GAME_VERSION;
	}


	const VuJsonContainer &cars = gameData["Cars"];
	const VuGameUtil::Names &carNames = VuGameUtil::IF()->getCarNames();
	for ( VUUINT i = 0; i < carNames.size(); i++ )
	{
		const std::string &carName = carNames[i];
		mCars[carName].mName = carName;
		mCars[carName].load(cars[carName]);
	}
	gameData["CurCarName"].getValue(mCurCarName);

	//modify by zc[2015/11/3]
	const VuJsonContainer& giftDialogStatus = gameData["GiftDialogStatus"];
	for (int i = 0; i < giftDialogStatus.size(); i++)
	{
		std::string strGiftDialog = giftDialogStatus[i]["Key"].asString();
		int GiftDialogStatus = giftDialogStatus[i]["Value"].asInt();

		m_GiftDialogStates[strGiftDialog] = GiftDialogStatus;
	}
	const VuJsonContainer& giftDialogButtonStatus = gameData["GiftDialogButtonStatus"];
	for (int i = 0; i < giftDialogButtonStatus.size(); i++)
	{
		std::string strGiftDialogButton = giftDialogButtonStatus[i]["Key"].asString();
		int GiftDialogButtonStatus = giftDialogButtonStatus[i]["Value"].asInt();

		m_GiftDialogButtonStates[strGiftDialogButton] = GiftDialogButtonStatus;
	}

	m_GiftDialogVersion = gameData["GiftDialogVersion"].asInt();
	//modify zc [2015/8/9]
	const VuJsonContainer& orderIds = gameData["Orders"];
	for (int i = 0; i < orderIds.size(); i++)
	{
		m_Orders.push_back(std::vector<Order>::value_type(orderIds[i]["OrderId"].asString(),orderIds[i]["OutTradeNo"].asString(),orderIds[i]["ItemName"].asString(),orderIds[i]["ItemNum"].asInt()));
	}

	const VuJsonContainer &drivers = gameData["Drivers"];
	const VuGameUtil::Names &driverNames = VuGameUtil::IF()->getDriverNames();
	for ( VUUINT i = 0; i < driverNames.size(); i++ )
	{
		const std::string &driverName = driverNames[i];
		mDrivers[driverName].mName = driverName;
		mDrivers[driverName].load(drivers[driverName]);
	}
	gameData["CurDriverName"].getValue(mCurDriverName);

	const VuJsonContainer &usedOffers = gameData["UsedOffers"];
	for ( int i = 0; i < usedOffers.size(); i++ )
		mUsedOffers.insert(usedOffers[i].asString());

	const VuJsonContainer &ownedPowerUps = gameData["OwnedPowerUps"];
	for ( int i = 0; i < ownedPowerUps.size(); i++ )
	{
		const std::string &powerUpName = ownedPowerUps[i].asString();
		mOwnedPowerUps.insert(powerUpName);
	}

	gameData["TuneUps"].getValue(mTuneUps);
	gameData["Tokens"].getValue(mTokens);
	gameData["TokenTimerStart"].getValue(mTokenTimerStart);
	gameData["TokensEverExhausted"].getValue(mTokensEverExhausted);

	mCalendars[DAILY_CHALLENGE_CALENDAR].load(gameData["DailyChallenges"]);
	mCalendars[DAILY_REWARD_CALENDAR].load(gameData["DailyRewards"]);

	gameData["UnlockedStage"].getValue(mUnlockedStage);
	gameData["IapMade"].getValue(mIapMade);

	gameData["NumCarsBought"].getValue(mNumCarsBought);
	gameData["NumUpgradesBought"].getValue(mNumUpgradesBought);
	gameData["NumDriversBought"].getValue(mNumDriversBought);
	gameData["NumPowerUpsBought"].getValue(mNumPowerUpsBought);
	gameData["NumTuneUpsBought"].getValue(mNumTuneUpsBought);

	gameData["FlaggedAsPirate"].getValue(mFlaggedAsPirate);



// modify by xlm,[/2015/2/8 ] ,从配置文件中加载数据
	gameData["showImmediateButtonTip"].getValue(m_bShowImmediateButtonTip); //是不是显示立即领取
	gameData["lastReceivedSvrTime"].getValue(m_llLastServerTime);
	gameData["receivedDays"].getValue(m_nReceivedDays);

	gameData["dailyrewardlasttime"].getValue(m_llDailyRewardLastTime);

//modify by zc[2015/9/10]
	gameData["LastCarChampChallengeTime"].getValue(m_lastCarChampChallengeTime);
// modify by zc[/2015/6/1 ] 
 	gameData["PlayerName"].getValue(mPlayerName);
	gameData["Uid"].getValue(mUid);
	gameData["GetNameTimes"].getValue(mGetNameTimes);
	gameData["TempPlayerName"].getValue(mTempPlayerName);
	gameData["1_8VersionFirstLogin"].getValue(mIs1_8VersionFirstLogin);
	//modify by zc[2015/7/2]
	{
		std::string strbcImage = "BROADCAST_IMAGE";
		std::string strbcNameImage = "BROADCAST_NAME_IMAGE";
		std::string strbcImageSize = "BROADCAST_IMAGE_SIZE";
		std::string strbcNameImageSize = "BROADCAST_NAME_IMAGE_SIZE";

		gameData["BROADCAST_IMAGE_SIZE"].getValue(mBroadCastSize);
		
		mBroadcastImages.clear();
		mBroadcastNameImages.clear();
		mBroadcastImages.resize(mBroadCastSize);
		mBroadcastNameImages.resize(mBroadCastSize);
		for (int i = 0; i < mBroadCastSize; i++)
		{
			char c[8];
			sprintf(c, "%d", i);

			//mBroadcastImageSizes.resize(mBroadCastSize);
			//std::string broadcastImageSize = strbcImageSize + c;
			//gameData[broadcastImageSize].getValue(mBroadcastImageSizes[i]);
			
			//mBroadcastNameImageSizes.resize(mBroadCastSize);
			//std::string broadcastNameImageSize = strbcNameImageSize + c;
			//gameData[broadcastNameImageSize].getValue(mBroadcastNameImageSizes[i]);

			
			std::string broadcastImage = strbcImage + c;
			const void *tempp1;
			mBroadcastImageSizes.resize(mBroadCastSize);
			gameData[broadcastImage].getValue(tempp1, mBroadcastImageSizes[i]);
			mBroadcastImages[i] = new VUBYTE[mBroadcastImageSizes[i]];
			VU_MEMCPY(mBroadcastImages[i], mBroadcastImageSizes[i], tempp1, mBroadcastImageSizes[i]);

			const void* tempp2;
			mBroadcastNameImageSizes.resize(mBroadCastSize);
			std::string broadcastNameImage = strbcNameImage + c;
			gameData[broadcastNameImage].getValue(tempp2,mBroadcastNameImageSizes[i]);
			mBroadcastNameImages[i] = new VUBYTE[mBroadcastNameImageSizes[i]];
			VU_MEMCPY(mBroadcastNameImages[i], mBroadcastNameImageSizes[i], tempp2, mBroadcastNameImageSizes[i]);
			//mBroadcastNameImages[i] = (VUBYTE*)tempp2;
		}

		
		gameData["BROADCAST_VERSION"].getValue(mBroadCastVersion);
		gameData["BROADCAST_DATA_OK"].getValue(mIsBroadcastDataOK);
	}



// modify by xlm,[/2014/12/3 ] 修改加载项

	//新手礼包

/*
	__AddActiveSpecials("StarterPack");
	__AddActiveSpecials("DisableAds");*/
	

	const VuJsonContainer &specials = gameData["Specials"];

	int nCount = specials.numMembers();
	for ( int i = 0; i < specials.numMembers(); i++ )
	{
		const std::string &key = specials.getMemberKey(i);

		const VuJsonContainer &data = VuGameUtil::IF()->specialDB()[key];
		if ( data.isObject() )
		{
			mActiveSpecials[key].mExpirationTime = specials[key].asInt64();
			mActiveSpecials[key].mStoreItemName = data["StoreItem"].asString();
			mActiveSpecials[key].mOneShot = data["OneShot"].asBool();
		}
	}
}

void VuGameManager::__AddActiveSpecials(std::string strKey)
{
	const VuJsonContainer &data = VuGameUtil::IF()->specialDB()[strKey];

	if (data.isObject())
	{
		mActiveSpecials[strKey].mExpirationTime = MAX_64_BIT;/*specials[key].asInt64();*/
		mActiveSpecials[strKey].mStoreItemName = data["StoreItem"].asString();
		mActiveSpecials[strKey].mOneShot = data["OneShot"].asBool();
	}

}

void				VuGameManager::setCalendarDay(eCalendars calendar, int daysSince2000)
{ 
	if (calendar == DAILY_REWARD_CALENDAR)
	{
		time((time_t*)&m_llDailyRewardLastTime);
		
	}
	mCalendars[calendar].setDay(daysSince2000); 
}

bool				VuGameManager::getCalendarDay(eCalendars calendar, int daysSince2000)
{ 
	if (calendar == DAILY_REWARD_CALENDAR)
	{
		if (m_llDailyRewardLastTime == 0)
		{
			return false;
		}
		else
		{
			time_t nowTime;
			time(&nowTime);
			if (nowTime > m_llDailyRewardLastTime && !IsSameDay(nowTime, m_llDailyRewardLastTime))
			{
				return false;
			}
			return true;
		}
		return true;
		
	}
	return mCalendars[calendar].getDay(daysSince2000); 
}

int					VuGameManager::getCalendarDaysInRow(eCalendars calendar, int daysSince2000)
{ 
	return mCalendars[calendar].getDaysInRow(daysSince2000); 
}

//*****************************************************************************
void VuGameManager::save()
{
	VuJsonContainer &gameData = VuProfileManager::IF()->dataWrite()["Game"];
	gameData.clear();

	mCurrencies[CT_STANDARD].save(gameData["SC"]);
	mCurrencies[CT_PREMIUM].save(gameData["PC"]);

	//modify by zc[2015/11/3]
	VuJsonContainer& giftDialogStatus = gameData["GiftDialogStatus"];
	for (std::map<std::string, int>::iterator iter = m_GiftDialogStates.begin(); iter != m_GiftDialogStates.end(); iter++)
	{
		VuJsonContainer& tempContainer = giftDialogStatus.append();
		tempContainer["Key"].putValue(iter->first);
		tempContainer["Value"].putValue(iter->second);
	}

	VuJsonContainer& giftDialogButtonStatus = gameData["GiftDialogButtonStatus"];
	for (std::map<std::string, int>::iterator iter = m_GiftDialogButtonStates.begin(); iter != m_GiftDialogButtonStates.end(); iter++)
	{
		VuJsonContainer& tempContainer = giftDialogButtonStatus.append();
		tempContainer["Key"].putValue(iter->first);
		tempContainer["Value"].putValue(iter->second);
	}

	gameData["GiftDialogVersion"].putValue(m_GiftDialogVersion);

	//modify by zc[2015/8/9]
	VuJsonContainer& orderIds = gameData["Orders"];

	for (std::vector<Order>::iterator iter = m_Orders.begin(); iter != m_Orders.end(); iter++)
	{
		VuJsonContainer& tempContainer = orderIds.append();
		tempContainer["OrderId"].putValue((*iter).m_orderId);
		tempContainer["ItemName"].putValue((*iter).m_itemName);
		tempContainer["ItemNum"].putValue((*iter).m_itemNum);
		tempContainer["OutTradeNo"].putValue((*iter).m_out_trade_no);
	}
	VuJsonContainer &cars = gameData["Cars"];
	const VuGameUtil::Names &carNames = VuGameUtil::IF()->getCarNames();
	for ( VUUINT i = 0; i < carNames.size(); i++ )
	{
		const std::string &carName = carNames[i];
		mCars[carName].save(cars[carName]);
	}
	gameData["CurCarName"].putValue(mCurCarName);

	VuJsonContainer &drivers = gameData["Drivers"];
	const VuGameUtil::Names &driverNames = VuGameUtil::IF()->getDriverNames();
	for ( VUUINT i = 0; i < driverNames.size(); i++ )
	{
		const std::string &driverName = driverNames[i];
		mDrivers[driverName].save(drivers[driverName]);
	}
	gameData["CurDriverName"].putValue(mCurDriverName);

	VuJsonContainer &usedOffers = gameData["UsedOffers"];
	for ( Offers::const_iterator iter = mUsedOffers.begin(); iter != mUsedOffers.end(); iter++ )
		usedOffers.append().putValue(*iter);

	VuJsonContainer &powerUps = gameData["OwnedPowerUps"];
	for ( OwnedPowerUps::const_iterator iter = mOwnedPowerUps.begin(); iter != mOwnedPowerUps.end(); iter++ )
		powerUps.append().putValue(*iter);

	gameData["TuneUps"].putValue(mTuneUps);
	gameData["Tokens"].putValue(mTokens);
	gameData["TokenTimerStart"].putValue(mTokenTimerStart);
	gameData["TokensEverExhausted"].putValue(mTokensEverExhausted);

	mCalendars[DAILY_CHALLENGE_CALENDAR].save(gameData["DailyChallenges"]);
	mCalendars[DAILY_REWARD_CALENDAR].save(gameData["DailyRewards"]);

	gameData["UnlockedStage"].putValue(mUnlockedStage);
	gameData["IapMade"].putValue(mIapMade);

	gameData["NumCarsBought"].putValue(mNumCarsBought);
	gameData["NumUpgradesBought"].putValue(mNumUpgradesBought);
	gameData["NumDriversBought"].putValue(mNumDriversBought);
	gameData["NumPowerUpsBought"].putValue(mNumPowerUpsBought);
	gameData["NumTuneUpsBought"].putValue(mNumTuneUpsBought);

	gameData["FlaggedAsPirate"].putValue(mFlaggedAsPirate);





	// modify by xlm,[/2015/2/8 ] ,从配置文件中加载数据
	gameData["showImmediateButtonTip"].putValue(m_bShowImmediateButtonTip); //是不是显示立即领取
	gameData["lastReceivedSvrTime"].putValue(m_llLastServerTime);
	gameData["receivedDays"].putValue(m_nReceivedDays);
	gameData["dailyrewardlasttime"].putValue(m_llDailyRewardLastTime);
	//modify by zc[2015/9/10]
	gameData["LastCarChampChallengeTime"].putValue(m_lastCarChampChallengeTime);
	//modify by zc[2015/6/1]
	gameData["PlayerName"].putValue(mPlayerName);
	gameData["Uid"].putValue(mUid);
	gameData["GetNameTimes"].putValue(mGetNameTimes);
	gameData["TempPlayerName"].putValue(mTempPlayerName);
	gameData["1_8VersionFirstLogin"].putValue(mIs1_8VersionFirstLogin);
	//modify by zc[2015/7/2]
	{
		std::string strbcImage = "BROADCAST_IMAGE";
		std::string strbcNameImage = "BROADCAST_NAME_IMAGE";
		std::string strbcImageSize = "BROADCAST_IMAGE_SIZE";
		std::string strbcNameImageSize = "BROADCAST_NAME_IMAGE_SIZE";
		for (int i = 0; i < mBroadCastSize; i++)
		{
			char c[8];
			sprintf(c, "%d", i);
			std::string broadcastImage = strbcImage + c;
			//const void* p = mBroadcastImages[i];
			gameData[broadcastImage].putValue(mBroadcastImages[i], mBroadcastImageSizes[i]);

			//const void* q = mBroadcastNameImages[i].c_str();
			std::string broadcastNameImage = strbcNameImage + c;
			gameData[broadcastNameImage].putValue(mBroadcastNameImages[i], mBroadcastNameImageSizes[i]);

			std::string broadcastImageSize = strbcImageSize + c;
			gameData[broadcastImageSize].putValue(mBroadcastImageSizes[i]);

			std::string broadcastNameImageSize = strbcNameImageSize + c;
			gameData[broadcastNameImageSize].putValue(mBroadcastNameImageSizes[i]);
		}

		gameData["BROADCAST_IMAGE_SIZE"].putValue(mBroadCastSize);
		gameData["BROADCAST_VERSION"].putValue(mBroadCastVersion);
		gameData["BROADCAST_DATA_OK"].putValue(mIsBroadcastDataOK);
	}

	gameData["GameVersion"].putValue(mGameVersion);

	VuJsonContainer &specials = gameData["Specials"];
	for ( const auto &iter : mActiveSpecials )
		specials[iter.first].putValue(iter.second.mExpirationTime);
}

#include <VuPurple/Util/VuGameUtil.h>
//*****************************************************************************
void VuGameManager::Car::load(const VuJsonContainer &data)
{
	//tony.hu
	skycn_minStage = VuGameUtil::IF()->carDB()[this->mName]["skycn_minStage"].asInt();
	skycn_maxStage = VuGameUtil::IF()->carDB()[this->mName]["skycn_maxStage"].asInt();
	if (mStage == 0){
		mStage = skycn_minStage;
	//	VUPRINTF("tony.hu Car=%s InitStage=%d", this->mName.c_str(), this->mStage );
	}
		
	//end tony.hu
	VuDataUtil::getValue(data["IsOwned"], mIsOwned);
	VuDataUtil::getValue(data["Stage"], mStage);
	VuDataUtil::getValue(data["Decal"], mDecal);
	VuDataUtil::getValue(data["DecalColor"], mDecalColor);
	VuDataUtil::getValue(data["PaintColor"], mPaintColor);
	VuDataUtil::getValue(data["Accel"], mAccel);
	VuDataUtil::getValue(data["Speed"], mSpeed);
	VuDataUtil::getValue(data["Handling"], mHandling);
	VuDataUtil::getValue(data["Tough"], mTough);
}

//*****************************************************************************
void VuGameManager::Car::save(VuJsonContainer &data) const
{
	VuDataUtil::putValue(data["IsOwned"], mIsOwned);
	VuDataUtil::putValue(data["Stage"], mStage);
	VuDataUtil::putValue(data["Decal"], mDecal);
	VuDataUtil::putValue(data["DecalColor"], mDecalColor);
	VuDataUtil::putValue(data["PaintColor"], mPaintColor);
	VuDataUtil::putValue(data["Accel"], mAccel);
	VuDataUtil::putValue(data["Speed"], mSpeed);
	VuDataUtil::putValue(data["Handling"], mHandling);
	VuDataUtil::putValue(data["Tough"], mTough);
}

//*****************************************************************************
int VuGameManager::Car::getStat(const char *statName) const
{
	int value = 0;

	if ( strcmp(statName, "Accel") == 0 ) value = mAccel;
	else if ( strcmp(statName, "Speed") == 0 ) value = mSpeed;
	else if ( strcmp(statName, "Handling") == 0 ) value = mHandling;
	else if ( strcmp(statName, "Tough") == 0 ) value = mTough;

	return value;
}

//*****************************************************************************
void VuGameManager::Car::setStat(const char *statName, int value)
{
	if ( strcmp(statName, "Accel") == 0 ) mAccel = value;
	else if ( strcmp(statName, "Speed") == 0 ) mSpeed = value;
	else if ( strcmp(statName, "Handling") == 0 ) mHandling = value;
	else if ( strcmp(statName, "Tough") == 0 ) mTough = value;
}

//*****************************************************************************
int VuGameManager::Car::getMaxLevel() const
{
	return VuGameUtil::IF()->getMaxCarLevel(mStage);
}

//*****************************************************************************
bool VuGameManager::Car::areStatsMaxed() const
{
	int maxLevel = getMaxLevel();

	return (mAccel >= maxLevel && mSpeed >= maxLevel && mHandling >= maxLevel && mTough >= maxLevel);
}

//*****************************************************************************
float VuGameManager::Car::getStageProgress() const
{
	int levels = VuGameUtil::IF()->getNumCarLevels(mStage);
	int maxLevel = levels - 1;
	return (float)(mAccel + mSpeed + mHandling + mTough)/(4*maxLevel);
}
//*****************************************************************************
int  VuGameManager::Car::getStage() const
{
	return mStage;
}

//*****************************************************************************
void VuGameManager::Driver::load(const VuJsonContainer &data)
{
	VuDataUtil::getValue(data["IsBeaten"], mIsBeaten);
	
	VuDataUtil::getValue(data["NumBossLosses"], mNumBossLosses);
	VuDataUtil::getValue(data["IsPurchased"], mIsPurchased);

	//modify by liudan
	if (mIsBeaten && (strcmp(mName.c_str(), "Skeleton") == 0))
	{
		if (VuGameManager::IF()->bVerNeedChange == true)
		{
			mIsBeaten = false;
			mIsPurchased = true;
			VuGameManager::IF()->bVerNeedChange = false;
		}
	}
}

//*****************************************************************************
void VuGameManager::Driver::save(VuJsonContainer &data) const
{
	VuDataUtil::putValue(data["IsBeaten"], mIsBeaten);
	
	VuDataUtil::putValue(data["NumBossLosses"], mNumBossLosses);
	VuDataUtil::putValue(data["IsPurchased"], mIsPurchased);
}

//*****************************************************************************
bool VuGameManager::Driver::isOnTeam() const
{
	if ( VuDevConfig::IF()->getParam("UnlockAll").asBool() )
		return true;

	return mIsDefault | mIsBeaten | mIsPurchased;
}

//*****************************************************************************
void VuGameManager::Currency::load(const VuJsonContainer &data)
{
	VuDataUtil::getValue(data["Earned"], mAmountEarned);
	VuDataUtil::getValue(data["Purchased"], mAmountPurchased);
	VuDataUtil::getValue(data["Spent"], mAmountSpent);
}

//*****************************************************************************
void VuGameManager::Currency::save(VuJsonContainer &data) const
{
	VuDataUtil::putValue(data["Earned"], mAmountEarned);
	VuDataUtil::putValue(data["Purchased"], mAmountPurchased);
	VuDataUtil::putValue(data["Spent"], mAmountSpent);
}

//*****************************************************************************
void VuGameManager::Currency::setDefaults()
{
	mAmountEarned = 0;
	mAmountPurchased = 0;
	mAmountSpent = 0;
}

//*****************************************************************************
void VuGameManager::Calendar::setDay(int daysSince2000)
{
	if ( daysSince2000 > 0 )
	{
		if ( mDays.find(daysSince2000) == mDays.end() )
		{
			mDays.insert(daysSince2000);
			VuProfileManager::IF()->save();
		}
	}
}

//*****************************************************************************
bool VuGameManager::Calendar::getDay(int daysSince2000)
{
	if ( mDays.find(daysSince2000) != mDays.end() )
		return true;

	return false;
}

//*****************************************************************************
int VuGameManager::Calendar::getDaysInRow(int daysSince2000)
{
	int daysInRow = 0;
	while ( mDays.find(daysSince2000) != mDays.end() )
	{
		daysInRow++;
		daysSince2000--;
	}

	return daysInRow;
}

//*****************************************************************************
void VuGameManager::Calendar::load(const VuJsonContainer &data)
{
	for ( int i = 0; i < data.size(); i++ )
		mDays.insert(data[i].asInt());
}

//*****************************************************************************
void VuGameManager::Calendar::save(VuJsonContainer &data)
{
	for ( int day : mDays )
		data.append().putValue(day);
}





// modify by xlm,[/2014/12/19 ] 
bool VuGameManager::isCarOwned(const std::string& carName)
{
	Car &car = mCars[carName];
	if (car.mIsOwned)
		return true;
	return false;
}



/*
@ Author:	xlm(2015-2-5 18:58)
@ Parameter:
@ Returns:
@ Description:获取当前已经领取了几天的登录奖励
*/
unsigned int VuGameManager::getCurrReceivedDays()
{
	return m_nReceivedDays;
}

/*
@ Author:	xlm(2015-2-5 19:01)
@ Parameter:
@ Returns:
@ Description:设置当前已经领取的天数
*/
void VuGameManager::setReceivedDays(unsigned int nDays)
{
	m_nReceivedDays = nDays;
}

/*
@ Author:	xlm(2015-2-5 19:03)
@ Parameter:
@ Returns:
@ Description:
*/
void VuGameManager::addReceivedDays()
{
	m_nReceivedDays++;

	m_llLastServerTime = m_llCurrServerTime;
	save();
}


/*
@ Author:	xlm(2015-2-6 10:10)
@ Parameter:
@ Returns:
@ Description: 1 表示已经领取  2 表示可领取  3表示未领取（不能领取）
*/
int VuGameManager::getCurrDayState(int nDay)
{
	if (nDay <= m_nReceivedDays)
	{
		return 1;
	}
	else if (nDay == (m_nReceivedDays+1))
	{
		if (m_llLastServerTime == 0)
		{
			return 2;
		}
		else
		{
			unsigned int nNowDay = m_llCurrServerTime / DAY_SECOND;
			unsigned int nLastDay = m_llLastServerTime / DAY_SECOND;
			if (nNowDay == nLastDay)
			{
				return 3;
			}
			else
			{
				return 2;
			}

		}
		
	}
	else
	{
		return 3;
	}
	
}

/*
@ Author:	xlm(2015-2-5 18:54)
@ Parameter:
@ Returns:
@ Description:
*/
bool VuGameManager::showImmediateGetButtonTip()
{
	return m_bShowImmediateButtonTip;
}
/*
@ Author:	xlm(2015-2-5 18:56)
@ Parameter:
@ Returns:
@ Description:
*/
void VuGameManager::setShowImmediateGetButtonTip(bool bShow)
{
	m_bShowImmediateButtonTip = bShow;

	save();
}


/*
@ Author:	xlm(2015-2-5 18:07)
@ Parameter:
@ Returns:
@ Description: 获取当前的服务器时间
*/
long long VuGameManager::getCurrentServerTime()
{
	return m_llCurrServerTime;
}

/*
@ Author:	xlm(2015-2-5 18:09)
@ Parameter:
@ Returns:
@ Description: //会主动去服务器获取时间
*/
void VuGameManager::refreshServerTime()
{
	char szMsgData[64] = { 0 };
	char* pMsgBody = szMsgData + gnNetMsgHeaderSize;

	STNetMsgHeader* pHeader = (STNetMsgHeader*)szMsgData;
	pHeader->wMsgID = htons(MSG_G_GET_SERVER_TIME_REQ);
	pHeader->dwDataLen = htonl(0);
	//m_pTcpSocket->send(szMsgData, gnNetMsgHeaderSize);
	VuNetManager::IF()->send(szMsgData, gnNetMsgHeaderSize);
}

/*
@ Author:	xlm(2015-2-5 18:10)
@ Parameter:
@ Returns:
@ Description: 获取上一次的服务器时间
*/
long long VuGameManager::getLastServerTime()
{
	return m_llLastServerTime;
}

/*
@ Author:	xlm(2015-2-5 18:52)
@ Parameter:
@ Returns:
@ Description: 设置当前服务器时间
*/
void VuGameManager::setCurrentServerTime(long long llTime)
{
	m_llCurrServerTime = llTime;

	struct tm* pToday;
	pToday = localtime((const time_t*)&llTime);
	mWeekDay = pToday->tm_wday;
	
}

/*
@ Author:	xlm(2015-2-9 12:23)
@ Parameter:
@ Returns:
@ Description:
*/
int VuGameManager::getAvailableLoginDay()
{
	if (m_llCurrServerTime == 0)
		return 0;
	if (m_llLastServerTime == 0 && m_llCurrServerTime != 0)
		return 1;
	bool bSameDay = IsSameDay(m_llCurrServerTime, m_llLastServerTime);
	if (bSameDay)
	{
		return 0;
	}
	else
	{
		int nDay = m_nReceivedDays + 1;
		if (nDay > 15)
		{
			return 0;
		}
		else
			return nDay;
	}
}


/*
@ Author:	xlm(2015-4-16 9:58)
@ Parameter:
@ Returns:
@ Description: 返回值0-6，代表周日-周六  返回-1代表没有获取到当前是星期几
*/
int	VuGameManager::getWeekDay()
{
	return mWeekDay;
}

void VuGameManager::setPlayerNameAndUid(std::string playername, std::string uid)
{
	mPlayerName = playername;
	mUid = uid;
	save();
}

void VuGameManager::setTempPlayerName(std::string tempPlayerName)
{
	mTempPlayerName = tempPlayerName;
	save();
}

void VuGameManager::addGetNameTimes()
{
	mGetNameTimes--;
	if (mGetNameTimes < 0)
		mGetNameTimes = 0;
	save();
}

bool	VuGameManager::IsHavePlayerName()
{
	return (!mPlayerName.empty());
}

std::vector<VUBYTE*>	VuGameManager::getBroadcastImages()
{
	return mBroadcastImages; 
}
std::vector<VUBYTE*> VuGameManager::getBroadcastNameImages() 
{
	return mBroadcastNameImages; 
}

std::vector<int>	VuGameManager::getBroadcastImageSizes()
{
	return mBroadcastImageSizes;
}

std::vector<int>	VuGameManager::getBroadcastNameImageSizes()
{
	return mBroadcastNameImageSizes;
}

int				VuGameManager::getBroadcastSize() 
{
	return mBroadCastSize; 
}
int			VuGameManager::getBroadcastIndex()
{
	return mBroadCastVersion; 
}
bool				VuGameManager::getBroadcastDataOK() 
{ 
	return mIsBroadcastDataOK;
}