//*****************************************************************************
//
//  Copyright (c) 2007-2008 Ralf Knoesel
//  Copyright (c) 2008-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  AI Utilities
// 
//*****************************************************************************
#include "VuEngine/Properties/VuProperty.h"
#include "VuEngine/HAL/Net/VuNet.h"
#include "VuEngine/Water/VuFluidsObject.h"

#include "VuPurple/AI/VuAiManager.h"
#include "VuPurple/AI/VuAiBrain.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuPurple/Track/VuTrackManager.h"
#include "VuPurple/Track/VuTrackSector.h"
#include "VuPurple/Modes/VuGameModeManager.h"
#include "VuPurple/Entities/Car/Driver/VuAiDriverEntity.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarChassis.h"

#include "VuAiUtils.h"

VuTcpSocket* spSocket = VUNULL;

//*****************************************************************************
eAiLane VuAiUtils::getLaneFromPos(const VuVector3& pos)
{
	const VuTrackSector* pMySector = VuTrackManager::IF()->findSectorFromPosition(pos);
	VUASSERT(pMySector, "This shouldn't be NULL anywhere that getLaneFromPos() gets called!");
	if (pMySector == NULL)
	{
		return LANE_CENTER;
	}

	float myTraversalAmount = pMySector->traversalAmount(pos);

	// Where am I on the racing line?
	VuVector3 myRacingLinePoint;
	pMySector->pointAlongRacingLine(myTraversalAmount, myRacingLinePoint);

	// Vector from center racing line to me. Should be perpendicular to racing line
	VuVector3 toMeUnitDir = (pos - myRacingLinePoint).normal();
	float dist = VuDist(pos, myRacingLinePoint);

	// Get perpendicular to unit vector of racing line
	VuVector3 racingLinePerp = VuCross(VuVector3(0.0f, 0.0f, 1.0f), pMySector->mUnitDir);

	float dot = VuDot(racingLinePerp, toMeUnitDir);
	if (dot < 0.0f)
	{
		if (dist > AI_LANE_WIDTH)
		{
			return LANE_RIGHT;
		}
		else
		{
			return LANE_CENTER;
		}
	}
	else
	{
		if (dist > AI_LANE_WIDTH)
		{
			return LANE_LEFT;
		}
		else
		{
			return LANE_CENTER;
		}
	}
}

//*****************************************************************************
bool VuAiUtils::connectToLogServer()
{
	spSocket = VuTcpSocket::create(0);
	if (spSocket && spSocket->connect("10.1.10.20", 12345, 5000))
	{
		spSocket->setNonBlocking(true);
	}
	else
	{
		delete spSocket;
		spSocket = VUNULL;
		return false;
	}

	return true;
}

//*****************************************************************************
void VuAiUtils::disconnectFromLogServer()
{
	if (spSocket)
	{
		spSocket->disconnect();
		delete spSocket;
		spSocket = VUNULL;
	}
}

//*****************************************************************************
void VuAiUtils::getCurrentTimeString(std::string& timeString)
{
	time_t time_create = time(0);
	struct tm time_info;
	VU_LOCALTIME(&time_create, &time_info);

	char tempStr[64];
	VU_SPRINTF(tempStr, 64, "%0.2d-%0.2d-%0.2d_%0.2d:%0.2d:%0.2d", time_info.tm_mon, time_info.tm_mday, time_info.tm_year, time_info.tm_hour, time_info.tm_min, time_info.tm_sec);

	timeString = tempStr;
}

//*****************************************************************************
void VuAiUtils::getVector3String(const VuVector3& v, std::string& str)
{
	char tempStr[64];
	VU_SNPRINTF(tempStr, 64, 64, "[%#.4f,%#.4f,%#.4f]", v.mX, v.mY, v.mZ);
	str = tempStr;
}

//*****************************************************************************
void VuAiUtils::reportEvent(const std::string& eventName, const std::string& data)
{
	if ( spSocket == VUNULL )
		return;

	std::string timeStr;
	getCurrentTimeString(timeStr);

	const std::string& gameEventName = VuGameUtil::IF()->getEventName();

	char logLine[1024];
	VU_SPRINTF(logLine, 1024, "{ \"Name\" : \"%s\", \"Data\" : \"%s\", \"Event\" : \"%s\", \"Time\": \"%s\" }, \n",
		eventName.c_str(),
		data.c_str(),
		gameEventName.c_str(),
		timeStr.c_str()
		);

	spSocket->send(logLine, (int)strlen(logLine));
}

