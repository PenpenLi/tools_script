//// CrownLayer.h created by php script. chris.li
#ifndef __HiFly__CrownLayer
#define __HiFly__CrownLayer

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

class CrownLayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(CrownLayer);
private:
	int rankN;
	bool init();
	void onEnter();
	void onExit();
	void loadUI();
	void refreshUI();
	void handleTouchEvent(Ref *pSender, TouchEventType type);

	//get ui reference marked by ui_xxx 
	Layout* rootNode;

	Layout* m_current1;

	Layout* m_current2;

	Layout* m_current3;

	Layout* m_last1;

	Layout* m_last2;

	Layout* m_last3;

	Button* m_continueBtn;

	Button* m_shareBtn;

};
#endif
