#include "RoleLayer.h"
#include "UiConfig.h"
#include "MainLayer.h"
#include "Player.h"
#include "Network.h"
#include "ShopLayer.h"
#include "RoleLevelInfo.h"
#include "util/GameSound.h"
//
#include "TipsInfo.h"
#include "BattleData.h"

bool RoleLayer::init()
{
	if (Layer::init())
	{
		//member variable init
		m_hpTimeText = 0;
		m_hpAddBtn = 0;
		m_diamondNumText = 0;
		m_diamondAddBtn = 0;
		m_goldNumText = 0;
		m_goldAddBtn = 0;

		m_roleSelectSvp = 0;

		m_roleImgV = new cocos2d::Vector<ImageView*>();

		m_selectRoleImgV = new cocos2d::Vector<ImageView*>();

		m_roleInfoText = 0;
		m_scoreBuffText = 0;
		m_goldBuffText = 0;
		m_propBuffText = 0;

		m_lvUpButton = 0;
		m_goldCostImg = nullptr;
		m_diamondCostImg = nullptr;
		m_lvUpImg = nullptr;
		m_breakImg = nullptr;
		m_buyImg = nullptr;
		m_buttonState = STATE_null;
		m_roleLock = nullptr;


		m_lvUpCostText = 0;
		m_returnBtn = 0;
		m_msgText = 0;

		m_frontRoleId = 0;

		//msg time 
		m_msgTime = 0.0f;
		m_msgSwitch = false;
		m_scrollSwitch = false;
		m_noticeSwitch = true;
		m_msgEndPosX = 0.0f;

		//Ui res
		importUI();

		//test
		//t_testUpdateTime = 0.0f;



		return true;
	}

	return false;
}

void RoleLayer::onEnter()
{
	Layer::onEnter();

	//update
	updateHp();
	updateDiamond();
	updateGold();

	switch (Player::getInstance()->getRoleType())
	{
	case 1:
	case 2:
	case 3:
		m_frontRoleId = 0;
		break;
	case 4:
	case 5:
	case 6:
	case 7:
		m_frontRoleId = 1;
		break;
	default:
		break;
	}

	//get role list
	if(0==Player::getInstance()->getRoleList()->size())
	{
		Network::getInstance()->do_RoleList_Req([this](const AckInfo &ackInfo) 
		{
			std::string msgInfo = ackInfo.getMessageInfo();

			if (msgInfo != "")
			{
				log("[RoleList] %s", msgInfo.c_str());
			}
			else
			{
				//ok
				log("[RoleList] : success~~!!");
				//level up info~
				updateLevelUpInfo();
				//role info
				updateRoleInfo();
			}
		});
	}
	else
	{
		//level up info~
		updateLevelUpInfo();
		//role info
		updateRoleInfo();
	}

	m_roleSelectSvp->refreshRolePostion(m_frontRoleId);

	m_selectRoleImgV->at(m_frontRoleId)->setVisible(true);

	//
	updateRoleState();
	update(0);

	//schedule
	this->scheduleUpdate();
	this->schedule(schedule_selector(RoleLayer::scheduleHpTime),1.0f);
	//tips
	this->schedule(schedule_selector(RoleLayer::scheduleMsg),TIME_tipsScheduleDelay);

	//level up info~
	//updateLevelUpInfo();

	//role info
	//updateRoleInfo();
    
    /*
    auto testLabel = Label::createWithSystemFont("前面有一篇讲配置文件的随笔，里面说到要想显示中文可以先把中文放在配置文件中，然后再读出来显示，这样可以避免中文乱码的问题","Marker Felt",24);
    testLabel->setPosition( Point(320, 450));
    
    testLabel->setColor(Color3B::WHITE);
    
    //testLabel->setPositionY(200);
    
    this->addChild(testLabel);
     */

}

void RoleLayer::onExit()
{
	Layer::onExit();

	//remove event listener
	//_eventDispatcher->removeCustomEventListeners("Return_shopLayer");

	//schedule
	this->unscheduleUpdate();
	this->unschedule(schedule_selector(RoleLayer::scheduleHpTime));
	this->unschedule(schedule_selector(RoleLayer::scheduleMsg));

}

