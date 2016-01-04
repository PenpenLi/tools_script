//
//  Network.cpp
//  HiFly
//
//  Created by chris li on 14-4-18.
//
//

#include "Network.h"
#include "HttpHelper.h"
#include "Player.h"
#include "MailListItem.h"
#include "TaskListItem.h"
#include "RoleListItem.h"
#include "view/common/TextFloatLabelLayer.h"
#include "BattleData.h"
#include "PlatformUtil.h"
#include "common/NetworkConnectLayer.h"

static Network* g_Network = nullptr;

////////////////////////////////////////keyword: login//////////////////////////////////////////////////
void Network::do_UserLogin_Req(std::string username, std::string password, std::function<void(const AckInfo &ackInfo)> ackUIHandler)
{
	registerAckUIHandler("UserLogin", ackUIHandler);

	UserLogin_Req login_req;

	login_req.set_username(username);
	login_req.set_password(password);

	std::string output;

	if (login_req.SerializeToString(&output))
	{
		log("[Network]INFO: login->\n{\n%s}", login_req.Utf8DebugString().c_str());

		sendData("UserLogin", output.c_str(), output.length());
	}
}

void Network::do_UserLogin_Ack(const std::string &dataStr)
{
	UserLogin_Ack loginAck;

	if (loginAck.ParseFromString(dataStr))
	{
		m_messageInfo = "";

		if (loginAck.has_msginfo())
		{
			log("[Network] errinfo:%s", loginAck.msginfo().c_str());

			m_messageInfo = loginAck.msginfo();

			TextFloatLabelLayer::getInstance()->show(m_messageInfo);
		}
		else {
			Player::getInstance()->setUserName(loginAck.username());
			Player::getInstance()->setNickname(loginAck.nickname());
			Player::getInstance()->setLevel(loginAck.level());
			Player::getInstance()->setStar(loginAck.star());
			Player::getInstance()->setCoin(loginAck.coin());
			Player::getInstance()->setDiamond(loginAck.diamond());
			Player::getInstance()->setHelpCount(loginAck.helpcount());
			Player::getInstance()->setRoleType(loginAck.roletype());
			Player::getInstance()->setMailCount(loginAck.mailcount());
			Player::getInstance()->setWeekScore(loginAck.weekscore());
			Player::getInstance()->setScore(loginAck.score());
			Player::getInstance()->setTime(loginAck.time());
			Player::getInstance()->setContent(loginAck.content());
			Player::getInstance()->setLoopContent(loginAck.imgpath());
			Player::getInstance()->setTaskCount(loginAck.avtivecount());
			Player::getInstance()->setHelpInc(loginAck.helpinc());
			Player::getInstance()->setBgmInc(loginAck.bgminc());
			Player::getInstance()->setSoundInc(loginAck.soundinc());
			Player::getInstance()->setSensitivityInc(loginAck.sensitivityinc());
			Player::getInstance()->setToken(loginAck.token());
			Player::getInstance()->setLoginCount(loginAck.logincount());
			Player::getInstance()->setLoginCountTotal(loginAck.logincounttotal());

			Player::getInstance()->setMyPlayerId(loginAck.playerid());

			Player::getInstance()->setCurrentLevel(loginAck.stageid());

			Player::getInstance()->setMyPlayerCount(loginAck.count());
		}

		log("[Network]INFO: login_req<-\n{\n%s}", (loginAck).Utf8DebugString().c_str());

		callUIHandler("UserLogin");
	}
	else
	{
		log("[Network]ERR:unknown protobuf message");
	}

}

void Network::do_UserLoginReward_Req(int rewardType, std::function<void(const AckInfo &ackInfo)> ackUIHandler)
{
	registerAckUIHandler("UserLoginReward", ackUIHandler);

	UserLoginReward_Req loginReward_req;

	loginReward_req.set_token(Player::getInstance()->getToken());

	std::string output;

	if (loginReward_req.SerializeToString(&output))
	{
		log("[Network]INFO: loginReward_req->\n{\n%s}", loginReward_req.Utf8DebugString().c_str());

		sendData("UserLoginReward", output.c_str(), output.length());
	}
}
void Network::do_UserLoginReward_Ack(const std::string &dataStr)
{
	UserLoginReward_Ack loginRewardAck;
	if (loginRewardAck.ParseFromString(dataStr))
	{
		m_messageInfo = "";

		if (loginRewardAck.has_msginfo())
		{
			log("[Network] errinfo:%s", loginRewardAck.msginfo().c_str());

			m_messageInfo = loginRewardAck.msginfo();
		}
		else {
			//Player::getInstance()->setCoin(loginRewardAck.coin());

			//Player::getInstance()->setDiamond(loginRewardAck.diamond());
		}

		log("[Network]INFO: loginReward_req<-\n{\n%s}", (loginRewardAck).Utf8DebugString().c_str());

		callUIHandler("UserLoginReward");
	}
	else
	{
		log("[Network]ERR:unknown protobuf message");
	}

}

void Network::do_Notice_Req(int rewardType, std::function<void(const AckInfo &ackInfo)> ackUIHandler)
{
	registerAckUIHandler("Notice", ackUIHandler);

	Notice_Req notice_req;

	notice_req.set_token(Player::getInstance()->getToken());

	std::string output;

	if (notice_req.SerializeToString(&output))
	{
		log("[Network]INFO: notice_req->\n{\n%s}", notice_req.Utf8DebugString().c_str());

		sendData("Notice", output.c_str(), output.length());
	}
}
void Network::do_Notice_Ack(const std::string &dataStr)
{
	Notice_Ack noticeAck;
	if (noticeAck.ParseFromString(dataStr))
	{
		m_messageInfo = "";

		if (noticeAck.has_msginfo())
		{
			log("[Network] errinfo:%s", noticeAck.msginfo().c_str());

			m_messageInfo = noticeAck.msginfo();
		}
		else {
			Player::getInstance()->setNoticeMsg(noticeAck.contents());
		}

		log("[Network]INFO: notice_req<-\n{\n%s}", (noticeAck).Utf8DebugString().c_str());

		callUIHandler("Notice");
	}
	else
	{
		log("[Network]ERR:unknown protobuf message");
	}
}

void Network::do_UserRegister_Req(std::string username, std::string password, std::function<void(const AckInfo &ackInfo)> ackUIHandler)
{
	registerAckUIHandler("UserRegister", ackUIHandler);

	UserRegister_Req register_req;

	register_req.set_username(username);
	register_req.set_password(password);

	std::string output;

	if (register_req.SerializeToString(&output))
	{
		log("[Network]INFO: register_req->\n{\n%s}", register_req.Utf8DebugString().c_str());

		sendData("UserRegister" , output.c_str(), output.length());
	}
}

void Network::do_UserRegister_Ack(const std::string &dataStr)
{
	UserRegister_Ack registerAck;

	if ((registerAck).ParseFromString(dataStr))
	{
		log("[Network]INFO: registerAck <-\n{\n%s}", (registerAck).Utf8DebugString().c_str());

		m_messageInfo = "";

		if (registerAck.has_msginfo())
		{
			log("[Network] errinfo:%s", registerAck.msginfo().c_str());

			m_messageInfo = registerAck.msginfo();

			TextFloatLabelLayer::getInstance()->show(m_messageInfo);
		}
		else
		{
			// update models
			//Player::getInstance()->dataUpdated();
		}

		callUIHandler("UserRegister");
	}
	else
	{
		log("[Network]ERR:unknown protobuf message");
	}
}

void Network::do_UpRole_Req(int roleType, std::function<void(const AckInfo &ackInfo)> ackUIHandler)
{
	registerAckUIHandler("UpRole", ackUIHandler);

	UpRole_Req req;

	req.set_token(Player::getInstance()->getToken());
	req.set_roletype(roleType);

	std::string output;

	if (req.SerializeToString(&output))
	{
		log("[Network]INFO: do_UpRole->\n{\n%s}", req.Utf8DebugString().c_str());

		sendData("UpRole" , output.c_str(), output.length());
	}
}

