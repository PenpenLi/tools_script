#include "SetLayer.h"
#include "ShopLayer.h"
#include "Network.h"
#include "Player.h"
#include "util/GameSound.h"
//
#include "UiConfig.h"
#include "TipsInfo.h"
#include "MailLayer.h"
#include "TaskLayer.h"
#include "RankLayer.h"
#include "NoticeLayer.h"
#include "login/StartAnimation.h"
#include "SimpleAudioEngine.h"
#include "common/TextFloatLabelLayer.h"
#include "MyHelper.h"

bool SetLayer::init()
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

void SetLayer::onEnter()
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
	//
	updateSetPal();

	//schedule
	this->schedule(schedule_selector(SetLayer::scheduleHpTime),1.0f);
	//tips
	this->schedule(schedule_selector(SetLayer::scheduleMsg),TIME_tipsScheduleDelay);

	//test

}

void SetLayer::onExit()
{
	Layer::onExit();

	//schedule
	this->unschedule(schedule_selector(SetLayer::scheduleHpTime));
	this->unschedule(schedule_selector(SetLayer::scheduleMsg));
}

//init
void SetLayer::variableInit()
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

	m_setPal = 0;
	m_sendHeartCb = 0;
	m_musicBgSdr = 0;
	m_soundEffectSdr = 0;
	m_sensitivityCbV.clear();
	m_platformBtn = 0;
	m_QABtn = 0;
	m_noviceBtn = 0;
	m_feedbackBtn = 0;
	m_thanksBtn = 0;

	m_QAPal = 0;
	m_QAReturnBtn = 0;

	m_feedbackPal = 0;
	m_feedbackText = 0;
	m_sendFeedbackBtn = 0;
	m_feedbackReturnBtn = nullptr;

	m_thanksPal = 0;
	m_giftTF = nullptr;
	m_nickNameText = nullptr;
	m_highestScoreText = nullptr;
	m_highestLevelsText = nullptr;
	m_taleBtn = 0;


	//~pal gift~
	m_giftPal = nullptr;
	m_giftGetBtn = nullptr;
	m_giftReturnBtn = nullptr;

	m_giftBtn = nullptr;

	m_Hptime = 0;
	m_HpNum = 0;

	m_msgImg = nullptr;
	m_msgText = nullptr;

	m_returnBtn = 0;
	//msg time 
	m_msgTime = 0.0f;
	m_msgSwitch = false;
	m_scrollSwitch = false;
	m_noticeSwitch = true;
	m_msgEndPosX = 0.0f;
}

