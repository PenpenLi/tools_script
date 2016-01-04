#include "ItemLayer.h"
#include "ShopLayer.h"
#include "UiConfig.h"
//
#include "Network.h"
#include "Player.h"

#include "battle/BattleScene.h"
#include "model/BattleData.h"
#include "FriendHelpLayer.h"
#include "util/GameSound.h"
#include "common/TextFloatLabelLayer.h"
//
#include "UiConfig.h"
#include "TipsInfo.h"
#include "PropsInfo.h"
#include "MailLayer.h"
#include "TaskLayer.h"
#include "RankLayer.h"
#include "NoticeLayer.h"
//
#include "view/battle/LoadLayer.h"

bool ItemLayer::init()
{
	if (Layer::init())
	{
		//member variable init
		variableInit();
		//Ui res
		importUI();

		return true;
	}

	return false;
}

void ItemLayer::onEnter()
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

	Network::getInstance()->do_ItemList_Req([this](const AckInfo &ackInfo) 
	{
		std::string msgInfo = ackInfo.getMessageInfo();

		if (msgInfo != "")
		{
			log("[ItemList] %s", msgInfo.c_str());
		}
		else
		{
			//ok
			log("[ItemList] : success~~!!");
			//
			updateItemNum();
		}
	});

	//schedule
	this->schedule(schedule_selector(ItemLayer::scheduleHpTime),1.0f);
	//tips
	this->schedule(schedule_selector(ItemLayer::scheduleMsg),TIME_tipsScheduleDelay);

}

void ItemLayer::onExit()
{
	Layer::onExit();

	//schedule
	this->unschedule(schedule_selector(ItemLayer::scheduleHpTime));
	this->unschedule(schedule_selector(ItemLayer::scheduleMsg));

	//itme add Listener
	//_eventDispatcher->removeCustomEventListeners("ItemAdd_Ack");
}

//init
void ItemLayer::variableInit()
{
	m_hpLoadingBarV.clear();

	m_assetsPal = 0;
	m_hpTimeText = 0;
	m_hpAddBtn = 0;
	m_diamondNumText = 0;
	m_diamondAddBtn = 0;
	m_goldNumText = 0;
	m_goldAddBtn = 0;

	m_activityPal = 0;
	m_mailBtn = 0;
	m_mailNumText = 0;
	m_taskBtn = 0;
	m_taskNumText = 0;
	m_rankBtn = 0;
	m_activityBtn = 0;

	m_Hptime = 0;
	m_HpNum = 0;

	m_returnBtn = 0;

	//msg time 
	m_msgTime = 0.0f;
	m_msgSwitch = false;
	m_scrollSwitch = false;
	m_noticeSwitch = true;
	m_msgEndPosX = 0.0f;
}

