//
//  FriendListItem.h
//  HiFly
//
//  Created by chris li on 14-6-6.
//
//

#ifndef HiFly_FriendListItem_h
#define HiFly_FriendListItem_h

#include "cocos2d.h"

class FriendListItem : public cocos2d::Ref
{
public:
    static FriendListItem* create()
    {
        auto ret = new FriendListItem();
        
        ret->autorelease();
        
        return ret;
    }

	int32_t playerId;
    
    std::string nickname;
    
    int32_t level;
    
    int32_t score;
    
    std::string distance;

	bool isSendHeart;

	int32_t roleType;
    
    int32_t helpTimes;
    
};

#endif