void Network::do_UpRole_Ack(const std::string &dataStr)
{
	UpRole_Ack ack;

	if (ack.ParseFromString(dataStr))
	{
		log("[Network]INFO: do_UpRole <-\n{\n%s}", ack.Utf8DebugString().c_str());

		m_messageInfo = "";

		if (ack.has_msginfo())
		{
			log("[Network] errinfo:%s", ack.msginfo().c_str());

			m_messageInfo = ack.msginfo();

			TextFloatLabelLayer::getInstance()->show(m_messageInfo);
		}
		else
		{
			// update models
			//Player::getInstance()->setRoleType(ack.roletype());

			int _frontRoleId = -1;

			switch (ack.roletype())
			{
			case 1:
			case 2:
			case 3:
				_frontRoleId = 0;
				break;
			case 4:
			case 5:
			case 6:
			case 7:
				_frontRoleId = 1;
				break;
			default:
				break;
			}

			if(-1!=_frontRoleId)
			{

				Player::getInstance()->getRoleList()->at(_frontRoleId)->roleType = ack.roletype();

				Player::getInstance()->getRoleList()->at(_frontRoleId)->level = ack.rolelevel();

				Player::getInstance()->getRoleList()->at(_frontRoleId)->breakType = ack.breaktype();

				Player::getInstance()->getRoleList()->at(_frontRoleId)->isMax = ack.ismax();


			}

			//Player::getInstance()->setRoleLevel(ack.rolelevel());

			Player::getInstance()->setCoin(ack.coin());

			Player::getInstance()->setDiamond(ack.diamond());
		}

		callUIHandler("UpRole");
	}
	else
	{
		log("[Network]ERR:unknown protobuf message");
	}
}
///////////////////////////////////////keywork: rigister///////////////////////////////////////////////

void Network::do_SetNickname_Req(std::string nickname, std::function<void(const AckInfo &ackInfo)> ackUIHandler)
{
	registerAckUIHandler("SetNickname", ackUIHandler);

	SetNickname_Req nickname_req;

	nickname_req.set_token(Player::getInstance()->getToken());
	nickname_req.set_nickname(nickname);

	std::string output;

	if (nickname_req.SerializeToString(&output))
	{
		log("[Network]INFO: nickname_req->\n{\n%s}", nickname_req.Utf8DebugString().c_str());

		sendData("SetNickname" , output.c_str(), output.length());
	}
}

void Network::do_SetNickname_Ack(const std::string &dataStr)
{
	SetNickname_Ack nicknameAck;

	if ((nicknameAck).ParseFromString(dataStr))
	{
		log("[Network]INFO: nicknameAck <-\n{\n%s}", (nicknameAck).Utf8DebugString().c_str());

		m_messageInfo = "";

		if (nicknameAck.has_msginfo())
		{
			log("[Network] errinfo:%s", nicknameAck.msginfo().c_str());

			m_messageInfo = nicknameAck.msginfo();

			TextFloatLabelLayer::getInstance()->show(m_messageInfo);
		}
		else
		{
			// update models
			Player::getInstance()->setNickname(nicknameAck.nickname());
		}

		callUIHandler("SetNickname");
	}
	else
	{
		log("[Network]ERR:unknown protobuf message");
	}
}

void Network::do_RankList_Req(int rankType, int scoreType, std::function<void(const AckInfo &ackInfo)> ackUIHandler)
{
	registerAckUIHandler("RankList", ackUIHandler);

	RankList_Req req;

	req.set_token(Player::getInstance()->getToken());

	req.set_ranktype(rankType);

	req.set_scoretype(scoreType);

	std::string output;

	if (req.SerializeToString(&output))
	{
		log("[Network]INFO: do_RankList_Req->\n{\n%s}", req.Utf8DebugString().c_str());

		sendData("RankList" , output.c_str(), output.length());
	}
}

void Network::do_RankList_Ack(const std::string &dataStr)
{
	RankList_Ack ack;

	if (ack.ParseFromString(dataStr))
	{
		log("[Network]INFO: do_RankList_Ack <-\n{\n%s}", ack.Utf8DebugString().c_str());

		m_messageInfo = "";

		if (ack.has_msginfo())
		{
			log("[Network] errinfo:%s", ack.msginfo().c_str());

			m_messageInfo = ack.msginfo();

			TextFloatLabelLayer::getInstance()->show(m_messageInfo);
		}
		else
		{
			// update models
			if (1==ack.ranktype())
			{
				Player::getInstance()->getPlatformRankList()->clear();

				for (int i = 0; i < ack.ranklist_size(); i++)
				{
					auto item = ack.ranklist(i);

					RankListItem *rankListItem = RankListItem::create();

					rankListItem->playerId = item.playerid();

					rankListItem->nickname = item.nickname();

					//rankListItem->level = item.level();
					//           
					//rankListItem->roleType = item.roletype();  

					rankListItem->score = item.score();

					Player::getInstance()->getPlatformRankList()->pushBack(rankListItem);
				}

				//Player::getInstance()->getPlatformRankList()->reverse();
			}
			else if (2==ack.ranktype())
			{
				Player::getInstance()->getWorldRankList()->clear();

				for (int i = 0; i < ack.ranklist_size(); i++)
				{
					auto item = ack.ranklist(i);

					RankListItem *rankListItem = RankListItem::create();

					rankListItem->playerId = item.playerid();

					rankListItem->nickname = item.nickname();

					//rankListItem->level = item.level();
					//           
					//rankListItem->roleType = item.roletype();  

					rankListItem->score = item.score();

					Player::getInstance()->getWorldRankList()->pushBack(rankListItem);
				}

				//Player::getInstance()->getWorldRankList()->reverse();
			}

			callUIHandler("RankList");
		}
	}
	else
	{
		log("[Network]ERR:unknown protobuf message");
	}
}

//last rank list
void Network::do_lastRankList_Req(std::function<void(const AckInfo &ackInfo)> ackUIHandler)
{
	registerAckUIHandler("lastRankList", ackUIHandler);

	lastRankList_Req req;

	req.set_token(Player::getInstance()->getToken());

	std::string output;

	if (req.SerializeToString(&output))
	{
		log("[Network]INFO: do_lastRankList_Req->\n{\n%s}", req.Utf8DebugString().c_str());

		sendData("lastRankList" , output.c_str(), output.length());
	}
}

void Network::do_lastRankList_Ack(const std::string &dataStr)
{
	lastRankList_Ack ack;

	if (ack.ParseFromString(dataStr))
	{
		log("[Network]INFO: do_lastRankList_Ack <-%s", ack.Utf8DebugString().c_str());

		m_messageInfo = "";

		if (ack.has_msginfo())
		{
			log("[Network] errinfo:%s", ack.msginfo().c_str());

			m_messageInfo = ack.msginfo();
		}
		else
		{
//			Player::getInstance()->getWorldRankList()->clear();

			for (int i = 0; i < ack.ranklist_size(); i++)
			{
				auto item = ack.ranklist(i);

				RankListItem *rankListItem = RankListItem::create();

				rankListItem->nickname = item.nickname();

				rankListItem->score = item.score();

				Player::getInstance()->getlastRankList()->pushBack(rankListItem);
			}

			//Player::getInstance()->getlastRankList()->reverse();


			callUIHandler("lastRankList");
		}
	}
	else
	{
		log("[Network]ERR:unknown protobuf message");
	}

}
void Network::do_Opinion_Req(std::string &opinionStr, std::function<void(const AckInfo &ackInfo)> ackUIHandler)
{
	registerAckUIHandler("Opinion", ackUIHandler);

	Opinion_Req req;

	req.set_token(Player::getInstance()->getToken());

	req.set_message(opinionStr);

	std::string output;

	if (req.SerializeToString(&output))
	{
		log("[Network]INFO: do_Opinion_Req->\n{\n%s}", req.Utf8DebugString().c_str());

		sendData("Opinion" , output.c_str(), output.length());
	}
}

void Network::do_Opinion_Ack(const std::string &dataStr)
{
	Opinion_Ack ack;

	if (ack.ParseFromString(dataStr))
	{
		log("[Network]INFO: Opinion_Ack <-\n{\n%s}", ack.Utf8DebugString().c_str());

		m_messageInfo = "";

		if (ack.has_msginfo())
		{
			log("[Network] errinfo:%s", ack.msginfo().c_str());

			m_messageInfo = ack.msginfo();

			TextFloatLabelLayer::getInstance()->show(m_messageInfo);
		}
		else
		{
			// update models
		}

		callUIHandler("Opinion");
	}
	else
	{
		log("[Network]ERR:unknown protobuf message");
	}
}

