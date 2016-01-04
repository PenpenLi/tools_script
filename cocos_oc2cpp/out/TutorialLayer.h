//
//  TutorialLayer.h
//  WaterBox2D
//
//  Created by Juan María Clavería Rosario on 11/06/14.
//  Copyright (c) 2014 Level App Studios. All rights reserved.
//

#include "cocos2d.h"
#include "Bullseye.h"
#include "Buoy.h"

class TutorialLayer : public CCLayer {public: <BuoyDelegate,BullseyeDelegate>
    void start();
};
