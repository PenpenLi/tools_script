//
//  BattleScene.cpp
//  HiFly
//
//  Created by chris li on 14-5-6.
//
//

#include "BattleScene.h"
#include "BattleDefine.h"
#include "util/GameSound.h"

USING_NS_CC;

bool BattleScene::init()
{
    if (Scene::init())
    {
        //Director::getInstance()->getEventDispatcher()->setDebugTouchEvent(true);
		//////////////////////////////////////////////
        m_dtData[0]= 0.016;m_dtData[1]= 0.016;m_dtData[2]= 0.016;
        
        
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ladder_bomb_action.plist");
        log("ladder_bomb_action.plist");
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("battle_ui_layer.plist");
        log("battle_ui_layer.plist");
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("props_action_1.plist");
        log("props_action_1.plist");
//		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("torpedo_warning.plist");
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("monster_bomb.plist");
        log("monster_bomb.plist");
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("action_Monster.plist");
        log("action_Monster.plist");
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile("GuardBrand.plist");
        log("GuardBrand.plist");
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile("FriendHelpAndMagnet.plist");
        log("FriendHelpAndMagnet.plist");
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Ready_Go.plist");
        log("Ready_Go.plist");
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("propWarning.plist");
        log("propWarning.plist");
//		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Hide_Ladder.plist");
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Coin_Animation.plist");
        log("Coin_Animation.plist");
		///////////////////////////////////////////////
        
        m_startPuaseCounter = 1;

        m_isPause = true;
        
        m_bgLayer = BgLayer::create();
        
        this->addChild(m_bgLayer);
        
        m_ladderLayer = LadderLayer::create();
        
        this->addChild(m_ladderLayer);
        
        m_heroLayer = HeroLayer::create();
        
        this->addChild(m_heroLayer);
        
        m_uiLayer = BattleUILayer::create();
        
        this->addChild(m_uiLayer);
        
        m_bulletLayer = BulletLayer::create();
        
        this->addChild(m_bulletLayer);

        return true;
    }
    
    return false;
}

void BattleScene::onEnter()
{
    Scene::onEnter();
    
    m_uiLayer->readyGo();
    
    scheduleUpdate();
}

void BattleScene::onExit()
{
    Scene::onExit();
}

void BattleScene::update(float delta)
{
    if (m_isPause) {return;}
    
    m_dtData[0] = m_dtData[1]; m_dtData[1] = m_dtData[2]; m_dtData[2] = delta;
    delta = (m_dtData[0] + m_dtData[1] + m_dtData[2]) * 1.0f /3;
    
    m_bgLayer->sceneUpdate(delta);
    
    m_ladderLayer->sceneUpdate(delta);
    
    m_heroLayer->sceneUpdate(delta);
    
    //m_bulletLayer->sceneUpdate(delta);
    
    //m_uiLayer->sceneUpdate(delta);
    
    // check collide
    checkCollide(m_ladderLayer, m_heroLayer);
}

void BattleScene::checkCollide(CollideInterface *aLayer, CollideInterface *bLayer)
{
    auto aCollideNode = aLayer->getCollideNode();
    
    auto bCollideNode = bLayer->getCollideNode();

    for (auto aNode : (*aCollideNode))
    {
        auto aNodeType = aLayer->getCollideNodeType(aNode);
        
        if (false == aNode->isVisible() || aNodeType == -1) {continue;}
        
        for (auto bNode : (*bCollideNode))
        {
            auto bNodeType = bLayer->getCollideNodeType(bNode);
            
            if (false == bNode->isVisible() || bNodeType == -1) {continue;}
            
            // When checking hero and ladder, heroLayer return (0,0,0,0) if hero is up, ignore collide
            // Or, return actual collide rect when hero is down
            // However, checking hero and monster, heroLayer return actual collide rect if hero is up
            auto aCollideRect = aLayer->getCollideRect(aNode, bNodeType);
            
            auto bCollideRect = bLayer->getCollideRect(bNode, aNodeType);

            if (aCollideRect.intersectsRect(bCollideRect))
            {
                aLayer->handleCollide(aNode, bNodeType);
                
                bLayer->handleCollide(bNode, aNodeType);
                
                return;
            }
        }
    }
}

void BattleScene::ntf(HeroNtf &heroNtf)
{
    m_bgLayer->ntf(heroNtf);
    
    m_ladderLayer->ntf(heroNtf);
    
    m_uiLayer->ntf(heroNtf);
}

void BattleScene::pause()
{
    m_isPause = true;
}

void BattleScene::resume()
{
    m_isPause = false;
}



