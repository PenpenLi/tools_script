//
//  TutorialLayer.m
//  WaterBox2D
//
//  Created by Juan María Clavería Rosario on 11/06/14.
//  Copyright (c) 2014 Level App Studios. All rights reserved.
//

#import "TutorialLayer.h"
#import "GameScene.h"
#import "GameLayer.h"
#import "CharacterStateJumping.h"
#import "CharacterStateDiving.h"
#import "SoundManager.h"
#import "Buoy.h"
#import "Analytics.h"
#import "TouchTracker.h"

static const ccColor3B COLOR_TUTORIAL_OK = {149,253,38};
static const ccColor3B COLOR_TUTORIAL_WARNING = {255,185,0};

#pragma mark -
#pragma mark TutorialPrompt
#pragma mark -

#define TAG_PROMPT_TEXT 1

@interface TutorialPrompt : CCNode
@end

@implementation TutorialPrompt

+(id)promptWithString:(NSString*)string{
    return [[[TutorialPrompt alloc]initWithString:string]autorelease];
}

-(id)initWithString:(NSString*)string{
    self = [super init];
    if (self){
        self.anchorPoint = ccp(0.5f,0.5f);
        CCLabelBMFont *label = [CCLabelBMFont labelWithString:string fntFile:@"fontMedium.fnt"];
        [self addChild:label z:0 tag:TAG_PROMPT_TEXT];
        self.contentSize = label.contentSize;
        label.position = ccp(self.contentSize.width*0.5f,self.contentSize.height*0.5f);
    }
    return self;
}

-(CCLabelBMFont*)text{
    return (CCLabelBMFont*)[self getChildByTag:TAG_PROMPT_TEXT];
}

@end


#pragma mark -
#pragma mark TutorialLayer
#pragma mark -

#define TAG_PROMPT_NODE 1
#define TAG_PROMPT 2
#define TAG_GESTURE 3
#define TAG_HALO 4
#define TAG_WARNING 5

#define TAG_ACTION_GESTURE 1


#define DURATION_HALO 0.7f

typedef enum{
    TUTORIAL_PROMPT_TYPE_ACCELERATE,
    TUTORIAL_PROMPT_TYPE_BRAKE,
    TUTORIAL_PROMPT_TYPE_JUMP,
    TUTORIAL_PROMPT_TYPE_DIVE,
    TUTORIAL_PROMPT_TYPE_SHOOT,
    TUTORIAL_PROMPT_TYPE_OK
} TutorialPromptType;

@implementation TutorialLayer{
    int currentStep;
}

