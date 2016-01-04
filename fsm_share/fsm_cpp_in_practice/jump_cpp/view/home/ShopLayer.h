#ifndef __HiFly__ShopLayer__
#define __HiFly__ShopLayer__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

//
#include "UiConfig.h"
#include "common/MyButton.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

//define
#define TAG_diamondListBtn 101
#define TAG_goldListBtn 102
#define TAG_hpListBtn 103

#define TAG_diamondListPal 10001
#define TAG_goldListPal 10002
#define TAG_hpListPal 10003

#define TAG_diamondBuy6 901
#define TAG_diamondBuy30 902
#define TAG_diamondBuy50 903
#define TAG_diamondBuy98 904
#define TAG_diamondBuy198 905

#define TAG_goldBuy50 801
#define TAG_goldBuy150 802
#define TAG_goldBuy400 803
#define TAG_goldBuy1000 804
#define TAG_goldBuy2000 805

#define TAG_hpBuy20 701
#define TAG_hpBuy80 702
#define TAG_hpBuy200 703
#define TAG_hpBuy600 704
#define TAG_hpBuy1000 705


class ShopLayer : public cocos2d::Layer
{
public:
    CREATE_FUNC(ShopLayer);
    
	//
	void initShopType(ShopType type);

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

	//msg
	void updateMsg();

  //schedule
	void scheduleHpTime(float delayTime);
	void scheduleMsg(float delayTime);

  //touchEvent
	//~button~
	//assets
	//void touchEventAddHp(Ref *pSender, TouchEventType type);
	//void touchEventAddDiamond(Ref *pSender, TouchEventType type);
	//void touchEventAddGold(Ref *pSender, TouchEventType type);
	void touchEventShopBtn(Ref *pSender, TouchEventType type);

	//shop list button
	void touchEventListBtn(Ref *pSender, TouchEventType type);

	//shop buy button
	void touchEventDiamondBuyBtn(Ref *pSender, TouchEventType type);
	void touchEventGoldBuyBtn(Ref *pSender, TouchEventType type);
	void touchEventHpBuyBtn(Ref *pSender, TouchEventType type);

	//return
	void touchEventReturnBtn(Ref *pSender, TouchEventType type);


  //helper
	void setWidgetButton(Widget* root, const char *name,int tag, SEL_TouchEvent selector);
	//gold buy
	void shopBuyGold(int type,int goodsId);
	//diamond buy
	void shopBuyHp(int type,int goodsId);
    
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

	//~shop list~
	//shop list button
	MyButton* m_diamondListBtn;
	MyButton* m_goldListBtn;
	MyButton* m_hpListBtn;

	//
	Layout* m_diamondListPal;
	Layout* m_goldListPal;
	Layout* m_hpListPal;

	//~return~
	Button* m_returnBtn;

	//msg
	Text* m_msgText;

	//~~~Hp get~~~
	float m_Hptime;
	int m_HpNum;

	//msg time 
	float m_msgTime;
	bool m_msgSwitch;
	bool m_scrollSwitch;
	bool m_noticeSwitch;
	float m_msgEndPosX;
};


#endif //__HiFly__ShopLayer__