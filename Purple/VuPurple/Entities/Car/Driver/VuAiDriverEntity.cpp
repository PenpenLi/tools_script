//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  AiDriver class
// 
//*****************************************************************************

#include "VuAiDriverEntity.h"

#include "VuEngine/Water/VuFluidsObject.h"
#include "VuEngine/Math/VuMatrix.h"
#include "VuEngine/Math/VuUnitConversion.h"
#include "VuEngine/Math/VuRand.h"
#include "VuEngine/Math/VuMathUtil.h"
#include "VuEngine/DB/VuStringDB.h"
#include "VuEngine/Util/VuColor.h"
#include "VuEngine/Util/VuDataUtil.h"
#include "VuEngine/Dev/VuDev.h"
#include "VuEngine/Dev/VuDevStat.h"
#include "VuEngine/Dev/VuDevMenu.h"
#include "VuEngine/Dev/VuDevConfig.h"
#include "VuEngine/Dev/VuDevUtil.h"
#include "VuEngine/Managers/VuTickManager.h"

#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Effects/VuCarEffect.h"
#include "VuPurple/Entities/Car/Parts/VuCarEngine.h"
#include "VuPurple/Entities/Car/Parts/VuCarPowerUpController.h"
#include "VuPurple/Entities/Car/Parts/VuCarEffectController.h"
#include "VuPurple/Track/VuTrackSector.h"
#include "VuPurple/Track/VuTrackManager.h"
#include "VuPurple/Dynamics/VuCollisionTypes.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Util/VuGameUtil.h"

#include "VuPurple/AI/VuAiTuningVariables.h"
#include "VuPurple/AI/VuAiManager.h"
#include "VuPurple/AI/VuAiDriverHint.h"
#include "VuPurple/AI/VuAiRayTest.h"
#include "VuPurple/AI/VuAiUtils.h"

IMPLEMENT_RTTI(VuAiDriver, VuDriverEntity);

static bool sDebugDraw = false;

//*****************************************************************************
VuAiDriver::VuAiDriver(VuCarEntity *pCar):
	VuDriverEntity(pCar),
	mSkill(SKILL_NORMAL),
	mPlan(PLAN_DEPTH),
	mGoalLine(GOAL_LINE_AI),
	mCarRadiusXY(0.0f),
	mSpeedHint(0.0f),
	mCurrentSpeed(0.0f),
	mGoalPosition(0,0,0),
	mGoalSpeed(0.0f),
	mpTargetOpponent(VUNULL),
	mYawVel(0.0f),
	mThrottleVel(0.0f),
	mPlayerTargetDist(0.0f),
	mPlayerTargetTimer(0.0f),
	mpAiTuningVariables(VUNULL),
	mpAiInstance(VUNULL)
{
	mName = "Ai Driver";

	static VuDevBoolOnce sOnce;
	
	if ( VuDevConfig::IF() )
	{
		VuDevConfig::IF()->getParam("DebugDrivers").getValue(sDebugDraw);
	}
	if(sOnce && VuDevMenu::IF())
	{
		VuDevMenu::IF()->addBool("AI/Debug/Drivers", sDebugDraw);
	}

	// 
	mDrivingControlModifiers.clear();
	mDrivingControlModifiers.reserve(16);
}

//*****************************************************************************
VuAiDriver::~VuAiDriver()
{
	if (mpAiTuningVariables)
	{
		delete mpAiTuningVariables;
		mpAiTuningVariables = VUNULL;
	}
}

//*****************************************************************************
void VuAiDriver::onGameInitialize()
{
	VuDriverEntity::onGameInitialize();

	REG_EVENT_HANDLER(VuAiDriver, OnStartLightSequence);
}

//*****************************************************************************
void VuAiDriver::onGameRelease()
{
	mEventMap.unregisterHandlers();

	VuDriverEntity::onGameRelease();
}

//*****************************************************************************
void VuAiDriver::dataModified(const std::string &driverName)
{
	VuDriverEntity::dataModified(driverName);

	const VuVector3 &extents = mpCar->getCollisionAabb().getExtents();
	mCarRadiusXY = VuMax(extents.mX, extents.mY);

	mName = VuStringDB::IF()->getString("Driver_" + driverName).c_str();
}

