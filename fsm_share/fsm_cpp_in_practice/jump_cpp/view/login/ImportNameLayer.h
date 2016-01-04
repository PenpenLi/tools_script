//// ImportNameLayer.h created by php script. chris.li
#ifndef __HiFly__ImportNameLayer
#define __HiFly__ImportNameLayer

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

class ImportNameLayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(ImportNameLayer);
private:
	bool init();
	void onEnter();
	void onExit();
	void loadUI();
	void handleTouchEvent(Ref *pSender, TouchEventType type);

	//get ui reference marked by ui_xxx 
	Button* m_startGameBtn;

	Text* m_name;

	Button* m_randomNameBtn;

	TextField* m_inputName;

};
#endif
