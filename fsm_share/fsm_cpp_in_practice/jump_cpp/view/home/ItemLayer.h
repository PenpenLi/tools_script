#ifndef __HiFly__ItemLayer__
#define __HiFly__ItemLayer__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

#define TAG_blessingBtn 921
#define TAG_beginSpurtBtn 922
#define TAG_protectBubbleBtn 923
#define TAG_deathSpurtBtn 924
#define TAG_scoreBuffBtn 925

class ItemLayer : public cocos2d::Layer
{
public:
    CREATE_FUNC(ItemLayer);
    
private:
    bool init();
    
    void onEnter();
    
    void onExit();
    
//~~~~~~~~~~~~function~~~~~~~~~~~~
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

	//item
	void updateItemNum();

	//msg
	void updateMsg();

  //schedule
	void scheduleHpTime(float delayTime);
	void scheduleMsg(float delayTime);

  //touchEvent
	//assets
	void touchEventShopBtn(Ref *pSender, TouchEventType type);
	//activity
	void touchEventActBtn(Ref *pSender, TouchEventType type);
	
	//item buy
	void touchEventItemBuyBtn(Ref *pSender, TouchEventType type);

	//continue
	void touchEventContinueBtn(Ref *pSender, TouchEventType type);
	//return
	void touchEventReturnBtn(Ref *pSender, TouchEventType type);


  //helper
	void setWidgetButton(Widget* root, const char *name,int tag, SEL_TouchEvent selector);
	//item Add
	void itemAdd(int type,int num);
    
//~~~~~~~~~~~~~~~~~~~~~~~~member variable~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~Panel Assets~
	Layout* m_assetsPal;
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
	Layout* m_activityPal;
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

	//item num
	Text* m_beginSpurtNum;
	Text* m_protectBubbleNum;
	Text* m_deathSpurtNum;
	Text* m_scoreBuffNum;

	Text* m_randomProps;
	
	//Text* m_
	//~message label~
	Text* m_msgText;

	//~continue~
	Button* m_continueBtn;
    //~return~
	Button* m_returnBtn;

	//Hp get
	float m_Hptime;
	int m_HpNum;

	//msg time 
	float m_msgTime;
	bool m_msgSwitch;
	bool m_scrollSwitch;
	bool m_noticeSwitch;
	float m_msgEndPosX;

	//
	//bool m_isLayer;
};


#endif //__HiFly__ItemLayer__