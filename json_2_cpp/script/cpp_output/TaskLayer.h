//// TaskLayer.h created by php script. chris.li
#ifndef __HiFly__TaskLayer
#define __HiFly__TaskLayer

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

class TaskLayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(TaskLayer);
private:
	bool init();
	void onEnter();
	void onExit();
	void loadUI();
	void handleTouchEvent(Ref *pSender, TouchEventType type);

	//get ui reference marked by ui_xxx 
	Layout* _Panel_14;

	ImageView* _Image_1;

	Layout* _Panel_5;

	Button* _m_returnBtn;

	ImageView* _m_notifyMsgLabel;

	Layout* _Panel_65;

	ScrollView* _m_taskListScrollView;

	ImageView* _Image_2;

	ImageView* _Image_6;

	ImageView* _Image_6_0;

	ImageView* _Image_6_0_1;

	Text* _Label_64;

	ImageView* _Image_66;

	Slider* _Slider_68;

	ListView* _m_taskListView;

	ImageView* _strengthImag;

	ImageView* _strengthImg2;

	ImageView* _Image_7_0_1;

	ImageView* _Image_7_0_1_2;

	ImageView* _Image_7_0_1_2_3;

	Text* _m_strengthMaxNum;

	Button* _m_addStrengthBtn;

	ImageView* _Image_7;

	Text* _m_diamondNum;

	Button* _m_addDiamondBtn;

	ImageView* _Image_7;

	Text* _m_goldNum;

	Button* _m_addGoldBtn;

	ImageView* _Image_67;

	Layout* _m_taskListItem;

	LoadingBar* _m_strengthBar1;

	LoadingBar* _m_strengthBar2;

	LoadingBar* _m_strengthBar3;

	LoadingBar* _m_strengthBar4;

	LoadingBar* _m_strengthBar5;

	ImageView* _item_bg;

	Text* _mm_taskDespLabel;

	Button* _mm_acceptBtn;

	ImageView* _mm_taskImg;

	ImageView* _Image_79;

	LoadingBar* _mm_taskProgressBar;

	TextAtlas* _mm_taskProgressLabel;

};
#endif