bool SetLayer::importUI()
{
	bool bRet = false;
	do{
		log("set layer import ~~~~");
		Widget* t_widget = GUIReader::getInstance()->widgetFromJsonFile("SetUi/SetUi.json");
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
			m_hpAddBtn->addTouchEventListener(this, toucheventselector(SetLayer::touchEventShopBtn));
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
			m_diamondAddBtn->addTouchEventListener(this, toucheventselector(SetLayer::touchEventShopBtn));
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
			m_goldAddBtn->addTouchEventListener(this, toucheventselector(SetLayer::touchEventShopBtn));
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
			m_mailBtn->addTouchEventListener(this, toucheventselector(SetLayer::touchEventActBtn));
		}
		else
			log("~~error~~ Mail button is Null ");

		//task
		m_taskNumText = static_cast<Text*>(m_activityPal->getChildByName("btn_task")->getChildByName("img_taskNumBg")->getChildByName("lab_taskNum"));
		m_taskBtn = static_cast<Button*>(m_activityPal->getChildByName("btn_task"));
		if (m_taskBtn)
		{
			m_taskBtn->setTag(TAG_btnTask);
			m_taskBtn->addTouchEventListener(this, toucheventselector(SetLayer::touchEventActBtn));
		}
		else
			log("~~error~~ Task button is Null ");

		//rank
		m_rankBtn = static_cast<Button*>(m_activityPal->getChildByName("btn_rank"));
		if (m_rankBtn)
		{
			m_rankBtn->setTag(TAG_btnRank);
			m_rankBtn->addTouchEventListener(this, toucheventselector(SetLayer::touchEventActBtn));
		}
		else
			log("~~error~~ Rank button is Null ");

		//act
		m_activityBtn = static_cast<Button*>(m_activityPal->getChildByName("btn_activity"));
		if (m_activityBtn)
		{
			m_activityBtn->setTag(TAG_btnActivity);
			m_activityBtn->addTouchEventListener(this, toucheventselector(SetLayer::touchEventActBtn));
		}
		else
			log("~~error~~ Activity button is Null ");

		//~pal set~
		m_setPal = static_cast<Layout*>(t_widget->getChildByName("pal_set"));
		//init
		m_setPal->setLocalZOrder(ZORDER_front);

		m_sendHeartCb = static_cast<CheckBox*>(m_setPal->getChildByName("pal_sendHeartSwitch")->getChildByName("cb_sendHeartSwitch"));
		if(m_sendHeartCb)
		{
			m_sendHeartCb->addTouchEventListener(this,toucheventselector(SetLayer::touchEventHeartCheckBox));
		}

		m_musicBgSdr = static_cast<Slider*>(m_setPal->getChildByName("pal_musicBg")->getChildByName("sdr_musicBg"));
		if(m_musicBgSdr)
		{
			//m_musicBgSdr->moveEvent
			m_musicBgSdr->setTag(TAG_musicBg);
			m_musicBgSdr->addEventListenerSlider(this,sliderpercentchangedselector(SetLayer::touchEventSoundSdr));
		}

		m_soundEffectSdr = static_cast<Slider*>(m_setPal->getChildByName("pal_soundEffect")->getChildByName("sdr_soundEffect"));
		if (m_soundEffectSdr)
		{
			m_soundEffectSdr->setTag(TAG_soundEffect);
			m_soundEffectSdr->addEventListenerSlider(this,sliderpercentchangedselector(SetLayer::touchEventSoundSdr));
		}

		Widget* t_sensitivityCbWidget = m_setPal->getChildByName("pal_sensitivity")->getChildByName("img_cbBg");

		CheckBox* t_sensitivityCb = 0;
		String* t_sensitivityCbStr = 0;
		int count = 3;
		for (int i = 0; i < count; i++)
		{
			t_sensitivityCbStr = String::createWithFormat("cb_sensitivityX%d",i+1);
			t_sensitivityCb = static_cast<CheckBox*>(t_sensitivityCbWidget->getChildByName(t_sensitivityCbStr->getCString()));
			t_sensitivityCb->setTag(TAG_sensitivityCb+i+1);
			t_sensitivityCb->addTouchEventListener(this,toucheventselector(SetLayer::touchEventSensitivityCb));
			m_sensitivityCbV.pushBack(t_sensitivityCb);
		}

		m_QABtn = static_cast<Button*>(m_setPal->getChildByName("btn_QA"));
		if (m_QABtn)
		{
			m_QABtn->setTag(TAG_setBtnQA);
			m_QABtn->addTouchEventListener(this,toucheventselector(SetLayer::touchEventSetBtn));
		}
		else
			log("~~error~~ QA button is Null ");

		m_noviceBtn = static_cast<Button*>(m_setPal->getChildByName("btn_novice"));
		if (m_noviceBtn)
		{
			m_noviceBtn->setTag(TAG_setBtnNovice);
			//m_noviceBtn->addTouchEventListener(this,toucheventselector(SetLayer::touchEventSetBtn));
		}
		else
			log("~~error~~ novice button is Null ");

		m_feedbackBtn = static_cast<Button*>(m_setPal->getChildByName("btn_feedback"));
		if (m_feedbackBtn)
		{
			m_feedbackBtn->setTag(TAG_setBtnFeedback);
			m_feedbackBtn->addTouchEventListener(this,toucheventselector(SetLayer::touchEventSetBtn));
		}
		else
			log("~~error~~ feedback button is Null ");

		m_thanksBtn = static_cast<Button*>(m_setPal->getChildByName("btn_thanks"));
		if (m_thanksBtn)
		{
			m_thanksBtn->setTag(TAG_setBtnThanks);
			m_thanksBtn->addTouchEventListener(this,toucheventselector(SetLayer::touchEventSetBtn));
		}
		else
			log("~~error~~ thanks button is Null ");

		//~pal QA~
		m_QAPal = static_cast<Layout*>(t_widget->getChildByName("pal_QA"));
		m_QAPal->setEnabled(false);
		m_QAPal->setVisible(true);
		m_QAPal->setLocalZOrder(ZORDER_front);

		m_QAReturnBtn = static_cast<Button*>(m_QAPal->getChildByName("btn_return"));
		if (m_QAReturnBtn)
		{
			m_QAReturnBtn->addTouchEventListener(this,toucheventselector(SetLayer::touchEventQAReturnBtn));
		}
		else
			log("~~error~~ QA return button is Null ");

		//~pal feedback~
		m_feedbackPal = static_cast<Layout*>(t_widget->getChildByName("pal_feedback"));
		m_feedbackPal->setEnabled(false);
		m_feedbackPal->setVisible(true);
		m_feedbackPal->setLocalZOrder(ZORDER_front);

		m_feedbackText = static_cast<TextField*>(Helper::seekWidgetByName(m_feedbackPal,"tf_sendFeed"));

		m_feedbackText->addEventListenerTextField(this,textfieldeventselector(SetLayer::textFieldEventFeedback));

		m_sendFeedbackBtn = static_cast<Button*>(m_feedbackPal->getChildByName("img_frame")->getChildByName("btn_sendFeedBack"));
		if (m_sendFeedbackBtn)
		{
			m_sendFeedbackBtn->addTouchEventListener(this,toucheventselector(SetLayer::touchEventFeedbackBtn));
		}
		else
			log("~~error~~ send Feedback button is Null ");

		m_feedbackReturnBtn = static_cast<Button*>(Helper::seekWidgetByName(m_feedbackPal,"btn_feedbackReturn"));
		if (m_feedbackReturnBtn)
		{
			m_feedbackReturnBtn->addTouchEventListener(this,toucheventselector(SetLayer::touchEventFeedbackBtn));
		}
		else
			log("~~error~~ Feedback return button is Null ");

		m_strNumText = static_cast<Text*>(Helper::seekWidgetByName(m_feedbackPal,"lab_strNum"));
		m_strNumText->setVisible(false);

		//~pal thanks~
		m_thanksPal = static_cast<Layout*>(t_widget->getChildByName("pal_thanks"));
		m_thanksPal->setEnabled(false);
		m_thanksPal->setVisible(true);
		m_thanksPal->setLocalZOrder(ZORDER_front);

		m_thanksPal->addTouchEventListener(this,toucheventselector(SetLayer::touchEventThanksPal));

		m_nickNameText = static_cast<Text*>(Helper::seekWidgetByName(m_thanksPal,"lab_playerName"));

		m_highestScoreText = static_cast<Text*>(Helper::seekWidgetByName(m_thanksPal,"lab_score"));

		m_highestLevelsText = static_cast<Text*>(Helper::seekWidgetByName(m_thanksPal,"lab_levels"));

		m_taleBtn = static_cast<Button*>(m_thanksPal->getChildByName("btn_taleBg"));
		if (m_taleBtn)
		{ 
			m_taleBtn->addTouchEventListener(this,toucheventselector(SetLayer::touchEventFaleBtn));
		}
		else
			log("~~error~~ backgroud tale button is Null ");

		//~pal gift~
		m_giftBtn = static_cast<Button*>(Helper::seekWidgetByName(t_widget,"btn_gift"));

		//hide the gift button
		m_giftBtn->setEnabled(false);

		if (m_giftBtn)
		{ 
			m_giftBtn->addTouchEventListener(this,toucheventselector(SetLayer::touchEventGiftBtn));
		}
		else
			log("~~error~~ gift button is Null ");

		m_giftPal = static_cast<Layout*>(Helper::seekWidgetByName(t_widget,"pal_gift"));
		m_giftPal->setEnabled(false);
		m_giftPal->setVisible(true);
		m_giftPal->setLocalZOrder(ZORDER_front);

		m_giftTF = static_cast<TextField*>(Helper::seekWidgetByName(m_giftPal,"tf_gift"));

		m_giftGetBtn = static_cast<Button*>(Helper::seekWidgetByName(m_giftPal,"btn_giftGet"));

		if (m_giftGetBtn)
		{ 
			m_giftGetBtn->addTouchEventListener(this,toucheventselector(SetLayer::touchEventGiftPalBtn));
		}
		else
			log("~~error~~ gift Get button is Null ");

		m_giftReturnBtn = static_cast<Button*>(Helper::seekWidgetByName(m_giftPal,"btn_giftReturn"));

		if (m_giftReturnBtn)
		{ 
			m_giftReturnBtn->addTouchEventListener(this,toucheventselector(SetLayer::touchEventGiftPalBtn));
		}
		else
			log("~~error~~ gift Get button is Null ");

		//Msg text
		m_msgImg = static_cast<ImageView*>(t_widget->getChildByName("img_msgBg"));

		m_msgText = static_cast<Text*>(m_msgImg->getChildByName("svp_msg")->getChildByName("lab_msg"));

		//return
		m_returnBtn = static_cast<Button*>(t_widget->getChildByName("btn_return"));
		if (m_returnBtn)
		{
			//m_returnBtn->setEnabled(false);
			m_returnBtn->addTouchEventListener(this, toucheventselector(SetLayer::touchEventReturnBtn));
		}
		else
			log("~~error~~ Return button is Null ");

		auto _listenerShop = EventListenerCustom::create("Return_shopLayer", [this](EventCustom* event) {
			log("AAAAAAAAAAAAAAA set layer");
			updateHp();
			updateGold();
			updateDiamond();
		});

		_eventDispatcher->addEventListenerWithSceneGraphPriority(_listenerShop, this);

		//event listener mail layer return call-back
		auto _listenerMail = EventListenerCustom::create("Return_mailLayer", [this](EventCustom* event) {
			log("BBBBBBBBBBBBBBB set layer");
			updateMail();
			updateHp();
			updateGold();
			updateDiamond();
		});

		_eventDispatcher->addEventListenerWithSceneGraphPriority(_listenerMail, this);

		auto _listenerTask = EventListenerCustom::create("Return_taskLayer", [this](EventCustom* event) {
			log("CCCCCCCCCCCCCC set layer");
			updateTask();
			updateGold();
			updateDiamond();
		});

		_eventDispatcher->addEventListenerWithSceneGraphPriority(_listenerTask, this);

		////disable

		bRet = true;
	}while(0);

	return bRet;
}

