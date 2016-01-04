//
//  RoleListItem.h
//  HiFly
//
//  Created by gongjun on 14-6-24.
//
//

#ifndef __HiFly__RoleListItem__
#define __HiFly__RoleListItem__

#include "cocos2d.h"

class RoleListItem : public cocos2d::Ref
{
public:
    static RoleListItem* create()
    {
        auto ret = new RoleListItem();
        
        ret->autorelease();
        
        return ret;
    }
    
	int32_t roleType;

	int32_t level;

	bool breakType;

	bool isMax;
};


#endif //__HiFly__RoleListItem__