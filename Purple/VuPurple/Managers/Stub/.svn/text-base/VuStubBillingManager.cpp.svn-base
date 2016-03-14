//*****************************************************************************
//
//  Copyright (c) 2012-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Stub Billing manager
//
//*****************************************************************************

#include "VuPurple/Managers/VuBillingManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Managers/VuMessageBoxManager.h"


#ifdef VURETAIL

	class VuStubBillingManager : public VuBillingManager
	{
	public:
		virtual void		startPurchaseInternal(const std::string &itemName) { onPurchaseResult(itemName, "RESULT_BILLING_UNAVAILABLE"); }
	};

#else // VURETAIL

	class VuStubBillingManager : public VuBillingManager, VuMessageBox::Callback
	{
	public:
		virtual void startPurchaseInternal(const std::string &itemName)
		{
			mItemName = itemName;
			VuMessageBoxParams messageBoxParams;
			messageBoxParams.mType = "SimpleAB";
			messageBoxParams.mHeading = "IAP Test";
			messageBoxParams.mBody = "'Purchase' simulates a successful IAP.";
			messageBoxParams.mTextA = "Purchase";
			messageBoxParams.mTextB = "Cancel";
			messageBoxParams.mPauseGame = true;

			VuMessageBox *pMB = VuMessageBoxManager::IF()->create(messageBoxParams);
			pMB->setCallback(this);
			pMB->removeRef();
		}

		virtual std::string	 getDeviceId()
		{
			return "test";
		}
		
		virtual std::string	 getTransactionId()
		{
			return "test";
		}

		virtual std::string	getSDKType()
		{
			return "test";
		}

		virtual void genOutTradeNo()
		{

		}

		virtual void onMessageBoxClosed(VuMessageBox *pMessageBox)
		{
			if ( pMessageBox->getResult() == "OnA" )
				onPurchaseResult(mItemName, "RESULT_OK");
			else
				onPurchaseResult(mItemName, "RESULT_USER_CANCELED");
		}

		std::string	mItemName;
	};

#endif // VURETAIL


// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuBillingManager, VuStubBillingManager);
