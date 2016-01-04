////TaskLayer.cpp created by php script. chris.li
#include "TaskLayer.h"
#include "Network.h"
#include "Player.h"
#include "../common/MyButton.h"
#include "../common/TextFloatLabelLayer.h"
#include "util/GameSound.h"
//
#include "ShopLayer.h"
#include "MailLayer.h"
#include "TaskLayer.h"
#include "TipsInfo.h"

bool TaskLayer::init()
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

void TaskLayer::onEnter()
{
	Layer::onEnter();

	m_taskListItem = static_cast<Layout*>(m_taskListItem->clone());

	CC_SAFE_RETAIN(m_taskListItem);

	refreshUI();

	Network::getInstance()->do_TaskList_Req([this](const AckInfo &ackInfo)
	{
		std::string msgInfo = ackInfo.getMessageInfo();

		if (msgInfo != "")
		{
			log("[TaskList_Ack] %s", msgInfo.c_str());
		}
		else
		{
			log("[TaskList_Ack] OK!");

			refreshUI();
		}
	});

	//schedule
	this->scheduleUpdate();
	this->schedule(schedule_selector(TaskLayer::scheduleHpTime),1.0f);
		//tips
	this->schedule(schedule_selector(TaskLayer::scheduleMsg),TIME_tipsScheduleDelay);

	//refresh
	refreshHp();
	refreshDiamond();
	refreshGold();
}

void TaskLayer::onExit()
{
	CC_SAFE_RELEASE(m_taskListItem);

	Layer::onExit();
}

void TaskLayer::loadUI()
{
	auto rootNode = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("TaskLayer/TaskLayer.json"));

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

	m_returnBtn-> addTouchEventListener(this, toucheventselector(TaskLayer::handleTouchEvent));

	//m_notifyMsgLabel = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_notifyMsgLabel"));

	m_taskListScrollView = static_cast<ScrollView*>(ui::Helper::seekWidgetByName(rootNode, "m_taskListScrollView"));

	m_taskListScrollView-> addTouchEventListener(this, toucheventselector(TaskLayer::handleTouchEvent));

	m_taskListView = static_cast<MyListView*>(ui::Helper::seekWidgetByName(rootNode, "m_taskListView"));

	//m_taskListView-> addTouchEventListener(this, toucheventselector(TaskLayer::handleTouchEvent));

	m_strengthMaxNum = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_strengthMaxNum"));

	m_addStrengthBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_addStrengthBtn"));

	m_addStrengthBtn-> addTouchEventListener(this, toucheventselector(TaskLayer::handleTouchEvent));

	m_diamondNum = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_diamondNum"));

	m_addDiamondBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_addDiamondBtn"));

	m_addDiamondBtn-> addTouchEventListener(this, toucheventselector(TaskLayer::handleTouchEvent));

	m_goldNum = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_goldNum"));

	m_addGoldBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_addGoldBtn"));

	m_addGoldBtn-> addTouchEventListener(this, toucheventselector(TaskLayer::handleTouchEvent));

	m_taskListItem = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "m_taskListItem"));

	m_taskListItem-> addTouchEventListener(this, toucheventselector(TaskLayer::handleTouchEvent));

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

	//msg
	m_notifyMsgLabel = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "lab_msg"));

	//dismiss the slider cause it can't work.
	m_taskSlider = static_cast<Slider*>(ui::Helper::seekWidgetByName(rootNode, "Slider_68"));
	m_taskListView->addTouchEventListener(this, toucheventselector(TaskLayer::listenEventTaskSlider));
	m_taskSlider->setTouchEnabled(false);
	//m_taskSlider->setVisible(true);

	//************* event listener ************************
	//event listener shop layer return call-back
	auto _listenerShop = EventListenerCustom::create("Return_shopLayer", [this](EventCustom* event) {
		log("AAAAAAAAAAAAAAA task layer");
		refreshHp();
		refreshGold();
		refreshDiamond();
	});

	_eventDispatcher->addEventListenerWithSceneGraphPriority(_listenerShop, this);
}

