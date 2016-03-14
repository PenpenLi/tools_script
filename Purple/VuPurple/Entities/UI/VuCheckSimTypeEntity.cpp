/*!
 * \file VuCheckSimTypeEntity.cpp
 * \date 2014/12/08 10:58
 *
 * \author: xlm
 * Contact: foreverxuliming@gmail.com
 *
 * \brief 用来检测是哪个运营商
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


//*****************************************************************************
// 
//*****************************************************************************
class VuCheckSimTypeEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuCheckSimTypeEntity();

protected:
	/*!
	* \author: xlm 2014/11/29
	* \brief: is a game coin item or rmb item
	*/
	VuRetVal			checkSimType(const VuParams &params); //return a bool value to the VuIfEntity

	// properties
	int					mSimType;
	// components
	VuScriptComponent	*mpScriptComponent;
};


IMPLEMENT_RTTI(VuCheckSimTypeEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuCheckSimTypeEntity);


//*****************************************************************************
VuCheckSimTypeEntity::VuCheckSimTypeEntity()
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	// scripting
	ADD_SCRIPT_INPUT(mpScriptComponent, VuCheckSimTypeEntity, checkSimType, VuRetVal::Bool, VuParamDecl());

	addProperty(new VuIntProperty("SimType", mSimType));

}

//*****************************************************************************
VuRetVal VuCheckSimTypeEntity::checkSimType(const VuParams &params)
{
	bool bRet = false;
	int nType = VuBillingManager::IF()->getCurSimType();
	if (nType == mSimType)
		bRet = true;
	else
		bRet = false;
	return VuRetVal(bRet);
	
}
