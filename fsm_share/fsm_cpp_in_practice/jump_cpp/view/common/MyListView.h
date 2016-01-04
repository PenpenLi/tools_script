#ifndef __HiFly__MyListView__
#define __HiFly__MyListView__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

class MyListView : public ListView
{
public:
    
	void setDirectionParent(SCROLLVIEW_DIR dir);

	void updateInnerContainerSizeV();

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
};


#endif //__HiFly__MyListView__