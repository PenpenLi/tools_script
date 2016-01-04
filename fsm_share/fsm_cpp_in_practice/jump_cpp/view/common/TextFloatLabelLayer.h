#ifndef __HiFly__TextFloatLabelLayer__
#define __HiFly__TextFloatLabelLayer__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "JsonConfigUtil.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

class TextFloatLabelLayer : public cocos2d::Layer
{
public:
    static TextFloatLabelLayer* getInstance();
    
    void show(std::string msg);
    
    void hide();
    
    bool isShow();

	bool init();
    
private:
    TextFloatLabelLayer() {};
    
    virtual ~TextFloatLabelLayer() {};
    
    void menuCloseCallback(Ref* pSender);
};

#endif /* defined(__HiFly__MsgBox__) */
