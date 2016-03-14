//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  GameManager class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/VuSystemComponent.h"
#include "VuEngine/Events/VuEventMap.h"
#include "VuEngine/Json/VuJsonContainer.h"
#include "VuEngine/Util/VuColor.h"

class VuPowerUp;
class VuPowerUpGroup;


class VuGameManager : public VuSystemComponent
{
	DECLARE_SYSTEM_COMPONENT(VuGameManager)
	DECLARE_EVENT_MAP

protected:
	// called by game
	friend class VuPurple;
	virtual bool init();
	virtual void postInit() { load(); }
	virtual void release();

public:
	VuGameManager();


	time_t				getGameStartTime();

	void				tickDecision(float fdt);

	// currency
	enum eCurrencyType { CT_STANDARD, CT_PREMIUM, CURRENCY_TYPE_COUNT };
	void				addCurrencyEarned(eCurrencyType type, int amount)    { mCurrencies[type].mAmountEarned += amount; }
	void				addCurrencyPurchased(eCurrencyType type, int amount) { mCurrencies[type].mAmountPurchased += amount; }
	int					getCurrency(eCurrencyType type)          { return getCurrencyEarned(type) + getCurrencyPurchased(type) - getCurrencySpent(type); }
	int					getCurrencyEarned(eCurrencyType type)    { return mCurrencies[type].mAmountEarned; }
	int					getCurrencyPurchased(eCurrencyType type) { return mCurrencies[type].mAmountPurchased; }
	int					getCurrencySpent(eCurrencyType type)     { return mCurrencies[type].mAmountSpent; }
	bool				canAfford(int sc, int pc) { return getCurrency(CT_STANDARD) >= sc && getCurrency(CT_PREMIUM) >= pc; }
	bool				spendCurrency(int sc, int pc);


	// cars
	struct Car
	{
		Car() : mIsFree(false), mIsOwned(false), mStage(0), mAccel(0), mSpeed(0), mHandling(0), mTough(0), mIsGold(false) {}

		void		load(const VuJsonContainer &data);
		void		save(VuJsonContainer &data) const;

		int			getStat(const char *statName) const;
		void		setStat(const char *statName, int value);

		int			getMaxLevel() const;
		bool		areStatsMaxed() const;
		float		getStageProgress() const;
		int		getStage() const;

		std::string	mName;
		bool		mIsFree;
		bool		mIsOwned;
		int			mStage;
		std::string	mDecal;
		std::string	mDecalColor;
		std::string	mPaintColor;
		int			mAccel;
		int			mSpeed;
		int			mHandling;
		int			mTough;
		bool		mIsGold;
		//tony.hu
		int         skycn_minStage;
		int         skycn_maxStage;
	};
	typedef std::map<std::string, Car> Cars;
	const std::string	&getCurCarName() { return mCurCarName; }
	const Car			&getCurCar() {
		return mCars[mCurCarName];
	}
	const Car			&getCar(const std::string &carName) { return mCars[carName]; }
	const Cars			&getCars() { return mCars; }
	void				setCurCar(const std::string &carName);
	bool				purchaseCar(const std::string &carName);
	bool				purchaseUpgradedCar(const std::string &carName);
	bool				purchaseCarUpgrade(const std::string &statName);
	bool				purchaseCarLevelUp(const std::string &carName, int newStage);
	bool				giveCarStage();
	bool				giveCar(const std::string &carName);
	bool				giveFullLvlCar(const std::string &carName);  //全部升级满之后的车；
	bool				giveCarUpgrade(const std::string &statName);
	void				paintCar(const std::string &decal, const std::string &decalColor, const std::string &paintColor);


	struct FullLvlCar
	{
		FullLvlCar() :isOwned(false){}
		std::string mCarName;
		std::string mCarTitle;
		std::string mCarSubTitle;
		int  mCurrentPrice;
		int  mDiscountInfo;
		int  mDiscountPrice;
		bool isOwned;
	};
	typedef std::map<std::string, FullLvlCar> FullLvlCars;

	const FullLvlCars & getFullLvlCars(){ return mFullLvlCar; }
	const std::vector<std::string> &getFullLvlCarNames(){ return mFullLvlCarNames; }