bool RoleLayer::importUI()
{
	bool bRet = false;
	do{
		Widget* t_widget = GUIReader::getInstance()->widgetFromJsonFile("RoleUi/RoleUi.json");
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
			m_hpAddBtn->addTouchEventListener(this, toucheventselector(RoleLayer::touchEventShopBtn));
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
			m_diamondAddBtn->addTouchEventListener(this, toucheventselector(RoleLayer::touchEventShopBtn));
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
			m_goldAddBtn->addTouchEventListener(this, toucheventselector(RoleLayer::touchEventShopBtn));
		}
		else
			log("~~error~~ add Gold button is Null ");


		//test
		//Widget* t_test  = t_widgetPalAssets->clone();
		//t_test->setPosition(t_test->getPosition()+ccp(0,50));
		//t_test->setVisible(true);
		//t_test->setZOrder(256);
		//this->addChild(t_test);


		//scrollview role select

		m_roleSelectSvp = static_cast<MyScrollView*>(t_widget->getChildByName("svp_roleSelect"));

		if (m_roleSelectSvp)
		{
			m_roleSelectSvp->addTouchEventListener(this, toucheventselector(RoleLayer::touchEventRoleSvp));
		}
		else
			log("~~error~~ add Role ScrollView is Null ");

		int t_childrenCount = m_roleSelectSvp->getChildrenCount();

		for (int count = 0; count < t_childrenCount; count++)
		{
			String* t_roleFrameStr = String::createWithFormat("img_roleBg_00%d",count+1);
			Widget* t_roleFrame = m_roleSelectSvp->getChildByName(t_roleFrameStr->getCString());
			roleInfo_s t_roleInfoS;

			t_roleInfoS.roleImg = static_cast<ImageView*>(t_roleFrame->getChildByName("img_role"));
			t_roleInfoS.roleInfoPal = static_cast<Layout*>(t_roleFrame->getChildByName("pal_roleInfo"));
			t_roleInfoS.lvNumText = static_cast<Text*>(t_roleInfoS.roleInfoPal->getChildByName("lab_level"));
			t_roleInfoS.lvLoadingBar = static_cast<LoadingBar*>(t_roleInfoS.roleInfoPal->getChildByName("img_pgbBg")->getChildByName("pgb_level"));
			m_roleInfoS.push_back(t_roleInfoS);
		}

		//select img
		m_roleImgV->clear();

		m_selectRoleImgV->clear();

		int _roleCount = 3;

		for (int i = 0; i < _roleCount; i++)
		{
			std::string _roleNameStr = StringUtils::format("img_roleBg_00%d",i+1);

			std::string _selectNameStr = StringUtils::format("img_selectRole_00%d",i+1);

			ImageView* _roleImg = static_cast<ImageView*>(Helper::seekWidgetByName(m_roleSelectSvp,_roleNameStr.c_str()));

			ImageView* _selectImg = static_cast<ImageView*>(Helper::seekWidgetByName(_roleImg,_selectNameStr.c_str()));

			//_roleImg->addTouchEventListener(this, toucheventselector(RoleLayer::touchEventRoleImg));

			m_roleImgV->pushBack(_roleImg);

			m_selectRoleImgV->pushBack(_selectImg);
		}

		//role lock  (if not buy role , lock it)
		m_roleLock = static_cast<ImageView*>(Helper::seekWidgetByName(m_roleSelectSvp,"img_lock"));

		//prop lv up info
		Widget* t_lvUpInfoWidget = t_widget->getChildByName("pal_level")->getChildByName("img_levelBg");
		int t_propCount = 4;
		for (int count = 0; count < t_propCount; count++)
		{
			String* t_propStr = 0;
			switch (count)
			{
			case 0 :
				t_propStr = String::create("pal_magnet");
				break;
			case 1 :
				t_propStr = String::create("pal_spring");
				break;
			case 2 :
				t_propStr = String::create("pal_plane");
				break;
			case 3 :
				t_propStr = String::create("pal_bubble");
				break;
			default:
				break;
			}
			Widget* t_propPal = t_lvUpInfoWidget->getChildByName(t_propStr->getCString());
			prop_s t_propS;
			t_propS.infoText = static_cast<Text*>(t_propPal->getChildByName("lab_info"));
			t_propS.maxImg = static_cast<ImageView*>(t_propPal->getChildByName("img_pgbBg")->getChildByName("img_max"));
			t_propS.lvLoadingBar = static_cast<LoadingBar*>(t_propPal->getChildByName("img_pgbBg")->getChildByName("pgb_level"));
			m_propS.push_back(t_propS);
		}

        
        //Testing Label
		m_roleInfoText = static_cast<Text*>(t_lvUpInfoWidget->getChildByName("label_roleInfo"));
        m_roleInfoText->setColor(Color3B::BLACK);
        
		Widget* t_buffWidget = t_lvUpInfoWidget->getChildByName("pal_levelInfo")->getChildByName("img_levelInfoBg");
		m_scoreBuffText = static_cast<Text*>(t_buffWidget->getChildByName("lab_scoreBuff"));
		m_goldBuffText = static_cast<Text*>(t_buffWidget->getChildByName("lab_goldBuff"));
		m_propBuffText = static_cast<Text*>(t_buffWidget->getChildByName("lab_propBuff"));

		m_lvUpButton = static_cast<Button*>(t_lvUpInfoWidget->getChildByName("btn_levelUp"));
		if (0!=m_lvUpButton)
		{
			m_lvUpButton->addTouchEventListener(this, toucheventselector(RoleLayer::touchEventLvUp));
		}
		else
			log("~~error~~ level Up Button is Null ");

		m_lvUpCostText = static_cast<Text*>(m_lvUpButton->getChildByName("lab_levelUpCost"));
		//
		m_lvUpCostText->setVisible(true);

		//level up button image
		m_goldCostImg = static_cast<ImageView*>(m_lvUpButton->getChildByName("img_goldCost"));
		m_diamondCostImg = static_cast<ImageView*>(m_lvUpButton->getChildByName("img_diamondCost"));
		m_lvUpImg = static_cast<ImageView*>(m_lvUpButton->getChildByName("img_levelUp"));
		m_breakImg = static_cast<ImageView*>(m_lvUpButton->getChildByName("img_break"));
		m_buyImg = static_cast<ImageView*>(m_lvUpButton->getChildByName("img_buy"));

		//return
		m_returnBtn = static_cast<Button*>(t_widget->getChildByName("btn_return"));
		if (0!=m_returnBtn)
		{
			m_returnBtn->addTouchEventListener(this, toucheventselector(RoleLayer::touchEventReturn));
			//m_returnBtn->addTouchEventListener(this,toucheventselector(MainLayer::touchEventUpdate));
		}
		else
			log("~~error~~ return Button is Null ");

		//msg
		m_msgText = static_cast<Text*>(t_widget->getChildByName("img_msgBg")->getChildByName("svp_msg")->getChildByName("lab_msg"));

		//event listener shop layer return call-back
		auto listener = EventListenerCustom::create("Return_shopLayer", [this](EventCustom* event) {
			log("AAAAAAAAAAAAAAA shop layer");
			updateHp();
			updateGold();
			updateDiamond();
		});

		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

		bRet = true;
	}while(0);

	return bRet;
}

