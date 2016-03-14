//*****************************************************************************
//
//  Copyright (c) 2012-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Class and utilities for metrics
// 
//*****************************************************************************

#include "VuEngine/HAL/Net/VuNet.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuPurple/Track/VuTrackManager.h"
#include "VuPurple/Track/VuTrackSector.h"
#include "VuPurple/Modes/VuGameModeManager.h"
#include "VuPurple/Entities/Car/Driver/VuHumanDriverEntity.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Games/VuGame.h"

#include "VuGameMetrics.h"

#ifdef ENABLE_VU_GAME_METRICS

//*****************************************************************************
VuGameMetrics::VuGameMetrics()
:
mEnabled(false),
mpGame(VUNULL),
mLastLogTime(0.0f),
mLogsPerSecond(0.0f),
mpSocket(VUNULL)
{
	mEnabled = false;
}

//*****************************************************************************
VuGameMetrics::~VuGameMetrics()
{
	mEnabled = false;
}

//*****************************************************************************
void VuGameMetrics::onGameStart(VuGame* pGame)
{
	if (mEnabled)
	{
		VUPRINTF("Error: GameMetrics is already enabled at onGameStart()\n");
		return;
	}

	if (!connect())
	{
		VUPRINTF("Error: GameMetrics could not connect to stats server! Disabled.\n");
		return;
	}

	mEnabled = true;

	mpGame = pGame;

	mLastLogTime = 0.0f;

	mLogsPerSecond = 2.0f;

	REG_EVENT_HANDLER(VuGameMetrics, OnRecovered);
	REG_EVENT_HANDLER(VuGameMetrics, OnTokenUsed);
	REG_EVENT_HANDLER(VuGameMetrics, OnTokenAdded);
	REG_EVENT_HANDLER(VuGameMetrics, OnTokenRefill);

	const VuJsonContainer &gameData = VuGameUtil::IF()->dataRead()["GameData"];
	mGameType = gameData["GameType"].asString();
}

//*****************************************************************************
void VuGameMetrics::onGameEnd()
{
	if (!mEnabled)
	{
		return;
	}

	// Write end of race stats
	for (int i = 0; i < mpGame->mCars.size(); i++)
	{
		VuCarEntity* pCar = mpGame->mCars[i];

		VuCarStats& stats = pCar->getStats();

		char tempStr[64];

		if (mGameType == "Race" ||
			mGameType == "Elimination" ||
			mGameType == "BoostBlitz" ||
			mGameType == "BossBattle")
		{
			// Time-based events
			VU_SPRINTF(tempStr, 64, "%#.2f", pCar->getStats().mTotalTime);
			reportEvent(pCar, "FinalTime", tempStr);
		}
		else if (mGameType == "DemolitionDerby" ||
			mGameType == "ShootingGallery" ||
			mGameType == "FollowLeader")
		{
			// Score-based events
			VU_SPRINTF(tempStr, 64, "%d", pCar->getStats().mScore);
			reportEvent(pCar, "FinalScore", tempStr);
		}

		VU_SPRINTF(tempStr, 64, "%d", stats.mPlace);
		reportEvent(pCar, "FinishPlace", tempStr);
	}

	disconnect();
}

//*****************************************************************************
void VuGameMetrics::onGameTick(float fdt)
{
	if (!mEnabled)
	{
		return;
	}

	if (VuSys::IF()->getTime() > mLastLogTime + 1.0f / mLogsPerSecond)
	{
		for (int i = 0; i < mpGame->mCars.size(); i++)
		{
			VuCarEntity* pCar = mpGame->mCars[i];
			if (pCar)
			{
				reportCarStatsEvent(pCar, VuGameUtil::IF()->getTrack());
			}
		}

		mLastLogTime = (float)VuSys::IF()->getTime();
	}
}

//*****************************************************************************
void VuGameMetrics::reportEvent(const std::string& eventName, const std::string& data)
{
	if (!mEnabled)
	{
		return;
	}

	std::string timeStr;
	getCurrentTimeString(timeStr);

	char logLine[1024];
	VU_SPRINTF(logLine, 1024, "{ \"User\" : \"%s\", \"Name\" : \"%s\", \"Data\" : \"%s\", \"Time\": \"%s\" }, \n",
		mUserIdentifier.c_str(),
		eventName.c_str(),
		data.c_str(),
		timeStr.c_str()
		);

	if (mpSocket)
	{
		mpSocket->send(logLine, strlen(logLine));
	}
}


