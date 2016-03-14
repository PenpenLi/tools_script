//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuCarSpawner
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Json/VuJsonContainer.h"

class VuAsset;
class VuCarEntity;
class VuVector3;
class VuTrackSector;


class VuCarSpawner
{
public:
	VuCarSpawner();
	~VuCarSpawner();

	struct LoadParams
	{
		std::string	mPlayerCar;
		std::string	mPlayerDriver;
		std::string	mPlayerDecal;
		std::string	mAiPersonality;
		std::string	mAiBrain;
	};

	void	configure(const VuJsonContainer &data);
	void	load(const std::string &playerCar, const std::string &playerDriver, const std::string &playerDecal);
	void	begin();
	void	end();
	void	update(float fdt);

private:
	typedef std::vector<std::string> Names;
	typedef std::vector<VuAsset *> Assets;
	typedef std::vector<VuCarEntity *> Cars;

	static void		createNames(const Names &srcNames, Names &dstNames, const std::string &ignoreName, int count);
	static VUUINT32	calcPointAlongRacingLine(VuCarEntity *pCar, float distance, VuVector3 &pos, const VuTrackSector *&pSector);
	VuCarEntity		*createCar();

	// constant data
	int				mMaxCarCount;
	float			mFrontSpawnDistance;
	float			mBackKillDistance;
	int				mCarVarietyCount;
	int				mDriverVarietyCount;

	float			mOptimalSpacing;
	float			mFrontKillDistance;
	float			mTrackLength;

	int				mSpawnCount;
	float			mSpawnTimer;
	VuJsonContainer	mCarData;

	Names			mCarNames;
	Names			mDriverNames;
	Names			mColorNames;
	Assets			mCarAssets;
	Cars			mPreloadedCars;
	Cars			mActiveCars;
};