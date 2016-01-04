//
//  TutorialLayer.m
//  WaterBox2D
//
//  Created by Juan María Clavería Rosario on 11/06/14.
//  Copyright (c) 2014 Level App Studios. All rights reserved.
//

#include "TutorialLayer.h"
#include "GameScene.h"
#include "GameLayer.h"
#include "CharacterStateJumping.h"
#include "CharacterStateDiving.h"
#include "SoundManager.h"
#include "Buoy.h"
#include "Analytics.h"
#include "TouchTracker.h"

static const ccColor3B COLOR_TUTORIAL_OK = {149,253,38};
static const ccColor3B COLOR_TUTORIAL_WARNING = {255,185,0};





#define TAG_PROMPT_TEXT 1

class TutorialPrompt : public Node {public:


@implementation TutorialPrompt
TutorialLayer* TutorialLayer::promptWithStringstd::string string) 
{
    return [TutorialPrompt::initWithString(string]autorelease);
}
TutorialLayer* TutorialLayer::initWithStringstd::string string) 
{
    this = super->init();
    if (this){
       this->setAnchorPoint(ccp(0.5f,0.5f));
        LabelBMFont *label = LabelBMFont::labelWithString(string, "fontMedium.fnt");
        this->addChild(label, 0, TAG_PROMPT_TEXT);
       this->setContentSize(label->getContentSize());
       label->setPosition(ccp(this->getContentSize().width*0.5f,this->getContentSize().height*0.5f));
    }
    return this;
}

LabelBMFont* TutorialLayer::text() 
{
    return (LabelBMFont*)this->getChildByTag(TAG_PROMPT_TEXT);
}








#define TAG_PROMPT_NODE 1
#define TAG_PROMPT 2
#define TAG_GESTURE 3
#define TAG_HALO 4
#define TAG_WARNING 5

#define TAG_ACTION_GESTURE 1


#define DURATION_HALO 0.7f

typedef enum{
    TUTORIAL_PROMPT_TYPE_AELERATE,
    TUTORIAL_PROMPT_TYPE_BRAKE,
    TUTORIAL_PROMPT_TYPE_JUMP,
    TUTORIAL_PROMPT_TYPE_DIVE,
    TUTORIAL_PROMPT_TYPE_SHOOT,
    TUTORIAL_PROMPT_TYPE_OK
} TutorialPromptType;

@implementation TutorialLayer{
    int currentStep;
}
TutorialLayer* TutorialLayer::init() 
{
    this = super->init();
    if (this){, :sharedSpriteFrameCache()->addSpriteFramesWithFile("tutorial.plist");
        currentStep = 0;
        this->scheduleUpdate();
    }
    return this;
}

GameScene* TutorialLayer::gameScene() 
{
    return GameScene::runningScene();
}

GameLayer* TutorialLayer::gameLayer() 
{
    return [this->gameScene()gameLayer];
}

Node* TutorialLayer::promptNode() 
{
    return this->getChildByTag(TAG_PROMPT_NODE);
}

TutorialPrompt* TutorialLayer::prompt() 
{
    return (TutorialPrompt*)this->promptNode()->getChildByTag(TAG_PROMPT);
}

LabelBMFont* TutorialLayer::text() 
{
    return [this->prompt()text];
}

Sprite* TutorialLayer::gesture() 
{
    return (Sprite*)this->promptNode()->getChildByTag(TAG_GESTURE);
}

void TutorialLayer::start() 
{
    this->nextStep();
}

