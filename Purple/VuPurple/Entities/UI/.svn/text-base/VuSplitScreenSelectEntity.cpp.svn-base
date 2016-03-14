//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Split Screen Select Entity
// 
//*****************************************************************************

#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Components/2dLayout/Vu2dLayoutComponent.h"
#include "VuEngine/Properties/VuBasicProperty.h"
#include "VuEngine/Properties/VuFontEnumProperty.h"
#include "VuEngine/UI/VuUI.h"
#include "VuEngine/UI/VuUIDrawUtil.h"
#include "VuEngine/UI/VuUIPropertyUtil.h"
#include "VuEngine/UI/VuUIInputUtil.h"
#include "VuEngine/HAL/GamePad/VuGamePad.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/Font/VuFontDraw.h"


class VuSplitScreenSelectEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuSplitScreenSelectEntity();

	virtual void		onGameInitialize();
	virtual void		onGameRelease();

protected:
	// event handlers
	void				OnUITick(const VuParams &params);
	void				OnUIGamePad(const VuParams &params);
	void				OnUIDraw(const VuParams &params);

	// scripting
	VuRetVal			IsValid(const VuParams &params);

	void				drawLayout(bool bSelected);
	void				drawIcons(int index, VUUINT32 padMask, float alpha);

	// components
	VuScriptComponent	*mpScriptComponent;

	// properties
	int					mPlayerCount;
	VuVector2			mPositions[5];
	VuVector2			mOffset;
	VuUIImageProperties	mDeviceImages[VuGamePad::NUM_DEVICE_TYPES];
	VuUIRectProperties	mImageRect;
	VuVector2			mIndexPosition;
	std::string			mIndexFont;

	int					mPadMasks[5];
};


IMPLEMENT_RTTI(VuSplitScreenSelectEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuSplitScreenSelectEntity);


//*****************************************************************************
VuSplitScreenSelectEntity::VuSplitScreenSelectEntity():
	mPlayerCount(4),
	mOffset(0,0),
	mIndexPosition(0,0)
{
	memset(mPositions, 0, sizeof(mPositions));

	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));
	addComponent(new Vu2dLayoutComponent(this, &VuSplitScreenSelectEntity::drawLayout));

	// properties
	addProperty(new VuIntProperty("Player Count", mPlayerCount));
	addProperty(new VuVector2Property("Offset", mOffset));
	addProperty(new VuVector2Property("Position 0", mPositions[0]));
	addProperty(new VuVector2Property("Position 1", mPositions[1]));
	addProperty(new VuVector2Property("Position 2", mPositions[2]));
	addProperty(new VuVector2Property("Position 3", mPositions[3]));
	addProperty(new VuVector2Property("Position 4", mPositions[4]));
	mDeviceImages[VuGamePad::DEVICE_UNKNOWN].addProperties(getProperties(), "Unknown Image");
	mDeviceImages[VuGamePad::DEVICE_GAMEPAD].addProperties(getProperties(), "GamePad Image");
	mDeviceImages[VuGamePad::DEVICE_SIMPLE_GAMEPAD].addProperties(getProperties(), "Simple GamePad Image");
	mDeviceImages[VuGamePad::DEVICE_REMOTE_CONTROL].addProperties(getProperties(), "Remote Control Image");
	mDeviceImages[VuGamePad::DEVICE_STEERING_WHEEL].addProperties(getProperties(), "Steering Wheel Image");
	addProperty(new VuRectProperty("Image Rect", mImageRect));
	addProperty(new VuVector2Property("Index Position", mIndexPosition));
	addProperty(new VuFontEnumProperty("Index Font", mIndexFont));

	// scripting
	ADD_SCRIPT_INPUT(mpScriptComponent, VuSplitScreenSelectEntity, IsValid, VuRetVal::Bool, VuParamDecl());

	// event handlers
	REG_EVENT_HANDLER(VuSplitScreenSelectEntity, OnUITick);
	REG_EVENT_HANDLER(VuSplitScreenSelectEntity, OnUIGamePad);
	REG_EVENT_HANDLER(VuSplitScreenSelectEntity, OnUIDraw);
}

//*****************************************************************************
void VuSplitScreenSelectEntity::onGameInitialize()
{
	mPadMasks[0] = 0xff;
	mPadMasks[1] = 0x0;
	mPadMasks[2] = 0x0;
	mPadMasks[3] = 0x0;
	mPadMasks[4] = 0x0;

	mPlayerCount = VuClamp(mPlayerCount, 2, 4);
}

//*****************************************************************************
void VuSplitScreenSelectEntity::onGameRelease()
{
	VuJsonContainer &pads = VuGameUtil::IF()->dataWrite()["SplitScreenPads"];
	pads.resize(mPlayerCount);
	for ( int i = 0; i < mPlayerCount; i++ )
		pads[i].putValue((int)VuHighBit(mPadMasks[i+1]));
}

//*****************************************************************************
void VuSplitScreenSelectEntity::OnUITick(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	float fdt = accessor.getFloat();

	for ( int padIndex = 0; padIndex < VuGamePad::MAX_NUM_PADS; padIndex++ )
	{
		if ( !VuGamePad::IF()->getController(padIndex).mIsConnected )
		{
			VUUINT32 padMask = 1<<padIndex;

			for ( int i = 1; i <= mPlayerCount; i++ )
				mPadMasks[i] &= ~padMask;
			mPadMasks[0] |= padMask;
		}
	}
}

