//
//  Sfx.m
//  WaterBox2D
//
//  Created by Juan María Clavería Rosario on 28/10/13.
//  Copyright (c) 2013 Level App Studios. All rights reserved.
//

#include "Sfx.h"
#include "Node+Animation.h"
#include "GameScene.h"





@interface Sfx ()
@property (copy,nonatomic)std::string poolName;
@property (retain,nonatomic) Action *action;
    float initialScale;
    float initialRotation;
    GLubyte initialOpacity;


@implementation Sfx
Sfx* Sfx::initWithSpriteFrame(SpriteFrame* spriteFrame) 
{
    this = super->initWithSpriteFrame(spriteFrame);
    if (this){
        this->action = NULL;
        this->initialScale = 1.0f;
        this->initialRotation = 0.0f;
        this->initialOpacity = 255;
    }
    return this;
}

void Sfx::cleanup() 
{
    super->cleanup();
    this->action = NULL;
}

void Sfx::dealloc() 
{
    this->poolName = NULL;
    this->action = NULL;
    super->dealloc();
}

void Sfx::preloadPool() 
{
    auto preloaded = new Vector<T>;
    
    for (int i = 0; i < 8; i++) preloaded->pushBack(this->pickHalo());
    for (int i = 0; i < 2; i++) preloaded->pushBack(this->smallHalo());
    for (int i = 0; i < 2; i++) preloaded->pushBack(this->bigHalo());
    for (int i = 0; i < 2; i++) preloaded->pushBack(this->smallHaloExplosion());
    for (int i = 0; i < 2; i++) preloaded->pushBack(this->hugeHaloExplosion());
    for (int i = 0; i < 2; i++) preloaded->pushBack(this->bigHaloExplosion());
    for (int i = 0; i < 2; i++) preloaded->pushBack(this->ringExplosion());
    for (int i = 0; i < 2; i++) preloaded->pushBack(this->hugeRingExplosion());
    for (int i = 0; i < 8; i++) preloaded->pushBack(this->gunHit());
    for (int i = 0; i < 5; i++) preloaded->pushBack(this->smallWaterCircle());
    for (int i = 0; i < 5; i++) preloaded->pushBack(this->bigWaterCircle());
    for (int i = 0; i < 2; i++) preloaded->pushBack(this->burstStar());
    for (int i = 0; i < 2; i++) preloaded->pushBack(this->burstBig());
    for (int i = 0; i < 2; i++) preloaded->pushBack(this->burstLaser());
    for (int i = 0; i < 2; i++) preloaded->pushBack(this->burstRedLaser());
    for (int i = 0; i < 2; i++) preloaded->pushBack(this->burstPlasma());
    for (int i = 0; i < 2; i++) preloaded->pushBack(this->burstHuge());
    for (int i = 0; i < 10; i++) preloaded->pushBack(this->ammunition());
    
    for (Sfx *obj in preloaded), :putInPool(obj, obj.poolName);
    
    preloaded->removeAllObjects();
}

Sfx* Sfx::haloWithScale(float scale) 
{
   std::string name = StringUtils::format("haloWithScale%.2f", scale);
    if (!ObjectPool::hasObject(name)){
        // Init
        Sfx *sprite = Sfx::createWithSpriteFrameName("sfxHalo.png");
        sprite.poolName = name;
       sprite->setBlendFunc((ccBlendFunc){GL_SRC_ALPHA,GL_ONE});
        sprite.initialScale = scale;
        
        // Action
        FadeOut *fadeOut = FadeOut::create(0.25f);
        CallFunc *block = CallFunc::create([this]() {
            sprite->removeFromParentAndCleanup(false);, :putInPool(sprite, name);
        });
        Sequence *sequence = Sequence::create(fadeOut,block,NULL);
        sprite.action = sequence;
        ObjectPool::putInPool(sprite, name);
    }
    return (Sfx*)ObjectPool::getFromPool(name);
}

