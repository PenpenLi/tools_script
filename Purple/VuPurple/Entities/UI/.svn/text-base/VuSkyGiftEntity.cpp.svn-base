/*!
 * \file VuSkyGiftEntity.cpp
 * \date 2014/12/17 6:14
 *
 * \author: yhu
 * Contact: 
 *
 * \brief gift system
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

#include "VuPurple/Managers/VuSkyGiftManager.h"



class VuSkyGiftEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuSkyGiftEntity();


protected:
	// scripting
	VuRetVal			showGiftUI(const VuParams &params);


	// components
	VuScriptComponent	*mpScriptComponent;

};

IMPLEMENT_RTTI(VuSkyGiftEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuSkyGiftEntity);


//*****************************************************************************
VuSkyGiftEntity::VuSkyGiftEntity()
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));


	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuSkyGiftEntity, showGiftUI);

}



//*****************************************************************************
VuRetVal VuSkyGiftEntity::showGiftUI(const VuParams &params)
{
	VuSkyGiftManager::IF()->showGiftUI(true);
	return VuRetVal();
}
