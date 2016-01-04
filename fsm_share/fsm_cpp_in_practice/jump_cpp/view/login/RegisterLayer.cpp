//
//  RegisterLayer.cpp
//  HiFly
//
//  Created by chris li on 14-5-6.
//
//

#include "RegisterLayer.h"
#include "Network.h"
#include "../home/HomeScene.h"
#include "StartAnimation.h"
#include "util/GameSound.h"

bool RegisterLayer::init()
{
	if (Layer::init())
	{
		//member variable init
		m_registerButton = 0;
		m_returnButton = 0;
		m_userName = 0;
		m_password = 0;
		m_passwordRe = 0;
		m_enterAction = 0;
		m_exitAction = 0;

		//
		m_loginUserName = "";
		m_loginPassword = "";

		//Ui res
		importUI();

		return true;
	}

	return false;
}

void RegisterLayer::onEnter()
{
	Layer::onEnter();
	//ActionManagerEx::getInstance()->playActionByName("LoginUi_register.json","enterAnimation");
	//if(m_enterAction)
	//	m_enterAction->play();
	log("~~~Register onEnter");
}

void RegisterLayer::onExit()
{
	Layer::onExit();
	if(m_exitAction)
		//m_exitAction->play();
			log("~~~Register onExit");

}

//import login Res 
bool RegisterLayer::importUI()
{
	bool bRet = false;
	do{
		Layout* t_layout = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("LoginUi/LoginUi_register.json"));
		this->addChild(t_layout);

		Widget* t_parent = t_layout->getChildByName("bg");

		m_registerButton = static_cast<Button*>(t_parent->getChildByName("btn_register"));

		if (m_registerButton)
		{
			m_registerButton->addTouchEventListener(this, toucheventselector(RegisterLayer::touchEventRegister));
		}

		m_returnButton = static_cast<Button*>(t_parent->getChildByName("btn_return"));

		if (m_returnButton)
		{
			m_returnButton->addTouchEventListener(this, toucheventselector(RegisterLayer::touchEventReturn));
		}

		if(!m_enterAction)
			m_enterAction = ActionManagerEx::getInstance()->getActionByName("LoginUi_register.json","enterAnimation");
		//if(!m_exitAction)
		//	m_exitAction = ActionManagerEx::getInstance()->getActionByName("LoginUi_register.json","exitAnimation");

		//
		//ActionManagerEx::getInstance()->destroyInstance();

		m_userName = static_cast<TextField*>(t_parent->getChildByName("tex_usr"));

		m_password = static_cast<TextField*>(t_parent->getChildByName("tex_psw"));

		m_passwordRe = static_cast<TextField*>(t_parent->getChildByName("tex_pswRe"));

		// add event listener Register
		//auto listener = EventListenerCustom::create("UserRegister_Ack", [this](EventCustom* event) {
		//	if (event->getUserData() != nullptr)
		//	{
		//		std::string *msgInfo = static_cast<std::string*> (event->getUserData());

		//		if (*msgInfo != "")
		//		{
		//			log("[RegisterLayer] %s", msgInfo->c_str());
		//		}
		//		else
		//		{
		//			//Network::getInstance()->do_UserLogin_Req(m_loginUserName,m_loginPassword,"0","0");
		//		}
		//	}

		//});

		//_eventDispatcher->addEventListenerWithFixedPriority(listener, 1);



		bRet = true;
	}while(0);

	return bRet;
}


//**********************************touch event*******************************
void RegisterLayer::touchEventRegister(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);

	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		//Register
		if (m_userName->getStringValue() != "" && m_password->getStringValue() != "" && m_passwordRe->getStringValue() != "")
		{
			if(m_password->getStringValue()==m_passwordRe->getStringValue())
			{
				log("~~~register : \n username : %s \n password : %s ",m_userName->getStringValue().c_str(),m_password->getStringValue().c_str());
				m_loginUserName = m_userName->getStringValue();
				m_loginPassword = m_password->getStringValue();
				//Network::getInstance()->do_UserRegister_Req(m_userName->getStringValue(),m_password->getStringValue());
				Network::getInstance()->do_UserRegister_Req(m_userName->getStringValue(),m_password->getStringValue(),[this](const AckInfo &ackInfo) 
				{
					std::string msgInfo = ackInfo.getMessageInfo();

					if (msgInfo != "")
					{
						log("[UserRegister] %s", msgInfo.c_str());
					}
					else
					{
						//ok
						log("[UserRegister] : success~~!!");
		/*				Network::getInstance()->do_UserLogin_Req(m_loginUserName,m_loginPassword,"0","0",[this](const AckInfo &ackInfo) 
						{
							std::string msgInfo = ackInfo.getMessageInfo();

							if (msgInfo != "")
							{
								log("[UserLogin] %s", msgInfo.c_str());
							}
							else
							{
								//ok
								log("[UserLogin] : success~~!!");
								Layer* layer =  StartAnimation::create();

								this->getScene()->addChild(layer);
							}
						});*/
					}
				});
			}
			else
				log("password and passwordRe difference~~!!!");
		}
		else
			log("please input all the info~!!");

		break;
	default:
		break;
	}
}

void RegisterLayer::touchEventReturn(Ref *pSender, TouchEventType type)
{
	GameSound::playEffectMusic(EFF_CLOSE_PAGE);

	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		log("~~~return");
		//m_exitAction->play();


		this->removeFromParentAndCleanup(true);

		//this->setVisible(false);

		break;
	default:
		break;
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~touch event~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~