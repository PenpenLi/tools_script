//
//  LoginLayer.h
//  HiFly
//
//  Created by chris li on 14-5-6.
//
//

#ifndef __HiFly__LoginLayer__
#define __HiFly__LoginLayer__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

class LoginLayer : public cocos2d::Layer
{
public:
    CREATE_FUNC(LoginLayer);
    
private:
    bool init();
    
    void onEnter();

	//void onEnterTransitionDidFinish();
    
    void onExit();
    
    void update(float dt);
    
//function
	//import login Res 
	bool importUI();

	//touchEvent
    void touchEventLogin(Ref *pSender, TouchEventType type);
	void touchEventLoadRegister(Ref *pSender, TouchEventType type);			//load Register view
    
    void refreshUI();
    
//member variable
    Button* m_loginButton;

	Button* m_registerButton;
    
    TextField *m_userName;
    
    TextField *m_password;
    
    TextField *m_passwordRe;

	ActionObject* m_enterAction;

	ActionObject* m_exitAction;
};

#endif /* defined(__HiFly__LoginLayer__) */
