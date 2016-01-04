//
//  ArmoryLayer.m
//  WaterBox2D
//
//  Created by Juan María Clavería Rosario on 02/04/14.
//  Copyright (c) 2014 Level App Studios. All rights reserved.
//

#include "ArmoryLayer.h"
#include "GameScene.h"
#include "SoundManager.h"
#include "ScheduleBlockAction.h"
#include "GameManager.h"
#include "NumberFormat.h"
#include "Analytics.h"
#include "ShopItem.h"
#include "CashNode.h"
#include "ParamManager.h"
#include "FyberHelper.h"
#include "Achievement.h"
#include "SKGameHelp.h"

#ifdef ANDROID
#include "FyberBridge.h"
#endif





class ArmoryMenuItem : public MenuItemSprite {public:
    int coins;
    int specialCoins;
    bool consumable;


@implementation ArmoryMenuItem
ArmoryLayer* ArmoryLayer::init() 
{
    this = super->init();
    if (this){
        this->coins = 0;
        this->specialCoins = 0;
        this->consumable = false;
    }
    return this;
}









#define TAG_MENU 1
#define TAG_TIMER_LABEL 2
#define TAG_DUMMY_TIMER 3

#define TAG_ACTION_TIMER 1

@implementation ArmoryLayer{
    int _numPlayer;
    ccTime _targetTime;
    ccTime _elapsedTime;
    ccTime _ticTacTime;
    int _lastItemTapped;
    bool _splitScreen;
}
ArmoryLayer* ArmoryLayer::layerWithTime(ccTime time, int numPlayer) 
 {
    return [ArmoryLayer::initWithTime(time, numPlayer]autorelease);
}
ArmoryLayer* ArmoryLayer::initWithTime(ccTime time, int numPlayer) 
 {
    this = super->init();
    if (this){
        GameState *state = this->gameState();
        AvatarConfig *avatar = state->avatarForPlayer(numPlayer);
        
        _numPlayer = numPlayer;
        _elapsedTime = 0.0f;
        _targetTime = time;
        _ticTacTime = 0.0f;
        _lastItemTapped = 0;
        _splitScreen = state.numPlayers == 2 && [this->gameScene()isSplitScreenEnabled];
        
        Menu *menu = Menu::menuWithItems(NULL);
        this->addChild(menu, 0, TAG_MENU);
       menu->setPosition(Point(0, 0));
        
        MenuItem *videoItem = NULL;
        
        int life = numPlayer == 1 ? state.lifePlayer1 : state.lifePlayer2;
        if (life > 0){
            // Armas
           Vector<T>* weapons = avatar->weapons();
            for (WeaponConfig *w in weapons){
                GameEntityType powerUpType = [w powerUpRelationship];
                Sprite *spriteOff = Sprite::createWithSpriteFrameName([PowerUp, powerUpType]);
                Sprite *spriteOn = Sprite::createWithSpriteFrameName([PowerUp, powerUpType]);
               spriteOn->setBlendFunc((ccBlendFunc){GL_SRC_ALPHA,GL_ONE});
                ArmoryMenuItem *item = ArmoryMenuItem::itemWithNormalSprite(spriteOff, spriteOn, this, @selector(didTapArmoryItem:));
                menu->addChild(item);
                item.tag = powerUpType;
                
                for (int i = 0; i < 5; i++){
                    Sprite *star = Sprite::createWithSpriteFrameName([w level] > i ? "armoryStarOn.png" : "armoryStarOff.png");
                    item->addChild(star, 5-i);
                   star->setScale(0.5f);
                    
                    switch (i) {
                        case 0:
                           star->setPosition(ccp(item->getContentSize().width*0.25f,item->getContentSize().height*1.15f));
                            break;
                        case 1:
                           star->setPosition(ccp(item->getContentSize().width*0.5f,item->getContentSize().height*1.15f));
                            break;
                        case 2:
                           star->setPosition(ccp(item->getContentSize().width*0.75f,item->getContentSize().height*1.15f));
                            break;
                        case 3:
                           star->setPosition(ccp(item->getContentSize().width*0.375f,item->getContentSize().height*1.15f + star->getContentSize().height*star->getScale()*0.7f));
                            break;
                        case 4:
                           star->setPosition(ccp(item->getContentSize().width*0.625f,item->getContentSize().height*1.15f + star->getContentSize().height*star->getScale()*0.7f));
                            break;
                        default:
                            break;
                    }
                }
                
                // Precio
                int numPowerUpsTotal = state->numPowerUpsBuyed();
                int numPowerUpsAdvance = state->numPowerUpsBuyedWithType(GAME_ENTITY_TYPE_POWER_UP_ADVANCE);
                int weaponsBuyed = numPowerUpsTotal - numPowerUpsAdvance;
                int coins,specialCoins;
                int numConsumables = [w numConsumables];
                int numPowerUpsBuyed = state->numPowerUpsBuyedWithType(powerUpType);
                bool isFree = state.numPlayers == 1 && (state->isGadgetEnabled(GADGET_TYPE_FIRST_WEAPON_FREE) && weaponsBuyed == 0);
                
                switch (numPowerUpsBuyed) {
                    case 0:
                        coins = ParamManager::sharedManager()->objectForKey("PowerUps")->objectForKey("weapon1")->objectForKey("coins")->intValue();
                        specialCoins = ParamManager::sharedManager()->objectForKey("PowerUps")->objectForKey("weapon1")->objectForKey("gems")->intValue();
                        break;
                    case 1:
                        coins = ParamManager::sharedManager()->objectForKey("PowerUps")->objectForKey("weapon2")->objectForKey("coins")->intValue();
                        specialCoins = ParamManager::sharedManager()->objectForKey("PowerUps")->objectForKey("weapon2")->objectForKey("gems")->intValue();
                        break;
                    case 2:
                        coins = ParamManager::sharedManager()->objectForKey("PowerUps")->objectForKey("weapon3")->objectForKey("coins")->intValue();
                        specialCoins = ParamManager::sharedManager()->objectForKey("PowerUps")->objectForKey("weapon3")->objectForKey("gems")->intValue();
                        break;
                    default:
                        coins = ParamManager::sharedManager()->objectForKey("PowerUps")->objectForKey("weapon4")->objectForKey("coins")->intValue();
                        specialCoins = ParamManager::sharedManager()->objectForKey("PowerUps")->objectForKey("weapon4")->objectForKey("gems")->intValue();
                        break;
                }
                
                
                Node *frame = NULL;
                if (isFree)
                    frame = this->createFrameFree();
                else if (numConsumables > 0){
                    item.consumable = true;
                    frame = this->createFrameWithNumItems(numConsumables);
                }
                else if (coins > 0){
                    item.coins = coins;
                    frame = this->createFrameWithCoins(coins);
                }
                else{
                    item.specialCoins = specialCoins;
                    frame = this->createFrameWithSpecialCoins(specialCoins);
                }

                item->addChild(frame);
               frame->setPosition(ccp(item->getContentSize().width*0.5f,-frame->getContentSize().height*0.5f));
            }
        }

        if (life == 0){
            Sprite *spriteOff = Sprite::createWithSpriteFrameName([PowerUp, GAME_ENTITY_TYPE_POWER_UP_HEALTH_PACK]);
            Sprite *spriteOn = Sprite::createWithSpriteFrameName([PowerUp, GAME_ENTITY_TYPE_POWER_UP_HEALTH_PACK]);
           spriteOn->setBlendFunc((ccBlendFunc){GL_SRC_ALPHA,GL_ONE});
            ArmoryMenuItem *item = ArmoryMenuItem::itemWithNormalSprite(spriteOff, spriteOn, this, @selector(didTapArmoryItem:));
            menu->addChild(item);
            item.tag = GAME_ENTITY_TYPE_POWER_UP_HEALTH_PACK;
            
            // Precio
            int coins,specialCoins;
            int paidReborns = MAX(0,state.reborns-(state.pokerReborns + (state.isVideoRewarded ?,  0)));
            switch (paidReborns) {
                case 0:
                    coins = ParamManager::sharedManager()->objectForKey("Reborn")->objectForKey("1")->objectForKey("coins")->intValue();
                    specialCoins = ParamManager::sharedManager()->objectForKey("Reborn")->objectForKey("1")->objectForKey("gems")->intValue();
                    break;
                case 1:
                    coins = ParamManager::sharedManager()->objectForKey("Reborn")->objectForKey("2")->objectForKey("coins")->intValue();
                    specialCoins = ParamManager::sharedManager()->objectForKey("Reborn")->objectForKey("2")->objectForKey("gems")->intValue();
                    break;
                case 2:
                    coins = ParamManager::sharedManager()->objectForKey("Reborn")->objectForKey("3")->objectForKey("coins")->intValue();
                    specialCoins = ParamManager::sharedManager()->objectForKey("Reborn")->objectForKey("3")->objectForKey("gems")->intValue();
                    break;
                case 3:
                    coins = ParamManager::sharedManager()->objectForKey("Reborn")->objectForKey("4")->objectForKey("coins")->intValue();
                    specialCoins = ParamManager::sharedManager()->objectForKey("Reborn")->objectForKey("4")->objectForKey("gems")->intValue();
                    break;
                default:
                    coins = ParamManager::sharedManager()->objectForKey("Reborn")->objectForKey("5")->objectForKey("coins")->intValue();
                    specialCoins = ParamManager::sharedManager()->objectForKey("Reborn")->objectForKey("5")->objectForKey("gems")->intValue();
                    break;
            }
            item.coins = coins;
            item.specialCoins = specialCoins;
            
            Node *frame = NULL;
            if (coins > 0)
                frame = this->createFrameWithCoins(coins);
            else
                frame = this->createFrameWithSpecialCoins(specialCoins);
            item->addChild(frame);
           frame->setPosition(ccp(item->getContentSize().width*0.5f,-frame->getContentSize().height*0.5f));
            
            
            // Ver vídeo
            bool canShowVideo = (state.numPlayers == 1 && !state.isVideoRewarded && SKGameHelper->shareIns()->checkVideoAvailable() != SK_VideoType_Nothing);
            if (canShowVideo){
                Sprite *videoSpriteNormal = Sprite::createWithSpriteFrameName("formButtonGreenNormal.png");
                Sprite *videoSpriteSelected = Sprite::createWithSpriteFrameName("formButtonGreenSelected.png");
                videoItem = [MenuItemSprite itemWithNormalSprite:videoSpriteNormal selectedSprite:videoSpriteSelected block:^ArmoryLayer* sender) {
                    // Pausa el timer, a veces Fyber tarda en dar la recompensa
                    Node *dummyTimer = this->getChildByTag(TAG_DUMMY_TIMER);
                    if (dummyTimer)
                        dummyTimer->pauseSchedulerAndActions();, :sharedManager()->playEffect(SOUND_BUTTON_OPEN);
                    this->schedule(@selector(checkVideoStatus));
                    // FyberHelper::startVideo();, :shareIns().typeVideoBounds = SK_Video_Reborn;
                    SKGameHelper->shareIns()->showVideoAds();
                    videoItem->setIsEnabled(true);
                    SoundManager->sharedManager()->pauseBackgroundMusic();
                });
                Sprite *icon = Sprite::createWithSpriteFrameName("formVideoIcon.png");
                videoItem->addChild(icon);
               icon->setPosition(ccp(videoItem->getContentSize().width*0.5f,videoItem->getContentSize().height*0.5f));
                
                LabelBMFont *label = LabelBMFont::labelWithString(NSLocalizedString("SHOP_WATCH_VIDEO", NULL), "fontMedium.fnt");
                videoItem->addChild(label);
               label->setScale(fminf(0.5f, videoItem->getContentSize().width*0.6f*videoItem->getScale()/label->getContentSize().width));
                //label.color = SHOP_ITEM_COLOR_FREE;
               label->setPosition(ccp(videoItem->getContentSize().width*0.65f,videoItem->getContentSize().height*0.5f));
                
                MenuItem *lastItem = [menu.children lastObject];
               videoItem->setScale(1.5f);
               videoItem->setPosition(ccp(lastItem->getPosition().x, lastItem->getPosition().y - (lastItem->getContentSize().height * 0.9f * lastItem->getScale() + videoItem->getContentSize().height*0.75f*videoItem->getScale())));
            }
            
            // Latido
           item->setScale(1.25f);
            Sprite *heart = Sprite::createWithSpriteFrameName("armoryHeart.png");
            item->addChild(heart, 2);
           heart->setPosition(ccp(item->getContentSize().width*0.5f,item->getContentSize().height*0.5f));
            ScaleBy *scaleBy1 = ScaleBy::create(0.4f, item->getScale());
            ScaleBy *scaleBy2 = (ScaleBy*)scaleBy1->reverse();
            Sequence *sequence = Sequence::create(scaleBy1,scaleBy2,NULL);
            RepeatForever *repeat = RepeatForever::create(sequence);
            heart->runAction(repeat);
            
            // Analíticas
            {
                // GameAnalytics
                AnalyticsEvent *event = AnalyticsEvent::eventWithName("Continue Screen");
                event->addSubEvent(AnalyticsEvent::eventWithName(canShowVideo ? "Video disponible" : "Video false disponible"));, :logGameAnalyticsDesignEvent(event);
            }
        }
        else{
            if (state.numPlayers == 2 && !_splitScreen && numPlayer == 1 && !this->gameState()->isPlayerDead(2)){
                Sprite *verticalBar = Sprite::createWithSpriteFrameName("armoryVerticalBar.png");
                this->addChild(verticalBar);
                verticalBar->getScale()Y = this->getContentSize().height*0.5f / verticalBar->getContentSize().height;
               verticalBar->setPosition(ccp(this->getContentSize().width*0.5f,this->getContentSize().height*0.5f));
               verticalBar->setOpacity(100);
            }
        }
        
        if (state->currentLevel() == PARALLAX_LAYER_TYPE_ENTRY && state.numPlayers == 1){
            // Booster
            GameEntityType powerUpType = GAME_ENTITY_TYPE_POWER_UP_ADVANCE;
            Sprite *spriteOff = Sprite::createWithSpriteFrameName([PowerUp, powerUpType]);
            Sprite *spriteOn = Sprite::createWithSpriteFrameName([PowerUp, powerUpType]);
           spriteOn->setBlendFunc((ccBlendFunc){GL_SRC_ALPHA,GL_ONE});
            ArmoryMenuItem *item = ArmoryMenuItem::itemWithNormalSprite(spriteOff, spriteOn, this, @selector(didTapArmoryItem:));
            menu->addChild(item);
            item.tag = powerUpType;
            
            LabelBMFont *label = LabelBMFont::labelWithString([NSString, NSLocalizedString("GAME_LEVEL_ENTRY",NULL),2], "fontSmall.fnt");
            item->addChild(label);
           label->setPosition(ccp(item->getContentSize().width*0.5f,item->getContentSize().height + label->getContentSize().height*0.65f));
            
            // Precio
            int coins = ParamManager::sharedManager()->objectForKey("PowerUps")->objectForKey("boost")->objectForKey("coins")->intValue();
            int specialCoins = ParamManager::sharedManager()->objectForKey("PowerUps")->objectForKey("boost")->objectForKey("gems")->intValue();
            int numConsumables = GameManager->sharedManager()->consumablesPowerUpAdvance();
            
            Node *frame = NULL;
            if (numConsumables > 0){
                item.consumable = true;
                frame = this->createFrameWithNumItems(numConsumables);
            }
            else if (coins > 0){
                item.coins = coins;
                frame = this->createFrameWithCoins(coins);
            }
            else{
                item.specialCoins = specialCoins;
                frame = this->createFrameWithSpecialCoins(specialCoins);
            }
            
            item->addChild(frame);
           frame->setPosition(ccp(item->getContentSize().width*0.5f,-frame->getContentSize().height*0.5f));
        }
        
        CGSize iconSize = SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("armoryIconFlameShot.png").rect.size;
        int numIcons = (int)[menu.children count];
        
        float itemScale;
        if (state.numPlayers == 1)
            itemScale = 1.0f;
        else if (_splitScreen)
            itemScale = [this->gameScene() splitScreenScale];
        else
            itemScale = 0.7f;

        float paddingX = iconSize.width*1.25f*itemScale;
        
        Point offset;
        if (state.numPlayers == 1 || _splitScreen){
            offset.x = (this->getContentSize().width - (paddingX * (numIcons-1)))*0.5f;
            
            if (_splitScreen)
                offset.y = this->getContentSize().height*0.5f - this->getContentSize().width*0.5f*0.4f;
            else
                offset.y = this->getContentSize().height*0.6f;
        }
        else{
            if (numPlayer == 1)
                offset.x = (this->getContentSize().width*0.25f - (paddingX * (numIcons-1) * 0.5f));
            else
                offset.x = (this->getContentSize().width*0.75f - (paddingX * (numIcons-1) * 0.5f));
            
            offset.y = this->getContentSize().height*0.6f;
        }
        
        int i = 0;
        for (MenuItem *item in menu.children){
           item->setPosition(ccpAdd(offset,ccp(paddingX*i,0.0f)));
           item->setScale(itemScale);
            
            // Agranda el área activa
            CGRect activeArea = item.activeArea;
            activeArea.origin = ccp(activeArea.origin.x,activeArea.origin.y - iconSize.height*0.5f);
            activeArea.size = Size(activeArea.size.width, activeArea.size.height + iconSize.height*0.5f);
            item.activeArea = activeArea;
            i++;
        }
        
        // Alijo
        CashNode *cashNode = NULL;
        if (life == 0){
            MenuItem *lastItem = [menu.children lastObject];
            cashNode = CashNode::specialCoinCashNode();
            this->addChild(cashNode);
           cashNode->setScale(itemScale);
           cashNode->setPosition(ccp(lastItem->getPosition().x, lastItem->getPosition().y - (lastItem->getContentSize().height * 0.9f * lastItem->getScale() + cashNode->getContentSize().height*0.75f*cashNode->getScale())));
        }
        else{
            MenuItem *lastItem = [menu.children lastObject];
            cashNode = CashNode::coinCashNode();
            this->addChild(cashNode);
           cashNode->setScale(itemScale);
           cashNode->setPosition(ccp(lastItem->getPosition().x + lastItem->getContentSize().width*0.5f*lastItem->getScale() - cashNode->getContentSize().width*0.5f*cashNode->getScale(), lastItem->getPosition().y - (lastItem->getContentSize().height * 0.9f * lastItem->getScale() + cashNode->getContentSize().height*0.75f*cashNode->getScale())));
        }
        cashNode.delegate = this;
        
        if (_targetTime > 0.0f){
            for (MenuItemSprite *item in menu.children){
                Sprite *mask = Sprite::createWithSpriteFrameName("armoryMask.png");
               mask->setOpacity(100);
                ProgressTimer *timer = ProgressTimer::progressWithSprite(mask);
                [item.normalImage addChild:timer];
                timer.type = kProgressTimerTypeRadial;
                timer.midpoint = ccp(0.5f,0.5f);
                timer.percentage = 0.0f;
               timer->setPosition(ccp(item->getContentSize().width*0.5f,item->getContentSize().height*0.5f));
                
                ScheduleBlockAction *action = [ScheduleBlockAction actionWithScheduleBlock:^(ccTime dt) {
                    timer.percentage = fminf(100.0f,timer.percentage + (100.0f/_targetTime) * dt);
                });
                timer->runAction(action);
            }
            
            // Timer
            Node *dummyTimer = Node::node();
            this->addChild(dummyTimer, 0, TAG_DUMMY_TIMER);
            
            DelayTime *delay = DelayTime::create(_targetTime);
            CallFunc *block = CallFunc::create([this]() {
                int life = _numPlayer == 1 ? state.lifePlayer1 : state.lifePlayer2;
                if (life == 0)
                    state->setSecondChanceRefused(true, _numPlayer);
                [this->gameScene() hideArmoryForPlayer:_numPlayer];
            });
            Sequence *sequence = Sequence::create(delay,block,NULL);
            sequence.tag = TAG_ACTION_TIMER;
            dummyTimer->runAction(sequence);
        }
        
        // Vídeo
        if (videoItem){
            menu->addChild(videoItem);
           videoItem->setPosition(ccp(cashNode->getPosition().x, cashNode->getPosition().y -(cashNode->getContentSize().height*cashNode->getScale()*0.6f + videoItem->getContentSize().height*videoItem->getScale()*0.5f)));
        }
        
        if (_splitScreen){
            if (numPlayer == 1)
               this->setRotation(90.0f);
            else
               this->setRotation(-90.0f);
        }
        
        this->scheduleUpdate();
    }
    return this;
}

