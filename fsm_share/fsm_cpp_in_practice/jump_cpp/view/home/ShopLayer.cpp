#include "ShopLayer.h"
#include "Network.h"
#include "Player.h"
#include "util/GameSound.h"
#include "PlatformUtil.h"
#include "common/TextFloatLabelLayer.h"
//
#include "TipsInfo.h"

bool ShopLayer::init()
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

void ShopLayer::onEnter()
{
	Layer::onEnter();
	//update
	updateHp();
	updateDiamond();
	updateGold();


	//schedule
	this->schedule(schedule_selector(ShopLayer::scheduleHpTime),1.0f);
		//tips
	this->schedule(schedule_selector(ShopLayer::scheduleMsg),TIME_tipsScheduleDelay);
    
    
    
    // added by chris.li
    auto listener = EventListenerCustom::create("EVENT_BUY_DIAMOND_OK", [this](EventCustom* event)
    {
        std::string *productId = static_cast<std::string*>(event->getUserData());
        
        log("[ShopLayer]buy success: %s", productId->c_str());
        GameSound::playEffectMusic(EFF_GET_DIAMOND);
        int addDiamondNum = 0;
        
        if (*productId == "com.zjy.HiFly3.diamond60")
        {
            addDiamondNum = 60;
        }
        else if (*productId == "com.zjy.HiFly3.diamond330")
        {
            addDiamondNum = 330;
        }
        
        else if (*productId == "com.zjy.HiFly3.diamond600")
        {
            addDiamondNum = 600;
        }
        
        else if (*productId == "com.zjy.HiFly3.diamond1274")
        {
            addDiamondNum = 1274;
        }
        else if (*productId == "com.zjy.HiFly3.diamond2970")
        {
            addDiamondNum = 2970;
        }
        
        Player::getInstance()->setDiamond(Player::getInstance()->getDiamond() + addDiamondNum);
        
        this->updateDiamond();
        
        TextFloatLabelLayer::getInstance()->show("钻石购买成功");
    });
    _eventDispatcher->addEventListenerWithFixedPriority(listener, 1);
}

void ShopLayer::onExit()
{
	Layer::onExit();


	//schedule
	this->unschedule(schedule_selector(ShopLayer::scheduleHpTime));
	this->unschedule(schedule_selector(ShopLayer::scheduleMsg));

	//shop buy Listener
	_eventDispatcher->removeCustomEventListeners("EVENT_BUY_DIAMOND_OK");
}

//init
void ShopLayer::variableInit()
{
	m_hpLoadingBarV.clear();

	m_hpTimeText = 0;
	m_hpAddBtn = 0;
	m_diamondNumText = 0;
	m_diamondAddBtn = 0;
	m_goldNumText = 0;
	m_goldAddBtn = 0;

	m_diamondListBtn = 0;
	m_goldListBtn = 0;
	m_hpListBtn = 0;

	m_diamondListPal = 0;
	m_goldListPal = 0;
	m_hpListPal = 0;

	m_returnBtn = 0;

	//m_msgText = 0;

	m_Hptime = 0;
	m_HpNum = 0;

	//msg time 
	m_msgTime = 0.0f;
	m_msgSwitch = false;
	m_scrollSwitch = false;
	m_noticeSwitch = true;
	m_msgEndPosX = 0.0f;
}

