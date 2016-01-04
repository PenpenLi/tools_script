////NotifyBoxUILayer.cpp created by php script. chris.li
#include "NotifyBoxUILayer.h"

NotifyBoxUILayer* g_NotifyBoxUILayer = nullptr;

NotifyBoxUILayer* NotifyBoxUILayer::getInstance()
{
    if (nullptr == g_NotifyBoxUILayer)
    {
        g_NotifyBoxUILayer = new NotifyBoxUILayer();
        
        if (!g_NotifyBoxUILayer->init())
        {
            g_NotifyBoxUILayer = nullptr;
        }
    }
    
    return g_NotifyBoxUILayer;
}


bool NotifyBoxUILayer::init()
{
	if (Layer::init())
	{
		//loadUI();
		
        return true;
	}

	return false;
}
void NotifyBoxUILayer::onEnter()
{
	Layer::onEnter();
}

void NotifyBoxUILayer::onExit()
{
	Layer::onExit();
}


void NotifyBoxUILayer::loadUI()
{
	auto rootNode = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("NotifyBoxUILayer/NotifyBoxUILayer.json"));

	this->addChild(rootNode);

	m_enableStage = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "m_enableStage"));

	m_enableStage-> addTouchEventListener(this, toucheventselector(NotifyBoxUILayer::handleTouchEvent));

	m_gameContinue = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "m_gameContinue"));

	m_gameContinue-> addTouchEventListener(this, toucheventselector(NotifyBoxUILayer::handleTouchEvent));

	m_callFriend = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "m_callFriend"));

	m_callFriend-> addTouchEventListener(this, toucheventselector(NotifyBoxUILayer::handleTouchEvent));

	m_accept = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "m_accept"));

	m_accept-> addTouchEventListener(this, toucheventselector(NotifyBoxUILayer::handleTouchEvent));

	m_going = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "m_going"));

	m_going-> addTouchEventListener(this, toucheventselector(NotifyBoxUILayer::handleTouchEvent));

	m_enableStageText1 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_enableStageText1"));

	m_enableStageText2 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_enableStageText2"));

	m_enableStageNoBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_enableStageNoBtn"));

	m_enableStageNoBtn-> addTouchEventListener(this, toucheventselector(NotifyBoxUILayer::handleTouchEvent));

	m_enableStageYesBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_enableStageYesBtn"));

	m_enableStageYesBtn-> addTouchEventListener(this, toucheventselector(NotifyBoxUILayer::handleTouchEvent));

	m_gameContinueText1 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_gameContinueText1"));

	m_gameContinueText2 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_gameContinueText2"));

	m_gameContinueNoBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_gameContinueNoBtn"));

	m_gameContinueNoBtn-> addTouchEventListener(this, toucheventselector(NotifyBoxUILayer::handleTouchEvent));

	m_gameContinueYesBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_gameContinueYesBtn"));

	m_gameContinueYesBtn-> addTouchEventListener(this, toucheventselector(NotifyBoxUILayer::handleTouchEvent));

	m_callFriendText1 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_callFriendText1"));

	m_callFriendText2 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_callFriendText2"));

	m_callFriendNoBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_callFriendNoBtn"));

	m_callFriendNoBtn-> addTouchEventListener(this, toucheventselector(NotifyBoxUILayer::handleTouchEvent));

	m_callFriendYesBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_callFriendYesBtn"));

	m_callFriendYesBtn-> addTouchEventListener(this, toucheventselector(NotifyBoxUILayer::handleTouchEvent));

	m_acceptText1 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_acceptText1"));

	m_acceptText2 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_acceptText2"));

	m_acceptNoBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_acceptNoBtn"));

	m_acceptNoBtn-> addTouchEventListener(this, toucheventselector(NotifyBoxUILayer::handleTouchEvent));

	m_acceptYesBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_acceptYesBtn"));

	m_acceptYesBtn-> addTouchEventListener(this, toucheventselector(NotifyBoxUILayer::handleTouchEvent));

	m_goingText1 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_goingText1"));

	m_goingText2 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_goingText2"));

	m_goingNoBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_goingNoBtn"));

	m_goingNoBtn-> addTouchEventListener(this, toucheventselector(NotifyBoxUILayer::handleTouchEvent));

	m_goingYesBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_goingYesBtn"));

	m_goingYesBtn-> addTouchEventListener(this, toucheventselector(NotifyBoxUILayer::handleTouchEvent));

	m_openLimit = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "m_openLimit"));

	m_openLimit-> addTouchEventListener(this, toucheventselector(NotifyBoxUILayer::handleTouchEvent));

	m_limitStage = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "m_limitStage"));

	m_limitStage-> addTouchEventListener(this, toucheventselector(NotifyBoxUILayer::handleTouchEvent));

	m_resourrection = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "m_resourrection"));

	m_resourrection-> addTouchEventListener(this, toucheventselector(NotifyBoxUILayer::handleTouchEvent));

	////

	m_openLimitText1 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_openLimitText1"));

	m_openLimitText2 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_openLimitText2"));

	m_openLimitNoBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_openLimitNoBtn"));

	m_openLimitNoBtn-> addTouchEventListener(this, toucheventselector(NotifyBoxUILayer::handleTouchEvent));

	m_openLimitYesBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_openLimitYesBtn"));

	m_openLimitYesBtn-> addTouchEventListener(this, toucheventselector(NotifyBoxUILayer::handleTouchEvent));

	/////

	m_limitStageText1 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_limitStageText1"));

	m_limitStageText2 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_limitStageText2"));

	m_limitStageText3 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_limitStageText3"));

	m_limitStageText4 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_limitStageText4"));

	m_limitStageNoBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_limitStageNoBtn"));

	m_limitStageNoBtn-> addTouchEventListener(this, toucheventselector(NotifyBoxUILayer::handleTouchEvent));

	m_limitStageYesBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_limitStageYesBtn"));

	m_limitStageYesBtn-> addTouchEventListener(this, toucheventselector(NotifyBoxUILayer::handleTouchEvent));

	/////
	m_resourrectionText1 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_resourrectionText1"));

	m_resourrectionText2 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_resourrectionText2"));

	m_resourrectionText3 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_resourrectionText3"));

	m_resourrectionText4 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_resourrectionText4"));

	m_resourrectionD = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_resourrectionD"));

	m_resourrectionT = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_resourrectionT"));

	m_resourrectionNoBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_resourrectionNoBtn"));

	m_resourrectionNoBtn-> addTouchEventListener(this, toucheventselector(NotifyBoxUILayer::handleTouchEvent));

	m_resourrectionYesBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_resourrectionYesBtn"));

	m_resourrectionYesBtn-> addTouchEventListener(this, toucheventselector(NotifyBoxUILayer::handleTouchEvent));
}

