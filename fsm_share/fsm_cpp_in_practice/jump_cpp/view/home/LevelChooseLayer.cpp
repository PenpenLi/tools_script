////LevelChooseLayer.cpp created by php script. chris.li
#include "LevelChooseLayer.h"
#include "../common/MyButton.h"
#include "Player.h"
#include "BattleData.h"
#include "Network.h"
#include "battle/BattleScene.h"
#include "battle/LoadLayer.h"
#include "util/GameSound.h"
//
#include "ShopLayer.h"
#include "MailLayer.h"
#include "TaskLayer.h"
#include "RankLayer.h"
#include "NoticeLayer.h"
#include "../common/NotifyBoxUILayer.h"
#include "../common/TextFloatLabelLayer.h"
#include "TipsInfo.h"
//
#include "HomeScene.h"

bool LevelChooseLayer::init()
{
	if (Layer::init())
	{
		//member variable init
		//Hp
		m_HpNum = 0;
		m_hpLoadingBarV.clear();

		loadUI();
		return true;
	}

	return false;
}
void LevelChooseLayer::onEnter()
{
	Layer::onEnter();

	///////////////////test////////////////////////

	//network : update task count and mail count
	Network::getInstance()->do_TaskCount_Req([this](const AckInfo &ackInfo){});

	for (int i = 1; i < 41; i++)
	{
		setLevelShell(i, false);
	}

	for (int i = 1; i < 41; i++)
	{
		for (int j = 1; j < 4; j++)
		{
			setLevelStarOn(i, j, false);
			setLevelStarOff(i, false);
		}
		setLevelStarOff(i, false);
	}

	for (int i = 2; i < 41; i++)
	{
		startLevelRoute(i, false);
	}

	m_newStage = 0;
	//Network
	Network::getInstance()->do_StageList_Req([this](const AckInfo &ackInfo)
	{
		std::string msgInfo = ackInfo.getMessageInfo();

		if (msgInfo != "")
		{
			log("[StageList_Ack] %s", msgInfo.c_str());
		}
		else
		{
			log("[StageList_Ack] OK!");
			refreshUI();
		}
	});

	//schedule
	this->schedule(schedule_selector(LevelChooseLayer::scheduleHpTime),1.0f);

	//refresh
	refreshHp();
	refreshDiamond();
	refreshGold();
	refreshMail();
	refreshTask();

	//////////////////////////////////////////////
	m_scrollviewOffset = 0.f;

	m_scrollView->jumpToBottom();
	m_scrollView->setTouchEnabled(true);
	m_scrollView->addEventListenerScrollView(this, scrollvieweventselector(LevelChooseLayer::scrollViewCallBack));

	m_scrollViewBG = Sprite::create("Scene_BlueSea_bg_2_1136.png");
	m_scrollViewBG->setAnchorPoint(Point::ZERO);
	//m_scrollViewBG->setPosition(320,1980); //1980
	this->addChild(m_scrollViewBG,-1);
}

void LevelChooseLayer::onExit()
{
	Layer::onExit();
}