//*****************************************************************************
void VuAiDriver::onLoad(const VuJsonContainer &data)
{
	VuDriverEntity::onLoad(data);

	if (mpAiTuningVariables == VUNULL)
	{
		mpAiTuningVariables = new VuAiTuningVariables();
		if (mpAiTuningVariables)
		{
			const VuJsonContainer& aiData = data["Ai"];
			const char* pPersonalityName = VUNULL;

			if (VuGameUtil::IF()->getEventData()["IsChallenge"].asBool())
			{
				pPersonalityName = "Challenge";
			}

			mpAiTuningVariables->onLoad(aiData, VUNULL, pPersonalityName);
		}
	}
}

//*****************************************************************************
void VuAiDriver::onTickDecision(float fdt)
{
	VuDriverEntity::onTickDecision(fdt);

	if (mpAiInstance)
	{
		// Update Statuses and tick Brain/Behaviors
		mpAiInstance->tickDecision(fdt);
	}

	VuMatrix modelMatrix = mpCar->getModelMatrix();
	modelMatrix.translateLocal(mpCar->getCollisionAabb().getCenter());
	float d = VuDot(modelMatrix.getAxisY(), mpCar->getLinearVelocity());
	float sign = d >= 0.0f ? 1.0f : -1.0f;
	mCurrentSpeed = mpCar->getLinearVelocity().mag() * sign;

	const VuVector3 &currentPosition = modelMatrix.getTrans();
	const VuVector3 &currentDirection = modelMatrix.getAxisY();

	float yawControl = 0.0f;
	float throttleControl = 0.0f;
	bool powerUpControl = false;

	if ( !isRagdollActive() )
	{
		const VuTrackSector *pCurrentSector = mPlan.getSectorForStep(0);
		if (pCurrentSector)
		{
			if (pCurrentSector->traversalAmount(currentPosition) >= 1.0f)
			{
				pCurrentSector = updatePlan();
			}
		}

		if(pCurrentSector)
		{
			// Determine where to go
			VuVector3 newGoalDirection;

			determineGoalTrackPosition(currentPosition, mGoalPosition, newGoalDirection);
			
			determineObstacles(currentPosition, mGoalPosition, fdt);

			// Determine speed/boost
			mGoalSpeed = VuMin(mSpeedHint, mpCar->getMaxForwardSpeed());

			// Track towards target
			if(mpTargetOpponent)
			{
				VuVector3 toTarget = (mpTargetOpponent->getModelPosition() - currentPosition).normal();
				float ahead = VuDot(toTarget, pCurrentSector->mUnitDir);
				if ( ahead > 0 )
				{
					// Target is ahead of us, gun it
					mGoalSpeed = FLT_MAX;
				}
			}

			// Last chance to modify goal position, or goal speed
			applyDrivingControlModifiers();
			
			// Determine inputs
			VuAiUtils::determineControls(mpCar, 
								currentPosition, 
								currentDirection, 
								mGoalPosition, 
								mCurrentSpeed, 
								mGoalSpeed, 
								fdt,
								yawControl, 
								throttleControl);
		}
	}

	applyDirectStickControlModifiers(yawControl, throttleControl);

	// Set stick and trigger inputs
	//
	setControls(fdt, yawControl, throttleControl);

	// Clear modifier list after controls have been set for this frame
	mDrivingControlModifiers.clear();

	// player dist
	//
	{
		mPlayerTargetTimer -= fdt;
		if ( mPlayerTargetTimer < 0.0f )
		{
			mPlayerTargetTimer = VuRand::global().range(PLAYER_TARGET_ADJ_TIME_MIN, PLAYER_TARGET_ADJ_TIME_MAX);
			mPlayerTargetDist = VuRand::global().range(PLAYER_TARGET_ADJ_DIST_MIN, PLAYER_TARGET_ADJ_DIST_MAX);
		}
	}

	if ( sDebugDraw )
	{
		// debug draw track plan
		if (mpCar == VuCarManager::IF()->getCameraTargetForViewport(0))
			for ( int i = 0; i < VuMin(mPlan.getStepCount(), 4); i++ )
				mPlan.getSectorForStep(i)->debugRender(VuColor(128, 128, 255));
	}

	mPreviousModelMatrix = mpCar->getModelMatrix();
}

