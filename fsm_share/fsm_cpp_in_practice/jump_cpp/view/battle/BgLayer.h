//
//  BgLayer.h
//  HiFly
//
//  Created by chris li on 14-5-13.
//
//

#ifndef __HiFly__BgLayer__
#define __HiFly__BgLayer__

#include "cocos2d.h"
#include "BattleDefine.h"

USING_NS_CC;

class BgLayer : public Node
{
public:
    CREATE_FUNC(BgLayer);
    
    BgLayer();
    
    virtual ~BgLayer();
    
    void sceneUpdate(float dt);
    
    void ntf(HeroNtf &heroNtf);
    
private:
    bool init();
    
    void onEnter();
    
    void onExit();
    
    void scrollDown(float distance);

	void torpedoWarning(float x);
    
	Node *m_layer1;
    
	Node *m_layer2;
    
	Node *m_layer3;
    
	Sprite *m_sizeBg1;
    
	Sprite *m_sizeBg2;

	Sprite *m_blackBg;

	Sprite *m_warning;
};

#endif /* defined(__HiFly__BgLayer__) */
