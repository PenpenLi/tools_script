//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Advertising Manager
// 
//*****************************************************************************

#pragma once

#include "VuEngine/VuSystemComponent.h"
#include "VuEngine/Entities/VuEntity.h"


class VuAdManager : public VuSystemComponent
{
	DECLARE_SYSTEM_COMPONENT(VuAdManager)

protected:
	// called by game
	friend class VuPurple;
	virtual bool init();
	virtual void release();

public:
	VuAdManager();

	enum eAdUnit { AD_UNIT_INTERSTITIAL, AD_UNIT_INCENTIVIZED, AD_UNIT_REWARD };

	bool		isAdReady(int adUnit);
	bool		showAd(int adUnit, VuEntity *pEntity);
	void		checkForAds();

protected:
	void			tick(float fdt);

	virtual bool	isAdReadyInternal(int adUnit) = 0;
	virtual void	showAdInternal(int adUnit) = 0;
	virtual void	checkForAdsInternal() = 0;

	void			onAdFinished(bool completed);

	bool				mBusy;
	VuWeakRef<VuEntity>	mEntity;
	float				mCheckTimer;
};