//*****************************************************************************
void VuAiDriver::onTeleported()
{
	VuTrackSector *pSector = VuTrackManager::IF()->findSectorFromPosition(mpCar->getModelPosition());

	//if (!pSector->pointInside(mpCar->getModelPosition()))
	//{
	//	VUPRINTF("VuAiDriver::onTeleported() the new point isn't inside the new sector.\n");
	//}

	replan(pSector);

	VuCarStats &stats = mpCar->getStats();
	stats.mCurrentSection = pSector->mSection;
	stats.mpNextCheckPoint = VuTrackManager::IF()->getNextCheckPoint(pSector->mSection);
}

//*****************************************************************************
void VuAiDriver::addControlModifierGoalPosition(const VuVector3& goalPos)
{
	VuAiDrivingControlModifier modifier;

	modifier.mType = MODIFIER_GOAL_POS;
	modifier.mGoalPos = goalPos;

	mDrivingControlModifiers.push_back(modifier);
}

//*****************************************************************************
void VuAiDriver::addControlModifierGoalSpeed(const float goalSpeed)
{
	VuAiDrivingControlModifier modifier;

	modifier.mType = MODIFIER_SPEED;
	modifier.mSpeed = goalSpeed;

	mDrivingControlModifiers.push_back(modifier);
}

//*****************************************************************************
void VuAiDriver::addControlModifierDirectStickControl(const float yaw, const float throttle)
{
	VuAiDrivingControlModifier modifier;

	modifier.mType = MODIFIER_DIRECT_STICK_CONTROL;
	modifier.mYaw = yaw;
	modifier.mThrottle = throttle;

	mDrivingControlModifiers.push_back(modifier);
}

//*****************************************************************************
void VuAiDriver::onRecover()
{
	VuDriverEntity::onRecover();

	if (mpAiInstance)
	{
		const VuVector3 pos = mpCar->getModelPosition();
		const VuVector3 prevPos = getPreviousModelMatrix().getTrans();

		VuAiUtils::reportRecovery(mpAiInstance.get(), prevPos, pos);

		if (getSkill() >= SKILL_NORMAL && mpCar->getCarStage() == 3)
		{
			VuCarEntity* pHumanCar = VUNULL;
			if (VuCarManager::IF()->getLocalHumanCarCount() > 0)
			{
				pHumanCar = VuCarManager::IF()->getLocalHumanCar(0);
			}

			float distanceToHuman = VuAiUtils::raceDistanceBetween(mpAiInstance->getCar(), pHumanCar);

			if (pHumanCar)
			{
				float immediateBendiness = mpAiInstance->getAiDriver()->getPlan().computeBendinessAtTime(3.0f); // seconds of lookahead at current speed

				if (distanceToHuman > 60.0f &&
					immediateBendiness < mpAiInstance->getAiTuningVariables()->mAiBoostBendiness)
				{
					mpAiInstance->usePowerup("Boost");

					VUPRINTF("RECOVERBOOST!\n");
				}
			}
		}
	}
}

//*****************************************************************************
void VuAiDriver::setFinished()
{
	VuDriverEntity::setFinished();

	if (mpAiInstance)
	{
		// Won/Lost
		VuAiUtils::reportEvent(mpAiInstance.get(), "Won", (mpCar->getStats().mPlace == 1) ? "true" : "false");

		// Final Place
		char s[64];
		VU_SPRINTF(s, 64, "%d", mpCar->getStats().mPlace);
		VuAiUtils::reportEvent(mpAiInstance.get(), "FinalPlace", s);

		VU_SPRINTF(s, 64, "%#.2f", mpCar->getStats().mTotalTime);
		VuAiUtils::reportEvent(mpAiInstance.get(), "FinalTime", s);
	}
}

//*****************************************************************************
void VuAiDriver::notifyOfEffect(const std::string& name, const VuCarEffect* pEffect)
{
	// Called when a CarEffect is applied to this driver's car
	//
	if (pEffect && pEffect->mpOriginator)
	{
		VuAiUtils::reportEvent(mpAiInstance.get(), "CarEffectApplied", name);
	}
}

//*****************************************************************************
const VuTrackSector *VuAiDriver::getCurrentSector() const
{
	if (VuAiManager::IF()->getRaceState() != VuAiManager::RACING)
	{
		return VUNULL;
	}

	return mPlan.getSectorForStep(0);
}

