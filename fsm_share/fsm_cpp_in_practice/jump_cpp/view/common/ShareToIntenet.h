//
//  ShareToIntenet.h
//  HiFly
//
//  Created by Artuira on 14-7-16.
//
//

#ifndef __HiFly__ShareToIntenet__
#define __HiFly__ShareToIntenet__

#include <iostream>
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"



using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

class ShareToIntenet : public LayerColor
{
public:
    static ShareToIntenet* getInstance();
    
    void showShareBox();
private:
    bool init();
    
    
	
};

#endif /* defined(__HiFly__ShareToIntenet__) */
