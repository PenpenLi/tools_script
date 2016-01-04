
#ifndef __HiFly__StartAnimation__
#define __HiFly__StartAnimation__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "CCNodeGrid.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCCustomCommand.h"

using namespace cocos2d;
using namespace cocostudio;

class StartAnimation : public cocos2d::Layer
{
public:
	StartAnimation(void);
	~StartAnimation(void);

	virtual bool init();
	CREATE_FUNC(StartAnimation);
	//¶¯»­¼àÌýÊÂ¼þ
	void animationEvent(cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string& movementID);
    
    CC_SYNTHESIZE(int, m_enterType, EnterType);
};

#endif