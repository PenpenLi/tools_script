////BattleUILayer.cpp created by php script. chris.li
#include "BattleUILayer.h"
#include "BattleData.h"
#include "util/GameSound.h"
#include "Player.h"
#include "RoleLevelInfo.h"
#include "Network.h"
#include "home/HomeScene.h"
#include "BattleScene.h"
#include "SimpleAudioEngine.h"
#include "GameOverLayer.h"

#define PROP_MAX 4
#define MASK_LAYER_TAG 101
#define SPRITE_LEADING_TAG_1 111
#define SPRITE_LEADING_TAG_2 112
#define SPRITE_LEADING_TAG_3 113
#define SPRITE_LEADING_TAG_4 114
#define SPRITE_LEADING_TAG_5 115

bool BattleUILayer::init()
{
	if (Layer::init())
	{
		GameSound::playBackgroundMusic(MUSIC_LAYER_INGAME);
		
		m_roleScore = 0.f;
		m_effectProp = 0.f;
		m_effectRandomProp = 0.f;
		m_effectRole = 0.f;

		m_goldAddRandomProp = 0.f;
		m_goldAddRole = 0.f;
		m_goldAddActivity = 0.f;

		m_priceBarPercent = 0;
		m_currentPriceBarPer = 0;

		m_frinedScore = 0;
		m_playerScore = 0;
		m_oldScore = 0;
		m_leadingPage = 0;

		m_leadngPage1IsOK = true;
		m_leadngPage2IsOK = true;
		m_leadngPage3IsOK = true;
		m_leadngPage4IsOK = true;
		m_leadngPage5IsOK = true;
        
        m_isReadyGoRunning = false;
        m_isPauseRunning = false;

		m_prop1 = 0; //PROP_SPRINT_TAG 

		m_prop2 = 0; //PROP_BUBBLE_TAG 
    
		m_prop3 = 0; //PROP_JET_TAG  
    
		m_prop4 = 0; //PROP_DIESPRINT_TAG

		m_prop5 = 0; //PROP_HELICOPTER_TAG

		m_prop6 = 0; //PROP_MAGNET_TAG

		m_prop7 = 0; //PROP_SPUERBUBBLE_TAG

		m_prop8 = 0; //PROP_MUSHROOM_TAG

		//Score Effect	 	
		updateEffectItem();
		loadUI();
        
		m_propIDs = new int[PROP_MAX];
			for (int i = 0; i < PROP_MAX; i++)
			{
				m_propIDs[i] = 0;
			}

        m_refreshCounter = 30;
        
		return true;
        
        
	}

	return false;
}
void BattleUILayer::onEnter()
{
	Layer::onEnter();

	m_count = 0;

	auto winS = Director::getInstance()->getWinSize();

	//Pause Menu
	SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("paly_button_up.png");
	Sprite* menuUp = Sprite::createWithSpriteFrame(frame);

	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("paly_picture_stop.png");
	Sprite* menuUpPause = Sprite::createWithSpriteFrame(frame);
	menuUpPause->setPosition(Point(40,45));
	menuUp->addChild(menuUpPause);

	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("paly_button_down.png");
	Sprite* menuDown = Sprite::createWithSpriteFrame(frame);

	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("paly_picture_stop.png");
	Sprite* menuDownPause = Sprite::createWithSpriteFrame(frame);
	menuDownPause->setPosition(Point(40,45));
	menuDown->addChild(menuDownPause);


	MenuItemSprite* itemSprite = MenuItemSprite::create(menuUp,menuDown,CC_CALLBACK_1(BattleUILayer::menuCallBackPause,this));
	m_pauseMenu = Menu::create(itemSprite,nullptr);
	m_pauseMenu->setPosition(580,60);
	this->addChild(m_pauseMenu,1);

	//Continue,Giveup Menu
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("paly_button_up_2.png");
	Sprite* continueMenuUp = Sprite::createWithSpriteFrame(frame);

	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("paly_button_down_2.png");
	Sprite* continueMenuDown = Sprite::createWithSpriteFrame(frame);

	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("paly_button_up_3.png");
	Sprite* giveupMenuUp = Sprite::createWithSpriteFrame(frame);

	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("paly_button_down_3.png");
	Sprite* giveupMenuDown = Sprite::createWithSpriteFrame(frame);

	

	MenuItemSprite* itemSp = MenuItemSprite::create(continueMenuUp,continueMenuDown,CC_CALLBACK_1(BattleUILayer::menuCallBackContinue,this));
	m_continueMenu = Menu::create(itemSp,nullptr);
	m_continueMenu->setPosition(300,500);
	m_continueMenu->setVisible(false);
	this->addChild(m_continueMenu,3);

	itemSp = MenuItemSprite::create(giveupMenuUp,giveupMenuUp,CC_CALLBACK_1(BattleUILayer::menuCallBackGiveUp,this));
	m_giveupMenu = Menu::create(itemSp,nullptr);
	m_giveupMenu->setPosition(300,300);
	m_giveupMenu->setVisible(false);
	this->addChild(m_giveupMenu,3);

	//Slider Bar
	if (BattleData::getInstance()->getBattleMode() == 0)
	{
		m_sliderBar = HPBar::create();
		this->addChild(m_sliderBar,1);

		m_sliderBar->setPosition(Point(100,50));
		m_sliderBar->setHP(0);
	}

	auto topLayer = Layer::create();
	//Score Number
	LabelAtlas* label = LabelAtlas::create("0123456789","paly_number.png",37,60,'0');
	label->setString("0");
	label->setAnchorPoint(Point(1,0));
	label->setPosition(Point(80,0));
	topLayer->addChild(label,1);
	m_scoreLabel = label;

	//Gold Number
	label = LabelAtlas::create("0123456789","paly_number_2.png",26,30,'0');
	label->setString("0");
	label->setAnchorPoint(Point(1,0));
	label->setPosition(Point(25,-35));
	topLayer->addChild(label,1);

	
	m_goldLabel = label;

	//Gold Coin
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("paly_picture_coin.png");
	Sprite* coin = Sprite::createWithSpriteFrame(frame);
	coin->setPosition(Point(60,-20));

	topLayer->setPosition(Point(winS.width - 100,winS.height  - 100));
	topLayer->addChild(coin,1);

	this->addChild(topLayer,1);
	//HP
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("paly_love.png");

	Player::getInstance()->getRoleType();

	int roleHP = 0;
	std::string roleName;
	std::string json = cocos2d::FileUtils::getInstance()->getStringFromFile("MESSAGE_INFO.json");
    
	rapidjson::Document d;
    
	if ("" != json)
	{
		 d.Parse<0>(json.c_str());
	}

	std::string keyStr = StringUtils::format("%d", 30);

	if (Player::getInstance()->getRoleType() < 4)
	{
		roleHP = 1;
		roleName = (d)[keyStr.c_str()]["text2"].GetString();
	}
	else if (Player::getInstance()->getRoleType() > 3)
	{
		roleHP = 2;
		roleName = (d)[keyStr.c_str()]["text1"].GetString();
	}

	if (Player::getInstance()->getPropRandom() == 23)
	{
		roleHP = 1;
	}
	else if (Player::getInstance()->getPropRandom() == 24)
	{
		roleHP = 2;
	}


	auto leftLayer = Layer::create();

	for (int i = 0; i < 2; i++)
	{
		Sprite* heartBg = Sprite::createWithSpriteFrame(frame);
		heartBg->setPosition(Point(110 + 30*i ,-20));
		leftLayer->addChild(heartBg,1);
        
        if (i == 0) 
		{
            m_heartBG1 = heartBg;          
        }

		if (i == 1)
		{
			m_heartBG2 = heartBg;
		}
	}

	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("paly_love_2.png");

	int i = 30;
	int w = 110;
	m_heart1 = Sprite::createWithSpriteFrame(frame);
	m_heart1->setPosition(Point(w, -20));
	leftLayer->addChild(m_heart1,1);

	m_heart2 = Sprite::createWithSpriteFrame(frame);
	m_heart2->setPosition(Point(w + i*1, -20));
	leftLayer->addChild(m_heart2,1);

	//m_heart3 = Sprite::createWithSpriteFrame(frame);
	//m_heart3->setPosition(Point(w + i*2, 900));
	//this->addChild(m_heart3,1);
	
	 if (Player::getInstance()->getPropRandom() == 23)
    {
		m_heartBG2->setVisible(false);
		m_heart2->setVisible(false);
        m_heartBG1->setVisible(true);
        m_heart1->setVisible(true);
		roleName = (d)[keyStr.c_str()]["text2"].GetString();
        roleHP = 1;
        
    }
    else if (Player::getInstance()->getPropRandom() == 24)
    {
		m_heartBG2->setVisible(true);
		m_heart2->setVisible(true);
		m_heartBG1->setVisible(true);
		m_heart1->setVisible(true);
		roleHP = 2;
		roleName = (d)[keyStr.c_str()]["text1"].GetString();
    }

	 if (roleHP == 1)
	 {
		m_heartBG2->setVisible(false);
		m_heart2->setVisible(false);
        m_heartBG1->setVisible(true);
        m_heart1->setVisible(true);
	 }
	 else if (roleHP == 2)
	 {
		m_heartBG2->setVisible(true);
		m_heart2->setVisible(true);
		m_heartBG1->setVisible(true);
		m_heart1->setVisible(true);
	 }
	changeHP(roleHP);

	//Player Picture

	//Player BG1
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("paly_bottom.png");
	Sprite* playerbg = Sprite::createWithSpriteFrame(frame);
	playerbg->setPosition(Point(50,0));
	leftLayer->addChild(playerbg,1);

	//Player Picture1

	std::string playerName;
	int playerType = Player::getInstance()->getRoleType();
    
    if (Player::getInstance()->getPropRandom() == 23) {
        playerType = 3;
    }
    else if (Player::getInstance()->getPropRandom() == 24)
    {
        playerType = 7;
    }
    
    if (playerType >= 1 && playerType <= 3)
    {
        playerName = "head_hippocampus.png";
    }
    else if (playerType >= 4)
    {
        playerName = "head_octopus.png";
    }

	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("paly_frame.png");
	Sprite* bubble1 = Sprite::createWithSpriteFrame(frame);
	Sprite* bubble2 = Sprite::createWithSpriteFrame(frame);

	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(playerName.c_str());
	Sprite* player1 = Sprite::createWithSpriteFrame(frame);
	player1->setPosition(Point(40,40));

	bubble1->addChild(player1);
	
	bubble1->setPosition(Point(50,0));
	leftLayer->addChild(bubble1,1);


	if (BattleData::getInstance()->getBattleMode() == 0)
	{

		//Player BG2
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("paly_bottom.png");
		playerbg = Sprite::createWithSpriteFrame(frame);
		playerbg->setPosition(Point(50,-80));
		leftLayer->addChild(playerbg,1);

		//Player Picture2
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("head_hippocampus.png");
		Sprite* player2 = Sprite::createWithSpriteFrame(frame);
		player2->setPosition(Point(40,40));

		bubble2->addChild(player2);

		bubble2->setPosition(Point(50,-80));
		leftLayer->addChild(bubble2,1);

		//Friend Lable

		auto friendList = Player::getInstance()->getFriendList();
		friendList->reverse();

		m_friendNicknameLable = LabelTTF::create("aaaa","Arial",20);
        m_friendNicknameLable->setColor(Color3B::BLACK);
		m_friendNicknameLable->setPosition(Point(97,-80));
		m_friendNicknameLable->setAnchorPoint(Point::ZERO);
		m_friendNicknameLable->setHorizontalAlignment(kCCTextAlignmentLeft);
		leftLayer->addChild(m_friendNicknameLable,1);

	

		std::string score_str = StringUtils::format("%d",m_frinedScore);
		auto playerScore = LabelTTF::create(score_str,"Arial",20);
        playerScore->setColor(Color3B::BLACK);
	

		m_friendScoreLable = playerScore;

		playerScore->setPosition(Point(120,-100));
		playerScore->setHorizontalAlignment(kCCTextAlignmentLeft);
		leftLayer->addChild(playerScore,1);

		if (!changeFriendScore(0))
		{
			//m_friendNicknameLable->setVisible(false);
			//m_friendScoreLable->setVisible(false);
			//bubble2->setVisible(false);
			//playerbg->setVisible(false);
			//player2->setVisible(false);

		}

	}


	//Player Label1
	auto playerLabel1 = LabelTTF::create(roleName,"Arial",20);
    playerLabel1->setColor(Color3B::BLACK);
	playerLabel1->setPosition(Point(140,15));
	playerLabel1->setHorizontalAlignment(kCCTextAlignmentLeft);;
	leftLayer->addChild(playerLabel1,1);

	leftLayer->setPosition(Point(0,winS.height  - 70));
	this->addChild(leftLayer);

    m_isReadyGoRunning = true;
    
    auto jumpReadyListener = EventListenerCustom::create("EVENT_ENTER_BACKGROUND", [this](EventCustom* event)
                                                         {
                                                             this->menuCallBackPause(Node::create());
                                                         });
    
    _eventDispatcher->addEventListenerWithFixedPriority(jumpReadyListener, 1);
    
    auto changeHeart1Listener = EventListenerCustom::create("EVENT_CHANGE_HEART1", [this](EventCustom* event)
                                                         {
                                                             m_heartBG2->setVisible(false);
															 m_heart2->setVisible(false);
                                                             m_heartBG1->setVisible(true);
                                                             m_heart1->setVisible(true);
                                                         });
    
    _eventDispatcher->addEventListenerWithFixedPriority(changeHeart1Listener, 1);
    
    auto changeHeart2Listener = EventListenerCustom::create("EVENT_CHANGE_HEART2", [this](EventCustom* event)
                                                            {
																 m_heartBG2->setVisible(true);
																 m_heart2->setVisible(true);
																 m_heartBG1->setVisible(true);
																 m_heart1->setVisible(true);
                                                            });
    
    _eventDispatcher->addEventListenerWithFixedPriority(changeHeart2Listener, 1);
    
	//readyGo();
    
    
}

