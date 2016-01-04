//
//  LoginScene.cpp
//  HiFly
//
//  Created by chris li on 14-5-6.
//
//

#include "LoginScene.h"
#include "LoginLayer.h"
#include "util/GameSound.h"
#include "EnterLayer.h"

USING_NS_CC;

bool LoginScene::init()
{
    if (Scene::init())
    {
        return true;
    }
    
    return false;
}

void LoginScene::onEnter()
{
    Scene::onEnter();
    
    do
    {
  //   Layer *loginLayer = LoginLayer::create();
        
	   	Layer *loginLayer = EnterLayer::create();

   //     loginLayer->setAnchorPoint(Point::ZERO);
        
   //     loginLayer->setPosition(0, 0);
        
        this->addChild(loginLayer);
        
    } while (0);
}

void LoginScene::onExit()
{
    do
    {
        
    } while (0);
    
    Scene::onExit();
}
