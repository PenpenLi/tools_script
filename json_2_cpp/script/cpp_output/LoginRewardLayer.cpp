////LoginRewardLayer.cpp created by php script. chris.li
#include "LoginRewardLayer.h"

bool LoginRewardLayer::init()
{
	if (Layer::init())
	{
		loadUI();
		return true;
	}

	return false;
}
void LoginRewardLayer::onEnter()
{
	Layer::onEnter();
}

void LoginRewardLayer::onExit()
{
	Layer::onExit();
}

void LoginRewardLayer::loadUI()
{
	auto rootNode = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("LoginRewardLayer/LoginRewardLayer.json"));

	this->addChild(rootNode);

	_Panel_14 = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "Panel_14"));

	_Panel_14-> addTouchEventListener(this, toucheventselector(LoginRewardLayer::handleTouchEvent));

	_Image_1 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_1"));

	_Panel_3 = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "Panel_3"));

	_Panel_3-> addTouchEventListener(this, toucheventselector(LoginRewardLayer::handleTouchEvent));

	_m_getRewardBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_getRewardBtn"));

	_m_getRewardBtn-> addTouchEventListener(this, toucheventselector(LoginRewardLayer::handleTouchEvent));

	_Panel_43 = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "Panel_43"));

	_Panel_43-> addTouchEventListener(this, toucheventselector(LoginRewardLayer::handleTouchEvent));

	_Panel_43_0 = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "Panel_43_0"));

	_Panel_43_0-> addTouchEventListener(this, toucheventselector(LoginRewardLayer::handleTouchEvent));

	_Image_2 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_2"));

	_Image_4 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_4"));

	_m_day_1 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_day_1"));

	_m_day_2 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_day_2"));

	_m_day_3 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_day_3"));

	_m_day_4 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_day_4"));

	_m_day_5 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_day_5"));

	_m_day_6 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_day_6"));

	_m_day_7 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_day_7"));

	_Image_5 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_5"));

	_Image_95 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_95"));

	_Label_96 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "Label_96"));

	_Label_9 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "Label_9"));

	_Label_9_0 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "Label_9_0"));

	_Image_10 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_10"));

	_Label_9 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "Label_9"));

	_Label_9_0 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "Label_9_0"));

	_Image_10 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_10"));

	_Label_9 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "Label_9"));

	_Label_9_0 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "Label_9_0"));

	_Image_10 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_10"));

	_Label_9 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "Label_9"));

	_Label_9_0 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "Label_9_0"));

	_Image_10 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_10"));

	_Label_9 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "Label_9"));

	_Label_9_0 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "Label_9_0"));

	_Image_10 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_10"));

	_Label_9 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "Label_9"));

	_Label_9_0 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "Label_9_0"));

	_Image_10 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_10"));

	_Label_9 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "Label_9"));

	_Label_9_0 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "Label_9_0"));

	_Image_10 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_10"));


}

void LoginRewardLayer::handleTouchEvent(Ref *pSender, TouchEventType type)
{
	if (pSender == nullptr)
	{
	}

	else if (pSender == _Panel_14 && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _Panel_3 && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _m_getRewardBtn && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _Panel_43 && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _Panel_43_0 && (TOUCH_EVENT_ENDED == type))
	{
	}


}

