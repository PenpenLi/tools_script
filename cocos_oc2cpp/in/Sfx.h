//
//  Sfx.h
//  WaterBox2D
//
//  Created by Juan María Clavería Rosario on 28/10/13.
//  Copyright (c) 2013 Level App Studios. All rights reserved.
//

#import "cocos2d.h"
#import "ObjectPool.h"

@interface Sfx : CCSprite <ObjectPoolProtocol>
+(void)preloadPool;
+(Sfx*)pickHalo;
+(Sfx*)smallHalo;
+(Sfx*)bigHalo;
+(Sfx*)hugeHalo;
+(Sfx*)smallHaloExplosion;
+(Sfx*)hugeHaloExplosion;
+(Sfx*)bigHaloExplosion;
+(Sfx*)ringExplosion;
+(Sfx*)hugeRingExplosion;
+(Sfx*)gunHit;
+(Sfx*)smallWaterCircle;
+(Sfx*)bigWaterCircle;
+(Sfx*)burstStar;
+(Sfx*)burstBig;
+(Sfx*)burstLaser;
+(Sfx*)burstRedLaser;
+(Sfx*)burstPlasma;
+(Sfx*)burstHuge;
+(Sfx*)ammunition;
@end
