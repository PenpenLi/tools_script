//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Daily Challenge entities
// 
//*****************************************************************************

#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Assets/VuAssetFactory.h"
#include "VuEngine/Assets/VuProjectAsset.h"
#include "VuEngine/Managers/VuProfileManager.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Util/VuTimeUtil.h"
#include "VuEngine/Util/VuSpreadsheetQuery.h"


//*****************************************************************************
// VuDailyChallengeEntity
//*****************************************************************************
class VuDailyChallengeEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuDailyChallengeEntity();

private:
	VuRetVal				Play(const VuParams &params);
	VuRetVal				SetCompleted(const VuParams &params);
	VuRetVal				IsCompleted(const VuParams &params);

	// components
	VuScriptComponent		*mpScriptComponent;
};

IMPLEMENT_RTTI(VuDailyChallengeEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuDailyChallengeEntity);

//*****************************************************************************
VuDailyChallengeEntity::VuDailyChallengeEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuDailyChallengeEntity, Play);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuDailyChallengeEntity, SetCompleted);
	ADD_SCRIPT_INPUT(mpScriptComponent, VuDailyChallengeEntity, IsCompleted, VuRetVal::Bool, VuParamDecl());
}

//*****************************************************************************
VuRetVal VuDailyChallengeEntity::Play(const VuParams &params)
{
	VuParams outParams;
	outParams.addString("ChallengeEvent");

	VuEventManager::IF()->broadcast("OnStartActionGame", outParams);

	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuDailyChallengeEntity::SetCompleted(const VuParams &params)
{
	int daysSince2000 = VuProfileManager::IF()->dataRead()["DailyChallenge"]["DaysSince2000"].asInt();
	VuGameManager::IF()->setCalendarDay(VuGameManager::DAILY_CHALLENGE_CALENDAR, daysSince2000);
	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuDailyChallengeEntity::IsCompleted(const VuParams &params)
{
	int daysSince2000 = VuProfileManager::IF()->dataRead()["DailyChallenge"]["DaysSince2000"].asInt();
	bool completed = VuGameManager::IF()->getCalendarDay(VuGameManager::DAILY_CHALLENGE_CALENDAR, daysSince2000);
	return VuRetVal(completed);
}

//*****************************************************************************
// VuCreateDailyChallengeEntity
//*****************************************************************************
class VuCreateDailyChallengeEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuCreateDailyChallengeEntity();

private:
	VuRetVal				Create(const VuParams &params);

	void					createData(int daysSince2000);

	typedef std::deque<std::string> FilteredNames;
	void					filterNames(const VuJsonContainer &names, FilteredNames &filteredNames, const std::string &filter1, const std::string &filter2 = "");

	// components
	VuScriptComponent		*mpScriptComponent;
};

IMPLEMENT_RTTI(VuCreateDailyChallengeEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuCreateDailyChallengeEntity);

//*****************************************************************************
VuCreateDailyChallengeEntity::VuCreateDailyChallengeEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuCreateDailyChallengeEntity, Create);
}

//*****************************************************************************
VuRetVal VuCreateDailyChallengeEntity::Create(const VuParams &params)
{
	int daysSince2000 = VuTimeUtil::calcDaysSince2000();
	if ( VuProfileManager::IF()->dataRead()["DailyChallenge"]["DaysSince2000"].asInt() != daysSince2000 )
	{
		createData(daysSince2000);
		VuProfileManager::IF()->dataWrite()["DailyChallenge"]["DaysSince2000"].putValue(daysSince2000);
	}

	// populate event data
	const VuJsonContainer &challengeData = VuProfileManager::IF()->dataRead()["DailyChallenge"];

	VuJsonContainer &eventData = VuGameUtil::IF()->dataWrite()["EventData"];

	const std::string &gameType = challengeData["GameType"].asString();
	const std::string &trackName = challengeData["Track"].asString();

	std::string projectAsset = trackName + "_" + gameType;
	if ( !VuAssetFactory::IF()->doesAssetExist<VuProjectAsset>(projectAsset) )
		projectAsset = trackName + "_Race";

	eventData["SeriesName"].clear();
	eventData["EventName"].clear();
	eventData["GameType"].putValue(gameType);
	eventData["ProjectAsset"].putValue(projectAsset);
	eventData["Track"].putValue(trackName);
	eventData["LapCount"] = VuGameUtil::IF()->constantDB()["DailyChallenge"][gameType]["LapCount"];
	eventData["TimeLimit"] = VuGameUtil::IF()->constantDB()["DailyChallenge"][gameType]["TimeLimit"];
	eventData["IsChallenge"].putValue(true);

	return VuRetVal();
}