bool ShopLayer::importUI()
{
	bool bRet = false;
	do{
		Widget* t_widget = GUIReader::getInstance()->widgetFromJsonFile("ShopUi/ShopUi.json");
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
			m_hpAddBtn->addTouchEventListener(this, toucheventselector(ShopLayer::touchEventShopBtn));
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
			m_diamondAddBtn->addTouchEventListener(this, toucheventselector(ShopLayer::touchEventShopBtn));
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
			m_goldAddBtn->addTouchEventListener(this, toucheventselector(ShopLayer::touchEventShopBtn));
		}
		else
			log("~~error~~ add Gold button is Null ");


		//return
		m_returnBtn = static_cast<Button*>(t_widget->getChildByName("btn_return"));
		if (m_returnBtn)
		{
			//m_returnBtn->setEnabled(false);
			m_returnBtn->addTouchEventListener(this, toucheventselector(ShopLayer::touchEventReturnBtn));
		}
		else
			log("~~error~~ Return button is Null ");

		m_diamondListBtn = static_cast<MyButton*>(Helper::seekWidgetByName(t_widget,"btn_diamondList"));
		if(m_diamondListBtn)
		{
			m_diamondListBtn->setTag(TAG_diamondListBtn);
			m_diamondListBtn->addTouchEventListener(this, toucheventselector(ShopLayer::touchEventListBtn));
		}
		else
			log("~~error~~ diamond list button is null");

		m_goldListBtn = static_cast<MyButton*>(Helper::seekWidgetByName(t_widget,"btn_goldList"));
		if(m_goldListBtn)
		{
			m_goldListBtn->setTag(TAG_goldListBtn);
			m_goldListBtn->addTouchEventListener(this, toucheventselector(ShopLayer::touchEventListBtn));
		}
		else
			log("~~error~~ gold list button is null");

		m_hpListBtn = static_cast<MyButton*>(Helper::seekWidgetByName(t_widget,"btn_hpList"));
		if(m_hpListBtn)
		{
			m_hpListBtn->setTag(TAG_hpListBtn);
			m_hpListBtn->addTouchEventListener(this, toucheventselector(ShopLayer::touchEventListBtn));
		}
		else
			log("~~error~~ Hp list button is null");

		m_diamondListPal = static_cast<Layout*>(Helper::seekWidgetByName(t_widget,"pal_diamondList"));
		//diamond buy button
		setWidgetButton(m_diamondListPal,"btn_diamond6",TAG_diamondBuy6,toucheventselector(ShopLayer::touchEventDiamondBuyBtn));
		setWidgetButton(m_diamondListPal,"btn_diamond30",TAG_diamondBuy30,toucheventselector(ShopLayer::touchEventDiamondBuyBtn));
		setWidgetButton(m_diamondListPal,"btn_diamond50",TAG_diamondBuy50,toucheventselector(ShopLayer::touchEventDiamondBuyBtn));
		setWidgetButton(m_diamondListPal,"btn_diamond98",TAG_diamondBuy98,toucheventselector(ShopLayer::touchEventDiamondBuyBtn));
		setWidgetButton(m_diamondListPal,"btn_diamond198",TAG_diamondBuy198,toucheventselector(ShopLayer::touchEventDiamondBuyBtn));

		m_goldListPal = static_cast<Layout*>(Helper::seekWidgetByName(t_widget,"pal_goldList"));
		//gold buy button
		setWidgetButton(m_goldListPal,"btn_gold50",TAG_goldBuy50,toucheventselector(ShopLayer::touchEventGoldBuyBtn));
		setWidgetButton(m_goldListPal,"btn_gold150",TAG_goldBuy150,toucheventselector(ShopLayer::touchEventGoldBuyBtn));
		setWidgetButton(m_goldListPal,"btn_gold400",TAG_goldBuy400,toucheventselector(ShopLayer::touchEventGoldBuyBtn));
		setWidgetButton(m_goldListPal,"btn_gold1000",TAG_goldBuy1000,toucheventselector(ShopLayer::touchEventGoldBuyBtn));
		setWidgetButton(m_goldListPal,"btn_gold2000",TAG_goldBuy2000,toucheventselector(ShopLayer::touchEventGoldBuyBtn));

		m_hpListPal = static_cast<Layout*>(Helper::seekWidgetByName(t_widget,"pal_hpList"));
		//hp buy button
		setWidgetButton(m_hpListPal,"btn_hp20",TAG_hpBuy20,toucheventselector(ShopLayer::touchEventHpBuyBtn));
		setWidgetButton(m_hpListPal,"btn_hp80",TAG_hpBuy80,toucheventselector(ShopLayer::touchEventHpBuyBtn));
		setWidgetButton(m_hpListPal,"btn_hp200",TAG_hpBuy200,toucheventselector(ShopLayer::touchEventHpBuyBtn));
		setWidgetButton(m_hpListPal,"btn_hp600",TAG_hpBuy600,toucheventselector(ShopLayer::touchEventHpBuyBtn));
		setWidgetButton(m_hpListPal,"btn_hp1000",TAG_hpBuy1000,toucheventselector(ShopLayer::touchEventHpBuyBtn));
		
		//Msg text
		m_msgText = static_cast<Text*>(t_widget->getChildByName("img_msgBg")->getChildByName("svp_msg")->getChildByName("lab_msg"));

		bRet = true;
	}while(0);

	return bRet;
}