//**********************************touch event*******************************
//Assets
void RoleLayer::touchEventShopBtn(Ref *pSender, TouchEventType type)
{
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

//ScrollView
void RoleLayer::touchEventRoleSvp(Ref *pSender, TouchEventType type)
{
	Widget* _sender = static_cast<Widget*>(pSender);

	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	case TOUCH_EVENT_CANCELED:
		log("~~~~ScrollView!!");
		m_roleSelectSvp->updateOffset(0.5f);			//0.5f  update offset time
		m_frontRoleId = m_roleSelectSvp->getFrontRoleId();
		log("~~~~front Role Id : %d",m_frontRoleId);
		updateLevelUpInfo();

		//select role~~
		//Point _start = _sender->getTouchStartPos();

		//Point _end = _sender->getTouchEndPos();

		Point _gapPos = _sender->getTouchStartPos()-_sender->getTouchEndPos();

		if(abs(_gapPos.x)<10&&abs(_gapPos.y)<10)
		{
			Rect _imgRect = m_roleImgV->at(m_frontRoleId)->boundingBox();

			Point _worldPot = m_roleImgV->at(m_frontRoleId)->getWorldPosition();

			_imgRect.origin = ccp(_worldPot.x-_imgRect.size.width/2,_worldPot.y-_imgRect.size.height/2);

			//log("~~~~~~~~~~~~~~~~~~~ equal StartPos and EndPos");
			//log("~~~~~~~~~~~~~~~ worldpos : x=%f y=%f",m_roleImgV->at(m_frontRoleId)->getWorldPosition().x,m_roleImgV->at(m_frontRoleId)->getWorldPosition().y);
			//log("~~~~~~~~~~~~~~~ boundingBox : x=%f y=%f",_imgRect.origin.x,_imgRect.origin.y);
			//log("~~~~~~~~~~~~~~~ point : x=%f y=%f",_sender->getTouchEndPos().x,_sender->getTouchEndPos().y);


			//_imgRect.origin = ccp()

			if(_imgRect.containsPoint(_sender->getTouchEndPos()))
			{
				log("~~~~~~~~~~~~~~~~~~~ select role img ");
				if(m_frontRoleId<Player::getInstance()->getRoleList()->size())
				{
					for (int i = 0; i < m_selectRoleImgV->size(); i++)
					{
						if(m_frontRoleId==i)
						{
							m_selectRoleImgV->at(i)->setVisible(true);

							Player::getInstance()->setRoleType(Player::getInstance()->getRoleList()->at(i)->roleType);
						}
						else
							m_selectRoleImgV->at(i)->setVisible(false);

					}
				}
			}

		}

		break;
		//m_frontRoleId = m_roleSelectSvp->getFrontRoleId();
		//log("~~~~front Role Id : %d",m_frontRoleId);
		//updateLevelUpInfo();
		//break;
		//default:
		//	break;
	}
}

