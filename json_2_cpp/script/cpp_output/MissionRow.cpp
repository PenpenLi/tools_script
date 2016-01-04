////MissionRow.cpp created by php script. chris.li
#include "MissionRow.h"

bool MissionRow::init()
{
	if (Layer::init())
	{
		loadUI();
		return true;
	}

	return false;
}
void MissionRow::onEnter()
{
	Layer::onEnter();
}

void MissionRow::onExit()
{
	Layer::onExit();
}

void MissionRow::loadUI()
{
	auto rootNode = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("MissionRow/MissionRow.json"));

	this->addChild(rootNode);

	_Node = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "Node"));

	_Node-> addTouchEventListener(this, toucheventselector(MissionRow::handleTouchEvent));

	_icon_Image = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "icon_Image"));

	_label_Label = static_cast<LabelBMFont*>(ui::Helper::seekWidgetByName(rootNode, "label_Label"));

	_shadow_Label = static_cast<LabelBMFont*>(ui::Helper::seekWidgetByName(rootNode, "shadow_Label"));


}

void MissionRow::handleTouchEvent(Ref *pSender, TouchEventType type)
{
	if (pSender == nullptr)
	{
	}

	else if (pSender == _Node && (TOUCH_EVENT_ENDED == type))
	{
	}


}

