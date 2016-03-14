//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Font macro handling
// 
//*****************************************************************************

#include "VuGameFontMacros.h"
#include "VuGameUtil.h"
#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Managers/VuStatsManager.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Managers/VuAchievementManager.h"
#include "VuPurple/Managers/VuCloudTuningManager.h"
#include "VuPurple/Managers/VuBillingManager.h"
#include "VuEngine/VuEngine.h"
#include "VuEngine/Assets/VuSpreadsheetAsset.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/Font/VuFontDraw.h"
#include "VuEngine/DB/VuStringDB.h"
#include "VuEngine/Managers/VuToastManager.h"
#include "VuEngine/Managers/VuMessageBoxManager.h"
#include "VuEngine/Util/VuStringUtil.h"
#include "VuEngine/Util/VuSpreadsheetQuery.h"
#include "VuPurple/Managers/VuPackManager.h"

#include "VuEngine/Util/VuUtf8.h"


class VuGameFontMacrosImpl : public VuGameFontMacros, public VuFontDraw::VuMacroHandler
{
public:
	virtual bool	init();
	virtual void	release();

	virtual void	setMacro(const char *macro, const char *text);

private:
	// VuFontDraw::VuMacroHandler
	virtual bool	handleMacro(const char *macro, std::string &text);

	typedef void (*Handler)(std::string &text);
	void			setHandler(const char *macro, Handler handler);


	static void		handle_STANDARD_CURRENCY(std::string &text);
	static void		handle_PREMIUM_CURRENCY(std::string &text);
	static void		handle_TOKENS(std::string &text);
	static void		handle_TOKEN_REFILL_PRICE(std::string &text);
	static void		handle_TOKEN_TIMER(std::string &text);
	static void		handle_SERIES_NAME(std::string &text);
	static void		handle_EVENT_NAME(std::string &text);
	static void		handle_LAP_COUNT(std::string &text);
	static void		handle_GAME_TYPE(std::string &text);
	static void		handle_GAME_TYPE_DESC(std::string &text);
	static void		handle_TRACK_NAME(std::string &text);
	static void		handle_SERIES_EARNED_STARS(std::string &text);
	static void		handle_SERIES_MAX_STARS(std::string &text);
	static void		handle_CHAMP_NAME(std::string &text);
	static void		handle_CHAMP_TOTAL_EVENTS(std::string &text);
	static void		handle_CHAMP_TRACK_NAME(std::string &text);
	static void		handle_CHAMP_EVENT_NUMBER(std::string &text);
	static void		handle_CHAMP_NEXT_TRACK_NAME(std::string &text);
	static void		handle_CHAMP_NEXT_EVENT_NUMBER(std::string &text);
	static void		handle_CHALLENGE_CAR_NAME(std::string &text);
	static void		handle_CHALLENGE_DRIVER_NAME(std::string &text);
	static void		handle_CHALLENGE_REWARD(std::string &text);
	static void		handle_CHALLENGE_SCORE(std::string &text);
	static void		handle_CHALLENGE_TARGET_SCORE(std::string &text);
	static void		handle_CHAMP_LICENSE(std::string &text);
	static void		handle_EVENT_RESULT(std::string &text);
	static void		handle_EVENT_SCORE(std::string &text);
	static void		handle_EVENT_HIGH_SCORE(std::string &text);
	static void		handle_EVENT_TARGET_SCORE(std::string &text);
	static void		handle_CAR_NAME(std::string &text);
	static void		handle_CAR_STAGE(std::string &text);
	static void		handle_CAR_PRICE(std::string &text);
	static void		handle_UNLOCK_DRIVER_PRICE(std::string &text);
	static void		handle_StarterPack_PRICE(std::string &text) { handlePackPrice("StarterPackEx", text); }
	static void		handle_SuperVipPack_PRICE(std::string &text){ handlePackPrice("SuperVipPack", text); }
	static void		handle_TuhaoPack_PRICE(std::string &text){ handlePackPrice("TuhaoPack", text); }

	static void		handle_SCAndPCPack_PRICE(std::string &text){ handlePackPrice("SCAndPCPack", text); }
	static void		handle_EuroPack_PRICE(std::string &text){ handlePackPrice("EuroPack", text); }
	static void		handle_MoonPack_PRICE(std::string &text){ handlePackPrice("MoonPack", text); }
	static void		handle_MusclePack_PRICE(std::string &text){ handlePackPrice("MusclePack", text); }

	static void		handle_Euro_PRICE(std::string &text){ handlePackPrice("Euro", text); }
	static void		handle_Moon_PRICE(std::string &text){ handlePackPrice("Moon", text); }
	static void		handle_Muscle_PRICE(std::string &text){ handlePackPrice("Muscle", text); }
	
	static void		handle_DisableAdsPack_PRICE(std::string &text){ handlePackPrice("DisableAds", text); }
	//static void		handle_BigGiftPack_PRICE(std::string &text){ handlePackPrice("BigGiftPack", text); }
	static void		handle_PropsPack_PRICE(std::string &text){ handlePackPrice("PropsPack", text); }
	static void		handle_WKPack_PRICE(std::string &text){ handlePackPrice("WKPack", text); }

	static void		handle_FullLevel_CAR_PRICE(std::string &text);

