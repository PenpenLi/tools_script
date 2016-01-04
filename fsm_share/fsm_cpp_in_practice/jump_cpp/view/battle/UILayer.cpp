//
//  UILayer.cpp
//  HiFly
//
//  Created by chris li on 14-5-14.
//
//

#include "UILayer.h"
#include "util/GameSound.h"

bool UILayer::init()
{
    if (Node::init())
    {
        return true;
    }
    
    return false;
}

void UILayer::sceneUpdate(float dt)
{
    
}