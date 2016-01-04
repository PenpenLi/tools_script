//
//  PropListItem.h
//  HiFly
//
//  Created by chris li on 14-6-18.
//
//

#ifndef HiFly_PropListItem_h
#define HiFly_PropListItem_h


#include "cocos2d.h"

class PropListItem : public cocos2d::Ref
{
public:
    static PropListItem* create()
    {
        auto ret = new PropListItem();
        
        ret->autorelease();
        
        return ret;
    }
    
    int32_t propId;
    
    int32_t propNum;
    
};

#endif
