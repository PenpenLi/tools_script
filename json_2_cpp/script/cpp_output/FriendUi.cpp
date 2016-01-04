////FriendUi.cpp created by php script. chris.li
#include "FriendUi.h"

bool FriendUi::init()
{
	if (Layer::init())
	{
		loadUI();
		return true;
	}

	return false;
}
void FriendUi::onEnter()
{
	Layer::onEnter();
}

void FriendUi::onExit()
{
	Layer::onExit();
}

void FriendUi::loadUI()
{
	auto rootNode = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("FriendUi/FriendUi.json"));

	this->addChild(rootNode);

	_pal_parent = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "pal_parent"));

	_pal_parent-> addTouchEventListener(this, toucheventselector(FriendUi::handleTouchEvent));

	_img_bg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_bg"));

	_pal_assets = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "pal_assets"));

	_pal_assets-> addTouchEventListener(this, toucheventselector(FriendUi::handleTouchEvent));

	_btn_return = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "btn_return"));

	_btn_return-> addTouchEventListener(this, toucheventselector(FriendUi::handleTouchEvent));

	_pal_btnAct = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "pal_btnAct"));

	_pal_btnAct-> addTouchEventListener(this, toucheventselector(FriendUi::handleTouchEvent));

	_img_msgBg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_msgBg"));

	_pal_friendFrame = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "pal_friendFrame"));

	_pal_friendFrame-> addTouchEventListener(this, toucheventselector(FriendUi::handleTouchEvent));

	_svp_friend = static_cast<ScrollView*>(ui::Helper::seekWidgetByName(rootNode, "svp_friend"));

	_svp_friend-> addTouchEventListener(this, toucheventselector(FriendUi::handleTouchEvent));

	_pal_roleInfo = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "pal_roleInfo"));

	_pal_roleInfo-> addTouchEventListener(this, toucheventselector(FriendUi::handleTouchEvent));

	_pal_friendSingle = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "pal_friendSingle"));

	_pal_friendSingle-> addTouchEventListener(this, toucheventselector(FriendUi::handleTouchEvent));

	_pal_friendDual = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "pal_friendDual"));

	_pal_friendDual-> addTouchEventListener(this, toucheventselector(FriendUi::handleTouchEvent));

	_pal_nearbyFriendSingle = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "pal_nearbyFriendSingle"));

	_pal_nearbyFriendSingle-> addTouchEventListener(this, toucheventselector(FriendUi::handleTouchEvent));

	_pal_nearbyFriendDual = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "pal_nearbyFriendDual"));

	_pal_nearbyFriendDual-> addTouchEventListener(this, toucheventselector(FriendUi::handleTouchEvent));

	_pal_platformFriendSingle = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "pal_platformFriendSingle"));

	_pal_platformFriendSingle-> addTouchEventListener(this, toucheventselector(FriendUi::handleTouchEvent));

	_pal_platformFriendDual = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "pal_platformFriendDual"));

	_pal_platformFriendDual-> addTouchEventListener(this, toucheventselector(FriendUi::handleTouchEvent));

	_img_bubble = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_bubble"));

	_img_hpFrameBg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_hpFrameBg"));

	_img_diamondFrameBg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_diamondFrameBg"));

	_img_goldFrameBg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_goldFrameBg"));

	_btn_mail = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "btn_mail"));

	_btn_mail-> addTouchEventListener(this, toucheventselector(FriendUi::handleTouchEvent));

	_btn_task = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "btn_task"));

	_btn_task-> addTouchEventListener(this, toucheventselector(FriendUi::handleTouchEvent));

	_btn_rank = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "btn_rank"));

	_btn_rank-> addTouchEventListener(this, toucheventselector(FriendUi::handleTouchEvent));

	_btn_activity = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "btn_activity"));

	_btn_activity-> addTouchEventListener(this, toucheventselector(FriendUi::handleTouchEvent));

	_lab_msg = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "lab_msg"));

	_img_friendFrameBg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_friendFrameBg"));

	_sdr_friend = static_cast<Slider*>(ui::Helper::seekWidgetByName(rootNode, "sdr_friend"));

	_sdr_friend-> addTouchEventListener(this, toucheventselector(FriendUi::handleTouchEvent));

	_lvp_nearbyFriend = static_cast<ListView*>(ui::Helper::seekWidgetByName(rootNode, "lvp_nearbyFriend"));

	_lvp_nearbyFriend-> addTouchEventListener(this, toucheventselector(FriendUi::handleTouchEvent));

	_lvp_friendList = static_cast<ListView*>(ui::Helper::seekWidgetByName(rootNode, "lvp_friendList"));

	_lvp_friendList-> addTouchEventListener(this, toucheventselector(FriendUi::handleTouchEvent));

	_lvp_platformFriend = static_cast<ListView*>(ui::Helper::seekWidgetByName(rootNode, "lvp_platformFriend"));

	_lvp_platformFriend-> addTouchEventListener(this, toucheventselector(FriendUi::handleTouchEvent));

	_img_roleFrame = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_roleFrame"));

	_img_itemBg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_itemBg"));

	_btn_sendHp = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "btn_sendHp"));

	_btn_sendHp-> addTouchEventListener(this, toucheventselector(FriendUi::handleTouchEvent));

	_img_itemBg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_itemBg"));

	_btn_sendHp = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "btn_sendHp"));

	_btn_sendHp-> addTouchEventListener(this, toucheventselector(FriendUi::handleTouchEvent));

	_img_itemBg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_itemBg"));

	_img_itemBg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_itemBg"));

	_img_itemBg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_itemBg"));

	_btn_invite = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "btn_invite"));

	_btn_invite-> addTouchEventListener(this, toucheventselector(FriendUi::handleTouchEvent));

	_img_itemBg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_itemBg"));

	_btn_invite = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "btn_invite"));

	_btn_invite-> addTouchEventListener(this, toucheventselector(FriendUi::handleTouchEvent));

	_img_hpBg_1 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_hpBg_1"));

	_img_hpBg_2 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_hpBg_2"));

	_img_hpBg_3 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_hpBg_3"));

	_img_hpBg_4 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_hpBg_4"));

	_img_hpBg_5 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_hpBg_5"));

	_lab_hpTime = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "lab_hpTime"));

	_btn_addHp = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "btn_addHp"));

	_btn_addHp-> addTouchEventListener(this, toucheventselector(FriendUi::handleTouchEvent));

	_img_diamond = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_diamond"));

	_lab_diamondNum = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "lab_diamondNum"));

	_btn_addDiamond = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "btn_addDiamond"));

	_btn_addDiamond-> addTouchEventListener(this, toucheventselector(FriendUi::handleTouchEvent));

	_img_gold = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_gold"));

	_lab_goldNum = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "lab_goldNum"));

	_btn_addGold = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "btn_addGold"));

	_btn_addGold-> addTouchEventListener(this, toucheventselector(FriendUi::handleTouchEvent));

	_img_mail = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_mail"));

	_lab_mail = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "lab_mail"));

	_img_mailNumBg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_mailNumBg"));

	_img_task = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_task"));

	_lab_task = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "lab_task"));

	_img_taskNumBg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_taskNumBg"));

	_img_rank = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_rank"));

	_lab_rank = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "lab_rank"));

	_img_activity = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_activity"));

	_lab_activity = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "lab_activity"));

	_img_friendLab = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_friendLab"));

	_btn_friendList = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "btn_friendList"));

	_btn_friendList-> addTouchEventListener(this, toucheventselector(FriendUi::handleTouchEvent));

	_btn_platformFriend = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "btn_platformFriend"));

	_btn_platformFriend-> addTouchEventListener(this, toucheventselector(FriendUi::handleTouchEvent));

	_btn_nearbyFriend = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "btn_nearbyFriend"));

	_btn_nearbyFriend-> addTouchEventListener(this, toucheventselector(FriendUi::handleTouchEvent));

	_img_title = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_title"));

	_img_palyerHead = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_palyerHead"));

	_lab_record = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "lab_record"));

	_labAls_score = static_cast<TextAtlas*>(ui::Helper::seekWidgetByName(rootNode, "labAls_score"));

	_lab_levels = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "lab_levels"));

	_labAls_levels = static_cast<TextAtlas*>(ui::Helper::seekWidgetByName(rootNode, "labAls_levels"));

	_btn_deleteFriend = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "btn_deleteFriend"));

	_btn_deleteFriend-> addTouchEventListener(this, toucheventselector(FriendUi::handleTouchEvent));

	_btn_addFriend = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "btn_addFriend"));

	_btn_addFriend-> addTouchEventListener(this, toucheventselector(FriendUi::handleTouchEvent));

	_btn_return = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "btn_return"));

	_btn_return-> addTouchEventListener(this, toucheventselector(FriendUi::handleTouchEvent));

	_img_playerHead = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_playerHead"));

	_img_role = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_role"));

	_lab_playerName = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "lab_playerName"));

	_labAls_score = static_cast<TextAtlas*>(ui::Helper::seekWidgetByName(rootNode, "labAls_score"));

	_img_playerHead = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_playerHead"));

	_img_role = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_role"));

	_lab_playerName = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "lab_playerName"));

	_labAls_score = static_cast<TextAtlas*>(ui::Helper::seekWidgetByName(rootNode, "labAls_score"));

	_img_playerHead = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_playerHead"));

	_img_role = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_role"));

	_lab_playerName = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "lab_playerName"));

	_labAls_score = static_cast<TextAtlas*>(ui::Helper::seekWidgetByName(rootNode, "labAls_score"));

	_img_playerHead = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_playerHead"));

	_img_role = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_role"));

	_lab_playerName = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "lab_playerName"));

	_labAls_score = static_cast<TextAtlas*>(ui::Helper::seekWidgetByName(rootNode, "labAls_score"));

	_img_playerHead = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_playerHead"));

	_lab_playerName = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "lab_playerName"));

	_img_playerHead = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_playerHead"));

	_lab_playerName = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "lab_playerName"));

	_pgb_hp_1 = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(rootNode, "pgb_hp_1"));

	_pgb_hp_2 = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(rootNode, "pgb_hp_2"));

	_pgb_hp_3 = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(rootNode, "pgb_hp_3"));

	_pgb_hp_4 = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(rootNode, "pgb_hp_4"));

	_pgb_hp_5 = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(rootNode, "pgb_hp_5"));

	_lab_mailNum = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "lab_mailNum"));

	_lab_taskNum = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "lab_taskNum"));

	_img_roleBubble = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_roleBubble"));

	_img_roleBubble = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_roleBubble"));

	_img_roleBubble = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_roleBubble"));

	_img_roleBubble = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "img_roleBubble"));


}

void FriendUi::handleTouchEvent(Ref *pSender, TouchEventType type)
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

	else if (pSender == _pal_btnAct && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _pal_friendFrame && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _svp_friend && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _pal_roleInfo && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _pal_friendSingle && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _pal_friendDual && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _pal_nearbyFriendSingle && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _pal_nearbyFriendDual && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _pal_platformFriendSingle && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _pal_platformFriendDual && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _btn_mail && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _btn_task && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _btn_rank && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _btn_activity && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _sdr_friend && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _lvp_nearbyFriend && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _lvp_friendList && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _lvp_platformFriend && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _btn_sendHp && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _btn_sendHp && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _btn_invite && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _btn_invite && (TOUCH_EVENT_ENDED == type))
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

	else if (pSender == _btn_friendList && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _btn_platformFriend && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _btn_nearbyFriend && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _btn_deleteFriend && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _btn_addFriend && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _btn_return && (TOUCH_EVENT_ENDED == type))
	{
	}


}