void Network::do_SetControl_Req(bool FriendsHeart, int Music, int Sound, int Sensitivity, std::function<void(const AckInfo &ackInfo)> ackUIHandler)
{
	registerAckUIHandler("SetControl", ackUIHandler);

	SetControl_Req req;

	req.set_token(Player::getInstance()->getToken());

	req.set_friendsheart(FriendsHeart);

	req.set_music(Music);

	req.set_sound(Sound);

	req.set_sensitivity(Sensitivity);

	std::string output;

	if (req.SerializeToString(&output))
	{
		log("[Network]INFO: do_SetControl_Req->\n{\n%s}", req.Utf8DebugString().c_str());

		sendData("SetControl" , output.c_str(), output.length());
	}
}

void Network::do_SetControl_Ack(const std::string &dataStr)
{
	Opinion_Ack ack;

	if (ack.ParseFromString(dataStr))
	{
		log("[Network]INFO: do_SetControl_Ack <-\n{\n%s}", ack.Utf8DebugString().c_str());

		m_messageInfo = "";

		if (ack.has_msginfo())
		{
			log("[Network] errinfo:%s", ack.msginfo().c_str());

			m_messageInfo = ack.msginfo();

			TextFloatLabelLayer::getInstance()->show(m_messageInfo);
		}
		else
		{
			// update models
		}

		callUIHandler("SetControl");
	}
	else
	{
		log("[Network]ERR:unknown protobuf message");
	}
}

void Network::do_FriendList_Req(int friendListType, std::function<void(const AckInfo &ackInfo)> ackUIHandler)
{
	registerAckUIHandler("FriendList", ackUIHandler);

	FriendList_Req req;

	req.set_token(Player::getInstance()->getToken());

	if(3==friendListType)
	{
		auto latitude = PlatformUtil::LBS_getLatitude();
        
        auto longitude = PlatformUtil::LBS_getLongitude();

		req.set_longitude(StringUtils::format("%f", latitude));

		req.set_latitude(StringUtils::format("%f", longitude));

	}

	req.set_pagetype(friendListType);

	std::string output;

	if (req.SerializeToString(&output))
	{
		log("[Network]INFO: do_FriendList_Req->\n{\n%s}", req.Utf8DebugString().c_str());

		sendData("FriendList" , output.c_str(), output.length());
	}
}

void Network::do_FriendList_Ack(const std::string &dataStr)
{
	FriendList_Ack ack;

	if (ack.ParseFromString(dataStr))
	{
		log("[Network]INFO: do_FriendList_Ack <-\n{\n%s}", ack.Utf8DebugString().c_str());

		m_messageInfo = "";

		if (ack.has_msginfo())
		{
			log("[Network] errinfo:%s", ack.msginfo().c_str());

			m_messageInfo = ack.msginfo();

			TextFloatLabelLayer::getInstance()->show(m_messageInfo);
		}
		else
		{
			// update models
			if(2==ack.pagetype())
			{
				Player::getInstance()->getFriendList()->clear();

				for (int i = 0; i < ack.friendlist_size(); i++)
				{
					auto item = ack.friendlist(i);

					FriendListItem *friendListItem = FriendListItem::create();

					friendListItem->playerId = item.playerid();

					friendListItem->nickname = item.nickname();

					friendListItem->level = item.level();

					friendListItem->score = item.score();

					friendListItem->helpTimes = item.helpnum();

					friendListItem->isSendHeart = item.issendheart();

					friendListItem->roleType = item.roletype();

					Player::getInstance()->getFriendList()->pushBack(friendListItem);
				}
			}
			else if(3==ack.pagetype())
			{
				Player::getInstance()->getNearbyList()->clear();

				for (int i = 0; i < ack.friendlist_size(); i++)
				{
					auto item = ack.friendlist(i);

					FriendListItem *nearbyListItem = FriendListItem::create();

					nearbyListItem->playerId = item.playerid();

					nearbyListItem->nickname = item.nickname();

					nearbyListItem->level = item.level();

					nearbyListItem->score = item.score();

					nearbyListItem->roleType = item.roletype();

					nearbyListItem->distance = item.distance();


					Player::getInstance()->getNearbyList()->pushBack(nearbyListItem);
				}
			}
		}

		callUIHandler("FriendList");
	}
	else
	{
		log("[Network]ERR:unknown protobuf message");
	}
}

void Network::do_MailList_Req(std::function<void(const AckInfo &ackInfo)> ackUIHandler)
{
	registerAckUIHandler("MailList", ackUIHandler);

	MailList_Req req;

	req.set_token(Player::getInstance()->getToken());

	std::string output;

	if (req.SerializeToString(&output))
	{
		log("[Network]INFO: do_MailList_Req->\n{\n%s}", req.Utf8DebugString().c_str());

		sendData("MailList" , output.c_str(), output.length());
	}
}

void Network::do_MailList_Ack(const std::string &dataStr)
{
	MailList_Ack ack;

	if (ack.ParseFromString(dataStr))
	{
		log("[Network]INFO: do_FriendList_Ack <-\n{\n%s}", ack.Utf8DebugString().c_str());

		m_messageInfo = "";

		if (ack.has_msginfo())
		{
			log("[Network] errinfo:%s", ack.msginfo().c_str());

			m_messageInfo = ack.msginfo();

			TextFloatLabelLayer::getInstance()->show(m_messageInfo);
		}
		else
		{
			// update models
			auto playerMailList = Player::getInstance()->getMailList();

			playerMailList->clear();

			for (int i = 0; i < ack.maillist_size(); i++)
			{
				auto item = ack.maillist(i);

				MailListItem *mailListItem = MailListItem::create();

				mailListItem->mailid = item.mailid();

				mailListItem->mailType = item.mailtype();

				mailListItem->title = item.title();

				mailListItem->status = item.status();

				mailListItem->itemId = item.itemid();

				mailListItem->itemNum = item.itemnum();

				mailListItem->contents = item.contents();

				mailListItem->domainTime = item.domaintime();

				mailListItem->playerId = item.playerid();

				mailListItem->nickName = item.nickname();

				playerMailList->pushBack(mailListItem);
			}

			playerMailList->reverse();
		}

		callUIHandler("MailList");
	}
	else
	{
		log("[Network]ERR:unknown protobuf message");
	}
}

void Network::do_GetMailProp_Req(int mailId, std::function<void(const AckInfo &ackInfo)> ackUIHandler)
{
	registerAckUIHandler("GetMailProp", ackUIHandler);

	GetMailProp_Req req;

	req.set_token(Player::getInstance()->getToken());

	req.set_mailid(mailId);

	std::string output;

	if (req.SerializeToString(&output))
	{
		log("[Network]INFO: do_GetMailProp_Req->\n{\n%s}", req.Utf8DebugString().c_str());

		sendData("GetMailProp" , output.c_str(), output.length());
	}
}

void Network::do_GetMailProp_Ack(const std::string &dataStr)
{
	GetMailProp_Ack ack;

	if (ack.ParseFromString(dataStr))
	{
		log("[Network]INFO: do_GetMailProp_Ack <-\n{\n%s}", ack.Utf8DebugString().c_str());

		m_messageInfo = "";

		if (ack.has_msginfo())
		{
			log("[Network] errinfo:%s", ack.msginfo().c_str());

			m_messageInfo = ack.msginfo();

			TextFloatLabelLayer::getInstance()->show(m_messageInfo);
		}
		else
		{
			// update models
			if(ack.coin())
			{
				Player::getInstance()->setCoin(ack.coin());
			}
			if(ack.diamond())
			{
				Player::getInstance()->setDiamond(ack.diamond());
			}
		}

		callUIHandler("GetMailProp");
	}
	else
	{
		log("[Network]ERR:unknown protobuf message");
	}
}

void Network::do_MailDel_Req(int mailId, std::function<void(const AckInfo &ackInfo)> ackUIHandler)
{
	registerAckUIHandler("MailDel", ackUIHandler);

	MailDel_Req req;

	req.set_token(Player::getInstance()->getToken());

	req.set_mailid(mailId);

	std::string output;

	if (req.SerializeToString(&output))
	{
		log("[Network]INFO: do_MailDel_Req->\n{\n%s}", req.Utf8DebugString().c_str());

		sendData("MailDel" , output.c_str(), output.length());
	}
}