	// modify by zc,[/2015/6/30]
	unsigned int mEnterMainMenuTimes;
	unsigned int getEnterMainMenuTimes()const { return mEnterMainMenuTimes; }
	void addEnterMainMenuTimes() { mEnterMainMenuTimes++; }

	// modify by xlm,[/2014/12/19 ] 
	bool				isCarOwned(const std::string& carName);

	/*
	@ Author:	xlm(2015-4-16 9:58)
	@ Parameter:
	@ Returns:
	@ Description: 返回值0-6，代表周日-周六  返回-1代表没有获取到当前是星期几
	*/
	int					getWeekDay();
	/*
	@ Author:	xlm(2015-2-5 18:58)
	@ Parameter:
	@ Returns:
	@ Description:获取当前已经领取了几天的登录奖励
	*/
	unsigned int getCurrReceivedDays();
	/*
	@ Author:	xlm(2015-2-5 19:01)
	@ Parameter:
	@ Returns:
	@ Description:设置当前已经领取的天数
	*/
	void setReceivedDays(unsigned int nDays);
	/*
	@ Author:	xlm(2015-2-5 19:03)
	@ Parameter:
	@ Returns:
	@ Description:
	*/
	void addReceivedDays();

	/*
	@ Author:	xlm(2015-2-6 10:10)
	@ Parameter:
	@ Returns:
	@ Description:
	*/
	int getCurrDayState(int nDay);


	/*
	@ Author:	xlm(2015-2-5 18:54)
	@ Parameter:
	@ Returns:
	@ Description:
	*/
	bool showImmediateGetButtonTip();
	/*
	@ Author:	xlm(2015-2-5 18:56)
	@ Parameter:
	@ Returns:
	@ Description:
	*/
	void setShowImmediateGetButtonTip(bool bShow);

	/*
	@ Author:	xlm(2015-2-5 18:07)
	@ Parameter:
	@ Returns:
	@ Description: 获取当前的服务器时间
	*/
	long long getCurrentServerTime();

	/*
	@ Author:	xlm(2015-2-5 18:09)
	@ Parameter:
	@ Returns:
	@ Description: //会主动去服务器获取时间
	*/
	void refreshServerTime();

	/*
	@ Author:	xlm(2015-2-5 18:10)
	@ Parameter:
	@ Returns:
	@ Description: 获取上一次的服务器时间
	*/
	long long getLastServerTime();

	/*
	@ Author:	xlm(2015-2-5 18:52)
	@ Parameter:
	@ Returns:
	@ Description: 设置当前服务器时间
	*/
	void setCurrentServerTime(long long llTime);

	/*
	@ Author:	xlm(2015-2-9 12:23)
	@ Parameter:
	@ Returns:
	@ Description:
	*/
	int getAvailableLoginDay();

	/*
	@ Author:	zc(2015-6-1 12:23)
	@ Parameter:
	@ Returns:
	@ Description:
	*/
	void setPlayerNameAndUid(std::string playername, std::string uid);

	void addGetNameTimes();

	void setTempPlayerName(std::string tempPlayerName);

	/*
	@ Author:	zc(2015-7-2 12:23)
	@ Parameter:
	@ Returns:
	@ Description:
	*/

	void saveInterface()
	{
		save();
	}
	void setBroadCastVersion(int version)
	{
		mBroadCastVersion = version;
	}
	void setBroadCastSize(int size)
	{
		mBroadCastSize = size;
	}
	void setBroadcastImages(std::vector<VUBYTE*> bcImages)
	{
		mBroadcastImages.clear();
		for (int i = 0; i < bcImages.size(); i++)
		{
			mBroadcastImages.push_back(bcImages[i]);
		}
	}
	void setBroadcastNameImages(std::vector<VUBYTE*> bcNameImages)
	{
		mBroadcastNameImages.clear();
		for (int i = 0; i < bcNameImages.size(); i++)
		{
			mBroadcastNameImages.push_back(bcNameImages[i]);
		}
	}

	void setBroadcastImageSizes(std::vector<int> broadcastImageSizes)
	{
		mBroadcastImageSizes.clear();
		mBroadcastImageSizes.resize(broadcastImageSizes.size());
		for (int i = 0; i < broadcastImageSizes.size(); i++)
		{
			mBroadcastImageSizes[i] = broadcastImageSizes[i];
		}
	}

