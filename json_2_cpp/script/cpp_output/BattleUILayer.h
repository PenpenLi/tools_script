//// BattleUILayer.h created by php script. chris.li
#ifndef __HiFly__BattleUILayer
#define __HiFly__BattleUILayer

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

class BattleUILayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(BattleUILayer);
private:
	bool init();
	void onEnter();
	void onExit();
	void loadUI();
	void handleTouchEvent(Ref *pSender, TouchEventType type);

	//get ui reference marked by ui_xxx 
	Layout* _Panel_14;

	Layout* _Panel_73;

	Layout* _m_pauseMenu;

	Layout* _Panel_8;

	ImageView* _Image_52;

	TextAtlas* _m_myScore;

	ImageView* _m_prop2;

	ImageView* _m_prop1;

	ImageView* _m_prop3;

	Button* _m_pauseBtn;

	Slider* _m_enterRewardLevelProgressBar;

	Button* _m_continueGameBtn;

	Button* _m_giveupGameBtn;

	ImageView* _Image_6;

	ImageView* _Image_6_0;

	TextAtlas* _m_myCoin;

	ImageView* _Image_99;

	ImageView* _Image_99;

	ImageView* _Image_99;

	ImageView* _Image_213;

	ImageView* _m_myHeadImg;

	Text* _m_myName;

	ImageView* _Image_13;

	ImageView* _Image_13_0;

	ImageView* _Image_13_0_1;

	ImageView* _m_friendHeadImg;

	Text* _m_friendName;

	Text* _m_friendScore;

	LoadingBar* _m_myHP1;

	LoadingBar* _m_myHP2;

	LoadingBar* _m_myHP3;

};
#endif