void LevelChooseLayer::loadUI()
{
	auto rootNode = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("LevelChooseLayer/LevelChooseLayer.json"));
	m_rootNode = rootNode;
	this->addChild(m_rootNode);

	//add layer bg and bubble
	Sprite* _bg = Sprite::create("commonUi/layerBg.jpg");
	_bg->setAnchorPoint(Point::ZERO);
	_bg->setPosition(Point::ZERO);
	_bg->setLocalZOrder(-1);
	m_rootNode->addChild(_bg);

	Sprite* _bubble = Sprite::create("commonUi/bubble.png");
	Size _winSize = this->getContentSize();
	_bubble->setPosition(ccp(_winSize.width/2,_winSize.height/2));
	_bubble->setLocalZOrder(-1);
	m_rootNode->addChild(_bubble);

	//
	m_scrollView = static_cast<ScrollView*>(ui::Helper::seekWidgetByName(rootNode, "ScrollView_84"));

	m_returnBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_returnBtn"));

	m_returnBtn-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_mailButton = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_mailButton"));

	m_mailButton-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_taskButton = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_taskButton"));

	m_taskButton-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_rankButton = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_rankButton"));

	m_rankButton-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_bbcButton = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_bbcButton"));

	m_bbcButton-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_starNum = static_cast<TextAtlas*>(ui::Helper::seekWidgetByName(rootNode, "m_starNum"));

	m_addHP = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_addHP"));

	m_addHP-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_hpState = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_hpState"));

	m_addDiamond = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_addDiamond"));

	m_addDiamond-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_addCoin = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_addCoin"));

	m_addCoin-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_coinNum = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_coinNum"));
	//
	m_diamondNum = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_diamondNum"));

	m_battle_shell_1 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_1"));

	m_battle_shell_1-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_shell_2 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_2"));

	m_battle_shell_2-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_shell_3 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_3"));

	m_battle_shell_3-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_shell_4 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_4"));

	m_battle_shell_4-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_shell_5 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_5"));

	m_battle_shell_5-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_shell_6 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_6"));

	m_battle_shell_6-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_shell_7 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_7"));

	m_battle_shell_7-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_shell_8 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_8"));

	m_battle_shell_8-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_shell_9 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_9"));

	m_battle_shell_9-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_shell_10 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_10"));

	m_battle_shell_10-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_shell_11 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_11"));

	m_battle_shell_11-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_shell_12 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_12"));

	m_battle_shell_12-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_shell_13 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_13"));

	m_battle_shell_13-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_shell_14 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_14"));

	m_battle_shell_14-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_shell_15 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_15"));

	m_battle_shell_15-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_shell_16 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_16"));

	m_battle_shell_16-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_shell_17 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_17"));

	m_battle_shell_17-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_shell_18 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_18"));

	m_battle_shell_18-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_shell_19 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_19"));

	m_battle_shell_19-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_shell_20 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_20"));

	m_battle_shell_20-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_shell_21 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_21"));

	m_battle_shell_21-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_shell_22 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_22"));

	m_battle_shell_22-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_shell_23 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_23"));

	m_battle_shell_23-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_shell_24 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_24"));

	m_battle_shell_24-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_shell_25 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_25"));

	m_battle_shell_25-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_shell_26 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_26"));

	m_battle_shell_26-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_shell_27 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_27"));

	m_battle_shell_27-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_shell_28 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_28"));

	m_battle_shell_28-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_shell_29 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_29"));

	m_battle_shell_29-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_shell_30 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_30"));

	m_battle_shell_30-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_shell_31 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_31"));

	m_battle_shell_31-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_shell_32 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_32"));

	m_battle_shell_32-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_shell_33 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_33"));

	m_battle_shell_33-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_shell_34 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_34"));

	m_battle_shell_34-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_shell_35 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_35"));

	m_battle_shell_35-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_shell_36 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_36"));

	m_battle_shell_36-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_shell_37 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_37"));

	m_battle_shell_37-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_shell_38 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_38"));

	m_battle_shell_38-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_shell_39 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_39"));

	m_battle_shell_39-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_shell_40 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_shell_40"));

	m_battle_shell_40-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));

	m_battle_star_off_1 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_1"));

	m_battle_star_off_2 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_2"));

	m_battle_star_off_3 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_3"));

	m_battle_star_off_4 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_4"));

	m_battle_star_off_5 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_5"));

	m_battle_star_off_6 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_6"));

	m_battle_star_off_7 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_7"));

	m_battle_star_off_8 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_8"));

	m_battle_star_off_9 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_9"));

	m_battle_star_off_10 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_10"));

	m_battle_star_off_11 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_11"));

	m_battle_star_off_12 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_12"));

	m_battle_star_off_13 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_13"));

	m_battle_star_off_14 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_14"));

	m_battle_star_off_15 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_15"));

	m_battle_star_off_16 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_16"));

	m_battle_star_off_17 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_17"));

	m_battle_star_off_18 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_18"));

	m_battle_star_off_19 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_19"));

	m_battle_star_off_20 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_20"));

	m_battle_star_off_21 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_21"));

	m_battle_star_off_22 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_22"));

	m_battle_star_off_23 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_23"));

	m_battle_star_off_24 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_24"));

	m_battle_star_off_25 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_25"));

	m_battle_star_off_26 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_26"));

	m_battle_star_off_27 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_27"));

	m_battle_star_off_28 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_28"));

	m_battle_star_off_29 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_29"));

	m_battle_star_off_30 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_30"));

	m_battle_star_off_31 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_31"));

	m_battle_star_off_32 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_32"));

	m_battle_star_off_33 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_33"));

	m_battle_star_off_34 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_34"));

	m_battle_star_off_35 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_35"));

	m_battle_star_off_36 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_36"));

	m_battle_star_off_37 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_37"));

	m_battle_star_off_38 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_38"));

	m_battle_star_off_39 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_39"));

	m_battle_star_off_40 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_40"));

	m_battle_star_off_41 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_41"));

	m_battle_star_off_42 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_42"));

	m_battle_star_off_43 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_43"));

	m_battle_star_off_44 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_44"));

	m_battle_star_off_45 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_45"));

	m_battle_star_off_46 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_46"));

	m_battle_star_off_47 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_47"));

	m_battle_star_off_48 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_48"));

	m_battle_star_off_49 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_49"));

	m_battle_star_off_50 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_50"));

	m_battle_star_off_51 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_51"));

	m_battle_star_off_52 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_52"));

	m_battle_star_off_53 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_53"));

	m_battle_star_off_54 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_54"));

	m_battle_star_off_55 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_55"));

	m_battle_star_off_56 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_56"));

	m_battle_star_off_57 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_57"));

	m_battle_star_off_58 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_58"));

	m_battle_star_off_59 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_59"));

	m_battle_star_off_60 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_60"));

	m_battle_star_off_61 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_61"));

	m_battle_star_off_62 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_62"));

	m_battle_star_off_63 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_63"));

	m_battle_star_off_64 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_64"));

	m_battle_star_off_65 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_65"));

	m_battle_star_off_66 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_66"));

	m_battle_star_off_67 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_67"));

	m_battle_star_off_68 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_68"));

	m_battle_star_off_69 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_69"));

	m_battle_star_off_70 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_70"));

	m_battle_star_off_71 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_71"));

	m_battle_star_off_72 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_72"));

	m_battle_star_off_73 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_73"));

	m_battle_star_off_74 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_74"));

	m_battle_star_off_75 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_75"));

	m_battle_star_off_76 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_76"));

	m_battle_star_off_77 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_77"));

	m_battle_star_off_78 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_78"));

	m_battle_star_off_79 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_79"));

	m_battle_star_off_80 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_80"));

	m_battle_star_off_81 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_81"));

	m_battle_star_off_82 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_82"));

	m_battle_star_off_83 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_83"));

	m_battle_star_off_84 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_84"));

	m_battle_star_off_85 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_85"));

	m_battle_star_off_86 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_86"));

	m_battle_star_off_87 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_87"));

	m_battle_star_off_88 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_88"));

	m_battle_star_off_89 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_89"));

	m_battle_star_off_90 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_90"));

	m_battle_star_off_91 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_91"));

	m_battle_star_off_92 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_92"));

	m_battle_star_off_93 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_93"));

	m_battle_star_off_94 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_94"));

	m_battle_star_off_95 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_95"));

	m_battle_star_off_96 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_96"));

	m_battle_star_off_97 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_97"));

	m_battle_star_off_98 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_98"));

	m_battle_star_off_99 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_99"));

	m_battle_star_off_100 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_100"));

	m_battle_star_off_101 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_101"));

	m_battle_star_off_102 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_102"));

	m_battle_star_off_103 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_103"));

	m_battle_star_off_104 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_104"));

	m_battle_star_off_105 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_105"));

	m_battle_star_off_106 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_106"));

	m_battle_star_off_107 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_107"));

	m_battle_star_off_108 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_108"));

	m_battle_star_off_109 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_109"));

	m_battle_star_off_110 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_110"));

	m_battle_star_off_111 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_111"));

	m_battle_star_off_112 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_112"));

	m_battle_star_off_113 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_113"));

	m_battle_star_off_114 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_114"));

	m_battle_star_off_115 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_115"));

	m_battle_star_off_116 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_116"));

	m_battle_star_off_117 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_117"));

	m_battle_star_off_118 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_118"));

	m_battle_star_off_119 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_119"));

	m_battle_star_off_120 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_off_120"));

	m_battle_point_off_1 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_1"));

	m_battle_point_off_2 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_2"));

	m_battle_point_off_3 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_3"));

	m_battle_point_off_4 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_4"));

	m_battle_point_off_5 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_5"));

	m_battle_point_off_6 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_6"));

	m_battle_point_off_7 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_7"));

	m_battle_point_off_8 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_8"));

	m_battle_point_off_9 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_9"));

	m_battle_point_off_10 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_10"));

	m_battle_point_off_11 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_11"));

	m_battle_point_off_12 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_12"));

	m_battle_point_off_13 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_13"));

	m_battle_point_off_14 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_14"));

	m_battle_point_off_15 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_15"));

	m_battle_point_off_16 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_16"));

	m_battle_point_off_17 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_17"));

	m_battle_point_off_18 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_18"));

	m_battle_point_off_19 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_19"));

	m_battle_point_off_20 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_20"));

	m_battle_point_off_21 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_21"));

	m_battle_point_off_22 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_22"));

	m_battle_point_off_23 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_23"));

	m_battle_point_off_24 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_24"));

	m_battle_point_off_25 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_25"));

	m_battle_point_off_26 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_26"));

	m_battle_point_off_27 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_27"));

	m_battle_point_off_28 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_28"));

	m_battle_point_off_29 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_29"));

	m_battle_point_off_30 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_30"));

	m_battle_point_off_31 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_31"));

	m_battle_point_off_32 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_32"));

	m_battle_point_off_33 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_33"));

	m_battle_point_off_34 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_34"));

	m_battle_point_off_35 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_35"));

	m_battle_point_off_36 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_36"));

	m_battle_point_off_37 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_37"));

	m_battle_point_off_38 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_38"));

	m_battle_point_off_39 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_39"));

	m_battle_point_off_40 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_40"));

	m_battle_point_off_41 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_41"));

	m_battle_point_off_42 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_42"));

	m_battle_point_off_43 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_43"));

	m_battle_point_off_44 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_44"));

	m_battle_point_off_45 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_45"));

	m_battle_point_off_46 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_46"));

	m_battle_point_off_47 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_47"));

	m_battle_point_off_48 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_48"));

	m_battle_point_off_49 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_49"));

	m_battle_point_off_50 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_50"));

	m_battle_point_off_51 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_51"));

	m_battle_point_off_52 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_52"));

	m_battle_point_off_53 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_53"));

	m_battle_point_off_54 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_54"));

	m_battle_point_off_55 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_55"));

	m_battle_point_off_56 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_56"));

	m_battle_point_off_57 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_57"));

	m_battle_point_off_58 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_58"));

	m_battle_point_off_59 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_59"));

	m_battle_point_off_60 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_60"));

	m_battle_point_off_61 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_61"));

	m_battle_point_off_62 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_62"));

	m_battle_point_off_63 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_63"));

	m_battle_point_off_64 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_64"));

	m_battle_point_off_65 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_65"));

	m_battle_point_off_66 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_66"));

	m_battle_point_off_67 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_67"));

	m_battle_point_off_68 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_68"));

	m_battle_point_off_69 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_69"));

	m_battle_point_off_70 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_70"));

	m_battle_point_off_71 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_71"));

	m_battle_point_off_72 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_72"));

	m_battle_point_off_73 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_73"));

	m_battle_point_off_74 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_74"));

	m_battle_point_off_75 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_75"));

	m_battle_point_off_76 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_76"));

	m_battle_point_off_77 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_77"));

	m_battle_point_off_78 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_78"));

	m_battle_point_off_79 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_79"));

	m_battle_point_off_80 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_80"));

	m_battle_point_off_81 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_81"));

	m_battle_point_off_82 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_82"));

	m_battle_point_off_83 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_83"));

	m_battle_point_off_84 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_84"));

	m_battle_point_off_85 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_85"));

	m_battle_point_off_86 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_86"));

	m_battle_point_off_87 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_87"));

	m_battle_point_off_88 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_88"));

	m_battle_point_off_89 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_89"));

	m_battle_point_off_90 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_90"));

	m_battle_point_off_91 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_91"));

	m_battle_point_off_92 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_92"));

	m_battle_point_off_93 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_93"));

	m_battle_point_off_94 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_94"));

	m_battle_point_off_95 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_95"));

	m_battle_point_off_96 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_96"));

	m_battle_point_off_97 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_97"));

	m_battle_point_off_98 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_98"));

	m_battle_point_off_99 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_99"));

	m_battle_point_off_100 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_100"));

	m_battle_point_off_101 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_101"));

	m_battle_point_off_102 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_102"));

	m_battle_point_off_103 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_103"));

	m_battle_point_off_104 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_104"));

	m_battle_point_off_105 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_105"));

	m_battle_point_off_106 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_106"));

	m_battle_point_off_107 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_107"));

	m_battle_point_off_108 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_108"));

	m_battle_point_off_109 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_109"));

	m_battle_point_off_110 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_110"));

	m_battle_point_off_111 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_111"));

	m_battle_point_off_112 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_112"));

	m_battle_point_off_113 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_113"));

	m_battle_point_off_114 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_114"));

	m_battle_point_off_115 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_115"));

	m_battle_point_off_116 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_116"));

	m_battle_point_off_117 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_off_117"));

	m_level_limit_6 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_level_limit_6"));

	m_level_limit_11 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_level_limit_11"));

	m_level_limit_16 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_level_limit_16"));

	m_level_limit_21 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_level_limit_21"));

	m_level_limit_26 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_level_limit_26"));

	m_level_limit_31 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_level_limit_31"));

	m_level_limit_36 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_level_limit_36"));

	m_hpState1 = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(rootNode, "m_hpState1"));
	m_hpLoadingBarV.pushBack(m_hpState1);

	m_hpState2 = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(rootNode, "m_hpState2"));
	m_hpLoadingBarV.pushBack(m_hpState2);

	m_hpState3 = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(rootNode, "m_hpState3"));
	m_hpLoadingBarV.pushBack(m_hpState3);

	m_hpState4 = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(rootNode, "m_hpState4"));
	m_hpLoadingBarV.pushBack(m_hpState4);

	m_hpState5 = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(rootNode, "m_hpState5"));
	m_hpLoadingBarV.pushBack(m_hpState5);

	m_mailNum = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_mailNum"));

	m_taskNum = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_taskNum"));

	m_battle_star_on_1 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_1"));

	m_battle_star_on_2 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_2"));

	m_battle_star_on_3 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_3"));

	m_battle_star_on_4 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_4"));

	m_battle_star_on_5 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_5"));

	m_battle_star_on_6 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_6"));

	m_battle_star_on_7 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_7"));

	m_battle_star_on_8 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_8"));

	m_battle_star_on_9 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_9"));

	m_battle_star_on_10 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_10"));

	m_battle_star_on_11 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_11"));

	m_battle_star_on_12 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_12"));

	m_battle_star_on_13 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_13"));

	m_battle_star_on_14 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_14"));

	m_battle_star_on_15 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_15"));

	m_battle_star_on_16 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_16"));

	m_battle_star_on_17 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_17"));

	m_battle_star_on_18 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_18"));

	m_battle_star_on_19 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_19"));

	m_battle_star_on_20 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_20"));

	m_battle_star_on_21 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_21"));

	m_battle_star_on_22 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_22"));

	m_battle_star_on_23 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_23"));

	m_battle_star_on_24 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_24"));

	m_battle_star_on_25 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_25"));

	m_battle_star_on_26 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_26"));

	m_battle_star_on_27 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_27"));

	m_battle_star_on_28 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_28"));

	m_battle_star_on_29 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_29"));

	m_battle_star_on_30 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_30"));

	m_battle_star_on_31 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_31"));

	m_battle_star_on_32 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_32"));

	m_battle_star_on_33 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_33"));

	m_battle_star_on_34 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_34"));

	m_battle_star_on_35 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_35"));

	m_battle_star_on_36 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_36"));

	m_battle_star_on_37 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_37"));

	m_battle_star_on_38 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_38"));

	m_battle_star_on_39 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_39"));

	m_battle_star_on_40 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_40"));

	m_battle_star_on_41 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_41"));

	m_battle_star_on_42 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_42"));

	m_battle_star_on_43 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_43"));

	m_battle_star_on_44 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_44"));

	m_battle_star_on_45 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_45"));

	m_battle_star_on_46 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_46"));

	m_battle_star_on_47 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_47"));

	m_battle_star_on_48 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_48"));

	m_battle_star_on_49 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_49"));

	m_battle_star_on_50 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_50"));

	m_battle_star_on_51 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_51"));

	m_battle_star_on_52 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_52"));

	m_battle_star_on_53 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_53"));

	m_battle_star_on_54 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_54"));

	m_battle_star_on_55 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_55"));

	m_battle_star_on_56 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_56"));

	m_battle_star_on_57 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_57"));

	m_battle_star_on_58 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_58"));

	m_battle_star_on_59 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_59"));

	m_battle_star_on_60 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_60"));

	m_battle_star_on_61 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_61"));

	m_battle_star_on_62 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_62"));

	m_battle_star_on_63 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_63"));

	m_battle_star_on_64 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_64"));

	m_battle_star_on_65 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_65"));

	m_battle_star_on_66 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_66"));

	m_battle_star_on_67 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_67"));

	m_battle_star_on_68 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_68"));

	m_battle_star_on_69 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_69"));

	m_battle_star_on_70 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_70"));

	m_battle_star_on_71 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_71"));

	m_battle_star_on_72 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_72"));

	m_battle_star_on_73 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_73"));

	m_battle_star_on_74 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_74"));

	m_battle_star_on_75 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_75"));

	m_battle_star_on_76 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_76"));

	m_battle_star_on_77 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_77"));

	m_battle_star_on_78 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_78"));

	m_battle_star_on_79 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_79"));

	m_battle_star_on_80 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_80"));

	m_battle_star_on_81 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_81"));

	m_battle_star_on_82 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_82"));

	m_battle_star_on_83 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_83"));

	m_battle_star_on_84 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_84"));

	m_battle_star_on_85 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_85"));

	m_battle_star_on_86 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_86"));

	m_battle_star_on_87 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_87"));

	m_battle_star_on_88 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_88"));

	m_battle_star_on_89 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_89"));

	m_battle_star_on_90 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_90"));

	m_battle_star_on_91 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_91"));

	m_battle_star_on_92 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_92"));

	m_battle_star_on_93 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_93"));

	m_battle_star_on_94 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_94"));

	m_battle_star_on_95 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_95"));

	m_battle_star_on_96 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_96"));

	m_battle_star_on_97 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_97"));

	m_battle_star_on_98 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_98"));

	m_battle_star_on_99 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_99"));

	m_battle_star_on_100 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_100"));

	m_battle_star_on_101 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_101"));

	m_battle_star_on_102 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_102"));

	m_battle_star_on_103 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_103"));

	m_battle_star_on_104 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_104"));

	m_battle_star_on_105 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_105"));

	m_battle_star_on_106 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_106"));

	m_battle_star_on_107 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_107"));

	m_battle_star_on_108 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_108"));

	m_battle_star_on_109 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_109"));

	m_battle_star_on_110 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_110"));

	m_battle_star_on_111 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_111"));

	m_battle_star_on_112 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_112"));

	m_battle_star_on_113 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_113"));

	m_battle_star_on_114 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_114"));

	m_battle_star_on_115 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_115"));

	m_battle_star_on_116 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_116"));

	m_battle_star_on_117 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_117"));

	m_battle_star_on_118 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_118"));

	m_battle_star_on_119 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_119"));

	m_battle_star_on_120 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_star_on_120"));

	m_battle_point_on_1 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_1"));

	m_battle_point_on_2 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_2"));

	m_battle_point_on_3 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_3"));

	m_battle_point_on_4 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_4"));

	m_battle_point_on_5 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_5"));

	m_battle_point_on_6 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_6"));

	m_battle_point_on_7 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_7"));

	m_battle_point_on_8 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_8"));

	m_battle_point_on_9 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_9"));

	m_battle_point_on_10 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_10"));

	m_battle_point_on_11 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_11"));

	m_battle_point_on_12 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_12"));

	m_battle_point_on_13 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_13"));

	m_battle_point_on_14 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_14"));

	m_battle_point_on_15 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_15"));

	m_battle_point_on_16 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_16"));

	m_battle_point_on_17 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_17"));

	m_battle_point_on_18 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_18"));

	m_battle_point_on_19 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_19"));

	m_battle_point_on_20 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_20"));

	m_battle_point_on_21 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_21"));

	m_battle_point_on_22 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_22"));

	m_battle_point_on_23 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_23"));

	m_battle_point_on_24 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_24"));

	m_battle_point_on_25 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_25"));

	m_battle_point_on_26 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_26"));

	m_battle_point_on_27 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_27"));

	m_battle_point_on_28 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_28"));

	m_battle_point_on_29 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_29"));

	m_battle_point_on_30 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_30"));

	m_battle_point_on_31 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_31"));

	m_battle_point_on_32 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_32"));

	m_battle_point_on_33 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_33"));

	m_battle_point_on_34 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_34"));

	m_battle_point_on_35 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_35"));

	m_battle_point_on_36 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_36"));

	m_battle_point_on_37 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_37"));

	m_battle_point_on_38 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_38"));

	m_battle_point_on_39 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_39"));

	m_battle_point_on_40 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_40"));

	m_battle_point_on_41 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_41"));

	m_battle_point_on_42 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_42"));

	m_battle_point_on_43 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_43"));

	m_battle_point_on_44 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_44"));

	m_battle_point_on_45 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_45"));

	m_battle_point_on_46 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_46"));

	m_battle_point_on_47 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_47"));

	m_battle_point_on_48 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_48"));

	m_battle_point_on_49 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_49"));

	m_battle_point_on_50 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_50"));

	m_battle_point_on_51 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_51"));

	m_battle_point_on_52 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_52"));

	m_battle_point_on_53 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_53"));

	m_battle_point_on_54 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_54"));

	m_battle_point_on_55 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_55"));

	m_battle_point_on_56 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_56"));

	m_battle_point_on_57 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_57"));

	m_battle_point_on_58 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_58"));

	m_battle_point_on_59 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_59"));

	m_battle_point_on_60 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_60"));

	m_battle_point_on_61 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_61"));

	m_battle_point_on_62 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_62"));

	m_battle_point_on_63 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_63"));

	m_battle_point_on_64 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_64"));

	m_battle_point_on_65 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_65"));

	m_battle_point_on_66 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_66"));

	m_battle_point_on_67 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_67"));

	m_battle_point_on_68 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_68"));

	m_battle_point_on_69 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_69"));

	m_battle_point_on_70 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_70"));

	m_battle_point_on_71 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_71"));

	m_battle_point_on_72 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_72"));

	m_battle_point_on_73 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_73"));

	m_battle_point_on_74 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_74"));

	m_battle_point_on_75 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_75"));

	m_battle_point_on_76 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_76"));

	m_battle_point_on_77 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_77"));

	m_battle_point_on_78 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_78"));

	m_battle_point_on_79 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_79"));

	m_battle_point_on_80 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_80"));

	m_battle_point_on_81 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_81"));

	m_battle_point_on_82 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_82"));

	m_battle_point_on_83 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_83"));

	m_battle_point_on_84 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_84"));

	m_battle_point_on_85 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_85"));

	m_battle_point_on_86 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_86"));

	m_battle_point_on_87 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_87"));

	m_battle_point_on_88 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_88"));

	m_battle_point_on_89 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_89"));

	m_battle_point_on_90 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_90"));

	m_battle_point_on_91 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_91"));

	m_battle_point_on_92 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_92"));

	m_battle_point_on_93 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_93"));

	m_battle_point_on_94 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_94"));

	m_battle_point_on_95 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_95"));

	m_battle_point_on_96 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_96"));

	m_battle_point_on_97 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_97"));

	m_battle_point_on_98 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_98"));

	m_battle_point_on_99 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_99"));

	m_battle_point_on_100 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_100"));

	m_battle_point_on_101 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_101"));

	m_battle_point_on_102 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_102"));

	m_battle_point_on_103 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_103"));

	m_battle_point_on_104 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_104"));

	m_battle_point_on_105 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_105"));

	m_battle_point_on_106 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_106"));

	m_battle_point_on_107 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_107"));

	m_battle_point_on_108 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_108"));

	m_battle_point_on_109 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_109"));

	m_battle_point_on_110 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_110"));

	m_battle_point_on_111 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_111"));

	m_battle_point_on_112 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_112"));

	m_battle_point_on_113 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_113"));

	m_battle_point_on_114 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_114"));

	m_battle_point_on_115 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_115"));

	m_battle_point_on_116 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_116"));

	m_battle_point_on_117 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_battle_point_on_117"));

	m_mailNumText = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_mailNum"));

	m_taskNumText = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_taskNum"));

	m_limit_button_shell_6 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_limit_button_shell_6"));
	m_limit_button_shell_6->setColor(Color3B::GRAY);
	m_limit_button_shell_6-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));
	m_limit_button_shell_11 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_limit_button_shell_11"));
	m_limit_button_shell_11->setColor(Color3B::GRAY);
	m_limit_button_shell_11-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));
	m_limit_button_shell_16 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_limit_button_shell_16"));
	m_limit_button_shell_16->setColor(Color3B::GRAY);
	m_limit_button_shell_16-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));
	m_limit_button_shell_21 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_limit_button_shell_21"));
	m_limit_button_shell_21->setColor(Color3B::GRAY);
	m_limit_button_shell_21-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));
	m_limit_button_shell_26 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_limit_button_shell_26"));
	m_limit_button_shell_26->setColor(Color3B::GRAY);
	m_limit_button_shell_26-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));
	m_limit_button_shell_31 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_limit_button_shell_31"));
	m_limit_button_shell_31->setColor(Color3B::GRAY);
	m_limit_button_shell_31-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));
	m_limit_button_shell_36 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_limit_button_shell_36"));
	m_limit_button_shell_36->setColor(Color3B::GRAY);
	m_limit_button_shell_36-> addTouchEventListener(this, toucheventselector(LevelChooseLayer::handleTouchEvent));



	//************* event listener ************************
	//event listener shop layer return call-back
	auto _listenerShop = EventListenerCustom::create("Return_shopLayer", [this](EventCustom* event) {
		log("AAAAAAAAAAAAAAA level choose layer");
		refreshHp();
		refreshGold();
		refreshDiamond();
	});

	_eventDispatcher->addEventListenerWithSceneGraphPriority(_listenerShop, this);

	//event listener mail layer return call-back
	auto _listenerMail = EventListenerCustom::create("Return_mailLayer", [this](EventCustom* event) {
		log("BBBBBBBBBBBBBBB level choose layer");
		refreshMail();
		refreshHp();
		refreshGold();
		refreshDiamond();
	});

	_eventDispatcher->addEventListenerWithSceneGraphPriority(_listenerMail, this);

	auto _listenerTask = EventListenerCustom::create("Return_taskLayer", [this](EventCustom* event) {
		log("CCCCCCCCCCCCCC level choose layer");
		refreshTask();
		refreshGold();
		refreshDiamond();
	});

	_eventDispatcher->addEventListenerWithSceneGraphPriority(_listenerTask, this);
}

