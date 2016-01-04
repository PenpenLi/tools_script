//
//  HeroLayer.h
//  HiFly
//
//  Created by chris li on 14-5-13.
//
//

#ifndef __HiFly__HeroLayer__
#define __HiFly__HeroLayer__


#include "cocos2d.h"
#include "Hero.h"
#include "CollideInterface.h"
#include "FSM/StateMachine.h"

USING_NS_CC;

class HeroLayer : public cocos2d::Layer, public CollideInterface
{
public:
    CREATE_FUNC(HeroLayer);
    
    void sceneUpdate(float dt);
    
    Vector<Node*>* getCollideNode();
    
    Rect getCollideRect(Node *myNode, int targetNodeType);
    
    void handleCollide(Node *myNode, int targetNodeType);
    
    int getCollideNodeType(Node *myNode);
    
    void heroJumpUp();
    
    Hero* getCurrHeroHelper();

	bool resourrection();

	bool buyResourrection();
    
    //Propterty: prop FSM state machine
    CC_SYNTHESIZE(StateMachine<HeroLayer>*, m_propFSM, PropFSM);
    
    //Propterty: spring, trampoline state machine, no counter
    CC_SYNTHESIZE(StateMachine<HeroLayer>*, m_onceFSM, OnceFSM);
    
    //Property: jump FSM state machine
    CC_SYNTHESIZE(StateMachine<HeroLayer>*, m_jumpFSM, JumpFSM);

	//Property: state FSM state machine
    CC_SYNTHESIZE(StateMachine<HeroLayer>*, m_lifeFSM, LifeFSM);
    
    //Property: jumpSpeed
    CC_SYNTHESIZE(float, m_bounceSpeed, BounceSpeed);
    
    //Property: bounceOk
    CC_SYNTHESIZE(float, m_bounceOk, BounceOk);
    
    //Property: heroPos
    CC_SYNTHESIZE(Point, m_heroPos, HeroPos);
    
    //Property: heroVel
    CC_SYNTHESIZE(Point, m_heroVel, HeroVel);
    
    //Property: heroAcc
    CC_SYNTHESIZE(Point, m_heroAcc, HeroAcc);
    
    //Property: is collide with spring
    CC_SYNTHESIZE(bool, m_springCollideOk, SpringCollideOk);
    
    //Property: is collide with trampoline
    CC_SYNTHESIZE(bool, m_trampolineCollideOk, TrampolineCollideOk);
    
    //Property: is collide with magnet
    CC_SYNTHESIZE(bool, m_magnetCollideOk, MagnetCollideOk);
    
    //Property: is collide with helicopter
    CC_SYNTHESIZE(bool, m_helicopterCollideOk, HelicopterCollideOk);
    
    //Property: is collide with shield
    CC_SYNTHESIZE(bool, m_shieldCollideOk, ShieldCollideOk);

	//Property: is collide with ship
    CC_SYNTHESIZE(bool, m_shipCollideOk, ShipCollideOk);
    
    //Property: is collide with medicine
    CC_SYNTHESIZE(bool, m_medicineCollideOk, MedicineCollideOk);
    
    //Property: hero touched
    CC_SYNTHESIZE(bool, m_touchClickOk, TouchClickOk);
    
	//Property: is jumpPause state over ?
    CC_SYNTHESIZE(bool, m_jumpPauseOk, JumpPauseOk);

	//Property: is jumpResume state over ?
    CC_SYNTHESIZE(bool, m_jumpResumeOk, JumpResumeOk);

    //Property: is jumpReady state over ?
    CC_SYNTHESIZE(bool, m_jumpReadyOverOk, JumpReadyOverOk);
    
    //Property: is jumpAgain1 animation play over "
    CC_SYNTHESIZE(bool, m_jumpAgain1OverOk, JumpAgain1OverOk);

	//Property: is jumpAgain2 animation play over "
    CC_SYNTHESIZE(bool, m_jumpAgain2Running, JumpAgain2Running);
    
    //Property: is reward levels over ?
    CC_SYNTHESIZE(bool, m_rewardLevelsOver, RewardLevelsOver);
    
    //Property: is pass jumpReadyState ?
    CC_SYNTHESIZE(bool, m_jumpAgainToken, JumpAgainToken);

	//Property: is pass jumpReadyState ?
    CC_SYNTHESIZE(bool, m_jumpDeath, JumpDeath);

