//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuLeaderboardManager
// 
//*****************************************************************************

#include "VuRankManager.h"
#include "VuGameServicesManager.h"
#include "VuEngine/Objects/VuRefObj.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Net/VuHttpClient.h"
#include "VuEngine/Json/VuJsonContainer.h"
#include "VuEngine/Json/VuJsonReader.h"
#include "VuEngine/Json/VuJsonWriter.h"
#include "../Util/VuGameFontMacros.h"
#include "VuGameManager.h"


#define BASE_URL "http://bbr.miyigame.com:8057/main_bbr"
//#define BASE_URL "http://172.16.3.205/main_bbr"
//#define BASE_URL "http://localhost:8080/leaderboards"

#define CACHE_TIME 60.0* 60 // seconds
#define WAITING_TIME 20.0f//seconds


// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuRankManager, VuRankManager);

//*****************************************************************************
VuRankManager::VuRankManager() :mHttpRequest(VUNULL), 
mGetNameHttpRequest(VUNULL),
mIsNeedUpdate(true), 
maddMillStone(0), 
mRankDataType(CUR_WEEK),
mCurRank(0),
mCurMilestone(0),
mTimeSlip(0)
{
	mWrap = new VuRankDataWrap;
	tempUserName = VuGameManager::IF()->getTempPlayerName();
}

//*****************************************************************************
bool VuRankManager::init()
{
	return true;
}
//*****************************************************************************
void VuRankManager::postInit()
{
	// create FSM
	{
		VuFSM::VuState *pState = mFSM.addState("GetRankData");
		pState->setEnterMethod(this, &VuRankManager::onGetRankDataEnter);
		pState->setExitMethod(this, &VuRankManager::onGetRankDataExit);
		pState->setTickMethod(this, &VuRankManager::onGetRankDataTick);

		pState = mFSM.addState("PostRankData");
		pState->setEnterMethod(this, &VuRankManager::onPostRankDataEnter);
		pState->setExitMethod(this, &VuRankManager::onPostRankDataExit);
		pState->setTickMethod(this, &VuRankManager::onPostRankDataTick);

		pState = mFSM.addState("Idle");

		//transition
		mFSM.addTransition("GetRankData", "Idle", "GetRankDataFinished");
		mFSM.addTransition("PostRankData", "Idle", "PostRankDataFinished");
		mFSM.addTransition("Idle", "GetRankData", "StartGetRankData");
		mFSM.addTransition("Idle", "PostRankData", "StartPostRankData");


		//player name
		VuFSM::VuState *pPlayerState = mPlayNameFSM.addState("PlayerNameIdle");

		pPlayerState = mPlayNameFSM.addState("GetPlayerName");	
		pPlayerState->setEnterMethod(this, &VuRankManager::onGetPlayerNameEnter);
		pPlayerState->setExitMethod(this, &VuRankManager::onGetPlayerNameExit);
		pPlayerState->setTickMethod(this, &VuRankManager::onGetPlayerNameTick);
		
		pPlayerState = mPlayNameFSM.addState("CommitPlayerName");
		pPlayerState->setEnterMethod(this, &VuRankManager::onCommitPlayerNameEnter);
		pPlayerState->setExitMethod(this, &VuRankManager::onCommitPlayerNameExit);
		pPlayerState->setTickMethod(this, &VuRankManager::onCommitPlayerNameTick);

		//transition
		
		mPlayNameFSM.addTransition("GetPlayerName", "PlayerNameIdle", "GetPlayerNameFinished");
		mPlayNameFSM.addTransition("CommitPlayerName", "PlayerNameIdle", "CommitPlayerNameFinished");
		mPlayNameFSM.addTransition("PlayerNameIdle","CommitPlayerName","StartCommitPlayerName");
		mPlayNameFSM.addTransition("PlayerNameIdle","GetPlayerName","StartGetPlayerName");
	}
	mFSM.begin();
	mPlayNameFSM.begin();
	VuTickManager::IF()->registerHandler(this, &VuRankManager::tickNetwork, "Network");
}
//*****************************************************************************
void VuRankManager::submitMilestone(float milestone)
{
	//if (mFSM.getCurState()->getName() == "GetRankData")
	//{
	//	mFSM.setCondition("GetRankDataFinished", true);
	//	mFSM.setCondition("StartPostRankData", true);
	//}
	//else if (mFSM.getCurState()->getName() == "Idle")
	//{
		mFSM.setCondition("StartPostRankData", true);
		mFSM.setCondition("PostRankDataFinished", false);
	//}
	maddMillStone = milestone;
	//if (playerName.length())
	//{
	//	VUHANDLE httpRequest = VuHttpClient::IF()->createRequest();
	//	
	//	//URL
	//	char url[256] = BASE_URL;

	//	//data
	//	VuJsonContainer data;
	//	std::string strData;
	//	VuJsonWriter writer;
	//	writer.configCompact(true);
	//	writer.saveToString(data, strData);

	//	VuHttpClient::IF()->setContentHeader(httpRequest, "Content-Type", "application/json");
	//	VuHttpClient::IF()->setContentHeader(httpRequest, "Content-Length", (int)strData.size());

	//	//send Request
	//	VuHttpClient::IF()->postAsync(httpRequest, url, strData);
	//	
	//	VuHttpClient::IF()->releaseRequest(httpRequest);
	//}
}

