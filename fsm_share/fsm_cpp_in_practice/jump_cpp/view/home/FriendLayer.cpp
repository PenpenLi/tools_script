#include "FriendLayer.h"
#include "ShopLayer.h"
#include "UiConfig.h"
#include "Network.h"
#include "Player.h"
#include "util/GameSound.h"

#include "FriendListItem.h"
#include "MyHelper.h"
//
#include "TipsInfo.h"
#include "MailLayer.h"
#include "RankLayer.h"
#include "TaskLayer.h"
#include "PlatformUtil.h"
#include "NoticeLayer.h"

bool FriendLayer::init()
{
	if (Layer::init())
	{
		//member variable init
		variableInit();
		//Ui res
		importUI();	


		//test
		//t_touchMoveDir = -1;
		return true;
	}

	return false;
}

void FriendLayer::onEnter()
{
	Layer::onEnter();

	//network : update task count and mail count
	Network::getInstance()->do_TaskCount_Req([this](const AckInfo &ackInfo){});

	//update
	updateHp();
	updateDiamond();
	updateGold();
	updateMail();
	updateTask();
	//updateMsg();

	//get friend list
	//Network::getInstance()->do_FriendList_Req(1);

	if(Player::getInstance()->getFriendList()->size())
		updateFriend();
	else
		Network::getInstance()->do_FriendList_Req(2,[this](const AckInfo &ackInfo) 
	{
		std::string msgInfo = ackInfo.getMessageInfo();

		if (msgInfo != "")
		{
			log("[FriendList] %s", msgInfo.c_str());
		}
		else
		{
			//ok
			log("[FriendList] : success~~!!");
			updateFriend();
		}
	});

	//friend list
	//updateFriend();

	//schedule
	this->schedule(schedule_selector(FriendLayer::scheduleHpTime),1.0f);
	//tips
	this->schedule(schedule_selector(FriendLayer::scheduleMsg),TIME_tipsScheduleDelay);

	this->scheduleUpdate();

	// added by chris.li
	auto listener = EventListenerCustom::create("EVENT_LBS_DATA", [this](EventCustom* event)
	{
		PlatformUtil::LBS_stop();

		Point *lbsData = static_cast<Point*>(event->getUserData());

		log("LBS_DATA {%f, %f}", lbsData->x, lbsData->y);

		Network::getInstance()->do_FriendList_Req(3,[this](const AckInfo &ackInfo)
		{
			std::string msgInfo = ackInfo.getMessageInfo();

			if (msgInfo != "")
			{
				log("[NearbyList] %s", msgInfo.c_str());
			}
			else
			{
				//ok
				log("[NearbyList] : success~~!!");
				//Button controll
				m_friendBtn->setLocalZOrder(ZORDER_behind);
				m_platformFriendBtn->setLocalZOrder(ZORDER_behind);
				m_friendBtn->setOnPressEnable(true);
				m_platformFriendBtn->setOnPressEnable(true);
				//listView controll
				m_friendLvp->setVisible(false);
				m_nearbyFriendLvp->setVisible(true);
				m_platformFriendLvp->setVisible(false);

				m_page = 1;
				updateNearbyFriend();
			}
		});

	});
	_eventDispatcher->addEventListenerWithFixedPriority(listener, 1);

}

void FriendLayer::onExit()
{
	Layer::onExit();

	//schedule
	this->unscheduleUpdate();
	this->unschedule(schedule_selector(FriendLayer::scheduleHpTime));
	this->unschedule(schedule_selector(FriendLayer::scheduleMsg));

	//remove friend list 
	_eventDispatcher->removeCustomEventListeners("EVENT_LBS_DATA");
}

//init
void FriendLayer::variableInit()
{
	m_hpLoadingBarV.clear();

	m_hpTimeText = 0;
	m_hpAddBtn = 0;
	m_diamondNumText = 0;
	m_diamondAddBtn = 0;
	m_goldNumText = 0;
	m_goldAddBtn = 0;

	m_mailBtn = 0;
	m_mailNumText = 0;
	m_taskBtn = 0;
	m_taskNumText = 0;
	m_rankBtn = 0;
	m_activityBtn = 0;

	m_lvpSlider = 0;
	m_friendBtn = 0;
	m_nearbyFriendBtn = 0;
	m_platformFriendBtn =0;
	m_friendLvp = 0;
	m_nearbyFriendLvp = 0;
	m_platformFriendLvp = 0;

	m_friendSinglePal = 0;
	m_friendDualPal = 0;
	m_nearbyFriendSinglePal = 0;
	m_nearbyFriendDualPal = 0;
	m_platformFriendSinglePal = 0;
	m_platformFriendDualPal = 0;

	m_infoPlayerId = 0;
	m_friendInfoPal = 0;
	m_playerHeadImg = 0;
	m_scoreText = 0;
	m_levelsText = 0;
	m_addFriendBtn = 0;
	m_deleteFriendBtn = 0;

	//m_friendPage = 0;
	//m_friendPageSum = 0;
	//m_page = 0;
	//m_pageSum = 0;
	//m_page = 0;
	//m_pageSum = 0;
	//common page
	m_page = 1;
	m_pageSum = 0;

	m_returnBtn = 0;

	m_msgText = 0;

	m_Hptime = 0;
	m_HpNum = 0;

	m_touchSwitch = true;
	m_sliderSwitch = false;

	m_sliderGapMax = 0;
	m_sliderGap = 0;

	//msg time 
	m_msgTime = 0.0f;
	m_msgSwitch = false;
	m_scrollSwitch = false;
	m_noticeSwitch = true;
	m_msgEndPosX = 0.0f;
	//friend number label
	m_friendNumText = nullptr;

}