	CC_SYNTHESIZE(bool, m_monsterTouchedEnable, MonsterTouchedEnable);

	//Property: is pass jumpReadyState ?
    CC_SYNTHESIZE(bool, m_jumpDeath1OverOk, JumpDeath1OverOk);

	CC_SYNTHESIZE(bool, m_jumpDeathSpriteReady, JumpDeathSpriteReady);

    CC_SYNTHESIZE(bool, m_isDeath, IsDeath);

	CC_SYNTHESIZE(bool, m_isShield, IsShield);


	CC_SYNTHESIZE(bool, m_isBig, IsBig);


	CC_SYNTHESIZE(bool, m_isHelicopter, IsHelicopter);


	CC_SYNTHESIZE(bool, m_isReward, IsReward);


	CC_SYNTHESIZE(bool, m_isAttackJump, IsAttackJump);


    CC_SYNTHESIZE(bool, m_enterInvincible, EnterInvincible);


	CC_SYNTHESIZE(bool, m_exitInvincible, ExitInvincible);

	
	CC_SYNTHESIZE(bool, m_isStartSprint,IsStartSprint);

	CC_SYNTHESIZE(bool, m_isSuperBubble,IsSuperBubble);

	CC_SYNTHESIZE(bool, m_isSuperBubbleRunning,IsSuperBubbleRunning);

	// none:0 have:1 used:2 exit:3;
	CC_SYNTHESIZE(int, m_isDeathSprint,IsDeathSprint);
    
    // monster touched location
    CC_SYNTHESIZE(Point, m_monsterTouchLocation, MonsterTouchLocation);
    
    // monster touched ?
    CC_SYNTHESIZE(bool, m_monsterTouchedOk, MonsterTouchedOk);
    
    // Accelerator enable
    CC_SYNTHESIZE(bool, m_accEnable, AccEnable);
    
    // Monster Killed ?
    CC_SYNTHESIZE(bool, m_monsterKilledOk, MonsterKilledOk);
    
    // Looking Right ?
    CC_SYNTHESIZE(bool, m_lookRight, LookRight);
    
    // Is Battle Level is Over
    CC_SYNTHESIZE(bool, m_battleLevelOver, BattleLevelOver);
    
    // Is change direction ?
    CC_SYNTHESIZE(bool, m_canChangeDirection, CanChangeDirection);

	//win prop
	CC_SYNTHESIZE(bool, m_isWinProp, IsWinProp);

	//FrinedMod
	CC_SYNTHESIZE(bool, m_isFriendMod, IsFriendMod);
	
	//Ready for Reward Level
    CC_SYNTHESIZE(bool, m_isGoingReward, IsGoingReward);

	//Bubble is running
    CC_SYNTHESIZE(bool, m_isBubbleRunning, IsBubbleRunning);
    
    //Ship is running
    CC_SYNTHESIZE(bool, m_isShipRunning, IsShipRunning);
    
    //helicopter is running
    CC_SYNTHESIZE(bool, m_isHelicopterRunning, IsHelicopterRunning);
    
    //startSprite is running
    CC_SYNTHESIZE(bool, m_isStartSpriteRunning, IsStartSpriteRunning);

	//seahorse is ready
	 CC_SYNTHESIZE(bool, m_isSeaHorseReadyToAttack, IsSeaHorseReadyToAttack);
    
    //Start Three Second Invincible
    CC_SYNTHESIZE(bool, m_threeSecondInvincibleOk, ThreeSecondInvincibleOk);

	//Hp was minused.Ho ready for minus.
	CC_SYNTHESIZE(bool, m_hpMinusReady, HpMinusReady);

	//Friend Running
	CC_SYNTHESIZE(int, m_isFriendRunning, IsFriendRunning);

	//Helicopter End Running
	CC_SYNTHESIZE(bool, m_isHelicopterEndRunning, IsHelicopterEndRunning);

	//Three Second Running
	CC_SYNTHESIZE(bool, m_isThreeSecondRunning, IsThreeSecondRunning);

	//Ship End Running
	CC_SYNTHESIZE(bool, m_isShipEndRunning, IsShipEndRunning);

	//Resourrection Running
	CC_SYNTHESIZE(bool, m_isResourrectionRunning, IsResourrectionRunning);

	//Resourrection
	CC_SYNTHESIZE(bool, m_isResourrection, IsResourrection);

	//PROP_TORPEDO_TAG Attacked
	CC_SYNTHESIZE(bool, m_torpedoAttacked, TorpedoAttacked)