void LevelChooseLayer::handleTouchEvent(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);

	if (pSender == nullptr)
	{
	}

	else if (pSender == m_limit_button_shell_6 && (TOUCH_EVENT_ENDED == type))
	{
		openLimitLevel(6);
	}

	else if (pSender == m_limit_button_shell_11 && (TOUCH_EVENT_ENDED == type))
	{
		openLimitLevel(11);
	}

	else if (pSender == m_limit_button_shell_16 && (TOUCH_EVENT_ENDED == type))
	{
		openLimitLevel(16);
	}

	else if (pSender == m_limit_button_shell_21 && (TOUCH_EVENT_ENDED == type))
	{
		openLimitLevel(21);
	}

	else if (pSender == m_limit_button_shell_26 && (TOUCH_EVENT_ENDED == type))
	{
		openLimitLevel(26);
	}
	else if (pSender == m_limit_button_shell_31 && (TOUCH_EVENT_ENDED == type))
	{
		openLimitLevel(31);
	}

	else if (pSender == m_limit_button_shell_36 && (TOUCH_EVENT_ENDED == type))
	{
		openLimitLevel(36);
	}

	else if (pSender == m_returnBtn && (TOUCH_EVENT_ENDED == type))
	{
		auto home = HomeScene::create();

		Director::getInstance()->replaceScene(home);
	}

	else if (pSender == m_mailButton && (TOUCH_EVENT_ENDED == type))
	{
		auto mailLayer = MailLayer::create();

		int _zOrder = this->getLocalZOrder();

		mailLayer->setLocalZOrder(_zOrder+1);

		this->getParent()->addChild(mailLayer);
	}

	else if (pSender == m_taskButton && (TOUCH_EVENT_ENDED == type))
	{
		auto taskLayer = TaskLayer::create();

		int _zOrder = this->getLocalZOrder();

		taskLayer->setLocalZOrder(_zOrder+1);

		this->getParent()->addChild(taskLayer);
	}

	else if (pSender == m_rankButton && (TOUCH_EVENT_ENDED == type))
	{
		RankLayer* t_rankLayer = RankLayer::create();

		int t_zOrder = this->getLocalZOrder();

		t_rankLayer->setLocalZOrder(t_zOrder+1);

		this->getParent()->addChild(t_rankLayer);
	}

	else if (pSender == m_bbcButton && (TOUCH_EVENT_ENDED == type))
	{
		auto noticeLayer = NoticeLayer::create();

		int t_zOrder = this->getLocalZOrder();

		noticeLayer->setLocalZOrder(t_zOrder+1);

		this->getParent()->addChild(noticeLayer);
	}

	else if (pSender == m_addHP && (TOUCH_EVENT_ENDED == type))
	{
		GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);

		ShopLayer* t_ShopLayer = ShopLayer::create();

		int t_zOrder = this->getLocalZOrder();

		t_ShopLayer->setLocalZOrder(t_zOrder+1);

		this->getParent()->addChild(t_ShopLayer);

		t_ShopLayer->initShopType(TYPE_shopHp);
	}

	else if (pSender == m_addDiamond && (TOUCH_EVENT_ENDED == type))
	{
		GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);

		ShopLayer* t_ShopLayer = ShopLayer::create();

		int t_zOrder = this->getLocalZOrder();

		t_ShopLayer->setLocalZOrder(t_zOrder+1);

		this->getParent()->addChild(t_ShopLayer);

		t_ShopLayer->initShopType(TYPE_shopDiamond);
	}

	else if (pSender == m_addCoin && (TOUCH_EVENT_ENDED == type))
	{
		GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);

		ShopLayer* t_ShopLayer = ShopLayer::create();

		int t_zOrder = this->getLocalZOrder();

		t_ShopLayer->setLocalZOrder(t_zOrder+1);

		this->getParent()->addChild(t_ShopLayer);

		t_ShopLayer->initShopType(TYPE_shopGold);
	}

	else if (pSender == m_battle_shell_1 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(1);
	}

	else if (pSender == m_battle_shell_2 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(2);
	}

	else if (pSender == m_battle_shell_3 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(3);
	}

	else if (pSender == m_battle_shell_4 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(4);
	}

	else if (pSender == m_battle_shell_5 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(5);
	}

	else if (pSender == m_battle_shell_6 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(6);
	}

	else if (pSender == m_battle_shell_7 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(7);
	}

	else if (pSender == m_battle_shell_8 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(8);
	}

	else if (pSender == m_battle_shell_9 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(9);
	}

	else if (pSender == m_battle_shell_10 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(10);
	}

	else if (pSender == m_battle_shell_11 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(11);
	}

	else if (pSender == m_battle_shell_12 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(12);
	}

	else if (pSender == m_battle_shell_13 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(13);
	}

	else if (pSender == m_battle_shell_14 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(14);
	}

	else if (pSender == m_battle_shell_15 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(15);
	}

	else if (pSender == m_battle_shell_16 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(16);
	}

	else if (pSender == m_battle_shell_17 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(17);
	}

	else if (pSender == m_battle_shell_18 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(18);
	}

	else if (pSender == m_battle_shell_19 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(19);
	}

	else if (pSender == m_battle_shell_20 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(20);
	}

	else if (pSender == m_battle_shell_21 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(21);
	}

	else if (pSender == m_battle_shell_22 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(22);
	}

	else if (pSender == m_battle_shell_23 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(23);
	}

	else if (pSender == m_battle_shell_24 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(24);
	}

	else if (pSender == m_battle_shell_25 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(25);
	}

	else if (pSender == m_battle_shell_26 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(26);
	}

	else if (pSender == m_battle_shell_27 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(27);
	}

	else if (pSender == m_battle_shell_28 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(28);
	}

	else if (pSender == m_battle_shell_29 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(29);
	}

	else if (pSender == m_battle_shell_30 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(30);
	}

	else if (pSender == m_battle_shell_31 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(31);
	}

	else if (pSender == m_battle_shell_32 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(32);
	}

	else if (pSender == m_battle_shell_33 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(33);
	}

	else if (pSender == m_battle_shell_34 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(34);
	}

	else if (pSender == m_battle_shell_35 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(35);
	}

	else if (pSender == m_battle_shell_36 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(36);
	}

	else if (pSender == m_battle_shell_37 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(37);
	}

	else if (pSender == m_battle_shell_38 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(38);
	}

	else if (pSender == m_battle_shell_39 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(39);
	}

	else if (pSender == m_battle_shell_40 && (TOUCH_EVENT_ENDED == type))
	{
		startGame(40);
	}


}