void Network::do_MailDel_Ack(const std::string &dataStr)
{
	MailDel_Ack ack;

	if (ack.ParseFromString(dataStr))
	{
		log("[Network]INFO: do_MailDel_Ack <-\n{\n%s}", ack.Utf8DebugString().c_str());

		m_messageInfo = "";

		if (ack.has_msginfo())
		{
			log("[Network] errinfo:%s", ack.msginfo().c_str());

			m_messageInfo = ack.msginfo();

			TextFloatLabelLayer::getInstance()->show(m_messageInfo);
		}
		else
		{
			// update models

		}

		callUIHandler("MailDel");
	}
	else
	{
		log("[Network]ERR:unknown protobuf message");
	}
}

void Network::do_StageResult_Req(int star, std::function<void(const AckInfo &ackInfo)> ackUIHandler)
{
	registerAckUIHandler("StageResult", ackUIHandler);

	StageResult_Req req;

	req.set_token(Player::getInstance()->getToken());

	req.set_coin(BattleData::getInstance()->getGold());

	req.set_score(BattleData::getInstance()->getScore());
    
    req.set_revive(0);

	//debug
	req.set_jumpheiht(0);

	req.set_stageid(BattleData::getInstance()->getBattleLevel());

	req.set_killmonster(BattleData::getInstance()->getMonsterKilledNum());

	req.set_star(star);

	std::string output;

	if (req.SerializeToString(&output))
	{
		log("[Network]INFO: do_StageResult_Req->\n{\n%s}", req.Utf8DebugString().c_str());

		sendData("StageResult" , output.c_str(), output.length());
	}
}

void Network::do_StageResult_Ack(const std::string &dataStr)
{
	StageResult_Ack ack;

	if (ack.ParseFromString(dataStr))
	{
		log("[Network]INFO: do_StageResult_Ack <-\n{\n%s}", ack.Utf8DebugString().c_str());

		m_messageInfo = "";

		if (ack.has_msginfo())
		{
			log("[Network] errinfo:%s", ack.msginfo().c_str());

			m_messageInfo = ack.msginfo();

			TextFloatLabelLayer::getInstance()->show(m_messageInfo);
		}
		else
		{
			// update models

			Player::getInstance()->setCrownType(ack.crowntype());

			Player::getInstance()->setWeekScore(ack.weekmax());

			Player::getInstance()->setHistoryMaxScore(ack.allmax());

			Player::getInstance()->setMaxScoreType(ack.maxtype());

			Player::getInstance()->setCoin(ack.coin());

			Player::getInstance()->setDiamond(ack.diamond());

			Player::getInstance()->setIsFirstPerfect(ack.isfirst());
		}

		callUIHandler("StageResult");
	}
	else
	{
		log("[Network]ERR:unknown protobuf message");
	}
}


void Network::do_Result_Req(int score, int coin, int reviveTimes, std::function<void(const AckInfo &ackInfo)> ackUIHandler)
{
	registerAckUIHandler("Result", ackUIHandler);

	Result_Req req;

	req.set_token(Player::getInstance()->getToken());

	req.set_coin(coin);

	req.set_score(score);
    
    //revivie
    req.set_revive(reviveTimes);

	//debug
	req.set_jumpheiht(0);

	req.set_killmonster(BattleData::getInstance()->getMonsterKilledNum());

	req.set_star(0);

	std::string output;

	if (req.SerializeToString(&output))
	{
		log("[Network]INFO: do_Result_Req->\n{\n%s}", req.Utf8DebugString().c_str());

		sendData("Result" , output.c_str(), output.length());
	}
}

void Network::do_Result_Ack(const std::string &dataStr)
{
	Result_Ack ack;

	if (ack.ParseFromString(dataStr))
	{
		log("[Network]INFO: do_Result_Ack <-\n{\n%s}", ack.Utf8DebugString().c_str());

		m_messageInfo = "";

		if (ack.has_msginfo())
		{
			log("[Network] errinfo:%s", ack.msginfo().c_str());

			m_messageInfo = ack.msginfo();

			TextFloatLabelLayer::getInstance()->show(m_messageInfo);
		}
		else
		{
			// update models
			Player::getInstance()->setWeekScore(ack.weekmax());

			Player::getInstance()->setHistoryMaxScore(ack.allmax());

			Player::getInstance()->setMaxScoreType(ack.maxtype());

			Player::getInstance()->setCoin(ack.coin());

			Player::getInstance()->setIsMore(ack.ismore());

			Player::getInstance()->setDiamond(ack.diamond());
		
		}

		callUIHandler("Result");
	}
	else
	{
		log("[Network]ERR:unknown protobuf message");
	}
}
void Network::do_ResultRank_Req(std::function<void(const AckInfo &ackInfo)> ackUIHandler)
{
	registerAckUIHandler("ResultRank", ackUIHandler);

	ResultRank_Req req;

	req.set_token(Player::getInstance()->getToken());

	std::string output;

	if (req.SerializeToString(&output))
	{
		log("[Network]INFO: do_ResultRank_Req->\n{\n%s}", req.Utf8DebugString().c_str());

		sendData("ResultRank" , output.c_str(), output.length());
	}
}
void Network::do_ResultRank_Ack(const std::string &dataStr)
{
	ResultRank_Ack ack;

	if (ack.ParseFromString(dataStr))
	{
		log("[Network]INFO: do_ResultRank_Ack <-\n{\n%s}", ack.Utf8DebugString().c_str());

		m_messageInfo = "";

		if (ack.has_msginfo())
		{
			log("[Network] errinfo:%s", ack.msginfo().c_str());

			m_messageInfo = ack.msginfo();

			TextFloatLabelLayer::getInstance()->show(m_messageInfo);
		}
		else
		{

		}

		callUIHandler("ResultRank");
	}
	else
	{
		log("[Network]ERR:unknown protobuf message");
	}
}
void Network::do_ShopBuy_Req(int buyType, int itemId, std::function<void(const AckInfo &ackInfo)> ackUIHandler)
{
	registerAckUIHandler("ShopBuy", ackUIHandler);

	ShopBuy_Req req;

	req.set_token(Player::getInstance()->getToken());

	req.set_buytype(buyType);

	req.set_itmeid(itemId);

	std::string output;

	if (req.SerializeToString(&output))
	{
		log("[Network]INFO: do_ShopBuy_Req->\n{\n%s}", req.Utf8DebugString().c_str());

		sendData("ShopBuy" , output.c_str(), output.length());
	}
}

void Network::do_ShopBuy_Ack(const std::string &dataStr)
{
	ShopBuy_Ack ack;

	if (ack.ParseFromString(dataStr))
	{
		log("[Network]INFO: do_ShopBuy_Ack <-\n{\n%s}", ack.Utf8DebugString().c_str());

		m_messageInfo = "";

		if (ack.has_msginfo())
		{
			log("[Network] errinfo:%s", ack.msginfo().c_str());

			m_messageInfo = ack.msginfo();

			TextFloatLabelLayer::getInstance()->show(m_messageInfo);
		}
		else
		{
			// update models
			Player::getInstance()->setStar(ack.heart());

			Player::getInstance()->setCoin(ack.coin());

			Player::getInstance()->setDiamond(ack.diamond());
		}

		callUIHandler("ShopBuy");
	}
	else
	{
		log("[Network]ERR:unknown protobuf message");
	}
}

void Network::do_ItemAdd_Req(int itemId, int itemNum, std::function<void(const AckInfo &ackInfo)> ackUIHandler)
{
	registerAckUIHandler("ItemAdd", ackUIHandler);

	ItemAdd_Req req;

	req.set_token(Player::getInstance()->getToken());

	req.set_itemid(itemId);

	req.set_count(itemNum);

	std::string output;

	if (req.SerializeToString(&output))
	{
		log("[Network]INFO: do_ItemAdd_Req->\n{\n%s}", req.Utf8DebugString().c_str());

		sendData("ItemAdd" , output.c_str(), output.length());
	}
}

