#include "RankLayer.h"
#include "ShopLayer.h"
#include "UiConfig.h"

#include "MyHelper.h"
#include "util/GameSound.h"
//
#include "TipsInfo.h"

bool RankLayer::init()
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

void RankLayer::onEnter()
{
	Layer::onEnter();

	//update
	updateHp();
	updateDiamond();
	updateGold();

	//get rank list 
	//Network::getInstance()->do_RankList_Req(1,2);

	//rank list page  *front rank list update
	//updateRankPageCount();
	//listview
	m_worldRankLvp->setVisible(true);

	//schedule
	this->schedule(schedule_selector(RankLayer::scheduleHpTime),1.0f);
	this->scheduleUpdate();
	//tips
	this->schedule(schedule_selector(RankLayer::scheduleMsg),TIME_tipsScheduleDelay);

	//
	if(0==Player::getInstance()->getWorldRankList()->size())
	{
		Network::getInstance()->do_RankList_Req(2,2,[this](const AckInfo &ackInfo) 
		{
			std::string msgInfo = ackInfo.getMessageInfo();

			if (msgInfo != "")
			{
				log("[RankList] %s", msgInfo.c_str());
			}
			else
			{
				//ok
				log("[RankList] : success~~!!");
				updateRankLvp(RANK_world);
			}
		});
	}
	else
	{
		updateRankLvp(RANK_world);
	}

}

void RankLayer::onExit()
{
	Layer::onExit();

	//schedule
	this->unscheduleUpdate();
	this->unschedule(schedule_selector(RankLayer::scheduleHpTime));

	//remove rank list listener
	//_eventDispatcher->removeCustomEventListeners("RankList_Ack");
}

//init
void RankLayer::variableInit()
{
	m_hpLoadingBarV.clear();

	m_hpTimeText = 0;
	m_hpAddBtn = 0;
	m_diamondNumText = 0;
	m_diamondAddBtn = 0;
	m_goldNumText = 0;
	m_goldAddBtn = 0;

	m_lvpSlider = 0;
	m_platformRankBtn = 0;
	m_worldRankBtn = 0;
	m_GCBtn = 0;

	m_rankSvp = 0;

	m_platformRankLvp = 0;
	m_worldRankLvp = 0;
	m_GCLvp = 0;

	m_rankSinglePal = 0;
	m_rankDualPal = 0;

	m_page = 0;
	m_pageSum = 0;
	m_page = 0;
	m_pageSum = 0;
	//
	m_page = 1;
	m_pageSum = 0;
	//m_rankListPage = 1;
	//m_rankListPageSum = 0;

	m_playerInfoPal = 0;

	m_playerHeadImg = 0;
	m_scoreText = 0;
	m_levelsText = 0;
	m_infoReturnBtn = 0;

	m_returnBtn = 0;

	m_msgText = nullptr;

	m_Hptime = 0;
	m_HpNum = 0;

	m_touchSwitch = true;
	m_sliderSwitch = false;

	m_sliderGapMax = 0;
	m_sliderGap = 0;
	//msg time 
	m_msgTime = 0.0f;
	m_msgSwitch = false;
	m_scrollSwitch = false;
	m_noticeSwitch = true;
	m_msgEndPosX = 0.0f;

}

