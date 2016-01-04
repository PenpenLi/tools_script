#ifndef __HiFly__SetLayer__
#define __HiFly__SetLayer__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "UiConfig.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

#define TAG_setBtnQA 801
#define TAG_setBtnNovice 802
#define TAG_setBtnFeedback 803
#define TAG_setBtnThanks 804

#define TAG_musicBg 701
#define TAG_soundEffect 702

enum SensitivityCb
{
	TAG_sensitivityCb = 900,
	TAG_sensitivityCbX1 = 901,
	TAG_sensitivityCbX2 = 902,
	TAG_sensitivityCbX3 = 903
};

class SetLayer : public cocos2d::Layer
{
public:
    CREATE_FUNC(SetLayer);
    
private:
    bool init();
    
    void onEnter();
    
    void onExit();
    
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~function~~~~~~~~~~~~~~~~~~~~~~~
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

	//set panel
	void updateSetPal();

	//msg
	void updateMsg();

  //schedule
	void scheduleHpTime(float delayTime);
	void scheduleFeedback(float delayTime);
	void scheduleMsg(float delayTime);

  //touchEvent
	//assets
	void touchEventShopBtn(Ref *pSender, TouchEventType type);
	//void touchEventAddHp(Ref *pSender, TouchEventType type);
	//void touchEventAddDiamond(Ref *pSender, TouchEventType type);
	//void touchEventAddGold(Ref *pSender, TouchEventType type);
	//activity
	void touchEventActBtn(Ref *pSender, TouchEventType type);
	//send heart CheckBox
	void touchEventHeartCheckBox(Ref *pSender, TouchEventType type);
	//Sensitivity CheckBox
	void touchEventSensitivityCb(Ref *pSender, TouchEventType type);
	//music & sound slider
	void touchEventSoundSdr(Ref *pSender, SliderEventType type);

	//set button
	void touchEventSetBtn(Ref *pSender, TouchEventType type);

	//return
	void touchEventReturnBtn(Ref *pSender, TouchEventType type);
	void touchEventQAReturnBtn(Ref *pSender, TouchEventType type);
	
	//panel button
	void touchEventFeedbackBtn(Ref *pSender, TouchEventType type);
	void touchEventThanksPal(Ref *pSender, TouchEventType type);
	void touchEventFaleBtn(Ref *pSender, TouchEventType type);

	//gift
	void touchEventGiftBtn(Ref *pSender, TouchEventType type);
	void touchEventGiftPalBtn(Ref *pSender, TouchEventType type);

  //textfield event
	void textFieldEventFeedback(Ref *pSender,TextFiledEventType type);

//helper
	void uploadingSetup();
    
//~~~~~~~~~~~~~~~~~~~~~~~~~~member variable~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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

  //~pal set~
	Layout* m_setPal;
	CheckBox* m_sendHeartCb;
	Slider* m_musicBgSdr;
	Slider* m_soundEffectSdr;
	Vector<CheckBox*> m_sensitivityCbV;
	Button* m_platformBtn;
	Button* m_QABtn;
	Button* m_noviceBtn;
	Button* m_feedbackBtn;
	Button* m_thanksBtn;
  //~pal QA~
	Layout* m_QAPal;
	Button* m_QAReturnBtn;
  //~pal feedback~
	Layout* m_feedbackPal;
	TextField* m_feedbackText;
	Button* m_sendFeedbackBtn;
	Button* m_feedbackReturnBtn;
	Text* m_strNumText;
  //~pal thanks~
	Layout* m_thanksPal;
	Text* m_nickNameText;
	Text* m_highestScoreText;
	Text* m_highestLevelsText;
	Button*	m_taleBtn;

  //~pal gift~
	Layout* m_giftPal;
	TextField* m_giftTF;
	Button* m_giftGetBtn;
	Button* m_giftReturnBtn;

	Button* m_giftBtn;

    //~message label~
	ImageView* m_msgImg;
	Text* m_msgText;

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

//test
};


#endif //__HiFly__SetLayer__