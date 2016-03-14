//*****************************************************************************
//
//  Copyright (c) 2009-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Utilities for game logic
// 
//*****************************************************************************

#include "VuGameUtil.h"

#include "VuPurple/Managers/VuStatsManager.h"
#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Managers/VuAnalyticsManager.h"
#include "VuPurple/PowerUps/VuPowerUpManager.h"
#include "VuPurple/Gfx/GfxComposer/VuPurpleGfxComposer.h"
#include "VuEngine/VuEngine.h"
#include "VuEngine/Assets/VuAssetFactory.h"
#include "VuEngine/Assets/VuSpreadsheetAsset.h"
#include "VuEngine/Managers/VuConfigManager.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Math/VuMatrix.h"
#include "VuEngine/Math/VuAabb.h"
#include "VuEngine/Water/VuWater.h"
#include "VuEngine/HAL/Gfx/VuGfx.h"
#include "VuEngine/HAL/Audio/VuAudio.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/DB/VuStringDB.h"
#include "VuEngine/Math/VuRand.h"
#include "VuEngine/Math/VuUnitConversion.h"
#include "VuEngine/Dynamics/Util/VuDynamicsRayTest.h"
#include "VuEngine/Util/VuUtf8.h"
#include "VuEngine/Util/VuStringUtil.h"
#include "VuEngine/Util/VuAudioUtil.h"
#include "VuEngine/Util/VuSpreadsheetQuery.h"
#include "VuEngine/Dev/VuDevConfig.h"

IMPLEMENT_SYSTEM_COMPONENT(VuGameUtil, VuGameUtil)


//*****************************************************************************
VuGameUtil::VuGameUtil():
	mAmbientColor(64,64,64),
	mDiffuseColor(255,255,255),
	mSceneComplexity(2),
	mCarDropShadows(true),
	mFrontEndCameraTransitionTime(0.0f),
	mPauseMenuShown(false),
	mPauseMenuFadeValue(0.0f),
	mPauseMenuDisallowed(0),
	mHighlightRate(1.0f),
	mHighlightColor(180, 180, 180),
	mCarPaintMode(0)
{
}

//*****************************************************************************
bool VuGameUtil::init()
{
	loadDBs();

	// configuration
	mSceneComplexity = VuConfigManager::IF()->getInt("Scene/Complexity")->mValue;
	mCarDropShadows = VuConfigManager::IF()->getBool("Effects/CarDropShadows")->mValue;

	VuConfigManager::IF()->registerIntHandler("Scene/Complexity", this, &VuGameUtil::configComplexity);
	VuConfigManager::IF()->registerBoolHandler("Effects/CarDropShadows", this, &VuGameUtil::configCarDropShadows);

	return true;
}

//*****************************************************************************
void VuGameUtil::release()
{
	VuConfigManager::IF()->unregisterIntHandler("Scene/Complexity", this);
	VuConfigManager::IF()->unregisterBoolHandler("Effects/CarDropShadows", this);

	unloadDBs();
}

//*****************************************************************************
void VuGameUtil::buildSafeMatrixForCar(VuVector3 pos, VuVector3 fwd, const VuAabb &localAabb, VuMatrix &mat, bool bCheckGround)
{
	VuVector3 up(0.0f, 0.0f, 1.0f);
	VuVector3 right = VuCross(fwd, up).normal();
	up = VuCross(right, fwd).normal();

	if ( bCheckGround )
	{
		class VuCheckGroundRayTestResult : public VuDynamicsRayTest::VuClosestResult
		{
		public:
			virtual bool needsCollision(VuRigidBody *pRigidBody)
			{
				if ( pRigidBody->getCollisionGroup() & COL_ENGINE_STATIC_PROP )
					return true;

				return false;
			}
			virtual bool addResult(const VuRigidBody *pRigidBody, float hitFraction, int triangleIndex, const VuVector3 &normal)
			{
				if ( (hitFraction <= mHitFraction) && (normal.mZ > 0.707107f) )
				{
					mpRigidBody = pRigidBody;
					mHitFraction = hitFraction;
					mTriangleIndex = triangleIndex;
					mHitNormal = normal;
					return true;
				}
				return false;
			}
		};

		VuVector3 from = pos + VuVector3(0.0f, 0.0f, 5.0f);
		VuVector3 to = pos + VuVector3(0.0f, 0.0f, -5.0f);
		VuCheckGroundRayTestResult result;
		VuDynamicsRayTest::test(from, to, result);
		if ( result.mbHasHit )
		{
			up = result.mHitNormal;
			right = VuCross(fwd, up).normal();
			fwd = VuCross(up, right).normal();

			pos.mZ = from.mZ + (to.mZ - from.mZ)*result.mHitFraction;
		}
	}

	mat.mX = VuVector4(right.mX, right.mY, right.mZ, 0.0f);
	mat.mY = VuVector4(fwd.mX, fwd.mY, fwd.mZ, 0.0f);
	mat.mZ = VuVector4(up.mX, up.mY, up.mZ, 0.0f);			
	mat.mT = VuVector4(pos.mX, pos.mY, pos.mZ, 1.0f);
}

//*****************************************************************************
float VuGameUtil::calcDisplayAspectRatio(VUHANDLE context)
{
	int displayWidth, displayHeight;
	VuGfx::IF()->getDisplaySize(context, displayWidth, displayHeight);
	return (float)displayWidth/displayHeight;
}

//*****************************************************************************
float VuGameUtil::calcCameraVertFov(VUHANDLE context)
{
	float vertFov = VuDegreesToRadians(constantDB()["Camera"]["VerticalFOV"].asFloat());

	return vertFov;
}

//*****************************************************************************
void VuGameUtil::chooseAi(const std::string &type, const std::string &human, int count, Names &ai)
{
	const VuJsonContainer &list = constantDB()[type];

	// create shuffle array
	std::vector<int> shuffleArray;
	shuffleArray.resize(list.size());
	VuRand::global().createShuffleArray((int)shuffleArray.size(), &shuffleArray[0]);

	// make shuffled array of all names
	Names names;
	for ( int i = 0; i < list.size(); i++ )
		names.push_back(list[shuffleArray[i]].asString());

	// make human last (if found)
	Names::iterator iter = std::find(names.begin(), names.end(), human);
	if ( iter != names.end() )
	{
		names.erase(iter);
		names.push_back(human);
	}

	// now populate ai list
	ai.clear();
	for ( int i = 0; i < count; i++ )
		ai.push_back(names[i%names.size()]);
}