bool RankLayer::importUI()
{
	bool bRet = false;
	do{
		Widget* t_widget = GUIReader::getInstance()->widgetFromJsonFile("RankUi/RankUi.json");
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
		//

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
			m_hpAddBtn->addTouchEventListener(this, toucheventselector(RankLayer::touchEventShopBtn));
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
			m_diamondAddBtn->addTouchEventListener(this, toucheventselector(RankLayer::touchEventShopBtn));
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
			m_goldAddBtn->addTouchEventListener(this, toucheventselector(RankLayer::touchEventShopBtn));
		}
		else
			log("~~error~~ add Gold button is Null ");

		//rank~~~
		Widget* t_rankFrameWidget = t_widget->getChildByName("pal_rankFrame");

		m_lvpSlider = static_cast<Slider*>(t_rankFrameWidget->getChildByName("sdr_rank"));
		m_lvpSlider->setTouchEnabled(false);
		m_lvpSlider->setVisible(false);

		Widget* t_rankBtnWidget = t_rankFrameWidget->getChildByName("img_rankFrameBg");

		m_platformRankBtn = static_cast<MyButton*>(t_rankBtnWidget->getChildByName("btn_platformRank"));
		if (m_platformRankBtn)
		{
			m_platformRankBtn->setEnabled(false);

			m_platformRankBtn->setTag(TAG_platformRankBtn);
			m_platformRankBtn->addTouchEventListener(this, toucheventselector(RankLayer::touchEventRankBtn));
		}
		else
			log("~~error~~ Platform rank button is Null ");

		m_worldRankBtn = static_cast<MyButton*>(t_rankBtnWidget->getChildByName("btn_worldRank"));
		if (m_worldRankBtn)
		{
			//init
			m_worldRankBtn->setLocalZOrder(ZORDER_front);
			m_worldRankBtn->setOnPressEnable(false);

			m_worldRankBtn->setTag(TAG_worldRankBtn);
			m_worldRankBtn->addTouchEventListener(this, toucheventselector(RankLayer::touchEventRankBtn));
		}
		else
			log("~~error~~ World rank button is Null ");

		m_GCBtn = static_cast<MyButton*>(t_rankBtnWidget->getChildByName("btn_GCRank"));
		if (m_GCBtn)
		{
			m_GCBtn->setEnabled(false);

			m_GCBtn->setTag(TAG_GCBtn);
			m_GCBtn->addTouchEventListener(this, toucheventselector(RankLayer::touchEventRankBtn));
		}
		else
			log("~~error~~ Game Center button is Null ");

		m_rankSvp = static_cast<ScrollView*>(t_rankFrameWidget->getChildByName("svp_rank"));

		m_platformRankLvp = static_cast<MyListView*>(m_rankSvp->getChildByName("lvp_platformRank"));

		m_worldRankLvp = static_cast<MyListView*>(m_rankSvp->getChildByName("lvp_worldRank"));

		m_GCLvp = static_cast<MyListView*>(m_rankSvp->getChildByName("lvp_GC"));

		m_platformRankLvp->setTag(TAG_platformRankLvp);
		m_platformRankLvp->addTouchEventListener(this, toucheventselector(RankLayer::listenEventRankLvp));

		m_worldRankLvp->setLocalZOrder(ZORDER_front);
		m_worldRankLvp->setTag(TAG_worldRankLvp);
		m_worldRankLvp->addTouchEventListener(this, toucheventselector(RankLayer::listenEventRankLvp));

		m_GCLvp->setTag(TAG_GCLvp);
		m_GCLvp->addTouchEventListener(this, toucheventselector(RankLayer::listenEventRankLvp));

		//model
		m_rankSinglePal = static_cast<Layout*>(t_widget->getChildByName("pal_rankSingle"));
		m_rankDualPal = static_cast<Layout*>(t_widget->getChildByName("pal_rankDual"));

		//friend info
		m_playerInfoPal = static_cast<Layout*>(t_widget->getChildByName("pal_palyerInfo"));
		m_playerInfoPal->setVisible(true);
		m_playerInfoPal->setEnabled(false);

		m_infoReturnBtn = static_cast<Button*>(m_playerInfoPal->getChildByName("img_infoFrame")->getChildByName("btn_return"));
		if(m_infoReturnBtn)
		{
			m_infoReturnBtn->addTouchEventListener(this,toucheventselector(RankLayer::touchEventInfoReturnBtn));
		}


		Widget* t_infoFrameWdt = m_playerInfoPal->getChildByName("img_infoFrame");

		m_playerHeadImg = static_cast<ImageView*>(t_infoFrameWdt->getChildByName("img_palyerHead"));

		m_scoreText = static_cast<TextAtlas*>(t_infoFrameWdt->getChildByName("labAls_score"));

		m_levelsText = static_cast<TextAtlas*>(t_infoFrameWdt->getChildByName("labAls_levels"));

		//Msg text
		m_msgText = static_cast<Text*>(t_widget->getChildByName("img_msgBg")->getChildByName("svp_msg")->getChildByName("lab_msg"));

		//return
		m_returnBtn = static_cast<Button*>(t_widget->getChildByName("btn_return"));
		if (m_returnBtn)
		{
			//m_returnBtn->setEnabled(false);
			m_returnBtn->addTouchEventListener(this, toucheventselector(RankLayer::touchEventReturnBtn));
		}
		else
			log("~~error~~ Return button is Null ");

		//************* event listener ************************
		//event listener shop layer return call-back
		auto _listenerShop = EventListenerCustom::create("Return_shopLayer", [this](EventCustom* event) {
			log("AAAAAAAAAAAAAAA rank layer");
			updateHp();
			updateGold();
			updateDiamond();
		});

		bRet = true;
	}while(0);

	return bRet;
}

