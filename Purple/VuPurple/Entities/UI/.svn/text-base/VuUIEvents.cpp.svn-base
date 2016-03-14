//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  UI Event entities
// 
//*****************************************************************************

#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Components/2dLayout/Vu2dLayoutComponent.h"
#include "VuEngine/Properties/VuEnumProperty.h"
#include "VuEngine/Properties/VuAngleProperty.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/HAL/Touch/VuTouch.h"
#include "VuEngine/UI/VuUIPropertyUtil.h"
#include "VuEngine/UI/VuUIDrawUtil.h"
#include "VuEngine/UI/VuUIInputUtil.h"
#include "VuEngine/Gfx/VuGfxUtil.h"


//*****************************************************************************
// TouchSpecial
//*****************************************************************************
class VuOnTouchSpecialEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuOnTouchSpecialEntity();

private:
	// event handlers
	void				OnUITick(const VuParams &params);
	void				OnUITouchSpecial(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;

	// properties
	int					mType;

	bool				mbPressed;
};

IMPLEMENT_RTTI(VuOnTouchSpecialEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuOnTouchSpecialEntity);


//*****************************************************************************
VuOnTouchSpecialEntity::VuOnTouchSpecialEntity():
	mType(0),
	mbPressed(false)
{
	addComponent(mpScriptComponent = new VuScriptComponent(this, 100));

	// add properties
	static VuStaticIntEnumProperty::Choice sTypeChoices[] =
	{
		{ "BackPressed", VuTouch::SPECIAL_BACK_PRESSED },
		{ VUNULL }
	};
	addProperty(new VuStaticIntEnumProperty("Type", mType, sTypeChoices));

	// event handlers
	REG_EVENT_HANDLER(VuOnTouchSpecialEntity, OnUITick);
	REG_EVENT_HANDLER(VuOnTouchSpecialEntity, OnUITouchSpecial);

	// scripting
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, Trigger);
}

//*****************************************************************************
void VuOnTouchSpecialEntity::OnUITick(const VuParams &params)
{
	if ( mbPressed )
	{
		mpScriptComponent->getPlug("Trigger")->execute();
		mbPressed = false;
	}
}

//*****************************************************************************
void VuOnTouchSpecialEntity::OnUITouchSpecial(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	int special = accessor.getInt();

	if ( mType == special )
	{
		mbPressed = true;
	}
}


//*****************************************************************************
// PedestalControl
//*****************************************************************************
class VuPedestalControlEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuPedestalControlEntity();

	virtual void		onGameInitialize();
	virtual void		onGameRelease();

protected:
	// scripting
	VuRetVal			SetIdle(const VuParams &params) { mCurRotSpeed = mIdleRotSpeed; return VuRetVal(); }
	VuRetVal			SetFast(const VuParams &params) { mCurRotSpeed = mFastRotSpeed; return VuRetVal(); }

	// event handlers
	void				OnUITick(const VuParams &params);
	void				OnUITouch(const VuParams &params);

	void				drawLayout(bool bSelected);

	// components
	VuScriptComponent	*mpScriptComponent;

	// properties
	VuUIRectProperties		mTouchRect;
	VuUIAnchorProperties	mAnchor;
	float					mIdleRotSpeed;
	float					mControlRotSpeed;
	float					mFastRotSpeed;

	bool				mNewTouch;
	bool				mTouchDown;
	bool				mTouchUp;
	VuVector2			mTouchDownPos;
	VuVector2			mTouchPos;

	enum eState { IDLE, SPINNING };
	eState				mState;
	float				mTargetRot;
	float				mStartRot;
	float				mCurRotSpeed;
};

IMPLEMENT_RTTI(VuPedestalControlEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuPedestalControlEntity);