void Network::do_ItemAdd_Ack(const std::string &dataStr)
{
	ItemAdd_Ack ack;

	if (ack.ParseFromString(dataStr))
	{
		log("[Network]INFO: do_ItemAdd_Ack <-\n{\n%s}", ack.Utf8DebugString().c_str());

		m_messageInfo = "";

		if (ack.has_msginfo())
		{
			log("[Network] errinfo:%s", ack.msginfo().c_str());

			m_messageInfo = ack.msginfo();

			TextFloatLabelLayer::getInstance()->show(m_messageInfo);
		}
		else
		{
			// update models
			int _cost = 0;

			if (ack.itemid() == 2)
			{
				Player::getInstance()->setProp1Num(ack.count());

				_cost = 200;
			}
			else if (ack.itemid() == 3)
			{
				Player::getInstance()->setProp2Num(ack.count());

				_cost = 500;
			}
			else if (ack.itemid() == 4)
			{
				Player::getInstance()->setProp3Num(ack.count());

				_cost = 800;
			}
			else if (ack.itemid() == 5)
			{
				Player::getInstance()->setProp4Num(ack.count());

				_cost = 1200;
			}
			else if (ack.itemid() > 12)
			{
				Player::getInstance()->setPropRandom(ack.itemid());

				_cost = 300;
			}

			Player::getInstance()->setCoin(Player::getInstance()->getCoin()-_cost);
		}

		callUIHandler("ItemAdd");
	}
	else
	{
		log("[Network]ERR:unknown protobuf message");
	}
}

void Network::do_ItemList_Req(std::function<void(const AckInfo &ackInfo)> ackUIHandler)
{
	registerAckUIHandler("ItemList", ackUIHandler);

	ItemList_Req req;

	req.set_token(Player::getInstance()->getToken());

	std::string output;

	if (req.SerializeToString(&output))
	{
		log("[Network]INFO: do_ItemList_Req->\n{\n%s}", req.Utf8DebugString().c_str());

		sendData("ItemList" , output.c_str(), output.length());
	}

}

void Network::do_ItemList_Ack(const std::string &dataStr)
{
	ItemList_Ack ack;

	if (ack.ParseFromString(dataStr))
	{
		log("[Network]INFO: do_ItemList_Ack <-\n{\n%s}", ack.Utf8DebugString().c_str());

		m_messageInfo = "";

		if (ack.has_msginfo())
		{
			log("[Network] errinfo:%s", ack.msginfo().c_str());

			m_messageInfo = ack.msginfo();

			TextFloatLabelLayer::getInstance()->show(m_messageInfo);
		}
		else
		{
			//init
			Player::getInstance()->setProp1Num(0);
			Player::getInstance()->setProp2Num(0);
			Player::getInstance()->setProp3Num(0);
			Player::getInstance()->setProp4Num(0);

			Player::getInstance()->setPropRandom(ack.itemid());

			for (int i = 0; i < ack.itemlist_size(); i++)
			{
				switch (ack.itemlist(i).itemid())
				{
				case 2:
					Player::getInstance()->setProp1Num(ack.itemlist(i).count());
					break;
				case 3:
					Player::getInstance()->setProp2Num(ack.itemlist(i).count());
					break;
				case 4:
					Player::getInstance()->setProp3Num(ack.itemlist(i).count());
					break;
				case 5:
					Player::getInstance()->setProp4Num(ack.itemlist(i).count());
					break;
				default:
					break;
				}
			}

			//propList->reverse();
		}

		callUIHandler("ItemList");
	}
	else
	{
		log("[Network]ERR:unknown protobuf message");
	}
}

void Network::do_AddFriend_Req(int playerId, int addType, std::function<void(const AckInfo &ackInfo)> ackUIHandler)
{
	registerAckUIHandler("AddFriend", ackUIHandler);

	AddFriend_Req req;

	req.set_token(Player::getInstance()->getToken());

	req.set_playerid(playerId);

	req.set_addtype(addType);

	std::string output;

	if (req.SerializeToString(&output))
	{
		log("[Network]INFO: do_AddFriend_Req->\n{\n%s}", req.Utf8DebugString().c_str());

		sendData("AddFriend" , output.c_str(), output.length());
	}

}

void Network::do_AddFriend_Ack(const std::string &dataStr)
{
	AddFriend_Ack ack;

	if (ack.ParseFromString(dataStr))
	{
		log("[Network]INFO: do_AddFriend_Ack <-\n{\n%s}", ack.Utf8DebugString().c_str());

		m_messageInfo = "";

		if (ack.has_msginfo())
		{
			log("[Network] errinfo:%s", ack.msginfo().c_str());

			m_messageInfo = ack.msginfo();

			TextFloatLabelLayer::getInstance()->show(m_messageInfo);
		}
		else
		{
			// update models

		}

		callUIHandler("AddFriend");
	}
	else
	{
		log("[Network]ERR:unknown protobuf message");
	}
}

void Network::do_DelFriend_Req(int playerId, int addType, std::function<void(const AckInfo &ackInfo)> ackUIHandler)
{
	registerAckUIHandler("DelFriend", ackUIHandler);

	DelFriend_Req req;

	req.set_token(Player::getInstance()->getToken());

	req.set_playerid(playerId);

	req.set_type(addType);

	std::string output;

	if (req.SerializeToString(&output))
	{
		log("[Network]INFO: do_DelFriend_Req->\n{\n%s}", req.Utf8DebugString().c_str());

		sendData("DelFriend" , output.c_str(), output.length());
	}

}

void Network::do_DelFriend_Ack(const std::string &dataStr)
{
	DelFriend_Ack ack;

	if (ack.ParseFromString(dataStr))
	{
		log("[Network]INFO: do_DelFriend_Ack <-\n{\n%s}", ack.Utf8DebugString().c_str());

		m_messageInfo = "";

		if (ack.has_msginfo())
		{
			log("[Network] errinfo:%s", ack.msginfo().c_str());

			m_messageInfo = ack.msginfo();

			TextFloatLabelLayer::getInstance()->show(m_messageInfo);
		}
		else
		{
			// update models

		}

		callUIHandler("DelFriend");
	}
	else
	{
		log("[Network]ERR:unknown protobuf message");
	}
}

void Network::do_TaskAccept_Req(int taskId, std::function<void(const AckInfo &ackInfo)> ackUIHandler)
{
	registerAckUIHandler("TaskAccept",ackUIHandler);

	TaskAccept_Req req;

	req.set_token(Player::getInstance()->getToken());

	req.set_task_id(taskId);

	std::string output;

	if (req.SerializeToString(&output))
	{
		log("[Network]INFO: do_TaskAccept_Req->\n{\n%s}", req.Utf8DebugString().c_str());

		sendData("TaskAccept" , output.c_str(), output.length());
	}
}

void Network::do_TaskAccept_Ack(const std::string &dataStr)
{
	TaskAccept_Ack ack;

	if (ack.ParseFromString(dataStr))
	{
		log("[Network]INFO: do_TaskAccept_Ack <-\n{\n%s}", ack.Utf8DebugString().c_str());

		m_messageInfo = "";

		if (ack.has_msginfo())
		{
			log("[Network] errinfo:%s", ack.msginfo().c_str());

			m_messageInfo = ack.msginfo();

			TextFloatLabelLayer::getInstance()->show(m_messageInfo);
		}
		else
		{
			// update models
			Player::getInstance()->setTaskCount(Player::getInstance()->getTaskCount()-1);

			Player::getInstance()->setCoin(ack.coin());

			Player::getInstance()->setDiamond(ack.diamond());

		}

		callUIHandler("TaskAccept");
	}
	else
	{
		log("[Network]ERR:unknown protobuf message");
	}
}

void Network::do_StageList_Req(std::function<void(const AckInfo &ackInfo)> ackUIHandler)
{
	registerAckUIHandler("StageList", ackUIHandler);

	StageList_Req req;

	req.set_token(Player::getInstance()->getToken());

	std::string output;

	if (req.SerializeToString(&output))
	{
		log("[Network]INFO: do_StageList_Req->\n{\n%s}", req.Utf8DebugString().c_str());

		sendData("StageList" , output.c_str(), output.length());
	}
}

