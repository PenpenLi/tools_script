//// FriendHelpLayer.h created by php script. chris.li
#ifndef __HiFly__FriendHelpLayer
#define __HiFly__FriendHelpLayer

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

class FriendHelpLayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(FriendHelpLayer);
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

	Layout* _Panel_54;

	ImageView* _Image_86;

	Button* _m_returnBtn;

	Button* _m_startGameBtn;

	Layout* _Panel_90;

	ScrollView* _m_friendListScrollView;

	ImageView* _Image_2;

	ImageView* _Image_4;

	ImageView* _Image_4_0;

	ImageView* _Image_4_0_1;

	Button* _m_mailBtn;

	Button* _m_taskBtn;

	Button* _m_rankBtn;

	Button* _m_activityBtn;

	Text* _m_notfiyMsgLabel;

	ImageView* _Image_91;

	Slider* _m_nextPageBtn;

	ListView* _m_friendListView;

	ImageView* _Image_5;

	ImageView* _Image_5_0;

	ImageView* _Image_5_1;

	ImageView* _strengthBar4;

	ImageView* _Image_5_3;

	Button* _m_addStrengthBtn;

	Text* _m_strengthMaxNum;

	ImageView* _Image_5;

	Button* _m_addDiamondBtn;

	Text* _m_diamondNum;

	ImageView* _Image_5;

	Button* _m_addGoldBtn;

	Text* _m_goldNum;

	ImageView* _Image_56;

	Text* _Label_57;

	ImageView* _Image_58;

	ImageView* _Image_56;

	Text* _Label_57;

	ImageView* _Image_58;

	ImageView* _Image_56;

	Text* _Label_57;

	ImageView* _Image_56;

	Text* _Label_57;

	ImageView* _Image_92;

	Layout* _m_friendListItem;

	LoadingBar* _m_strengthBar1;

	LoadingBar* _m_strengthBar2;

	LoadingBar* _m_strengthBar3;

	LoadingBar* _m_strengthBar4;

	LoadingBar* _m_strengthBar5;

	Text* _Label_59;

	Text* _Label_59;

	ImageView* _Image_97;

	Button* _mm_chooseBtn;

	ImageView* _mm_headImg;

	ImageView* _mm_heroImg;

	Text* _mm_friendName;

	TextAtlas* _mm_scoreAtlasLabel;

	ImageView* _Image_106;

	ImageView* _Image_107;

	Text* _Label_108;

	ImageView* _Image_100;

};
#endif