void BattleUILayer::onExit()
{
	Layer::onExit();
    
    _eventDispatcher->removeCustomEventListeners("EVENT_ENTER_BACKGROUND");
    _eventDispatcher->removeCustomEventListeners("EVENT_CHANGE_HEART1");
    _eventDispatcher->removeCustomEventListeners("EVENT_CHANGE_HEART2");

	delete []m_propIDs;
}

void BattleUILayer::loadUI()
{
}

void BattleUILayer::menuCallBackLeading(Ref* pSender)
{
	GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);
	//do something to continue
	auto scene = Director::getInstance()->getRunningScene();
	auto battleScene = dynamic_cast<BattleScene*>(scene);

	battleScene->resume();

	leadingPlayer(m_leadingPage, false);
	
}

void BattleUILayer::menuCallBackContinue(Ref* pSender)
{
	GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);
	this->removeChildByTag(MASK_LAYER_TAG);
	m_continueMenu->setVisible(false);
	m_giveupMenu->setVisible(false);


	//do something to continue
	auto scene = Director::getInstance()->getRunningScene();
	auto battleScene = dynamic_cast<BattleScene*>(scene);

	//battleScene->resume();
    if (m_isReadyGoRunning) {
        return;
    }
	readyGo();
    m_isReadyGoRunning = true;
    m_isPauseRunning = false;
	
}

