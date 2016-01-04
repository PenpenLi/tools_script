//
//  Hero.h
//  HiFly
//
//  Created by chris li on 14-5-4.
//
//

#ifndef __HiFly__Hero__
#define __HiFly__Hero__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "CCNodeGrid.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCCustomCommand.h"

using namespace cocos2d;
using namespace cocostudio;

class Hero : public cocos2d::Node
{
public:
    static Hero* create(int heroId);
    
    CC_SYNTHESIZE(int, m_heroId, HeroId);
    
    void setJumpState(std::string &jumpState, float param1, float param2, float param3);
	void setBgState(std::string &bgState, float param1, float param2, float param3);
    
private:
    Hero();
    
    virtual ~Hero();
    
    bool init(int heroId);
    
    void onFrameEvent(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);
    
    void animationEvent(Armature *armature, MovementEventType movementType, const std::string& movementID);
    
    Armature *m_heroArmature;
	Armature *m_heroBgArmature;
};

#endif /* defined(__HiFly__Hero__) */
