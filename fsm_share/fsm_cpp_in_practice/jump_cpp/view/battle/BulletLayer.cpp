//
//  BulletLayer.cpp
//  HiFly
//
//  Created by chris li on 14-5-13.
//
//

#include "BulletLayer.h"
#include "util/GameSound.h"
#include "BattleDefine.h"
#include "LevelBook.h"
#include "LevelHelper.h"

bool BulletLayer::init()
{
    if (Node::init())
    {
        return true;
    }
    
    return false;
}

void BulletLayer::onEnter()
{
    Node::onEnter();
    
    m_layer = LayerColor::create(Color4B::WHITE, Director::getInstance()->getWinSize().width, Director::getInstance()->getWinSize().height);
    
    m_layer->setOpacity(0);
    
    this->addChild(m_layer);
    
    auto rewardLevelsStartListener = EventListenerCustom::create(EVENT_REWARD_LEVELS_START, [this](EventCustom* event)
    {
        log("EVENT_REWARD_LEVELS_START");
        
        this->enterReward();

    });
    _eventDispatcher->addEventListenerWithFixedPriority(rewardLevelsStartListener, 1);
    
    auto rewardLevelsEndListener = EventListenerCustom::create(EVENT_REWARD_LEVELS_END, [this](EventCustom* event)
    {
        log("EVENT_REWARD_LEVELS_END");
        
        auto rewardLevel = static_cast<int*>(event->getUserData());
        
        this->exitReward(*rewardLevel);
    });
    _eventDispatcher->addEventListenerWithFixedPriority(rewardLevelsEndListener, 1);
    
    auto loadResListener = EventListenerCustom::create(EVENT_LOAD_RES, [this](EventCustom* event) {
    
        log("EVENT_LOAD_RES");
                                                                   
        //this->exitReward();
    });
    _eventDispatcher->addEventListenerWithFixedPriority(loadResListener, 1);
}

void BulletLayer::onExit()
{
    _eventDispatcher->removeCustomEventListeners(EVENT_REWARD_LEVELS_END);
    
    _eventDispatcher->removeCustomEventListeners(EVENT_REWARD_LEVELS_START);
    
    _eventDispatcher->removeCustomEventListeners(EVENT_LOAD_RES);
    
    Node::onExit();
}

void BulletLayer::sceneUpdate(float dt)
{
    
}

Vector<Bullet*>* BulletLayer::getBullets()
{
    return &m_bullets;
}

void BulletLayer::enterReward()
{
    m_layer->setOpacity(0);
    
    auto action1 = cocos2d::FadeTo::create(1, 255);
    
    auto action2 = cocos2d::FadeTo::create(1, 0);
    
    m_layer->runAction(cocos2d::Sequence::create(action1, action2, CallFunc::create([](){}), NULL));
}

void BulletLayer::exitReward(int nextRewardLevel)
{
    /*
    m_layer->setOpacity(0);
    
    auto action1 = cocos2d::FadeTo::create(1, 255);
    
    auto action2 = cocos2d::FadeTo::create(1, 0);
    
    m_layer->runAction(cocos2d::Sequence::create(action1, action2, CallFunc::create([](){}), NULL));
    */
    m_nextLevel = nextRewardLevel + 30000;
    
    m_nextLevelStr = LevelBook::getInstance()->getLevelConfig(m_nextLevel);
    
    log("[BulletLayer::loadLevel] %s", m_nextLevelStr.c_str());
    
    scheduleUpdate();
    
    m_layer->setOpacity(255);
}

void BulletLayer::update(float delta)
{
    std::string ladderSetFileName = getLadderSetFileName();
    
    log("[BulletLayer] load: %s", ladderSetFileName.c_str());
    
    m_layer->setOpacity(m_layer->getOpacity()-20);
    
    if (ladderSetFileName == "")
    {
        m_layer->setOpacity(0);
        unscheduleUpdate();
    }
    
    else
    {
        LevelHelper::getInstance()->loadRewardLadderSet(ladderSetFileName);
    }
}


std::string BulletLayer::getLadderSetFileName()
{
    int pos = m_nextLevelStr.find_first_of('-');
    
    if (pos != std::string::npos)
    {
        auto ladderSetName = m_nextLevelStr.substr(0, pos);
        
        ladderSetName += ".ccbi";
        
        m_nextLevelStr = m_nextLevelStr.substr(pos+1);
        
        return ladderSetName;
    }
    
    return "";
}


