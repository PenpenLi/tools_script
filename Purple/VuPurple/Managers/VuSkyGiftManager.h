//*****************************************************************************
// 
//  VuSkyGiftManager class
// 
//*****************************************************************************




#ifndef __VUSKY_GIFT_MANAGER_H__
#define __VUSKY_GIFT_MANAGER_H__







#include "VuEngine/VuSystemComponent.h"
#include "VuEngine/Managers/VuMessageBoxManager.h"

class VuSkyGiftManager : public VuSystemComponent
{
	DECLARE_SYSTEM_COMPONENT(VuSkyGiftManager)

protected:
	// called by game
	friend class VuPurple;
	virtual bool init();
	virtual void release();


public:
	virtual void	showGiftUI( bool bshow = true);
	//Callback
	virtual void	onGiftCallback(const std::string& giftText );
};


#endif