bool ItemLayer::importUI()
{
	bool bRet = false;
	do{

		Widget* t_widget = GUIReader::getInstance()->widgetFromJsonFile("ItemUi/ItemUi.json");
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
		m_assetsPal = static_cast<Layout*>(t_widget->getChildByName("pal_assets"));

		//HP
		Widget* m_assetsPalHp = m_assetsPal->getChildByName("img_hpFrameBg");
		for(int num = 1 ; num<6 ; ++num)
		{
			//get Hp LoadingBar
			String* t_loadingBarBgStr = String::createWithFormat("img_hpBg_%d",num);
			String* t_loadingBarStr = String::createWithFormat("pgb_hp_%d",num);
			LoadingBar* t_progressTimer = static_cast<LoadingBar*>(m_assetsPalHp->getChildByName(t_loadingBarBgStr->getCString())->getChildByName(t_loadingBarStr->getCString()));
			m_hpLoadingBarV.pushBack(t_progressTimer);
		}
		m_hpTimeText = static_cast<Text*>(m_assetsPalHp->getChildByName("lab_hpTime"));
		m_hpAddBtn = static_cast<Button*>(m_assetsPalHp->getChildByName("btn_addHp"));
		if (m_hpAddBtn)
		{
			m_hpAddBtn->setTag(TAG_shopHpBtn);
			m_hpAddBtn->addTouchEventListener(this, toucheventselector(ItemLayer::touchEventShopBtn));
		}
		else
			log("~~error~~ add Hp button is Null ");

		//diamond
		Widget* m_assetsPalDiamond = m_assetsPal->getChildByName("img_diamondFrameBg");
		m_diamondNumText = static_cast<Text*>(m_assetsPalDiamond->getChildByName("lab_diamondNum"));
		m_diamondAddBtn = static_cast<Button*>(m_assetsPalDiamond->getChildByName("btn_addDiamond"));
		if (m_diamondAddBtn)
		{
			m_diamondAddBtn->setTag(TAG_shopDiamondBtn);
			m_diamondAddBtn->addTouchEventListener(this, toucheventselector(ItemLayer::touchEventShopBtn));
		}
		else
			log("~~error~~ add Diamond button is Null ");

		//gold
		Widget* m_assetsPalGold = m_assetsPal->getChildByName("img_goldFrameBg");
		m_goldNumText = static_cast<Text*>(m_assetsPalGold->getChildByName("lab_goldNum"));
		m_goldAddBtn = static_cast<Button*>(m_assetsPalGold->getChildByName("btn_addGold"));
		if (m_goldAddBtn)
		{
			m_goldAddBtn->setTag(TAG_shopGoldBtn);
			m_goldAddBtn->addTouchEventListener(this, toucheventselector(ItemLayer::touchEventShopBtn));
		}
		else
			log("~~error~~ add Gold button is Null ");

		//~Panel Activity~
		m_activityPal = static_cast<Layout*>(t_widget->getChildByName("pal_btnAct"));

		//mail
		m_mailNumText = static_cast<Text*>(m_activityPal->getChildByName("btn_mail")->getChildByName("img_mailNumBg")->getChildByName("lab_mailNum"));
		m_mailBtn = static_cast<Button*>(m_activityPal->getChildByName("btn_mail"));
		if (m_mailBtn)
		{
			m_mailBtn->setTag(TAG_btnMail);
			m_mailBtn->addTouchEventListener(this, toucheventselector(ItemLayer::touchEventActBtn));
		}
		else
			log("~~error~~ Mail button is Null ");

		//task
		m_taskNumText = static_cast<Text*>(m_activityPal->getChildByName("btn_task")->getChildByName("img_taskNumBg")->getChildByName("lab_taskNum"));
		m_taskBtn = static_cast<Button*>(m_activityPal->getChildByName("btn_task"));
		if (m_taskBtn)
		{
			m_taskBtn->setTag(TAG_btnTask);
			m_taskBtn->addTouchEventListener(this, toucheventselector(ItemLayer::touchEventActBtn));
		}
		else
			log("~~error~~ Task button is Null ");

		//rank
		m_rankBtn = static_cast<Button*>(m_activityPal->getChildByName("btn_rank"));
		if (m_rankBtn)
		{
			m_rankBtn->setTag(TAG_btnRank);
			m_rankBtn->addTouchEventListener(this, toucheventselector(ItemLayer::touchEventActBtn));
		}
		else
			log("~~error~~ Rank button is Null ");

		//act
		m_activityBtn = static_cast<Button*>(m_activityPal->getChildByName("btn_activity"));
		if (m_activityBtn)
		{
			m_activityBtn->setTag(TAG_btnActivity);
			m_activityBtn->addTouchEventListener(this, toucheventselector(ItemLayer::touchEventActBtn));
		}
		else
			log("~~error~~ Activity button is Null ");

		//continue
		m_continueBtn = static_cast<Button*>(t_widget->getChildByName("btn_continue"));
		if (m_continueBtn)
		{
			//m_returnBtn->setEnabled(false);
			m_continueBtn->addTouchEventListener(this, toucheventselector(ItemLayer::touchEventContinueBtn));
		}
		else
			log("~~error~~ Continue button is Null ");

		//return
		m_returnBtn = static_cast<Button*>(t_widget->getChildByName("btn_return"));
		if (m_returnBtn)
		{
			//m_returnBtn->setEnabled(false);
			m_returnBtn->addTouchEventListener(this, toucheventselector(ItemLayer::touchEventReturnBtn));
		}
		else
			log("~~error~~ Return button is Null ");

		//get prop num label and random prop type
		Widget* t_itemPal = t_widget->getChildByName("pal_item");

		m_beginSpurtNum = static_cast<Text*>(Helper::seekWidgetByName(t_itemPal,"lab_beginSpurtNum"));
		m_protectBubbleNum = static_cast<Text*>(Helper::seekWidgetByName(t_itemPal,"lab_protectBubbleNum"));
		m_deathSpurtNum = static_cast<Text*>(Helper::seekWidgetByName(t_itemPal,"lab_deathSpurtNum"));
		m_scoreBuffNum = static_cast<Text*>(Helper::seekWidgetByName(t_itemPal,"lab_scoreBuffNum"));

		m_randomProps = static_cast<Text*>(Helper::seekWidgetByName(t_widget,"m_randomLab"));

		setWidgetButton(t_itemPal,"btn_blessing",TAG_blessingBtn,toucheventselector(ItemLayer::touchEventItemBuyBtn));
		setWidgetButton(t_itemPal,"btn_beginSpurt",TAG_beginSpurtBtn,toucheventselector(ItemLayer::touchEventItemBuyBtn));
		setWidgetButton(t_itemPal,"btn_protectBubble",TAG_protectBubbleBtn,toucheventselector(ItemLayer::touchEventItemBuyBtn));
		setWidgetButton(t_itemPal,"btn_deathSpurt",TAG_deathSpurtBtn,toucheventselector(ItemLayer::touchEventItemBuyBtn));
		setWidgetButton(t_itemPal,"btn_scoreBuff",TAG_scoreBuffBtn,toucheventselector(ItemLayer::touchEventItemBuyBtn));

		//Msg text
		m_msgText = static_cast<Text*>(t_widget->getChildByName("img_msgBg")->getChildByName("svp_msg")->getChildByName("lab_msg"));

		//************* event listener ************************
		//event listener shop layer return call-back
		auto _listenerShop = EventListenerCustom::create("Return_shopLayer", [this](EventCustom* event) {
			log("AAAAAAAAAAAAAAA item layer");
			updateHp();
			updateGold();
			updateDiamond();
		});

		_eventDispatcher->addEventListenerWithSceneGraphPriority(_listenerShop, this);

		//event listener mail layer return call-back
		auto _listenerMail = EventListenerCustom::create("Return_mailLayer", [this](EventCustom* event) {
			log("BBBBBBBBBBBBBBB item layer");
			updateMail();
			updateHp();
			updateGold();
			updateDiamond();
		});

		_eventDispatcher->addEventListenerWithSceneGraphPriority(_listenerMail, this);

		auto _listenerTask = EventListenerCustom::create("Return_taskLayer", [this](EventCustom* event) {
			log("CCCCCCCCCCCCCC item layer");
			updateTask();
			updateGold();
			updateDiamond();
		});

		_eventDispatcher->addEventListenerWithSceneGraphPriority(_listenerTask, this);


		bRet = true;
	}while(0);

	return bRet;
}

