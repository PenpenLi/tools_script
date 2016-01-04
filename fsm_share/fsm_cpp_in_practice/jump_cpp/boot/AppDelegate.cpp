#include "AppDelegate.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"

#include "login/LoginScene.h"
#include "home/HomeScene.h"
#include "login/EnterLayer.h"
#include "battle/BattleScene.h"
//#include "C2DXShareSDK.h"

//using namespace cn::sharesdk;

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    
    //初始化ShareSDK
    //C2DXShareSDK::open(CCString::create("256b69b9e9b3"), false);
    //C2DXShareSDK::open(CCString::create("api20"), false);
    
    //初始化社交平台信息
    this -> initPlatformConfig();
    // 1. initialize director
    auto director = Director::getInstance();
    
    // turn on display FPS
    director->setDisplayStats(false);
    
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("My Game");
        director->setOpenGLView(glview);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
        glview->setFrameSize(640, 960);
#endif
    }

    auto screenSize = glview->getFrameSize();
    auto designSize = Size(640, 960);
    log("SCREEN[%0.0f, %0.0f], DESIGN[%0.0f, %0.0f]", screenSize.width, screenSize.height, designSize.width, designSize.height);
    glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::FIXED_WIDTH);
    //director->setContentScaleFactor(screenSize.height/designSize.height);
    
    // 2. add file search path
    auto fileUtils = FileUtils::getInstance();
    std::vector<std::string> searchPaths;
    searchPaths.push_back("res");
    searchPaths.push_back("res/ccs");
    searchPaths.push_back("res/config");
    searchPaths.push_back("res/sound");
    searchPaths.push_back("res/ccb");
    searchPaths.push_back("res/ccb/level");
    searchPaths.push_back("res/ccb/ladder");
    searchPaths.push_back("res/ccb/monster");
    searchPaths.push_back("res/ccb/prop");
    searchPaths.push_back("res/ccb/guard");
    searchPaths.push_back("res/ccb/ccbResources");
    searchPaths.push_back("res/png");
    searchPaths.push_back("res/png/bgLayer");
    searchPaths.push_back("res/png/actor");
    searchPaths.push_back("res/png/ladderAction");
    searchPaths.push_back("res/png/props");
    searchPaths.push_back("res/png/NetworkConnectAnimation");
    
    fileUtils->setSearchPaths(searchPaths);

    // 3. create a scene. it's an autorelease object
    //auto scene = HelloWorld::createScene();
    auto scene = LoginScene::create();
    //auto scene = BattleScene::create();
    //auto scene = EnterLayer::create();

    // run
    director->runWithScene(scene);
	

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    
    cocos2d::EventCustom event("EVENT_ENTER_BACKGROUND");
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void AppDelegate::initPlatformConfig()
{
    /*
    //新浪微博
    auto *sinaConfigDict = Dictionary::create();
    sinaConfigDict -> setObject(CCString::create("214311603"), "app_key");
    sinaConfigDict -> setObject(CCString::create("dfca2f7c400f3b2e1d020fe5535c1814"), "app_secret");
    sinaConfigDict -> setObject(CCString::create("http://www.zhijianyou.com.cn"), "redirect_uri");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeSinaWeibo, sinaConfigDict);
    
    //腾讯微博
    auto *tcConfigDict = Dictionary::create();
    tcConfigDict -> setObject(CCString::create("801524696"), "app_key");
    tcConfigDict -> setObject(CCString::create("d97d783258c9b306afdf3d97dff896bf"), "app_secret");
    tcConfigDict -> setObject(CCString::create("http://www.zhijianyou.com.cn"), "redirect_uri");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeTencentWeibo, tcConfigDict);
    */
    /*
    //微信
    auto *wcConfigDict = Dictionary::create();
    wcConfigDict -> setObject(CCString::create("wxb8484c14867eafeb"), "app_id");
    
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeWeixiSession, wcConfigDict);
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeWeixiTimeline, wcConfigDict);
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeWeixiFav, wcConfigDict);
     */
}

