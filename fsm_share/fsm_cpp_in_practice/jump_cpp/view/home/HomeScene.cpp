//
//  HomeScene.cpp
//  HiFly
//
//  Created by chris li on 14-5-6.
//
//

#include "HomeScene.h"
#include "MainLayer.h"
#include "Network.h"
#include "Player.h"
#include "SimpleAudioEngine.h"
#include "RoleLayer.h"
#include "FriendLayer.h"
#include "SetLayer.h"
#include "RankLayer.h"
#include "ShopLayer.h"
#include "ItemLayer.h"
#include "LevelChooseLayer.h"
#include "util/GameSound.h"
#include "platform/CCDevice.h"
//
#include "common/TextFloatLabelLayer.h"
#include "TipsInfo.h"

USING_NS_CC;

bool HomeScene::init()
{
	if (Scene::init())
	{
		m_errorTime = 0.0f;
		if (CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying() == false)
		{
			GameSound::playBackgroundMusic(MUSIC_LAYER_NORMAL);
		} 
		return true;
	}

	return false;
}

void HomeScene::onEnter()
{
	Scene::onEnter();

	do
	{
		//Director::getInstance()->getEventDispatcher()->setDebugTouchEvent(true);
		
		

		Device::setAccelerometerInterval(1.0/10.0f);

		if(Player::getInstance()->getLevelChooseSwitch())
		{
			Layer *levelChooseLayer = LevelChooseLayer::create();

			levelChooseLayer->setAnchorPoint(Point::ZERO);

			levelChooseLayer->setPosition(0, 0);

			this->addChild(levelChooseLayer);

			Player::getInstance()->setLevelChooseSwitch(false);
		}
		else
		{
			Layer *loginLayer = MainLayer::create();

			loginLayer->setAnchorPoint(Point::ZERO);

			loginLayer->setPosition(0, 0);

			this->addChild(loginLayer);
		}

		//remain Time
		Network::getInstance()->do_UpHeart_Req([this](const AckInfo &ackInfo) 
		{
			std::string msgInfo = ackInfo.getMessageInfo();

			if (msgInfo != "")
			{
				log("[UpHeart] %s", msgInfo.c_str());
			}
			else
			{
				//ok
				this->schedule(schedule_selector(HomeScene::scheduleHpTime),1.0f);

				Player::getInstance()->setIsStarUpdate(true);
			}
		});

	} while (0);
}

void HomeScene::onExit()
{
	do
	{
		this->unschedule(schedule_selector(HomeScene::scheduleHpTime));

	} while (0);

	Scene::onExit();
}

void HomeScene::scheduleHpTime(float delayTime)
{
	float _remainTime = Player::getInstance()->getRemainTime();

	if(_remainTime>0)
	{
		Player::getInstance()->setRemainTime(_remainTime-delayTime);
		//error time
		m_errorTime = 0.0f;
	}
	else if(-1==_remainTime)
	{	
		//error time
		m_errorTime = 0.0f;
	}
	else
	{
		m_errorTime += delayTime;

		Network::getInstance()->do_UpHeart_Req([this](const AckInfo &ackInfo) 
		{
			std::string msgInfo = ackInfo.getMessageInfo();

			if (msgInfo != "")
			{
				log("[UpHeart] %s", msgInfo.c_str());
			}
			else
			{
				//ok
				Player::getInstance()->setIsStarUpdate(true);
			}
		});

		if(m_errorTime>4.0f)
		{
			TextFloatLabelLayer::getInstance()->show(TipsInfo::getInstance()->getMessage("hp_error"));
			this->unschedule(schedule_selector(HomeScene::scheduleHpTime));
		}
	}
}