//level up Info
void RoleLayer::touchEventLvUp(Ref *pSender, TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		//log("~~~~Level Up!!");
		if(STATE_levelUp==m_buttonState)
		{
			//log("~~~roleType ~: %d",Player::getInstance()->getRoleList()->at(m_frontRoleId)->roleType);
			Network::getInstance()->do_UpRole_Req(Player::getInstance()->getRoleList()->at(m_frontRoleId)->roleType, [this](const AckInfo &ackInfo) 
			{
				std::string msgInfo = ackInfo.getMessageInfo();

				if (msgInfo != "")
				{
					log("[UpRole_Ack] %s", msgInfo.c_str());
				}
				else
				{
					//ok
					log("[UpRole] : level up success~!!");
					GameSound::playEffectMusic(EFF_RANK_UP);
					updateGold();
					updateDiamond();
					updateLevelUpInfo();
					updateRoleInfo();
				}
			});
		}
		else if(STATE_buy==m_buttonState)
		{
			int _roleType = -1;

			switch (m_frontRoleId)
			{
			case 0:
				_roleType = 1;
				break;
			case 1:
				_roleType = 4;
				break;
			default:
				break;
			}

			if(-1!=_roleType)
			{
				Network::getInstance()->do_BuyRole_Req(_roleType, [this](const AckInfo &ackInfo) 
				{
					std::string msgInfo = ackInfo.getMessageInfo();

					if (msgInfo != "")
					{
						log("[BuyRole_Ack] %s", msgInfo.c_str());
					}
					else
					{
						//ok
						GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);
						log("[BuyRole] : buy role success~!!");
						updateGold();
						updateDiamond();
						updateLevelUpInfo();
						updateRoleInfo();

						m_roleLock->setVisible(false);
					}
				});
			}
		}
		break;
	}
}

//return
void RoleLayer::touchEventReturn(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_CLOSE_PAGE);
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		log("~~~~Return!!");

		//listen event publish
		std::string eventName = "Return_roleLayer";
		EventCustom event(eventName);
		Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);

		int _chooseType = 1;

		for (int i = 0; i < m_selectRoleImgV->size(); i++)
		{
			if(m_selectRoleImgV->at(i)->isVisible())
			{
				Player::getInstance()->setLevel(Player::getInstance()->getRoleList()->at(i)->level);

				_chooseType = Player::getInstance()->getRoleList()->at(i)->roleType;

				Player::getInstance()->setRoleType(_chooseType);
			}
		}

		Network::getInstance()->do_ChooseRole_Req(_chooseType, [this](const AckInfo &ackInfo) 
		{
			std::string msgInfo = ackInfo.getMessageInfo();

			if (msgInfo != "")
			{
				log("[ChooseRole_Ack] %s", msgInfo.c_str());
			}
			else
			{
				//ok
				log("[ChooseRole] : Choose Role success~!!");

			}
		});

		this->removeFromParentAndCleanup(true);

		break;
	}
}