	void setBroadcastNameImageSizes(std::vector<int> broadcastNameImageSizes)
	{
		mBroadcastNameImageSizes.clear();
		mBroadcastNameImageSizes.resize(broadcastNameImageSizes.size());
		for (int i = 0; i < broadcastNameImageSizes.size(); i++)
		{
			mBroadcastNameImageSizes[i] = broadcastNameImageSizes[i];
		}
	}

	void setIsBroadcastDataOk(bool isok)
	{
		mIsBroadcastDataOK = isok;
	}

	std::vector<VUBYTE*>	getBroadcastImages();
	std::vector<VUBYTE*> getBroadcastNameImages();
	int				getBroadcastSize();
	int				getBroadcastIndex();
	bool				getBroadcastDataOK();
	std::vector<int>	getBroadcastImageSizes();
	std::vector<int>	getBroadcastNameImageSizes();
	// drivers
	struct Driver
	{
		Driver() : mIsDefault(false), mIsBeaten(false), mNumBossLosses(0), mIsInteresting(false), mIsPurchased(false) {}

		void		load(const VuJsonContainer &data);
		void		save(VuJsonContainer &data) const;

		bool		isOnTeam() const;

		std::string	mName;
		bool		mIsDefault;
		bool		mIsBeaten;
		bool		mIsPurchased;
		int			mNumBossLosses;
		bool		mIsInteresting; // not saved
	};
	typedef std::map<std::string, Driver> Drivers;
	const std::string	&getCurDriverName() { return mCurDriverName; }
	const Driver		&getCurDriver() { return mDrivers[mCurDriverName]; }
	const Driver		&getDriver(const std::string &driverName) { return mDrivers[driverName]; }
	const Drivers		&getDrivers() { return mDrivers; }
	void				setCurDriver(const std::string &driverName);
	void				setDriverBeaten(const std::string &driverName);
	bool				getDriverBeaten(const std::string &driverName);
	bool				purchaseDriver(const std::string &driverName);
	bool				purchaseDriverFromPack(const std::string &driverName); //从礼包渠道购买车手。
	void				clearDriverInteresting(const std::string &driverName);
	bool				isDriverInteresting(const std::string &driverName);
	void    setDriverBeatenInDuelEvent(const std::string &driverName);

	// For keeping track of how many losses to a given boss
	void				lostToBoss(const std::string& bossName);
	int					numBossLosses(const std::string& bossName);

	// offers
	bool				hasUsedOffer(const std::string &offerName) { return mUsedOffers.find(offerName) != mUsedOffers.end(); }
	void				useOffer(const std::string &offerName) { mUsedOffers.insert(offerName); }

	// power-ups
	typedef std::set<std::string> OwnedPowerUps;
	const OwnedPowerUps	&getOwnedPowerUps() { return mOwnedPowerUps; }
	bool				isPowerUpOwned(const std::string &powerUpName) { return mOwnedPowerUps.find(powerUpName) != mOwnedPowerUps.end(); }
	bool				purchasePowerUp(const std::string &powerUpName);
	bool				givePowerUp(const std::string &powerUpName);

	// tune-ups
	int					getTuneUps() { return mTuneUps; }
	bool				useTuneUp();
	void				addTuneUps(int count) { mTuneUps += count; }
	bool				purchaseTuneUps(int count, int pricePC);

	// tokens
	int					getTokens();
	bool				useToken();
	void				addToken();
	void				refillTokens();
	bool				purchaseTokenRefill();

	bool				PurchaseTokenRefill(int nCount, int nPrice);

	int					getTokenTimer() { return mTokenTimer; }

	// modify by xlm,[/2014/12/8 ] 添加token，忽略上限
	void				addTokenNoLimit(int nCount);

	// calendars
	enum eCalendars { DAILY_CHALLENGE_CALENDAR, DAILY_REWARD_CALENDAR, CALENDAR_COUNT };

	void				setCalendarDay(eCalendars calendar, int daysSince2000); /*{ mCalendars[calendar].setDay(daysSince2000); }*/
	bool				getCalendarDay(eCalendars calendar, int daysSince2000);/* { return mCalendars[calendar].getDay(daysSince2000); }*/
	int					getCalendarDaysInRow(eCalendars calendar, int daysSince2000); /*{ return mCalendars[calendar].getDaysInRow(daysSince2000); }*/