bool FriendLayer::importUI()
{
	bool bRet = false;
	do{
		Widget* t_widget = GUIReader::getInstance()->widgetFromJsonFile("FriendUi/FriendUi.json");
		this->addChild(static_cast<Layout*>(t_widget));

		//add layer bg and bubble
		Sprite* _bg = Sprite::create("commonUi/layerBg.jpg");
		_bg->setAnchorPoint(Point::ZERO);
		_bg->setPosition(Point::ZERO);
		_bg->setLocalZOrder(-1);
		t_widget->addChild(_bg);

		Sprite* _bubble = Sprite::create("commonUi/bubble.png");
		Size _winSize = this->getContentSize();
		_bubble->setPosition(ccp(_winSize.width/2,_winSize.height/2));
		_bubble->setLocalZOrder(-1);
		t_widget->addChild(_bubble);

		//~Panel Assets~
		Widget* t_widgetPalAssets = t_widget->getChildByName("pal_assets");

		//HP
		Widget* t_widgetPalAssetsHp = t_widgetPalAssets->getChildByName("img_hpFrameBg");
		for(int num = 1 ; num<6 ; ++num)
		{
			//get Hp LoadingBar
			String* t_loadingBarBgStr = String::createWithFormat("img_hpBg_%d",num);
			String* t_loadingBarStr = String::createWithFormat("pgb_hp_%d",num);
			LoadingBar* t_progressTimer = static_cast<LoadingBar*>(t_widgetPalAssetsHp->getChildByName(t_loadingBarBgStr->getCString())->getChildByName(t_loadingBarStr->getCString()));
			m_hpLoadingBarV.pushBack(t_progressTimer);
		}
		m_hpTimeText = static_cast<Text*>(t_widgetPalAssetsHp->getChildByName("lab_hpTime"));
		m_hpAddBtn = static_cast<Button*>(t_widgetPalAssetsHp->getChildByName("btn_addHp"));
		if (m_hpAddBtn)
		{
			m_hpAddBtn->setTag(TAG_shopHpBtn);
			m_hpAddBtn->addTouchEventListener(this, toucheventselector(FriendLayer::touchEventShopBtn));
		}
		else
			log("~~error~~ add Hp button is Null ");

		//diamond
		Widget* t_widgetPalAssetsDiamond = t_widgetPalAssets->getChildByName("img_diamondFrameBg");
		m_diamondNumText = static_cast<Text*>(t_widgetPalAssetsDiamond->getChildByName("lab_diamondNum"));
		m_diamondAddBtn = static_cast<Button*>(t_widgetPalAssetsDiamond->getChildByName("btn_addDiamond"));
		if (m_diamondAddBtn)
		{
			m_diamondAddBtn->setTag(TAG_shopDiamondBtn);
			m_diamondAddBtn->addTouchEventListener(this, toucheventselector(FriendLayer::touchEventShopBtn));
		}
		else
			log("~~error~~ add Diamond button is Null ");

		//gold
		Widget* t_widgetPalAssetsGold = t_widgetPalAssets->getChildByName("img_goldFrameBg");
		m_goldNumText = static_cast<Text*>(t_widgetPalAssetsGold->getChildByName("lab_goldNum"));
		m_goldAddBtn = static_cast<Button*>(t_widgetPalAssetsGold->getChildByName("btn_addGold"));
		if (m_goldAddBtn)
		{
			m_goldAddBtn->setTag(TAG_shopGoldBtn);
			m_goldAddBtn->addTouchEventListener(this, toucheventselector(FriendLayer::touchEventShopBtn));
		}
		else
			log("~~error~~ add Gold button is Null ");

		//~Panel Activity~
		Widget* t_widgetPalAct = t_widget->getChildByName("pal_btnAct");

		//mail
		m_mailNumText = static_cast<Text*>(t_widgetPalAct->getChildByName("btn_mail")->getChildByName("img_mailNumBg")->getChildByName("lab_mailNum"));
		m_mailBtn = static_cast<Button*>(t_widgetPalAct->getChildByName("btn_mail"));
		if (m_mailBtn)
		{
			m_mailBtn->addTouchEventListener(this, toucheventselector(FriendLayer::touchEventMail));
		}
		else
			log("~~error~~ Mail button is Null ");

		//task
		m_taskNumText = static_cast<Text*>(t_widgetPalAct->getChildByName("btn_task")->getChildByName("img_taskNumBg")->getChildByName("lab_taskNum"));
		m_taskBtn = static_cast<Button*>(t_widgetPalAct->getChildByName("btn_task"));
		if (m_taskBtn)
		{
			m_taskBtn->addTouchEventListener(this, toucheventselector(FriendLayer::touchEventTask));
		}
		else
			log("~~error~~ Task button is Null ");

		//rank
		m_rankBtn = static_cast<Button*>(t_widgetPalAct->getChildByName("btn_rank"));
		if (m_rankBtn)
		{
			m_rankBtn->addTouchEventListener(this, toucheventselector(FriendLayer::touchEventRank));
		}
		else
			log("~~error~~ Rank button is Null ");

		//act
		m_activityBtn = static_cast<Button*>(t_widgetPalAct->getChildByName("btn_activity"));
		if (m_activityBtn)
		{
			m_activityBtn->addTouchEventListener(this, toucheventselector(FriendLayer::touchEventActivity));
		}
		else
			log("~~error~~ Activity button is Null ");

		//friend
		Widget* t_friendFrameWidget = t_widget->getChildByName("pal_friendFrame");

		m_lvpSlider = static_cast<Slider*>(t_friendFrameWidget->getChildByName("sdr_friend"));
		m_lvpSlider->setTouchEnabled(false);
		m_lvpSlider->setVisible(false);

		Widget* t_friendBtnWidget = t_friendFrameWidget->getChildByName("img_friendFrameBg");

		m_friendBtn = static_cast<MyButton*>(t_friendBtnWidget->getChildByName("btn_friendList"));
		if (m_friendBtn)
		{
			//init
			m_friendBtn->setLocalZOrder(ZORDER_front);
			m_friendBtn->setOnPressEnable(false);

			m_friendBtn->setTag(TAG_friendBtn);
			m_friendBtn->addTouchEventListener(this, toucheventselector(FriendLayer::touchEventFriendBtn));
		}
		else
			log("~~error~~ Friend button is Null ");

		m_nearbyFriendBtn = static_cast<MyButton*>(t_friendBtnWidget->getChildByName("btn_nearbyFriend"));
		if (m_nearbyFriendBtn)
		{
			m_nearbyFriendBtn->setTag(TAG_nearbyFriendBtn);
			m_nearbyFriendBtn->addTouchEventListener(this, toucheventselector(FriendLayer::touchEventFriendBtn));
		}
		else
			log("~~error~~Nearby Friend button is Null ");

		m_platformFriendBtn = static_cast<MyButton*>(t_friendBtnWidget->getChildByName("btn_platformFriend"));
		if (m_platformFriendBtn)
		{
			m_platformFriendBtn->setEnabled(false);

			m_platformFriendBtn->setTag(TAG_platformFriendBtn);
			m_platformFriendBtn->addTouchEventListener(this, toucheventselector(FriendLayer::touchEventFriendBtn));
		}
		else
			log("~~error~~Platform Friend button is Null ");

		m_friendSvp = static_cast<ScrollView*>(t_friendFrameWidget->getChildByName("svp_friend"));

		m_friendLvp = static_cast<MyListView*>(m_friendSvp->getChildByName("lvp_friendList"));

		m_nearbyFriendLvp = static_cast<MyListView*>(m_friendSvp->getChildByName("lvp_nearbyFriend"));

		m_platformFriendLvp = static_cast<MyListView*>(m_friendSvp->getChildByName("lvp_platformFriend"));

		m_friendLvp->setLocalZOrder(ZORDER_front);
		m_friendLvp->setTag(TAG_friendLvp);
		m_friendLvp->addTouchEventListener(this, toucheventselector(FriendLayer::listenEventFriendLvp));

		m_nearbyFriendLvp->setTag(TAG_nearbyFriendLvp);
		m_nearbyFriendLvp->addTouchEventListener(this, toucheventselector(FriendLayer::listenEventFriendLvp));

		m_platformFriendLvp->setTag(TAG_platformFriendLvp);
		m_platformFriendLvp->addTouchEventListener(this, toucheventselector(FriendLayer::listenEventFriendLvp));

		//model
		m_friendSinglePal = static_cast<Layout*>(t_widget->getChildByName("pal_friendSingle"));
		m_friendDualPal = static_cast<Layout*>(t_widget->getChildByName("pal_friendDual"));
		m_nearbyFriendSinglePal = static_cast<Layout*>(t_widget->getChildByName("pal_nearbyFriendSingle"));
		m_nearbyFriendDualPal = static_cast<Layout*>(t_widget->getChildByName("pal_nearbyFriendDual"));
		m_platformFriendSinglePal = static_cast<Layout*>(t_widget->getChildByName("pal_platformFriendSingle"));
		m_platformFriendDualPal = static_cast<Layout*>(t_widget->getChildByName("pal_platformFriendDual"));

		//friend info
		m_friendInfoPal = static_cast<Layout*>(t_widget->getChildByName("pal_roleInfo"));
		m_friendInfoPal->setVisible(true);
		m_friendInfoPal->setEnabled(false);
		//friend info return
		m_friendInfoPal->getChildByName("img_roleFrame")->getChildByName("btn_return")->addTouchEventListener(this, toucheventselector(FriendLayer::touchEventInfoReturn));

		Widget* t_roleFrameWdt = m_friendInfoPal->getChildByName("img_roleFrame");

		m_playerHeadImg = static_cast<ImageView*>(t_roleFrameWdt->getChildByName("img_palyerHead"));

		m_scoreText = static_cast<TextAtlas*>(t_roleFrameWdt->getChildByName("labAls_score"));

		m_levelsText = static_cast<TextAtlas*>(t_roleFrameWdt->getChildByName("labAls_levels"));

		m_addFriendBtn = static_cast<Button*>(t_roleFrameWdt->getChildByName("btn_addFriend"));
		if(m_addFriendBtn)
		{
			m_addFriendBtn->setTag(TAG_addfriendBtn);
			m_addFriendBtn->setLocalZOrder(ZORDER_behind);
			//m_addFriendBtn->setVisible(true);
			//m_addFriendBtn->setEnabled(false);
			m_addFriendBtn->addTouchEventListener(this, toucheventselector(FriendLayer::touchEventFriendInfoBtn));
		}
		else
			log("addFriendBtn is null");

		m_deleteFriendBtn = static_cast<Button*>(t_roleFrameWdt->getChildByName("btn_deleteFriend")); 
		if(m_deleteFriendBtn)
		{
			m_deleteFriendBtn->setTag(TAG_deletefriendBtn);
			m_deleteFriendBtn->setLocalZOrder(ZORDER_behind);
			//m_deleteFriendBtn->setVisible(true);
			//m_deleteFriendBtn->setEnabled(false);
			m_deleteFriendBtn->addTouchEventListener(this, toucheventselector(FriendLayer::touchEventFriendInfoBtn));
		}
		else
			log("deleteFriendBtn is null");

		//Msg text
		m_msgText = static_cast<Text*>(t_widget->getChildByName("img_msgBg")->getChildByName("svp_msg")->getChildByName("lab_msg"));

		//friend number label
		m_friendNumText = static_cast<Text*>(Helper::seekWidgetByName(t_widget,"lab_friendNum"));

		//return
		m_returnBtn = static_cast<Button*>(t_widget->getChildByName("btn_return"));
		if (m_returnBtn)
		{
			//m_returnBtn->setEnabled(false);
			m_returnBtn->addTouchEventListener(this, toucheventselector(FriendLayer::touchEventReturn));
		}
		else
			log("~~error~~ Return button is Null ");

		//************* event listener ************************
		//event listener shop layer return call-back
		auto _listenerShop = EventListenerCustom::create("Return_shopLayer", [this](EventCustom* event) {
			log("AAAAAAAAAAAAAAA Friend layer");
			updateHp();
			updateGold();
			updateDiamond();
		});
		//_listenerShop->set
		_eventDispatcher->addEventListenerWithSceneGraphPriority(_listenerShop, this);

		//event listener mail layer return call-back
		auto _listenerMail = EventListenerCustom::create("Return_mailLayer", [this](EventCustom* event) {
			log("BBBBBBBBBBBBBBB Friend layer");
			updateMail();
			updateHp();
			updateGold();
			updateDiamond();
		});

		_eventDispatcher->addEventListenerWithSceneGraphPriority(_listenerMail, this);

		auto _listenerTask = EventListenerCustom::create("Return_taskLayer", [this](EventCustom* event) {
			log("CCCCCCCCCCCCCC Friend layer");
			updateTask();
			updateGold();
			updateDiamond();
		});

		_eventDispatcher->addEventListenerWithSceneGraphPriority(_listenerTask, this);

		//
		//m_friendSvp->getInnerContainer()->setsize
		bRet = true;
	}while(0);

	return bRet;
}