//*****************************************************************************
void VuAiUtils::reportEvent(VuAiInstance* pInstance, const std::string& eventName, const std::string& data)
{
	if ( spSocket == VUNULL )
		return;

	if (!pInstance || !pInstance->mpCar || !pInstance->mpBrain || !pInstance->getAiTuningVariables())
	{
		return;
	}
	
	const std::string& gameEventName = VuGameUtil::IF()->getEventName();

	const std::string &trackName = VuGameUtil::IF()->getTrack();

	const std::string& opponent = pInstance->getCar()->getOpponentName();

	std::string timeStr;
	getCurrentTimeString(timeStr);

	char logLine[1024];
	VU_SPRINTF(logLine, 1024, "{ \"Name\" : \"%s\", \"Data\" : \"%s\", \"Car\" : \"%s\", \"Driver\" : \"%s\", \"Event\" : \"%s\", \"Stage\": %d, \"Skill\": %d, \"Opponent\" : \"%s\", \"Brain\" : \"%s\", \"Personality\" : \"%s\", \"Track\": \"%s\", \"Time\": \"%s\"}, \n",
		eventName.c_str(),
		data.c_str(),
		pInstance->getCar()->getCarName().c_str(),
		pInstance->getCar()->getDriverName().c_str(),
		gameEventName.c_str(),
		pInstance->getCar()->getCarStage(),
		pInstance->getAiTuningVariables()->mAiSkill,
		opponent.c_str(),
		pInstance->getAiBrain()->getAiBrainName().c_str(),
		pInstance->getAiTuningVariables()->mAiPersonalityName.c_str(),
		trackName.c_str(),
		timeStr.c_str()
		);

	spSocket->send(logLine, (int)strlen(logLine));

//	VUPRINTF(logLine);
}

//*****************************************************************************
void VuAiUtils::reportHotSpotEvent(float avgFrameRate, int drawCalls, int primitiveCount, const VuVector3& cameraPos, const VuVector3& cameraDir)
{
	if ( spSocket == VUNULL )
		return;

	std::string timeStr;
	getCurrentTimeString(timeStr);

	const std::string &trackName = VuGameUtil::IF()->getTrack();

	std::string cameraPosStr;
	getVector3String(cameraPos, cameraPosStr);

	std::string cameraDirStr;
	getVector3String(cameraDir, cameraDirStr);

	char logLine[1024];
	VU_SPRINTF(logLine, 1024, "{ \"Name\" : \"HotSpot\", \"Track\" : \"%s\", \"FrameRate\" : %#.3f, \"DrawCalls\" : %ld, \"PrimitiveCount\" : %ld, \"CameraPos\" : %s, \"CameraDir\" : %s, \"Time\": \"%s\" }, \n",
		trackName.c_str(),
		avgFrameRate,
		drawCalls,
		primitiveCount,
		cameraPosStr.c_str(),
		cameraDirStr.c_str(),
		timeStr.c_str()
		);

	spSocket->send(logLine, (int)strlen(logLine));
}


//*****************************************************************************
void VuAiUtils::reportCarStatsEvent(VuAiInstance* pInstance, const std::string& eventName)
{
	if ( spSocket == VUNULL )
		return;

	if (!pInstance || !pInstance->mpCar || !pInstance->mpBrain || !pInstance->getAiTuningVariables())
	{
		return;
	}

	const std::string &trackName = VuGameUtil::IF()->getTrack();
	const std::string& opponent = pInstance->getCar()->getOpponentName();

	std::string timeStr;
	getCurrentTimeString(timeStr);

	char logLine[1024];
	VU_SPRINTF(logLine, 1024, "{ \"Name\" : \"CarStats\", \"Data\" : \"%s\", \"Car\" : \"%s\", \"Driver\" : \"%s\", \"Stage\": %d, \"Skill\": %d, \"Opponent\" : \"%s\", \"Brain\" : \"%s\", \"Personality\" : \"%s\", \"Track\": \"%s\", \"Throttle\": %#.4f, \"Steer\": %#.4f, \"Skid\": %s, \"Pack\": %d, \"DesiredPack\": %d, \"Place\": %d, \"Time\": \"%s\"}, \n",
		eventName.c_str(),
		pInstance->getCar()->getCarName().c_str(),
		pInstance->getCar()->getDriverName().c_str(),
		pInstance->getCar()->getCarStage(),
		pInstance->getAiTuningVariables()->mAiSkill,
		opponent.c_str(),
		pInstance->getAiBrain()->getAiBrainName().c_str(),
		pInstance->getAiTuningVariables()->mAiPersonalityName.c_str(),
		trackName.c_str(),
		pInstance->mpCar->getThrottleControl(),
		pInstance->mpCar->getYawControl(),
		pInstance->mIsSkidding ? "true" : "false",
		(int)pInstance->getCarPack(pInstance->mpCar),
		(int)pInstance->getAiTuningVariables()->mDesiredCarPack,
		pInstance->mpCar->getStats().mPlace,
		timeStr.c_str()
		);

	spSocket->send(logLine, (int)strlen(logLine));

	//	VUPRINTF(logLine);
}