Sfx* Sfx::haloExplosionWithScale(float scale) 
{
   std::string name = StringUtils::format("haloExplosionWithScale%.2f", scale);
    if (!ObjectPool::hasObject(name)){
        // Init
        Sfx *sprite = Sfx::createWithSpriteFrameName("sfxHaloExplosion.png");
        sprite.poolName = name;
       sprite->setBlendFunc((ccBlendFunc){GL_SRC_ALPHA,GL_ONE});
        sprite.initialScale = scale;
        
        // Action
        FadeOut *fadeOut = FadeOut::create(0.15f);
        CallFunc *block = CallFunc::create([this]() {
            sprite->removeFromParentAndCleanup(false);, :putInPool(sprite, name);
        });
        Sequence *sequence = Sequence::create(fadeOut,block,NULL);
        sprite.action = sequence;
        ObjectPool::putInPool(sprite, name);
    }
    return (Sfx*)ObjectPool::getFromPool(name);
}

Sfx* Sfx::ringExplosionWithScale(float scale) 
{
   std::string name = StringUtils::format("ringExplosionWithScale%.2f", scale);
    if (!ObjectPool::hasObject(name)){
        // Init
        Sfx *sprite = Sfx::createWithSpriteFrameName("sfxSmallExplosion02.png");
        sprite.poolName = name;
       sprite->setBlendFunc((ccBlendFunc){GL_SRC_ALPHA,GL_ONE});
        sprite.initialRotation = arc4random()%360;
        
        // Action
        FadeOut *fadeOut = FadeOut::create(0.4f);
        ScaleBy *scaleBy = ScaleBy::create(0.4f, scale);
        Spawn *spawn = Spawn::create(fadeOut,scaleBy,NULL);
        CallFunc *block = CallFunc::create([this]() {
            sprite->removeFromParentAndCleanup(false);, :putInPool(sprite, name);
        });
        Sequence *sequence = Sequence::create(spawn,block,NULL);
        sprite.action = sequence;
        ObjectPool::putInPool(sprite, name);
    }
    return (Sfx*)ObjectPool::getFromPool(name);
}

Sfx* Sfx::ringExplosion() 
{
    return this->ringExplosionWithScale(10.0f);
}

Sfx* Sfx::hugeRingExplosion() 
{
    return this->ringExplosionWithScale(60.0f);
}

Sfx* Sfx::pickHalo() 
{
   std::string name = "pickHalo";
    if (!ObjectPool::hasObject(name)){
        // Init
        Sfx *sprite = Sfx::createWithSpriteFrameName("sfxPickHalo.png");
        sprite.poolName = name;
       sprite->setBlendFunc((ccBlendFunc){GL_SRC_ALPHA,GL_ONE});
        sprite.initialScale = 2.0f;
        
        // Action
        FadeOut *fadeOut = FadeOut::create(0.25f);
        ScaleBy *scaleBy = ScaleBy::create(0.25f, 3.0f);
        Spawn *spawn = Spawn::create(fadeOut,scaleBy,NULL);
        CallFunc *block = CallFunc::create([this]() {
            sprite->removeFromParentAndCleanup(false);, :putInPool(sprite, name);
        });
        Sequence *sequence = Sequence::create(spawn,block,NULL);
        sprite.action = sequence;
        ObjectPool::putInPool(sprite, name);
    }
    return (Sfx*)ObjectPool::getFromPool(name);
}

Sfx* Sfx::smallHalo() 
{
    return this->haloWithScale(3.0f);
}

Sfx* Sfx::smallHaloExplosion() 
{
    return this->haloExplosionWithScale(3.0f);
}

Sfx* Sfx::bigHalo() 
{
    return this->haloWithScale(7.0f);
}

Sfx* Sfx::hugeHalo() 
{
    return this->haloWithScale(14.0f);
}

Sfx* Sfx::hugeHaloExplosion() 
{
    return this->haloExplosionWithScale(14.0f);
}

Sfx* Sfx::bigHaloExplosion() 
{
    return this->haloExplosionWithScale(7.0f);
}

Sfx* Sfx::gunHit() 
{
   std::string name = "gunHit";
    if (!ObjectPool::hasObject(name)){
        // Init
        Sfx *sprite = Sfx::createWithSpriteFrameName("sfxSmallExplosion01.png");
        sprite.poolName = name;
        
        // Action
       Vector<T>* frames = Node::spriteFramesWithSequence("sfxSmallExplosion%.2d.png", 5);
        Animation *animation = Animation::animationWithSpriteFrames(frames, 0.05f);
        Animate *animate = Node::actionAnimation(animation);
        CallFunc *block = CallFunc::create([this]() {
            sprite->removeFromParentAndCleanup(false);, :putInPool(sprite, name);
        });
        Sequence *sequence = Sequence::create(animate,block,NULL);
        sprite.action = sequence;
        ObjectPool::putInPool(sprite, name);
    }
    return (Sfx*)ObjectPool::getFromPool(name);
}

