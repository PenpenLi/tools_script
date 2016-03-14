//*****************************************************************************
//
//  Copyright (c) 2007-2008 Ralf Knoesel
//  Copyright (c) 2008-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  AI Utilities
// 
//*****************************************************************************

#pragma once

#include "VuPurple/AI/VuAiTuningVariables.h"
#include "VuPurple/AI/VuAiInstance.h"
#include "VuPurple/Entities/Car/VuCarStats.h"

class VuAiUtils
{
public:
	static eAiLane getLaneFromPos(const VuVector3& pos);

	static void registerAiForStats(VuAiInstance* pInstance);

	static bool connectToLogServer();
	static void disconnectFromLogServer();

	static void getCurrentTimeString(std::string& timeStr);
	static void getVector3String(const VuVector3& v, std::string& str);

	static void reportEvent(const std::string& eventName, const std::string& data);
	static void reportEvent(VuAiInstance* pInstance, const std::string& eventName, const std::string& data);
	static void reportHotSpotEvent(float avgFrameRate, int drawCalls, int primitiveCount, const VuVector3& cameraPos, const VuVector3& cameraDir);
	static void reportCarStatsEvent(VuAiInstance* pInstance, const std::string& eventName);
	static void reportRecovery(VuAiInstance* pInstance, const VuVector3& prevPos, const VuVector3& curPos);

	static bool inFrontOfMe(VuCarEntity* pMe, VuCarEntity* pHim);

	// Positive he's in front of me, Negative he's behind me in the race
	static float raceDistanceBetween(VuCarEntity* pMe, VuCarEntity* pHim); 

	static void determineGoalPos(VuCarEntity* pCar, const VuVector3& currentPosition, VuVector3& goalPosition, VuVector3& newGoalDirection);

	static void determineControls(VuCarEntity* pCar, 
								const VuVector3 &currentPosition,
								const VuVector3 &currentDirection,
								const VuVector3 &goalPosition,
								float currentSpeed,
								float goalSpeed,
								float fdt,
								float &yawControl,
								float &throttleControl);
};