GameScene* ArmoryLayer::gameScene() 
{
    return GameScene::runningScene();
}

GameLayer* ArmoryLayer::gameLayer() 
{
    return [this->gameScene()gameLayer];
}

GameState* ArmoryLayer::gameState() 
{
    return [this->gameScene()gameState];
}

LabelBMFont* ArmoryLayer::timerLabel() 
{
    return (LabelBMFont*)this->getChildByTag(TAG_TIMER_LABEL);
}

Menu* ArmoryLayer::menu() 
{
    return (Menu*)this->getChildByTag(TAG_MENU);
}

Node* ArmoryLayer::createFrameWithCoins(int coins) 
{
    Sprite *frame = Sprite::createWithSpriteFrameName("formButtonPrice.png");
    
    LabelBMFont *label = LabelBMFont::labelWithString(StringUtils::format("%d", coins), "fontMedium.fnt");
    frame->addChild(label);
   label->setScale(0.6f);
    label.color = SHOP_ITEM_COLOR_COIN;
    
    Sprite *coin = Sprite::createWithSpriteFrameName("guiCoin.png");
    frame->addChild(coin);
   coin->setScale(frame->getContentSize().height*0.75f/coin->getContentSize().height);
    
    float totalWidth = label->getContentSize().width*label->getScale() + coin->getContentSize().width*coin->getScale();
   coin->setPosition(ccp(frame->getContentSize().width*0.5f - (totalWidth*0.5f - coin->getContentSize().width*coin->getScale()*0.5f),frame->getContentSize().height*0.5f));
   label->setPosition(ccp(coin->getPosition().x + coin->getContentSize().width*coin->getScale()*0.5f + label->getContentSize().width*label->getScale()*0.5f,coin->getPosition().y));
    
    return frame;
}

