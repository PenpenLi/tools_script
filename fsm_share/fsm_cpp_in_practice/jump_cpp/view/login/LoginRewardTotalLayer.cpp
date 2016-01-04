////LoginRewardTotalLayer.cpp created by php script. chris.li
#include "LoginRewardTotalLayer.h"
#include "Network.h"
#include "Player.h"
#include "RegisterLayer.h"
#include "../home/HomeScene.h"
#include "home/NoticeLayer.h"
#include "home/LastWeekRankLayer.h"
#include "util/GameSound.h"

bool LoginRewardTotalLayer::init()
{
	if (Layer::init())
	{
		loadUI();
		return true;
	}

	return false;
}

void LoginRewardTotalLayer::onEnter()
{
	Layer::onEnter();
}

void LoginRewardTotalLayer::onExit()
{
	Layer::onExit();
}

void LoginRewardTotalLayer::loadUI()
{
	auto rootNode = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("LoginSumRewardLayer/LoginSumRewardLayer.json"));

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

	m_getRewardBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_getRewardBtn"));

	m_getRewardBtn-> addTouchEventListener(this, toucheventselector(LoginRewardTotalLayer::handleTouchEvent));

	//1-7
	int count = Player::getInstance()->getLoginCountTotal();

//	count = 2;
	for (int i = 1; i < 8; i++)
	{
		std::string name = StringUtils::format("m_day_%d", i);

		auto imageView = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, name.c_str()));
		if ((i) < count)
		{
			auto markImg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(imageView, "Got"));
			imageView->setOpacity(200);
			imageView->setColor(Color3B(155, 155, 155));
			markImg->setVisible(true);
		}else if ((i) == count)
		{
			auto markImg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(imageView, "Ring"));
			markImg->setVisible(true);
		}
		m_imgViewList.pushBack(imageView);
	}

}

void LoginRewardTotalLayer::handleTouchEvent(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_CHANGE_PAGE);

	if (pSender == nullptr)
	{
	}

	else if (pSender == m_getRewardBtn && (TOUCH_EVENT_ENDED == type))
	{
		/*Network::getInstance()->do_UserLoginReward_Req(1, [](const AckInfo &ackInfo) {

		std::string msgInfo = ackInfo.getMessageInfo();

		if (msgInfo != "")
		{
			log("[do_UserLoginReward_Req] %s", msgInfo.c_str());
		}
		else
		{
			auto scene = HomeScene::create();

			Director::getInstance()->replaceScene(scene);
		}
		});*/

//		int c = Player::getInstance()->getLoginCountTotal();
//		c = 2;
		if ((Player::getInstance()->getLoginCountTotal() == 1) && (Player::getInstance()->getMyPlayerCount() > 1))
		{
			auto layer = LastWeekRankLayer::create();

			this->getScene()->addChild(layer);
		}else
		{
			auto layer = NoticeLayer::create();

			this->getScene()->addChild(layer);
		}


	}


}