//*****************************************************************************
void VuGameUtil::playSfx(eSfx sfx)
{
	static const char *sEventNames[] =
	{
		"Global/UI/Back",		// SFX_UI_BACK
		"Global/UI/Select",		// SFX_UI_SELECT
		"Global/UI/Navigate",	// SFX_UI_NAVIGATE
		"Global/UI/Alert",		// SFX_UI_ALERT
	};
	VU_COMPILE_TIME_ASSERT(sizeof(sEventNames)/sizeof(sEventNames[0]) == SFX_COUNT);

	VUASSERT(sfx >= 0 && sfx < SFX_COUNT, "invalid sfx");

	VuAudioUtil::playSfx(sEventNames[sfx]);
}

//*****************************************************************************
const VuGameUtil::SfxChoice *VuGameUtil::getSfxChoices()
{
	static VuStaticIntEnumProperty::Choice sSfxChoices[] =
	{
		{ "SFX_UI_BACK", SFX_UI_BACK },
		{ "SFX_UI_SELECT", SFX_UI_SELECT },
		{ "SFX_UI_NAVIGATE", SFX_UI_NAVIGATE },
		{ "SFX_UI_ALERT", SFX_UI_ALERT },
		{ VUNULL }
	};
	return sSfxChoices;
}

//*****************************************************************************
void VuGameUtil::startMusic(eMusic music)
{
	if ( VuDevConfig::IF()->getParam("MusicMute").asBool() )
		return;

	if ( mMusic.isPlaying() )
		return;

	if ( VuAudio::isOtherAudioPlaying() )
		return;

	const VuJsonContainer *pMusicData = VUNULL;
	if ( music == MUSIC_MENU )
	{
		pMusicData = &constantDB()["FrontEndMusic"];
	}
	else if ( music == MUSIC_GAME )
	{
		const VuJsonContainer &musicList = constantDB()["MusicList"];
		if ( musicList.size() )
		{
			VuJsonContainer &musicOrder = dataWrite()["Music"]["Order"];
			VuJsonContainer &musicIndex = dataWrite()["Music"]["Index"];

			if ( musicOrder.isNull() )
			{
				VuArray<int> orderArray(0);
				orderArray.resize(musicList.size());
				VuRand::global().createShuffleArray(orderArray.size(), &orderArray.begin());
				for ( int i = 0; i < orderArray.size(); i++ )
					musicOrder[i].putValue(orderArray[i]);
			}
			else
			{
				musicIndex.putValue((musicIndex.asInt() + 1)%musicList.size());
			}

			pMusicData = &musicList[musicOrder[musicIndex.asInt()].asInt()];
		}
	}

	if ( pMusicData )
	{
		const char *assetName = (*pMusicData)["Stream"].asCString();
		mMusicVolume = VuDecibelsToRatio((*pMusicData)["Volume"].asFloat());

		if ( mMusic.create(assetName, true) )
		{
			mMusic.play(false, "music");
			mMusic.setVolume(mMusicVolume);
		}
	}
}

//*****************************************************************************
void VuGameUtil::setMusicVolume(float vol)
{
	mMusic.setVolume(vol*mMusicVolume);
}

//*****************************************************************************
void VuGameUtil::stopMusic()
{
	mMusic.stop();
	mMusic.release();
}

//*****************************************************************************
int VuGameUtil::calcEarnedSeriesStars(const char *seriesName)
{
	int earnedStars = 0;

	if ( VuStatsManager::IF() )
	{
		// stars earned from winning events
		const VuSpreadsheetAsset *pSA = VuGameUtil::IF()->eventSpreadsheet();
		int seriesColumn = pSA->getColumnIndex("Series");
		int eventColumn = pSA->getColumnIndex("Event");
		int typeColumn = pSA->getColumnIndex("Type");
		for ( int iRow = 0; iRow < pSA->getRowCount(); iRow++ )
		{
			if ( strcmp(pSA->getField(iRow, seriesColumn).asCString(), seriesName) == 0 )
			{
				if ( strcmp(pSA->getField(iRow, typeColumn).asCString(), "BossBattle") != 0 )
				{
					const char *eventName = pSA->getField(iRow, eventColumn).asCString();
					int place;
					if ( VuStatsManager::IF()->getBestPlace(eventName, place) )
						earnedStars += VuClamp(4 - place, 0, 3);
				}
			}
		}
	}

	return earnedStars;
}

//*****************************************************************************
int VuGameUtil::calcMaxSeriesStars(const char *seriesName)
{
	int maxStars = 0;

	const VuSpreadsheetAsset *pSA = VuGameUtil::IF()->eventSpreadsheet();
	int seriesColumn = pSA->getColumnIndex("Series");
	int typeColumn = pSA->getColumnIndex("Type");
	for ( int iRow = 0; iRow < pSA->getRowCount(); iRow++ )
		if ( strcmp(pSA->getField(iRow, seriesColumn).asCString(), seriesName) == 0 )
			if ( strcmp(pSA->getField(iRow, typeColumn).asCString(), "BossBattle") != 0 )
				maxStars += 3;

	return maxStars;
}

//*****************************************************************************
bool VuGameUtil::isDemoMode()
{
	return strstr(VuAssetFactory::IF()->getSku().c_str(), "Demo") != VUNULL;
}

//*****************************************************************************
bool VuGameUtil::isPaidMode()
{
	if (VuAssetFactory::IF()->getSku() == "Xb1" ||
		VuAssetFactory::IF()->getSku() == "Ps4" ||
		VuAssetFactory::IF()->getSku() == "Lunar")
	{
		return true;
	}

	return false;
}

//*****************************************************************************
void VuGameUtil::setShadowSplitDistances(const char *entryName)
{
	const VuJsonContainer &entry = constantDB()["ShadowSplits"][entryName];
	if ( entry.isArray() )
	{
		VuPurpleGfxComposer::IF()->setShadowSplitDistances(entry[0].asFloat(), entry[1].asFloat(), entry[2].asFloat());
	}
}

