//
//  MsgBox.h
//  HiFly
//
//  Created by chris li on 14-4-28.
//
//

#ifndef __HiFly__MsgBox__
#define __HiFly__MsgBox__

#include "cocos2d.h"

class MsgBox : public cocos2d::Layer
{
public:
    static MsgBox* getInstance();
    
    void show(std::string msg);
    
    void hide();
    
    bool isShow();
    
private:
    MsgBox() {};
    
    virtual ~MsgBox() {};
    
    void menuCloseCallback(Ref* pSender);
};

#endif /* defined(__HiFly__MsgBox__) */
