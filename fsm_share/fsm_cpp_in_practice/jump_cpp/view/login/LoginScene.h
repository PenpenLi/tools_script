//
//  LoginScene.h
//  HiFly
//
//  Created by chris li on 14-5-6.
//
//

#ifndef __HiFly__LoginScene__
#define __HiFly__LoginScene__

#include "cocos2d.h"

class LoginScene : public cocos2d::Scene
{
public:
    LoginScene():m_loginLayer(nullptr) {}
    
    virtual ~LoginScene() {}
    
    CREATE_FUNC(LoginScene);
    
    // property
    CC_SYNTHESIZE_READONLY(cocos2d::Layer*, m_loginLayer, LoginLayer);

private:
    bool init();
    
    void onEnter();
    
    void onExit();
};

#endif /* defined(__HiFly__LoginScene__) */