Node* ArmoryLayer::createFrameWithSpecialCoins(int specialCoins) 
{
    Sprite *frame = Sprite::createWithSpriteFrameName("formButtonPrice.png");
    
    LabelBMFont *label = LabelBMFont::labelWithString(StringUtils::format("%d", specialCoins), "fontMedium.fnt");
    frame->addChild(label);
   label->setScale(0.6f);
    label.color = SHOP_ITEM_COLOR_SPECIAL_COIN;
    
    Sprite *coin = Sprite::createWithSpriteFrameName("guiSpecialCoin.png");
    frame->addChild(coin);
   coin->setScale(frame->getContentSize().height*0.75f/coin->getContentSize().height);
    
    float totalWidth = label->getContentSize().width*label->getScale() + coin->getContentSize().width*coin->getScale();
   coin->setPosition(ccp(frame->getContentSize().width*0.5f - (totalWidth*0.5f - coin->getContentSize().width*coin->getScale()*0.5f),frame->getContentSize().height*0.5f));
   label->setPosition(ccp(coin->getPosition().x + coin->getContentSize().width*coin->getScale()*0.5f + label->getContentSize().width*label->getScale()*0.5f,coin->getPosition().y));
    
    return frame;
}

Node* ArmoryLayer::createFrameWithNumItems(int numItems) 
{
    Sprite *frame = Sprite::createWithSpriteFrameName("formButtonPrice.png");
    
   std::string string = NSString::stringWithFormat(NSLocalizedString("SHOP_LEFT", NULL),numItems);
    LabelBMFont *label = LabelBMFont::labelWithString(string, "fontMedium.fnt");
    frame->addChild(label);
   label->setPosition(ccp(frame->getContentSize().width*0.5f,frame->getContentSize().height*0.5f));
   label->setScale(0.6f);
    label.color = SHOP_ITEM_COLOR_LEFT;
    
    return frame;
}

