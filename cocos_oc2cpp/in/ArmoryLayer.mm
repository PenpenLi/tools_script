//
//  ArmoryLayer.m
//  WaterBox2D
//
//  Created by Juan María Clavería Rosario on 02/04/14.
//  Copyright (c) 2014 Level App Studios. All rights reserved.
//

#import "ArmoryLayer.h"
#import "GameScene.h"
#import "SoundManager.h"
#import "CCScheduleBlockAction.h"
#import "GameManager.h"
#import "NumberFormat.h"
#import "Analytics.h"
#import "ShopItem.h"
#import "CashNode.h"
#import "ParamManager.h"
#import "FyberHelper.h"
#import "Achievement.h"
#import "SKGameHelp.h"

#ifdef ANDROID
#import "FyberBridge.h"
#endif

#pragma mark -
#pragma mark ArmoryMenuItem
#pragma mark -

@interface ArmoryMenuItem : CCMenuItemSprite
@property int coins;
@property int specialCoins;
@property BOOL consumable;
@end

@implementation ArmoryMenuItem

-(id)init{
    self = [super init];
    if (self){
        self.coins = 0;
        self.specialCoins = 0;
        self.consumable = NO;
    }
    return self;
}

@end



#pragma mark -
#pragma mark ArmoryLayer
#pragma mark -

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
    BOOL _splitScreen;
}

+(id)layerWithTime:(ccTime)time numPlayer:(int)numPlayer{
    return [[[ArmoryLayer alloc]initWithTime:time numPlayer:numPlayer]autorelease];
}

