//
//  GameSound.cpp
//  HiJump
//
//  Created by imac on 13-11-6.
//
//
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "GameSound.h"


USING_NS_CC;
using namespace CocosDenshion;

static int g_currEffectMusicId = 0;

void GameSound::preloadMusic() {
	float bgV = 0.3f;
	float effV = 0.6f;
	if (CCUserDefault::getInstance()->getFloatForKey("bgVolume"))
	{
		 bgV = CCUserDefault::getInstance()->getFloatForKey("bgVolume");
	}
	if (CCUserDefault::getInstance()->getFloatForKey("effVolume"))
	{
		effV = CCUserDefault::getInstance()->getFloatForKey("effVolume");
	}
    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(MUSIC_LAYER_INGAME);
    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(MUSIC_LAYER_NORMAL);
    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(MUSIC_LAYER_REWARD);
    SimpleAudioEngine::sharedEngine()->setEffectsVolume(effV);
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(bgV);
}

void GameSound::playBackgroundMusic(std::string musicName) {

	SimpleAudioEngine::sharedEngine()->playBackgroundMusic(musicName.c_str(), true);
}

void GameSound::playEffectMusic(std::string musicName) {
	g_currEffectMusicId = SimpleAudioEngine::sharedEngine()->playEffect(musicName.c_str()); 
}

void GameSound::stopEffectMusic(std::string musicName) {
	if (g_currEffectMusicId != 0)
	{
		SimpleAudioEngine::sharedEngine()->stopEffect(g_currEffectMusicId);
	}
}

void GameSound::playEffectMusic(std::string musicName, bool bl) {
	g_currEffectMusicId = SimpleAudioEngine::sharedEngine()->playEffect(musicName.c_str(), true); 
}