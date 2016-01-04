////LevelGameOver.cpp created by php script. chris.li
#include "LevelGameOver.h"
#include "BattleData.h"
#include "Network.h"
#include "Player.h"
#include "SimpleAudioEngine.h"
#include "home/HomeScene.h"
#include "home/LevelChooseLayer.h"

//#include "C2DXShareSDK/C2DXShareSDK.h"
//#include "common/ShareToIntenet.h"
#include "util/GameSound.h"
USING_NS_CC;
//using namespace cn::sharesdk;

bool LevelGameOver::init()
{
	if (Layer::init())
	{
		loadUI();
		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
		return true;
	}

	return false;
}
void LevelGameOver::onEnter()
{
	Layer::onEnter();

	refreshUI();

	int playTime = BattleData::getInstance()->getPlayTime();
	int star = 0;

	if (playTime <= 90)
	{
		//3 stars
		star = 3;
	}
	else if (playTime > 90 && playTime <= 100)
	{
		star = 2;
	}
	else if (playTime > 100 && playTime <= 120)
	{
		star = 1;
	}
	else if (playTime > 120)
	{
		star = 1;
	}

	if (BattleData::getInstance()->getGameWin() == 1)
	{
		Network::getInstance()->do_StageResult_Req(star, [this](const AckInfo &ackInfo)
		{
			std::string msgInfo = ackInfo.getMessageInfo();
        
			if (msgInfo != "")
			{
				log("[Result_Ack] %s", msgInfo.c_str());
			}
			else
			{
				log("[Result_Ack] OK!");
            
				refreshUI();
			}
		});
	}
	else
	{
		refreshUI();
	}

}

void LevelGameOver::onExit()
{
	Layer::onExit();
}

void LevelGameOver::loadUI()
{
	auto rootNode = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("LevelGameOver/LevelGameOver.json"));

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

	m_continueBtn-> addTouchEventListener(this, toucheventselector(LevelGameOver::handleTouchEvent));

	m_shareBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_shareBtn"));

	m_shareBtn-> addTouchEventListener(this, toucheventselector(LevelGameOver::handleTouchEvent));

	m_winPicText = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_winPicText"));

	m_lossPicText = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_lossPicText"));

	m_lossPic = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "m_lossPic"));

	m_lossPic-> addTouchEventListener(this, toucheventselector(LevelGameOver::handleTouchEvent));

	m_winPic = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_winPic"));

	m_winStar1 = static_cast<CheckBox*>(ui::Helper::seekWidgetByName(rootNode, "m_winStar1"));

	m_winStar1-> addTouchEventListener(this, toucheventselector(LevelGameOver::handleTouchEvent));

	m_winStar2 = static_cast<CheckBox*>(ui::Helper::seekWidgetByName(rootNode, "m_winStar2"));

	m_winStar2-> addTouchEventListener(this, toucheventselector(LevelGameOver::handleTouchEvent));

	m_winStar3 = static_cast<CheckBox*>(ui::Helper::seekWidgetByName(rootNode, "m_winStar3"));

	m_winStar3-> addTouchEventListener(this, toucheventselector(LevelGameOver::handleTouchEvent));

	m_scoreLabel = static_cast<TextAtlas*>(ui::Helper::seekWidgetByName(rootNode, "m_scoreLabel"));

	m_scoreLabelText = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_scoreLabelText"));

	m_coinLabelText = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_coinLabelText"));

	m_coinLabel = static_cast<TextAtlas*>(ui::Helper::seekWidgetByName(rootNode, "m_coinLabel"));

	//gift layer
	m_giftPal = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode,"m_rewardPal"));
	m_giftPal->setVisible(false);

	m_giftBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(m_giftPal,"m_rewardBtn"));
	m_giftBtn->addTouchEventListener(this, toucheventselector(LevelGameOver::touchEventGiftBtn));

}


void LevelGameOver::shareToIntenet()
{
    //ShareToIntenet::getInstance()->showShareBox();
}

