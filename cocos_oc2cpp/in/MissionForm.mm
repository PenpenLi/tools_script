//
//  MissionForm.m
//  WaterBox2D
//
//  Created by Juan María Clavería Rosario on 11/04/14.
//  Copyright (c) 2014 Level App Studios. All rights reserved.
//

#import "MissionForm.h"
#import "CCBReader.h"
#import "GameState.h"
#import "SoundManager.h"
#import "ShopCoinsLayer.h"
#import "ShopSpecialCoinsLayer.h"
#import "NumberFormat.h"
#import "GameManager.h"
#import "ParticleManager.h"
#import "Sfx.h"
#import "Analytics.h"
#import "GameManager.h"
#import "CashNode.h"
#import "Achievement.h"

#pragma mark -
#pragma mark MissionSkipNode
#pragma mark -

#define TAG_SKIP_MISSION_SPRITE 3
#define TAG_SKIP_MISSION_LABEL 4
#define TAG_SKIP_MISSION_MENU 5
#define TAG_SKIP_MISSION_BUTTON 6
#define TAG_SKIP_MISSION_PRICE_LABEL 7
#define TAG_SKIP_MISSION_PRICE_SPRITE 8
#define TAG_SKIP_MISSION_TITLE 9
#define TAG_SKIP_MISSION_CLOSE_ITEM 10

@interface MissionSkipTouchLayer : CCLayer
@end

@protocol MissionSkipNodeDelegate <NSObject>
-(void)didBuyMission;
-(void)didClose;
@end

@interface MissionSkipNode : CCNode <CashNodeDelegate>{
    Mission *_mission;
    BOOL _bought;
}
@property (assign) id<MissionSkipNodeDelegate> delegate;
@end

@implementation MissionSkipNode

+(id)nodeWithMission:(Mission*)mission{
    MissionSkipNode *node = (MissionSkipNode*)[CCBReader nodeGraphFromFile:@"MissionSkip.ccbi"];
    [node loadMission:mission];
    return node;
}

-(void)dealloc{
    self.delegate = nil;
    [_mission release];
    [super dealloc];
}

-(void)didLoadFromCCB{
    _bought = NO;
    [self menu].touchPriority = kCCMenuHandlerPriority - 12;
    [self title].string = NSLocalizedString(@"MISSION_SKIP_TITLE", nil);
    [[self missionLabel]setWidth:[self missionLabel].contentSize.width];
    ((CCSprite*)[self closeItem].selectedImage).blendFunc = (ccBlendFunc){GL_SRC_ALPHA,GL_ONE};
    
    MissionSkipTouchLayer *layer = [MissionSkipTouchLayer node];
    [self addChild:layer z:-1];
    layer.touchEnabled = YES;
    layer.touchMode = kCCTouchesOneByOne;
    layer.touchSwallow = YES;
    layer.touchPriority = [self menu].touchPriority+1;
    
    // Alijo
    CCMenuItem *button = [self button];
    CashNode *cashNode = [CashNode specialCoinCashNode];
    [self addChild:cashNode];
    cashNode.position = ccp(button.position.x, button.position.y - button.contentSize.height);
    cashNode.delegate = self;
    [cashNode setTouchPriority:(int)layer.touchPriority-1];
}

-(void)loadMission:(Mission*)mission{
    _mission = [mission retain];
    
    [[self missionSprite]setDisplayFrame:[[CCSpriteFrameCache sharedSpriteFrameCache]spriteFrameByName:[mission icon]]];
    [self missionLabel].string = [mission text];
    
    CCLabelBMFont *priceLabel = [self priceLabel];
    CCSprite *priceSprite = [self priceSprite];
    
    priceLabel.string = [NumberFormat formatInteger:[mission price]];
    
    float totalWidth = priceSprite.contentSize.width*priceSprite.scale + priceLabel.contentSize.width*priceLabel.scale;
    priceSprite.position = ccpAdd([self button].position, ccp(priceSprite.contentSize.width*priceSprite.scale*0.5f - totalWidth*0.5f,0.0f));
    priceLabel.position = ccp(priceSprite.position.x + priceSprite.contentSize.width*0.5f*priceSprite.scale + priceLabel.contentSize.width*priceLabel.scale*0.5f,priceSprite.position.y);
}

-(CCSprite*)missionSprite{
    return (CCSprite*)[self getChildByTag:TAG_SKIP_MISSION_SPRITE];
}

