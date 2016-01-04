//
//  Sfx.m
//  WaterBox2D
//
//  Created by Juan María Clavería Rosario on 28/10/13.
//  Copyright (c) 2013 Level App Studios. All rights reserved.
//

#import "Sfx.h"
#import "CCNode+Animation.h"
#import "GameScene.h"

#pragma mark -
#pragma mark Sfx
#pragma mark -

@interface Sfx ()
@property (copy,nonatomic) NSString *poolName;
@property (retain,nonatomic) CCAction *action;
@property float initialScale;
@property float initialRotation;
@property GLubyte initialOpacity;
@end

@implementation Sfx

-(id)initWithSpriteFrame:(CCSpriteFrame*)spriteFrame{
    self = [super initWithSpriteFrame:spriteFrame];
    if (self){
        self.action = nil;
        self.initialScale = 1.0f;
        self.initialRotation = 0.0f;
        self.initialOpacity = 255;
    }
    return self;
}

-(void)cleanup{
    [super cleanup];
    self.action = nil;
}

-(void)dealloc{
    self.poolName = nil;
    self.action = nil;
    [super dealloc];
}

+(void)preloadPool{
    NSMutableArray *preloaded = [NSMutableArray arrayWithCapacity:10];
    
    for (int i = 0; i < 8; i++) [preloaded addObject:[self pickHalo]];
    for (int i = 0; i < 2; i++) [preloaded addObject:[self smallHalo]];
    for (int i = 0; i < 2; i++) [preloaded addObject:[self bigHalo]];
    for (int i = 0; i < 2; i++) [preloaded addObject:[self smallHaloExplosion]];
    for (int i = 0; i < 2; i++) [preloaded addObject:[self hugeHaloExplosion]];
    for (int i = 0; i < 2; i++) [preloaded addObject:[self bigHaloExplosion]];
    for (int i = 0; i < 2; i++) [preloaded addObject:[self ringExplosion]];
    for (int i = 0; i < 2; i++) [preloaded addObject:[self hugeRingExplosion]];
    for (int i = 0; i < 8; i++) [preloaded addObject:[self gunHit]];
    for (int i = 0; i < 5; i++) [preloaded addObject:[self smallWaterCircle]];
    for (int i = 0; i < 5; i++) [preloaded addObject:[self bigWaterCircle]];
    for (int i = 0; i < 2; i++) [preloaded addObject:[self burstStar]];
    for (int i = 0; i < 2; i++) [preloaded addObject:[self burstBig]];
    for (int i = 0; i < 2; i++) [preloaded addObject:[self burstLaser]];
    for (int i = 0; i < 2; i++) [preloaded addObject:[self burstRedLaser]];
    for (int i = 0; i < 2; i++) [preloaded addObject:[self burstPlasma]];
    for (int i = 0; i < 2; i++) [preloaded addObject:[self burstHuge]];
    for (int i = 0; i < 10; i++) [preloaded addObject:[self ammunition]];
    
    for (Sfx *obj in preloaded)
        [ObjectPool putInPool:obj poolName:obj.poolName];
    
    [preloaded removeAllObjects];
}

+(Sfx*)haloWithScale:(float)scale{
    NSString *name = [NSString stringWithFormat:@"haloWithScale%.2f",scale];
    if (![ObjectPool hasObject:name]){
        // Init
        Sfx *sprite = [Sfx spriteWithSpriteFrameName:@"sfxHalo.png"];
        sprite.poolName = name;
        sprite.blendFunc = (ccBlendFunc){GL_SRC_ALPHA,GL_ONE};
        sprite.initialScale = scale;
        
        // Action
        CCFadeOut *fadeOut = [CCFadeOut actionWithDuration:0.25f];
        CCCallBlock *block = [CCCallBlock actionWithBlock:^{
            [sprite removeFromParentAndCleanup:NO];
            [ObjectPool putInPool:sprite poolName:name];
        }];
        CCSequence *sequence = [CCSequence actions:fadeOut,block,nil];
        sprite.action = sequence;
        [ObjectPool putInPool:sprite poolName:name];
    }
    return (Sfx*)[ObjectPool getFromPool:name];
}

+(Sfx*)haloExplosionWithScale:(float)scale{
    NSString *name = [NSString stringWithFormat:@"haloExplosionWithScale%.2f",scale];
    if (![ObjectPool hasObject:name]){
        // Init
        Sfx *sprite = [Sfx spriteWithSpriteFrameName:@"sfxHaloExplosion.png"];
        sprite.poolName = name;
        sprite.blendFunc = (ccBlendFunc){GL_SRC_ALPHA,GL_ONE};
        sprite.initialScale = scale;
        
        // Action
        CCFadeOut *fadeOut = [CCFadeOut actionWithDuration:0.15f];
        CCCallBlock *block = [CCCallBlock actionWithBlock:^{
            [sprite removeFromParentAndCleanup:NO];
            [ObjectPool putInPool:sprite poolName:name];
        }];
        CCSequence *sequence = [CCSequence actions:fadeOut,block,nil];
        sprite.action = sequence;
        [ObjectPool putInPool:sprite poolName:name];
    }
    return (Sfx*)[ObjectPool getFromPool:name];
}

