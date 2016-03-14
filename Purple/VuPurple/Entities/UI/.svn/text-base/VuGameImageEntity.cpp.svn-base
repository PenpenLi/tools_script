//*****************************************************************************
//
//  Copyright (c) 2012-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Game Image entity
// 
//*****************************************************************************

#include "VuGameImageEntity.h"
#include "VuEngine/UI/VuUI.h"
#include "VuEngine/UI/VuUIDrawUtil.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Components/2dLayout/Vu2dLayoutComponent.h"
#include "VuEngine/Components/VuTransitionComponent.h"
#include "VuEngine/Properties/VuAngleProperty.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/HAL/Gfx/VuTexture.h"


//*****************************************************************************
// VuGameTextBaseEntity
//*****************************************************************************

IMPLEMENT_RTTI(VuGameImageBaseEntity, VuEntity);


//*****************************************************************************
VuGameImageBaseEntity::VuGameImageBaseEntity():
	mInitiallyVisible(true),
	mColor(255,255,255),
	mDstRect(40,40,20,20),
	mSrcRect(0,0,1,1),
	mRotation(0),
	mFlipX(false),
	mFlipY(false),
	mAlpha(1.0f),
	mVisible(true)
{
	// properties
	addProperty(new VuBoolProperty("Visible", mInitiallyVisible));
	addProperty(new VuColorProperty("Color", mColor));
	addProperty(new VuRectProperty("Rect", mDstRect));
	addProperty(new VuRectProperty("Src Rect", mSrcRect));
	addProperty(new VuAngleProperty("Rotation", mRotation));
	addProperty(new VuBoolProperty("FlipX", mFlipX));
	addProperty(new VuBoolProperty("FlipY", mFlipY));
	ADD_UI_ANCHOR_PROPERTIES(getProperties(), mAnchor, "");

	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this, 100));
	addComponent(new Vu2dLayoutComponent(this, &VuGameImageBaseEntity::drawLayout));
	addComponent(mpTransitionComponent = new VuTransitionComponent(this));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(getComponent<VuScriptComponent>(), VuGameImageBaseEntity, Show);
	ADD_SCRIPT_INPUT_NOARGS(getComponent<VuScriptComponent>(), VuGameImageBaseEntity, Hide);

	// event handlers
	REG_EVENT_HANDLER(VuGameImageBaseEntity, OnUITick);
	REG_EVENT_HANDLER(VuGameImageBaseEntity, OnUIDraw);
}

//*****************************************************************************
void VuGameImageBaseEntity::onGameInitialize()
{
	mVisible = mInitiallyVisible;
}

//*****************************************************************************
void VuGameImageBaseEntity::OnUITick(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	float fdt = accessor.getFloat();

	mpTransitionComponent->tick(fdt);
}

//*****************************************************************************
void VuGameImageBaseEntity::OnUIDraw(const VuParams &params)
{
	if ( mVisible )
		drawImage(mpTransitionComponent->alpha());
}

//*****************************************************************************
void VuGameImageBaseEntity::drawImage(float alpha)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	VuGfxUtil *pGfxUtil = VuGfxUtil::IF();
	float authAR = uiDrawParams.mAuthScale.mX/uiDrawParams.mAuthScale.mY;

	VuRect dstRect = uiDrawParams.transform(mDstRect);

	mAnchor.apply(dstRect, dstRect);

	VuVector3 vCenter(dstRect.getCenter().mX, dstRect.getCenter().mY, 0);
	VuMatrix mat = VuMatrix::identity();
	mat.translate(-vCenter);
	mat.scale(VuVector3(authAR, 1.0f, 1.0f));
	mat.rotateZ(mRotation);
	mat.scale(VuVector3(1.0f/authAR, 1.0f, 1.0f));
	mat.translate(vCenter);

	VuRect srcRect = mSrcRect;
	if ( mFlipX )
	{
		srcRect.mX += srcRect.mWidth;
		srcRect.mWidth *= -1;
	}
	if ( mFlipY )
	{
		srcRect.mY += srcRect.mHeight;
		srcRect.mHeight *= -1;
	}

	VuColor color = mColor;
	color.mA = VuRound(color.mA*alpha*mAlpha);

	pGfxUtil->pushMatrix(mat*pGfxUtil->getMatrix());
	{
		if(VuTexture *pTexture = getTexture())
		{
			pGfxUtil->drawTexture2d(uiDrawParams.mDepth, pTexture, color, dstRect, srcRect);
		}
		else
		{
			pGfxUtil->drawFilledRectangle2d(uiDrawParams.mDepth, color, dstRect);
		}
	}
	pGfxUtil->popMatrix();
}


//*****************************************************************************
// VuGameTextEntity
//*****************************************************************************

class VuGameImageEntity : public VuGameImageBaseEntity
{
	DECLARE_RTTI

public:
	VuGameImageEntity();

	virtual VuTexture	*getTexture() { return mImage.getTexture(); }

protected:
	VuUIImageProperties	mImage;
};

IMPLEMENT_RTTI(VuGameImageEntity, VuGameImageBaseEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuGameImageEntity);


//*****************************************************************************
VuGameImageEntity::VuGameImageEntity()
{
	// properties
	mImage.addProperties(getProperties(), "Texture Asset");
}