//*****************************************************************************
void VuAiUtils::reportRecovery(VuAiInstance* pInstance, const VuVector3& prevPos, const VuVector3& curPos)
{
	if ( spSocket == VUNULL )
		return;

	if (!pInstance || !pInstance->mpCar || !pInstance->mpBrain || !pInstance->getAiTuningVariables())
	{
		return;
	}

	const std::string &trackName = VuGameUtil::IF()->getTrack();

	const std::string& opponent = pInstance->getCar()->getOpponentName();

	std::string timeStr;
	getCurrentTimeString(timeStr);

	std::string prevPosStr;
	getVector3String(prevPos, prevPosStr);

	std::string curPosStr;
	getVector3String(curPos, curPosStr);

	char logLine[1024];
	VU_SPRINTF(logLine, 1024, "{ \"Name\" : \"Recovery\", \"Data\" : \"true\", \"Car\" : \"%s\", \"Driver\" : \"%s\", \"Stage\": %d, \"Skill\": %d, \"Opponent\" : \"%s\", \"Track\": \"%s\", \"RecoverFrom\": %s, \"RecoverTo\": %s, \"Time\": \"%s\"}, \n",
		pInstance->getCar()->getCarName().c_str(),
		pInstance->getCar()->getDriverName().c_str(),
		pInstance->getCar()->getCarStage(),
		pInstance->getAiTuningVariables()->mAiSkill,
		opponent.c_str(),
		trackName.c_str(),
		prevPosStr.c_str(),
		curPosStr.c_str(),
		timeStr.c_str()
		);

	spSocket->send(logLine, (int)strlen(logLine));
}

//*****************************************************************************
bool VuAiUtils::inFrontOfMe(VuCarEntity* pMe, VuCarEntity* pHim)
{
	VUASSERT(pMe, "Invalid car passed to VuAiUtils::inFrontOfMe()");

	if (!pHim)
	{
		return false;
	}

	return pMe->getStats().mDistFromStart < pHim->getStats().mDistFromStart;
}

//*****************************************************************************
float VuAiUtils::raceDistanceBetween(VuCarEntity* pMe, VuCarEntity* pHim)
{
	VUASSERT(pMe, "Invalid car passed to VuAiUtils::inFrontOfMe()");

	if (!pHim)
	{
		return 0.0f;
	}

	return pHim->getStats().mDistFromStart - pMe->getStats().mDistFromStart;
}

