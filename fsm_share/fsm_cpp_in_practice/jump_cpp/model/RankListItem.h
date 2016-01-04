//
//  RankList.h
//  HiFly
//
//  Created by chris li on 14-6-4.
//
//

#ifndef __HiFly__RankList__
#define __HiFly__RankList__

#include "cocos2d.h"

class RankListItem : public cocos2d::Ref
{
public:
    static RankListItem* create()
    {
        auto ret = new RankListItem();
        
        ret->autorelease();
        
        return ret;
    }
	int32_t playerId;
    
    std::string nickname;
    
    int32_t level;
    
    int32_t score;
    
    int32_t roleType;
    
};
#endif /* defined(__HiFly__RankList__) */