Sfx* Sfx::waterCircleWithScale(float scale, ccTime time) 
 {
   std::string name = StringUtils::format("waterCircleWithScale%.2f", scale);
    if (!ObjectPool::hasObject(name)){
        // Init
        Sfx *sprite = Sfx::createWithSpriteFrameName("sfxWaterCircle.png");
        sprite.poolName = name;
       sprite->setBlendFunc((ccBlendFunc){GL_SRC_ALPHA,GL_ONE});
       sprite->setAnchorPoint(ccp(0.5f,0.8f));
        sprite.initialScale = scale*0.25f;
        
        // Action
        FadeOut *fadeOut = FadeOut::create(time);
        ScaleBy *scaleTo = ScaleTo::create(time, scale);
        Spawn *spawn = Spawn::create(fadeOut,scaleTo,NULL);
        CallFunc *block = CallFunc::create([this]() {
            sprite->removeFromParentAndCleanup(false);, :putInPool(sprite, name);
        });
        Sequence *sequence = Sequence::create(spawn,block,NULL);
        sprite.action = sequence;
        ObjectPool::putInPool(sprite, name);
    }
    return (Sfx*)ObjectPool::getFromPool(name);
}

Sfx* Sfx::smallWaterCircle() 
{
    return this->waterCircleWithScale(2.5f, 0.5f);
}

Sfx* Sfx::bigWaterCircle() 
{
    return this->waterCircleWithScale(4.0f, 0.75f);
}

Sfx* Sfx::burstStar() 
{
   std::string name = "bustStar";
    if (!ObjectPool::hasObject(name)){
        // Init
        Sfx *sprite = Sfx::createWithSpriteFrameName("sfxBurstStar.png");
        sprite.poolName = name;
       sprite->setBlendFunc((ccBlendFunc){GL_SRC_ALPHA,GL_ONE});
        sprite.initialScale = 2.0f;
        
        // Action
        FadeOut *fadeOut = FadeOut::create(0.15f);
        CallFunc *block = CallFunc::create([this]() {
            sprite->removeFromParentAndCleanup(false);, :putInPool(sprite, name);
        });
        Sequence *sequence = Sequence::create(fadeOut,block,NULL);
        sprite.action = sequence;
        ObjectPool::putInPool(sprite, name);
    }
    return (Sfx*)ObjectPool::getFromPool(name);
}

Sfx* Sfx::burstLaser() 
{
   std::string name = "burstLaser";
    if (!ObjectPool::hasObject(name)){
        // Init
        Sfx *sprite = Sfx::createWithSpriteFrameName("sfxBurstLaser.png");
        sprite.poolName = name;
       sprite->setBlendFunc((ccBlendFunc){GL_SRC_ALPHA,GL_ONE});
        sprite.initialScale = 2.0f;
        
        // Action
        FadeOut *fadeOut = FadeOut::create(0.15f);
        CallFunc *block = CallFunc::create([this]() {
            sprite->removeFromParentAndCleanup(false);, :putInPool(sprite, name);
        });
        Sequence *sequence = Sequence::create(fadeOut,block,NULL);
        sprite.action = sequence;
        ObjectPool::putInPool(sprite, name);
    }
    return (Sfx*)ObjectPool::getFromPool(name);
}

Sfx* Sfx::burstRedLaser() 
{
   std::string name = "burstRedLaser";
    if (!ObjectPool::hasObject(name)){
        // Init
        Sfx *sprite = Sfx::createWithSpriteFrameName("sfxBurstRedLaser.png");
        sprite.poolName = name;
       sprite->setBlendFunc((ccBlendFunc){GL_SRC_ALPHA,GL_ONE});
        sprite.initialScale = 2.0f;
        
        // Action
        FadeOut *fadeOut = FadeOut::create(0.15f);
        CallFunc *block = CallFunc::create([this]() {
            sprite->removeFromParentAndCleanup(false);, :putInPool(sprite, name);
        });
        Sequence *sequence = Sequence::create(fadeOut,block,NULL);
        sprite.action = sequence;
        ObjectPool::putInPool(sprite, name);
    }
    return (Sfx*)ObjectPool::getFromPool(name);
}