-(id)initWithTime:(ccTime)time numPlayer:(int)numPlayer{
    self = [super init];
    if (self){
        GameState *state = [self gameState];
        AvatarConfig *avatar = [state avatarForPlayer:numPlayer];
        
        _numPlayer = numPlayer;
        _elapsedTime = 0.0f;
        _targetTime = time;
        _ticTacTime = 0.0f;
        _lastItemTapped = 0;
        _splitScreen = state.numPlayers == 2 && [[self gameScene]isSplitScreenEnabled];
        
        CCMenu *menu = [CCMenu menuWithItems:nil];
        [self addChild:menu z:0 tag:TAG_MENU];
        menu.position = CGPointZero;
        
        CCMenuItem *videoItem = nil;
        
        int life = numPlayer == 1 ? state.lifePlayer1 : state.lifePlayer2;
        if (life > 0){
            // Armas
            NSArray *weapons = [avatar weapons];
            for (WeaponConfig *w in weapons){
                GameEntityType powerUpType = [w powerUpRelationship];
                CCSprite *spriteOff = [CCSprite spriteWithSpriteFrameName:[PowerUp largeIconFrameNameWithType:powerUpType]];
                CCSprite *spriteOn = [CCSprite spriteWithSpriteFrameName:[PowerUp largeIconFrameNameWithType:powerUpType]];
                spriteOn.blendFunc = (ccBlendFunc){GL_SRC_ALPHA,GL_ONE};
                ArmoryMenuItem *item = [ArmoryMenuItem itemWithNormalSprite:spriteOff selectedSprite:spriteOn target:self selector:@selector(didTapArmoryItem:)];
                [menu addChild:item];
                item.tag = powerUpType;
                
                for (int i = 0; i < 5; i++){
                    CCSprite *star = [CCSprite spriteWithSpriteFrameName:[w level] > i ? @"armoryStarOn.png" : @"armoryStarOff.png"];
                    [item addChild:star z:5-i];
                    star.scale = 0.5f;
                    
                    switch (i) {
                        case 0:
                            star.position = ccp(item.contentSize.width*0.25f,item.contentSize.height*1.15f);
                            break;
                        case 1:
                            star.position = ccp(item.contentSize.width*0.5f,item.contentSize.height*1.15f);
                            break;
                        case 2:
                            star.position = ccp(item.contentSize.width*0.75f,item.contentSize.height*1.15f);
                            break;
                        case 3:
                            star.position = ccp(item.contentSize.width*0.375f,item.contentSize.height*1.15f + star.contentSize.height*star.scale*0.7f);
                            break;
                        case 4:
                            star.position = ccp(item.contentSize.width*0.625f,item.contentSize.height*1.15f + star.contentSize.height*star.scale*0.7f);
                            break;
                        default:
                            break;
                    }
                }
                
                // Precio
                int numPowerUpsTotal = [state numPowerUpsBuyed];
                int numPowerUpsAdvance = [state numPowerUpsBuyedWithType:GAME_ENTITY_TYPE_POWER_UP_ADVANCE];
                int weaponsBuyed = numPowerUpsTotal - numPowerUpsAdvance;
                int coins,specialCoins;
                int numConsumables = [w numConsumables];
                int numPowerUpsBuyed = [state numPowerUpsBuyedWithType:powerUpType];
                BOOL isFree = state.numPlayers == 1 && ([state isGadgetEnabled:GADGET_TYPE_FIRST_WEAPON_FREE] && weaponsBuyed == 0);
                
                switch (numPowerUpsBuyed) {
                    case 0:
                        coins = [[[[[ParamManager sharedManager]objectForKey:@"PowerUps"]objectForKey:@"weapon1"]objectForKey:@"coins"]intValue];
                        specialCoins = [[[[[ParamManager sharedManager]objectForKey:@"PowerUps"]objectForKey:@"weapon1"]objectForKey:@"gems"]intValue];
                        break;
                    case 1:
                        coins = [[[[[ParamManager sharedManager]objectForKey:@"PowerUps"]objectForKey:@"weapon2"]objectForKey:@"coins"]intValue];
                        specialCoins = [[[[[ParamManager sharedManager]objectForKey:@"PowerUps"]objectForKey:@"weapon2"]objectForKey:@"gems"]intValue];
                        break;
                    case 2:
                        coins = [[[[[ParamManager sharedManager]objectForKey:@"PowerUps"]objectForKey:@"weapon3"]objectForKey:@"coins"]intValue];
                        specialCoins = [[[[[ParamManager sharedManager]objectForKey:@"PowerUps"]objectForKey:@"weapon3"]objectForKey:@"gems"]intValue];
                        break;
                    default:
                        coins = [[[[[ParamManager sharedManager]objectForKey:@"PowerUps"]objectForKey:@"weapon4"]objectForKey:@"coins"]intValue];
                        specialCoins = [[[[[ParamManager sharedManager]objectForKey:@"PowerUps"]objectForKey:@"weapon4"]objectForKey:@"gems"]intValue];
                        break;
                }
                
                
                CCNode *frame = nil;
                if (isFree)
                    frame = [self createFrameFree];
                else if (numConsumables > 0){
                    item.consumable = YES;
                    frame = [self createFrameWithNumItems:numConsumables];
                }
                else if (coins > 0){
                    item.coins = coins;
                    frame = [self createFrameWithCoins:coins];
                }
                else{
                    item.specialCoins = specialCoins;
                    frame = [self createFrameWithSpecialCoins:specialCoins];
                }

                [item addChild:frame];
                frame.position = ccp(item.contentSize.width*0.5f,-frame.contentSize.height*0.5f);
            }
        }

        if (life == 0){
            CCSprite *spriteOff = [CCSprite spriteWithSpriteFrameName:[PowerUp largeIconFrameNameWithType:GAME_ENTITY_TYPE_POWER_UP_HEALTH_PACK]];
            CCSprite *spriteOn = [CCSprite spriteWithSpriteFrameName:[PowerUp largeIconFrameNameWithType:GAME_ENTITY_TYPE_POWER_UP_HEALTH_PACK]];
            spriteOn.blendFunc = (ccBlendFunc){GL_SRC_ALPHA,GL_ONE};
            ArmoryMenuItem *item = [ArmoryMenuItem itemWithNormalSprite:spriteOff selectedSprite:spriteOn target:self selector:@selector(didTapArmoryItem:)];
            [menu addChild:item];
            item.tag = GAME_ENTITY_TYPE_POWER_UP_HEALTH_PACK;
            
            // Precio
            int coins,specialCoins;
            int paidReborns = MAX(0,state.reborns-(state.pokerReborns + (state.isVideoRewarded ? 1 : 0)));
            switch (paidReborns) {
                case 0:
                    coins = [[[[[ParamManager sharedManager]objectForKey:@"Reborn"]objectForKey:@"1"]objectForKey:@"coins"]intValue];
                    specialCoins = [[[[[ParamManager sharedManager]objectForKey:@"Reborn"]objectForKey:@"1"]objectForKey:@"gems"]intValue];
                    break;
                case 1:
                    coins = [[[[[ParamManager sharedManager]objectForKey:@"Reborn"]objectForKey:@"2"]objectForKey:@"coins"]intValue];
                    specialCoins = [[[[[ParamManager sharedManager]objectForKey:@"Reborn"]objectForKey:@"2"]objectForKey:@"gems"]intValue];
                    break;
                case 2:
                    coins = [[[[[ParamManager sharedManager]objectForKey:@"Reborn"]objectForKey:@"3"]objectForKey:@"coins"]intValue];
                    specialCoins = [[[[[ParamManager sharedManager]objectForKey:@"Reborn"]objectForKey:@"3"]objectForKey:@"gems"]intValue];
                    break;
                case 3:
                    coins = [[[[[ParamManager sharedManager]objectForKey:@"Reborn"]objectForKey:@"4"]objectForKey:@"coins"]intValue];
                    specialCoins = [[[[[ParamManager sharedManager]objectForKey:@"Reborn"]objectForKey:@"4"]objectForKey:@"gems"]intValue];
                    break;
                default:
                    coins = [[[[[ParamManager sharedManager]objectForKey:@"Reborn"]objectForKey:@"5"]objectForKey:@"coins"]intValue];
                    specialCoins = [[[[[ParamManager sharedManager]objectForKey:@"Reborn"]objectForKey:@"5"]objectForKey:@"gems"]intValue];
                    break;
            }
            item.coins = coins;
            item.specialCoins = specialCoins;
            
            CCNode *frame = nil;
            if (coins > 0)
                frame = [self createFrameWithCoins:coins];
            else
                frame = [self createFrameWithSpecialCoins:specialCoins];
            [item addChild:frame];
            frame.position = ccp(item.contentSize.width*0.5f,-frame.contentSize.height*0.5f);
            
            
            // Ver vídeo
            BOOL canShowVideo = (state.numPlayers == 1 && !state.isVideoRewarded && [[SKGameHelper shareIns] checkVideoAvailable] != SK_VideoType_Nothing);
            if (canShowVideo){
                CCSprite *videoSpriteNormal = [CCSprite spriteWithSpriteFrameName:@"formButtonGreenNormal.png"];
                CCSprite *videoSpriteSelected = [CCSprite spriteWithSpriteFrameName:@"formButtonGreenSelected.png"];
                videoItem = [CCMenuItemSprite itemWithNormalSprite:videoSpriteNormal selectedSprite:videoSpriteSelected block:^(id sender) {
                    // Pausa el timer, a veces Fyber tarda en dar la recompensa
                    CCNode *dummyTimer = [self getChildByTag:TAG_DUMMY_TIMER];
                    if (dummyTimer)
                        [dummyTimer pauseSchedulerAndActions];
                    
                    [[SoundManager sharedManager]playEffect:SOUND_BUTTON_OPEN];
                    [self schedule:@selector(checkVideoStatus)];
                    // [FyberHelper startVideo];
                    [SKGameHelper shareIns].typeVideoBounds = SK_Video_Reborn;
                    [[SKGameHelper shareIns] showVideoAds];
                    [videoItem setIsEnabled:YES];
                    [[SoundManager sharedManager]pauseBackgroundMusic];
                }];
                CCSprite *icon = [CCSprite spriteWithSpriteFrameName:@"formVideoIcon.png"];
                [videoItem addChild:icon];
                icon.position = ccp(videoItem.contentSize.width*0.5f,videoItem.contentSize.height*0.5f);
                
                CCLabelBMFont *label = [CCLabelBMFont labelWithString:NSLocalizedString(@"SHOP_WATCH_VIDEO", nil) fntFile:@"fontMedium.fnt"];
                [videoItem addChild:label];
                label.scale = fminf(0.5f, videoItem.contentSize.width*0.6f*videoItem.scale/label.contentSize.width);
                //label.color = SHOP_ITEM_COLOR_FREE;
                label.position = ccp(videoItem.contentSize.width*0.65f,videoItem.contentSize.height*0.5f);
                
                CCMenuItem *lastItem = [menu.children lastObject];
                videoItem.scale = 1.5f;
                videoItem.position = ccp(lastItem.position.x, lastItem.position.y - (lastItem.contentSize.height * 0.9f * lastItem.scale + videoItem.contentSize.height*0.75f*videoItem.scale));
            }
            
            // Latido
            item.scale = 1.25f;
            CCSprite *heart = [CCSprite spriteWithSpriteFrameName:@"armoryHeart.png"];
            [item addChild:heart z:2];
            heart.position = ccp(item.contentSize.width*0.5f,item.contentSize.height*0.5f);
            CCScaleBy *scaleBy1 = [CCScaleBy actionWithDuration:0.4f scale:item.scale];
            CCScaleBy *scaleBy2 = (CCScaleBy*)[scaleBy1 reverse];
            CCSequence *sequence = [CCSequence actions:scaleBy1,scaleBy2,nil];
            CCRepeatForever *repeat = [CCRepeatForever actionWithAction:sequence];
            [heart runAction:repeat];
            
            // Analíticas
            {
                // GameAnalytics
                AnalyticsEvent *event = [AnalyticsEvent eventWithName:@"Continue Screen"];
                [event addSubEvent:[AnalyticsEvent eventWithName:canShowVideo ? @"Video disponible" : @"Video no disponible"]];
                [Analytics logGameAnalyticsDesignEvent:event];
            }
        }
        else{
            if (state.numPlayers == 2 && !_splitScreen && numPlayer == 1 && ![[self gameState]isPlayerDead:2]){
                CCSprite *verticalBar = [CCSprite spriteWithSpriteFrameName:@"armoryVerticalBar.png"];
                [self addChild:verticalBar];
                verticalBar.scaleY = self.contentSize.height*0.5f / verticalBar.contentSize.height;
                verticalBar.position = ccp(self.contentSize.width*0.5f,self.contentSize.height*0.5f);
                verticalBar.opacity = 100;
            }
        }
        
        if ([state currentLevel] == PARALLAX_LAYER_TYPE_ENTRY && state.numPlayers == 1){
            // Booster
            GameEntityType powerUpType = GAME_ENTITY_TYPE_POWER_UP_ADVANCE;
            CCSprite *spriteOff = [CCSprite spriteWithSpriteFrameName:[PowerUp largeIconFrameNameWithType:powerUpType]];
            CCSprite *spriteOn = [CCSprite spriteWithSpriteFrameName:[PowerUp largeIconFrameNameWithType:powerUpType]];
            spriteOn.blendFunc = (ccBlendFunc){GL_SRC_ALPHA,GL_ONE};
            ArmoryMenuItem *item = [ArmoryMenuItem itemWithNormalSprite:spriteOff selectedSprite:spriteOn target:self selector:@selector(didTapArmoryItem:)];
            [menu addChild:item];
            item.tag = powerUpType;
            
            CCLabelBMFont *label = [CCLabelBMFont labelWithString:[NSString stringWithFormat:NSLocalizedString(@"GAME_LEVEL_ENTRY",nil),2] fntFile:@"fontSmall.fnt"];
            [item addChild:label];
            label.position = ccp(item.contentSize.width*0.5f,item.contentSize.height + label.contentSize.height*0.65f);
            
            // Precio
            int coins = [[[[[ParamManager sharedManager]objectForKey:@"PowerUps"]objectForKey:@"boost"]objectForKey:@"coins"]intValue];
            int specialCoins = [[[[[ParamManager sharedManager]objectForKey:@"PowerUps"]objectForKey:@"boost"]objectForKey:@"gems"]intValue];
            int numConsumables = [[GameManager sharedManager]consumablesPowerUpAdvance];
            
            CCNode *frame = nil;
            if (numConsumables > 0){
                item.consumable = YES;
                frame = [self createFrameWithNumItems:numConsumables];
            }
            else if (coins > 0){
                item.coins = coins;
                frame = [self createFrameWithCoins:coins];
            }
            else{
                item.specialCoins = specialCoins;
                frame = [self createFrameWithSpecialCoins:specialCoins];
            }
            
            [item addChild:frame];
            frame.position = ccp(item.contentSize.width*0.5f,-frame.contentSize.height*0.5f);
        }
        
        CGSize iconSize = [[CCSpriteFrameCache sharedSpriteFrameCache]spriteFrameByName:@"armoryIconFlameShot.png"].rect.size;
        int numIcons = (int)[menu.children count];
        
        float itemScale;
        if (state.numPlayers == 1)
            itemScale = 1.0f;
        else if (_splitScreen)
            itemScale = [[self gameScene] splitScreenScale];
        else
            itemScale = 0.7f;

        float paddingX = iconSize.width*1.25f*itemScale;
        
        CGPoint offset;
        if (state.numPlayers == 1 || _splitScreen){
            offset.x = (self.contentSize.width - (paddingX * (numIcons-1)))*0.5f;
            
            if (_splitScreen)
                offset.y = self.contentSize.height*0.5f - self.contentSize.width*0.5f*0.4f;
            else
                offset.y = self.contentSize.height*0.6f;
        }
        else{
            if (numPlayer == 1)
                offset.x = (self.contentSize.width*0.25f - (paddingX * (numIcons-1) * 0.5f));
            else
                offset.x = (self.contentSize.width*0.75f - (paddingX * (numIcons-1) * 0.5f));
            
            offset.y = self.contentSize.height*0.6f;
        }
        
        int i = 0;
        for (CCMenuItem *item in menu.children){
            item.position = ccpAdd(offset,ccp(paddingX*i,0.0f));
            item.scale = itemScale;
            
            // Agranda el área activa
            CGRect activeArea = item.activeArea;
            activeArea.origin = ccp(activeArea.origin.x,activeArea.origin.y - iconSize.height*0.5f);
            activeArea.size = CGSizeMake(activeArea.size.width, activeArea.size.height + iconSize.height*0.5f);
            item.activeArea = activeArea;
            i++;
        }
        
        // Alijo
        CashNode *cashNode = nil;
        if (life == 0){
            CCMenuItem *lastItem = [menu.children lastObject];
            cashNode = [CashNode specialCoinCashNode];
            [self addChild:cashNode];
            cashNode.scale = itemScale;
            cashNode.position = ccp(lastItem.position.x, lastItem.position.y - (lastItem.contentSize.height * 0.9f * lastItem.scale + cashNode.contentSize.height*0.75f*cashNode.scale));
        }
        else{
            CCMenuItem *lastItem = [menu.children lastObject];
            cashNode = [CashNode coinCashNode];
            [self addChild:cashNode];
            cashNode.scale = itemScale;
            cashNode.position = ccp(lastItem.position.x + lastItem.contentSize.width*0.5f*lastItem.scale - cashNode.contentSize.width*0.5f*cashNode.scale, lastItem.position.y - (lastItem.contentSize.height * 0.9f * lastItem.scale + cashNode.contentSize.height*0.75f*cashNode.scale));
        }
        cashNode.delegate = self;
        
        if (_targetTime > 0.0f){
            for (CCMenuItemSprite *item in menu.children){
                CCSprite *mask = [CCSprite spriteWithSpriteFrameName:@"armoryMask.png"];
                mask.opacity = 100;
                CCProgressTimer *timer = [CCProgressTimer progressWithSprite:mask];
                [item.normalImage addChild:timer];
                timer.type = kCCProgressTimerTypeRadial;
                timer.midpoint = ccp(0.5f,0.5f);
                timer.percentage = 0.0f;
                timer.position = ccp(item.contentSize.width*0.5f,item.contentSize.height*0.5f);
                
                CCScheduleBlockAction *action = [CCScheduleBlockAction actionWithScheduleBlock:^(ccTime dt) {
                    timer.percentage = fminf(100.0f,timer.percentage + (100.0f/_targetTime) * dt);
                }];
                [timer runAction:action];
            }
            
            // Timer
            CCNode *dummyTimer = [CCNode node];
            [self addChild:dummyTimer z:0 tag:TAG_DUMMY_TIMER];
            
            CCDelayTime *delay = [CCDelayTime actionWithDuration:_targetTime];
            CCCallBlock *block = [CCCallBlock actionWithBlock:^{
                int life = _numPlayer == 1 ? state.lifePlayer1 : state.lifePlayer2;
                if (life == 0)
                    [state setSecondChanceRefused:YES forPlayer:_numPlayer];
                [[self gameScene] hideArmoryForPlayer:_numPlayer];
            }];
            CCSequence *sequence = [CCSequence actions:delay,block,nil];
            sequence.tag = TAG_ACTION_TIMER;
            [dummyTimer runAction:sequence];
        }
        
        // Vídeo
        if (videoItem){
            [menu addChild:videoItem];
            videoItem.position = ccp(cashNode.position.x, cashNode.position.y -(cashNode.contentSize.height*cashNode.scale*0.6f + videoItem.contentSize.height*videoItem.scale*0.5f));
        }
        
        if (_splitScreen){
            if (numPlayer == 1)
                self.rotation = 90.0f;
            else
                self.rotation = -90.0f;
        }
        
        [self scheduleUpdate];
    }
    return self;
}

