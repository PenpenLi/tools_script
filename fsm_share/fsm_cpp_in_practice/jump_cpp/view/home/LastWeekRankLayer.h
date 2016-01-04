//// LastWeekRankLayer.h created by php script. chris.li
#ifndef __HiFly__LastWeekRankLayer
#define __HiFly__LastWeekRankLayer

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

class LastWeekRankLayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(LastWeekRankLayer);

	void refreshUI();

private:
	bool init();
	void onEnter();
	void onExit();
	void loadUI();
	void handleTouchEvent(Ref *pSender, TouchEventType type);
	int rankN;

	//get ui reference marked by ui_xxx 
	ScrollView* m_rankListScrollView;

	Button* m_ContinueBtn;

	ListView* m_rankListView;

	Layout* m_rankListItem;

};
#endif
