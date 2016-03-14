//*****************************************************************************
//
//  Copyright (c) 2008-2008 Ralf Knoesel
//  Copyright (c) 2008-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  AssetGameMode class
// 
//*****************************************************************************

#pragma once

#include "VuGameMode.h"


class VuAssetGameMode : public VuGameMode
{
	DECLARE_RTTI

public:
	VuAssetGameMode() : mVersion(-1), mTotalAssetCount(0), mCurAssetCount(0), mCurAssetType(0), mCurAssetName(0), mStartTime(0) {}

	virtual bool		enter(const std::string &prevMode);
	virtual void		exit();

protected:
	bool		enumNextAsset(std::string &strType, std::string &strName);
	bool		loadAssetNames();

	typedef std::vector<std::string> AssetNames;
	typedef std::pair<std::string, AssetNames> AssetType;
	typedef std::list<AssetType> AssetTypes;

	static bool	assetTypeComp(const AssetType &assetType0, const AssetType &assetType1);

	std::string		mSku;
	int				mVersion;
	int				mTotalAssetCount;
	int				mCurAssetCount;
	int				mCurAssetType;
	int				mCurAssetName;
	double			mStartTime;
	AssetTypes		mAssetTypes;
};
