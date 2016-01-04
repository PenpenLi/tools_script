//
//  PlatformUtil.h
//  HiFly
//
//  Created by chris li on 14-6-4.
//
//

#ifndef __HiFly__PlatformUtil__
#define __HiFly__PlatformUtil__

#include "cocos2d.h"

class PlatformUtil
{
public:
    //Game Center
    static void GC_init();
    
    static bool GC_isOK();
    
    //AppStore IAP
    static void IOS_alterString(std::string alterString);
    
    static void IOS_getProducts();
    
    static void IOS_buyProduct(int productType);
    
    static const std::string GC_getPlayerId();
    
    static const std::string GC_getDisplayName();
    
    // LBS API
    static void LBS_start();
    
    static void LBS_stop();
    
    static double LBS_getLongitude();
    
    static double LBS_getLatitude();
    
    // Get Geust UserId
    static std::string getGuestID();
    
    static bool checkNetwork();
};

#endif /* defined(__HiFly__PlatformUtil__) */