	static void		handle_UPGRADED_CAR_STAGE(std::string &text);
	static void		handle_UPGRADED_CAR_PRICE(std::string &text);
	static void		handle_UPGRADED_CAR_SAVINGS(std::string &text);
	static void		handle_DRIVER_NAME(std::string &text);
	static void		handle_DRIVER_PRICE(std::string &text);
	static void		handle_DRIVER_DESC(std::string &text);
	static void		handle_DRIVER_LOCKED_DESC(std::string &text);
	static void		handle_DRIVER_ABILITY_NAME(std::string &text);
	static void		handle_DRIVER_ABILITY_DESC(std::string &text);
	static void		handle_ACCEL_PRICE(std::string &text) { handleUpgradePrice("Accel", text); }
	static void		handle_SPEED_PRICE(std::string &text) { handleUpgradePrice("Speed", text); }
	static void		handle_HANDLING_PRICE(std::string &text) { handleUpgradePrice("Handling", text); }
	static void		handle_TOUGH_PRICE(std::string &text) { handleUpgradePrice("Tough", text); }
	static void		handle_POWER_UP_NAME(std::string &text);
	static void		handle_POWER_UP_DESC(std::string &text);
	static void		handle_POWER_UP_PRICE(std::string &text);
	static void		handle_REWARD_CURRENT(std::string &text) { handleReward(1, text); }
	static void		handle_REWARD_DOUBLE(std::string &text) { handleReward(2, text); }
	static void		handle_REWARD_FIRST(std::string &text);
	static void		handle_BOSS_NAME(std::string &text);
	static void		handle_SERIES_REQUIRED_BOSS(std::string &text);
	static void		handle_SERIES_REQUIRED_STAGE(std::string &text);
	static void		handle_TUNE_UPS(std::string &text);
	static void		handle_ACHIEVEMENTS_EARNED(std::string &text);
	static void		handle_ACHIEVEMENTS_TOTAL(std::string &text);
	static void		handle_ACHIEVEMENTS_SCORE_EARNED(std::string &text);
	static void		handle_ACHIEVEMENTS_SCORE_TOTAL(std::string &text);
	static void		handle_TOAST_TEXT(std::string &text);
	static void		handle_ACHIEVEMENT_TOAST_REWARD(std::string &text);
	static void		handle_MB_HEADING(std::string &text);
	static void		handle_MB_BODY(std::string &text);
	static void		handle_MB_BUTTON_A(std::string &text);
	static void		handle_MB_BUTTON_B(std::string &text);
	static void		handle_BOSS_ABILITY_NAME(std::string &text);
	static void		handle_BOSS_ABILITY_DESC(std::string &text);
	static void		handle_BOSS_INTRO(std::string &text);
	static void		handle_BOSS_WIN(std::string &text);
	static void		handle_BOSS_LOSE(std::string &text);
	static void		handle_CARCHAMP_EVENT_NUMBER(std::string &text);
	static void		handle_CARCHAMP_NEXT_TRACK_NAME(std::string &text);
	static void		handle_CARCHAMP_NEXT_EVENT_NUMBER(std::string &text);
	static void		handle_CARCHAMP_TOTAL_EVENTS(std::string &text);
	static void		handle_CARCHAMP_STAGE(std::string &text);
	static void		handle_POWER_UP_BONUSB_PRICE(std::string &text);
	static void		handle_POWER_UP_BONUSC_PRICE(std::string &text);

	static void		handleUpgradePrice(const char *stat, std::string &text);
	static void		handlePackPrice(const char *packItem, std::string &text);
	static void		handleReward(int mult, std::string &text);

	//modify by liudan
	static void handleMonsterTruckPackPrice(std::string &text);

// modify by xlm,[/2015/2/5 ] 

	//typedef std::hash_map<VUUINT32, std::string> MacroStrings;
	typedef std::map<VUUINT32, std::string> MacroStrings;

	MacroStrings	mMacroStrings;
	
// modify by xlm,[/2015/2/5 ] 
	//typedef std::hash_map<VUUINT32, Handler> Handlers;
	typedef std::map<VUUINT32, Handler> Handlers;

	Handlers		mHandlers;
};

IMPLEMENT_SYSTEM_COMPONENT(VuGameFontMacros, VuGameFontMacrosImpl)