//*****************************************************************************
void VuRankManager::release()
{
	VuTickManager::IF()->unregisterHandlers(this);
	mFSM.end();
}
//*****************************************************************************
void VuRankManager::tickNetwork(float fdt)
{
	mFSM.evaluate();
	mFSM.tick(fdt);
	mPlayNameFSM.evaluate();
	mPlayNameFSM.tick(fdt);
	mTimeSlip += fdt;
	if (mTimeSlip > CACHE_TIME)
	{
		mFSM.setCondition("StartGetRankData", true);
		mFSM.setCondition("GetRankDataFinished", false);
		mTimeSlip = 0.0;
	}
}
//*****************************************************************************
void VuRankManager::onGetPlayerNameTick(float fdt)
{
	if (mGetNameHttpRequest)
	{
		VuHttpClient::eStatus status = VuHttpClient::IF()->getStatus(mGetNameHttpRequest);
		if (status == VuHttpClient::STATUS_RESPONSE_RECEIVED)
		{
			VuJsonContainer response;
			VuJsonReader jsonReader;
			if (jsonReader.loadFromString(response, VuHttpClient::IF()->getResponse(mGetNameHttpRequest)))
			{
				std::string playername = response["msg"]["name"].asString();
				tempUserName = playername;
				VuGameFontMacros::IF()->setMacro("TEMP_USER_NAME", tempUserName.c_str());

				VuGameManager::IF()->addGetNameTimes();
				VuGameManager::IF()->setTempPlayerName(tempUserName);

				char str[128];
				VU_SPRINTF(str, sizeof(str), "(%d/%d)", VuGameManager::IF()->getNameTimes(), 3);
				VuGameFontMacros::IF()->setMacro("GET_NAME_TIMES", str);
			}
			mPlayNameFSM.setCondition("GetPlayerNameFinished", true);
			mPlayNameFSM.setCondition("StartGetPlayerName", false);
		}
		else
		{
			mWrap->mTime += fdt;
			if (mWrap->mTime > WAITING_TIME)
			{
				mPlayNameFSM.setCondition("GetPlayerNameFinished", true);
				mPlayNameFSM.setCondition("StartGetPlayerName", false);
				mWrap->mTime = 0;
			}
		}
	}
					
}
//*****************************************************************************
VuRankManager::VuRankDataWrap* VuRankManager::getRankDatas()
{
	return mWrap;
}
//*****************************************************************************
void VuRankManager::onGetRankDataEnter()
{
	VUPRINTF("enter onGetRankDataEnter");
	mHttpRequest = VuHttpClient::IF()->createRequest();
	
	VuJsonContainer data;
	data["uid"].putValue(VuGameManager::IF()->getUid());
	
	data["method"].putValue("getRanking");
	if (mRankDataType == CUR_WEEK)
		data["listtype"].putValue("playerscore");
	else
		data["listtype"].putValue("weekscore");
	std::string strData;
	VuJsonWriter writer;
	writer.configCompact(true);
	writer.saveToString(data, strData);

	// header
	VuHttpClient::IF()->setContentHeader(mHttpRequest, "Content-Type", "application/json");
	VuHttpClient::IF()->setContentHeader(mHttpRequest, "Content-Length", (int)strData.size());
	// send request
	VuHttpClient::IF()->postAsync(mHttpRequest, BASE_URL, strData);

}
//*****************************************************************************
void VuRankManager::onGetRankDataExit()
{
	VUPRINTF("enter onGetRankDataExit");
	if (mHttpRequest)
	{
		VuHttpClient::IF()->releaseRequest(mHttpRequest);
		mHttpRequest = VUNULL;
	}
}
//*****************************************************************************
void VuRankManager::startGetPlayerName()
{
	mPlayNameFSM.setCondition("StartGetPlayerName", true);
	mPlayNameFSM.setCondition("GetPlayerNameFinished", false);
}
//*****************************************************************************
void VuRankManager::startGetRank()
{
	mFSM.setCondition("StartGetRankData", true);
	mFSM.setCondition("GetRankDataFinished", false);
}
//*****************************************************************************
void	VuRankManager::commitTempPlayerName()
{
	//mPlayNameFSM.setCondition("");
	mPlayNameFSM.setCondition("StartCommitPlayerName", true);
	mPlayNameFSM.setCondition("CommitPlayerNameFinished", false);
}
//*****************************************************************************
void VuRankManager::setRankDataType(RankDataType rankDataType)
{
	mRankDataType = rankDataType;
}
//*****************************************************************************
void VuRankManager::onGetRankDataTick(float fdt)
{
	VUPRINTF("enter onGetRankDataTick");
	if (mHttpRequest)
	{
		VuHttpClient::eStatus status = VuHttpClient::IF()->getStatus(mHttpRequest);
		if (status == VuHttpClient::STATUS_RESPONSE_RECEIVED)
		{
			VUPRINTF("enter onGetRankDataTick");
			VuJsonContainer response;
			VuJsonReader jsonReader;
			if (jsonReader.loadFromString(response, VuHttpClient::IF()->getResponse(mHttpRequest)))
			{
				VUPRINTF(response["msg"].asCString());
				VuJsonContainer msgContainer = response["msg"];
				if (response["msg"]["type"].asString() == "success")
				{
					if (response["msg"]["list"].size() > 0)
					{
						//update rankData ,so we should clear the data
						if (mWrap)
						{
							mWrap->mRankDatas.clear();
							for (int i = 0; i < response["msg"]["list"].size(); i++)
							{
								std::string strName = response["msg"]["list"][i]["name"].asString();
								int idx = response["msg"]["list"][i]["idx"].asInt();
								std::string strUid = response["msg"]["list"][i]["uid"].asString();
								int mileStone = response["msg"]["list"][i]["num"].asInt();

								VuRankData dataItem;
								dataItem.mGamePlayer = strName;
								dataItem.mRank = idx;
								dataItem.mValue = mileStone;
								mWrap->mRankDatas.push_back(dataItem);
							}
							mCurRank = response["msg"]["selfdata"]["idx"].asInt();
							mCurMilestone = response["msg"]["selfdata"]["num"].asInt();
						}
					}
					else
					{
						if (mWrap)
							mWrap->mRankDatas.clear();
					}
					mFSM.setCondition("GetRankDataFinished", true);
					mFSM.setCondition("StartGetRankData", false);
				}
			}
		}
		else
		{
			mWrap->mTime += fdt;
			if (mWrap->mTime > WAITING_TIME)
			{
				mFSM.setCondition("GetRankDataFinished", true);
				mFSM.setCondition("StartGetRankData", false);
				mWrap->mTime = 0;
			}
		}
	}
}
//*****************************************************************************
void VuRankManager::onPostRankDataEnter()
{
	VUPRINTF("enter onGetRankDataEnter");
	mHttpRequest = VuHttpClient::IF()->createRequest();

	VuJsonContainer data;
	std::string strData;
	
	std::string uid = VuGameManager::IF()->getUid();
	data["uid"].putValue(uid);
	data["method"].putValue("updateUserData");
	data["addScoreNum"].putValue(maddMillStone);

	VuJsonWriter writer;
	writer.configCompact(true);
	writer.saveToString(data, strData);

	// header
	VuHttpClient::IF()->setContentHeader(mHttpRequest, "Content-Type", "application/json");
	VuHttpClient::IF()->setContentHeader(mHttpRequest, "Content-Length", (int)strData.size());
	// send request
	VuHttpClient::IF()->postAsync(mHttpRequest, BASE_URL, strData);
}

