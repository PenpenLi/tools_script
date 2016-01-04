//
//  MissionRow.m
//  WaterBox2D
//
//  Created by Juan María Clavería Rosario on 08/04/14.
//  Copyright (c) 2014 Level App Studios. All rights reserved.
//

#include "MissionRow.h"
#include "BReader.h"
#include "SKGameHelp.h"

#define TAG_ICON 1
#define TAG_LABEL 2
#define TAG_SHADOW 3

@implementation MissionRow{
    Mission *_mission;
}
MissionRow* MissionRow::rowWithMission(Mission* mission) 
{
    MissionRow *row = (MissionRow*)BReader::nodeGraphFromFile("MissionRow.ccbi");
    row->setMission(mission);
    return row;
}

void MissionRow::didLoadFromB() 
{
    _mission = NULL;
    LabelBMFont *label = this->label();
    LabelBMFont *shadow = this->shadow();
    
    TextAlignment alignment = kTextAlignmentLeft;
    float width = 300.0f;//this->getContentSize().width*0.75f/label->getScale();
    
    label->setAlignment(alignment);
    shadow->setAlignment(alignment);
//    label->setWidth(width);
//    shadow->setWidth(width);
    
    shadow.visible = false;
}

void MissionRow::dealloc() 
{
    _mission->release();
    super->dealloc();
}

Sprite* MissionRow::icon() 
{
    return (Sprite*)this->getChildByTag(TAG_ICON);
}

LabelBMFont* MissionRow::label() 
{
    return (LabelBMFont*)this->getChildByTag(TAG_LABEL);
}

LabelBMFont* MissionRow::shadow() 
{
    return (LabelBMFont*)this->getChildByTag(TAG_SHADOW);
}

Mission* MissionRow::mission() 
{
    return _mission;
}

void MissionRow::setMission(Mission* mission) 
{
    _mission->release();
    _mission = mission->retain();
    this->label().string = this->shadow().string = mission->text();
    this->icon()->setDisplayFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(mission->icon()));
}

void MissionRow::setSemiTransparent(bool semitransparent) 
{
    int opacity = semitransparent ? 128 : 255;
    this->label().opacity = opacity;
    this->shadow().opacity = opacity;
    this->icon().opacity = opacity;
}

void MissionRow::setShadow(bool shadow) 
{
    this->shadow().visible = shadow;
}


