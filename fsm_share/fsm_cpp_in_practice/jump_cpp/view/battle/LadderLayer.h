//
//  LadderLayer.h
//  HiFly
//
//  Created by chris li on 14-5-13.
//
//

#ifndef __HiFly__LadderLayer__
#define __HiFly__LadderLayer__

#include "cocos2d.h"
#include "CollideInterface.h"
#include "BattleDefine.h"

USING_NS_CC;


class LadderLayer : public Node, public CollideInterface
{
public:
    CREATE_FUNC(LadderLayer);
    
    void sceneUpdate(float dt);

    void ntf(HeroNtf &heroNtf);
    
    // interface API
    Vector<Node*>* getCollideNode();
    
    Rect getCollideRect(Node *myNode, int targetNodeType);
    
    void handleCollide(Node *myNode, int targetNodeType);
    
    int getCollideNodeType(Node *myNode);
    
    // Property: magnet state
    CC_SYNTHESIZE(int, m_magnetState, MagnetState);
    
    // Property: hero world pos
    CC_SYNTHESIZE(Point, m_heroPos, HeroPos);
    
    // Property: is is reward levels
    CC_SYNTHESIZE(int, m_levelType, LevelType);


	CC_SYNTHESIZE(bool, m_isAttack, IsAttack);


	CC_SYNTHESIZE(bool, m_isInvincible, IsInvincible);
    
private:
    bool init();
    
    void onEnter();
    
    void onExit();
    
    void scrollDown(float distance);
    
    void initLadderSet();
    
    void resetLadderSet();
    
    void resetLadderVector();
    
    Node* getLadderSetHelper();
    
    Sprite* getSpriteHelper(Node *node);
    
    void handleMagnetEffect(Node *node, float dt);

    Vector<Node*> m_ladderVector;
    
    Node *m_ladderSet1;
    
    Node *m_ladderSet2;
    
    int m_rewardLevel;
};

#endif /* defined(__HiFly__LadderLayer__) */
