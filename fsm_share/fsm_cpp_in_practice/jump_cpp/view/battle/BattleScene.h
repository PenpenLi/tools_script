//
//  BattleScene.h
//  HiFly
//
//  Created by chris li on 14-5-6.
//
//

#ifndef __HiFly__BattleScene__
#define __HiFly__BattleScene__

#include "cocos2d.h"
#include "BgLayer.h"
#include "LadderLayer.h"
#include "HeroLayer.h"
#include "BulletLayer.h"
#include "BattleUILayer.h"
#include "BattleDefine.h"

class BattleScene : public cocos2d::Scene
{
public:
    CREATE_FUNC(BattleScene);
    
    void ntf(HeroNtf &heroNtf);
    
    void pause();
    
    void resume();
    
private:
    bool init();
    
    void onEnter();
    
    void onExit();
    
    void update(float delta) override;
    
    void checkCollide(CollideInterface *aLayer, CollideInterface *bLayer);
    
    BgLayer *m_bgLayer;
    
    LadderLayer *m_ladderLayer;
    
    HeroLayer *m_heroLayer;
    
    BulletLayer *m_bulletLayer;
    
    BattleUILayer *m_uiLayer;
    
    bool m_isPause;
    
    unsigned int m_startPuaseCounter;
    
    float m_dtData[3];
};

#endif /* defined(__HiFly__BattleScene__) */
