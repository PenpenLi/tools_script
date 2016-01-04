//
//  ShareToIntenet.cpp
//  HiFly
//
//  Created by Artuira on 14-7-16.
//
//

#include "ShareToIntenet.h"
#include "C2DXShareSDK.h"
#include "TextFloatLabelLayer.h"

USING_NS_CC;
using namespace cn::sharesdk;

ShareToIntenet* g_ShareToIntenet = nullptr;

ShareToIntenet* ShareToIntenet::getInstance()
{
    if (nullptr == g_ShareToIntenet)
    {
        g_ShareToIntenet = new ShareToIntenet();
        
        if (!g_ShareToIntenet->init())
        {
            g_ShareToIntenet = nullptr;
        }
    }
    
    return g_ShareToIntenet;
}

bool ShareToIntenet::init()
{
	if (LayerColor::init())
    {
        
        return true;
    }
    
    return false;
}

void authResultHandler(C2DXResponseState state, C2DXPlatType platType, CCDictionary *error)
{
    switch (state) {
        case C2DXResponseStateSuccess:
            CCLog("授权成功");
            break;
        case C2DXResponseStateFail:
            CCLog("授权失败");
            break;
        default:
            break;
    }
}

void getUserResultHandler(C2DXResponseState state, C2DXPlatType platType, CCDictionary *userInfo, CCDictionary *error)
{
    if (state == C2DXResponseStateSuccess)
    {
        //输出用户信息
        try
        {
            CCArray *allKeys = userInfo -> allKeys();
			allKeys->retain();
            for (int i = 0; i < allKeys -> count(); i++)
            {
                CCString *key = (CCString *)allKeys -> objectAtIndex(i);
                CCObject *obj = userInfo -> objectForKey(key -> getCString());
                
                CCLog("key = %s", key -> getCString());
                if (dynamic_cast<CCString *>(obj))
                {
                    CCLog("value = %s", dynamic_cast<CCString *>(obj) -> getCString());
                }
                else if (dynamic_cast<CCInteger *>(obj))
                {
                    CCLog("value = %d", dynamic_cast<CCInteger *>(obj) -> getValue());
                }
                else if (dynamic_cast<CCDouble *>(obj))
                {
                    CCLog("value = %f", dynamic_cast<CCDouble *>(obj) -> getValue());
                }
            }
			allKeys->release();
        }
        catch(...)
        {
            CCLog("==============error");
        }
        
    }
}

void shareResultHandler(C2DXResponseState state, C2DXPlatType platType, CCDictionary *shareInfo, CCDictionary *error)
{
    switch (state) {
        case C2DXResponseStateSuccess:
            TextFloatLabelLayer::getInstance()->show("分享成功！");
            break;
        case C2DXResponseStateFail:
        {
            TextFloatLabelLayer::getInstance()->show("分享失败！");
            
            log("%s",error->objectForKey("errorDescription"));
        }

            break;
        default:
            break;
    }
}



void ShareToIntenet::showShareBox()
{
    TextFloatLabelLayer::getInstance()->show("分享功能暂未开放,敬请期待！");
    return;
    
    /*
    CCDictionary *content = CCDictionary::create();
    content -> setObject(CCString::create("这是一条测试内容"), "content");
    content -> setObject(CCString::create("http://img0.bdstatic.com/img/image/shouye/systsy-11927417755.jpg"), "image");
    content -> setObject(CCString::create("测试标题"), "title");
    content -> setObject(CCString::create("测试描述"), "description");
    content -> setObject(CCString::create("http://www.zhijianyou.com.cn"), "url");
    content -> setObject(CCString::createWithFormat("%d", C2DXContentTypeNews), "type");
    content -> setObject(CCString::create("http://www.zhijianyou.com.cn"), "siteUrl");
    content -> setObject(CCString::create("ShareSDK"), "site");
    
    content -> setObject(CCString::create("extInfo"), "extInfo");
    
    C2DXShareSDK::showShareMenu(NULL, content, Point(100, 100), C2DXMenuArrowDirectionLeft, shareResultHandler);
     */
    
    //我在这个萌萌哒游戏里获得了高分，小伙伴们一起来啊！
    
    //我在这款炒鸡萌的游戏中又获胜了，不服？来战！
    
    //我要让全世界知道，今年最清新的游戏被它承包了。不信来看看啊
    float type = CCRANDOM_0_1()*3;
    
    std::string title;
    
    int number = (int)type;
    if (number == 0) {
        title = "我在这个萌萌哒游戏里获得了高分，小伙伴们一起来啊！";
    }
    else if (number == 1)
    {
        title = "我在这款炒鸡萌的游戏中又获胜了，不服？来战！";
    }
    else if (number == 2)
    {
        title = "我要让全世界知道，今年最清新的游戏被它承包了。不信来看看啊";
    }
    else
    {
        title = "我要让全世界知道，今年最清新的游戏被它承包了。不信来看看啊";
    }
    
    std::string tt = "我在《海马去哪儿》里邀请你来玩~";
    
    CCDictionary *content = CCDictionary::create();
    content -> setObject(CCString::create(title.c_str()), "content");
    content -> setObject(CCString::create("http://115.29.194.242:8000/haima.png"), "image");
    content -> setObject(CCString::create(tt.c_str()), "title");
    content -> setObject(CCString::create("海马去哪儿"), "description");
    content -> setObject(CCString::create("http://www.zhijianyou.com.cn"), "url");
    content -> setObject(CCString::createWithFormat("%d", C2DXContentTypeNews), "type");
    content -> setObject(CCString::create("http://www.zhijianyou.com.cn"), "siteUrl");
    content -> setObject(CCString::create("zhijianyou"), "site");

    content -> setObject(CCString::create("extInfo"), "extInfo");
    
    
    
    auto plats = CCArray::create();
    auto weixin = CCInteger::create(C2DXPlatTypeWeixiSession);
    auto pengyouq = CCInteger::create(C2DXPlatTypeWeixiTimeline);
    
    plats->addObject(weixin);
    plats->addObject(pengyouq);
    
    C2DXShareSDK::showShareMenu(plats, content, CCPointMake(100, 100), C2DXMenuArrowDirectionLeft, shareResultHandler);
    //C2DXShareSDK::showShareView(C2DXPlatTypeWeixiSession, content, shareResultHandler);
    //C2DXShareSDK::showShareView(C2DXPlatTypeWeixiTimeline, content, shareResultHandler);
}