	// unlocked stage
	int					getHighestCarStage();
	int					getUnlockedStage() { return mUnlockedStage; }
	void				unlockStage(int stage) { mUnlockedStage = VuMax(mUnlockedStage, stage); }

	// iap made
	bool				getIapMade() { return mIapMade; }
	void				setIapMade() { mIapMade = true; }

	// purchase info
	int					numCarsBought() { return mNumCarsBought; }
	int					numUpgradesBought() { return mNumUpgradesBought; }
	int					numDriversBought() { return mNumDriversBought; }
	int					numPowerUpsBought() { return mNumPowerUpsBought; }
	int					numTuneUpsBought() { return mNumTuneUpsBought; }

	// specials
	struct Special
	{
		VUINT64		mExpirationTime; // seconds since 2000
		std::string	mStoreItemName;
		bool		mOneShot;
	};
	typedef std::map<std::string, Special> Specials;
	void				beginSpecial(const std::string &name, float duration);
	const Specials		&activeSpecials() { return mActiveSpecials; }

	// misc
	bool				isFlaggedAsPirate() { return mFlaggedAsPirate; }
	void				setFlaggedAsPirate() { mFlaggedAsPirate = true; }

	bool				isConfirmedPirate();

private:
	// event handlers
	void				OnSaveProfile(const VuParams &params);
	void				OnCloudDataPostSync(const VuParams &params);
	void				OnPurchaseMade(const VuParams &params);

	void				setDefaults();

	void				load();
	void				save();

	void __AddActiveSpecials(std::string strKey);


	struct Currency
	{
		void	load(const VuJsonContainer &data);
		void	save(VuJsonContainer &data) const;
		void	setDefaults();

		int		mAmountEarned;
		int		mAmountPurchased;
		int		mAmountSpent;
	};
	Currency		mCurrencies[CURRENCY_TYPE_COUNT];

	Cars			mCars;
	std::string		mCurCarName;

	FullLvlCars mFullLvlCar;
	std::vector<std::string> mFullLvlCarNames;

	Drivers			mDrivers;
	std::string		mCurDriverName;

	typedef std::set<std::string> Offers;
	Offers			mUsedOffers;

	OwnedPowerUps	mOwnedPowerUps;

	int				mTuneUps;
	int				mTokens;
	VUINT64			mTokenTimerStart; // seconds since 2000
	bool			mTokensEverExhausted;



	class Calendar
	{
	public:
		void			setDay(int daysSince2000);
		bool			getDay(int daysSince2000);
		int				getDaysInRow(int daysSince2000);

		void			load(const VuJsonContainer &data);
		void			save(VuJsonContainer &data);
		void			clear() { mDays.clear(); }

		std::set<int>	mDays;
	};
	Calendar		mCalendars[CALENDAR_COUNT];

	int				mUnlockedStage;
	bool			mIapMade;

	Specials		mActiveSpecials;

	// purchase info
	int				mNumCarsBought;
	int				mNumUpgradesBought;
	int				mNumDriversBought;
	int				mNumPowerUpsBought;
	int				mNumTuneUpsBought;

	// misc
	bool			mFlaggedAsPirate;

	float			mUpdateTimer;
	int				mTokenTimer;

	// modify by xlm,[/2015/2/5 ] 
	int	m_nReceivedDays;
	bool			m_bShowImmediateButtonTip;

	VUINT64 m_llCurrServerTime;   //当前服务器时间
	VUINT64 m_llLastServerTime;  //上一次的服务器时间

	int				mWeekDay;

	time_t			mGameStartTime;

	VUINT64	m_llDailyRewardLastTime;
	//modify by zc[2015/9/10]
	VUINT64 m_lastCarChampChallengeTime;
	
// modify by zc[/2015/5/1]
	std::string		mUid;
	std::string		mPlayerName;
	std::string		mTempPlayerName;
	int	mGetNameTimes;
// modify by zc[/2015/7/2]
	int				mBroadCastVersion;//公告index，用来判断是否更新
	int				mBroadCastSize;//公告数量
	std::vector<VUBYTE*> mBroadcastImages;
	std::vector<int>	mBroadcastImageSizes;
	std::vector<VUBYTE*>	mBroadcastNameImages;
	std::vector<int>	mBroadcastNameImageSizes;
	bool				mIsBroadcastDataOK;
	bool				mIs1_8VersionFirstLogin;

