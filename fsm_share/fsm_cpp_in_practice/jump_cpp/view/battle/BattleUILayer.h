//// BattleUILayer.h created by php script. chris.li
#ifndef __HiFly__BattleUILayer
#define __HiFly__BattleUILayer

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "HPBar.h"
#include "BattleDefine.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

class BattleUILayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(BattleUILayer);
    
    //////////////////added by chris////////////////
    void sceneUpdate(float dt);
    
    void ntf(HeroNtf &heroNtfData);
    
    void readyGo();
    
private:
	bool init();
	void onEnter();
	void onExit();
	void loadUI();
	void handleTouchEvent(Ref *pSender, TouchEventType type);
    
    ////////////////////////// CODING HERE ///////////////
    int m_refreshCounter;

	bool propStart(int propTag);
	void propSetTime(int propTag, int time);
	void propOverTime(int propTag);

	void setScore(std::string score);
	void setGold(std::string gold);

	//CallBack Methods
	void menuCallBackPause(Ref* pSender);
	void menuCallBackContinue(Ref* pSender);
	void menuCallBackGiveUp(Ref* pSender);
	void menuCallBackLeading(Ref* pSender);

	void changeHP(int hp);

	void changeSlider(float p);

	bool changeFriendScore(int score);

	void openPriceLevel();
	//Vector<int> m_propIDs;

	void updateEffectItem();
	
	void leadingPlayer(int page, bool onOff);

    bool m_isReadyGoRunning;
    
    bool m_isPauseRunning;
    
	int m_count;

	int m_oldScore;

	int m_frinedScore;
	int m_playerScore;

	int* m_propIDs;

	int m_currentPriceBarPer;

	int m_leadingPage;

	bool m_leadngPage1IsOK;
	bool m_leadngPage2IsOK;
	bool m_leadngPage3IsOK;
	bool m_leadngPage4IsOK;
	bool m_leadngPage5IsOK;

	float m_prop1; //PROP_SPRINT_TAG 

    float m_prop2; //PROP_BUBBLE_TAG 
    
    float m_prop3; //PROP_JET_TAG  
    
    float m_prop4; //PROP_DIESPRINT_TAG

	float m_prop5; //PROP_HELICOPTER_TAG

	float m_prop6; //PROP_MAGNET_TAG

	float m_prop7; //PROP_SPUERBUBBLE_TAG

	float m_prop8; //PROP_MUSHROOM_TAG

	float m_priceBarPercent;

	float m_effectProp;
	float m_effectRandomProp;
	float m_effectRole;

	float m_goldAddRandomProp;
	float m_goldAddRole;
	float m_goldAddActivity;

	float m_roleScore;

	LabelTTF* m_friendNicknameLable;
	LabelTTF* m_friendScoreLable;

	HPBar* m_sliderBar;
	LabelAtlas* m_scoreLabel;
	LabelAtlas* m_goldLabel;
	
	Menu* m_pauseMenu;
	Menu* m_continueMenu;
	Menu* m_giveupMenu;

	LayerColor* m_maskLayer;

	Sprite* m_heart1;
	Sprite* m_heart2;
	Sprite* m_heart3;

	Sprite* m_heartBG1;
	Sprite* m_heartBG2;
	Sprite* m_heartBG3;
};
#endif
