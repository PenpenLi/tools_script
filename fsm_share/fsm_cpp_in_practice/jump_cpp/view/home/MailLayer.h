//// MailLayer.h created by php script. chris.li
#ifndef __HiFly__MailLayer
#define __HiFly__MailLayer

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
//
#include "common/MyListView.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

enum MyShowListView
{
	SHOW_strength,
	SHOW_friend,
	SHOW_system
};

class MailUserObject : public cocos2d::Ref
{
public:
    static MailUserObject* create(std::string name, int mailId, int mailType)
    {
        auto ret = new MailUserObject();
        
        ret->init(name, mailId, mailType);
        
        ret->autorelease();
        
        return ret;
    }
    
    void init(std::string name, int mailId, int mailType)
    {
        this->name = name;
        
        this->mailId = mailId;
        
        this->mailType = mailType;
    }
    
    std::string name;
    
    int mailId;
    
    int mailType;
};

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
	Button* m_returnBtn;
    
	Button* m_acceptAllBtn;
    
	Text* m_notifyMsgLabel;
    
	MyListView* m_friendMailListTab;
    
	MyListView* m_systemMailListTab;
    
	MyListView* m_strengthListTab;
    
	Text* m_strengthMaxNum;
    
	Button* m_addStrengthBtn;
    
	Text* m_diamondNum;
    
	Button* m_addDiamondBtn;
    
	Text* m_goldNum;
    
	Button* m_addGoldBtn;
    
	Button* m_friendMailListBtn;
    
	Button* m_systemMailListBtn;
    
	Button* m_strengthListBtn;
    
	Layout* m_friendMailList_inviteItem;
    
	Layout* m_systemMailListItem;
    
	Layout* m_acceptStrengthItem;
    
	LoadingBar* m_strengthBar1;
    
	LoadingBar* m_strengthBar2;
    
	LoadingBar* m_strengthBar3;
    
	LoadingBar* m_strengthBar4;
    
	LoadingBar* m_strengthBar5;
    
	ImageView* m_friendMailList_mark;
    
	ImageView* m_systemMailList_mark;
    
	ImageView* m_strengthMailList_mark;
    
	Text* m_friendMailList_num;
    
	Text* m_systemMailList_num;
    
	Text* m_strengthMailList_num;

	//show listview current
	MyShowListView m_showListView;
	//listView slider
	Slider* m_lvpSlider;
	//gap
	bool m_touchSwitch;
	bool m_sliderSwitch;
	float m_sliderGapMax;
	float m_sliderGap;
	//msg time 
	float m_msgTime;
	bool m_msgSwitch;
	bool m_scrollSwitch;
	bool m_noticeSwitch;
	float m_msgEndPosX;
	
	//Point m_msgBeginPos;
    
    //////////////////////////////// CODING HERE /////////////////////////////////
    void refreshMailList();

	void listViewListenEvent(Ref *pSender, TouchEventType type);

	//schedule
	void update(float delayTime);
	void scheduleHpTime(float delayTime);
	void scheduleMsg(float delayTime);
    
    //refresh
	void refreshHp();
	void refreshDiamond();
	void refreshGold();
	void refreshMsg();

	/////////////////////////////////////member variable///////////
	//HP
	int m_HpNum;
	Vector<LoadingBar*>	m_hpLoadingBarV;


};
#endif