//**********************************touch event*******************************
//~~~Button~~~ 
//Assets
void RankLayer::touchEventShopBtn(Ref *pSender, TouchEventType type)
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

//rank button
void RankLayer::touchEventRankBtn(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);

	MyButton* t_senderBtn = static_cast<MyButton*>(pSender);
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		switch (t_senderBtn->getTag())
		{
		case TAG_platformRankBtn:
			log("TAG_platformRankBtn");
			//Button controll
			m_worldRankBtn->setLocalZOrder(ZORDER_behind);
			m_GCBtn->setLocalZOrder(ZORDER_behind);
			m_worldRankBtn->setOnPressEnable(true);
			m_GCBtn->setOnPressEnable(true);
			//listView controll
			//m_platformRankLvp->setLocalZOrder(ZORDER_front);
			m_platformRankLvp->setVisible(true);
			//m_worldRankLvp->setLocalZOrder(ZORDER_behind);
			m_worldRankLvp->setVisible(false);
			//m_GCLvp->setLocalZOrder(ZORDER_behind);
			m_GCLvp->setVisible(false);

			m_page = 1;
			updateRankLvp(RANK_platform);
			break;
		case TAG_worldRankBtn:
			log("TAG_worldRankBtn");
			if(0==Player::getInstance()->getWorldRankList()->size())
			{
				Network::getInstance()->do_RankList_Req(2,2,[this](const AckInfo &ackInfo) 
				{
					std::string msgInfo = ackInfo.getMessageInfo();

					if (msgInfo != "")
					{
						log("[RankList] %s", msgInfo.c_str());
					}
					else
					{
						//ok
						log("[RankList] : success~~!!");
						updateRankLvp(RANK_world);

						//Button controll
						m_platformRankBtn->setLocalZOrder(ZORDER_behind);
						m_GCBtn->setLocalZOrder(ZORDER_behind);
						m_platformRankBtn->setOnPressEnable(true);
						m_GCBtn->setOnPressEnable(true);
						//listView controll
						//m_platformRankLvp->setLocalZOrder(ZORDER_behind);
						m_platformRankLvp->setVisible(false);
						//m_worldRankLvp->setLocalZOrder(ZORDER_front);
						m_worldRankLvp->setVisible(true);
						//m_GCLvp->setLocalZOrder(ZORDER_behind);
						m_GCLvp->setVisible(false);

						m_page = 1;
						updateRankLvp(RANK_world);
					}
				});
			}
			else
			{
				//Button controll
				m_platformRankBtn->setLocalZOrder(ZORDER_behind);
				m_GCBtn->setLocalZOrder(ZORDER_behind);
				m_platformRankBtn->setOnPressEnable(true);
				m_GCBtn->setOnPressEnable(true);
				//listView controll
				//m_platformRankLvp->setLocalZOrder(ZORDER_behind);
				m_platformRankLvp->setVisible(false);
				//m_worldRankLvp->setLocalZOrder(ZORDER_front);
				m_worldRankLvp->setVisible(true);
				//m_GCLvp->setLocalZOrder(ZORDER_behind);
				m_GCLvp->setVisible(false);

				m_page = 1;
				updateRankLvp(RANK_world);
			}
			break;
		case TAG_GCBtn:
			log("TAG_GCBtn");
			//Button controll
			m_platformRankBtn->setLocalZOrder(ZORDER_behind);
			m_worldRankBtn->setLocalZOrder(ZORDER_behind);
			m_platformRankBtn->setOnPressEnable(true);
			m_worldRankBtn->setOnPressEnable(true);
			//listView controll
			//m_platformRankLvp->setLocalZOrder(ZORDER_behind);
			m_platformRankLvp->setVisible(false);
			//m_worldRankLvp->setLocalZOrder(ZORDER_behind);
			m_worldRankLvp->setVisible(false);
			//m_GCLvp->setLocalZOrder(ZORDER_front);
			m_GCLvp->setVisible(true);

			//m_platformFriendPage = 1;
			//updatePlatformFriend();
			break;
		default:
			break;
		}
		t_senderBtn->setLocalZOrder(ZORDER_front);
		t_senderBtn->setOnPressEnable(false);
		//t_senderBtn->setTouchEnabled(false);
		//t_senderBtn->setEnabled(false);
		break;
	default:
		break;
	}
}

