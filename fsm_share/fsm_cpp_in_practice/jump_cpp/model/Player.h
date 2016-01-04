//
//  Player.h
//  HiFly
//
//  Created by chris li on 14-4-21.
//
//

#ifndef __HiFly__Player__
#define __HiFly__Player__
#include "cocos2d.h"
#include "Model.h"
#include "RankListItem.h"
#include "FriendListItem.h"
#include "MailListItem.h"
#include "PropListItem.h"
#include "TaskListItem.h"
#include "StageListItem.h"
#include "RoleListItem.h"

class Player : public Model
{
public:
    static Player* getInstance();
    
    virtual void dataUpdated();
    
private:
    Player() {}
    
    virtual ~Player() {}
    
    void init();
    
    //members
    //user id
    CC_SYNTHESIZE(std::string, m_userId, UserId);
    
    CC_SYNTHESIZE(int, m_myPlayerId, MyPlayerId);
    
    //user name
    CC_SYNTHESIZE(std::string, m_userName, UserName);

	//nickname
    CC_SYNTHESIZE(std::string, m_nickname, Nickname);

	//level
    CC_SYNTHESIZE(int, m_level, Level);

	//star
    CC_SYNTHESIZE(int, m_star, Star);

	//level star
	CC_SYNTHESIZE(int, m_levelStar, LevelStar);

	//current level
	CC_SYNTHESIZE(int, m_currentLevel, CurrentLevel);

	//star remain Time
	CC_SYNTHESIZE(float, m_remainTime, RemainTime);

	//star max Time
	CC_SYNTHESIZE(float, m_maxTime, MaxTime);

	//star is update
	CC_SYNTHESIZE(bool, m_isStarUpdate, IsStarUpdate);

	//coin
    CC_SYNTHESIZE(int, m_coin, Coin);

	//diamond
	CC_SYNTHESIZE(int, m_diamond, Diamond);

	//helpCount
	CC_SYNTHESIZE(int, m_helpCount, HelpCount);

	//roleType
	CC_SYNTHESIZE(int, m_roleType, RoleType);
    
    //roleLevel
    CC_SYNTHESIZE(int, m_roleLevel, RoleLevel);

	//mailCount
	CC_SYNTHESIZE(int, m_mailCount, MailCount);
    
    //taskCount
    CC_SYNTHESIZE(int, m_taskCount, TaskCount);

    //history max score
    CC_SYNTHESIZE(int, m_historyMaxScore, HistoryMaxScore);
    
    //0 -- nothing, 1 -- weeek max score, 2 -- history max score
    CC_SYNTHESIZE(int, m_maxScoreType, MaxScoreType);
    
	//Crown Type
	CC_SYNTHESIZE(int, m_crownType, CrownType);

	//weekScore
	CC_SYNTHESIZE(int, m_weekScore, WeekScore);

	//score
	CC_SYNTHESIZE(int, m_score, Score);

	//time
	CC_SYNTHESIZE(int, m_time, Time);

	//content
	CC_SYNTHESIZE(std::string, m_content, Content);

	//loop content
	CC_SYNTHESIZE(std::string, m_loopContent, LoopContent);

	//imgPath
	CC_SYNTHESIZE(std::string, m_imgPath, ImgPath);

	//avtiveCount
	CC_SYNTHESIZE(int, m_avtiveCount, AvtiveCount);

	//loginCount
	CC_SYNTHESIZE(int, m_loginCount, LoginCount);

	//loginCountTotal
	CC_SYNTHESIZE(int, m_loginCountTotal, LoginCountTotal);

	//////////////////////Settings/////////////////////////
	//helpInc
	CC_SYNTHESIZE(bool, m_helpInc, HelpInc);

	//bgmInc
	CC_SYNTHESIZE(int, m_bgmInc, BgmInc);

	//soundInc
	CC_SYNTHESIZE(int, m_soundInc, SoundInc);

	//sensitivityInc
	CC_SYNTHESIZE(int, m_sensitivityInc, SensitivityInc);
	//////////////////////////////////////////////////////
    
    /////////////////// Prop Buy /////////////////////////////
	CC_SYNTHESIZE(int, m_propRandom, PropRandom);

    CC_SYNTHESIZE(int, m_prop1Num, Prop1Num);
    
    CC_SYNTHESIZE(int, m_prop2Num, Prop2Num);
    
    CC_SYNTHESIZE(int, m_prop3Num, Prop3Num);
    
    CC_SYNTHESIZE(int, m_prop4Num, Prop4Num);
    //////////////////////////////////////////////////////////

	////////////////////////friend info///////////////////////
	CC_SYNTHESIZE(int, m_highestScore, HighestScore);

	CC_SYNTHESIZE(int, m_highestLevels, HighestLevels);
	//////////////////////////////////////////////////////////

	/////////////////////Stage Info//////////////////////////
	CC_SYNTHESIZE(int, m_levelNewStage, LevelNewStage);
	/////////////////////////////////////////////////////////
	//token
	CC_SYNTHESIZE(std::string, m_token, Token);
    
    //platform rank list
    CC_SYNTHESIZE_READONLY(cocos2d::Vector<RankListItem*>*, m_platformRankList, PlatformRankList);

	//world rank list
    CC_SYNTHESIZE_READONLY(cocos2d::Vector<RankListItem*>*, m_worldRankList, WorldRankList);
    
    //friend list
    CC_SYNTHESIZE_READONLY(cocos2d::Vector<FriendListItem*>*, m_friendList, FriendList);

	//nearby list
    CC_SYNTHESIZE_READONLY(cocos2d::Vector<FriendListItem*>*, m_nearbyList, NearbyList);
    
    //mail list
    CC_SYNTHESIZE_READONLY(cocos2d::Vector<MailListItem*>*, m_mailList, MailList);
    
    //item list
    CC_SYNTHESIZE_READONLY(cocos2d::Vector<PropListItem*>*, m_propList, PropList);
    
    //task list
    CC_SYNTHESIZE_READONLY(cocos2d::Vector<TaskListItem*>*, m_taskList, TaskList);
	//stage list
	CC_SYNTHESIZE_READONLY(cocos2d::Vector<StageListItem*>*, m_stageList, StageList);
	
	//role list
	CC_SYNTHESIZE_READONLY(cocos2d::Vector<RoleListItem*>*, m_roleList, RoleList);

	//last rank list
	CC_SYNTHESIZE_READONLY(cocos2d::Vector<RankListItem*>*, m_lastRankList, lastRankList);

	//notice
	CC_SYNTHESIZE(std::string, m_noticeMsg, NoticeMsg);

	//count
	CC_SYNTHESIZE(int, m_myPlayerCount, MyPlayerCount);

	//control homescene go to levelChooseLayer
	CC_SYNTHESIZE(bool, m_levelChooseSwitch, LevelChooseSwitch);


	//isMore
	CC_SYNTHESIZE(bool, m_isMore, IsMore);

	//gift info
	CC_SYNTHESIZE(std::string, m_giftInfo, GiftInfo);

	//first all star complete the levels
	CC_SYNTHESIZE(bool, m_isFirstPerfect, IsFirstPerfect);

};

#endif /* defined(__HiFly__Player__) */