//**********************************touch event*******************************
//~~~Button~~~ 
//Assets
void FriendLayer::touchEventShopBtn(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);
	Button* t_senderBtn = static_cast<Button*>(pSender);
	ShopLayer *t_ShopLayer = 0;
	int t_zOrder = 0;
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		log("~~~~~enter shop!!");
		t_ShopLayer = ShopLayer::create();

		t_ShopLayer->setAnchorPoint(Point::ZERO);

		t_ShopLayer->setPosition(0, 0);

		t_zOrder = this->getLocalZOrder();

		t_ShopLayer->setLocalZOrder(t_zOrder+1);

		this->getParent()->addChild(t_ShopLayer);

		switch (t_senderBtn->getTag())
		{
		case TAG_shopHpBtn:
			t_ShopLayer->initShopType(TYPE_shopHp);
			break;
		case TAG_shopDiamondBtn:
			t_ShopLayer->initShopType(TYPE_shopDiamond);
			break;
		case TAG_shopGoldBtn:
			t_ShopLayer->initShopType(TYPE_shopGold);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

//Activity
void FriendLayer::touchEventMail(Ref *pSender, TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		log("~~~~~enter Mail Scene!!");
		auto mailLayer = MailLayer::create();

		int _zOrder = this->getLocalZOrder();

		mailLayer->setLocalZOrder(_zOrder+1);

		this->getParent()->addChild(mailLayer);
		break;
	}
}

void FriendLayer::touchEventTask(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		log("~~~~~enter Task Scene!!");

		auto taskLayer = TaskLayer::create();

		int _zOrder = this->getLocalZOrder();

		taskLayer->setLocalZOrder(_zOrder+1);

		this->getParent()->addChild(taskLayer);
		break;
	}
}