void LevelChooseLayer::setStarNum(int star)
{
	m_starNum->setStringValue(StringUtils::format("%d",star));
}

void LevelChooseLayer::setHP(int hp)
{
	if (hp == 0)
	{
		m_hpState1->setPercent(0);
	}
	else if (hp == 1)
	{

	}
	else if (hp == 2)
	{

	}
	else if (hp == 3)
	{

	}
	else if (hp == 4)
	{

	}
	else if (hp == 5)
	{

	}
	else
	{
		log("HP Setting Error From LevelChooseLayer.");
	}
}

void LevelChooseLayer::scrollViewCallBack(Ref *pSender, TouchEventType type)
{
	//static float s_scrollview_offset;
	Point p = m_scrollView->getInnerContainer()->getPosition();

	float d = p.y - m_scrollviewOffset;

	m_scrollviewOffset = p.y;


	//log("OFFSET: %f",s_scrollview_offset);
	//log("SCROLLVIEW :X:%f Y:%f" ,p.x,p.y);

	p = m_scrollViewBG->getPosition();
	//m_scrollViewBG->setPosition(Point(p.x,p.y + d*0.93));

	//log("BG: X:%f Y:%f" ,p.x,p.y);
}

void LevelChooseLayer::setDiamond(int)
{

}

void LevelChooseLayer::setCoin(int)
{

}

