////GameOverLayer.cpp created by php script. chris.li
#include "GameOverLayer.h"
#include "home/HomeScene.h"
#include "home/MainLayer.h"
#include "network/Network.h"
#include "model/BattleData.h"
#include "model/Player.h"
#include "home/RankLayer.h"
#include "RankingUpLayer.h"
#include "home/CrownCurrentLayer.h"
#include "util/GameSound.h"
#include "SimpleAudioEngine.h"
//#include "common/ShareToIntenet.h"

USING_NS_CC;


bool GameOverLayer::init()
{
	if (Layer::init())
	{
		loadUI();
		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
//		continueLayer = MainLayer::create();
		return true;
	}

	return false;
}
void GameOverLayer::onEnter()
{
	Layer::onEnter();

    refreshUI();
    
    Network::getInstance()->do_Result_Req(BattleData::getInstance()->getScore(), BattleData::getInstance()->getGold(), BattleData::getInstance()->getReviveTimes(),[this](const AckInfo &ackInfo)
    {
        std::string msgInfo = ackInfo.getMessageInfo();
        
        if (msgInfo != "")
        {
            log("[Result_Ack] %s", msgInfo.c_str());
        }
        else
        {
            log("[Result_Ack] OK!");
            Player::getInstance()->setScore(BattleData::getInstance()->getScore());
			log("~~~BattleData~~Score~~~~%d", BattleData::getInstance()->getScore());
            log("~~~Player~~Score2222~~~~%d", Player::getInstance()->getScore());

            refreshUI();
        }
    });
}

void GameOverLayer::onExit()
{
	Layer::onExit();
}

void GameOverLayer::loadUI()
{
	auto rootNode = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("GameOverLayer/GameOverLayer.json"));

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

	m_goonBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "Button_45"));

	m_goonBtn-> addTouchEventListener(this, toucheventselector(GameOverLayer::handleTouchEvent));

	m_shareBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "Button_45_0"));

	m_shareBtn-> addTouchEventListener(this, toucheventselector(GameOverLayer::handleTouchEvent));

	m_heroImg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_5"));

	m_scoreAtlasLabel = static_cast<TextAtlas*>(ui::Helper::seekWidgetByName(rootNode, "AtlasLabel_21"));

	m_scoreExtraLabel = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "Label_35"));

	m_maxWeekScoreImg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_42"));

	m_historyHighestScoreAtlasLabel = static_cast<TextAtlas*>(ui::Helper::seekWidgetByName(rootNode, "AtlasLabel_19"));

    m_coinNum = static_cast<TextAtlas*>(ui::Helper::seekWidgetByName(rootNode, "AtlasLabel_26"));

	m_coinNumLabel = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "Label_35_0"));
	
}

/*
void shareResultHandler(C2DXResponseState state, C2DXPlatType platType, CCDictionary *shareInfo, CCDictionary *error)
{
    switch (state) {
        case C2DXResponseStateSuccess:
            CCLog("分享成功");
            break;
        case C2DXResponseStateFail:
            CCLog("分享失败");
            break;
        default:
            break;
    }
}
*/

void GameOverLayer::shareToIntenet()
{
    //ShareToIntenet::getInstance()->showShareBox();
}

void GameOverLayer::handleTouchEvent(Ref *pSender, TouchEventType type)
{
	if (pSender == nullptr)
	{
	}

	else if (pSender == m_goonBtn && (TOUCH_EVENT_ENDED == type))
	{
		GameSound::playEffectMusic(EFF_CHANGE_PAGE);
		log("BattleData->getScore()~~~~~~%d", BattleData::getInstance()->getScore());
		log("Player->getIsMore()~~~~~~%d", Player::getInstance()->getIsMore());

		if (Player::getInstance()->getIsMore() == 1) 
		{
			int index = 1;
			auto mList = Player::getInstance()->getWorldRankList();
			for (auto friendItem : *(mList))
			{
				if (Player::getInstance()->getScore() > (*friendItem).score)
				{
					Layer* layer = RankingUpLayer::create();
					if(index <= 3)
					{
						layer = CrownCurrentLayer::create();
					}
					this->getScene()->addChild(layer);
					break;                                              
				}
				index++;
			}
		}
		 else
			{
				auto home = HomeScene::create();

				Director::getInstance()->replaceScene(home);
			}
	}

	else if (pSender == m_shareBtn && (TOUCH_EVENT_ENDED == type))
	{
        shareToIntenet();
	}
}

void GameOverLayer::refreshUI()
{
	GameSound::playEffectMusic(EFF_GOAL_UP);

	std::string json = cocos2d::FileUtils::getInstance()->getStringFromFile("MESSAGE_INFO.json");
    
	rapidjson::Document d;
    
	if ("" != json)
	{
		 d.Parse<0>(json.c_str());
	}

	std::string keyStr = StringUtils::format("%d", 31);

	std::string scoreLabel = (d)[keyStr.c_str()]["text1"].GetString();
	std::string coinLabel = (d)[keyStr.c_str()]["text2"].GetString();

    if (Player::getInstance()->getMaxScoreType() == 1)
    {
        m_maxWeekScoreImg->setVisible(true);
    }
    else {
        m_maxWeekScoreImg->setVisible(false);
    }
	
	mScore = Player::getInstance()->getScore();
	if ((mScore == Player::getInstance()->getHighestScore()) || (mScore == Player::getInstance()->getHistoryMaxScore()))
	{
		GameSound::playEffectMusic(EFF_NEW_RECORD);
	}
            

    
	int monsterS = BattleData::getInstance()->getMonsterScore();
    
    m_scoreAtlasLabel->setStringValue(StringUtils::format("%d", (BattleData::getInstance()->getScore()) + monsterS));
    
    m_historyHighestScoreAtlasLabel->setStringValue(StringUtils::format("%d", Player::getInstance()->getHistoryMaxScore()));
    
    m_coinNum->setStringValue(StringUtils::format("%d", BattleData::getInstance()->getGold()));
     
	float scorePercent = BattleData::getInstance()->getScoreAddPercent();
	scorePercent = scorePercent * 100;

	std::string scorePercentStr = StringUtils::format("%d",(int)scorePercent);
	scoreLabel = scoreLabel + scorePercentStr + "%";
	m_scoreExtraLabel->setText(scoreLabel.c_str());

	float coinPercent = BattleData::getInstance()->getGoldAddPercent();
	coinPercent = coinPercent * 100;
	std::string coinPercentStr = StringUtils::format("%d",(int)coinPercent);
	coinLabel = coinLabel + coinPercentStr + "%";
	m_coinNumLabel->setText(coinLabel.c_str());

}