    //modify by liudan   2015/7/20  19:53
	std::string mPurchase_FullLvlCarName; //正在购买的满级车辆名称

	std::string mGameVersion;
	bool bVerNeedChange;
public:
	// modify by zc[/2015/7/2]
	bool		IsHavePlayerName();
	std::string getPlayerName()const{ return mPlayerName; }
	std::string getUid()const{ return mUid; }
	int getNameTimes() const{ return mGetNameTimes; }
	std::string getTempPlayerName() const { return mTempPlayerName; }

	bool			getIs1_8VersionFirstLogin() const { return mIs1_8VersionFirstLogin; }
	void			set1_8VersionFirstLogin() { mIs1_8VersionFirstLogin = false; }
	//modify by zc[2015/8/9]
	struct Order{
		Order(std::string orderId, std::string out_trade_no ,std::string itemName, int itemNum)
		{
			m_orderId = orderId;
			m_itemName = itemName;
			m_itemNum = itemNum;
			m_out_trade_no = out_trade_no;
		}
		std::string m_orderId;
		std::string m_out_trade_no;
		std::string m_itemName;
		int m_itemNum;
	};


	std::vector<Order> m_Orders;
	const std::vector<Order> &getOrders(){ return m_Orders; }
	void addNeedProcessedOrder(std::string orderId,std::string out_trade_no,std::string itemName)
	{
		int size = m_Orders.size();
		m_Orders.push_back(Order(orderId, out_trade_no,itemName, 0));
	}
	int getNeedProcessedOrderNum() { return m_Orders.size(); }

	bool isExistOrder(std::string orderId)
	{
		for (std::vector<Order>::iterator iter = m_Orders.begin(); iter != m_Orders.end(); iter++)
		{
			if ((*iter).m_orderId == orderId)
			{
				return true;
			}
		}
		return false;
	}

	void addSpecialOrderNum(std::string orderId)
	{
		for (std::vector<Order>::iterator iter = m_Orders.begin(); iter != m_Orders.end(); iter++)
		{
			if ((*iter).m_orderId == orderId)
			{
				(*iter).m_itemNum++;
				if ((*iter).m_itemNum == 5)
					m_Orders.erase(iter);
				return;
			}
			
		}
	}

	void deleteProcessedOrder(std::string orderId)
	{
		for (std::vector<Order>::iterator iter = m_Orders.begin(); iter != m_Orders.end(); iter++)
		{
			if ((*iter).m_orderId == orderId)
			{
				m_Orders.erase(iter);
				return;
			}
		}
	}

	//modify by zc[2015/9/10]
	void setLastCarChampChallengeTime(VUINT64 lastCarChampChallengeTime)
	{
		m_lastCarChampChallengeTime = lastCarChampChallengeTime;
	}
	VUINT64 getLastCarChampChallengeTime() const
	{
		return m_lastCarChampChallengeTime;
	}
    //modify by liudan   2015/7/20  19:53
	bool			mbroadcastClicked;
	void			clickBroadcast() { mbroadcastClicked = true; }
	bool			isClickBroadcast(){ return mbroadcastClicked; }
	void setPurchase_FullLvlCarName(std::string str){
		mPurchase_FullLvlCarName = str;
	}
	std::string getPurchaseFullLvlCarName()
	{
		return mPurchase_FullLvlCarName;
	}

	//modify by zc[2015/11/3]
	std::map<std::string ,int> m_GiftDialogStates;
	std::map<std::string, int> m_GiftDialogButtonStates;
	int m_GiftDialogVersion;
	
	int getGiftDialogVersion() const { return m_GiftDialogVersion; }
	std::map<std::string, int> getGiftDialogButtonStates() const { return m_GiftDialogButtonStates; }
	std::map<std::string, int> getGiftDialogStates() const { return m_GiftDialogStates; }
	void setGiftDialogStates(std::map<std::string, int> giftDialogStates,std::map<std::string,int> giftDialogButtonStates, int version) 
	{
		m_GiftDialogStates = giftDialogStates;
		m_GiftDialogButtonStates = giftDialogButtonStates;
		m_GiftDialogVersion = version; 
	}
};