Sfx* Sfx::burstPlasma() 
{
   std::string name = "bustPlasma";
    if (!ObjectPool::hasObject(name)){
        // Init
        Sfx *sprite = Sfx::createWithSpriteFrameName("sfxBurstPlasma.png");
        sprite.poolName = name;
       sprite->setBlendFunc((ccBlendFunc){GL_SRC_ALPHA,GL_ONE});
        sprite.initialScale = 2.0f;
        
        // Action
        FadeOut *fadeOut = FadeOut::create(0.15f);
        CallFunc *block = CallFunc::create([this]() {
            sprite->removeFromParentAndCleanup(false);, :putInPool(sprite, name);
        });
        Sequence *sequence = Sequence::create(fadeOut,block,NULL);
        sprite.action = sequence;
        ObjectPool::putInPool(sprite, name);
    }
    return (Sfx*)ObjectPool::getFromPool(name);
}

Sfx* Sfx::burstBig() 
{
   std::string name = "bustBig";
    if (!ObjectPool::hasObject(name)){
        // Init
        Sfx *sprite = Sfx::createWithSpriteFrameName("sfxBurstBig.png");
        sprite.poolName = name;
       sprite->setBlendFunc((ccBlendFunc){GL_SRC_ALPHA,GL_ONE});
        sprite.initialScale = 2.0f;
        
        // Action
        FadeOut *fadeOut = FadeOut::create(0.15f);
        CallFunc *block = CallFunc::create([this]() {
            sprite->removeFromParentAndCleanup(false);, :putInPool(sprite, name);
        });
        Sequence *sequence = Sequence::create(fadeOut,block,NULL);
        sprite.action = sequence;
        ObjectPool::putInPool(sprite, name);
    }
    return (Sfx*)ObjectPool::getFromPool(name);
}

Sfx* Sfx::burstHuge() 
{
   std::string name = "bustHuge";
    if (!ObjectPool::hasObject(name)){
        // Init
        Sfx *sprite = Sfx::createWithSpriteFrameName("sfxBurstBig.png");
        sprite.poolName = name;
       sprite->setBlendFunc((ccBlendFunc){GL_SRC_ALPHA,GL_ONE});
        sprite.initialScale = 4.0f;
        
        // Action
        FadeOut *fadeOut = FadeOut::create(0.15f);
        CallFunc *block = CallFunc::create([this]() {
            sprite->removeFromParentAndCleanup(false);, :putInPool(sprite, name);
        });
        Sequence *sequence = Sequence::create(fadeOut,block,NULL);
        sprite.action = sequence;
        ObjectPool::putInPool(sprite, name);
    }
    return (Sfx*)ObjectPool::getFromPool(name);
}

Sfx* Sfx::ammunition() 
{
   std::string name = "ammunition";
    if (!ObjectPool::hasObject(name)){
        // Init
        Sfx *sprite = Sfx::createWithSpriteFrameName("sfxAmmunition.png");
        sprite.poolName = name;
        
        // Action
        int maxMov = (int)(Director->sharedDirector()->winSize().height * 0.15f);
        JumpBy *jumpBy = [JumpBy create:1.0f
                                               position:ccp((arc4random()%maxMov)-(maxMov*0.5f),0.0f)
                                                 height:maxMov
                                                  jumps:1];
        RotateBy *rotateBy = RotateBy::create(1.0f, -180.0f + (arc4random()%360));
        DelayTime *delay = DelayTime::create(0.7f);
        FadeOut *fadeOut = FadeOut::create(0.3f);
        Sequence *sequence1 = Sequence::create(delay,fadeOut,NULL);
        Spawn *spawn = Spawn::create(jumpBy,rotateBy,sequence1,NULL);
        CallFunc *block = CallFunc::create([this]() {
            sprite->removeFromParentAndCleanup(false);, :putInPool(sprite, name);
        });
        Sequence *sequence2 = Sequence::create(spawn,block,NULL);
        sprite.action = sequence2;
        ObjectPool::putInPool(sprite, name);
    }
    return (Sfx*)ObjectPool::getFromPool(name);
}

void Sfx::didUnuse() 
{
    this->stopAction(this->action);
    this->removeFromParentAndCleanup(false);
}

void Sfx::willReuse() 
{
   this->setScale(this->initialScale);
   this->setRotation(this->initialRotation);
   this->setOpacity(this->initialOpacity);
    this->runAction(this->action);
    [this->action step:0.0f];
}


