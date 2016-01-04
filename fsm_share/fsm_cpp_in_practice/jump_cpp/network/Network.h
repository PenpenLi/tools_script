//
//  Network.h
//  HiFly
//
//  Created by chris li on 14-4-18.
//
//

#ifndef __HiFly__Network__
#define __HiFly__Network__

#include "cocos2d.h"
#include "protobuf/ProtobufMsg.h"

class AckHandler : public cocos2d::Ref
{
public:
    static AckHandler* create();
    
    //std::function<void(const char*)> parseData;
    std::function<void(const std::string &dataStr)> parseData;

private:
    AckHandler() {};
    
    virtual ~AckHandler() {};
};


class AckInfo : public cocos2d::Ref
{
public:
    AckInfo() {}
    
    virtual ~AckInfo() {}
    
    CC_SYNTHESIZE(std::string, m_messageInfo, MessageInfo);
};


class AckUIHandler : public cocos2d::Ref
{
public:
    static AckUIHandler* create(std::function<void(const AckInfo &ackInfo)> ackUIHandler)
    {
        auto ret = new AckUIHandler();
        
        ret->autorelease();
        
        ret->ackUIHandler = ackUIHandler;
        
        return ret;
    }
    
    std::function<void(const AckInfo &ackInfo)> ackUIHandler;
};

class Network
{
public:
    //login request
    void do_UserLogin_Req(std::string username, std::string password, std::function<void(const AckInfo &ackInfo)> ackUIHandler);
    void do_UserLogin_Ack(const std::string &dataStr);

	//login Reward
	void do_UserLoginReward_Req(int rewardType, std::function<void(const AckInfo &ackInfo)> ackUIHandler);
	void do_UserLoginReward_Ack(const std::string &dataStr);
    
	//notice
	void do_Notice_Req(int rewardType, std::function<void(const AckInfo &ackInfo)> ackUIHandler);
	void do_Notice_Ack(const std::string &dataStr);

    //register
    void do_UserRegister_Req(std::string username, std::string password, std::function<void(const AckInfo &ackInfo)> ackUIHandler);
    void do_UserRegister_Ack(const std::string &dataStr);
    
    //set nickname
	void do_SetNickname_Req(std::string nickname, std::function<void(const AckInfo &ackInfo)> ackUIHandler);
    void do_SetNickname_Ack(const std::string &dataStr);
    
    //upgrade role level
    void do_UpRole_Req(int roleType, std::function<void(const AckInfo &ackInfo)> ackUIHandler);
    void do_UpRole_Ack(const std::string &dataStr);
    
    //rank list, rankType ---world or platform, scoreType --- last week or this week
    void do_RankList_Req(int rankType, int scoreType, std::function<void(const AckInfo &ackInfo)> ackUIHandler);
    void do_RankList_Ack(const std::string &dataStr);
    
	//last rank list
	void do_lastRankList_Req(std::function<void(const AckInfo &ackInfo)> ackUIHandler);
	void do_lastRankList_Ack(const std::string &dataStr);

    //friend list
    void do_FriendList_Req(int friendListType, std::function<void(const AckInfo &ackInfo)> ackUIHandler);
    void do_FriendList_Ack(const std::string &dataStr);
    
    //add friend. addType == 1 addFriend, addType == 2 inviteFriend
    void do_AddFriend_Req(int playerId, int addType, std::function<void(const AckInfo &ackInfo)> ackUIHandler);
    void do_AddFriend_Ack(const std::string &dataStr);
    
    //delete friend
    void do_DelFriend_Req(int playerId, int addType, std::function<void(const AckInfo &ackInfo)> ackUIHandler);
    void do_DelFriend_Ack(const std::string &dataStr);
    
    //set opinion setting
    void do_Opinion_Req(std::string &opinionStr, std::function<void(const AckInfo &ackInfo)> ackUIHandler);
    void do_Opinion_Ack(const std::string &dataStr);
    
    //Set Control Setting
    void do_SetControl_Req(bool FriendsHeart, int Music, int Sound, int Sensitivity, std::function<void(const AckInfo &ackInfo)> ackUIHandler);
    void do_SetControl_Ack(const std::string &dataStr);
    
    // Mail List
    void do_MailList_Req(std::function<void(const AckInfo &ackInfo)> ackUIHandler);
    void do_MailList_Ack(const std::string &dataStr);
    
    // Get Mail Prop
    void do_GetMailProp_Req(int mailId, std::function<void(const AckInfo &ackInfo)> ackUIHandler);
    void do_GetMailProp_Ack(const std::string &dataStr);
    
    // Del Mail
    void do_MailDel_Req(int mailId, std::function<void(const AckInfo &ackInfo)> ackUIHandler);
    void do_MailDel_Ack(const std::string &dataStr);
    
