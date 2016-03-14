//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  UI Table class
// 
//*****************************************************************************

#include "VuTableEntity.h"
#include "VuEngine/Components/2dLayout/Vu2dLayoutComponent.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Components/VuTransitionComponent.h"
#include "VuEngine/Assets/VuTextureAsset.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/Font/VuFontDraw.h"
#include "VuEngine/Gfx/GfxSort/VuGfxSort.h"
#include "VuEngine/UI/VuUI.h"
#include "VuEngine/UI/VuUIDrawUtil.h"


IMPLEMENT_RTTI(VuTableEntity, VuEntity);


//*****************************************************************************
VuTableEntity::VuTableEntity():
	mInitiallyVisible(true),
	mMaxRows(6),
	mSpacing(0.0f),
	mHighlightColor(255,255,255),
	mRowRect(0,0,0,0),
	mRowSrcRect(0,0,1,1),
	mRowHighlightSrcRect(0,0,1,1),
	mVisible(true)
{
	// components
	addComponent(new Vu2dLayoutComponent(this, &VuTableEntity::drawLayout));
	addComponent(mpScriptComponent = new VuScriptComponent(this));
	addComponent(mpTransitionComponent = new VuTransitionComponent(this));

	// properties
	addProperty(new VuBoolProperty("Initially Visible", mInitiallyVisible));
	addProperty(new VuIntProperty("Max Rows", mMaxRows));
	addProperty(new VuFloatProperty("Spacing", mSpacing));
	addProperty(new VuColorProperty("Highlight Color", mHighlightColor));
	addProperty(new VuRectProperty("Row Rect", mRowRect));
	addProperty(mpRowTextureAssetProperty = new VuAssetProperty<VuTextureAsset>("Row Texture", mRowTexture));
	addProperty(mpRowHighlightTextureAssetProperty = new VuAssetProperty<VuTextureAsset>("Row Highlight Texture", mRowHighlightTexture));
	addProperty(new VuRectProperty("Row Src Rect", mRowSrcRect));
	addProperty(new VuRectProperty("Row Highlight Src Rect", mRowHighlightSrcRect));
	ADD_UI_ANCHOR_PROPERTIES(getProperties(), mAnchor, "");

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuTableEntity, Show);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuTableEntity, Hide);

	// event handlers
	REG_EVENT_HANDLER(VuTableEntity, OnUITick);
	REG_EVENT_HANDLER(VuTableEntity, OnUIDraw);
}

//*****************************************************************************
VuTableEntity::~VuTableEntity()
{
	for ( int iCol = 0; iCol < (int)mColumns.size(); iCol++ )
		delete mColumns[iCol];
}

//*****************************************************************************
void VuTableEntity::onGameInitialize()
{
	mVisible = mInitiallyVisible;
}

//*****************************************************************************
void VuTableEntity::OnUITick(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	float fdt = accessor.getFloat();

	mpTransitionComponent->tick(fdt);
}

//*****************************************************************************
void VuTableEntity::OnUIDraw(const VuParams &params)
{
	if ( mVisible )
		drawTable(mpTransitionComponent->alpha());
}

//*****************************************************************************
void VuTableEntity::drawLayout(bool bSelected)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	VuFontDraw *pFontDraw = VuGfxUtil::IF()->fontDraw();

	for ( int iRow = 0; iRow < mMaxRows; iRow++ )
	{
		bool highlight = (iRow == 0);

		drawElements(uiDrawParams, iRow, highlight, 1.0f);

		// draw text
		for ( int iCol = 0; iCol < (int)mColumns.size(); iCol++ )
		{
			const Column &column = *mColumns[iCol];
			if ( column.mEnabled )
			{
				const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(column.mFont);
				VuRect dstRect = uiDrawParams.transform(column.mRect + VuVector2(0.0f, iRow*mSpacing));

				VuFontDrawParams params = fontEntry.params();
				if ( highlight )
					params.mColor = mHighlightColor;

				pFontDraw->drawString(uiDrawParams.mDepth, fontEntry.font(), column.mColumnName.c_str(), params, dstRect, column.mStringFormat, 1.0f);

				if ( bSelected )
					VuGfxUtil::IF()->drawRectangleOutline2d(uiDrawParams.mDepth - GFX_SORT_DEPTH_STEP, mHighlightColor, dstRect);
			}
		}
	}
}

//*****************************************************************************
void VuTableEntity::drawTable(float alpha)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	VuFontDraw *pFontDraw = VuGfxUtil::IF()->fontDraw();

	for ( int iRow = 0; iRow < getRowCount(); iRow++ )
	{
		bool highlight = getRowHighlight(iRow);

		drawElements(uiDrawParams, iRow, highlight, alpha);

		// draw text
		for ( int iCol = 0; iCol < (int)mColumns.size(); iCol++ )
		{
			const Column &column = *mColumns[iCol];
			if ( column.mEnabled )
			{
				const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(column.mFont);
				VuRect rect = uiDrawParams.transform(column.mRect + VuVector2(0.0f, iRow*mSpacing));
				mAnchor.apply(rect, rect);

				VuFontDrawParams fdParams = fontEntry.params();
				fdParams.mSize *= 0.5f*(uiDrawParams.mLocalScale.mX + uiDrawParams.mLocalScale.mY);
				if ( highlight )
					fdParams.mColor = mHighlightColor;

				pFontDraw->drawString(uiDrawParams.mDepth, fontEntry.font(), getItemText(iRow, iCol), fdParams, rect, column.mStringFormat, alpha);
			}
		}
	}
}

//*****************************************************************************
void VuTableEntity::drawElements(const VuUIDrawParams &uiDrawParams, int iRow, bool highlight, float alpha)
{
	// draw row texture
	VuTextureAsset *pRowTextureAsset = mpRowTextureAssetProperty->getAsset();
	VuRect srcRect = mRowSrcRect;
	if ( highlight )
	{
		pRowTextureAsset = mpRowHighlightTextureAssetProperty->getAsset();
		srcRect = mRowHighlightSrcRect;
	}

	if ( pRowTextureAsset )
	{
		if ( VuTexture *pTexture = pRowTextureAsset->getTexture() )
		{
			VuRect rect = uiDrawParams.transform(mRowRect + VuVector2(0.0f, iRow*mSpacing));
			mAnchor.apply(rect, rect);
			VuColor color(255,255,255, VuRound(255*alpha));
			VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth + GFX_SORT_DEPTH_STEP, pTexture, color, rect, srcRect);
		}
	}
}
