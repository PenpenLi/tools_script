//
//  LadderLayer.cpp
//  HiFly
//
//  Created by chris li on 14-5-13.
//
//

#include "LadderLayer.h"
#include "LevelHelper.h"
#include "BattleDefine.h"
#include "BattleData.h"
#include "util/GameSound.h"
#include "BattleScene.h"
#include "SimpleAudioEngine.h"
bool LadderLayer::init()
{
	if (Node::init())
	{
		setLevelType(0);

		setIsAttack(false);

		setIsInvincible(false);
        
        m_rewardLevel = 1;

		return true;
	}

	return false;
}

void LadderLayer::onEnter()
{
	Node::onEnter();
    
    initLadderSet();

	do
	{
		// add event listener
		auto listener = EventListenerCustom::create(EVENT_HERO_NTF, [this](EventCustom* event)
		{
			HeroNtf *heroNtf = static_cast<HeroNtf*>(event->getUserData());

			// scorll down screen
			if (nullptr != heroNtf && heroNtf->scrollDistance > 0)
			{
				scrollDown(heroNtf->scrollDistance);
			}

			// refresh hero pos
			if (0 != heroNtf->x && 0 != heroNtf->y)
			{
				setHeroPos(Point(heroNtf->x, heroNtf->y));
			}

			// magnet state process
			if (0 != heroNtf->magnetState)
			{
				setMagnetState(heroNtf->magnetState);
			}

			// is enter reward levels
			if (0 != heroNtf->levelType)
			{
				setLevelType(heroNtf->levelType);
			}
		});

		_eventDispatcher->addEventListenerWithFixedPriority(listener, 1);

		auto attackStartListener = EventListenerCustom::create(EVENT_ATTACK_START, [this](EventCustom* event)
		{
			setIsAttack(true);
		});

		_eventDispatcher->addEventListenerWithFixedPriority(attackStartListener, 1);

		auto attackEndListener = EventListenerCustom::create(EVENT_ATTACK_END, [this](EventCustom* event)
		{
			setIsAttack(false);
		});

		_eventDispatcher->addEventListenerWithFixedPriority(attackEndListener, 1);

		auto invincibleStartListener = EventListenerCustom::create(EVENT_INVINCIBLE_START, [this](EventCustom* event)
		{
			setIsInvincible(true);
		});

		_eventDispatcher->addEventListenerWithFixedPriority(invincibleStartListener, 1);

		auto invincibleEndListener = EventListenerCustom::create(EVENT_INVINCIBLE_END, [this](EventCustom* event)
		{
			setIsInvincible(false);
		});

		_eventDispatcher->addEventListenerWithFixedPriority(invincibleEndListener, 1);

	} while(0);
}

void LadderLayer::onExit()
{
	do
	{
		_eventDispatcher->removeCustomEventListeners(EVENT_HERO_NTF);

	} while (0);

	Node::onExit();
}

void LadderLayer::sceneUpdate(float dt)
{
	for (auto node : m_ladderVector)
	{
		auto sprite = getSpriteHelper(node);

		auto worldPos = sprite->convertToWorldSpaceAR(Point::ZERO);

		if (worldPos.y < 0 || worldPos.y > Director::getInstance()->getWinSize().height) {continue;}

		//handle magnet effect
		if (sprite->getTag() == PROP_GOLD_TAG || sprite->getTag() == PROP_GOLD_SMALL_TAG)
		{
			if (1 == getMagnetState())
			{
				node->setTag(PROP_GOLD_ABSORB_TAG);
			}
			handleMagnetEffect(node, dt);
		} 
	}
}

void LadderLayer::initLadderSet()
{
    auto battleLevel = BattleData::getInstance()->getBattleLevel();
    
    // Level Mode
    if (battleLevel > 0 && battleLevel <= 40)
    {
        BattleData::getInstance()->setBattleMode(1);
    }
    else
    {
        BattleData::getInstance()->setBattleMode(0);
    }
    
    log("------------------------ Loading LadderSet -----------------------");
    log("BattleMode %d, BattleLevel %d", BattleData::getInstance()->getBattleMode(), BattleData::getInstance()->getBattleLevel());
    
	LevelHelper::getInstance()->chooseLevel(battleLevel);

	LevelHelper::getInstance()->chooseRewardLevel(m_rewardLevel++);
    
    log("------------------------ Loading LadderSet End--------------------");

	m_ladderSet1 = getLadderSetHelper();

	m_ladderSet2 = getLadderSetHelper();

	if (m_ladderSet1 && m_ladderSet2)
	{
		m_ladderSet1->setPosition(0, 0);

		addChild(m_ladderSet1);

		m_ladderSet2->setPosition(0, LADDER_SET_HEIGHT);

		addChild(m_ladderSet2);

		resetLadderVector();
	}
}

void LadderLayer::scrollDown(float distance)
{
	if (nullptr == m_ladderSet1 || nullptr == m_ladderSet2) return;

	m_ladderSet1->setPositionY(m_ladderSet1->getPositionY()-distance);

	m_ladderSet2->setPositionY(m_ladderSet2->getPositionY()-distance);

	if (m_ladderSet2->getPositionY() < -5)
	{
		resetLadderSet();
	}
}

void LadderLayer::resetLadderSet()
{
	removeChild(m_ladderSet1);

	m_ladderSet1 = m_ladderSet2;

	m_ladderSet2 = getLadderSetHelper();

	if (nullptr == m_ladderSet2)
	{
		// level is over
        log("LadderLayer:: level is over");

		return;
	}

	m_ladderSet2->setPosition(0, m_ladderSet1->getPositionY()+LADDER_SET_HEIGHT);

	addChild(m_ladderSet2);

	resetLadderVector();
}