//*****************************************************************************
bool VuGameFontMacrosImpl::init()
{
	VuGfxUtil::IF()->fontDraw()->addMacroHandler(this);

	setHandler("STANDARD_CURRENCY", handle_STANDARD_CURRENCY);
	setHandler("PREMIUM_CURRENCY", handle_PREMIUM_CURRENCY);
	setHandler("TOKENS", handle_TOKENS);
	setHandler("TOKEN_REFILL_PRICE", handle_TOKEN_REFILL_PRICE);
	setHandler("TOKEN_TIMER", handle_TOKEN_TIMER);
	setHandler("SERIES_NAME", handle_SERIES_NAME);
	setHandler("EVENT_NAME", handle_EVENT_NAME);
	setHandler("LAP_COUNT", handle_LAP_COUNT);
	setHandler("GAME_TYPE", handle_GAME_TYPE);
	setHandler("GAME_TYPE_DESC", handle_GAME_TYPE_DESC);
	setHandler("TRACK_NAME", handle_TRACK_NAME);
	setHandler("SERIES_EARNED_STARS", handle_SERIES_EARNED_STARS);
	setHandler("SERIES_MAX_STARS", handle_SERIES_MAX_STARS);
	setHandler("CHAMP_NAME", handle_CHAMP_NAME);
	setHandler("CHAMP_TOTAL_EVENTS", handle_CHAMP_TOTAL_EVENTS);
	setHandler("CHAMP_TRACK_NAME", handle_CHAMP_TRACK_NAME);
	setHandler("CHAMP_EVENT_NUMBER", handle_CHAMP_EVENT_NUMBER);
	setHandler("CHAMP_NEXT_TRACK_NAME", handle_CHAMP_NEXT_TRACK_NAME);
	setHandler("CHAMP_NEXT_EVENT_NUMBER", handle_CHAMP_NEXT_EVENT_NUMBER);
	setHandler("CHALLENGE_CAR_NAME", handle_CHALLENGE_CAR_NAME);
	setHandler("CHALLENGE_DRIVER_NAME", handle_CHALLENGE_DRIVER_NAME);
	setHandler("CHALLENGE_REWARD", handle_CHALLENGE_REWARD);
	setHandler("CHALLENGE_SCORE", handle_CHALLENGE_SCORE);
	setHandler("CHALLENGE_TARGET_SCORE", handle_CHALLENGE_TARGET_SCORE);
	setHandler("CHAMP_LICENSE", handle_CHAMP_LICENSE);
	setHandler("EVENT_RESULT", handle_EVENT_RESULT);
	setHandler("EVENT_SCORE", handle_EVENT_SCORE);
	setHandler("EVENT_HIGH_SCORE", handle_EVENT_HIGH_SCORE);
	setHandler("EVENT_TARGET_SCORE", handle_EVENT_TARGET_SCORE);
	setHandler("CAR_NAME", handle_CAR_NAME);
	setHandler("CAR_STAGE", handle_CAR_STAGE);
	setHandler("CAR_PRICE", handle_CAR_PRICE);
	setHandler("UNLOCK_DRIVER_PRICE", handle_UNLOCK_DRIVER_PRICE);
	setHandler("FullLevel_CAR_PRICE", handle_FullLevel_CAR_PRICE);
	setHandler("UPGRADED_CAR_STAGE", handle_UPGRADED_CAR_STAGE);
	setHandler("UPGRADED_CAR_PRICE", handle_UPGRADED_CAR_PRICE);
	setHandler("UPGRADED_CAR_SAVINGS", handle_UPGRADED_CAR_SAVINGS);
	setHandler("DRIVER_NAME", handle_DRIVER_NAME);
	setHandler("DRIVER_PRICE", handle_DRIVER_PRICE);
	setHandler("DRIVER_DESC", handle_DRIVER_DESC);
	setHandler("DRIVER_LOCKED_DESC", handle_DRIVER_LOCKED_DESC);
	setHandler("DRIVER_ABILITY_NAME", handle_DRIVER_ABILITY_NAME);
	setHandler("DRIVER_ABILITY_DESC", handle_DRIVER_ABILITY_DESC);
	setHandler("ACCEL_PRICE", handle_ACCEL_PRICE);
	setHandler("SPEED_PRICE", handle_SPEED_PRICE);
	setHandler("HANDLING_PRICE", handle_HANDLING_PRICE);
	setHandler("TOUGH_PRICE", handle_TOUGH_PRICE);
	setHandler("POWER_UP_NAME", handle_POWER_UP_NAME);
	setHandler("POWER_UP_DESC", handle_POWER_UP_DESC);
	setHandler("POWER_UP_PRICE", handle_POWER_UP_PRICE);
	setHandler("REWARD_CURRENT", handle_REWARD_CURRENT);
	setHandler("REWARD_DOUBLE", handle_REWARD_DOUBLE);
	setHandler("REWARD_FIRST", handle_REWARD_FIRST);
	setHandler("BOSS_NAME", handle_BOSS_NAME);
	setHandler("SERIES_REQUIRED_BOSS", handle_SERIES_REQUIRED_BOSS);
	setHandler("SERIES_REQUIRED_STAGE", handle_SERIES_REQUIRED_STAGE);
	setHandler("TUNE_UPS", handle_TUNE_UPS);
	setHandler("ACHIEVEMENTS_EARNED", handle_ACHIEVEMENTS_EARNED);
	setHandler("ACHIEVEMENTS_TOTAL", handle_ACHIEVEMENTS_TOTAL);
	setHandler("ACHIEVEMENTS_SCORE_EARNED", handle_ACHIEVEMENTS_SCORE_EARNED);
	setHandler("ACHIEVEMENTS_SCORE_TOTAL", handle_ACHIEVEMENTS_SCORE_TOTAL);
	setHandler("TOAST_TEXT", handle_TOAST_TEXT);
	setHandler("ACHIEVEMENT_TOAST_REWARD", handle_ACHIEVEMENT_TOAST_REWARD);
	setHandler("MB_HEADING", handle_MB_HEADING);
	setHandler("MB_BODY", handle_MB_BODY);
	setHandler("MB_BUTTON_A", handle_MB_BUTTON_A);
	setHandler("MB_BUTTON_B", handle_MB_BUTTON_B);
	setHandler("BOSS_ABILITY_NAME", handle_BOSS_ABILITY_NAME);
	setHandler("BOSS_ABILITY_DESC", handle_BOSS_ABILITY_DESC);
	setHandler("BOSS_INTRO", handle_BOSS_INTRO);
	setHandler("BOSS_WIN", handle_BOSS_WIN);
	setHandler("BOSS_LOSE", handle_BOSS_LOSE);

	setHandler("CARCHAMP_EVENT_NUMBER", handle_CARCHAMP_EVENT_NUMBER);
	setHandler("CARCHAMP_NEXT_TRACK_NAME", handle_CARCHAMP_NEXT_TRACK_NAME);
	setHandler("CARCHAMP_NEXT_EVENT_NUMBER", handle_CARCHAMP_NEXT_EVENT_NUMBER);
	setHandler("CARCHAMP_TOTAL_EVENTS", handle_CARCHAMP_TOTAL_EVENTS);
	setHandler("CARCHAMP_STAGE", handle_CARCHAMP_STAGE);
	setHandler("POWER_UP_BONUSB_PRICE", handle_POWER_UP_BONUSB_PRICE);
	setHandler("POWER_UP_BONUSC_PRICE", handle_POWER_UP_BONUSC_PRICE);

	setHandler("MonsterTruckPackPrice", handleMonsterTruckPackPrice);

	setHandler("PRICE_StarterPack", handle_StarterPack_PRICE);
	setHandler("PRICE_SuperVipPack", handle_SuperVipPack_PRICE);
	setHandler("PRICE_PropsPack", handle_PropsPack_PRICE);
	setHandler("PRICE_WKPack", handle_WKPack_PRICE);
	setHandler("PRICE_TuhaoPack", handle_TuhaoPack_PRICE);
	setHandler("PRICE_DisableAdsPack", handle_DisableAdsPack_PRICE);
	setHandler("PRICE_SCAndPCPack", handle_SCAndPCPack_PRICE);
	setHandler("PRICE_EuroPack", handle_EuroPack_PRICE);
	setHandler("PRICE_MoonPack", handle_MoonPack_PRICE);
	setHandler("PRICE_MusclePack", handle_MusclePack_PRICE);
	setHandler("PRICE_Euro", handle_Euro_PRICE);
	setHandler("PRICE_Moon", handle_Moon_PRICE);
	setHandler("PRICE_Muscle", handle_Muscle_PRICE);
	// constants
	setMacro("APP_VERSION", VuSys::IF()->getVersion());


	return true;
}

//*****************************************************************************
void VuGameFontMacrosImpl::setMacro(const char *macro, const char *text)
{
	VUUINT32 macroHash = VuHash::fnv32String(macro);
	mMacroStrings[macroHash] = text;
}

//*****************************************************************************
void VuGameFontMacrosImpl::release()
{
	VuGfxUtil::IF()->fontDraw()->removeMacroHandler(this);
}

//*****************************************************************************
bool VuGameFontMacrosImpl::handleMacro(const char *macro, std::string &text)
{
	VUUINT32 macroHash = VuHash::fnv32String(macro);

	// check macro strings
	const auto itStrings = mMacroStrings.find(macroHash);
	if ( itStrings != mMacroStrings.end() )
	{
		text += itStrings->second;
		return true;
	}

	// check handlers
	const auto itHandlers = mHandlers.find(macroHash);
	if ( itHandlers != mHandlers.end() )
	{
		itHandlers->second(text);
		return true;
	}

	return false;
}

