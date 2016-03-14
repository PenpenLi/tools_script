//*****************************************************************************
//
//  Copyright (c) 2012-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Class and utilities for metrics
// 
//*****************************************************************************

#pragma once

// Undefine to remove game metrics code from game
#ifndef VURETAIL
//	#define ENABLE_VU_GAME_METRICS
#endif

#ifdef ENABLE_VU_GAME_METRICS

#include "VuEngine/Events/VuEventMap.h"

class VuCarEntity;
class VuGame;
class VuTcpSocket;
class VuGameMetrics
{
	DECLARE_EVENT_MAP

public:
	VuGameMetrics();
	virtual ~VuGameMetrics();

	void	onGameStart(VuGame* pGame);
	void	onGameEnd();
	void	onGameTick(float fdt);

	// Reporting Events
	void	reportEvent(const std::string& eventName, const std::string& data);
	void	reportEvent(VuCarEntity* pCar, const std::string& eventName, const std::string& data);
	void	reportCarStatsEvent(VuCarEntity* pCar, const std::string& eventData);
	void	reportRecovery(VuCarEntity* pCar, const VuVector3& prevPos, const VuVector3& curPos);

private:
	bool	connect();
	void	disconnect();

	void getVector3String(const VuVector3& v, std::string& str);
	void getCurrentTimeString(std::string& timeString);
	void getDriverType(VuCarEntity* pCar, std::string& driverTypeStr);

	// Event Handlers for tracking game happenings
	void OnRecovered(const VuParams& params);
	void OnTokenUsed(const VuParams& params);
	void OnTokenAdded(const VuParams& params);
	void OnTokenRefill(const VuParams& params);

	// vars
	bool	mEnabled;
	VuGame* mpGame;
	float	mLastLogTime;
	float	mLogsPerSecond;
	VuTcpSocket* mpSocket;
	std::string mUserIdentifier;
	std::string mGameType;

};

#endif
