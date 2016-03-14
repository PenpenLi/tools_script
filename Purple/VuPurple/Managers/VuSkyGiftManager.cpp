//*****************************************************************************
// 
//  VuSkyGiftManager class
// 
//*****************************************************************************


#include "VuSkyGiftManager.h"

#include "VuPurple/Managers/VuGameManager.h"
#include "VuEngine/Managers/VuProfileManager.h"

#include <stdlib.h>
#include <string.h>
// the interface
//IMPLEMENT_SYSTEM_COMPONENT(VuSkyGiftManager, VuSkyGiftManager);


//*****************************************************************************
bool VuSkyGiftManager::init()
{
	return true;
}

//*****************************************************************************
void VuSkyGiftManager::release()
{

}

//*****************************************************************************
void VuSkyGiftManager::showGiftUI(bool bshow /*=true*/)
{
	
}

//*****************************************************************************
void VuSkyGiftManager::onGiftCallback(const std::string& giftText )
{
	if ( giftText.size() >= 2 ){
		
		const char type = giftText.at(0);
		switch( type )
		{//soft
		case 's':
		case 'S':
		{
			int amount = atoi(giftText.substr(1, giftText.size() - 1).c_str());
			VuGameManager::IF()->addCurrencyEarned(VuGameManager::CT_STANDARD, amount);
			VuProfileManager::IF()->save();
		}
		break;
		//hard
		case 'h':
		case 'H':
		{
			int amount = atoi(giftText.substr(1, giftText.size() - 1).c_str());
			VuGameManager::IF()->addCurrencyEarned(VuGameManager::CT_PREMIUM, amount);
			VuProfileManager::IF()->save();
		}
		break;
		//car
		case 'c':
		case 'C':
		{
			std::string carName = (giftText.substr(1, giftText.size() - 1));
			const VuGameManager::Cars cars = VuGameManager::IF()->getCars();
			 
			if ( cars.end() != cars.find( carName ) )
			{
			   VuGameManager::IF()->giveCar( carName );
			   VuProfileManager::IF()->save();
			}
		}
		break;
		//token
		case 't':
		case 'T':
		{
		    int amount = atoi(giftText.substr(1, giftText.size() - 1).c_str());
			VuGameManager::IF()->addTokenNoLimit( amount );
			VuProfileManager::IF()->save();
		}
		break;
		}
	}
}
