//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  StatsManager class
// 
//*****************************************************************************

#include "VuStatsManager.h"
#include "VuAnalyticsManager.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/Managers/VuGameServicesManager.h"
#include "VuPurple/Managers/VuLeaderboardManager.h"
#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Assets/VuSpreadsheetAsset.h"
#include "VuEngine/Util/VuSpreadsheetQuery.h"
#include "VuEngine/Util/VuStringUtil.h"
#include "../Util/CommonUtility.h"


// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuStatsManager, VuStatsManager);



//****************************************************************************
void VuStatsManager::recordRaceResult(const std::string &eventName, VuCarEntity *pCar)
{
	recordTime(pCar, eventName);
	recordCommon(pCar, eventName);
}

//****************************************************************************
void VuStatsManager::recordBoostBlitzResult(const std::string &eventName, VuCarEntity *pCar)
{
	recordTime(pCar, eventName);
	recordCommon(pCar, eventName);
}

//****************************************************************************
void VuStatsManager::recordEliminationResult(const std::string &eventName, VuCarEntity *pCar)
{
	recordDistance(pCar, eventName);
	recordCommon(pCar, eventName);
}

//****************************************************************************
void VuStatsManager::recordShootingGalleryResult(const std::string &eventName, VuCarEntity *pCar)
{
	recordScore(pCar, eventName);
	recordCommon(pCar, eventName);
}

//****************************************************************************
void VuStatsManager::recordDemolitionDerbyResult(const std::string &eventName, VuCarEntity *pCar)
{
	recordScore(pCar, eventName);
	recordCommon(pCar, eventName);
}

//****************************************************************************
void VuStatsManager::recordFollowLeaderResult(const std::string &eventName, VuCarEntity *pCar)
{
	recordScore(pCar, eventName);
	recordCommon(pCar, eventName);
}

//****************************************************************************
void VuStatsManager::recordBossBattleResult(const std::string &eventName, VuCarEntity *pCar)
{
	recordCommon(pCar, eventName, true);
}
//****************************************************************************
void VuStatsManager::recordCarChampRaceResult(VuCarEntity *pCar)
{
	recordCommon(pCar, "", "CarChamp");
}

//*****************************************************************************
void VuStatsManager::addToFloatStat(const char *stat, float value)
{
	VuJsonContainer &data = statsWrite()[stat];
	data.putValue(data.asFloat() + value);
}

//*****************************************************************************
void VuStatsManager::addToIntStat(const char *stat, int value)
{
	VuJsonContainer &data = statsWrite()[stat];
	data.putValue(data.asInt() + value);
}

//*****************************************************************************
void VuStatsManager::recordMaxFloatStat(const char *stat, float value)
{
	VuJsonContainer &data = statsWrite()[stat];
	if ( value > data.asFloat() )
		data.putValue(value);
}

//*****************************************************************************
void VuStatsManager::recordMaxIntStat(const char *stat, int value)
{
	VuJsonContainer &data = statsWrite()[stat];
	if ( value > data.asInt() )
		data.putValue(value);
}

//*****************************************************************************
void VuStatsManager::recordMaxStringStat(const char *stat, const std::string &value)
{
	VuJsonContainer &data = statsWrite()[stat];
	if ( value > data.asString() )
		data.putValue(value);
}

//*****************************************************************************
int VuStatsManager::getBestScore(const char *gameType) const
{
	int bestScore = 0;

	const VuSpreadsheetAsset *pSA = VuGameUtil::IF()->eventSpreadsheet();
	int eventColumn = pSA->getColumnIndex("Event");

	VuSpreadsheetQuery::VuStringEqual expression("Type", gameType);
	int rowIndex = VuSpreadsheetQuery::findFirstRow(pSA, expression);
	while ( rowIndex < pSA->getRowCount() )
	{
		const char *eventName = pSA->getField(rowIndex, eventColumn).asCString();

		int score;
		if ( VuStatsManager::IF()->getBestScore(eventName, score) )
			bestScore = VuMax(bestScore, score);

		rowIndex = VuSpreadsheetQuery::findNextRow(pSA, expression, rowIndex);
	}

	return bestScore;
}