void LevelGameOver::handleTouchEvent(Ref *pSender, TouchEventType type)
{
	if (pSender == nullptr)
	{
	}

	else if (pSender == m_continueBtn && (TOUCH_EVENT_ENDED == type))
	{
		GameSound::playEffectMusic(EFF_CHANGE_PAGE);

		Player::getInstance()->setLevelChooseSwitch(true);

		auto home = HomeScene::create();

		//BattleData::getInstance()->setBattleMode(1);

		//auto layer = LevelChooseLayer::create();

		////this->getScene()->addChild(layer);
		////Director::getInstance()->getRunningScene()->addChild(layer);
		//home->addChild(layer);
        
        Director::getInstance()->replaceScene(home);
	
	}

	else if (pSender == m_shareBtn && (TOUCH_EVENT_ENDED == type))
	{
		GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);
        shareToIntenet();
	}

	else if (pSender == m_lossPic && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == m_winStar1 && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == m_winStar2 && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == m_winStar3 && (TOUCH_EVENT_ENDED == type))
	{
	}


}

//gift get btn event
void LevelGameOver::touchEventGiftBtn(Ref *pSender, TouchEventType type)
{
	if(TOUCH_EVENT_ENDED==type)
	{
		m_giftPal->setVisible(false);
	}
}

void LevelGameOver::refreshUI()
{
	GameSound::playEffectMusic(EFF_GOAL_UP);

	int playTime = BattleData::getInstance()->getPlayTime();

	if (playTime <= 90)
	{
		//3 stars
		m_winStar1->setSelectedState(true);
		m_winStar2->setSelectedState(true);
		m_winStar3->setSelectedState(true);
	}
	else if (playTime > 90 && playTime <= 100)
	{
		m_winStar1->setSelectedState(true);
		m_winStar2->setSelectedState(true);
		m_winStar3->setSelectedState(false);
	}
	else if (playTime > 100 && playTime <= 120)
	{
		m_winStar1->setSelectedState(true);
		m_winStar2->setSelectedState(false);
		m_winStar3->setSelectedState(false);
	}
	else if (playTime > 120)
	{
		m_winStar1->setSelectedState(true);
		m_winStar2->setSelectedState(false);
		m_winStar3->setSelectedState(false);
	}

	if (BattleData::getInstance()->getGameWin() == 2)
	{
		m_winStar1->setSelectedState(false);
		m_winStar2->setSelectedState(false);
		m_winStar3->setSelectedState(false);
	}

	int ms = BattleData::getInstance()->getMonsterScore();

	if(BattleData::getInstance()->getGameWin() == 1)
		m_scoreLabel->setStringValue(StringUtils::format("%d",BattleData::getInstance()->getScore() + ms));
	else
		m_scoreLabel->setStringValue("0");

	if(BattleData::getInstance()->getGameWin() == 1)
		m_coinLabel->setStringValue(StringUtils::format("%d",BattleData::getInstance()->getGold()));
	else
		m_coinLabel->setStringValue("0");

	std::string json = cocos2d::FileUtils::getInstance()->getStringFromFile("MESSAGE_INFO.json");
    
	rapidjson::Document d;
    
	if ("" != json)
	{
		 d.Parse<0>(json.c_str());
	}

	std::string keyStr = StringUtils::format("%d", 31);

	std::string scoreLabel = (d)[keyStr.c_str()]["text1"].GetString();
	std::string coinLabel = (d)[keyStr.c_str()]["text2"].GetString();
     
	float scorePercent = BattleData::getInstance()->getScoreAddPercent();
	scorePercent = scorePercent * 100;

	std::string scorePercentStr = StringUtils::format("%d",(int)scorePercent);
	scoreLabel = scoreLabel + scorePercentStr + "%";
	m_scoreLabelText->setText(scoreLabel.c_str());

	float coinPercent = BattleData::getInstance()->getGoldAddPercent();
	coinPercent = coinPercent * 100;
	std::string coinPercentStr = StringUtils::format("%d",(int)coinPercent);
	coinLabel = coinLabel + coinPercentStr + "%";
	m_coinLabelText->setText(coinLabel.c_str());

	if (BattleData::getInstance()->getGameWin() == 1)
	{
		m_winPic->setVisible(true);
		m_lossPic->setVisible(false);

		m_winPicText->setVisible(true);
		m_lossPicText->setVisible(false);
	}
	else if (BattleData::getInstance()->getGameWin() == 2)
	{
		m_winPic->setVisible(false);
		m_lossPic->setVisible(true);

		m_winPicText->setVisible(false);
		m_lossPicText->setVisible(true);
	}

	if(Player::getInstance()->getIsFirstPerfect())
	{
		m_giftPal->setVisible(true);
		Player::getInstance()->setIsFirstPerfect(false);
	}
}