//
//  HeroLayer.cpp
//  HiFly
//
//  Created by chris li on 14-5-13.
//
//
#include "BattleDefine.h"
#include "HeroLayer.h"
#include "cocostudio/CocoStudio.h"
#include "CCNodeGrid.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCCustomCommand.h"
#include "BattleScene.h"
#include "BattleData.h"
#include "GameOverLayer.h"
#include "home/HomeScene.h"
#include "Player.h"
#include "util/GameSound.h"
#include "RoleLevelInfo.h"
#include "Network.h"
#include "LevelGameOver.h"
#include "SimpleAudioEngine.h"
#include "common/NotifyBoxUILayer.h"

using namespace cocos2d;
using namespace cocostudio;

bool HeroLayer::init()
{
    if (Layer::init())
    {
        // init property
        m_heroAcc = Point(0, -1600.0f);
        
        m_heroVel = Point(0, 0);
        
        m_heroPos = Point(Director::getInstance()->getWinSize().width/2, Director::getInstance()->getWinSize().height/4);
        
        //this->setPosition(Director::getInstance()->getWinSize().width/2, Director::getInstance()->getWinSize().height/4);
        
        m_bounceSpeed = BOUNCE_NORMAL_SPEED;

		m_sliderPer = 0.f;
        
        setSpringCollideOk(false);
        
        setTrampolineCollideOk(false);
        
        setHelicopterCollideOk(false);
        
        setShieldCollideOk(false);

		setShipCollideOk(false);
        
        setMedicineCollideOk(false);
        
        setTouchClickOk(false);
        
        setJumpReadyOverOk(true);
        
        setJumpAgain1OverOk(false);

		setJumpAgain2Running(false);
        
        setRewardLevelsOver(false);
        
        setJumpAgainToken(false);

		setJumpDeath(false);

		setJumpDeath1OverOk(false);

		setJumpPauseOk(false);

		setJumpResumeOk(false);

		setIsDeath(false);

		setIsShield(false);

		setIsHelicopter(false);

		setIsReward(false);

		setEnterInvincible(false);

		setExitInvincible(false);

		setIsAttackJump(false);

		setIsBig(false);

		setIsStartSprint(false);

		setIsDeathSprint(0);
        
        setMonsterTouchedOk(false);
        
        setAccEnable(true);
        
        setMonsterKilledOk(false);
        
        setLookRight(false);
        
        setBattleLevelOver(false);
		
		setIsFriendMod(false);

		setMonsterTouchedEnable(true);

		setIsBubbleRunning(false);
		
		setIsSuperBubble(false);

		setIsSuperBubbleRunning(false);
        
        setIsHelicopterRunning(false);
        
        setIsShipRunning(false);
        
        setGold(0);
        
        setStar(0);

		setJumpDeathSpriteReady(false);

		setIsGoingReward(false);

		setIsSeaHorseReadyToAttack(true);
        
        setThreeSecondInvincibleOk(false);
        
        setIsStartSpriteRunning(false);

		setHpMinusReady(true);
        
		setIsFriendRunning(false);

		setTorpedoAttacked(false);

		setLadderBombAttacked(false);

		setIsShipEndRunning(false);

		setIsHelicopterRunning(false);

		setIsThreeSecondRunning(false);

		setResurrectionTimes(3);

		setIsResourrectionRunning(false);

		setIsResourrection(false);

		setIsRealDie(false);

		setIsBuyResourrection(false);

        //create Hero
        resetHero(Player::getInstance()->getRoleType(), Player::getInstance()->getLevel());
        
        //setTouchEnabled(true);
        
        //setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
        
        return true;
    }
    
    return false;
}

void HeroLayer::resetHero(int roleType, int level)
{
    setIsDeath(false);
	setJumpDeath(false);
	setBounceOk(true);
	setBounceSpeed(BOUNCE_NORMAL_SPEED);
    
	BattleData::getInstance()->setIsRealDie(false);
    //setEnterInvincible(true);
    
    
    
	if (getIsFriendMod())
	{
		setIsStartSprint(true);
		setThreeSecondInvincibleOk(false);
		getJumpFSM()->changeState(JumpReadyState::getInstance());
	}

	if (getIsResourrection())
	{
		setIsStartSprint(true);
		setThreeSecondInvincibleOk(false);
		getJumpFSM()->changeState(JumpReadyState::getInstance());
	}
	

	if (!getIsFriendMod() && !getIsResourrection())
	{
        setThreeSecondInvincibleOk(true);
	}

    // reset hero battle config data
	BattleData::getInstance()->setRoleLevel(Player::getInstance()->getLevel());
    BattleData::getInstance()->setRoleType(Player::getInstance()->getRoleType());
    
    if (Player::getInstance()->getPropRandom() == 23)
    {
        BattleData::getInstance()->setRoleLevel(30);
        BattleData::getInstance()->setRoleType(3);
        roleType = 1;
        
    }
    else if (Player::getInstance()->getPropRandom() == 24)
    {
        BattleData::getInstance()->setRoleLevel(40);
        BattleData::getInstance()->setRoleType(7);
        roleType = 4;
    }

	if(roleType < 4)
    {
        BattleData::getInstance()->setHp(1);
        EventCustom event("EVENT_CHANGE_HEART1");
        
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    }
    else if (roleType > 3)
    {
        BattleData::getInstance()->setHp(2);
        EventCustom event("EVENT_CHANGE_HEART2");
        
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);

    }

	BattleData::getInstance()->setProp1(Player::getInstance()->getProp1Num());
	BattleData::getInstance()->setProp2(Player::getInstance()->getProp2Num());
	BattleData::getInstance()->setProp3(Player::getInstance()->getProp3Num());
	BattleData::getInstance()->setProp4(Player::getInstance()->getProp4Num());
	BattleData::getInstance()->setPropRandom(Player::getInstance()->getPropRandom());
	//Player::getInstance()->setIsMore(false);
	//log("Player->getIsMore()~~~~~~%d", Player::getInstance()->getIsMore());
	if (BattleData::getInstance()->getBattleMode() == 0 )
	{

		if (BattleData::getInstance()->getProp1() > 0)
		{
			if (!getIsFriendMod())
			{
				setIsStartSprint(true);
                setThreeSecondInvincibleOk(false);
			}
			
		}

		if (BattleData::getInstance()->getProp3() > 0)
		{
//			if (getIsFriendMod())
//			{
//				setIsDeathSprint(0);
//			}
//			else
//			{
			if (!getIsResourrection())
			{
				setIsDeathSprint(1);
			}
				
				//BattleData::getInstance()->setProp3(0);	
				
		}
		if (BattleData::getInstance()->getProp4() > 0)
		{
		
			BattleData::getInstance()->setEffectRole(0.2f);
		}

	}
    if (m_heroes.size() != 0)
    {
        auto preHero = m_heroes.back();
    
        m_heroes.popBack();
    
        this->removeChild(preHero);
    }
    
    int heroTag = HERO_1_TAG;
    
    if (roleType >= 1 && roleType <= 3)
    {
        heroTag = HERO_2_TAG;
        
        setCanChangeDirection(true);
    }
    
    else if (roleType >= 4)
    {
        heroTag = HERO_1_TAG;
        
        setCanChangeDirection(false);
    }
    
    Hero *hero = Hero::create(heroTag);
    hero->setScale(1.2f);
    m_heroes.pushBack(hero);
    
    this->addChild(hero);
    
    heroJumpUp();
}

void HeroLayer::onEnter()
{
    Node::onEnter();
    
	m_playTimeCounter = 0;

    do
    {
		//setIsStartSprint(true);
		//setIsDeathSprint(1);
		// init state machine FSM
        setAccelerometerEnabled(true);
        
        setAccelerometerInterval(1.0f/10.0f);
        
		m_propFSM = new StateMachine<HeroLayer>(this);

		m_propFSM->setCurrentState(PropNormalState::getInstance());

		m_propFSM->setGlobalState(nullptr);
        
        m_onceFSM = new StateMachine<HeroLayer>(this);
        
        m_onceFSM->setCurrentState(OnceNormalState::getInstance());
        
        m_jumpFSM = new StateMachine<HeroLayer>(this);
        
        m_jumpFSM->setCurrentState(JumpReadyState::getInstance());

		m_lifeFSM = new StateMachine<HeroLayer>(this);
        
        m_lifeFSM->setCurrentState(LifeNormalState::getInstance());
        
        // listen jump ready over event that emitted by Hero after play jumpReady animation
        auto jumpReadyListener = EventListenerCustom::create(EVENT_JUMP_READY_OVER, [this](EventCustom* event)
        {
            this->setJumpReadyOverOk(true);
        });
        
        _eventDispatcher->addEventListenerWithFixedPriority(jumpReadyListener, 1);
        
        
        auto jumpAgain1Listener = EventListenerCustom::create(EVENT_JUMP_AGAIN1_OVER, [this](EventCustom* event)
        {
            this->setJumpAgain1OverOk(true);
        });
        
        _eventDispatcher->addEventListenerWithFixedPriority(jumpAgain1Listener, 1);
        

		auto jumpDeath1Listener = EventListenerCustom::create(EVENT_JUMP_DEATH1_OVER, [this](EventCustom* event)
        {
            this->setJumpDeath1OverOk(true);
        });
        
        _eventDispatcher->addEventListenerWithFixedPriority(jumpDeath1Listener, 1);
        
        
        auto rewardLevelsEndListener = EventListenerCustom::create(EVENT_REWARD_LEVELS_END, [this](EventCustom* event)
        {
			GameSound::playBackgroundMusic(MUSIC_LAYER_INGAME);

            this->setRewardLevelsOver(true);
        });
        
        _eventDispatcher->addEventListenerWithFixedPriority(rewardLevelsEndListener, 1);
        
        auto rewardLevelMagnetStart = EventListenerCustom::create(EVENT_REWARD_MAGNET_START, [this](EventCustom* event)
        {
            PropMagnetState::getInstance()->enter(this);
            m_propFSM->setGlobalState(PropMagnetState::getInstance());

        });
        
        _eventDispatcher->addEventListenerWithFixedPriority(rewardLevelMagnetStart, 1);
        
        auto battleLevelsEndListener = EventListenerCustom::create(EVENT_BATTLE_LEVELS_END, [this](EventCustom* event)
        {
            this->setBattleLevelOver(true);
            
            this->checkGameOver();
        });
        
        _eventDispatcher->addEventListenerWithFixedPriority(battleLevelsEndListener, 1);

		////////////////////////
		auto vortextTouchedListener = EventListenerCustom::create(EVENT_VORTEX_TOUCHED, [this](EventCustom* event)
        {
            auto hero = getCurrHeroHelper();
			hero->setVisible(false);
        });
        
        _eventDispatcher->addEventListenerWithFixedPriority(vortextTouchedListener, 1);
        /////////////////////////

		////////////////////////
		auto jumpEnableTouchedListener = EventListenerCustom::create(EVENT_MONSTER_ENABLE_TOUCHED, [this](EventCustom* event)
        {
            setMonsterTouchedEnable(true);
			
        });
        
        _eventDispatcher->addEventListenerWithFixedPriority(jumpEnableTouchedListener, 1);
        /////////////////////////

		auto attackMonsterReadyListener = EventListenerCustom::create(EVENT_ATTACK_READY, [this](EventCustom* event)
        {
            setIsSeaHorseReadyToAttack(true);
			
        });
        
        _eventDispatcher->addEventListenerWithFixedPriority(attackMonsterReadyListener, 1);

		//////////////////////////////////
        
        auto monsterTouchedListener = EventListenerCustom::create(EVENT_MONSTER_TOUCHED, [this](EventCustom* event)
        {
			if (this->getMonsterTouchedEnable() == false) {return;}
            
            if (getIsHelicopterRunning() || getIsShipRunning() || getIsBig())
            {
                this->setMonsterTouchedOk(false);
            }
            else
            {
				if (getIsSeaHorseReadyToAttack())
				{
					this->setMonsterTouchedOk(true);
				}
				else
				{
					
				}

                
            }
            
            
            auto touchLocation = static_cast<Point*>(event->getUserData());
            
            this->setMonsterTouchLocation(Point(touchLocation->x, touchLocation->y));
        });
        
        _eventDispatcher->addEventListenerWithFixedPriority(monsterTouchedListener, 1);
        
        
        // touch listener
		auto touchListener = EventListenerTouchOneByOne::create();
        
		touchListener->setSwallowTouches(true);
        
        touchListener->debugString = "heroLayerTouch";
		
        touchListener->onTouchBegan = [=](Touch* touch, Event* event)
		{
			setTouchClickOk(true);
            return true;
		};
        
		touchListener->onTouchMoved = [=](Touch* touch, Event* event)
		{
			//auto target = static_cast<Sprite*>(event->getCurrentTarget());
			//target->setPosition(target->getPosition()+touch->getDelta());

			if(!getIsDeath() || getIsStartSprint())
			{
				//m_heroPos.x = touch->getLocation().x;
			}
		};
        
		touchListener->onTouchEnded = [=](Touch* touch, Event* event)
		{
			auto target = static_cast<Sprite*>(event->getCurrentTarget());
		};
        
        // zhangyu register touch listener
        if (getCurrHeroHelper()->getHeroId() == HERO_1_TAG || true)
        {
            _eventDispatcher->addEventListenerWithFixedPriority(touchListener, 1);
        }
        
        this->sceneUpdate(0.001);

    } while (0);
}