void BattleUILayer::menuCallBackGiveUp(Ref* pSender)
{
	GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);
	this->removeChildByTag(MASK_LAYER_TAG);
	m_continueMenu->setVisible(false);
	m_giveupMenu->setVisible(false);
	
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	auto home = HomeScene::create();
    
	if (BattleData::getInstance()->getBattleMode() == 0)
	{
		auto gameScene = static_cast<BattleScene*>(this->getScene());

		BattleData::getInstance()->setGameWin(2);

		auto resultLayer = GameOverLayer::create();

		this->getScene()->addChild(resultLayer);
		BattleData::getInstance()->setIsRealDie(false);
		gameScene->pause();
	}
	else
	{
		Director::getInstance()->replaceScene(home);
	}
    
	//do something to give up
}

void BattleUILayer::menuCallBackPause(Ref* pSender)
{
	GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);
    
    if (m_isPauseRunning || m_isReadyGoRunning) {
        return;
    }
    
    m_isPauseRunning = true;
	m_continueMenu->setVisible(true);
	m_giveupMenu->setVisible(true);

	//Pause Layer
	LayerColor* m_maskLayer = LayerColor::create(Color4B::BLACK, Director::getInstance()->getWinSize().width, Director::getInstance()->getWinSize().height);
    m_maskLayer->setOpacity(180); 
	m_maskLayer->setTag(MASK_LAYER_TAG);
    this->addChild(m_maskLayer,2);

	auto scene = Director::getInstance()->getRunningScene();
	auto battleScene = dynamic_cast<BattleScene*>(scene);

	battleScene->pause();



}