    // Game Result
    void do_Result_Req(int score, int coin, int reviveTimes, std::function<void(const AckInfo &ackInfo)> ackUIHandler);
    void do_Result_Ack(const std::string &dataStr);
    
	//Result Rank
	void do_ResultRank_Req(std::function<void(const AckInfo &ackInfo)> ackUIHandler);
	void do_ResultRank_Ack(const std::string &dataStr);

    // Shop Buy
    void do_ShopBuy_Req(int buyType, int itemId, std::function<void(const AckInfo &ackInfo)> ackUIHandler);
    void do_ShopBuy_Ack(const std::string &dataStr);
    
    // Prop Item buy
    void do_ItemAdd_Req(int itemId, int itemNum, std::function<void(const AckInfo &ackInfo)> ackUIHandler);
    void do_ItemAdd_Ack(const std::string &dataStr);
    
    // Prop Item List
    void do_ItemList_Req(std::function<void(const AckInfo &ackInfo)> ackUIHandler);
    void do_ItemList_Ack(const std::string &dataStr);
    
    // Task List
    void do_TaskList_Req(std::function<void(const AckInfo &ackInfo)> ackUIHandler);
    void do_TaskList_Ack(const std::string &dataStr);

	// Friend Info
	void do_FriendInfo_Req(int playerId, std::function<void(const AckInfo &ackInfo)> ackUIHandler);
	void do_FriendInfo_Ack(const std::string &dataStr);

	// Choose Role
	void do_ChooseRole_Req(int roleType, std::function<void(const AckInfo &ackInfo)> ackUIHandler);
	void do_ChooseRole_Ack(const std::string &dataStr);

	// Send Heart
	void do_SendHeart_Req(int playerId , std::function<void(const AckInfo &ackInfo)> ackUIHandler);
	void do_SendHeart_Ack(const std::string &dataStr);

	// Task Accept
    void do_TaskAccept_Req(int taskId, std::function<void(const AckInfo &ackInfo)> ackUIHandler);
    void do_TaskAccept_Ack(const std::string &dataStr);

	// Up Heart
	void do_UpHeart_Req(std::function<void(const AckInfo &ackInfo)> ackUIHandler);
	void do_UpHeart_Ack(const std::string &dataStr);
	
    // Stage List
    void do_StageList_Req(std::function<void(const AckInfo &ackInfo)> ackUIHandler);
    void do_StageList_Ack(const std::string &dataStr);

	void do_StageResult_Req(int star, std::function<void(const AckInfo &ackInfo)> ackUIHandler);
    void do_StageResult_Ack(const std::string &dataStr);
	
	// Role List
	void do_RoleList_Req(std::function<void(const AckInfo &ackInfo)> ackUIHandler);
	void do_RoleList_Ack(const std::string &dataStr);

	// Choose Limit Stage
	void do_ChooseStage_Req(int stage,std::function<void(const AckInfo &ackInfo)> ackUIHandler);
	void do_ChooseStage_Ack(const std::string &dataStr);

	// Buy Role
	void do_BuyRole_Req(int roleType,std::function<void(const AckInfo &ackInfo)> ackUIHandler);
	void do_BuyRole_Ack(const std::string &dataStr);
    
    // Start Infinite Mode Game
    void do_StartGame_Req(int helperPlayerId, std::function<void(const AckInfo &ackInfo)> ackUIHandler);
    void do_StartGame_Ack(const std::string &dataStr);

	// update task count and mail count
	void do_TaskCount_Req(std::function<void(const AckInfo &ackInfo)> ackUIHandler);
	void do_TaskCount_Ack(const std::string &dataStr);

	// stage Game
	void do_StageGame_Req(std::function<void(const AckInfo &ackInfo)> ackUIHandler);
	void do_StageGame_Ack(const std::string &dataStr);

	// gift
	void do_Gift_Req(std::string giftId,std::function<void(const AckInfo &ackInfo)> ackUIHandler);
    void do_Gift_Ack(const std::string &dataStr);

    //[TODO 3] declear request and ack function
    
    
    static Network* getInstance();
    
    void parseAckData(const std::string msgType, const std::string &dataStr);
    
private:
    Network();
    
    virtual ~Network() {}
    
    void sendData(std::string msgType, const char *data, const unsigned long dataSize);
    
    void registerAckHandler(std::string msgType, std::function<void(const std::string &dataStr)> parseFunc);
    
    void registerAckUIHandler(std::string msgType, std::function<void(const AckInfo &ackInfo)> ackUIhandler);
    
    void callUIHandler(std::string msgType);
    
    cocos2d::Map<std::string, AckHandler*> m_ackHandlers;
    
    cocos2d::Map<std::string, AckUIHandler*> m_ackUIHandlers;
    
    std::string m_messageInfo;
    
    AckInfo m_ackInfo;
};

#endif /* defined(__HiFly__Network__) */