//*****************************************************************************
void VuRankManager::onPostRankDataExit()
{
	VUPRINTF("enter onPostRankDataExit");
	if (mHttpRequest)
	{
		VuHttpClient::IF()->releaseRequest(mHttpRequest);
		mHttpRequest = VUNULL;
	}
}
//*****************************************************************************
void VuRankManager::onPostRankDataTick(float fdt)
{
	if (mHttpRequest)
	{
		VuHttpClient::eStatus status = VuHttpClient::IF()->getStatus(mHttpRequest);
		if (status == VuHttpClient::STATUS_RESPONSE_RECEIVED)
		{
			VuJsonContainer response;
			VuJsonReader jsonReader;
			if (jsonReader.loadFromString(response, VuHttpClient::IF()->getResponse(mHttpRequest)))
			{
				if (response["msg"]["type"].asString() == "success")
				{
					VUPRINTF("onPostRankData ok");
				}
				mFSM.setCondition("PostRankDataFinished", true);
				mFSM.setCondition("StartPostRankData", false);
			}
			{
				mFSM.setCondition("PostRankDataFinished", true);
				mFSM.setCondition("StartPostRankData", false);
			}
		}
	}
}

void	VuRankManager::onGetPlayerNameEnter()
{
	mGetNameHttpRequest = VuHttpClient::IF()->createRequest();
	VuJsonContainer data;
	data["method"].putValue("getPlateNumber");
	std::string strData;
	VuJsonWriter writer;
	writer.configCompact(true);
	writer.saveToString(data, strData);

	// header
	VuHttpClient::IF()->setContentHeader(mGetNameHttpRequest, "Content-Type", "application/json");
	VuHttpClient::IF()->setContentHeader(mGetNameHttpRequest, "Content-Length", (int)strData.size());
	// send request
	VuHttpClient::IF()->postAsync(mGetNameHttpRequest, BASE_URL, strData);
}

