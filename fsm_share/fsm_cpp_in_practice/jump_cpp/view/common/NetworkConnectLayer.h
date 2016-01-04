#ifndef __HiFly__NetworkConnectLayer__
#define __HiFly__NetworkConnectLayer__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

class NetworkConnectLayer : public LayerColor
{
public:
    static NetworkConnectLayer* getInstance();
	
	void networkConnect();
    void networkConnectStop();

private:
    bool init();


	EventListenerTouchOneByOne* m_listner;
	Sprite* m_sprine;
	Animate* m_sprineAnimate;

	int m_connectCount;
};


#endif //__HiFly__NetworkConnectLayer__