//*****************************************************************************
void VuCreateDailyChallengeEntity::createData(int daysSince2000)
{
	VuJsonContainer &data = VuProfileManager::IF()->dataWrite()["DailyChallenge"];
	std::string prevTrack = data["Track"].asString();
	std::string prevGameType = data["GameType"].asString();
	std::string prevCar = data["Car"].asString();
	std::string prevDriver = data["Driver"].asString();
	std::string prevSkin = data["Skin"].asString();
	data.clear();

	const VuJsonContainer &constants = VuGameUtil::IF()->constantDB()["DailyChallenge"];
	const VuJsonContainer &names = VuGameUtil::IF()->constantDB()["Names"];

	// stage
	int stage = VuGameManager::IF()->getUnlockedStage();
	data["Stage"].putValue(stage);

	// track
	FilteredNames tracks;
	filterNames(names["Tracks"], tracks, prevTrack);
	const std::string &trackName = tracks[VuRand::global().range(0, (int)tracks.size())];
	data["Track"].putValue(trackName);

	// game type
	FilteredNames gameTypes;
	filterNames(constants["GameTypes"], gameTypes, prevGameType);
	const std::string &gameType = gameTypes[VuRand::global().range(0, (int)gameTypes.size())];
	data["GameType"].putValue(gameType);

	// car
	FilteredNames cars;
	filterNames(names["Cars"], cars, prevCar, VuGameManager::IF()->getCurCar().mName);
	const std::string &carName = cars[VuRand::global().range(0, (int)cars.size())];
	data["Car"].putValue(carName);

	// driver
	FilteredNames drivers;
	filterNames(names["Drivers"], drivers, prevDriver, VuGameManager::IF()->getCurDriver().mName);
	const std::string &driverName = drivers[VuRand::global().range(0, (int)drivers.size())];
	data["Driver"].putValue(driverName);

	// skin
	FilteredNames skins;
	filterNames(names["CarSkins"], skins, prevSkin, "");
	const std::string &skinName = skins[VuRand::global().range(0, (int)skins.size())];
	data["Skin"].putValue(skinName);

	const VuJsonContainer &skinData = VuGameUtil::IF()->carSkinDB()[skinName];
	data["Decal"] = skinData["Decal"];
	data["DecalColor"] = skinData["DecalColor"];
	data["PaintColor"] = skinData["PaintColor"];

	// opponents
	if ( gameType == "Race" || gameType == "Elimination" )
	{
		FilteredNames opponentCars;
		filterNames(names["Cars"], opponentCars, carName);
		std::vector<int> carShuffleArray;
		carShuffleArray.resize(opponentCars.size());
		VuRand::global().createShuffleArray((int)carShuffleArray.size(), &carShuffleArray[0]);

		FilteredNames opponentDrivers;
		filterNames(names["Drivers"], opponentDrivers, driverName);
		std::vector<int> driverShuffleArray;
		driverShuffleArray.resize(opponentDrivers.size());
		VuRand::global().createShuffleArray((int)driverShuffleArray.size(), &driverShuffleArray[0]);

		for ( int i = 0; i < 5; i++ )
		{
			VuJsonContainer &opponentData = data["Opponents"][i];

			const std::string &opponentCar = opponentCars[carShuffleArray[i]];
			const std::string &opponentDriver = opponentDrivers[driverShuffleArray[i]];
			const std::string &opponentSkin = VuGameUtil::IF()->driverDB()[opponentDriver]["AiSkin"].asString();

			const VuJsonContainer &opponentSkinData = VuGameUtil::IF()->carSkinDB()[opponentSkin];

			opponentData["Stage"].putValue(stage);
			opponentData["Car"].putValue(opponentCar);
			opponentData["Driver"].putValue(opponentDriver);
			opponentData["Decal"] = opponentSkinData["Decal"];
			opponentData["DecalColor"] = opponentSkinData["DecalColor"];
			opponentData["PaintColor"] = opponentSkinData["PaintColor"];
			opponentData["Accel"].putValue(VuRand::global().range(0, 3));
			opponentData["Speed"].putValue(VuRand::global().range(0, 3));
			opponentData["Handling"].putValue(VuRand::global().range(0, 3));
			opponentData["Tough"].putValue(VuRand::global().range(0, 3));
			opponentData["Skill"].putValue(2);
			opponentData["Personality"].putValue("Default");
			opponentData["Brain"].putValue("Default");
		}

		// Give the Challenge brain to a random opponent
		data["Opponents"][VuRand::global().range(0, 4)]["Personality"].putValue("Challenge");
	}

	// reward
	{
		const VuJsonContainer &rewards = constants["Rewards"][stage];
		int daysInRow = VuGameManager::IF()->getCalendarDaysInRow(VuGameManager::DAILY_CHALLENGE_CALENDAR, daysSince2000 - 1);
		int rewardIndex = VuMin(daysInRow, rewards.size() - 1);
		int reward = rewards[rewardIndex].asInt();
		data["Reward"].putValue(reward);
	}

	// criteria
	{
		const VuSpreadsheetAsset *pSA = VuGameUtil::IF()->dailyChallengeSpreadsheet();
		int rowIndex = VuSpreadsheetQuery::findFirstRow(pSA,
			VuSpreadsheetQuery::VuAnd(
				VuSpreadsheetQuery::VuIntEqual("Stage", stage),
				VuSpreadsheetQuery::VuStringEqual("Track", trackName.c_str())
			)
		);

		if ( gameType == "BoostBlitz" )
			data["Criteria"].putValue(pSA->getField(rowIndex, "BlitzTime").asInt());
		else if ( gameType == "DemolitionDerby" )
			data["Criteria"].putValue(pSA->getField(rowIndex, "DerbyScore").asInt());
		else if ( gameType == "ShootingGallery" )
			data["Criteria"].putValue(pSA->getField(rowIndex, "GalleryScore").asInt());
		else if ( gameType == "FollowLeader" )
			data["Criteria"].putValue(pSA->getField(rowIndex, "FollowScore").asInt());

		data["TimeFactor"].putValue(pSA->getField(rowIndex, "TimeFactor").asFloat());
		data["BreadCrumbCount"].putValue(pSA->getField(rowIndex, "Crumbs").asInt());
	}

	VuProfileManager::IF()->save();
}

