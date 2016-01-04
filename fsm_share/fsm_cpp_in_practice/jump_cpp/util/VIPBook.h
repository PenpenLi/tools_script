//
//  VIPBook.h
//  HiFly
//
//  Created by chris li on 14-4-22.
//
//

#ifndef __HiFly__VIPBook__
#define __HiFly__VIPBook__
#include "cocos2d.h"

class VIPBook
{
public:
    static VIPBook* getInstance();
    
private:
    VIPBook() {}
    virtual ~VIPBook() {}
    void init();
};

#endif /* defined(__HiFly__VIPBook__) */