-(CCLabelBMFont*)missionLabel{
    return (CCLabelBMFont*)[self getChildByTag:TAG_SKIP_MISSION_LABEL];
}

-(CCLabelBMFont*)priceLabel{
    return (CCLabelBMFont*)[self getChildByTag:TAG_SKIP_MISSION_PRICE_LABEL];
}

-(CCSprite*)priceSprite{
    return (CCSprite*)[self getChildByTag:TAG_SKIP_MISSION_PRICE_SPRITE];
}

-(CCLabelBMFont*)title{
    return (CCLabelBMFont*)[self getChildByTag:TAG_SKIP_MISSION_TITLE];
}

-(CCMenu*)menu{
    return (CCMenu*)[self getChildByTag:TAG_SKIP_MISSION_MENU];
}

-(CCMenuItem*)button{
    return (CCMenuItem*)[[self menu]getChildByTag:TAG_SKIP_MISSION_BUTTON];
}

-(CCMenuItemSprite*)closeItem{
    return (CCMenuItemSprite*)[[self menu]getChildByTag:TAG_SKIP_MISSION_CLOSE_ITEM];
}

-(void)didTapBuyItem{
    BOOL enoughMoney = [GameManager sharedManager].specialCoins >= [_mission price];
    
    if (enoughMoney){
        [[SoundManager sharedManager]playEffect:SOUND_BUTTON_BUY];
        [_mission setAccomplished:YES];
        [_mission setRewarded:YES];
        [GameManager sharedManager].specialCoins -= [_mission price];
        [GameManager sharedManager].specialCoinsSpent += [_mission price];
        [[GameManager sharedManager]save];
        _bought = YES;
        
        [[self missionSprite]setDisplayFrame:[[CCSpriteFrameCache sharedSpriteFrameCache]spriteFrameByName:[_mission icon]]];
        [self runMissionParticlesWithPosition:[self missionSprite].position];
        
        [[self menu]setEnabled:NO];
        CCDelayTime *delay = [CCDelayTime actionWithDuration:2.0f];
        CCCallBlock *block = [CCCallBlock actionWithBlock:^{
            if (self.delegate)
                [self.delegate didBuyMission];
            [self removeFromParent];
        }];
        CCSequence *sequence = [CCSequence actions:delay,block,nil];
        [self runAction:sequence];
        
        // Analíticas
        // Flurry
        {
            NSString *ident = [NSString stringWithFormat:@"Misión %d (%d)",[_mission order],[_mission ident]];
            AnalyticsEvent *event = [AnalyticsEvent eventWithName:@"Mission Skipped"];
            [event addParam:[AnalyticsParam paramStringWithString:ident key:@"Misión"]];
            [event addParam:[AnalyticsParam paramSelectedCharacter]];
            [event addParam:[AnalyticsParam paramSelectedVehicle]];
            [event addParam:[AnalyticsParam paramMilitarRange]];
            [event addParam:[AnalyticsParam paramIsFirstSession]];
            [Analytics logFlurryEvent:event];
        }
        
        // Game Analytics
        {
            NSString *ident = [NSString stringWithFormat:@"Misión %d (%d)",[_mission order],[_mission ident]];
            AnalyticsEvent *event = [AnalyticsEvent eventWithName:@"Mission"];
            [event addParam:[AnalyticsParam paramStringWithString:ident key:@"Skipped"]];
            [Analytics logGameAnalyticsDesignEvent:event];
            [Analytics logGameAnalyticsExpenseAndResumeEventsWithCoins:0 specialCoins:[_mission price] fromEvent:@"Skipped"];
        }
        
        // Misiones
        MilitarRange *range = [MilitarRange currentRange];
        NSArray *rangeMissions = [Mission missionsWithRange:[range type]];
        BOOL allAccomplished = YES;
        
        for (Mission *m in rangeMissions)
            allAccomplished &= [m isAccomplished];
        
        if (allAccomplished){
            // Analíticas
            // Flurry
            {
                AnalyticsEvent *event = [AnalyticsEvent eventWithName:@"Range completed"];
                [event addParam:[AnalyticsParam paramSelectedCharacter]];
                [event addParam:[AnalyticsParam paramSelectedVehicle]];
                [event addParam:[AnalyticsParam paramMilitarRange]];
                [event addParam:[AnalyticsParam paramIsFirstSession]];
                [Analytics logFlurryEvent:event];
            }
        }
        
        // Logros
        [Achievement didSkipMission];
    }
    else{
        ShopSpecialCoinsLayer *layer = [ShopSpecialCoinsLayer layerComesFrom:@"mission skip" isPlanned:NO];
        [self.parent addChild:layer z:100];
        [[SoundManager sharedManager]playEffect:SOUND_BUTTON_OPEN];
    }
}

