////email_ui.cpp created by php script. chris.li
#include "email_ui.h"

bool email_ui::init()
{
	if (Layer::init())
	{
		loadUI();
		return true;
	}

	return false;
}
void email_ui::onEnter()
{
	Layer::onEnter();
}

void email_ui::onExit()
{
	Layer::onExit();
}

void email_ui::loadUI()
{
	auto rootNode = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("email_ui/email_ui.json"));

	this->addChild(rootNode);

	_pal_parent = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "pal_parent"));

	_pal_parent-> addTouchEventListener(this, toucheventselector(email_ui::handleTouchEvent));

	_img_bg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_bg"));

	_pal_assets = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "pal_assets"));

	_pal_assets-> addTouchEventListener(this, toucheventselector(email_ui::handleTouchEvent));

	_btn_return = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "btn_return"));

	_btn_return-> addTouchEventListener(this, toucheventselector(email_ui::handleTouchEvent));

	_img_msgBg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_msgBg"));

	_mailListBg = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "mailListBg"));

	_mailListBg-> addTouchEventListener(this, toucheventselector(email_ui::handleTouchEvent));

	_mailListTabs = static_cast<ScrollView*>(ui::Helper::seekWidgetByName(rootNode, "mailListTabs"));

	_mailListTabs-> addTouchEventListener(this, toucheventselector(email_ui::handleTouchEvent));

	_Button_129 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "Button_129"));

	_Button_129-> addTouchEventListener(this, toucheventselector(email_ui::handleTouchEvent));

	_img_bubble = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_bubble"));

	_img_hpFrameBg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_hpFrameBg"));

	_img_diamondFrameBg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_diamondFrameBg"));

	_img_goldFrameBg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_goldFrameBg"));

	_label_notify_msg = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "label_notify_msg"));

	_mailListBg1 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "mailListBg1"));

	_mailListBg2 = static_cast<Slider*>(ui::Helper::seekWidgetByName(rootNode, "mailListBg2"));

	_mailListBg2-> addTouchEventListener(this, toucheventselector(email_ui::handleTouchEvent));

	_friendMailList_tab = static_cast<ListView*>(ui::Helper::seekWidgetByName(rootNode, "friendMailList_tab"));

	_friendMailList_tab-> addTouchEventListener(this, toucheventselector(email_ui::handleTouchEvent));

	_systemMailList_tab = static_cast<ListView*>(ui::Helper::seekWidgetByName(rootNode, "systemMailList_tab"));

	_systemMailList_tab-> addTouchEventListener(this, toucheventselector(email_ui::handleTouchEvent));

	_img_hpBg_1 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_hpBg_1"));

	_img_hpBg_2 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_hpBg_2"));

	_img_hpBg_3 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_hpBg_3"));

	_img_hpBg_4 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_hpBg_4"));

	_img_hpBg_5 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_hpBg_5"));

	_lab_hpTime = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "lab_hpTime"));

	_btn_addHp = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "btn_addHp"));

	_btn_addHp-> addTouchEventListener(this, toucheventselector(email_ui::handleTouchEvent));

	_img_diamond = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_diamond"));

	_lab_diamondNum = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "lab_diamondNum"));

	_btn_addDiamond = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "btn_addDiamond"));

	_btn_addDiamond-> addTouchEventListener(this, toucheventselector(email_ui::handleTouchEvent));

	_img_gold = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_gold"));

	_lab_goldNum = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "lab_goldNum"));

	_btn_addGold = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "btn_addGold"));

	_btn_addGold-> addTouchEventListener(this, toucheventselector(email_ui::handleTouchEvent));

	_img_mail = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_mail"));

	_btn_friendMailList = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "btn_friendMailList"));

	_btn_friendMailList-> addTouchEventListener(this, toucheventselector(email_ui::handleTouchEvent));

	_btn_systemMailList = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "btn_systemMailList"));

	_btn_systemMailList-> addTouchEventListener(this, toucheventselector(email_ui::handleTouchEvent));

	_friendMailList_item = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "friendMailList_item"));

	_friendMailList_item-> addTouchEventListener(this, toucheventselector(email_ui::handleTouchEvent));

	_friendMailList_item2 = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "friendMailList_item2"));

	_friendMailList_item2-> addTouchEventListener(this, toucheventselector(email_ui::handleTouchEvent));

	_pal_itemSingle = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "pal_itemSingle"));

	_pal_itemSingle-> addTouchEventListener(this, toucheventselector(email_ui::handleTouchEvent));

	_pal_itemSingle_0 = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "pal_itemSingle_0"));

	_pal_itemSingle_0-> addTouchEventListener(this, toucheventselector(email_ui::handleTouchEvent));

	_pgb_hp_1 = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(rootNode, "pgb_hp_1"));

	_pgb_hp_2 = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(rootNode, "pgb_hp_2"));

	_pgb_hp_3 = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(rootNode, "pgb_hp_3"));

	_pgb_hp_4 = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(rootNode, "pgb_hp_4"));

	_pgb_hp_5 = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(rootNode, "pgb_hp_5"));

	_friendMailList_num = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "friendMailList_num"));

	_systemMailList_num = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "systemMailList_num"));

	_img_itemBg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_itemBg"));

	_Button_121_0_0 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "Button_121_0_0"));

	_Button_121_0_0-> addTouchEventListener(this, toucheventselector(email_ui::handleTouchEvent));

	_Button_121_0_0_1 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "Button_121_0_0_1"));

	_Button_121_0_0_1-> addTouchEventListener(this, toucheventselector(email_ui::handleTouchEvent));

	_Button_121_0_0_2 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "Button_121_0_0_2"));

	_Button_121_0_0_2-> addTouchEventListener(this, toucheventselector(email_ui::handleTouchEvent));

	_img_itemBg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_itemBg"));

	_Button_121_0_0 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "Button_121_0_0"));

	_Button_121_0_0-> addTouchEventListener(this, toucheventselector(email_ui::handleTouchEvent));

	_Button_121_0_0_1 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "Button_121_0_0_1"));

	_Button_121_0_0_1-> addTouchEventListener(this, toucheventselector(email_ui::handleTouchEvent));

	_Button_121_0_0_2 = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "Button_121_0_0_2"));

	_Button_121_0_0_2-> addTouchEventListener(this, toucheventselector(email_ui::handleTouchEvent));

	_img_itemBg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_itemBg"));

	_Image_107 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_107"));

	_img_itemBg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_itemBg"));

	_Image_107_0 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_107_0"));

	_label_friendMailList_num = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "label_friendMailList_num"));

	_label_systemMailList_num = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "label_systemMailList_num"));

	_img_playerHead = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_playerHead"));

	_lab_playerName = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "lab_playerName"));

	_TextField_480 = static_cast<TextField*>(ui::Helper::seekWidgetByName(rootNode, "TextField_480"));

	_TextField_480-> addTouchEventListener(this, toucheventselector(email_ui::handleTouchEvent));

	_img_playerHead = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_playerHead"));

	_lab_playerName = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "lab_playerName"));

	_TextField_480 = static_cast<TextField*>(ui::Helper::seekWidgetByName(rootNode, "TextField_480"));

	_TextField_480-> addTouchEventListener(this, toucheventselector(email_ui::handleTouchEvent));

	_img_playerHead = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_playerHead"));

	_lab_playerName = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "lab_playerName"));

	_TextField_480 = static_cast<TextField*>(ui::Helper::seekWidgetByName(rootNode, "TextField_480"));

	_TextField_480-> addTouchEventListener(this, toucheventselector(email_ui::handleTouchEvent));

	_Image_108 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_108"));

	_img_playerHead = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_playerHead"));

	_lab_playerName = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "lab_playerName"));

	_TextField_480 = static_cast<TextField*>(ui::Helper::seekWidgetByName(rootNode, "TextField_480"));

	_TextField_480-> addTouchEventListener(this, toucheventselector(email_ui::handleTouchEvent));

	_Image_108 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_108"));


}

void email_ui::handleTouchEvent(Ref *pSender, TouchEventType type)
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

	else if (pSender == _btn_return && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _mailListBg && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _mailListTabs && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _Button_129 && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _mailListBg2 && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _friendMailList_tab && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _systemMailList_tab && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _btn_addHp && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _btn_addDiamond && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _btn_addGold && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _btn_friendMailList && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _btn_systemMailList && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _friendMailList_item && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _friendMailList_item2 && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _pal_itemSingle && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _pal_itemSingle_0 && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _Button_121_0_0 && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _Button_121_0_0_1 && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _Button_121_0_0_2 && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _Button_121_0_0 && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _Button_121_0_0_1 && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _Button_121_0_0_2 && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _TextField_480 && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _TextField_480 && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _TextField_480 && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _TextField_480 && (TOUCH_EVENT_ENDED == type))
	{
	}


}