void TaskLayer::handleTouchEvent(Ref *pSender, TouchEventType type)
{


	if (pSender == nullptr)
	{
	}

	else if (pSender == m_returnBtn && (TOUCH_EVENT_ENDED == type))
	{
		GameSound::playEffectMusic(EFF_CLOSE_PAGE);

		//listen event publish
		std::string eventName = "Return_taskLayer";
		EventCustom event(eventName);
		Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);

		this->removeFromParentAndCleanup(true);
	}

	else if (pSender == m_taskListScrollView && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == m_taskListView && (TOUCH_EVENT_ENDED == type))
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
		GameSound::playEffectMusic(EFF_CHANGE_PAGE);

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

	else if (pSender == m_taskListItem && (TOUCH_EVENT_ENDED == type))
	{
	}
	else if (TOUCH_EVENT_ENDED == type)
	{
		GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);

		auto senderNode = static_cast<Node*>(pSender);
		auto taskUserObject = static_cast<TaskUserObject*>(senderNode->getUserObject());

		if (taskUserObject->name == "mm_acceptBtn")
		{
			log("task getting!");

			Network::getInstance()->do_TaskAccept_Req(taskUserObject->taskId, [this](const AckInfo &ackInfo)
			{
				std::string msgInfo = ackInfo.getMessageInfo();

				if (msgInfo != "")
				{
					log("[TaskAccept_Ack] %s", msgInfo.c_str());
				}
				else
				{
					afterNetwork();

					refreshGold();
					refreshDiamond();

					std::string json = cocos2d::FileUtils::getInstance()->getStringFromFile("MESSAGE_INFO.json");

					rapidjson::Document d;

					if ("" != json)
					{
						d.Parse<0>(json.c_str());
					}

					std::string keyStr = StringUtils::format("%d", 25);
					const char *key = keyStr.c_str();

					std::string discript = (d)[keyStr.c_str()]["text1"].GetString();

					TextFloatLabelLayer::getInstance()->show(discript.c_str());
					log("[TaskAccept_Ack] OK!");

				}
			});

			//afterNetwork();
		}

	}
}

void TaskLayer::afterNetwork()
{
	refreshUI();
	Network::getInstance()->do_TaskList_Req([this](const AckInfo &ackInfo)
	{
		std::string msgInfo = ackInfo.getMessageInfo();

		if (msgInfo != "")
		{
			log("TaskAccept_Ack[TaskList_Ack] %s", msgInfo.c_str());
		}
		else
		{
			log("TaskAccept_Ack[TaskList_Ack] OK!");

			refreshUI();
		}
	});

}