//return
void RankLayer::touchEventReturnBtn(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_CLOSE_PAGE);

	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		log("~~~~~RankLayer return!!");

		this->removeFromParentAndCleanup(true);
		break;
	default:
		break;
	}
}

void RankLayer::touchEventInfoReturnBtn(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_CLOSE_PAGE);

	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		log("~~~~~player info return!!");
		m_playerInfoPal->setEnabled(false);
		break;
	default:
		break;
	}
}

//pal touch
void RankLayer::listenEventRankLvp(Ref *pSender, TouchEventType type)
{
	Widget* t_sender = static_cast<Widget*>(pSender);

	Rect t_rect;

	switch (type)
	{
	case TOUCH_EVENT_BEGAN:
		break;
	case TOUCH_EVENT_MOVED:
		//list touch choose HORIZONTAL or VERTICAL
		if(m_touchSwitch)
		{
			if(abs(t_sender->getTouchMovePos().x-t_sender->getTouchStartPos().x)>abs(t_sender->getTouchMovePos().y-t_sender->getTouchStartPos().y))
			{
				switch (t_sender->getTag())
				{
				case TAG_platformRankLvp:
					m_platformRankLvp->setDirectionParent(SCROLLVIEW_DIR_NONE);
					break;
				case TAG_worldRankLvp:
					m_worldRankLvp->setDirectionParent(SCROLLVIEW_DIR_NONE);
					break;
				default:
					break;
				}
				m_rankSvp->setDirection(SCROLLVIEW_DIR_HORIZONTAL);
			}
			else
			{
				//show Slider 
				if(m_sliderSwitch)
					m_lvpSlider->setVisible(true);


				switch (t_sender->getTag())
				{
				case TAG_platformRankLvp:
					m_platformRankLvp->setDirectionParent(SCROLLVIEW_DIR_VERTICAL);
					break;
				case TAG_worldRankLvp:
					m_worldRankLvp->setDirectionParent(SCROLLVIEW_DIR_VERTICAL);
					break;
				default:
					break;
				}
				m_rankSvp->setDirection(SCROLLVIEW_DIR_NONE);
			}
			m_touchSwitch = false;
		}
		//Slider gap
		switch (t_sender->getTag())
		{
		case TAG_platformRankLvp:
			m_sliderGap = m_sliderGapMax + m_platformRankLvp->getInnerContainer()->getPosition().y;
			break;
		case TAG_worldRankLvp:
			m_sliderGap = m_sliderGapMax + m_worldRankLvp->getInnerContainer()->getPosition().y;
			break;
		default:
			break;
		}
		break;

	case TOUCH_EVENT_ENDED:
		m_touchSwitch = true;
		log("~~~~touch ended!!");
		if(TAG_platformRankLvp==t_sender->getTag())
		{
			Rect t_rect;
			for( auto& item : m_platformRankLvp->getItems())
			{
				t_rect.origin = item->getWorldPosition();
				t_rect.size = SIZE_friendItem;

				if(t_rect.containsPoint(t_sender->getTouchEndPos()))
				{
					log("enter info layout for tag : %d ",item->getTag());

					int t = item->getTag();

					int _playerId = Player::getInstance()->getPlatformRankList()->at(item->getTag()-TAG_platformRankPal)->playerId;
					//int _playerId = 0;			//CW

					Network::getInstance()->do_FriendInfo_Req(_playerId,[this](const AckInfo &ackInfo) 
					{
						std::string msgInfo = ackInfo.getMessageInfo();

						if (msgInfo != "")
						{
							log("[FriendInfo] %s", msgInfo.c_str());
						}
						else
						{
							//ok
							updatePlayerInfoPal(TAG_platformRankPal);
							m_playerInfoPal->setEnabled(true);
						}
					});

					//updatePlayerInfoPal(TAG_platformRankPal);
					//m_playerInfoPal->setEnabled(true);
					break;
				}
			}
		}
		else if(TAG_worldRankLvp==t_sender->getTag())
		{
			Rect t_rect;
			for( auto& item : m_worldRankLvp->getItems())
			{
				t_rect.origin = item->getWorldPosition();
				t_rect.size = SIZE_friendItem;

				if(t_rect.containsPoint(t_sender->getTouchEndPos()))
				{
					log("enter info layout for tag : %d ",item->getTag());

					int _playerId = Player::getInstance()->getWorldRankList()->at(item->getTag()-TAG_worldRankPal)->playerId;
					//int _playerId = 0;			//CW

					Network::getInstance()->do_FriendInfo_Req(_playerId,[this](const AckInfo &ackInfo) 
					{
						std::string msgInfo = ackInfo.getMessageInfo();

						if (msgInfo != "")
						{
							log("[FriendInfo] %s", msgInfo.c_str());
						}
						else
						{
							//ok
							updatePlayerInfoPal(TAG_worldRankPal);
							m_playerInfoPal->setEnabled(true);
						}
					});

					//updatePlayerInfoPal(TAG_worldRankPal);
					//m_playerInfoPal->setEnabled(true);
					break;
				}
			}
		}

		break;

	case TOUCH_EVENT_CANCELED:
		m_lvpSlider->setVisible(false);

		m_touchSwitch = true;

		switch (t_sender->getTag())
		{
		case TAG_platformRankLvp:
			if(m_platformRankLvp->getWorldPosition().x-m_rankSvp->getWorldPosition().x>GAP_turnPage&&m_page>1)
			{
				m_page--;
				updateRankLvp(RANK_platform);
			}
			else if(m_platformRankLvp->getWorldPosition().x-m_rankSvp->getWorldPosition().x<-GAP_turnPage&&m_page<m_pageSum)
			{
				m_page++;
				updateRankLvp(RANK_platform);
			}
			break;
		case TAG_worldRankLvp:
			if(m_worldRankLvp->getWorldPosition().x-m_rankSvp->getWorldPosition().x>GAP_turnPage&&m_page>1)
			{
				m_page--;
				updateRankLvp(RANK_world);
			}
			else if(m_worldRankLvp->getWorldPosition().x-m_rankSvp->getWorldPosition().x<-GAP_turnPage&&m_page<m_pageSum)
			{
				m_page++;
				updateRankLvp(RANK_world);
			}
			break;
			//case TAG_platformFriendLvp:
			//	if(m_platformFriendLvp->getWorldPosition().x-m_friendSvp->getWorldPosition().x>GAP_turnPage&&m_platformFriendPage>1)
			//	{
			//		m_platformFriendPage--;
			//		updatePlatformFriend();
			//	}
			//	else if(m_platformFriendLvp->getWorldPosition().x-m_friendSvp->getWorldPosition().x<-GAP_turnPage&&m_platformFriendPage<m_platformFriendPageSum)
			//	{
			//		m_platformFriendPage++;
			//		updatePlatformFriend();
			//	}
			//	break;
		default:
			break;
		}

		//log("inner size : %f ",((ListView*)pSender)->getInnerContainerSize().height);
		//log("Direction : %d ",m_friendLvp->getDirection());
		log("Tag : %d ",t_sender->getTag());
		log("~~~~canceled!!");
		break;
	default:
		break;
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~touch event~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//**********************************update*******************************
//assets
void RankLayer::updateHp()
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

void RankLayer::updateDiamond()
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

void RankLayer::updateGold()
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
void RankLayer::updateMsg()
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

//Rank List
void RankLayer::updateRankLvp(RankType type)
{
	int t_page = 0;
	int t_pageSum = 0;
	int t_itemCount = 0;		//
	if(type==RANK_platform)
	{
		//clear list
		m_platformRankLvp->removeAllItems();

		t_itemCount = Player::getInstance()->getPlatformRankList()->size();
	}
	else if(type==RANK_world)
	{
		//clear list
		m_worldRankLvp->removeAllItems();

		t_itemCount = Player::getInstance()->getWorldRankList()->size();		//CW
	}

	if(t_itemCount%10||0==t_itemCount)
		m_pageSum = static_cast<int>(t_itemCount/10) + 1; //CW
	else
		m_pageSum = static_cast<int>(t_itemCount/10);

	t_page = m_page;
	t_pageSum = static_cast<int>(t_itemCount/10) + 1;;

	int count = 0;			//CW
	//CW
	if(t_page<t_pageSum&&t_page>0)
		count = 10;
	else if(t_page==t_pageSum)
		count = t_itemCount - 10*(t_page-1);			//CW

	int t_itemNum = 0;

	Layout* t_itemRank = 0;

	for (int i = 0; i < count; i++)
	{
		t_itemNum = 10*(t_page-1)+i;

		if(0==i%2)
			t_itemRank = static_cast<Layout*>(m_rankDualPal->clone());
		else
			t_itemRank = static_cast<Layout*>(m_rankSinglePal->clone());

		t_itemRank->setVisible(true);

		//set tag for rank item
		RankListItem* _item = nullptr;
		if(type==RANK_platform)
		{
			_item = Player::getInstance()->getPlatformRankList()->at(t_itemNum);
			t_itemRank->setTag(t_itemNum+TAG_platformRankPal);
		}
		else if(type==RANK_world)
		{
			_item = Player::getInstance()->getWorldRankList()->at(t_itemNum);
			t_itemRank->setTag(t_itemNum+TAG_worldRankPal);
		}

		//player name
		Text* t_playerName = static_cast<Text*>(t_itemRank->getChildByName("lab_playerName"));
		std::string t_playerNameStr = StringUtils::format("%s",_item->nickname.c_str());		//T
		t_playerName->setText(t_playerNameStr.c_str());		//CW
		//player score
		TextAtlas* t_score = static_cast<TextAtlas*>(t_itemRank->getChildByName("labAls_score"));
		//std::string t_scoreStr = StringUtils::format("2,222,0%d",t_itemNum);
		int _itemScore = _item->score;
		t_score->setStringValue(MyHelper::changeToScoreStringRight(_itemScore));

		ImageView* t_itemBg = static_cast<ImageView*>(t_itemRank->getChildByName("img_itemBg"));
		//player head
		ImageView* t_playerHead = static_cast<ImageView*>(t_itemBg->getChildByName("img_playerHead"));

		if(0==t_itemNum)
		{
			ImageView* t_medal = static_cast<ImageView*>(t_itemBg->getChildByName("img_medal"));
			t_medal->setVisible(true);
		}
		else if(1==t_itemNum)
		{
			ImageView* t_medal = static_cast<ImageView*>(t_itemBg->getChildByName("img_medal"));
			t_medal->setVisible(true);
			t_medal->loadTexture("commonUi/medalCopper.png");
		}
		else if(2==t_itemNum)
		{
			ImageView* t_medal = static_cast<ImageView*>(t_itemBg->getChildByName("img_medal"));
			t_medal->setVisible(true);
			t_medal->loadTexture("commonUi/medalIron.png");
		}
		else
		{
			TextAtlas* t_rank = static_cast<TextAtlas*>(t_itemBg->getChildByName("labAls_rank"));
			std::string t_rankStr = StringUtils::format("%d",t_itemNum+1);
			t_rank->setStringValue(t_rankStr.c_str());
		}

		if(type==RANK_platform)
		{
			m_platformRankLvp->pushBackCustomItem(t_itemRank);
		}
		else if(type==RANK_world)
		{
			m_worldRankLvp->pushBackCustomItem(t_itemRank);
		}
	}

	//offset  the list gap
	int _offsetCount = 6 - count;

	MyListView* _currentLvp = 0;

	if(type==RANK_platform)
	{
		_currentLvp = m_platformRankLvp;
	}
	else if(type==RANK_world)
	{
		_currentLvp = m_worldRankLvp;
	}

	if(_offsetCount>0)
	{
		for (int i = 0; i < _offsetCount; i++)
		{
			ImageView* _listFrameS = ImageView::create("commonUi/listFrameS.png");

			ImageView* _listFrameD = ImageView::create("commonUi/listFrameD.png");

			if(0==(count+i)%2)
				_currentLvp->pushBackCustomItem(_listFrameD);
			else
				_currentLvp->pushBackCustomItem(_listFrameS);
		}
	}

	float t_gapMax = 0;
	if(type==RANK_platform)
	{
		m_platformRankLvp->updateInnerContainerSizeV();
		m_platformRankLvp->updateSizeAndPosition();
		//Slider
		t_gapMax = m_platformRankLvp->getInnerContainer()->getPosition().y;
	}
	else if(type==RANK_world)
	{
		m_worldRankLvp->updateInnerContainerSizeV();
		m_worldRankLvp->updateSizeAndPosition();
		//Slider
		t_gapMax = m_worldRankLvp->getInnerContainer()->getPosition().y;
	}

	//Slider
	if(t_gapMax)
	{
		m_sliderSwitch = true;
		m_sliderGapMax = abs(t_gapMax);
	}
	else
		m_sliderSwitch = false;
}

//Rank List Page
//void RankLayer::updateRankPageCount()
//{
//	int t_platformRankCount = Player::getInstance()->getPlatformRankList()->size();	//CW
//	int t_worldRankCount = Player::getInstance()->getWorldRankList()->size();		//CW
//
//	m_page = 1;
//	m_pageSum = static_cast<int>(t_platformRankCount/10) + 1;
//
//	m_page = 1;
//	m_pageSum = static_cast<int>(t_worldRankCount/10) + 1;
//}

//player info
void RankLayer::updatePlayerInfoPal(int type)
{
	if(TAG_platformRankPal==type)
	{
	}
	else if(TAG_worldRankPal==type)
	{
	}
	else
		log("~~error~~ rank list type error");

	//m_playerHeadImg->setColor(ccc3(188,188,188));

	//std::string t_scoreStr = StringUtils::format("2,222,%d",tag);
	m_scoreText->setStringValue(MyHelper::changeToScoreStringCenter(Player::getInstance()->getHighestScore()));

	std::string t_levels = StringUtils::format("%d",Player::getInstance()->getHighestLevels());
	m_levelsText->setStringValue(t_levels.c_str());
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~update~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//**********************************schedule*******************************
void RankLayer::scheduleHpTime(float delayTime)
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

void RankLayer::update(float delayTime)
{
	//set slider
	float t_sliderScale = m_sliderGap/m_sliderGapMax;
	if(t_sliderScale<0)
		m_lvpSlider->setPercent(4);
	else if(t_sliderScale<1)
		m_lvpSlider->setPercent(4+92*(t_sliderScale));
	else
		m_lvpSlider->setPercent(96);
}

//notice msg scorll
void RankLayer::scheduleMsg(float delayTime)
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