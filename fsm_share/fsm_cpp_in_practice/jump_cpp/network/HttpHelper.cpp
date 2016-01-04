//
//  HttpHelper.cpp
//  HiFly
//
//  Created by chris li on 14-4-18.
//
//

#include "HttpHelper.h"
#include "Network.h"
#include "common/MsgBox.h"
#include "common/NetworkConnectLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::network;

static const std::string SERVER_URL_HEADER = "115.29.194.242:8000/jumpGame/src/";
//static const std::string SERVER_URL_HEADER = "http://www.jump.com/server/src/";

static HttpHelper* g_httpHelper = nullptr;

HttpHelper* HttpHelper::getInstance()
{
    if (nullptr == g_httpHelper)
    {
        g_httpHelper = new HttpHelper();
    }
    
    return g_httpHelper;
}


void HttpHelper::sendData(std::string requestType, const char *data, const unsigned int dataSize)
{
    //MsgBox::getInstance()->show("正在通信...");

	NetworkConnectLayer::getInstance()->networkConnect();

    HttpRequest* request = new HttpRequest();
    
    std::string requestUrl = SERVER_URL_HEADER + requestType + ".php";
    
    log("[HttpHelper] requestUrl->%s", requestUrl.c_str());
    
    request->setUrl(requestUrl.c_str());
    
    request->setRequestType(HttpRequest::Type::POST);
    
    request->setResponseCallback(this, httpresponse_selector(HttpHelper::onHttpRequestCompleted));
    
    // write the post data
    request->setRequestData(data, dataSize);
    
    request->setTag(requestType.c_str());
    
    log("[HttpHelper] -> msg-%s", request->getTag());
    HttpClient::getInstance()->send(request);
    
    request->release();
}

void HttpHelper::onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    //MsgBox::getInstance()->hide();
    
	NetworkConnectLayer::getInstance()->networkConnectStop();

    if (!response)
    {
        return;
    }
    
    std::string requestType = response->getHttpRequest()->getTag();
    
    //log("[HttpHelper] msg-%s <-", requestType.c_str());
    
    //long statusCode = response->getResponseCode();
    
    //log("response code: %ld", statusCode);
    
    if (!response->isSucceed())
    {
        log("[HttpHelper] response failed");
        
        log("[HttpHelper] error buffer: %s", response->getErrorBuffer());
        
        return;
    }
    
    // dump data
    std::vector<char> *buffer = response->getResponseData();
    /*
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        printf("%x", (*buffer)[i]);
    }
    */
    printf("\n");
    
    std::string dataStr;
    
    dataStr.assign(buffer->data(), buffer->size());
    
    Network::getInstance()->parseAckData(requestType, dataStr);
}