-(void)didTapCloseItem{
    if (self.delegate){
        if (_bought)
            [self.delegate didBuyMission];
        else
            [self.delegate didClose];
    }
    [[SoundManager sharedManager]playEffect:SOUND_BUTTON_CLOSE];
    [self removeFromParent];
}

-(void)didTapCoinsCashNode:(CashNode*)node{
    ShopCoinsLayer *layer = [ShopCoinsLayer layerComesFrom:@"mission skip" isPlanned:NO];
    [self.parent addChild:layer z:100];
    [[SoundManager sharedManager]playEffect:SOUND_BUTTON_OPEN];
}

-(void)didTapSpecialCoinsCashNode:(CashNode*)node{
    ShopSpecialCoinsLayer *layer = [ShopSpecialCoinsLayer layerComesFrom:@"mission skip" isPlanned:NO];
    [self.parent addChild:layer z:100];
    [[SoundManager sharedManager]playEffect:SOUND_BUTTON_OPEN];
}

-(void)runMissionParticlesWithPosition:(CGPoint)position{
    {
        CCParticleSystem *particles = [[ParticleManager sharedManager]particlesWithFile:PARTICLES_BIG_SMOKE];
        [self addChild:particles z:10];
        particles.position = position;
        particles.autoRemoveOnFinish = YES;
    }
    
    {
        ccBlendFunc blend1 = (ccBlendFunc){GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA};
        ccBlendFunc blend2 = (ccBlendFunc){GL_SRC_ALPHA,GL_ONE};
        CCParticleSystem *particles = [[ParticleManager sharedManager]doubleBlendParticlesWithFile:PARTICLES_BIG_EXPLOSION blend1:blend1 blend2:blend2];
        [self addChild:particles z:10];
        particles.position = position;
        particles.autoRemoveOnFinish = YES;
    }
    
    {
        Sfx *sfx = [Sfx bigHaloExplosion];
        [self addChild:sfx z:10];
        sfx.position = position;
    }
    
    {
        Sfx *sfx = [Sfx ringExplosion];
        [self addChild:sfx z:10];
        sfx.position = position;
    }
    
    {
        CCParticleSystem *particles = [[ParticleManager sharedManager]particlesWithFile:PARTICLES_MELT_UPDATE_RANGE];
        [self addChild:particles z:10];
        particles.position = position;
        particles.autoRemoveOnFinish = YES;
        particles.posVar = ccpMult(particles.posVar, 0.2f);
        particles.life *= 0.5f;
        particles.lifeVar *= 0.5f;
        particles.startSize *= 0.2f;
        particles.startSizeVar *= 0.2f;
        particles.endSize *= 0.2f;
        particles.endSizeVar *= 0.2f;
        particles.speed *= 0.5f;
        particles.speedVar *= 0.5f;
        particles.blendAdditive = YES;
    }
}

@end

@implementation MissionSkipTouchLayer
-(BOOL)ccTouchBegan:(UITouch *)touch withEvent:(UIEvent *)event{
    [((MissionSkipNode*)self.parent)didTapCloseItem];
    return YES;
}
@end



#pragma mark -
#pragma mark MissionForm
#pragma mark -

#define TAG_LABEL_TITLE 1
#define TAG_SPRITE_MISSION_1 2
#define TAG_SPRITE_MISSION_2 3
#define TAG_SPRITE_MISSION_3 4
#define TAG_SPRITE_MISSION_4 5
#define TAG_LABEL_MISSION_1 6
#define TAG_LABEL_MISSION_2 7
#define TAG_LABEL_MISSION_3 8
#define TAG_LABEL_MISSION_4 9
#define TAG_LABEL_LEVEL 10
#define TAG_SPRITE_RANGE 11
#define TAG_MENU 12
#define TAG_MENU_ITEM_SKIP_1 13
#define TAG_MENU_ITEM_SKIP_2 14
#define TAG_MENU_ITEM_SKIP_3 15
#define TAG_MENU_ITEM_SKIP_4 16
#define TAG_SKIP_LABEL_1 17
#define TAG_SKIP_LABEL_2 18
#define TAG_SKIP_LABEL_3 19
#define TAG_SKIP_LABEL_4 20
#define TAG_BACKGROUND 21
#define TAG_LABEL_NO_MORE_MISSIONS 22
#define TAG_SPRITE_LINE_1 23
#define TAG_SPRITE_LINE_2 24
#define TAG_SPRITE_LINE_3 25
#define TAG_LABEL_CONTENT 26

