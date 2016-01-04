////FriendHelpLayer.cpp created by php script. chris.li
#include "FriendHelpLayer.h"

bool FriendHelpLayer::init()
{
	if (Layer::init())
	{
		loadUI();
		return true;
	}

	return false;
}
void FriendHelpLayer::onEnter()
{
	Layer::onEnter();
}

void FriendHelpLayer::onExit()
{
	Layer::onExit();
}

void FriendHelpLayer::loadUI()
{
	auto rootNode = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("FriendHelpLayer/FriendHelpLayer.json"));

	this->addChild(rootNode);

	_Panel_14 = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "Panel_14"));

	_Panel_14-> addTouchEventListener(this, toucheventselector(FriendHelpLayer::handleTouchEvent));

	_Image_1 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_1"));

	_Panel_3 = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "Panel_3"));

	_Panel_3-> addTouchEventListener(this, toucheventselector(FriendHelpLayer::handleTouchEvent));

	_Panel_54 = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "Panel_54"));

	_Panel_54-> addTouchEventListener(this, toucheventselector(FriendHelpLayer::handleTouchEvent));

	_Image_86 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_86"));

	_m_returnBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_returnBtn"));

	_m_returnBtn-> addTouchEventListener(this, toucheventselector(FriendHelpLayer::handleTouchEvent));

	_m_startGameBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_startGameBtn"));

	_m_startGameBtn-> addTouchEventListener(this, toucheventselector(FriendHelpLayer::handleTouchEvent));

	_Panel_90 = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "Panel_90"));

	_Panel_90-> addTouchEventListener(this, toucheventselector(FriendHelpLayer::handleTouchEvent));

	_m_friendListScrollView = static_cast<ScrollView*>(ui::Helper::seekWidgetByName(rootNode, "m_friendListScrollView"));

	_m_friendListScrollView-> addTouchEventListener(this, toucheventselector(FriendHelpLayer::handleTouchEvent));

	_Image_2 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_2"));

	_Image_4 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_4"));

	_Image_4_0 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_4_0"));

	_Image_4_0_1 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_4_0_1"));

	_m_mailBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_mailBtn"));

	_m_mailBtn-> addTouchEventListener(this, toucheventselector(FriendHelpLayer::handleTouchEvent));

	_m_taskBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_taskBtn"));

	_m_taskBtn-> addTouchEventListener(this, toucheventselector(FriendHelpLayer::handleTouchEvent));

	_m_rankBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_rankBtn"));

	_m_rankBtn-> addTouchEventListener(this, toucheventselector(FriendHelpLayer::handleTouchEvent));

	_m_activityBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_activityBtn"));

	_m_activityBtn-> addTouchEventListener(this, toucheventselector(FriendHelpLayer::handleTouchEvent));

	_m_notfiyMsgLabel = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_notfiyMsgLabel"));

	_Image_91 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_91"));

	_m_nextPageBtn = static_cast<Slider*>(ui::Helper::seekWidgetByName(rootNode, "m_nextPageBtn"));

	_m_nextPageBtn-> addTouchEventListener(this, toucheventselector(FriendHelpLayer::handleTouchEvent));

	_m_friendListView = static_cast<ListView*>(ui::Helper::seekWidgetByName(rootNode, "m_friendListView"));

	_m_friendListView-> addTouchEventListener(this, toucheventselector(FriendHelpLayer::handleTouchEvent));

	_Image_5 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_5"));

	_Image_5_0 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_5_0"));

	_Image_5_1 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_5_1"));

	_strengthBar4 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "strengthBar4"));

	_Image_5_3 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_5_3"));

	_m_addStrengthBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_addStrengthBtn"));

	_m_addStrengthBtn-> addTouchEventListener(this, toucheventselector(FriendHelpLayer::handleTouchEvent));

	_m_strengthMaxNum = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_strengthMaxNum"));

	_Image_5 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_5"));

	_m_addDiamondBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_addDiamondBtn"));

	_m_addDiamondBtn-> addTouchEventListener(this, toucheventselector(FriendHelpLayer::handleTouchEvent));

	_m_diamondNum = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_diamondNum"));

	_Image_5 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_5"));

	_m_addGoldBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_addGoldBtn"));

	_m_addGoldBtn-> addTouchEventListener(this, toucheventselector(FriendHelpLayer::handleTouchEvent));

	_m_goldNum = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "m_goldNum"));

	_Image_56 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_56"));

	_Label_57 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "Label_57"));

	_Image_58 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_58"));

	_Image_56 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_56"));

	_Label_57 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "Label_57"));

	_Image_58 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_58"));

	_Image_56 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_56"));

	_Label_57 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "Label_57"));

	_Image_56 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_56"));

	_Label_57 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "Label_57"));

	_Image_92 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_92"));

	_m_friendListItem = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "m_friendListItem"));

	_m_friendListItem-> addTouchEventListener(this, toucheventselector(FriendHelpLayer::handleTouchEvent));

	_m_strengthBar1 = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(rootNode, "m_strengthBar1"));

	_m_strengthBar2 = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(rootNode, "m_strengthBar2"));

	_m_strengthBar3 = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(rootNode, "m_strengthBar3"));

	_m_strengthBar4 = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(rootNode, "m_strengthBar4"));

	_m_strengthBar5 = static_cast<LoadingBar*>(ui::Helper::seekWidgetByName(rootNode, "m_strengthBar5"));

	_Label_59 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "Label_59"));

	_Label_59 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "Label_59"));

	_Image_97 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_97"));

	_mm_chooseBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "mm_chooseBtn"));

	_mm_chooseBtn-> addTouchEventListener(this, toucheventselector(FriendHelpLayer::handleTouchEvent));

	_mm_headImg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "mm_headImg"));

	_mm_heroImg = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "mm_heroImg"));

	_mm_friendName = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "mm_friendName"));

	_mm_scoreAtlasLabel = static_cast<TextAtlas*>(ui::Helper::seekWidgetByName(rootNode, "mm_scoreAtlasLabel"));

	_Image_106 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_106"));

	_Image_107 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_107"));

	_Label_108 = static_cast<Text*>(ui::Helper::seekWidgetByName(rootNode, "Label_108"));

	_Image_100 = static_cast<ImageView*>(ui::Helper::seekWidgetByName(rootNode, "Image_100"));


}

void FriendHelpLayer::handleTouchEvent(Ref *pSender, TouchEventType type)
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

	else if (pSender == _Panel_54 && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _m_returnBtn && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _m_startGameBtn && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _Panel_90 && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _m_friendListScrollView && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _m_mailBtn && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _m_taskBtn && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _m_rankBtn && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _m_activityBtn && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _m_nextPageBtn && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _m_friendListView && (TOUCH_EVENT_ENDED == type))
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

	else if (pSender == _m_friendListItem && (TOUCH_EVENT_ENDED == type))
	{
	}

	else if (pSender == _mm_chooseBtn && (TOUCH_EVENT_ENDED == type))
	{
	}


}

