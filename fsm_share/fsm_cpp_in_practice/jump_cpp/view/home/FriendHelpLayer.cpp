////FriendHelpLayer.cpp created by php script. chris.li
#include "FriendHelpLayer.h"
#include "model/Player.h"
#include "model/BattleData.h"
#include "view/battle/BattleScene.h"
#include "network/Network.h"
#include "ItemLayer.h"
#include "util/GameSound.h"
//#include "view/battle/LoadLayer.h"
//
#include "ShopLayer.h"
#include "MailLayer.h"
#include "TaskLayer.h"
#include "RankLayer.h"
#include "NoticeLayer.h"
#include "TipsInfo.h"
#include "MyHelper.h"
#include "../common/TextFloatLabelLayer.h"

bool FriendHelpLayer::init()
{
	if (Layer::init())
	{
		//member variable init
		//Hp
		m_HpNum = 0;
		m_hpLoadingBarV.clear();
		//msg time 
		m_msgTime = 0.0f;
		m_msgSwitch = false;
		m_scrollSwitch = false;
		m_noticeSwitch = true;
		m_msgEndPosX = 0.0f;

		loadUI();
		return true;
	}

	return false;
}

void FriendHelpLayer::onEnter()
{
	Layer::onEnter();

	//network : update task count and mail count
	Network::getInstance()->do_TaskCount_Req([this](const AckInfo &ackInfo){});

	m_friendListItemSelected = nullptr;

	m_friendListItem = static_cast<Layout*>(m_friendListItem->clone());

	CC_SAFE_RETAIN(m_friendListItem);

	refreshUI();

	Network::getInstance()->do_FriendList_Req(2, [this](const AckInfo &ackInfo) {

		std::string msgInfo = ackInfo.getMessageInfo();

		if (msgInfo != "")
		{
			log("[FriendList_Ack] %s", msgInfo.c_str());
		}
		else
		{
			log("[FriendList_Ack] OK!");

			refreshUI();
		}
	});

	//schedule
	this->schedule(schedule_selector(FriendHelpLayer::scheduleHpTime),1.0f);
	//tips
	this->schedule(schedule_selector(FriendHelpLayer::scheduleMsg),TIME_tipsScheduleDelay);

	//refresh
	refreshHp();
	refreshDiamond();
	refreshGold();
	refreshMail();
	refreshTask();

}

void FriendHelpLayer::onExit()
{
	CC_SAFE_RELEASE(m_friendListItem);

	Layer::onExit();
}

