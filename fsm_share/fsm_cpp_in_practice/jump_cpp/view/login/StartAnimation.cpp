#include "StartAnimation.h"
#include "../home/NoticeLayer.h"
#include "LoginRewardLayer.h"
#include "Network.h"
#include "model/Player.h"
#include "home/MainLayer.h"
#include "home/SetLayer.h"
#include "home/HomeScene.h"
StartAnimation::StartAnimation(void)
{
}


StartAnimation::~StartAnimation(void)
{
}

bool StartAnimation::init(){
	if(!Layer::init()){
		return false;
	}
    
    setEnterType(0);
    
    auto listener = EventListenerTouchOneByOne::create();
    
	listener->setSwallowTouches(true);
	
	listener->onTouchBegan = [=](Touch* touch,Event* event){
        
	    return true;
	};
    
	listener->onTouchMoved = [=](Touch* touch,Event* event){};
    
	listener->onTouchEnded = [=](Touch* touch,Event* event){};
	listener->onTouchCancelled = [=](Touch* touch,Event* event){};
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);
    
	LayerColor* m_maskLayer = LayerColor::create(Color4B::BLACK, Director::getInstance()->getWinSize().width, Director::getInstance()->getWinSize().height);
	m_maskLayer->setOpacity(500); 
	this->addChild(m_maskLayer,this->getLocalZOrder() - 1);

	auto winSize = Director::getInstance()->getWinSize();
	//如果资源太大太多可以用这种方式加载
	//	ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("xxx.ExportJson", this, schedule_selector("回调函数"));
    /*
	ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/startAnimation/startAnimation.ExportJson");
	Armature* armature = Armature::create("startAnimation");
	armature->getAnimation()->play("Animation6");
	armature->setPosition(Point(winSize.width/2 + 43, 34));
	armature->setAnchorPoint(Point(0.5, 0.25));
	this->addChild(armature);
	//	添加动画监听
	armature->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(StartAnimation::animationEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    */

	return true;
}

void StartAnimation::animationEvent(cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string& movementID){

	if (movementType == COMPLETE){
		if (movementID == "Animation6"){
//			armature->stopAllActions();
//			armature->removeFromParent();
            
            /*if (getEnterType() == 1)
            {
                this->removeFromParent();
                return;
            }*/
            
			if(Player::getInstance()->getMyPlayerCount() >= 1)
			{
               /* if (getEnterType() == 1)
                {
                    this->removeFromParent();
                }
                else
                {*/
                    Director::getInstance()->replaceScene(HomeScene::create());
                }
	
//			}
            else
			{
				Layer* layer =  LoginRewardLayer::create();
				this->getScene()->addChild(layer);
			}
		}

	}
}