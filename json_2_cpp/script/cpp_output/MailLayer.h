//// MailLayer.h created by php script. chris.li
#ifndef __HiFly__MailLayer
#define __HiFly__MailLayer

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

class MailLayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(MailLayer);
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

	Button* _m_returnBtn;

	ImageView* _img_msgBg;

	Layout* _mailListBg;

	ScrollView* _mailListTabs;

	Button* _m_acceptAllBtn;

	ImageView* _img_bubble;

	ImageView* _img_hpFrameBg;

	ImageView* _img_diamondFrameBg;

	ImageView* _img_goldFrameBg;

	Text* _m_notifyMsgLabel;

	ImageView* _mailListBg1;

	Slider* _mailListBg2;

	ListView* _m_friendMailListTab;

	ListView* _m_systemMailListTab;

	ListView* _m_strengthListTab;

	ImageView* _img_hpBg_1;

	ImageView* _img_hpBg_2;

	ImageView* _img_hpBg_3;

	ImageView* _img_hpBg_4;

	ImageView* _img_hpBg_5;

	Text* _m_strengthMaxNum;

	Button* _m_addStrengthBtn;

	ImageView* _img_diamond;

	Text* _m_diamondNum;

	Button* _m_addDiamondBtn;

	ImageView* _img_gold;

	Text* _m_goldNum;

	Button* _m_addGoldBtn;

	ImageView* _img_mail;

	Button* _m_friendMailListBtn;

	Button* _m_systemMailListBtn;

	Button* _m_strengthListBtn;

	Layout* _m_friendMailList_inviteItem;

	Layout* _m_systemMailListItem;

	Layout* _m_acceptStrengthItem;

	LoadingBar* _m_strengthBar1;

	LoadingBar* _m_strengthBar2;

	LoadingBar* _m_strengthBar3;

	LoadingBar* _m_strengthBar4;

	LoadingBar* _m_strengthBar5;

	ImageView* _m_friendMailList_mark;

	ImageView* _m_systemMailList_mark;

	ImageView* _m_strengthMailList_mark;

	ImageView* _img_itemBg;

	Button* _mm_refuseInviteBtn;

	Button* _mm_acceptInviteBtn;

	ImageView* _img_itemBg;

	ImageView* _Image_107;

	ImageView* _mm_playerHeadImg;

	Button* _mm_acceptBtn;

	Text* _m_friendMailList_num;

	Text* _m_systemMailList_num;

	Text* _m_strengthMailList_num;

	ImageView* _mm_playerHeadImg;

	Text* _mm_playerNameLabel;

	TextField* _mm_mailItemContentLabel;

	ImageView* _mm_headImg;

	Text* _mm_playerName;

	TextField* _mm_mailItemContentLabel;

	ImageView* _mm_systemNtfImg;

	ImageView* _mm_playerHeadImg;

	Text* _mm_playerNameLabel;

	TextField* _mm_mailItemContentLabel;

};
#endif
