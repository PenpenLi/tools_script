//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Game class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Objects/VuRTTI.h"
#include "VuEngine/Events/VuEventMap.h"
#include "VuEngine/Containers/VuArray.h"
#include "VuEngine/HAL/Keyboard/VuKeyboard.h"
#include "VuEngine/Math/VuMatrix.h"
#include "VuEngine/Math/VuVector2.h"
#include "VuEngine/Util/VuFSM.h"
#include "VuEngine/Util/VuStringFormat.h"
#include "VuEngine/Json/VuJsonContainer.h"

#include "VuPurple/Util/VuGameMetrics.h"
#include "VuPurple/Managers/VuPackManager.h"

class VuProject;
class VuJsonContainer;
class VuCarEntity;
class VuAiWaypointEntity;
class VuHUDEntity;


class VuGame: VuKeyboard::Callback
{
	DECLARE_RTTI
	DECLARE_EVENT_MAP

public:
	VuGame(VuProject *pProject);
	virtual ~VuGame();

	void				load(const VuJsonContainer &data);
	void				begin();
	void				end();
	bool				tick(float fdt);
	void				draw();

	// For AI Debugging
	bool				allCarsFinished();

protected:
	// event handlers
	void				FinishGame(const VuParams &params);

	// VuKeyboard::Callback
	virtual void		onKeyDown(VUUINT32 key);

	virtual int			getNumOpponents() { return 5; }
	virtual void		onLoad(const VuJsonContainer &data) {}
	virtual void		onPreBegin() {}
	virtual void		onPostBegin() {}
	virtual void		onEnd() {}
	virtual void		onTick(float fdt) {}
	virtual void		onCheckpointCrossed(VuCarEntity *pCar, VuAiWaypointEntity *pCP) {}
	virtual void		onCarFinished(VuCarEntity *pCar) {}

	void				updatePreGameTiming(float preGameTime);
	void				updateTiming(float fdt);
	void				updateDistances(float fdt);
	void				updateCheckPoints();

	void				setCarFinished(VuCarEntity *pCar);

	void				loadHUD(const char *name, const char *assetName);
	void				useHUD(const char *name);
	VuHUDEntity			*getCurHUDEntity();
	VuProject			*getCurHUDProject();


	typedef VuArray<VuCarEntity *> Cars;

	VuFSM				mGameFSM;
	VuProject			*mpProject;
	Cars				mCars;
	float				mTrackLength;
	std::string			mSeriesName;
	std::string			mEventName;
	int					mLapCount;
	std::string			mTrackName;
	std::string			mGameType;
	bool				mIsChallenge;
	double				mPreGameTime;
	bool				mStartLightsOut;

	typedef std::map<std::string, VuProject *> HUDs;
	HUDs				mHUDs;
	std::string			mCurHUD;

#ifdef ENABLE_VU_GAME_METRICS
	VuGameMetrics		mGameMetrics;

	friend class VuGameMetrics;
#endif
};