//**********************************touch event*******************************
//Assets
void SetLayer::touchEventShopBtn(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_CHANGE_PAGE);
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
void SetLayer::touchEventActBtn(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_CHANGE_PAGE);
	Widget* t_sender = static_cast<Widget*>(pSender);
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		log("activity");

		int _zOrder = this->getLocalZOrder();

		MailLayer* _mailLayer = nullptr;

		RankLayer* _rankLayer = nullptr;

		TaskLayer* _taskLayer = nullptr;

		//NoticeLayer* _notcieLayer = nullptr;

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

			//save and uploading setup
			//Set Control Req
			if(m_sensitivityCbV.at(0)->getSelectedState())
				Player::getInstance()->setSensitivityInc(1);
			else if(m_sensitivityCbV.at(1)->getSelectedState())
				Player::getInstance()->setSensitivityInc(2);
			else if(m_sensitivityCbV.at(2)->getSelectedState())
				Player::getInstance()->setSensitivityInc(3);
			else
				log("sensitivity error~~~~!!!!");

			//set local sensitivity & musicInc & soundInc
			Player::getInstance()->setHelpInc(m_sendHeartCb->getSelectedState());
			Player::getInstance()->setBgmInc(m_musicBgSdr->getPercent());
			Player::getInstance()->setSoundInc(m_soundEffectSdr->getPercent());

			Network::getInstance()->do_SetControl_Req(m_sendHeartCb->getSelectedState(),m_musicBgSdr->getPercent(),m_soundEffectSdr->getPercent(),Player::getInstance()->getSensitivityInc(),[=](const AckInfo &ackInfo) 
			{
				std::string msgInfo = ackInfo.getMessageInfo();

				if (msgInfo != "")
				{
					log("[SetControl] %s", msgInfo.c_str());
				}
				else
				{
					//ok
					log("SetControl sucess~!");

					NoticeLayer* _notcieLayer = NoticeLayer::create();

					_notcieLayer->setLocalZOrder(_zOrder+1);

					this->getParent()->addChild(_notcieLayer);
				}
			});



			break;
			//default:
			//	log("~~~~~~~~~~~~~error : not found Activity button tag");
			//	break;
		}
		break;

	}
}