void Network::do_StageList_Ack(const std::string &dataStr)
{
	StageList_Ack ack;

	if (ack.ParseFromString(dataStr))
	{
		log("[Network]INFO: do_StageList_Ack <-\n{\n%s}", ack.Utf8DebugString().c_str());

		m_messageInfo = "";

		if (ack.has_msginfo())
		{
			log("[Network] errinfo:%s", ack.msginfo().c_str());

			m_messageInfo = ack.msginfo();

			TextFloatLabelLayer::getInstance()->show(m_messageInfo);
		}
		else
		{
			// update models
			auto stageList = Player::getInstance()->getStageList();

			//save star total
			Player::getInstance()->setLevelStar(ack.sumstar());
			Player::getInstance()->setLevelNewStage(ack.newstage());

			stageList->clear();

			for (int i = 0; i < ack.stagelist_size(); i++)
			{
				auto item = ack.stagelist(i);

				auto stageListItem = StageListItem::create();

				stageListItem->stageId = item.stageid();

				stageListItem->star = item.star();


				stageList->pushBack(stageListItem);
			}

		}

		callUIHandler("StageList");
	}
	else
	{
		log("[Network]ERR:unknown protobuf message from StageList");
	}
}
void Network::do_TaskList_Req(std::function<void(const AckInfo &ackInfo)> ackUIHandler)
{
	registerAckUIHandler("TaskList", ackUIHandler);

	TaskList_Req req;

	req.set_token(Player::getInstance()->getToken());

	std::string output;

	if (req.SerializeToString(&output))
	{
		log("[Network]INFO: do_TaskList_Req->\n{\n%s}", req.Utf8DebugString().c_str());

		sendData("TaskList" , output.c_str(), output.length());
	}

}

void Network::do_TaskList_Ack(const std::string &dataStr)
{
	TaskList_Ack ack;

	if (ack.ParseFromString(dataStr))
	{
		log("[Network]INFO: do_TaskList_Ack <-\n{\n%s}", ack.Utf8DebugString().c_str());

		m_messageInfo = "";

		if (ack.has_msginfo())
		{
			log("[Network] errinfo:%s", ack.msginfo().c_str());

			m_messageInfo = ack.msginfo();

			TextFloatLabelLayer::getInstance()->show(m_messageInfo);
		}
		else
		{
			// update models
			auto taskList = Player::getInstance()->getTaskList();

			taskList->clear();

			for (int i = 0; i < ack.tasklist_size(); i++)
			{
				auto item = ack.tasklist(i);

				auto taskListItem = TaskListItem::create();

				taskListItem->taskId = item.taskid();

				taskListItem->taskState = item.tasktype();

				taskListItem->taskAllTimes = item.total();


				taskListItem->taskTimes = item.present();

				taskList->pushBack(taskListItem);
			}

			// taskList->reverse();
		}

		callUIHandler("TaskList");
	}
	else
	{
		log("[Network]ERR:unknown protobuf message");
	}
}

// Friend Info
void Network::do_FriendInfo_Req(int playerId, std::function<void(const AckInfo &ackInfo)> ackUIHandler)
{
	registerAckUIHandler("FriendInfo", ackUIHandler);

	FriendInfo_Req req;

	req.set_token(Player::getInstance()->getToken());

	req.set_playerid(playerId);

	std::string output;

	if (req.SerializeToString(&output))
	{
		log("[Network]INFO: do_FriendInfo_Req->\n{\n%s}", req.Utf8DebugString().c_str());

		sendData("FriendInfo" , output.c_str(), output.length());
	}
}
void Network::do_FriendInfo_Ack(const std::string &dataStr)
{
	FriendInfo_Ack ack;

	if (ack.ParseFromString(dataStr))
	{
		log("[Network]INFO: do_FriendInfo_Ack <-\n{\n%s}", ack.Utf8DebugString().c_str());

		m_messageInfo = "";

		if (ack.has_msginfo())
		{
			log("[Network] errinfo:%s", ack.msginfo().c_str());

			m_messageInfo = ack.msginfo();

			TextFloatLabelLayer::getInstance()->show(m_messageInfo);
		}
		else
		{
			// update models
			Player::getInstance()->setHighestScore(ack.score());

			Player::getInstance()->setHighestLevels(ack.stage());
		}

		callUIHandler("FriendInfo");
	}
	else
	{
		log("[Network]ERR:unknown protobuf message");
	}
}

// Choose Role
void Network::do_ChooseRole_Req(int roleType, std::function<void(const AckInfo &ackInfo)> ackUIHandler)
{
	registerAckUIHandler("ChooseRole", ackUIHandler);

	ChooseRole_Req req;

	req.set_token(Player::getInstance()->getToken());

	req.set_roletype(roleType);

	std::string output;

	if (req.SerializeToString(&output))
	{
		log("[Network]INFO: do_ChooseRole_Req->\n{\n%s}", req.Utf8DebugString().c_str());

		sendData("ChooseRole" , output.c_str(), output.length());
	}
}
void Network::do_ChooseRole_Ack(const std::string &dataStr)
{
	ChooseRole_Ack ack;

	if (ack.ParseFromString(dataStr))
	{
		log("[Network]INFO: do_ChooseRole_Ack <-\n{\n%s}", ack.Utf8DebugString().c_str());

		m_messageInfo = "";

		if (ack.has_msginfo())
		{
			log("[Network] errinfo:%s", ack.msginfo().c_str());

			m_messageInfo = ack.msginfo();

			TextFloatLabelLayer::getInstance()->show(m_messageInfo);
		}
		else
		{
			// update models

		}

		callUIHandler("ChooseRole");
	}
	else
	{
		log("[Network]ERR:unknown protobuf message");
	}
}

// Send Heart
void Network::do_SendHeart_Req(int playerId , std::function<void(const AckInfo &ackInfo)> ackUIHandler)
{
	registerAckUIHandler("SendHeart", ackUIHandler);

	SendHeart_Req req;

	req.set_token(Player::getInstance()->getToken());

	req.set_playerid(playerId);

	std::string output;

	if (req.SerializeToString(&output))
	{
		log("[Network]INFO: do_SendHeart_Req->\n{\n%s}", req.Utf8DebugString().c_str());

		sendData("SendHeart" , output.c_str(), output.length());
	}
}
void Network::do_SendHeart_Ack(const std::string &dataStr)
{
	SendHeart_Ack ack;

	if (ack.ParseFromString(dataStr))
	{
		log("[Network]INFO: do_SendHeart_Ack <-\n{\n%s}", ack.Utf8DebugString().c_str());

		m_messageInfo = "";

		if (ack.has_msginfo())
		{
			log("[Network] errinfo:%s", ack.msginfo().c_str());

			m_messageInfo = ack.msginfo();

			TextFloatLabelLayer::getInstance()->show(m_messageInfo);
		}
		else
		{
			// update models

		}

		callUIHandler("SendHeart");
	}
	else
	{
		log("[Network]ERR:unknown protobuf message");
	}
}

// Up Heart
void Network::do_UpHeart_Req(std::function<void(const AckInfo &ackInfo)> ackUIHandler)
{
	registerAckUIHandler("UpHeart", ackUIHandler);

	UpHeart_Req req;

	req.set_token(Player::getInstance()->getToken());

	std::string output;

	if (req.SerializeToString(&output))
	{
		log("[Network]INFO: do_UpHeart_Req->\n{\n%s}", req.Utf8DebugString().c_str());

		sendData("UpHeart" , output.c_str(), output.length());
	}
}
void Network::do_UpHeart_Ack(const std::string &dataStr)
{
	UpHeart_Ack ack;

	if (ack.ParseFromString(dataStr))
	{
		log("[Network]INFO: do_UpHeart_Ack <-\n{\n%s}", ack.Utf8DebugString().c_str());

		m_messageInfo = "";

		if (ack.has_msginfo())
		{
			log("[Network] errinfo:%s", ack.msginfo().c_str());

			m_messageInfo = ack.msginfo();

			TextFloatLabelLayer::getInstance()->show(m_messageInfo);
		}
		else
		{
			// update models
			Player::getInstance()->setStar(ack.heart());

			Player::getInstance()->setRemainTime(static_cast<float>(ack.remaintime()));

			Player::getInstance()->setMaxTime(static_cast<float>(ack.maxtime()));
		}

		callUIHandler("UpHeart");
	}
	else
	{
		log("[Network]ERR:unknown protobuf message");
	}
}

