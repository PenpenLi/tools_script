////CrownLayer.cpp created by php script. chris.li
#include "CrownCurrentLayer.h"
#include "NoticeLayer.h"
#include "Player.h"
#include "battle/RankingUpLayer.h"
#include "network/Network.h"
#include "util/GameSound.h"

bool CrownCurrentLayer::init()
{
	if (Layer::init())
	{
		loadUI();
		rankN = 0;

		return true;
	}

	return false;
}
void CrownCurrentLayer::onEnter()
{
	Layer::onEnter();

	refreshUI();
}

void CrownCurrentLayer::onExit()
{
	Layer::onExit();
}

void CrownCurrentLayer::loadUI()
{

	rootNode = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("CrownLayer/CrownLayer.json"));
	
	this->addChild(rootNode);

	
	Sprite* _bg = Sprite::create("commonUi/layerBg.jpg");
	_bg->setAnchorPoint(Point::ZERO);
	_bg->setPosition(Point::ZERO);
	_bg->setLocalZOrder(-1);
	rootNode->addChild(_bg);

	m_current1 = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "m_current1"));

	m_current1-> addTouchEventListener(this, toucheventselector(CrownCurrentLayer::handleTouchEvent));

	m_current2 = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "m_current2"));

	m_current2-> addTouchEventListener(this, toucheventselector(CrownCurrentLayer::handleTouchEvent));

	m_current3 = static_cast<Layout*>(ui::Helper::seekWidgetByName(rootNode, "m_current3"));

	m_current3-> addTouchEventListener(this, toucheventselector(CrownCurrentLayer::handleTouchEvent));

	m_continueBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_continueBtn"));

	m_continueBtn-> addTouchEventListener(this, toucheventselector(CrownCurrentLayer::handleTouchEvent));

	m_shareBtn = static_cast<Button*>(ui::Helper::seekWidgetByName(rootNode, "m_shareBtn"));

	m_shareBtn-> addTouchEventListener(this, toucheventselector(CrownCurrentLayer::handleTouchEvent));


}

void CrownCurrentLayer::handleTouchEvent(Ref *pSender, TouchEventType type)
{
	if (pSender == nullptr)
	{
	}

	else if (pSender == m_continueBtn && (TOUCH_EVENT_ENDED == type))
	{
		GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);
		
		Layer* layer = RankingUpLayer::create();

		this->getScene()->addChild(layer);
	}

	else if (pSender == m_shareBtn && (TOUCH_EVENT_ENDED == type))
	{
		GameSound::playEffectMusic(EFF_NOMAL_CLICK_BTN);
	}


}

void CrownCurrentLayer::refreshUI()
{
	int index = 0;
	int myRank = 0;
	auto ranklist = Player::getInstance()->getWorldRankList();
	
	for (auto friendItem : *ranklist)
	{
		index++;
		if((Player::getInstance()->getScore()) > (friendItem->score))
		{ 
			if (index == 1)
			{
				m_current1->setVisible(true);
				ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/Crown/Crown_1.ExportJson");
				Armature* armature = Armature::create("Crown_1");
				armature->getAnimation()->play("Animation1");
				armature->setPositionX(Director::getInstance()->getWinSize().width / 2);	
				armature->setPositionY(Director::getInstance()->getWinSize().height / 3 * 2);	
				armature->setAnchorPoint(Point(0.5, 0));
				this->addChild(armature);
				break;

			}else if (index == 2)
			{
				m_current2->setVisible(true);
				ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/Crown/Crown_2.ExportJson");
				Armature* armature = Armature::create("Crown_2");
				armature->getAnimation()->play("Animation1");
				armature->setPositionX(Director::getInstance()->getWinSize().width / 2);	
				armature->setPositionY(Director::getInstance()->getWinSize().height / 3 * 2);	
				armature->setAnchorPoint(Point(0.5, 0));
				this->addChild(armature);
				break;
			}else if (index == 3)
			{
				m_current3->setVisible(true);
				ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/Crown/Crown_3.ExportJson");
				Armature* armature = Armature::create("Crown_3");
				armature->getAnimation()->play("Animation1");
				armature->setPositionX(Director::getInstance()->getWinSize().width / 2);	
				armature->setPositionY(Director::getInstance()->getWinSize().height / 3 * 2);	
				armature->setAnchorPoint(Point(0.5, 0.5));
				this->addChild(armature);
				break;
			}
	
		}
	}
}