//*****************************************************************************
void VuGameUtil::configureAttractEvent(VuJsonContainer &gameData)
{
	gameData = VuGameUtil::IF()->constantDB()["GameConfigs"]["Attract"];
}

//*****************************************************************************
void VuGameUtil::configureDemoEvent(VuJsonContainer &gameData)
{
	gameData = VuGameUtil::IF()->constantDB()["GameConfigs"]["Demo"];

	configurePlayerCar(gameData["Cars"][0]);
	reduceOpponents(gameData);
}

//*****************************************************************************
void VuGameUtil::configureSinglePlayerEvent(VuJsonContainer &gameData)
{
	gameData = VuGameUtil::IF()->dataRead()["EventData"];

	configurePlayerCar(gameData["Cars"][0]);
	reduceOpponents(gameData);

	const std::string &eventName = VuGameUtil::IF()->getEventName();
	const VuJsonContainer &eventData = VuGameUtil::IF()->eventDB()[eventName];
	gameData["Criteria"] = eventData["Criteria"];
	gameData["BreadCrumbCount"] = eventData["BreadCrumbCount"];
	gameData["TimeFactor"] = eventData["TimeFactor"];

	// limit player stage
	const std::string &seriesName = VuGameUtil::IF()->getSeriesName();
	int seriesStage = VuGameUtil::IF()->seriesDB()[seriesName]["Stage"].asInt();
	int playerStage = gameData["Cars"][0]["Properties"]["Stage"].asInt();
	if ( playerStage > seriesStage )
	{
		VuJsonContainer &carProps = gameData["Cars"][0]["Properties"];

		int maxLevel = VuGameUtil::IF()->getMaxCarLevel(seriesStage);

		carProps["Stage"].putValue(seriesStage);
		carProps["Accel"].putValue(maxLevel);
		carProps["Speed"].putValue(maxLevel);
		carProps["Handling"].putValue(maxLevel);
		carProps["Tough"].putValue(maxLevel);
	}


#ifndef VURETAIL
// DEBUG
//
// Game Balancing Utility
//
	if (VuDevConfig::IF()->getParam("TuneCarToEvent").asBool() == true)
	{
		// acceleration data
		int rowIndex = VuSpreadsheetQuery::findFirstRow(VuGameUtil::IF()->eventSpreadsheet(),
			VuSpreadsheetQuery::VuAnd(
				VuSpreadsheetQuery::VuStringEqual("Series", VuGameUtil::IF()->getSeriesName().c_str()),
				VuSpreadsheetQuery::VuStringEqual("Type", "BossBattle")
			)
		);

		const char* pOpponentName = VuGameUtil::IF()->eventSpreadsheet()->getField(rowIndex-1, "Opponents").asCString();
		

		// See if we want the minimally upgraded car for this series, instead of the maximally upgraded one
		std::string firstEventName;
		if (VuDevConfig::IF()->getParam("MinimumUpgradeForSeries").asBool() == true)
		{
			std::string eventName = VuGameUtil::IF()->getEventName();
			firstEventName = eventName.substr(0, eventName.size() - 2);
			firstEventName.append("01");

			int firstEventIndex = VuSpreadsheetQuery::findFirstRow(VuGameUtil::IF()->eventSpreadsheet(),
				VuSpreadsheetQuery::VuStringEqual("Event", firstEventName.c_str()));

			pOpponentName = VuGameUtil::IF()->eventSpreadsheet()->getField(firstEventIndex, "Opponents").asCString();
		}

		const VuSpreadsheetAsset* pOppSpreadsheet = VuGameUtil::IF()->opponentSpreadsheet();

		int oppRowIndex = VuSpreadsheetQuery::findFirstRow(pOppSpreadsheet, VuSpreadsheetQuery::VuStringEqual("Name", pOpponentName));

		VUINT	stage = pOppSpreadsheet->getField(oppRowIndex, "Stage").asInt(),
				accel = pOppSpreadsheet->getField(oppRowIndex, "Accel").asInt(),
				speed = pOppSpreadsheet->getField(oppRowIndex, "Speed").asInt(),
				handling = pOppSpreadsheet->getField(oppRowIndex, "Handling").asInt(),
				toughness = pOppSpreadsheet->getField(oppRowIndex, "Tough").asInt();

		VuDataUtil::putValue(gameData["Cars"][0]["Properties"]["Stage"], stage);
		VuDataUtil::putValue(gameData["Cars"][0]["Properties"]["Accel"], accel);
		VuDataUtil::putValue(gameData["Cars"][0]["Properties"]["Speed"], speed);
		VuDataUtil::putValue(gameData["Cars"][0]["Properties"]["Handling"], handling);
		VuDataUtil::putValue(gameData["Cars"][0]["Properties"]["Tough"], toughness);

		VUPRINTF("DEBUG: Player Car Tuned:\nStage:%d\nAccel:%d\nSpeed:%d\nHndlg:%d\nTough:%d\n", stage, accel, speed, handling, toughness);
	}
#endif
}

//*****************************************************************************
void VuGameUtil::configureChallengeEvent(VuJsonContainer &gameData)
{
	gameData = VuGameUtil::IF()->dataRead()["EventData"];

	const VuJsonContainer &challengeData = VuProfileManager::IF()->dataRead()["DailyChallenge"];

	// configure player car
	{
		VuJsonContainer &playerProps = gameData["Cars"][0]["Properties"];

		playerProps["Car"] = challengeData["Car"];
		playerProps["Stage"] = challengeData["Stage"];

		playerProps["Decal"] = challengeData["Decal"];
		playerProps["DecalColor"] = challengeData["DecalColor"];
		playerProps["PaintColor"] = challengeData["PaintColor"];

		playerProps["Driver"] = challengeData["Driver"];
		playerProps["DriverType"].putValue("Human");

		playerProps["Accel"].putValue(0);
		playerProps["Speed"].putValue(0);
		playerProps["Handling"].putValue(0);
		playerProps["Tough"].putValue(0);
	}

	// configure opponent cars
	const VuJsonContainer &opponents = challengeData["Opponents"];
	for ( int i = 0; i < opponents.size(); i++ )
	{
		VuJsonContainer &opponentProps = gameData["Opponents"][i]["Properties"];
		opponentProps = opponents[i];
		opponentProps["DriverType"].putValue("AI");

		VuJsonContainer &opponentAi = gameData["Opponents"][i]["Ai"];
		opponentAi["AiPersonality"].putValue("Default");
		opponentAi["AiSkill"].putValue(2);
		opponentAi["AiBrain"].putValue("Default");
	}

	// criteria
	gameData["Criteria"][0] = challengeData["Criteria"];
	gameData["TimeFactor"] = challengeData["TimeFactor"];
	gameData["BreadCrumbCount"] = challengeData["BreadCrumbCount"];
}