void FriendLayer::touchEventRank(Ref *pSender, TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		log("~~~~~enter Rank Scene!!");

		RankLayer* t_rankLayer = RankLayer::create();

		int t_zOrder = this->getLocalZOrder();

		t_rankLayer->setLocalZOrder(t_zOrder+1);

		this->getParent()->addChild(t_rankLayer);

		break;
	}
}

void FriendLayer::touchEventActivity(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		log("~~~~~enter Activity Scene!!");

		auto noticeLayer = NoticeLayer::create();

		int t_zOrder = this->getLocalZOrder();

		noticeLayer->setLocalZOrder(t_zOrder+1);

		this->getParent()->addChild(noticeLayer);
		break;
	}
}

//friend
void FriendLayer::touchEventFriendBtn(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);
	MyButton* t_senderBtn = static_cast<MyButton*>(pSender);
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		switch (t_senderBtn->getTag())
		{
		case TAG_friendBtn:
			//Button controll
			m_nearbyFriendBtn->setLocalZOrder(ZORDER_behind);
			m_platformFriendBtn->setLocalZOrder(ZORDER_behind);
			m_nearbyFriendBtn->setOnPressEnable(true);
			m_platformFriendBtn->setOnPressEnable(true);
			//listView controll
			//m_friendLvp->setLocalZOrder(ZORDER_front);
			m_friendLvp->setVisible(true);
			//m_nearbyFriendLvp->setLocalZOrder(ZORDER_behind);
			m_nearbyFriendLvp->setVisible(false);
			//m_platformFriendLvp->setLocalZOrder(ZORDER_behind);
			m_platformFriendLvp->setVisible(false);

			m_page = 1;
			//m_pageSum = static_cast<int>(22/10) + 1; //CW
			updateFriend();

			break;
		case TAG_nearbyFriendBtn:

			//Button controll
			m_friendBtn->setLocalZOrder(ZORDER_behind);
			m_platformFriendBtn->setLocalZOrder(ZORDER_behind);
			m_friendBtn->setOnPressEnable(true);
			m_platformFriendBtn->setOnPressEnable(true);

			//listView controll
			m_friendLvp->setVisible(false);
			m_nearbyFriendLvp->setVisible(true);
			m_platformFriendLvp->setVisible(false);

			if(0==Player::getInstance()->getNearbyList()->size())
			{
				PlatformUtil::LBS_start();

				updateNearbyFriend();
				/*
				Network::getInstance()->do_FriendList_Req(3,[this](const AckInfo &ackInfo) 
				{
				PlatformUtil::LBS_stop();

				std::string msgInfo = ackInfo.getMessageInfo();

				if (msgInfo != "")
				{
				log("[NearbyList] %s", msgInfo.c_str());
				}
				else
				{
				//ok
				log("[NearbyList] : success~~!!");

				m_page = 1;

				}
				});
				*/
			}
			else
			{
				////Button controll
				//m_friendBtn->setLocalZOrder(ZORDER_behind);
				//m_platformFriendBtn->setLocalZOrder(ZORDER_behind);
				//m_friendBtn->setOnPressEnable(true);
				//m_platformFriendBtn->setOnPressEnable(true);
				//listView controll
				//m_friendLvp->setLocalZOrder(ZORDER_behind);
				//m_friendLvp->setVisible(false);
				//m_nearbyFriendLvp->setLocalZOrder(ZORDER_front);
				//m_nearbyFriendLvp->setVisible(true);
				//m_platformFriendLvp->setLocalZOrder(ZORDER_behind);
				//m_platformFriendLvp->setVisible(false);

				m_page = 1;
				//m_pageSum = static_cast<int>(28/10) + 1; //CW
				updateNearbyFriend();
			}
			break;
		case TAG_platformFriendBtn:
			//Button controll
			m_friendBtn->setLocalZOrder(ZORDER_behind);
			m_nearbyFriendBtn->setLocalZOrder(ZORDER_behind);
			m_friendBtn->setOnPressEnable(true);
			m_nearbyFriendBtn->setOnPressEnable(true);
			//listView controll
			//m_friendLvp->setLocalZOrder(ZORDER_behind);
			m_friendLvp->setVisible(false);
			//m_nearbyFriendLvp->setLocalZOrder(ZORDER_behind);
			m_nearbyFriendLvp->setVisible(false);
			//m_platformFriendLvp->setLocalZOrder(ZORDER_front);
			m_platformFriendLvp->setVisible(true);

			m_page = 1;
			//m_pageSum = static_cast<int>(32/10) + 1; //CW
			updatePlatformFriend();
			break;
		default:
			break;
		}
		t_senderBtn->setLocalZOrder(ZORDER_front);
		t_senderBtn->setOnPressEnable(false);
		break;
	default:
		break;
	}
}

//addFriend button and deleteFriend button~
void FriendLayer::touchEventFriendInfoBtn(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);
	Widget* t_sender = static_cast<Widget*>(pSender);
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		if (TAG_addfriendBtn==t_sender->getTag())
		{
			log("Friend Info add Button~!");
			Network::getInstance()->do_AddFriend_Req(m_infoPlayerId,2,[this](const AckInfo &ackInfo) 
			{
				std::string msgInfo = ackInfo.getMessageInfo();

				if (msgInfo != "")
				{
					log("[AddFriend] %s", msgInfo.c_str());
				}
				else
				{
					//ok
					//Network::getInstance()->do_FriendList_Req(2,[this](const AckInfo &ackInfo){});
					//Network::getInstance()->do_FriendList_Req(3,[this](const AckInfo &ackInfo) 
					//{
					//	std::string msgInfo = ackInfo.getMessageInfo();

					//	if (msgInfo != "")
					//	{
					//		log("[NearbyList] %s", msgInfo.c_str());
					//	}
					//	else
					//	{
					//		//ok
					//		log("[NearbyList] : success~~!!");
					//		updateNearbyFriend();
					//	}
					//});
					m_friendInfoPal->setEnabled(false);
				}
			});
		}
		else if(TAG_deletefriendBtn==t_sender->getTag())
		{
			log("Friend Info delete Button~!");
			Network::getInstance()->do_DelFriend_Req(m_infoPlayerId,1,[this](const AckInfo &ackInfo) 
			{
				std::string msgInfo = ackInfo.getMessageInfo();

				if (msgInfo != "")
				{
					log("[DelFriend] %s", msgInfo.c_str());
				}
				else
				{
					//ok
					Network::getInstance()->do_FriendList_Req(2,[this](const AckInfo &ackInfo) 
					{
						std::string msgInfo = ackInfo.getMessageInfo();

						if (msgInfo != "")
						{
							log("[FriendList] %s", msgInfo.c_str());
						}
						else
						{
							//ok
							log("[FriendList] : success~~!!");
							updateFriend();
							m_friendInfoPal->setEnabled(false);
						}
					});
				}
			});
		}
		else
			log("Friend Info Button is not find");
		break;
	default:
		break;
	}
}

