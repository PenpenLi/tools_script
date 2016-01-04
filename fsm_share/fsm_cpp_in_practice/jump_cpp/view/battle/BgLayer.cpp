//
//  BgLayer.cpp
//  HiFly
//
//  Created by chris li on 14-5-13.
//
//

#include "BgLayer.h"
#include "BattleDefine.h"
#include "util/GameSound.h"
#include "BattleScene.h"

BgLayer::BgLayer() : m_layer1(nullptr), m_layer2(nullptr), m_layer3(nullptr)
{
}

BgLayer::~BgLayer() {}

bool BgLayer::init()
{
    if (Node::init())
    {
        // add background
        //auto bg0 = Sprite::create("Scene_BlueSea_bg_2_1136.png");
        auto bg0 = Sprite::create("Scene_BlueSea_bg_1136.png");
        
        bg0->setAnchorPoint(Point(0, 0));
        
        addChild(bg0);
        
        // add side background
        //m_sizeBg1 = Sprite::create("Scene_BlueSea_side_2.png");
        m_sizeBg1 = Sprite::create("Scene_BlueSea_side.png");
        
        m_sizeBg1->setAnchorPoint(Point(0, 0));
        
        m_sizeBg1->setPosition(0, 0);
        
        addChild(m_sizeBg1);
        
        //m_sizeBg2 = Sprite::create("Scene_BlueSea_side_2.png");
        m_sizeBg2 = Sprite::create("Scene_BlueSea_side.png");
        
        m_sizeBg2->setAnchorPoint(Point(0, 0));
        
        m_sizeBg2->setPosition(0, LADDER_SET_HEIGHT);
        
        addChild(m_sizeBg2);

		m_blackBg = Sprite::create("Scene_Black_bg.png");
        
        m_blackBg->setScaleY(Director::getInstance()->getWinSize().height/960);

		m_blackBg->setAnchorPoint(Point(0, 0));

		m_blackBg->setOpacity(0);

		addChild(m_blackBg);

        //读取粒子效果
        ParticleSystem* m_emitter1 = ParticleSystemQuad::create("bubble.plist");
        
        m_emitter1->retain();
        
        ParticleBatchNode *batch = ParticleBatchNode::createWithTexture(m_emitter1->getTexture());
        
        batch->addChild(m_emitter1);
        
        batch->setPositionY(-200);
        
        //addChild(batch, 10);
        
        m_emitter1->release();
        
        return true;
    }
    
    return false;
}

void BgLayer::onEnter()
{
    Node::onEnter();
    
    do
    {
        // add event listener
        auto listener = EventListenerCustom::create(EVENT_HERO_NTF, [this](EventCustom* event) {
            
            HeroNtf *heroNtf = static_cast<HeroNtf*>(event->getUserData());
            
            if (nullptr != heroNtf && heroNtf->scrollDistance > 0)
            {
                scrollDown(heroNtf->scrollDistance/2);
            }
        });

		_eventDispatcher->addEventListenerWithFixedPriority(listener, 1);

		auto warningListener = EventListenerCustom::create(EVENT_WARNING_END, [this](EventCustom* event)
		{
			float *positionX = static_cast<float*>(event->getUserData());

			if (nullptr != positionX)
			{
				torpedoWarning(*positionX);
			}
		});

		_eventDispatcher->addEventListenerWithFixedPriority(warningListener, 1);

		auto rewardLevelsStartListener = EventListenerCustom::create(EVENT_REWARD_LEVELS_START, [this](EventCustom* event)
		{
            //m_blackBg->setColor(Color3B::WHITE);
			m_blackBg->runAction(FadeTo::create(1,150));
            //m_blackBg->runAction(TintTo::create(2, 255, 255, 255));
            
			auto battleScene = static_cast<BattleScene*>(Director::getInstance()->getRunningScene());
        
			HeroNtf heroNtf = {0};

			heroNtf.levelType = 1;

			heroNtf.heroUIDataNtf.tag = 1;

			battleScene->ntf(heroNtf);
        
		});

		_eventDispatcher->addEventListenerWithFixedPriority(rewardLevelsStartListener, 1);

		auto rewardLevelsEndListener = EventListenerCustom::create(EVENT_REWARD_LEVELS_END, [this](EventCustom* event)
		{
			m_blackBg->runAction(FadeTo::create(1,0));

			auto battleScene = static_cast<BattleScene*>(Director::getInstance()->getRunningScene());
        
			HeroNtf heroNtf = {0};

			heroNtf.levelType = 2;

			heroNtf.heroUIDataNtf.tag = 1;

			battleScene->ntf(heroNtf);
		});

		_eventDispatcher->addEventListenerWithFixedPriority(rewardLevelsEndListener, 1);
    } while(0);
}

