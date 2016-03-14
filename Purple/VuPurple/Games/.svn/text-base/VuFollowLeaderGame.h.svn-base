//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  FollowLeaderGame class
// 
//*****************************************************************************

#pragma once

#include "VuGame.h"

class VuGhostRecorder;
class VuGhostCarEntity;


class VuFollowLeaderGame : public VuGame
{
	DECLARE_RTTI

public:
	VuFollowLeaderGame(VuProject *pProject);
	~VuFollowLeaderGame();

private:
	virtual void	onLoad(const VuJsonContainer &data);
	virtual void	onPreBegin();
	virtual void	onEnd();
	virtual void	onCarFinished(VuCarEntity *pCar);

	// event handlers
	void			OnRecovered(const VuParams& params);
	void			OnBreadCrumb(const VuParams& params);

	void			onIntroEnter();
	void			onIntroExit();
	void			onIntroTick(float fdt);

	void			onPreGameEnter();
	void			onPreGameExit();
	void			onPreGameTick(float fdt);

	void			onGameEnter();
	void			onGameExit();
	void			onGameTick(float fdt);

	void			onPostGameEnter();
	void			onPostGameExit();

	void			onRecordEnter();
	void			onRecordExit();
	void			onRecordTick(float fdt);

	VuGhostRecorder		*mpRecorder;
	VuGhostCarEntity	*mpGhostCar;

	int				mBreadCrumbCount;
	int				mBreadCrumbDelay;
	int				mBreadCrumbsDropped;

	float			mTimeFactor;
	float			mMaintainDistFactor;
	float			mMaintainDistRange0;
	float			mMaintainDistRange1;
	float			mTimeOutTime;
	float			mCountdownTime;
	float			mTimeSinceLastCrumb;
};