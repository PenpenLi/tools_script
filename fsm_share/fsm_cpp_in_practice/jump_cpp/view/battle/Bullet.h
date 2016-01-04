//
//  Bullet.h
//  HiFly
//
//  Created by chris li on 14-5-14.
//
//

#ifndef __HiFly__Bullet__
#define __HiFly__Bullet__

#include "cocos2d.h"

USING_NS_CC;

class Bullet : public Node
{
public:
    CREATE_FUNC(Bullet);
    
private:
    bool init();
};

#endif /* defined(__HiFly__Bullet__) */
