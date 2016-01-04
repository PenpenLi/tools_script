//
//  Player.cpp
//  HiFly
//  Player is root of all modle tree
//  Created by chris li on 14-4-21.
//
//

#include "Player.h"
USING_NS_CC;

static Player *g_Player = nullptr;

Player* Player::getInstance()
{
    if (nullptr == g_Player)
    {
        g_Player = new Player();
        
        g_Player->init();
    }
    
    return g_Player;
}

void Player::dataUpdated()
{
    /*
    std::string eventName = EVENT_MODEL_PLAYER;
    
    EventCustom event(eventName);
    
    //event.setUserData(&contact);
    
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    */
}

void Player::init()
{
	m_isStarUpdate = false;

	m_remainTime = 0;

    m_platformRankList = new cocos2d::Vector<RankListItem*>();

	m_worldRankList = new cocos2d::Vector<RankListItem*>();
    
    m_friendList = new cocos2d::Vector<FriendListItem*>();

	m_nearbyList = new cocos2d::Vector<FriendListItem*>();
    
    m_mailList = new cocos2d::Vector<MailListItem*>();
    
    m_propList = new cocos2d::Vector<PropListItem*>();
    
    m_taskList = new cocos2d::Vector<TaskListItem*>();
	m_stageList = new cocos2d::Vector<StageListItem*>();
	
	m_roleList = new cocos2d::Vector<RoleListItem*>();

	m_lastRankList = new cocos2d::Vector<RankListItem*>();
	
	m_propRandom = 0;

	m_levelChooseSwitch = false;

	m_isMore = false;

	m_isFirstPerfect = false;
}