//*****************************************************************************
const std::string &VuStatsManager::getFavoriteCar()
{
	const VuJsonContainer &cars = VuGameUtil::IF()->constantDB()["Names"]["Cars"];

	int favoriteIndex = 0;
	float favoriteTime = 0;
	for ( int i = 0; i < cars.size(); i++ )
	{
		float time = statsRead()["Cars"][cars[i].asString()]["TotalTime"].asFloat();
		if ( time > favoriteTime )
		{
			favoriteIndex = i;
			favoriteTime = time;
		}
	}

	return cars[favoriteIndex].asString();
}

//*****************************************************************************
const std::string &VuStatsManager::getFavoriteDriver()
{
	const VuJsonContainer &drivers = VuGameUtil::IF()->constantDB()["Names"]["Drivers"];

	int favoriteIndex = 0;
	float favoriteTime = 0;
	for ( int i = 0; i < drivers.size(); i++ )
	{
		float time = statsRead()["Drivers"][drivers[i].asString()]["TotalTime"].asFloat();
		if ( time > favoriteTime )
		{
			favoriteIndex = i;
			favoriteTime = time;
		}
	}

	return drivers[favoriteIndex].asString();
}

//*****************************************************************************
void VuStatsManager::recordCarChampResult(const char *carName, int stage, int place)
{
	statsWrite()["CarChamp"][carName][stage]["Place"].putValue(place);

	// record games played
	addToIntStat("ChampionshipsPlayed", 1);

	if ( place == 1 )
		addToIntStat("ChampionshipWins", 1);
	if ( place >= 1 && place <= 3 )
		addToIntStat("ChampionshipPodiums", 1);
}
//*****************************************************************************
void VuStatsManager::recordFailureTimes(VuCarEntity* pCar, const std::string& eventName)
{

	addToIntStat("FailureTimes", 1);
}

//*****************************************************************************
void VuStatsManager::recordTime(VuCarEntity *pCar, const std::string &eventName)
{
	if ( eventName.length() )
	{
		VuJsonContainer &data = statsWrite()["Events"][eventName];

		float totalTime = (float)pCar->getStats().mTotalTime;

		float bestTime;
		if ( !getBestTime(eventName.c_str(), bestTime) || totalTime < bestTime )
			data["Time"].putValue(totalTime);
	}
}

//*****************************************************************************
void VuStatsManager::recordDistance(VuCarEntity *pCar, const std::string &eventName)
{
	if ( eventName.length() )
	{
		VuJsonContainer &data = statsWrite()["Events"][eventName];

		float distance = (float)pCar->getStats().mDistanceDriven;

		float bestDistance;
		if ( !getBestDistance(eventName.c_str(), bestDistance) || distance > bestDistance )
			data["Distance"].putValue(distance);
	}
}

//*****************************************************************************
void VuStatsManager::recordScore(VuCarEntity *pCar, const std::string &eventName)
{
	if ( eventName.length() )
	{
		VuJsonContainer &data = statsWrite()["Events"][eventName];

		int score = pCar->getStats().mScore;

		int bestScore;
		if ( !getBestScore(eventName.c_str(), bestScore) || score > bestScore )
			data["Score"].putValue(score);
	}
}

