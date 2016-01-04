//// EnterLayer.h created by php script. chris.li
#ifndef __HiFly__EnterLayer
#define __HiFly__EnterLayer

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

class EnterLayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(EnterLayer);
private:
	bool init();
	void onEnter();
	void onExit();
	void loadUI();
	void handleTouchEvent(Ref *pSender, TouchEventType type);
	void update(float dt);
	void btnAnimation(Button& btn);

	//get ui reference marked by ui_xxx 
	ImageView* m_bg;

	ImageView* m_img;

	Button* m_startBtn;

};
#endif