//*****************************************************************************
void VuGameUtil::configureTutorialDriveEvent(VuJsonContainer &gameData)
{
	gameData = VuGameUtil::IF()->constantDB()["GameConfigs"]["TutorialDrive"];

	configurePlayerCar(gameData["Cars"][0]);
}

//*****************************************************************************
void VuGameUtil::configureTutorialPowerUpEvent(VuJsonContainer &gameData)
{
	gameData = VuGameUtil::IF()->constantDB()["GameConfigs"]["TutorialPowerUp"];

	configurePlayerCar(gameData["Cars"][0]);
}

//*****************************************************************************
void VuGameUtil::configureTutorialRaceEvent(VuJsonContainer &gameData)
{
	gameData = VuGameUtil::IF()->constantDB()["GameConfigs"]["TutorialRace"];

	configurePlayerCar(gameData["Cars"][0]);
}

//*****************************************************************************
void VuGameUtil::configureSplitScreenEvent(VuJsonContainer &gameData)
{
	const std::string &champName = VuGameUtil::IF()->dataRead()["EventData"]["ChampName"].asString();
	const VuJsonContainer &champData = VuGameUtil::IF()->champDB()[champName];

	int champStage = VuGameUtil::IF()->dataRead()["EventData"]["ChampStage"].asInt();

	gameData["ChampName"].putValue(champName);
	gameData["ChampStage"].putValue(champStage);

	const VuJsonContainer &pads = VuGameUtil::IF()->dataRead()["SplitScreenPads"];
	for ( int i = 0; i < pads.size(); i++ )
	{
		const VuJsonContainer &configData = VuGameUtil::IF()->dataRead()["SplitScreenConfig"][i];
		VuJsonContainer &carData = gameData["Cars"][i];

		carData["Properties"]["Car"] = configData["Car"];

		carData["Properties"]["Stage"].putValue(champStage);

		const std::string &carSkin = configData["Skin"].asString();
		const VuJsonContainer &paintJob = VuGameUtil::IF()->carSkinDB()[carSkin];

		carData["Properties"]["Decal"] = paintJob["Decal"];
		carData["Properties"]["PaintColor"] = paintJob["PaintColor"];
		carData["Properties"]["DecalColor"] = paintJob["DecalColor"];

		carData["Properties"]["Driver"] = configData["Driver"];
		carData["Properties"]["DriverType"].putValue("Human");
		carData["Properties"]["PadIndex"].putValue(pads[i].asInt());
	}

	// analytics
	{
		VuJsonContainer variables;
		variables["2 Player"].putValue(pads.size() == 2);
		variables["3 Player"].putValue(pads.size() == 3);
		variables["4 Player"].putValue(pads.size() == 4);

		VuAnalyticsManager::IF()->logEvent("Split Screen Started", "Championship", champName.c_str(), variables);
	}
}

//*****************************************************************************
void VuGameUtil::configureDuelEvent(VuJsonContainer &gameData)
{
	const std::string &eventName = VuGameUtil::IF()->dataRead()["EventData"]["EventName"].asString();

	const VuJsonContainer &eventData = VuGameUtil::IF()->eventDB()[eventName];

	gameData = eventData;

	gameData["Opponents"][0] = eventData["Opponents"][VuGameManager::IF()->getUnlockedStage()];
	gameData["Opponents"].resize(1);

	configurePlayerCar(gameData["Cars"][0]);
}

//*****************************************************************************
void VuGameUtil::configureCarChampEvent(VuJsonContainer &gameData)
{
	const std::string &carName = VuGameUtil::IF()->dataRead()["CarChamp"]["Car"].asString();

	const VuJsonContainer &carChampData = VuProfileManager::IF()->dataRead()["CarChampData"][carName];

	int stage = carChampData["Stage"].asInt();
	const VuJsonContainer &constants = VuGameUtil::IF()->constantDB()["CarChamps"];

	int raceCount = carChampData["Races"].size();

	int raceIndex = 0;
	for ( raceIndex = 0; raceIndex < raceCount; raceIndex++ )
		if ( !carChampData["Races"][raceIndex].hasMember("Place") )
			break;

	const VuJsonContainer &raceData = carChampData["Races"][raceIndex];

	const std::string &trackName = raceData["Track"].asString();
	std::string projectAsset = trackName + "_Race";

	gameData["SeriesName"].clear();
	gameData["EventName"].clear();
	gameData["GameType"].putValue("CarChampRace");
	gameData["ProjectAsset"].putValue(projectAsset);
	gameData["Track"].putValue(trackName);
	gameData["LapCount"] = constants["LapCount"];
	gameData["IsChallenge"].putValue(false);

	// configure player car
	{
		const VuGameManager::Car &car = VuGameManager::IF()->getCar(carName);

		VuJsonContainer &playerProps = gameData["Cars"][0]["Properties"];

		playerProps["Car"].putValue(carName);
		playerProps["Stage"] = carChampData["Stage"];
		playerProps["Driver"] = carChampData["Driver"];
		playerProps["DriverType"].putValue("Human");

		playerProps["Decal"].putValue(car.mDecal);
		playerProps["DecalColor"].putValue(car.mDecalColor);
		playerProps["PaintColor"].putValue(car.mPaintColor);

		playerProps["Accel"].putValue(car.mAccel);
		playerProps["Speed"].putValue(car.mSpeed);
		playerProps["Handling"].putValue(car.mHandling);
		playerProps["Tough"].putValue(car.mTough);

		playerProps["IsGold"].putValue(car.mIsGold);
	}
	// configure opponent cars
	const VuJsonContainer &opponents = carChampData["Opponents"];
	for ( int i = 0; i < opponents.size(); i++ )
	{
		VuJsonContainer &opponentProps = gameData["Opponents"][i]["Properties"];
		opponentProps = opponents[i];
		opponentProps["DriverType"].putValue("AI");

		VuJsonContainer &opponentAi = gameData["Opponents"][i]["Ai"];
		opponentAi["AiPersonality"].putValue("Default");
		opponentAi["AiSkill"].putValue(2);
		opponentAi["AiBrain"].putValue("Default");
	}
}


