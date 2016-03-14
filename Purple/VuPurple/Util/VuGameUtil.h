//*****************************************************************************
//
//  Copyright (c) 2009-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Utilities for game logic
// 
//*****************************************************************************

#pragma once

#include "VuEngine/VuSystemComponent.h"
#include "VuEngine/Json/VuJsonContainer.h"
#include "VuEngine/Assets/VuDBAsset.h"
#include "VuEngine/Properties/VuEnumProperty.h"
#include "VuEngine/HAL/Audio/VuAudioStream.h"
#include "VuEngine/Math/VuRand.h"

class VuDBAsset;
class VuSpreadsheetAsset;
class VuVector3;
class VuMatrix;
class VuAabb;


class VuGameUtil : public VuSystemComponent
{
	DECLARE_SYSTEM_COMPONENT(VuGameUtil)

protected:
	friend class VuPurple;

	virtual bool init();
	virtual void release();

public:
	VuGameUtil();

	const VuJsonContainer	&constantDB()      { return mpDBAssets[CONSTANT_DB]->getDB(); }
	const VuJsonContainer	&carDB()	       { return mpDBAssets[CAR_DB]->getDB(); }
	const VuJsonContainer	&driverDB()        { return mpDBAssets[DRIVER_DB]->getDB(); }
	const VuJsonContainer	&champDB()         { return mpDBAssets[CHAMP_DB]->getDB(); }
	const VuJsonContainer	&ragdollDB()       { return mpDBAssets[RAGDOLL_DB]->getDB(); }
	const VuJsonContainer	&unlockDB()        { return mpDBAssets[UNLOCK_DB]->getDB(); }
	const VuJsonContainer	&leaderboardDB()   { return mpDBAssets[LEADERBOARD_DB]->getDB(); }
	const VuJsonContainer	&inputDB()         { return mpDBAssets[INPUT_DB]->getDB(); }
	const VuJsonContainer	&colorDB()         { return mpDBAssets[COLOR_DB]->getDB(); }
	const VuJsonContainer	&decalDB()         { return mpDBAssets[DECAL_DB]->getDB(); }
	const VuJsonContainer	&abilityDB()       { return mpDBAssets[ABILITY_DB]->getDB(); }
	const VuJsonContainer	&storeDB()         { return mpDBAssets[STORE_DB]->getDB(); }
	const VuJsonContainer	&wheelDB()         { return mpDBAssets[WHEEL_DB]->getDB(); }
	const VuJsonContainer	&propDB()          { return mpDBAssets[PROP_DB]->getDB(); }
	const VuJsonContainer	&seriesDB()        { return mpDBAssets[SERIES_DB]->getDB(); }
	const VuJsonContainer	&eventDB()         { return mpDBAssets[EVENT_DB]->getDB(); }
	const VuJsonContainer	&powerUpDB()       { return mpDBAssets[POWER_UP_DB]->getDB(); }
	const VuJsonContainer	&carEffectDB()     { return mpDBAssets[CAR_EFFECT_DB]->getDB(); }
	const VuJsonContainer	&carSkinDB()       { return mpDBAssets[CAR_SKIN_DB]->getDB(); }
	const VuJsonContainer	&aiPersonalityDB() { return mpDBAssets[AI_PERSONALITY_DB]->getDB(); }
	const VuJsonContainer	&specialDB()       { return mpDBAssets[SPECIAL_DB]->getDB(); }
	const VuJsonContainer &giftDialogDB() { return mpDBAssets[GIFTDIALOG_DB]->getDB(); }

	const VuSpreadsheetAsset *carSpreadsheet() { return mpSpreadsheetAssets[CAR_SPREADSHEET]; }
	const VuSpreadsheetAsset *driverSpreadsheet() { return mpSpreadsheetAssets[DRIVER_SPREADSHEET]; }
	const VuSpreadsheetAsset *eventSpreadsheet() { return mpSpreadsheetAssets[EVENT_SPREADSHEET]; }
	const VuSpreadsheetAsset *opponentSpreadsheet() { return mpSpreadsheetAssets[OPPONENT_SPREADSHEET]; }
	const VuSpreadsheetAsset *dailyChallengeSpreadsheet() { return mpSpreadsheetAssets[DAILY_CHALLENGE_SPREADSHEET]; }
	const VuSpreadsheetAsset *achievementSpreadsheet() { return mpSpreadsheetAssets[ACHIEVEMENT_SPREADSHEET]; }

	static void				buildSafeMatrixForCar(VuVector3 pos, VuVector3 fwd, const VuAabb &localAabb, VuMatrix &mat, bool bCheckGround);

	float					calcDisplayAspectRatio(VUHANDLE context = VUNULL);
	float					calcCameraVertFov(VUHANDLE context = VUNULL);

	typedef std::vector<std::string> Names;
	void					chooseAi(const std::string &type, const std::string &human, int count, Names &ai);

	// gfx
	const VuColor			&getAmbientColor() { return mAmbientColor; }
	const VuColor			&getDiffuseColor() { return mDiffuseColor; }