//*****************************************************************************
void VuGameMetrics::reportEvent(VuCarEntity* pCar, const std::string& eventName, const std::string& data)
{
	if (!mEnabled)
	{
		return;
	}

	const std::string &trackName = VuGameUtil::IF()->getTrack();

	std::string timeStr;
	getCurrentTimeString(timeStr);

	std::string driverTypeStr;
	getDriverType(pCar, driverTypeStr);

	const std::string& opponent = pCar->getOpponentName();
	const std::string& raceEventName = mpGame->mEventName;
	const std::string& seriesName = mpGame->mSeriesName;

	char logLine[1024];
	VU_SPRINTF(logLine, 1024, "{ \"User\" : \"%s\", \"Name\" : \"%s\", \"Data\" : \"%s\", \"Car\" : \"%s\", \"Driver\" : \"%s\", \"Stage\": %d, \"DriverType\": \"%s\", \"Opponent\": \"%s\", \"Track\": \"%s\", \"Series\": \"%s\", \"Event\": \"%s\", \"Time\": \"%s\"}, \n",
		mUserIdentifier.c_str(),
		eventName.c_str(),
		data.c_str(),
		pCar->getCarName().c_str(),
		pCar->getDriverName().c_str(),
		pCar->getCarStage(),
		driverTypeStr.c_str(),
		opponent.c_str(),
		trackName.c_str(),
		seriesName.c_str(),
		raceEventName.c_str(),
		timeStr.c_str()
		);

	if (mpSocket)
	{
		mpSocket->send(logLine, strlen(logLine));
	}
}

//*****************************************************************************
void VuGameMetrics::reportCarStatsEvent(VuCarEntity* pCar, const std::string& eventData)
{
	if (!mEnabled)
	{
		return;
	}

	const std::string &trackName = VuGameUtil::IF()->getTrack();

	std::string timeStr;
	getCurrentTimeString(timeStr);

	std::string driverTypeStr;
	getDriverType(pCar, driverTypeStr);

	std::string carPosStr;
	getVector3String(pCar->getModelPosition(), carPosStr);

	const std::string& opponent = pCar->getOpponentName();

	const std::string& eventName = mpGame->mEventName;
	const std::string& seriesName = mpGame->mSeriesName;

	char logLine[1024];
	VU_SPRINTF(logLine, 1024, "{ \"User\" : \"%s\", \"Name\" : \"CarMetrics\", \"Data\" : \"%s\", \"Car\" : \"%s\", \"Driver\" : \"%s\", \"Stage\": %d, \"DriverType\": \"%s\", \"Opponent\": \"%s\", \"Track\": \"%s\", \"Series\": \"%s\", \"Event\": \"%s\", \"Throttle\": %#.4f, \"Steer\": %#.4f, \"Place\": %d, \"Pos\": %s, \"Time\": \"%s\"}, \n",
		mUserIdentifier.c_str(),
		eventData.c_str(),
		pCar->getCarName().c_str(),
		pCar->getDriverName().c_str(),
		pCar->getCarStage(),
		driverTypeStr.c_str(),
		opponent.c_str(),
		trackName.c_str(),
		seriesName.c_str(),
		eventName.c_str(),
		pCar->getThrottleControl(),
		pCar->getYawControl(),
		pCar->getStats().mPlace,
		carPosStr.c_str(),
		timeStr.c_str()
		);

	if (mpSocket)
	{
		mpSocket->send(logLine, strlen(logLine));
	}
}


//*****************************************************************************
void VuGameMetrics::reportRecovery(VuCarEntity* pCar, const VuVector3& prevPos, const VuVector3& curPos)
{
	if (!mEnabled)
	{
		return;
	}

	const std::string &trackName = VuGameUtil::IF()->getTrack();

	std::string timeStr;
	getCurrentTimeString(timeStr);

	std::string driverTypeStr;
	getDriverType(pCar, driverTypeStr);

	std::string prevPosStr;
	getVector3String(prevPos, prevPosStr);

	std::string curPosStr;
	getVector3String(curPos, curPosStr);

	const std::string& opponent = pCar->getOpponentName();
	const std::string& eventName = mpGame->mEventName;
	const std::string& seriesName = mpGame->mSeriesName;

	char logLine[1024];
	VU_SPRINTF(logLine, 1024, "{ \"User\" : \"%s\", \"Name\" : \"Recovery\", \"Data\" : \"true\", \"Car\" : \"%s\", \"Driver\" : \"%s\", \"Stage\": %d, \"DriverType\": \"%s\", \"Opponent\": \"%s\", \"Track\": \"%s\", \"Series\": \"%s\", \"Event\": \"%s\", \"RecoverFrom\": %s, \"RecoverTo\": %s, \"Time\": \"%s\"}, \n",
		mUserIdentifier.c_str(),
		pCar->getCarName().c_str(),
		pCar->getDriverName().c_str(),
		pCar->getCarStage(),
		driverTypeStr.c_str(),
		opponent.c_str(),
		trackName.c_str(),
		seriesName.c_str(),
		eventName.c_str(),
		prevPosStr.c_str(),
		curPosStr.c_str(),
		timeStr.c_str()
		);

	if (mpSocket)
	{
		mpSocket->send(logLine, strlen(logLine));
	}

}

