//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  ProgressBar entity
// 
//*****************************************************************************

#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/2dLayout/Vu2dLayoutComponent.h"
#include "VuEngine/UI/VuUI.h"
#include "VuEngine/UI/VuUIDrawUtil.h"
#include "VuEngine/UI/VuUIPropertyUtil.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/GfxSort/VuGfxSort.h"



class VuProgressBarEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuProgressBarEntity();

protected:
	// event handlers
	void							OnProgressUpdate(const VuParams &params);
	void							OnUIDraw(const VuParams &params);

	void							drawLayout(bool bSelected);
	void							drawProgress(float progress, float alpha);

	// components

	// properties
	VuUIRectProperties				mRect;
	VuColor							mColor;
	VuUIAnchorProperties			mAnchor;

	float							mProgress;
};

IMPLEMENT_RTTI(VuProgressBarEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuProgressBarEntity);


//*****************************************************************************
VuProgressBarEntity::VuProgressBarEntity():
	mColor(255,255,255)
{
	// properties
	addProperty(new VuRectProperty("Rect", mRect));
	addProperty(new VuColorProperty("Color", mColor));
	ADD_UI_ANCHOR_PROPERTIES(getProperties(), mAnchor, "");

	// components
	addComponent(new Vu2dLayoutComponent(this, &VuProgressBarEntity::drawLayout));

	// event handlers
	REG_EVENT_HANDLER(VuProgressBarEntity, OnProgressUpdate);
	REG_EVENT_HANDLER(VuProgressBarEntity, OnUIDraw);
}

//*****************************************************************************
void VuProgressBarEntity::OnProgressUpdate(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	mProgress = accessor.getFloat();
}

//*****************************************************************************
void VuProgressBarEntity::OnUIDraw(const VuParams &params)
{
	float alpha = 1.0f;

	drawProgress(mProgress, alpha);
}

//*****************************************************************************
void VuProgressBarEntity::drawLayout(bool bSelected)
{
	if ( bSelected )
	{
		VuUIDrawParams uiDrawParams;
		VuUIDrawUtil::getParams(this, uiDrawParams);

		VuGfxUtil::IF()->drawRectangleOutline2d(uiDrawParams.mDepth, mColor, uiDrawParams.transform(mRect));
	}

	drawProgress(0.5f, 1.0f);
}

//*****************************************************************************
void VuProgressBarEntity::drawProgress(float progress, float alpha)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	VuRect rect = uiDrawParams.transform(mRect);
	rect.mWidth *= progress;
	mAnchor.apply(rect, rect);

	VuColor color = mColor;
	color.mA = VuRound(color.mA*alpha);

	VuGfxUtil::IF()->drawFilledRectangle2d(uiDrawParams.mDepth, color, rect);
}
