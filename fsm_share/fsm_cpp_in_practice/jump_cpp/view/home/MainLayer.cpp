#include "MainLayer.h"
#include <string>

//
#include "RoleLayer.h"
#include "FriendLayer.h"
#include "SetLayer.h"
#include "Player.h"
#include "RankLayer.h"
#include "ShopLayer.h"
#include "ItemLayer.h"
#include "TaskLayer.h"
#include "MailLayer.h"
#include "FriendHelpLayer.h"
#include "LevelChooseLayer.h"
#include "NoticeLayer.h"

#include "BattleData.h"
//#include "MailLayer.h"
#include "battle/BattleScene.h"
#include "util/GameSound.h"
//
#include "TipsInfo.h"

bool MainLayer::init()
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

void MainLayer::onEnter()
{
	Layer::onEnter();

	//network : update task count and mail count
	Network::getInstance()->do_TaskCount_Req([this](const AckInfo &ackInfo){});

	//update
	updateDiamond();
	updateGold();
	updateMail();
	updateTask();
	//updateMsg();
	updateRoleImg();

	//if(Player::getInstance()->getIsStarUpdate())
	updateHp();

	//schedule
	this->schedule(schedule_selector(MainLayer::scheduleHpTime),1.0f);
	
    //tips
	this->schedule(schedule_selector(MainLayer::scheduleMsg),TIME_tipsScheduleDelay);

}

void MainLayer::onExit()
{
	Layer::onExit();

	//schedule
	this->unschedule(schedule_selector(MainLayer::scheduleHpTime));
	this->unschedule(schedule_selector(MainLayer::scheduleMsg));

	//_eventDispatcher->removeAllEventListeners();
}

//init
void MainLayer::variableInit()
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

	m_battleLevelBtn = 0;
	m_battleEndlessBtn = 0;

	m_mainBtn = 0;
	m_roleBtn = 0;
	m_friendBtn = 0;
	m_setupBtn = 0;

	m_roleImage = 0;

	m_msgText = 0;

	m_Hptime = 0;
	m_HpNum = 0;
	//msg time 
	m_msgTime = 0.0f;
	m_msgSwitch = false;
	m_scrollSwitch = false;
	m_noticeSwitch = true;
	m_msgEndPosX = 0.0f;

}

