//
//  StageListItem.h
//  HiFly
//
//  Created by chris li on 14-6-19.
//
//

#ifndef HiFly_StageListItem_h
#define HiFly_StageListItem_h


#include "cocos2d.h"

class StageListItem : public cocos2d::Ref
{
public:
    static StageListItem* create()
    {
        auto ret = new StageListItem();
        
        ret->autorelease();
        
        return ret;
    }
    
    int32_t stageId ;
    
    int32_t star ;
};

#endif