void HeroLayer::onExit()
{
    do {
        delete m_propFSM;
        
        m_propFSM = nullptr;
        
        delete  m_jumpFSM;
        
        m_jumpFSM = nullptr;

		delete  m_lifeFSM;
        
        m_lifeFSM = nullptr;
        
        delete m_onceFSM;
        
        m_onceFSM = nullptr;
        
        _eventDispatcher->removeCustomEventListeners(EVENT_JUMP_READY_OVER);
        
        _eventDispatcher->removeCustomEventListeners(EVENT_JUMP_AGAIN1_OVER);

		_eventDispatcher->removeCustomEventListeners(EVENT_REWARD_LEVELS_END);
        
        _eventDispatcher->removeCustomEventListeners(EVENT_JUMP_DEATH1_OVER);
        
        _eventDispatcher->removeCustomEventListeners(EVENT_MONSTER_TOUCHED);
        
        _eventDispatcher->removeCustomEventListeners(EVENT_BATTLE_LEVELS_END);
        
        _eventDispatcher->removeAllEventListeners();
        
    } while (0);
    
    Node::onExit();
}

void HeroLayer::sceneUpdate(float dt)
{
	m_playTimeCounter++;

    updateFSM(dt);
    
    updatePos(dt);
}

void HeroLayer::addMonsterScore(int tag)
{
	if (tag == MONSTER_JELLYFISH_BIG_TAG)
	{
		BattleData::getInstance()->addMonsterScore(120);
	}
	else if (tag == MONSTER_JELLYFISH_SMALL_TAG)
	{
		BattleData::getInstance()->addMonsterScore(85);
	}
	else if (tag == MONSTER_PIRANHA_BIG_TAG)
	{
		BattleData::getInstance()->addMonsterScore(45);
	}
	else if (tag == MONSTER_PIRANHA_NORMAL_TAG)
	{
		BattleData::getInstance()->addMonsterScore(65);
	}
	else if (tag == MONSTER_PIRANHA_SMALL_TAG)
	{
		BattleData::getInstance()->addMonsterScore(85);
	}
	else if (tag == MONSTER_SEASNAKE_BIG_TAG)
	{
		BattleData::getInstance()->addMonsterScore(100);
	}
	else if (tag == MONSTER_SEASNAKE_SMALL_TAG)
	{
		BattleData::getInstance()->addMonsterScore(75);
	}
}

///////////////////////////////////////////////// Colllide Interface API ////////////////////////////////////////////////////////
Vector<Node*>* HeroLayer::getCollideNode()
{
    return &m_heroes;
}

Rect HeroLayer::getCollideRect(Node *myNode, int targetNodeType)
{
    auto worldPos = myNode->convertToWorldSpaceAR(Point(0, 0));
    
    auto size = getSpriteHelper(myNode)->getBoundingBox().size;
    
	if (targetNodeType > MONSTER_BEGIN_TAG &&
		targetNodeType < MONSTER_END_TAG)
	{
		size.height -= 50;
	}
    
    if (PROP_GOLD_SMALL_TAG || PROP_GOLD_TAG)
	{
		size.height -= 50;
	}
    //size.height = 5; size.width = 5;
    
    Rect rect = Rect(worldPos.x-size.width*0.3f, worldPos.y-size.height*0.3f, size.width*0.6f, size.height*0.6f);
	if (getIsDeath())
	{
		return Rect(999999,999999,0,0);
	}

    // 1. collide with ladder, spring, trampoline
    if ((targetNodeType > LADDER_BEGIN_TAG && targetNodeType < LADDER_END_TAG) ||
        targetNodeType == PROP_SPRING_TAG || targetNodeType == PROP_TRAMPOLINE_TAG)
    {
        if (this->getJumpFSM()->getCurrentState() == JumpDownState::getInstance())
        {
            rect = Rect(worldPos.x-size.width/2, worldPos.y-size.height/2, size.width, size.height/3);
        }
        else
        {
			rect = Rect(999999, 999999, 0, 0);
        }
    }
    
    // 2. collide with monster
    if (targetNodeType > MONSTER_BEGIN_TAG && targetNodeType < MONSTER_END_TAG)
    {
        rect = Rect(worldPos.x-size.width*0.3f, worldPos.y-size.height*0.3f, size.width*0.6f, size.height*0.6f);
    }

    return rect;
}

void HeroLayer::handleCollide(Node *myNode, int targetNodeType)
{
    // 1. collide with ladder
    if (targetNodeType > LADDER_BEGIN_TAG && targetNodeType < LADDER_END_TAG)
    {
		if (getPropFSM()->getCurrentState() == PropMedicineState::getInstance())
		{
			GameSound::playEffectMusic(EFF_TOBIG_TOUCHLADDER);
		}
		if (targetNodeType == LADDER_BOMB_OFF_TAG)
		{
			GameSound::playEffectMusic(EFF_BOMB_LADDER);

			if (!getLadderBombAttacked())
			{
				setBounceOk(true);
			
				//if (getIsShield() || getIsBig())
				if (getIsShield())
				{
					setIsShield(false);
				
				}
				else if (getIsBig())
				{

				}
				else 
				{
                    int hp = hp = BattleData::getInstance()->getHp();
                    if (getHpMinusReady())
                    {
                        hp = BattleData::getInstance()->getHp() - 1;
                        BattleData::getInstance()->setHp(hp);
                        setHpMinusReady(false);
                        
                        HeroNtf heroNtf = {0};
                        
                        heroNtf.heroUIDataNtf.tag = 1;
                        //It's a special setting. The value of hp isn't changged.
                        heroNtf.heroUIDataNtf.hp = hp + 1;
                        auto battleScene = static_cast<BattleScene*>(Director::getInstance()->getRunningScene());
                        battleScene->ntf(heroNtf);
                    }
                    
                    if (hp <= 0)
                    {
                        setJumpDeath(true);
                        GameSound::playEffectMusic(EFF_DROP_DOWN);
                    }
				}
			}
			

			setLadderBombAttacked(true);
			
		}
		else if (targetNodeType == LADDER_BROKEN_LONG_TAG)
		{
			setBounceOk(false);
		} 
		else 
		{
			setBounceOk(true);
		}
    }

	else if (targetNodeType > MONSTER_BEGIN_TAG && targetNodeType < MONSTER_END_TAG)
	{
		bool isShield = false;
        if (getJumpFSM()->getCurrentState() == JumpKillMonsterState::getInstance())
        {
			log("Kill Monster!");
			setMonsterKilledOk(true);

			addMonsterScore(targetNodeType);
			BattleData::getInstance()->addMonsterKilledNum(1);
        }
		else
		{
			if (getIsShield())
			{
				//setMonsterTouchedOk(false);
				setIsSeaHorseReadyToAttack(false);
				log("Enter Shield!");
				isShield = true;
				setIsShield(false);
				if (getIsSuperBubble())
				{
					BattleData::getInstance()->setProp2(0);
					setIsSuperBubble(false);
				}
			}
			else if (getIsBig())
			{

			}
			else 
			{
				log("Hp minus!");
				if (isShield == true)
				{
					isShield = false;
					return;
				}
				int hp = hp = BattleData::getInstance()->getHp();
				if (getHpMinusReady())
				{
					hp = BattleData::getInstance()->getHp() - 1;
					BattleData::getInstance()->setHp(hp);
					setHpMinusReady(false);

					HeroNtf heroNtf = {0}; 

					heroNtf.heroUIDataNtf.tag = 1;
					//It's a special setting. The value of hp isn't changged.
					heroNtf.heroUIDataNtf.hp = hp + 1;
					auto battleScene = static_cast<BattleScene*>(Director::getInstance()->getRunningScene());
					battleScene->ntf(heroNtf);
				}

				if (hp <= 0)
				{
					setJumpDeath(true);
					GameSound::playEffectMusic(EFF_DROP_DOWN);
				}
			}	
		}
	}
    
    else if (targetNodeType == PROP_SPRING_TAG)
    {
        setSpringCollideOk(true);
        setBounceOk(true);
    }
    
    else if (targetNodeType == PROP_TRAMPOLINE_TAG)
    {
        setTrampolineCollideOk(true);
        setBounceOk(true);
    }
    
    else if (targetNodeType == PROP_GOLD_TAG)
    {
        setGold(getGold()+1);
    }

	else if (targetNodeType == PROP_GOLD_SMALL_TAG)
	{
		setGold(getGold()+1);
	}
    
    else if (targetNodeType == PROP_STAR_TAG)
    {
        setStar(getStar()+1);
		
		m_sliderPer += 5.f;
		HeroNtf heroNtf = {0};

		heroNtf.heroUIDataNtf.tag = 1;
		heroNtf.heroUIDataNtf.star = 1;

		auto battleScene = static_cast<BattleScene*>(Director::getInstance()->getRunningScene());
		battleScene->ntf(heroNtf);
		//test move to sheild
    }
    
    else if (targetNodeType == PROP_HELICOPTER_TAG && !getIsReward())
    {
        if (getIsStartSpriteRunning() == false)
        {
            setIsHelicopter(true);
            
            setHelicopterCollideOk(true);
        }
    }
    
    else if (targetNodeType == PROP_MAGNET_TAG && !getIsReward())
    {
		PropMagnetState::getInstance()->enter(this);
        m_propFSM->setGlobalState(PropMagnetState::getInstance());
		
    }

	else if (targetNodeType == PROP_SHIELD_TAG && !getIsReward())
    {
        if (getIsStartSpriteRunning() == false)
        {
            setShieldCollideOk(true);
            setIsShield(true);
        }
		
    }
    
    else if (targetNodeType == PROP_MEDICINE_TAG && !getIsReward())
    {
        if (!getIsStartSpriteRunning() || !getIsSuperBubbleRunning())
        {
            if (getIsStartSpriteRunning() == false)
            {
                setMedicineCollideOk(true);
            }
        }
        
		
    }

	 else if (targetNodeType == PROP_TORPEDO_TAG)
    {
        // hero die 这里改成判断状态，不应该在这里直接死亡
		GameSound::playEffectMusic(EFF_HIT_BY_MISSILE);
		if (!getTorpedoAttacked())
		{
			if(!getIsReward())
			{
				if(getIsShield())
				{
					setIsShield(false);
					if (getIsSuperBubbleRunning())
					{
						setIsSuperBubbleRunning(false);
					}
				}

				else 
				{
					setJumpDeath(true);
					GameSound::playEffectMusic(EFF_DROP_DOWN);
				}
			}
		}


		setTorpedoAttacked(true);
    }

	else if (targetNodeType == PROP_VORTEX_TAG)
    {
		GameSound::playEffectMusic(EFF_INTO_SEABOTTOM);
		if (BattleData::getInstance()->getBattleMode() != 0)
		{
			setIsWinProp(true);
			setBattleLevelOver(true);
		}
  
    }

	else if (targetNodeType == PROP_SHIP_TAG && !getIsReward())
    {
        if (getIsStartSpriteRunning() == false)
        {
            setShipCollideOk(true);
        }
		
    }
    
    
    // update FSM
    updateFSM(0);
}

