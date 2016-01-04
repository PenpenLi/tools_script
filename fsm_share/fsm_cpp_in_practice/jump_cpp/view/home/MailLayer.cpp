////MailLayer.cpp created by php script. chris.li
#include "MailLayer.h"
#include "Network.h"
#include "Player.h"
//
#include "UiConfig.h"
#include "ShopLayer.h"
#include "util/GameSound.h"
#include "TipsInfo.h"

bool MailLayer::init()
{
	if (Layer::init())
	{
		//member variable init
		//Hp
		m_HpNum = 0;
		m_hpLoadingBarV.clear();
		//listView slider
		m_lvpSlider = nullptr;
		//gap
		m_touchSwitch = true;
		m_sliderSwitch = false;
		m_sliderGapMax = 0.0f;
		m_sliderGap = 0.0f;
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

void MailLayer::onEnter()
{
	Layer::onEnter();

	m_friendMailList_inviteItem = static_cast<Layout*>(m_friendMailList_inviteItem->clone());

	CC_SAFE_RETAIN(m_friendMailList_inviteItem);

	m_acceptStrengthItem = static_cast<Layout*>(m_acceptStrengthItem->clone());

	CC_SAFE_RETAIN(m_acceptStrengthItem);

	m_systemMailListItem = static_cast<Layout*>(m_systemMailListItem->clone());

	CC_SAFE_RETAIN(m_systemMailListItem);

	// init tab Order
	m_strengthListBtn->setZOrder(3);

	m_strengthListBtn->setClickedDown();

	m_strengthListTab->setVisible(true);

	m_friendMailListTab->setVisible(false);

	m_systemMailListTab->setVisible(false);

	m_friendMailList_mark->setVisible(false);

	m_systemMailList_mark->setVisible(false);

	m_strengthMailList_mark->setVisible(false);

	refreshMailList();

	//Get newest MailList
	Network::getInstance()->do_MailList_Req([this](const AckInfo &ackInfo)
	{
		std::string msgInfo = ackInfo.getMessageInfo();

		if (msgInfo != "")
		{
			log("[MailList_Ack] %s", msgInfo.c_str());
		}
		else
		{
			log("[MailList_Ack] OK!");

			refreshMailList();

			//slider
			float t_gapMax = 0;
			//get Slider gapMax
			m_strengthListTab->updateInnerContainerSizeV();

			m_strengthListTab->updateSizeAndPosition();

			t_gapMax = m_strengthListTab->getInnerContainer()->getPosition().y;

			if(t_gapMax)
			{
				m_sliderSwitch = true;
				m_sliderGapMax = abs(t_gapMax);
			}
			else
				m_sliderSwitch = false;
		}
	});

	//show listview current
	m_showListView = SHOW_strength;

	//schedule
	this->scheduleUpdate();
	this->schedule(schedule_selector(MailLayer::scheduleHpTime),1.0f);
	//tips
	this->schedule(schedule_selector(MailLayer::scheduleMsg),TIME_tipsScheduleDelay);

	//refresh
	refreshHp();
	refreshDiamond();
	refreshGold();

	//msg
	//m_notifyMsgLabel->setPosition(ccp(300,m_notifyMsgLabel->getPosition().y));

	//Point _pos = m_notifyMsgLabel->getPosition();

	//log("notifyMsgLabel Position : x = %f y = %f",_pos.x,_pos.y);
}

void MailLayer::onExit()
{
	CC_SAFE_RELEASE(m_friendMailList_inviteItem);

	CC_SAFE_RELEASE(m_acceptStrengthItem);

	CC_SAFE_RELEASE(m_systemMailListItem);

	//remove event listener
	//_eventDispatcher->removeCustomEventListeners("Return_shopLayer");

	Layer::onExit();

}

void MailLayer::loadUI()
{
	auto rootNode = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("MailLayer/MailLayer.json"));

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

	m_returnBtn-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

	m_acceptAllBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_acceptAllBtn"));

	m_acceptAllBtn-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

	m_notifyMsgLabel = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_notifyMsgLabel"));
	m_notifyMsgLabel->setVisible(false);

	m_friendMailListTab = static_cast<MyListView*>(ui::Helper::seekWidgetByName(rootNode, "m_friendMailListTab"));

	m_friendMailListTab-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

	m_systemMailListTab = static_cast<MyListView*>(ui::Helper::seekWidgetByName(rootNode, "m_systemMailListTab"));

	m_systemMailListTab-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

	m_strengthListTab = static_cast<MyListView*>(ui::Helper::seekWidgetByName(rootNode, "m_strengthListTab"));

	m_strengthListTab-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

	m_strengthMaxNum = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_strengthMaxNum"));

	m_addStrengthBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_addStrengthBtn"));

	m_addStrengthBtn-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

	m_diamondNum = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_diamondNum"));

	m_addDiamondBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_addDiamondBtn"));

	m_addDiamondBtn-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

	m_goldNum = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_goldNum"));

	m_addGoldBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_addGoldBtn"));

	m_addGoldBtn-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

	m_friendMailListBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_friendMailListBtn"));

	m_friendMailListBtn-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

	m_systemMailListBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_systemMailListBtn"));

	m_systemMailListBtn-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

	m_strengthListBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_strengthListBtn"));

	m_strengthListBtn-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

	m_friendMailList_inviteItem = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "m_friendMailList_inviteItem"));

	m_friendMailList_inviteItem-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

	m_systemMailListItem = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "m_systemMailListItem"));

	m_systemMailListItem-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

	m_acceptStrengthItem = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "m_acceptStrengthItem"));

	m_acceptStrengthItem-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

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

	m_friendMailList_mark = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_friendMailList_mark"));

	m_systemMailList_mark = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_systemMailList_mark"));

	m_strengthMailList_mark = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_strengthMailList_mark"));

	m_friendMailList_num = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_friendMailList_num"));

	m_systemMailList_num = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_systemMailList_num"));

	m_strengthMailList_num = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_strengthMailList_num"));

	m_lvpSlider = static_cast<Slider*>(ui::Helper::seekWidgetByName(rootNode, "m_slider"));
	m_lvpSlider->setTouchEnabled(false);
	m_lvpSlider->setVisible(false);

	m_friendMailListTab->addTouchEventListener(this, toucheventselector(MailLayer::listViewListenEvent));
	m_systemMailListTab->addTouchEventListener(this, toucheventselector(MailLayer::listViewListenEvent));
	m_strengthListTab->addTouchEventListener(this, toucheventselector(MailLayer::listViewListenEvent));

	//msg
	Layout* _msgFrame = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "pal_msg"));

	//event listener shop layer return call-back
	auto listener = EventListenerCustom::create("Return_shopLayer", [this](EventCustom* event) {
		log("AAAAAAAAAAAAAAA mail layer");
		refreshHp();
		refreshGold();
		refreshDiamond();
	});

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void MailLayer::handleTouchEvent(Ref *pSender, TouchEventType type)
{
	if (pSender == nullptr)
	{
	}

	else if (pSender == m_returnBtn && (TOUCH_EVENT_ENDED == type))
	{
		GameSound::playEffectMusic(EFF_CLOSE_PAGE);

		std::string eventName = "Return_mailLayer";
		EventCustom event(eventName);
		Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);

		this->removeFromParentAndCleanup(true);
	}

	else if (pSender == m_acceptAllBtn && (TOUCH_EVENT_ENDED == type))
	{
		GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);

		if(SHOW_strength==m_showListView)
		{
			//accept all the strength mail
			Network::getInstance()->do_MailDel_Req(77777, [this](const AckInfo &ackInfo)
			{
				std::string msgInfo = ackInfo.getMessageInfo();

				if (msgInfo != "")
				{
					log("[strengthMailDel_Ack] %s", msgInfo.c_str());
				}
				else
				{
					log("[strengthMailDel_Ack] OK!");

					auto mailList = Player::getInstance()->getMailList();

					for (int i = mailList->size()-1; i >= 0; i--)
					{
						auto mailListItem = mailList->at(i);

						log("type: %d", mailListItem->mailType);

						if (mailListItem->mailType == 2)
						{
							mailList->eraseObject(mailListItem);
						}
					}

					refreshMailList();
					//update the strength num
					Network::getInstance()->do_UpHeart_Req([this](const AckInfo &ackInfo) 
					{
						std::string msgInfo = ackInfo.getMessageInfo();

						if (msgInfo != "")
						{
							log("[UpHeart] %s", msgInfo.c_str());
						}
						else
						{
							//ok
							Player::getInstance()->setIsStarUpdate(true);
						}
					});
				}
			});
		}
		else if(SHOW_friend==m_showListView)
		{
			//accept friend mail
			Network::getInstance()->do_MailDel_Req(88888, [this](const AckInfo &ackInfo)
			{
				std::string msgInfo = ackInfo.getMessageInfo();

				if (msgInfo != "")
				{
					log("[friendMailDel_Ack] %s", msgInfo.c_str());
				}
				else
				{
					log("[friendMailDel_Ack] OK!");

					auto mailList = Player::getInstance()->getMailList();

					for (int i = mailList->size()-1; i >= 0; i--)
					{
						auto mailListItem = mailList->at(i);

						log("type: %d", mailListItem->mailType);

						if (mailListItem->mailType == 0)
						{
							mailList->eraseObject(mailListItem);
						}
					}

					refreshMailList();

					//update friend list
					Network::getInstance()->do_FriendList_Req(2,[this](const AckInfo &ackInfo){});
				}
			});
		}
		else if(SHOW_system==m_showListView)
		{
			Network::getInstance()->do_MailDel_Req(66666, [this](const AckInfo &ackInfo)
			{
				std::string msgInfo = ackInfo.getMessageInfo();

				if (msgInfo != "")
				{
					log("[systemMailDel_Ack] %s", msgInfo.c_str());
				}
				else
				{
					log("[systemMailDel_Ack] OK!");

					auto mailList = Player::getInstance()->getMailList();

					for (int i = mailList->size()-1; i >= 0; i--)
					{
						auto mailListItem = mailList->at(i);

						log("type: %d", mailListItem->mailType);

						if (mailListItem->mailType == 1 || mailListItem->mailType == 3)
						{
							mailList->eraseObject(mailListItem);
						}
					}

					refreshMailList();
				}
			});
		}

	}

	//else if (pSender == m_friendMailListTab && (TOUCH_EVENT_MOVED == type))
	//{
	//}

	//else if (pSender == m_friendMailListTab && (TOUCH_EVENT_CANCELED == type))
	//{
	//}

	//else if (pSender == m_systemMailListTab && (TOUCH_EVENT_MOVED == type))
	//{
	//}

	//else if (pSender == m_systemMailListTab && (TOUCH_EVENT_CANCELED == type))
	//{
	//}

	//else if (pSender == m_strengthListTab && (TOUCH_EVENT_BEGAN == type))
	//{
	//}

	//else if (pSender == m_strengthListTab && (TOUCH_EVENT_CANCELED == type))
	//{
	//}

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
		GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);

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

	else if (pSender == m_friendMailListBtn && (TOUCH_EVENT_ENDED == type))
	{
		GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);

		m_friendMailListBtn->setZOrder(3);

		m_systemMailListBtn->setZOrder(1);

		m_strengthListBtn->setZOrder(1);

		m_friendMailListBtn->setClickedDown();

		m_systemMailListBtn->setClickedNormal();

		m_strengthListBtn->setClickedNormal();

		m_friendMailListTab->setVisible(true);

		m_systemMailListTab->setVisible(false);

		m_strengthListTab->setVisible(false);

		//
		m_showListView = SHOW_friend;

		//slider
		float t_gapMax = 0;
		//get Slider gapMax
		m_friendMailListTab->updateInnerContainerSizeV();

		m_friendMailListTab->updateSizeAndPosition();

		t_gapMax = m_friendMailListTab->getInnerContainer()->getPosition().y;

		if(t_gapMax)
		{
			m_sliderSwitch = true;
			m_sliderGapMax = abs(t_gapMax);
		}
		else
			m_sliderSwitch = false;
	}

	else if (pSender == m_systemMailListBtn && (TOUCH_EVENT_ENDED == type))
	{
		GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);

		m_friendMailListBtn->setZOrder(1);

		m_systemMailListBtn->setZOrder(3);

		m_strengthListBtn->setZOrder(1);

		m_friendMailListBtn->setClickedNormal();

		m_systemMailListBtn->setClickedDown();

		m_strengthListBtn->setClickedNormal();

		m_friendMailListTab->setVisible(false);

		m_systemMailListTab->setVisible(true);

		//m_systemMailList_mark->setVisible(false);

		m_strengthListTab->setVisible(false);

		//
		m_showListView = SHOW_system;

		//slider
		float t_gapMax = 0;
		//get Slider gapMax
		m_systemMailListTab->updateInnerContainerSizeV();

		m_systemMailListTab->updateSizeAndPosition();

		t_gapMax = m_systemMailListTab->getInnerContainer()->getPosition().y;

		if(t_gapMax)
		{
			m_sliderSwitch = true;
			m_sliderGapMax = abs(t_gapMax);
		}
		else
			m_sliderSwitch = false;
	}

	else if (pSender == m_strengthListBtn && (TOUCH_EVENT_ENDED == type))
	{
		GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);

		m_friendMailListBtn->setZOrder(1);

		m_systemMailListBtn->setZOrder(1);

		m_strengthListBtn->setZOrder(3);

		m_friendMailListBtn->setClickedNormal();

		m_systemMailListBtn->setClickedNormal();

		m_strengthListBtn->setClickedDown();

		m_friendMailListTab->setVisible(false);

		m_systemMailListTab->setVisible(false);

		m_strengthListTab->setVisible(true);

		//m_systemMailList_mark->setVisible(false);

		//
		m_showListView = SHOW_strength;

		//slider
		float t_gapMax = 0;
		//get Slider gapMax
		m_strengthListTab->updateInnerContainerSizeV();

		m_strengthListTab->updateSizeAndPosition();

		t_gapMax = m_strengthListTab->getInnerContainer()->getPosition().y;

		if(t_gapMax)
		{
			m_sliderSwitch = true;
			m_sliderGapMax = abs(t_gapMax);
		}
		else
			m_sliderSwitch = false;
	}
	/*
	else if (pSender == m_friendMailList_inviteItem && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == m_friendMailList_strengthItem && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == m_systemMailListItem && (TOUCH_EVENT_ENDED == type))
	{
	}


	else if (pSender == m_refuseInviteBtn && (TOUCH_EVENT_ENDED == type))
	{

	}

	else if (pSender == m_acceptInviteBtn && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == m_sendStrengthBtn && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == m_mailItemContentLabel && (TOUCH_EVENT_ENDED == type))
	{
	}
	*/

	///////////////////////////////// CODING HERE ///////////////////////////////////////
	else if (TOUCH_EVENT_ENDED == type)
	{
		GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);

		auto senderNode = static_cast<Node*>(pSender);

		auto mailUserObject = static_cast<MailUserObject*>(senderNode->getUserObject());

		if (mailUserObject == nullptr) {return;}

		auto mailList = Player::getInstance()->getMailList();

		MailListItem *targetMailListItem = nullptr;

		for (auto mailListItem : (*mailList))
		{
			if (mailListItem->mailid == mailUserObject->mailId)
			{
				targetMailListItem = mailListItem;

				break;
			}
		}

		if (targetMailListItem == nullptr) {return;}

		if (mailUserObject->name == "mm_acceptInviteBtn")
		{
			log("mm_acceptInviteBtn");

			// add Friend
			Network::getInstance()->do_AddFriend_Req(targetMailListItem->playerId, 1, [=](const AckInfo &ackInfo)
			{
				std::string msgInfo = ackInfo.getMessageInfo();

				if (msgInfo != "")
				{
					log("[AddFriend_Ack] %s", msgInfo.c_str());
				}
				else
				{
					log("[AddFriend_Ack] OK!");

					if (targetMailListItem)
					{
						Player::getInstance()->getMailList()->eraseObject(targetMailListItem, false);

						refreshMailList();

						//update friend list
						Network::getInstance()->do_FriendList_Req(2,[this](const AckInfo &ackInfo){});
						Network::getInstance()->do_FriendList_Req(3,[this](const AckInfo &ackInfo){});
					}
				}
			});
		}

		else if (mailUserObject->name == "mm_refuseInviteBtn")
		{
			log("mm_refuseInviteBtn");

			// refuse Friend
			Network::getInstance()->do_DelFriend_Req(targetMailListItem->playerId, 2, [=](const AckInfo &ackInfo)
			{
				std::string msgInfo = ackInfo.getMessageInfo();

				if (msgInfo != "")
				{
					log("[do_DelFriend_Req] %s", msgInfo.c_str());
				}
				else
				{
					log("[do_DelFriend_Req] OK!");

					if (targetMailListItem)
					{
						Player::getInstance()->getMailList()->eraseObject(targetMailListItem, false);

						refreshMailList();
					}
				}
			});
		}

		else if (mailUserObject->name == "mm_friendMailList_inviteItem")
		{
			log("mm_friendMailList_inviteItem");
		}

		else if (mailUserObject->name == "mm_acceptStrengthBtn")
		{
			log("mm_acceptStrengthBtn");
			//accept strength mail
			Network::getInstance()->do_GetMailProp_Req(mailUserObject->mailId, [=](const AckInfo &ackInfo)
			{
				std::string msgInfo = ackInfo.getMessageInfo();

				if (msgInfo != "")
				{
					log("[do_acceptStrength_Req] %s", msgInfo.c_str());
				}
				else
				{
					log("[do_acceptStrength_Req] OK!");

					if (targetMailListItem)
					{
						Player::getInstance()->getMailList()->eraseObject(targetMailListItem, false);

						refreshMailList();
						//update strength num
						Network::getInstance()->do_UpHeart_Req([this](const AckInfo &ackInfo) 
						{
							std::string msgInfo = ackInfo.getMessageInfo();

							if (msgInfo != "")
							{
								log("[UpHeart] %s", msgInfo.c_str());
							}
							else
							{
								//ok
								refreshHp();
							}
						});
					}
				}
			});
		}

		else if (mailUserObject->name == "mm_propImage")
		{
			log("mm_propImage");
			Network::getInstance()->do_GetMailProp_Req(mailUserObject->mailId,[=](const AckInfo &ackInfo) 
			{
				std::string msgInfo = ackInfo.getMessageInfo();

				if (msgInfo != "")
				{
					log("[GetMailProp] %s", msgInfo.c_str());
				}
				else
				{
					log("[GetMailProp] : success!");
					if (targetMailListItem)
					{
						Player::getInstance()->getMailList()->eraseObject(targetMailListItem, false);
						refreshMailList();

						refreshGold();
						refreshDiamond();
					}
				}
			});
		}
	}
}