void LevelChooseLayer::setMailNum(int)
{

}

void LevelChooseLayer::setTaskNum(int)
{

}

void LevelChooseLayer::setLevelStarOn(int level, int stars, bool onOff)
{
	//m_battle_star_on_94
	if (stars == 0)
	{
		return;
	}

	std::string starStr = "m_battle_star_on_" + StringUtils::format("%d",(level - 1) * 3 + stars);
	auto star = static_cast<ImageView*>(ui::Helper::seekWidgetByName(m_rootNode, starStr.c_str()));
	if (star)
	{
		star->setVisible(onOff);
	}
}

void LevelChooseLayer::setLevelStarOff(int level, bool onOff)
{
	//m_battle_star_on_94
	for (int i = 1; i < 4; i++)
	{
		std::string starStr = "m_battle_star_off_" + StringUtils::format("%d",(level - 1) * 3 + i);
		auto star = static_cast<ImageView*>(ui::Helper::seekWidgetByName(m_rootNode, starStr.c_str()));
		if (star)
		{
			star->setVisible(onOff);
		}
	}
}

void LevelChooseLayer::setLevelLimit(int level, bool onOff)
{
	std::string limitStr = "m_level_limit_" + StringUtils::format("%d",level);
	auto limit = static_cast<ImageView*>(ui::Helper::seekWidgetByName(m_rootNode, limitStr.c_str()));

	auto limitStrBtn = "m_limit_button_shell_" + StringUtils::format("%d",level);
	auto limitBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(m_rootNode, limitStrBtn.c_str()));

	if (limit)
	{
		limit->setVisible(onOff);
	}

	if (limitBtn)
	{
		limitBtn->setVisible(false);
	}
}

