//*****************************************************************************
//
//  Copyright (c) 2007-2008 Ralf Knoesel
//  Copyright (c) 2008-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  For tracking global use of powerups by AI
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Dynamics/Util/VuDynamicsRayTest.h"


class VuAiPowerUpTracker
{
public:
	VuAiPowerUpTracker();

	void init();
	void raceStart();
	void release();

	const char* getBestPowerUpByGroup(const char* pGroupName);
	
	void powerUpWasUsed(const char* pPowerUpName);

private:

	void buildFor(const char* pGroupName);

	struct TrackingItem
	{
		TrackingItem() : mpName(VUNULL) {}

		VUUINT32		mPowerUpHashName;
		VUUINT32		mUsageCount;
		const char*		mpName;
	};

	typedef std::vector<TrackingItem*> TrackingList;

	TrackingList mAllPowerUps;

	std::map<VUUINT32, TrackingList> mPowerUpMap;
};