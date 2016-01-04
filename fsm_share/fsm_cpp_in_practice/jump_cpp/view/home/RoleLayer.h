#ifndef __HiFly__RoleLayer__
#define __HiFly__RoleLayer__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "common/MyScrollView.h"
#include <vector>

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

#define TAG_RoleImg

struct roleInfo_s
{
	ImageView* roleImg;
	Layout* roleInfoPal;
	Text* lvNumText;
	LoadingBar* lvLoadingBar;
};

struct prop_s
{
	ImageView* maxImg;
	Text* infoText;
	LoadingBar* lvLoadingBar;
};

enum MyButtonState
{
	STATE_null,
	STATE_levelUp,
	STATE_buy
};

class RoleLayer : public cocos2d::Layer
{
public:
    CREATE_FUNC(RoleLayer);
    
private:
    bool init();
    
    void onEnter();
    
    void onExit();
    
//function
	//import login Res 
	bool importUI();

  //update
	//Assets
	void updateHp();
	void updateDiamond();
	void updateGold();
	//msg
	void updateMsg();

	//scrollView
	void updateRoleState();

	//level up info
	void updateLevelUpInfo();

	//role info
	void updateRoleInfo();

  //schedule
	void update(float delayTime);
	void scheduleHpTime(float delayTime);
	//msg
	void scheduleMsg(float delayTime);

  //touchEvent
	//assets
	void touchEventShopBtn(Ref *pSender, TouchEventType type);
	//void touchEventAddHp(Ref *pSender, TouchEventType type);
	//void touchEventAddDiamond(Ref *pSender, TouchEventType type);
	//void touchEventAddGold(Ref *pSender, TouchEventType type);

	//ScrollView
	void touchEventRoleSvp(Ref *pSender, TouchEventType type);

	//void touchEventRoleImg(Ref *pSender, TouchEventType type);

	//level up Info
	void touchEventLvUp(Ref *pSender, TouchEventType type);
	//return
	void touchEventReturn(Ref *pSender, TouchEventType type);
    
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
	
  //~scrollview role select~
	MyScrollView* m_roleSelectSvp;

	std::vector<roleInfo_s> m_roleInfoS;

	Vector<ImageView*>* m_roleImgV;

	Vector<ImageView*>* m_selectRoleImgV;

	ImageView* m_roleLock;

  //lv up Info
	std::vector<prop_s> m_propS;

	Text* m_roleInfoText;
    
    //Test Label
    Label* m_roleInfoLabel;

	Text* m_scoreBuffText;
	Text* m_goldBuffText;
	Text* m_propBuffText;

	Button* m_lvUpButton;
	//Button state
	MyButtonState m_buttonState;

	Text* m_lvUpCostText;

	ImageView* m_goldCostImg;
	ImageView* m_diamondCostImg;
	ImageView* m_lvUpImg;
	ImageView* m_breakImg;
	ImageView* m_buyImg;

  //return
	Button* m_returnBtn;

  //notice
	Text* m_msgText;

	//role id
	int m_frontRoleId;

	//Hp get
	float m_Hptime;
	int m_HpNum;

	//msg time 
	float m_msgTime;
	bool m_msgSwitch;
	bool m_scrollSwitch;
	bool m_noticeSwitch;
	float m_msgEndPosX;

//test
};


#endif //__HiFly__RoleLayer__