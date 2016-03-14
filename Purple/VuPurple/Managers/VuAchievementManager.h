//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  AchievementManager class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/VuSystemComponent.h"
#include "VuEngine/Managers/VuToastManager.h"


class VuAchievementManager : public VuSystemComponent
{
	DECLARE_SYSTEM_COMPONENT(VuAchievementManager)

public:
	virtual bool		init();

	void				setProgress(int achievementId, float progress);
	float				getProgress(int achievementId);
	void				unlock(int achievementId) { setProgress(achievementId, 1.0f); }
	bool				isUnlocked(int achievementId) { return getProgress(achievementId) >= 1.0f; }

	struct VuAchievement
	{
		int	mId;
		int	mPoints;
		int	mPC;
	};
	typedef std::deque<VuAchievement> Achievements;

	const Achievements	&getAchievements() { return mAchievements; }

	int					totalCount() { return (int)mAchievements.size(); }
	int					unlockedCount();

	int					totalScore();
	int					unlockedScore();

protected:
	Achievements		mAchievements;
};


class VuAchievementToast : public VuToast
{
	DECLARE_RTTI
public:
	int		mReward;
};
