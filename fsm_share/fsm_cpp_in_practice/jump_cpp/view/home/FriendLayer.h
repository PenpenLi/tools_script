#ifndef __HiFly__FriendLayer__
#define __HiFly__FriendLayer__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

//
#include "common/MyListView.h"
#include "common/MyButton.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

//define
#define TAG_friendBtn 101
#define TAG_nearbyFriendBtn 102
#define TAG_platformFriendBtn 103

#define TAG_addfriendBtn 201
#define TAG_deletefriendBtn 202

#define TAG_sendHpBtn 200
#define TAG_inviteBtn 300

#define TAG_freidPal 1000
#define TAG_nearbyFriendPal 2000

#define TAG_friendLvp 10001
#define TAG_nearbyFriendLvp 10002
#define TAG_platformFriendLvp 10003

#define GAP_turnPage 120.0f

class FriendLayer : public cocos2d::Layer
{
public:
    CREATE_FUNC(FriendLayer);
    
private:
    bool init();
    
    void onEnter();
    
    void onExit();
    
//function
	//init
	void variableInit();
	//import login Res 
	bool importUI();

  //update
	//Assets
	void updateHp();
	void updateDiamond();
	void updateGold();
	//activity
	void updateMail();
	void updateTask();
	//msg
	void updateMsg();

	//Friend List
	void updateFriend();
	void updateNearbyFriend();
	void updatePlatformFriend();

	//Friend List Page
	//void updateFriendPageCount();

	//friend info
	void updateFriendInfoPal(int type);

  //schedule
	void update(float delayTime);
	void scheduleHpTime(float delayTime);
	void scheduleMsg(float delayTime);

  //touchEvent
	//~button~
	//assets
	void touchEventShopBtn(Ref *pSender, TouchEventType type);
	//void touchEventAddHp(Ref *pSender, TouchEventType type);
	//void touchEventAddDiamond(Ref *pSender, TouchEventType type);
	//void touchEventAddGold(Ref *pSender, TouchEventType type);
	//activity
	void touchEventMail(Ref *pSender, TouchEventType type);
	void touchEventTask(Ref *pSender, TouchEventType type);
	void touchEventRank(Ref *pSender, TouchEventType type);
	void touchEventActivity(Ref *pSender, TouchEventType type);
	//friend
	void touchEventFriendBtn(Ref *pSender, TouchEventType type);
	//void touchEventNearbyFriend(Ref *pSender, TouchEventType type);
	//void touchEventPlatformFriend(Ref *pSender, TouchEventType type);
	void touchEventFriendInfoBtn(Ref *pSender, TouchEventType type);

	//send hp
	void touchEventSendHp(Ref *pSender, TouchEventType type);
	//Invite
	void touchEventInvite(Ref *pSender, TouchEventType type);
	//return
	void touchEventReturn(Ref *pSender, TouchEventType type);
	void touchEventInfoReturn(Ref *pSender, TouchEventType type);

	//
	void listenEventFriendLvp(Ref *pSender, TouchEventType type);
	//void listenEventFriendItem(Ref *pSender, TouchEventType type);

  //helper
	//std::string changeToScoreString(int score);
    
//member variable
  //~Panel Assets~
	//HP
	Vector<LoadingBar*>	m_hpLoadingBarV;
	Text*	m_hpTimeText;
	Button* m_hpAddBtn;

    //diamond
	Text* m_diamondNumText;
	Button*	m_diamondAddBtn;

	//gold
	Text* m_goldNumText;
	Button* m_goldAddBtn;

  //~Panel Activity~
	//mail
	Button*	m_mailBtn;
	Text* m_mailNumText;

	//task
	Button*	m_taskBtn;
	Text* m_taskNumText;

	//rank
	Button*	m_rankBtn;

	//activity
	Button*	m_activityBtn;

	//message label
	Text* m_msgText;

  //~friend~
	Slider* m_lvpSlider;
	MyButton* m_friendBtn;
	MyButton* m_nearbyFriendBtn;
	MyButton* m_platformFriendBtn;

	ScrollView* m_friendSvp;

	MyListView* m_friendLvp;
	MyListView* m_nearbyFriendLvp;
	MyListView* m_platformFriendLvp;

	Layout* m_friendSinglePal;
	Layout* m_friendDualPal;
	Layout* m_nearbyFriendSinglePal;
	Layout* m_nearbyFriendDualPal;
	Layout* m_platformFriendSinglePal;
	Layout* m_platformFriendDualPal;

	//list page count
	//int m_friendPage;
	//int m_friendPageSum;
	//int m_nearbyFriendPage;
	//int m_nearbyFriendPageSum;
	//int m_platformFriendPage;
	//int m_platformFriendPageSum;

		//common page
	int m_page;
	int m_pageSum;

	//friend info
	Layout* m_friendInfoPal;

	int m_infoPlayerId;

	ImageView* m_playerHeadImg;
	TextAtlas* m_scoreText;
	TextAtlas* m_levelsText;
	Button* m_addFriendBtn;
	Button* m_deleteFriendBtn;

  //~return~
	Button* m_returnBtn;

	//Hp get
	float m_Hptime;
	int m_HpNum;

	//Switch
	bool m_touchSwitch;
	bool m_sliderSwitch;

  //gap
	float m_sliderGapMax;
	float m_sliderGap;

	//msg time 
	float m_msgTime;
	bool m_msgSwitch;
	bool m_scrollSwitch;
	bool m_noticeSwitch;
	float m_msgEndPosX;

	//friend number label
	Text* m_friendNumText;

//test
};


#endif //__HiFly__FriendLayer__