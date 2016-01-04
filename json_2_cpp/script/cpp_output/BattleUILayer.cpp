////BattleUILayer.cpp created by php script. chris.li
#include "BattleUILayer.h"

bool BattleUILayer::init()
{
	if (Layer::init())
	{
		loadUI();
		return true;
	}

	return false;
}
void BattleUILayer::onEnter()
{
	Layer::onEnter();
}

void BattleUILayer::onExit()
{
	Layer::onExit();
}

void BattleUILayer::loadUI()
{
	auto rootNode = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("BattleUILayer/BattleUILayer.json"));

	this->addChild(rootNode);

	_Panel_14 = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "Panel_14"));

	_Panel_14-> addTouchEventListener(this, toucheventselector(BattleUILayer::handleTouchEvent));

	_Panel_73 = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "Panel_73"));

	_Panel_73-> addTouchEventListener(this, toucheventselector(BattleUILayer::handleTouchEvent));

	_m_pauseMenu = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "m_pauseMenu"));

	_m_pauseMenu-> addTouchEventListener(this, toucheventselector(BattleUILayer::handleTouchEvent));

	_Panel_8 = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "Panel_8"));

	_Panel_8-> addTouchEventListener(this, toucheventselector(BattleUILayer::handleTouchEvent));

	_Image_52 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_52"));

	_m_myScore = static_cast<TextAtlas*>(ui::Helper::seekWidgetByName(rootNode, "m_myScore"));

	_m_prop2 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_prop2"));

	_m_prop1 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_prop1"));

	_m_prop3 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_prop3"));

	_m_pauseBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_pauseBtn"));

	_m_pauseBtn-> addTouchEventListener(this, toucheventselector(BattleUILayer::handleTouchEvent));

	_m_enterRewardLevelProgressBar = static_cast<Slider*>(ui::Helper::seekWidgetByName(rootNode, "m_enterRewardLevelProgressBar"));

	_m_enterRewardLevelProgressBar-> addTouchEventListener(this, toucheventselector(BattleUILayer::handleTouchEvent));

	_m_continueGameBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_continueGameBtn"));

	_m_continueGameBtn-> addTouchEventListener(this, toucheventselector(BattleUILayer::handleTouchEvent));

	_m_giveupGameBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_giveupGameBtn"));

	_m_giveupGameBtn-> addTouchEventListener(this, toucheventselector(BattleUILayer::handleTouchEvent));

	_Image_6 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_6"));

	_Image_6_0 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_6_0"));

	_m_myCoin = static_cast<TextAtlas*>(ui::Helper::seekWidgetByName(rootNode, "m_myCoin"));

	_Image_99 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_99"));

	_Image_99 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_99"));

	_Image_99 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_99"));

	_Image_213 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_213"));

	_m_myHeadImg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_myHeadImg"));

	_m_myName = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_myName"));

	_Image_13 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_13"));

	_Image_13_0 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_13_0"));

	_Image_13_0_1 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_13_0_1"));

	_m_friendHeadImg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_friendHeadImg"));

	_m_friendName = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_friendName"));

	_m_friendScore = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_friendScore"));

	_m_myHP1 = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(rootNode, "m_myHP1"));

	_m_myHP2 = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(rootNode, "m_myHP2"));

	_m_myHP3 = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(rootNode, "m_myHP3"));


}

void BattleUILayer::handleTouchEvent(Ref *pSender, TouchEventType type)
{
	if (pSender == nullptr)
	{
	}

	else if (pSender == _Panel_14 && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _Panel_73 && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _m_pauseMenu && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _Panel_8 && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _m_pauseBtn && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _m_enterRewardLevelProgressBar && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _m_continueGameBtn && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _m_giveupGameBtn && (TOUCH_EVENT_ENDED == type))
	{
	}


}