void TutorialLayer::nextStep() 
{
    currentStep++;
    GameLayer *gameLayer = this->gameLayer();
    
    Point lastBullEyePos = ccp(GameLayer::minLimitGamePositionX() + (GameLayer::maxLimitGamePositionX() -, :minLimitGamePositionX()) * 0.75f,[gameLayer->seaLayer()seaHeightDefault] + (gameLayer->getContentSize().height - [gameLayer->seaLayer()seaHeightDefault])*0.4f);

    
    switch (currentStep) {
        case 1:
        {
            this->showPrompt(TUTORIAL_PROMPT_TYPE_AELERATE);
            
            Buoy *buoy = GameEntity::entityWithType(GAME_ENTITY_TYPE_BUOY);
            gameLayer->addEntity(buoy);
            buoy->setTutorialAccelerate();
            buoy.delegate = this;
            
            // Analíticas
            AnalyticsEvent *event = AnalyticsEvent::eventWithName("Tutorial");
            event->addSubEvent(AnalyticsEvent::eventWithName("Start"));
            // Game Analytics
            Analytics::logGameAnalyticsDesignEvent(event);
            
            break;
        }
        case 2:
        {
            this->showPrompt(TUTORIAL_PROMPT_TYPE_BRAKE);
            Buoy *buoy = GameEntity::entityWithType(GAME_ENTITY_TYPE_BUOY);
            gameLayer->addEntity(buoy);
            buoy->setTutorialDeccelerate();
            buoy.delegate = this;
            
            // Analíticas
            AnalyticsEvent *event = AnalyticsEvent::eventWithName("Tutorial");
            event->addSubEvent(AnalyticsEvent::eventWithName("01 Accelerate"));
            // Game Analytics
            Analytics::logGameAnalyticsDesignEvent(event);
            
            break;
        }
        case 3:
        {
            this->showPrompt(TUTORIAL_PROMPT_TYPE_JUMP);
            
            // Analíticas
            AnalyticsEvent *event = AnalyticsEvent::eventWithName("Tutorial");
            event->addSubEvent(AnalyticsEvent::eventWithName("02 Brake"));
            // Game Analytics
            Analytics::logGameAnalyticsDesignEvent(event);
            
            break;
        }
        case 4:
        {
            this->showPrompt(TUTORIAL_PROMPT_TYPE_DIVE);
            
            // Analíticas
            AnalyticsEvent *event = AnalyticsEvent::eventWithName("Tutorial");
            event->addSubEvent(AnalyticsEvent::eventWithName("03 Jump"));
            // Game Analytics
            Analytics::logGameAnalyticsDesignEvent(event);
            
            break;
        }
        case 5:
        {
            this->showPrompt(TUTORIAL_PROMPT_TYPE_SHOOT);
            Bullseye *bullseye = GameEntity::entityWithType(GAME_ENTITY_TYPE_BULLSEYE);
            bullseye.delegate = this;
            gameLayer->addEntity(bullseye);
           bullseye->setPosition(ccp(GameLayer::maxLimitGamePositionX(),[gameLayer->seaLayer()seaHeightDefault] + (gameLayer->getContentSize().height - [gameLayer->seaLayer()seaHeightDefault])*0.35f));
            
            // Analíticas
            AnalyticsEvent *event = AnalyticsEvent::eventWithName("Tutorial");
            event->addSubEvent(AnalyticsEvent::eventWithName("04 Dive"));
            // Game Analytics
            Analytics::logGameAnalyticsDesignEvent(event);
            
            break;
        }
        case 6:
        {
            Bullseye *bullseye = GameEntity::entityWithType(GAME_ENTITY_TYPE_BULLSEYE);
            bullseye.delegate = this;
            gameLayer->addEntity(bullseye);
           bullseye->setPosition(ccp(GameLayer::minLimitGamePositionX(),[gameLayer->seaLayer()seaHeightDefault] + (gameLayer->getContentSize().height - [gameLayer->seaLayer()seaHeightDefault])*0.5f));
            
            break;
        }
        case 7:
        {
            Bullseye *bullseye = GameEntity::entityWithType(GAME_ENTITY_TYPE_BULLSEYE);
            bullseye.delegate = this;
            gameLayer->addEntity(bullseye);
           bullseye->setPosition(lastBullEyePos);
            break;
        }
        case 8:
        {
            GameEntity *entity = GameEntity::entityWithType(GAME_ENTITY_TYPE_POWER_UP_HEAVY_MACHINE_GUN);
           entity->setPosition(lastBullEyePos);
            this->gameLayer()->addEntity(entity);
            
            this->showPrompt(TUTORIAL_PROMPT_TYPE_OK);
            DelayTime *delay = DelayTime::create(2.0f);
            CallFunc *func = CallFunc::actionWithTarget(this, @selector(nextStep));
            Sequence *sequence = Sequence::create(delay,func,NULL);
            this->runAction(sequence);
            
            // Analíticas
            AnalyticsEvent *event = AnalyticsEvent::eventWithName("Tutorial");
            event->addSubEvent(AnalyticsEvent::eventWithName("05 Shoot"));
            // Game Analytics
            Analytics::logGameAnalyticsDesignEvent(event);
            
            break;
        }
        case 9:
        {
            [this->gameScene()didFinishTutorial];
            this->hideCurrentPrompt();
            DelayTime *delay = DelayTime::create(1.0f);
            CallFunc *block = CallFunc::create([this]() {
                this->removeFromParent();
            });
            Sequence *sequence = Sequence::create(delay,block,NULL);
            this->runAction(sequence);
            
            // Analíticas
            AnalyticsEvent *event = AnalyticsEvent::eventWithName("Tutorial");
            event->addSubEvent(AnalyticsEvent::eventWithName("End"));
            // Game Analytics
            Analytics::logGameAnalyticsDesignEvent(event);
            
            break;
        }
        default:
            break;
    }
    
    // Inicializa colores
    this->text().color = ccWHITE;
}