int HeroLayer::getCollideNodeType(Node *myNode)
{
    auto sprite = static_cast<Sprite*>(myNode->getChildren().back());
    
    return sprite->getTag();
}

///////////////////////////////////////////////// Input Handler API ////////////////////////////////////////////////////////
void HeroLayer::onAcceleration(Acceleration* acc, Event* unused_event)
{
    if (getAccEnable() == false) {return;}
    
    float accFilter = 0.05f;
    
    m_heroVel.x = m_heroVel.x * accFilter + acc->x * (1.0f - accFilter) * 2400.0f;
}

bool HeroLayer::onTouchBegan(Touch *touch, Event *unused_event)
{

    // just for test
    /*
    if (getBounceSpeed() == 900.0f)
    {
        setBounceSpeed(900.0f*1.5);
    }
    else
    {
        setBounceSpeed(900.0f);
    }
    */
    
    return true;
}

void HeroLayer::onTouchMoved(Touch *touch, Event *unused_event)
{
	if(!getIsDeath())
	{
		m_heroPos.x = touch->getLocation().x;
	}
}

void HeroLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
    
}

void HeroLayer::updatePos(float dt)
{
    // 1. update X position
    m_heroPos.x += m_heroVel.x * dt;
    
    if (m_heroPos.x > Director::getInstance()->getWinSize().width)
    {
        if (getIsReward() == true)
        {
            m_heroPos.x = Director::getInstance()->getWinSize().width;
        }
        else
        {
            m_heroPos.x = 0;
        }
    }
    else if (m_heroPos.x < 0)
    {
        if (getIsReward() == true)
        {
            m_heroPos.x = 0;
        }
        else
        {
            m_heroPos.x = Director::getInstance()->getWinSize().width;
        }
    }

    // 2. update Y position
    m_heroVel.y += m_heroAcc.y * dt;
    
    m_heroPos.y += m_heroVel.y * dt;
    
    if (m_heroPos.y > Director::getInstance()->getWinSize().height/2)
    {
        float scrollDistance = m_heroPos.y - Director::getInstance()->getWinSize().height/2;
        
        m_heroPos.y = Director::getInstance()->getWinSize().height/2;
        
        // notify other layer to scrool down in calling way
        auto battleScene = static_cast<BattleScene*>(Director::getInstance()->getRunningScene());
        
        HeroNtf heroNtf = {0};
		heroNtf.heroUIDataNtf.gold = getGold();

		heroNtf.heroUIDataNtf.tag = 1;
        
        heroNtf.scrollDistance = scrollDistance;

		if (!getIsReward())
		{
			float h = Director::getInstance()->getWinSize().height / 10;
			float newH = scrollDistance / h;
			float per = newH * 0.5f;

			m_sliderPer += per;

			if (m_sliderPer >= 100.f)
			{
				if (BattleData::getInstance()->getBattleMode() == 0)
				{
					setIsGoingReward(true);

                    /*
					auto prop = Sprite::createWithSpriteFrameName("Reward_Enter_Animate_1");
					Point heroP = getCurrHeroHelper()->getPosition();

					prop->setPosition(heroP);
					//prop->setTag(-1);
					auto animation = Animation::create();
					for(int i=1;i<=9;++i)
					{
						std::string spriteFrameName = StringUtils::format("Reward_Enter_Animate_%d.png",i);
						SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
						animation->addSpriteFrame(frame);
					}
					animation->setDelayPerUnit(0.02f);
					animation->setRestoreOriginalFrame(false);
					auto action = Animate::create(animation);
					auto callFunc = CallFunc::create([prop]()
					{
						prop->setVisible(false);

					});
					auto seq = Sequence::createWithTwoActions(action, callFunc);
					prop->runAction(seq);

					this->addChild(prop);
                    */
					}
					m_sliderPer = 0.f;
			}
		}
        


        battleScene->ntf(heroNtf);
        /*
        // notify other layer to scroll down in message way
        std::string eventName = EVENT_HERO_NTF;
        
        EventCustom event(eventName);
        
        HeroNtf heroNtf = {0};
        
        heroNtf.scrollDistance = scrollDistance;
        
        event.setUserData(&heroNtf);
        
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        */
    }
    
    getCurrHeroHelper()->setPosition(m_heroPos);
    
    // 3. check if there is need to change looking dirction
    if (getCanChangeDirection())
    {
        if (m_heroVel.x < -40 && m_lookRight)
        {
            getCurrHeroHelper()->setScaleX(fabs(getCurrHeroHelper()->getScaleX()));
            
            m_lookRight = false;
        }
        
        else if (m_heroVel.x > 40 && !m_lookRight)
        {
            getCurrHeroHelper()->setScaleX(-1 * fabs(getCurrHeroHelper()->getScaleX()));
            
            m_lookRight = true;
        }
    }
    
    // 4. check if game is over
    checkGameOver();
}

void HeroLayer::sendPropMessage(int tag, bool ret)
{
	auto battleScene = static_cast<BattleScene*>(Director::getInstance()->getRunningScene());
        
	HeroNtf heroNtf = {0};

	heroNtf.heroUIDataNtf.tag = 1;
        
	if (tag == PROP_BUBBLE_TAG)
	{
		heroNtf.heroUIDataNtf.prop2 = 5;
	}
	else if (tag == PROP_JET_TAG)
	{
		heroNtf.heroUIDataNtf.prop3 = 5;
	}
	else if (tag == PROP_HELICOPTER_TAG)
	{
		heroNtf.heroUIDataNtf.prop5 = 5;
	}
	else if (tag == PROP_MAGNET_TAG)
	{
		heroNtf.heroUIDataNtf.prop6 = 5;
	}
	else if (tag == PROP_MUSHROOM_TAG)
	{
		heroNtf.heroUIDataNtf.prop8 = 5;
	}
        
    battleScene->ntf(heroNtf);
}

bool HeroLayer::buyResourrection()
{
	return true;
}

bool HeroLayer::resourrection()
{
	if (!getIsBuyResourrection())
	{
		if (getResurrectionTimes() > 0)
		{
			setIsResourrection(true);
			setIsBuyResourrection(true);

			int r = getResurrectionTimes();

			int d = 8;
			if (r == 3)
			{
				d = 20;
			}
			else if (r == 2)
			{
				d = 40;
			}
			else if (r == 1)
			{
				d = 80;
			}

			auto gameScene = static_cast<BattleScene*>(this->getScene());
			gameScene->pause();
            
            if (Player::getInstance()->getDiamond() < d) {BattleData::getInstance()->setIsRealDie(true);return false;}
            
			NotifyBoxUILayer::getInstance()->showFourString(8,StringUtils::format("%d",d),StringUtils::format("%d",r),"","",
				[this](){
					if (buyResourrection())
					{
						setResurrectionTimes(getResurrectionTimes() - 1);
                        
                        getPropFSM()->changeState(PropNormalState::getInstance());
                        getJumpFSM()->changeState(JumpReadyState::getInstance());
                        getLifeFSM()->changeState(LifeNormalState::getInstance());
                        getOnceFSM()->changeState(OnceNormalState::getInstance());

						resetHero(Player::getInstance()->getRoleType(),Player::getInstance()->getRoleLevel());

						Size winS = Director::getInstance()->getWinSize();
						setHeroPos(Point(winS.width / 2, winS.height /3 ));
                    

						auto gameScene = static_cast<BattleScene*>(Director::getInstance()->getRunningScene());
						gameScene->resume();

						
					}
					else
					{
						BattleData::getInstance()->setIsRealDie(true);

						auto gameScene = static_cast<BattleScene*>(Director::getInstance()->getRunningScene());
						gameScene->resume();
					}
                    
                    NotifyBoxUILayer::getInstance()->stop();
			},
				[this](){
					NotifyBoxUILayer::getInstance()->stop();
					auto gameScene = static_cast<BattleScene*>(Director::getInstance()->getRunningScene());
					BattleData::getInstance()->setIsRealDie(true);
					gameScene->resume();
					
			}
				);
		}
		else
		{
			setIsResourrection(false);
            BattleData::getInstance()->setIsRealDie(true);
		}
	}

	
	return false;
}


