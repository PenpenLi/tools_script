#include "NetworkConnectLayer.h"
#include "math.h"
#include <string>

#define SPRINE_TAG 1235
NetworkConnectLayer* g_NetworkConnectLayer = nullptr;

NetworkConnectLayer* NetworkConnectLayer::getInstance()
{
    if (nullptr == g_NetworkConnectLayer)
    {
        g_NetworkConnectLayer = new NetworkConnectLayer();
        
        if (!g_NetworkConnectLayer->init())
        {
            g_NetworkConnectLayer = nullptr;
        }
    }
    
    return g_NetworkConnectLayer;
}

bool NetworkConnectLayer::init()
{
	if (LayerColor::init())
    {
		//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("NetworkConnectAnimation.plist");
		m_connectCount = 0;

		return true;
	}
    
    return false;
}

void NetworkConnectLayer::networkConnect()
{
	m_connectCount++;

	if (m_connectCount > 1) {return;}

    if (this->getParent() && this->isRunning()) {return;}
    
	auto currentScene = Director::getInstance()->getRunningScene();

	if (currentScene == nullptr || this->getParent() != nullptr)
	{
		return;
	}

	auto dispatcher = Director::getInstance()->getEventDispatcher(); 
	
	auto maskLayer = LayerColor::create(Color4B::BLACK, Director::getInstance()->getWinSize().width, Director::getInstance()->getWinSize().height);
	//auto maskLayer = Layer::create();
	maskLayer->setOpacity(80); 
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

    auto animation = Animation::create();

    for (int i = 1; i < 11; i++)
    {
        char* fileName = new char[120];
        sprintf(fileName,"Network_Waiting_%d.png",i);
        animation->addSpriteFrameWithFile(fileName);
    }
    animation->setDelayPerUnit(0.06f);
    animation->setRestoreOriginalFrame(false);

    auto m_sprineAnimate = Animate::create(animation);

    m_sprine = Sprite::create("Network_Waiting_1.png");
    m_sprine->setPosition(Point(Director::getInstance()->getWinSize().width / 2, Director::getInstance()->getWinSize().height / 2));
    m_sprine->setTag(SPRINE_TAG);
	m_sprine->setLocalZOrder(100);
    Director::getInstance()->getRunningScene()->addChild(m_sprine);
	m_sprine->setOpacity(200.f);

	auto acts = Repeat::create(dynamic_cast<ActionInterval *>(Sequence::create(m_sprineAnimate, m_sprineAnimate, NULL)),20);
    m_sprine->runAction(acts);
}

void NetworkConnectLayer::networkConnectStop()
{
	m_connectCount--;

    if (this->getParent() && this->isRunning() && m_connectCount <= 0)
    {
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->removeEventListenersForTarget(this);
        
        m_sprine->removeFromParent();
        
        this->removeAllChildren();
        this->removeFromParent();
    }
}