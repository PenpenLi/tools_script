//// FriendHelpLayer.h created by php script. chris.li
#ifndef __HiFly__FriendHelpLayer
#define __HiFly__FriendHelpLayer

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "common/MyListView.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

class FriendHelpLayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(FriendHelpLayer);
    
     void refreshUI();
    
private:
	bool init();
	void onEnter();
	void onExit();
	void loadUI();
	void handleTouchEvent(Ref *pSender, TouchEventType type);

	//get ui reference marked by ui_xxx 
	Button* m_returnBtn;

	Button* m_startGameBtn;

	ScrollView* m_friendListScrollView;

	Button* m_mailBtn;

	Button* m_taskBtn;

	Button* m_rankBtn;

	Button* m_activityBtn;

	Text* m_notfiyMsgLabel;

	Slider* m_nextPageBtn;

	ListView* m_friendListView;

	Button* m_addStrengthBtn;

	Text* m_strengthMaxNum;

	Button* m_addDiamondBtn;

	Text* m_diamondNum;

	Button* m_addGoldBtn;

	Text* m_goldNum;

	Layout* m_friendListItem;

	LoadingBar* m_strengthBar1;

	LoadingBar* m_strengthBar2;

	LoadingBar* m_strengthBar3;

	LoadingBar* m_strengthBar4;

	LoadingBar* m_strengthBar5;
    
    //tmp value
    Layout* m_friendListItemSelected;

	//////////////////////////////// CODING HERE /////////////////////////////////
	//schedule
	void scheduleHpTime(float delayTime);
	void scheduleMsg(float delayTime);

	//refresh
	void refreshHp();
	void refreshDiamond();
	void refreshGold();
	void refreshMsg();
	void refreshMail();
	void refreshTask();

	/////////////////////////////////////member variable///////////
	//HP
	int m_HpNum;
	Vector<LoadingBar*>	m_hpLoadingBarV;

	//mail & task
	Text* m_mailNumText;
	Text* m_taskNumText;

	//msg time 
	float m_msgTime;
	bool m_msgSwitch;
	bool m_scrollSwitch;
	bool m_noticeSwitch;
	float m_msgEndPosX;

	Text* m_notifyMsgLabel;

};
#endif
