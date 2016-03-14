//*****************************************************************************
//
//  Copyright (c) 2012-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  BillingManager class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/VuSystemComponent.h"
#include "VuEngine/Managers/VuMessageBoxManager.h"
#include "VuPurple/Managers/VuRequestOrderManager.h"


class FirstChargeProcessor :public VuMessageBox::Callback
{
public:
	virtual void onMessageBoxClosed(VuMessageBox *pMessageBox);

};

class Callback { public: virtual void onMessageBoxClosed(VuMessageBox *pMessageBox) = 0; };


class VuBillingItemProcessor :public VuMessageBox::Callback
{
public:
	virtual void onMessageBoxClosed(VuMessageBox *pMessageBox);
	void setItemName(std::string itemName) { m_itemName = itemName; }
protected:
	std::string m_itemName;
};




class VuBillingManager : public VuSystemComponent, public RequestOrderDelegate
{
	DECLARE_SYSTEM_COMPONENT(VuBillingManager)
public:
	enum BILLING_TYPE
	{
		LOCAL_MESSAGE = 0,
		WEIXIN
	};

protected:
	// called by game
	friend class VuPurple;
	virtual bool init();
	virtual void postInit();

public:
	VuBillingManager();

	virtual ~VuBillingManager();

	void				startPurchase(const std::string &itemName);
	void				startPurchaseInternal(const std::string& itemName,float needMoney);
	//uc
	//bool				isBusy() { return mBusy; }
	bool				isBusy() { return false; }
	
	// pricing
	void				setPrice(const char *itemName, const char *price);
	const std::string	&getPrice(const std::string &itemName) { return mPrices[itemName]; }

	// managed items
	bool				isOwned(const char *itemName);
	void				setOwned(const char *itemName);
	virtual void		restoreManagedItems() {}
	// unlocked stuff
	bool				isUnlocked(const char *theThing);

	// gifts
	void				receiveGift(const char *itemName) { processItem(itemName); }
	
	// analytics
	void				logPurchaseAnalyticsEvent(const char *itemName);

	virtual int					getCurSimType() { return 0; }

	virtual void			moreGame() {}

	virtual void			telecomExitGame() {}

	//virtual void			CheckGameBaseSetting() {}

	//zouchao 2015/7/24
	void prePurchase(std::string& itemName );
	void	setBillingType(BILLING_TYPE type);

	BILLING_TYPE getCurrentBillingType() const;

	void setCurrentClickItem(std::string itemName);

	std::string getCurrentClickItem() const { return m_currentClickItem; }
	
	bool isCurrentBigPayment();

	virtual void onGetOrderId(std::string strOrderId, std::string strOutTradeNo,std::string purchaseItem);
	virtual void onSuccessVerification(std::string itemName);
	virtual std::string	 getDeviceId() = 0;
	virtual std::string	 getTransactionId() = 0;
	virtual std::string	 getSDKType() = 0;
	virtual void genOutTradeNo() = 0;
protected:
	virtual void		startPurchaseInternal(const std::string &itemName) = 0;

	void				onPurchaseResult(const std::string &itemName, const std::string &result);

	void				processItem(const char *itemName);

	bool				mBusy;

	typedef std::map<std::string, std::string> Prices;
	Prices				mPrices;
	//zouchao 2015/7/24
	BILLING_TYPE m_billingType;
	std::string		m_currentClickItem;
	FirstChargeProcessor*		mFirstChargeProcessor;
	VuBillingItemProcessor*	mBillingItemProcessor;
};
