//
//  HomeScene.h
//  HiFly
//
//  Created by chris li on 14-5-6.
//
//

#ifndef __HiFly__HomeScene__
#define __HiFly__HomeScene__

#include "cocos2d.h"

class HomeScene : public cocos2d::Scene
{
public:
    CREATE_FUNC(HomeScene);
    
private:
	bool init();

	void onEnter();
    
    void onExit();

	//time schedule
	void scheduleHpTime(float delayTime);
	float m_errorTime;
};

#endif /* defined(__HiFly__HomeScene__) */