void FriendHelpLayer::loadUI()
{
	auto rootNode = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("FriendHelpLayer/FriendHelpLayer.json"));

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

	m_returnBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_returnBtn"));

	m_returnBtn-> addTouchEventListener(this, toucheventselector(FriendHelpLayer::handleTouchEvent));

	m_startGameBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_startGameBtn"));

	m_startGameBtn-> addTouchEventListener(this, toucheventselector(FriendHelpLayer::handleTouchEvent));

	m_friendListScrollView = static_cast<ScrollView*>(ui::Helper::seekWidgetByName(rootNode, "m_friendListScrollView"));

	//m_friendListScrollView-> addTouchEventListener(this, toucheventselector(FriendHelpLayer::handleTouchEvent));

	m_mailBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_mailBtn"));

	m_mailBtn-> addTouchEventListener(this, toucheventselector(FriendHelpLayer::handleTouchEvent));

	m_taskBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_taskBtn"));

	m_taskBtn-> addTouchEventListener(this, toucheventselector(FriendHelpLayer::handleTouchEvent));

	m_rankBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_rankBtn"));

	m_rankBtn-> addTouchEventListener(this, toucheventselector(FriendHelpLayer::handleTouchEvent));

	m_activityBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_activityBtn"));

	m_activityBtn-> addTouchEventListener(this, toucheventselector(FriendHelpLayer::handleTouchEvent));

	m_notfiyMsgLabel = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_notfiyMsgLabel"));

	m_nextPageBtn = static_cast<Slider*>(ui::Helper::seekWidgetByName(rootNode, "m_nextPageBtn"));

	//m_nextPageBtn-> addTouchEventListener(this, toucheventselector(FriendHelpLayer::handleTouchEvent));

	m_friendListView = static_cast<ListView*>(ui::Helper::seekWidgetByName(rootNode, "m_friendListView"));

	//m_friendListView-> addTouchEventListener(this, toucheventselector(FriendHelpLayer::handleTouchEvent));

	m_addStrengthBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_addStrengthBtn"));

	m_addStrengthBtn-> addTouchEventListener(this, toucheventselector(FriendHelpLayer::handleTouchEvent));

	m_strengthMaxNum = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_strengthMaxNum"));

	m_addDiamondBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_addDiamondBtn"));

	m_addDiamondBtn-> addTouchEventListener(this, toucheventselector(FriendHelpLayer::handleTouchEvent));

	m_diamondNum = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_diamondNum"));

	m_addGoldBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_addGoldBtn"));

	m_addGoldBtn-> addTouchEventListener(this, toucheventselector(FriendHelpLayer::handleTouchEvent));

	m_goldNum = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_goldNum"));

	m_friendListItem = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "m_friendListItem"));

	//m_friendListItem-> addTouchEventListener(this, toucheventselector(FriendHelpLayer::handleTouchEvent));

	m_strengthBar1 = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(rootNode, "m_strengthBar1"));
	m_hpLoadingBarV.pushBack(m_strengthBar1);

	m_strengthBar2 = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(rootNode, "m_strengthBar2"));
	m_hpLoadingBarV.pushBack(m_strengthBar2);

	m_strengthBar3 = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(rootNode, "m_strengthBar3"));
	m_hpLoadingBarV.pushBack(m_strengthBar3);

	m_strengthBar4 = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(rootNode, "m_strengthBar4"));
	m_hpLoadingBarV.pushBack(m_strengthBar4);

	m_strengthBar5 = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(rootNode, "m_strengthBar5"));
	m_hpLoadingBarV.pushBack(m_strengthBar5);

	m_mailNumText = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_mailNum"));

	m_taskNumText = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_taskNum"));

	//msg
	m_notifyMsgLabel = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_notfiyMsgLabel"));

	//************* event listener ************************
	//event listener shop layer return call-back
	auto _listenerShop = EventListenerCustom::create("Return_shopLayer", [this](EventCustom* event) {
		log("AAAAAAAAAAAAAAA Friend helper layer");
		refreshHp();
		refreshGold();
		refreshDiamond();
	});

	_eventDispatcher->addEventListenerWithSceneGraphPriority(_listenerShop, this);

	//event listener mail layer return call-back
	auto _listenerMail = EventListenerCustom::create("Return_mailLayer", [this](EventCustom* event) {
		log("BBBBBBBBBBBBBBB Friend helper layer");
		refreshMail();
		refreshHp();
		refreshGold();
		refreshDiamond();
	});

	_eventDispatcher->addEventListenerWithSceneGraphPriority(_listenerMail, this);

	auto _listenerTask = EventListenerCustom::create("Return_taskLayer", [this](EventCustom* event) {
		log("CCCCCCCCCCCCCC Friend helper layer");
		refreshTask();
		refreshGold();
		refreshDiamond();
	});

	_eventDispatcher->addEventListenerWithSceneGraphPriority(_listenerTask, this);
}