-(GameScene*)gameScene{
    return [GameScene runningScene];
}

-(GameLayer*)gameLayer{
    return [[self gameScene]gameLayer];
}

-(GameState*)gameState{
    return [[self gameScene]gameState];
}

-(CCLabelBMFont*)timerLabel{
    return (CCLabelBMFont*)[self getChildByTag:TAG_TIMER_LABEL];
}

-(CCMenu*)menu{
    return (CCMenu*)[self getChildByTag:TAG_MENU];
}

-(CCNode*)createFrameWithCoins:(int)coins{
    CCSprite *frame = [CCSprite spriteWithSpriteFrameName:@"formButtonPrice.png"];
    
    CCLabelBMFont *label = [CCLabelBMFont labelWithString:[NSString stringWithFormat:@"%d",coins] fntFile:@"fontMedium.fnt"];
    [frame addChild:label];
    label.scale = 0.6f;
    label.color = SHOP_ITEM_COLOR_COIN;
    
    CCSprite *coin = [CCSprite spriteWithSpriteFrameName:@"guiCoin.png"];
    [frame addChild:coin];
    coin.scale = frame.contentSize.height*0.75f/coin.contentSize.height;
    
    float totalWidth = label.contentSize.width*label.scale + coin.contentSize.width*coin.scale;
    coin.position = ccp(frame.contentSize.width*0.5f - (totalWidth*0.5f - coin.contentSize.width*coin.scale*0.5f),frame.contentSize.height*0.5f);
    label.position = ccp(coin.position.x + coin.contentSize.width*coin.scale*0.5f + label.contentSize.width*label.scale*0.5f,coin.position.y);
    
    return frame;
}

