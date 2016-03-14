//*****************************************************************************
//
//  Copyright (c) 2012-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Android Billing manager
//
//*****************************************************************************

#pragma once

#include <jni.h>
#include "VuPurple/Managers/VuBillingManager.h"
#include "VuEngine/Events/VuEventMap.h"


class VuAndroidBillingManager : public VuBillingManager
{
	DECLARE_EVENT_MAP

public:
	// this function MUST be called from the application's JNI_OnLoad,
	// from a function known to be called by JNI_OnLoad, or from a function
	// in a Java-called thread.
	static void		bindJavaMethods(JNIEnv *jniEnv, jobject classLoaderObject, jmethodID findClassMethod);

	VuAndroidBillingManager();

	virtual void	postInit();

	static bool		getItemName(const std::string &storeItemName, std::string &itemName);

	virtual int					getCurSimType();

	virtual void			moreGame();

	virtual void			telecomExitGame();

	//virtual void CheckGameBaseSetting();
	virtual std::string	 getDeviceId();
	virtual std::string	 getTransactionId();
	virtual std::string	 getSDKType();
	virtual void genOutTradeNo();
protected:
	// event handlers
	void			OnAndroidPurchaseResult(const VuParams &params);
	void			OnAddOwnedItem(const VuParams &params);
	void			OnAndroidSetItemPrice(const VuParams &params);
	void			OnAndroidLogPurchaseAnalyticsEvent(const VuParams &params);

	virtual void	startPurchaseInternal(const std::string &itemName);



	typedef std::set<std::string> OwnedItems;
	OwnedItems		mOwnedItems;
};

