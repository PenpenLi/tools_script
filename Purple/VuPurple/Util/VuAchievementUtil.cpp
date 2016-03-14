//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Utilities for achievements
// 
//*****************************************************************************

#include "VuAchievementUtil.h"
#include "VuPurple/Managers/VuAchievementManager.h"
#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Managers/VuStatsManager.h"
#include "VuPurple/PowerUps/VuPowerUpManager.h"
#include "VuPurple/Util/VuGameUtil.h"


namespace VuAchievementUtil
{
	void handleStarsEarnedInSeries(int id, const char *seriesName)
	{
		int earnedStars = VuGameUtil::IF()->calcEarnedSeriesStars(seriesName);
		int maxStars = VuGameUtil::IF()->calcMaxSeriesStars(seriesName);
		VuAchievementManager::IF()->setProgress(id, (float)earnedStars/(float)maxStars); // Earn all stars in a series
	}
}

//*****************************************************************************
void VuAchievementUtil::recalculateAchievements()
{
	// Earn all stars in a series
	handleStarsEarnedInSeries(1, "SeriesA");
	handleStarsEarnedInSeries(2, "SeriesB");
	handleStarsEarnedInSeries(3, "SeriesC");
	handleStarsEarnedInSeries(4, "SeriesD");
	handleStarsEarnedInSeries(5, "SeriesE");
	handleStarsEarnedInSeries(6, "SeriesF");
	handleStarsEarnedInSeries(7, "SeriesG");
	handleStarsEarnedInSeries(8, "SeriesH");

	// distance stats
	{
		float totalDistance = VuStatsManager::IF()->getFloatStat("TotalDistance");

		VuAchievementManager::IF()->setProgress(14, totalDistance/(100*1000)); // Drive 100 km
		VuAchievementManager::IF()->setProgress(15, totalDistance/(1000*1000)); // Drive 1000 km
		VuAchievementManager::IF()->setProgress(16, totalDistance/(9289*1000)); // Drive the length of the Trans-Siberian Railway (9,289km)
	}

	VuAchievementManager::IF()->setProgress(17, VuGameManager::IF()->getCurrencyEarned(VuGameManager::CT_STANDARD)/1000000.0f); // Earn over 1,000,000 coins in your career

	// smashed
	VuAchievementManager::IF()->setProgress(18, VuStatsManager::IF()->getIntStat("SmashedSeagulls")/100.0f);
	VuAchievementManager::IF()->setProgress(19, VuStatsManager::IF()->getIntStat("SmashedCrabs")/40.0f);
	VuAchievementManager::IF()->setProgress(20, VuStatsManager::IF()->getIntStat("SmashedLavaMonsters")/20.0f);
	VuAchievementManager::IF()->setProgress(21, VuStatsManager::IF()->getIntStat("SmashedPalms")/500.0f);
	VuAchievementManager::IF()->setProgress(22, VuStatsManager::IF()->getIntStat("SmashedDeathBats")/50.0f);
	VuAchievementManager::IF()->setProgress(23, VuStatsManager::IF()->getIntStat("SmashedYeti")/25.0f);
	VuAchievementManager::IF()->setProgress(24, VuStatsManager::IF()->getIntStat("SmashedPenguins")/100.0f);

	const VuGameManager::Cars &cars = VuGameManager::IF()->getCars();
	int numCarsPurchased = (int)std::count_if(cars.begin(), cars.end(), [](const std::pair<std::string, VuGameManager::Car> &iter) { return iter.second.mIsOwned && !iter.second.mIsFree; } );
	VuAchievementManager::IF()->setProgress(9, numCarsPurchased/5.0f); // buy 5 cars

	const VuGameManager::Drivers &drivers = VuGameManager::IF()->getDrivers();
	int numDriversRecruited = (int)std::count_if(drivers.begin(), drivers.end(), [](const std::pair<std::string, VuGameManager::Driver> &iter) { return iter.second.mIsBeaten && !iter.second.mIsDefault; } );
	VuAchievementManager::IF()->setProgress(10, numDriversRecruited/5.0f); // recruit 5 drivers

	int numPowerUpsUnlocked = 0;
	for ( int i = 0; i < VuPowerUpManager::IF()->getPowerUpCount(); i++ )
	{
		const VuPowerUp *pPowerUp = VuPowerUpManager::IF()->getPowerUp(i);
		if ( pPowerUp->mPrice > 0 && VuGameManager::IF()->isPowerUpOwned(pPowerUp->mName) )
			numPowerUpsUnlocked++;
	}
	VuAchievementManager::IF()->setProgress(11, numPowerUpsUnlocked/10.0f); // unlock 10 powerups
}