Node* ArmoryLayer::createFrameFree() 
{
    Sprite *frame = Sprite::createWithSpriteFrameName("formButtonPrice.png");
    
    LabelBMFont *label = LabelBMFont::labelWithString(NSLocalizedString("SHOP_FREE", NULL), "fontMedium.fnt");
    frame->addChild(label);
   label->setScale(0.6f);
    label.color = SHOP_ITEM_COLOR_FREE;
   label->setPosition(ccp(frame->getContentSize().width*0.5f,frame->getContentSize().height*0.5f));
    
    return frame;
}

void ArmoryLayer::didTapArmoryItem(ArmoryMenuItem* item) 
{
    // Comprueba si puede comprar el ítem
    // Si la llamada viene de haber cerrado el formulario de monedas y faltan monedas
    // false se vuelve a preguntar
    GameScene *gameScene = this->gameScene();
    GameState *gameState = gameScene->gameState();
    GameManager *manager = GameManager::sharedManager();
    if (manager.coins < item.coins){
        if (_lastItemTapped == 0){
            _lastItemTapped = (int)item.tag;
            gameScene->pauseGameAndShowPauseLayer(false, _numPlayer);
            ShopCoinsLayer *shopLayer = ShopCoinsLayer::layerComesFrom("armory", false);
            shopLayer.delegate = this;
            gameScene->addChild(shopLayer, 10000);
            
            if (_splitScreen){
                if (_numPlayer == 1)
                   shopLayer->setRotation(90.0f);
                else
                   shopLayer->setRotation(-90.0f);
               shopLayer->setScale(0.85f);
            }
        }
        else
            _lastItemTapped = 0;
        return;
    }
    else if (manager.specialCoins < item.specialCoins){
        if (_lastItemTapped == 0){
            _lastItemTapped = (int)item.tag;
            gameScene->pauseGameAndShowPauseLayer(false, _numPlayer);
            ShopSpecialCoinsLayer *shopLayer = ShopSpecialCoinsLayer::layerComesFrom("armory", false);
            shopLayer.delegate = this;
            gameScene->addChild(shopLayer, 10000);
            
            if (_splitScreen){
                if (_numPlayer == 1)
                   shopLayer->setRotation(90.0f);
                else
                   shopLayer->setRotation(-90.0f);
               shopLayer->setScale(0.85f);
            }
        }
        else
            _lastItemTapped = 0;
        return;
    }
    else{
        if (item.tag == GAME_ENTITY_TYPE_POWER_UP_ADVANCE && item.consumable)
            manager.consumablesPowerUpAdvance = MAX(0,manager.consumablesPowerUpAdvance-1);
        
        gameState.powerUpsUsed++;
        gameState.coinsSpent += item.coins;
        gameState.specialCoinsSpent += item.specialCoins;
        manager.coins -= item.coins;
        manager.coinsSpent += item.coins;
        manager.specialCoins -= item.specialCoins;
        manager.specialCoinsSpent += item.specialCoins;
        manager->save();
        
        // Logros
        Achievement::didWasteMoney();
        
        // Pequeño efecto gráfico
        Sprite *sprite = Sprite::spriteWithSpriteFrame(((Sprite*)item.normalImage).displayFrame);
       sprite->setBlendFunc((ccBlendFunc){GL_SRC_ALPHA,GL_ONE});
        FadeOut *fadeOut = FadeOut::create(0.5f);
        ScaleBy *scaleBy = ScaleBy::create(0.5f, 2.0f);
        Spawn *spawn = Spawn::create(fadeOut,scaleBy,NULL);
        CallFunc *block = CallFunc::create([this]() {
            sprite->removeFromParent();
        });
        Sequence *sequence = Sequence::create(spawn,block,NULL);
        sprite->runAction(sequence);
        this->gameLayer()->addNodeInParticlesContainer(sprite);
       sprite->setPosition(item->getPosition());
    }

    GameEntityType powerUpType = GAME_ENTITY_TYPE_NONE;
    WeaponType weaponType = WEAPON_TYPE_NONE;
    switch (item.tag) {
        case GAME_ENTITY_TYPE_POWER_UP_HEAVY_MACHINE_GUN:
            weaponType = WEAPON_TYPE_HEAVY_MACHINE_GUN;
            SoundManager::sharedManager()->playEffect(SOUND_POWER_UP_HEAVY_MACHINE_GUN);
            gameState->loadWeapon(weaponType, _numPlayer);
            powerUpType = GAME_ENTITY_TYPE_POWER_UP_HEAVY_MACHINE_GUN;
            break;
        case GAME_ENTITY_TYPE_POWER_UP_ROCKET_LAUNCHER:
            weaponType = WEAPON_TYPE_ROCKET_LAUNCHER;
            SoundManager::sharedManager()->playEffect(SOUND_POWER_UP_ROCKET_LAUNCHER);
            gameState->loadWeapon(weaponType, _numPlayer);
            powerUpType = GAME_ENTITY_TYPE_POWER_UP_ROCKET_LAUNCHER;
            break;
        case GAME_ENTITY_TYPE_POWER_UP_FLAME_THROWER:
            weaponType = WEAPON_TYPE_FLAME_SHOT;
            SoundManager::sharedManager()->playEffect(SOUND_POWER_UP_FLAME_THROWER);
            gameState->loadWeapon(weaponType, _numPlayer);
            powerUpType = GAME_ENTITY_TYPE_POWER_UP_FLAME_THROWER;
            break;
        case GAME_ENTITY_TYPE_POWER_UP_SPREAD_GUN:
            weaponType = WEAPON_TYPE_SPREAD_GUN;
            SoundManager::sharedManager()->playEffect(SOUND_POWER_UP_SPREAD_GUN);
            gameState->loadWeapon(weaponType, _numPlayer);
            powerUpType = GAME_ENTITY_TYPE_POWER_UP_SPREAD_GUN;
            break;
        case GAME_ENTITY_TYPE_POWER_UP_LASER:
            weaponType = WEAPON_TYPE_LASER;
            SoundManager::sharedManager()->playEffect(SOUND_POWER_UP_LASER);
            gameState->loadWeapon(weaponType, _numPlayer);
            powerUpType = GAME_ENTITY_TYPE_POWER_UP_LASER;
            break;
        case GAME_ENTITY_TYPE_POWER_UP_CLUSTER:
            weaponType = WEAPON_TYPE_CLUSTER;
            SoundManager::sharedManager()->playEffect(SOUND_POWER_UP_CLUSTER);
            gameState->loadWeapon(weaponType, _numPlayer);
            powerUpType = GAME_ENTITY_TYPE_POWER_UP_CLUSTER;
            break;
        case GAME_ENTITY_TYPE_POWER_UP_HOMING_MISSILE:
            weaponType = WEAPON_TYPE_HOMING_MISSILE;
            SoundManager::sharedManager()->playEffect(SOUND_POWER_UP_HOMING_MISSILE);
            gameState->loadWeapon(weaponType, _numPlayer);
            powerUpType = GAME_ENTITY_TYPE_POWER_UP_HOMING_MISSILE;
            break;
        case GAME_ENTITY_TYPE_POWER_UP_PLASMA:
            weaponType = WEAPON_TYPE_PLASMA;
            SoundManager::sharedManager()->playEffect(SOUND_POWER_UP_PLASMA);
            gameState->loadWeapon(weaponType, _numPlayer);
            powerUpType = GAME_ENTITY_TYPE_POWER_UP_PLASMA;
            break;
        case GAME_ENTITY_TYPE_POWER_UP_CANNON:
            weaponType = WEAPON_TYPE_CANNON;
            SoundManager::sharedManager()->playEffect(SOUND_POWER_UP_CANNON);
            gameState->loadWeapon(weaponType, _numPlayer);
            powerUpType = GAME_ENTITY_TYPE_POWER_UP_CANNON;
            break;
        case GAME_ENTITY_TYPE_POWER_UP_FOOTBALL:
            weaponType = WEAPON_TYPE_FOOTBALL;
            SoundManager::sharedManager()->playEffect(SOUND_POWER_UP_FOOTBALL);
            gameState->loadWeapon(weaponType, _numPlayer);
            powerUpType = GAME_ENTITY_TYPE_POWER_UP_FOOTBALL;
            break;
        case GAME_ENTITY_TYPE_POWER_UP_RED_LASER:
            weaponType = WEAPON_TYPE_RED_LASER;
            SoundManager::sharedManager()->playEffect(SOUND_POWER_UP_RED_LASER);
            gameState->loadWeapon(weaponType, _numPlayer);
            powerUpType = GAME_ENTITY_TYPE_POWER_UP_RED_LASER;
            break;
        case GAME_ENTITY_TYPE_POWER_UP_HEALTH_PACK:
            if (this->gameState()->isPlayerDead(_numPlayer)){
                if (!this->gameState()->isSecondChanceRefusedForPlayer(_numPlayer)){
                    // Renacer
                    SoundManager::sharedManager()->playEffect(SOUND_POWER_UP_HEALTH_PACK);
                    if (_numPlayer == 1){
                        gameState.lifePlayer1 = MIN(gameState.lifePlayer1+1, gameState.maxLife);
                        powerUpType = GAME_ENTITY_TYPE_POWER_UP_HEALTH_PACK;
                        [[this->gameLayer()firstCharacter]reborn];
                        gameState.reborns++;
                    }
                    else{
                        gameState.lifePlayer2 = MIN(gameState.lifePlayer2+1, gameState.maxLife);
                        powerUpType = GAME_ENTITY_TYPE_POWER_UP_HEALTH_PACK;
                        [[this->gameLayer()secondCharacter]reborn];
                        gameState.reborns++;
                    }
                }
            }
            else{
                SoundManager::sharedManager()->playEffect(SOUND_POWER_UP_HEALTH_PACK);
                if (_numPlayer == 1){
                    gameState.lifePlayer1 = MIN(gameState.lifePlayer1+1, gameState.maxLife);
                    powerUpType = GAME_ENTITY_TYPE_POWER_UP_HEALTH_PACK;
                }
                else{
                    gameState.lifePlayer2 = MIN(gameState.lifePlayer2+1, gameState.maxLife);
                    powerUpType = GAME_ENTITY_TYPE_POWER_UP_HEALTH_PACK;
                }
            }
            break;
        case GAME_ENTITY_TYPE_POWER_UP_ADVANCE:
            powerUpType = GAME_ENTITY_TYPE_POWER_UP_ADVANCE;
            break;
        default:
            break;
    }
    
    if (powerUpType != GAME_ENTITY_TYPE_NONE){
        if (weaponType != WEAPON_TYPE_NONE)
            gameState->resetWeaponItemBoxTimer();
        
        if (powerUpType == GAME_ENTITY_TYPE_POWER_UP_ADVANCE){
            this->gameLayer()->enablePowerUpAdvance(true);
        }
        else{
            this->gameLayer()->addPickEntityParticlesWithPosition(item->convertToWorldSpaceAR(Point(0, 0)));
            [[this->gameScene()guiLayer]didPickPowerUp:powerUpType, item->convertToWorldSpaceAR(Point(0, 0)], _numPlayer);
        }
        
        if (item.consumable){
            if (weaponType != WEAPON_TYPE_NONE){
                WeaponConfig *w = WeaponConfig::weaponWithType(weaponType);
                [w removeConsumable];
            }
        }
        
        gameState->addPowerUpBuyed(powerUpType);
        [this->gameScene() hideArmoryForPlayer:_numPlayer];
        
        
        // Analíticas
        {
            GameEntity *dummy = GameEntity::entityWithType(powerUpType);
            dummy->cleanup();
            
            // Flurry
            {
                if (powerUpType != GAME_ENTITY_TYPE_POWER_UP_HEALTH_PACK){
                   std::string powerUpString = StringUtils::format("%@,%dveces", dummy->ident(),gameState->numPowerUpsBuyedWithType(powerUpType));
                    AnalyticsEvent *event = AnalyticsEvent::eventWithName("Purchase Power Up");
                    event->addParam(AnalyticsParam::paramStringWithString(powerUpString, "Power Up"));, :logFlurryEvent(event);
                }
                else{
                    AnalyticsEvent *event = AnalyticsEvent::eventWithName("Purchase Life");
                    event->addParam(AnalyticsParam::paramValueWithInt([gameState, powerUpType], "Veces"));, :logFlurryEvent(event);
                }
            }
            
            // Game Analytics
            {
                if (powerUpType == GAME_ENTITY_TYPE_POWER_UP_HEALTH_PACK){
                   std::string levelString = StringUtils::format("Fase %d", [this->gameState()currentLevelNumber]);
                    
                    AnalyticsEvent *event = AnalyticsEvent::eventWithName("Continue");
                    AnalyticsEvent *subEvent = AnalyticsEvent::eventWithName(levelString);
                    event->addSubEvent(subEvent);
                    subEvent->addParam(AnalyticsParam::paramStringWithString("Gema" key:"Pago"));, :logGameAnalyticsDesignEvent(event);, :logGameAnalyticsExpenseAndResumeEventsWithCoins(item.coins specialCoins:item.specialCoins, "Compra vidas");
                }
                else{
                    AnalyticsEvent *event = AnalyticsEvent::eventWithName("Purchase Power Up");
                    event->addParam(AnalyticsParam::paramStringWithInt([gameState numPowerUpsBuyedWithType:powerUpType] key:dummy->ident()));, :logGameAnalyticsDesignEvent(event);                    , :logGameAnalyticsExpenseAndResumeEventsWithCoins(item.coins specialCoins:item.specialCoins fromEvent:"Power Up", dummy->ident());
                }
            }
        }
    }
}