void BattleUILayer::handleTouchEvent(Ref *pSender, TouchEventType type)
{

}

void BattleUILayer::sceneUpdate(float dt)
{
	m_count++;
}

void BattleUILayer::propSetTime(int propTag, int time)
{
	float p ;
	if (propTag == PROP_SPRINT_TAG)
	{
		p = time / (m_prop1 * 60.f);
	}
	else if (propTag  == PROP_BUBBLE_TAG)
	{
		p = time / (m_prop2 * 60.f);
	}
	else if (propTag  == PROP_JET_TAG)
	{
		p = time / (m_prop3 * 60.f);	
	}
	else if (propTag  == PROP_DIESPRINT_TAG)
	{
		p = time / (m_prop4 * 60.f);	
	}
	else if (propTag  == PROP_HELICOPTER_TAG)
	{	
		p = time / (m_prop5 * 60.f);
	}
	else if (propTag  == PROP_MAGNET_TAG)
	{
		p = time / (m_prop6 * 60.f);	
	}
	else if (propTag  == PROP_SPUERBUBBLE_TAG)
	{
		p = time / (m_prop7 * 60.f);	
	}
	else if (propTag  == PROP_MUSHROOM_TAG)
	{
		p = time / (m_prop8 * 60.f);	
	}

	auto maskLayer = static_cast<Sprite*>(this->getChildByTag(propTag)->getChildByTag(propTag));

	float a = 63 * p;
	//float per = 63 - a;
	float per = a;
	maskLayer->setPosition(31 , 35 - per);
	maskLayer->setTextureRect(Rect(0,per,63,63));

}

bool BattleUILayer::propStart(int propTag)
{
	int updateProp = 0;

	for (int i = 0; i < PROP_MAX; i++)
	{
		if (m_propIDs[i] == propTag)
		{
			return false;
		}
	}

	int grid = 0;

	for (grid = 0; grid < PROP_MAX; grid++)
	{
		if (m_propIDs[grid] == 0)
		{
			m_propIDs[grid] = propTag;
			break;
		}
	}

	if (grid != 4)
	{
		SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("paly_bottom.png");
		Sprite* playButton = Sprite::createWithSpriteFrame(frame);
		playButton->setTag(propTag);
		this->addChild(playButton);

		std::string frameName;
		
		if (propTag == PROP_SPRINT_TAG)
		{
			frameName = "paly_sprint.png";
		}
		else if (propTag  == PROP_BUBBLE_TAG)
		{
			frameName = "paly_bubble.png";
		}
		else if (propTag  == PROP_JET_TAG)
		{
			frameName = "paly_jet.png";
		}
		else if (propTag  == PROP_DIESPRINT_TAG)
		{
			frameName = "paly_diesprint.png";
		}
		else if (propTag  == PROP_HELICOPTER_TAG)
		{
			frameName = "paly_helicopter.png";
		}
		else if (propTag  == PROP_MAGNET_TAG)
		{
			frameName = "paly_magnet.png";
		}
		else if (propTag  == PROP_SPUERBUBBLE_TAG)
		{
			frameName = "paly_superBubble.png";
		}
		else if (propTag  == PROP_MUSHROOM_TAG)
		{
			frameName = "paly_mushroom.png";
		}

		SpriteFrame *propFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName.c_str());
		Sprite* propPic = Sprite::createWithSpriteFrame(propFrame);

		playButton->addChild(propPic);

		auto maskLayer = Sprite::create("paly_cooling.png");
		//LayerColor* maskLayer = LayerColor::create(Color4B::BLACK, playButton->getContentSize().width - 4, playButton->getContentSize().height + 3);
		maskLayer->setTag(propTag);
		maskLayer->setOpacity(180);
		maskLayer->setPosition(Point(0,0));

		playButton->addChild(maskLayer);

		propPic->setPosition(Point(30,33));

		auto winS = Director::getInstance()->getWinSize();

		playButton->setPosition(Point(winS.width - 50,winS.height - 180 - 70 * grid));
		playButton->setTag(propTag);
	}
	
	return true;
}

