//
//  MissionRow.h
//  WaterBox2D
//
//  Created by Juan María Clavería Rosario on 08/04/14.
//  Copyright (c) 2014 Level App Studios. All rights reserved.
//

#include "cocos2d.h"
#include "Mission.h"

class MissionRow : public CCNode {public:
   static MissionRow* rowWithMission(Mission* mission);
    void setSemiTransparent(bool semitransparent);
    void setShadow(bool shadow);
};
