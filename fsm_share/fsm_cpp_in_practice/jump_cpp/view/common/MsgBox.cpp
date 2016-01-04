//
//  MsgBox.cpp
//  HiFly
//
//  Created by chris li on 14-4-28.
//
//

#include "MsgBox.h"
USING_NS_CC;

MsgBox* g_MsgBox = nullptr;

MsgBox* MsgBox::getInstance()
{
    if (nullptr == g_MsgBox)
    {
        g_MsgBox = new MsgBox();
        
        if (!g_MsgBox->init())
        {
            g_MsgBox = nullptr;
        }
    }
    
    return g_MsgBox;
}

void MsgBox::show(std::string msg)
{
    Scene *currScene = Director::getInstance()->getRunningScene();
    
    if (nullptr != currScene && nullptr == this->getParent())
    {
        currScene->addChild(this);
        
        //auto maskSprite = Sprite::create();
        //maskSprite->setContentSize(Director::getInstance()->getWinSize());
        

        //this->addChild(maskSprite);
        
        //add mask layer
        auto maskLayer = LayerColor::create(Color4B::ORANGE, Director::getInstance()->getWinSize().width, Director::getInstance()->getWinSize().height);
        maskLayer->setOpacity(50);
        
        this->addChild(maskLayer);
        
        /*
        auto tip = Label::createWithSystemFont(msg, "Marker Felt", 32);
        this->addChild(tip);
        
        auto listener1 = EventListenerTouchOneByOne::create();
        listener1->onTouchBegan = [](Touch* touch, Event* event) {return true;};
        listener1->onTouchMoved = [](Touch* touch, Event* event) {};
        listener1->onTouchEnded = [=](Touch* touch, Event* event) {};
        listener1->setSwallowTouches(true);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);
        this->setContentSize(Size(100, 100));
        
        auto closeItem = MenuItemImage::create(
                                               "CloseNormal.png",
                                               "CloseSelected.png",
                                               [this](Ref* pSender){
                                                   this->hide();
                                               });
        
        closeItem->setPosition(50,50);
        
        // create menu, it's an autorelease object
        auto menu = Menu::create(closeItem, NULL);
        menu->setPosition(Point::ZERO);
        this->addChild(menu, 2);
         */
    }
}

void MsgBox::menuCloseCallback(Ref* pSender)
{
    this->hide();
}

bool MsgBox::isShow()
{
    if (nullptr != this->getParent())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void MsgBox::hide()
{
    _eventDispatcher->removeEventListenersForTarget(this);
    
    this->removeFromParent();
}