void BattleUILayer::propOverTime(int propTag)
{
	//m_propIDs.eraseObject(propID);
	int grid = 0;
		for (grid = 0; grid < PROP_MAX; grid++)
		{
			if (m_propIDs[grid] == propTag)
			{
				m_propIDs[grid] = 0;
				break;
			}
		}

	if (grid != 4)
	{
		this->removeChildByTag(propTag);
	}
}

void BattleUILayer::setScore(std::string score)
{
	m_scoreLabel->setString(score.c_str());
	
}

void BattleUILayer::setGold(std::string gold)
{
	m_goldLabel->setString(gold.c_str());
}
void BattleUILayer::changeHP(int hp)
{
	if (hp == 0)
	{
		m_heart1->setVisible(false);
		m_heart2->setVisible(false);
		//m_heart3->setVisible(false);
	}
	else if (hp == 1)
	{
		m_heart1->setVisible(true);
		m_heart2->setVisible(false);
		//m_heart3->setVisible(false);
	}
	else if (hp == 2)
	{
		m_heart1->setVisible(true);
		m_heart2->setVisible(true);
		//m_heart3->setVisible(false);
	}
	else if (hp == 3)
	{
		m_heart1->setVisible(true);
		m_heart2->setVisible(true);
		//m_heart3->setVisible(true);
	}
}

void BattleUILayer::openPriceLevel()
{

}

void BattleUILayer::changeSlider(float p)
{
	if (BattleData::getInstance()->getBattleMode() != 0)
	{
		return;
	}

	m_priceBarPercent += p;

	if (m_priceBarPercent > 1.f)
	{

		m_priceBarPercent = m_priceBarPercent * 10;
		int per = (int)m_priceBarPercent;
		int t = per % 10;
		per -= t;
		float r = t /10.f;
		m_priceBarPercent = r;

		per = per /10;


		m_currentPriceBarPer += per;

		if (m_currentPriceBarPer > 100)
		{
			openPriceLevel();
			return;
		}
		else
		{
			m_sliderBar->setHP(m_currentPriceBarPer);
		}
		
		//m_priceBarPercent -= 1.f;
	}



	
}

bool BattleUILayer::changeFriendScore(int score)
{
	if (BattleData::getInstance()->getBattleMode() != 0)
	{
		return false;
	}

	int playerScoreNum = score;
	auto friendList = Player::getInstance()->getFriendList();
	
	std::string nickName;

	int tempScore = 0;

	for (auto item : (*friendList))
	{
		if (item->score > playerScoreNum)
		{
			if (tempScore > item->score && tempScore != 0)
			{
				tempScore = item->score;
				nickName = item->nickname;
			}
			else if (tempScore == 0)
			{
				tempScore = item->score;
				nickName = item->nickname;
			}

		}


	}

	m_frinedScore = tempScore;
		
	m_friendNicknameLable->setString(nickName);

	m_friendScoreLable->setString(StringUtils::format("%d",m_frinedScore));

	m_frinedScore = 0;

	if (m_frinedScore == 0)
	{
		return false;
	}

	return true;
}