void ArmoryLayer::enableMenu(bool enable) 
{
    this->menu()->setEnabled(enable);
}

void ArmoryLayer::update(ccTime dt) 
{
    if (_targetTime > 0.0f){
        _elapsedTime += dt;
        ccTime rest = fmaxf(0.0f,_targetTime - _elapsedTime);
        LabelBMFont *label = this->timerLabel();
        label.string = StringUtils::format("%.1f", rest);
        _ticTacTime += dt;
        if (_ticTacTime > 1.0f){
            _ticTacTime = 0.0f;
            SoundManager::sharedManager()->playEffect(SOUND_TIC_TAC);
        }
    }
}

void ArmoryLayer::checkVideoStatus() 
{
    bool completed = SKGameHelper::shareIns().typeFinished == SK_Player_Done; // Player Success
    bool aborted = SKGameHelper::shareIns().typeFinished == SK_Player_Close;  // Player UnSuccess
    
    if (completed || aborted)
        SoundManager->sharedManager()->resumeBackgroundMusic();
    
    if (completed){
        this->unschedule(@selector(checkVideoStatus));, :sharedManager()->playEffect(SOUND_POWER_UP_HEALTH_PACK);
        GameState *gameState = this->gameState();
        gameState.lifePlayer1 = MIN(gameState.lifePlayer1+1, gameState.maxLife);
        gameState.isVideoRewarded = true;
        [[this->gameLayer()firstCharacter]reborn];
        gameState.reborns++;
        [this->gameScene() hideArmoryForPlayer:_numPlayer];
        
        // Analíticas
        // Flurry
        {
            AnalyticsEvent *event = AnalyticsEvent::eventWithName("Watch Video");
            event->addParam(AnalyticsParam::paramMilitarRange());
            event->addParam(AnalyticsParam::paramIsFirstSession());, :logFlurryEvent(event);
        }
        // Game Analytics
        {
           std::string levelString = StringUtils::format("Fase %d", [this->gameState()currentLevelNumber]);
            
            AnalyticsEvent *event = AnalyticsEvent::eventWithName("Continue");
            AnalyticsEvent *subEvent = AnalyticsEvent::eventWithName(levelString);
            event->addSubEvent(subEvent);
            subEvent->addParam(AnalyticsParam::paramStringWithString("Video", "Pago"));, :logGameAnalyticsDesignEvent(event);
        }
    }
    else if (aborted){
        Node *dummyTimer = this->getChildByTag(TAG_DUMMY_TIMER);
        dummyTimer->resumeSchedulerAndActions();
    }
}