-(CCNode*)createFrameWithSpecialCoins:(int)specialCoins{
    CCSprite *frame = [CCSprite spriteWithSpriteFrameName:@"formButtonPrice.png"];
    
    CCLabelBMFont *label = [CCLabelBMFont labelWithString:[NSString stringWithFormat:@"%d",specialCoins] fntFile:@"fontMedium.fnt"];
    [frame addChild:label];
    label.scale = 0.6f;
    label.color = SHOP_ITEM_COLOR_SPECIAL_COIN;
    
    CCSprite *coin = [CCSprite spriteWithSpriteFrameName:@"guiSpecialCoin.png"];
    [frame addChild:coin];
    coin.scale = frame.contentSize.height*0.75f/coin.contentSize.height;
    
    float totalWidth = label.contentSize.width*label.scale + coin.contentSize.width*coin.scale;
    coin.position = ccp(frame.contentSize.width*0.5f - (totalWidth*0.5f - coin.contentSize.width*coin.scale*0.5f),frame.contentSize.height*0.5f);
    label.position = ccp(coin.position.x + coin.contentSize.width*coin.scale*0.5f + label.contentSize.width*label.scale*0.5f,coin.position.y);
    
    return frame;
}

-(CCNode*)createFrameWithNumItems:(int)numItems{
    CCSprite *frame = [CCSprite spriteWithSpriteFrameName:@"formButtonPrice.png"];
    
    NSString *string = [NSString stringWithFormat:NSLocalizedString(@"SHOP_LEFT", nil),numItems];
    CCLabelBMFont *label = [CCLabelBMFont labelWithString:string fntFile:@"fontMedium.fnt"];
    [frame addChild:label];
    label.position = ccp(frame.contentSize.width*0.5f,frame.contentSize.height*0.5f);
    label.scale = 0.6f;
    label.color = SHOP_ITEM_COLOR_LEFT;
    
    return frame;
}

