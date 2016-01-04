#ifndef __HiFly__MainLayer__
#define __HiFly__MainLayer__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

class MainLayer : public cocos2d::Layer
{
public:
    CREATE_FUNC(MainLayer);
    
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
	//role image
	void updateRoleImg();

	//return main layer and update

  //schedule
	void scheduleHpTime(float delayTime);
	void scheduleMsg(float delayTime);

  //touchEvent
	//assets
	//void touchEventAddHp(Ref *pSender, TouchEventType type);
	//void touchEventAddDiamond(Ref *pSender, TouchEventType type);
	//void touchEventAddGold(Ref *pSender, TouchEventType type);
	void touchEventShopBtn(Ref *pSender, TouchEventType type);
	//activity
	void touchEventMail(Ref *pSender, TouchEventType type);
	void touchEventTask(Ref *pSender, TouchEventType type);
	void touchEventRank(Ref *pSender, TouchEventType type);
	void touchEventActivity(Ref *pSender, TouchEventType type);
	//Battle
	void touchEventBattleLevel(Ref *pSender, TouchEventType type);
	void touchEventBattleEndless(Ref *pSender, TouchEventType type);
	//Control
	void touchEventControlMain(Ref *pSender, TouchEventType type);
	void touchEventControlRole(Ref *pSender, TouchEventType type);
	void touchEventControlFriend(Ref *pSender, TouchEventType type);
	void touchEventControlSet(Ref *pSender, TouchEventType type);
	//role img
	void touchEventRoleImg(Ref *pSender, TouchEventType type);
    
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

	//~Panel battle~
	Button* m_battleLevelBtn;

	Button* m_battleEndlessBtn;

	//~Panel Controll~
	Button* m_mainBtn;

	Button* m_roleBtn;

	Button* m_friendBtn;

	Button* m_setupBtn;

	//role img
	ImageView* m_roleImage;

	//Layout* m_rolePal;

	//message label
	Text* m_msgText;

	//Hp get
	float m_Hptime;
	int m_HpNum;

	//msg time 
	float m_msgTime;
	bool m_msgSwitch;
	bool m_scrollSwitch;
	bool m_noticeSwitch;
	float m_msgEndPosX;

//animation
	Armature* m_armatureHero_1;
	Armature* m_armatureHero_2;

};

#endif //__HiFly__MainLayer__