//*****************************************************************************
void VuSplitScreenSelectEntity::OnUIGamePad(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	int channel = accessor.getInt();
	bool down = accessor.getBool();
	int padIndex = accessor.getInt();

	if ( down )
	{
		bool up = channel == VuUIInputUtil::CHANNEL_UP;
		bool down = channel == VuUIInputUtil::CHANNEL_DOWN;
		bool left = channel == VuUIInputUtil::CHANNEL_LEFT;
		bool right = channel == VuUIInputUtil::CHANNEL_RIGHT;

		if ( up | down | left | right )
		{
			VUUINT32 padMask = 1<<padIndex;

			// determine current window
			int curWindow = 0;
			for ( int i = 1; i <= mPlayerCount; i++ )
				if ( padMask & mPadMasks[i] )
					curWindow = i;

			int newWindow = curWindow;

			if ( mPlayerCount == 2 )
			{
				if ( curWindow == 0 )
				{
					if ( up ) newWindow = 1;
					if ( down ) newWindow = 2;
				}
				else if ( curWindow == 1 )
				{
					if ( down ) newWindow = 2;
				}
				else if ( curWindow == 2 )
				{
					if ( up ) newWindow = 1;
				}
			}
			else if ( mPlayerCount == 3 )
			{
				if ( curWindow == 0 )
				{
					if ( up ) newWindow = 1;
					if ( down ) newWindow = 2;
					if ( left ) newWindow = 2;
					if ( right ) newWindow = 3;
				}
				else if ( curWindow == 1 )
				{
					if ( down ) newWindow = 2;
					if ( left ) newWindow = 2;
					if ( right ) newWindow = 3;
				}
				else if ( curWindow == 2 )
				{
					if ( up ) newWindow = 1;
					if ( right ) newWindow = 3;
				}
				else if ( curWindow == 3 )
				{
					if ( up ) newWindow = 1;
					if ( left ) newWindow = 2;
				}
			}
			else if ( mPlayerCount == 4 )
			{
				if ( curWindow == 0 )
				{
					if ( up ) newWindow = 1;
					if ( down ) newWindow = 3;
					if ( left ) newWindow = 1;
					if ( right ) newWindow = 2;
				}
				else if ( curWindow == 1 )
				{
					if ( down ) newWindow = 3;
					if ( right ) newWindow = 2;
				}
				else if ( curWindow == 2 )
				{
					if ( down ) newWindow = 4;
					if ( left ) newWindow = 1;
				}
				else if ( curWindow == 3 )
				{
					if ( up ) newWindow = 1;
					if ( right ) newWindow = 4;
				}
				else if ( curWindow == 4 )
				{
					if ( up ) newWindow = 2;
					if ( left ) newWindow = 3;
				}
			}

			if ( newWindow != curWindow )
			{
				VuGameUtil::IF()->playSfx(VuGameUtil::SFX_UI_NAVIGATE);

				for ( int i = 0; i <= mPlayerCount; i++ )
					mPadMasks[i] &= ~padMask;
				mPadMasks[newWindow] |= padMask;
			}
		}
	}
}

//*****************************************************************************
void VuSplitScreenSelectEntity::OnUIDraw(const VuParams &params)
{
	float alpha = 1.0f;

	for ( int i = 0; i <= mPlayerCount; i++ )
		drawIcons(i, mPadMasks[i], alpha);
}

//*****************************************************************************
VuRetVal VuSplitScreenSelectEntity::IsValid(const VuParams &params)
{
	bool valid = true;

	for ( int i = 1; i <= mPlayerCount; i++ )
		if ( VuBitCount(mPadMasks[i]) != 1 )
			valid = false;

	return VuRetVal(valid);
}

//*****************************************************************************
void VuSplitScreenSelectEntity::drawLayout(bool bSelected)
{
	for ( int i = 0; i <= VuClamp(mPlayerCount, 2, 4); i++ )
		drawIcons(i, 0xff, 1.0f);
}

//*****************************************************************************
void VuSplitScreenSelectEntity::drawIcons(int index, VUUINT32 padMask, float alpha)
{
	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	VuVector2 offset = mPositions[index];

	for ( int padIndex = 0; padIndex < VuGamePad::MAX_NUM_PADS; padIndex++ )
	{
		if ( (1<<padIndex) & padMask )
		{
			if ( !VuGamePad::IF() || VuGamePad::IF()->getController(padIndex).mIsConnected )
			{
				// image
				{
					VuTexture *pTexture = VuGfxUtil::IF()->whiteTexture();
					if ( VuGamePad::IF() )
					{
						int deviceType = VuGamePad::IF()->getController(padIndex).mDeviceType;
						pTexture = mDeviceImages[deviceType].getTexture();
					}

					VuColor color(255,255,255);
					color.mA = (VUUINT8)VuRound(alpha*255.0f);
					VuGfxUtil::IF()->drawTexture2d(uiDrawParams.mDepth + GFX_SORT_DEPTH_STEP, pTexture, color, uiDrawParams.transform(mImageRect + offset));
				}

				// index
				{
					const VuFontDB::VuEntry &fontEntry = VuFontDB::IF()->getFont(mIndexFont);
					char strIndex[32];
					VU_SPRINTF(strIndex, sizeof(strIndex), "%d", padIndex + 1);
					VuVector2 pos = uiDrawParams.transform(mIndexPosition + offset);
					VuRect rect(pos.mX, pos.mY, 0.0f, 0.0f);
					VUUINT32 flags = VUGFX_TEXT_DRAW_HCENTER|VUGFX_TEXT_DRAW_VCENTER;
					VuGfxUtil::IF()->fontDraw()->drawString(uiDrawParams.mDepth, fontEntry.font(), strIndex, fontEntry.params(), rect, flags, alpha);
				}

				offset += mOffset;
			}
		}
	}
}