void LevelChooseLayer::startLevelRoute(int level, bool onOff)
{
	if (level < 2)
	{

	}
	else
	{
		for (int i = 1; i < 4; i++)
		{
			std::string pointStr = "m_battle_point_on_" + StringUtils::format("%d",(level - 2) * 3 + i);
			auto point = static_cast<ImageView*>(ui::Helper::seekWidgetByName(m_rootNode, pointStr.c_str()));
			point->setVisible(onOff);
		}
	}
}

void LevelChooseLayer::setLevelShell(int level, bool onOff)
{
	std::string shellStr = "m_battle_shell_" + StringUtils::format("%d",level);
	auto shell = static_cast<MyButton*>(ui::Helper::seekWidgetByName(m_rootNode, shellStr.c_str()));

	if (onOff)
	{
		shell->setOnPressEnable(true);
		setLevelLimit(level, false);
		if (level == (Player::getInstance()->getLevelNewStage()))
		{
			shell->setColor(Color3B::WHITE);
		}else
		{
			shell->setColor(Color3B::WHITE);
		}
	}
	else
	{
		shell->setOnPressEnable(false);
		shell->setColor(Color3B::GRAY);
	}

}

void LevelChooseLayer::refreshUI()
{
	auto stageList = Player::getInstance()->getStageList();

	m_totalStar = Player::getInstance()->getLevelStar();
	int newStage = Player::getInstance()->getLevelNewStage();

	setStarNum(m_totalStar);
	if (newStage > 1)
	{
		for (int i = 0; i < (stageList->size() - 1); i++)
		{
            
            if (i > 40) break;
            
			auto stageItem = stageList->at(i);

			for (int i = 1; i < (stageItem->star + 1); i++)
			{
				setLevelStarOn(stageItem->stageId, i, true);
			}

			setLevelStarOn(stageItem->stageId, stageItem->star, true);
			setLevelStarOff(stageItem->stageId, true);
			setLevelShell(stageItem->stageId, true);
			startLevelRoute(stageItem->stageId, true);
			
		}

		startNewStage(newStage);
	}
	else
	{
		for (auto stageItem : (*stageList))
		{
            if (stageItem->stageId > 40) {
                break;
            }
			for (int i = 1; i < (stageItem->star + 1); i++)
			{
				setLevelStarOn(stageItem->stageId, i, true);
			}

			setLevelStarOn(stageItem->stageId, stageItem->star, true);
			setLevelStarOff(stageItem->stageId, true);
			setLevelShell(stageItem->stageId, true);
			startLevelRoute(stageItem->stageId, true);

		}
	}

}

