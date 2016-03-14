//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Stub Advertising manager
//
//*****************************************************************************

#include "VuEngine/Managers/VuMessageBoxManager.h"
#include "VuEngine/Dev/VuDevConfig.h"
#include "VuPurple/Managers/VuAdManager.h"


#if VU_DISABLE_TEST_ADS

class VuStubAdManager : public VuAdManager
{
protected:
	virtual bool	isAdReadyInternal(int adUnit) { return false; }
	virtual void	showAdInternal(int adUnit) {}
	virtual void	checkForAdsInternal() {}
};

// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuAdManager, VuStubAdManager);

#else

class VuStubAdManager : public VuAdManager, VuMessageBox::Callback
{
protected:
	virtual bool	isAdReadyInternal(int adUnit) { return !VuDevConfig::IF()->getParam("DisableAds").asBool(); }
	virtual void	showAdInternal(int adUnit);
	virtual void	checkForAdsInternal() {}

	virtual void	onMessageBoxClosed(VuMessageBox *pMessageBox);
};


// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuAdManager, VuStubAdManager);


//*****************************************************************************
void VuStubAdManager::showAdInternal(int adUnit)
{
	VuMessageBoxParams messageBoxParams;

	if ( adUnit == VuAdManager::AD_UNIT_INTERSTITIAL )
	{
		messageBoxParams.mType = "SimpleA";
		messageBoxParams.mHeading = "Interstitial Ad";
		messageBoxParams.mBody = "Placeholder for an instersitital ad unit.";
		messageBoxParams.mTextA = "OK";
	}
	else if ( adUnit == VuAdManager::AD_UNIT_INCENTIVIZED )
	{
		messageBoxParams.mType = "SimpleAB";
		messageBoxParams.mHeading = "Incentivized Ad";
		messageBoxParams.mBody = "Placeholder for an incentivized ad unit.";
		messageBoxParams.mTextA = "Complete";
		messageBoxParams.mTextB = "Abort";
	}
	else if ( adUnit == VuAdManager::AD_UNIT_REWARD )
	{
		messageBoxParams.mType = "SimpleA";
		messageBoxParams.mHeading = "Reward Ad";
		messageBoxParams.mBody = "Placeholder for a reward ad unit.";
		messageBoxParams.mTextA = "OK";
	}

	VuMessageBox *pMB = VuMessageBoxManager::IF()->create(messageBoxParams);
	pMB->setCallback(this);
	pMB->removeRef();
}

//*****************************************************************************
void VuStubAdManager::onMessageBoxClosed(VuMessageBox *pMessageBox)
{
	bool completed = (pMessageBox->getResult() == "OnA");

	onAdFinished(completed);
}

#endif // VU_DISABLE_TEST_ADS