//*****************************************************************************
const VuJsonContainer &VuGameUtil::getEventData()
{
	if ( dataRead().hasMember("GameData") )
		return dataRead()["GameData"];
	else if ( dataRead().hasMember("EventData") )
		return dataRead()["EventData"];

	return VuJsonContainer::null;
}

//*****************************************************************************
const char *VuGameUtil::getPreviousSeriesName(const std::string &series)
{
	int rowIndex = VuSpreadsheetQuery::findFirstRow(eventSpreadsheet(),
		VuSpreadsheetQuery::VuStringEqual("Series", series.c_str())
	);

	if ( rowIndex > 0 && rowIndex < eventSpreadsheet()->getRowCount() )
		return eventSpreadsheet()->getField(rowIndex - 1, "Series").asCString();

	return "";
}

//*****************************************************************************
const char *VuGameUtil::getNextSeriesName(const std::string &series)
{
	int rowIndex = VuSpreadsheetQuery::findLastRow(eventSpreadsheet(),
		VuSpreadsheetQuery::VuStringEqual("Series", series.c_str())
	);

	if ( rowIndex < eventSpreadsheet()->getRowCount() )
		return eventSpreadsheet()->getField(rowIndex + 1, "Series").asCString();

	return "";
}

//*****************************************************************************
const char *VuGameUtil::getSeriesBossName(const std::string &series)
{
	int rowIndex = VuSpreadsheetQuery::findFirstRow(eventSpreadsheet(),
		VuSpreadsheetQuery::VuAnd(
			VuSpreadsheetQuery::VuStringEqual("Series", series.c_str()),
			VuSpreadsheetQuery::VuStringEqual("Type", "BossBattle")
		)
	);

	const char *opponentName = eventSpreadsheet()->getField(rowIndex, "Opponents").asCString();
	rowIndex = VuSpreadsheetQuery::findFirstRow(opponentSpreadsheet(), VuSpreadsheetQuery::VuStringEqual("Name", opponentName));
	return opponentSpreadsheet()->getField(rowIndex, "Driver").asCString();
}

//*****************************************************************************
void VuGameUtil::placeFormat(int place, char *str, VUUINT maxLen)
{
	char temp[32];
	VU_SPRINTF(temp, sizeof(temp), "Place_%d", place);
	VU_STRCPY(str, maxLen, VuStringDB::IF()->getString(temp).c_str());
}

//*****************************************************************************
void VuGameUtil::distanceFormat(float meters, char *str, VUUINT maxLen)
{
	if ( meters >= 100000 )
	{
		VuStringUtil::integerFormat(VuRound(meters/1000), str, maxLen);
		VU_STRCAT(str, maxLen, " km");
	}
	else
	{
		VuStringUtil::integerFormat(VuRound(meters), str, maxLen);
		VU_STRCAT(str, maxLen, " m");
	}
}

//*****************************************************************************
void VuGameUtil::distanceFormatKm(float meters, char *str, VUUINT maxLen)
{
	VuStringUtil::integerFormat(VuRound(meters/1000), str, maxLen);
	VU_STRCAT(str, maxLen, " km");
}

//*****************************************************************************
bool VuGameUtil::getColor(const std::string &name, VuColor &color)
{
	color.set(0,0,0);
	const VuJsonContainer &entry = VuDataUtil::findArrayMember(colorDB(), "Name", name.c_str());
	return VuDataUtil::getValue(entry["Color"], color);
}

//*****************************************************************************
bool VuGameUtil::getDecal(const std::string &name, std::string &textureName)
{
	const VuJsonContainer &entry = VuDataUtil::findArrayMember(decalDB(), "Name", name.c_str());
	return VuDataUtil::getValue(entry["Texture"], textureName);
}

//*****************************************************************************
void VuGameUtil::configurePlayerCar(VuJsonContainer &carData)
{
	const VuGameManager::Car &car = VuGameManager::IF()->getCurCar();

	VuDataUtil::putValue(carData["Properties"]["Car"], car.mName);
	VuDataUtil::putValue(carData["Properties"]["Stage"], car.mStage);

	VuDataUtil::putValue(carData["Properties"]["Decal"], car.mDecal);
	VuDataUtil::putValue(carData["Properties"]["DecalColor"], car.mDecalColor);
	VuDataUtil::putValue(carData["Properties"]["PaintColor"], car.mPaintColor);

	const std::string &driverName = VuGameManager::IF()->getCurDriverName();
	VuDataUtil::putValue(carData["Properties"]["Driver"], driverName);
	VuDataUtil::putValue(carData["Properties"]["DriverType"], "Human");

	VuDataUtil::putValue(carData["Properties"]["Accel"], car.mAccel);
	VuDataUtil::putValue(carData["Properties"]["Speed"], car.mSpeed);
	VuDataUtil::putValue(carData["Properties"]["Handling"], car.mHandling);
	VuDataUtil::putValue(carData["Properties"]["Tough"], car.mTough);
}