void HeroLayer::checkGameOver()
{
	if (getBattleLevelOver() == true)
	{
		BattleData::getInstance()->setGameWin(1);

		int time = (int)m_playTimeCounter / 60.f;
		BattleData::getInstance()->setPlayTime(time);

		auto gameScene = static_cast<BattleScene*>(this->getScene());

		auto resultLayer = LevelGameOver::create();

        this->getScene()->addChild(resultLayer);
    
        gameScene->pause();
	}

	if (getIsFriendMod())
	{
		//just once
		setIsFriendMod(false);

		setIsFriendRunning(true);

		auto gameScene = static_cast<BattleScene*>(this->getScene());
		
		getCurrHeroHelper()->setVisible(false);
		gameScene->pause();
	}

	if (getIsDeathSprint() == 3)
	{
		setIsDeathSprint(0);
		setJumpDeath(true);
	}

    if ((m_heroPos.y < 0))
    {
        // just test
        // m_heroVel.y = getBounceSpeed();
		setIsDeath(true);
        
        auto gameScene = static_cast<BattleScene*>(this->getScene());
        
        // just testing
        // 0 - infinite mode
        if (BattleData::getInstance()->getBattleMode() == 0)
        {
            // friend Help me, continue Jump
            if (BattleData::getInstance()->getHelperPlayerId() != 0)
            {
                auto friendList = Player::getInstance()->getFriendList();
                
                for (auto item : (*friendList))
                {
                    if (item->playerId == BattleData::getInstance()->getHelperPlayerId())
                    {
                        log("Friend Help You!!! Jump Again!!");

						setIsFriendMod(true);
                        
                        resetHero(item->roleType, item->level);
                        
                        BattleData::getInstance()->setHelperPlayerId(0);

						Size winS = Director::getInstance()->getWinSize();
						setHeroPos(Point(winS.width / 2, winS.height /3 ));

						auto myNode = Sprite::createWithSpriteFrameName("FriendHelp_Animate_1.png");
						auto animation = Animation::create();
						for(int i=1;i<=6;++i)
						{
							std::string spriteFrameName = StringUtils::format("FriendHelp_Animate_%d.png",i);
							SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
							animation->addSpriteFrame(frame);
						}
						animation->setDelayPerUnit(0.05f);
						animation->setRestoreOriginalFrame(false);
						auto action = Animate::create(animation);
						auto callFunc = CallFunc::create([=](){
							myNode->removeFromParent();
							auto gameScene = static_cast<BattleScene*>(Director::getInstance()->getRunningScene());
							
							getCurrHeroHelper()->setVisible(true);
							gameScene->resume();
						});
						auto seq = Sequence::createWithTwoActions(action, callFunc);
                        myNode->runAction(seq);
						myNode->setPosition(Point(winS.width / 2, winS.height /3));

						auto helpLabel = Sprite::createWithSpriteFrameName("FriendHelp_Label.png");
						helpLabel->setScale(0.1f);
						helpLabel->setPosition(Point(winS.width / 2, winS.height /3 + 50));
						callFunc = CallFunc::create([helpLabel](){
							helpLabel->removeFromParent();
						});

						seq = Sequence::createWithTwoActions(ScaleTo::create(1,1,1), callFunc);
						helpLabel->runAction(seq);
						this->addChild(helpLabel);
						this->addChild(myNode);

                        return;
                    }
                }
            }       
            // real die
            else 
            {
				if (getIsDeathSprint() == 1)
				{
					setJumpDeathSpriteReady(true);
					return;
				}
				
				if (getIsDeathSprint() != 2)
				{		

					resourrection();
                    
                    BattleData::getInstance()->setReviveTimes(3-getResurrectionTimes());

					if (BattleData::getInstance()->getIsRealDie())
					{
						BattleData::getInstance()->setGameWin(2);

						auto resultLayer = GameOverLayer::create();

						this->getScene()->addChild(resultLayer);
						BattleData::getInstance()->setIsRealDie(false);
						gameScene->pause();
					}

				}


            }

        }
        
        // 1 - finite mode, Or return to mainScene
        else
        {

			if (getIsDeathSprint() == 1)
			{
				return;
			}

			if (BattleData::getInstance()->getBattleMode() == 0)
			{

			}
			else
			{
				int time = (int)m_playTimeCounter / 60.f;
				BattleData::getInstance()->setPlayTime(time);

				BattleData::getInstance()->setGameWin(2);

				auto gameScene = static_cast<BattleScene*>(this->getScene());

				auto resultLayer = LevelGameOver::create();

				this->getScene()->addChild(resultLayer);
				BattleData::getInstance()->setIsRealDie(false);
				gameScene->pause();
			}   


        }
    }
}

void HeroLayer::heroJumpUp()
{
    m_heroVel.y = getBounceSpeed();
}

Sprite* HeroLayer::getSpriteHelper(Node *node)
{
    auto sprite = static_cast<Sprite*>(node->getChildByTag(HERO_1_TAG));
    
    return sprite;
}

void HeroLayer::updateFSM(float dt)
{
    if (m_onceFSM)
    {
        m_onceFSM->update();
    }
    
    if (m_propFSM)
    {
        m_propFSM->update();
    }
    
    if (m_jumpFSM)
    {
        m_jumpFSM->update();
    }

	if (m_lifeFSM)
    {
        m_lifeFSM->update();
    }
}

// bata version API
Hero* HeroLayer::getCurrHeroHelper()
{
    return static_cast<Hero*>(m_heroes.back());
}

/***************************************** State Define ***************************************************/
void LifeNormalState::enter(HeroLayer *heroLayer)
{
	//std::string bgState = "NullProp";

	//if (!heroLayer->getIsBubbleRunning())
	//{
	//	heroLayer->getCurrHeroHelper()->setBgState(bgState, 0, 0, 0);
	//}
    
    
}

void LifeNormalState::execute(HeroLayer *heroLayer)
{
	//jump death state
	if (heroLayer->getHeroPos().y < 0 && heroLayer->getIsDeathSprint() == 1)
	{
		if (heroLayer->getJumpDeathSpriteReady())
		{
			heroLayer->setIsDeathSprint(2);

			heroLayer->getPropFSM()->changeState(PropDeathSprintState::getInstance());
		}

	}

	if (heroLayer->getJumpDeath())
	{
		heroLayer->setJumpDeath(false);

		heroLayer->getJumpFSM()->changeState(JumpDeath1State::getInstance());
	}

	if (heroLayer->getEnterInvincible())
	{
		heroLayer->setEnterInvincible(false);

		heroLayer->getLifeFSM()->changeState(LifeInvincibleState::getInstance());
	}
}

void LifeNormalState::exit(HeroLayer *heroLayer)
{
    
}

void LifeInvincibleState::enter(HeroLayer *heroLayer)
{
	std::string eventName = EVENT_INVINCIBLE_START;

	EventCustom event(eventName);

	Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);

	heroLayer->setHpMinusReady(false);
}

void LifeInvincibleState::execute(HeroLayer *heroLayer)
{
	if (heroLayer->getIsDeathSprint() == 2 || heroLayer->getIsDeathSprint() == 3)
	{
		//heroLayer->setJumpDeath(true);
	}

	else
	{
		heroLayer->setJumpDeath(false);
	}

	if (heroLayer->getExitInvincible())
	{
		heroLayer->setExitInvincible(false);

		heroLayer->getLifeFSM()->changeState(LifeNormalState::getInstance());
	}
}

void LifeInvincibleState::exit(HeroLayer *heroLayer)
{
	std::string eventName = EVENT_INVINCIBLE_END;

	EventCustom event(eventName);

	Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);

	heroLayer->setHpMinusReady(true);
}

// Prop Normal state
void PropNormalState::enter(HeroLayer *heroLayer)
{
    heroLayer->setBounceSpeed(BOUNCE_NORMAL_SPEED);

	std::string bgState = "NullProp";
    
    heroLayer->getCurrHeroHelper()->setBgState(bgState, 0, 0, 0);
    
    heroLayer->getCurrHeroHelper()->setScale(1.2);
}

void PropNormalState::execute(HeroLayer *heroLayer)
{
    //enter helicopter state
    if (heroLayer->getHelicopterCollideOk())
    {
        heroLayer->setHelicopterCollideOk(false);
        
        heroLayer->getPropFSM()->changeState(PropHelicopterState::getInstance());
    }
    
    //enter medicine state
    if (heroLayer->getMedicineCollideOk())
    {
        heroLayer->setMedicineCollideOk(false);
        
        heroLayer->getPropFSM()->changeState(PropMedicineState::getInstance());
    }

	//enter ship state
	if (heroLayer->getShipCollideOk() && (!heroLayer->getJumpAgain2Running()))
	{
		heroLayer->setShipCollideOk(false);

		heroLayer->getPropFSM()->changeState(PropShipState::getInstance());
	}

	//enter shield state
	if (heroLayer->getShieldCollideOk())
	{
		heroLayer->setShieldCollideOk(false);

		heroLayer->getPropFSM()->changeState(PropShieldState::getInstance());
	}

	//enter shield state
	if (heroLayer->getIsStartSprint())
	{
		heroLayer->getPropFSM()->changeState(PropStartSprintState::getInstance());
	}

	if (heroLayer->getIsSuperBubble())
	{
		//heroLayer->setIsSuperBubbleRunning(true);
		//heroLayer->getPropFSM()->changeState(PropShieldState::getInstance());
	}
    
    if (heroLayer->getThreeSecondInvincibleOk())
    {
        heroLayer->setThreeSecondInvincibleOk(false);
        heroLayer->getPropFSM()->changeState(PropThreeSecondInvincible::getInstance());
    }
}

void PropNormalState::exit(HeroLayer *heroLayer)
{
	
}

