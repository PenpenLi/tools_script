#include "HelloWorldScene.h"
#include "Network.h"
#include "VIPBook.h"
#include "common/MsgBox.h"
#include "Player.h"
#include "PlatformUtil.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        token = 0;
        counter = 0;
        
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 2);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = LabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    label->setPosition(Point(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 1);
    
    scheduleUpdate();
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    if (MsgBox::getInstance()->isShow())
    {
        MsgBox::getInstance()->hide();
    }
    else
    {
        MsgBox::getInstance()->show("This is MsgBox!!!!");
    }
    
    //Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //exit(0);
#endif
}

void HelloWorld::onEnter()
{
    Layer::onEnter();
    
    //2. test config
    //VIPBook::getInstance();
    
    //3. test MsgBox
    //MsgBox::getInstance()->show("This is MsgBox!!!!");
    
    //4. test LBS
    //PlatformUtil::LBS_start();
}


void HelloWorld::update(float dt)
{
    counter++;
    if (counter == 60*5)
    {
        counter = 0;
        
        token++;
        
        step();
    }
}

void HelloWorld::step()
{
    //auto jingdu = PlatformUtil::LBS_getLatitude();
    
    //auto weidu = PlatformUtil::LBS_getLongitude();
    
    //log("jingdu %lf, weidu %lf", jingdu, weidu);
//#if 0
    //1. test network: register
#if 0
    if (1 == token)
    {
        Network::getInstance()->do_UserRegister_Req("lcs", "lcs");
        auto listener = EventListenerCustom::create("UserRegister_Ack", [this](EventCustom* event) {
            log("[Test]UserRegister_Ack");
            if (event->getUserData() != nullptr)
            {
                std::string *msgInfo = static_cast<std::string*> (event->getUserData());
                
                if (*msgInfo != "")
                {
                    log("[UserRegister] %s", msgInfo->c_str());
                }
                
                //CCASSERT(*msgInfo == "", "UserRegister failed");
                
                log("[UserRegister] pass");
            }
        });
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    }

    //2. test login
    if (2 == token)
    {
        Network::getInstance()->do_UserLogin_Req("lcs", "lcs", "", "");
        auto login_ack = EventListenerCustom::create("UserLogin_Ack", [this](EventCustom* event) {
            log("[Test]UserLogin_Ack");
            std::string *msgInfo = static_cast<std::string*> (event->getUserData());
            
            if (*msgInfo != "")
            {
                log("[UserLogin] %s", msgInfo->c_str());
            }
            
            CCASSERT(*msgInfo == "", "UserLogin failed");
            
            log("[UserLogin] pass");
        });
        _eventDispatcher->addEventListenerWithSceneGraphPriority(login_ack, this);
    }

    if (3 == token)
    {
        //3. test setNickName
        Network::getInstance()->do_SetNickname_Req("兽兽1");
        auto SetNickname_Ack = EventListenerCustom::create("SetNickname_Ack", [this](EventCustom* event) {
            log("[Test]SetNickname_Ack");
            
            std::string *msgInfo = static_cast<std::string*> (event->getUserData());
            
            if (*msgInfo != "")
            {
                log("[SetNickname] %s", msgInfo->c_str());
            }
            
            CCASSERT(*msgInfo == "", "SetNickname failed");
            CCASSERT(Player::getInstance()->getNickname() == "兽兽1", "SetNickname failed");
            
            log("[SetNickname] pass");
        });
        _eventDispatcher->addEventListenerWithSceneGraphPriority(SetNickname_Ack, this);
    }
        
    if (4 == token)
    {
        //4. test role level upgrade
        int preRoleLevel = Player::getInstance()->getRoleLevel();
        Network::getInstance()->do_UpRole_Req(1);
        auto UpRole_Ack = EventListenerCustom::create("UpRole_Ack", [preRoleLevel](EventCustom* event) {
            log("[Test]UpRole_Ack");
            
            std::string *msgInfo = static_cast<std::string*> (event->getUserData());
            
            if (*msgInfo != "")
            {
                log("[UpRole_Ack] %s", msgInfo->c_str());
            }
            
            //CCASSERT(*msgInfo == "", "UpRole_Ack failed");
            //CCASSERT(Player::getInstance()->getRoleLevel() == (preRoleLevel+1), "UpRole_Ack failed");
            
            log("[UpRole_Ack] pass");
        });
        _eventDispatcher->addEventListenerWithSceneGraphPriority(UpRole_Ack, this);
    }
    
    if (5 == token)
    {
        // 5 test rank list
        Network::getInstance()->do_RankList_Req(1, 1);
        auto RankList_Ack = EventListenerCustom::create("RankList_Ack", [this](EventCustom* event) {
            log("[Test]RankList_Ack");
            
            std::string *msgInfo = static_cast<std::string*> (event->getUserData());
            
            if (*msgInfo != "")
            {
                log("[RankList_Ack] %s", msgInfo->c_str());
            }
            
            CCASSERT(*msgInfo == "", "RankList_Ack failed");
            
            log("[RankList_Ack] pass");
        });
        _eventDispatcher->addEventListenerWithSceneGraphPriority(RankList_Ack, this);
    }
    
    if (6 == token)
    {
        // test Opinion
        
    }
#endif
}