@interface MissionForm () <MissionSkipNodeDelegate>
@end

@implementation MissionForm{
    NSArray *_missions;
    float _missionLabelWidth;
    float _maxLabelScale;
    BOOL _skip;
}

+(id)formWithSkipButton:(BOOL)skip{
    MissionForm *form = (MissionForm*)[CCBReader nodeGraphFromFile:@"Missions.ccbi"];
    [form loadWithSkip:skip];
    return form;
}

+(id)formWithSkipButton:(BOOL)skip width:(float)width{
    MissionForm *node = (MissionForm*)[CCBReader nodeGraphFromFile:@"Missions.ccbi"];
    
    float originalWidth = node.contentSize.width;
    node.contentSize = CGSizeMake(width, node.contentSize.height);
    
    float childScale = width / originalWidth;
    [node setMissionLabelWidth:[node missionLabelWidth]*childScale];
    
    [node background].scaleX *= childScale;
    [node menu].contentSize = CGSizeMake([node menu].contentSize.width * childScale, [node menu].contentSize.height);
    
    for (CCNode *child in [node children])
        child.position = ccp(child.position.x * childScale, child.position.y);
    
    for (CCNode *child in [[node menu]children])
        child.position = ccp(child.position.x * childScale, child.position.y);
    
    [node loadWithSkip:skip];
    return node;
}

-(void)dealloc{
    [_missions release];
    [super dealloc];
}

-(CCLabelBMFont*)title{
    return (CCLabelBMFont*)[self getChildByTag:TAG_LABEL_TITLE];
}

-(CCSprite*)mission1Sprite{
    return (CCSprite*)[self getChildByTag:TAG_SPRITE_MISSION_1];
}

-(CCSprite*)mission2Sprite{
    return (CCSprite*)[self getChildByTag:TAG_SPRITE_MISSION_2];
}

-(CCSprite*)mission3Sprite{
    return (CCSprite*)[self getChildByTag:TAG_SPRITE_MISSION_3];
}

-(CCSprite*)mission4Sprite{
    return (CCSprite*)[self getChildByTag:TAG_SPRITE_MISSION_4];
}

-(CCLabelBMFont*)mission1Label{
    return (CCLabelBMFont*)[self getChildByTag:TAG_LABEL_MISSION_1];
}

-(CCLabelBMFont*)mission2Label{
    return (CCLabelBMFont*)[self getChildByTag:TAG_LABEL_MISSION_2];
}

-(CCLabelBMFont*)mission3Label{
    return (CCLabelBMFont*)[self getChildByTag:TAG_LABEL_MISSION_3];
}

-(CCLabelBMFont*)mission4Label{
    return (CCLabelBMFont*)[self getChildByTag:TAG_LABEL_MISSION_4];
}

-(CCLabelBMFont*)levelLabel{
    return (CCLabelBMFont*)[self getChildByTag:TAG_LABEL_LEVEL];
}

-(CCSprite*)rangeSprite{
    return (CCSprite*)[self getChildByTag:TAG_SPRITE_RANGE];
}

-(CCSprite*)background{
    return (CCSprite*)[self getChildByTag:TAG_BACKGROUND];
}

-(CCMenu*)menu{
    return (CCMenu*)[self getChildByTag:TAG_MENU];
}

-(CCMenuItem*)skipItem1{
    return (CCMenuItem*)[[self menu]getChildByTag:TAG_MENU_ITEM_SKIP_1];
}

-(CCMenuItem*)skipItem2{
    return (CCMenuItem*)[[self menu]getChildByTag:TAG_MENU_ITEM_SKIP_2];
}

-(CCMenuItem*)skipItem3{
    return (CCMenuItem*)[[self menu]getChildByTag:TAG_MENU_ITEM_SKIP_3];
}

-(CCMenuItem*)skipItem4{
    return (CCMenuItem*)[[self menu]getChildByTag:TAG_MENU_ITEM_SKIP_4];
}

-(CCLabelBMFont*)skipLabel1{
    return (CCLabelBMFont*)[self getChildByTag:TAG_SKIP_LABEL_1];
}