//send hp
void FriendLayer::touchEventSendHp(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);
	MyButton* t_sender = static_cast<MyButton*>(pSender);
	int t_playerId = 0;
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		log("~~~send HP to tag: %d ",((Widget*)pSender)->getTag());	
		t_playerId = Player::getInstance()->getFriendList()->at(t_sender->getTag()-TAG_sendHpBtn)->playerId;
		Network::getInstance()->do_SendHeart_Req(t_playerId,[=](const AckInfo &ackInfo) 
		{
			std::string msgInfo = ackInfo.getMessageInfo();

			if (msgInfo != "")
			{
				log("[SendHeart] %s", msgInfo.c_str());
			}
			else
			{
				//ok
				log("[SendHeart] : success~~!!");
				Player::getInstance()->getFriendList()->at(t_sender->getTag()-TAG_sendHpBtn)->isSendHeart = false;
				t_sender->setOnPressEnable(false);
			}
		});
		break;
	default:
		break;
	}
}

//Invite
void FriendLayer::touchEventInvite(Ref *pSender, TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		log("~~~Invite friend tag: %d ",((Widget*)pSender)->getTag());
		break;
	default:
		break;
	}
}

//listview listen event
void FriendLayer::listenEventFriendLvp(Ref *pSender, TouchEventType type)
{
	Widget* t_sender = static_cast<Widget*>(pSender);

	Rect t_rect;
	//the parent is listview 
	//ListView* t_parent = static_cast<ListView*>(t_sender->getParent()->getParent());

	switch (type)
	{
	case TOUCH_EVENT_BEGAN:
		//log("touch begin : %f %f",t_sender->getTouchStartPos().x,t_sender->getTouchStartPos().y);
		break;
	case TOUCH_EVENT_MOVED:
		//list touch choose HORIZONTAL or VERTICAL
		if(m_touchSwitch)
		{
			if(abs(t_sender->getTouchMovePos().x-t_sender->getTouchStartPos().x)>abs(t_sender->getTouchMovePos().y-t_sender->getTouchStartPos().y))
			{
				switch (t_sender->getTag())
				{
				case TAG_friendLvp:
					m_friendLvp->setDirectionParent(SCROLLVIEW_DIR_NONE);
					break;
				case TAG_nearbyFriendLvp:
					m_nearbyFriendLvp->setDirectionParent(SCROLLVIEW_DIR_NONE);
					break;
				case TAG_platformFriendLvp:
					m_platformFriendLvp->setDirectionParent(SCROLLVIEW_DIR_NONE);
					break;
				default:
					break;
				}
				m_friendSvp->setDirection(SCROLLVIEW_DIR_HORIZONTAL);
			}
			else
			{
				//show Slider 
				if(m_sliderSwitch)
					m_lvpSlider->setVisible(true);

				switch (t_sender->getTag())
				{
				case TAG_friendLvp:
					m_friendLvp->setDirectionParent(SCROLLVIEW_DIR_VERTICAL);
					break;
				case TAG_nearbyFriendLvp:
					m_nearbyFriendLvp->setDirectionParent(SCROLLVIEW_DIR_VERTICAL);
					break;
				case TAG_platformFriendLvp:
					m_platformFriendLvp->setDirectionParent(SCROLLVIEW_DIR_VERTICAL);
					break;
				default:
					break;
				}
				m_friendSvp->setDirection(SCROLLVIEW_DIR_NONE);
			}
			m_touchSwitch = false;
		}
		//Slider gap
		switch (t_sender->getTag())
		{
		case TAG_friendLvp:
			m_sliderGap = m_sliderGapMax + m_friendLvp->getInnerContainer()->getPosition().y;
			break;
		case TAG_nearbyFriendLvp:
			m_sliderGap = m_sliderGapMax + m_nearbyFriendLvp->getInnerContainer()->getPosition().y;
			break;
		case TAG_platformFriendLvp:
			m_sliderGap = m_sliderGapMax + m_platformFriendLvp->getInnerContainer()->getPosition().y;
			break;
		default:
			break;
		}
		break;

	case TOUCH_EVENT_ENDED:
		m_touchSwitch = true;
		log("~~~~touch ended!!");
		if(TAG_friendLvp==t_sender->getTag())
		{
			Rect t_rect;
			for( auto& item : m_friendLvp->getItems())
			{
				t_rect.origin = item->getWorldPosition();
				t_rect.size = SIZE_friendItem;

				if(t_rect.containsPoint(t_sender->getTouchEndPos()))
				{
					log("enter info layout for tag : %d ",item->getTag());

					m_infoPlayerId = Player::getInstance()->getFriendList()->at(item->getTag()-TAG_freidPal)->playerId;

					Network::getInstance()->do_FriendInfo_Req(m_infoPlayerId,[this](const AckInfo &ackInfo) 
					{
						std::string msgInfo = ackInfo.getMessageInfo();

						if (msgInfo != "")
						{
							log("[FriendInfo] %s", msgInfo.c_str());
						}
						else
						{
							//ok
							updateFriendInfoPal(TAG_freidPal);
							m_friendInfoPal->setEnabled(true);

						}
					});

					break;
				}
			}
		}
		else if(TAG_nearbyFriendLvp==t_sender->getTag())
		{
            if (Player::getInstance()->getNearbyList()->size() == 0) {return;}
            
			Rect t_rect;
			for( auto& item : m_nearbyFriendLvp->getItems())
			{
				t_rect.origin = item->getWorldPosition();
				t_rect.size = SIZE_friendItem;

				if(t_rect.containsPoint(t_sender->getTouchEndPos()))
				{
					log("enter info layout for tag : %d ",item->getTag());
					int t = item->getTag();
                    
                    if ((Player::getInstance()->getNearbyList()->size()-1) <  (item->getTag()-TAG_nearbyFriendPal)) {return;}
                    
					m_infoPlayerId = Player::getInstance()->getNearbyList()->at(item->getTag()-TAG_nearbyFriendPal)->playerId;

					Network::getInstance()->do_FriendInfo_Req(m_infoPlayerId,[this](const AckInfo &ackInfo) 
					{
						std::string msgInfo = ackInfo.getMessageInfo();

						if (msgInfo != "")
						{
							log("[FriendInfo] %s", msgInfo.c_str());
						}
						else
						{
							//ok
							updateFriendInfoPal(TAG_nearbyFriendPal);
							m_friendInfoPal->setEnabled(true);
						}
					});

					break;
				}
			}
		}

		break;

	case TOUCH_EVENT_CANCELED:
		m_lvpSlider->setVisible(false);

		m_touchSwitch = true;

		switch (t_sender->getTag())
		{
		case TAG_friendLvp:
			if(m_friendLvp->getWorldPosition().x-m_friendSvp->getWorldPosition().x>GAP_turnPage&&m_page>1)
			{
				m_page--;
				updateFriend();
			}
			else if(m_friendLvp->getWorldPosition().x-m_friendSvp->getWorldPosition().x<-GAP_turnPage&&m_page<m_pageSum)
			{
				m_page++;
				updateFriend();
			}
			break;
		case TAG_nearbyFriendLvp:
			if(m_nearbyFriendLvp->getWorldPosition().x-m_friendSvp->getWorldPosition().x>GAP_turnPage&&m_page>1)
			{
				m_page--;
				updateNearbyFriend();
			}
			else if(m_nearbyFriendLvp->getWorldPosition().x-m_friendSvp->getWorldPosition().x<-GAP_turnPage&&m_page<m_pageSum)
			{
				m_page++;
				updateNearbyFriend();
			}
			break;
		case TAG_platformFriendLvp:
			if(m_platformFriendLvp->getWorldPosition().x-m_friendSvp->getWorldPosition().x>GAP_turnPage&&m_page>1)
			{
				m_page--;
				updatePlatformFriend();
			}
			else if(m_platformFriendLvp->getWorldPosition().x-m_friendSvp->getWorldPosition().x<-GAP_turnPage&&m_page<m_pageSum)
			{
				m_page++;
				updatePlatformFriend();
			}
			break;
		default:
			break;
		}

		//log("inner size : %f ",((ListView*)pSender)->getInnerContainerSize().height);
		//log("Direction : %d ",m_friendLvp->getDirection());
		log("Tag : %d ",t_sender->getTag());
		log("~~~~canceled!!");
		break;
	default:
		break; 
	}
}