//send heart CheckBox
void SetLayer::touchEventHeartCheckBox(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		log("send heart CheckBox");
		//if(m_sendHeartCb->getSelectedState())
		//	//set the checkBox opposite  , checkBox changed later
		//		Player::getInstance()->setHelpInc(false);
		//else
		//	Player::getInstance()->setHelpInc(true);
		break;
	default:
		break;
	}
}

//Sensitivity CheckBox
void SetLayer::touchEventSensitivityCb(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);
	Widget* t_sender = static_cast<Widget*>(pSender);
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		for( auto& element : m_sensitivityCbV)
		{
			//if(element->getTag()==t_sender->getTag())

			//set all state false  , the sender is negate
			element->setSelectedState(false);
			//else
			//element->setSelectedState(false);
		}
		break;
	default:
		break;
	}
}
//music & sound slider
void SetLayer::touchEventSoundSdr(Ref *pSender, SliderEventType type)
{
	Widget* t_sender = static_cast<Widget*>(pSender);
	//log("type : %d ",type);
	//t_sender->hit
	switch (type)
	{
	case SLIDER_PERCENTCHANGED:
		if (t_sender->getTag() == TAG_musicBg)
		{
			log("Music Slider : %d !!!!",m_musicBgSdr->getPercent());
			float sb = m_musicBgSdr->getPercent() / 100.0;
			CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(sb);
		}
		else if (t_sender->getTag() == TAG_soundEffect)
		{
			log("Sound Slider : %d !!!!",m_soundEffectSdr->getPercent());
			float se = m_soundEffectSdr->getPercent() / 100.0;
			CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(se);
		}

	default:
		break;
	}
}