//**********************************touch event*******************************
//Assets
void ItemLayer::touchEventShopBtn(Ref *pSender, TouchEventType type)
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

//activity button
void ItemLayer::touchEventActBtn(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);
	Widget* t_sender = static_cast<Widget*>(pSender);
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		log("activity");

		int _zOrder = this->getLocalZOrder();

		MailLayer* _mailLayer = nullptr;

		RankLayer* _rankLayer = nullptr;

		TaskLayer* _taskLayer = nullptr;

		NoticeLayer* _notcieLayer = nullptr;

		switch (t_sender->getTag())
		{
		case TAG_btnMail:
			log("~~~~~enter Mail Scene!!");
			_mailLayer = MailLayer::create();

			_mailLayer->setLocalZOrder(_zOrder+1);

			this->getParent()->addChild(_mailLayer);
			break;
		case TAG_btnTask:
			log("~~~~~enter Task Scene!!");
			_taskLayer = TaskLayer::create();

			_taskLayer->setLocalZOrder(_zOrder+1);

			this->getParent()->addChild(_taskLayer);
			break;
		case TAG_btnRank:
			log("~~~~~enter Rank Scene!!");

			_rankLayer = RankLayer::create();

			_rankLayer->setLocalZOrder(_zOrder+1);

			this->getParent()->addChild(_rankLayer);
			break;
		case TAG_btnActivity:
			log("~~~~~enter Activity Scene!!");
			_notcieLayer = NoticeLayer::create();

			_notcieLayer->setLocalZOrder(_zOrder+1);

			this->getParent()->addChild(_notcieLayer);
			break;
			//default:
			//	log("~~~~~~~~~~~~~error : not found Activity button tag");
			//	break;
		}
		break;
	}
}