-(CCLabelBMFont*)skipLabel2{
    return (CCLabelBMFont*)[self getChildByTag:TAG_SKIP_LABEL_2];
}

-(CCLabelBMFont*)skipLabel3{
    return (CCLabelBMFont*)[self getChildByTag:TAG_SKIP_LABEL_3];
}

-(CCLabelBMFont*)skipLabel4{
    return (CCLabelBMFont*)[self getChildByTag:TAG_SKIP_LABEL_4];
}

-(CCSprite*)line1Sprite{
    return (CCSprite*)[self getChildByTag:TAG_SPRITE_LINE_1];
}

-(CCSprite*)line2Sprite{
    return (CCSprite*)[self getChildByTag:TAG_SPRITE_LINE_2];
}

-(CCSprite*)line3Sprite{
    return (CCSprite*)[self getChildByTag:TAG_SPRITE_LINE_3];
}

-(CCNode*)labelContentNode{
    return [self getChildByTag:TAG_LABEL_CONTENT];
}

-(float)missionLabelWidth{
    return _missionLabelWidth;
}

-(void)setMissionLabelWidth:(float)width{
    _missionLabelWidth = width;
}

-(void)didLoadFromCCB{
    [self menu].touchPriority = kCCMenuHandlerPriority - 11;
    _missionLabelWidth = 140; //[self labelContentNode].contentSize.width/[self mission1Label].scale;
    _maxLabelScale = [self mission1Label].scale;
}

-(void)loadWithSkip:(BOOL)skip{
    _skip = skip;
    
    CCSpriteFrameCache *cache = [CCSpriteFrameCache sharedSpriteFrameCache];
    
    [self title].string = NSLocalizedString(@"MISSION_TITLE", nil);
    
    MilitarRange *range = [MilitarRange currentRange];
    [self levelLabel].string = [NSString stringWithFormat:NSLocalizedString(@"MISSION_LEVEL",nil),[range level]];
    [[self rangeSprite]setDisplayFrame:[cache spriteFrameByName:[range spriteNameMini]]];
    
    [self skipLabel1].string =
        [self skipLabel2].string =
        [self skipLabel3].string =
        [self skipLabel4].string = NSLocalizedString(@"MISSION_SKIP_SHORT", nil);
    CGSize skipLabelSize = [self skipLabel1].contentSize;
    CGSize skipItemSize = [self skipItem1].contentSize;
    float skipLabelScale = fminf([self skipLabel1].scale, skipItemSize.width*0.9f / skipLabelSize.width);
    [self skipLabel1].scale =
        [self skipLabel2].scale =
        [self skipLabel3].scale =
        [self skipLabel4].scale = skipLabelScale;
    _missions = [[Mission missionsWithRange:[range type]]retain];
    
    [self reloadMissions];
}