//*****************************************************************************
float VuAiDriver::getAnimationThrottleControl() const
{
	return getAudioThrottleControl();
}

//*****************************************************************************
const VuTrackSector *VuAiDriver::updatePlan()
{
	if (!mpAiInstance)
	{
		return VUNULL;
	}

	mPlan.update();

	const VuPlanSectorInfo* pSectorInfo = mPlan.getSectorInfoForStep(1);
	if (pSectorInfo)
	{
		mpAiInstance->mBlockedLane = pSectorInfo->mBlockedLane;
	}

	return mPlan.getSectorForStep(0);
}

//*****************************************************************************
void VuAiDriver::replan(const VuTrackSector *pStartingSector)
{
	if (!mpAiInstance)
	{
		return;
	}

	mPlan.createPlan(mpCar, pStartingSector);

	const VuPlanSectorInfo* pSectorInfo = mPlan.getSectorInfoForStep(1);
	if (pSectorInfo)
	{
		mpAiInstance->mBlockedLane = pSectorInfo->mBlockedLane;
	}
}

//*****************************************************************************
const VuTrackPlan *VuAiDriver::getCurrentPlan() const
{
	return &mPlan;
}

//*****************************************************************************
void VuAiDriver::updateHints(const VuAiDriverHints &hints)
{
	mSpeedHint = hints.mSpeed;
}

//*****************************************************************************
VuAiTuningVariables* VuAiDriver::getAiTuningVariables(bool getRawData)
{
	// Default to base attributes
	VuAiTuningVariables* pVars = mpAiTuningVariables;

	if (getRawData)
	{
		return pVars;
	}

	// See what part of the race we're in
	eAiRaceProgress progress = PROGRESS_EARLY_RACE;
	if (mpAiInstance != VUNULL)
	{
		progress = mpAiInstance->getRaceProgress();
	}

	// Pass back a pointer to the data for the race progress
	switch (progress)
	{
	case PROGRESS_EARLY_RACE: if (mpAiTuningVariables->mpRaceScriptEarly != VUNULL) pVars = mpAiTuningVariables->mpRaceScriptEarly; break;
	case PROGRESS_MID_RACE:if (mpAiTuningVariables->mpRaceScriptMid != VUNULL) pVars = mpAiTuningVariables->mpRaceScriptMid; break;
	case PROGRESS_LATE_RACE:if (mpAiTuningVariables->mpRaceScriptLate != VUNULL) pVars = mpAiTuningVariables->mpRaceScriptLate; break;
	}

	VUASSERT(pVars, "This value can't be NULL. Ever. Something has gone wrong with the load sequencing.");

	return pVars;
}

//*****************************************************************************
void VuAiDriver::updateStatPage(VuDevStatPage *pPage) const
{
	VUASSERT(pPage, "NULL page passed to VuAiDriver::updateStatPage");

	pPage->printf("Target/Hinted/Current Speed: %0.1f / %0.1f / %0.1f\n", VuMetersPerSecondToMph(mGoalSpeed), VuMetersPerSecondToMph(mSpeedHint), VuMetersPerSecondToMph(mCurrentSpeed));
}

//*****************************************************************************
void VuAiDriver::OnStartLightSequence(const VuParams& params)
{
	VuParams::VuAccessor accessor(params);

	const char* pEventType = accessor.getString();
	if (pEventType)
	{
		if (!strcmp(pEventType, "BoostPoint"))
		{	
			// Some formula for boosting that gives higher skill values an easier time
			float normalizedSkill = (float)getSkill() / (float)SKILL_MAX;

			// And higher class cars boost more
			float normalizedStage = (float)mpCar->getCarStage() / 4.0f;

			float boostThreshhold = (normalizedSkill + normalizedStage) * 0.5f;

			// Cap at 95% (some small chance to not boost, even if we're a stud)
			boostThreshhold = VuMin(0.95f, boostThreshhold);

			// Always let the boss boost off the line
			if (mpAiInstance.get() && mpAiInstance->mGameType == "BossBattle")
			{
				boostThreshhold = 1.0f;
			}

			if (VuRand::global().rand() < boostThreshhold)
			{
				mpCar->getEffectController()->applyEffect("BoostStart", mpCar);

				VuAiUtils::reportEvent(getAiInstance(), "BoostStart", "true");
			}
			else
			{
				VuAiUtils::reportEvent(getAiInstance(), "BoostStart", "false");
			}
		}
	}
}