//ListView Listener Event
void MailLayer::listViewListenEvent(Ref *pSender, TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_MOVED:
		//log("TOUCH_EVENT_MOVED");
		if(m_touchSwitch)
		{
			if(m_sliderSwitch)
				m_lvpSlider->setVisible(true);

			m_touchSwitch = false;
		}
		if(m_friendMailListTab==pSender)
		{
			m_sliderGap = m_sliderGapMax + m_friendMailListTab->getInnerContainer()->getPosition().y;
		}
		else if(m_systemMailListTab==pSender)
		{
			m_sliderGap = m_sliderGapMax + m_systemMailListTab->getInnerContainer()->getPosition().y;
		}
		else if(m_strengthListTab==pSender)
		{
			m_sliderGap = m_sliderGapMax + m_strengthListTab->getInnerContainer()->getPosition().y;
		}
		break;
	case TOUCH_EVENT_ENDED:
		break;
	case TOUCH_EVENT_CANCELED:
		//log("TOUCH_EVENT_CANCELED");
		m_touchSwitch = true;

		m_lvpSlider->setVisible(false);
		//if(m_friendMailListTab==pSender)
		//{

		//}
		//else if(m_systemMailListTab==pSender)
		//{

		//}
		//else if(m_strengthListTab==pSender)
		//{

		//}
		break;
	default:
		break;
	}
}