//set button
void SetLayer::touchEventSetBtn(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);
	Widget* t_sender = static_cast<Widget*>(pSender);
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		log("set button");
		//hide the set main layer
		m_assetsPal->setEnabled(false);
		m_activityPal->setEnabled(false);
		m_setPal->setEnabled(false);
		m_returnBtn->setEnabled(false);
		m_msgImg->setEnabled(false);
		m_giftBtn->setEnabled(false);

		switch (t_sender->getTag())
		{
		case TAG_setBtnQA:
			log("QA button");
			m_QAPal->setEnabled(true);
			break;
		case TAG_setBtnNovice:
			log("Novice button");
			break;
		case TAG_setBtnFeedback:
			log("Feedback button");
			//clean up text
			m_feedbackText->setText("");
			m_feedbackPal->setEnabled(true);
			//CW
			//this->schedule(schedule_selector(SetLayer::scheduleFeedback),0.3f);
			break;
		case TAG_setBtnThanks:
			log("Thanks button");
			m_nickNameText->setText(Player::getInstance()->getNickname());

			Network::getInstance()->do_FriendInfo_Req(Player::getInstance()->getMyPlayerId(),[this](const AckInfo &ackInfo) 
			{
				std::string msgInfo = ackInfo.getMessageInfo();

				if (msgInfo != "")
				{
					log("[MyInfo] %s", msgInfo.c_str());
				}
				else
				{
					//ok
					std::string _scoreStr = StringUtils::format("%s%s",TipsInfo::getInstance()->getMessage("highest_score").c_str(),MyHelper::changeToScoreStringCenter(Player::getInstance()->getHighestScore()).c_str());

					m_highestScoreText->setText(_scoreStr);

					std::string _levelsStr = StringUtils::format("%s%d",TipsInfo::getInstance()->getMessage("highest_levels").c_str(),Player::getInstance()->getHighestLevels());

					m_highestLevelsText->setText(_levelsStr);

					m_thanksPal->setEnabled(true);
				}
			});

			break;
		default:
			log("~~~error: set button not found tag");
			break;
		}
		break;
	default:
		break;
	}
}

//return 
void SetLayer::touchEventReturnBtn(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_CLOSE_PAGE);
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		log("~~~~~Set Layer return!!");

		uploadingSetup();

		this->removeFromParentAndCleanup(true);

		break;
	default:
		break;
	}
}

