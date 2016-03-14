//*****************************************************************************
//
//  Copyright (c) 2041-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Daily Challenge entities
// 
//*****************************************************************************

#include "VuPurple/Entities/UI/VuGameImageEntity.h"
#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Assets/VuTextureAsset.h"
#include "VuEngine/Managers/VuProfileManager.h"


//*****************************************************************************
// VuCreateCarChampEntity
//*****************************************************************************
class VuCreateCarChampEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuCreateCarChampEntity();

private:
	VuRetVal				Create(const VuParams &params);

	typedef std::deque<std::string> FilteredNames;
	void					filterNames(const VuJsonContainer &names, FilteredNames &filteredNames, const std::string &filter1);

	// components
	VuScriptComponent		*mpScriptComponent;
};

IMPLEMENT_RTTI(VuCreateCarChampEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuCreateCarChampEntity);

//*****************************************************************************
VuCreateCarChampEntity::VuCreateCarChampEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuCreateCarChampEntity, Create);
}

//*****************************************************************************
VuRetVal VuCreateCarChampEntity::Create(const VuParams &params)
{
	int stage = VuGameUtil::IF()->dataRead()["CarChamp"]["Stage"].asInt();
	const std::string &carName = VuGameUtil::IF()->dataRead()["CarChamp"]["Car"].asString();
	const std::string &driverName = VuGameUtil::IF()->dataRead()["CarChamp"]["Driver"].asString();

	const VuJsonContainer &stageConstants = VuGameUtil::IF()->constantDB()["CarChamps"]["Stages"][stage];
	const VuGameManager::Car &car = VuGameManager::IF()->getCar(carName);

	VuJsonContainer &carChampData = VuProfileManager::IF()->dataWrite()["CarChampData"][carName];
	carChampData.clear();

	// player
	carChampData["Stage"].putValue(stage);
	carChampData["Driver"].putValue(driverName);

	// opponents
	{
		const VuJsonContainer &names = VuGameUtil::IF()->constantDB()["Names"];

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

		const VuJsonContainer &opponents = stageConstants["Opponents"];
		int numOpponents = VuMin(opponents.size(), 5);
		for ( int i = 0; i < numOpponents; i++ )
		{
			VuJsonContainer &opponentData = carChampData["Opponents"][i];

			// Accel, Speed, Handling, Tough, Personality, Skill, and Brain are defined in constantDB
			opponentData = opponents[i];

			// determine car, driver, and skin
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
		}
	}

	// tracks
	{
		int raceCount = stageConstants["RaceCount"].asInt();
		int trackCount = stageConstants["Tracks"].size();

		std::vector<int> trackShuffleArray;
		trackShuffleArray.resize(trackCount);
		VuRand::global().createShuffleArray(trackCount, &trackShuffleArray[0]);

		for ( int iRace = 0; iRace < raceCount; iRace++ )
		{
			VuJsonContainer &raceData = carChampData["Races"][iRace];

			const std::string &track = stageConstants["Tracks"][trackShuffleArray[iRace]].asString();
			raceData["Track"].putValue(track);
		}
	}

	return VuRetVal();
}

//*****************************************************************************
void VuCreateCarChampEntity::filterNames(const VuJsonContainer &names, FilteredNames &filteredNames, const std::string &filter1)
{
	for ( int i = 0; i < names.size(); i++ )
	{
		const std::string &name = names[i].asString();
		if ( name != filter1 )
			filteredNames.push_back(name);
	}
}


//*****************************************************************************
// VuCarChampStateEntity
//*****************************************************************************
class VuCarChampStateEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuCarChampStateEntity();

private:
	VuRetVal			IsFinished(const VuParams &params);
	VuRetVal			IsFirstWin(const VuParams &params);
	VuRetVal			ConfigCarChamp(const VuParams &params);
	VuRetVal			IsInProgress(const VuParams &params);
	VuRetVal			setLastCarChampTime(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuCarChampStateEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuCarChampStateEntity);

//*****************************************************************************
VuCarChampStateEntity::VuCarChampStateEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this, 100));

	ADD_SCRIPT_INPUT(mpScriptComponent, VuCarChampStateEntity, IsFinished, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT(mpScriptComponent, VuCarChampStateEntity, IsFirstWin, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT(mpScriptComponent, VuCarChampStateEntity, IsInProgress, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuCarChampStateEntity, ConfigCarChamp);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuCarChampStateEntity, setLastCarChampTime);
}

