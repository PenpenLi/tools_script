//
//  MissionForm.h
//  WaterBox2D
//
//  Created by Juan María Clavería Rosario on 11/04/14.
//  Copyright (c) 2014 Level App Studios. All rights reserved.
//

#import "cocos2d.h"
#import "Mission.h"

@interface MissionForm : CCNode
+(id)formWithSkipButton:(BOOL)skip;
// Por si hace falta un ancho diferente
+(id)formWithSkipButton:(BOOL)skip width:(float)width;
-(void)showLevel:(BOOL)show;
-(void)showTitle:(BOOL)show;
-(void)reloadMissions;
-(CCSprite*)mission1Sprite;
-(CCSprite*)mission2Sprite;
-(CCSprite*)mission3Sprite;
-(CCSprite*)mission4Sprite;
-(CCLabelBMFont*)mission1Label;
-(CCLabelBMFont*)mission2Label;
-(CCLabelBMFont*)mission3Label;
-(CCLabelBMFont*)mission4Label;
@end
