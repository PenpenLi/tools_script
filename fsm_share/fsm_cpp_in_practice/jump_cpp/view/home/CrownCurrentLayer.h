//// CrownLayer.h created by php script. chris.li
#ifndef __HiFly__CrownCurrentLayer
#define __HiFly__CrownCurrentLayer

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

class CrownCurrentLayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(CrownCurrentLayer);
private:
	int rankN;
	bool init();
	void refreshUI();
	void onEnter();
	void onExit();
	void loadUI();
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
