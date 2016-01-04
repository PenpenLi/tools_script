////CrownLayer.cpp created by php script. chris.li
#include "CrownLayer.h"
#include "NoticeLayer.h"
#include "Player.h"
#include "util/GameSound.h"
#include "network/Network.h"

bool CrownLayer::init()
{
	if (Layer::init())
	{
		loadUI();
		rankN = 0;
		return true;
	}

	return false;
}
void CrownLayer::onEnter()
{
	Layer::onEnter();

    refreshUI();
}

void CrownLayer::onExit()
{
	Layer::onExit();
}

void CrownLayer::loadUI()
{
	rootNode = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("CrownLayer/CrownLayer.json"));

	this->addChild(rootNode);

	//add layer bg and bubble
	Sprite* _bg = Sprite::create("commonUi/layerBg.jpg");
	_bg->setAnchorPoint(Point::ZERO);
	_bg->setPosition(Point::ZERO);
	_bg->setLocalZOrder(-1);
	rootNode->addChild(_bg);

	Sprite* _bubble = Sprite::create("commonUi/bubble.png");
	Size _winSize = this->getContentSize();
	_bubble->setPosition(ccp(_winSize.width/2,_winSize.height/2));
	_bubble->setLocalZOrder(-1);
	rootNode->addChild(_bubble);

	m_last1 = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "m_last1"));

	m_last1-> addTouchEventListener(this, toucheventselector(CrownLayer::handleTouchEvent));

	m_last2 = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "m_last2"));

	m_last2-> addTouchEventListener(this, toucheventselector(CrownLayer::handleTouchEvent));

	m_last3 = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "m_last3"));

	m_last3-> addTouchEventListener(this, toucheventselector(CrownLayer::handleTouchEvent));

	m_continueBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_continueBtn"));

	m_continueBtn-> addTouchEventListener(this, toucheventselector(CrownLayer::handleTouchEvent));

	m_shareBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_shareBtn"));

	m_shareBtn-> addTouchEventListener(this, toucheventselector(CrownLayer::handleTouchEvent));


}

void CrownLayer::handleTouchEvent(Ref *pSender, TouchEventType type)
{
	if (pSender == nullptr)
	{
	}

	else if (pSender == m_continueBtn && (TOUCH_EVENT_ENDED == type))
	{
		GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);

		Layer* layer = NoticeLayer::create();

		this->getScene()->addChild(layer);
	}

	else if (pSender == m_shareBtn && (TOUCH_EVENT_ENDED == type))
	{
		GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);
	}


}
void CrownLayer::refreshUI()
{	
	auto ranklist = Player::getInstance()->getlastRankList();

	for (int i = 0; i < 3; i ++)
	{
		auto rankListItem = (*ranklist).at(i);

		if ((rankListItem->playerId) == (Player::getInstance()->getMyPlayerId()))
		{
			rankN = i + 1;
			break;
		}
	}

		if (rankN == 1)
		{
			m_last1->setVisible(true);
			ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/Crown/Crown_1.ExportJson");
			Armature* armature = Armature::create("Crown_1");
			armature->getAnimation()->play("Animation1");
			armature->setPositionX(Director::getInstance()->getWinSize().width / 2);	
			armature->setPositionY(Director::getInstance()->getWinSize().height / 3 * 2);	
			armature->setAnchorPoint(Point(0.5, 0));
			this->addChild(armature);
		}else if (rankN == 2)
		{
			m_last2->setVisible(true);
			ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/Crown/Crown_2.ExportJson");
			Armature* armature = Armature::create("Crown_2");
			armature->getAnimation()->play("Animation1");
			armature->setPositionX(Director::getInstance()->getWinSize().width / 2);	
			armature->setPositionY(Director::getInstance()->getWinSize().height / 3 * 2);	
			armature->setAnchorPoint(Point(0.5, 0));
			this->addChild(armature);
		}else if (rankN == 3)
		{
			m_last3->setVisible(true);
			ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/Crown/Crown_3.ExportJson");
			Armature* armature = Armature::create("Crown_3");
			armature->getAnimation()->play("Animation1");
			armature->setPositionX(Director::getInstance()->getWinSize().width / 2);	
			armature->setPositionY(Director::getInstance()->getWinSize().height / 3 * 2);	
			armature->setAnchorPoint(Point(0.5, 0.5));
			this->addChild(armature);
		}
}
