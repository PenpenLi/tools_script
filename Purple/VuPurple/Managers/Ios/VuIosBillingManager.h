//*****************************************************************************
//
//  Copyright (c) 2012-2014 Vector Unit Inc
//  Confidential Trade Secrets
//
//  Ios Billing manager
//
//*****************************************************************************

#include "VuPurple/Managers/VuBillingManager.h"


class VuIosBillingManager : public VuBillingManager
{
public:
	VuIosBillingManager();
	
	virtual bool		init();
	virtual void		release();
	
	// platform-specific functionality
	static VuIosBillingManager *IF() { return static_cast<VuIosBillingManager *>(VuBillingManager::IF()); }
	
	void				setProductInfo(const char *id, const char *price);
	void				verifyPurchase(const char *id, const char *receipt);
	void				onIosPurchaseResult(const char *id, const char *result);
	
	void				registerFuseTransaction(const char *transactionId, const char *id);
	void				purchaseVerifiedByFuse(const char *transactionId);
	
	void				onRestoreCompleted(bool success);
	
protected:
	void				tick(float fdt);
	
	virtual void		restoreManagedItems();
	virtual void		startPurchaseInternal(const std::string &itemName);
	
	std::string			mVerifyProductName;
	VUUINT32			mVerifyKey;
	VUHANDLE			mVerifyHttpRequest;
	
	typedef std::map<std::string, std::string> FuseTransactions;
	FuseTransactions	mFuseTransactions;
};
