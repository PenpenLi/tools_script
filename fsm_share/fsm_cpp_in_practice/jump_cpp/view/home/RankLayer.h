#ifndef __HiFly__RankLayer__
#define __HiFly__RankLayer__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "NetWork.h"
#include "Player.h"

//
#include "common/MyListView.h"
#include "common/MyButton.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

//define
#define TAG_platformRankBtn 101
#define TAG_worldRankBtn 102
#define TAG_GCBtn 103

#define TAG_platformRankPal 1000
#define TAG_worldRankPal 2000

#define TAG_platformRankLvp 10001
#define TAG_worldRankLvp 10002
#define TAG_GCLvp 10003

#define GAP_turnPage 120.0f

enum RankType
{
	RANK_platform,
	RANK_world
};

class RankLayer : public cocos2d::Layer
{
public:
    CREATE_FUNC(RankLayer);
    
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

	//Rank List
	void updateRankLvp(RankType type);

	//Rank List Page
	//void updateRankPageCount();

	//player info
	void updatePlayerInfoPal(int type);

	//msg
	void updateMsg();

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

	//rank button
	void touchEventRankBtn(Ref *pSender, TouchEventType type);

	//return
	void touchEventReturnBtn(Ref *pSender, TouchEventType type);
	void touchEventInfoReturnBtn(Ref *pSender, TouchEventType type);

    //pal touch
	void listenEventRankLvp(Ref *pSender, TouchEventType type);
    
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

  //~rank~
	Slider* m_lvpSlider;
	MyButton* m_platformRankBtn;
	MyButton* m_worldRankBtn;
	MyButton* m_GCBtn;

	ScrollView* m_rankSvp;

	MyListView* m_platformRankLvp;
	MyListView* m_worldRankLvp;
	MyListView* m_GCLvp;
	
	//model
	Layout* m_rankSinglePal;
	Layout* m_rankDualPal;

	//list page count
	int m_platformRankPage;
	int m_platformRankPageSum;
	int m_worldRankPage;
	int m_worldRankPageSum;

		//
	int m_page;
	int m_pageSum;
	
	//player info
	Layout* m_playerInfoPal;

	ImageView* m_playerHeadImg;
	TextAtlas* m_scoreText;
	TextAtlas* m_levelsText;
	Button* m_infoReturnBtn;

	//msg
	Text* m_msgText;

	//~return~
	Button* m_returnBtn;

	//~~~Hp get~~~
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
};


#endif //__HiFly__RankLayer__