//void FriendLayer::listenEventFriendItem(Ref *pSender, TouchEventType type)
//{
//	switch (type)
//	{
//        case TOUCH_EVENT_ENDED:
//            log("~~~~~FriendItem!!");
//            break;
//        default:
//            break;
//	}
//}

//return
void FriendLayer::touchEventReturn(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_CLOSE_PAGE);

	//listen event publish
	std::string t_messageInfo;
	std::string eventName = "Btn_returnMainLayer";
	EventCustom event(eventName);
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		log("~~~~~FriendLayer return!!");
		//~~~~
		event.setUserData(&t_messageInfo);

		Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);

		this->removeFromParentAndCleanup(true);
		break;
	default:
		break;
	}
}

void FriendLayer::touchEventInfoReturn(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_CLOSE_PAGE);
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		log("~~~~~Friend Info Layer return!!");
		m_friendInfoPal->setEnabled(false);
		break;
	default:
		break;
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~touch event~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//**********************************schedule*******************************
void FriendLayer::scheduleHpTime(float delayTime)
{
	float _remainTime = Player::getInstance()->getRemainTime();

	if(_remainTime>0)
	{
		//log("~~~~~lalal time : %f ",m_Hptime);
		int t_time = static_cast<int>(_remainTime);
		String* t_timeText = 0;
		if(t_time%60>9)
			t_timeText = String::createWithFormat("%d:%d",static_cast<int>(t_time/60),t_time%60);
		else
			t_timeText = String::createWithFormat("%d:0%d",static_cast<int>(t_time/60),t_time%60);

		m_hpTimeText->setText(t_timeText->getCString());

		if(m_HpNum<5)
			m_hpLoadingBarV.at(m_HpNum)->setPercent(100.0f*(1.0f-_remainTime/Player::getInstance()->getMaxTime()));
	}
	if(Player::getInstance()->getIsStarUpdate())
		updateHp();
}

void FriendLayer::update(float delayTime)
{
	//set slider
	float t_sliderScale = m_sliderGap/m_sliderGapMax;
	if(t_sliderScale<0)
		m_lvpSlider->setPercent(4);
	else if(t_sliderScale<1)
		m_lvpSlider->setPercent(4+92*(t_sliderScale));
	else
		m_lvpSlider->setPercent(96);
}

