//
//  TutorialLayer.h
//  WaterBox2D
//
//  Created by Juan María Clavería Rosario on 11/06/14.
//  Copyright (c) 2014 Level App Studios. All rights reserved.
//

#import "cocos2d.h"
#import "Bullseye.h"
#import "Buoy.h"

@interface TutorialLayer : CCLayer <BuoyDelegate,BullseyeDelegate>
-(void)start;
@end