//*****************************************************************************
VuPedestalControlEntity::VuPedestalControlEntity():
	mIdleRotSpeed(VU_PIDIV4),
	mControlRotSpeed(VU_2PI),
	mFastRotSpeed(VU_PIDIV2),
	mNewTouch(false),
	mTouchDown(false),
	mTouchUp(false),
	mState(IDLE),
	mTargetRot(0),
	mStartRot(0),
	mCurRotSpeed(0)
{
	// components
	addComponent(new Vu2dLayoutComponent(this, &VuPedestalControlEntity::drawLayout));
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	// properties
	addProperty(new VuRectProperty("Touch Rect", mTouchRect));
	ADD_UI_ANCHOR_PROPERTIES(getProperties(), mAnchor, "");
	addProperty(new VuAngleProperty("Idle Rot Speed", mIdleRotSpeed));
	addProperty(new VuAngleProperty("Control Rot Speed", mControlRotSpeed));
	addProperty(new VuAngleProperty("Fast Rot Speed", mFastRotSpeed));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuPedestalControlEntity, SetIdle);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuPedestalControlEntity, SetFast);

	// event handlers
	REG_EVENT_HANDLER(VuPedestalControlEntity, OnUITick);
	REG_EVENT_HANDLER(VuPedestalControlEntity, OnUITouch);
}

//*****************************************************************************
void VuPedestalControlEntity::onGameInitialize()
{
	mCurRotSpeed = mIdleRotSpeed;

	VuEventManager::IF()->broadcast("OnPedestalControlBegin");
}

//*****************************************************************************
void VuPedestalControlEntity::onGameRelease()
{
	VuEventManager::IF()->broadcast("OnPedestalControlEnd");
}

//*****************************************************************************
void VuPedestalControlEntity::OnUITick(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	float fdt = accessor.getFloat();

	if ( mState == IDLE )
	{
		if ( mNewTouch )
		{
			VuUIDrawParams uiDrawParams;
			VuUIDrawUtil::getParams(this, uiDrawParams);

			VuRect touchRect = uiDrawParams.transform(mTouchRect);
			mAnchor.apply(touchRect, touchRect);

			if ( touchRect.contains(mTouchPos) )
			{
				mState = SPINNING;
				mStartRot = mTargetRot;
			}
		}

		mTargetRot += mCurRotSpeed*fdt;
	}
	else if ( mState == SPINNING )
	{
		if ( mTouchDown )
		{
			VuUIDrawParams uiDrawParams;
			VuUIDrawUtil::getParams(this, uiDrawParams);

			VuVector2 touchDelta = uiDrawParams.transformInv(mTouchPos) - uiDrawParams.transformInv(mTouchDownPos);

			mTargetRot = mStartRot + mControlRotSpeed*touchDelta.mX/1080.0f;
		}
		else
		{
			mState = IDLE;
		}
	}

	if ( mTouchUp )
	{
		mTouchDown = false;
		mTouchUp = false;
	}
	mNewTouch = false;

	VuParams outParams;
	outParams.addFloat(mTargetRot);
	VuEventManager::IF()->broadcast("OnPedestalControlUpdate", outParams);
}

//*****************************************************************************
void VuPedestalControlEntity::OnUITouch(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	int action = accessor.getInt();
	VuVector2 touch = accessor.getVector2();

	if ( action == VuUIInputUtil::TOUCH_DOWN )
	{
		if ( !mTouchDown )
		{
			mNewTouch = true;
			mTouchDown = true;
			mTouchDownPos = touch;
			mTouchPos = touch;
		}
	}
	else if ( action == VuUIInputUtil::TOUCH_UP )
	{
		mTouchUp = true;
	}
	else if ( action == VuUIInputUtil::TOUCH_MOVE )
	{
		mTouchPos = touch;
	}
}

//*****************************************************************************
void VuPedestalControlEntity::drawLayout(bool bSelected)
{
	if ( bSelected )
	{
		VuUIDrawParams uiDrawParams;
		VuUIDrawUtil::getParams(this, uiDrawParams);
		VuRect touchRect = uiDrawParams.transform(mTouchRect);
		VuGfxUtil::IF()->drawRectangleOutline2d(0.0f, VuColor(255,255,255), touchRect);
	}
}
