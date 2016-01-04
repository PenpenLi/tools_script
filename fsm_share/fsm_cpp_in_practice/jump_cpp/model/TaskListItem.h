//
//  TaskListItem.h
//  HiFly
//
//  Created by chris li on 14-6-19.
//
//

#ifndef HiFly_TaskListItem_h
#define HiFly_TaskListItem_h


#include "cocos2d.h"

class TaskListItem : public cocos2d::Ref
{
public:
    static TaskListItem* create()
    {
        auto ret = new TaskListItem();
        
        ret->autorelease();
        
        return ret;
    }
    
    int32_t taskId;
    
    int32_t taskState;

	int32_t taskTimes;

	int32_t taskAllTimes;
};

#endif
