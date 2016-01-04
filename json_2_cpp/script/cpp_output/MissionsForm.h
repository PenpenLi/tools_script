//// MissionsForm.h created by php script. chris.li
#ifndef __HiFly__MissionsForm
#define __HiFly__MissionsForm

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

class MissionsForm : public cocos2d::Layer
{
public:
	CREATE_FUNC(MissionsForm);
private:
	bool init();
	void onEnter();
	void onExit();
	void loadUI();
	void handleTouchEvent(Ref *pSender, TouchEventType type);

	//get ui reference marked by ui_xxx 
	Layout* _Node;

	ImageView* _fondo_Image;

	ImageView* _line1_Image;

	ImageView* _line2_Image;

	ImageView* _line3_Image;

	LabelBMFont* _title_Label;

	LabelBMFont* _level_Label;

	ImageView* _range_Image;

	Layout* _mision1_Panel;

	Layout* _mision2_Panel;

	Layout* _mision3_Panel;

	Layout* _mision4_Panel;

	Layout* _labelContainer;

	ImageView* _mision1_Image;

	LabelBMFont* _mision1_Label;

	Button* _skip1_Button;

	ImageView* _mision2_Image;

	LabelBMFont* _mision2_Label;

	Button* _skip2_Button;

	ImageView* _mision3_Image;

	LabelBMFont* _mision3_Label;

	Button* _skip3_Button;

	ImageView* _mision4_Image;

	LabelBMFont* _mision4_Label;

	Button* _skip4_Button;

	LabelBMFont* _skip1_Label;

	LabelBMFont* _skip2_Label;

	LabelBMFont* _skip3_Label;

	LabelBMFont* _skip4_Label;

};
#endif