void LevelChooseLayer::startGame(int level)
{
	log("Start Game :%d",level);

	if(Player::getInstance()->getStar()>0)
	{
		Network::getInstance()->do_StageGame_Req([=](const AckInfo &ackInfo) 
		{
			std::string msgInfo = ackInfo.getMessageInfo();

			if (msgInfo != "")
			{
				log("[StageGame] %s", msgInfo.c_str());
			}
			else
			{
				//ok
				log("[StageGame] : success~~!!");

				GameSound::playEffectMusic(EFF_USE_COUNT);

				Player::getInstance()->setStar(Player::getInstance()->getStar()-1);

				BattleData::getInstance()->setBattleLevel(level);

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
}

void LevelChooseLayer::startNewStage(int level)
{
	if (level < 2)
	{

	}
	else
	{
		auto act1 = FadeIn::create(0.5);
		auto act2 = Sequence::create(DelayTime::create(1.0f),FadeIn::create(0.5),nullptr);
		auto act3 = Sequence::create(DelayTime::create(1.5f),FadeIn::create(0.5),CallFunc::create(this,callfunc_selector(LevelChooseLayer::startNewShell)),nullptr);

		std::string pointStr = "m_battle_point_on_" + StringUtils::format("%d",(level - 2) * 3 + 1);
		auto point = static_cast<ImageView*>(ui::Helper::seekWidgetByName(m_rootNode, pointStr.c_str()));
		point->setVisible(true);
		point->setOpacity(0.0f);
		point->runAction(act1);

		pointStr = "m_battle_point_on_" + StringUtils::format("%d",(level - 2) * 3 + 2);
		point = static_cast<ImageView*>(ui::Helper::seekWidgetByName(m_rootNode, pointStr.c_str()));
		point->setVisible(true);
		point->setOpacity(0.0f);
		point->runAction(act2);

		pointStr = "m_battle_point_on_" + StringUtils::format("%d",(level - 2) * 3 + 3);
		point = static_cast<ImageView*>(ui::Helper::seekWidgetByName(m_rootNode, pointStr.c_str()));
		point->setVisible(true);
		point->setOpacity(0.0f);
		point->runAction(act3);

		setLevelLimit(level,false);
	}


}

void LevelChooseLayer::startNewShell()
{
	int newStage = Player::getInstance()->getLevelNewStage();
	if (m_newStage > 0)
	{
		newStage = m_newStage;
		setLevelShell(newStage,true);
		setLevelStarOn(newStage,0,true);
		setLevelStarOff(newStage,true);
	}
	else
	{
		newStage = Player::getInstance()->getLevelNewStage();
		setLevelShell(newStage,true);
		auto stageList = Player::getInstance()->getStageList();
		setLevelStarOn(newStage,stageList->at(newStage-1)->star,true);
		setLevelStarOff(newStage,true);
	}



}

//**********************************schedule******************************
void LevelChooseLayer::scheduleHpTime(float delayTime)
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

		m_hpState->setText(t_timeText->getCString());

		if(m_HpNum<5)
			m_hpLoadingBarV.at(m_HpNum)->setPercent(100.0f*(1.0f-_remainTime/Player::getInstance()->getMaxTime()));
	}
	if(Player::getInstance()->getIsStarUpdate())
		refreshHp();
}

//***************************************refresh***************************
void LevelChooseLayer::refreshHp()
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

		m_hpState->setText(t_timeText->getCString());
	}
	else if(5==m_HpNum)
	{
		for(int num = 0 ; num < 5 ; ++num)
		{
			m_hpLoadingBarV.at(num)->setPercent(100);
		}

		m_hpState->setText("MAX");
	}
	else if(m_HpNum>5)
	{
		for(int num = 0 ; num < 5 ; ++num)
		{
			m_hpLoadingBarV.at(num)->setPercent(100);
		}

		std::string t_hpNumStr = StringUtils::format("+%d",m_HpNum-5);
		m_hpState->setText(t_hpNumStr.c_str());
	}

	//switch
	Player::getInstance()->setIsStarUpdate(false);

}
void LevelChooseLayer::refreshDiamond()
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
void LevelChooseLayer::refreshGold()
{
	int t_gold = Player::getInstance()->getCoin();
	if(t_gold<0)
	{
		log("~~error~~ gold num is error");
		t_gold = 0;
	}
	std::string coinStr = StringUtils::format("%d", t_gold);//CW
	m_coinNum->setText(coinStr.c_str());
}

