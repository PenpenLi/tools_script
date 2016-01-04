////TaskLayer.cpp created by php script. chris.li
#include "TaskLayer.h"

bool TaskLayer::init()
{
	if (Layer::init())
	{
		loadUI();
		return true;
	}

	return false;
}
void TaskLayer::onEnter()
{
	Layer::onEnter();
}

void TaskLayer::onExit()
{
	Layer::onExit();
}

void TaskLayer::loadUI()
{
	auto rootNode = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("TaskLayer/TaskLayer.json"));

	this->addChild(rootNode);

	_Panel_14 = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "Panel_14"));

	_Panel_14-> addTouchEventListener(this, toucheventselector(TaskLayer::handleTouchEvent));

	_Image_1 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_1"));

	_Panel_5 = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "Panel_5"));

	_Panel_5-> addTouchEventListener(this, toucheventselector(TaskLayer::handleTouchEvent));

	_m_returnBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_returnBtn"));

	_m_returnBtn-> addTouchEventListener(this, toucheventselector(TaskLayer::handleTouchEvent));

	_m_notifyMsgLabel = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "m_notifyMsgLabel"));

	_Panel_65 = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "Panel_65"));

	_Panel_65-> addTouchEventListener(this, toucheventselector(TaskLayer::handleTouchEvent));

	_m_taskListScrollView = static_cast<ScrollView*>(ui::Helper::seekWidgetByName(rootNode, "m_taskListScrollView"));

	_m_taskListScrollView-> addTouchEventListener(this, toucheventselector(TaskLayer::handleTouchEvent));

	_Image_2 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_2"));

	_Image_6 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_6"));

	_Image_6_0 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_6_0"));

	_Image_6_0_1 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_6_0_1"));

	_Label_64 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "Label_64"));

	_Image_66 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_66"));

	_Slider_68 = static_cast<Slider*>(ui::Helper::seekWidgetByName(rootNode, "Slider_68"));

	_Slider_68-> addTouchEventListener(this, toucheventselector(TaskLayer::handleTouchEvent));

	_m_taskListView = static_cast<ListView*>(ui::Helper::seekWidgetByName(rootNode, "m_taskListView"));

	_m_taskListView-> addTouchEventListener(this, toucheventselector(TaskLayer::handleTouchEvent));

	_strengthImag = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "strengthImag"));

	_strengthImg2 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "strengthImg2"));

	_Image_7_0_1 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_7_0_1"));

	_Image_7_0_1_2 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_7_0_1_2"));

	_Image_7_0_1_2_3 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_7_0_1_2_3"));

	_m_strengthMaxNum = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_strengthMaxNum"));

	_m_addStrengthBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_addStrengthBtn"));

	_m_addStrengthBtn-> addTouchEventListener(this, toucheventselector(TaskLayer::handleTouchEvent));

	_Image_7 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_7"));

	_m_diamondNum = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_diamondNum"));

	_m_addDiamondBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_addDiamondBtn"));

	_m_addDiamondBtn-> addTouchEventListener(this, toucheventselector(TaskLayer::handleTouchEvent));

	_Image_7 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_7"));

	_m_goldNum = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_goldNum"));

	_m_addGoldBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_addGoldBtn"));

	_m_addGoldBtn-> addTouchEventListener(this, toucheventselector(TaskLayer::handleTouchEvent));

	_Image_67 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_67"));

	_m_taskListItem = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "m_taskListItem"));

	_m_taskListItem-> addTouchEventListener(this, toucheventselector(TaskLayer::handleTouchEvent));

	_m_strengthBar1 = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(rootNode, "m_strengthBar1"));

	_m_strengthBar2 = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(rootNode, "m_strengthBar2"));

	_m_strengthBar3 = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(rootNode, "m_strengthBar3"));

	_m_strengthBar4 = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(rootNode, "m_strengthBar4"));

	_m_strengthBar5 = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(rootNode, "m_strengthBar5"));

	_item_bg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "item_bg"));

	_mm_taskDespLabel = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "mm_taskDespLabel"));

	_mm_acceptBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "mm_acceptBtn"));

	_mm_acceptBtn-> addTouchEventListener(this, toucheventselector(TaskLayer::handleTouchEvent));

	_mm_taskImg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "mm_taskImg"));

	_Image_79 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_79"));

	_mm_taskProgressBar = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(rootNode, "mm_taskProgressBar"));

	_mm_taskProgressLabel = static_cast<TextAtlas*>(ui::Helper::seekWidgetByName(rootNode, "mm_taskProgressLabel"));


}

void TaskLayer::handleTouchEvent(Ref *pSender, TouchEventType type)
{
	if (pSender == nullptr)
	{
	}

	else if (pSender == _Panel_14 && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _Panel_5 && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _m_returnBtn && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _Panel_65 && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _m_taskListScrollView && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _Slider_68 && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _m_taskListView && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _m_addStrengthBtn && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _m_addDiamondBtn && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _m_addGoldBtn && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _m_taskListItem && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _mm_acceptBtn && (TOUCH_EVENT_ENDED == type))
	{
	}


}