	// sfx
	enum eSfx { SFX_UI_BACK, SFX_UI_SELECT, SFX_UI_NAVIGATE, SFX_UI_ALERT, SFX_COUNT };
	typedef VuStaticIntEnumProperty::Choice SfxChoice;
	void					playSfx(eSfx sfx);
	static const SfxChoice	*getSfxChoices();

	void					playRewardTickSfx();

	// music
	enum eMusic { MUSIC_MENU, MUSIC_GAME };
	void					startMusic(eMusic music);
	void					setMusicVolume(float vol);
	void					stopMusic();

	// persistent data
	const VuJsonContainer	&dataRead()		{ return mData; }
	VuJsonContainer			&dataWrite()	{ return mData; }

	// series info
	int						calcEarnedSeriesStars(const char *seriesName);
	int						calcMaxSeriesStars(const char *seriesName);

	// demo mode
	bool					isDemoMode();
	bool					isPaidMode();

	// scene complexity
	int						getSceneComplexity() { return mSceneComplexity; }
	bool					getCarDropShadows() { return mCarDropShadows; }

	void					setShadowSplitDistances(const char *entryName);

	static void				configureAttractEvent(VuJsonContainer &gameData);
	static void				configureDemoEvent(VuJsonContainer &gameData);
	static void				configureSinglePlayerEvent(VuJsonContainer &gameData);
	static void				configureChallengeEvent(VuJsonContainer &gameData);
	static void				configureTutorialDriveEvent(VuJsonContainer &gameData);
	static void				configureTutorialPowerUpEvent(VuJsonContainer &gameData);
	static void				configureTutorialRaceEvent(VuJsonContainer &gameData);
	static void				configureSplitScreenEvent(VuJsonContainer &gameData);
	static void				configureDuelEvent(VuJsonContainer &gameData);
	static void				configureCarChampEvent(VuJsonContainer &gameData);

	// game data access
	const VuJsonContainer	&getEventData();
	const std::string		&getChampName() { return getEventData()["ChampName"].asString(); }
	const int				getChampStage() { return getEventData()["ChampStage"].asInt(); }
	const std::string		&getSeriesName() { return getEventData()["SeriesName"].asString(); }
	const std::string		&getEventName() { return getEventData()["EventName"].asString(); }
	const std::string		&getGameType() { return getEventData()["GameType"].asString(); }
	const std::string		&getTrack() { return getEventData()["Track"].asString(); }
	const char				*getPreviousSeriesName(const std::string &series);
	const char				*getNextSeriesName(const std::string &series);
	const char				*getSeriesBossName(const std::string &series);

	// string format
	static void				placeFormat(int place, char *str, VUUINT maxLen);
	static void				distanceFormat(float meters, char *str, VUUINT maxLen);
	static void				distanceFormatKm(float meters, char *str, VUUINT maxLen);


	// color lookup
	bool					getColor(const std::string &name, VuColor &color);
	bool					getDecal(const std::string &name, std::string &textureName);

	static void				configurePlayerCar(VuJsonContainer &carData);
	static void				reduceOpponents(VuJsonContainer &gameData);

	void					pushDisallowPauseMenu() { mPauseMenuDisallowed++; }
	void					popDisallowPauseMenu() { mPauseMenuDisallowed--; }
	bool					isPauseMenuAllowed() { return mPauseMenuDisallowed == 0; }

	const Names				&getCarNames() { return mCarNames; }
	const Names				&getDriverNames() { return mDriverNames; }

	void					setShowroomCar(const std::string &car) { mShowroomCar = car; }
	const std::string		&getSelectedCar();

	void					setLoungeDriver(const std::string &driver) { mLoungeDriver = driver; }
	const std::string		&getSelectedDriver();

	// prices

	/*!
 	* \author: xlm 2014/11/29  
 	* \brief: 获取汽车的价格类型， coin or rmb or dim
 	*/
	int						getCarPriceType(const std::string &carName);
	int						getCarPrice(const std::string &carName);
	int						getFullLevelCarCurPrice(const std::string &carName);
	int						getFullLevelCarPrimaryPrice(const std::string &carName);
	int						getUpgradedCarPrice(const std::string &carName);
	int						getCarUpgradePrice(const std::string &carName, int stage, int level, const std::string &statName);
	int						getCarLevelUpPrice(const std::string &carName, int newStage);
	int						getPowerUpPrice(const std::string &powerUpName);

	int						getNumCarLevels(int stage);
	int						getMaxCarLevel(int stage) { return getNumCarLevels(stage) - 1; }
	float					interpolateCarLevel(const std::string &carName, int stage, int level, const char *columnName);

	void					reloadDBs();

// modify by xlm,[/2015/4/15 ] 
	int getDriverPrice(const std::string& driverName);