//*****************************************************************************
// private:
//*****************************************************************************

//*****************************************************************************
bool VuGameMetrics::connect()
{
	mpSocket = VuTcpSocket::create(0);
	if (mpSocket && mpSocket->connect("10.1.10.20", 12345, 5000))
	{
		mpSocket->setNonBlocking(true);

		mUserIdentifier = VuSys::IF()->getUserIdentifier();
	}
	else
	{
		delete mpSocket;
		mpSocket = VUNULL;
		return false;
	}

	return true;
}

//*****************************************************************************
void VuGameMetrics::disconnect()
{
	if (mpSocket)
	{
		mpSocket->disconnect();
		delete mpSocket;
		mpSocket = VUNULL;
	}
}

//*****************************************************************************
void VuGameMetrics::getVector3String(const VuVector3& v, std::string& str)
{
	char tempStr[64];
	VU_SNPRINTF(tempStr, 64, 64, "[%#.4f,%#.4f,%#.4f]", v.mX, v.mY, v.mZ);
	str = tempStr;
}

//*****************************************************************************
void VuGameMetrics::getCurrentTimeString(std::string& timeString)
{
	time_t time_create = time(0);
	struct tm time_info;
	VU_LOCALTIME(&time_create, &time_info);

	char tempStr[64];
	VU_SPRINTF(tempStr, 64, "%0.2d-%0.2d-%0.2d_%0.2d:%0.2d:%0.2d", time_info.tm_mon, time_info.tm_mday, time_info.tm_year, time_info.tm_hour, time_info.tm_min, time_info.tm_sec);

	timeString = tempStr;
}

//*****************************************************************************
void VuGameMetrics::getDriverType(VuCarEntity* pCar, std::string& driverTypeStr)
{
	VuDriverEntity* pDriver = pCar->getDriver();

	if (pDriver->isAi())
	{
		driverTypeStr = "AI";
	}
	else if (pDriver->isHuman())
	{
		driverTypeStr = "Human";
	}
	else
	{
		driverTypeStr = "Other";
	}
}

//*****************************************************************************
void VuGameMetrics::OnRecovered(const VuParams& params)
{
	VuParams::VuAccessor data(params);

	VuEntity* pEntity = data.getEntity();
	if (!pEntity || !pEntity->isType("VuCarEntity"))
	{
		return;
	}

	VuCarEntity* pCar = static_cast<VuCarEntity*>(pEntity);

	VuVector3 prevPos = data.getVector3();
	VuVector3 curPos = data.getVector3();

	reportRecovery(pCar, prevPos, curPos);
}

//*****************************************************************************
void VuGameMetrics::OnTokenUsed(const VuParams& params)
{
	VuParams::VuAccessor data(params);
	int tokens = data.getInt();
	char str[32];
	VU_SPRINTF(str, sizeof(str), "%d", tokens);

	reportEvent("TokenUsed", str);
}

//*****************************************************************************
void VuGameMetrics::OnTokenAdded(const VuParams& params)
{
	VuParams::VuAccessor data(params);
	int tokens = data.getInt();
	char str[32];
	VU_SPRINTF(str, sizeof(str), "%d", tokens);

	reportEvent("TokenAdded", str);
}

//*****************************************************************************
void VuGameMetrics::OnTokenRefill(const VuParams& params)
{
	VuParams::VuAccessor data(params);
	int tokens = data.getInt();
	char str[32];
	VU_SPRINTF(str, sizeof(str), "%d", tokens);

	reportEvent("TokenRefill", str);
}

//*****************************************************************************


#endif // ENABLE_VU_GAME_METRICS