void FriendHelpLayer::handleTouchEvent(Ref *pSender, TouchEventType type)
{

	if (pSender == nullptr)
	{
	}

	else if (pSender == m_returnBtn && (TOUCH_EVENT_ENDED == type))
	{
		GameSound::playEffectMusic(EFF_CLOSE_PAGE);

		std::string eventName = "Return_friendHelpLayer";
		EventCustom event(eventName);
		Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);

		this->removeFromParentAndCleanup(true);
	}

	else if (pSender == m_startGameBtn && (TOUCH_EVENT_ENDED == type))
	{
		GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);
		// when selected friend help
		if (m_friendListItemSelected)
		{
			auto selectedFriend = static_cast<FriendListItem*>(m_friendListItemSelected->getUserData());

			if (selectedFriend->playerId != Player::getInstance()->getMyPlayerId())
			{
				BattleData::getInstance()->setHelperPlayerId(selectedFriend->playerId);

				log("[FriendHelpLayer] selected friend %d", selectedFriend->playerId);
			}
			else
			{
				BattleData::getInstance()->setHelperPlayerId(0);
			}
		}

		auto t_itemLayer = ItemLayer::create();

		int t_zOrder = this->getLocalZOrder();

		t_itemLayer->setLocalZOrder(t_zOrder+1);

		this->getParent()->addChild(t_itemLayer);

		//BattleData::getInstance()->setBattleLevel(1);

		//auto scene = BattleScene::create();

		//Director::getInstance()->replaceScene(scene);
	}

	else if (pSender == m_friendListScrollView && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == m_mailBtn && (TOUCH_EVENT_ENDED == type))
	{
		auto mailLayer = MailLayer::create();

		int _zOrder = this->getLocalZOrder();

		mailLayer->setLocalZOrder(_zOrder+1);

		this->getParent()->addChild(mailLayer);
	}

	else if (pSender == m_taskBtn && (TOUCH_EVENT_ENDED == type))
	{
		auto taskLayer = TaskLayer::create();

		int _zOrder = this->getLocalZOrder();

		taskLayer->setLocalZOrder(_zOrder+1);

		this->getParent()->addChild(taskLayer);
	}

	else if (pSender == m_rankBtn && (TOUCH_EVENT_ENDED == type))
	{
		RankLayer* t_rankLayer = RankLayer::create();

		int t_zOrder = this->getLocalZOrder();

		t_rankLayer->setLocalZOrder(t_zOrder+1);

		this->getParent()->addChild(t_rankLayer);
	}

	else if (pSender == m_activityBtn && (TOUCH_EVENT_ENDED == type))
	{
		auto noticeLayer = NoticeLayer::create();

		int t_zOrder = this->getLocalZOrder();

		noticeLayer->setLocalZOrder(t_zOrder+1);

		this->getParent()->addChild(noticeLayer);
	}

	else if (pSender == m_nextPageBtn && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == m_friendListView && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == m_addStrengthBtn && (TOUCH_EVENT_ENDED == type))
	{
		GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);

		ShopLayer* t_ShopLayer = ShopLayer::create();

		int t_zOrder = this->getLocalZOrder();

		t_ShopLayer->setLocalZOrder(t_zOrder+1);

		this->getParent()->addChild(t_ShopLayer);

		t_ShopLayer->initShopType(TYPE_shopHp);
	}

	else if (pSender == m_addDiamondBtn && (TOUCH_EVENT_ENDED == type))
	{
		GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);

		ShopLayer* t_ShopLayer = ShopLayer::create();

		int t_zOrder = this->getLocalZOrder();

		t_ShopLayer->setLocalZOrder(t_zOrder+1);

		this->getParent()->addChild(t_ShopLayer);

		t_ShopLayer->initShopType(TYPE_shopDiamond);
	}

	else if (pSender == m_addGoldBtn && (TOUCH_EVENT_ENDED == type))
	{
		GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);

		ShopLayer* t_ShopLayer = ShopLayer::create();

		int t_zOrder = this->getLocalZOrder();

		t_ShopLayer->setLocalZOrder(t_zOrder+1);

		this->getParent()->addChild(t_ShopLayer);

		t_ShopLayer->initShopType(TYPE_shopGold);
	}

	else if ((TOUCH_EVENT_ENDED == type))
	{
		auto selectedIndex = m_friendListView->getCurSelectedIndex();

		auto selectedItem = m_friendListView->getItem(selectedIndex);

		FriendListItem* frindItem = static_cast<FriendListItem*>(selectedItem->getUserData());

		if(frindItem->helpTimes<5)
		{
			if (m_friendListItemSelected)
			{
				auto clickedImg1 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(m_friendListItemSelected, "mm_clickedImg"));

				clickedImg1->setVisible(false);
			}

			if(m_friendListItemSelected == static_cast<Layout*>(selectedItem))
			{
				m_friendListItemSelected = nullptr;
			}
			else
			{

				auto clickedImg2 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(selectedItem, "mm_clickedImg"));

				clickedImg2->setVisible(true);

				m_friendListItemSelected = static_cast<Layout*>(selectedItem);

			}
		}

	}
}