+(Sfx*)ringExplosionWithScale:(float)scale{
    NSString *name = [NSString stringWithFormat:@"ringExplosionWithScale%.2f",scale];
    if (![ObjectPool hasObject:name]){
        // Init
        Sfx *sprite = [Sfx spriteWithSpriteFrameName:@"sfxSmallExplosion02.png"];
        sprite.poolName = name;
        sprite.blendFunc = (ccBlendFunc){GL_SRC_ALPHA,GL_ONE};
        sprite.initialRotation = arc4random()%360;
        
        // Action
        CCFadeOut *fadeOut = [CCFadeOut actionWithDuration:0.4f];
        CCScaleBy *scaleBy = [CCScaleBy actionWithDuration:0.4f scale:scale];
        CCSpawn *spawn = [CCSpawn actions:fadeOut,scaleBy,nil];
        CCCallBlock *block = [CCCallBlock actionWithBlock:^{
            [sprite removeFromParentAndCleanup:NO];
            [ObjectPool putInPool:sprite poolName:name];
        }];
        CCSequence *sequence = [CCSequence actions:spawn,block,nil];
        sprite.action = sequence;
        [ObjectPool putInPool:sprite poolName:name];
    }
    return (Sfx*)[ObjectPool getFromPool:name];
}

+(Sfx*)ringExplosion{
    return [self ringExplosionWithScale:10.0f];
}

+(Sfx*)hugeRingExplosion{
    return [self ringExplosionWithScale:60.0f];
}

+(Sfx*)pickHalo{    
    NSString *name = @"pickHalo";
    if (![ObjectPool hasObject:name]){
        // Init
        Sfx *sprite = [Sfx spriteWithSpriteFrameName:@"sfxPickHalo.png"];
        sprite.poolName = name;
        sprite.blendFunc = (ccBlendFunc){GL_SRC_ALPHA,GL_ONE};
        sprite.initialScale = 2.0f;
        
        // Action
        CCFadeOut *fadeOut = [CCFadeOut actionWithDuration:0.25f];
        CCScaleBy *scaleBy = [CCScaleBy actionWithDuration:0.25f scale:3.0f];
        CCSpawn *spawn = [CCSpawn actions:fadeOut,scaleBy,nil];
        CCCallBlock *block = [CCCallBlock actionWithBlock:^{
            [sprite removeFromParentAndCleanup:NO];
            [ObjectPool putInPool:sprite poolName:name];
        }];
        CCSequence *sequence = [CCSequence actions:spawn,block,nil];
        sprite.action = sequence;
        [ObjectPool putInPool:sprite poolName:name];
    }
    return (Sfx*)[ObjectPool getFromPool:name];
}

+(Sfx*)smallHalo{
    return [self haloWithScale:3.0f];
}

+(Sfx*)smallHaloExplosion{
    return [self haloExplosionWithScale:3.0f];
}

+(Sfx*)bigHalo{
    return [self haloWithScale:7.0f];
}

+(Sfx*)hugeHalo{
    return [self haloWithScale:14.0f];
}

+(Sfx*)hugeHaloExplosion{
    return [self haloExplosionWithScale:14.0f];
}

+(Sfx*)bigHaloExplosion{
    return [self haloExplosionWithScale:7.0f];
}

+(Sfx*)gunHit{
    NSString *name = @"gunHit";
    if (![ObjectPool hasObject:name]){
        // Init
        Sfx *sprite = [Sfx spriteWithSpriteFrameName:@"sfxSmallExplosion01.png"];
        sprite.poolName = name;
        
        // Action
        NSArray *frames = [CCNode spriteFramesWithSequence:@"sfxSmallExplosion%.2d.png" numFrames:5];
        CCAnimation *animation = [CCAnimation animationWithSpriteFrames:frames delay:0.05f];
        CCAnimate *animate = [CCNode actionAnimation:animation];
        CCCallBlock *block = [CCCallBlock actionWithBlock:^{
            [sprite removeFromParentAndCleanup:NO];
            [ObjectPool putInPool:sprite poolName:name];
        }];
        CCSequence *sequence = [CCSequence actions:animate,block,nil];
        sprite.action = sequence;
        [ObjectPool putInPool:sprite poolName:name];
    }
    return (Sfx*)[ObjectPool getFromPool:name];
}