void BattleUILayer::ntf(HeroNtf &heroNtfData)
{
    if (heroNtfData.heroUIDataNtf.tag == 0) {return;}

	if (heroNtfData.heroUIDataNtf.gold != 0)
	{
		float goldNum = heroNtfData.heroUIDataNtf.gold;

		goldNum = goldNum * (1 + m_goldAddRole + m_goldAddRandomProp);

		int coin = Player::getInstance()->getCoin();
		coin += (int)goldNum;
		BattleData::getInstance()->setGold(goldNum);
		
		setGold(StringUtils::format("%d", (int)goldNum));
		
	}
    
    auto heroNtf = heroNtfData.heroUIDataNtf;
    
	char chr[64];

	if (heroNtf.hp != 0)
	{
		changeHP(heroNtf.hp - 1);
	}
	

	float h = Director::getInstance()->getWinSize().height / 10;
	float newH = heroNtfData.scrollDistance / h;
	float newScore = (newH * 20) * (1.f + m_effectProp + m_effectRandomProp + m_effectRole);
	float oldScore = newH * 20;

	m_oldScore += (int)oldScore;

	m_roleScore += newScore;

	int score = (int)m_roleScore;

	m_playerScore = score;

	if (m_playerScore > m_frinedScore && m_playerScore > 0)
	{
		changeFriendScore(m_playerScore);
	}

	

	float starPercent = (float)heroNtfData.heroUIDataNtf.star;

	float per;

	if (heroNtfData.levelType == 2)
	{
		m_currentPriceBarPer = 0;
		m_sliderBar->setHP(0);
	}

	if (starPercent > 0)
	{
		per = starPercent * 5;
		changeSlider(per);
	}
	else if (heroNtfData.scrollDistance > 0)
	{
		per = newH * 0.5f;

		setScore(StringUtils::format("%d",m_playerScore));
		BattleData::getInstance()->setScore(m_playerScore);
		//log("Score:%d",Player::getInstance()->getScore());
		changeSlider(per);

		auto scene = Director::getInstance()->getRunningScene();
		auto battleScene = dynamic_cast<BattleScene*>(scene);

	

		if (m_oldScore > 50 && BattleData::getInstance()->getBattleLevel() == 1)
		{
			if (m_leadngPage2IsOK)
			{
				leadingPlayer(2,true);
				battleScene->pause();
				m_leadngPage2IsOK = false;
			}

		}
		if (m_oldScore > 230 && BattleData::getInstance()->getBattleLevel() == 1)
		{
			if (m_leadngPage1IsOK)
			{
				leadingPlayer(1,true);
				battleScene->pause();
				m_leadngPage1IsOK = false;
			}
		}
		if (m_oldScore > 850 && BattleData::getInstance()->getBattleLevel() == 1)
		{
			if (m_leadngPage3IsOK)
			{
				leadingPlayer(3,true);
				battleScene->pause();
				m_leadngPage3IsOK = false;
			}
		}
		if (m_oldScore > 100 && BattleData::getInstance()->getBattleLevel() == 5)
		{
			if (m_leadngPage4IsOK)
			{
				leadingPlayer(4,true);
				battleScene->pause();
				m_leadngPage4IsOK = false;
			}
		}
		if (m_playerScore > 80 && BattleData::getInstance()->getBattleLevel() == 2)
		{
			if (m_leadngPage1IsOK)
			{
				//leadingPlayer(1,true);
				//battleScene->pause();
				//m_leadngPage1IsOK = false;
			}
		}
	}

	//Change Prop
	if (heroNtf.prop1 > 0)
	{
		propStart(PROP_SPRINT_TAG);
		propSetTime(PROP_SPRINT_TAG,heroNtf.prop1);
	}
	if (heroNtf.prop1 < 0)
	{
		propOverTime(PROP_SPRINT_TAG);
	}

	if (heroNtf.prop2 > 0)
	{
		propStart(PROP_BUBBLE_TAG);
		propSetTime(PROP_BUBBLE_TAG,heroNtf.prop2);
	}
	if (heroNtf.prop2 < 0)
	{
		propOverTime(PROP_BUBBLE_TAG);
	}

	if (heroNtf.prop3 > 0)
	{
		propStart(PROP_JET_TAG);
		propSetTime(PROP_JET_TAG,heroNtf.prop3);
	}
	if (heroNtf.prop3 < 0)
	{
		propOverTime(PROP_JET_TAG);
	}

	if (heroNtf.prop4 > 0)
	{
		propStart(PROP_DIESPRINT_TAG);
		propSetTime(PROP_DIESPRINT_TAG,heroNtf.prop4);
	}
	if (heroNtf.prop4 < 0)
	{
		propOverTime(PROP_DIESPRINT_TAG);
	}
	if (heroNtf.prop5 > 0)
	{
		propStart(PROP_HELICOPTER_TAG);
		propSetTime(PROP_HELICOPTER_TAG,heroNtf.prop5);
	}
	if (heroNtf.prop5 < 0)
	{
		propOverTime(PROP_HELICOPTER_TAG);
	}
	if (heroNtf.prop6 > 0)
	{
		propStart(PROP_MAGNET_TAG);
		propSetTime(PROP_MAGNET_TAG,heroNtf.prop6);
	}
	if (heroNtf.prop6 < 0)
	{
		propOverTime(PROP_MAGNET_TAG);
	}
	if (heroNtf.prop7 > 0)
	{
		propStart(PROP_SPUERBUBBLE_TAG);
		propSetTime(PROP_SPUERBUBBLE_TAG,heroNtf.prop7);
	}
	if (heroNtf.prop7 < 0)
	{
		propOverTime(PROP_SPUERBUBBLE_TAG);
	}
	if (heroNtf.prop8 > 0)
	{
		propStart(PROP_MUSHROOM_TAG);
		propSetTime(PROP_MUSHROOM_TAG,heroNtf.prop8);
	}
	if (heroNtf.prop8 < 0)
	{
		propOverTime(PROP_MUSHROOM_TAG);
	}

}