//*****************************************************************************
void VuGameUtil::reduceOpponents(VuJsonContainer &gameData)
{
	if ( gameData["Cars"].size() + gameData["Opponents"].size() <= 6 )
		return;

	std::set<std::string> humanDrivers;
	for ( int i = 0; i < gameData["Cars"].size(); i++ )
		humanDrivers.insert(gameData["Cars"][i]["Properties"]["Driver"].asString());

	std::vector<std::string> opponents;
	for ( int i = 0; i < gameData["Opponents"].size(); i++ )
	{
		const std::string &opponentName = gameData["Opponents"][i].asString();
		int rowIndex = VuSpreadsheetQuery::findFirstRow(VuGameUtil::IF()->opponentSpreadsheet(), VuSpreadsheetQuery::VuStringEqual("Name", opponentName.c_str()));
		const char *driverName = VuGameUtil::IF()->opponentSpreadsheet()->getField(rowIndex, "Driver").asCString();
		if ( humanDrivers.find(driverName) == humanDrivers.end() )
			opponents.push_back(opponentName);
	}

	opponents.resize(VuMin((int)opponents.size(), 6 - gameData["Cars"].size()));

	gameData["Opponents"].clear();
	for ( auto &opponent : opponents )
		gameData["Opponents"].append().putValue(opponent);
}

//*****************************************************************************
const std::string &VuGameUtil::getSelectedCar()
{
	if ( !mShowroomCar.empty() )
		return mShowroomCar;

	if ( VuGameManager::IF() )
		return VuGameManager::IF()->getCurCarName();

	return VuJsonContainer::null.asString();
}

//*****************************************************************************
const std::string &VuGameUtil::getSelectedDriver()
{
	if ( !mLoungeDriver.empty() )
		return mLoungeDriver;

	if ( VuGameManager::IF() )
		return VuGameManager::IF()->getCurDriverName();

	return VuJsonContainer::null.asString();
}

//*****************************************************************************
int VuGameUtil::getCarPrice(const std::string &carName)
{
	int rowIndex = VuSpreadsheetQuery::findFirstRow(carSpreadsheet(),
		VuSpreadsheetQuery::VuStringEqual("Car", carName.c_str())
	);

	int price = carSpreadsheet()->getField(rowIndex, "Price").asInt();

// modify by xlm,[/2014/12/11 ] 
	//price = 10*VuRound(price/10.0f);

	return price;
}

int VuGameUtil::getFullLevelCarCurPrice(const std::string &carName)
{
	int rowIndex = VuSpreadsheetQuery::findFirstRow(carSpreadsheet(),
		VuSpreadsheetQuery::VuStringEqual("Car", carName.c_str())
		);

	int price = carSpreadsheet()->getField(rowIndex, "FullLvl").asInt();

	return price;
}

int VuGameUtil::getFullLevelCarPrimaryPrice(const std::string &carName)
{
	int rowIndex = VuSpreadsheetQuery::findFirstRow(carSpreadsheet(),
		VuSpreadsheetQuery::VuStringEqual("Car", carName.c_str())
		);

	int price = carSpreadsheet()->getField(rowIndex, "PrimaryPrice").asInt();

	return price;
}

/*!
* \author: xlm 2014/11/29
* \brief: 获取汽车的价格类型， coin or rmb or dim
*/
int	VuGameUtil::getCarPriceType(const std::string &carName)
{
	int rowIndex = VuSpreadsheetQuery::findFirstRow(carSpreadsheet(),
		VuSpreadsheetQuery::VuStringEqual("Car", carName.c_str())
		);

	int nType = carSpreadsheet()->getField(rowIndex, "CoinType").asInt();
	return nType;
}




//*****************************************************************************
int VuGameUtil::getUpgradedCarPrice(const std::string &carName)
{
	int rowIndex = VuSpreadsheetQuery::findFirstRow(carSpreadsheet(),
		VuSpreadsheetQuery::VuAnd(
			VuSpreadsheetQuery::VuStringEqual("Car", carName.c_str()),
			VuSpreadsheetQuery::VuIntEqual("Stage", VuGameManager::IF()->getUnlockedStage())
		)
	);

	int price = carSpreadsheet()->getField(rowIndex, "Price").asInt();
	price = 10*VuRound(price/10.0f);

	return price;
}

//*****************************************************************************
int VuGameUtil::getCarUpgradePrice(const std::string &carName, int stage, int level, const std::string &statName)
{
	int rowIndex = VuSpreadsheetQuery::findFirstRow(carSpreadsheet(),
		VuSpreadsheetQuery::VuStringEqual("Car", carName.c_str())
	);

	char columnName[64];
	VU_SPRINTF(columnName, sizeof(columnName), "%s Price", statName.c_str());
	int columnIndex = carSpreadsheet()->getColumnIndex(columnName);

	int levels = 0;
	float basePrice = 0.0f;
	float endPrice = 0.0f;
	for ( int curStage = 0; curStage <= stage; curStage++ )
	{
		levels = VuGameUtil::IF()->getNumCarLevels(curStage);
		float totalPrice = carSpreadsheet()->getField(rowIndex + curStage, columnIndex).asFloat();

		basePrice = endPrice;
		totalPrice -= (levels - 1)*basePrice;
		endPrice = basePrice + 2.0f*totalPrice/levels;
	}

	float fPrice = VuLerp(basePrice, endPrice, (float)level/(levels - 1));
	int iPrice = 10*VuRound(fPrice/10.0f);

	return iPrice;
}

//*****************************************************************************
int VuGameUtil::getCarLevelUpPrice(const std::string &carName, int newStage)
{
	const VuGameManager::Car &car = VuGameManager::IF()->getCar(carName);

	int priceSC = 0;

	if ( !car.mIsOwned )
	{
		priceSC += getCarPrice(carName);
	}

	if ( newStage > car.mStage )
	{
		// level up to next level
		{
			int maxLevel = car.getMaxLevel();

			for ( int level = car.mAccel + 1; level <= maxLevel; level++ )
				priceSC += VuGameUtil::IF()->getCarUpgradePrice(carName, car.mStage, level, "Accel");

			for ( int level = car.mSpeed + 1; level <= maxLevel; level++ )
				priceSC += VuGameUtil::IF()->getCarUpgradePrice(carName, car.mStage, level, "Speed");

			for ( int level = car.mHandling + 1; level <= maxLevel; level++ )
				priceSC += VuGameUtil::IF()->getCarUpgradePrice(carName, car.mStage, level, "Handling");

			for ( int level = car.mTough + 1; level <= maxLevel; level++ )
				priceSC += VuGameUtil::IF()->getCarUpgradePrice(carName, car.mStage, level, "Tough");
		}

		// level up subsequent levels
		for ( int stage = car.mStage + 1; stage < newStage; stage++ )
		{
			int maxLevel = VuGameUtil::IF()->getMaxCarLevel(stage);

			for ( int level = 1; level <= maxLevel; level++ )
				priceSC += VuGameUtil::IF()->getCarUpgradePrice(carName, stage, level, "Accel");

			for ( int level = 1; level <= maxLevel; level++ )
				priceSC += VuGameUtil::IF()->getCarUpgradePrice(carName, stage, level, "Speed");

			for ( int level = 1; level <= maxLevel; level++ )
				priceSC += VuGameUtil::IF()->getCarUpgradePrice(carName, stage, level, "Handling");

			for ( int level = 1; level <= maxLevel; level++ )
				priceSC += VuGameUtil::IF()->getCarUpgradePrice(carName, stage, level, "Tough");
		}
	}

	return priceSC;
}