void SetLayer::touchEventQAReturnBtn(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_CLOSE_PAGE);
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		log("~~~~~touchEventQAReturnBtn !!");
		m_assetsPal->setEnabled(true);
		m_activityPal->setEnabled(true);
		m_setPal->setEnabled(true);
		m_returnBtn->setEnabled(true);
		m_msgImg->setEnabled(true);
		m_giftBtn->setEnabled(true);

		m_QAPal->setEnabled(false);
		break;
	default:
		break;
	}
}

void SetLayer::touchEventFeedbackBtn(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);
	if(TOUCH_EVENT_ENDED==type)
	{
		if(pSender==m_sendFeedbackBtn)
		{
			if(0==m_feedbackText->getStringValue().size())
			{
				TextFloatLabelLayer::getInstance()->show(TipsInfo::getInstance()->getMessage("feedback_null"));
			}
			else
			{
				m_assetsPal->setEnabled(true);
				m_activityPal->setEnabled(true);
				m_setPal->setEnabled(true);
				m_returnBtn->setEnabled(true);
				m_msgImg->setEnabled(true);
				this->unschedule(schedule_selector(SetLayer::scheduleFeedback));

				m_feedbackPal->setEnabled(false);
				//feed back up to server
				std::string tmpStr = m_feedbackText->getStringValue();
				Network::getInstance()->do_Opinion_Req(tmpStr,[this](const AckInfo &ackInfo) 
				{
					std::string msgInfo = ackInfo.getMessageInfo();

					if (msgInfo != "")
					{
						log("[] %s", msgInfo.c_str());
					}
					else
					{
						//ok
						//log("");
					}
				});
			}
		}
		else if(pSender==m_feedbackReturnBtn)
		{
			m_assetsPal->setEnabled(true);
			m_activityPal->setEnabled(true);
			m_setPal->setEnabled(true);
			m_returnBtn->setEnabled(true);
			m_msgImg->setEnabled(true);
			m_giftBtn->setEnabled(true);
			//CW
			//this->unschedule(schedule_selector(SetLayer::scheduleFeedback));

			m_feedbackPal->setEnabled(false);
		}
	}
}

void SetLayer::touchEventThanksPal(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);
	//log("~~~~~touchEventFaleBtn !!");
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		log("~~~~~touchEventThanksPal !!");

		m_assetsPal->setEnabled(true);
		m_activityPal->setEnabled(true);
		m_setPal->setEnabled(true);
		m_returnBtn->setEnabled(true);
		m_msgImg->setEnabled(true);
		m_giftBtn->setEnabled(true);

		m_thanksPal->setEnabled(false);
		break;
	default:
		break;
	}
}

void SetLayer::touchEventFaleBtn(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_CHANGE_PAGE);
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		log("~~~~~touchEventFaleBtn !!");

		this->setVisible(false);

		auto layer = StartAnimation::create();

		layer->setEnterType(1);

		getScene()->addChild(layer);
		//		this->addChild(layer);
		//		auto sc = layer->getScene();

		//		Director::getInstance()->replaceScene(sc);
		break;
		//default:
		//	break;
	}
}

//gift
void SetLayer::touchEventGiftBtn(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);
	if(TOUCH_EVENT_ENDED == type)
	{
		m_assetsPal->setEnabled(false);
		m_activityPal->setEnabled(false);
		m_setPal->setEnabled(false);
		m_returnBtn->setEnabled(false);
		m_msgImg->setEnabled(false);
		m_giftBtn->setEnabled(false);

		m_giftPal->setEnabled(true);

		m_giftTF->setText("");
	}
}