void NotifyBoxUILayer::handleTouchEvent(Ref *pSender, TouchEventType type)
{
	if (pSender == nullptr)
	{
	}

	else if (pSender == m_enableStage && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == m_gameContinue && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == m_callFriend && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == m_accept && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == m_going && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == m_enableStageNoBtn && (TOUCH_EVENT_ENDED == type))
	{
		callCancelNotifyBoxUIHandler();
	}

	else if (pSender == m_enableStageYesBtn && (TOUCH_EVENT_ENDED == type))
	{
		callAcceptNotifyBoxUIHandler();
	}

	else if (pSender == m_gameContinueNoBtn && (TOUCH_EVENT_ENDED == type))
	{
		callCancelNotifyBoxUIHandler();
	}

	else if (pSender == m_gameContinueYesBtn && (TOUCH_EVENT_ENDED == type))
	{
		callAcceptNotifyBoxUIHandler();
	}

	else if (pSender == m_callFriendNoBtn && (TOUCH_EVENT_ENDED == type))
	{
		callCancelNotifyBoxUIHandler();
	}

	else if (pSender == m_callFriendYesBtn && (TOUCH_EVENT_ENDED == type))
	{
		callAcceptNotifyBoxUIHandler();
	}

	else if (pSender == m_acceptNoBtn && (TOUCH_EVENT_ENDED == type))
	{
		callCancelNotifyBoxUIHandler();
	}

	else if (pSender == m_acceptYesBtn && (TOUCH_EVENT_ENDED == type))
	{
		callAcceptNotifyBoxUIHandler();
	}

	else if (pSender == m_goingNoBtn && (TOUCH_EVENT_ENDED == type))
	{
		callCancelNotifyBoxUIHandler();
	}

	else if (pSender == m_goingYesBtn && (TOUCH_EVENT_ENDED == type))
	{
		callAcceptNotifyBoxUIHandler();
	}

	else if (pSender == m_openLimitNoBtn && (TOUCH_EVENT_ENDED == type))
	{
		callCancelNotifyBoxUIHandler();
	}

	else if (pSender == m_openLimitYesBtn && (TOUCH_EVENT_ENDED == type))
	{
		callAcceptNotifyBoxUIHandler();
	}

	else if (pSender == m_limitStageNoBtn && (TOUCH_EVENT_ENDED == type))
	{
		callCancelNotifyBoxUIHandler();
	}

	else if (pSender == m_limitStageYesBtn && (TOUCH_EVENT_ENDED == type))
	{
		callAcceptNotifyBoxUIHandler();
	}

	else if (pSender == m_resourrectionNoBtn && (TOUCH_EVENT_ENDED == type))
	{
		callCancelNotifyBoxUIHandler();
	}

	else if (pSender == m_resourrectionYesBtn && (TOUCH_EVENT_ENDED == type))
	{
		callAcceptNotifyBoxUIHandler();
	}
}