void	VuRankManager::onGetPlayerNameExit()
{
	if (mGetNameHttpRequest)
	{
		VuHttpClient::IF()->releaseRequest(mGetNameHttpRequest);
		mGetNameHttpRequest = VUNULL;
	}
}

void	VuRankManager::onCommitPlayerNameEnter()
{
	mGetNameHttpRequest = VuHttpClient::IF()->createRequest();
	VuJsonContainer data;
	data["method"].putValue("createUserName");
	data["name"].putValue(VuGameManager::IF()->getTempPlayerName());
	data["getName"].putValue(0);
	std::string strData;
	VuJsonWriter writer;
	writer.configCompact(true);
	writer.saveToString(data, strData);

	// header
	VuHttpClient::IF()->setContentHeader(mGetNameHttpRequest, "Content-Type", "application/json");
	VuHttpClient::IF()->setContentHeader(mGetNameHttpRequest, "Content-Length", (int)strData.size());
	// send request
	VuHttpClient::IF()->postAsync(mGetNameHttpRequest, BASE_URL, strData);
}
void	VuRankManager::onCommitPlayerNameExit()
{
	if (mGetNameHttpRequest)
	{
		VuHttpClient::IF()->releaseRequest(mGetNameHttpRequest);
		mGetNameHttpRequest = VUNULL;
	}
}
void	VuRankManager::onCommitPlayerNameTick(float fdt)
{
	if (mGetNameHttpRequest)
	{
		VuHttpClient::eStatus status = VuHttpClient::IF()->getStatus(mGetNameHttpRequest);
		if (status == VuHttpClient::STATUS_RESPONSE_RECEIVED)
		{
			VuJsonContainer response;
			VuJsonReader jsonReader;
			if (jsonReader.loadFromString(response, VuHttpClient::IF()->getResponse(mGetNameHttpRequest)))
			{
				if (response["msg"]["type"].asString() == "success")
				{
					std::string playerName = response["msg"]["name"].asString();
					std::string uid = response["msg"]["uid"].asString();
					VuGameManager::IF()->setPlayerNameAndUid(playerName, uid);
				}
				mPlayNameFSM.setCondition("CommitPlayerNameFinished", true);
				mPlayNameFSM.setCondition("StartCommitPlayerName", false);	
			}
		}
		else
		{
			mWrap->mTime += fdt;
			if (mWrap->mTime > WAITING_TIME)
			{
				mPlayNameFSM.setCondition("CommitPlayerNameFinished", true);
				mPlayNameFSM.setCondition("StartCommitPlayerName", false);
				mWrap->mTime = 0;
			}
		}
	}
}

void VuRankManager::tabRankDataType()
{
	mRankDataType = (mRankDataType == CUR_WEEK) ? LAST_WEEK : CUR_WEEK;
	if (mFSM.getCurState()->getName() == "Idle")
	{
		mFSM.setCondition("StartGetRankData", true);
		mFSM.setCondition("GetRankDataFinished", false);
	}
}