//*****************************************************************************
void VuAiDriver::getPointAlongRacingLine(const VuTrackSector* pSector, float t, VuVector3& point)
{
	switch (mGoalLine)
	{
	case GOAL_LINE_AI: pSector->pointAlongRacingLine(t, point); return;
//	case GOAL_LINE_PRO: pSector->pointAlongProLine(t, point); return;
//	case GOAL_LINE_SHORTCUT: pSector->pointAlongShortcutLine(t, point); return;
	}

	pSector->pointAlongRacingLine(t, point);
}

//*****************************************************************************
const VuVector3& VuAiDriver::getEnterPoint(const VuTrackSector* pSector)
{
	switch (mGoalLine)
	{
	case GOAL_LINE_AI: return pSector->mEnterPoint;
//	case GOAL_LINE_PRO: return pSector->mEnterPointProLine;
//	case GOAL_LINE_SHORTCUT: return pSector->mEnterPointShortcutLine;
	}

	return pSector->mEnterPoint;
}

//*****************************************************************************
const VuVector3& VuAiDriver::getExitPoint(const VuTrackSector* pSector)
{
	switch (mGoalLine)
	{
	case GOAL_LINE_AI: return pSector->mExitPoint;
//	case GOAL_LINE_PRO: return pSector->mExitPointProLine;
//	case GOAL_LINE_SHORTCUT: return pSector->mExitPointShortcutLine;
	}

	return pSector->mExitPoint;
}

//*****************************************************************************
float VuAiDriver::getSectorLength(const VuTrackSector* pSector)
{
	switch (mGoalLine)
	{
	case GOAL_LINE_AI: return pSector->mLength;
//	case GOAL_LINE_PRO: return (pSector->mExitPointProLine - pSector->mEnterPointProLine).mag2d();
//	case GOAL_LINE_SHORTCUT: return (pSector->mExitPointShortcutLine - pSector->mEnterPointProLine).mag2d();
	}

	return pSector->mLength;
}

//*****************************************************************************
void VuAiDriver::getSectorUnitDir(const VuTrackSector* pSector, VuVector3& outDir)
{
	outDir = pSector->mUnitDir;

	switch (mGoalLine)
	{
	case GOAL_LINE_AI: outDir = pSector->mUnitDir; break;
//	case GOAL_LINE_PRO: outDir = (pSector->mExitPointProLine - pSector->mEnterPointProLine).normal(); break;
//	case GOAL_LINE_SHORTCUT: outDir = (pSector->mExitPointShortcutLine - pSector->mEnterPointShortcutLine).normal(); break;
	}
}