	//LADDER_BOMB_OFF_TAG Attacked
	CC_SYNTHESIZE(bool, m_ladderBombAttacked, LadderBombAttacked)

	//Recurrection Times
	CC_SYNTHESIZE(int, m_resurrectionTimes, ResurrectionTimes);

	//Real Die
	CC_SYNTHESIZE(bool, m_isRealDie, IsRealDie);

	//Buy Resourrection
	CC_SYNTHESIZE(bool, m_isBuyResourrection, IsBuyResourrection);

    //////////////////////////////////// Hero UI Data ////////////////////////////////////
    //Property: collect coin number
    CC_SYNTHESIZE(int, m_gold, Gold);
    
    //Property: collect star number
    CC_SYNTHESIZE(int, m_star, Star);

private:
    HeroLayer() {}
    
    virtual ~HeroLayer() {}
    
    bool init();
    
    void onEnter();
    
    void onExit();
    
    // input event handler
    void onAcceleration(Acceleration* acc, Event* unused_event);
    bool onTouchBegan(Touch *touch, Event *unused_event);
    void onTouchMoved(Touch *touch, Event *unused_event);
    void onTouchEnded(Touch *touch, Event *unused_event);
    
    void updatePos(float dt);
    
    void updateFSM(float dt);
    
    void checkGameOver();
    
	void addMonsterScore(int tag);

	void sendPropMessage(int tag, bool ret);
	
	void resetHero(int roleType, int level);
    
    Sprite* getSpriteHelper(Node *node);
    
    Vector<Node*> m_heroes;

	int m_playTimeCounter;

	float m_sliderPer;
};

/***************************************** State Define ***********************************************/
class LifeNormalState : public State<HeroLayer>
{
public:
    static LifeNormalState* getInstance()
    {
        static LifeNormalState state;
        
        return &state;
    }
    
    //execute when the state is entered
    virtual void enter(HeroLayer* heroLayer);
    
    //execute when each update occurs
    virtual void execute(HeroLayer* heroLayer);
    
    //execute when the state is exited
    virtual void exit(HeroLayer* heroLayer);
};

class LifeInvincibleState : public State<HeroLayer>
{
public:
    static LifeInvincibleState* getInstance()
    {
        static LifeInvincibleState state;
        
        return &state;
    }
    
    //execute when the state is entered
    virtual void enter(HeroLayer* heroLayer);
    
    //execute when each update occurs
    virtual void execute(HeroLayer* heroLayer);
    
    //execute when the state is exited
    virtual void exit(HeroLayer* heroLayer);
};

class PropNormalState : public State<HeroLayer>
{
public:
    static PropNormalState* getInstance()
    {
        static PropNormalState state;
        
        return &state;
    }
    
    //execute when the state is entered
    virtual void enter(HeroLayer* heroLayer);
    
    //execute when each update occurs
    virtual void execute(HeroLayer* heroLayer);
    
    //execute when the state is exited
    virtual void exit(HeroLayer* heroLayer);
};

//3 second Invincible
class PropThreeSecondInvincible : public State<HeroLayer>
{
public:
    static PropThreeSecondInvincible* getInstance()
    {
        static PropThreeSecondInvincible state;
        
		//reset
        return &state;
    }
    
    //execute when the state is entered
    virtual void enter(HeroLayer* heroLayer);
    
    //execute when each update occurs
    virtual void execute(HeroLayer* heroLayer);
    
    //execute when the state is exited
    virtual void exit(HeroLayer* heroLayer);
    
private:
    int m_counter;
};


//magnet
class PropMagnetState : public State<HeroLayer>
{
public:
    static PropMagnetState* getInstance()
    {
        static PropMagnetState state;
        
		//reset 
        return &state;
    }
    
    //execute when the state is entered
    virtual void enter(HeroLayer* heroLayer);
    
    //execute when each update occurs
    virtual void execute(HeroLayer* heroLayer);
    
    //execute when the state is exited
    virtual void exit(HeroLayer* heroLayer);
    
private:
    int m_counter;
};

//helicopter
class PropHelicopterState : public State<HeroLayer>
{
public:
    static PropHelicopterState* getInstance()
    {
        static PropHelicopterState state;
        
        return &state;
    }
    
    //execute when the state is entered
    virtual void enter(HeroLayer* heroLayer);
    
    //execute when each update occurs
    virtual void execute(HeroLayer* heroLayer);
    
