//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  LeaderboardManager class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/VuSystemComponent.h"


class VuLeaderboardManager : public VuSystemComponent
{
	DECLARE_SYSTEM_COMPONENT(VuLeaderboardManager)

public:
	VuLeaderboardManager();

	virtual bool	init();
	virtual void	release();

	void			submitScore(const std::string &lbName, VUINT64 value);
	void			clearCache(const std::string &lbName);

	class VuScore
	{
	public:
		int			mRank;
		VUINT64		mValue;
		std::string	mGamerID;
		std::string	mGamerTag;
	};
	typedef std::vector<VuScore> Scores;
	enum eStatus { RETRIEVE_PENDING, RETRIEVE_SUCCESS, RETRIEVE_FAILURE };

	VUHANDLE		createQuery(const std::string &lbName);
	void			releaseQuery(VUHANDLE hQuery);
	eStatus			getStatus(VUHANDLE hQuery);
	const Scores	&getScores(VUHANDLE hQuery);

private:
	void			tickNetwork(float fdt);

	class VuQuery;
	typedef std::deque<VuQuery *> Queries;
	Queries		mQueries;

	VuQuery		*mpActiveQuery;
	VUHANDLE	mHttpRequest;
};
