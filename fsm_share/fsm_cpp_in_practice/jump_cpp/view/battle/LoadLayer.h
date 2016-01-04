//
//  LoadLayer.h
//  HiFly
//
//  Created by chris li on 14-6-24.
//
//

#ifndef __HiFly__LoadLayer__
#define __HiFly__LoadLayer__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "CCNodeGrid.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCCustomCommand.h"

using namespace cocos2d;
using namespace cocostudio;

USING_NS_CC;

class LoadLayer : public cocos2d::Layer
{
public:
    static LoadLayer* create(int level, std::function<void()> onLoadFinishCallback);
    
    void load();

private:
    LoadLayer() {};
    
    virtual ~LoadLayer() {};
    
    void update(float delta) override;
    
    bool init(int level, std::function<void()> onLoadFinishCallback);
    
    void onEnter();
    
    void onExit();
    
    std::string getLadderSetFileName();
    
    std::function<void()> m_onLoadFinishCallback;
    
    int m_level;
    
    int m_ladderSetTotalNum;
    
    int m_ladderSetLoadNum;
    
    std::string m_levelStr;
    
    Label* m_tipLabel;
};

#endif /* defined(__HiFly__LoadLayer__) */
