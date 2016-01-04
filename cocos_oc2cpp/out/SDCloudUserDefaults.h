//
//  SDCloudUserDefaults.h
//
//  Created by Stephen Darlington on 01/09/2011.
//  Copyright (c) 2011 Wandle Software Limited. All rights reserved.
//

#include <Foundation/Foundation.h>

class SDCloudUserDefaults : public NSObject {public:

   std::string stringForKeystd::string aKey);
    bool boolForKeystd::string aKey);
   static SDCloudUserDefaults* objectForKeystd::string aKey);
    NSInteger integerForKeystd::string aKey);
    float floatForKeystd::string aKey);

    void setStringstd::string aString,std::string aKey);
    void setBool(bool aBool,std::string aKey);
    void setObjectstatic SDCloudUserDefaults* anObject,std::string aKey);
    void setInteger(NSInteger anInteger,std::string aKey);
    void setFloat(float aFloat,std::string aKey);

    void removeObjectForKeystd::string aKey);

    void synchronize();

    void registerForNotifications();
    void removeNotifications();

};
