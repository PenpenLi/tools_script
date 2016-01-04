#ifndef __HiFly__TouchedEventMaskLayer__
#define __HiFly__TouchedEventMaskLayer__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

class TouchedEventMaskLayer : public LayerColor
{
public:
    static TouchedEventMaskLayer* getInstance();

	
	void networkConnect();
    void networkConnectStop();

private:
    bool init();


	EventListenerTouchOneByOne* m_listner;
};


#endif //__HiFly__TouchedEventMaskLayer__