//item buy
void ItemLayer::touchEventItemBuyBtn(Ref *pSender, TouchEventType type)
{
	Button* t_senderBtn = static_cast<Button*>(pSender);
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		log("~~~~~ItemLayer buy button : %d",t_senderBtn->getTag());
		switch (t_senderBtn->getTag())
		{
		case TAG_blessingBtn:
			log("TAG_blessingBtn");
			GameSound::playEffectMusic(EFF_RANDOM_ITEM);
			//Network::getInstance()->do_ItemAdd_Req(1,1);
			itemAdd(1,1);
			break;
		case TAG_beginSpurtBtn:
			log("TAG_beginSpurtBtn");
			GameSound::playEffectMusic(EFF_SUCCESS_BUY_PROP);
			//Network::getInstance()->do_ItemAdd_Req(2,1);
			itemAdd(2,1);
			break;
		case TAG_protectBubbleBtn:
			log("TAG_protectBubbleBtn");
			GameSound::playEffectMusic(EFF_SUCCESS_BUY_PROP);
			//Network::getInstance()->do_ItemAdd_Req(3,1);
			itemAdd(3,1);
			break;
		case TAG_deathSpurtBtn:
			log("TAG_deathSpurtBtn");
			GameSound::playEffectMusic(EFF_SUCCESS_BUY_PROP);
			//Network::getInstance()->do_ItemAdd_Req(4,1);
			itemAdd(4,1);
			break;
		case TAG_scoreBuffBtn:
			log("TAG_scoreBuffBtn");
			GameSound::playEffectMusic(EFF_SUCCESS_BUY_PROP);
			//Network::getInstance()->do_ItemAdd_Req(5,1);
			itemAdd(5,1);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

//continue
void ItemLayer::touchEventContinueBtn(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		log("~~~~~Continue!!");

		//FriendHelpLayer* _friendHelpLayer = FriendHelpLayer::create();

		//int _zOrder = this->getLocalZOrder();

		//_friendHelpLayer->setLocalZOrder(_zOrder+1);

		//this->getParent()->addChild(_friendHelpLayer);

		//Start Game

		if(Player::getInstance()->getStar()>0)
		{
			Network::getInstance()->do_StartGame_Req(BattleData::getInstance()->getHelperPlayerId(), [this](const AckInfo &ackInfo)
			{
				std::string msgInfo = ackInfo.getMessageInfo();

				if (msgInfo != "")
				{
					log("[do_StartGame_Req] %s", msgInfo.c_str());
				}
				else
				{
					log("[do_StartGame_Req] OK!");
                    
                    int level = 10000 + (int)(CCRANDOM_0_1()*4 + 1);
                    
                    if (level > 10005) {level = 10001;}
                    
                    log("Choose-Level: ---------------%d---------------", level);
                    
                    BattleData::getInstance()->setBattleLevel(level);

					GameSound::playEffectMusic(EFF_USE_COUNT);

					Player::getInstance()->setStar(Player::getInstance()->getStar()-1);

					auto loadLayer = LoadLayer::create(level, [this](void) {

						auto scene = BattleScene::create();

						Director::getInstance()->replaceScene(scene);
					});

					this->addChild(loadLayer);

					loadLayer->load();

				}
			});
		}
		else
		{
			TextFloatLabelLayer::getInstance()->show(TipsInfo::getInstance()->getMessage("hp_lack"));
		}

		break;
	}
}

//return 
void ItemLayer::touchEventReturnBtn(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_CLOSE_PAGE);
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		log("~~~~~Set Layer return!!");
		//listen event publish
		std::string eventName = "Return_itemLayer";
		EventCustom event(eventName);
		Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);

		this->removeFromParentAndCleanup(true);

		break;
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~touch event~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//**********************************schedule*******************************
void ItemLayer::scheduleHpTime(float delayTime)
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

//notice msg scorll
void ItemLayer::scheduleMsg(float delayTime)
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
void ItemLayer::updateHp()
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

void ItemLayer::updateDiamond()
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

void ItemLayer::updateGold()
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
void ItemLayer::updateMail()
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

void ItemLayer::updateTask()
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
void ItemLayer::updateMsg()
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