//*****************************************************************************
int VuGameUtil::getPowerUpPrice(const std::string &powerUpName)
{
	int price = 0;

	if ( const VuPowerUp *pPowerUp = VuPowerUpManager::IF()->getPowerUpByName(powerUpName.c_str()) )
		price = pPowerUp->mPrice;

	return price;
}

//*****************************************************************************
int VuGameUtil::getNumCarLevels(int stage)
{
	return constantDB()["Game"]["NumCarLevels"][stage].asInt();
}

//*****************************************************************************
float VuGameUtil::interpolateCarLevel(const std::string &carName, int stage, int level, const char *columnName)
{
	int rowIndex = VuSpreadsheetQuery::findFirstRow(carSpreadsheet(),
		VuSpreadsheetQuery::VuAnd(
			VuSpreadsheetQuery::VuStringEqual("Car", carName.c_str()),
			VuSpreadsheetQuery::VuIntEqual("Stage", stage)
		)
	);

	int columnIndex = carSpreadsheet()->getColumnIndex(columnName);

	float value0 = VuGameUtil::IF()->carSpreadsheet()->getField(rowIndex, columnIndex).asFloat();
	float value1 = VuGameUtil::IF()->carSpreadsheet()->getField(rowIndex + 1, columnIndex).asFloat();

	int levels = VuGameUtil::IF()->getNumCarLevels(stage);
	float value = VuLerp(value0, value1, (float)level/levels);

	return value;
}

//*****************************************************************************
void VuGameUtil::reloadDBs()
{
	unloadDBs();
	loadDBs();

	VuEventManager::IF()->broadcast("OnReloadDBs");
}

//*****************************************************************************
void VuGameUtil::buildOpponentData(const std::string &opponentName, VuJsonContainer &opponentData)
{
	opponentData.clear();

	const VuSpreadsheetAsset *pSA = VuGameUtil::IF()->opponentSpreadsheet();

	VuSpreadsheetQuery::VuStringEqual expression("Name", opponentName.c_str());
	int rowIndex = VuSpreadsheetQuery::findFirstRow(pSA, expression);

	const VuFastContainer &row = pSA->getRow(rowIndex);

	const char *carName = pSA->getField(row, "Car").asCString();
	const char *driverName = pSA->getField(row, "Driver").asCString();
	const VuJsonContainer &skinData = carSkinDB()[driverDB()[driverName]["AiSkin"].asString()];

	opponentData["Properties"]["Car"].putValue(carName);
	opponentData["Properties"]["Driver"].putValue(driverName);
	opponentData["Properties"]["Decal"] = skinData["Decal"];
	opponentData["Properties"]["DecalColor"] = skinData["DecalColor"];
	opponentData["Properties"]["PaintColor"] = skinData["PaintColor"];
	opponentData["Properties"]["Stage"].putValue(pSA->getField(row, "Stage").asInt());
	opponentData["Properties"]["Accel"].putValue(pSA->getField(row, "Accel").asInt());
	opponentData["Properties"]["Speed"].putValue(pSA->getField(row, "Speed").asInt());
	opponentData["Properties"]["Handling"].putValue(pSA->getField(row, "Handling").asInt());
	opponentData["Properties"]["Tough"].putValue(pSA->getField(row, "Tough").asInt());

	// Store the opponent table entry with the car's carData for lookup/reference later
	opponentData["Properties"]["OpponentName"].putValue(opponentName);

	opponentData["Ai"]["AiPersonality"].putValue(pSA->getField(row, "Personality").asCString());
	opponentData["Ai"]["AiSkill"].putValue(pSA->getField(row, "Skill").asInt());
	opponentData["Ai"]["AiBrain"].putValue(pSA->getField(row, "Brain").asCString());
}

//*****************************************************************************
VuColor VuGameUtil::calcHighlightColor(float alpha)
{
	float ratio = 0.5f + 0.5f*(float)sin(VuSys::IF()->getTime()*VU_2PI*mHighlightRate);
	VuColor color = VuLerp(VuColor(255,255,255), mHighlightColor, ratio);
	color.mA = VuRound(alpha*255);
	return color;
}

//*****************************************************************************
//*****************************************************************************
void VuGameUtil::createCarChampTable(const std::string &carName, CarChampTable &table)
{
	const VuJsonContainer &carChampData = VuProfileManager::IF()->dataRead()["CarChampData"][carName];
	int stage = carChampData["Stage"].asInt();

	// player
	{
		const VuGameManager::Car &car = VuGameManager::IF()->getCar(carName);

		CarChampTableEntry entry;
		entry.mCar = carName;
		entry.mStage = stage;
		entry.mDriver = carChampData["Driver"].asString();
		entry.mDecal = car.mDecal;
		entry.mDecalColor = car.mDecalColor;
		entry.mPaintColor = car.mPaintColor;
		entry.mIsGold = car.mIsGold;
		entry.mPoints = carChampData["Points"].asInt();
		entry.mPlayer = true;
		table.push_back(entry);
	}

	// opponents
	for ( int i = 0; i < 5; i++ )
	{
		const VuJsonContainer &opponentData = carChampData["Opponents"][i];

		CarChampTableEntry entry;
		entry.mCar = opponentData["Car"].asString();
		entry.mStage = stage;
		entry.mDriver = opponentData["Driver"].asString();
		entry.mDecal = opponentData["Decal"].asString();
		entry.mDecalColor = opponentData["DecalColor"].asString();
		entry.mPaintColor = opponentData["PaintColor"].asString();
		entry.mIsGold = false;
		entry.mPoints = opponentData["Points"].asInt();
		entry.mPlayer = false;
		table.push_back(entry);
	}

	// sort entries
	std::sort(table.begin(), table.end(), [](const CarChampTableEntry &entry0, const CarChampTableEntry &entry1)
	{
		if ( entry0.mPoints != entry1.mPoints )
			return entry0.mPoints > entry1.mPoints;

		return entry0.mPlayer;
	});
}

