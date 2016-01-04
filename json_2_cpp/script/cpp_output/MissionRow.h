//// MissionRow.h created by php script. chris.li
#ifndef __HiFly__MissionRow
#define __HiFly__MissionRow

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

class MissionRow : public cocos2d::Layer
{
public:
	CREATE_FUNC(MissionRow);
private:
	bool init();
	void onEnter();
	void onExit();
	void loadUI();
	void handleTouchEvent(Ref *pSender, TouchEventType type);

	//get ui reference marked by ui_xxx 
	Layout* _Node;

	ImageView* _icon_Image;

	LabelBMFont* _label_Label;

	LabelBMFont* _shadow_Label;

};
#endif
