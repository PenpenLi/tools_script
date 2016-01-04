//// NotifyBoxUILayer.h created by php script. chris.li
#ifndef __HiFly__NotifyBoxUILayer
#define __HiFly__NotifyBoxUILayer

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

class NotifyBoxUIHandler : public cocos2d::Ref
{
public:

    static NotifyBoxUIHandler* create(std::function<void()> notifyBoxUIHandler)
    {
        auto ret = new NotifyBoxUIHandler();
        
        ret->autorelease();
        
        ret->notifyBoxUIHandler = notifyBoxUIHandler;
        
        return ret;
    }
    
    std::function<void()> notifyBoxUIHandler;
};


class NotifyBoxUILayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(NotifyBoxUILayer);

	
	static NotifyBoxUILayer* getInstance();
public:
	//NotifyBox Type ID
	//EnableStage : 1
	//GameContinue : 2
	//CallFriend : 3
	//Accept : 4
	//Going : 5
	//This name transport form buttons' name that in the art resource.

	void show(int typeId, std::string str1, std::string str2, std::function<void()> acceptNotifyBoxUIHandler, std::function<void()> cancelNotifyBoxUIHandler);
	void stop();

	void showFourString(int typeId, std::string str1, std::string str2,std::string str3, std::string str4,  std::function<void()> acceptNotifyBoxUIHandler, std::function<void()> cancelNotifyBoxUIHandler);
private:

	void setNotifyBoxType(int typeId);

	bool setAcceptNotifyBoxUIHandler(std::function<void()> notifyBoxUIHandler);
	bool setCancelNotifyBoxUIHandler(std::function<void()> notifyBoxUIHandler);

	void setNotifyBoxText(std::string str1, std::string str2);

	bool init();
	void onEnter();
	void onExit();
	void loadUI();
	void handleTouchEvent(Ref *pSender, TouchEventType type);

	//get ui reference marked by ui_xxx 
	Layout* m_enableStage;

	Layout* m_gameContinue;

	Layout* m_callFriend;

	Layout* m_accept;

	Layout* m_going;

	Layout* m_openLimit;
	
	Layout* m_limitStage;
	
	Layout* m_resourrection;

	Text* m_enableStageText1;

	Text* m_enableStageText2;

	Button* m_enableStageNoBtn;

	Button* m_enableStageYesBtn;

	Text* m_gameContinueText1;

	Text* m_gameContinueText2;

	Button* m_gameContinueNoBtn;

	Button* m_gameContinueYesBtn;

	Text* m_callFriendText1;

	Text* m_callFriendText2;

	Button* m_callFriendNoBtn;

	Button* m_callFriendYesBtn;

	Text* m_acceptText1;

	Text* m_acceptText2;

	Button* m_acceptNoBtn;

	Button* m_acceptYesBtn;

	Text* m_goingText1;

	Text* m_goingText2;

	Button* m_goingNoBtn;

	Button* m_goingYesBtn;

	Text* m_openLimitText1;

	Text* m_openLimitText2;

	Button* m_openLimitNoBtn;

	Button* m_openLimitYesBtn;

	Text* m_limitStageText1;

	Text* m_limitStageText2;

	Text* m_limitStageText3;

	Text* m_limitStageText4;

	Button* m_limitStageNoBtn;

	Button* m_limitStageYesBtn;

	Text* m_resourrectionText1;

	Text* m_resourrectionText2;

	Text* m_resourrectionText3;

	Text* m_resourrectionText4;

	Text* m_resourrectionT;
	Text* m_resourrectionD;

	Button* m_resourrectionNoBtn;

	Button* m_resourrectionYesBtn;

	//Coding Here

	CC_SYNTHESIZE(int, m_notifyBoxId, NotifyBoxID);

	void callAcceptNotifyBoxUIHandler();
	void callCancelNotifyBoxUIHandler();

    NotifyBoxUIHandler* m_acceptNotifyBoxUIHandler;
	NotifyBoxUIHandler* m_cancelNotifyBoxUIHandler;

};
#endif
