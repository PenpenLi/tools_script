//// LoginRewardTotalLayer.h created by php script. chris.li
#ifndef __HiFly__LoginRewardTotalLayer
#define __HiFly__LoginRewardTotalLayer

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

class LoginRewardTotalLayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(LoginRewardTotalLayer);
private:
	bool init();
	void onEnter();
	void onExit();
	void loadUI();
	void handleTouchEvent(Ref *pSender, TouchEventType type);

	//
	cocos2d::Vector<ImageView*> m_imgViewList;
	//get ui reference marked by ui_xxx 
	Button* m_getRewardBtn;

	ImageView* m_day_1;

	ImageView* m_day_2;

	ImageView* m_day_3;

	ImageView* m_day_4;

	ImageView* m_day_5;

	ImageView* m_day_6;

	ImageView* m_day_7;

};
#endif