void LadderLayer::resetLadderVector()
{
	m_ladderVector.clear();

	m_ladderVector.pushBack(m_ladderSet1->getChildren());

	m_ladderVector.pushBack(m_ladderSet2->getChildren());

	//log("LadderVector Size: %zd", m_ladderVector.size());
}

Vector<Node*>* LadderLayer::getCollideNode()
{
	return &m_ladderVector;
}

Rect LadderLayer::getCollideRect(Node *myNode, int targetNodeType)
{
	int myNodeType = getCollideNodeType(myNode);

	auto sprite = getSpriteHelper(myNode);

	auto worldPos = sprite->convertToWorldSpaceAR(Point(0, 0));

	auto size = sprite->getBoundingBox().size;

	if (targetNodeType == HERO_1_TAG)
	{
		//size.height -= 50;
	}

	Rect rect = Rect(worldPos.x-size.width/2, worldPos.y-size.height/2, size.width, size.height);

	// 1. ladder collide with hero
	if (myNodeType > LADDER_BEGIN_TAG && myNodeType < LADDER_END_TAG)
	{
		if (targetNodeType > HERO_BEGIN_TAG && targetNodeType < HERO_END_TAG)
		{
			rect = Rect(worldPos.x-size.width/2, worldPos.y-size.height/4, size.width, size.height);
		}
	}

	else if (targetNodeType == PROP_TRAMPOLINE_TAG)
	{
		rect =  Rect(worldPos.x-size.width/2, worldPos.y-size.height/2, size.width*2, size.height * 2);
	}

	return rect;
}

