//// NoticeLayer.h created by php script. chris.li
#ifndef __HiFly__NoticeLayer
#define __HiFly__NoticeLayer

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

class NoticeLayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(NoticeLayer);
private:
	bool init();
	void onEnter();
	void onExit();
	void loadUI();
	void handleTouchEvent(Ref *pSender, TouchEventType type);

	//get ui reference marked by ui_xxx 
	Button* m_continue;

	Text* m_label;

};
#endif