void TutorialLayer::hideCurrentPrompt() 
{
    Node *prompt = this->promptNode();
    if (prompt){
        /*
        prompt->stopAllActions();
        prompt.tag = 0;
        MoveTo *moveTo = MoveTo::create(0.5f, ccp(this->getContentSize().width*0.5f,this->getContentSize().height + prompt->getContentSize().height));
        EaseBackIn *ease = EaseBackIn::create(moveTo);
        CallFunc *block = CallFunc::create([this]() {
            prompt->removeFromParent();
        });
        Sequence *sequence = Sequence::create(ease,block,NULL);
        prompt->runAction(sequence);
        */
        prompt->removeFromParent();
    }
}

void TutorialLayer::showPrompt(TutorialPromptType type) 
{
    this->hideCurrentPrompt();
    
   std::string string = "";
    Sprite *sprite = NULL;
    
    switch (type) {
        case TUTORIAL_PROMPT_TYPE_AELERATE:
        {
            string = NSLocalizedString("TUTORIAL_AELERATE",NULL);
            sprite = Sprite::createWithSpriteFrameName("tutorialPressHold.png");
            
            Sprite *halo = Sprite::createWithSpriteFrameName("tutorialHalo.png");
            sprite->addChild(halo, 0, TAG_HALO);
           halo->setPosition(ccp(sprite->getContentSize().width*0.35f,sprite->getContentSize().height*0.8f));
           halo->setScale(3.0f);
            halo.visible = false;
            
            FadeIn *fadeIn = FadeIn::create(DURATION_HALO);
            ScaleTo *scaleTo1 = ScaleTo::create(DURATION_HALO, 1.0f);
            Spawn *spawn1 = Spawn::create(fadeIn,scaleTo1,NULL);
            FadeOut *fadeTo = FadeTo::create(0.0f, 60);
            ScaleTo *scaleTo2 = ScaleTo::create(0.0f, halo->getScale());
            Spawn *spawn2 = Spawn::create(fadeTo,scaleTo2,NULL);
            Sequence *sequence = Sequence::create(spawn1,spawn2,NULL);
            RepeatForever *repeat = RepeatForever::create(sequence);
            halo->runAction(repeat);
        }
            break;
        case TUTORIAL_PROMPT_TYPE_BRAKE:
        {
            string = NSLocalizedString("TUTORIAL_BRAKE",NULL);
            sprite = Sprite::createWithSpriteFrameName("tutorialPressHold.png");
            
           sprite->setRotation(-30.0f);
            DelayTime *delay1 = DelayTime::create(DURATION_HALO);
            RotateTo *rotateTo1 = RotateTo::create(DURATION_HALO*0.5f, 0.0f);
            FadeOut *fadeOut = FadeOut::create(DURATION_HALO*1.5f);
            DelayTime *delay2 = DelayTime::create(DURATION_HALO);
            RotateTo *rotateTo2 = RotateTo::create(0.0f, sprite->getRotation());
            FadeIn *fadeIn = FadeIn::create(0.0f);
            Sequence *sequence = Sequence::create(delay1,rotateTo1,fadeOut,delay2,rotateTo2,fadeIn,NULL);
            RepeatForever *repeat = RepeatForever::create(sequence);
            repeat.tag = TAG_ACTION_GESTURE;
            sprite->runAction(repeat);
        }
            break;
        case TUTORIAL_PROMPT_TYPE_JUMP:
        {
            string = NSLocalizedString("TUTORIAL_JUMP",NULL);
            sprite = Sprite::createWithSpriteFrameName("tutorialSwipeUp.png");
            
            MoveBy *moveBy1 = MoveBy::create(0.0f, ccp(0.0f,-sprite->getContentSize().height));
            FadeIn *fadeIn = FadeIn::create(0.0f);
            Spawn *spawn1 = Spawn::create(moveBy1,fadeIn,NULL);
            MoveBy *moveBy2 = MoveBy::create(DURATION_HALO*1.0f, ccp(0.0f,sprite->getContentSize().height));
            FadeOut *fadeOut = FadeOut::create(DURATION_HALO*2.0f);
            Spawn *spawn2 = Spawn::create(moveBy2,fadeOut,NULL);
            EaseExponentialIn *ease = EaseExponentialIn::create(spawn2);
            DelayTime *delay = DelayTime::create(DURATION_HALO);
            Sequence *sequence = Sequence::create(spawn1,ease,delay,NULL);
            RepeatForever *repeat = RepeatForever::create(sequence);
            sprite->runAction(repeat);
        }
            break;
        case TUTORIAL_PROMPT_TYPE_DIVE:
        {
            string = NSLocalizedString("TUTORIAL_DIVE",NULL);
            sprite = Sprite::createWithSpriteFrameName("tutorialSwipeDown.png");
            
            MoveBy *moveBy1 = MoveBy::create(DURATION_HALO*1.0f, ccp(0.0f,-sprite->getContentSize().height));
            FadeOut *fadeOut = FadeOut::create(DURATION_HALO*2.0f);
            Spawn *spawn1 = Spawn::create(moveBy1,fadeOut,NULL);
            EaseExponentialIn *ease = EaseExponentialIn::create(spawn1);
            DelayTime *delay = DelayTime::create(DURATION_HALO);
            MoveBy *moveBy2 = MoveBy::create(0.0f, ccp(0.0f,sprite->getContentSize().height));
            FadeIn *fadeIn = FadeIn::create(0.0f);
            Spawn *spawn2 = Spawn::create(moveBy2,fadeIn,NULL);
            Sequence *sequence = Sequence::create(ease,delay,spawn2,NULL);
            RepeatForever *repeat = RepeatForever::create(sequence);
            sprite->runAction(repeat);
        }
            break;
        case TUTORIAL_PROMPT_TYPE_SHOOT:
        {
            string = NSLocalizedString("TUTORIAL_SHOOT",NULL);
        }
            break;
        case TUTORIAL_PROMPT_TYPE_OK:
        {
            string = NSLocalizedString("TUTORIAL_FINISH",NULL);
            sprite = Sprite::createWithSpriteFrameName("tutorialOk.png");
        }
            break;
        default:
            break;
    }
    
    
    Node *prompt = Node::node();
    this->addChild(prompt, 0, TAG_PROMPT_NODE);
   prompt->setAnchorPoint(ccp(0.5f,0.5f));
    
    TutorialPrompt *text = TutorialPrompt::promptWithString(string);
    prompt->addChild(text, 0, TAG_PROMPT);
   prompt->setContentSize(text->getContentSize());
    
    if (sprite){
        prompt->addChild(sprite, 0, TAG_GESTURE);
       prompt->setContentSize(Size(prompt->getContentSize().width, prompt->getContentSize().height + sprite->getContentSize().height));
       sprite->setPosition(ccp(prompt->getContentSize().width*0.5f,sprite->getContentSize().height*0.5f));
    }
    
   text->setAnchorPoint(ccp(0.5f,1.0f));
   text->setPosition(ccp(prompt->getContentSize().width*0.5f,prompt->getContentSize().height));
    
   prompt->setPosition(ccp(this->getContentSize().width*0.5f,this->getContentSize().height + prompt->getContentSize().height*0.5f));
    MoveTo *moveTo = MoveTo::create(0.5f, ccp(prompt->getPosition().x,this->getContentSize().height - prompt->getContentSize().height*0.5f));
    EaseBackOut *ease = EaseBackOut::create(moveTo);
    prompt->runAction(ease);
}