// Prop Magnet state
void PropMagnetState::enter(HeroLayer *heroLayer)
{
    //heroLayer->setTrampolineCollideOk(false);
	if (m_counter > 0)
	{
		if (heroLayer->getChildByTag(ANIMATION_MAGNET_TAG))
		{
			if (heroLayer->getChildByTag(ANIMATION_MAGNET_TAG)->getParent())
			{
				heroLayer->getChildByTag(ANIMATION_MAGNET_TAG)->removeFromParent();
			}
		}
	}
	int time = 0;
	if (BattleData::getInstance()->getIsBreak())
	{
		int roleLevel = BattleData::getInstance()->getRoleLevel() + 0.5f;
		time = RoleLevelInfo::getInstance()->getMagnetTime(StringUtils::format("%d",roleLevel));
	}
	else
	{
		int roleLevel = BattleData::getInstance()->getRoleLevel();
		time = RoleLevelInfo::getInstance()->getMagnetTime(StringUtils::format("%d",roleLevel));
	}

    m_counter = 60*time;
    
    // notify other layer to scrool down in calling way
    auto battleScene = static_cast<BattleScene*>(Director::getInstance()->getRunningScene());
    
    HeroNtf heroNtf = {0};
    
    heroNtf.magnetState = 1;
    heroNtf.heroUIDataNtf.tag = 1;

    battleScene->ntf(heroNtf);

	auto winS = Director::getInstance()->getWinSize();
	auto myNode = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Magnet_Animate_1.png"));
	auto animation = Animation::create();
	for(int i=1;i<=12;++i)
	{
		std::string spriteFrameName = StringUtils::format("Magnet_Animate_%d.png",i);
		SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
		animation->addSpriteFrame(frame);
	}
	animation->setDelayPerUnit(0.05f);
	animation->setRestoreOriginalFrame(false);
	auto action = Animate::create(animation);
	auto callFunc = CallFunc::create([=](){
		myNode->removeFromParent();

		});
	auto actR = RepeatForever::create(action);
	//auto seq = Sequence::createWithTwoActions(actR);
    myNode->runAction(actR);
	auto roleP = heroLayer->getCurrHeroHelper()->getPosition();
	myNode->setPosition(Point(roleP.x, roleP.y));
	myNode->setTag(ANIMATION_MAGNET_TAG);
	heroLayer->addChild(myNode);
    /*
    // notify other layer magnetState
    std::string eventName = EVENT_HERO_NTF;
    
    EventCustom event(eventName);
    
    HeroNtf heroNtf = {0};
    
    heroNtf.magnetState = 1;
    
    event.setUserData(&heroNtf);
    
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    */
}

void PropMagnetState::execute(HeroLayer *heroLayer)
{
    //PropMagnet Counter
    m_counter--;

	auto battleScene = static_cast<BattleScene*>(Director::getInstance()->getRunningScene());
    
    HeroNtf heroNtf = {0};
    
    heroNtf.magnetState = 1;
	heroNtf.heroUIDataNtf.tag = 1;
    heroNtf.heroUIDataNtf.prop6 = m_counter;

    battleScene->ntf(heroNtf);

    if (m_counter <= 0)
    {
		heroLayer->getPropFSM()->setGlobalState(nullptr);
		PropMagnetState::getInstance()->exit(heroLayer);
    }

	if (m_counter <= 2*60)
	{
		heroNtf.magnetState = 2;
	}
    
    // notify other layer to scrool down in calling way
    
    auto hero = heroLayer->getCurrHeroHelper();
    
    auto heroWorldPos = hero->convertToWorldSpaceAR(Point::ZERO);
    
    heroNtf.x = heroWorldPos.x;
    
    heroNtf.y = heroWorldPos.y;
    
    battleScene->ntf(heroNtf);

	auto animate = heroLayer->getChildByTag(ANIMATION_MAGNET_TAG);
	auto roleP = heroLayer->getCurrHeroHelper()->getPosition();

	if (animate)
	{
		animate->setPosition(Point(roleP.x,roleP.y));
	}
	
	//
    /*
    // notify other layer that hero worldPos
    std::string eventName = EVENT_HERO_NTF;
    
    EventCustom event(eventName);
    
    HeroNtf heroNtf = {0};
    
    auto hero = heroLayer->getCurrHeroHelper();
    
    auto heroWorldPos = hero->convertToWorldSpaceAR(Point::ZERO);
    
    heroNtf.x = heroWorldPos.x;
    
    heroNtf.y = heroWorldPos.y;
    
    event.setUserData(&heroNtf);
    
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
     */
}

void PropMagnetState::exit(HeroLayer *heroLayer)
{
    // notify other layer to scrool down in calling way
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();

    auto battleScene = static_cast<BattleScene*>(Director::getInstance()->getRunningScene());
    
    HeroNtf heroNtf = {0};
    
	heroNtf.heroUIDataNtf.prop6 = -1;
	heroNtf.heroUIDataNtf.tag = 1;

    battleScene->ntf(heroNtf);
    
	heroLayer->getChildByTag(ANIMATION_MAGNET_TAG)->removeFromParent();

	m_counter = 0;
    /*
    // notify other layer magnetState
    std::string eventName = EVENT_HERO_NTF;
    
    EventCustom event(eventName);
    
    HeroNtf heroNtf = {0};
    
    heroNtf.magnetState = 2;
    
    event.setUserData(&heroNtf);
    
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    */
}

// Prop Helicopter state
void PropHelicopterState::enter(HeroLayer *heroLayer)
{
	heroLayer->setJumpPauseOk(true);
    
    heroLayer->setIsHelicopterRunning(true);
    
	heroLayer->setEnterInvincible(true);

	int time;
	if (BattleData::getInstance()->getIsBreak())
	{
		int roleLevel = BattleData::getInstance()->getRoleLevel() + 0.5f;
		time = RoleLevelInfo::getInstance()->getHelicopterTime(StringUtils::format("%d",roleLevel));
	}
	else
	{
		int roleLevel = BattleData::getInstance()->getRoleLevel();
		time = RoleLevelInfo::getInstance()->getHelicopterTime(StringUtils::format("%d",roleLevel));
	}

    m_counter = 60*time;

	auto heroAcc = Point(0, 0);

	heroLayer->setHeroAcc(heroAcc);

	auto heroVel = Point(0, 400);

	heroLayer->setHeroVel(heroVel);

	heroLayer->setIsHelicopterEndRunning(false);
}

void PropHelicopterState::execute(HeroLayer *heroLayer)
{
    //PropMagnet Counter
    m_counter--;
    
	
	HeroNtf heroNtf = {0};
	heroNtf.heroUIDataNtf.tag = 1;
	heroNtf.heroUIDataNtf.prop5 = m_counter;
    
    auto heroVel = heroLayer->getHeroVel();
    heroVel.y = 400;
    heroLayer->setHeroVel(heroVel);

	auto battleScene = static_cast<BattleScene*>(Director::getInstance()->getRunningScene());
	battleScene->ntf(heroNtf);

	if (heroLayer->getHelicopterCollideOk())
	{
		heroLayer->setHelicopterCollideOk(false);

		int time;
		if (BattleData::getInstance()->getIsBreak())
		{
			int roleLevel = BattleData::getInstance()->getRoleLevel() + 0.5f;
			time = RoleLevelInfo::getInstance()->getHelicopterTime(StringUtils::format("%d",roleLevel));
		}
		else
		{
			int roleLevel = BattleData::getInstance()->getRoleLevel();
			time = RoleLevelInfo::getInstance()->getHelicopterTime(StringUtils::format("%d",roleLevel));
		}

		m_counter = 60*time;

	}

	if (m_counter <= 65 && !heroLayer->getIsHelicopterEndRunning())
	{
		//auto heroS = heroLayer->getCurrHeroHelper();
		//auto act = Blink::create(6,10);
		//auto repeat = RepeatForever::create(act);

		//heroS->runAction(repeat);

		heroLayer->setIsHelicopterEndRunning(true);

	}

    if (m_counter <= 0)
    {
        heroLayer->getPropFSM()->changeState(PropNormalState::getInstance());
    }

	if (heroLayer->getIsReward())
	{
		m_counter = 0;
	}

	//enter shield state
	if (heroLayer->getShieldCollideOk())
	{
		m_counter = 0;
	}

	//enter shield state
	if (heroLayer->getShipCollideOk())
	{
		m_counter = 0;
	}

	if (heroLayer->getMedicineCollideOk())
	{
		m_counter = 0;
	}
}

void PropHelicopterState::exit(HeroLayer *heroLayer)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
	auto heroS = heroLayer->getCurrHeroHelper();
	heroS->stopAllActions();

    heroLayer->setIsHelicopterRunning(false);
    
	heroLayer->setJumpResumeOk(true);

	heroLayer->setHelicopterCollideOk(false);

	heroLayer->setExitInvincible(true);

	heroLayer->setIsHelicopter(false);

	HeroNtf heroNtf = {0};
	heroNtf.heroUIDataNtf.tag = 1;
	heroNtf.heroUIDataNtf.prop5 = -1;

	auto battleScene = static_cast<BattleScene*>(Director::getInstance()->getRunningScene());
	battleScene->ntf(heroNtf);
    
    heroLayer->setIsHelicopter(false);
}

// Prop Helicopter state
void PropMedicineState::enter(HeroLayer *heroLayer)
{
	heroLayer->setEnterInvincible(true);

	heroLayer->setIsBig(true);
    
    heroLayer->setBounceSpeed(800.0f*2);
    
	int time;
	if (BattleData::getInstance()->getIsBreak())
	{
		int roleLevel = BattleData::getInstance()->getRoleLevel() + 0.5f;
		time = RoleLevelInfo::getInstance()->getShipTime(StringUtils::format("%d",roleLevel));
	}
	else
	{
		int roleLevel = BattleData::getInstance()->getRoleLevel();
		time = RoleLevelInfo::getInstance()->getShipTime(StringUtils::format("%d",roleLevel));
	}

    m_counter = 60*time;

	//heroLayer->getCurrHeroHelper()->runAction(ScaleTo::create(1,1.5));
    heroLayer->getCurrHeroHelper()->setScale(2.0f);
}

void PropMedicineState::execute(HeroLayer *heroLayer)
{
    //PropMagnet Counter
	heroLayer->setBounceSpeed(800.0f*2);
	m_counter--;

	HeroNtf heroNtf = {0};
	heroNtf.heroUIDataNtf.tag = 1;
	heroNtf.heroUIDataNtf.prop8 = m_counter;

	auto battleScene = static_cast<BattleScene*>(Director::getInstance()->getRunningScene());
	battleScene->ntf(heroNtf);

	if(heroLayer->getMedicineCollideOk())
	{
		heroLayer->setMedicineCollideOk(false);

		m_counter = 60*50;
	}

    if (m_counter <= 0)
    {
        heroLayer->getCurrHeroHelper()->setScale(1.2);
        heroLayer->getPropFSM()->changeState(PropNormalState::getInstance());
    }

	if (heroLayer->getIsReward())
	{
		m_counter = 0;
	}

	if (heroLayer->getShieldCollideOk())
	{
		m_counter = 0;
	}

	if (heroLayer->getHelicopterCollideOk())
	{
		m_counter = 0;
	}

	if (heroLayer->getShipCollideOk())
	{
		m_counter = 0;
	}
}

void PropMedicineState::exit(HeroLayer *heroLayer)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();

	//heroLayer->getCurrHeroHelper()->runAction(ScaleTo::create(1,1.2));
    heroLayer->getCurrHeroHelper()->setScale(1.2);

	heroLayer->setExitInvincible(true);

	heroLayer->setMedicineCollideOk(false);

	heroLayer->setIsBig(false);

	HeroNtf heroNtf = {0};
	heroNtf.heroUIDataNtf.tag = 1;
	heroNtf.heroUIDataNtf.prop8 = -1;

	auto battleScene = static_cast<BattleScene*>(Director::getInstance()->getRunningScene());
	battleScene->ntf(heroNtf);
}

