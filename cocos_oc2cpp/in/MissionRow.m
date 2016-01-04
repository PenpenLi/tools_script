//
//  MissionRow.m
//  WaterBox2D
//
//  Created by Juan María Clavería Rosario on 08/04/14.
//  Copyright (c) 2014 Level App Studios. All rights reserved.
//

#import "MissionRow.h"
#import "CCBReader.h"
#import "SKGameHelp.h"

#define TAG_ICON 1
#define TAG_LABEL 2
#define TAG_SHADOW 3

@implementation MissionRow{
    Mission *_mission;
}

+(id)rowWithMission:(Mission*)mission{
    MissionRow *row = (MissionRow*)[CCBReader nodeGraphFromFile:@"MissionRow.ccbi"];
    [row setMission:mission];
    return row;
}

-(void)didLoadFromCCB{
    _mission = nil;
    CCLabelBMFont *label = [self label];
    CCLabelBMFont *shadow = [self shadow];
    
    CCTextAlignment alignment = kCCTextAlignmentLeft;
    float width = 300.0f;//self.contentSize.width*0.75f/label.scale;
    
    [label setAlignment:alignment];
    [shadow setAlignment:alignment];
//    [label setWidth:width];
//    [shadow setWidth:width];
    
    shadow.visible = NO;
}

-(void)dealloc{
    [_mission release];
    [super dealloc];
}

-(CCSprite*)icon{
    return (CCSprite*)[self getChildByTag:TAG_ICON];
}

-(CCLabelBMFont*)label{
    return (CCLabelBMFont*)[self getChildByTag:TAG_LABEL];
}

-(CCLabelBMFont*)shadow{
    return (CCLabelBMFont*)[self getChildByTag:TAG_SHADOW];
}

-(Mission*)mission{
    return _mission;
}

-(void)setMission:(Mission*)mission{
    [_mission release];
    _mission = [mission retain];
    [self label].string = [self shadow].string = [mission text];
    [[self icon]setDisplayFrame:[[CCSpriteFrameCache sharedSpriteFrameCache]spriteFrameByName:[mission icon]]];
}

-(void)setSemiTransparent:(BOOL)semitransparent{
    int opacity = semitransparent ? 128 : 255;
    [self label].opacity = opacity;
    [self shadow].opacity = opacity;
    [self icon].opacity = opacity;
}

-(void)setShadow:(BOOL)shadow{
    [self shadow].visible = shadow;
}

@end