void BattleUILayer::updateEffectItem()
{
		bool breakType;
		int roleLevel;

		//Score And Gold
		roleLevel = BattleData::getInstance()->getRoleLevel();
    
        if (BattleData::getInstance()->getPropRandom() == 23) {
            roleLevel = 40;
        }
        else if (BattleData::getInstance()->getPropRandom() == 24)
        {
            roleLevel = 30;
        }

		if (Player::getInstance()->getRoleType() == 1)
		{
			breakType = false;
		}
		else if (Player::getInstance()->getRoleType() == 2)
		{
			if (roleLevel == 10)
			{
				breakType = true;
			}
			else
			{
				breakType = false;
			}
		}
		else if (Player::getInstance()->getRoleType() == 3)
		{
			if (roleLevel == 20)
			{
				breakType = true;
			}
			else
			{
				breakType = false;
			}
		}
		else if (Player::getInstance()->getRoleType() == 4)
		{
            breakType = false;

		}
		else if (Player::getInstance()->getRoleType() == 5)
		{
            if (roleLevel == 10)
			{
				breakType = true;
			}
			else
			{
				breakType = false;
			}
		}
		else if (Player::getInstance()->getRoleType() == 6)
		{
			if (roleLevel == 20)
			{
				breakType = true;
			}
			else
			{
				breakType = false;
			}
		}
		else if (Player::getInstance()->getRoleType() == 7)
		{
			if (roleLevel == 30)
			{
				breakType = true;
			}
			else
			{
				breakType = false;
			}
		}

		if (breakType == true)
		{
			roleLevel += 0.5f;
			m_effectRole = RoleLevelInfo::getInstance()->getScoreExtra(StringUtils::format("%d",roleLevel));
			m_goldAddRole = RoleLevelInfo::getInstance()->getCoinExtra(StringUtils::format("%d",roleLevel));
		}
		else
		{
			m_effectRole = RoleLevelInfo::getInstance()->getScoreExtra(StringUtils::format("%d",roleLevel));
			m_goldAddRole = RoleLevelInfo::getInstance()->getCoinExtra(StringUtils::format("%d",roleLevel));
		}
    
        if (Player::getInstance()->getRoleType() > 3)
        {
            m_effectRole += 0.2;
        }

    if (BattleData::getInstance()->getProp4() > 0 && BattleData::getInstance()->getBattleMode() == 0)
		{
			m_effectProp = 0.2;
		}

		if (BattleData::getInstance()->getPropRandom() == 19)
		{
			m_effectRandomProp = 0.05;
		}
		else if (BattleData::getInstance()->getPropRandom() == 20)
		{
			m_effectRandomProp = 0.1f;
		}
		else if (BattleData::getInstance()->getPropRandom() == 21)
		{
			m_effectRandomProp = 0.15f;
		}
		else if (BattleData::getInstance()->getPropRandom() == 25)
		{
			m_goldAddRandomProp = 2.f;
		}
		else if (BattleData::getInstance()->getPropRandom() == 26)
		{
			m_goldAddRandomProp = 0.5;
		}
    
		//Prop Time
		if (BattleData::getInstance()->getIsBreak())
		{
			roleLevel = BattleData::getInstance()->getRoleLevel() + 0.5f;
		}
		else
		{
			roleLevel = BattleData::getInstance()->getRoleLevel();
		}

		if (BattleData::getInstance()->getProp1() == 0)
		{
			if (BattleData::getInstance()->getPropRandom() == 13)
			{
				m_prop1 = 5;
			}
			else if (BattleData::getInstance()->getPropRandom() == 14)
			{
				m_prop1 = 8;
			}
			else if (BattleData::getInstance()->getPropRandom() == 15)
			{
				m_prop1 = 15;
			}
		}
		else
		{
			m_prop1 = 6;
		}

		m_prop2 = RoleLevelInfo::getInstance()->getShieldTime(StringUtils::format("%d",roleLevel));

		m_prop3 = 5;

		if (BattleData::getInstance()->getProp3() == 0)
		{
			if (BattleData::getInstance()->getPropRandom() == 16)
			{
				m_prop4 = 5;
			}
			else if (BattleData::getInstance()->getPropRandom() == 17)
			{
				m_prop4 = 8;
			}
			else if (BattleData::getInstance()->getPropRandom() == 18)
			{
				m_prop4 = 12;
			}
		}
		else
		{
			m_prop4 = 6;
		}
		

		m_prop5 = RoleLevelInfo::getInstance()->getHelicopterTime(StringUtils::format("%d",roleLevel));

		m_prop6 = RoleLevelInfo::getInstance()->getMagnetTime(StringUtils::format("%d",roleLevel));

		m_prop7 = 0;

		m_prop8 = RoleLevelInfo::getInstance()->getShipTime(StringUtils::format("%d",roleLevel));

		BattleData::getInstance()->setGoldAddPercent(m_goldAddRole + m_goldAddRandomProp);

		float per1 = m_effectProp + m_effectRandomProp + m_effectRole;
		BattleData::getInstance()->setScoreAddPercent(per1);
	
}

