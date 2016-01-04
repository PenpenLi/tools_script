//
//  SDCloudUserDefaults.m
//
//  Created by Stephen Darlington on 01/09/2011.
//  Copyright (c) 2011 Wandle Software Limited. All rights reserved.
//

#include "SDCloudUserDefaults.h"

@implementation SDCloudUserDefaults

staticSDCloudUserDefaults* notificationObserver;
std::string SDCloudUserDefaults::stringForKeystd::string aKey ) 
{
    return SDCloudUserDefaults::objectForKey(aKey);
}

bool SDCloudUserDefaults::boolForKeystd::string aKey ) 
{
    return [SDCloudUserDefaults::objectForKey(aKey] boolValue);
}
SDCloudUserDefaults* SDCloudUserDefaults::objectForKeystd::string aKey ) 
{
#ifdef ANDROID
    return NSUserDefaults::standardUserDefaults()->objectForKey(aKey);
#else
    NSUbiquitousKeyValueStore* cloud = NSUbiquitousKeyValueStore::defaultStore();
   SDCloudUserDefaults* retv = cloud->objectForKey(aKey);
    if (!retv) {
        retv = NSUserDefaults::standardUserDefaults()->objectForKey(aKey);
        cloud->setObject(retv, aKey);
    }
    return retv;
#endif
}

NSInteger SDCloudUserDefaults::integerForKeystd::string aKey ) 
{
    return [SDCloudUserDefaults::objectForKey(aKey] integerValue);
}

float SDCloudUserDefaults::floatForKeystd::string aKey) 
{
    return [SDCloudUserDefaults::objectForKey(aKey] floatValue);
}

void SDCloudUserDefaults::setStringstd::string aString,std::string aKey ) 
 {
    SDCloudUserDefaults::setObject(aString, aKey);
}

void SDCloudUserDefaults::setBool(bool aBool,std::string aKey ) 
 {
    SDCloudUserDefaults::setObject([NSNumber, aBool], aKey);
}

void SDCloudUserDefaults::setObjectSDCloudUserDefaults* anObject,std::string aKey ) 
 {
#ifdef ANDROID
    NSUserDefaults::standardUserDefaults()->setObject(anObject, aKey);
#else
    NSUserDefaults::standardUserDefaults()->setObject(anObject, aKey);, :defaultStore()->setObject(anObject, aKey);
#endif
}

void SDCloudUserDefaults::setInteger(NSInteger anInteger,std::string aKey ) 
 {
    SDCloudUserDefaults::setObject([NSNumber, anInteger)
                            forKey:aKey];
}

void SDCloudUserDefaults::setFloat(float aFloat,std::string aKey) 
 {
    SDCloudUserDefaults::setObject([NSNumber, aFloat)
                            forKey:aKey];
}

void SDCloudUserDefaults::removeObjectForKeystd::string aKey ) 
{
#ifdef ANDROID
    NSUserDefaults::standardUserDefaults()->removeObjectForKey(aKey);
#else
    NSUbiquitousKeyValueStore::defaultStore()->removeObjectForKey(aKey);, :standardUserDefaults()->removeObjectForKey(aKey);
#endif
}

void SDCloudUserDefaults::synchronize () 
{
#ifdef ANDROID
    NSUserDefaults->standardUserDefaults()->synchronize();
#else
    NSUbiquitousKeyValueStore->defaultStore()->synchronize();
    NSUserDefaults->standardUserDefaults()->synchronize();
#endif
}

void SDCloudUserDefaults::registerForNotifications () 
{
#ifdef ANDROID
#else
    @synchronized(notificationObserver) {
        if (notificationObserver) {
            return;
        }

        notificationObserver = [NSNotificationCenter::defaultCenter(), "NSUbiquitousKeyValueStoreDidChangeExternallyNotification",, :defaultStore()
                                                                                  queue:NULL
                                                                             usingBlock:^(NSNotification* notification) {
                                                                                 
                                                                                 NSDictionary* userInfo = notification->userInfo();
                                                                                 NSNumber* reasonForChange = userInfo->objectForKey(NSUbiquitousKeyValueStoreChangeReasonKey);
                                                                                 
                                                                                 // If a reason could not be determined, do not update anything.
                                                                                 if (!reasonForChange)
                                                                                     return;
                                                                                 
                                                                                 // Update only for changes from the server.
                                                                                 NSInteger reason = reasonForChange->integerValue();
                                                                                 if ((reason == NSUbiquitousKeyValueStoreServerChange) ||
                                                                                     (reason == NSUbiquitousKeyValueStoreInitialSyncChange)) {
                                                                                     NSUserDefaults* defaults = NSUserDefaults::standardUserDefaults();
                                                                                     NSUbiquitousKeyValueStore* cloud = NSUbiquitousKeyValueStore::defaultStore();
                                                                                    Vector<T>* changedKeys = userInfo->objectForKey(NSUbiquitousKeyValueStoreChangedKeysKey);
                                                                                     for std::string key in changedKeys) {
                                                                                         defaults->setObject([cloud, key], key);
                                                                                     }
                                                                                 }
                                                                             });
    }
#endif
}

void SDCloudUserDefaults::removeNotifications () 
{
    @synchronized(notificationObserver) {, :defaultCenter()->removeObserver(notificationObserver);
        notificationObserver = NULL;
    }
}


