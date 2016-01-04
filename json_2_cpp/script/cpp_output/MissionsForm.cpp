////MissionsForm.cpp created by php script. chris.li
#include "MissionsForm.h"

bool MissionsForm::init()
{
	if (Layer::init())
	{
		loadUI();
		return true;
	}

	return false;
}
void MissionsForm::onEnter()
{
	Layer::onEnter();
}

void MissionsForm::onExit()
{
	Layer::onExit();
}

void MissionsForm::loadUI()
{
	auto rootNode = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("MissionsForm/MissionsForm.json"));

	this->addChild(rootNode);

	_Node = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "Node"));

	_Node-> addTouchEventListener(this, toucheventselector(MissionsForm::handleTouchEvent));

	_fondo_Image = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "fondo_Image"));

	_line1_Image = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "line1_Image"));

	_line2_Image = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "line2_Image"));

	_line3_Image = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "line3_Image"));

	_title_Label = static_cast<LabelBMFont*>(ui::Helper::seekWidgetByName(rootNode, "title_Label"));

	_level_Label = static_cast<LabelBMFont*>(ui::Helper::seekWidgetByName(rootNode, "level_Label"));

	_range_Image = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "range_Image"));

	_mision1_Panel = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "mision1_Panel"));

	_mision1_Panel-> addTouchEventListener(this, toucheventselector(MissionsForm::handleTouchEvent));

	_mision2_Panel = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "mision2_Panel"));

	_mision2_Panel-> addTouchEventListener(this, toucheventselector(MissionsForm::handleTouchEvent));

	_mision3_Panel = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "mision3_Panel"));

	_mision3_Panel-> addTouchEventListener(this, toucheventselector(MissionsForm::handleTouchEvent));

	_mision4_Panel = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "mision4_Panel"));

	_mision4_Panel-> addTouchEventListener(this, toucheventselector(MissionsForm::handleTouchEvent));

	_labelContainer = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "labelContainer"));

	_labelContainer-> addTouchEventListener(this, toucheventselector(MissionsForm::handleTouchEvent));

	_mision1_Image = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "mision1_Image"));

	_mision1_Label = static_cast<LabelBMFont*>(ui::Helper::seekWidgetByName(rootNode, "mision1_Label"));

	_skip1_Button = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "skip1_Button"));

	_skip1_Button-> addTouchEventListener(this, toucheventselector(MissionsForm::handleTouchEvent));

	_mision2_Image = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "mision2_Image"));

	_mision2_Label = static_cast<LabelBMFont*>(ui::Helper::seekWidgetByName(rootNode, "mision2_Label"));

	_skip2_Button = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "skip2_Button"));

	_skip2_Button-> addTouchEventListener(this, toucheventselector(MissionsForm::handleTouchEvent));

	_mision3_Image = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "mision3_Image"));

	_mision3_Label = static_cast<LabelBMFont*>(ui::Helper::seekWidgetByName(rootNode, "mision3_Label"));

	_skip3_Button = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "skip3_Button"));

	_skip3_Button-> addTouchEventListener(this, toucheventselector(MissionsForm::handleTouchEvent));

	_mision4_Image = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "mision4_Image"));

	_mision4_Label = static_cast<LabelBMFont*>(ui::Helper::seekWidgetByName(rootNode, "mision4_Label"));

	_skip4_Button = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "skip4_Button"));

	_skip4_Button-> addTouchEventListener(this, toucheventselector(MissionsForm::handleTouchEvent));

	_skip1_Label = static_cast<LabelBMFont*>(ui::Helper::seekWidgetByName(rootNode, "skip1_Label"));

	_skip2_Label = static_cast<LabelBMFont*>(ui::Helper::seekWidgetByName(rootNode, "skip2_Label"));

	_skip3_Label = static_cast<LabelBMFont*>(ui::Helper::seekWidgetByName(rootNode, "skip3_Label"));

	_skip4_Label = static_cast<LabelBMFont*>(ui::Helper::seekWidgetByName(rootNode, "skip4_Label"));


}

void MissionsForm::handleTouchEvent(Ref *pSender, TouchEventType type)
{
	if (pSender == nullptr)
	{
	}

	else if (pSender == _Node && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _mision1_Panel && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _mision2_Panel && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _mision3_Panel && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _mision4_Panel && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _labelContainer && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _skip1_Button && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _skip2_Button && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _skip3_Button && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _skip4_Button && (TOUCH_EVENT_ENDED == type))
	{
	}


}