bool MainLayer::importUI()
{
	bool bRet = false;
	do{
		Widget* t_widget = GUIReader::getInstance()->widgetFromJsonFile("MainUi/MainUi.json");
		this->addChild(static_cast<Layout*>(t_widget));

		//~Panel Assets~
		Widget* t_widgetPalAssets = t_widget->getChildByName("pal_assets");

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
			m_hpAddBtn->addTouchEventListener(this, toucheventselector(MainLayer::touchEventShopBtn));
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
			m_diamondAddBtn->addTouchEventListener(this, toucheventselector(MainLayer::touchEventShopBtn));
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
			m_goldAddBtn->addTouchEventListener(this, toucheventselector(MainLayer::touchEventShopBtn));
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
			m_mailBtn->addTouchEventListener(this, toucheventselector(MainLayer::touchEventMail));
		}
		else
			log("~~error~~ Mail button is Null ");

		//task
		m_taskNumText = static_cast<Text*>(t_widgetPalAct->getChildByName("btn_task")->getChildByName("img_taskNumBg")->getChildByName("lab_taskNum"));
		m_taskBtn = static_cast<Button*>(t_widgetPalAct->getChildByName("btn_task"));
		if (m_taskBtn)
		{
			m_taskBtn->addTouchEventListener(this, toucheventselector(MainLayer::touchEventTask));
		}
		else
			log("~~error~~ Task button is Null ");

		//rank
		m_rankBtn = static_cast<Button*>(t_widgetPalAct->getChildByName("btn_rank"));
		if (m_rankBtn)
		{
			m_rankBtn->addTouchEventListener(this, toucheventselector(MainLayer::touchEventRank));
		}
		else
			log("~~error~~ Rank button is Null ");

		//act
		m_activityBtn = static_cast<Button*>(t_widgetPalAct->getChildByName("btn_activity"));
		if (m_activityBtn)
		{
			m_activityBtn->addTouchEventListener(this, toucheventselector(MainLayer::touchEventActivity));
		}
		else
			log("~~error~~ Activity button is Null ");

		//~Panel battle~
		Widget* t_widgetPalBattle = t_widget->getChildByName("pal_btnBattle");

		//battleLevel
		m_battleLevelBtn = static_cast<Button*>(t_widgetPalBattle->getChildByName("btn_battleLevel"));
		if (m_battleLevelBtn)
		{
			m_battleLevelBtn->addTouchEventListener(this, toucheventselector(MainLayer::touchEventBattleLevel));
		}
		else
			log("~~error~~ Level Battle button is Null ");

		//battleEndless
		m_battleEndlessBtn = static_cast<Button*>(t_widgetPalBattle->getChildByName("btn_battleEndless"));
		if (m_battleEndlessBtn)
		{
			m_battleEndlessBtn->addTouchEventListener(this, toucheventselector(MainLayer::touchEventBattleEndless));
		}
		else
			log("~~error~~ Endless Battle button is Null ");

		//~Panel Controll~
		Widget* t_widgetPalControl = t_widget->getChildByName("pal_btnControl");

		//main
		m_mainBtn = static_cast<Button*>(t_widgetPalControl->getChildByName("btn_main"));
		if (m_mainBtn)
		{
			m_mainBtn->addTouchEventListener(this, toucheventselector(MainLayer::touchEventControlMain));
		}
		else
			log("~~error~~ Main button is Null ");

		//role
		m_roleBtn = static_cast<Button*>(t_widgetPalControl->getChildByName("btn_role"));
		if (m_roleBtn)
		{
			m_roleBtn->addTouchEventListener(this, toucheventselector(MainLayer::touchEventControlRole));
		}
		else
			log("~~error~~ Role button is Null ");

		//friend
		m_friendBtn = static_cast<Button*>(t_widgetPalControl->getChildByName("btn_friend"));
		if (m_friendBtn)
		{
			m_friendBtn->addTouchEventListener(this, toucheventselector(MainLayer::touchEventControlFriend));
		}
		else
			log("~~error~~ friend button is Null ");

		//set
		m_setupBtn = static_cast<Button*>(t_widgetPalControl->getChildByName("btn_setup"));
		if (m_setupBtn)
		{
			m_setupBtn->addTouchEventListener(this, toucheventselector(MainLayer::touchEventControlSet));
		}
		else
			log("~~error~~ setup button is Null ");

		//role img ?
		m_roleImage = static_cast<ImageView*>(t_widget->getChildByName("img_roleBg")->getChildByName("img_role"));

		//m_rolePal = static_cast<Layout*>(Helper::seekWidgetByName(t_widget,"m_rolePal"));

		if(m_roleImage)
		{
			m_roleImage->addTouchEventListener(this, toucheventselector(MainLayer::touchEventRoleImg));

			//m_rolePal->addTouchEventListener(this, toucheventselector(MainLayer::touchEventRoleImg));
		}

		//Msg text
		m_msgText = static_cast<Text*>(t_widget->getChildByName("img_msgBg")->getChildByName("svp_msg")->getChildByName("lab_msg"));
		m_msgText->setVisible(false);


//load Armature
		Point _rolePos = m_roleImage->getPosition();

		ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/Horse__1/haimazayan.ExportJson");

		m_armatureHero_1 = Armature::create("haimazayan");

		m_armatureHero_1->setPosition(_rolePos);

		m_armatureHero_1->setVisible(false);

		m_roleImage->getParent()->addChild(m_armatureHero_1);

		ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/Devilfish/zhangyuzayan.ExportJson");

		m_armatureHero_2 = Armature::create("zhangyuzayan");

		m_armatureHero_2->setPosition(_rolePos);

		m_armatureHero_2->setVisible(false);

		m_roleImage->getParent()->addChild(m_armatureHero_2);

	//************* event listener ************************
		//event listener shop layer return call-back
		auto _listenerShop = EventListenerCustom::create("Return_shopLayer", [this](EventCustom* event) {
			log("AAAAAAAAAAAAAAA main layer");
			updateHp();
			updateGold();
			updateDiamond();

			updateTask();
			updateMail();
		});

		_eventDispatcher->addEventListenerWithSceneGraphPriority(_listenerShop, this);

		//event listener mail layer return call-back
		auto _listenerMail = EventListenerCustom::create("Return_mailLayer", [this](EventCustom* event) {
			log("BBBBBBBBBBBBBBB main layer");
			updateHp();
			updateGold();
			updateDiamond();

			updateTask();
			updateMail();
		});

		_eventDispatcher->addEventListenerWithSceneGraphPriority(_listenerMail, this);

		auto _listenerTask = EventListenerCustom::create("Return_taskLayer", [this](EventCustom* event) {
			log("CCCCCCCCCCCCCC main layer");
			updateGold();
			updateDiamond();

			updateTask();
			updateMail();
		});

		_eventDispatcher->addEventListenerWithSceneGraphPriority(_listenerTask, this);

		auto _listenerItem = EventListenerCustom::create("Return_friendHelpLayer", [this](EventCustom* event) {
			log("DDDDDDDDDDDDDD main layer");
			updateGold();
			updateDiamond();

			updateTask();
			updateMail();
		});

		_eventDispatcher->addEventListenerWithSceneGraphPriority(_listenerItem, this);

		auto _listenerRole = EventListenerCustom::create("Return_roleLayer", [this](EventCustom* event) {
			log("EEEEEEEEEEEEEEE main layer");
			updateGold();
			updateDiamond();
			updateRoleImg();

			updateTask();
			updateMail();
		});

		_eventDispatcher->addEventListenerWithSceneGraphPriority(_listenerRole, this);

		bRet = true;
	}while(0);

	return bRet;
}

//**********************************touch event*******************************
//Assets
void MainLayer::touchEventShopBtn(Ref *pSender, TouchEventType type)
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
void MainLayer::touchEventMail(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);

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

void MainLayer::touchEventTask(Ref *pSender, TouchEventType type)
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

