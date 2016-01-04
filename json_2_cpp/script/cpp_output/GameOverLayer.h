//// GameOverLayer.h created by php script. chris.li
#ifndef __HiFly__GameOverLayer
#define __HiFly__GameOverLayer

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

class GameOverLayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(GameOverLayer);
private:
	bool init();
	void onEnter();
	void onExit();
	void loadUI();
	void handleTouchEvent(Ref *pSender, TouchEventType type);

	//get ui reference marked by ui_xxx 
	Layout* _Panel_14;

	ImageView* _Image_1;

	ImageView* _Image_3;

	Button* _m_goonBtn;

	Button* _m_shareBtn;

	ImageView* _Image_2;

	ImageView* _Image_4;

	ImageView* _m_heroImg;

	Text* _Label_8;

	TextAtlas* _m_scoreAtlasLabel;

	ImageView* _Image_22;

	Text* _m_scoreExtraLabel;

	ImageView* _m_maxWeekScoreImg;

	TextAtlas* _m_historyHighestScoreAtlasLabel;

	TextAtlas* _m_coinNum;

};
#endif