-(CCNode*)createFrameFree{
    CCSprite *frame = [CCSprite spriteWithSpriteFrameName:@"formButtonPrice.png"];
    
    CCLabelBMFont *label = [CCLabelBMFont labelWithString:NSLocalizedString(@"SHOP_FREE", nil) fntFile:@"fontMedium.fnt"];
    [frame addChild:label];
    label.scale = 0.6f;
    label.color = SHOP_ITEM_COLOR_FREE;
    label.position = ccp(frame.contentSize.width*0.5f,frame.contentSize.height*0.5f);
    
    return frame;
}

-(void)didTapArmoryItem:(ArmoryMenuItem*)item{
    // Comprueba si puede comprar el ítem
    // Si la llamada viene de haber cerrado el formulario de monedas y faltan monedas
    // no se vuelve a preguntar
    GameScene *gameScene = [self gameScene];
    GameState *gameState = [gameScene gameState];
    GameManager *manager = [GameManager sharedManager];
    if (manager.coins < item.coins){
        if (_lastItemTapped == 0){
            _lastItemTapped = (int)item.tag;
            [gameScene pauseGameAndShowPauseLayer:NO forPlayer:_numPlayer];
            ShopCoinsLayer *shopLayer = [ShopCoinsLayer layerComesFrom:@"armory" isPlanned:NO];
            shopLayer.delegate = self;
            [gameScene addChild:shopLayer z:10000];
            
            if (_splitScreen){
                if (_numPlayer == 1)
                    shopLayer.rotation = 90.0f;
                else
                    shopLayer.rotation = -90.0f;
                shopLayer.scale = 0.85f;
            }
        }
        else
            _lastItemTapped = 0;
        return;
    }
    else if (manager.specialCoins < item.specialCoins){
        if (_lastItemTapped == 0){
            _lastItemTapped = (int)item.tag;
            [gameScene pauseGameAndShowPauseLayer:NO forPlayer:_numPlayer];
            ShopSpecialCoinsLayer *shopLayer = [ShopSpecialCoinsLayer layerComesFrom:@"armory" isPlanned:NO];
            shopLayer.delegate = self;
            [gameScene addChild:shopLayer z:10000];
            
            if (_splitScreen){
                if (_numPlayer == 1)
                    shopLayer.rotation = 90.0f;
                else
                    shopLayer.rotation = -90.0f;
                shopLayer.scale = 0.85f;
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
        [manager save];
        
        // Logros
        [Achievement didWasteMoney];
        
        // Pequeño efecto gráfico
        CCSprite *sprite = [CCSprite spriteWithSpriteFrame:((CCSprite*)item.normalImage).displayFrame];
        sprite.blendFunc = (ccBlendFunc){GL_SRC_ALPHA,GL_ONE};
        CCFadeOut *fadeOut = [CCFadeOut actionWithDuration:0.5f];
        CCScaleBy *scaleBy = [CCScaleBy actionWithDuration:0.5f scale:2.0f];
        CCSpawn *spawn = [CCSpawn actions:fadeOut,scaleBy,nil];
        CCCallBlock *block = [CCCallBlock actionWithBlock:^{
            [sprite removeFromParent];
        }];
        CCSequence *sequence = [CCSequence actions:spawn,block,nil];
        [sprite runAction:sequence];
        [[self gameLayer]addNodeInParticlesContainer:sprite];
        sprite.position = item.position;
    }

    GameEntityType powerUpType = GAME_ENTITY_TYPE_NONE;
    WeaponType weaponType = WEAPON_TYPE_NONE;
    switch (item.tag) {
        case GAME_ENTITY_TYPE_POWER_UP_HEAVY_MACHINE_GUN:
            weaponType = WEAPON_TYPE_HEAVY_MACHINE_GUN;
            [[SoundManager sharedManager]playEffect:SOUND_POWER_UP_HEAVY_MACHINE_GUN];
            [gameState loadWeapon:weaponType forPlayer:_numPlayer];
            powerUpType = GAME_ENTITY_TYPE_POWER_UP_HEAVY_MACHINE_GUN;
            break;
        case GAME_ENTITY_TYPE_POWER_UP_ROCKET_LAUNCHER:
            weaponType = WEAPON_TYPE_ROCKET_LAUNCHER;
            [[SoundManager sharedManager]playEffect:SOUND_POWER_UP_ROCKET_LAUNCHER];
            [gameState loadWeapon:weaponType forPlayer:_numPlayer];
            powerUpType = GAME_ENTITY_TYPE_POWER_UP_ROCKET_LAUNCHER;
            break;
        case GAME_ENTITY_TYPE_POWER_UP_FLAME_THROWER:
            weaponType = WEAPON_TYPE_FLAME_SHOT;
            [[SoundManager sharedManager]playEffect:SOUND_POWER_UP_FLAME_THROWER];
            [gameState loadWeapon:weaponType forPlayer:_numPlayer];
            powerUpType = GAME_ENTITY_TYPE_POWER_UP_FLAME_THROWER;
            break;
        case GAME_ENTITY_TYPE_POWER_UP_SPREAD_GUN:
            weaponType = WEAPON_TYPE_SPREAD_GUN;
            [[SoundManager sharedManager]playEffect:SOUND_POWER_UP_SPREAD_GUN];
            [gameState loadWeapon:weaponType forPlayer:_numPlayer];
            powerUpType = GAME_ENTITY_TYPE_POWER_UP_SPREAD_GUN;
            break;
        case GAME_ENTITY_TYPE_POWER_UP_LASER:
            weaponType = WEAPON_TYPE_LASER;
            [[SoundManager sharedManager]playEffect:SOUND_POWER_UP_LASER];
            [gameState loadWeapon:weaponType forPlayer:_numPlayer];
            powerUpType = GAME_ENTITY_TYPE_POWER_UP_LASER;
            break;
        case GAME_ENTITY_TYPE_POWER_UP_CLUSTER:
            weaponType = WEAPON_TYPE_CLUSTER;
            [[SoundManager sharedManager]playEffect:SOUND_POWER_UP_CLUSTER];
            [gameState loadWeapon:weaponType forPlayer:_numPlayer];
            powerUpType = GAME_ENTITY_TYPE_POWER_UP_CLUSTER;
            break;
        case GAME_ENTITY_TYPE_POWER_UP_HOMING_MISSILE:
            weaponType = WEAPON_TYPE_HOMING_MISSILE;
            [[SoundManager sharedManager]playEffect:SOUND_POWER_UP_HOMING_MISSILE];
            [gameState loadWeapon:weaponType forPlayer:_numPlayer];
            powerUpType = GAME_ENTITY_TYPE_POWER_UP_HOMING_MISSILE;
            break;
        case GAME_ENTITY_TYPE_POWER_UP_PLASMA:
            weaponType = WEAPON_TYPE_PLASMA;
            [[SoundManager sharedManager]playEffect:SOUND_POWER_UP_PLASMA];
            [gameState loadWeapon:weaponType forPlayer:_numPlayer];
            powerUpType = GAME_ENTITY_TYPE_POWER_UP_PLASMA;
            break;
        case GAME_ENTITY_TYPE_POWER_UP_CANNON:
            weaponType = WEAPON_TYPE_CANNON;
            [[SoundManager sharedManager]playEffect:SOUND_POWER_UP_CANNON];
            [gameState loadWeapon:weaponType forPlayer:_numPlayer];
            powerUpType = GAME_ENTITY_TYPE_POWER_UP_CANNON;
            break;
        case GAME_ENTITY_TYPE_POWER_UP_FOOTBALL:
            weaponType = WEAPON_TYPE_FOOTBALL;
            [[SoundManager sharedManager]playEffect:SOUND_POWER_UP_FOOTBALL];
            [gameState loadWeapon:weaponType forPlayer:_numPlayer];
            powerUpType = GAME_ENTITY_TYPE_POWER_UP_FOOTBALL;
            break;
        case GAME_ENTITY_TYPE_POWER_UP_RED_LASER:
            weaponType = WEAPON_TYPE_RED_LASER;
            [[SoundManager sharedManager]playEffect:SOUND_POWER_UP_RED_LASER];
            [gameState loadWeapon:weaponType forPlayer:_numPlayer];
            powerUpType = GAME_ENTITY_TYPE_POWER_UP_RED_LASER;
            break;
        case GAME_ENTITY_TYPE_POWER_UP_HEALTH_PACK:
            if ([[self gameState]isPlayerDead:_numPlayer]){
                if (![[self gameState]isSecondChanceRefusedForPlayer:_numPlayer]){
                    // Renacer
                    [[SoundManager sharedManager]playEffect:SOUND_POWER_UP_HEALTH_PACK];
                    if (_numPlayer == 1){
                        gameState.lifePlayer1 = MIN(gameState.lifePlayer1+1, gameState.maxLife);
                        powerUpType = GAME_ENTITY_TYPE_POWER_UP_HEALTH_PACK;
                        [[[self gameLayer]firstCharacter]reborn];
                        gameState.reborns++;
                    }
                    else{
                        gameState.lifePlayer2 = MIN(gameState.lifePlayer2+1, gameState.maxLife);
                        powerUpType = GAME_ENTITY_TYPE_POWER_UP_HEALTH_PACK;
                        [[[self gameLayer]secondCharacter]reborn];
                        gameState.reborns++;
                    }
                }
            }
            else{
                [[SoundManager sharedManager]playEffect:SOUND_POWER_UP_HEALTH_PACK];
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
            [gameState resetWeaponItemBoxTimer];
        
        if (powerUpType == GAME_ENTITY_TYPE_POWER_UP_ADVANCE){
            [[self gameLayer]enablePowerUpAdvance:YES];
        }
        else{
            [[self gameLayer]addPickEntityParticlesWithPosition:[item convertToWorldSpaceAR:CGPointZero]];
            [[[self gameScene]guiLayer]didPickPowerUp:powerUpType fromPosition:[item convertToWorldSpaceAR:CGPointZero] forPlayer:_numPlayer];
        }
        
        if (item.consumable){
            if (weaponType != WEAPON_TYPE_NONE){
                WeaponConfig *w = [WeaponConfig weaponWithType:weaponType];
                [w removeConsumable];
            }
        }
        
        [gameState addPowerUpBuyed:powerUpType];
        [[self gameScene] hideArmoryForPlayer:_numPlayer];
        
        
        // Analíticas
        {
            GameEntity *dummy = [GameEntity entityWithType:powerUpType];
            [dummy cleanup];
            
            // Flurry
            {
                if (powerUpType != GAME_ENTITY_TYPE_POWER_UP_HEALTH_PACK){
                    NSString *powerUpString = [NSString stringWithFormat:@"%@,%dveces",[dummy ident],[gameState numPowerUpsBuyedWithType:powerUpType]];
                    AnalyticsEvent *event = [AnalyticsEvent eventWithName:@"Purchase Power Up"];
                    [event addParam:[AnalyticsParam paramStringWithString:powerUpString key:@"Power Up"]];
                    [Analytics logFlurryEvent:event];
                }
                else{
                    AnalyticsEvent *event = [AnalyticsEvent eventWithName:@"Purchase Life"];
                    [event addParam:[AnalyticsParam paramValueWithInt:[gameState numPowerUpsBuyedWithType:powerUpType] key:@"Veces"]];
                    [Analytics logFlurryEvent:event];
                }
            }
            
            // Game Analytics
            {
                if (powerUpType == GAME_ENTITY_TYPE_POWER_UP_HEALTH_PACK){
                    NSString *levelString = [NSString stringWithFormat:@"Fase %d",[[self gameState]currentLevelNumber]];
                    
                    AnalyticsEvent *event = [AnalyticsEvent eventWithName:@"Continue"];
                    AnalyticsEvent *subEvent = [AnalyticsEvent eventWithName:levelString];
                    [event addSubEvent:subEvent];
                    [subEvent addParam:[AnalyticsParam paramStringWithString:@"Gema" key:@"Pago"]];
                    
                    [Analytics logGameAnalyticsDesignEvent:event];
                    [Analytics logGameAnalyticsExpenseAndResumeEventsWithCoins:item.coins specialCoins:item.specialCoins fromEvent:@"Compra vidas"];
                }
                else{
                    AnalyticsEvent *event = [AnalyticsEvent eventWithName:@"Purchase Power Up"];
                    [event addParam:[AnalyticsParam paramStringWithInt:[gameState numPowerUpsBuyedWithType:powerUpType] key:[dummy ident]]];
                    [Analytics logGameAnalyticsDesignEvent:event];                    
                    [Analytics logGameAnalyticsExpenseAndResumeEventsWithCoins:item.coins specialCoins:item.specialCoins fromEvent:@"Power Up" subEvent:[dummy ident]];
                }
            }
        }
    }
}

-(void)enableMenu:(BOOL)enable{
    [[self menu]setEnabled:enable];
}

-(void)update:(ccTime)dt{
    if (_targetTime > 0.0f){
        _elapsedTime += dt;
        ccTime rest = fmaxf(0.0f,_targetTime - _elapsedTime);
        CCLabelBMFont *label = [self timerLabel];
        label.string = [NSString stringWithFormat:@"%.1f",rest];
        _ticTacTime += dt;
        if (_ticTacTime > 1.0f){
            _ticTacTime = 0.0f;
            [[SoundManager sharedManager]playEffect:SOUND_TIC_TAC];
        }
    }
}

-(void)checkVideoStatus{
    BOOL completed = [SKGameHelper shareIns].typeFinished == SK_Player_Done; // Player Success
    BOOL aborted = [SKGameHelper shareIns].typeFinished == SK_Player_Close;  // Player UnSuccess
    
    if (completed || aborted)
        [[SoundManager sharedManager]resumeBackgroundMusic];
    
    if (completed){
        [self unschedule:@selector(checkVideoStatus)];
        [[SoundManager sharedManager]playEffect:SOUND_POWER_UP_HEALTH_PACK];
        GameState *gameState = [self gameState];
        gameState.lifePlayer1 = MIN(gameState.lifePlayer1+1, gameState.maxLife);
        gameState.isVideoRewarded = YES;
        [[[self gameLayer]firstCharacter]reborn];
        gameState.reborns++;
        [[self gameScene] hideArmoryForPlayer:_numPlayer];
        
        // Analíticas
        // Flurry
        {
            AnalyticsEvent *event = [AnalyticsEvent eventWithName:@"Watch Video"];
            [event addParam:[AnalyticsParam paramMilitarRange]];
            [event addParam:[AnalyticsParam paramIsFirstSession]];
            [Analytics logFlurryEvent:event];
        }
        // Game Analytics
        {
            NSString *levelString = [NSString stringWithFormat:@"Fase %d",[[self gameState]currentLevelNumber]];
            
            AnalyticsEvent *event = [AnalyticsEvent eventWithName:@"Continue"];
            AnalyticsEvent *subEvent = [AnalyticsEvent eventWithName:levelString];
            [event addSubEvent:subEvent];
            [subEvent addParam:[AnalyticsParam paramStringWithString:@"Video" key:@"Pago"]];
            
            [Analytics logGameAnalyticsDesignEvent:event];
        }
    }
    else if (aborted){
        CCNode *dummyTimer = [self getChildByTag:TAG_DUMMY_TIMER];
        [dummyTimer resumeSchedulerAndActions];
    }
}

//
// ShopCoinsDelegate
//
-(void)shopCoinsLayerWillClose{
    [[self gameScene]resumeGame];
    ArmoryMenuItem *item = (ArmoryMenuItem*)[[self menu]getChildByTag:_lastItemTapped];
    if (item)
        [self didTapArmoryItem:item];
}

//
// ShopSpecialCoinsDelegate
//
-(void)shopSpecialCoinsLayerWillClose{
    [[self gameScene]resumeGame];
    ArmoryMenuItem *item = (ArmoryMenuItem*)[[self menu]getChildByTag:_lastItemTapped];
    if (item)
        [self didTapArmoryItem:item];
}

//
// CashNodeDelegate
//
-(void)didTapCoinsCashNode:(CashNode *)node{
    [[SoundManager sharedManager]playEffect:SOUND_BUTTON_OPEN];
    
    GameScene *gameScene = [self gameScene];
    [gameScene pauseGameAndShowPauseLayer:NO forPlayer:_numPlayer];
    
    ShopCoinsLayer *shopLayer = [ShopCoinsLayer layerComesFrom:@"armory" isPlanned:YES];
    [gameScene addChild:shopLayer z:10000];
    shopLayer.delegate = self;
    
    if (_splitScreen){
        if (_numPlayer == 1)
            shopLayer.rotation = 90.0f;
        else
            shopLayer.rotation = -90.0f;
        shopLayer.scale = 0.85f;
    }
}

-(void)didTapSpecialCoinsCashNode:(CashNode *)node{
    [[SoundManager sharedManager]playEffect:SOUND_BUTTON_OPEN];
    
    GameScene *gameScene = [self gameScene];
    [gameScene pauseGameAndShowPauseLayer:NO forPlayer:_numPlayer];
    
    ShopSpecialCoinsLayer *shopLayer = [ShopSpecialCoinsLayer layerComesFrom:@"armory" isPlanned:YES];
    [gameScene addChild:shopLayer z:10000];
    shopLayer.delegate = self;
    
    if (_splitScreen){
        if (_numPlayer == 1)
            shopLayer.rotation = 90.0f;
        else
            shopLayer.rotation = -90.0f;
        shopLayer.scale = 0.85f;
    }
}

@end
