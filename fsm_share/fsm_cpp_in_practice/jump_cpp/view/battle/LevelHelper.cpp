//
//  LevelHelper.cpp
//  HiFly
//
//  Created by chris li on 14-5-15.
//
//

#include "LevelHelper.h"
#include "LevelBook.h"
#include "util/GameSound.h"
#include "BattleData.h"
#include "BattleDefine.h"
using namespace cocosbuilder;

static LevelHelper* g_LevelHelper = nullptr;

LevelHelper* LevelHelper::getInstance()
{
    if (nullptr == g_LevelHelper)
    {
        g_LevelHelper = new LevelHelper();
    }
    
    return g_LevelHelper;
}

// return nullptr when this level is over
Node* LevelHelper::getLadderSet(int ladderSetType)
{
    // normal ladderSet
    if (0 == ladderSetType)
    {
        if (m_ladderSetVector.size() == 0)
        {
            if (BattleData::getInstance()->getBattleMode() == 0)
            {
                //auto level = BattleData::getInstance()->getBattleLevel();
                
                int level = 20000 + (int)(CCRANDOM_0_1()*4);
                
                chooseLevel(level);
                
                //BattleData::getInstance()->setBattleLevel(level);
                
                //chooseLevel(BattleData::getInstance()->getBattleLevel());
            }
            else
            {
                return nullptr;
            }
        }
        
        auto ladderSet = m_ladderSetVector.back();

        m_ladderSetVector.popBack();
        
        log("[LevelHelper::getLadderSet] fetch ladderSet - %s", ladderSet->debugString.c_str());

        return ladderSet;
    }
    
    // reward ladderSet
    else
    {
        if (m_rewardLadderSetVector.size() > 0)
        {
            auto ladderSet = m_rewardLadderSetVector.back();
            
            m_rewardLadderSetVector.popBack();
            
            log("[LevelHelper::getLadderSet] fetch ladderSet - %s", ladderSet->debugString.c_str());
            
            return ladderSet;
        }
        else
        {
            return nullptr;
        }
    }
}

void LevelHelper::loadLadderSet(std::string ladderSetName)
{
    Node *ladderSet = readCCBI(ladderSetName);
    
    if (ladderSet)
    {
        ladderSet->debugString = ladderSetName;
        
        m_ladderSetVector.pushBack(ladderSet);
    }
    else
    {
        log("[LevelHelper] there is no %s", ladderSetName.c_str());
    }
}

void LevelHelper::loadRewardLadderSet(std::string ladderSetName)
{
    Node *ladderSet = readCCBI(ladderSetName);
    
    if (ladderSet)
    {
        ladderSet->debugString = ladderSetName;
        
        //m_rewardLadderSetVector.pushBack(ladderSet);
        m_rewardLadderSetVector.insert(0, ladderSet);
    }
    else
    {
        //m_rewardLadderSetVector.reverse();
        log("[LevelHelper] there is no %s", ladderSetName.c_str());
    }
}

void LevelHelper::chooseLevel(int level)
{
    m_ladderSetVector.clear();
    
    m_level = level;
    
    std::string levelStr = LevelBook::getInstance()->getLevelConfig(level);
    
    log("[LevelHelper] levelStr %s", levelStr.c_str());
    
    int pos = 0;
    
    std::string ladderSetName = "";
    
    while ((pos = levelStr.find_first_of('-')) != std::string::npos)
    {
        ladderSetName = levelStr.substr(0, pos);
        
        levelStr = levelStr.substr(pos+1);
        
        ladderSetName += ".ccbi";
        
        log("[LevelHelper]chooseLevel %s", ladderSetName.c_str());
        
        Node *ladderSet = readCCBI(ladderSetName);
        
        if (ladderSet)
        {
            ladderSet->debugString = ladderSetName;
            
            m_ladderSetVector.pushBack(ladderSet);
        }
        else
        {
            log("[LevelHelper] there is no %s", ladderSetName.c_str());
        }
    }
    
    m_ladderSetVector.reverse();
}

void LevelHelper::chooseRewardLevel(int level)
{
    m_rewardLadderSetVector.clear();
    //test
    level = 1;
    
    std::string levelStr = LevelBook::getInstance()->getLevelConfig(level+30000);
    
    log("[LevelHelper] levelStr %s", levelStr.c_str());
    
    int pos = 0;
    
    std::string ladderSetName = "";
    
    while ((pos = levelStr.find_first_of('-')) != std::string::npos)
    {
        ladderSetName = levelStr.substr(0, pos);
        
        levelStr = levelStr.substr(pos+1);
        
        ladderSetName += ".ccbi";
        
        log("[LevelHelper]chooseRewardLevel %s", ladderSetName.c_str());
        
        Node *ladderSet = readCCBI(ladderSetName);
        
        if (ladderSet)
        {
            m_rewardLadderSetVector.pushBack(ladderSet);
            
            EventCustom event(EVENT_LOAD_RES);
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        }
        else
        {
            log("[LevelHelper]chooseRewardLevel there is no %s", ladderSetName.c_str());
        }
    }
    
    m_rewardLadderSetVector.reverse();
}

void LevelHelper::chooseInfiniteLevel()
{
    m_level = 88888;
    
    chooseLevel(m_level);
}

Node* LevelHelper::readCCBI(std::string &ladderSetName)
{
    NodeLoaderLibrary * ccNodeLoaderLibrary = NodeLoaderLibrary::newDefaultNodeLoaderLibrary();
    
    cocosbuilder::CCBReader * ccbReader = new cocosbuilder::CCBReader(ccNodeLoaderLibrary);
    
    auto node = ccbReader->readNodeGraphFromFile(ladderSetName.c_str());
    
    ccbReader->release();
    
    return node;
}