    //execute when the state is exited
    virtual void exit(HeroLayer* heroLayer);
    
private:
    int m_counter;
};

//mashuroom
class PropMedicineState : public State<HeroLayer>
{
public:
    static PropMedicineState* getInstance()
    {
        static PropMedicineState state;
        
        return &state;
    }
    
    //execute when the state is entered
    virtual void enter(HeroLayer* heroLayer);
    
    //execute when each update occurs
    virtual void execute(HeroLayer* heroLayer);
    
    //execute when the state is exited
    virtual void exit(HeroLayer* heroLayer);
    
private:
    int m_counter;
};

//bubble
class PropShieldState : public State<HeroLayer>
{
public:
    static PropShieldState* getInstance()
    {
        static PropShieldState state;
        
        return &state;
    }
    
    //execute when the state is entered
    virtual void enter(HeroLayer* heroLayer);
    
    //execute when each update occurs
    virtual void execute(HeroLayer* heroLayer);
    
    //execute when the state is exited
    virtual void exit(HeroLayer* heroLayer);
    
private:
    int m_counter;
};

//jet
class PropShipState : public State<HeroLayer>
{
public:
    static PropShipState* getInstance()
    {
        static PropShipState state;
        
        return &state;
    }
    
    //execute when the state is entered
    virtual void enter(HeroLayer* heroLayer);
    
    //execute when each update occurs
    virtual void execute(HeroLayer* heroLayer);
    
    //execute when the state is exited
    virtual void exit(HeroLayer* heroLayer);
    
private:
    int m_counter;
};

//sprint
class PropStartSprintState : public State<HeroLayer>
{
public:
	static PropStartSprintState* getInstance()
	{
		static PropStartSprintState state;

		return &state;
	}

	//execute when the state is entered
	virtual void enter(HeroLayer* heroLayer);

	//execute when each update occurs
	virtual void execute(HeroLayer* heroLayer);

	//execute when the state is exited
	virtual void exit(HeroLayer* heroLayer);

private:
	int m_counter;
};

//diesprint
class PropDeathSprintState : public State<HeroLayer>
{
public:
	static PropDeathSprintState* getInstance()
	{
		static PropDeathSprintState state;

		return &state;
	}

	//execute when the state is entered
	virtual void enter(HeroLayer* heroLayer);

	//execute when each update occurs
	virtual void execute(HeroLayer* heroLayer);

	//execute when the state is exited
	virtual void exit(HeroLayer* heroLayer);

private:
	int m_counter;
};

class JumpPauseState : public State<HeroLayer>
{
public:
    static JumpPauseState* getInstance()
    {
        static JumpPauseState state;
        
        return &state;
    }
    
    //execute when the state is entered
    virtual void enter(HeroLayer* heroLayer);
    
    //execute when each update occurs
    virtual void execute(HeroLayer* heroLayer);
    
    //execute when the state is exited
    virtual void exit(HeroLayer* heroLayer);
};

class JumpReadyState : public State<HeroLayer>
{
public:
    static JumpReadyState* getInstance()
    {
        static JumpReadyState state;
        
        return &state;
    }
    
    //execute when the state is entered
    virtual void enter(HeroLayer* heroLayer);
    
    //execute when each update occurs
    virtual void execute(HeroLayer* heroLayer);
    
    //execute when the state is exited
    virtual void exit(HeroLayer* heroLayer);
};

class JumpUpState : public State<HeroLayer>
{
public:
    static JumpUpState* getInstance()
    {
        static JumpUpState state;
        
        return &state;
    }
    
    //execute when the state is entered
    virtual void enter(HeroLayer* heroLayer);
    
    //execute when each update occurs
    virtual void execute(HeroLayer* heroLayer);
    
    //execute when the state is exited
    virtual void exit(HeroLayer* heroLayer);
};

class JumpSuspendState : public State<HeroLayer>
{
public:
    static JumpSuspendState* getInstance()
    {
        static JumpSuspendState state;
        
        return &state;
    }
    
    //execute when the state is entered
    virtual void enter(HeroLayer* heroLayer);
    
    //execute when each update occurs
    virtual void execute(HeroLayer* heroLayer);
    
    //execute when the state is exited
    virtual void exit(HeroLayer* heroLayer);
};

class JumpDownState : public State<HeroLayer>
{
public:
    static JumpDownState* getInstance()
    {
        static JumpDownState state;
        
        return &state;
    }
    