void FriendHelpLayer::refreshUI()
{
	auto friendList = Player::getInstance()->getFriendList();

	m_friendListView->removeAllItems();

	int _itemCount = 0;

	for (auto friendListItem : (*friendList))
	{
		if (Player::getInstance()->getMyPlayerId() != friendListItem->playerId&&friendListItem->helpTimes<5)
		{
			auto item = m_friendListItem->clone();

			item->setVisible(true);

			item-> addTouchEventListener(this, toucheventselector(FriendHelpLayer::handleTouchEvent));

			item->setUserData(friendListItem);

			auto headImg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(item, "mm_headImg"));

			auto heroImg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(item, "mm_heroImg"));

			int _roleType = friendListItem->roleType;

			if(_roleType<4)
				_roleType = 1;
			else if(_roleType<8)
				_roleType = 4;

			std::string _pathStr = StringUtils::format("commonUi/imgRole_%d.png",_roleType);
			heroImg->loadTexture(_pathStr);

			auto helpCostFreeImg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(item, "Image_free"));
			helpCostFreeImg->setVisible(false);

			auto clickedImg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(item, "mm_clickedImg"));

			auto chooseBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(item, "mm_chooseBtn"));

			auto helpCostDiamondImg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(item, "mm_helpCostDiamondImg"));

			auto helpCostCoinImg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(item, "mm_helpCostCoinImg"));

			auto costNum = static_cast<Text*>(ui::Helper::seekWidgetByName(item, "mm_costNum"));

			if (friendListItem->roleType > 3)
			{
				//if(friendListItem->helpTimes<5)
				//{
				helpCostCoinImg->setVisible(false);

				helpCostDiamondImg->setVisible(true);

				costNum->setVisible(true);

				costNum->setText("10");
				//}
				//else
				//{
				//	item->getChildByName("Image_97")->setColor(ccc3(80,80,80));
				//}
			}
			else
			{
				if(Player::getInstance()->getHelpCount())
				{
					//if(friendListItem->helpTimes<5)
					//{
					helpCostFreeImg->setVisible(true);
					//}
					//else
					//{
					//	item->getChildByName("Image_97")->setColor(ccc3(80,80,80));
					//}
				}
				else
				{

					if(friendListItem->helpTimes==0)
					{
						helpCostCoinImg->setVisible(true);

						helpCostDiamondImg->setVisible(false);

						int _level = friendListItem->level;

						int _cost = 1;

						if(_level)
							_cost = 10*_level;

						std::string _costStr = StringUtils::format("%d",_cost);

						costNum->setVisible(true);

						costNum->setText(_costStr);
					}
					else if(friendListItem->helpTimes<5)
					{
						helpCostCoinImg->setVisible(false);

						helpCostDiamondImg->setVisible(true);

						costNum->setVisible(true);

						costNum->setText("10");
					}
					//else 
					//{
					//	item->getChildByName("Image_97")->setColor(ccc3(80,80,80));
					//}
				}
			}

			auto friendName = static_cast<Text*>(ui::Helper::seekWidgetByName(item, "mm_friendName"));

			auto score = static_cast<TextAtlas*>(ui::Helper::seekWidgetByName(item, "mm_scoreAtlasLabel"));

			friendName->setText(friendListItem->nickname);

			score->setStringValue(MyHelper::changeToScoreStringCenter(friendListItem->score));

			m_friendListView->pushBackCustomItem(item);

			++_itemCount;
		}
	}

	//offset  the list gap
	int _offsetCount = 6 - _itemCount;

	if(_offsetCount>0)
	{
		for (int i = 0; i < _offsetCount; i++)
		{
			ImageView* _listFrameS = ImageView::create("commonUi/listFrameS.png");

			m_friendListView->pushBackCustomItem(_listFrameS);
		}
	}
}

//**********************************schedule******************************
void FriendHelpLayer::scheduleHpTime(float delayTime)
{
	float _remainTime = Player::getInstance()->getRemainTime();

	if(_remainTime>0)
	{
		//log("~~~~~lalal time : %f ",m_Hptime);
		int t_time = static_cast<int>(_remainTime);
		String* t_timeText = 0;
		if(t_time%60 >9)
			t_timeText = String::createWithFormat("%d:%d",static_cast<int>(t_time/60),t_time%60);
		else
			t_timeText = String::createWithFormat("%d:0%d",static_cast<int>(t_time/60),t_time%60);

		m_strengthMaxNum->setText(t_timeText->getCString());

		if(m_HpNum<5)
			m_hpLoadingBarV.at(m_HpNum)->setPercent(100.0f*(1.0f-_remainTime/Player::getInstance()->getMaxTime()));
	}
	if(Player::getInstance()->getIsStarUpdate())
		refreshHp();
}

void FriendHelpLayer::scheduleMsg(float delayTime)
{

	if(m_msgTime>TIME_tipsDelay)
	{
		m_msgSwitch = true;
	}

	if(m_msgSwitch)
	{
		refreshMsg();
		m_msgSwitch = false;
		m_scrollSwitch = true;
		m_msgTime = 0.0f;
	}

	if(m_scrollSwitch)
	{
		Point _msgPos = m_notifyMsgLabel->getPosition();
		if(_msgPos.x>m_msgEndPosX)
		{
			m_notifyMsgLabel->setPosition(ccp(_msgPos.x-SPEED_tipsScroll,_msgPos.y));
		}
		else
			m_scrollSwitch = false;
	}
	else
	{
		m_msgTime += delayTime;
	}
}

