//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuHUDScorePopupEntity
// 
//*****************************************************************************

#include "VuPurple/Managers/VuCarManager.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarPowerUpController.h"
#include "VuEngine/Components/2dLayout/Vu2dLayoutComponent.h"
#include "VuEngine/Properties/VuFontEnumProperty.h"
#include "VuEngine/Assets/VuTextureAsset.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/Font/VuFontDraw.h"
#include "VuEngine/UI/VuUI.h"
#include "VuEngine/UI/VuUIDrawUtil.h"
#include "VuEngine/UI/VuUIPropertyUtil.h"
#include "VuEngine/Util/VuStringUtil.h"


//*****************************************************************************
// Slot
//*****************************************************************************

class VuHUDScorePopupEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuHUDScorePopupEntity();

protected:
	// event handlers
	void					OnUIDraw(const VuParams &params);

	// properties
	int						mViewport;
	std::string				mFont;
};

IMPLEMENT_RTTI(VuHUDScorePopupEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuHUDScorePopupEntity);

//*****************************************************************************
VuHUDScorePopupEntity::VuHUDScorePopupEntity():
	mViewport(0)
{
	// properties
	addProperty(new VuIntProperty("Viewport", mViewport));
	addProperty(new VuFontEnumProperty("Font", mFont));

	// event handlers
	REG_EVENT_HANDLER(VuHUDScorePopupEntity, OnUIDraw);
}

//*****************************************************************************
void VuHUDScorePopupEntity::OnUIDraw(const VuParams &params)
{
	float alpha = 1.0f;

	if ( VuCarEntity *pCar = VuCarManager::IF()->getCameraTargetForViewport(mViewport) )
	{
		for ( const auto &scorePopup : pCar->getScorePopups() )
		{
			if ( !scorePopup.mBoing.isDone() )
			{
				char strText[256];
				VuStringUtil::integerFormat(scorePopup.mScore, strText, 256);
		
				VuRect rect(scorePopup.mScreenPos.mX, scorePopup.mScreenPos.mY, 0.0f, 0.0f);
				VUUINT32 flags = VUGFX_TEXT_DRAW_HCENTER|VUGFX_TEXT_DRAW_VCENTER;

				const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mFont);
				VuFontDrawParams fdParams = fontEntry.params();
				fdParams.mSize *= scorePopup.mBoing.mScale;

				float alpha = 1.0f - scorePopup.mBoing.getCurTime()/scorePopup.mBoing.mDuration;

				VuGfxUtil::IF()->fontDraw()->drawString(0.0f, fontEntry.font(), strText, fdParams, rect, flags, alpha);
			}
		}
	}
}