//ChooseStage
void Network::do_ChooseStage_Req(int stage,std::function<void(const AckInfo &ackInfo)> ackUIHandler)
{
	registerAckUIHandler("ChooseStage", ackUIHandler);

	ChooseStage_Req req;

	req.set_token(Player::getInstance()->getToken());
	
	req.set_stageid(stage);

	std::string output;

	if (req.SerializeToString(&output))
	{
		log("[Network]INFO: do_ChooseStage_Req->\n{\n%s}", req.Utf8DebugString().c_str());

		sendData("ChooseStage" , output.c_str(), output.length());
	}
}

void Network::do_ChooseStage_Ack(const std::string &dataStr)
{
	ChooseStage_Ack ack;

	if (ack.ParseFromString(dataStr))
	{
		log("[Network]INFO: do_ChooseStage_Ack <-\n{\n%s}", ack.Utf8DebugString().c_str());

		m_messageInfo = "";

		if (ack.has_msginfo())
		{
			log("[Network] errinfo:%s", ack.msginfo().c_str());

			m_messageInfo = ack.msginfo();

			TextFloatLabelLayer::getInstance()->show(m_messageInfo);
		}
		else
		{
			// update models

			
		}

		callUIHandler("ChooseStage");
	}
	else
	{
		log("[Network]ERR:unknown protobuf message");
	}
}

// Role List
void Network::do_RoleList_Req(std::function<void(const AckInfo &ackInfo)> ackUIHandler)
{
	registerAckUIHandler("RoleList", ackUIHandler);

	RoleList_Req req;

	req.set_token(Player::getInstance()->getToken());

	std::string output;

	if (req.SerializeToString(&output))
	{
		log("[Network]INFO: do_RoleList_Req->\n{\n%s}", req.Utf8DebugString().c_str());

		sendData("RoleList" , output.c_str(), output.length());
	}
}
void Network::do_RoleList_Ack(const std::string &dataStr)
{
	RoleList_Ack ack;

	if (ack.ParseFromString(dataStr))
	{
		log("[Network]INFO: do_RoleList_Ack <-\n{\n%s}", ack.Utf8DebugString().c_str());

		m_messageInfo = "";

		if (ack.has_msginfo())
		{
			log("[Network] errinfo:%s", ack.msginfo().c_str());

			m_messageInfo = ack.msginfo();

			TextFloatLabelLayer::getInstance()->show(m_messageInfo);
		}
		else
		{
			// update models
			auto roleList = Player::getInstance()->getRoleList();

			roleList->clear();

			for (int i = 0; i < ack.rolelist_size(); i++)
			{
				auto item = ack.rolelist(i);

				auto _roleListItem = RoleListItem::create();

				_roleListItem->roleType = item.roletype();

				_roleListItem->level = item.level();

				_roleListItem->breakType = item.breaktype();

				_roleListItem->isMax = item.ismax();

				roleList->pushBack(_roleListItem);
			}
		}

		callUIHandler("RoleList");
	}
	else
	{
		log("[Network]ERR:unknown protobuf message");
	}
}

// Buy Role
void Network::do_BuyRole_Req(int roleType,std::function<void(const AckInfo &ackInfo)> ackUIHandler)
{
	registerAckUIHandler("BuyRole", ackUIHandler);

	BuyRole_Req req;

	req.set_token(Player::getInstance()->getToken());

	req.set_roletype(roleType);

	std::string output;

	if (req.SerializeToString(&output))
	{
		log("[Network]INFO: do_BuyRole_Req->\n{\n%s}", req.Utf8DebugString().c_str());

		sendData("BuyRole" , output.c_str(), output.length());
	}
}
void Network::do_BuyRole_Ack(const std::string &dataStr)
{
	BuyRole_Ack ack;

	if (ack.ParseFromString(dataStr))
	{
		log("[Network]INFO: do_BuyRole_Ack <-\n{\n%s}", ack.Utf8DebugString().c_str());

		m_messageInfo = "";

		if (ack.has_msginfo())
		{
			log("[Network] errinfo:%s", ack.msginfo().c_str());

			m_messageInfo = ack.msginfo();

			TextFloatLabelLayer::getInstance()->show(m_messageInfo);
		}
		else
		{
			// update models
			Player::getInstance()->setCoin(ack.coin());

			Player::getInstance()->setDiamond(ack.diamond());

			auto _roleListItem = RoleListItem::create();

			_roleListItem->roleType = ack.roletype();

			_roleListItem->level = 0;

			_roleListItem->breakType = false;

			_roleListItem->isMax = false;

			Player::getInstance()->getRoleList()->pushBack(_roleListItem);
		}

		callUIHandler("BuyRole");
	}
	else
	{
		log("[Network]ERR:unknown protobuf message");
	}
}


void Network::do_StartGame_Req(int helperPlayerId, std::function<void(const AckInfo &ackInfo)> ackUIHandler)
{
	registerAckUIHandler("StartGame", ackUIHandler);

	StartGame_Req req;

	req.set_token(Player::getInstance()->getToken());

	req.set_playerid(helperPlayerId);

	std::string output;

	if (req.SerializeToString(&output))
	{
		log("[Network]INFO: do_StartGame_Req->\n{\n%s}", req.Utf8DebugString().c_str());

		sendData("StartGame" , output.c_str(), output.length());
	}
}


void Network::do_StartGame_Ack(const std::string &dataStr)
{
	StartGame_Ack ack;

	if (ack.ParseFromString(dataStr))
	{
		log("[Network]INFO: StartGame_Ack <-\n{\n%s}", ack.Utf8DebugString().c_str());

		m_messageInfo = "";

		if (ack.has_msginfo())
		{
			log("[Network] errinfo:%s", ack.msginfo().c_str());

			m_messageInfo = ack.msginfo();

			TextFloatLabelLayer::getInstance()->show(m_messageInfo);
		}
		else
		{
			// update models
			Player::getInstance()->setHelpCount(ack.helpcount());
		}

		callUIHandler("StartGame");
	}
	else
	{
		log("[Network]ERR:unknown protobuf message");
	}
}


// update task count and mail count
void Network::do_TaskCount_Req(std::function<void(const AckInfo &ackInfo)> ackUIHandler)
{
	registerAckUIHandler("TaskCount", ackUIHandler);

	TaskCount_Req req;

	req.set_token(Player::getInstance()->getToken());

	std::string output;

	if (req.SerializeToString(&output))
	{
		log("[Network]INFO: do_TaskCount_Req->\n{\n%s}", req.Utf8DebugString().c_str());

		sendData("TaskCount" , output.c_str(), output.length());
	}
}

void Network::do_TaskCount_Ack(const std::string &dataStr)
{
	TaskCount_Ack ack;

	if (ack.ParseFromString(dataStr))
	{
		log("[Network]INFO: TaskCount_Ack <-\n{\n%s}", ack.Utf8DebugString().c_str());

		m_messageInfo = "";

		if (ack.has_msginfo())
		{
			log("[Network] errinfo:%s", ack.msginfo().c_str());

			m_messageInfo = ack.msginfo();

			TextFloatLabelLayer::getInstance()->show(m_messageInfo);
		}
		else
		{
			// update models
			Player::getInstance()->setMailCount(ack.mailcount());

			Player::getInstance()->setTaskCount(ack.taskcount());

			//Player::getInstance()->set
		}

		callUIHandler("TaskCount");
	}
	else
	{
		log("[Network]ERR:unknown protobuf message");
	}
}

// stage Game
void Network::do_StageGame_Req(std::function<void(const AckInfo &ackInfo)> ackUIHandler)
{
	registerAckUIHandler("StageGame", ackUIHandler);

	StageGame_Req req;

	req.set_token(Player::getInstance()->getToken());

	std::string output;

	if (req.SerializeToString(&output))
	{
		log("[Network]INFO: do_StageGame_Req->\n{\n%s}", req.Utf8DebugString().c_str());

		sendData("StageGame" , output.c_str(), output.length());
	}
}