//***************************************refresh***************************
void FriendHelpLayer::refreshHp()
{
	m_HpNum = Player::getInstance()->getStar();

	//error
	if(m_HpNum<0)
	{
		m_HpNum = 0;
		log("[HpNum]: hp number is error");
	}
	else if (m_HpNum<5)
	{
		float _Hptime = Player::getInstance()->getRemainTime();

		for(int num = 0 ; num < 5 ; ++num)
		{
			if(num<	m_HpNum)
				m_hpLoadingBarV.at(num)->setPercent(100);
			else
				m_hpLoadingBarV.at(num)->setPercent(0);

			log("~~~~~~Percent : %d ",m_hpLoadingBarV.at(num)->getPercent());
		}
		m_hpLoadingBarV.at(m_HpNum)->setPercent(100.0f*(1.0f-_Hptime/Player::getInstance()->getMaxTime()));			//Percent of get HP time

		//set time Text
		int t_time = static_cast<int>(_Hptime);
		String* t_timeText = 0;
		if(t_time%60>9)
			t_timeText = String::createWithFormat("%d:%d",static_cast<int>(t_time/60),t_time%60);
		else
			t_timeText = String::createWithFormat("%d:0%d",static_cast<int>(t_time/60),t_time%60);

		m_strengthMaxNum->setText(t_timeText->getCString());
	}
	else if(5==m_HpNum)
	{
		for(int num = 0 ; num < 5 ; ++num)
		{
			m_hpLoadingBarV.at(num)->setPercent(100);
		}

		m_strengthMaxNum->setText("MAX");
	}
	else if(m_HpNum>5)
	{
		for(int num = 0 ; num < 5 ; ++num)
		{
			m_hpLoadingBarV.at(num)->setPercent(100);
		}

		std::string t_hpNumStr = StringUtils::format("+%d",m_HpNum-5);
		m_strengthMaxNum->setText(t_hpNumStr.c_str());
	}

	//switch
	Player::getInstance()->setIsStarUpdate(false);

}
void FriendHelpLayer::refreshDiamond()
{
	int t_diamond = Player::getInstance()->getDiamond();
	if(t_diamond<0)
	{
		log("~~error~~ diamond num is error");
		t_diamond = 0;
	}
	std::string diamondNumStr = StringUtils::format("%d", t_diamond);	//CW
	m_diamondNum->setText(diamondNumStr.c_str());
}
void FriendHelpLayer::refreshGold()
{
	int t_gold = Player::getInstance()->getCoin();
	if(t_gold<0)
	{
		log("~~error~~ gold num is error");
		t_gold = 0;
	}
	std::string coinStr = StringUtils::format("%d", t_gold);//CW
	m_goldNum->setText(coinStr.c_str());
}

void FriendHelpLayer::refreshMail()
{
	int t_mailNum = Player::getInstance()->getMailCount();
	if(t_mailNum==0)
	{
		m_mailNumText->getParent()->setVisible(false);
	}
	else if(t_mailNum>0)
	{
		m_mailNumText->getParent()->setVisible(true);
		std::string mailNumStr = StringUtils::format("%d", t_mailNum);
		m_mailNumText->setText(mailNumStr.c_str());
	}
	else
	{
		log("mail number is error ~");
		m_mailNumText->getParent()->setVisible(false);
	}
}
void FriendHelpLayer::refreshTask()
{
	int t_taskNum = Player::getInstance()->getTaskCount();

	if(t_taskNum==0)
	{
		m_taskNumText->getParent()->setVisible(false);
	}
	else if(t_taskNum>0)
	{
		m_taskNumText->getParent()->setVisible(true);
		std::string taskNumStr = StringUtils::format("%d", t_taskNum);
		m_taskNumText->setText(taskNumStr.c_str());
	}
	else
	{
		log("task number is error ~!");
		m_taskNumText->getParent()->setVisible(false);
	}
}

void FriendHelpLayer::refreshMsg()
{
	if(m_noticeSwitch)
	{
		m_notifyMsgLabel->setVisible(true);
		m_noticeSwitch = false;
		if(Player::getInstance()->getContent().size()==0)
		{
			int _rand = static_cast<int>(CCRANDOM_0_1() * COUNT_tips);
			m_notifyMsgLabel->setText(TipsInfo::getInstance()->getTips(_rand));
		}
		else
		{
			m_notifyMsgLabel->setText(Player::getInstance()->getLoopContent());
		}
	}
	else
	{
		int _rand = static_cast<int>(CCRANDOM_0_1() * COUNT_tips);
		m_notifyMsgLabel->setText(TipsInfo::getInstance()->getTips(_rand));
	}

	Point _msgBeginPos = ccp(m_notifyMsgLabel->getParent()->getContentSize().width+20,m_notifyMsgLabel->getPosition().y);		//+20 is adjusting

	m_msgEndPosX = 0-m_notifyMsgLabel->getContentSize().width-20;		//-20 is adjusting

	m_notifyMsgLabel->setPosition(_msgBeginPos);
}