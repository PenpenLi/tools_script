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
#include "VuEngine/Util/VuFSM.h"


class VuRankManager : public VuSystemComponent
{
	DECLARE_SYSTEM_COMPONENT(VuRankManager)

public:
	//////////////////////////////////////////////////////////////////////////
	enum eStatus{ RETRIEVE_PENDING,RETRIEVE_SUCCESS,RETRIEVE_FAILURE };
	enum RankDataType{CUR_WEEK,LAST_WEEK};
	class VuRankData
	{
	public:
		int mRank;
		VUINT64 mValue;
		std::string mGamePlayer;
	};
	typedef std::vector<VuRankData> RankDatas;

	class VuRankDataWrap
	{
	public:
		RankDatas mRankDatas;
		eStatus mStatus;
		float mTime;
		float mWatingTime;
		VuRankDataWrap() :mStatus(RETRIEVE_PENDING),mTime(0),mWatingTime(0){}
	};

	//////////////////////////////////////////////////////////////////////////
	
	VuRankManager();
	int getCurRank() const {return  mCurRank; }
	int getCurMilestone() const { return mCurMilestone; }
	void submitMilestone(float milestone);
	void tabRankDataType();
	VuRankDataWrap* getRankDatas();
	void startGetPlayerName();
	void	commitTempPlayerName();
	void startGetRank();
	void setRankDataType(RankDataType rankDataType);
protected:
	friend class VuPurple;
	virtual bool	init();
	virtual void postInit();
	virtual void	release();

	void updateData();//hourly refresh

	// FSM
	void			onGetRankDataEnter();
	void			onGetRankDataExit();
	void			onGetRankDataTick(float fdt);

	void			onPostRankDataEnter();
	void			onPostRankDataExit();
	void			onPostRankDataTick(float fdt);


	void			onGetPlayerNameEnter();
	void			onGetPlayerNameExit();
	void			onGetPlayerNameTick(float fdt);

	void			onCommitPlayerNameEnter();
	void			onCommitPlayerNameExit();
	void			onCommitPlayerNameTick(float fdt);
	
private:
	void			tickNetwork(float fdt);
	VUHANDLE mHttpRequest;
	VUHANDLE mGetNameHttpRequest;
	VUHANDLE mCommitNameHttpRequest;
	VuRankDataWrap* mWrap;
	float			maddMillStone;
	bool			mIsNeedUpdate;
	RankDataType mRankDataType;
	VuFSM			mFSM;
	VuFSM			mPlayNameFSM;
	std::string tempUserName;
	int				mCurRank;
	int				mCurMilestone;
	float				mTimeSlip;
};