// Prop Helicopter state
void PropShieldState::enter(HeroLayer *heroLayer)
{
	log("Enter Shield Prop!");

	//heroLayer->setMonsterTouchedOk(false);

	heroLayer->setIsBubbleRunning(true);

	int time;
	if (BattleData::getInstance()->getIsBreak())
	{
		int roleLevel = BattleData::getInstance()->getRoleLevel() + 0.5f;
		time = RoleLevelInfo::getInstance()->getShieldTime(StringUtils::format("%d",roleLevel));
	}
	else
	{
		int roleLevel = BattleData::getInstance()->getRoleLevel();
		time = RoleLevelInfo::getInstance()->getShieldTime(StringUtils::format("%d",roleLevel));
	}

    m_counter = 60*time;

	std::string bgState;

	if (heroLayer->getIsSuperBubble())
	{
		bgState = "ShieldShop";
	}
	else
	{
		 bgState = "ShieldNormal";
	}
    
    heroLayer->getCurrHeroHelper()->setBgState(bgState, 0, 0, 0);

	heroLayer->setEnterInvincible(true);
}

void PropShieldState::execute(HeroLayer *heroLayer)
{
    //PropMagnet Counter
	log("Execute Shield Prop!");
	if (!heroLayer->getIsSuperBubble())
	{
		m_counter--;
	}
    
	if (!heroLayer->getIsSuperBubble())
	{
		HeroNtf heroNtf = {0};
		heroNtf.heroUIDataNtf.tag = 1;
		heroNtf.heroUIDataNtf.prop2 = m_counter;

		auto battleScene = static_cast<BattleScene*>(Director::getInstance()->getRunningScene());
		battleScene->ntf(heroNtf);
	}
    
	if(!heroLayer->getIsShield())
	{
		m_counter = 0;
	}

    if (m_counter <= 0 && !heroLayer->getIsSuperBubble())
    {
        heroLayer->getPropFSM()->changeState(PropNormalState::getInstance());
    }

	if (!heroLayer->getIsSuperBubbleRunning() && heroLayer->getIsSuperBubble())
	{
		heroLayer->getPropFSM()->changeState(PropNormalState::getInstance());
        
        heroLayer->setIsShield(false);
        
        heroLayer->setIsBubbleRunning(false);
	}
    
	//enter ship state
	if (heroLayer->getShipCollideOk())
	{
		m_counter = 0;
		heroLayer->setIsSuperBubbleRunning(false);
	}

	if (heroLayer->getMedicineCollideOk())
	{
		m_counter = 0;
		heroLayer->setIsSuperBubbleRunning(false);
	}

	if (heroLayer->getHelicopterCollideOk())
	{
		m_counter = 0;
		heroLayer->setIsSuperBubbleRunning(false);
	}

	if (heroLayer->getIsReward())
	{
		m_counter = 0;
		heroLayer->setIsSuperBubbleRunning(false);
	}

}

void PropShieldState::exit(HeroLayer *heroLayer)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
	log("Exit Shield Prop!");

    heroLayer->setExitInvincible(true);

	//heroLayer->setMonsterTouchedOk(true);

	//heroLayer->setIsShield(false);

	//heroLayer->setIsBubbleRunning(false);

	if (!heroLayer->getIsSuperBubble())
	{
		HeroNtf heroNtf = {0};
		heroNtf.heroUIDataNtf.tag = 1;
		heroNtf.heroUIDataNtf.prop2 = -1;

		auto battleScene = static_cast<BattleScene*>(Director::getInstance()->getRunningScene());
		battleScene->ntf(heroNtf);
	}


}

//Three Second Invincible

void PropThreeSecondInvincible::enter(HeroLayer *heroLayer)
{
    log("Enter PropThreeSecondInvincible");

	heroLayer->setIsThreeSecondRunning(true);
    
    heroLayer->setEnterInvincible(true);
    
    int time = 3;
    m_counter = time * 60;
    
    
}

void PropThreeSecondInvincible::execute(HeroLayer *heroLayer)
{
    m_counter--;
    
    if (m_counter <= 0) {
        heroLayer->getPropFSM()->changeState(PropNormalState::getInstance());
    }
    
    if (heroLayer->getIsReward())
	{
		m_counter = 0;
	}
    
	if (heroLayer->getShieldCollideOk())
	{
		m_counter = 0;
	}
    
	if (heroLayer->getHelicopterCollideOk())
	{
		m_counter = 0;
	}
    
	if (heroLayer->getShipCollideOk())
	{
		m_counter = 0;
	}
}

void PropThreeSecondInvincible::exit(HeroLayer *heroLayer)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();

    log("Exit PropThreeSecondInvincible");

	heroLayer->setIsThreeSecondRunning(false);
    
    auto heroS = heroLayer->getCurrHeroHelper();
    
    heroS->stopAllActions();
    
    heroLayer->setExitInvincible(true);
    
    if (BattleData::getInstance()->getProp2() > 0)
    {
        //heroLayer->setIsSuperBubble(true);
        //heroLayer->setIsSuperBubbleRunning(true);
        //heroLayer->setShieldCollideOk(true);
        //heroLayer->setIsShield(true);
        
		
    }
}

// Prop Ship state
void PropShipState::enter(HeroLayer *heroLayer)
{
	heroLayer->setTouchClickOk(false);
    
    heroLayer->setIsShipRunning(true);

	heroLayer->setShipCollideOk(false);

	heroLayer->setJumpPauseOk(true);

	heroLayer->setEnterInvincible(true);

	m_counter = 60*5;

	auto heroAcc = Point(0, 0);
    
    heroLayer->setHeroAcc(heroAcc);
    
    auto heroVel = Point(0, 900);
    
    heroLayer->setHeroVel(heroVel);

	std::string bgState = "ShipProp";
    
    heroLayer->getCurrHeroHelper()->setBgState(bgState, 0, 0, 0);

	heroLayer->setIsHelicopterEndRunning(false);
}

void PropShipState::execute(HeroLayer *heroLayer)
{
//	heroLayer->setBounceSpeed(2800.0f);
    //PropMagnet Counter
    m_counter--;

	HeroNtf heroNtf = {0};
	heroNtf.heroUIDataNtf.tag = 1;
	heroNtf.heroUIDataNtf.prop3 = m_counter;
    
    auto heroVel = heroLayer->getHeroVel();
    heroVel.y = 900;
    heroLayer->setHeroVel(heroVel);

	auto battleScene = static_cast<BattleScene*>(Director::getInstance()->getRunningScene());
	battleScene->ntf(heroNtf);

	//enter ship state
	if (heroLayer->getShipCollideOk())
	{
		heroLayer->setShipCollideOk(false);

		m_counter = 60*5;
	}

	if (m_counter <= 65 && !heroLayer->getIsShipEndRunning())
	{
		//auto heroS = heroLayer->getCurrHeroHelper();
		//auto act = Blink::create(6,10);
		//auto repeat = RepeatForever::create(act);

		//heroS->runAction(repeat);

		heroLayer->setIsShipEndRunning(true);
	}
    
    if (m_counter <= 0)
    {
        heroLayer->getPropFSM()->changeState(PropNormalState::getInstance());
    }

	//enter shield state
	if (heroLayer->getShieldCollideOk())
	{
		m_counter = 0;
	}

	//enter helicopter state
	if (heroLayer->getHelicopterCollideOk())
	{
		m_counter = 0;
	}

	if(heroLayer->getMedicineCollideOk())
	{
		m_counter = 0;
	}

	if (heroLayer->getIsReward())
	{
		m_counter = 0;
	}
}

void PropShipState::exit(HeroLayer *heroLayer)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();

	auto heroS = heroLayer->getCurrHeroHelper();
	heroS->stopAllActions();

	heroLayer->setJumpResumeOk(true);

	heroLayer->setExitInvincible(true);

	HeroNtf heroNtf = {0};
	heroNtf.heroUIDataNtf.tag = 1;
	heroNtf.heroUIDataNtf.prop3 = -1;

	auto battleScene = static_cast<BattleScene*>(Director::getInstance()->getRunningScene());
	battleScene->ntf(heroNtf);

	heroLayer->setTouchClickOk(false);
    
    heroLayer->setIsShipRunning(false);
    
    
}

void PropStartSprintState::enter(HeroLayer *heroLayer)
{
    heroLayer->setIsStartSpriteRunning(true);
    
    auto heroS = heroLayer->getCurrHeroHelper();
    
    log("HeroS %d%d",heroS->getZOrder());
    
	heroLayer->setJumpPauseOk(true);

	heroLayer->setEnterInvincible(true);

	m_counter = 60* 6 ;

	if (heroLayer->getIsFriendRunning() || heroLayer->getIsResourrection() || heroLayer->getIsFriendMod())
	{
		m_counter = 60 * 2;
	}

	if (BattleData::getInstance()->getPropRandom() == 13)
	{
			m_counter = 60 * 5;
	}
	else if (BattleData::getInstance()->getPropRandom() == 14)
	{
			m_counter = 60 * 8;
	}
	else if (BattleData::getInstance()->getPropRandom() == 15)
	{
			m_counter = 60 * 15;
	}

	auto heroAcc = Point(0, 0);

	heroLayer->setHeroAcc(heroAcc);

	auto heroVel = Point(0, 1000);

	heroLayer->setHeroVel(heroVel);

	std::string bgState = "JumpFly";

	heroLayer->getCurrHeroHelper()->setJumpState(bgState, 0, 0, 0);
}

void PropStartSprintState::execute(HeroLayer *heroLayer)
{
	m_counter--;

	HeroNtf heroNtf = {0};
	heroNtf.heroUIDataNtf.tag = 1;
	heroNtf.heroUIDataNtf.prop1 = m_counter;
    
    auto heroVel = heroLayer->getHeroVel();
    heroVel.y = 1000;
    heroLayer->setHeroVel(heroVel);

	if (!heroLayer->getIsFriendRunning())
	{
		if (!heroLayer->getIsResourrection())
		{
			auto battleScene = static_cast<BattleScene*>(Director::getInstance()->getRunningScene());
			battleScene->ntf(heroNtf);
		}

	}


	if (m_counter <= 0)
	{
		heroLayer->getPropFSM()->changeState(PropNormalState::getInstance());
	}
}

