////NoticeLayer.cpp created by php script. chris.li
#include "NoticeLayer.h"
#include "../home/HomeScene.h"
#include "Network.h"
#include "Player.h"
#include "util/GameSound.h"

bool NoticeLayer::init()
{
	if (Layer::init())
	{
		loadUI();

		return true;
	}

	return false;
}
void NoticeLayer::onEnter()
{
	Layer::onEnter();
	Network::getInstance()->do_Notice_Req(1, [this](const AckInfo &ackInfo) {

		std::string msgInfo = ackInfo.getMessageInfo();
		if (msgInfo != "")
		{
			log("[UserLogin] %s", msgInfo.c_str());
		}
		else
		{
			m_label->setText(Player::getInstance()->getNoticeMsg());
		}
	});
}

void NoticeLayer::onExit()
{
	Layer::onExit();
}

void NoticeLayer::loadUI()
{
	auto rootNode = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("NoticeLayer/NoticeLayer.json"));

	this->addChild(rootNode);

	//add layer bg and bubble
	Sprite* _bg = Sprite::create("commonUi/layerBg.jpg");
	_bg->setAnchorPoint(Point::ZERO);
	_bg->setPosition(Point::ZERO);
	_bg->setLocalZOrder(-1);
	rootNode->addChild(_bg);

	Sprite* _bubble = Sprite::create("commonUi/bubble.png");
	Size _winSize = this->getContentSize();
	_bubble->setPosition(ccp(_winSize.width/2,_winSize.height/2));
	_bubble->setLocalZOrder(-1);
	rootNode->addChild(_bubble);

	m_continue = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_continue"));

	m_continue-> addTouchEventListener(this, toucheventselector(NoticeLayer::handleTouchEvent));

	m_label = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_label"));

	//m_label->setsize
}

void NoticeLayer::handleTouchEvent(Ref *pSender, TouchEventType type)
{

	if (pSender == nullptr)
	{
	}

	else if (pSender == m_continue && (TOUCH_EVENT_ENDED == type))
	{
		GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);

		auto scene = HomeScene::create();

		Director::getInstance()->replaceScene(scene);
	}


}