void BgLayer::onExit()
{
    do
    {
        _eventDispatcher->removeCustomEventListeners(EVENT_HERO_NTF);
        
        _eventDispatcher->removeCustomEventListeners(EVENT_REWARD_LEVELS_END);
        
        _eventDispatcher->removeCustomEventListeners(EVENT_REWARD_LEVELS_START);
        
        _eventDispatcher->removeCustomEventListeners(EVENT_WARNING_END);
        
    } while (0);
    
    Node::onExit();
}

void BgLayer::scrollDown(float distance)
{
    m_sizeBg1->setPositionY(m_sizeBg1->getPositionY()-distance);
    
    m_sizeBg2->setPositionY(m_sizeBg2->getPositionY()-distance);
    
    if (m_sizeBg2->getPositionY() < 0)
    {
        m_sizeBg1->setPositionY(m_sizeBg2->getPositionY()+LADDER_SET_HEIGHT);
        
        auto tmp = m_sizeBg1;
        
        m_sizeBg1 = m_sizeBg2;
        
        m_sizeBg2 = tmp;
    }
}

void BgLayer::torpedoWarning(float x)
{
	/*auto blink = Blink::create(1, 2);

	//SpriteFrame *frame = SpriteFrameCache::getInstance()->spriteFrameByName("torpedo_warning.png");
	auto sprite = Sprite::createWithSpriteFrameName("torpedo_warning.png");

	sprite->setAnchorPoint(Point(0.5, 1));

	sprite->setPosition(Point(x, Director::getInstance()->getWinSize().height));

	sprite->setVisible(false);

	sprite->runAction(blink);

	addChild(sprite);*/	
//	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("png/props/propWarning.plist");	
	
	////////////////////ray/////////////////////////////
	auto ray = Sprite::createWithSpriteFrameName("prop_ray_1.png");
	ray->setScaleY(100.5f);
	ray->setScaleX(1.2f);
	ray->setPosition(Point(x, Director::getInstance()->getWinSize().height + 500));
	auto animation_ray = Animation::create();
	for(int i=0;i<=11;++i)
	{
		std::string spriteFrameName = StringUtils::format("prop_ray_%d.png",i + 1);
		SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
		animation_ray->addSpriteFrame(frame);
	}
	animation_ray->setDelayPerUnit(0.1f);
	animation_ray->setRestoreOriginalFrame(false);
	auto action_ray = Animate::create(animation_ray);
	auto callFunc_ray = CallFunc::create([ray]()
      {
			ray->removeFromParent();

			//auto scene = Director::getInstance()->getRunningScene();
			//auto battleScene = dynamic_cast<BattleScene*>(scene);

			//battleScene->resume();
	});

	auto seq_ray = Sequence::create(action_ray, action_ray, callFunc_ray, NULL);
//	auto forever_ray = RepeatForever::create(action_ray);
	ray->runAction(seq_ray);
	this->addChild(ray,100);

	////////////////////warning/////////////////////////////
	auto warning = Sprite::createWithSpriteFrameName("prop_warning_1.png");

	warning->setPosition(Point(x, Director::getInstance()->getWinSize().height));
	auto animation_warning = Animation::create();
	for(int i=0;i<=11;++i)
	{
		std::string spriteFrameName = StringUtils::format("prop_warning_%d.png",i + 1);
		SpriteFrame *frame = SpriteFrameCache::getInstance()->spriteFrameByName(spriteFrameName);
		animation_warning->addSpriteFrame(frame);
	}
	animation_warning->setDelayPerUnit(0.1f);
	animation_warning->setRestoreOriginalFrame(false);
	auto action_warning = Animate::create(animation_warning);
//	auto forever_warning = RepeatForever::create(action_warning);
	auto callFunc_warning = CallFunc::create([warning]()
      {
			warning->removeFromParent();

			//auto scene = Director::getInstance()->getRunningScene();
			//auto battleScene = dynamic_cast<BattleScene*>(scene);

			//battleScene->resume();
	});

	auto seq_warning = Sequence::create(action_warning, action_warning, callFunc_warning, NULL);
	warning->runAction(seq_warning);
	this->addChild(warning,101);
}

void BgLayer::ntf(HeroNtf &heroNtf)
{
    scrollDown(heroNtf.scrollDistance/2);
}

void BgLayer::sceneUpdate(float dt)
{
}

