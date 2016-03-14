/*!
 * \file VuMoreGameEntity.cpp
 * \date 2014/12/08 13:14
 *
 * \author: xlm
 * Contact: foreverxuliming@gmail.com
 *
 * \brief 更多游戏
 *
 * TODO: long description
 *
 * \note
*/

#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Components/2dLayout/Vu2dLayoutComponent.h"
#include "VuEngine/Properties/VuFontEnumProperty.h"
#include "VuEngine/UI/VuUIPropertyUtil.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/Font/VuFontDraw.h"
#include "VuEngine/Gfx/VuGfxDrawParams.h"
#include "VuEngine/Managers/VuViewportManager.h"
#include "VuEngine/Pfx/VuPfx.h"
#include "VuEngine/DB/VuStringDB.h"
#include "VuEngine/UI/VuUIDrawUtil.h"
#include "VuEngine/Util/VuTimeUtil.h"
#include "VuEngine/Util/VuStringUtil.h"

#include "VuPurple/Managers/VuBillingManager.h"



class VuMoreGameEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuMoreGameEntity();


protected:
	// scripting
	VuRetVal			doMoreGame(const VuParams &params);


	// components
	VuScriptComponent	*mpScriptComponent;

};

IMPLEMENT_RTTI(VuMoreGameEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuMoreGameEntity);


//*****************************************************************************
VuMoreGameEntity::VuMoreGameEntity()
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));


	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuMoreGameEntity, doMoreGame);

}



//*****************************************************************************
VuRetVal VuMoreGameEntity::doMoreGame(const VuParams &params)
{
	/*
	#if defined VUANDROID
	if ( !VuBillingManager::IF()->isBusy() )
	{
	VuBillingManager::IF()->startPurchase(mItemName);
	}

	#elif defined VUIOS

	#elif defined VUWIN32 || defined VUWINSTORE || defined VUWINPHONE
	if (!VuBillingManager::IF()->isBusy())
	{
	VuBillingManager::IF()->startPurchase(mItemName);
	}

	#endif

	return VuRetVal();*/

	//原版


	/*if (!VuBillingManager::IF()->isBusy())
	{
		const std::string &selectedCar = VuGameUtil::IF()->getSelectedCar();
		mItemName = selectedCar;
		VuBillingManager::IF()->startPurchase(mItemName);
	}*/

	VuBillingManager::IF()->moreGame();
	return VuRetVal();
}
