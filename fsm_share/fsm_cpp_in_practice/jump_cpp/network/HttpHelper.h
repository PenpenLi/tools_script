//
//  HttpHelper.h
//  HiFly
//
//  Created by chris li on 14-4-18.
//
//

#ifndef __HiFly__HttpHelper__
#define __HiFly__HttpHelper__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "network/HttpClient.h"

USING_NS_CC;
USING_NS_CC_EXT;

class HttpHelper : public Ref
{
public:
    static HttpHelper* getInstance();
    
    void sendData(std::string requestType, const char *data, const unsigned int dataSize);
    
private:
    HttpHelper() {}
    
    virtual ~HttpHelper() {}
    
    //Http Response Callback
    void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
};

#endif /* defined(__HiFly__HttpHelper__) */