void LadderLayer::handleCollide(Node *myNode, int targetNodeType)
{
	int myNodeType = getCollideNodeType(myNode);

	if (myNodeType > LADDER_BEGIN_TAG && myNodeType < LADDER_END_TAG)
	{
		auto ladder = getSpriteHelper(myNode);

		if(myNodeType == LADDER_ONCE_SHORT_TAG)
		{
			GameSound::playEffectMusic(EFF_SINGLE_TREAD_LADDER);

			if (ladder->getNumberOfRunningActions() == 0)
			{
				auto animation = Animation::create();
				for(int i=1;i<=6;++i)
				{
					std::string spriteFrameName = StringUtils::format("ladder_once_short_%d.png",i);
					SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
					animation->addSpriteFrame(frame);
				}
				animation->setDelayPerUnit(0.02f);
				animation->setRestoreOriginalFrame(false);
				auto action = Animate::create(animation);
				auto callFunc = CallFunc::create([myNode](){
					myNode->setVisible(false);
				});
				auto seq = Sequence::createWithTwoActions(action, callFunc);
				ladder->runAction(seq);
			}
		}
		
		else if(myNodeType == LADDER_ONCE_LONG_TAG)
		{
			GameSound::playEffectMusic(EFF_SINGLE_TREAD_LADDER);
			if (ladder->getNumberOfRunningActions() == 0)
			{
				auto animation = Animation::create();
				for(int i=1;i<=7;++i)
				{
					std::string spriteFrameName = StringUtils::format("ladder_once_long_%d.png",i);
					SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
					animation->addSpriteFrame(frame);
				}
				animation->setDelayPerUnit(0.02f);
				animation->setRestoreOriginalFrame(false);
				auto action = Animate::create(animation);
				auto callFunc = CallFunc::create([myNode](){
					myNode->setVisible(false);
				});
				auto seq = Sequence::createWithTwoActions(action, callFunc);
				ladder->runAction(seq);
			}
		}
		else if(myNodeType == LADDER_BROKEN_LONG_TAG)
		{
			GameSound::playEffectMusic(EFF_BREAK_LADDER);


			/*CocosDenshion::SimpleAudioEngine* eff_break = NULL;
			eff_break->playEffect(EFF_BREAK_LADDER);
			eff_break->setEffectsVolume(1.0f);*/
			if (ladder->getNumberOfRunningActions() == 0)
			{
				auto animation = Animation::create();
				for(int i=1;i<=7;++i)
				{
					std::string spriteFrameName = StringUtils::format("ladder_broken_%d.png",i);
					SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
					animation->addSpriteFrame(frame);
				}
				animation->setDelayPerUnit(0.05f);
				animation->setRestoreOriginalFrame(false);
				auto action = Animate::create(animation);
				auto callFunc = CallFunc::create([myNode](){
					myNode->setVisible(false);
				});
				auto seq = Sequence::createWithTwoActions(action, callFunc);
				ladder->runAction(seq);
			}
		}
		else if(myNodeType == LADDER_RANDOM_MOVE_TAG)
		{
			GameSound::playEffectMusic(EFF_TREAD_LADDER);
			for (auto node : m_ladderVector)
			{
				auto sprite = getSpriteHelper(node);

				auto worldPos = sprite->convertToWorldSpaceAR(Point::ZERO);

				if (worldPos.y < 0 || worldPos.y > Director::getInstance()->getWinSize().height) {continue;}

				//handle magnet effect
				if (sprite->getTag() == LADDER_RANDOM_MOVE_TAG)
				{
					float random = (CCRANDOM_0_1()-0.5)*200;
					if(worldPos.x < 100 && random<0 || worldPos.x > Director::getInstance()->getWinSize().width-100 && random > 0)
					{
						random = -random;
					} 
					auto moveBy = MoveBy::create(0.3, Point(random, 0));
					sprite->runAction(moveBy);
				} 
			}
		}

		else if(myNodeType == LADDER_CONTROL_TAG)
		{
			GameSound::playEffectMusic(EFF_CONTROL_LADDER);
			auto prop = getSpriteHelper(myNode);
			auto animation = Animation::create();
			for(int i=1;i<=9;++i)
			{
				std::string spriteFrameName = StringUtils::format("ladder_control_%d.png",i);
				SpriteFrame *frame = SpriteFrameCache::getInstance()->spriteFrameByName(spriteFrameName);
				animation->addSpriteFrame(frame);
			}
			animation->setDelayPerUnit(0.1f);
			animation->setRestoreOriginalFrame(false);
			auto action = Animate::create(animation);
			prop->runAction(action);
			_eventDispatcher->removeEventListenersForTarget(ladder, true);

			auto moveBy = MoveBy::create(0.05, Point(0, -10));

			auto moveByBack = moveBy->reverse();

			auto seqAction = Sequence::createWithTwoActions(moveBy, moveByBack);

			ladder->runAction(seqAction);
		}

		else if(myNodeType == LADDER_NORMAL_LONG_TAG)
		{
			GameSound::playEffectMusic(EFF_TREAD_LADDER);
			auto moveBy = MoveBy::create(0.05, Point(0, -10));

			auto moveByBack = moveBy->reverse();

			auto seqAction = Sequence::createWithTwoActions(moveBy, moveByBack);

			ladder->runAction(seqAction);
		}

		else
		{
			GameSound::playEffectMusic(EFF_TREAD_LADDER);

			auto moveBy = MoveBy::create(0.05, Point(0, -10));

			auto moveByBack = moveBy->reverse();

			auto seqAction = Sequence::createWithTwoActions(moveBy, moveByBack);

			ladder->runAction(seqAction);
		}
	}

	else if (myNodeType > MONSTER_BEGIN_TAG && myNodeType < MONSTER_END_TAG)
	{
		GameSound::playEffectMusic(EFF_TOUCH_MONSTER);
		if (getIsAttack() || getIsInvincible())
		{
			auto prop = getSpriteHelper(myNode);
			prop->setTag(-1);
			prop->stopAllActions();
			auto animation = Animation::create();
			for(int i=1;i<=14;++i)
			{
				std::string spriteFrameName = StringUtils::format("monster_bomb_%d.png",i);
				SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
				animation->addSpriteFrame(frame);
			}
			animation->setDelayPerUnit(0.02f);
			animation->setRestoreOriginalFrame(false);
			auto action = Animate::create(animation);
			auto callFunc = CallFunc::create([this, myNode]()
            {
				myNode->setVisible(false);
				EventCustom event(EVENT_MONSTER_ENABLE_TOUCHED);
               
				Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);

				EventCustom event1(EVENT_ATTACK_READY);
               
				Director::getInstance()->getEventDispatcher()->dispatchEvent(&event1);
                
                //chris.li added
                auto monster = this->getSpriteHelper(myNode);
                Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(monster);
			});
			auto seq = Sequence::createWithTwoActions(action, callFunc);
			prop->runAction(seq);
		}
	}

	else if (myNodeType == PROP_SPRING_TAG)
	{
		GameSound::playEffectMusic(EFF_SPRING);
		auto prop = getSpriteHelper(myNode);
		//prop->setAnchorPoint(Point(0.5, 0));
		if (prop->getNumberOfRunningActions() == 0)
		{
			auto animation = Animation::create();
			for(int i=1;i<=5;++i)
			{
				std::string spriteFrameName = StringUtils::format("prop_spring_%d.png",i);
				SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
				animation->addSpriteFrame(frame);
			}
			animation->setDelayPerUnit(0.1f);
			animation->setRestoreOriginalFrame(true);
			auto action = Animate::create(animation);
			prop->runAction(action);
		}
	}

	else if (myNodeType == PROP_TRAMPOLINE_TAG)
	{
		GameSound::playEffectMusic(EFF_TRAMPOLINE);
		auto prop = getSpriteHelper(myNode);
		//prop->setAnchorPoint(Point(0.5, 0));
		if (prop->getNumberOfRunningActions() == 0)
		{
			auto animation = Animation::create();
			for(int i=1;i<=11;++i)
			{
				std::string spriteFrameName = StringUtils::format("prop_trampoline_%d.png",i);
				SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
				animation->addSpriteFrame(frame);
			}
			animation->setDelayPerUnit(0.02f);
			animation->setRestoreOriginalFrame(true);
			auto action = Animate::create(animation);
			prop->runAction(action);
		}
	}

	else if (myNodeType == PROP_GOLD_TAG)
	{
		GameSound::playEffectMusic(EFF_BIG_GOLD);
		//myNode->setVisible(false);
        
        auto prop = getSpriteHelper(myNode);
        prop->setTag(-1);
        prop->stopAllActions();
        auto animation = Animation::create();
        for(int i=1;i<=10;++i)
        {
            std::string spriteFrameName = StringUtils::format("m_coin_animate_%d.png",i);
            SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
            animation->addSpriteFrame(frame);
        }
        animation->setDelayPerUnit(0.02f);
        animation->setRestoreOriginalFrame(false);
        auto action = Animate::create(animation);
        auto callFunc = CallFunc::create([myNode]() {
            myNode->setVisible(false);
        });
        auto seq = Sequence::createWithTwoActions(action, callFunc);
        prop->runAction(seq);
	}

	else if (myNodeType == PROP_GOLD_SMALL_TAG)
	{
		GameSound::playEffectMusic(EFF_GOLD);
		myNode->setVisible(false);
	}

	else if (myNodeType == PROP_STAR_TAG)
	{
		GameSound::playEffectMusic(EFF_REWARD_START);
		myNode->setVisible(false);
	}

	else if (myNodeType == PROP_MAGNET_TAG)
	{
		GameSound::playEffectMusic(EFF_MAGNET, 1);
		myNode->setVisible(false);
        
        EventCustom event(EVENT_REWARD_MAGNET_START);
        
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
	}
	//mo gu
	else if (myNodeType == PROP_MEDICINE_TAG)
	{
		GameSound::playEffectMusic(EFF_RANDOM_ITEM);
		myNode->setVisible(false);
	}

	else if (myNodeType == PROP_SHIELD_TAG)
	{
		GameSound::playEffectMusic(EFF_BUBBLE_SHIELD, 1);
		myNode->setVisible(false);
	}

	else if (myNodeType == PROP_HELICOPTER_TAG)
	{
		GameSound::playEffectMusic(EFF_PROPELLER, 1);
		myNode->setVisible(false);
	}

	else if (myNodeType == PROP_TORPEDO_TAG)
	{
		//GameSound::playEffectMusic(EFF_FISH_MISSILE);
	}

	else if (myNodeType == PROP_VORTEX_TAG)
	{
		GameSound::playEffectMusic(EFF_SEABOTTOM);
		//myNode->setVisible(false);

		auto prop = getSpriteHelper(myNode);
		prop->setTag(-1);
		prop->stopAllActions();
		auto animation = Animation::create();
		for(int i=1;i<5;i++)
		{
			std::string spriteFrameName = StringUtils::format("prop_vortex_%d.png",i);
			//SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
			animation->addSpriteFrameWithFile(spriteFrameName);
		}
		animation->setDelayPerUnit(0.02f);
		animation->setRestoreOriginalFrame(false);
		auto action = Animate::create(animation);
		auto callFunc = CallFunc::create([myNode]()
            {
				myNode->setVisible(false);

				auto scene = Director::getInstance()->getRunningScene();
				auto battleScene = dynamic_cast<BattleScene*>(scene);

				battleScene->resume();
			});

		EventCustom event(EVENT_VORTEX_TOUCHED);
                    
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
		
		auto acts = Repeat::create(action,10);
		auto seq = Sequence::createWithTwoActions(acts, callFunc);
		auto distance = Director::getInstance()->getWinSize().height - prop->getPosition().y + 100;
		auto move = MoveTo::create(1.5,Point(prop->getPosition().x,prop->getPosition().y + distance));
		auto spawn = Spawn::createWithTwoActions(seq,move);
		auto scene = Director::getInstance()->getRunningScene();
		auto battleScene = dynamic_cast<BattleScene*>(scene);

		battleScene->pause();

		prop->runAction(spawn);
	}

	else if (myNodeType == PROP_SHIP_TAG)
	{
		GameSound::playEffectMusic(EFF_FLIGHT_BOOSTER, 1);
		myNode->setVisible(false);
	}
}