void BattleUILayer::leadingPlayer(int page, bool onOff)
{
	SpriteFrame *smallBoard = SpriteFrameCache::getInstance()->getSpriteFrameByName("leading_small_board.png");
	if (page ==1)
	{
		m_leadingPage = 1;
		SpriteFrame *bigBoard = SpriteFrameCache::getInstance()->getSpriteFrameByName("leading_big_board.png");
		SpriteFrame *picFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("leading_action.png");

		Sprite* pic = Sprite::createWithSpriteFrame(picFrame);
		Sprite* board = Sprite::createWithSpriteFrame(bigBoard);

		if (onOff)
		{
			board->addChild(pic);
			board->setTag(SPRITE_LEADING_TAG_1);
			board->setAnchorPoint(Point(0,0));
			pic->setAnchorPoint(Point(0,0));

			auto itemSp = MenuItemSprite::create(board,board,CC_CALLBACK_1(BattleUILayer::menuCallBackLeading,this));
			auto menu = Menu::create(itemSp,nullptr);
			menu->setTag(SPRITE_LEADING_TAG_1);
			this->addChild(menu);
		}
		else
		{
			this->removeChildByTag(SPRITE_LEADING_TAG_1);
		}
		
	}
	else if (page == 2)
	{
		m_leadingPage = 2;
		SpriteFrame *picFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("leading_jump.png");

		Sprite* pic = Sprite::createWithSpriteFrame(picFrame);
		Sprite* board = Sprite::createWithSpriteFrame(smallBoard);

		if (onOff)
		{
			board->addChild(pic);
			board->setTag(SPRITE_LEADING_TAG_2);
			board->setAnchorPoint(Point(0,0));
			pic->setAnchorPoint(Point(0,0));

			auto itemSp = MenuItemSprite::create(board,board,CC_CALLBACK_1(BattleUILayer::menuCallBackLeading,this));
			auto menu = Menu::create(itemSp,nullptr);
			menu->setTag(SPRITE_LEADING_TAG_2);
			this->addChild(menu);
		}
		else
		{
			this->removeChildByTag(SPRITE_LEADING_TAG_2);
		}
	}
	else if (page == 3)
	{
		m_leadingPage = 3;
		SpriteFrame *picFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("leading_through.png");

		Sprite* pic = Sprite::createWithSpriteFrame(picFrame);
		Sprite* board = Sprite::createWithSpriteFrame(smallBoard);

		if (onOff)
		{
			board->addChild(pic);
			board->setTag(SPRITE_LEADING_TAG_3);
			board->setAnchorPoint(Point(0,0));
			pic->setAnchorPoint(Point(0,0));

			auto itemSp = MenuItemSprite::create(board,board,CC_CALLBACK_1(BattleUILayer::menuCallBackLeading,this));
			auto menu = Menu::create(itemSp,nullptr);
			menu->setTag(SPRITE_LEADING_TAG_3);
			this->addChild(menu);
		}
		else
		{
			this->removeChildByTag(SPRITE_LEADING_TAG_3);
		}
	}
	else if (page == 4)
	{
		m_leadingPage = 4;
		SpriteFrame *picFrame;
		
		if (BattleData::getInstance()->getRoleType() > 3)
		{
			picFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("leading_jump2.png");
		}
		else if (BattleData::getInstance()->getRoleType() < 4)
		{
			picFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("leading_attack.png");
		}

		Sprite* pic = Sprite::createWithSpriteFrame(picFrame);
		Sprite* board = Sprite::createWithSpriteFrame(smallBoard);

		if (onOff)
		{
			board->addChild(pic);
			board->setTag(SPRITE_LEADING_TAG_4);
			board->setAnchorPoint(Point(0,0));
			pic->setAnchorPoint(Point(0,0));

			auto itemSp = MenuItemSprite::create(board,board,CC_CALLBACK_1(BattleUILayer::menuCallBackLeading,this));
			auto menu = Menu::create(itemSp,nullptr);
			menu->setTag(SPRITE_LEADING_TAG_4);
			this->addChild(menu);
		}
		else
		{
			this->removeChildByTag(SPRITE_LEADING_TAG_4);
		}
	}
	else if (page == 5)
	{
		m_leadingPage = 5;
		SpriteFrame *picFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("leading_attack.png");

		
		Sprite* pic = Sprite::createWithSpriteFrame(picFrame);
		Sprite* board = Sprite::createWithSpriteFrame(smallBoard);

		if (onOff)
		{
			board->addChild(pic);
			board->setTag(SPRITE_LEADING_TAG_5);
			board->setAnchorPoint(Point(0,0));
			pic->setAnchorPoint(Point(0,0));

			auto itemSp = MenuItemSprite::create(board,board,CC_CALLBACK_1(BattleUILayer::menuCallBackLeading,this));
			auto menu = Menu::create(itemSp,nullptr);
			menu->setTag(SPRITE_LEADING_TAG_5);
			this->addChild(menu);
		}
		else
		{
			this->removeChildByTag(SPRITE_LEADING_TAG_5);
		}
	}
	else if (page == 6)
	{


	}
	
}

void BattleUILayer::readyGo()
{
	auto ready = Sprite::createWithSpriteFrameName("Ready_Go_1.png");
	Size winS = Director::getInstance()->getWinSize();

	ready->setPosition(Point(winS.width /2 , winS.height /2));
	auto animation = Animation::create();
	for(int i=1;i<=4;++i)
	{
		std::string spriteFrameName = StringUtils::format("Ready_Go_%d.png",i);
		SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
		animation->addSpriteFrame(frame);
	}
	animation->setDelayPerUnit(1.f);
	animation->setRestoreOriginalFrame(false);
	auto action = Animate::create(animation);
	auto callFunc = CallFunc::create([=]()
      {
			ready->removeFromParent();

			auto scene = Director::getInstance()->getRunningScene();
			auto battleScene = dynamic_cast<BattleScene*>(scene);

			m_isReadyGoRunning = false;

			battleScene->resume();
	});

	auto seq = Sequence::createWithTwoActions(action, callFunc);
	ready->runAction(seq);

	this->addChild(ready,100);
}