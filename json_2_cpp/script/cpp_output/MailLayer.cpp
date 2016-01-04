////MailLayer.cpp created by php script. chris.li
#include "MailLayer.h"

bool MailLayer::init()
{
	if (Layer::init())
	{
		loadUI();
		return true;
	}

	return false;
}
void MailLayer::onEnter()
{
	Layer::onEnter();
}

void MailLayer::onExit()
{
	Layer::onExit();
}

void MailLayer::loadUI()
{
	auto rootNode = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("MailLayer/MailLayer.json"));

	this->addChild(rootNode);

	_pal_parent = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "pal_parent"));

	_pal_parent-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

	_img_bg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_bg"));

	_pal_assets = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "pal_assets"));

	_pal_assets-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

	_m_returnBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_returnBtn"));

	_m_returnBtn-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

	_img_msgBg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_msgBg"));

	_mailListBg = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "mailListBg"));

	_mailListBg-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

	_mailListTabs = static_cast<ScrollView*>(ui::Helper::seekWidgetByName(rootNode, "mailListTabs"));

	_mailListTabs-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

	_m_acceptAllBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_acceptAllBtn"));

	_m_acceptAllBtn-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

	_img_bubble = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_bubble"));

	_img_hpFrameBg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_hpFrameBg"));

	_img_diamondFrameBg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_diamondFrameBg"));

	_img_goldFrameBg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_goldFrameBg"));

	_m_notifyMsgLabel = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_notifyMsgLabel"));

	_mailListBg1 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "mailListBg1"));

	_mailListBg2 = static_cast<Slider*>(ui::Helper::seekWidgetByName(rootNode, "mailListBg2"));

	_mailListBg2-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

	_m_friendMailListTab = static_cast<ListView*>(ui::Helper::seekWidgetByName(rootNode, "m_friendMailListTab"));

	_m_friendMailListTab-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

	_m_systemMailListTab = static_cast<ListView*>(ui::Helper::seekWidgetByName(rootNode, "m_systemMailListTab"));

	_m_systemMailListTab-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

	_m_strengthListTab = static_cast<ListView*>(ui::Helper::seekWidgetByName(rootNode, "m_strengthListTab"));

	_m_strengthListTab-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

	_img_hpBg_1 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_hpBg_1"));

	_img_hpBg_2 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_hpBg_2"));

	_img_hpBg_3 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_hpBg_3"));

	_img_hpBg_4 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_hpBg_4"));

	_img_hpBg_5 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_hpBg_5"));

	_m_strengthMaxNum = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_strengthMaxNum"));

	_m_addStrengthBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_addStrengthBtn"));

	_m_addStrengthBtn-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

	_img_diamond = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_diamond"));

	_m_diamondNum = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_diamondNum"));

	_m_addDiamondBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_addDiamondBtn"));

	_m_addDiamondBtn-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

	_img_gold = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_gold"));

	_m_goldNum = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_goldNum"));

	_m_addGoldBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_addGoldBtn"));

	_m_addGoldBtn-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

	_img_mail = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_mail"));

	_m_friendMailListBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_friendMailListBtn"));

	_m_friendMailListBtn-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

	_m_systemMailListBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_systemMailListBtn"));

	_m_systemMailListBtn-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

	_m_strengthListBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_strengthListBtn"));

	_m_strengthListBtn-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

	_m_friendMailList_inviteItem = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "m_friendMailList_inviteItem"));

	_m_friendMailList_inviteItem-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

	_m_systemMailListItem = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "m_systemMailListItem"));

	_m_systemMailListItem-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

	_m_acceptStrengthItem = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "m_acceptStrengthItem"));

	_m_acceptStrengthItem-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

	_m_strengthBar1 = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(rootNode, "m_strengthBar1"));

	_m_strengthBar2 = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(rootNode, "m_strengthBar2"));

	_m_strengthBar3 = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(rootNode, "m_strengthBar3"));

	_m_strengthBar4 = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(rootNode, "m_strengthBar4"));

	_m_strengthBar5 = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(rootNode, "m_strengthBar5"));

	_m_friendMailList_mark = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_friendMailList_mark"));

	_m_systemMailList_mark = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_systemMailList_mark"));

	_m_strengthMailList_mark = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_strengthMailList_mark"));

	_img_itemBg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_itemBg"));

	_mm_refuseInviteBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "mm_refuseInviteBtn"));

	_mm_refuseInviteBtn-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

	_mm_acceptInviteBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "mm_acceptInviteBtn"));

	_mm_acceptInviteBtn-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

	_img_itemBg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_itemBg"));

	_Image_107 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_107"));

	_mm_playerHeadImg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "mm_playerHeadImg"));

	_mm_acceptBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "mm_acceptBtn"));

	_mm_acceptBtn-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

	_m_friendMailList_num = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_friendMailList_num"));

	_m_systemMailList_num = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_systemMailList_num"));

	_m_strengthMailList_num = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_strengthMailList_num"));

	_mm_playerHeadImg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "mm_playerHeadImg"));

	_mm_playerNameLabel = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "mm_playerNameLabel"));

	_mm_mailItemContentLabel = static_cast<TextField*>(ui::Helper::seekWidgetByName(rootNode, "mm_mailItemContentLabel"));

	_mm_mailItemContentLabel-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

	_mm_headImg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "mm_headImg"));

	_mm_playerName = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "mm_playerName"));

	_mm_mailItemContentLabel = static_cast<TextField*>(ui::Helper::seekWidgetByName(rootNode, "mm_mailItemContentLabel"));

	_mm_mailItemContentLabel-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));

	_mm_systemNtfImg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "mm_systemNtfImg"));

	_mm_playerHeadImg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "mm_playerHeadImg"));

	_mm_playerNameLabel = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "mm_playerNameLabel"));

	_mm_mailItemContentLabel = static_cast<TextField*>(ui::Helper::seekWidgetByName(rootNode, "mm_mailItemContentLabel"));

	_mm_mailItemContentLabel-> addTouchEventListener(this, toucheventselector(MailLayer::handleTouchEvent));


}

void MailLayer::handleTouchEvent(Ref *pSender, TouchEventType type)
{
	if (pSender == nullptr)
	{
	}

	else if (pSender == _pal_parent && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _pal_assets && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _m_returnBtn && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _mailListBg && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _mailListTabs && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _m_acceptAllBtn && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _mailListBg2 && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _m_friendMailListTab && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _m_systemMailListTab && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _m_strengthListTab && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _m_addStrengthBtn && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _m_addDiamondBtn && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _m_addGoldBtn && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _m_friendMailListBtn && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _m_systemMailListBtn && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _m_strengthListBtn && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _m_friendMailList_inviteItem && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _m_systemMailListItem && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _m_acceptStrengthItem && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _mm_refuseInviteBtn && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _mm_acceptInviteBtn && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _mm_acceptBtn && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _mm_mailItemContentLabel && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _mm_mailItemContentLabel && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _mm_mailItemContentLabel && (TOUCH_EVENT_ENDED == type))
	{
	}


}

