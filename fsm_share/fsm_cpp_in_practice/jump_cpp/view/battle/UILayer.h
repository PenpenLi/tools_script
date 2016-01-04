//
//  UILayer.h
//  HiFly
//
//  Created by chris li on 14-5-14.
//
//

#ifndef __HiFly__UILayer__
#define __HiFly__UILayer__

#include "cocos2d.h"

USING_NS_CC;

class UILayer : public Node
{
public:
    CREATE_FUNC(UILayer);
    
    void sceneUpdate(float dt);
    
private:
    bool init();
};

#endif /* defined(__HiFly__UILayer__) */