void Network::do_StageGame_Ack(const std::string &dataStr)
{
	StageGame_Ack ack;

	if (ack.ParseFromString(dataStr))
	{
		log("[Network]INFO: StageGame_Ack <-\n{\n%s}", ack.Utf8DebugString().c_str());

		m_messageInfo = "";

		if (ack.has_msginfo())
		{
			log("[Network] errinfo:%s", ack.msginfo().c_str());

			m_messageInfo = ack.msginfo();

			TextFloatLabelLayer::getInstance()->show(m_messageInfo);
		}
		else
		{
			// update models
		}

		callUIHandler("StageGame");
	}
	else
	{
		log("[Network]ERR:unknown protobuf message");
	}
}

// gift
void Network::do_Gift_Req(std::string giftId,std::function<void(const AckInfo &ackInfo)> ackUIHandler)
{
	registerAckUIHandler("Gift", ackUIHandler);

	Gift_Req req;

	req.set_token(Player::getInstance()->getToken());

	req.set_giftid(giftId);

	std::string output;

	if (req.SerializeToString(&output))
	{
		log("[Network]INFO: do_Gift_Req->\n{\n%s}", req.Utf8DebugString().c_str());

		sendData("Gift" , output.c_str(), output.length());
	}
}
void Network::do_Gift_Ack(const std::string &dataStr)
{
	Gift_Ack ack;

	if (ack.ParseFromString(dataStr))
	{
		log("[Network]INFO: Gift_Ack <-\n{\n%s}", ack.Utf8DebugString().c_str());

		m_messageInfo = "";

		if (ack.has_msginfo())
		{
			log("[Network] errinfo:%s", ack.msginfo().c_str());

			m_messageInfo = ack.msginfo();

			TextFloatLabelLayer::getInstance()->show(m_messageInfo);
		}
		else
		{
			// update models
			Player::getInstance()->setCoin(ack.coin());

			Player::getInstance()->setDiamond(ack.diamond());

			Player::getInstance()->setStar(ack.heart());

			Player::getInstance()->setGiftInfo(ack.info());
		}

		callUIHandler("Gift");
	}
	else
	{
		log("[Network]ERR:unknown protobuf message");
	}
}
///////////////////////////////////////keywork: setNickname///////////////////////////////////////////////

//[TODO 4] define request and ack function

Network::Network()
{
	//register ack handler here
	registerAckHandler("UserLogin", CC_CALLBACK_1(Network::do_UserLogin_Ack, this));
	registerAckHandler("UserLoginReward", CC_CALLBACK_1(Network::do_UserLoginReward_Ack, this));
	registerAckHandler("Notice", CC_CALLBACK_1(Network::do_Notice_Ack, this));
	registerAckHandler("UserRegister", CC_CALLBACK_1(Network::do_UserRegister_Ack, this));
	registerAckHandler("SetNickname", CC_CALLBACK_1(Network::do_SetNickname_Ack, this));
	registerAckHandler("UpRole", CC_CALLBACK_1(Network::do_UpRole_Ack, this));
	registerAckHandler("RankList", CC_CALLBACK_1(Network::do_RankList_Ack, this));
	registerAckHandler("lastRankList", CC_CALLBACK_1(Network::do_lastRankList_Ack, this));
	registerAckHandler("Opinion", CC_CALLBACK_1(Network::do_Opinion_Ack, this));
	registerAckHandler("SetControl", CC_CALLBACK_1(Network::do_SetControl_Ack, this));
	registerAckHandler("FriendList", CC_CALLBACK_1(Network::do_FriendList_Ack, this));
	registerAckHandler("AddFriend", CC_CALLBACK_1(Network::do_AddFriend_Ack, this));
	registerAckHandler("DelFriend", CC_CALLBACK_1(Network::do_DelFriend_Ack, this));
	registerAckHandler("MailList", CC_CALLBACK_1(Network::do_MailList_Ack, this));
	registerAckHandler("GetMailProp", CC_CALLBACK_1(Network::do_GetMailProp_Ack, this));
	registerAckHandler("MailDel", CC_CALLBACK_1(Network::do_MailDel_Ack, this));
	registerAckHandler("Result", CC_CALLBACK_1(Network::do_Result_Ack, this));
	registerAckHandler("ShopBuy", CC_CALLBACK_1(Network::do_ShopBuy_Ack, this));
	registerAckHandler("ItemAdd", CC_CALLBACK_1(Network::do_ItemAdd_Ack, this));
	registerAckHandler("ItemList", CC_CALLBACK_1(Network::do_ItemList_Ack, this));
	registerAckHandler("TaskList", CC_CALLBACK_1(Network::do_TaskList_Ack, this));
	registerAckHandler("TaskAccept", CC_CALLBACK_1(Network::do_TaskAccept_Ack, this));
	registerAckHandler("StageList", CC_CALLBACK_1(Network::do_StageList_Ack, this));
	registerAckHandler("FriendInfo", CC_CALLBACK_1(Network::do_FriendInfo_Ack, this));
	registerAckHandler("ChooseRole", CC_CALLBACK_1(Network::do_ChooseRole_Ack, this));
	registerAckHandler("SendHeart", CC_CALLBACK_1(Network::do_SendHeart_Ack, this));
	registerAckHandler("UpHeart", CC_CALLBACK_1(Network::do_UpHeart_Ack, this));
	registerAckHandler("RoleList", CC_CALLBACK_1(Network::do_RoleList_Ack, this));
	registerAckHandler("BuyRole", CC_CALLBACK_1(Network::do_BuyRole_Ack, this));
	registerAckHandler("ResultRank", CC_CALLBACK_1(Network::do_ResultRank_Ack, this));
	registerAckHandler("StageResult", CC_CALLBACK_1(Network::do_StageResult_Ack, this));
	registerAckHandler("StartGame", CC_CALLBACK_1(Network::do_StartGame_Ack, this));
	registerAckHandler("ChooseStage", CC_CALLBACK_1(Network::do_ChooseStage_Ack, this));
	registerAckHandler("TaskCount", CC_CALLBACK_1(Network::do_TaskCount_Ack, this));
	registerAckHandler("StageGame", CC_CALLBACK_1(Network::do_StageGame_Ack, this));
	registerAckHandler("Gift", CC_CALLBACK_1(Network::do_Gift_Ack, this));
	//[TODO 5] register ack handler
}

////////////////////////////!!! Don't Edit Flowing Code !!!/////////////////////////////////////////////
void Network::registerAckHandler(std::string msgType, std::function<void(const std::string &dataStr)> parseFunc)
{
	AckHandler *ackHandler = AckHandler::create();

	ackHandler->parseData = parseFunc;

	if (nullptr == m_ackHandlers.at(msgType))
	{
		ackHandler->retain();

		m_ackHandlers.insert(msgType, ackHandler);
	}
	else
	{
		log("[Network]ERR: msg type has been registered");
	}
}

void Network::registerAckUIHandler(std::string msgType, std::function<void(const AckInfo &ackInfo)> ackUIHandler)
{
	m_ackUIHandlers.insert(msgType, AckUIHandler::create(ackUIHandler));
}

void Network::callUIHandler(std::string msgType)
{
	auto ackUIHandlerObject = m_ackUIHandlers.at(msgType);

	m_ackInfo.setMessageInfo(m_messageInfo);

	if (ackUIHandlerObject)
	{
		ackUIHandlerObject->ackUIHandler(m_ackInfo);
	}

	//NetworkConnectLayer::getInstance()->networkConnectStop();
}

Network* Network::getInstance()
{
	if (nullptr == g_Network)
	{
		g_Network = new Network();
	}

	return g_Network;
}

void Network::sendData(std::string msgType, const char *data, const unsigned long dataSize)
{
	HttpHelper::getInstance()->sendData(msgType, data, dataSize);
}

void Network::parseAckData(const std::string msgType, const std::string &dataStr)
{
	AckHandler *ackHandler = m_ackHandlers.at(msgType);

	if (nullptr != ackHandler)
	{
		ackHandler->parseData(dataStr);
	}
	else
	{
		log("[Network]ERR: no ackHandler for msg-%s", msgType.c_str());
	}
}

AckHandler* AckHandler::create()
{
	AckHandler* ackHandler = new AckHandler();

	if (ackHandler)
	{
		ackHandler->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ackHandler);
	}

	return ackHandler;
}



