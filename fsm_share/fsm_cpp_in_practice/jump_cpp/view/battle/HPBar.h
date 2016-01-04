//
//  HPBar.h
//  test
//
//  Created by Artuira on 14-2-10.
//
//

#ifndef __test__HPBar__
#define __test__HPBar__

#include "cocos2d.h"


USING_NS_CC;


class HPBar : public Layer
{
public:
    HPBar();
    ~HPBar();
    
    static HPBar* createHPBar();
    CREATE_FUNC(HPBar);
    bool init();
    
public:
    
public:
    void setOnDisplay();
    void setOffDisplay();
    
    void setHP(int scale);
    
    void setHPre();
private:
    float m_fWidth;
    float m_fHeight;
    float m_fGrid;
    
    int m_iLastScale;
    int m_iCurrentScale;
    int m_iPreScale;
    
    Sprite* m_ccsBar;
    Sprite* m_ccsBarPre;
    Sprite* m_ccsOctopusSprite;
};
#endif /* defined(__test__HPBar__) */
