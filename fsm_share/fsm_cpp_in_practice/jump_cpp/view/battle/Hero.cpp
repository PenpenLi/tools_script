//
//  Hero.cpp
//  HiFly
//
//  Created by chris li on 14-5-4.
//
//

#include "Hero.h"
#include "BattleDefine.h"
#include "util/GameSound.h"

USING_NS_CC;

Hero::Hero() : m_heroId(HERO_1_TAG)
{
}

Hero::~Hero()
{
}

Hero* Hero::create(int heroId)
{
    Hero* hero = new Hero();
    
    if (hero->init(heroId))
    {
        hero->autorelease();
        
        return hero;
    }
    
    CC_SAFE_DELETE(hero);
    
    return nullptr;
}



void Hero::setJumpState(std::string &jumpState, float param1, float param2, float param3)
{
    if (jumpState == "JumpSuspend")
    {
        MovementData *data = m_heroArmature->getAnimation()->getAnimationData()->getMovement("JumpSuspend");
        
        data->durationTo = 1;
        data->durationTween = 0;
    }
    
    m_heroArmature->getAnimation()->play(jumpState);
}

void Hero::setBgState(std::string &bgState, float param1, float param2, float param3)
{
	if (bgState == "NullProp")
    {
        MovementData *data = m_heroBgArmature->getAnimation()->getAnimationData()->getMovement("NullProp");
        data->durationTo = 1;
        data->durationTween = 0;
    }

    m_heroBgArmature->getAnimation()->play(bgState);
}

bool Hero::init(int heroId)
{
    m_heroId = heroId;
    
    std::string heroFile = "animation/Devilfish/Devilfish.ExportJson";
    
    std::string heroName = "Devilfish";
    
    if (HERO_1_TAG == heroId)
    {
        heroFile = "animation/Devilfish/Devilfish.ExportJson";
    }
    
    else if (HERO_2_TAG == heroId)
    {
        heroFile = "animation/Horse__1/Horse__1.ExportJson";
        
        heroName = "Horse__1";
    }
    
    // load ccs data
    ArmatureDataManager::getInstance()->addArmatureFileInfo(heroFile);

    m_heroArmature = Armature::create(heroName);
    
    m_heroArmature->setTag(m_heroId);

	ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/BG_Devilfish/BG_Devilfish.ExportJson");

	m_heroBgArmature = Armature::create("BG_Devilfish");

	m_heroBgArmature->getAnimation()->play("NullProp");

	m_heroBgArmature->setTag(m_heroId);
    
    //m_heroArmature->getAnimation()->play("JumpReady");
    
    //m_heroArmature->getAnimation()->setFrameEventCallFunc(CC_CALLBACK_0(Hero::onFrameEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
    
    m_heroArmature->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(Hero::animationEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    
    if (nullptr != m_heroArmature)
    {
		m_heroArmature->setTag(HERO_1_TAG);
        addChild(m_heroArmature);

		if (nullptr != m_heroBgArmature)
		{
			addChild(m_heroBgArmature);
		}
        
        return true;
    }
    
    return false;
}

void Hero::onFrameEvent(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex)
{
    // "JUMP_READY_OVER" defined in cocostudio action editor
    if ("JUMP_READY_OVER" == evt)
    {
        // notify other layer
        std::string eventName = EVENT_JUMP_READY_OVER;
        
        EventCustom event(eventName);
        
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    }
}

void Hero::animationEvent(Armature *armature, MovementEventType movementType, const std::string& movementID)
{
    if (COMPLETE == movementType)
    {
        // notify other layer
        std::string eventName;
        
        if ("JumpReady" == movementID)
        {
            eventName = EVENT_JUMP_READY_OVER;
        }
        else if ("JumpAgain1" == movementID)
        {
            eventName = EVENT_JUMP_AGAIN1_OVER;
        }
		else if ("JumpDeath1" == movementID)
        {
            eventName = EVENT_JUMP_DEATH1_OVER;
        }
        
        EventCustom event(eventName);
        
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    }
}