//*****************************************************************************
void VuAiDriver::determineGoalTrackPosition(const VuVector3 &currentPosition,
											VuVector3 &newGoalPosition, 
											VuVector3 &newGoalDirection)
{
	mGoalLine = GOAL_LINE_AI;

	const VuTrackSector *pCurSector = mPlan.getSectorForStep(0);
	float tCur = pCurSector->traversalAmount(currentPosition);

	if (tCur < FLT_EPSILON)
	{
		int i = 0; 
		i++;
	}

	// reduce look ahead distance based on how far from best line we are
	float lookAheadDist = LOOKAHEAD_DISTANCE;
	{
		VuVector3 linePos;
		getPointAlongRacingLine(pCurSector, tCur, linePos);

		float lineWidth = VuLerp(pCurSector->mEnterWidth, pCurSector->mExitWidth, tCur);
		float ratio = VuDist2d(currentPosition, linePos)/lineWidth;
		ratio = (ratio - LOOKAHEAD_CORRECTION_RATIO)/(1.0f - LOOKAHEAD_CORRECTION_RATIO);
		ratio = VuClamp(ratio, 0.0f, 1.0f);
		lookAheadDist = VuLerp(LOOKAHEAD_DISTANCE, LOOKAHEAD_DISTANCE_OUT, ratio);
	}

	float sectorLength = getSectorLength(pCurSector);

	VuVector3 sectorUnitDir;
	getSectorUnitDir(pCurSector, sectorUnitDir);

	// calculate distance to end of current sector
	float distToCurSectorExit = (1.0f - tCur)*sectorLength;

	// if distance to end of current sector is greater than look ahead distance, we're done
	if ( distToCurSectorExit > lookAheadDist )
	{
		getPointAlongRacingLine(pCurSector, tCur, newGoalPosition);

		if (sDebugDraw) VuDev::IF()->drawSphere(newGoalPosition, 1.0f, VuColor(55, 55, 255), 5, 5);

		newGoalPosition += lookAheadDist*sectorUnitDir;

		if (sDebugDraw) VuDev::IF()->drawSphere(newGoalPosition, 1.0f, VuColor(0, 255, 0), 5, 5);

		newGoalDirection = sectorUnitDir;
	}
	else
	{
		// determine distance remaining
		float distanceRemaining = lookAheadDist - distToCurSectorExit;

		// Clean up the line so that it's clipped to the inside or outside corner
		// of the track (string pulling)
		bool interFound = false;
		for ( int goalStep = 1; goalStep < mPlan.getStepCount(); goalStep++ )
		{
			const VuTrackSector *pGoalSector = mPlan.getSectorForStep(goalStep);

			// collide goal sector racing line against all preceding portals
			VuVector2 enterPos(&getEnterPoint(pGoalSector).mX);
			VuVector2 exitPos(&getExitPoint(pGoalSector).mX);
			VuVector2 curPos(currentPosition.mX, currentPosition.mY);
			float t = 1.0f;
			for ( int portalStep = 1; portalStep <= goalStep; portalStep++ )
			{
				const VuTrackSector *pPortalSector = mPlan.getSectorForStep(portalStep);

				VuVector2 leftPos(&pPortalSector->mEnterLeft.mX);
				VuVector2 rightPos(&pPortalSector->mEnterRight.mX);

				leftPos = curPos + 1000.0f*(leftPos - curPos).normal();
				rightPos = curPos + 1000.0f*(rightPos - curPos).normal();

				if (sDebugDraw && mpCar == VuCarManager::IF()->getCameraTargetForViewport(0))
				{
					VuVector3 left(leftPos.mX, leftPos.mY, pPortalSector->mEnterLeft.mZ);
					VuVector3 right(rightPos.mX, rightPos.mY, pPortalSector->mEnterRight.mZ);
					VuVector3 cur(curPos.mX, curPos.mY, currentPosition.mZ);

					VuDev::IF()->drawLine(cur, left, VuColor(255, 255, 0));
					VuDev::IF()->drawLine(cur, right, VuColor(255, 200, 55));
				}

				VuVector2 inter;
				if ( VuMathUtil::lineSegLineSegIntersection2d(enterPos, exitPos, curPos, leftPos, inter) ||
					 VuMathUtil::lineSegLineSegIntersection2d(enterPos, exitPos, curPos, rightPos, inter) )
				{
					t = VuMin(t, (inter - enterPos).mag()/(exitPos - enterPos).mag());

					if (sDebugDraw && mpCar == VuCarManager::IF()->getCameraTargetForViewport(0))
					{
						VuVector3 cur(inter.mX, inter.mY, pPortalSector->mEnterPoint.mZ);
						VuDev::IF()->drawSphere(cur, 1.0f, VuColor(255, 55, 55), 5, 5);
					}
				}
			}

			float goalSectorLength = getSectorLength(pGoalSector);
			VuVector3 goalSectorUnitDir;

			getSectorUnitDir(pGoalSector, goalSectorUnitDir);
			// make sure we respect a minimum look ahead distance
			t = VuMax(t, (MIN_LOOKAHEAD_DISTANCE - (lookAheadDist - distanceRemaining)) / goalSectorLength);

			// clamp t based on remaining look ahead distance
			t = VuMin(t, distanceRemaining / goalSectorLength);

			if ( t < 1.0f )
			{
				getPointAlongRacingLine(pGoalSector, t, newGoalPosition);
				newGoalDirection = goalSectorUnitDir;
				break;
			}

			newGoalPosition = getExitPoint(pGoalSector);
			newGoalDirection = goalSectorUnitDir;
			
			distanceRemaining -= goalSectorLength;
		}
	}
}