//*****************************************************************************
int VuGameUtil::getCarChampRewardSC(int stage, int place)
{
	const char *key = isPaidMode() ? "Paid SC Reward" : "SC Reward";
	return VuGameUtil::IF()->constantDB()["CarChamps"]["Stages"][stage][key][place - 1].asInt();
}

//*****************************************************************************
int VuGameUtil::getCarChampRewardPC(int stage)
{
	const char *key = isPaidMode() ? "Paid PC Reward" : "PC Reward";
	return VuGameUtil::IF()->constantDB()["CarChamps"]["Stages"][stage][key].asInt();
}
void VuGameUtil::loadDBs()
{
	static const char *sDBNames[] =
	{
		"ConstantDB",      // CONSTANT_DB
		"CarDB",           // CAR_DB
		"DriverDB",        // DRIVER_DB
		"ChampDB",         // CHAMP_DB,
		"RagdollDB",       // RAGDOLL_DB
		"UnlockDB",        // UNLOCK_DB
		"LeaderboardDB",   // LEADERBOARD_DB
		"InputDB",         // INPUT_DB
		"ColorDB",         // COLOR_DB
		"DecalDB",         // DECAL_DB
		"AbilityDB",       // ABILITY_DB
		"StoreDB",         // STORE_DB
		"WheelDB",         // WHEEL_DB
		"PropDB",          // PROP_DB
		"SeriesDB",        // SERIES_DB
		"EventDB",         // EVENT_DB
		"PowerUpDB",       // POWER_UP_DB
		"CarEffectDB",     // CAR_EFFECT_DB
		"CarSkinDB",       // CAR_SKIN_DB
		"AiPersonalityDB", // AI_PERSONALITY_DB
		"SpecialDB",       // SPECIAL_DB
		"GiftDialogDB",
	};
	VU_COMPILE_TIME_ASSERT(sizeof(sDBNames)/sizeof(sDBNames[0]) == DB_COUNT);

	for ( int i = 0; i < DB_COUNT; i++ )
		mpDBAssets[i] = VuAssetFactory::IF()->createAsset<VuDBAsset>(sDBNames[i]);

	static const char *sSpreadsheetNames[] =
	{
		"Cars",            // CAR_SPREADSHEET
		"Drivers",         // DRIVER_SPREADSHEET
		"Events",          // EVENT_SPREADSHEET
		"Opponents",       // OPPONENT_SPREADSHEET
		"DailyChallenges", // DAILY_CHALLENGE_SPREADSHEET
		"Achievements",    // ACHIEVEMENT_SPREADSHEET
	};
	VU_COMPILE_TIME_ASSERT(sizeof(sSpreadsheetNames)/sizeof(sSpreadsheetNames[0]) == SPREADSHEET_COUNT);

	for ( int i = 0; i < SPREADSHEET_COUNT; i++ )
		mpSpreadsheetAssets[i] = VuAssetFactory::IF()->createAsset<VuSpreadsheetAsset>(sSpreadsheetNames[i]);

	// car/driver names
	mCarNames.clear();
	mDriverNames.clear();


	//test xlm
/*
	const VuJsonContainer &powerups = constantDB()["PowerUpConfig"];

	std::string powerupname;
	std::string racetype;
	int nprice;

	for (int i = 0; i < powerups.size(); i++)
	{
		racetype = powerups[i]["RaceType"].asString();
		powerupname = powerups[i]["PowerUp"].asString();
		nprice = powerups[i]["Price"].asInt();
	}
*/

	const VuJsonContainer &names = constantDB()["Names"];
	const VuJsonContainer &skuNames = names[VuAssetFactory::IF()->getSku()];
	const VuJsonContainer &cars = skuNames.hasMember("Cars") ? skuNames["Cars"] : names["Cars"];
	const VuJsonContainer &drivers = skuNames.hasMember("Drivers") ? skuNames["Drivers"] : names["Drivers"];
	for (int i = 0; i < cars.size(); i++)
		mCarNames.push_back(cars[i].asString());
	for (int i = 0; i < drivers.size(); i++)
		mDriverNames.push_back(drivers[i].asString());

	// constants
	VuDataUtil::getValue(constantDB()["Graphics"]["Ambient Color"], mAmbientColor);
	VuDataUtil::getValue(constantDB()["Graphics"]["Diffuse Color"], mDiffuseColor);
	VuDataUtil::getValue(constantDB()["UI"]["HighlightRate"], mHighlightRate);
}

//*****************************************************************************
void VuGameUtil::unloadDBs()
{
	for ( int i = 0; i < DB_COUNT; i++ )
		VuAssetFactory::IF()->releaseAsset(mpDBAssets[i]);

	for ( int i = 0; i < SPREADSHEET_COUNT; i++ )
		VuAssetFactory::IF()->releaseAsset(mpSpreadsheetAssets[i]);
}


// modify by xlm,[/2015/4/15 ] 
int VuGameUtil::getDriverPrice(const std::string& driverName)
{
	const VuSpreadsheetAsset *pSA = VuGameUtil::IF()->driverSpreadsheet();
	int rowIndex = VuSpreadsheetQuery::findFirstRow(pSA, VuSpreadsheetQuery::VuStringEqual("Driver", driverName.c_str()));

	int nPrice = -1;
	VuFastDataUtil::getValue(pSA->getField(rowIndex, "UnlockPrice"), nPrice);
	return nPrice;
}