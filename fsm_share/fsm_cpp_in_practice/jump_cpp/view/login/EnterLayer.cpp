////EnterLayer.cpp created by php script. chris.li
#include "EnterLayer.h"
#include "StartAnimation.h"
#include "util/PlatformUtil.h"
#include "LoginLayer.h"
#include "Network.h"
#include "Player.h"
#include "RegisterLayer.h"
#include "../home/HomeScene.h"
#include "../login/LoginRewardLayer.h"
#include "../home/NoticeLayer.h"
#include "util/GameSound.h"
#include "battle/GameOverLayer.h"
#include "ImportNameLayer.h"
#include "../common/NetworkConnectLayer.h"
#include "SimpleAudioEngine.h"
bool EnterLayer::init()
{
	if (Layer::init())
	{
		loadUI();
		
		if (CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying() == false)
		{
			GameSound::playBackgroundMusic(MUSIC_LAYER_NORMAL);
		} 
		return true;
	}

	return false;
}
void EnterLayer::onEnter()
{
	Layer::onEnter();
    PlatformUtil::GC_init();

    std::string uuid = cocos2d::CCUserDefault::getInstance()->getStringForKey("uuid");
    if (uuid == "")
    {
        scheduleUpdate();
        
        NetworkConnectLayer::getInstance()->networkConnect();
    }
}

void EnterLayer::update(float dt)
{
    if (PlatformUtil::GC_isOK() == true)
    {
        auto playerId = PlatformUtil::GC_getPlayerId();
        
        log("GC:playerId <==> %s", playerId.c_str());
        
        auto displayName = PlatformUtil::GC_getDisplayName();
        
        log("GC:displayName <==> %s", displayName.c_str());
        
        unscheduleUpdate();
        
        NetworkConnectLayer::getInstance()->networkConnectStop();
    }
}
void EnterLayer::onExit()
{
	Layer::onExit();
}

void EnterLayer::loadUI()
{
	auto rootNode = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("EnterLayer/EnterLayer.json"));

	this->addChild(rootNode);

	m_bg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_bg"));

	m_img = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_img"));

	m_startBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_startBtn"));
	btnAnimation(*m_startBtn);

	m_startBtn-> addTouchEventListener(this, toucheventselector(EnterLayer::handleTouchEvent));
}

void EnterLayer::handleTouchEvent(Ref *pSender, TouchEventType type)
{
	if (pSender == nullptr)
	{
	}

	else if (pSender == m_startBtn && (TOUCH_EVENT_ENDED == type))
	{
		GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);
        
        if (PlatformUtil::checkNetwork() == false) {return;}

		////////////////////~~INTO~GAME~CENTER~/////////////////////////////////////

		std::string playerId;
        
        if (cocos2d::CCUserDefault::getInstance()->getStringForKey("uuid") == "")
        {
			if (PlatformUtil::GC_isOK() && PlatformUtil::GC_getPlayerId() != "")
            {
                playerId = PlatformUtil::GC_getPlayerId();
                
                cocos2d::CCUserDefault::getInstance()->setStringForKey("uuid", playerId);
                
                auto displayName = PlatformUtil::GC_getDisplayName();
                
                cocos2d::CCUserDefault::getInstance()->setStringForKey("displayName", playerId);
			}
            else
			{
				playerId = PlatformUtil::getGuestID();
                
                cocos2d::CCUserDefault::getInstance()->setStringForKey("uuid", playerId);
			}
            
        
            cocos2d::CCUserDefault::getInstance()->setStringForKey("password", "zhijianyou2014@163.com");
		}
        else
        {
            playerId = cocos2d::CCUserDefault::getInstance()->getStringForKey("uuid");
		}
        
        log("playerId - %s", playerId.c_str());
/////******************  
        
        Network::getInstance()->do_UserLogin_Req(playerId, "zhijianyou2014@163.com", [this](const AckInfo &ackInfo) {
            
            std::string msgInfo = ackInfo.getMessageInfo();
            
            if (msgInfo != "")
            {
                log("[UserLogin] %s", msgInfo.c_str());
            }
            else
            {
				log("[UserLogin] OKOKOKOK");

//			 std::string displayName = PlatformUtil::GC_getDisplayName();

				 if (Player::getInstance()->getNickname() == "")
				{	
						log("NONONO~~~~displayerName - %s~~~NONONO", PlatformUtil::GC_getDisplayName().c_str());
						 Layer* importLayer = ImportNameLayer::create();

						 getScene()->addChild(importLayer);
				}else
				{
					log("YOUYOUYOU~~~getNickname - %s~~~~YOUYOUYOU", Player::getInstance()->getNickname().c_str());
					Layer* layer = NoticeLayer::create();
				
					if(Player::getInstance()->getMyPlayerCount() <= 0)
					{
						this->setVisible(false);

				//		this->removeFromParentAndCleanup(true);

					    layer = StartAnimation::create();
					}else if (Player::getInstance()->getLoginCount() >= 0) 
					{
						layer = LoginRewardLayer::create();
					}
					//	Layer* layer = GameOverLayer::create();
					getScene()->addChild(layer);
					//this->removeFromParentAndCleanup(true);
				}

            }
        });
    


		////////////////////~FIRST~LOGIN~LAYER~/////////////////////////////////////////
//		auto *loginLayer = ImportName::create();

//		loginLayer->setAnchorPoint(Point::ZERO);

//		loginLayer->setPosition(0, 0);

//		this->getParent()->addChild(loginLayer, this->getLocalZOrder() + 2);
/////////////////////////////////////////////////////////////
	}


}

void EnterLayer::btnAnimation(Button& btn)
{
	Action* a = ScaleTo::create(0.33, 1.03, 0.95);
	Action* b = ScaleTo::create(0.33, 0.96, 1.02);
	Action* c = ScaleTo::create(0.33, 1, 1);
	Sequence* seq = Sequence::create((FiniteTimeAction*)a, (FiniteTimeAction*)b, (FiniteTimeAction*)c, NULL);
	(&btn)->runAction(RepeatForever::create(seq));
}