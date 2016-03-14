//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  StatsManager class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/VuSystemComponent.h"
#include "VuEngine/Managers/VuProfileManager.h"

class VuCarEntity;


class VuStatsManager : public VuSystemComponent
{
	DECLARE_SYSTEM_COMPONENT(VuStatsManager)

protected:
	// called by game
	friend class VuPurple;
	virtual bool init() { return true; }

public:
	// record results
	void	recordRaceResult(const std::string &eventName, VuCarEntity *pCar);
	void	recordBoostBlitzResult(const std::string &eventName, VuCarEntity *pCar);
	void	recordEliminationResult(const std::string &eventName, VuCarEntity *pCar);
	void	recordShootingGalleryResult(const std::string &eventName, VuCarEntity *pCar);
	void	recordDemolitionDerbyResult(const std::string &eventName, VuCarEntity *pCar);
	void	recordFollowLeaderResult(const std::string &eventName, VuCarEntity *pCar);
	void	recordBossBattleResult(const std::string &eventName, VuCarEntity *pCar);
	void	recordCarChampRaceResult(VuCarEntity *pCar);
	void	recordCarChampResult(const char *carName, int stage, int place);

	// get info
	bool	getBestPlace(const char *eventName, int &place) const { return statsRead()["Events"][eventName]["Place"].getValue(place); }
	bool	getBestTime(const char *eventName, float &time) const { return statsRead()["Events"][eventName]["Time"].getValue(time); }
	bool	getBestDistance(const char *eventName, float &distance) const { return statsRead()["Events"][eventName]["Distance"].getValue(distance); }
	bool	getBestScore(const char *eventName, int &score) const { return statsRead()["Events"][eventName]["Score"].getValue(score); }
	bool	getCarChampBestPlace(const char *carName, int stage, int &place) { return statsRead()["CarChamp"][carName][stage]["Place"].getValue(place); }
	bool    getFailTimes(const char* eventName, int& failTimes) const  {return statsRead()["Events"][eventName]["FailTimes"].getValue(failTimes); }
	bool    getEventTimes(const char* eventName, int &enterTimes) const { return statsRead()["Events"][eventName]["TimesPlayed"].getValue(enterTimes); }

	const std::string	&getHighestEventPlayedName() const { return statsRead()["HighestEventPlayedName"].asString(); }
	int					getHighestEventPlayedIndex() const { return statsRead()["HighestEventPlayedIndex"].asInt(); }

	// misc stats
	float				getFloatStat(const char *stat) { return statsRead()[stat].asFloat(); }
	int					getIntStat(const char *stat) { return statsRead()[stat].asInt(); }
	void				addToFloatStat(const char *stat, float value);
	void				addToIntStat(const char *stat, int value);
	void				recordMaxFloatStat(const char *stat, float value);
	void				recordMaxIntStat(const char *stat, int value);
	void				recordMaxStringStat(const char *stat, const std::string &value);
	int					getBestScore(const char *gameType) const;


	const std::string	&getFavoriteCar();
	const std::string	&getFavoriteDriver();

	void    recordEnterEventTimes(const std::string &eventName);

private:
	const VuJsonContainer	&statsRead() const { return VuProfileManager::IF()->dataRead()["Stats"]; }
	VuJsonContainer			&statsWrite() { return VuProfileManager::IF()->dataWrite()["Stats"]; }


	void recordFailureTimes(VuCarEntity* pCar, const std::string& eventName);
	void	recordTime(VuCarEntity *pCar, const std::string &eventName);
	void	recordDistance(VuCarEntity *pCar, const std::string &eventName);
	void	recordScore(VuCarEntity *pCar, const std::string &eventName);
	void	recordCommon(VuCarEntity *pCar, const std::string &eventName, bool bossBattle = false);
	
};
