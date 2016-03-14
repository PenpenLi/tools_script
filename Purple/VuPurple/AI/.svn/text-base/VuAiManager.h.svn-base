//*****************************************************************************
//
//  Copyright (c) 2009-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  AIManager class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/VuSystemComponent.h"
#include "VuEngine/Math/VuMathUtil.h"
#include "VuEngine/Math/VuUnitConversion.h"
#include "VuEngine/Containers/VuObjectArray.h"
#include "VuEngine/Json/VuJsonContainer.h"

#include "VuPurple/AI/VuAiTuningVariables.h"
#include "VuPurple/AI/VuAiInstance.h"
#include "VuPurple/AI/VuAiPowerUpTracker.h"

class VuCarEntity;
class VuAiWaypointEntity;
class VuMatrix;
class VuTrackSector;
class VuAiDriver;

class VuAiManager : public VuSystemComponent
{
	DECLARE_SYSTEM_COMPONENT(VuAiManager)

protected:
	friend class VuPurple;

	virtual bool init();
	virtual void release();

public:
	VuAiManager();
	~VuAiManager();

	enum eRaceState
	{
		UNINITIALIZED,
		INITIALIZED,
		STAGING,
		RACING,
		ENDED,
	};

	// Called by the game at appropriate times in the sequence
	void		onGameStart();
	void		onRaceStart();
	void		onRaceEnd();

	// Resets the manager to default state
	void		reset();

	void		tickDecision(float fdt);

	// Adding and removing cars from the race
	void			addToRace(VuCarEntity *pCar, float finishTime, float topSpeedMPH);
	void			removeFromRace(VuCarEntity *pCar);
	void			notifyBranchChanges(int sectorCount, const VuTrackSector **apSectors);
	bool			canRace();	// true if the track is built and exists
	bool			isFinishedTraversingSector(const VuVector3 &prevPos, const VuVector3 &curPos, const VuTrackSector *pSector) const;

	void			powerUpWasUsed(const char* pName);
	const char*		getBestPowerUpByGroup(const char* pGroupName);

	void			downTuneAiCar(VuCarEntity* pCar, int numLosses);

	// Accessors
	bool			isRecordDebugEvents() { return mRecordDebugEvents;  }

	VuTrackSector*	getFirstRaceSector() { return mpFirstSector; }
	VuTrackSector*	getLastRaceSector() { return mpLastSector; }

	float			getRaceTimeElapsed() { return mRaceTimeElapsed;  }

	eRaceState		getRaceState() { return mRaceState; }

	VuAiGlobalConstants* getGlobalConstants() { return &mAiGlobalConstants; }

	bool			isWedgeDetectionEnabled() { return mWedgeDetectionEnabled; }


	// Public STATIC accessors for static values exposed to the Tweaker tool at runtime
	static float	getWedgedDeltaThreshold() { return smWedgedDeltaThreshold; }
	static float	getTimeSpentWedgedThreshold() { return smTimeSpentWedgedThreshold; }

	// Debug
	VuJsonContainer &addDebugEvent(const char *pName, const char *pEventName, const VuVector3* pPos = VUNULL);
	int				getNumInstances() { return mAIObjects.size(); }
	VuAiInstance*	getInstance(int index) { return mAIObjects[index]; }

	float			getLastDeathBatTime() { return mLastDeathBatTime; }
	void			setLastDeathBatTime(float deathBatTime) { mLastDeathBatTime = deathBatTime; }

private:
	void			getStartingStats(const VuCarEntity *pCar, int &lapCount, VuAiWaypointEntity **ppNextCheckPoint) const;

	// Called by riders to get information
	float			getMaxSpeedMPH() const;

	// Debug function
	void			moveCarTo(VuCarEntity *pCar, const VuMatrix &matrix);

private:


private:
	typedef VuObjectArray<VuAiInstance *> AIInstances;

	void			adjustCompetition(float fdt);
	void			findCompetitor(float fdt);
	void			assignCompetitor(VuCarEntity *pCar, VuAiDriver *pLastCompetitor);
	void			tickRacing(float fdt);
	void			doCarPlacement();
	void			enableWedgeDetection(bool enable);
	void			determineCarPacks(VuAiInstance* pMyInstance);	// Groups cars into "Behind/Near/Ahead" based on distance travelled in the race

	static bool		carDistSortFunction(VuCarEntity* pOne, VuCarEntity* pTwo);

	// AI Manager state variables
	eRaceState		mRaceState;
	float			mRaceTimeElapsed;

	float			mLastDeathBatTime;

	// Array of AI Instances
	AIInstances		mAIObjects;

	VuTrackSector *	mpFirstSector;
	VuTrackSector *	mpLastSector;

	bool			mWedgeDetectionEnabled;

	VuAiGlobalConstants mAiGlobalConstants;
	VuAiPowerUpTracker	mPowerUpTracker;

	// Debug Methods
	void			updateStatPage() const;
	void			drawTrack() const;
	void			writeDebugEvents();
	void			renderDebugEvents(const char *pName);

	VuJsonContainer	mDebugEvents;
	bool			mRecordDebugEvents;
	bool			mRenderDebugEvents;
	bool			mLoadDebugEvents;

	// Debug Variables
	static float	smMaxSpeed;

	static float	smMinSimRadius;
	static float	smMaxSimRadius;

	static float	smWedgedDeltaThreshold;
	static float	smTimeSpentWedgedThreshold;
	static bool		smDebugWaypoints;
	static bool		smForceDrive;
};