+(Sfx*)waterCircleWithScale:(float)scale time:(ccTime)time{
    NSString *name = [NSString stringWithFormat:@"waterCircleWithScale%.2f",scale];
    if (![ObjectPool hasObject:name]){
        // Init
        Sfx *sprite = [Sfx spriteWithSpriteFrameName:@"sfxWaterCircle.png"];
        sprite.poolName = name;
        sprite.blendFunc = (ccBlendFunc){GL_SRC_ALPHA,GL_ONE};
        sprite.anchorPoint = ccp(0.5f,0.8f);
        sprite.initialScale = scale*0.25f;
        
        // Action
        CCFadeOut *fadeOut = [CCFadeOut actionWithDuration:time];
        CCScaleBy *scaleTo = [CCScaleTo actionWithDuration:time scale:scale];
        CCSpawn *spawn = [CCSpawn actions:fadeOut,scaleTo,nil];
        CCCallBlock *block = [CCCallBlock actionWithBlock:^{
            [sprite removeFromParentAndCleanup:NO];
            [ObjectPool putInPool:sprite poolName:name];
        }];
        CCSequence *sequence = [CCSequence actions:spawn,block,nil];
        sprite.action = sequence;
        [ObjectPool putInPool:sprite poolName:name];
    }
    return (Sfx*)[ObjectPool getFromPool:name];
}

+(Sfx*)smallWaterCircle{
    return [self waterCircleWithScale:2.5f time:0.5f];
}

+(Sfx*)bigWaterCircle{
    return [self waterCircleWithScale:4.0f time:0.75f];
}

+(Sfx*)burstStar{
    NSString *name = @"bustStar";
    if (![ObjectPool hasObject:name]){
        // Init
        Sfx *sprite = [Sfx spriteWithSpriteFrameName:@"sfxBurstStar.png"];
        sprite.poolName = name;
        sprite.blendFunc = (ccBlendFunc){GL_SRC_ALPHA,GL_ONE};
        sprite.initialScale = 2.0f;
        
        // Action
        CCFadeOut *fadeOut = [CCFadeOut actionWithDuration:0.15f];
        CCCallBlock *block = [CCCallBlock actionWithBlock:^{
            [sprite removeFromParentAndCleanup:NO];
            [ObjectPool putInPool:sprite poolName:name];
        }];
        CCSequence *sequence = [CCSequence actions:fadeOut,block,nil];
        sprite.action = sequence;
        [ObjectPool putInPool:sprite poolName:name];
    }
    return (Sfx*)[ObjectPool getFromPool:name];
}

+(Sfx*)burstLaser{
    NSString *name = @"burstLaser";
    if (![ObjectPool hasObject:name]){
        // Init
        Sfx *sprite = [Sfx spriteWithSpriteFrameName:@"sfxBurstLaser.png"];
        sprite.poolName = name;
        sprite.blendFunc = (ccBlendFunc){GL_SRC_ALPHA,GL_ONE};
        sprite.initialScale = 2.0f;
        
        // Action
        CCFadeOut *fadeOut = [CCFadeOut actionWithDuration:0.15f];
        CCCallBlock *block = [CCCallBlock actionWithBlock:^{
            [sprite removeFromParentAndCleanup:NO];
            [ObjectPool putInPool:sprite poolName:name];
        }];
        CCSequence *sequence = [CCSequence actions:fadeOut,block,nil];
        sprite.action = sequence;
        [ObjectPool putInPool:sprite poolName:name];
    }
    return (Sfx*)[ObjectPool getFromPool:name];
}

+(Sfx*)burstRedLaser{
    NSString *name = @"burstRedLaser";
    if (![ObjectPool hasObject:name]){
        // Init
        Sfx *sprite = [Sfx spriteWithSpriteFrameName:@"sfxBurstRedLaser.png"];
        sprite.poolName = name;
        sprite.blendFunc = (ccBlendFunc){GL_SRC_ALPHA,GL_ONE};
        sprite.initialScale = 2.0f;
        
        // Action
        CCFadeOut *fadeOut = [CCFadeOut actionWithDuration:0.15f];
        CCCallBlock *block = [CCCallBlock actionWithBlock:^{
            [sprite removeFromParentAndCleanup:NO];
            [ObjectPool putInPool:sprite poolName:name];
        }];
        CCSequence *sequence = [CCSequence actions:fadeOut,block,nil];
        sprite.action = sequence;
        [ObjectPool putInPool:sprite poolName:name];
    }
    return (Sfx*)[ObjectPool getFromPool:name];
}

