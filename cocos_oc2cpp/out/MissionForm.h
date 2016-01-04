//
//  MissionForm.h
//  WaterBox2D
//
//  Created by Juan María Clavería Rosario on 11/04/14.
//  Copyright (c) 2014 Level App Studios. All rights reserved.
//

#include "cocos2d.h"
#include "Mission.h"

class MissionForm : public CCNode {public:
   static MissionForm* formWithSkipButton(bool skip);
// Por si hace falta un ancho diferente
   static MissionForm* formWithSkipButton(bool skip, float width);
    void showLevel(bool show);
    void showTitle(bool show);
    void reloadMissions();
    CCSprite* mission1Sprite();
    CCSprite* mission2Sprite();
    CCSprite* mission3Sprite();
    CCSprite* mission4Sprite();
    CCLabelBMFont* mission1Label();
    CCLabelBMFont* mission2Label();
    CCLabelBMFont* mission3Label();
    CCLabelBMFont* mission4Label();
};
