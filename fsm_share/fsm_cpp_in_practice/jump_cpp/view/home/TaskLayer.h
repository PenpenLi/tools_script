//// TaskLayer.h created by php script. chris.li
#ifndef __HiFly__TaskLayer
#define __HiFly__TaskLayer

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "JsonConfigUtil.h"
#include "../common/MyListView.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;


class TaskUserObject : public cocos2d::Ref
{
public:
    static TaskUserObject* create(std::string name, int taskId, int taskState)
    {
        auto ret = new TaskUserObject();
        
        ret->init(name, taskId, taskState);
        
        ret->autorelease();
        
        return ret;
    }
    
    void init(std::string name, int taskId, int taskState)
    {
        this->name = name;
        
        this->taskId = taskId;
        
        this->taskState = taskState;
    }
    
    std::string name;
    
    int taskId;
    
    int taskState;
};

class TaskLayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(TaskLayer);
private:
	bool init();
	void onEnter();
	void onExit();
	void loadUI();
	void handleTouchEvent(Ref *pSender, TouchEventType type);

	//get ui reference marked by ui_xxx 
	Button* m_returnBtn;

	//ImageView* m_notifyMsgLabel;

	ScrollView* m_taskListScrollView;

	MyListView* m_taskListView;

	Text* m_strengthMaxNum;

	Button* m_addStrengthBtn;

	Text* m_diamondNum;

	Button* m_addDiamondBtn;

	Text* m_goldNum;

	Button* m_addGoldBtn;

	Layout* m_taskListItem;

	LoadingBar* m_strengthBar1;

	LoadingBar* m_strengthBar2;

	LoadingBar* m_strengthBar3;

	LoadingBar* m_strengthBar4;

	LoadingBar* m_strengthBar5;
    
	Button* mm_acceptBtn;
	
    //////////////// CODING HERE ////////////////////

	void listenEventTaskSlider(Ref *pSender, TouchEventType type);

    void refreshUI();

	void afterNetwork();

	void update(float delayTime);

	Slider* m_taskSlider;

	float m_sliderGapMax;
	float m_sliderGap;
	bool m_sliderSwitch;

	//schedule
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

	//msg time 
	float m_msgTime;
	bool m_msgSwitch;
	bool m_scrollSwitch;
	bool m_noticeSwitch;
	float m_msgEndPosX;

	Text* m_notifyMsgLabel;
};
#endif