+(Sfx*)burstPlasma{
    NSString *name = @"bustPlasma";
    if (![ObjectPool hasObject:name]){
        // Init
        Sfx *sprite = [Sfx spriteWithSpriteFrameName:@"sfxBurstPlasma.png"];
        sprite.poolName = name;
        sprite.blendFunc = (ccBlendFunc){GL_SRC_ALPHA,GL_ONE};
        sprite.initialScale = 2.0f;
        
        // Action
        CCFadeOut *fadeOut = [CCFadeOut actionWithDuration:0.15f];
        CCCallBlock *block = [CCCallBlock actionWithBlock:^{
            [sprite removeFromParentAndCleanup:NO];
            [ObjectPool putInPool:sprite poolName:name];
        }];
        CCSequence *sequence = [CCSequence actions:fadeOut,block,nil];
        sprite.action = sequence;
        [ObjectPool putInPool:sprite poolName:name];
    }
    return (Sfx*)[ObjectPool getFromPool:name];
}

+(Sfx*)burstBig{
    NSString *name = @"bustBig";
    if (![ObjectPool hasObject:name]){
        // Init
        Sfx *sprite = [Sfx spriteWithSpriteFrameName:@"sfxBurstBig.png"];
        sprite.poolName = name;
        sprite.blendFunc = (ccBlendFunc){GL_SRC_ALPHA,GL_ONE};
        sprite.initialScale = 2.0f;
        
        // Action
        CCFadeOut *fadeOut = [CCFadeOut actionWithDuration:0.15f];
        CCCallBlock *block = [CCCallBlock actionWithBlock:^{
            [sprite removeFromParentAndCleanup:NO];
            [ObjectPool putInPool:sprite poolName:name];
        }];
        CCSequence *sequence = [CCSequence actions:fadeOut,block,nil];
        sprite.action = sequence;
        [ObjectPool putInPool:sprite poolName:name];
    }
    return (Sfx*)[ObjectPool getFromPool:name];
}

+(Sfx*)burstHuge{
    NSString *name = @"bustHuge";
    if (![ObjectPool hasObject:name]){
        // Init
        Sfx *sprite = [Sfx spriteWithSpriteFrameName:@"sfxBurstBig.png"];
        sprite.poolName = name;
        sprite.blendFunc = (ccBlendFunc){GL_SRC_ALPHA,GL_ONE};
        sprite.initialScale = 4.0f;
        
        // Action
        CCFadeOut *fadeOut = [CCFadeOut actionWithDuration:0.15f];
        CCCallBlock *block = [CCCallBlock actionWithBlock:^{
            [sprite removeFromParentAndCleanup:NO];
            [ObjectPool putInPool:sprite poolName:name];
        }];
        CCSequence *sequence = [CCSequence actions:fadeOut,block,nil];
        sprite.action = sequence;
        [ObjectPool putInPool:sprite poolName:name];
    }
    return (Sfx*)[ObjectPool getFromPool:name];
}

+(Sfx*)ammunition{
    NSString *name = @"ammunition";
    if (![ObjectPool hasObject:name]){
        // Init
        Sfx *sprite = [Sfx spriteWithSpriteFrameName:@"sfxAmmunition.png"];
        sprite.poolName = name;
        
        // Action
        int maxMov = (int)([[CCDirector sharedDirector]winSize].height * 0.15f);
        CCJumpBy *jumpBy = [CCJumpBy actionWithDuration:1.0f
                                               position:ccp((arc4random()%maxMov)-(maxMov*0.5f),0.0f)
                                                 height:maxMov
                                                  jumps:1];
        CCRotateBy *rotateBy = [CCRotateBy actionWithDuration:1.0f angle:-180.0f + (arc4random()%360)];
        CCDelayTime *delay = [CCDelayTime actionWithDuration:0.7f];
        CCFadeOut *fadeOut = [CCFadeOut actionWithDuration:0.3f];
        CCSequence *sequence1 = [CCSequence actions:delay,fadeOut,nil];
        CCSpawn *spawn = [CCSpawn actions:jumpBy,rotateBy,sequence1,nil];
        CCCallBlock *block = [CCCallBlock actionWithBlock:^{
            [sprite removeFromParentAndCleanup:NO];
            [ObjectPool putInPool:sprite poolName:name];
        }];
        CCSequence *sequence2 = [CCSequence actions:spawn,block,nil];
        sprite.action = sequence2;
        [ObjectPool putInPool:sprite poolName:name];
    }
    return (Sfx*)[ObjectPool getFromPool:name];
}

-(void)didUnuse{
    [self stopAction:self.action];
    [self removeFromParentAndCleanup:NO];
}

-(void)willReuse{
    self.scale = self.initialScale;
    self.rotation = self.initialRotation;
    self.opacity = self.initialOpacity;
    [self runAction:self.action];
    [self.action step:0.0f];
}

@end
