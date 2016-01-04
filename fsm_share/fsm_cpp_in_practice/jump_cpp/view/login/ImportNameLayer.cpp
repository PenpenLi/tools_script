////ImportName.cpp created by php script. chris.li
#include "ImportNameLayer.h"
#include "Network.h"
#include "Player.h"
#include "../home/HomeScene.h"
#include "../login/LoginRewardLayer.h"
#include "../home/NoticeLayer.h"
#include "StartAnimation.h"
#include "util/GameSound.h"
#include "util/PlatformUtil.h"
#include "SimpleAudioEngine.h"
bool ImportNameLayer::init()
{
    if (Layer::init())
    {
        loadUI();
        if (CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying() == false)
        {
            GameSound::playBackgroundMusic(MUSIC_LAYER_NORMAL);
        } 
        return true;
    }

	return false;
}

void ImportNameLayer::onEnter()
{
	Layer::onEnter();
}

void ImportNameLayer::onExit()
{
	Layer::onExit();
}

void ImportNameLayer::loadUI()
{
	/////////////////~BLACK~LAYER~//////////////////////////
		LayerColor* m_maskLayer = LayerColor::create(Color4B::BLACK, Director::getInstance()->getWinSize().width, Director::getInstance()->getWinSize().height);
		m_maskLayer->setOpacity(100); 
		this->addChild(m_maskLayer,this->getLocalZOrder() - 1);


	auto rootNode = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("ImportNameLayer/ImportNameLayer.json"));

	this->addChild(rootNode);

	m_startGameBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_startGameBtn"));

	m_startGameBtn-> addTouchEventListener(this, toucheventselector(ImportNameLayer::handleTouchEvent));

	m_name = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_name"));

	m_randomNameBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_randomNameBtn"));

	m_randomNameBtn-> addTouchEventListener(this, toucheventselector(ImportNameLayer::handleTouchEvent));

	m_inputName = static_cast<TextField*>(ui::Helper::seekWidgetByName(rootNode, "m_inputName"));

	m_inputName->setColor(Color3B::BLACK);

	m_inputName-> addTouchEventListener(this, toucheventselector(ImportNameLayer::handleTouchEvent));
    
    auto GC_name = PlatformUtil::GC_getDisplayName();
    
    m_inputName->setText(GC_name.c_str());
}

void ImportNameLayer::handleTouchEvent(Ref *pSender, TouchEventType type)
{
	if (pSender == nullptr)
	{
	}

	else if (pSender == m_startGameBtn && (TOUCH_EVENT_ENDED == type))
	{
		log("~~~~inPutName-- %s ~~~", m_inputName->getStringValue().c_str());
		if (m_inputName->getStringValue() != "")
		{
			GameSound::playEffectMusic(EFF_CHANGE_PAGE);
            
			Network::getInstance()->do_SetNickname_Req(m_inputName->getStringValue(), [this](const AckInfo &ackInfo) {
                
                std::string msgInfo = ackInfo.getMessageInfo();
                
                if (msgInfo != "")
                {
                    log("[UserLogin] %s", msgInfo.c_str());
                }
                else
                {
                    log("YOUYOUYOU~~~Nickname - %s~~~~YOUYOUYOU", m_inputName->getStringValue().c_str());
                    
                    this->setVisible(false);
                    
                    Layer* layer = StartAnimation::create();
                    getScene()->addChild(layer);
                }
            });
        }
    }
	else if (pSender == m_randomNameBtn && (TOUCH_EVENT_ENDED == type))
	{
		GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);
	}


}