int LadderLayer::getCollideNodeType(Node *myNode)
{
	auto sprite = getSpriteHelper(myNode);

	return sprite->getTag();
}

Sprite* LadderLayer::getSpriteHelper(Node *node)
{
	if (0 != node->getChildren().size())
	{
		auto sprite = static_cast<Sprite*>(node->getChildren().back());

		return sprite;
	}

	return nullptr;
}

Node* LadderLayer::getLadderSetHelper()
{
	Node *ladderSet = nullptr;

	ladderSet = LevelHelper::getInstance()->getLadderSet(getLevelType());

	//when reward level is over
	if (nullptr == ladderSet) 
	{
		if (getLevelType() == 1)
		{
			//LevelHelper::getInstance()->chooseRewardLevel(m_rewardLevel++);

			ladderSet = LevelHelper::getInstance()->getLadderSet(0);

			std::string eventName = EVENT_REWARD_LEVELS_END;

			EventCustom event(eventName);
            
            event.setUserData(&m_rewardLevel);
            
            m_rewardLevel++;
        
			Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);

			setLevelType(0);
            
            if (m_rewardLevel > 5) {m_rewardLevel = 1;}
		}
		else
		{
            std::string eventName = EVENT_BATTLE_LEVELS_END;
            
			EventCustom event(eventName);
            
			Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
            
			return nullptr;
		}

	}
	
    // register touch click event
    

	//pre precess ladder
	for (auto ladderNode : ladderSet->getChildren())
	{
		auto ladder = getSpriteHelper(ladderNode);

		CCASSERT(ladder != nullptr, "[LadderLayer] ladder NULL");

		auto ladderType = ladder->getTag();
        
        if (-1 == ladderType) {continue;}

		if (LADDER_MOVE_LANDSCAPE_LONG_TAG == ladderType || LADDER_MOVE_LANDSCAPE_SHORT_TAG == ladderType)
		{
			int speed = 100;

			auto pos = ladder->convertToWorldSpaceAR(Point::ZERO);

			float moveRightDistance = Director::getInstance()->getWinSize().width - pos.x - ladder->getBoundingBox().size.width/2;

			float moveLeftDistance = pos.x - ladder->getBoundingBox().size.width/2;

			auto moveByRight = MoveBy::create(moveRightDistance/speed, Point(moveRightDistance, 0));

			auto moveByBackRight = moveByRight->reverse();

			auto moveByLeft = MoveBy::create(moveLeftDistance/speed, Point(-moveLeftDistance, 0));

			auto moveByBackLeft = moveByLeft->reverse();

			auto seq = Sequence::create(moveByRight, moveByBackRight, moveByLeft, moveByBackLeft, NULL);

			auto repeat = RepeatForever::create(seq);

			ladderNode->runAction(repeat);
		}

		else if (LADDER_MOVE_PORTRAIT_LONG_TAG == ladderType || LADDER_MOVE_PORTRAIT_SHORT_TAG == ladderType)
		{
			int speed = 100;

			float moveUpDistance = Director::getInstance()->getWinSize().height/3;

			float moveDownDistance = Director::getInstance()->getWinSize().width/3;

			auto moveByUp = MoveBy::create(moveUpDistance/speed, Point(0, moveUpDistance));

			auto moveByBackUp = moveByUp->reverse();

			auto moveByDown = MoveBy::create(moveDownDistance/speed, Point(0, -moveDownDistance));

			auto moveByBackDown = moveByDown->reverse();

			auto seq = Sequence::create(moveByDown, moveByBackDown, moveByUp, moveByBackUp, NULL);

			auto repeat = RepeatForever::create(seq);

			ladderNode->runAction(repeat);
		}

		else if (LADDER_HIDE_LONG_TAG == ladderType || LADDER_HIDE_SHORT_TAG == ladderType)
		{
			auto blink = Blink::create(3, 1);
			auto repeat = RepeatForever::create(blink);
			ladderNode->runAction(repeat);
			/*auto animation = Animation::create();
					for(int i=1;i<=2;++i)
					{
						std::string spriteFrameName = StringUtils::format("Hide_Ladder_%d.png",i);
						SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
						animation->addSpriteFrame(frame);
					}
					animation->setDelayPerUnit(1.f);
					animation->setRestoreOriginalFrame(false);
					auto action = Animate::create(animation);
					ladder->runAction(RepeatForever::create(action));*/
		}

		else if(LADDER_BOMB_TAG == ladderType)
		{
			auto callFunc = CallFunc::create([ladder](){
				if (ladder->getTag() == LADDER_BOMB_TAG) 
				{
					GameSound::playEffectMusic(EFF_BOMB_LADDER);
					auto animation = Animation::create();
					for(int i=1;i<=4;++i)
					{
						std::string spriteFrameName = StringUtils::format("ladder_bomb_%d.png",i);
						SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
						animation->addSpriteFrame(frame);
					}
					animation->setDelayPerUnit(0.05f);
					animation->setRestoreOriginalFrame(false);
					auto action = Animate::create(animation);
					ladder->runAction(action);
					ladder->setTag(LADDER_BOMB_OFF_TAG);
				}

				else if (ladder->getTag() == LADDER_BOMB_OFF_TAG)
				{
					auto animation = Animation::create();
					for(int i=5;i<=7;++i)
					{
						std::string spriteFrameName = StringUtils::format("ladder_bomb_%d.png",i);
						SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
						animation->addSpriteFrame(frame);
					}
					SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("ladder_bomb_1.png");
					animation->addSpriteFrame(frame);
					animation->setDelayPerUnit(0.05f);
					animation->setRestoreOriginalFrame(false);
					auto action = Animate::create(animation);
					ladder->runAction(action);
					ladder->setTag(LADDER_BOMB_TAG);
				}
			});
			auto seq = Sequence::createWithTwoActions(DelayTime::create(3), callFunc);
			auto forever = RepeatForever::create(seq);
			ladderNode->runAction(forever);
		}

		else if (LADDER_CONTROL_TAG	== ladderType)
		{
			auto listener = EventListenerTouchOneByOne::create();
			listener->setSwallowTouches(true);

			listener->onTouchBegan = [=](Touch* touch, Event* event){
				auto target = static_cast<Sprite*>(event->getCurrentTarget());
				Point locationInNode = target->convertToNodeSpace(touch->getLocation());
				Size s = target->getContentSize();
				Rect rect = Rect(0, 0, s.width, s.height);
				if (rect.containsPoint(locationInNode))
				{
					return true;
				}
				return false;
			};

			listener->onTouchMoved = [=](Touch* touch, Event* event){
				auto target = static_cast<Sprite*>(event->getCurrentTarget());
				target->setPosition(target->getPosition()+touch->getDelta());
			};

			listener->onTouchEnded = [=](Touch* touch, Event* event){
				
			};
			_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, ladder);

			//_eventDispatcher->setPriority(listener,1);
			//_eventDispatcher->addEventListenerWithFixedPriority(listener, 1);
		}

		else if (MONSTER_PIRANHA_BIG_TAG == ladderType)
		{
			GameSound::playEffectMusic(EFF_MONSTER_APPEAR);
			auto animation = Animation::create();
			for(int i=1;i<=6;++i)
			{
				std::string spriteFrameName = StringUtils::format("Moster_Piranha_big_%d.png",i);
				SpriteFrame *frame = SpriteFrameCache::getInstance()->spriteFrameByName(spriteFrameName);
				animation->addSpriteFrame(frame);
			}
			animation->setDelayPerUnit(0.1f);
			animation->setRestoreOriginalFrame(true);
			auto action = Animate::create(animation);
			auto forever = RepeatForever::create(action);
			ladder->runAction(forever);
            
            ///////////////////////////// Register Touch Event /////////////////////////////
            auto listener = EventListenerTouchOneByOne::create();
            
			listener->setSwallowTouches(true);
            
			listener->onTouchBegan = [=](Touch* touch, Event* event)
            {
				auto target = static_cast<Sprite*>(event->getCurrentTarget());
				
                Point locationInNode = target->convertToNodeSpace(touch->getLocation());
				
                Size s = target->getContentSize();
				
                Rect rect = Rect(0, 0, s.width, s.height);
				
                if (rect.containsPoint(locationInNode))
				{
                    // this monster hit by Touch, notify heroLayer
                    EventCustom event(EVENT_MONSTER_TOUCHED);
                    
                    Point location = touch->getLocation();
                    
                    event.setUserData(&location);
                    
                    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
                    
					return true;
				}
				
                return false;
			};
            
			listener->onTouchMoved = [=](Touch* touch, Event* event)
            {
			};
            
			listener->onTouchEnded = [=](Touch* touch, Event* event){
			};
			
            _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, ladder);
            ///////////////////////////// Register Touch Event End/////////////////////////////
		}

		else if (MONSTER_PIRANHA_NORMAL_TAG == ladderType)
		{
			GameSound::playEffectMusic(EFF_MONSTER_APPEAR);
			auto animation = Animation::create();
			for(int i=1;i<=6;++i)
			{
				std::string spriteFrameName = StringUtils::format("Moster_Piranha_normal_%d.png",i);
				SpriteFrame *frame = SpriteFrameCache::getInstance()->spriteFrameByName(spriteFrameName);
				animation->addSpriteFrame(frame);
			}
			animation->setDelayPerUnit(0.1f);
			animation->setRestoreOriginalFrame(true);
			auto action = Animate::create(animation);
			auto forever = RepeatForever::create(action);
			ladder->runAction(forever);
            
            ///////////////////////////// Register Touch Event /////////////////////////////
            auto listener = EventListenerTouchOneByOne::create();
            
			listener->setSwallowTouches(true);
            
			listener->onTouchBegan = [=](Touch* touch, Event* event)
            {
				auto target = static_cast<Sprite*>(event->getCurrentTarget());
				
                Point locationInNode = target->convertToNodeSpace(touch->getLocation());
				
                Size s = target->getContentSize();
				
                Rect rect = Rect(0, 0, s.width, s.height);
				
                if (rect.containsPoint(locationInNode))
				{
                    // this monster hit by Touch, notify heroLayer
                    EventCustom event(EVENT_MONSTER_TOUCHED);
                    
                    Point location = touch->getLocation();
                    
                    event.setUserData(&location);
                    
                    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
                    
					return true;
				}
				
                return false;
			};
            
			listener->onTouchMoved = [=](Touch* touch, Event* event)
            {
			};
            
			listener->onTouchEnded = [=](Touch* touch, Event* event){
			};
			
            _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, ladder);
            ///////////////////////////// Register Touch Event End/////////////////////////////
		}

		else if (MONSTER_PIRANHA_SMALL_TAG == ladderType)
		{
			GameSound::playEffectMusic(EFF_MONSTER_APPEAR);
			auto animation = Animation::create();
			for(int i=1;i<=6;++i)
			{
				std::string spriteFrameName = StringUtils::format("Moster_Piranha_small_%d.png",i);
				SpriteFrame *frame = SpriteFrameCache::getInstance()->spriteFrameByName(spriteFrameName);
				animation->addSpriteFrame(frame);
			}
			animation->setDelayPerUnit(0.1f);
			animation->setRestoreOriginalFrame(true);
			auto action = Animate::create(animation);
			auto forever = RepeatForever::create(action);
			ladder->runAction(forever);
            
            ///////////////////////////// Register Touch Event /////////////////////////////
            auto listener = EventListenerTouchOneByOne::create();
            
			listener->setSwallowTouches(true);
            
			listener->onTouchBegan = [=](Touch* touch, Event* event)
            {
				auto target = static_cast<Sprite*>(event->getCurrentTarget());
				
                Point locationInNode = target->convertToNodeSpace(touch->getLocation());
				
                Size s = target->getContentSize();
				
                Rect rect = Rect(0, 0, s.width, s.height);
				
                if (rect.containsPoint(locationInNode))
				{
                    // this monster hit by Touch, notify heroLayer
                    EventCustom event(EVENT_MONSTER_TOUCHED);
                    
                    Point location = touch->getLocation();
                    
                    event.setUserData(&location);
                    
                    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
                    
					return true;
				}
				
                return false;
			};
            
			listener->onTouchMoved = [=](Touch* touch, Event* event)
            {
			};
            
			listener->onTouchEnded = [=](Touch* touch, Event* event){
			};
			
            _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, ladder);
            ///////////////////////////// Register Touch Event End/////////////////////////////
		}

		else if (MONSTER_JELLYFISH_BIG_TAG == ladderType)
		{
			GameSound::playEffectMusic(EFF_MONSTER_APPEAR);

			int speed = 100;

			float moveUpDistance = Director::getInstance()->getWinSize().height/3;

			float moveDownDistance = Director::getInstance()->getWinSize().width/3;

			auto moveByUp = MoveBy::create(moveUpDistance/speed, Point(0, moveUpDistance));

			auto moveByBackUp = moveByUp->reverse();

			auto moveByDown = MoveBy::create(moveDownDistance/speed, Point(0, -moveDownDistance));

			auto moveByBackDown = moveByDown->reverse();

			auto seq = Sequence::create(moveByDown, moveByBackDown, moveByUp, moveByBackUp, NULL);

			auto repeat = RepeatForever::create(seq);

			ladderNode->runAction(repeat);
            
            ///////////////////////////// Register Touch Event /////////////////////////////
            auto listener = EventListenerTouchOneByOne::create();
            
			listener->setSwallowTouches(true);
            
			listener->onTouchBegan = [=](Touch* touch, Event* event)
            {
				auto target = static_cast<Sprite*>(event->getCurrentTarget());
				
                Point locationInNode = target->convertToNodeSpace(touch->getLocation());
				
                Size s = target->getContentSize();
				
                Rect rect = Rect(0, 0, s.width, s.height);
				
                if (rect.containsPoint(locationInNode))
				{
                    // this monster hit by Touch, notify heroLayer
                    EventCustom event(EVENT_MONSTER_TOUCHED);
                    
                    Point location = touch->getLocation();
                    
                    event.setUserData(&location);
                    
                    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
                    
					return true;
				}
				
                return false;
			};
            
			listener->onTouchMoved = [=](Touch* touch, Event* event)
            {
			};
            
			listener->onTouchEnded = [=](Touch* touch, Event* event){
			};
			
            _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, ladder);
            ///////////////////////////// Register Touch Event End/////////////////////////////
		}

		else if (MONSTER_JELLYFISH_SMALL_TAG == ladderType)
		{
			GameSound::playEffectMusic(EFF_MONSTER_APPEAR);

			int speed = 100;

			float moveUpDistance = Director::getInstance()->getWinSize().height/3;

			float moveDownDistance = Director::getInstance()->getWinSize().width/3;

			auto moveByUp = MoveBy::create(moveUpDistance/speed, Point(0, moveUpDistance));

			auto moveByBackUp = moveByUp->reverse();

			auto moveByDown = MoveBy::create(moveDownDistance/speed, Point(0, -moveDownDistance));

			auto moveByBackDown = moveByDown->reverse();

			auto seq = Sequence::create(moveByDown, moveByBackDown, moveByUp, moveByBackUp, NULL);

			auto repeat = RepeatForever::create(seq);

			ladderNode->runAction(repeat);
            
            ///////////////////////////// Register Touch Event /////////////////////////////
            auto listener = EventListenerTouchOneByOne::create();
            
			listener->setSwallowTouches(true);
            
			listener->onTouchBegan = [=](Touch* touch, Event* event)
            {
				auto target = static_cast<Sprite*>(event->getCurrentTarget());
				
                Point locationInNode = target->convertToNodeSpace(touch->getLocation());
				
                Size s = target->getContentSize();
				
                Rect rect = Rect(0, 0, s.width, s.height);
				
                if (rect.containsPoint(locationInNode))
				{
                    // this monster hit by Touch, notify heroLayer
                    EventCustom event(EVENT_MONSTER_TOUCHED);
                    
                    Point location = touch->getLocation();
                    
                    event.setUserData(&location);
                    
                    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
                    
					return true;
				}
				
                return false;
			};
            
			listener->onTouchMoved = [=](Touch* touch, Event* event)
            {
			};
            
			listener->onTouchEnded = [=](Touch* touch, Event* event){
			};
			
            _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, ladder);
            ///////////////////////////// Register Touch Event End/////////////////////////////
		}

		else if (MONSTER_SEASNAKE_BIG_TAG == ladderType)
		{
			GameSound::playEffectMusic(EFF_MONSTER_APPEAR);
			auto animation = Animation::create();
			for(int i=1;i<=6;++i)
			{
				std::string spriteFrameName = StringUtils::format("Moster_seasnake_big_%d.png",i);
				SpriteFrame *frame = SpriteFrameCache::getInstance()->spriteFrameByName(spriteFrameName);
				animation->addSpriteFrame(frame);
			}
			animation->setDelayPerUnit(0.1f);
			animation->setRestoreOriginalFrame(true);
			auto action = Animate::create(animation);
			auto forever = RepeatForever::create(action);
			ladder->runAction(forever);

			int speed = 100;

			auto pos = ladder->convertToWorldSpaceAR(Point::ZERO);

			float moveFromRightDistance = Director::getInstance()->getWinSize().width - pos.x + ladder->getBoundingBox().size.width/2;

			float moveLeftDistance = pos.x + ladder->getBoundingBox().size.width/2;

			auto moveByLeft = MoveBy::create(moveLeftDistance/speed, Point(-moveLeftDistance, 0));

			auto callFunc = CallFunc::create([ladder,ladderNode](){

				ladderNode->setPositionX(Director::getInstance()->getWinSize().width + ladder->getBoundingBox().size.width/2);

			});

			auto moveformRight = MoveBy::create(moveFromRightDistance/speed, Point(-moveFromRightDistance, 0));

			auto seq = Sequence::create(moveByLeft, callFunc, moveformRight, NULL);

			auto repeat = RepeatForever::create(seq);

			ladderNode->runAction(repeat);
            
            ///////////////////////////// Register Touch Event /////////////////////////////
            auto listener = EventListenerTouchOneByOne::create();
            
			listener->setSwallowTouches(true);
            
			listener->onTouchBegan = [=](Touch* touch, Event* event)
            {
				auto target = static_cast<Sprite*>(event->getCurrentTarget());
				
                Point locationInNode = target->convertToNodeSpace(touch->getLocation());
				
                Size s = target->getContentSize();
				
                Rect rect = Rect(0, 0, s.width, s.height);
				
                if (rect.containsPoint(locationInNode))
				{
                    // this monster hit by Touch, notify heroLayer
                    EventCustom event(EVENT_MONSTER_TOUCHED);
                    
                    Point location = touch->getLocation();
                    
                    event.setUserData(&location);
                    
                    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
                    
					return true;
				}
				
                return false;
			};
            
			listener->onTouchMoved = [=](Touch* touch, Event* event)
            {
			};
            
			listener->onTouchEnded = [=](Touch* touch, Event* event){
			};
			
            _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, ladder);
            ///////////////////////////// Register Touch Event End/////////////////////////////
		}

		else if (MONSTER_SEASNAKE_SMALL_TAG == ladderType)
		{
			GameSound::playEffectMusic(EFF_MONSTER_APPEAR);
			auto animation = Animation::create();
			for(int i=1;i<=6;++i)
			{
				std::string spriteFrameName = StringUtils::format("Moster_seasnake_small_%d.png",i);
				SpriteFrame *frame = SpriteFrameCache::getInstance()->spriteFrameByName(spriteFrameName);
				animation->addSpriteFrame(frame);
			}
			animation->setDelayPerUnit(0.1f);
			animation->setRestoreOriginalFrame(true);
			auto action = Animate::create(animation);
			auto forever = RepeatForever::create(action);
			ladder->runAction(forever);

			int speed = 100;

			auto pos = ladder->convertToWorldSpaceAR(Point::ZERO);

			float moveFromRightDistance = Director::getInstance()->getWinSize().width - pos.x + ladder->getBoundingBox().size.width/2;

			float moveLeftDistance = pos.x + ladder->getBoundingBox().size.width/2;

			auto moveByLeft = MoveBy::create(moveLeftDistance/speed, Point(-moveLeftDistance, 0));

			auto callFunc = CallFunc::create([ladder,ladderNode](){

				ladderNode->setPositionX(Director::getInstance()->getWinSize().width + ladder->getBoundingBox().size.width/2);

			});

			auto moveformRight = MoveBy::create(moveFromRightDistance/speed, Point(-moveFromRightDistance, 0));

			auto seq = Sequence::create(moveByLeft, callFunc, moveformRight, NULL);

			auto repeat = RepeatForever::create(seq);

			ladderNode->runAction(repeat);
            
            ///////////////////////////// Register Touch Event /////////////////////////////
            auto listener = EventListenerTouchOneByOne::create();
            
			listener->setSwallowTouches(true);
            
			listener->onTouchBegan = [=](Touch* touch, Event* event)
            {
				auto target = static_cast<Sprite*>(event->getCurrentTarget());
				
                Point locationInNode = target->convertToNodeSpace(touch->getLocation());
				
                Size s = target->getContentSize();
				
                Rect rect = Rect(0, 0, s.width, s.height);
				
                if (rect.containsPoint(locationInNode))
				{
                    // this monster hit by Touch, notify heroLayer
                    EventCustom event(EVENT_MONSTER_TOUCHED);
                    
                    Point location = touch->getLocation();
                    
                    event.setUserData(&location);
                    
                    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
                    
					return true;
				}
				
                return false;
			};
            
			listener->onTouchMoved = [=](Touch* touch, Event* event)
            {
			};
            
			listener->onTouchEnded = [=](Touch* touch, Event* event){
			};
			
            _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, ladder);
            ///////////////////////////// Register Touch Event End/////////////////////////////
		}

		else if (PROP_TORPEDO_TAG == ladderType)
		{
            GameSound::playEffectMusic(EFF_FISH_MISSILE);
			int speed = 250;
			auto moveByDown = MoveBy::create(LADDER_SET_HEIGHT/speed, Point(0, -LADDER_SET_HEIGHT*2));
			ladderNode->runAction(moveByDown);
			auto animation = Animation::create();
			for(int i=1;i<=4;++i)
			{
				std::string spriteFrameName = StringUtils::format("prop_torpedo_%d.png",i);
				SpriteFrame *frame = SpriteFrameCache::getInstance()->spriteFrameByName(spriteFrameName);
				animation->addSpriteFrame(frame);
			}
			animation->setDelayPerUnit(0.1f);
			animation->setRestoreOriginalFrame(true);
			auto action = Animate::create(animation);
			auto forever = RepeatForever::create(action);
			ladder->runAction(forever);

			std::string eventName = EVENT_WARNING_END;
			EventCustom event(eventName);
			float x = ladderNode->getPositionX();
			event.setUserData(&x);
			Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
		}

		else if (PROP_VORTEX_TAG == ladderType)
		{

		}

		else if (PROP_SPRING_TAG == ladderType)
		{

		}

		else if (PROP_TRAMPOLINE_TAG == ladderType)
		{

		}

		else if (-1 == ladderType)
		{

		}
	}

	return ladderSet;
}

void LadderLayer::handleMagnetEffect(Node *node, float dt)
{
	if (node->getTag() == PROP_GOLD_ABSORB_TAG)
	{
		float speed = 1000;

		float distance = speed * dt;

		auto nodePos = getSpriteHelper(node)->convertToWorldSpaceAR(Point::ZERO);

		auto heroPos = getHeroPos();

		auto direction = heroPos - nodePos;

		direction = direction.normalize() * distance;

		auto flyToPos = node->getPosition() + direction;

		node->setPosition(flyToPos);
	}
}

void LadderLayer::ntf(HeroNtf &heroNtf)
{
    //scrollDown(heroNtf.scrollDistance);
    
    // scorll down screen
    if (heroNtf.scrollDistance > 0)
    {
        scrollDown(heroNtf.scrollDistance);
    }
    
    // refresh hero pos
    if (0 != heroNtf.x && 0 != heroNtf.y)
    {
        setHeroPos(Point(heroNtf.x, heroNtf.y));
    }
    
    // magnet state process
    if (0 != heroNtf.magnetState)
    {
        setMagnetState(heroNtf.magnetState);
    }
    
    // is enter reward levels
    if (0 != heroNtf.levelType)
    {
        setLevelType(heroNtf.levelType);
    }
}
