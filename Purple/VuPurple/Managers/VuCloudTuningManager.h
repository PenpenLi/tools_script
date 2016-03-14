//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuCloudTuningManager
// 
//*****************************************************************************

#pragma once

#include "VuEngine/VuSystemComponent.h"
#include "VuEngine/Json/VuJsonContainer.h"

class VuDBAsset;


class VuCloudTuningManager : public VuSystemComponent
{
	DECLARE_SYSTEM_COMPONENT(VuCloudTuningManager)

public:
	VuCloudTuningManager();

	virtual bool			init();
	virtual void			release();

	const VuJsonContainer	&tuningDB() { return mTuningDB; }

	// convenience variables
	struct Variables
	{
		int		mMaxTokens;
		int		mTokenRefillPrice;
		int		mTokenRefillTime;
	};
	const Variables			&variables() { return mVariables; }

protected:
	virtual bool			wasGameConfigurationReceived() = 0;
	virtual void			getGameConfigurationValue(const std::string &key, std::string &value) = 0;

	void					tickNetwork(float fdt);

	void					setVariables();

	VuDBAsset				*mpTuningDB;
	VuJsonContainer			mTuningDB;
	float					mTimeOut;
	Variables				mVariables;
};