//*****************************************************************************
void VuStatsManager::recordCommon(VuCarEntity *pCar, const std::string &eventName, bool bossBattle)
{
	const VuCarStats &stats = pCar->getStats();

	addToFloatStat("TotalTime", (float)stats.mTotalTime);
	addToFloatStat("TotalDistance", (float)stats.mDistanceDriven);

	float carTotalTime = statsRead()["Cars"][pCar->getCarName()]["TotalTime"].asFloat();
	carTotalTime += (float)stats.mTotalTime;
	statsWrite()["Cars"][pCar->getCarName()]["TotalTime"].putValue(carTotalTime);

	float driverTotalTime = statsRead()["Drivers"][pCar->getDriverName()]["TotalTime"].asFloat();
	driverTotalTime += (float)stats.mTotalTime;
	statsWrite()["Drivers"][pCar->getCarName()]["TotalTime"].putValue(driverTotalTime);

	// record games played
	addToIntStat("CareerGamesPlayed", 1);

	// smashed things
	addToIntStat("SmashedSeagulls", stats.mSmashedStuff[VuCarStats::SEAGULL]);
	addToIntStat("SmashedPalms", stats.mSmashedStuff[VuCarStats::PALM]);
	addToIntStat("SmashedCrabs", stats.mSmashedStuff[VuCarStats::CRAB]);
	addToIntStat("SmashedLavaMonsters", stats.mSmashedStuff[VuCarStats::LAVA_MONSTER]);
	addToIntStat("SmashedDeathBats", stats.mSmashedStuff[VuCarStats::DEATH_BAT]);
	addToIntStat("SmashedYeti", stats.mSmashedStuff[VuCarStats::YETI]);
	addToIntStat("SmashedPenguins", stats.mSmashedStuff[VuCarStats::PENGUIN]);

	// longest jump
	recordMaxFloatStat("LongestJump", stats.mMaxJumpDistance);

	// update leaderboards
	VuGameServicesManager::IF()->submitScore("TotalDistance", VuTruncate(float(0.001*getFloatStat("TotalDistance"))));
	VuLeaderboardManager::IF()->submitScore("Weekly", (VUINT64)(stats.mDistanceDriven));

	// common event stats
	if ( eventName.length() )
	{
		VuJsonContainer &data = statsWrite()["Events"][eventName];

		int timesPlayed = data["TimesPlayed"].asInt();
		data["TimesPlayed"].putValue(timesPlayed + 1);

		// place
		int place = stats.mPlace;
		int bestPlace;
		if ( !getBestPlace(eventName.c_str(), bestPlace) || place < bestPlace )
			data["Place"].putValue(place);

		//GiftDialog stats
		VuJsonContainer giftDialogDB = VuGameUtil::IF()->giftDialogDB();
		VuJsonContainer giftDialogItem = giftDialogDB[eventName];
		
		std::time_t rawTime;
		std::time(&rawTime);

		std::time_t lastTime = data["PlayTime"].asInt64();

		int condition = VuStringUtil::readInt( giftDialogItem["PopCondition"].asString().c_str());

		do 
		{
			if (!IsSameDay(rawTime, lastTime))
			{
				data["FailTimes"].putValue(0);
				
			}

			if (place > condition)
			{
				int times = data["FailTimes"].asInt();
				data["FailTimes"].putValue(++times);
			}
			else
			{
				data["FailTimes"].putValue(0);
			}
		} while (0);

		
		data["PlayTime"].putValue((VUINT64)rawTime);
		// record wins / podiums
		if ( !bossBattle )
		{
			if ( place == 1 )
				addToIntStat("CareerWins", 1);
			if ( place >= 1 && place <= 3 )
				addToIntStat("CareerPodiums", 1);
		}

		// record highest event played
		int rowIndex = VuSpreadsheetQuery::findFirstRow(VuGameUtil::IF()->eventSpreadsheet(),
			VuSpreadsheetQuery::VuStringEqual("Event", eventName.c_str())
		);
		int eventIndex = rowIndex + 1;

		recordMaxIntStat("HighestEventPlayedIndex", eventIndex);
		recordMaxStringStat("HighestEventPlayedName", eventName);

		// analytics - How are players progressing through the game?
		if ( timesPlayed == 0 )
		{
			VuJsonContainer variables;

			variables["Coins Earned"].putValue(VuGameManager::IF()->getCurrencyEarned(VuGameManager::CT_STANDARD));
			variables["Coins Spent"].putValue(VuGameManager::IF()->getCurrencySpent(VuGameManager::CT_STANDARD));
			variables["Gems Earned"].putValue(VuGameManager::IF()->getCurrencyEarned(VuGameManager::CT_PREMIUM));
			variables["Gems Spent"].putValue(VuGameManager::IF()->getCurrencySpent(VuGameManager::CT_PREMIUM));
			variables["IAP Made"].putValue(VuGameManager::IF()->getIapMade());
			variables["Place"].putValue(stats.mPlace);
			variables["Car Upgrade %"].putValue(100.0f*VuGameManager::IF()->getCurCar().getStageProgress());

			VuAnalyticsManager::IF()->logEvent("Finished Event First Time", "Event Name", eventName.c_str(), variables);

			VuAnalyticsManager::IF()->setLevel(eventIndex);
		}
	}
}

void   VuStatsManager::recordEnterEventTimes(const std::string &eventName)
{
	VuJsonContainer &data = statsWrite()["Events"][eventName];
	int timesPlayed = data["TimesPlayed"].asInt();
	data["TimesPlayed"].putValue(timesPlayed + 1);
}