//
// ShopCoinsDelegate
//
void ArmoryLayer::shopCoinsLayerWillClose() 
{
    [this->gameScene()resumeGame];
    ArmoryMenuItem *item = (ArmoryMenuItem*)this->menu()->getChildByTag(_lastItemTapped);
    if (item)
        this->didTapArmoryItem(item);
}

//
// ShopSpecialCoinsDelegate
//
void ArmoryLayer::shopSpecialCoinsLayerWillClose() 
{
    [this->gameScene()resumeGame];
    ArmoryMenuItem *item = (ArmoryMenuItem*)this->menu()->getChildByTag(_lastItemTapped);
    if (item)
        this->didTapArmoryItem(item);
}

//
// CashNodeDelegate
//
void ArmoryLayer::didTapCoinsCashNode(CashNode * node) 
{
    SoundManager::sharedManager()->playEffect(SOUND_BUTTON_OPEN);
    
    GameScene *gameScene = this->gameScene();
    gameScene->pauseGameAndShowPauseLayer(false, _numPlayer);
    
    ShopCoinsLayer *shopLayer = ShopCoinsLayer::layerComesFrom("armory", true);
    gameScene->addChild(shopLayer, 10000);
    shopLayer.delegate = this;
    
    if (_splitScreen){
        if (_numPlayer == 1)
           shopLayer->setRotation(90.0f);
        else
           shopLayer->setRotation(-90.0f);
       shopLayer->setScale(0.85f);
    }
}

void ArmoryLayer::didTapSpecialCoinsCashNode(CashNode * node) 
{
    SoundManager::sharedManager()->playEffect(SOUND_BUTTON_OPEN);
    
    GameScene *gameScene = this->gameScene();
    gameScene->pauseGameAndShowPauseLayer(false, _numPlayer);
    
    ShopSpecialCoinsLayer *shopLayer = ShopSpecialCoinsLayer::layerComesFrom("armory", true);
    gameScene->addChild(shopLayer, 10000);
    shopLayer.delegate = this;
    
    if (_splitScreen){
        if (_numPlayer == 1)
           shopLayer->setRotation(90.0f);
        else
           shopLayer->setRotation(-90.0f);
       shopLayer->setScale(0.85f);
    }
}