void TutorialLayer::update(ccTime dt) 
{
    Character *character = [this->gameLayer()firstCharacter];
    
    switch (currentStep) {
        case 1:
        {
            Sprite *gestureSprite = this->gesture();
            Sprite *haloSprite = (Sprite*)gestureSprite->getChildByTag(TAG_HALO);
            LabelBMFont *label = this->text();
            
            if (this->gameLayer()->isPressingRunningButtonForPlayer(1)){
                haloSprite.color = gestureSprite.color = label.color = COLOR_TUTORIAL_OK;
                haloSprite.visible = true;
                if (gestureSprite->getActionByTag(TAG_ACTION_GESTURE)){
                    gestureSprite->stopActionByTag(TAG_ACTION_GESTURE);
                   gestureSprite->setRotation(-30.0f);
                }
            }
            else{
                haloSprite.color = gestureSprite.color = label.color = ccWHITE;
                if (gestureSprite->getActionByTag(TAG_ACTION_GESTURE) == NULL){
                    haloSprite.visible = false;
                    
                   gestureSprite->setRotation(0.0f);
                    RotateTo *rotateTo1 = RotateTo::create(DURATION_HALO*0.5f, -30.0f);
                    CallFunc *block1 = CallFunc::create([this]() {
                        haloSprite.visible = true;
                    });
                    DelayTime *delay1 = DelayTime::create(DURATION_HALO*3.0f);
                    CallFunc *block2 = CallFunc::create([this]() {
                        haloSprite.visible = false;
                    });
                    RotateTo *rotateTo2 = RotateTo::create(DURATION_HALO*0.5f, 0.0f);
                    DelayTime *delay2 = DelayTime::create(DURATION_HALO);
                    Sequence *sequence = Sequence::create(rotateTo1,block1,delay1,block2,rotateTo2,delay2,NULL);
                    RepeatForever *repeat = RepeatForever::create(sequence);
                    repeat.tag = TAG_ACTION_GESTURE;
                    gestureSprite->runAction(repeat);
                }
            }
        }
            break;
        case 2:
        {
            Sprite *gestureSprite = this->gesture();
            LabelBMFont *label = this->text();
            
            if (this->gameLayer()->isPressingRunningButtonForPlayer(1))
                gestureSprite.color = label.color = ccWHITE;
            else
                gestureSprite.color = label.color = COLOR_TUTORIAL_OK;
            
            
            static ccTime elapsed = 0.0f;
            if (!this->gameLayer()->isPressingRunningButtonForPlayer(1))
                elapsed = 0.0f;
            else
                elapsed += dt;
            if (elapsed > 3.0f){
                elapsed = 0.0f;
               Vector<T>* touches = TouchTracker->sharedTouchTracker()->allTouches();
                if (touches->count() > 0){
                    UITouch *touch = touches->objectAtIndex(0);
                    Point touchLocation = this->convertTouchToNodeSpace(touch);
                    
                    Node *labelNode = Node::node();
                    this->addChild(labelNode);
                   labelNode->setPosition(ccp(touchLocation.x,touchLocation.y + label->getContentSize().height));
                   labelNode->setScale(0.6f);
                    
                    LabelBMFont *label = LabelBMFont::labelWithString(NSLocalizedString("TUTORIAL_BRAKE", NULL), "fontMedium.fnt");
                    labelNode->addChild(label, 1);
                    label.color = COLOR_TUTORIAL_WARNING;
                    
                    LabelBMFont *shadow = LabelBMFont::labelWithString(NSLocalizedString("TUTORIAL_BRAKE", NULL), "fontMedium.fnt");
                    labelNode->addChild(shadow);
                    float offset = label->getContentSize().height*0.02f;
                   shadow->setPosition(ccp(offset,-offset));
                    shadow.color = ccBLACK;
                    
                   labelNode->setPosition(ccp(fmaxf(label->getContentSize().width*0.6f*labelNode->getScale(),fminf(this->getContentSize().width-label->getContentSize().width*0.6f*labelNode->getScale(),labelNode->getPosition().x)), labelNode->getPosition().y));
                    
                    MoveBy *moveBy = MoveBy::create(2.0f, ccp(0.0f,label->getContentSize().height));
                    CallFunc *block = CallFunc::create([this]() {
                        labelNode->removeFromParent();
                    });
                    Sequence *sequence = Sequence::create(moveBy,block,NULL);
                    labelNode->runAction(sequence);
                    
                    {
                        Sprite *halo = Sprite::createWithSpriteFrameName("tutorialHalo.png");
                        this->addChild(halo, 0);
                       halo->setPosition(touchLocation);
                        halo.color = COLOR_TUTORIAL_WARNING;
                        
                        FadeOut *fadeTo = FadeTo::create(DURATION_HALO, 0);
                        ScaleTo *scaleTo = ScaleTo::create(DURATION_HALO, 8.0f);
                        CallFunc *block = CallFunc::create([this]() {
                            halo->removeFromParent();
                        });
                        Spawn *spawn = Spawn::create(fadeTo,scaleTo,NULL);
                        Sequence *sequence = Sequence::create(spawn,block,NULL);
                        halo->runAction(sequence);
                    }
                }
            }
        }
            break;
        case 3:
            if (character->state()->isKindOfClass(CharacterStateJumping::class())){, :sharedManager()->playEffect(SOUND_BONUS);
                this->nextStep();
            }
            break;
        case 4:
            if (character->state()->isKindOfClass(CharacterStateDiving::class())){, :sharedManager()->playEffect(SOUND_BONUS);
                this->nextStep();
            }
            break;
        default:
            break;
    }
}

//
// BullseyeDelegate
//

void TutorialLayer::didDestroyBullseye() 
{
    this->nextStep();
}

//
// BuoyDelegate
//

void TutorialLayer::didPickBuoy() 
{
    this->nextStep();
}