//*****************************************************************************
void VuAiDriver::determineObstacles(const VuVector3 &currentPosition, VuVector3 &goalPosition, float fdt)
{
	const VuVector3 up(0.0f, 0.0f, 1.0f);

	// Check for obstacles between us and our newly chosen goal position
	VUUINT32 collisionMask = COL_GAME_CAR;
	VUUINT32 extRejectMask = EXT_COL_GAME_RAMP;
	VuAiRayTestResult result(mpCar->getRigidBody(), collisionMask, extRejectMask);

	VuVector3 testPosition(goalPosition.mX, goalPosition.mY, currentPosition.mZ);//newGoalPosition.mZ);//

	VuDynamicsRayTest::test(currentPosition, testPosition, result);

	if(result.mbHasHit)
	{
		int numCasts = 0;

		do
		{
			float obstacleRadiusXY = FLT_MAX;
			VuVector3 obstaclePosition(0.0f, 0.0f, 0.0f);

			if(result.mpRigidBody)
			{
				btVector3 rbMin, rbMax;
				result.mpRigidBody->getAabb(rbMin, rbMax);

				VuVector3 min, max;

				min = VuDynamicsUtil::toVuVector3(rbMin);
				max = VuDynamicsUtil::toVuVector3(rbMax);

				VuVector3 extents = (max - min) * 0.5f;
				
				obstacleRadiusXY = VuMax(extents.mX, extents.mY);
				obstaclePosition = min + extents;
			}

			// Are we actually closing on this obstacle?
			VuVector3 hitPos = currentPosition + (result.mHitFraction * (testPosition - currentPosition));

			VuVector3 otherLinearVel = VuDynamicsUtil::toVuVector3(result.mpRigidBody->getLinearVelocity());
			VuVector3 closingVector = otherLinearVel - mpCar->getLinearVelocity();

			//if(closingVector.mag() > OBSTACLE_AVOIDANCE_CLOSURE_RATE)
			{
				// If this obstacle is small enough, try and go around it
				if(obstacleRadiusXY <= MAX_OBSTACLE_AVOIDANCE_RADIUS)
				{
					if(sDebugDraw)
					{
						VuDev::IF()->drawArc(obstaclePosition,
											 VuVector3(0.0f, 0.0f, 1.0f),
											 VuVector3(1.0f, 0.0f, 0.0f),
											 VU_2PI, obstacleRadiusXY, 8, VuColor(255, 255, 255));
					}

					// Pick the way around the obstacle that is most in line with our current velocity
					VuVector3 toward = (currentPosition - obstaclePosition).normal();
					VuVector3 right = VuCross(toward, up).normal();
					
					VuVector3 a = obstaclePosition + right * (obstacleRadiusXY + mCarRadiusXY);
					VuVector3 b = obstaclePosition - right * (obstacleRadiusXY + mCarRadiusXY);

					VuVector3 toA = (a - currentPosition).normal();
					VuVector3 toB = (b - currentPosition).normal();
					
					VuVector3 dir = mpCar->getLinearVelocity().normal();

					float dotA = VuDot(toA, dir);
					float dotB = VuDot(toB, dir);

					VuVector3 newPosition = dotA > dotB ? a : b;

					//goalPosition = dotA > dotB ? a : b;

					goalPosition = VuLerp(goalPosition, newPosition, 0.33f);
				}
				else 
				{
					// Else if this obstacle is too large, slide along it until
					// hopefully we get a clear line of sight to our goal again

					// If we hit something with a normal horizontal enough

					float dotUp = VuDot(result.mHitNormal, up);
					
					if(VuAbs(dotUp) < MIN_VERTICAL_COLLISION_ANGLE)
					{
						hitPos += (currentPosition - testPosition).normal() * mCarRadiusXY;

						if (sDebugDraw && mpCar == VuCarManager::IF()->getCameraTargetForViewport(0)) VuDev::IF()->drawLine(hitPos, hitPos + result.mHitNormal, VuColor(255, 0, 255));
						if (sDebugDraw && mpCar == VuCarManager::IF()->getCameraTargetForViewport(0)) VuDev::IF()->drawLine(currentPosition, hitPos, VuColor(255, 0, 0));

						float d = VuDot(testPosition - hitPos, result.mHitNormal);
						
						goalPosition = testPosition - d * result.mHitNormal;
					}
				}
			}

			result.mbHasHit = false;

			testPosition = goalPosition;

			VuDynamicsRayTest::test(currentPosition, testPosition, result);

		} while(result.mbHasHit && numCasts++ < MAX_OBSTACLE_RAYCASTS);

		if (sDebugDraw && mpCar == VuCarManager::IF()->getCameraTargetForViewport(0)) VuDev::IF()->drawLine(currentPosition, goalPosition, VuColor(0, 0, 255));
	}
}

