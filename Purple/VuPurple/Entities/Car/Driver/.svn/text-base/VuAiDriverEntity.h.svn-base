//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  AiDriver class
// 
//*****************************************************************************

#pragma once

#include "VuDriverEntity.h"
#include "VuPurple/Track/VuTrackPlan.h"
#include "VuEngine/Math/VuVector3.h"
#include "VuPurple/AI/VuAiTuningVariables.h"
#include "VuPurple/AI/VuAiInstance.h"
#include "VuEngine/Objects/VuRefObj.h"

#define PLAN_DEPTH						(8)
#define LOOKAHEAD_DISTANCE				(75.0f)
#define LOOKAHEAD_DISTANCE_OUT			(25.0f)
#define MIN_LOOKAHEAD_DISTANCE			(10.0f)
#define LOOKAHEAD_CORRECTION_RATIO		(0.5f)
#define STEERING_ANTICIPATION_TIME		(0.25f)
#define ACOS_KEEPAHEAD_ANGLE			(0.866025404f)

#define MIN_VERTICAL_COLLISION_ANGLE	(0.707f)
#define MAX_OBSTACLE_RAYCASTS			(2)
#define MAX_OBSTACLE_AVOIDANCE_RADIUS	(40.0f)
#define OBSTACLE_AVOIDANCE_CLOSURE_RATE (10.0f)

#define YAW_SMOOTH_TIME					0.2f
#define THROTTLE_SMOOTH_TIME			0.2f

#define PLAYER_TARGET_ADJ_TIME_MIN		2.0f
#define PLAYER_TARGET_ADJ_TIME_MAX		4.0f

#define PLAYER_TARGET_ADJ_DIST_MIN		-5.0f
#define PLAYER_TARGET_ADJ_DIST_MAX		10.0f

#define RECOVER_TIMER					2.0f // s

class VuTrackSector;
class VuDevStatPage;
class VuTrackPlan;
class VuAiDriverHints;
class VuAiTuningVariables;

class VuAiDriver : public VuDriverEntity
{
	DECLARE_RTTI

public:

	enum eGoalLine { GOAL_LINE_AI, GOAL_LINE_PRO, GOAL_LINE_SHORTCUT };

	VuAiDriver(VuCarEntity *pCar);
	virtual ~VuAiDriver();

	virtual void				onGameInitialize();
	virtual void				onGameRelease();

	virtual bool				isHuman()	{ return false; }
	virtual bool				isAi()		{ return true; }
	virtual bool				isLocal()	{ return true; }
	virtual bool				isRemote()	{ return false; }
	virtual bool				isUi()		{ return false; }

	virtual void				dataModified(const std::string &driverName);
	virtual void				onLoad(const VuJsonContainer &data);
	virtual void				onTickDecision(float fdt);
	virtual void				onTeleported();
	virtual void				onRecover();
	virtual void				setFinished();
	virtual void				notifyOfEffect(const std::string& name, const VuCarEffect* pEffect);

	virtual const VuTrackSector	*getCurrentSector() const;

	virtual float				getAnimationThrottleControl() const;

	const VuTrackSector			*updatePlan();
	void						replan(const VuTrackSector *pStartingSector);
	VuTrackPlan					&getPlan() { return mPlan; }
	const VuTrackPlan			*getCurrentPlan() const;
	void						updateHints(const VuAiDriverHints &hints);

	void						setGoalLineToFollow(eGoalLine line) { mGoalLine = line; }
	eGoalLine					getGoalLineFollowing() { return mGoalLine; }

	VuAiTuningVariables*		getAiTuningVariables(bool getRawData = false);
	VuAiInstance*				getAiInstance() { return mpAiInstance.get(); }
	void						setAiInstance(VuAiInstance* pInstance) { mpAiInstance = pInstance; }

	float						getCurrentSpeed() const	{ return mCurrentSpeed; }

	// Targeting
	void						setTargetOpponent(VuCarEntity *pTarget)	{ mpTargetOpponent = pTarget; }		// Assigned by the AI Manager when a suitable target is found
	VuCarEntity					*getTargetOpponent()					{ return mpTargetOpponent; }		// 
	float						getPlayerTargetDist() const { return mPlayerTargetDist; }

	const VuMatrix&				getPreviousModelMatrix() { return mPreviousModelMatrix; }

	void						setSkill(eSkill skill)	{ mSkill = skill; }
	eSkill						getSkill() const		{ return mSkill; }

	// Return the name of the Brain for this AI driver
	std::string					getBrainName() { return getAiTuningVariables()->mAiBrainName; }

	// Control Modifiers
	void						addControlModifierGoalPosition(const VuVector3& goalPos);
	void						addControlModifierGoalSpeed(const float goalSpeed);
	void						addControlModifierDirectStickControl(const float yaw, const float throttle);

	// Debug
	void						updateStatPage(VuDevStatPage *pPage) const;

private:
	// Event handler for start lights and boosting at start
	void					OnStartLightSequence(const VuParams& params);

	const VuVector3&		getEnterPoint(const VuTrackSector* pSector);
	const VuVector3&		getExitPoint(const VuTrackSector* pSector);
	float					getSectorLength(const VuTrackSector* pSector);
	void					getPointAlongRacingLine(const VuTrackSector* pSector, float t, VuVector3& point);
	void					getSectorUnitDir(const VuTrackSector* pSector, VuVector3& outDir);

	void					determineGoalTrackPosition(const VuVector3 &currentPosition, 
												   VuVector3 &newGoalPosition, 
												   VuVector3 &newGoalDirection);

	void					determineObstacles(const VuVector3 &currentPosition, 
										   VuVector3 &goalPosition,
										   float fdt);

	void					determineControls(const VuVector3 &currentPosition, 
										  const VuVector3 &currentDirection, 
										  const VuVector3 &goalPosition, 
										  float currentSpeed, 
										  float goalSpeed,
										  float fdt,
										  float &yawControl,
										  float &throttleControl);

	enum eModifierType
	{
		MODIFIER_NONE,

		MODIFIER_SPEED,
		MODIFIER_GOAL_POS,
		
		MODIFIER_DIRECT_STICK_CONTROL,

		MODIFIER_MAX
	};

	struct VuAiDrivingControlModifier
	{
		VuAiDrivingControlModifier() : mType(MODIFIER_NONE), mSpeed(0.0f), mYaw(0.0f), mThrottle(0.0f) {}

		eModifierType	mType;
		float			mSpeed;
		VuVector3		mGoalPos;
		float			mYaw;
		float			mThrottle;
	};

	typedef std::vector<VuAiDrivingControlModifier> ModifierList;

	ModifierList			mDrivingControlModifiers;

	// Iterates through driving control modifiers and applies them
	void					applyDrivingControlModifiers();

	// Direct stick control
	void					applyDirectStickControlModifiers(float& throttle, float&yaw);

	// Set the stick and throttle for the car
	void					setControls(float fdt, float yawControl, float throttleControl);

	// Instance Variables
	//
	eSkill					mSkill;
	VuTrackPlan				mPlan;
	eGoalLine				mGoalLine;

	float					mCarRadiusXY;
	float					mSpeedHint;
	float					mCurrentSpeed;
	VuVector3				mGoalPosition;
	float					mGoalSpeed;
	VuCarEntity				*mpTargetOpponent;
	VuMatrix				mPreviousModelMatrix;

	float					mYawVel;
	float					mThrottleVel;

	float					mPlayerTargetDist;
	float					mPlayerTargetTimer;

	VuWeakRef<VuAiInstance>	mpAiInstance;
	VuAiTuningVariables*	mpAiTuningVariables;
};