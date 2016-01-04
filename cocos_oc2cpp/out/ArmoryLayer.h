//
//  ArmoryLayer.h
//  WaterBox2D
//
//  Created by Juan María Clavería Rosario on 02/04/14.
//  Copyright (c) 2014 Level App Studios. All rights reserved.
//

#include "CCLayer.h"
#include "ShopCoinsLayer.h"
#include "ShopSpecialCoinsLayer.h"
#include "CashNode.h"

class ArmoryLayer : public CCLayer {public: <ShopCoinsDelegate,ShopSpecialCoinsLayerDelegate,CashNodeDelegate>
   static ArmoryLayer* layerWithTime(ccTime time, int numPlayer);
    void enableMenu(bool enable);
};