//**********************************touch event*******************************
//~~~Button~~~ 
//Assets
void ShopLayer::touchEventShopBtn(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);

	Button* t_senderBtn = static_cast<Button*>(pSender);
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		log("~~~~~enter shop!!");
		switch (t_senderBtn->getTag())
		{
		case TAG_shopHpBtn:
			//Button controll
			m_hpListBtn->setLocalZOrder(ZORDER_front);
			m_diamondListBtn->setLocalZOrder(ZORDER_behind);
			m_goldListBtn->setLocalZOrder(ZORDER_behind);
			m_hpListBtn->setOnPressEnable(false);
			m_diamondListBtn->setOnPressEnable(true);
			m_goldListBtn->setOnPressEnable(true);
			//list panel controll
			m_diamondListPal->setVisible(false);
			m_goldListPal->setVisible(false);
			m_hpListPal->setVisible(true);
			break;
		case TAG_shopDiamondBtn:
			//Button controll
			m_hpListBtn->setLocalZOrder(ZORDER_behind);
			m_diamondListBtn->setLocalZOrder(ZORDER_front);
			m_goldListBtn->setLocalZOrder(ZORDER_behind);
			m_hpListBtn->setOnPressEnable(true);
			m_diamondListBtn->setOnPressEnable(false);
			m_goldListBtn->setOnPressEnable(true);
			//list panel controll
			m_diamondListPal->setVisible(true);
			m_goldListPal->setVisible(false);
			m_hpListPal->setVisible(false);
			break;
		case TAG_shopGoldBtn:
			//Button controll
			m_hpListBtn->setLocalZOrder(ZORDER_behind);
			m_diamondListBtn->setLocalZOrder(ZORDER_behind);
			m_goldListBtn->setLocalZOrder(ZORDER_front);
			m_hpListBtn->setOnPressEnable(true);
			m_diamondListBtn->setOnPressEnable(true);
			m_goldListBtn->setOnPressEnable(false);
			//list panel controll
			m_diamondListPal->setVisible(false);
			m_goldListPal->setVisible(true);
			m_hpListPal->setVisible(false);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

//void ShopLayer::touchEventAddHp(Ref *pSender, TouchEventType type)
//{
//	switch (type)
//	{
//        case TOUCH_EVENT_ENDED:
//            log("~~~~~enter HP shop!!");
//			//m_friendLvp->setDirection(SCROLLVIEW_DIR_HORIZONTAL);
//            break;
//        default:
//            break;
//	}
//}
//
//void ShopLayer::touchEventAddDiamond(Ref *pSender, TouchEventType type) 
//{
//	switch (type)
//	{
//        case TOUCH_EVENT_ENDED:
//            log("~~~~~enter Diamond shop!!");
//            break;
//        default:
//            break;
//	} 
//}
//
//void ShopLayer::touchEventAddGold(Ref *pSender, TouchEventType type)
//{
//	switch (type)
//	{
//        case TOUCH_EVENT_ENDED:
//            log("~~~~~enter Gold shop!!");
//            break;
//        default:
//            break;
//	}
//}

//shop list button
void ShopLayer::touchEventListBtn(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);

	MyButton* t_senderBtn = static_cast<MyButton*>(pSender);
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		log("~~~~~list button ");
		switch (t_senderBtn->getTag())
		{
		case TAG_diamondListBtn:
			//Button controll
			m_goldListBtn->setLocalZOrder(ZORDER_behind);
			m_hpListBtn->setLocalZOrder(ZORDER_behind);
			m_goldListBtn->setOnPressEnable(true);
			m_hpListBtn->setOnPressEnable(true);
			//list panel controll
			m_diamondListPal->setVisible(true);
			m_goldListPal->setVisible(false);
			m_hpListPal->setVisible(false);
			break;
		case TAG_goldListBtn:
			//Button controll
			m_diamondListBtn->setLocalZOrder(ZORDER_behind);
			m_hpListBtn->setLocalZOrder(ZORDER_behind);
			m_diamondListBtn->setOnPressEnable(true);
			m_hpListBtn->setOnPressEnable(true);
			//list panel controll
			m_diamondListPal->setVisible(false);
			m_goldListPal->setVisible(true);
			m_hpListPal->setVisible(false);
			break;
		case TAG_hpListBtn:
			//Button controll
			m_diamondListBtn->setLocalZOrder(ZORDER_behind);
			m_goldListBtn->setLocalZOrder(ZORDER_behind);
			m_diamondListBtn->setOnPressEnable(true);
			m_goldListBtn->setOnPressEnable(true);
			//list panel controll
			m_diamondListPal->setVisible(false);
			m_goldListPal->setVisible(false);
			m_hpListPal->setVisible(true);
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

//shop buy button
void ShopLayer::touchEventDiamondBuyBtn(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);

	Button* t_senderBtn = static_cast<Button*>(pSender);
    
    int buyType = 1;
	if (type == TOUCH_EVENT_ENDED)
    {
        log("~~~~~buy button : %d",t_senderBtn->getTag());
		switch (t_senderBtn->getTag())
		{
            case TAG_diamondBuy6:
                log("TAG_diamondBuy6");
                buyType = 1;
                break;
            case TAG_diamondBuy30:
                log("TAG_diamondBuy30");
                buyType = 2;
                break;
            case TAG_diamondBuy50:
                log("TAG_diamondBuy50");
                buyType = 3;
                break;
            case TAG_diamondBuy98:
                log("TAG_diamondBuy98");
                buyType = 4;
                break;
            case TAG_diamondBuy198:
                log("TAG_diamondBuy198");
                buyType = 5;
                break;
            default:
                break;
		}
        
        PlatformUtil::IOS_buyProduct(buyType);
    }
}

void ShopLayer::touchEventGoldBuyBtn(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);

	Button* t_senderBtn = static_cast<Button*>(pSender);
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		log("~~~~~buy button : %d",t_senderBtn->getTag());
		switch (t_senderBtn->getTag())
		{
		case TAG_goldBuy50:
			log("TAG_goldBuy50");
			shopBuyGold(2,1);
			break;
		case TAG_goldBuy150:
			log("TAG_goldBuy150");
			shopBuyGold(2,2);
			break;
		case TAG_goldBuy400:
			log("TAG_goldBuy400");
			shopBuyGold(2,3);
			break;
		case TAG_goldBuy1000:
			log("TAG_goldBuy1000");
			shopBuyGold(2,4);
			break;
		case TAG_goldBuy2000:
			log("TAG_goldBuy2000");
			shopBuyGold(2,5);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}
void ShopLayer::touchEventHpBuyBtn(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);

	Button* t_senderBtn = static_cast<Button*>(pSender);
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		log("~~~~~buy button : %d",t_senderBtn->getTag());
		switch (t_senderBtn->getTag())
		{
		case TAG_hpBuy20:
			log("TAG_hpBuy20");
			shopBuyHp(1,1);
			break;
		case TAG_hpBuy80:
			log("TAG_hpBuy80");
			shopBuyHp(1,2);
			break;
		case TAG_hpBuy200:
			log("TAG_hpBuy200");
			shopBuyHp(1,3);
			break;
		case TAG_hpBuy600:
			log("TAG_hpBuy600");
			shopBuyHp(1,4);
			break;
		case TAG_hpBuy1000:
			log("TAG_hpBuy1000");
			shopBuyHp(1,5);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

//return
void ShopLayer::touchEventReturnBtn(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_CLOSE_PAGE);

	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		log("~~~~~ShopLayer return!!");

		std::string eventName = "Return_shopLayer";
		EventCustom event(eventName);
		Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);

		this->removeFromParentAndCleanup(true);
		break;
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~touch event~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//**********************************update*******************************
//assets
void ShopLayer::updateHp()
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

void ShopLayer::updateDiamond()
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

void ShopLayer::updateGold()
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

//msg
void ShopLayer::updateMsg()
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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~update~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//**********************************schedule*******************************
void ShopLayer::scheduleHpTime(float delayTime)
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
void ShopLayer::scheduleMsg(float delayTime)
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

//************************************helper*************************************************
void ShopLayer::setWidgetButton(Widget* root, const char *name,int tag, SEL_TouchEvent selector)
{
	Button* t_btn = 0;
	t_btn = static_cast<Button*>(Helper::seekWidgetByName(root,name));
	if(t_btn)
	{
		t_btn->setTag(tag);
		t_btn->addTouchEventListener(this,selector);
	}
	else
		log("helper widget button is null");
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~helper~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//
void ShopLayer::initShopType(ShopType type)
{
	switch (type)
	{
	case TYPE_shopDiamond:
		m_diamondListBtn->setLocalZOrder(ZORDER_front);
		m_diamondListBtn->setOnPressEnable(false);
		m_diamondListPal->setVisible(true);
		break;
	case TYPE_shopGold:
		m_goldListBtn->setLocalZOrder(ZORDER_front);
		m_goldListBtn->setOnPressEnable(false);
		m_goldListPal->setVisible(true);
		break;
	case TYPE_shopHp:
		m_hpListBtn->setLocalZOrder(ZORDER_front);
		m_hpListBtn->setOnPressEnable(false);
		m_hpListPal->setVisible(true);
		break;
	default:
		break;
	}
}

//gold buy
void ShopLayer::shopBuyGold(int type,int goodsId)
{
	Network::getInstance()->do_ShopBuy_Req(type,goodsId,[this](const AckInfo &ackInfo) 
	{
		std::string msgInfo = ackInfo.getMessageInfo();

		if (msgInfo != "")
		{
			log("[GoldBuy] %s", msgInfo.c_str());
		}
		else
		{
			//ok
			GameSound::playEffectMusic(EFF_GET_GOLD);
			GameSound::playEffectMusic(EFF_USE_DIAMOND);
			updateDiamond();
			updateGold();
			//
			TextFloatLabelLayer::getInstance()->show(TipsInfo::getInstance()->getMessage("buy_success"));
		}
	});
}

//Hp buy
void ShopLayer::shopBuyHp(int type,int goodsId)
{
	Network::getInstance()->do_ShopBuy_Req(type,goodsId,[this](const AckInfo &ackInfo) 
	{
		std::string msgInfo = ackInfo.getMessageInfo();

		if (msgInfo != "")
		{
			log("[HpBuy] %s", msgInfo.c_str());
		}
		else
		{
			//ok
			GameSound::playEffectMusic(EFF_USE_DIAMOND);
			updateDiamond();
			if(Player::getInstance()->getStar()>=5)
			{
				Player::getInstance()->setRemainTime(-1);
			}
			updateHp();
			//Network::getInstance()->do_UpHeart_Req([this](const AckInfo &ackInfo){});
			TextFloatLabelLayer::getInstance()->show(TipsInfo::getInstance()->getMessage("buy_success"));
		}
	});
}