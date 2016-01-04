//// FriendUi.h created by php script. chris.li
#ifndef __HiFly__FriendUi
#define __HiFly__FriendUi

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

class FriendUi : public cocos2d::Layer
{
public:
	CREATE_FUNC(FriendUi);
private:
	bool init();
	void onEnter();
	void onExit();
	void loadUI();
	void handleTouchEvent(Ref *pSender, TouchEventType type);

	//get ui reference marked by ui_xxx 
	Layout* _pal_parent;

	ImageView* _img_bg;

	Layout* _pal_assets;

	Button* _btn_return;

	Layout* _pal_btnAct;

	ImageView* _img_msgBg;

	Layout* _pal_friendFrame;

	ScrollView* _svp_friend;

	Layout* _pal_roleInfo;

	Layout* _pal_friendSingle;

	Layout* _pal_friendDual;

	Layout* _pal_nearbyFriendSingle;

	Layout* _pal_nearbyFriendDual;

	Layout* _pal_platformFriendSingle;

	Layout* _pal_platformFriendDual;

	ImageView* _img_bubble;

	ImageView* _img_hpFrameBg;

	ImageView* _img_diamondFrameBg;

	ImageView* _img_goldFrameBg;

	Button* _btn_mail;

	Button* _btn_task;

	Button* _btn_rank;

	Button* _btn_activity;

	Text* _lab_msg;

	ImageView* _img_friendFrameBg;

	Slider* _sdr_friend;

	ListView* _lvp_nearbyFriend;

	ListView* _lvp_friendList;

	ListView* _lvp_platformFriend;

	ImageView* _img_roleFrame;

	ImageView* _img_itemBg;

	Button* _btn_sendHp;

	ImageView* _img_itemBg;

	Button* _btn_sendHp;

	ImageView* _img_itemBg;

	ImageView* _img_itemBg;

	ImageView* _img_itemBg;

	Button* _btn_invite;

	ImageView* _img_itemBg;

	Button* _btn_invite;

	ImageView* _img_hpBg_1;

	ImageView* _img_hpBg_2;

	ImageView* _img_hpBg_3;

	ImageView* _img_hpBg_4;

	ImageView* _img_hpBg_5;

	Text* _lab_hpTime;

	Button* _btn_addHp;

	ImageView* _img_diamond;

	Text* _lab_diamondNum;

	Button* _btn_addDiamond;

	ImageView* _img_gold;

	Text* _lab_goldNum;

	Button* _btn_addGold;

	ImageView* _img_mail;

	Text* _lab_mail;

	ImageView* _img_mailNumBg;

	ImageView* _img_task;

	Text* _lab_task;

	ImageView* _img_taskNumBg;

	ImageView* _img_rank;

	Text* _lab_rank;

	ImageView* _img_activity;

	Text* _lab_activity;

	ImageView* _img_friendLab;

	Button* _btn_friendList;

	Button* _btn_platformFriend;

	Button* _btn_nearbyFriend;

	ImageView* _img_title;

	ImageView* _img_palyerHead;

	Text* _lab_record;

	TextAtlas* _labAls_score;

	Text* _lab_levels;

	TextAtlas* _labAls_levels;

	Button* _btn_deleteFriend;

	Button* _btn_addFriend;

	Button* _btn_return;

	ImageView* _img_playerHead;

	ImageView* _img_role;

	Text* _lab_playerName;

	TextAtlas* _labAls_score;

	ImageView* _img_playerHead;

	ImageView* _img_role;

	Text* _lab_playerName;

	TextAtlas* _labAls_score;

	ImageView* _img_playerHead;

	ImageView* _img_role;

	Text* _lab_playerName;

	TextAtlas* _labAls_score;

	ImageView* _img_playerHead;

	ImageView* _img_role;

	Text* _lab_playerName;

	TextAtlas* _labAls_score;

	ImageView* _img_playerHead;

	Text* _lab_playerName;

	ImageView* _img_playerHead;

	Text* _lab_playerName;

	LoadingBar* _pgb_hp_1;

	LoadingBar* _pgb_hp_2;

	LoadingBar* _pgb_hp_3;

	LoadingBar* _pgb_hp_4;

	LoadingBar* _pgb_hp_5;

	Text* _lab_mailNum;

	Text* _lab_taskNum;

	ImageView* _img_roleBubble;

	ImageView* _img_roleBubble;

	ImageView* _img_roleBubble;

	ImageView* _img_roleBubble;

};
#endif