void PropStartSprintState::exit(HeroLayer *heroLayer)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();

	heroLayer->setIsStartSprint(false);

	heroLayer->setJumpResumeOk(true);

	heroLayer->setExitInvincible(true);

	HeroNtf heroNtf = {0};
	heroNtf.heroUIDataNtf.tag = 1;
	heroNtf.heroUIDataNtf.prop1 = -1;

	auto battleScene = static_cast<BattleScene*>(Director::getInstance()->getRunningScene());
	battleScene->ntf(heroNtf);
    
    heroLayer->setIsStartSpriteRunning(false);
    
    if (BattleData::getInstance()->getProp2() > 0)
    {
        //heroLayer->setIsSuperBubble(true);
        //heroLayer->setIsSuperBubbleRunning(true);
        //heroLayer->setShieldCollideOk(true);
        //heroLayer->setIsShield(true);
		
    }
}

void PropDeathSprintState::enter(HeroLayer *heroLayer)
{
	heroLayer->setJumpPauseOk(true);

	heroLayer->setEnterInvincible(true);

	m_counter = 60*4;

	if (BattleData::getInstance()->getPropRandom() == 16)
	{
		m_counter = 60 * 5;
	}
	else if (BattleData::getInstance()->getPropRandom() == 17)
	{
		m_counter = 60 * 8;
	}
	else if (BattleData::getInstance()->getPropRandom() == 18)
	{
		m_counter = 60 * 12;
	}

	auto heroAcc = Point(0, 0);

	heroLayer->setHeroAcc(heroAcc);

	auto heroVel = Point(0, 1000);

	heroLayer->setHeroVel(heroVel);

	std::string jumpState = "JumpFly";

	heroLayer->getCurrHeroHelper()->setJumpState(jumpState, 0, 0, 0);
}

void PropDeathSprintState::execute(HeroLayer *heroLayer)
{
	m_counter--;

	HeroNtf heroNtf = {0};
	heroNtf.heroUIDataNtf.tag = 1;
	heroNtf.heroUIDataNtf.prop4 = m_counter;
    
    auto heroVel = heroLayer->getHeroVel();
    heroVel.y = 1000;
    heroLayer->setHeroVel(heroVel);

	auto battleScene = static_cast<BattleScene*>(Director::getInstance()->getRunningScene());
	battleScene->ntf(heroNtf);

	if (m_counter <= 0)
	{
		heroLayer->getPropFSM()->changeState(PropNormalState::getInstance());
	}
}

void PropDeathSprintState::exit(HeroLayer *heroLayer)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();

	heroLayer->setIsDeathSprint(3);

	heroLayer->setIsDeath(true);

	heroLayer->setJumpResumeOk(true);

	heroLayer->setExitInvincible(true);

	HeroNtf heroNtf = {0};
	heroNtf.heroUIDataNtf.tag = 1;
	heroNtf.heroUIDataNtf.prop4 = -1;

	auto battleScene = static_cast<BattleScene*>(Director::getInstance()->getRunningScene());
	battleScene->ntf(heroNtf);
}

// Jump Pause State
void JumpPauseState::enter(HeroLayer *heroLayer)
{
    // run Pause Animition
	std::string jumpState;
	if(heroLayer->getIsHelicopter())
	{
		jumpState = "JumpHelicopter";
	}

	else if(heroLayer->getIsStartSprint() || heroLayer->getIsDeathSprint() == 2)
	{
		jumpState = "JumpFly";
	}
	
	else
	{
		jumpState = "JumpDown";
	}
    
    heroLayer->getCurrHeroHelper()->setJumpState(jumpState, 0, 0, 0);
}

void JumpPauseState::execute(HeroLayer *heroLayer)
{
    if (heroLayer->getJumpResumeOk())
    {
        heroLayer->setJumpResumeOk(false);
        
        heroLayer->getJumpFSM()->changeState(JumpReadyState::getInstance());
    }
}

void JumpPauseState::exit(HeroLayer *heroLayer)
{
    
}

// Jump Ready State
void JumpReadyState::enter(HeroLayer *heroLayer)
{
    std::string jumpState = "JumpReady";
    
    heroLayer->getCurrHeroHelper()->setJumpState(jumpState, 0, 0, 0);
    
    //heroLayer->heroJumpUp();
    
    Point heroAcc = heroLayer->getHeroAcc();
    
    heroAcc.y = 0.0f;
    
    heroLayer->setHeroAcc(heroAcc);
    
    Point heroVel = heroLayer->getHeroVel();
    
    heroVel.y = -1;
    
    heroLayer->setHeroVel(heroVel);
}

void JumpReadyState::execute(HeroLayer *heroLayer)
{
    // change state to jump up state when jumpReady animation play over
    if (heroLayer->getJumpReadyOverOk())
    {
        heroLayer->setJumpReadyOverOk(false);
        
		if(!heroLayer->getIsStartSprint())
		{
			heroLayer->getJumpFSM()->changeState(JumpUpState::getInstance());
		}
    }

	if (heroLayer->getJumpPauseOk())
    {
        heroLayer->setJumpPauseOk(false);
        
        heroLayer->getJumpFSM()->changeState(JumpPauseState::getInstance());
    }
}

void JumpReadyState::exit(HeroLayer *heroLayer)
{
    heroLayer->setJumpAgainToken(true);
    
    heroLayer->setTouchClickOk(false);

	heroLayer->setHpMinusReady(true);

	heroLayer->setTorpedoAttacked(false);

	heroLayer->setLadderBombAttacked(false);

	heroLayer->setIsResourrectionRunning(false);

	heroLayer->setIsBuyResourrection(false);

}

// Jump Up State
void JumpUpState::enter(HeroLayer *heroLayer)
{
	Point heroAcc = heroLayer->getHeroAcc();

	heroAcc.y = -1600.0f;

	heroLayer->setHeroAcc(heroAcc);

    std::string jumpState;

	if (heroLayer->getIsThreeSecondRunning())
	{
		jumpState = "JumpUp";
	}
	else
	{
		jumpState = "JumpUp";
	}
    
    heroLayer->getCurrHeroHelper()->setJumpState(jumpState, 0, 0, 0);
    
    heroLayer->heroJumpUp();
}

void JumpUpState::execute(HeroLayer *heroLayer)
{
    // change to jumpSuspendState, total suspend Time is 1/3 s, up time 1/6, down time 1/6, v = a * t
    //auto suspendSpeed = -heroLayer->getHeroAcc().y/4;
    auto suspendSpeed = heroLayer->getBounceSpeed()/4;
    
    if (heroLayer->getHeroVel().y < suspendSpeed)
    {
        heroLayer->getJumpFSM()->changeState(JumpSuspendState::getInstance());
    }

	if (heroLayer->getJumpPauseOk())
    {
        heroLayer->setJumpPauseOk(false);
        
        heroLayer->getJumpFSM()->changeState(JumpPauseState::getInstance());
    }

	 // change to jumpAgainState
	if (heroLayer->getCurrHeroHelper()->getHeroId() == HERO_1_TAG &&
        heroLayer->getTouchClickOk() &&
        heroLayer->getJumpAgainToken())
		//&&
        //!heroLayer->getIsBig())
    {
        heroLayer->setTouchClickOk(false);
        
        heroLayer->setJumpAgainToken(false);
        
        heroLayer->getJumpFSM()->changeState(JumpAgain1State::getInstance());
    }
    
    //Only hero_2 can enter to JumpKillMonsterState
    if (heroLayer->getCurrHeroHelper()->getHeroId() == HERO_2_TAG &&
        heroLayer->getMonsterTouchedOk() == true &&
        !heroLayer->getIsShipRunning() &&
        !heroLayer->getIsHelicopterRunning())
    {
        heroLayer->setMonsterTouchedOk(false);
        
        heroLayer->getJumpFSM()->changeState(JumpKillMonsterState::getInstance());
    }
}

void JumpUpState::exit(HeroLayer *heroLayer)
{
    
}

// Jump Suspend
void JumpSuspendState::enter(HeroLayer *heroLayer)
{
    std::string jumpState = "JumpSuspend";
    
    auto suspendSpeed = heroLayer->getBounceSpeed()/4;
    
    // ((suspendSpeed/4)/Acc.y)*2
    auto suspendTime = -suspendSpeed/(2*heroLayer->getHeroAcc().y);
    
    heroLayer->getCurrHeroHelper()->setJumpState(jumpState, suspendTime, 0, 0);
}

void JumpSuspendState::execute(HeroLayer *heroLayer)
{
    //when suspend time is up, change to downState
    //auto suspendSpeed = -heroLayer->getHeroAcc().y/4;
    auto suspendSpeed = heroLayer->getBounceSpeed()/4;
    
    if (heroLayer->getHeroVel().y < -suspendSpeed)
    {
        heroLayer->getJumpFSM()->changeState(JumpDownState::getInstance());
    }
    
    // change to jumpReadyState when collide happen
    if (heroLayer->getBounceOk())
    {
        heroLayer->setBounceOk(false);
    }
    
    // change to jumpAgainState
    if (heroLayer->getCurrHeroHelper()->getHeroId() == HERO_1_TAG &&
        heroLayer->getTouchClickOk() &&
        heroLayer->getJumpAgainToken() &&
        !heroLayer->getIsBig())
    {
        heroLayer->setTouchClickOk(false);
        
        heroLayer->setJumpAgainToken(false);
        
        heroLayer->getJumpFSM()->changeState(JumpAgain1State::getInstance());
    }

	if (heroLayer->getJumpPauseOk())
    {
        heroLayer->setJumpPauseOk(false);
        
        heroLayer->getJumpFSM()->changeState(JumpPauseState::getInstance());
    }
    
    //Only hero_2 can enter to JumpKillMonsterState
    if (heroLayer->getCurrHeroHelper()->getHeroId() == HERO_2_TAG &&
        heroLayer->getMonsterTouchedOk() == true &&
        !heroLayer->getIsShipRunning() &&
        !heroLayer->getIsHelicopterRunning())
    {

        heroLayer->setMonsterTouchedOk(false);
        
        heroLayer->getJumpFSM()->changeState(JumpKillMonsterState::getInstance());
    }
}

void JumpSuspendState::exit(HeroLayer *heroLayer)
{
    
}

// Jump Down State
void JumpDownState::enter(HeroLayer *heroLayer)
{
    std::string jumpState = "JumpDown";
    
    heroLayer->getCurrHeroHelper()->setJumpState(jumpState, 0, 0, 0);
    
    heroLayer->setBounceOk(false);
}