//notice msg scorll
void FriendLayer::scheduleMsg(float delayTime)
{

	if(m_msgTime>TIME_tipsDelay)
	{
		m_msgSwitch = true;
	}

	if(m_msgSwitch)
	{
		updateMsg();
		m_msgSwitch = false;
		m_scrollSwitch = true;
		m_msgTime = 0.0f;
	}

	if(m_scrollSwitch)
	{
		Point _msgPos = m_msgText->getPosition();
		if(_msgPos.x>m_msgEndPosX)
		{
			m_msgText->setPosition(ccp(_msgPos.x-SPEED_tipsScroll,_msgPos.y));
		}
		else
			m_scrollSwitch = false;
	}
	else
	{
		m_msgTime += delayTime;
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~schedule~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//**********************************update*******************************
//assets
void FriendLayer::updateHp()
{
	m_HpNum = Player::getInstance()->getStar();	//CW

	//error
	if(m_HpNum<0)
	{
		m_HpNum = 0;

		//m_Hptime = -1;
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

		m_hpTimeText->setText(t_timeText->getCString());
	}
	else if(5==m_HpNum)
	{
		for(int num = 0 ; num < 5 ; ++num)
		{
			m_hpLoadingBarV.at(num)->setPercent(100);
		}

		m_hpTimeText->setText("MAX");
	}
	else if(m_HpNum>5)
	{
		for(int num = 0 ; num < 5 ; ++num)
		{
			m_hpLoadingBarV.at(num)->setPercent(100);
		}

		std::string t_hpNumStr = StringUtils::format("+%d",m_HpNum-5);
		m_hpTimeText->setText(t_hpNumStr.c_str());
	}

	//switch
	Player::getInstance()->setIsStarUpdate(false);
}

void FriendLayer::updateDiamond()
{
	int t_diamond = Player::getInstance()->getDiamond();
	if(t_diamond<0)
	{
		log("~~error~~ diamond num is error");
		t_diamond = 0;
	}
	std::string diamondNumStr = StringUtils::format("%d", t_diamond);	//CW
	m_diamondNumText->setText(diamondNumStr.c_str());
}

void FriendLayer::updateGold()
{
	int t_gold = Player::getInstance()->getCoin();
	if(t_gold<0)
	{
		log("~~error~~ gold num is error");
		t_gold = 0;
	}
	std::string coinStr = StringUtils::format("%d", t_gold);//CW
	m_goldNumText->setText(coinStr.c_str());
}

//activity
void FriendLayer::updateMail()
{
	int t_mailNum = Player::getInstance()->getMailCount();
	if(t_mailNum==0)
	{
		m_mailNumText->getParent()->setVisible(false);
	}
	else if(t_mailNum>0)
	{
		std::string mailNumStr = StringUtils::format("%d", t_mailNum);
		m_mailNumText->setText(mailNumStr.c_str());
	}
	else
	{
		log("mail number is error ~");
		m_mailNumText->getParent()->setVisible(false);
	}
}

void FriendLayer::updateTask()
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

//msg
void FriendLayer::updateMsg()
{
	if(m_noticeSwitch)
	{
		m_msgText->setVisible(true);
		m_noticeSwitch = false;
		if(Player::getInstance()->getContent().size()==0)
		{
			int _rand = static_cast<int>(CCRANDOM_0_1() * COUNT_tips);
			m_msgText->setText(TipsInfo::getInstance()->getTips(_rand));
		}
		else
		{
			m_msgText->setText(Player::getInstance()->getLoopContent());
		}
	}
	else
	{
		int _rand = static_cast<int>(CCRANDOM_0_1() * COUNT_tips);
		m_msgText->setText(TipsInfo::getInstance()->getTips(_rand));
	}

	Point _msgBeginPos = ccp(m_msgText->getParent()->getContentSize().width+20,m_msgText->getPosition().y);		//+20 is adjusting

	m_msgEndPosX = 0-m_msgText->getContentSize().width-20;		//-20 is adjusting

	m_msgText->setPosition(_msgBeginPos);

}

//Friend List
void FriendLayer::updateFriend()
{
	//TT
	//m_page = 1;
	int _friendCount = Player::getInstance()->getFriendList()->size();
	if(_friendCount%10||0==_friendCount)
		m_pageSum = static_cast<int>(_friendCount/10) + 1; //CW
	else
		m_pageSum = static_cast<int>(_friendCount/10);

	//clear list
	m_friendLvp->removeAllItems();

	int count = 0;			//CW
	//CW
	if(m_page<m_pageSum&&m_page>0)
		count = 10;
	else if(m_page==m_pageSum)
		count = _friendCount - 10*(m_page-1);		

	Layout* t_itemFriend = 0;
	for (int i = 0; i < count; i++)
	{
		FriendListItem* _item = Player::getInstance()->getFriendList()->at(10*(m_page-1)+i);

		if(0==i%2)
			t_itemFriend = static_cast<Layout*>(m_friendDualPal->clone());
		else
			t_itemFriend = static_cast<Layout*>(m_friendSinglePal->clone());

		t_itemFriend->setVisible(true);
		//send button
		MyButton* t_sendHpBtn = static_cast<MyButton*>(t_itemFriend->getChildByName("btn_sendHp"));
		if(_item->isSendHeart)
		{
			t_sendHpBtn->setTag(TAG_sendHpBtn+10*(m_page-1)+i);
			t_sendHpBtn->addTouchEventListener(this,toucheventselector(FriendLayer::touchEventSendHp));
		}
		else
			t_sendHpBtn->setOnPressEnable(false);

		ImageView* t_itemBg = static_cast<ImageView*>(t_itemFriend->getChildByName("img_itemBg"));
		//Player Head
		ImageView* t_playerHead = static_cast<ImageView*>(t_itemBg->getChildByName("img_playerHead"));

		//role
		ImageView* t_role = static_cast<ImageView*>(t_itemBg->getChildByName("img_role"));

		int _roleType = _item->roleType;

		if(_roleType<4)
			_roleType = 1;
		else if(_roleType<8)
			_roleType = 4;

		std::string _pathStr = StringUtils::format("commonUi/imgRole_%d.png",_roleType);
		t_role->loadTexture(_pathStr);

		//player name
		Text* t_playerName = static_cast<Text*>(t_itemBg->getChildByName("lab_playerName"));
		std::string t_playerNameStr = StringUtils::format("%s",_item->nickname.c_str());		//T
		t_playerName->setText(t_playerNameStr);

		//palyer socre
		TextAtlas* t_score = static_cast<TextAtlas*>(t_itemBg->getChildByName("labAls_score"));
		//T
		//std::string t_scoreStr = "";
		int _itemScore = _item->score;
		//if(10*(m_page-1)+i<10)
		//	t_scoreStr = String::createWithFormat("2,222,00%d",10*(m_page-1)+i);
		//else
		//	t_scoreStr = String::createWithFormat("2,222,0%d",10*(m_page-1)+i);

		t_score->setStringValue(MyHelper::changeToScoreStringRight(_itemScore));		//CW

		//set tag for friend item
		t_itemFriend->setTag(TAG_freidPal+10*(m_page-1)+i);

		m_friendLvp->pushBackCustomItem(t_itemFriend);
	}
	//offset  the list gap
	int _offsetCount = 6 - count;

	if(_offsetCount>0)
	{
		for (int i = 0; i < _offsetCount; i++)
		{
			ImageView* _listFrameS = ImageView::create("commonUi/listFrameS.png");

			ImageView* _listFrameD = ImageView::create("commonUi/listFrameD.png");

			if(0==(count+i)%2)
				m_friendLvp->pushBackCustomItem(_listFrameD);
			else
				m_friendLvp->pushBackCustomItem(_listFrameS);
		}
	}

	m_friendLvp->updateInnerContainerSizeV();
	m_friendLvp->updateSizeAndPosition();

	//Slider
	float t_gapMax = m_friendLvp->getInnerContainer()->getPosition().y;
	if(t_gapMax)
	{
		m_sliderSwitch = true;
		m_sliderGapMax = abs(t_gapMax);
	}
	else
		m_sliderSwitch = false;

	//friend number label
	m_friendNumText->setVisible(true);
	std::string _friendNumStr = StringUtils::format("%d/30",_friendCount);
	m_friendNumText->setText(_friendNumStr);

}

void FriendLayer::updateNearbyFriend()
{
	//TT
	//m_page = 1;
	//m_pageSum = Player::getInstance()->getNearbyList()->size();
	//m_pageSum = static_cast<int>(28/10) + 1; //CW
	int _friendCount = Player::getInstance()->getNearbyList()->size();
	if(_friendCount%10||0==_friendCount)
		m_pageSum = static_cast<int>(_friendCount/10) + 1; //CW
	else
		m_pageSum = static_cast<int>(_friendCount/10);

	//clear list
	m_nearbyFriendLvp->removeAllItems();

	int count = 0;			//CW
	//CW
	if(m_page<m_pageSum&&m_page>0)
		count = 10;
	else if(m_page==m_pageSum)
		count = _friendCount - 10*(m_page-1);

	Layout* t_itemFriend = 0;
	for (int i = 0; i < count; i++)
	{
		FriendListItem* _item = Player::getInstance()->getNearbyList()->at(10*(m_page-1)+i);

		if(0==i%2)
			t_itemFriend = static_cast<Layout*>(m_nearbyFriendDualPal->clone());
		else
			t_itemFriend = static_cast<Layout*>(m_nearbyFriendSinglePal->clone());

		t_itemFriend->setVisible(true);

		ImageView* t_itemBg = static_cast<ImageView*>(t_itemFriend->getChildByName("img_itemBg"));
		//Player Head
		ImageView* t_playerHead = static_cast<ImageView*>(t_itemBg->getChildByName("img_playerHead"));

		//role
		ImageView* t_role = static_cast<ImageView*>(t_itemBg->getChildByName("img_role"));

		int _roleType = _item->roleType;

		if(_roleType<4)
			_roleType = 1;
		else if(_roleType<8)
			_roleType = 4;

		std::string _pathStr = StringUtils::format("commonUi/imgRole_%d.png",_roleType);
		t_role->loadTexture(_pathStr);


		//player name
		Text* t_playerName = static_cast<Text*>(t_itemBg->getChildByName("lab_playerName"));
		std::string t_playerNameStr = StringUtils::format("%s",_item->nickname.c_str());
		//cut out the nickname
		if(t_playerNameStr.size()>24)
			t_playerNameStr = t_playerNameStr.substr(0,24);

		t_playerName->setText(t_playerNameStr);
		//_item->distance.
		Text* _playerDistance = static_cast<Text*>(t_itemBg->getChildByName("lab_playerDistance"));
		std::string t_playerDistance = "";

		int _distance = 0;

		if(!_item->distance.empty())
			_distance = std::stoi(_item->distance);

		if(_distance<0)
			log("distance is error");
		else if(_distance<1000)
		{
			if(_distance<100)
				_distance = 100;

			t_playerDistance = StringUtils::format("%dM",_distance);		//T
		}
		else
		{
			float _distanceKm = static_cast<float>(_distance)/1000.0f;
			t_playerDistance = StringUtils::format("%0.1fKM",_distanceKm);
		}
		_playerDistance->setText(t_playerDistance);
		//palyer socre
		TextAtlas* t_score = static_cast<TextAtlas*>(t_itemBg->getChildByName("labAls_score"));
		int _itemScore = _item->score;
		//if(10*(m_page-1)+i<10)
		//	t_scoreStr = String::createWithFormat("2,222,00%d",10*(m_page-1)+i);
		//else
		//	t_scoreStr = String::createWithFormat("2,222,0%d",10*(m_page-1)+i);

		t_score->setStringValue(MyHelper::changeToScoreStringRight(_itemScore));		//CW
		//t_score->setStringValue(t_scoreStr->getCString());		//CW
		//set tag for nearbyFriend item
		t_itemFriend->setTag(TAG_nearbyFriendPal+10*(m_page-1)+i);

		m_nearbyFriendLvp->pushBackCustomItem(t_itemFriend);
	}

	int _offsetCount = 6 - count; //offset  the list gap

	if(_offsetCount>0)
	{
		for (int i = 0; i < _offsetCount; i++)
		{
			ImageView* _listFrameS = ImageView::create("commonUi/listFrameS.png");

			ImageView* _listFrameD = ImageView::create("commonUi/listFrameD.png");

			if(0==(count+i)%2)
				m_nearbyFriendLvp->pushBackCustomItem(_listFrameD);
			else
				m_nearbyFriendLvp->pushBackCustomItem(_listFrameS);
		}
	}

	m_nearbyFriendLvp->updateInnerContainerSizeV();
	m_nearbyFriendLvp->updateSizeAndPosition();

	//Slider
	float t_gapMax = m_nearbyFriendLvp->getInnerContainer()->getPosition().y;
	if(t_gapMax)
	{
		m_sliderSwitch = true;                                                                                                                        
		m_sliderGapMax = abs(t_gapMax);
	}
	else
		m_sliderSwitch = false;

	//friend number label
	m_friendNumText->setVisible(false);
}

void FriendLayer::updatePlatformFriend()
{
	m_pageSum = static_cast<int>(32/10) + 1; //CW
	//clear list
	m_platformFriendLvp->removeAllItems();

	int count = 0;			//CW
	//CW
	if(m_page<m_pageSum&&m_page>0)
		count = 10;
	else if(m_page==m_pageSum)
		count = 32 - 10*(m_page-1);

	Layout* t_itemFriend = 0;
	for (int i = 0; i < count; i++)
	{
		if(0==i%2)
			t_itemFriend = static_cast<Layout*>(m_platformFriendDualPal->clone());
		else
			t_itemFriend = static_cast<Layout*>(m_platformFriendSinglePal->clone());

		t_itemFriend->setVisible(true);

		//invite button
		Button* t_inviteBtn = static_cast<Button*>(t_itemFriend->getChildByName("btn_invite"));
		t_inviteBtn->setTag(TAG_inviteBtn+10*(m_page-1)+i);
		t_inviteBtn->addTouchEventListener(this,toucheventselector(FriendLayer::touchEventInvite));

		ImageView* t_itemBg = static_cast<ImageView*>(t_itemFriend->getChildByName("img_itemBg"));
		//Player Head
		ImageView* t_playerHead = static_cast<ImageView*>(t_itemBg->getChildByName("img_playerHead"));
		//player name
		Text* t_playerName = static_cast<Text*>(t_itemBg->getChildByName("lab_playerName"));
		String* t_playerNameStr = String::createWithFormat("Player%d",10*(m_page-1)+i);		//T
		t_playerName->setText(t_playerNameStr->getCString());		//CW

		m_platformFriendLvp->pushBackCustomItem(t_itemFriend);
	}

	m_platformFriendLvp->updateInnerContainerSizeV();
	m_platformFriendLvp->updateSizeAndPosition();

	//Slider
	float t_gapMax = m_platformFriendLvp->getInnerContainer()->getPosition().y;
	if(t_gapMax)
	{
		m_sliderSwitch = true;
		m_sliderGapMax = abs(t_gapMax);
	}
	else
		m_sliderSwitch = false;

	//friend number label
	m_friendNumText->setVisible(false);
}

//friend info
void FriendLayer::updateFriendInfoPal(int type)
{
	if(TAG_freidPal==type)
	{
		m_addFriendBtn->setVisible(false);
		//m_addFriendBtn->setEnabled(false);
		m_deleteFriendBtn->setVisible(true);
		//m_deleteFriendBtn->setEnabled(true);
		m_addFriendBtn->setLocalZOrder(ZORDER_behind);
		m_deleteFriendBtn->setLocalZOrder(ZORDER_front);
		//T
		//String* t_scoreStr = String::createWithFormat("2,222,%d",tag);
		m_scoreText->setStringValue(MyHelper::changeToScoreStringCenter(Player::getInstance()->getHighestScore()));

		std::string t_levelsStr = StringUtils::format("%d",Player::getInstance()->getHighestLevels());
		m_levelsText->setStringValue(t_levelsStr);
	}
	else if(TAG_nearbyFriendPal==type)
	{
		m_addFriendBtn->setVisible(true);
		//m_addFriendBtn->setEnabled(true);
		m_deleteFriendBtn->setVisible(false);
		//m_deleteFriendBtn->setEnabled(false);
		m_addFriendBtn->setLocalZOrder(ZORDER_front);
		m_deleteFriendBtn->setLocalZOrder(ZORDER_behind);
		//T
		//String* t_scoreStr = String::createWithFormat("2,222,%d",tag);
		m_scoreText->setStringValue(MyHelper::changeToScoreStringCenter(Player::getInstance()->getHighestScore()));

		std::string t_levelsStr = StringUtils::format("%d",Player::getInstance()->getHighestLevels());
		m_levelsText->setStringValue(t_levelsStr);
	}
	else
		log("friend list type error~~~~~!!!!");
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~update~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//********************************helper*********************************
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~helper~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~