//void RoleLayer::touchEventRoleImg(Ref *pSender, TouchEventType type)
//{
//	switch (type)
//	{
//	case TOUCH_EVENT_ENDED:
//		log("~~~~role img!!");
//	default:
//		break;
//	}
//}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~touch event~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//**********************************schedule*******************************
void RoleLayer::update(float delayTime)
{
	//t_testUpdateTime += delayTime;
	//log("~~~update time : %f",t_testUpdateTime);

	Vector<Node*> t_children = m_roleSelectSvp->getChildren();

	for( auto& element : t_children)
	{
		ImageView* t_child = static_cast<ImageView*>(element);
		Point t_worldPoint = t_child->getWorldPosition();
		Point t_Point = t_child->getPosition();
		float t_midPointX = m_roleSelectSvp->getSize().width/2;
		float t_offset = abs(t_worldPoint.x-t_midPointX);
		float t_scale = 1-t_offset/gap_role;
		float t_scaleT = t_offset/gap_role;

		Color3B t_ccc3 = ccc3(120+135*t_scale,120+135*t_scale,120+135*t_scale);
		if(t_scale>0)
		{
			t_child->setScale(0.5f+t_scale/2);
			t_child->setPosition(ccp(t_Point.x,248-80*(1-t_scaleT*t_scaleT)));
			//color
			t_child->setColor(t_ccc3);
			t_child->getChildByName("img_role")->setColor(t_ccc3);

			if(t_scale>0.8f)
			{
				t_child->setZOrder(ZORDER_front);
				t_child->getChildByName("pal_roleInfo")->setZOrder(ZORDER_front+1);
			}
			else
			{
				t_child->setZOrder(ZORDER_behind);
				t_child->getChildByName("pal_roleInfo")->setZOrder(ZORDER_behind-1);
			}

		}
		else
		{
			t_child->setScale(0.5f);
			t_child->setPosition(ccp(t_Point.x,248));

			//color
			Color3B t_backColor = ccc3(120,120,120);
			t_child->setColor(t_backColor);
			t_child->getChildByName("img_role")->setColor(t_backColor);

			//Visible and ZOrder
			t_child->setZOrder(1);
			t_child->getChildByName("pal_roleInfo")->setZOrder(ZORDER_behind-1);
		}

	}
}

void RoleLayer::scheduleHpTime(float delayTime)
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
void RoleLayer::scheduleMsg(float delayTime)
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
void RoleLayer::updateHp()
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

void RoleLayer::updateDiamond()
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

void RoleLayer::updateGold()
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


//ScrollView 
void RoleLayer::updateRoleState()
{
	Vector<Node*> t_children = m_roleSelectSvp->getChildren();

	for( auto& element : t_children)
	{
		ImageView* t_child = static_cast<ImageView*>(element);
		Point t_worldPoint = t_child->getWorldPosition();
		float t_offset = abs(t_worldPoint.x-320.0f);
		float t_scale = 1-t_offset/gap_role;
		if(t_scale>0)
		{
			t_child->setScale(0.5f+t_scale/2);
		}
		else
		{
			t_child->setScale(0.5f);
		}

	}

	//test
}

