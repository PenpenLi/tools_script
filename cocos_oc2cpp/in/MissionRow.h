//
//  MissionRow.h
//  WaterBox2D
//
//  Created by Juan María Clavería Rosario on 08/04/14.
//  Copyright (c) 2014 Level App Studios. All rights reserved.
//

#import "cocos2d.h"
#import "Mission.h"

@interface MissionRow : CCNode
+(id)rowWithMission:(Mission*)mission;
-(void)setSemiTransparent:(BOOL)semitransparent;
-(void)setShadow:(BOOL)shadow;
@end
