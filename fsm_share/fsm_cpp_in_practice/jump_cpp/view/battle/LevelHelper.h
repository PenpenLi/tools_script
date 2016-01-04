//
//  LevelHelper.h
//  HiFly
//
//  Created by chris li on 14-5-15.
//
//

#ifndef __HiFly__LevelHelper__
#define __HiFly__LevelHelper__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocosbuilder/CocosBuilder.h"

USING_NS_CC;
USING_NS_CC_EXT;

class LadderSet : public Node
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(LadderSet, create);
    
    LadderSet() {};
    
    virtual ~LadderSet() {};
};

class LadderSetNodeLoader : public cocosbuilder::NodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(LadderSetNodeLoader, loader);
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(LadderSet);
};

class LevelHelper
{
public:
    static LevelHelper* getInstance();
    
    Node* getLadderSet(int ladderSetType);
    
    void loadLadderSet(std::string ladderSetName);
    
    void loadRewardLadderSet(std::string ladderSetName);
    
    void chooseLevel(int level);
    
    void chooseRewardLevel(int level);
    
    void chooseInfiniteLevel();
    
private:
    Node* readCCBI(std::string &ladderSetName);
    
    cocos2d::Vector<Node*> m_ladderSetVector;
    
    cocos2d::Vector<Node*> m_rewardLadderSetVector;
    
    int m_level;
};

#endif /* defined(__HiFly__LevelHelper__) */
