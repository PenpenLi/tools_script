//
//  RegisterLayer.h
//  HiFly
//
//  Created by chris li on 14-5-6.
//
//

#ifndef __HiFly__RegisterLayer__
#define __HiFly__RegisterLayer__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

class RegisterLayer : public cocos2d::Layer
{
public:
    CREATE_FUNC(RegisterLayer);
    
private:
    bool init();
    
    void onEnter();
    
    void onExit();
    
//function
	//import login Res 
	bool importUI();

	//touchEvent
	void touchEventRegister(Ref *pSender, TouchEventType type);			//Register
	void touchEventReturn(Ref *pSender, TouchEventType type);
    
    void refreshUI();
    
//member variable
	Button* m_registerButton;

	Button* m_returnButton;
    
    TextField *m_userName;
    
    TextField *m_password;
    
    TextField *m_passwordRe;

	ActionObject* m_enterAction;

	ActionObject* m_exitAction;

	//login
	std::string m_loginUserName;
	std::string m_loginPassword;
};

#endif /* defined(__HiFly__RegisterLayer__) */