//*****************************************************************************
void VuAiDriver::determineControls(const VuVector3 &currentPosition, 
								   const VuVector3 &currentDirection, 
								   const VuVector3 &goalPosition, 
								   float currentSpeed, 
								   float goalSpeed,
								   float fdt,
								   float &yawControl,
								   float &throttleControl)
{
	if(sDebugDraw)
		VuDev::IF()->drawLine(currentPosition, goalPosition, VuColor(0, 255, 0));

	if(fdt > 0.0f)
	{
		VuVector3 currentPosition2D(currentPosition.mX, currentPosition.mY, 0.0f);
		VuVector3 currentDirection2D(currentDirection.mX, currentDirection.mY, 0.0f);
		VuVector3 goalPos2D(goalPosition.mX, goalPosition.mY, 0.0f);

		VuVector3 linVel = mpCar->getLinearVelocity();
		if ( linVel.mag2dSquared() > 5.0f && mpCar->getFluidsObject()->getSubmergedVolume() > 0.0f )
			currentDirection2D = VuVector3(linVel.mX, linVel.mY, 0.0f).normal();

		// Yaw Control
		VuVector3 toGoal = goalPos2D - currentPosition2D;
		toGoal.normalize();

		// anticipate using angular velocity
		float angVelZ = mpCar->getAngularVelocity().mZ;
		currentDirection2D = VuMatrix::rotationZ(angVelZ*STEERING_ANTICIPATION_TIME).transformNormal(currentDirection2D);
		
		float cpz = VuCross(currentDirection2D, toGoal).mZ;

		yawControl = -cpz * 4.0f;
		yawControl = VuClamp(yawControl, -1.0f, 1.0f);

		// Throttle Control
		if(goalSpeed > currentSpeed)
		{
			throttleControl = 1.0f;
		}
		else if(goalSpeed < 0.75f * currentSpeed)
		{
			throttleControl = -1.0f;
		}
		else
		{
			throttleControl = 0.0f;
		}
	}
}

//*****************************************************************************
void VuAiDriver::applyDrivingControlModifiers()
{
	if (mDrivingControlModifiers.size() == 0)
	{
		return;
	}

	for (auto modifier : mDrivingControlModifiers)
	{
		switch (modifier.mType)
		{
		case MODIFIER_SPEED:
			mGoalSpeed = modifier.mSpeed;
			break;
		case MODIFIER_GOAL_POS:
			mGoalPosition = modifier.mGoalPos;
			break;
		}
	}
}

//*****************************************************************************
void VuAiDriver::applyDirectStickControlModifiers(float& yaw, float& throttle)
{
	if (mDrivingControlModifiers.size() == 0)
	{
		return;
	}

	float localyaw = 0.0f;
	float localthrottle = 0.0f;
	
	int numModifiersApplied = 0;
	for (auto modifier : mDrivingControlModifiers)
	{
		switch (modifier.mType)
		{
		case MODIFIER_DIRECT_STICK_CONTROL:
			localyaw += modifier.mYaw;
			localthrottle += modifier.mThrottle;
			numModifiersApplied++;
			break;
		}
	}

	if (numModifiersApplied > 0)
	{
		yaw = localyaw / numModifiersApplied;
		throttle = localthrottle / numModifiersApplied;
	}
}

//*****************************************************************************
void VuAiDriver::setControls(float fdt, float yawControl, float throttleControl)
{
	// damping
	float yawValue = mpCar->getRawYawControl();
	yawValue = VuMathUtil::smoothCD(yawValue, yawControl, mYawVel, YAW_SMOOTH_TIME, fdt);

	float throttleValue = mpCar->getThrottleControl();
	throttleValue = VuMathUtil::smoothCD(throttleValue, throttleControl, mThrottleVel, THROTTLE_SMOOTH_TIME, fdt);

	mpCar->setYawControl(yawValue);
	mpCar->setThrottleControl(throttleValue);
}