void LevelChooseLayer::refreshMail()
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
void LevelChooseLayer::refreshTask()
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

void LevelChooseLayer::openLimitLevel(int level)
{
	int limitStarNUm = 0;
	if (level == (Player::getInstance()->getCurrentLevel() + 1))
	{
		if (level == 6)
		{
			limitStarNUm = 10;
		}
		else if (level == 11)
		{
			limitStarNUm = 25;
		}
		else if (level == 16)
		{
			limitStarNUm = 40;
		}
		else if (level == 21)
		{
			limitStarNUm = 55;
		}
		else if (level == 26)
		{
			limitStarNUm = 70;
		}
		else if (level == 31)
		{
			limitStarNUm = 85;
		}
		else if (level == 36)
		{
			limitStarNUm = 100;
		}

		if (limitStarNUm > m_totalStar)
		{
			return;
		}
	}
	else
	{
		return;
	}

	m_newStage = level;
	std::string json = cocos2d::FileUtils::getInstance()->getStringFromFile("MESSAGE_INFO.json");

	rapidjson::Document d;

	if ("" != json)
	{
		d.Parse<0>(json.c_str());
	}

	std::string keyStr = StringUtils::format("%d",0);

	std::string text1 = (d)[keyStr.c_str()]["text1"].GetString();
	std::string text2 = (d)[keyStr.c_str()]["text2"].GetString();
	std::string text3 = (d)[keyStr.c_str()]["text3"].GetString();

	std::string limitStr = text1 + StringUtils::format("%d",limitStarNUm) + text2;



	NotifyBoxUILayer::getInstance()->show(
		1,
		limitStr,
		text3,
		[=](){
			Network::getInstance()->do_ChooseStage_Req(level,[=](const AckInfo &ackInfo)
			{
				std::string msgInfo = ackInfo.getMessageInfo();

				if (msgInfo != "")
				{
					log("ChooseStage_Ack[TaskList_Ack] %s", msgInfo.c_str());
				}
				else
				{
					log("ChooseStage_Ack[TaskList_Ack] OK!");
					startLimitLevel(level);
					startNewStage(level);

				}
			});
			NotifyBoxUILayer::getInstance()->stop();
	},
		[=](){

			NotifyBoxUILayer::getInstance()->stop();

	}
	);


}

void LevelChooseLayer::startLimitLevel(int level)
{
	log("New Level:%d",level);
}