void SetLayer::touchEventGiftPalBtn(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);
	if(TOUCH_EVENT_ENDED == type)
	{
		if(m_giftGetBtn == pSender)
		{
			log("get gift ~~~");
			if(12==m_giftTF->getStringValue().size())
			{
				Network::getInstance()->do_Gift_Req(m_giftTF->getStringValue(),[this](const AckInfo &ackInfo) 
				{
					std::string msgInfo = ackInfo.getMessageInfo();

					if (msgInfo != "")
					{
						log("[GiftGet] %s", msgInfo.c_str());
					}
					else
					{
						//ok
						updateHp();
						updateGold();
						updateDiamond();

						TextFloatLabelLayer::getInstance()->show(Player::getInstance()->getGiftInfo());
					}
				});
			}
			else
			{
				TextFloatLabelLayer::getInstance()->show(TipsInfo::getInstance()->getMessage("gift_codeErr"));
			}
		}
		else if(m_giftReturnBtn == pSender)
		{
			m_assetsPal->setEnabled(true);
			m_activityPal->setEnabled(true);
			m_setPal->setEnabled(true);
			m_returnBtn->setEnabled(true);
			m_msgImg->setEnabled(true);
			m_giftBtn->setEnabled(true);

			m_giftPal->setEnabled(false);
		}
		else
		{
			log("gift pal~ error");
		}
	}
}

//textfield event
void SetLayer::textFieldEventFeedback(Ref *pSender,TextFiledEventType type)
{
	if(TEXTFIELD_EVENT_ATTACH_WITH_IME==type)
	{
		log("TEXTFIELD_EVENT_ATTACH_WITH_IME");
		//m_feedbackText->setAttachWithIME(true);
	}
	else if(TEXTFIELD_EVENT_DETACH_WITH_IME==type)
	{
		log("TEXTFIELD_EVENT_DETACH_WITH_IME");
	}
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~touch event~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//**********************************schedule*******************************
void SetLayer::scheduleHpTime(float delayTime)
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
void SetLayer::scheduleMsg(float delayTime)
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

//Feedback fieldText string number 
void SetLayer::scheduleFeedback(float delayTime)
{
	std::string _numStr = StringUtils::format("%d/99",m_feedbackText->getStringValue().size());
	m_strNumText->setText(_numStr);
	//m_feedbackText->setInsertText(false);
	//m_feedbackText->setTouchAreaEnabled(false);
	//m_feedbackText->setMaxLengthEnabled(false);
	//m_feedbackText->setAttachWithIME(false);
	//m_feedbackText->setDetachWithIME(false);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~schedule~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//**********************************update*******************************
//assets
void SetLayer::updateHp()
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

void SetLayer::updateDiamond()
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

void SetLayer::updateGold()
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
void SetLayer::updateMail()
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

void SetLayer::updateTask()
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

void SetLayer::updateMsg()
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

//set panel
void SetLayer::updateSetPal()
{
	m_sendHeartCb->setSelectedState(Player::getInstance()->getHelpInc());	//CW
	m_musicBgSdr->setPercent(Player::getInstance()->getBgmInc());	//CW
	m_soundEffectSdr->setPercent(Player::getInstance()->getSoundInc());	//CW
	m_sensitivityCbV.at(Player::getInstance()->getSensitivityInc()-1)->setSelectedState(true);		//CW
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~update~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//***************************helper****************************
void SetLayer::uploadingSetup()
{
	//Set Control Req parameter
	bool _friendsHeart;
	int _musicPercent = 0;
	int _SoundPercent = 0;
	int _sensitivity = 0;

	//Set Control Req
	_friendsHeart = m_sendHeartCb->getSelectedState();

	_musicPercent = m_musicBgSdr->getPercent();

	_SoundPercent = m_soundEffectSdr->getPercent();

	if(m_sensitivityCbV.at(0)->getSelectedState())
		_sensitivity = 1;
	else if(m_sensitivityCbV.at(1)->getSelectedState())
		_sensitivity = 2;
	else if(m_sensitivityCbV.at(2)->getSelectedState())
		_sensitivity = 3;
	else
		log("sensitivity error~~~~!!!!");

	//set local sensitivity & musicInc & soundInc
	Player::getInstance()->setHelpInc(_friendsHeart);
	Player::getInstance()->setSensitivityInc(_sensitivity);
	Player::getInstance()->setBgmInc(_musicPercent);
	Player::getInstance()->setSoundInc(_SoundPercent);


	Network::getInstance()->do_SetControl_Req(_friendsHeart,_musicPercent,_SoundPercent,_sensitivity,[this](const AckInfo &ackInfo) 
	{
		std::string msgInfo = ackInfo.getMessageInfo();

		if (msgInfo != "")
		{
			log("[SetControl] %s", msgInfo.c_str());
		}
		else
		{
			//ok
			log("SetControl sucess~!");
		}
	});
}