    //execute when the state is entered
    virtual void enter(HeroLayer* heroLayer);
    
    //execute when each update occurs
    virtual void execute(HeroLayer* heroLayer);
    
    //execute when the state is exited
    virtual void exit(HeroLayer* heroLayer);
};

class JumpAgain1State : public State<HeroLayer>
{
public:
    static JumpAgain1State* getInstance()
    {
        static JumpAgain1State state;
        
        return &state;
    }
    
    //execute when the state is entered
    virtual void enter(HeroLayer* heroLayer);
    
    //execute when each update occurs
    virtual void execute(HeroLayer* heroLayer);
    
    //execute when the state is exited
    virtual void exit(HeroLayer* heroLayer);
};

class JumpAgain2State : public State<HeroLayer>
{
public:
    static JumpAgain2State* getInstance()
    {
        static JumpAgain2State state;
        
        return &state;
    }
    
    //execute when the state is entered
    virtual void enter(HeroLayer* heroLayer);
    
    //execute when each update occurs
    virtual void execute(HeroLayer* heroLayer);
    
    //execute when the state is exited
    virtual void exit(HeroLayer* heroLayer);
};

class JumpFlyState : public State<HeroLayer>
{
public:
    static JumpFlyState* getInstance()
    {
        static JumpFlyState state;
        
        return &state;
    }
    
    //execute when the state is entered
    virtual void enter(HeroLayer* heroLayer);
    
    //execute when each update occurs
    virtual void execute(HeroLayer* heroLayer);
    
    //execute when the state is exited
    virtual void exit(HeroLayer* heroLayer);
};

class JumpDeath1State : public State<HeroLayer>
{
public:
    static JumpDeath1State* getInstance()
    {
        static JumpDeath1State state;
        
        return &state;
    }
    
    //execute when the state is entered
    virtual void enter(HeroLayer* heroLayer);
    
    //execute when each update occurs
    virtual void execute(HeroLayer* heroLayer);
    
    //execute when the state is exited
    virtual void exit(HeroLayer* heroLayer);
};

class JumpDeath2State : public State<HeroLayer>
{
public:
    static JumpDeath2State* getInstance()
    {
        static JumpDeath2State state;
        
        return &state;
    }
    
    //execute when the state is entered
    virtual void enter(HeroLayer* heroLayer);
    
    //execute when each update occurs
    virtual void execute(HeroLayer* heroLayer);
    
    //execute when the state is exited
    virtual void exit(HeroLayer* heroLayer);
};

// Only Hero2 can enter this state
class JumpKillMonsterState : public State<HeroLayer>
{
public:
    static JumpKillMonsterState* getInstance()
    {
        static JumpKillMonsterState state;
        
        return &state;
    }
    
    //execute when the state is entered
    virtual void enter(HeroLayer* heroLayer);
    
    //execute when each update occurs
    virtual void execute(HeroLayer* heroLayer);
    
    //execute when the state is exited
    virtual void exit(HeroLayer* heroLayer);
};

class OnceNormalState : public State<HeroLayer>
{
public:
    static OnceNormalState* getInstance()
    {
        static OnceNormalState state;
        
        return &state;
    }
    
    //execute when the state is entered
    virtual void enter(HeroLayer* heroLayer);
    
    //execute when each update occurs
    virtual void execute(HeroLayer* heroLayer);
    
    //execute when the state is exited
    virtual void exit(HeroLayer* heroLayer);
};

class OnceSpringState : public State<HeroLayer>
{
public:
    static OnceSpringState* getInstance()
    {
        static OnceSpringState state;
        
        return &state;
    }
    
    //execute when the state is entered
    virtual void enter(HeroLayer* heroLayer);
    
    //execute when each update occurs
    virtual void execute(HeroLayer* heroLayer);
    
    //execute when the state is exited
    virtual void exit(HeroLayer* heroLayer);
};

class OnceTrampolineState : public State<HeroLayer>
{
public:
    static OnceTrampolineState* getInstance()
    {
        static OnceTrampolineState state;
        
        return &state;
    }
    
    //execute when the state is entered
    virtual void enter(HeroLayer* heroLayer);
    
    //execute when each update occurs
    virtual void execute(HeroLayer* heroLayer);
    
    //execute when the state is exited
    virtual void exit(HeroLayer* heroLayer);
};


#endif /* defined(__HiFly__HeroLayer__) */
