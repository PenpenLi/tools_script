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
    
    void refreshUI();
    
    void shareToIntenet();
private:
	bool init();
	void onEnter();
	void onExit();
	void loadUI();
	void handleTouchEvent(Ref *pSender, TouchEventType type);

	int mScore;
	//get ui reference marked by ui_xxx 
	Button* m_goonBtn;

	Button* m_shareBtn;

	ImageView* m_heroImg;

	TextAtlas* m_scoreAtlasLabel;

	Text* m_scoreExtraLabel;

	ImageView* m_maxWeekScoreImg;

	TextAtlas* m_historyHighestScoreAtlasLabel;
    
    TextAtlas* m_coinNum;

	Text* m_coinNumLabel;
};
#endif
