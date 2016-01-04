////RankingUpLayer.cpp created by php script. chris.li
#include "RankingUpLayer.h"
#include "Player.h"
#include "home/HomeScene.h"
#include "network/Network.h"
#include "util/GameSound.h"

bool RankingUpLayer::init()
{
	if (Layer::init())
	{
		loadUI();
		index = 1;
		myScore = 0;
		myRank= 1;
		otherScore = 0;

		return true;
	}

	return false;
}
void RankingUpLayer::onEnter()
{
	Layer::onEnter();

	refreshUI();
}

void RankingUpLayer::onExit()
{
	Layer::onExit();
}

void RankingUpLayer::loadUI()
{
	auto rootNode = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("RankingUpLayer/RankingUpLayer.json"));

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

	m_continueBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_continueBtn"));

	m_continueBtn-> addTouchEventListener(this, toucheventselector(RankingUpLayer::handleTouchEvent));

	m_shareBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_shareBtn"));

	m_shareBtn-> addTouchEventListener(this, toucheventselector(RankingUpLayer::handleTouchEvent));

	m_me = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_me"));

	m_friend = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_friend"));

	auto mName = static_cast<Text*>(ui::Helper::seekWidgetByName(m_me, "mm_friendName"));
	mName->setText(Player::getInstance()->getNickname());

	auto mScore = static_cast<TextAtlas*>(ui::Helper::seekWidgetByName(m_me, "mm_scoreAtlas"));
	mScore->setStringValue(StringUtils::format("%d", Player::getInstance()->getScore()));

}

void RankingUpLayer::handleTouchEvent(Ref *pSender, TouchEventType type)
{
	if (pSender == nullptr)
	{
	}

	else if (pSender == m_continueBtn && (TOUCH_EVENT_ENDED == type))
	{
		GameSound::playEffectMusic(EFF_CHANGE_PAGE);

		auto scene = HomeScene::create();

		Director::getInstance()->replaceScene(scene);
	}

	else if (pSender == m_shareBtn && (TOUCH_EVENT_ENDED == type))
	{
		GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);
	}


}

void RankingUpLayer::moveItem()
{
	GameSound::playEffectMusic(EFF_RANK_UP);

	Size winSize = Director::getInstance()->getWinSize();
	auto moveUp = MoveTo::create(1.5f, Point(winSize.width/2,winSize.height/4 * 3));

	auto moveDown = MoveTo::create(1.5f, Point(winSize.width/2,winSize.height/3));

	m_me->runAction(moveUp);

	m_friend->runAction(moveDown);
}

void RankingUpLayer::refreshUI()
{
	auto mRank = static_cast<TextAtlas*>(ui::Helper::seekWidgetByName(m_me, "mm_rankAtlas"));

	auto mHead = static_cast<ImageView*>(ui::Helper::seekWidgetByName(m_me, "mm_headImg"));

	auto fName = static_cast<Text*>(ui::Helper::seekWidgetByName(m_friend, "mm_friendName"));

	auto fScore = static_cast<TextAtlas*>(ui::Helper::seekWidgetByName(m_friend, "mm_scoreAtlas"));

	auto fRank = static_cast<TextAtlas*>(ui::Helper::seekWidgetByName(m_friend, "mm_rankAtlas"));

	auto fHead = static_cast<ImageView*>(ui::Helper::seekWidgetByName(m_friend, "mm_headImg"));

	myScore = Player::getInstance()->getScore();

	auto ranklist = Player::getInstance()->getWorldRankList();//getFriendList

	for (auto friendItem : *ranklist)
	{
		if (myScore > (friendItem->score))
		{
			myRank = index;

			mRank->setStringValue(StringUtils::format("%d", myRank));

			fName->setText(friendItem->nickname);

			fScore->setStringValue(StringUtils::format("%d", friendItem->score));

			fRank->setStringValue(StringUtils::format("%d", myRank + 1));

			break;
		}

		index++;
	}

	moveItem();
}