	// split-screen ownership contention
	void					claimDriver(const std::string &driverName) { mClaimedDrivers.insert(driverName); }
	void					discardDriver(const std::string &driverName) { mClaimedDrivers.erase(driverName); }
	bool					isDriverClaimed(const std::string &driverName) { return mClaimedDrivers.find(driverName) != mClaimedDrivers.end(); }

	// opponent creation
	void					buildOpponentData(const std::string &opponentName, VuJsonContainer &opponentData);

	void					screenStackClear() { mScreenStack.clear(); }
	void					screenStackPush(const std::string &screen) { mScreenStack.push_back(screen); }
	void					screenStackPop() { mScreenStack.pop_back(); }
	std::string				&screenStackTop() 
	{ 
		return mScreenStack.back();
	}
	int						screenStackSize() { return (int)mScreenStack.size(); }

	const std::string		&getLastScreen() { return mLastScreen; }
	void					setLastScreen(const std::string &screen) { mLastScreen = screen; }

	void					setFrontEndCameraTransitionTime(float time) { mFrontEndCameraTransitionTime = time; }
	float					getFrontEndCameraTransitionTime() { return mFrontEndCameraTransitionTime; }

	void					setPauseMenuShown(bool shown) { mPauseMenuShown = shown; }
	void					setPauseMenuFadeAmount(float amount) { mPauseMenuFadeValue = amount; }
	bool					getPauseMenuShown() { return mPauseMenuShown; }
	float					getPauseMenuFadeValue() { return mPauseMenuFadeValue; }

	VuColor					calcHighlightColor(float alpha);

	void					pushCarPaintMode() { mCarPaintMode++; }
	void					popCarPaintMode() { mCarPaintMode--; }
	void					setCarPaint(const std::string &decal, const std::string &decalColor, const std::string &paintColor) { mCarPaintDecal = decal; mCarPaintDecalColor = decalColor; mCarPaintPaintColor = paintColor; }
	bool					getCarPaintMode() { return mCarPaintMode > 0; }
	const std::string		&getCarPaintDecal() { return mCarPaintDecal; }
	const std::string		&getCarPaintDecalColor() { return mCarPaintDecalColor; }
	const std::string		&getCarPaintPaintColor() { return mCarPaintPaintColor; }
	struct CarChampTableEntry
	{
		std::string	mCar;
		int			mStage;
		std::string	mDriver;
		std::string	mDecal;
		std::string	mDecalColor;
		std::string	mPaintColor;
		bool		mIsGold;
		int			mPoints;
		bool		mPlayer;
	};
	typedef std::vector<CarChampTableEntry> CarChampTable;
	void					createCarChampTable(const std::string &carName, CarChampTable &table);
	int						getCarChampRewardSC(int stage, int place);
	int						getCarChampRewardPC(int stage);

private:
	void					loadDBs();
	void					unloadDBs();

	void					configComplexity(int value) { mSceneComplexity = value; }
	void					configCarDropShadows(bool value) { mCarDropShadows = value; }


	enum { CONSTANT_DB, CAR_DB, DRIVER_DB, CHAMP_DB, RAGDOLL_DB, UNLOCK_DB, LEADERBOARD_DB, INPUT_DB, COLOR_DB, DECAL_DB, ABILITY_DB, STORE_DB, WHEEL_DB, PROP_DB, SERIES_DB, EVENT_DB, POWER_UP_DB, CAR_EFFECT_DB, CAR_SKIN_DB, AI_PERSONALITY_DB, SPECIAL_DB, GIFTDIALOG_DB, DB_COUNT };
	VuDBAsset				*mpDBAssets[DB_COUNT];

	enum { CAR_SPREADSHEET, DRIVER_SPREADSHEET, EVENT_SPREADSHEET, OPPONENT_SPREADSHEET, DAILY_CHALLENGE_SPREADSHEET, ACHIEVEMENT_SPREADSHEET, SPREADSHEET_COUNT };
	VuSpreadsheetAsset		*mpSpreadsheetAssets[SPREADSHEET_COUNT];

	VuColor					mAmbientColor;
	VuColor					mDiffuseColor;

	VuAudioStream			mMusic;
	float					mMusicVolume;
	VuJsonContainer			mData;
	int						mSceneComplexity;
	bool					mCarDropShadows;

	Names					mCarNames;
	Names					mDriverNames;

	std::string				mShowroomCar;
	std::string				mLoungeDriver;

	typedef std::set<std::string> ClaimedDrivers;
	ClaimedDrivers			mClaimedDrivers;

	typedef std::deque<std::string> ScreenStack;
	ScreenStack				mScreenStack;
	std::string				mLastScreen;

	float					mFrontEndCameraTransitionTime;
	bool					mPauseMenuShown;
	float					mPauseMenuFadeValue;
	int						mPauseMenuDisallowed;
	float					mHighlightRate;
	VuColor					mHighlightColor;

	int						mCarPaintMode;
	std::string				mCarPaintDecal;
	std::string				mCarPaintDecalColor;
	std::string				mCarPaintPaintColor;
	bool					mCarPaintIsGold;
};