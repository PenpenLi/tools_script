//
//  LoadLayer.cpp
//  HiFly
//
//  Created by chris li on 14-6-24.
//
//

#include "LoadLayer.h"
#include "util/LevelBook.h"
#include "LevelHelper.h"
#include "util/GameSound.h"

LoadLayer* LoadLayer::create(int level, std::function<void()> onLoadFinishCallback)
{
    auto layer = new LoadLayer();
    
    if (layer->init(level, onLoadFinishCallback))
    {
        layer->autorelease();
        
        return layer;
    }
    
    return nullptr;
}

void LoadLayer::load()
{
    m_levelStr = LevelBook::getInstance()->getLevelConfig(m_level);
    
    log("[LoadLayer::loadLevel] %s", m_levelStr.c_str());
    
    m_ladderSetTotalNum = 0;
    
    m_ladderSetLoadNum = 0;
    
    for (int i = 0; i < m_levelStr.length(); i++)
    {
        auto markChar = m_levelStr.at(i);
        
        if (markChar == '-')
        {
            m_ladderSetTotalNum++;
        }
    }
    
    scheduleUpdate();
}

void LoadLayer::update(float delta)
{
    std::string ladderSetFileName = getLadderSetFileName();
    
    log("[LoadLayer] load: %s", ladderSetFileName.c_str());
    
    if (ladderSetFileName == "")
    {
        unscheduleUpdate();
        
        m_onLoadFinishCallback();
        
        this->removeFromParentAndCleanup(true);
    }
    
    else
    {
        m_ladderSetLoadNum++;
        
        LevelHelper::getInstance()->loadLadderSet(ladderSetFileName);
        
        
        int percent = m_ladderSetLoadNum*100 / m_ladderSetTotalNum;
        
        auto tip = StringUtils::format("%d", percent);
        
        tip += "%";
        
        m_tipLabel->setString(tip.c_str());
    }
}

bool LoadLayer::init(int level, std::function<void()> onLoadFinishCallback)
{
    if (Layer::init())
    {
        m_level = level;
        
        m_onLoadFinishCallback = onLoadFinishCallback;
        
        // add background
        auto bg0 = Sprite::create("Scene_BlueSea_bg_1136.png");
        
        bg0->setAnchorPoint(Point(0, 0));
        
        addChild(bg0);
        
        // add animation
        ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/LoadingAnimation/LoadingAnimation.ExportJson");
        
        auto armature = Armature::create("LoadingAnimation");
        
        armature->setPosition(Point(Director::getInstance()->getWinSize().width/2, Director::getInstance()->getWinSize().height/2));
        
        this->addChild(armature);
        
        armature->getAnimation()->play("Animation1");
        
        //m_tipLabel = LabelAtlas::create("0123456789","paly_number.png",37,60,'0');
        m_tipLabel = Label::create("0 / 0","Arial", 36);
        
        m_tipLabel->setPosition(Point(Director::getInstance()->getWinSize().width/2, Director::getInstance()->getWinSize().height/2-100));
        
        this->addChild(m_tipLabel);
        
        return true;
    }
    
    return false;
}

void LoadLayer::onEnter()
{
    Layer::onEnter();
}

void LoadLayer::onExit()
{
    Layer::onExit();
}

std::string LoadLayer::getLadderSetFileName()
{
    int pos = m_levelStr.find_first_of('-');
    
    if (pos != std::string::npos)
    {
        auto ladderSetName = m_levelStr.substr(0, pos);
        
        ladderSetName += ".ccbi";
        
        m_levelStr = m_levelStr.substr(pos+1);
        
        return ladderSetName;
    }
    
    return "";
}

