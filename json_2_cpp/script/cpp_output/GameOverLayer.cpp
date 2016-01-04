////GameOverLayer.cpp created by php script. chris.li
#include "GameOverLayer.h"

bool GameOverLayer::init()
{
	if (Layer::init())
	{
		loadUI();
		return true;
	}

	return false;
}
void GameOverLayer::onEnter()
{
	Layer::onEnter();
}

void GameOverLayer::onExit()
{
	Layer::onExit();
}

void GameOverLayer::loadUI()
{
	auto rootNode = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("GameOverLayer/GameOverLayer.json"));

	this->addChild(rootNode);

	_Panel_14 = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "Panel_14"));

	_Panel_14-> addTouchEventListener(this, toucheventselector(GameOverLayer::handleTouchEvent));

	_Image_1 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_1"));

	_Image_3 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_3"));

	_m_goonBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_goonBtn"));

	_m_goonBtn-> addTouchEventListener(this, toucheventselector(GameOverLayer::handleTouchEvent));

	_m_shareBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_shareBtn"));

	_m_shareBtn-> addTouchEventListener(this, toucheventselector(GameOverLayer::handleTouchEvent));

	_Image_2 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_2"));

	_Image_4 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_4"));

	_m_heroImg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_heroImg"));

	_Label_8 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "Label_8"));

	_m_scoreAtlasLabel = static_cast<TextAtlas*>(ui::Helper::seekWidgetByName(rootNode, "m_scoreAtlasLabel"));

	_Image_22 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_22"));

	_m_scoreExtraLabel = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_scoreExtraLabel"));

	_m_maxWeekScoreImg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_maxWeekScoreImg"));

	_m_historyHighestScoreAtlasLabel = static_cast<TextAtlas*>(ui::Helper::seekWidgetByName(rootNode, "m_historyHighestScoreAtlasLabel"));

	_m_coinNum = static_cast<TextAtlas*>(ui::Helper::seekWidgetByName(rootNode, "m_coinNum"));


}

void GameOverLayer::handleTouchEvent(Ref *pSender, TouchEventType type)
{
	if (pSender == nullptr)
	{
	}

	else if (pSender == _Panel_14 && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _m_goonBtn && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _m_shareBtn && (TOUCH_EVENT_ENDED == type))
	{
	}


}