-(id)init{
    self = [super init];
    if (self){
        [[CCSpriteFrameCache sharedSpriteFrameCache]addSpriteFramesWithFile:@"tutorial.plist"];
        currentStep = 0;
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

-(CCNode*)promptNode{
    return [self getChildByTag:TAG_PROMPT_NODE];
}

-(TutorialPrompt*)prompt{
    return (TutorialPrompt*)[[self promptNode]getChildByTag:TAG_PROMPT];
}

-(CCLabelBMFont*)text{
    return [[self prompt]text];
}

-(CCSprite*)gesture{
    return (CCSprite*)[[self promptNode]getChildByTag:TAG_GESTURE];
}

-(void)start{
    [self nextStep];
}

-(void)nextStep{
    currentStep++;
    GameLayer *gameLayer = [self gameLayer];
    
    CGPoint lastBullEyePos = ccp([GameLayer minLimitGamePositionX] + ([GameLayer maxLimitGamePositionX] - [GameLayer minLimitGamePositionX]) * 0.75f,[[gameLayer seaLayer]seaHeightDefault] + (gameLayer.contentSize.height - [[gameLayer seaLayer]seaHeightDefault])*0.4f);

    
    switch (currentStep) {
        case 1:
        {
            [self showPrompt:TUTORIAL_PROMPT_TYPE_ACCELERATE];
            
            Buoy *buoy = [GameEntity entityWithType:GAME_ENTITY_TYPE_BUOY];
            [gameLayer addEntity:buoy];
            [buoy setTutorialAccelerate];
            buoy.delegate = self;
            
            // Analíticas
            AnalyticsEvent *event = [AnalyticsEvent eventWithName:@"Tutorial"];
            [event addSubEvent:[AnalyticsEvent eventWithName:@"Start"]];
            // Game Analytics
            [Analytics logGameAnalyticsDesignEvent:event];
            
            break;
        }
        case 2:
        {
            [self showPrompt:TUTORIAL_PROMPT_TYPE_BRAKE];
            Buoy *buoy = [GameEntity entityWithType:GAME_ENTITY_TYPE_BUOY];
            [gameLayer addEntity:buoy];
            [buoy setTutorialDeccelerate];
            buoy.delegate = self;
            
            // Analíticas
            AnalyticsEvent *event = [AnalyticsEvent eventWithName:@"Tutorial"];
            [event addSubEvent:[AnalyticsEvent eventWithName:@"01 Accelerate"]];
            // Game Analytics
            [Analytics logGameAnalyticsDesignEvent:event];
            
            break;
        }
        case 3:
        {
            [self showPrompt:TUTORIAL_PROMPT_TYPE_JUMP];
            
            // Analíticas
            AnalyticsEvent *event = [AnalyticsEvent eventWithName:@"Tutorial"];
            [event addSubEvent:[AnalyticsEvent eventWithName:@"02 Brake"]];
            // Game Analytics
            [Analytics logGameAnalyticsDesignEvent:event];
            
            break;
        }
        case 4:
        {
            [self showPrompt:TUTORIAL_PROMPT_TYPE_DIVE];
            
            // Analíticas
            AnalyticsEvent *event = [AnalyticsEvent eventWithName:@"Tutorial"];
            [event addSubEvent:[AnalyticsEvent eventWithName:@"03 Jump"]];
            // Game Analytics
            [Analytics logGameAnalyticsDesignEvent:event];
            
            break;
        }
        case 5:
        {
            [self showPrompt:TUTORIAL_PROMPT_TYPE_SHOOT];
            Bullseye *bullseye = [GameEntity entityWithType:GAME_ENTITY_TYPE_BULLSEYE];
            bullseye.delegate = self;
            [gameLayer addEntity:bullseye];
            bullseye.position = ccp([GameLayer maxLimitGamePositionX],[[gameLayer seaLayer]seaHeightDefault] + (gameLayer.contentSize.height - [[gameLayer seaLayer]seaHeightDefault])*0.35f);
            
            // Analíticas
            AnalyticsEvent *event = [AnalyticsEvent eventWithName:@"Tutorial"];
            [event addSubEvent:[AnalyticsEvent eventWithName:@"04 Dive"]];
            // Game Analytics
            [Analytics logGameAnalyticsDesignEvent:event];
            
            break;
        }
        case 6:
        {
            Bullseye *bullseye = [GameEntity entityWithType:GAME_ENTITY_TYPE_BULLSEYE];
            bullseye.delegate = self;
            [gameLayer addEntity:bullseye];
            bullseye.position = ccp([GameLayer minLimitGamePositionX],[[gameLayer seaLayer]seaHeightDefault] + (gameLayer.contentSize.height - [[gameLayer seaLayer]seaHeightDefault])*0.5f);
            
            break;
        }
        case 7:
        {
            Bullseye *bullseye = [GameEntity entityWithType:GAME_ENTITY_TYPE_BULLSEYE];
            bullseye.delegate = self;
            [gameLayer addEntity:bullseye];
            bullseye.position = lastBullEyePos;
            break;
        }
        case 8:
        {
            GameEntity *entity = [GameEntity entityWithType:GAME_ENTITY_TYPE_POWER_UP_HEAVY_MACHINE_GUN];
            entity.position = lastBullEyePos;
            [[self gameLayer]addEntity:entity];
            
            [self showPrompt:TUTORIAL_PROMPT_TYPE_OK];
            CCDelayTime *delay = [CCDelayTime actionWithDuration:2.0f];
            CCCallFunc *func = [CCCallFunc actionWithTarget:self selector:@selector(nextStep)];
            CCSequence *sequence = [CCSequence actions:delay,func,nil];
            [self runAction:sequence];
            
            // Analíticas
            AnalyticsEvent *event = [AnalyticsEvent eventWithName:@"Tutorial"];
            [event addSubEvent:[AnalyticsEvent eventWithName:@"05 Shoot"]];
            // Game Analytics
            [Analytics logGameAnalyticsDesignEvent:event];
            
            break;
        }
        case 9:
        {
            [[self gameScene]didFinishTutorial];
            [self hideCurrentPrompt];
            CCDelayTime *delay = [CCDelayTime actionWithDuration:1.0f];
            CCCallBlock *block = [CCCallBlock actionWithBlock:^{
                [self removeFromParent];
            }];
            CCSequence *sequence = [CCSequence actions:delay,block,nil];
            [self runAction:sequence];
            
            // Analíticas
            AnalyticsEvent *event = [AnalyticsEvent eventWithName:@"Tutorial"];
            [event addSubEvent:[AnalyticsEvent eventWithName:@"End"]];
            // Game Analytics
            [Analytics logGameAnalyticsDesignEvent:event];
            
            break;
        }
        default:
            break;
    }
    
    // Inicializa colores
    [self text].color = ccWHITE;
}

-(void)hideCurrentPrompt{
    CCNode *prompt = [self promptNode];
    if (prompt){
        /*
        [prompt stopAllActions];
        prompt.tag = 0;
        CCMoveTo *moveTo = [CCMoveTo actionWithDuration:0.5f position:ccp(self.contentSize.width*0.5f,self.contentSize.height + prompt.contentSize.height)];
        CCEaseBackIn *ease = [CCEaseBackIn actionWithAction:moveTo];
        CCCallBlock *block = [CCCallBlock actionWithBlock:^{
            [prompt removeFromParent];
        }];
        CCSequence *sequence = [CCSequence actions:ease,block,nil];
        [prompt runAction:sequence];
        */
        [prompt removeFromParent];
    }
}

-(void)showPrompt:(TutorialPromptType)type{
    [self hideCurrentPrompt];
    
    NSString *string = @"";
    CCSprite *sprite = nil;
    
    switch (type) {
        case TUTORIAL_PROMPT_TYPE_ACCELERATE:
        {
            string = NSLocalizedString(@"TUTORIAL_ACCELERATE",nil);
            sprite = [CCSprite spriteWithSpriteFrameName:@"tutorialPressHold.png"];
            
            CCSprite *halo = [CCSprite spriteWithSpriteFrameName:@"tutorialHalo.png"];
            [sprite addChild:halo z:0 tag:TAG_HALO];
            halo.position = ccp(sprite.contentSize.width*0.35f,sprite.contentSize.height*0.8f);
            halo.scale = 3.0f;
            halo.visible = NO;
            
            CCFadeIn *fadeIn = [CCFadeIn actionWithDuration:DURATION_HALO];
            CCScaleTo *scaleTo1 = [CCScaleTo actionWithDuration:DURATION_HALO scale:1.0f];
            CCSpawn *spawn1 = [CCSpawn actions:fadeIn,scaleTo1,nil];
            CCFadeOut *fadeTo = [CCFadeTo actionWithDuration:0.0f opacity:60];
            CCScaleTo *scaleTo2 = [CCScaleTo actionWithDuration:0.0f scale:halo.scale];
            CCSpawn *spawn2 = [CCSpawn actions:fadeTo,scaleTo2,nil];
            CCSequence *sequence = [CCSequence actions:spawn1,spawn2,nil];
            CCRepeatForever *repeat = [CCRepeatForever actionWithAction:sequence];
            [halo runAction:repeat];
        }
            break;
        case TUTORIAL_PROMPT_TYPE_BRAKE:
        {
            string = NSLocalizedString(@"TUTORIAL_BRAKE",nil);
            sprite = [CCSprite spriteWithSpriteFrameName:@"tutorialPressHold.png"];
            
            sprite.rotation = -30.0f;
            CCDelayTime *delay1 = [CCDelayTime actionWithDuration:DURATION_HALO];
            CCRotateTo *rotateTo1 = [CCRotateTo actionWithDuration:DURATION_HALO*0.5f angle:0.0f];
            CCFadeOut *fadeOut = [CCFadeOut actionWithDuration:DURATION_HALO*1.5f];
            CCDelayTime *delay2 = [CCDelayTime actionWithDuration:DURATION_HALO];
            CCRotateTo *rotateTo2 = [CCRotateTo actionWithDuration:0.0f angle:sprite.rotation];
            CCFadeIn *fadeIn = [CCFadeIn actionWithDuration:0.0f];
            CCSequence *sequence = [CCSequence actions:delay1,rotateTo1,fadeOut,delay2,rotateTo2,fadeIn,nil];
            CCRepeatForever *repeat = [CCRepeatForever actionWithAction:sequence];
            repeat.tag = TAG_ACTION_GESTURE;
            [sprite runAction:repeat];
        }
            break;
        case TUTORIAL_PROMPT_TYPE_JUMP:
        {
            string = NSLocalizedString(@"TUTORIAL_JUMP",nil);
            sprite = [CCSprite spriteWithSpriteFrameName:@"tutorialSwipeUp.png"];
            
            CCMoveBy *moveBy1 = [CCMoveBy actionWithDuration:0.0f position:ccp(0.0f,-sprite.contentSize.height)];
            CCFadeIn *fadeIn = [CCFadeIn actionWithDuration:0.0f];
            CCSpawn *spawn1 = [CCSpawn actions:moveBy1,fadeIn,nil];
            CCMoveBy *moveBy2 = [CCMoveBy actionWithDuration:DURATION_HALO*1.0f position:ccp(0.0f,sprite.contentSize.height)];
            CCFadeOut *fadeOut = [CCFadeOut actionWithDuration:DURATION_HALO*2.0f];
            CCSpawn *spawn2 = [CCSpawn actions:moveBy2,fadeOut,nil];
            CCEaseExponentialIn *ease = [CCEaseExponentialIn actionWithAction:spawn2];
            CCDelayTime *delay = [CCDelayTime actionWithDuration:DURATION_HALO];
            CCSequence *sequence = [CCSequence actions:spawn1,ease,delay,nil];
            CCRepeatForever *repeat = [CCRepeatForever actionWithAction:sequence];
            [sprite runAction:repeat];
        }
            break;
        case TUTORIAL_PROMPT_TYPE_DIVE:
        {
            string = NSLocalizedString(@"TUTORIAL_DIVE",nil);
            sprite = [CCSprite spriteWithSpriteFrameName:@"tutorialSwipeDown.png"];
            
            CCMoveBy *moveBy1 = [CCMoveBy actionWithDuration:DURATION_HALO*1.0f position:ccp(0.0f,-sprite.contentSize.height)];
            CCFadeOut *fadeOut = [CCFadeOut actionWithDuration:DURATION_HALO*2.0f];
            CCSpawn *spawn1 = [CCSpawn actions:moveBy1,fadeOut,nil];
            CCEaseExponentialIn *ease = [CCEaseExponentialIn actionWithAction:spawn1];
            CCDelayTime *delay = [CCDelayTime actionWithDuration:DURATION_HALO];
            CCMoveBy *moveBy2 = [CCMoveBy actionWithDuration:0.0f position:ccp(0.0f,sprite.contentSize.height)];
            CCFadeIn *fadeIn = [CCFadeIn actionWithDuration:0.0f];
            CCSpawn *spawn2 = [CCSpawn actions:moveBy2,fadeIn,nil];
            CCSequence *sequence = [CCSequence actions:ease,delay,spawn2,nil];
            CCRepeatForever *repeat = [CCRepeatForever actionWithAction:sequence];
            [sprite runAction:repeat];
        }
            break;
        case TUTORIAL_PROMPT_TYPE_SHOOT:
        {
            string = NSLocalizedString(@"TUTORIAL_SHOOT",nil);
        }
            break;
        case TUTORIAL_PROMPT_TYPE_OK:
        {
            string = NSLocalizedString(@"TUTORIAL_FINISH",nil);
            sprite = [CCSprite spriteWithSpriteFrameName:@"tutorialOk.png"];
        }
            break;
        default:
            break;
    }
    
    
    CCNode *prompt = [CCNode node];
    [self addChild:prompt z:0 tag:TAG_PROMPT_NODE];
    prompt.anchorPoint = ccp(0.5f,0.5f);
    
    TutorialPrompt *text = [TutorialPrompt promptWithString:string];
    [prompt addChild:text z:0 tag:TAG_PROMPT];
    prompt.contentSize = text.contentSize;
    
    if (sprite){
        [prompt addChild:sprite z:0 tag:TAG_GESTURE];
        prompt.contentSize = CGSizeMake(prompt.contentSize.width, prompt.contentSize.height + sprite.contentSize.height);
        sprite.position = ccp(prompt.contentSize.width*0.5f,sprite.contentSize.height*0.5f);
    }
    
    text.anchorPoint = ccp(0.5f,1.0f);
    text.position = ccp(prompt.contentSize.width*0.5f,prompt.contentSize.height);
    
    prompt.position = ccp(self.contentSize.width*0.5f,self.contentSize.height + prompt.contentSize.height*0.5f);
    CCMoveTo *moveTo = [CCMoveTo actionWithDuration:0.5f position:ccp(prompt.position.x,self.contentSize.height - prompt.contentSize.height*0.5f)];
    CCEaseBackOut *ease = [CCEaseBackOut actionWithAction:moveTo];
    [prompt runAction:ease];
}

-(void)update:(ccTime)dt{
    Character *character = [[self gameLayer]firstCharacter];
    
    switch (currentStep) {
        case 1:
        {
            CCSprite *gestureSprite = [self gesture];
            CCSprite *haloSprite = (CCSprite*)[gestureSprite getChildByTag:TAG_HALO];
            CCLabelBMFont *label = [self text];
            
            if ([[self gameLayer]isPressingRunningButtonForPlayer:1]){
                haloSprite.color = gestureSprite.color = label.color = COLOR_TUTORIAL_OK;
                haloSprite.visible = YES;
                if ([gestureSprite getActionByTag:TAG_ACTION_GESTURE]){
                    [gestureSprite stopActionByTag:TAG_ACTION_GESTURE];
                    gestureSprite.rotation = -30.0f;
                }
            }
            else{
                haloSprite.color = gestureSprite.color = label.color = ccWHITE;
                if ([gestureSprite getActionByTag:TAG_ACTION_GESTURE] == nil){
                    haloSprite.visible = NO;
                    
                    gestureSprite.rotation = 0.0f;
                    CCRotateTo *rotateTo1 = [CCRotateTo actionWithDuration:DURATION_HALO*0.5f angle:-30.0f];
                    CCCallBlock *block1 = [CCCallBlock actionWithBlock:^{
                        haloSprite.visible = YES;
                    }];
                    CCDelayTime *delay1 = [CCDelayTime actionWithDuration:DURATION_HALO*3.0f];
                    CCCallBlock *block2 = [CCCallBlock actionWithBlock:^{
                        haloSprite.visible = NO;
                    }];
                    CCRotateTo *rotateTo2 = [CCRotateTo actionWithDuration:DURATION_HALO*0.5f angle:0.0f];
                    CCDelayTime *delay2 = [CCDelayTime actionWithDuration:DURATION_HALO];
                    CCSequence *sequence = [CCSequence actions:rotateTo1,block1,delay1,block2,rotateTo2,delay2,nil];
                    CCRepeatForever *repeat = [CCRepeatForever actionWithAction:sequence];
                    repeat.tag = TAG_ACTION_GESTURE;
                    [gestureSprite runAction:repeat];
                }
            }
        }
            break;
        case 2:
        {
            CCSprite *gestureSprite = [self gesture];
            CCLabelBMFont *label = [self text];
            
            if ([[self gameLayer]isPressingRunningButtonForPlayer:1])
                gestureSprite.color = label.color = ccWHITE;
            else
                gestureSprite.color = label.color = COLOR_TUTORIAL_OK;
            
            
            static ccTime elapsed = 0.0f;
            if (![[self gameLayer]isPressingRunningButtonForPlayer:1])
                elapsed = 0.0f;
            else
                elapsed += dt;
            if (elapsed > 3.0f){
                elapsed = 0.0f;
                NSArray *touches = [[TouchTracker sharedTouchTracker]allTouches];
                if ([touches count] > 0){
                    UITouch *touch = [touches objectAtIndex:0];
                    CGPoint touchLocation = [self convertTouchToNodeSpace:touch];
                    
                    CCNode *labelNode = [CCNode node];
                    [self addChild:labelNode];
                    labelNode.position = ccp(touchLocation.x,touchLocation.y + label.contentSize.height);
                    labelNode.scale = 0.6f;
                    
                    CCLabelBMFont *label = [CCLabelBMFont labelWithString:NSLocalizedString(@"TUTORIAL_BRAKE", nil) fntFile:@"fontMedium.fnt"];
                    [labelNode addChild:label z:1];
                    label.color = COLOR_TUTORIAL_WARNING;
                    
                    CCLabelBMFont *shadow = [CCLabelBMFont labelWithString:NSLocalizedString(@"TUTORIAL_BRAKE", nil) fntFile:@"fontMedium.fnt"];
                    [labelNode addChild:shadow];
                    float offset = label.contentSize.height*0.02f;
                    shadow.position = ccp(offset,-offset);
                    shadow.color = ccBLACK;
                    
                    labelNode.position = ccp(fmaxf(label.contentSize.width*0.6f*labelNode.scale,fminf(self.contentSize.width-label.contentSize.width*0.6f*labelNode.scale,labelNode.position.x)), labelNode.position.y);
                    
                    CCMoveBy *moveBy = [CCMoveBy actionWithDuration:2.0f position:ccp(0.0f,label.contentSize.height)];
                    CCCallBlock *block = [CCCallBlock actionWithBlock:^{
                        [labelNode removeFromParent];
                    }];
                    CCSequence *sequence = [CCSequence actions:moveBy,block,nil];
                    [labelNode runAction:sequence];
                    
                    {
                        CCSprite *halo = [CCSprite spriteWithSpriteFrameName:@"tutorialHalo.png"];
                        [self addChild:halo z:0];
                        halo.position = touchLocation;
                        halo.color = COLOR_TUTORIAL_WARNING;
                        
                        CCFadeOut *fadeTo = [CCFadeTo actionWithDuration:DURATION_HALO opacity:0];
                        CCScaleTo *scaleTo = [CCScaleTo actionWithDuration:DURATION_HALO scale:8.0f];
                        CCCallBlock *block = [CCCallBlock actionWithBlock:^{
                            [halo removeFromParent];
                        }];
                        CCSpawn *spawn = [CCSpawn actions:fadeTo,scaleTo,nil];
                        CCSequence *sequence = [CCSequence actions:spawn,block,nil];
                        [halo runAction:sequence];
                    }
                }
            }
        }
            break;
        case 3:
            if ([[character state]isKindOfClass:[CharacterStateJumping class]]){
                [[SoundManager sharedManager]playEffect:SOUND_BONUS];
                [self nextStep];
            }
            break;
        case 4:
            if ([[character state]isKindOfClass:[CharacterStateDiving class]]){
                [[SoundManager sharedManager]playEffect:SOUND_BONUS];
                [self nextStep];
            }
            break;
        default:
            break;
    }
}

//
// BullseyeDelegate
//

-(void)didDestroyBullseye{
    [self nextStep];
}

//
// BuoyDelegate
//

-(void)didPickBuoy{
    [self nextStep];
}

@end