void NotifyBoxUILayer::setNotifyBoxType(int type)
{
    loadUI();
    
	setNotifyBoxID(type);

	if (getNotifyBoxID() == 1)
	{
		m_enableStage->setVisible(true);
	}
	else if (getNotifyBoxID() == 2)
	{
		m_gameContinue->setVisible(true);
	}
	else if (getNotifyBoxID() == 3)
	{
		m_callFriend->setVisible(true);
	}
	else if (getNotifyBoxID() == 4)
	{
		m_accept->setVisible(true);
	}
	else if (getNotifyBoxID() == 5)
	{
		m_going->setVisible(true);
	}
	else if (getNotifyBoxID() == 6)
	{
		m_openLimit->setVisible(true);
	}
	else if (getNotifyBoxID() == 7)
	{
		m_limitStage->setVisible(true);
	}
	else if (getNotifyBoxID() == 8)
	{
		m_resourrection->setVisible(true);
	}
}

bool NotifyBoxUILayer::setAcceptNotifyBoxUIHandler(std::function<void()> notifyBoxUIHandler)
{
	m_acceptNotifyBoxUIHandler = NotifyBoxUIHandler::create(notifyBoxUIHandler);
	m_acceptNotifyBoxUIHandler->retain();
		return true;
}

bool NotifyBoxUILayer::setCancelNotifyBoxUIHandler(std::function<void()> notifyBoxUIHandler)
{
	m_cancelNotifyBoxUIHandler = NotifyBoxUIHandler::create(notifyBoxUIHandler);
	m_cancelNotifyBoxUIHandler->retain();
		return true;
}

void NotifyBoxUILayer::callAcceptNotifyBoxUIHandler()
{
	m_acceptNotifyBoxUIHandler->notifyBoxUIHandler();
}

void NotifyBoxUILayer::callCancelNotifyBoxUIHandler()
{
	m_cancelNotifyBoxUIHandler->notifyBoxUIHandler();
}

void NotifyBoxUILayer::setNotifyBoxText(std::string str1, std::string str2)
{
	if (getNotifyBoxID() == 1)
	{
		m_enableStageText1->setText(str1.c_str());
		m_enableStageText2->setText(str2.c_str());
	}
	else if (getNotifyBoxID() == 2)
	{
		m_gameContinueText1->setText(str1.c_str());
		m_gameContinueText2->setText(str2.c_str());
	}
	else if (getNotifyBoxID() == 3)
	{
		m_callFriendText1->setText(str1.c_str());
		m_callFriendText2->setText(str2.c_str());
	}
	else if (getNotifyBoxID() == 4)
	{
		m_acceptText1->setText(str1.c_str());
		m_acceptText2->setText(str2.c_str());
	}
	else if (getNotifyBoxID() == 5)
	{
		m_goingText1->setText(str1.c_str());
		m_goingText2->setText(str2.c_str());
	}
	else
	{
	
	}
	
}

void NotifyBoxUILayer::show(int typeId, std::string str1, std::string str2, std::function<void()> acceptNotifyBoxUIHandler, std::function<void()> cancelNotifyBoxUIHandler)
{
	setNotifyBoxType(typeId);
	setNotifyBoxText(str1,str2);
	setAcceptNotifyBoxUIHandler(acceptNotifyBoxUIHandler);
	setCancelNotifyBoxUIHandler(cancelNotifyBoxUIHandler);

	Director::getInstance()->getRunningScene()->addChild(this);

}

void NotifyBoxUILayer::showFourString(int typeId, std::string str1, std::string str2,std::string str3, std::string str4,  std::function<void()> acceptNotifyBoxUIHandler, std::function<void()> cancelNotifyBoxUIHandler)
{
	setNotifyBoxType(typeId);
	setAcceptNotifyBoxUIHandler(acceptNotifyBoxUIHandler);
	setCancelNotifyBoxUIHandler(cancelNotifyBoxUIHandler);

	if (getNotifyBoxID() == 6)
	{
		std::string t1 = m_openLimitText1->getStringValue();
		t1 += t1 + str1;

		m_openLimitText1->setText(t1.c_str());
	}
	else if (getNotifyBoxID() == 7)
	{
		std::string t1 = m_limitStageText1->getStringValue();
		t1 += t1 + str1;
		m_limitStageText1->setText(t1.c_str());

		std::string t2 = m_limitStageText2->getStringValue();
		t2 += t2 + str2;
		m_limitStageText2->setText(t2.c_str());

		std::string t3 = m_limitStageText3->getStringValue();
		t3 += t3 + str3;
		m_limitStageText3->setText(t3.c_str());

	}
	else if (getNotifyBoxID() == 8)
	{
		m_resourrectionD->setText(str1.c_str());
		m_resourrectionT->setText(str2.c_str());
	}

	Director::getInstance()->getRunningScene()->addChild(this);
}

void NotifyBoxUILayer::stop()
{
	m_acceptNotifyBoxUIHandler->release();
	m_cancelNotifyBoxUIHandler->release();

	//this->removeFromParent();
    Director::getInstance()->getRunningScene()->removeChild(this);
}