//item
void ItemLayer::updateItemNum()
{
	int _item1Num = Player::getInstance()->getProp1Num();
	if(0==_item1Num)
		m_beginSpurtNum->getParent()->setVisible(false);
	else if(_item1Num>0)
	{
		m_beginSpurtNum->getParent()->setVisible(true);
		std::string t_beginSpurtNumStr = StringUtils::format("%d",_item1Num);
		m_beginSpurtNum->setText(t_beginSpurtNumStr.c_str());
	}
	else
	{
		log("item 1 num is error~~!!!");
		m_beginSpurtNum->getParent()->setVisible(false);
	}

	int _item2Num = Player::getInstance()->getProp2Num();
	if(0==_item2Num)
		m_protectBubbleNum->getParent()->setVisible(false);
	else if(_item2Num>0)
	{
		m_protectBubbleNum->getParent()->setVisible(true);
		std::string t_protectBubbleNumStr = StringUtils::format("%d",_item2Num);
		m_protectBubbleNum->setText(t_protectBubbleNumStr.c_str());
	}
	else
	{
		log("item 2 num is error~~!!!");
		m_protectBubbleNum->getParent()->setVisible(false);
	}

	int _item3Num = Player::getInstance()->getProp3Num();
	if(0==_item3Num)
		m_deathSpurtNum->getParent()->setVisible(false);
	else if(_item3Num>0)
	{
		m_deathSpurtNum->getParent()->setVisible(true);
		std::string t_deathSpurtNumStr = StringUtils::format("%d",_item3Num);
		m_deathSpurtNum->setText(t_deathSpurtNumStr.c_str());
	}
	else
	{
		log("item 3 num is error~~!!!");
		m_deathSpurtNum->getParent()->setVisible(false);
	}

	int _item4Num = Player::getInstance()->getProp4Num();
	if(0==_item4Num)
		m_scoreBuffNum->getParent()->setVisible(false);
	else if(_item4Num>0)
	{
		m_scoreBuffNum->getParent()->setVisible(true);
		std::string t_scoreBuffNumStr = StringUtils::format("%d",_item4Num);
		m_scoreBuffNum->setText(t_scoreBuffNumStr.c_str());
	}
	else
	{
		log("item 4 num is error~~!!!");
		m_scoreBuffNum->getParent()->setVisible(false);
	}

	int _randomPropType = Player::getInstance()->getPropRandom();
	if(0==_randomPropType)
		m_randomProps->getParent()->setVisible(false);
	else if(_randomPropType>12&&_randomPropType<27)
	{
		m_randomProps->getParent()->setVisible(true);
		std::string _propsStr = PropsInfo::getInstance()->getProps(_randomPropType);
		m_randomProps->setText(_propsStr);
	}
	else
	{
		log("random item type is error~~!!!");
		m_randomProps->getParent()->setVisible(false);
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~update~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//************************************helper*************************************************
void ItemLayer::setWidgetButton(Widget* root, const char *name,int tag, SEL_TouchEvent selector)
{
	Button* t_btn = 0;
	t_btn = static_cast<Button*>(Helper::seekWidgetByName(root,name));
	if(t_btn)
	{
		t_btn->setTag(tag);
		t_btn->addTouchEventListener(this,selector);
	}
	else
		log("helper widget button in ItemLayer is null");
}

//item Add
void ItemLayer::itemAdd(int type,int num)
{
	int _cost = 0;

	if (type == 2)
	{
		_cost = 200;
	}
	else if (type == 3)
	{
		_cost = 500;
	}
	else if (type == 4)
	{
		_cost = 800;
	}
	else if (type == 5)
	{
		_cost = 1200;
	}
	else if (type == 1)
	{
		_cost = 300;
	}

	if(Player::getInstance()->getCoin()>_cost)
	{
		Network::getInstance()->do_ItemAdd_Req(type,num,[this](const AckInfo &ackInfo) 
		{
			std::string msgInfo = ackInfo.getMessageInfo();

			if (msgInfo != "")
			{
				log("[ItemAdd] %s", msgInfo.c_str());
			}
			else
			{
				//ok
				log("[ItemAdd] : success~~!!");
				updateItemNum();
				updateGold();
			}
		});
	}
	else
	{
		log("gold is not enough~~");
		TextFloatLabelLayer::getInstance()->show(TipsInfo::getInstance()->getMessage("gold_lack"));
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~helper~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~