////////////////////////////////////// CODING HERE /////////////////////////////////////////////////
void MailLayer::refreshMailList()
{
	int _mailCount = 0;

	int _strengthMailCount = 0;

	int _friendMailCount = 0;

	int _systemMailCount = 0;

	m_friendMailListTab->removeAllItems();

	m_systemMailListTab->removeAllItems();

	m_strengthListTab->removeAllItems();

	auto mailList = Player::getInstance()->getMailList();

	for (auto mailListItem : (*mailList))
	{
		log("type: %d", mailListItem->mailType);

		if (mailListItem->mailType == 1||mailListItem->mailType == 3)
		{
			auto item = m_systemMailListItem->clone();

			m_systemMailListTab->pushBackCustomItem(item);

			//
			//set player head image
			auto _playerHead = static_cast<ImageView*>(ui::Helper::seekWidgetByName(item, "mm_playerHeadImg"));
			//set player name text
			auto _playerName = static_cast<Text*>(ui::Helper::seekWidgetByName(item, "mm_playerNameLabel"));

			std::string _nameStr = "";
			//system  mail  set  the  name  by  title
			if(-1==mailListItem->playerId||0==mailListItem->playerId)
			{
				_nameStr = StringUtils::format("%s  %s",mailListItem->title.c_str(),mailListItem->domainTime.c_str());
			}
			else
			{
				_nameStr = StringUtils::format("%s  %s",mailListItem->nickName.c_str(),mailListItem->domainTime.c_str());
			}

			_playerName->setText(_nameStr);
			//set mail content text
			auto _mailContent = static_cast<Text*>(ui::Helper::seekWidgetByName(item, "mm_mailItemContentLabel"));

			_mailContent->setText(mailListItem->contents);
			//set send Prop image
			auto _propImage = static_cast<ImageView*>(ui::Helper::seekWidgetByName(item, "mm_systemNtfImg"));

			if(mailListItem->itemId==688)
			{
				//gold
				_propImage->loadTexture("commonUi/icon_gold.png");
			}
			else if(mailListItem->itemId==699)
			{
				//diamond
				_propImage->loadTexture("commonUi/icon_diamond.png");
			}

			_propImage->addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

			auto _propImageUserObject = MailUserObject::create("mm_propImage", mailListItem->mailid, mailListItem->mailType);

			_propImage->setUserObject(_propImageUserObject);

			if(-1==mailListItem->playerId)
			{
				_propImage->getParent()->setVisible(false);
			}
			else
			{
				++_mailCount;

				++_systemMailCount;
			}

		}

		else if (mailListItem->mailType == 2)
		{
			auto item = m_acceptStrengthItem->clone();

			auto acceptBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(item, "mm_acceptBtn"));

			acceptBtn-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

			auto acceptInviteBtnUserObject = MailUserObject::create("mm_acceptStrengthBtn", mailListItem->mailid, mailListItem->mailType);

			acceptBtn->setUserObject(acceptInviteBtnUserObject);

			m_strengthListTab->pushBackCustomItem(item);

			++_mailCount;

			++_strengthMailCount;

			//
			//set player head image
			auto _playerHead = static_cast<ImageView*>(ui::Helper::seekWidgetByName(item, "mm_playerHeadImg"));
			//set player name text
			auto _playerName = static_cast<Text*>(ui::Helper::seekWidgetByName(item, "mm_playerNameLabel"));

			std::string _nameStr = "";
			//system  mail  set  the  name  by  title
			if(-1==mailListItem->playerId||0==mailListItem->playerId)
			{
				_nameStr = StringUtils::format("%s  %s",mailListItem->title.c_str(),mailListItem->domainTime.c_str());
			}
			else
			{
				_nameStr = StringUtils::format("%s  %s",mailListItem->nickName.c_str(),mailListItem->domainTime.c_str());
			}

			_playerName->setText(_nameStr);
			//set mail content text
			auto _mailContent = static_cast<Text*>(ui::Helper::seekWidgetByName(item, "mm_mailItemContentLabel"));

			_mailContent->setText(mailListItem->contents);

		}

		//inviteItem
		else if (mailListItem->mailType == 0)
		{
			auto item = m_friendMailList_inviteItem->clone();

			item->setVisible(true);

			item->setTag(mailListItem->mailid);

			auto mailItemUserObject = MailUserObject::create("mm_friendMailList_inviteItem", mailListItem->mailid, mailListItem->mailType);

			item->setUserObject(mailItemUserObject);


			auto refuseInviteBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(item, "mm_refuseInviteBtn"));

			refuseInviteBtn-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

			auto refuseInviteBtnUserObject = MailUserObject::create("mm_refuseInviteBtn", mailListItem->mailid, mailListItem->mailType);

			refuseInviteBtn->setUserObject(refuseInviteBtnUserObject);


			auto acceptInviteBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(item, "mm_acceptInviteBtn"));

			acceptInviteBtn-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

			auto acceptInviteBtnUserObject = MailUserObject::create("mm_acceptInviteBtn", mailListItem->mailid, mailListItem->mailType);

			acceptInviteBtn->setUserObject(acceptInviteBtnUserObject);

			m_friendMailListTab->pushBackCustomItem(item);

			//
			//set player head image
			auto _playerHead = static_cast<ImageView*>(ui::Helper::seekWidgetByName(item, "mm_playerHeadImg"));
			//set player name text
			auto _playerName = static_cast<Text*>(ui::Helper::seekWidgetByName(item, "mm_playerNameLabel"));

			std::string _nameStr = "";
			//system  mail  set  the  name  by  title
			if(-1==mailListItem->playerId||0==mailListItem->playerId)
			{
				_nameStr = StringUtils::format("%s  %s",mailListItem->title.c_str(),mailListItem->domainTime.c_str());
			}
			else
			{
				_nameStr = StringUtils::format("%s  %s",mailListItem->nickName.c_str(),mailListItem->domainTime.c_str());
			}

			_playerName->setText(_nameStr);
			//set mail content text
			auto _mailContent = static_cast<Text*>(ui::Helper::seekWidgetByName(item, "mm_mailItemContentLabel"));

			_mailContent->setText(mailListItem->contents);

			if(-1==mailListItem->playerId)
			{
				acceptInviteBtn->setEnabled(false);

				refuseInviteBtn->setEnabled(false);
			}
			else
			{
				++_mailCount;

				++_friendMailCount;
			}

		}

	}

	Player::getInstance()->setMailCount(_mailCount);

	if(_strengthMailCount>0)
	{
		m_strengthMailList_mark->setVisible(true);

		m_strengthMailList_num->setText(StringUtils::format("%d",_strengthMailCount));
	}
	else
	{
		m_strengthMailList_mark->setVisible(false);
	}

	if(_friendMailCount>0)
	{
		m_friendMailList_mark->setVisible(true);

		m_friendMailList_num->setText(StringUtils::format("%d",_friendMailCount));
	}
	else
	{
		m_friendMailList_mark->setVisible(false);
	}

	if(_systemMailCount>0)
	{
		m_systemMailList_mark->setVisible(true);

		m_systemMailList_num->setText(StringUtils::format("%d",_systemMailCount));
	}
	else
	{
		m_systemMailList_mark->setVisible(false);
	}

	//offset  the list gap
	int _offsetCount = 6 - _systemMailCount;

	if(_offsetCount>0)
	{
		for (int i = 0; i < _offsetCount; i++)
		{
			ImageView* _listFrameS = ImageView::create("commonUi/listFrameS.png");

			m_systemMailListTab->pushBackCustomItem(_listFrameS);
		}
	}

	_offsetCount = 6 - _strengthMailCount;

	if(_offsetCount>0)
	{
		for (int i = 0; i < _offsetCount; i++)
		{
			ImageView* _listFrameS = ImageView::create("commonUi/listFrameS.png");

			m_strengthListTab->pushBackCustomItem(_listFrameS);
		}
	}

	_offsetCount = 6 - _friendMailCount;

	if(_offsetCount>0)
	{
		for (int i = 0; i < _offsetCount; i++)
		{
			ImageView* _listFrameS = ImageView::create("commonUi/listFrameS.png");

			m_friendMailListTab->pushBackCustomItem(_listFrameS);
		}
	}

}

//**********************************schedule******************************
void MailLayer::update(float delayTime)
{
	//set slider
	if(m_sliderSwitch)
	{
		float t_sliderScale = m_sliderGap/m_sliderGapMax;
		if(t_sliderScale<0)
			m_lvpSlider->setPercent(4);
		else if(t_sliderScale<1)
			m_lvpSlider->setPercent(4+92*(t_sliderScale));
		else
			m_lvpSlider->setPercent(96);
	}
}

void MailLayer::scheduleHpTime(float delayTime)
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

void MailLayer::scheduleMsg(float delayTime)
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
void MailLayer::refreshHp()
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
void MailLayer::refreshDiamond()
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
void MailLayer::refreshGold()
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

void MailLayer::refreshMsg()
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