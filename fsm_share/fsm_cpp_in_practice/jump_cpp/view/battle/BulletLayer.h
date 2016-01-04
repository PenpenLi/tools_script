//
//  BulletLayer.h
//  HiFly
//
//  Created by chris li on 14-5-13.
//
//

#ifndef __HiFly__BulletLayer__
#define __HiFly__BulletLayer__

#include "cocos2d.h"
#include "Bullet.h"

USING_NS_CC;

class BulletLayer : public Node
{
public:
    CREATE_FUNC(BulletLayer);
    
    void sceneUpdate(float dt);
    
    Vector<Bullet*>* getBullets();
    
    void enterReward();
    
    void exitReward(int nextRewardLevel);
    
private:
    bool init();
    void onEnter();
	void onExit();
    
    void update(float delta) override;
    
    std::string getLadderSetFileName();

    Vector<Bullet*> m_bullets;
    
    Layer *m_layer;
    
    Sprite *m_sprite;
    
    int m_nextLevel;
    std::string m_nextLevelStr;
};

#endif /* defined(__HiFly__BulletLayer__) */