//*****************************************************************************
void VuGameFontMacrosImpl::setHandler(const char *macro, Handler handler)
{
	VUUINT32 macroHash = VuHash::fnv32String(macro);
	mHandlers[macroHash] = handler;
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_STANDARD_CURRENCY(std::string &text)
{
	int sc = VuGameManager::IF()->getCurrency(VuGameManager::CT_STANDARD);

	char str[32];
	VuStringUtil::integerFormat(sc, str, sizeof(str));
	text += str;
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_PREMIUM_CURRENCY(std::string &text)
{
	int pc = VuGameManager::IF()->getCurrency(VuGameManager::CT_PREMIUM);

	char str[32];
	VuStringUtil::integerFormat(pc, str, sizeof(str));
	text += str;
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_TOKENS(std::string &text)
{
	if ( VuBillingManager::IF()->isUnlocked("DisableAds") )
	{
		text += VuStringDB::IF()->getString("Common_Infinity");
	}
	else
	{
		int tokens = VuGameManager::IF()->getTokens();

		char str[32];
		VuStringUtil::integerFormat(tokens, str, sizeof(str));
		text += str;
	}
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_TOKEN_REFILL_PRICE(std::string &text)
{
	int price = VuCloudTuningManager::IF()->variables().mTokenRefillPrice;

	char str[32];
	VuStringUtil::integerFormat(price, str, sizeof(str));
	text += str;
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_TOKEN_TIMER(std::string &text)
{
	if ( VuGameManager::IF()->getTokens() < VuCloudTuningManager::IF()->variables().mMaxTokens )
	{
		int timer = VuGameManager::IF()->getTokenTimer();

		char str[32];
		VuStringUtil::timeFormatSeconds(VuStringUtil::MM_SS, timer, str, sizeof(str));
		text += str;
	}
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_SERIES_NAME(std::string &text)
{
	const std::string &seriesName = VuGameUtil::IF()->getSeriesName();
	text += VuStringDB::IF()->getString(seriesName);
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_EVENT_NAME(std::string &text)
{
	const std::string &eventName = VuGameUtil::IF()->getEventName();
	text += VuStringDB::IF()->getString(eventName);
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_LAP_COUNT(std::string &text)
{
	int lapCount = VuGameUtil::IF()->getEventData()["LapCount"].asInt();

	char str[32];
	VU_SPRINTF(str, sizeof(str), "%d", lapCount);
	text += str;
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_GAME_TYPE(std::string &text)
{
	const std::string &gameType = VuGameUtil::IF()->getEventData()["GameType"].asString();

	char str[128];
	VU_SPRINTF(str, sizeof(str), "GameType_%s", gameType.c_str());
	text += VuStringDB::IF()->getString(str);
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_GAME_TYPE_DESC(std::string &text)
{
	const std::string &gameType = VuGameUtil::IF()->getEventData()["GameType"].asString();

	char str[128];
	VU_SPRINTF(str, sizeof(str), "GameType_%s_Desc", gameType.c_str());
	text += VuStringDB::IF()->getString(str);
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_TRACK_NAME(std::string &text)
{
	const std::string &track = VuGameUtil::IF()->getEventData()["Track"].asString();

	char str[128];
	VU_SPRINTF(str, sizeof(str), "Track_%s", track.c_str());
	text += VuStringDB::IF()->getString(str);
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_SERIES_EARNED_STARS(std::string &text)
{
	const std::string &seriesName = VuGameUtil::IF()->getSeriesName();
	int earnedStars = VuGameUtil::IF()->calcEarnedSeriesStars(seriesName.c_str());

	char str[32];
	VU_SPRINTF(str, sizeof(str), "%d", earnedStars);
	text += str;
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_SERIES_MAX_STARS(std::string &text)
{
	const std::string &seriesName = VuGameUtil::IF()->getSeriesName();
	int maxStars = VuGameUtil::IF()->calcMaxSeriesStars(seriesName.c_str());

	char str[32];
	VU_SPRINTF(str, sizeof(str), "%d", maxStars);
	text += str;
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_CHAMP_NAME(std::string &text)
{
	const std::string &champName = VuGameUtil::IF()->getChampName();
	text += VuStringDB::IF()->getString(champName);
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_CHAMP_TOTAL_EVENTS(std::string &text)
{
	const std::string &champName = VuGameUtil::IF()->getChampName();
	int totalEvents = VuGameUtil::IF()->champDB()[champName]["Events"].size();

	char str[32];
	VU_SPRINTF(str, sizeof(str), "%d", totalEvents);
	text += str;
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_CHAMP_TRACK_NAME(std::string &text)
{
	const std::string &champName = VuGameUtil::IF()->getChampName();
	int eventNumber = VuGameUtil::IF()->dataRead()["GameData"]["EventIndex"].asInt();
	const std::string &trackName = VuGameUtil::IF()->champDB()[champName]["Events"][eventNumber].asString();

	char str[128];
	VU_SPRINTF(str, sizeof(str), "Track_%s", trackName.c_str());
	text += VuStringDB::IF()->getString(str);
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_CHAMP_EVENT_NUMBER(std::string &text)
{
	int eventIndex = VuGameUtil::IF()->dataRead()["GameData"]["EventIndex"].asInt();

	char str[32];
	VU_SPRINTF(str, sizeof(str), "%d", eventIndex + 1);
	text += str;
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_CHAMP_NEXT_TRACK_NAME(std::string &text)
{
	const std::string &champName = VuGameUtil::IF()->getChampName();
	int eventNumber = VuGameUtil::IF()->dataRead()["GameData"]["EventIndex"].asInt();
	int nextEventNumber = eventNumber + 1;
	const std::string &trackName = VuGameUtil::IF()->champDB()[champName]["Events"][nextEventNumber].asString();

	char str[128];
	VU_SPRINTF(str, sizeof(str), "Track_%s", trackName.c_str());
	text += VuStringDB::IF()->getString(str);
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_CHAMP_NEXT_EVENT_NUMBER(std::string &text)
{
	int eventIndex = VuGameUtil::IF()->dataRead()["GameData"]["EventIndex"].asInt();
	int nextEventIndex = eventIndex + 1;

	char str[32];
	VU_SPRINTF(str, sizeof(str), "%d", nextEventIndex + 1);
	text += str;
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_CHALLENGE_CAR_NAME(std::string &text)
{
	const std::string &carName = VuProfileManager::IF()->dataRead()["DailyChallenge"]["Car"].asString();

	char str[64];
	VU_SPRINTF(str, sizeof(str), "Car_%s", carName.c_str());
	text += VuStringDB::IF()->getString(str);
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_CHALLENGE_DRIVER_NAME(std::string &text)
{
	const std::string &driverName = VuProfileManager::IF()->dataRead()["DailyChallenge"]["Driver"].asString();

	char str[64];
	VU_SPRINTF(str, sizeof(str), "Driver_%s", driverName.c_str());
	text += VuStringDB::IF()->getString(str);
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_CHALLENGE_REWARD(std::string &text)
{
	int sc = VuProfileManager::IF()->dataRead()["DailyChallenge"]["Reward"].asInt();

	char str[64];
	VuStringUtil::integerFormat(sc, str, sizeof(str));
	text += str;
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_CHALLENGE_SCORE(std::string &text)
{
	const VuJsonContainer &challengeData = VuProfileManager::IF()->dataRead()["DailyChallenge"];
	const std::string &gameType = challengeData["GameType"].asString();

	char str[64] = "";

	if ( VuCarManager::IF()->getLocalHumanCarCount() )
	{
		VuCarEntity *pCar = VuCarManager::IF()->getLocalHumanCar(0);
		const VuCarStats &stats = pCar->getStats();

		if ( gameType == "Race" || gameType == "Elimination" )
			VuGameUtil::placeFormat(stats.mPlace, str, sizeof(str));
		else if ( gameType == "BoostBlitz" )
			VuStringUtil::timeFormatSeconds(VuStringUtil::MM_SS_HH, (float)stats.mTotalTime, str, sizeof(str));
		else
			VuStringUtil::integerFormat(stats.mScore, str, sizeof(str));
	}

	text += str;
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_CHALLENGE_TARGET_SCORE(std::string &text)
{
	const VuJsonContainer &challengeData = VuProfileManager::IF()->dataRead()["DailyChallenge"];
	const std::string &gameType = challengeData["GameType"].asString();
	const VuJsonContainer &criteria = challengeData["Criteria"];

	char str[64] = "";

	if ( gameType == "Race" || gameType == "Elimination" )
		VuGameUtil::placeFormat(1, str, sizeof(str));
	else if ( gameType == "BoostBlitz" )
		VuStringUtil::timeFormatSeconds(VuStringUtil::MM_SS_HH, criteria.asFloat(), str, sizeof(str));
	else
		VuStringUtil::integerFormat(criteria.asInt(), str, sizeof(str));

	text += str;
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_CHAMP_LICENSE(std::string &text)
{
	int stage = VuGameUtil::IF()->getChampStage();

	char str[32];
	VU_SPRINTF(str, sizeof(str), "License%d", stage);
	text += VuStringDB::IF()->getString(str);
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_EVENT_RESULT(std::string &text)
{
	char str[32] = "";

	if ( VuCarManager::IF()->getLocalHumanCarCount() )
	{
		VuCarEntity *pCar = VuCarManager::IF()->getLocalHumanCar(0);

		const std::string &gameType = VuGameUtil::IF()->getGameType();
		if ( gameType == "Race" || gameType == "Elimination" )
			VuGameUtil::placeFormat(pCar->getStats().mPlace, str, sizeof(str));
		else if ( gameType == "BoostBlitz" )
			VuStringUtil::timeFormatSeconds(VuStringUtil::MM_SS_HH, (float)pCar->getStats().mTotalTime, str, sizeof(str));
		else if ( gameType == "ShootingGallery" || gameType == "DemolitionDerby" || gameType == "FollowLeader" )
			VuStringUtil::integerFormat(pCar->getStats().mScore, str, sizeof(str));
	}

	text += str;
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_EVENT_SCORE(std::string &text)
{
	char str[32] = "";

	if ( VuCarManager::IF()->getLocalHumanCarCount() )
	{
		VuCarEntity *pCar = VuCarManager::IF()->getLocalHumanCar(0);

		const std::string &gameType = VuGameUtil::IF()->getGameType();
		if ( gameType == "Race" || gameType == "BoostBlitz" )
		{
			VuStringUtil::timeFormatSeconds(VuStringUtil::MM_SS_HH, (float)pCar->getStats().mTotalTime, str, sizeof(str));
		}
		else if ( gameType == "Elimination" )
		{
			VuStringUtil::integerFormat((int)pCar->getStats().mDistanceDriven, str, sizeof(str));
			VU_STRCAT(str, sizeof(str), " m");
		}
		else if ( gameType == "ShootingGallery" || gameType == "DemolitionDerby" || gameType == "FollowLeader" )
		{
			VuStringUtil::integerFormat(pCar->getStats().mScore, str, sizeof(str));
		}
	}

	text += str;
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_EVENT_HIGH_SCORE(std::string &text)
{
	char str[32] = "";

	const std::string &eventName = VuGameUtil::IF()->getEventName();
	const std::string &gameType = VuGameUtil::IF()->getGameType();

	if ( gameType == "Race" || gameType == "BoostBlitz" )
	{
		float bestTime = 0;
		if ( VuStatsManager::IF()->getBestTime(eventName.c_str(), bestTime) )
			VuStringUtil::timeFormatSeconds(VuStringUtil::MM_SS_HH, bestTime, str, sizeof(str));
		else
			VU_STRCPY(str, sizeof(str), VuStringDB::IF()->getString("Common_MissingTime").c_str());
	}
	else if ( gameType == "Elimination" )
	{
		float bestDistance = 0;
		if ( VuStatsManager::IF()->getBestDistance(eventName.c_str(), bestDistance) )
		{
			VuStringUtil::integerFormat((int)bestDistance, str, sizeof(str));
			VU_STRCAT(str, sizeof(str), " m");
		}
		else
		{
			VU_STRCPY(str, sizeof(str), VuStringDB::IF()->getString("Common_MissingDistance").c_str());
		}
	}
	else if ( gameType == "ShootingGallery" || gameType == "DemolitionDerby" || gameType == "FollowLeader" )
	{
		int bestScore = 0;
		if ( VuStatsManager::IF()->getBestScore(eventName.c_str(), bestScore) )
			VuStringUtil::integerFormat(bestScore, str, sizeof(str));
		else
			VU_STRCPY(str, sizeof(str), VuStringDB::IF()->getString("Common_MissingScore").c_str());
	}

	text += str;
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_EVENT_TARGET_SCORE(std::string &text)
{
	const std::string &eventName = VuGameUtil::IF()->getEventName();
	const std::string &gameType = VuGameUtil::IF()->getGameType();
	const VuJsonContainer &criteria = VuGameUtil::IF()->eventDB()[eventName]["Criteria"];

	char str[64] = "";

	if ( gameType == "Race" || gameType == "Elimination" )
		VuGameUtil::placeFormat(1, str, sizeof(str));
	else if ( gameType == "BoostBlitz" )
		VuStringUtil::timeFormatSeconds(VuStringUtil::MM_SS_HH, criteria[0].asFloat(), str, sizeof(str));
	else
		VuStringUtil::integerFormat(criteria[0].asInt(), str, sizeof(str));

	text += str;
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_CAR_NAME(std::string &text)
{
	const std::string &selectedCar = VuGameUtil::IF()->getSelectedCar();

	char str[64];
	VU_SPRINTF(str, sizeof(str), "Car_%s", selectedCar.c_str());
	text += VuStringDB::IF()->getString(str);
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_CAR_STAGE(std::string &text)
{
	const std::string &selectedCar = VuGameUtil::IF()->getSelectedCar();
	int stage = VuGameManager::IF()->getCar(selectedCar).mStage;

	char str[64];
	VU_SPRINTF(str, sizeof(str), "Stage_%d", stage);
	text += VuStringDB::IF()->getString(str);
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_CAR_PRICE(std::string &text)
{
	const std::string &selectedCar = VuGameUtil::IF()->getSelectedCar();
	int price = VuGameUtil::IF()->getCarPrice(selectedCar);

// modify by xlm,[/2014/11/29 ] 判断货币类型
	int nPriceType = VuGameUtil::IF()->getCarPriceType(selectedCar);

	char str[64];
	VuStringUtil::integerFormat(price, str, sizeof(str));
	if (nPriceType == 2)
	{
		text += str;
		text += VuStringDB::IF()->getStringSelf("PriceUnit");

/*
		wchar_t* strUnit = L"元";
		std::string strUtfUnit;
		VuUtf8::convertWCharStringToUtf8String(strUnit, strUtfUnit);

		text += strUtfUnit;

		
		//text += "元";*/
	}
	else if (nPriceType == 3)
	{
		text += str;
		text += VuStringDB::IF()->getStringSelf("DiamondUnit");
	}
	else
	{
		text += str;
		text += VuStringDB::IF()->getStringSelf("showroom_coin_unit");
	}

	
}

 void		VuGameFontMacrosImpl::handle_UNLOCK_DRIVER_PRICE(std::string &text)
{
	const std::string &selectedDriver = VuGameUtil::IF()->getSelectedDriver();
	const VuSpreadsheetAsset *pSA = VuGameUtil::IF()->driverSpreadsheet();
	int rowIndex = VuSpreadsheetQuery::findFirstRow(pSA, VuSpreadsheetQuery::VuStringEqual("Driver", selectedDriver.c_str()));
	int nPrice = -1;
	VuFastDataUtil::getValue(pSA->getField(rowIndex, "UnlockPrice"), nPrice);

	char str[64];
	VuStringUtil::integerFormat(nPrice, str, sizeof(str));

	text += str;
	text += VuStringDB::IF()->getStringSelf("DiamondUnit");
}

//modify by liudan
//*****************************************************************************
void VuGameFontMacrosImpl::handle_FullLevel_CAR_PRICE(std::string &text)
{
	const std::string &selectedCar = VuGameUtil::IF()->getSelectedCar();
	int price = VuGameUtil::IF()->getFullLevelCarCurPrice(selectedCar);
	char str[64];
	VuStringUtil::integerFormat(price, str, sizeof(str));

	text += str;
	text += VuStringDB::IF()->getStringSelf("PriceUnit");
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_UPGRADED_CAR_STAGE(std::string &text)
{
	int stage = VuGameManager::IF()->getUnlockedStage();

	char str[64];
	VU_SPRINTF(str, sizeof(str), "Stage_%d", stage);
	text += VuStringDB::IF()->getString(str);
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_UPGRADED_CAR_PRICE(std::string &text)
{
	const std::string &selectedCar = VuGameUtil::IF()->getSelectedCar();
	int price = VuGameUtil::IF()->getUpgradedCarPrice(selectedCar);

	char str[64];
	VuStringUtil::integerFormat(price, str, sizeof(str));
	text += str;
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_UPGRADED_CAR_SAVINGS(std::string &text)
{
	const std::string &selectedCar = VuGameUtil::IF()->getSelectedCar();
	int discountedPrice = VuGameUtil::IF()->getUpgradedCarPrice(selectedCar);

	int truePrice = 0;
	int rowIndex = VuSpreadsheetQuery::findFirstRow(VuGameUtil::IF()->carSpreadsheet(),
		VuSpreadsheetQuery::VuStringEqual("Car", selectedCar.c_str())
	);
	for ( int i = 0; i < VuGameManager::IF()->getUnlockedStage(); i++ )
	{
		truePrice += VuGameUtil::IF()->carSpreadsheet()->getField(rowIndex + i, "Accel Price").asInt();
		truePrice += VuGameUtil::IF()->carSpreadsheet()->getField(rowIndex + i, "Speed Price").asInt();
		truePrice += VuGameUtil::IF()->carSpreadsheet()->getField(rowIndex + i, "Handling Price").asInt();
		truePrice += VuGameUtil::IF()->carSpreadsheet()->getField(rowIndex + i, "Tough Price").asInt();
	}

	int savings = VuRound(100.0f*(truePrice - discountedPrice)/truePrice);

	char str[64];
	VuStringUtil::integerFormat(savings, str, sizeof(str));
	text += str;
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_DRIVER_NAME(std::string &text)
{
	const std::string &selectedDriver = VuGameUtil::IF()->getSelectedDriver();

	char str[64];
	VU_SPRINTF(str, sizeof(str), "Driver_%s", selectedDriver.c_str());
	text += VuStringDB::IF()->getString(str);
}


//*****************************************************************************
void VuGameFontMacrosImpl::handle_DRIVER_PRICE(std::string &text)
{
	text += "0";
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_DRIVER_DESC(std::string &text)
{
	const std::string &selectedDriver = VuGameUtil::IF()->getSelectedDriver();

	char str[64];
	VU_SPRINTF(str, sizeof(str), "Driver_%s_Desc", selectedDriver.c_str());
	text += VuStringDB::IF()->getString(str);
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_DRIVER_LOCKED_DESC(std::string &text)
{
	const std::string &selectedDriver = VuGameUtil::IF()->getSelectedDriver();

	char str[64];
	VU_SPRINTF(str, sizeof(str), "Driver_%s_Locked_Desc", selectedDriver.c_str());
	text += VuStringDB::IF()->getString(str);
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_DRIVER_ABILITY_NAME(std::string &text)
{
	const std::string &selectedDriver = VuGameUtil::IF()->getSelectedDriver();

	char str[64];
	VU_SPRINTF(str, sizeof(str), "Driver_%s_Ability", selectedDriver.c_str());
	text += VuStringDB::IF()->getString(str);
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_DRIVER_ABILITY_DESC(std::string &text)
{
	const std::string &selectedDriver = VuGameUtil::IF()->getSelectedDriver();

	char str[64];
	VU_SPRINTF(str, sizeof(str), "Driver_%s_Ability_Desc", selectedDriver.c_str());
	text += VuStringDB::IF()->getString(str);
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_POWER_UP_NAME(std::string &text)
{
	const std::string &powerUp = VuGameUtil::IF()->dataRead()["SelectedPowerUp"].asString();

	char str[64];
	VU_SPRINTF(str, sizeof(str), "PowerUp_%s", powerUp.c_str());
	text += VuStringDB::IF()->getString(str);
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_POWER_UP_DESC(std::string &text)
{
	const std::string &powerUp = VuGameUtil::IF()->dataRead()["SelectedPowerUp"].asString();

	char str[64];
	VU_SPRINTF(str, sizeof(str), "PowerUpDesc_%s", powerUp.c_str());
	text += VuStringDB::IF()->getString(str);
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_POWER_UP_PRICE(std::string &text)
{
	const std::string &powerUp = VuGameUtil::IF()->dataRead()["SelectedPowerUp"].asString();
	int price = VuGameUtil::IF()->getPowerUpPrice(powerUp);

	char str[64];
	VuStringUtil::integerFormat(price, str, sizeof(str));
	text += str;
}

//*****************************************************************************
void VuGameFontMacrosImpl::handleUpgradePrice(const char *stat, std::string &text)
{
	const VuGameManager::Car &car = VuGameManager::IF()->getCurCar();
	int nextLevel = car.getStat(stat) + 1;
	int priceSC = VuGameUtil::IF()->getCarUpgradePrice(car.mName, car.mStage, nextLevel, stat);

	char str[64];
	VuStringUtil::integerFormat(priceSC, str, sizeof(str));
	text += str;
}

//modify by liudan
void VuGameFontMacrosImpl::handlePackPrice(const char *packItem, std::string &text)
{
	//const VuGameManager::Car &car = VuGameManager::IF()->getCurCar();
	//int nextLevel = car.getStat(stat) + 1;
	//int priceSC = VuGameUtil::IF()->getCarUpgradePrice(car.mName, car.mStage, nextLevel, stat);
	const VuJsonContainer &itemData = VuDataUtil::findArrayMember(VuGameUtil::IF()->storeDB(), "SkyMobi", packItem);
	std::string itemPrice = itemData["StubPrice"].asString();

	text += itemPrice;
}

//*****************************************************************************
void VuGameFontMacrosImpl::handleReward(int mult, std::string &text)
{
	int amount = 0;

	if ( VuCarManager::IF()->getLocalHumanCarCount() )
	{
		VuCarEntity *pCar = VuCarManager::IF()->getLocalHumanCar(0);
		int place = pCar->getStats().mPlace;

		const std::string &gameType = VuGameUtil::IF()->getEventData()["GameType"].asString();

		if ( gameType == "QuickRace" )
		{
			//int stage = VuGameUtil::IF()->dataRead()["QuickRace"]["Stage"].asInt();
			//amount = VuGameUtil::IF()->getQuickRaceRewardSC(stage, place);
		}
		else if ( gameType == "CarChampRace" )
		{
		}
		else if ( VuGameUtil::IF()->getEventData()["IsChallenge"].asBool() )
		{
			if ( place == 1 )
			{
				amount = VuProfileManager::IF()->dataRead()["DailyChallenge"]["Reward"].asInt();
			}
		}
		else
		{
			if ( place >= 1 && place <= 6 )
			{
				const std::string &eventName = VuGameUtil::IF()->getEventName();

				const VuSpreadsheetAsset *pSA = VuGameUtil::IF()->eventSpreadsheet();
				int rowIndex = VuSpreadsheetQuery::findFirstRow(pSA, VuSpreadsheetQuery::VuStringEqual("Event", eventName.c_str()));

				int columnIndex = pSA->getColumnIndex("SC") + place - 1;

				amount = pSA->getField(rowIndex, columnIndex).asInt();
			}
		}
	}
	else
	{
		// assume it's a CarChamp
		const std::string &carName = VuGameUtil::IF()->dataRead()["CarChamp"]["Car"].asString();
		int stage = VuGameUtil::IF()->dataRead()["CarChamp"]["Stage"].asInt();

		// determine place
		int place = 0;
		VuGameUtil::CarChampTable table;
		VuGameUtil::IF()->createCarChampTable(carName, table);
		for ( int i = 0; i < (int)table.size(); i++ )
			if ( table[i].mPlayer )
				place = i + 1;

		amount = VuGameUtil::IF()->getCarChampRewardSC(stage, place);
	}

	char str[32];
	VuStringUtil::integerFormat(amount*mult, str, sizeof(str));
	text += str;
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_REWARD_FIRST(std::string &text)
{
	const std::string &eventName = VuGameUtil::IF()->getEventName();

	const VuSpreadsheetAsset *pSA = VuGameUtil::IF()->eventSpreadsheet();
	int rowIndex = VuSpreadsheetQuery::findFirstRow(pSA, VuSpreadsheetQuery::VuStringEqual("Event", eventName.c_str()));

	int columnIndex = pSA->getColumnIndex("SC");

	int sc = pSA->getField(rowIndex, "SC").asInt();
	int pc = pSA->getField(rowIndex, "PC").asInt();

	char str[32];
	VuStringUtil::integerFormat(sc + pc, str, sizeof(str));
	text += str;
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_BOSS_NAME(std::string &text)
{
	const std::string &seriesName = VuGameUtil::IF()->getSeriesName();
	const std::string &bossName = VuGameUtil::IF()->getSeriesBossName(seriesName);

	char str[128] = "Driver_";
	VU_STRCAT(str, sizeof(str), bossName.c_str());
	text += VuStringDB::IF()->getString(str);
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_SERIES_REQUIRED_BOSS(std::string &text)
{
	const std::string &seriesName = VuGameUtil::IF()->getSeriesName();
	const std::string &previousSeriesName = VuGameUtil::IF()->getPreviousSeriesName(seriesName);
	const std::string &bossName = VuGameUtil::IF()->getSeriesBossName(previousSeriesName);

	char str[128] = "Driver_";
	VU_STRCAT(str, sizeof(str), bossName.c_str());
	text += VuStringDB::IF()->getString(str);
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_SERIES_REQUIRED_STAGE(std::string &text)
{
	const std::string &seriesName = VuGameUtil::IF()->getSeriesName();
	int stage = VuGameUtil::IF()->seriesDB()[seriesName]["Stage"].asInt();

	char str[64];
	VU_SPRINTF(str, sizeof(str), "Stage_%d", stage);
	text += VuStringDB::IF()->getString(str);
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_TUNE_UPS(std::string &text)
{
	char str[64];
	VuStringUtil::integerFormat(VuGameManager::IF()->getTuneUps(), str, sizeof(str));
	text += str;
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_ACHIEVEMENTS_EARNED(std::string &text)
{
	char str[64];
	VuStringUtil::integerFormat(VuAchievementManager::IF()->unlockedCount(), str, sizeof(str));
	text += str;
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_ACHIEVEMENTS_TOTAL(std::string &text)
{
	char str[64];
	VuStringUtil::integerFormat(VuAchievementManager::IF()->totalCount(), str, sizeof(str));
	text += str;
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_ACHIEVEMENTS_SCORE_EARNED(std::string &text)
{
	char str[64];
	VuStringUtil::integerFormat(VuAchievementManager::IF()->unlockedScore(), str, sizeof(str));
	text += str;
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_ACHIEVEMENTS_SCORE_TOTAL(std::string &text)
{
	char str[64];
	VuStringUtil::integerFormat(VuAchievementManager::IF()->totalScore(), str, sizeof(str));
	text += str;
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_TOAST_TEXT(std::string &text)
{
	if ( VuToastManager::IF() )
	{
		if ( VuToast *pToast = VuToastManager::IF()->getActiveToast() )
			text += pToast->mText;
	}
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_ACHIEVEMENT_TOAST_REWARD(std::string &text)
{
	if ( VuToastManager::IF() )
	{
		if ( VuToast *pToast = VuToastManager::IF()->getActiveToast() )
		{
			if ( pToast->isDerivedFrom(VuAchievementToast::msRTTI) )
			{
				VuAchievementToast *pAchToast = static_cast<VuAchievementToast *>(pToast);

				char str[16];
				VuStringUtil::integerFormat(pAchToast->mReward, str, sizeof(str));
				text += str;
			}
		}
	}
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_MB_HEADING(std::string &text)
{
	if ( VuMessageBox *pMessageBox = VuMessageBoxManager::IF()->getActiveMessageBox() )
		text += VuStringDB::IF()->getStringSelf(pMessageBox->params().mHeading);
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_MB_BODY(std::string &text)
{
	if ( VuMessageBox *pMessageBox = VuMessageBoxManager::IF()->getActiveMessageBox() )
		text += VuStringDB::IF()->getStringSelf(pMessageBox->params().mBody);
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_MB_BUTTON_A(std::string &text)
{
	if ( VuMessageBox *pMessageBox = VuMessageBoxManager::IF()->getActiveMessageBox() )
		text += VuStringDB::IF()->getStringSelf(pMessageBox->params().mTextA);
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_MB_BUTTON_B(std::string &text)
{
	if ( VuMessageBox *pMessageBox = VuMessageBoxManager::IF()->getActiveMessageBox() )
		text += VuStringDB::IF()->getStringSelf(pMessageBox->params().mTextB);
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_BOSS_ABILITY_NAME(std::string &text)
{
	const char *bossName = VuGameUtil::IF()->getSeriesBossName(VuGameUtil::IF()->getSeriesName());

	char str[128];
	VU_SPRINTF(str, sizeof(str), "Driver_%s_Ability", bossName);
	text += VuStringDB::IF()->getString(str);
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_BOSS_ABILITY_DESC(std::string &text)
{
	const char *bossName = VuGameUtil::IF()->getSeriesBossName(VuGameUtil::IF()->getSeriesName());

	char str[128];
	VU_SPRINTF(str, sizeof(str), "Driver_%s_Ability_Desc", bossName);
	text += VuStringDB::IF()->getString(str);
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_BOSS_INTRO(std::string &text)
{
	const char *bossName = VuGameUtil::IF()->getSeriesBossName(VuGameUtil::IF()->getSeriesName());

	char str[128];
	VU_SPRINTF(str, sizeof(str), "Boss_%s_Intro", bossName);
	text += VuStringDB::IF()->getString(str);
}

void VuGameFontMacrosImpl::handleMonsterTruckPackPrice(std::string &text)
{
	bool bDiscount = VuPackManager::IF()->isPackOk(VuPackManager::Pack_LanXiang);

	const VuJsonContainer &itemData = VuDataUtil::findArrayMember(VuGameUtil::IF()->storeDB(), "Name", bDiscount ? "MonsterTruckPackDiscount" : "MonsterTruckPack");
	std::string strNeedMoney = itemData["StubPrice"].asString();
	text += strNeedMoney;
}


//*****************************************************************************
void VuGameFontMacrosImpl::handle_BOSS_WIN(std::string &text)
{
	const char *bossName = VuGameUtil::IF()->getSeriesBossName(VuGameUtil::IF()->getSeriesName());

	char str[128];
	VU_SPRINTF(str, sizeof(str), "Boss_%s_Win", bossName);
	text += VuStringDB::IF()->getString(str);
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_BOSS_LOSE(std::string &text)
{
	const char *bossName = VuGameUtil::IF()->getSeriesBossName(VuGameUtil::IF()->getSeriesName());

	char str[128];
	VU_SPRINTF(str, sizeof(str), "Boss_%s_Lose", bossName);
	text += VuStringDB::IF()->getString(str);
}
//*****************************************************************************
void VuGameFontMacrosImpl::handle_CARCHAMP_EVENT_NUMBER(std::string &text)
{
	const std::string &carName = VuGameUtil::IF()->dataRead()["CarChamp"]["Car"].asString();
	const VuJsonContainer &carChampData = VuProfileManager::IF()->dataRead()["CarChampData"][carName];
	const VuJsonContainer &races = carChampData["Races"];

	int eventNumber = 0;
	int raceCount = races.size();
	for ( int i = 0; i < raceCount; i++ )
		if ( races[i].hasMember("Place") )
			eventNumber++;

	char str[32];
	VU_SPRINTF(str, sizeof(str), "%d", eventNumber);
	text += str;
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_CARCHAMP_NEXT_TRACK_NAME(std::string &text)
{
	const std::string &carName = VuGameUtil::IF()->dataRead()["CarChamp"]["Car"].asString();
	const VuJsonContainer &carChampData = VuProfileManager::IF()->dataRead()["CarChampData"][carName];
	const VuJsonContainer &races = carChampData["Races"];

	int eventNumber = 0;
	int raceCount = races.size();
	for ( int i = 0; i < raceCount; i++ )
		if ( races[i].hasMember("Place") )
			eventNumber++;

	const std::string &track = races[eventNumber]["Track"].asString();

	char str[128];
	VU_SPRINTF(str, sizeof(str), "Track_%s", track.c_str());
	text += VuStringDB::IF()->getString(str);
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_CARCHAMP_NEXT_EVENT_NUMBER(std::string &text)
{
	const std::string &carName = VuGameUtil::IF()->dataRead()["CarChamp"]["Car"].asString();
	const VuJsonContainer &carChampData = VuProfileManager::IF()->dataRead()["CarChampData"][carName];
	const VuJsonContainer &races = carChampData["Races"];

	int eventNumber = 0;
	int raceCount = races.size();
	for ( int i = 0; i < raceCount; i++ )
		if ( races[i].hasMember("Place") )
			eventNumber++;

	char str[32];
	VU_SPRINTF(str, sizeof(str), "%d", eventNumber + 1);
	text += str;
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_CARCHAMP_TOTAL_EVENTS(std::string &text)
{
	const std::string &carName = VuGameUtil::IF()->dataRead()["CarChamp"]["Car"].asString();
	const VuJsonContainer &carChampData = VuProfileManager::IF()->dataRead()["CarChampData"][carName];
	const VuJsonContainer &races = carChampData["Races"];

	char str[32];
	VU_SPRINTF(str, sizeof(str), "%d", races.size());
	text += str;
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_CARCHAMP_STAGE(std::string &text)
{
	int stage = VuGameUtil::IF()->dataRead()["CarChamp"]["Stage"].asInt();

	char str[64];
	VU_SPRINTF(str, sizeof(str), "Stage_%d", stage);
	text += VuStringDB::IF()->getString(str);
}

//*****************************************************************************
//*****************************************************************************
void VuGameFontMacrosImpl::handle_POWER_UP_BONUSB_PRICE(std::string &text)
{
	int price = VuGameUtil::IF()->dataRead()["PowerUpGame"]["BonusBPrice"].asInt();

	char str[64];
	VuStringUtil::integerFormat(price, str, sizeof(str));
	text += str;
}

//*****************************************************************************
void VuGameFontMacrosImpl::handle_POWER_UP_BONUSC_PRICE(std::string &text)
{
	int price = VuGameUtil::IF()->dataRead()["PowerUpGame"]["BonusCPrice"].asInt();

	char str[64];
	VuStringUtil::integerFormat(price, str, sizeof(str));
	text += str;
}
