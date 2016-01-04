//// LevelGameOver.h created by php script. chris.li
#ifndef __HiFly__LevelGameOver
#define __HiFly__LevelGameOver

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

class LevelGameOver : public cocos2d::Layer
{
public:
	CREATE_FUNC(LevelGameOver);

	void refreshUI();
    
    void shareToIntenet();
private:
	bool init();
	void onEnter();
	void onExit();
	void loadUI();
	void handleTouchEvent(Ref *pSender, TouchEventType type);

	//gift get btn event
	void touchEventGiftBtn(Ref *pSender, TouchEventType type);

	//get ui reference marked by ui_xxx 
	Button* m_continueBtn;

	Button* m_shareBtn;

	ImageView* m_winPicText;

	ImageView* m_lossPicText;

	Layout* m_lossPic;

	ImageView* m_winPic;

	CheckBox* m_winStar1;

	CheckBox* m_winStar2;

	CheckBox* m_winStar3;

	TextAtlas* m_scoreLabel;

	Text* m_scoreLabelText;

	Text* m_coinLabelText;

	TextAtlas* m_coinLabel;

	//gift layer
	Layout* m_giftPal;

	Button* m_giftBtn;
};
#endif