void TaskLayer::refreshUI()
{

	m_taskListView->removeAllItems();

	auto taskList = Player::getInstance()->getTaskList();

	for (auto taskItem :(*taskList))
	{
		auto item = static_cast<Layout*>(m_taskListItem->clone());
		auto button = static_cast<MyButton*>(ui::Helper::seekWidgetByName(item,"mm_acceptBtn"));

		//Task Label and Price Label
		auto textLabel = static_cast<Text*>(ui::Helper::seekWidgetByName(item, "mm_taskDespLabel"));

		std::string json = cocos2d::FileUtils::getInstance()->getStringFromFile("TASK_INFO.json");

		rapidjson::Document d;

		if ("" != json)
		{
			d.Parse<0>(json.c_str());
		}

		std::string keyStr = StringUtils::format("%d", taskItem->taskId);
		//std::string keyStr = StringUtils::format("%d", 10084);
		const char *key = keyStr.c_str();

		std::string discript = (d)[keyStr.c_str()]["discript"].GetString();
		discript +=  + "\n";

		int coins = (d)[keyStr.c_str()]["coin"].GetInt();
		int diamonds = (d)[keyStr.c_str()]["diamond"].GetInt();

		std::string defaultStr = StringUtils::format("%d", 10000);
		std::string price = (d)[defaultStr.c_str()]["price"].GetString();
		discript += price;

		if (coins > 0)
		{
			std::string coinText = (d)[defaultStr.c_str()]["coin"].GetString();
			discript += coinText +  StringUtils::format("%d", coins);
		}

		if (diamonds > 0)
		{
			std::string diamondText = (d)[defaultStr.c_str()]["diamond"].GetString();
			discript += diamondText +  StringUtils::format("%d", diamonds);
		}
		textLabel->setText(discript);

		//Price Type mm_taskImg

		auto priceTypeCoin = static_cast<ListView*>(ui::Helper::seekWidgetByName(item, "mm_taskImg"));
		auto priceTypeDiamond = static_cast<ListView*>(ui::Helper::seekWidgetByName(item, "mm_taskImg_0"));

		if (coins > 0)
		{
			priceTypeDiamond->setVisible(false);
			priceTypeCoin->setVisible(true);
		}

		if (diamonds > 0)
		{
			priceTypeDiamond->setVisible(true);
			priceTypeCoin->setVisible(false);
		}		

		//int i = (d)[keyStr.c_str()]["coin"].GetInt();
		button-> addTouchEventListener(this, toucheventselector(TaskLayer::handleTouchEvent));

		if (taskItem->taskState == 0)
		{
			//task price can't be accepted.
			button->setOnPressEnable(false);

		}
		else if (taskItem->taskState == 1)
		{
			button->setOnPressEnable(true);
		}
		else
		{
			//button->setVisible(false);
		}

		auto taskUserObject = TaskUserObject::create("mm_acceptBtn",taskItem->taskId,taskItem->taskState);
		button->setUserObject(taskUserObject);

		auto bar = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(item,"mm_taskProgressBar"));
		float percent = taskItem->taskTimes / taskItem->taskAllTimes;
		bar->setPercent(percent * 100);

		auto barLabel = static_cast<TextAtlas*>(ui::Helper::seekWidgetByName(item,"mm_taskProgressLabel"));
		char times[20];
		char allTimes[20];

		sprintf(times,"%d",taskItem->taskTimes);
		sprintf(allTimes,"%d",taskItem->taskAllTimes);

		std::string a(times);
		std::string b(allTimes);

		std::string text = a + "/" + b;
		barLabel->setStringValue(text);
		m_taskListView->pushBackCustomItem(item);
	}

	m_taskListView->updateInnerContainerSizeV();
	m_taskListView->updateSizeAndPosition();

	float t_gapMax = m_taskListView->getInnerContainer()->getPosition().y;
	if(t_gapMax)
	{
		m_sliderSwitch = true;
		m_sliderGapMax = abs(t_gapMax);
	}
	else
		m_sliderSwitch = false;
}

void TaskLayer::update(float delayTime)
{
	//set slider
	float t_sliderScale = m_sliderGap/m_sliderGapMax;
	if(t_sliderScale<0)
		m_taskSlider->setPercent(4);
	else if(t_sliderScale<1)
		m_taskSlider->setPercent(4+92*(t_sliderScale));
	else
		m_taskSlider->setPercent(96);
}

void TaskLayer::listenEventTaskSlider(Ref *pSender, TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_BEGAN:
		//log("touch begin : %f %f",t_sender->getTouchStartPos().x,t_sender->getTouchStartPos().y);
		break;
	case TOUCH_EVENT_MOVED:

		//show Slider 
		m_sliderGap = m_sliderGapMax + m_taskListView->getInnerContainer()->getPosition().y;

		if(m_sliderSwitch)
			m_taskSlider->setVisible(true);
		break;

	case TOUCH_EVENT_ENDED:

		m_taskSlider->setVisible(false);
		break;

	case TOUCH_EVENT_CANCELED:

		m_taskSlider->setVisible(false);
		break;

	default:
		break; 
	}
}

//**********************************schedule******************************
void TaskLayer::scheduleHpTime(float delayTime)
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

		m_strengthMaxNum->setText(t_timeText->getCString());

		if(m_HpNum<5)
			m_hpLoadingBarV.at(m_HpNum)->setPercent(100.0f*(1.0f-_remainTime/Player::getInstance()->getMaxTime()));
	}
	if(Player::getInstance()->getIsStarUpdate())
		refreshHp();
}

void TaskLayer::scheduleMsg(float delayTime)
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
void TaskLayer::refreshHp()
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
void TaskLayer::refreshDiamond()
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
void TaskLayer::refreshGold()
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

void TaskLayer::refreshMsg()
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