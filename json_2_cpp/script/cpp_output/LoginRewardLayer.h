//// LoginRewardLayer.h created by php script. chris.li
#ifndef __HiFly__LoginRewardLayer
#define __HiFly__LoginRewardLayer

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

class LoginRewardLayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(LoginRewardLayer);
private:
	bool init();
	void onEnter();
	void onExit();
	void loadUI();
	void handleTouchEvent(Ref *pSender, TouchEventType type);

	//get ui reference marked by ui_xxx 
	Layout* _Panel_14;

	ImageView* _Image_1;

	Layout* _Panel_3;

	Button* _m_getRewardBtn;

	Layout* _Panel_43;

	Layout* _Panel_43_0;

	ImageView* _Image_2;

	ImageView* _Image_4;

	ImageView* _m_day_1;

	ImageView* _m_day_2;

	ImageView* _m_day_3;

	ImageView* _m_day_4;

	ImageView* _m_day_5;

	ImageView* _m_day_6;

	ImageView* _m_day_7;

	ImageView* _Image_5;

	ImageView* _Image_95;

	Text* _Label_96;

	Text* _Label_9;

	Text* _Label_9_0;

	ImageView* _Image_10;

	Text* _Label_9;

	Text* _Label_9_0;

	ImageView* _Image_10;

	Text* _Label_9;

	Text* _Label_9_0;

	ImageView* _Image_10;

	Text* _Label_9;

	Text* _Label_9_0;

	ImageView* _Image_10;

	Text* _Label_9;

	Text* _Label_9_0;

	ImageView* _Image_10;

	Text* _Label_9;

	Text* _Label_9_0;

	ImageView* _Image_10;

	Text* _Label_9;

	Text* _Label_9_0;

	ImageView* _Image_10;

};
#endif
