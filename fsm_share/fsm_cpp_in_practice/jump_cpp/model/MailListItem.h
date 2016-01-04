//
//  MailListItem.h
//  HiFly
//
//  Created by chris li on 14-6-11.
//
//

#ifndef HiFly_MailListItem_h
#define HiFly_MailListItem_h

#include "cocos2d.h"

class MailListItem : public cocos2d::Ref
{
public:
    static MailListItem* create()
    {
        auto ret = new MailListItem();
        
        ret->autorelease();
        
        return ret;
    }
    
    int32_t mailid;
    
    int32_t mailType;
    
    std::string title;
    
    int32_t status;
    
    int32_t playerId;

	int32_t itemId;

	int32_t itemNum;

	std::string nickName;

	std::string contents;
    
    std::string domainTime;
    
};

#endif
