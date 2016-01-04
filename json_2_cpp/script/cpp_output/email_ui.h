//// email_ui.h created by php script. chris.li
#ifndef __HiFly__email_ui
#define __HiFly__email_ui

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

class email_ui : public cocos2d::Layer
{
public:
	CREATE_FUNC(email_ui);
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

	ImageView* _img_msgBg;

	Layout* _mailListBg;

	ScrollView* _mailListTabs;

	Button* _Button_129;

	ImageView* _img_bubble;

	ImageView* _img_hpFrameBg;

	ImageView* _img_diamondFrameBg;

	ImageView* _img_goldFrameBg;

	Text* _label_notify_msg;

	ImageView* _mailListBg1;

	Slider* _mailListBg2;

	ListView* _friendMailList_tab;

	ListView* _systemMailList_tab;

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

	Button* _btn_friendMailList;

	Button* _btn_systemMailList;

	Layout* _friendMailList_item;

	Layout* _friendMailList_item2;

	Layout* _pal_itemSingle;

	Layout* _pal_itemSingle_0;

	LoadingBar* _pgb_hp_1;

	LoadingBar* _pgb_hp_2;

	LoadingBar* _pgb_hp_3;

	LoadingBar* _pgb_hp_4;

	LoadingBar* _pgb_hp_5;

	ImageView* _friendMailList_num;

	ImageView* _systemMailList_num;

	ImageView* _img_itemBg;

	Button* _Button_121_0_0;

	Button* _Button_121_0_0_1;

	Button* _Button_121_0_0_2;

	ImageView* _img_itemBg;

	Button* _Button_121_0_0;

	Button* _Button_121_0_0_1;

	Button* _Button_121_0_0_2;

	ImageView* _img_itemBg;

	ImageView* _Image_107;

	ImageView* _img_itemBg;

	ImageView* _Image_107_0;

	Text* _label_friendMailList_num;

	Text* _label_systemMailList_num;

	ImageView* _img_playerHead;

	Text* _lab_playerName;

	TextField* _TextField_480;

	ImageView* _img_playerHead;

	Text* _lab_playerName;

	TextField* _TextField_480;

	ImageView* _img_playerHead;

	Text* _lab_playerName;

	TextField* _TextField_480;

	ImageView* _Image_108;

	ImageView* _img_playerHead;

	Text* _lab_playerName;

	TextField* _TextField_480;

	ImageView* _Image_108;

};
#endif