//*****************************************************************************
void VuCreateDailyChallengeEntity::filterNames(const VuJsonContainer &names, FilteredNames &filteredNames, const std::string &filter1, const std::string &filter2)
{
	for ( int i = 0; i < names.size(); i++ )
	{
		const std::string &name = names[i].asString();
		if ( name != filter1 && name != filter2 )
			filteredNames.push_back(name);
	}
}


//*****************************************************************************
// VuDailyChallengeDayEntity
//*****************************************************************************
class VuDailyChallengeDayEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuDailyChallengeDayEntity();

private:
	VuRetVal				GetDay(const VuParams &params) { return VuRetVal(getDay()); }
	VuRetVal				In(const VuParams &params);

	int						getDay();

	// components
	VuScriptComponent		*mpScriptComponent;
};

IMPLEMENT_RTTI(VuDailyChallengeDayEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuDailyChallengeDayEntity);

//*****************************************************************************
VuDailyChallengeDayEntity::VuDailyChallengeDayEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	ADD_SCRIPT_INPUT(mpScriptComponent, VuDailyChallengeDayEntity, GetDay, VuRetVal::Int, VuParamDecl());
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuDailyChallengeDayEntity, In);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, 1);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, 2);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, 3);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, 4);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, 5);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, 6);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, 7+);
}

//*****************************************************************************
VuRetVal VuDailyChallengeDayEntity::In(const VuParams &params)
{
	int day = getDay();

	if ( day == 1 ) mpScriptComponent->getPlug("1")->execute();
	if ( day == 2 ) mpScriptComponent->getPlug("2")->execute();
	if ( day == 3 ) mpScriptComponent->getPlug("3")->execute();
	if ( day == 4 ) mpScriptComponent->getPlug("4")->execute();
	if ( day == 5 ) mpScriptComponent->getPlug("5")->execute();
	if ( day == 6 ) mpScriptComponent->getPlug("6")->execute();
	if ( day == 7 ) mpScriptComponent->getPlug("7+")->execute();

	return VuRetVal();
}

//*****************************************************************************
int VuDailyChallengeDayEntity::getDay()
{
	int daysSince2000 = VuProfileManager::IF()->dataRead()["DailyChallenge"]["DaysSince2000"].asInt();
	int daysInRow = VuGameManager::IF()->getCalendarDaysInRow(VuGameManager::DAILY_CHALLENGE_CALENDAR, daysSince2000 - 1);
	int day = VuClamp(daysInRow + 1, 1, 7);

	return day;
}