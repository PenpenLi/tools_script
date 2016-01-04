//
//  PlatformUtil.cpp
//  HiFly
//
//  Created by chris li on 14-6-4.
//
//
#include "cocos2d.h"
#include "PlatformUtil.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "OCAdapter.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#else
//#error "unknown platform"
#endif

void PlatformUtil::GC_init()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    OCAdapter::GC_init();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
#else
#endif
}

bool PlatformUtil::GC_isOK()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return OCAdapter::GC_isOK();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return true;
#else
    return true;
#endif
}

const std::string PlatformUtil::GC_getPlayerId()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    const std::string playerID = OCAdapter::GC_getPlayerId();
    return playerID;
    //return "debug123456";
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return "debug123456";
#else
    return "debug123456";
#endif
}

const std::string PlatformUtil::GC_getDisplayName()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    const std::string playerID = OCAdapter::GC_getDisplayName();
    return playerID;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return "SB";
#else
    return "";
#endif
}

void PlatformUtil::LBS_start()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    OCAdapter::LBS_start();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
#else
#endif
}

void PlatformUtil::LBS_stop()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    OCAdapter::LBS_stop();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
#else
#endif
}

double PlatformUtil::LBS_getLatitude()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return OCAdapter::LBS_getLatitude();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return 120.0;
#else
    return 120.0;
#endif
}

double PlatformUtil::LBS_getLongitude()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return OCAdapter::LBS_getLongitude();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return 120.0;
#else
    return 120.0;
#endif
}

void PlatformUtil::IOS_alterString(std::string alterString)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    OCAdapter::IOS_alterString(alterString);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#else
#endif
}

void PlatformUtil::IOS_getProducts()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    OCAdapter::IOS_getProducts();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#else
#endif
}

void PlatformUtil::IOS_buyProduct(int productType)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    OCAdapter::IOS_buyProduct(productType);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#else
#endif
}

std::string PlatformUtil::getGuestID()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::string uuid = OCAdapter::IOS_getUUID();
    return "Guest" + uuid;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return "debug123456";
#else
    return "debug123456";
#endif
}

bool PlatformUtil::checkNetwork()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    auto ret = OCAdapter::checkNetwork();
    return ret;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return false;
#else
    return false;
#endif
}