//*****************************************************************************
VuRetVal VuCarChampStateEntity::IsFinished(const VuParams &params)
{
	const std::string &carName = VuGameUtil::IF()->dataRead()["CarChamp"]["Car"].asString();
	const VuJsonContainer &carChampData = VuProfileManager::IF()->dataRead()["CarChampData"][carName];

	int raceCount = carChampData["Races"].size();
	bool finished = carChampData["Races"][raceCount - 1].hasMember("Place");

	return VuRetVal(finished);
}

//*****************************************************************************
VuRetVal VuCarChampStateEntity::IsFirstWin(const VuParams &params)
{
	const std::string &carName = VuGameUtil::IF()->dataRead()["CarChamp"]["Car"].asString();
	VuJsonContainer &carChampData = VuProfileManager::IF()->dataWrite()["CarChampData"][carName];

	bool firstWin = carChampData["FirstWin"].asBool();

	return VuRetVal(firstWin);
}
//*****************************************************************************
VuRetVal	VuCarChampStateEntity::ConfigCarChamp(const VuParams &params)
{
	std::string cur_DriverName = VuGameManager::IF()->getCurDriverName();
	std::string cur_CarName = VuGameManager::IF()->getCurCarName();
	int cur_carStage = VuGameManager::IF()->getCurCar().getStage();


	const VuGameManager::Car &car = VuGameManager::IF()->getCar(cur_CarName);

	
	VuGameUtil::IF()->dataWrite()["CarChamp"]["Car"].putValue(cur_CarName);
	VuGameUtil::IF()->dataWrite()["CarChamp"]["Stage"].putValue(cur_carStage);
	VuGameUtil::IF()->dataWrite()["CarChamp"]["Driver"].putValue(cur_DriverName);
	
	return VuRetVal();
}
//*****************************************************************************
VuRetVal	VuCarChampStateEntity::IsInProgress(const VuParams &params)
{
	bool result = false;
	std::string cur_CarName = VuGameManager::IF()->getCurCarName();
	if (VuProfileManager::IF())
	{
		const VuJsonContainer &carChampData = VuProfileManager::IF()->dataRead()["CarChampData"][cur_CarName];
		const VuJsonContainer &races = carChampData["Races"];

		if (races.size() && !races[races.size() - 1].hasMember("Place"))
			result = true;
	}
	return VuRetVal(result);
}
//*****************************************************************************
VuRetVal	VuCarChampStateEntity::setLastCarChampTime(const VuParams &params)
{
	VUINT64 curLocalTime = VuTimeUtil::calcSecondsSince2000();
	VuGameManager::IF()->setLastCarChampChallengeTime(curLocalTime);
	return VuRetVal();
}
//*****************************************************************************
// Next Event Image Element
//*****************************************************************************
class VuNextCarChampTrackImageEntity : public VuGameImageBaseEntity
{
	DECLARE_RTTI

public:
	VuNextCarChampTrackImageEntity() : mpTextureAsset(VUNULL) {}

	virtual void onGameInitialize()
	{
		VuGameImageBaseEntity::onGameInitialize();

		const std::string &carName = VuGameUtil::IF()->dataRead()["CarChamp"]["Car"].asString();
		const VuJsonContainer &carChampData = VuProfileManager::IF()->dataRead()["CarChampData"][carName];

		int raceCount = carChampData["Races"].size();
		for ( int i = 0; i < raceCount; i++ )
		{
			const VuJsonContainer &raceData = carChampData["Races"][i];
			if ( !raceData.hasMember("Place") )
			{
				const std::string &trackName = raceData["Track"].asString();
				const std::string &assetName = "UI/SelectionIcons/Track_" + trackName;
				mpTextureAsset = VuAssetFactory::IF()->createAsset<VuTextureAsset>(assetName, VuAssetFactory::OPTIONAL_ASSET);
				break;
			}
		}
	}
	virtual void onGameRelease()
	{
		VuGameImageBaseEntity::onGameRelease();

		VuAssetFactory::IF()->releaseAsset(mpTextureAsset);
	}

	virtual VuTexture	*getTexture() { return mpTextureAsset ? mpTextureAsset->getTexture() : VUNULL; }

private:
	VuTextureAsset		*mpTextureAsset;
};

IMPLEMENT_RTTI(VuNextCarChampTrackImageEntity, VuGameImageBaseEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuNextCarChampTrackImageEntity);


