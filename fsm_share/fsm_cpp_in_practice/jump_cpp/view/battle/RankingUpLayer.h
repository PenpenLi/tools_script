//// RankingUpLayer.h created by php script. chris.li
#ifndef __HiFly__RankingUpLayer
#define __HiFly__RankingUpLayer

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

class RankingUpLayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(RankingUpLayer);
private:
	bool init();
	void refreshUI();
	void onEnter();
	void onExit();
	void loadUI();
	void handleTouchEvent(Ref *pSender, TouchEventType type);
	void moveItem();

	//get ui reference marked by ui_xxx 
	Button* m_continueBtn;

	Button* m_shareBtn;

	ImageView* m_me;

	ImageView* m_friend;

	int index;
	int myScore;
	int myRank;
	int otherScore;
};
#endif