//level up info
void RoleLayer::updateLevelUpInfo()
{
	int _level = 0;

	std::string _levelStr = "0";

	std::string _nextLevelStr = "1";

	//if(m_frontRoleId)
	if(m_frontRoleId<Player::getInstance()->getRoleList()->size())
	{
		_level = Player::getInstance()->getRoleList()->at(m_frontRoleId)->level;

		m_buttonState = STATE_levelUp;

		if (Player::getInstance()->getRoleList()->at(m_frontRoleId)->isMax)
		{
			m_lvUpButton->setVisible(false);
		}
		else
		{
			m_lvUpButton->setVisible(true);
		}

		_levelStr = StringUtils::format("%d",_level);

		_nextLevelStr = StringUtils::format("%d",_level+1);

		if(_level%10||0==_level)
		{
			m_lvUpImg->setVisible(true);
			m_breakImg->setVisible(false);
			m_buyImg->setVisible(false);
			m_goldCostImg->setVisible(true);
			m_diamondCostImg->setVisible(false);

			//std::string _costStr = StringUtils::format("%d",RoleLevelInfo::getInstance()->getLvUpCost(_level));
			//m_lvUpCostText->setText(_costStr);
		}
		else
		{
			if(Player::getInstance()->getRoleList()->at(m_frontRoleId)->breakType)
			{
				m_lvUpImg->setVisible(false);
				m_breakImg->setVisible(true);
				m_buyImg->setVisible(false);

				if(_level/10>1)
				{
					m_goldCostImg->setVisible(false);
					m_diamondCostImg->setVisible(true);
				}
				else
				{
					m_goldCostImg->setVisible(true);
					m_diamondCostImg->setVisible(false);
				}

				_nextLevelStr = StringUtils::format("%d.5",_level);

				BattleData::getInstance()->setIsBreak(true);
				//std::string _costStr = StringUtils::format("%d",RoleLevelInfo::getInstance()->getLvUpCost(_level));
				//m_lvUpCostText->setText(_costStr);
			}
			else
			{
				m_lvUpImg->setVisible(true);
				m_breakImg->setVisible(false);
				m_buyImg->setVisible(false);
				m_goldCostImg->setVisible(true);
				m_diamondCostImg->setVisible(false);

				_levelStr = StringUtils::format("%d.5",_level);

				BattleData::getInstance()->setIsBreak(false);
				//std::string _costStr = StringUtils::format("%d",RoleLevelInfo::getInstance()->getLvUpCost(_level));
				//m_lvUpCostText->setText(_costStr);
			}
		}

		std::string _costStr = StringUtils::format("%d",RoleLevelInfo::getInstance()->getLvUpCost(_levelStr));

		m_lvUpCostText->setText(_costStr);
	}
	else
	{
		m_lvUpButton->setVisible(true);

		_level = 0;
		m_lvUpImg->setVisible(false);
		m_breakImg->setVisible(false);
		m_buyImg->setVisible(true);

		switch (m_frontRoleId)
		{
		case 0:
			break;
		case 1:
			m_goldCostImg->setVisible(false);
			m_diamondCostImg->setVisible(true);
			m_lvUpCostText->setText("300");
			//role lock
			m_roleLock->setVisible(true);
			break;
		case 2:
			m_goldCostImg->setVisible(false);
			m_diamondCostImg->setVisible(true);
			m_lvUpCostText->setText("???");
			break;
		default:
			break;
		}

		m_buttonState = STATE_buy;

		

	}

	if(m_frontRoleId<2)
	{
		int _scoreBuff = static_cast<int>(RoleLevelInfo::getInstance()->getScoreExtra(_levelStr)*100);
		int _scoreBuffNext = static_cast<int>(RoleLevelInfo::getInstance()->getScoreExtra(_nextLevelStr)*100);
		std::string _scoreBuffStr = StringUtils::format("分数加成%d%%->%d%%",_scoreBuff,_scoreBuffNext);
		m_scoreBuffText->setText(_scoreBuffStr.c_str());

		int _goldBuff = static_cast<int>(RoleLevelInfo::getInstance()->getCoinExtra(_levelStr)*100);
		int _goldBuffNext = static_cast<int>(RoleLevelInfo::getInstance()->getCoinExtra(_nextLevelStr)*100);
		std::string _goldBuffStr = StringUtils::format("金币加成%d%%->%d%%",_goldBuff,_goldBuffNext);
		m_goldBuffText->setText(_goldBuffStr.c_str());

		m_propBuffText->setText(RoleLevelInfo::getInstance()->getEffect(_levelStr));

		//item info
		float _scaleSumMM = 0.0f;
		float _scaleSumSS = 0.0f;
		if(0==m_frontRoleId)
		{
			_scaleSumMM = 8.0f;
			_scaleSumSS = 7.0f;
		}
		else
		{
			_scaleSumMM = 10.0f;
			_scaleSumSS = 10.0f;
		}

		for (int i = 0; i < 4; i++)
		{
			std::string _infoStr = "";
			int _percent = 0;
			switch (i)
			{
			case 0:
				_infoStr = StringUtils::format("U型磁铁持续%0.1f秒",RoleLevelInfo::getInstance()->getMagnetTime(_levelStr));
				if(_level%4>0)
					_percent = 100*(static_cast<float>(_level/4+1)/_scaleSumMM);
				else
					_percent = 100*(static_cast<float>(_level/4)/_scaleSumMM);
				break;
			case 1:
				_infoStr = StringUtils::format("大型海蘑菇持续%0.1f秒",RoleLevelInfo::getInstance()->getShipTime(_levelStr));
				if(_level%4>1)
					_percent = 100*(static_cast<float>(_level/4+1)/_scaleSumMM);
				else
					_percent = 100*(static_cast<float>(_level/4)/_scaleSumMM);
				break;
			case 2:
				_infoStr = StringUtils::format("螺旋桨持续%0.1f秒",RoleLevelInfo::getInstance()->getHelicopterTime(_levelStr));
				if(_level%4>2)
					_percent = 100*(static_cast<float>(_level/4+1)/_scaleSumSS);
				else
					_percent = 100*(static_cast<float>(_level/4)/_scaleSumSS);
				break;
			case 3:
				_infoStr = StringUtils::format("气泡保护罩持续%0.1f秒",RoleLevelInfo::getInstance()->getShieldTime(_levelStr));
				_percent = 100*(static_cast<float>(_level/4)/_scaleSumSS);
				break;
			default:
				break;
			}
			m_propS.at(i).infoText->setText(_infoStr);

			if(_percent>100)
				_percent = 100;

			m_propS.at(i).lvLoadingBar->setPercent(_percent);

			if(100==_percent)
				m_propS.at(i).maxImg->setVisible(true);
			else
				m_propS.at(i).maxImg->setVisible(false);
		}
	}
	else if(2==m_frontRoleId)
	{
		m_scoreBuffText->setText("??????");
		m_goldBuffText->setText("??????");
		m_propBuffText->setText("??????");
		for (int i = 0; i < 4; i++)
		{
			m_propS.at(i).infoText->setText("??????");
			m_propS.at(i).lvLoadingBar->setPercent(0);
			m_propS.at(i).maxImg->setVisible(false);
		}
	}

    //m_roleInfoText->setColor(Color3B::BLACK);
    //m_roleInfoText->setFontName("Marker Felt");
    //m_roleInfoText->setFontSize(24);
    //m_roleInfoText->setBright(true);
    //m_roleInfoText->setBrightStyle(BrightStyle::BRIGHT_HIGHLIGHT);
	m_roleInfoText->setText(RoleLevelInfo::getInstance()->getRoleIntroduce(m_frontRoleId));
    //m_roleInfoLabel->setString(RoleLevelInfo::getInstance()->getRoleIntroduce(m_frontRoleId));
}