//*****************************************************************************
void VuAiUtils::determineGoalPos(VuCarEntity* pCar, const VuVector3& currentPosition, VuVector3& newGoalPosition, VuVector3& newGoalDirection)
{
	VuTrackSector* pCurSector = VuTrackManager::IF()->findSectorFromPosition(currentPosition);
	float tCur = pCurSector->traversalAmount(currentPosition);

	VuTrackPlan plan(8);
	plan.createPlan(pCar, pCurSector);

	// reduce look ahead distance based on how far from best line we are
	float lookAheadDist = LOOKAHEAD_DISTANCE;
	{
		VuVector3 linePos;
		pCurSector->pointAlongRacingLine(tCur, linePos);

		float lineWidth = VuLerp(pCurSector->mEnterWidth, pCurSector->mExitWidth, tCur);
		float ratio = VuDist2d(currentPosition, linePos) / lineWidth;
		ratio = (ratio - LOOKAHEAD_CORRECTION_RATIO) / (1.0f - LOOKAHEAD_CORRECTION_RATIO);
		ratio = VuClamp(ratio, 0.0f, 1.0f);
		lookAheadDist = VuLerp(LOOKAHEAD_DISTANCE, LOOKAHEAD_DISTANCE_OUT, ratio);
	}

	float sectorLength = pCurSector->mLength;

	VuVector3 sectorUnitDir = pCurSector->mUnitDir;

	// calculate distance to end of current sector
	float distToCurSectorExit = (1.0f - tCur)*sectorLength;

	// if distance to end of current sector is greater than look ahead distance, we're done
	if (distToCurSectorExit > lookAheadDist)
	{
		pCurSector->pointAlongRacingLine(tCur, newGoalPosition);
		newGoalPosition += lookAheadDist*sectorUnitDir;
		newGoalDirection = sectorUnitDir;
	}
	else
	{
		// determine distance remaining
		float distanceRemaining = lookAheadDist - distToCurSectorExit;

		// Clean up the line so that it's clipped to the inside or outside corner
		// of the track (string pulling)
		bool interFound = false;
		for (int goalStep = 1; goalStep < plan.getStepCount(); goalStep++)
		{
			const VuTrackSector *pGoalSector = plan.getSectorForStep(goalStep);

			// collide goal sector racing line against all preceding portals
			VuVector2 enterPos(&pGoalSector->mEnterPoint.mX);
			VuVector2 exitPos(&pGoalSector->mExitPoint.mX);
			VuVector2 curPos(currentPosition.mX, currentPosition.mY);
			float t = 1.0f;
			for (int portalStep = 1; portalStep <= goalStep; portalStep++)
			{
				const VuTrackSector *pPortalSector = plan.getSectorForStep(portalStep);

				VuVector2 leftPos(&pPortalSector->mEnterLeft.mX);
				VuVector2 rightPos(&pPortalSector->mEnterRight.mX);

				leftPos = curPos + 1000.0f*(leftPos - curPos).normal();
				rightPos = curPos + 1000.0f*(rightPos - curPos).normal();

				VuVector2 inter;
				if (VuMathUtil::lineSegLineSegIntersection2d(enterPos, exitPos, curPos, leftPos, inter) ||
					VuMathUtil::lineSegLineSegIntersection2d(enterPos, exitPos, curPos, rightPos, inter))
				{
					t = VuMin(t, (inter - enterPos).mag() / (exitPos - enterPos).mag());
				}
			}

			float goalSectorLength = pGoalSector->mLength;
			VuVector3 goalSectorUnitDir = pGoalSector->mUnitDir;

			// make sure we respect a minimum look ahead distance
			t = VuMax(t, (MIN_LOOKAHEAD_DISTANCE - (lookAheadDist - distanceRemaining)) / goalSectorLength);

			// clamp t based on remaining look ahead distance
			t = VuMin(t, distanceRemaining / goalSectorLength);

			if (t < 1.0f)
			{
				pGoalSector->pointAlongRacingLine(t, newGoalPosition);
				newGoalDirection = goalSectorUnitDir;
				break;
			}

			newGoalPosition = pGoalSector->mExitPoint;
			newGoalDirection = goalSectorUnitDir;

			distanceRemaining -= goalSectorLength;
		}
	}
}

//*****************************************************************************
void VuAiUtils::determineControls(VuCarEntity* pCar,
									const VuVector3 &currentPosition,
									const VuVector3 &currentDirection,
									const VuVector3 &goalPosition,
									float currentSpeed,
									float goalSpeed,
									float fdt,
									float &yawControl,
									float &throttleControl)
{
	if (fdt > 0.0f)
	{
		VuVector3 currentPosition2D(currentPosition.mX, currentPosition.mY, 0.0f);
		VuVector3 currentDirection2D(currentDirection.mX, currentDirection.mY, 0.0f);
		VuVector3 goalPos2D(goalPosition.mX, goalPosition.mY, 0.0f);

		VuVector3 linVel = pCar->getLinearVelocity();
		if (linVel.mag2dSquared() > 5.0f && pCar->getFluidsObject()->getSubmergedVolume() > 0.0f)
		{
			currentDirection2D = VuVector3(linVel.mX, linVel.mY, 0.0f).normal();
		}

		// Yaw Control
		VuVector3 toGoal = goalPos2D - currentPosition2D;
		toGoal.normalize();

		// anticipate using angular velocity
		float angVelZ = pCar->getAngularVelocity().mZ;
		currentDirection2D = VuMatrix::rotationZ(angVelZ*STEERING_ANTICIPATION_TIME).transformNormal(currentDirection2D);

		float cpz = VuCross(currentDirection2D, toGoal).mZ;

		yawControl = -cpz * 4.0f;
		yawControl = VuClamp(yawControl, -1.0f, 1.0f);

		// Throttle Control
		if (goalSpeed > currentSpeed)
		{
			throttleControl = 1.0f;
		}
		else if (goalSpeed < 0.75f * currentSpeed)
		{
			throttleControl = -1.0f;
		}
		else
		{
			throttleControl = 0.0f;
		}
	}
}