void JumpDownState::execute(HeroLayer *heroLayer)
{
    // change to jumpReadyState
    if (heroLayer->getBounceOk())
    {
        heroLayer->setBounceOk(false);
        
        heroLayer->getJumpFSM()->changeState(JumpReadyState::getInstance());
    }

	// change to reward levels state
    if (heroLayer->getIsGoingReward())
    {
		heroLayer->setStar(0);
	
        heroLayer->getJumpFSM()->changeState(JumpFlyState::getInstance());
    }
    
    // Only HERO_1 change to jumpAgainState
    if (heroLayer->getCurrHeroHelper()->getHeroId() == HERO_1_TAG &&
        heroLayer->getTouchClickOk() &&
        heroLayer->getJumpAgainToken() &&
        !heroLayer->getIsBig())
    {
        heroLayer->setTouchClickOk(false);
        
        heroLayer->setJumpAgainToken(false);
        
        heroLayer->getJumpFSM()->changeState(JumpAgain1State::getInstance());
    }

	if (heroLayer->getJumpPauseOk())
    {
        heroLayer->setJumpPauseOk(false);
        
        heroLayer->getJumpFSM()->changeState(JumpPauseState::getInstance());
    }
    
    //Only hero_2 can enter to JumpKillMonsterState
    if (heroLayer->getCurrHeroHelper()->getHeroId() == HERO_2_TAG &&
        heroLayer->getMonsterTouchedOk() == true &&
        !heroLayer->getIsShipRunning() &&
        !heroLayer->getIsHelicopterRunning())
    {
		
        heroLayer->setMonsterTouchedOk(false);
        
        heroLayer->getJumpFSM()->changeState(JumpKillMonsterState::getInstance());
    }
}

void JumpDownState::exit(HeroLayer *heroLayer)
{
    
}

// Jump Again State
void JumpAgain1State::enter(HeroLayer *heroLayer)
{
	heroLayer->setEnterInvincible(true);

    std::string jumpState = "JumpAgain1";
    
    heroLayer->getCurrHeroHelper()->setJumpState(jumpState, 0, 0, 0);
    
    heroLayer->setBounceSpeed(800.0f);
    
    heroLayer->heroJumpUp();
}

void JumpAgain1State::execute(HeroLayer *heroLayer)
{
    if (heroLayer->getJumpAgain1OverOk())
    {
        heroLayer->setJumpAgain1OverOk(false);
        
        heroLayer->getJumpFSM()->changeState(JumpAgain2State::getInstance());
    }

	if (heroLayer->getJumpPauseOk())
    {
        heroLayer->setJumpPauseOk(false);
        
        heroLayer->getJumpFSM()->changeState(JumpPauseState::getInstance());
    }
}

void JumpAgain1State::exit(HeroLayer *heroLayer)
{
    heroLayer->setBounceSpeed(BOUNCE_NORMAL_SPEED);
}

// Jump Again State
void JumpAgain2State::enter(HeroLayer *heroLayer)
{
	std::string eventName = EVENT_ATTACK_START;

	EventCustom event(eventName);

	Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);

	

    std::string jumpState = "JumpAgain2";
    
    heroLayer->getCurrHeroHelper()->setJumpState(jumpState, 0, 0, 0);

	heroLayer->setJumpAgain2Running(true);
}

void JumpAgain2State::execute(HeroLayer *heroLayer)
{
    if (heroLayer->getHeroVel().y < 0)
    {
        heroLayer->getJumpFSM()->changeState(JumpDownState::getInstance());
    }

	if (heroLayer->getJumpPauseOk())
    {
        heroLayer->setJumpPauseOk(false);
        
        heroLayer->getJumpFSM()->changeState(JumpPauseState::getInstance());
    }

	if (heroLayer->getShipCollideOk())
	{

	}
}

void JumpAgain2State::exit(HeroLayer *heroLayer)
{
	std::string eventName = EVENT_ATTACK_END;

	EventCustom event(eventName);

	Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);


	heroLayer->setExitInvincible(true);

	heroLayer->setJumpAgain2Running(false);
}

// Jump Fly State
void JumpFlyState::enter(HeroLayer *heroLayer)
{
	GameSound::playEffectMusic(EFF_REWARDLAYER_START);

	GameSound::playBackgroundMusic(MUSIC_LAYER_REWARD);

    auto heroAcc = Point(0, 0);
    
    heroLayer->setHeroAcc(heroAcc);
    
    auto heroVel = Point(0, 400);
    
    heroLayer->setHeroVel(heroVel);

	// notify other layer to scrool down in calling way
    auto battleScene = static_cast<BattleScene*>(Director::getInstance()->getRunningScene());
    
    HeroNtf heroNtf = {0};

    heroNtf.levelType = 1;
        
    battleScene->ntf(heroNtf);

	heroLayer->setIsReward(true);

	heroLayer->setEnterInvincible(true);

	std::string eventName = EVENT_REWARD_LEVELS_START;

	EventCustom event(eventName);

	Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    
    std::string bgState = "JumpFly";
    
	heroLayer->getCurrHeroHelper()->setJumpState(bgState, 0, 0, 0);
}

void JumpFlyState::execute(HeroLayer *heroLayer)
{
    if (heroLayer->getRewardLevelsOver())
    {
        heroLayer->setRewardLevelsOver(false);

		heroLayer->getJumpFSM()->changeState(JumpReadyState::getInstance());
    }
}

void JumpFlyState::exit(HeroLayer *heroLayer)
{
	GameSound::playEffectMusic(EFF_REWARDLAYER_END);

    auto heroAcc = Point(0, -1600.0f);
    
    heroLayer->setHeroAcc(heroAcc);

	heroLayer->setIsReward(false);

	heroLayer->setIsGoingReward(false); 

	heroLayer->setExitInvincible(true);

	if (BattleData::getInstance()->getProp2() > 0)
    {
        //heroLayer->setIsSuperBubble(true);
        //heroLayer->setIsSuperBubbleRunning(true);
        //heroLayer->setShieldCollideOk(true);
        //heroLayer->setIsShield(true);
		
    }
    
    //auto heroVel = Point(0, 0);
    
    //auto heroPos = Point(320, 480);
}

// Jump Death1 State
void JumpDeath1State::enter(HeroLayer *heroLayer)
{
	Point heroAcc = heroLayer->getHeroAcc();

	heroAcc.y = -1600.0f;

	heroLayer->setHeroAcc(heroAcc);

	heroLayer->setIsDeath(true);

    std::string jumpState = "JumpDeath1";
    
    heroLayer->getCurrHeroHelper()->setJumpState(jumpState, 0, 0, 0);

	heroLayer->setBounceSpeed(400.0f);

	heroLayer->heroJumpUp();
}

void JumpDeath1State::execute(HeroLayer *heroLayer)
{
	if(heroLayer->getJumpDeath1OverOk())
	{
		heroLayer->setJumpDeath1OverOk(false);

		heroLayer->getJumpFSM()->changeState(JumpDeath2State::getInstance());
	}
}

void JumpDeath1State::exit(HeroLayer *heroLayer)
{

}

// Jump Death2 State
void JumpDeath2State::enter(HeroLayer *heroLayer)
{
    std::string jumpState = "JumpDeath2";
    
    heroLayer->getCurrHeroHelper()->setJumpState(jumpState, 0, 0, 0);
}

void JumpDeath2State::execute(HeroLayer *heroLayer)
{
	
}

void JumpDeath2State::exit(HeroLayer *heroLayer)
{

}

// Jump Kill Monster State
void JumpKillMonsterState::enter(HeroLayer *heroLayer)
{
	heroLayer->setMonsterTouchedEnable(false);

    heroLayer->setAccEnable(false);
    
    heroLayer->setHeroAcc(Point(0, 0));
    
    auto monsterLocation = heroLayer->getMonsterTouchLocation();
    
    auto heroLocation = heroLayer->getHeroPos();
    
    auto subLocation = monsterLocation - heroLocation;
    
    auto speed = subLocation / 0.2;
    
    heroLayer->setHeroVel(speed);
    
    // notify ladderLayer that monster can be killed
    std::string eventName = EVENT_ATTACK_START;
    
	EventCustom event(eventName);
    
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    
    std::string jumpState = "JumpBlink";
    
    heroLayer->getCurrHeroHelper()->setJumpState(jumpState, 0, 0, 0);
}

void JumpKillMonsterState::execute(HeroLayer *heroLayer)
{
	if (heroLayer->getMonsterKilledOk())
    {
        heroLayer->setMonsterKilledOk(false);
        
        heroLayer->getJumpFSM()->changeState(JumpUpState::getInstance());
    }
}

void JumpKillMonsterState::exit(HeroLayer *heroLayer)
{
    heroLayer->setAccEnable(true);
    
    //Point heroAcc = heroLayer->getHeroAcc();
    
	//heroAcc.y = -1600.0f;
    
	//heroLayer->setHeroAcc(heroAcc);
    
    auto heroVel = heroLayer->getHeroVel();
    
    heroVel.x = 0;
    
    heroLayer->setHeroVel(heroVel);
    
    // notify ladderLayer that monster can not be killed
    std::string eventName = EVENT_ATTACK_END;
    
	EventCustom event(eventName);
    
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    
    std::string jumpState = "JumpAttack";
    
    heroLayer->getCurrHeroHelper()->setJumpState(jumpState, 0, 0, 0);

	//heroLayer->setMonsterTouchedEnable(true);
}

// Once Normal state
void OnceNormalState::enter(HeroLayer *heroLayer)
{
    heroLayer->setBounceSpeed(BOUNCE_NORMAL_SPEED);
}

void OnceNormalState::execute(HeroLayer *heroLayer)
{
    // enter spring state
    if (heroLayer->getSpringCollideOk())
    {
        heroLayer->setSpringCollideOk(false);
        
        heroLayer->getOnceFSM()->changeState(OnceSpringState::getInstance());
    }
    
    //enter trampoline state
    if (heroLayer->getTrampolineCollideOk())
    {
        heroLayer->setTrampolineCollideOk(false);
        
        heroLayer->getOnceFSM()->changeState(OnceTrampolineState::getInstance());
    }
}

void OnceNormalState::exit(HeroLayer *heroLayer)
{
}

// Once Spring state
void OnceSpringState::enter(HeroLayer *heroLayer)
{
    heroLayer->setSpringCollideOk(false);
    
    heroLayer->setBounceSpeed(1600.0f);
}

void OnceSpringState::execute(HeroLayer *heroLayer)
{
    //change to normal state when arrive the top point
    if (heroLayer->getHeroVel().y > 0 && heroLayer->getHeroVel().y < heroLayer->getBounceSpeed()/4)
    {
        heroLayer->getOnceFSM()->changeState(OnceNormalState::getInstance());
    }
}

void OnceSpringState::exit(HeroLayer *heroLayer)
{
}

// Once Trampoline state
void OnceTrampolineState::enter(HeroLayer *heroLayer)
{
    heroLayer->setTrampolineCollideOk(false);
    
    heroLayer->setBounceSpeed(2000.0f);
}

void OnceTrampolineState::execute(HeroLayer *heroLayer)
{
    //change to normal state
    if (heroLayer->getHeroVel().y > 0 && heroLayer->getHeroVel().y < heroLayer->getBounceSpeed()/4)
    {
        heroLayer->getOnceFSM()->changeState(OnceNormalState::getInstance());
    }
}

void OnceTrampolineState::exit(HeroLayer *heroLayer)
{
}
