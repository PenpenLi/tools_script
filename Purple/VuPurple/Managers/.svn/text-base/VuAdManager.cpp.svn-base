//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Advertising Manager
// 
//*****************************************************************************

#include "VuAdManager.h"
#include "VuBillingManager.h"
#include "VuGameManager.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/HAL/Audio/VuAudio.h"


#define AD_INITIAL_DELAY (5) // seconds
#define AD_REFRESH_DELAY (1) // seconds
#define AD_CHECK_INTERVAL (60) // seconds


//*****************************************************************************
VuAdManager::VuAdManager():
	mBusy(false),
	mCheckTimer(AD_INITIAL_DELAY)
{
}

//*****************************************************************************
bool VuAdManager::init()
{
	VuTickManager::IF()->registerHandler(this, &VuAdManager::tick, "Decision");

	return true;
}

//*****************************************************************************
void VuAdManager::release()
{
	VuTickManager::IF()->unregisterHandlers(this);
}

//*****************************************************************************
bool VuAdManager::isAdReady(int adUnit)
{
	if ( !VuSys::IF()->hasTouch() )
		return false;

	if ( VuBillingManager::IF()->isUnlocked("DisableAds") && !VuGameManager::IF()->isConfirmedPirate() )
		return false;

	if ( mBusy )
		return false;

	return isAdReadyInternal(adUnit);
}

//*****************************************************************************
bool VuAdManager::showAd(int adUnit, VuEntity *pEntity)
{
	if ( !isAdReady(adUnit) )
		return false;

	VuAudio::IF()->pushMasterPause();
	VuTickManager::IF()->pushPauseRequest();

	mBusy = true;
	mEntity = pEntity;
	showAdInternal(adUnit);

	return true;
}

//*****************************************************************************
void VuAdManager::checkForAds()
{
	if ( VuSys::IF()->hasTouch() && !mBusy )
	{
		checkForAdsInternal();
		mCheckTimer = AD_CHECK_INTERVAL;
	}
}

//*****************************************************************************
void VuAdManager::onAdFinished(bool completed)
{
	if ( mBusy )
	{
		mBusy = false;

		VuAudio::IF()->popMasterPause();
		VuTickManager::IF()->popPauseRequest();

		if ( mEntity )
		{
			VuParams params;
			params.addBool(completed);
			mEntity.get()->mEventMap.handle("OnAdFinished", params);
		}

		mCheckTimer = AD_REFRESH_DELAY;
	}
}

//*****************************************************************************
void VuAdManager::tick(float fdt)
{
	fdt = VuTickManager::IF()->getRealDeltaTime();

	mCheckTimer -= fdt;
	if ( mCheckTimer <= 0 )
		checkForAds();
}