void MainLayer::touchEventRank(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);

	//RankLayer *t_rankLayer = 0;
	//int t_zOrder = 0;
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

void MainLayer::touchEventActivity(Ref *pSender, TouchEventType type)
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

//Battle
void MainLayer::touchEventBattleLevel(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);

	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		log("~~~~~enter Battle Level Scene!!");

		BattleData::getInstance()->setBattleMode(1);

		auto layer = LevelChooseLayer::create();

		this->getScene()->addChild(layer);

		break;
	}
}

void MainLayer::touchEventBattleEndless(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);

	ItemLayer *t_itemLayer = 0;
	int t_zOrder = 0;
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		log("~~~~~enter Battle Endless Scene!!");
		int cLevel = Player::getInstance()->getCurrentLevel();
		if (Player::getInstance()->getCurrentLevel() < 6)
		{
			//return;
		}

		BattleData::getInstance()->setBattleMode(0);

		FriendHelpLayer* _friendHelpLayer = FriendHelpLayer::create();

		int _zOrder = this->getLocalZOrder();

		_friendHelpLayer->setLocalZOrder(_zOrder+1);

		this->getParent()->addChild(_friendHelpLayer);

		//           auto battleScene = BattleScene::create();

		//           this->getParent()->addChild(friendHelpLayer);

		break;
	}
}

//Control
void MainLayer::touchEventControlMain(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);

	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		log("~~~~~enter Main Scene!!");
		break;
	default:
		break;
	}
}

void MainLayer::touchEventControlRole(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);

	Layer *t_roleLayer = 0;
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		log("~~~~~enter Role Scene!!");

		t_roleLayer = RoleLayer::create();

		t_roleLayer->setAnchorPoint(Point::ZERO);

		t_roleLayer->setPosition(0, 0);

		t_roleLayer->setLocalZOrder(ZORDER_front);

		this->getParent()->addChild(t_roleLayer);

		break;
	default:
		break;
	}
}

void MainLayer::touchEventControlFriend(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);

	Layer *t_friendLayer = 0;
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		log("~~~~~enter Friend Scene!!");

		t_friendLayer = FriendLayer::create();

		t_friendLayer->setAnchorPoint(Point::ZERO);

		t_friendLayer->setPosition(0, 0);

		t_friendLayer->setLocalZOrder(ZORDER_front);

		this->getParent()->addChild(t_friendLayer);

		break;
	default:
		break;
	}
}

void MainLayer::touchEventControlSet(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);

	Layer *t_setLayer = 0;
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		log("~~~~~enter Setup Scene!!");

		t_setLayer = SetLayer::create();

		t_setLayer->setAnchorPoint(Point::ZERO);

		t_setLayer->setPosition(0, 0);

		t_setLayer->setLocalZOrder(ZORDER_front);

		this->getParent()->addChild(t_setLayer);

		break;
	default:
		break;
	}
}

//role img
void MainLayer::touchEventRoleImg(Ref *pSender, TouchEventType type)
{
	
	GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);
	if(TOUCH_EVENT_ENDED==type)
	{
		log("touch role img~~~");
		//CW
		auto t_roleLayer = RoleLayer::create();

		t_roleLayer->setLocalZOrder(ZORDER_front);

		this->getParent()->addChild(t_roleLayer);

////test
//		Network::getInstance()->do_UpHeart_Req([this](const AckInfo &ackInfo) 
//		{
//			std::string msgInfo = ackInfo.getMessageInfo();
//
//			if (msgInfo != "")
//			{
//				log("[UpHeart] %s", msgInfo.c_str());
//			}
//			else
//			{
//				//ok
//				//this->schedule(schedule_selector(HomeScene::scheduleHpTime),1.0f);
//
//				Player::getInstance()->setIsStarUpdate(true);
//			}
//		});

	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~touch event~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//**********************************schedule*******************************
void MainLayer::scheduleHpTime(float delayTime)
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
void MainLayer::scheduleMsg(float delayTime)
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
void MainLayer::updateHp()
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

void MainLayer::updateDiamond()
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

void MainLayer::updateGold()
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
void MainLayer::updateMail()
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

void MainLayer::updateTask()
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
void MainLayer::updateMsg()
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
			std::string _msgStr = Player::getInstance()->getLoopContent();
			//_msgStr.replace(" ","",);
			//_msgStr = _msgStr.replace(_msgStr.begin(),_msgStr.end()," ","");
			m_msgText->setText(_msgStr);
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

//role image
void MainLayer::updateRoleImg()
{
	int _roleType = Player::getInstance()->getRoleType();

	if(_roleType<4)
	{
		_roleType = 1;

		m_armatureHero_1->setVisible(true);

		m_armatureHero_2->setVisible(false);

		m_armatureHero_1->getAnimation()->play("Animation1");
	}
	else if(_roleType<8)
	{
		_roleType = 4;

		m_armatureHero_1->setVisible(false);

		m_armatureHero_2->setVisible(true);

		m_armatureHero_2->getAnimation()->play("Animation1");
	}

	//std::string _imgStr = StringUtils::format("commonUi/imgRoleB_%d.png",_roleType);
	//m_roleImage->loadTexture(_imgStr);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~update~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~