-(void)reloadMissions{
    [self removeChildByTag:TAG_LABEL_NO_MORE_MISSIONS];
    CCSpriteFrameCache *cache = [CCSpriteFrameCache sharedSpriteFrameCache];
    
    if (_missions && [_missions count] == 4){
        Mission *mission1 = [_missions objectAtIndex:0];
        Mission *mission2 = [_missions objectAtIndex:1];
        Mission *mission3 = [_missions objectAtIndex:2];
        Mission *mission4 = [_missions objectAtIndex:3];
        
        [[self mission1Sprite]setDisplayFrame:[cache spriteFrameByName:[mission1 icon]]];
        [[self mission2Sprite]setDisplayFrame:[cache spriteFrameByName:[mission2 icon]]];
        [[self mission3Sprite]setDisplayFrame:[cache spriteFrameByName:[mission3 icon]]];
        [[self mission4Sprite]setDisplayFrame:[cache spriteFrameByName:[mission4 icon]]];
        
        CCLabelBMFont *mission1Label = [self mission1Label];
        CCLabelBMFont *mission2Label = [self mission2Label];
        CCLabelBMFont *mission3Label = [self mission3Label];
        CCLabelBMFont *mission4Label = [self mission4Label];
        mission1Label.string = [mission1 text];
        mission2Label.string = [mission2 text];
        mission3Label.string = [mission3 text];
        mission4Label.string = [mission4 text];

        _missionLabelWidth = [self labelContentNode].contentSize.width/mission1Label.scale;
        if (_skip) {
            _missionLabelWidth -= [self skipItem1].contentSize.width;
        }
        [mission1Label setWidth:_missionLabelWidth];
        [mission2Label setWidth:_missionLabelWidth];
        [mission3Label setWidth:_missionLabelWidth];
        [mission4Label setWidth:_missionLabelWidth];
//        CCNode *labelContentNode = [self labelContentNode];
//        
//        mission1Label.scale = fminf(_maxLabelScale, labelContentNode.contentSize.height/mission1Label.contentSize.height);
//        mission2Label.scale = fminf(_maxLabelScale, labelContentNode.contentSize.height/mission2Label.contentSize.height);
//        mission3Label.scale = fminf(_maxLabelScale, labelContentNode.contentSize.height/mission3Label.contentSize.height);
//        mission4Label.scale = fminf(_maxLabelScale, labelContentNode.contentSize.height/mission4Label.contentSize.height);
//        
//        [mission1Label setWidth:_missionLabelWidth*(_maxLabelScale/mission1Label.scale)];
//        [mission2Label setWidth:_missionLabelWidth*(_maxLabelScale/mission2Label.scale)];
//        [mission3Label setWidth:_missionLabelWidth*(_maxLabelScale/mission3Label.scale)];
//        [mission4Label setWidth:_missionLabelWidth*(_maxLabelScale/mission4Label.scale)];
        
        if (_skip){
            [self skipItem1].visible = [self skipItem1].isEnabled = [self skipLabel1].visible = ![mission1 isAccomplished];
            [self skipItem2].visible = [self skipItem2].isEnabled = [self skipLabel2].visible = ![mission2 isAccomplished];
            [self skipItem3].visible = [self skipItem3].isEnabled = [self skipLabel3].visible = ![mission3 isAccomplished];
            [self skipItem4].visible = [self skipItem4].isEnabled = [self skipLabel4].visible = ![mission4 isAccomplished];
        }
        else{
            [self menu].visible = NO;
            [self skipLabel1].visible = NO;
            [self skipLabel2].visible = NO;
            [self skipLabel3].visible = NO;
            [self skipLabel4].visible = NO;
        }
    }
    else{
        [self menu].visible = NO;
        [self skipLabel1].visible = NO;
        [self skipLabel2].visible = NO;
        [self skipLabel3].visible = NO;
        [self skipLabel4].visible = NO;
        [self mission1Sprite].visible = NO;
        [self mission2Sprite].visible = NO;
        [self mission3Sprite].visible = NO;
        [self mission4Sprite].visible = NO;
        [self mission1Label].visible = NO;
        [self mission2Label].visible = NO;
        [self mission3Label].visible = NO;
        [self mission4Label].visible = NO;
        [self line1Sprite].visible = NO;
        [self line2Sprite].visible = NO;
        [self line3Sprite].visible = NO;
        
        CCLabelBMFont *label = [CCLabelBMFont labelWithString:NSLocalizedString(@"MISSION_NO_MORE", nil) fntFile:@"fontSmall.fnt"];
        [self addChild:label z:1 tag:TAG_LABEL_NO_MORE_MISSIONS];
        label.position = ccp(self.contentSize.width*0.5f,self.contentSize.height*0.5f);
    }
}

-(void)didTapSkipItem:(CCMenuItem*)item{
    Mission *mission = nil;
    switch (item.tag) {
        case TAG_MENU_ITEM_SKIP_1:
            mission = [_missions objectAtIndex:0];
            break;
        case TAG_MENU_ITEM_SKIP_2:
            mission = [_missions objectAtIndex:1];
            break;
        case TAG_MENU_ITEM_SKIP_3:
            mission = [_missions objectAtIndex:2];
            break;
        case TAG_MENU_ITEM_SKIP_4:
            mission = [_missions objectAtIndex:3];
            break;
        default:
            break;
    }
    if (mission){
        [[SoundManager sharedManager]playEffect:SOUND_BUTTON_OPEN];
        MissionSkipNode *node = [MissionSkipNode nodeWithMission:mission];
        node.position = self.position;
        node.delegate = self;
        [self.parent addChild:node z:self.zOrder];
        self.visible = NO;
    }
}

-(void)showLevel:(BOOL)show{
    [self rangeSprite].visible = show;
    [self levelLabel].visible = show;
}

-(void)showTitle:(BOOL)show{
    [self title].visible = show;
}

//
// MissionSkipNodeDelegate
//

-(void)didBuyMission{
    [self reloadMissions];
    self.visible = YES;
}

-(void)didClose{
    self.visible = YES;
}

@end