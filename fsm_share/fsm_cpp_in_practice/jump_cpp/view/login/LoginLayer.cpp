//
//  LoginLayer.cpp
//  HiFly
//
//  Created by chris li on 14-5-6.
//
//

#include "LoginLayer.h"
#include "Network.h"
#include "Player.h"
#include "RegisterLayer.h"
#include "../home/HomeScene.h"
#include "../login/LoginRewardLayer.h"
#include "../home/NoticeLayer.h"
#include "util/GameSound.h"
#include "StartAnimation.h"
#include "battle/GameOverLayer.h"
#include "util/PlatformUtil.h"

bool LoginLayer::init()
{
    if (Layer::init())
    {
	//member variable init
	m_loginButton = 0;
	m_registerButton = 0;
	m_userName = 0;
	m_password = 0;
	m_passwordRe = 0;
	m_enterAction = 0;
	m_exitAction = 0;

	//Ui res
	importUI();
        
    scheduleUpdate();

    //GameSound::playBackgroundMusic(MUSIC_LAYER_INGAME);

        return true;
    }
    
    return false;
}

void LoginLayer::update(float dt)
{
    /*if (PlatformUtil::GC_isOK() == true)
    {
        auto playerId = PlatformUtil::GC_getPlayerId();
        
        log("GC:playerId <==> %s", playerId.c_str());
        
        auto displayName = PlatformUtil::GC_getDisplayName();
        
        log("GC:displayName <==> %s", displayName.c_str());
        
        unscheduleUpdate();
    }*/
}

void LoginLayer::onEnter()
{
    Layer::onEnter();

	if(m_enterAction)
		m_enterAction->play();
    
    
    //Test PlatformUtil
    
    PlatformUtil::GC_init();
    
    //PlatformUtil::IOS_getProducts();
    
    //PlatformUtil::IOS_buyProduct(1);


}

//void LoginLayer::onEnterTransitionDidFinish()
//{
//	log("~~~~onEnterTransitionDidFinish");
//}

void LoginLayer::onExit()
{
    do
    {
		//m_exitAction->play();
		log("~~~Login onExit");
		_eventDispatcher->removeAllEventListeners();
    } while (0);
    
    Layer::onExit();
}

//import login Res 
bool LoginLayer::importUI()
{
	bool bRet = false;
	do{
		Layout* t_layout = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("LoginUi/LoginUi_login.json"));
		this->addChild(t_layout);

		Widget* t_parent = t_layout->getChildByName("bg");

		m_loginButton = static_cast<Button*>(t_parent->getChildByName("btn_login"));

	    if (m_loginButton)
        {
            m_loginButton->addTouchEventListener(this, toucheventselector(LoginLayer::touchEventLogin));
        }

		m_registerButton = static_cast<Button*>(t_parent->getChildByName("btn_register"));

	    if (m_registerButton)
        {
            //m_registerButton->addTouchEventListener(this, toucheventselector(LoginLayer::touchEventLoadRegister));
        }

		m_userName = static_cast<TextField*>(t_parent->getChildByName("tex_user"));

		m_password = static_cast<TextField*>(t_parent->getChildByName("tex_psw"));

		m_enterAction = ActionManagerEx::getInstance()->getActionByName("LoginUi_login.json","enterAnimation");

		m_exitAction = ActionManagerEx::getInstance()->getActionByName("LoginUi_login.json","exitAnimation");

		// add event listener Login



		bRet = true;
	}while(0);

	return bRet;
}

//**********************************touch event*******************************
void LoginLayer::touchEventLogin(Ref *pSender, TouchEventType type)
{

	GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);
    
    if (TOUCH_EVENT_ENDED == type)
    {
        std::string playerId;
        
        std::string displayName;
        
        if (PlatformUtil::GC_isOK())
        {
            playerId = PlatformUtil::GC_getPlayerId();
            
            displayName = PlatformUtil::GC_getDisplayName();
            
            log("playerId - %s, displayerName - %s", playerId.c_str(), displayName.c_str());
        }
        
        else
        {
            playerId = PlatformUtil::getGuestID();
            
            displayName = PlatformUtil::GC_getDisplayName();
        }
        
        cocos2d::CCUserDefault::sharedUserDefault()->setStringForKey("uuid", playerId);
        
        cocos2d::CCUserDefault::sharedUserDefault()->setStringForKey("password", "zhijianyou2014@163.com");
        
   /*     Network::getInstance()->do_UserLogin_Req(playerId, "zhijianyou2014@163.com", "180", "120", [this](const AckInfo &ackInfo) {
            
            std::string msgInfo = ackInfo.getMessageInfo();
            
            if (msgInfo != "")
            {
                log("[UserLogin] %s", msgInfo.c_str());
            }
            else
            {
                Layer* layer = NoticeLayer::create();
				
                if(Player::getInstance()->getMyPlayerCount() <= 1)
                {
                    layer = StartAnimation::create();
                }else if (Player::getInstance()->getLoginCount() >= 0)
                {
                    layer = LoginRewardLayer::create();
                }
                //	Layer* layer = GameOverLayer::create();
                getScene()->addChild(layer);
                //this->removeFromParentAndCleanup(true);
            }
        });*/
    }
}

void LoginLayer::touchEventLoadRegister(Ref *pSender, TouchEventType type)
{
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~touch event~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


void LoginLayer::refreshUI()
{
    log("fuck!!");
}