//role info
void RoleLayer::updateRoleInfo()
{

	int _roleCount = 3;

	for (int i = 0; i < _roleCount; i++)
	{

		if(i < Player::getInstance()->getRoleList()->size())
		{
			m_roleInfoS.at(i).lvNumText->setVisible(true);

			m_roleInfoS.at(i).lvLoadingBar->getParent()->setVisible(true);

			//change role image
			//std::string _imgStr = StringUtils::format("commonUi/imgRoleB_%d.png",Player::getInstance()->getRoleList()->at(i)->roleType);
			//m_roleInfoS.at(i).roleImg->loadTexture(_imgStr);

			m_roleInfoS.at(i).roleImg->setScale(SCALE_roleImgAdjust);

			int _level = Player::getInstance()->getRoleList()->at(i)->level;

			std::string _lvString = StringUtils::format("lv.%d",_level);

			m_roleInfoS.at(i).lvNumText->setText(_lvString);

			int _percent = 0;

			if(_level%10)
				_percent = (_level%10)*10;
			else
			{
				if(Player::getInstance()->getRoleList()->at(i)->breakType)
					_percent = 100;
				else
					_percent = 0;
			}

			m_roleInfoS.at(i).lvLoadingBar->setPercent(_percent);

		}
		else
		{
			//m_roleInfoS.at(i).roleInfoPal->setVisible(false);

			m_roleInfoS.at(i).lvNumText->setVisible(false);

			m_roleInfoS.at(i).lvLoadingBar->getParent()->setVisible(false);
		}
	}
}

//msg
void RoleLayer::updateMsg()
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
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~update~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~