////LastWeekRankLayer.cpp created by php script. chris.li
#include "LastWeekRankLayer.h"
#include "../model/Player.h"
#include "network/Network.h"
#include "HomeScene.h"
#include "NoticeLayer.h"
#include "CrownLayer.h"
#include "util/GameSound.h"

bool LastWeekRankLayer::init()
{
	if (Layer::init())
	{
		loadUI();
		rankN = 0;
		return true;
	}

	return false;
}
void LastWeekRankLayer::onEnter()
{
	Layer::onEnter();

	m_rankListItem = static_cast<Layout*>(m_rankListItem->clone());

	CC_SAFE_RETAIN(m_rankListItem);

	refreshUI();

	Network::getInstance()->do_lastRankList_Req([this](const AckInfo &ackInfo) {

		std::string msgInfo = ackInfo.getMessageInfo();

		if (msgInfo != "")
		{
			log("[lastRankList_Ack] %s", msgInfo.c_str());
		}
		else
		{
			log("[lastRankList_Ack] OK!");
			refreshUI();
		}
	});

}

void LastWeekRankLayer::onExit()
{
	Layer::onExit();
}

void LastWeekRankLayer::loadUI()
{
	auto rootNode = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("LastWeekRankLayer/LastWeekRankLayer.json"));

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

	m_rankListScrollView = static_cast<ScrollView*>(ui::Helper::seekWidgetByName(rootNode, "m_friendListScrollView"));

	m_ContinueBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_ContinueBtn"));

	m_ContinueBtn-> addTouchEventListener(this, toucheventselector(LastWeekRankLayer::handleTouchEvent));

	m_rankListView = static_cast<ListView*>(ui::Helper::seekWidgetByName(rootNode, "m_friendListView"));

	m_rankListItem = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "m_friendListItem"));


}

void LastWeekRankLayer::handleTouchEvent(Ref *pSender, TouchEventType type)
{
	if (pSender == nullptr)
	{
	}

	else if (pSender == m_ContinueBtn && (TOUCH_EVENT_ENDED == type))
	{
		GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);

		Layer* layer = NoticeLayer::create();

		if (rankN > 0 && rankN < 4)
		{
			layer = CrownLayer::create();
		}

		this->getScene()->addChild(layer);
	}
}

void LastWeekRankLayer::refreshUI()
{
	auto ranklist = Player::getInstance()->getlastRankList();

	m_rankListView->removeAllItems();

	for (int i = 0; i < (*ranklist).size(); i ++)
	{
		auto rankListItem = (*ranklist).at(i);

		auto item = m_rankListItem->clone();

		item->setVisible(true);

		item->setUserData(rankListItem);

		ui::TextAtlas* rankNum = nullptr;

		auto headImg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(item, "mm_headImg"));

		auto rankImg1 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(item, "mm_rankImg_1"));
		rankImg1->setVisible(false);

		auto rankImg2 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(item, "mm_rankImg_2"));
		rankImg2->setVisible(false);

		auto rankImg3 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(item, "mm_rankImg_3"));
		rankImg3->setVisible(false);

		auto friendName = static_cast<Text*>(ui::Helper::seekWidgetByName(item, "mm_friendName"));

		auto score = static_cast<TextAtlas*>(ui::Helper::seekWidgetByName(item, "mm_scoreAtlas"));

		if (i < 3)
		{
			switch (i)
			{
			case 0:
				rankImg1->setVisible(true); break;
			case 1:
				rankImg2->setVisible(true); break;
			case 2:
				rankImg3->setVisible(true); break;
			}
		}else
		{
			rankNum = static_cast<TextAtlas*>(ui::Helper::seekWidgetByName(item, "rankNum"));

			rankNum->setStringValue(StringUtils::format("%d", i + 1));
		}

		friendName->setText(rankListItem->nickname);

		if (rankListItem->playerId == Player::getInstance()->getMyPlayerId())
		{
			rankN = i + 1;
		}

		score->setStringValue(StringUtils::format("%d", rankListItem->score));

		m_rankListView->pushBackCustomItem(item);


	}
}