#include "TouchedEventMaskLayer.h"
#include "math.h"
#include <string>

TouchedEventMaskLayer* g_TouchedEventMaskLayer = nullptr;

TouchedEventMaskLayer* TouchedEventMaskLayer::getInstance()
{
    if (nullptr == g_TouchedEventMaskLayer)
    {
        g_TouchedEventMaskLayer = new TouchedEventMaskLayer();
        
        if (!g_TouchedEventMaskLayer->init())
        {
            g_TouchedEventMaskLayer = nullptr;
        }
    }
    
    return g_TouchedEventMaskLayer;
}

bool TouchedEventMaskLayer::init()
{
	if (LayerColor::init())
    {

			return true;
		}
    
    return false;
}

void TouchedEventMaskLayer::networkConnect()
{
	auto currentScene = Director::getInstance()->getRunningScene();

	if (currentScene == nullptr || this->getParent() != nullptr)
	{
		return;
	}

	auto dispatcher = Director::getInstance()->getEventDispatcher(); 
	
	auto maskLayer = LayerColor::create(Color4B::BLACK, Director::getInstance()->getWinSize().width, Director::getInstance()->getWinSize().height);
	maskLayer->setOpacity(180); 
	this->addChild(maskLayer);

	auto listener = EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(true);
	
	listener->onTouchBegan = [=](Touch* touch,Event* event){
	
	    return true;
	};

	listener->onTouchMoved = [=](Touch* touch,Event* event){};

	listener->onTouchEnded = [=](Touch* touch,Event* event){};
	listener->onTouchCancelled = [=](Touch* touch,Event* event){};

	

	
    currentScene->addChild(this,100);

	dispatcher->addEventListenerWithSceneGraphPriority(listener,this);

}

void TouchedEventMaskLayer::networkConnectStop()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher(); 
	dispatcher->removeEventListenersForTarget(this);

	this->removeAllChildren();
	this->removeFromParent();

}