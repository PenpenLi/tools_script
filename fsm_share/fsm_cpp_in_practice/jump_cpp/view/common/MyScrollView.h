#ifndef __HiFly__MyScrollView__
#define __HiFly__MyScrollView__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

class MyScrollView : public ScrollView
{
public:
	void scrollToPoint();

	void logSizePositionInfo();

	void updateOffset(float time);

	int getFrontRoleId();

	void refreshRolePostion(int roleId);

	//void setSwallowsTouches(bool swallowsTouches);

	////operators
	//void setTouchEnabled(bool enabled);
    
private:
    bool init();
    
    //void onEnter();
    //
    //void onExit();
    
//function
	//import login Res

	//bool importUI();

	//touchEvent
    
//member variable
	bool _swallowsTouches;
	Touch::DispatchMode _touchMode;
	EventListener* _touchListener;
};


#endif //__HiFly__MyScrollView__