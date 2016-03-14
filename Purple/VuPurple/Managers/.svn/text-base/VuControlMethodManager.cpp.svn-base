//*****************************************************************************
//
//  Copyright (c) 2012-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  ControlMethodManager class
// 
//*****************************************************************************

#include "VuControlMethodManager.h"
#include "VuPurple/Managers/VuSettingsManager.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Managers/VuInputManager.h"
#include "VuEngine/HAL/GamePad/VuGamePad.h"


// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuControlMethodManager, VuControlMethodManager);


const char *sControlMethodNames[] =
{
	"GamePad",  // METHOD_GAMEPAD
	"Keyboard", // METHOD_KEYBOARD
	"Tilt",     // METHOD_TILT
	"TouchA",   // METHOD_TOUCH_A
	"TouchB",   // METHOD_TOUCH_B
};
VU_COMPILE_TIME_ASSERT(sizeof(sControlMethodNames)/sizeof(sControlMethodNames[0]) == VuControlMethodManager::NUM_METHODS);


//*****************************************************************************
VuControlMethodManager::VuControlMethodManager():
	mMethod(METHOD_KEYBOARD),
	mMessageBoxActive(false),
	mGamePadUsed(false),
	mRecentPadIndex(0)
{
	REG_EVENT_HANDLER(VuControlMethodManager, OnGamePadDisconnected);
}

//*****************************************************************************
VuControlMethodManager::~VuControlMethodManager()
{
}

//*****************************************************************************
bool VuControlMethodManager::init()
{
	VuTouch::IF()->addLowLevelCallback(this);
	VuKeyboard::IF()->addCallback(this, false);

	VuTickManager::IF()->registerHandler(this, &VuControlMethodManager::tickInput, "Input");

	VuInputManager::IF()->setConfig(VuInputManager::CONFIG_KEYBOARD);

	return true;
}

//*****************************************************************************
void VuControlMethodManager::release()
{
	VuTouch::IF()->removeLowLevelCallback(this);
	VuKeyboard::IF()->removeCallback(this);

	VuTickManager::IF()->unregisterHandlers(this);
}

//*****************************************************************************
const char *VuControlMethodManager::getMethodName()
{
	return sControlMethodNames[mMethod];
}

//*****************************************************************************
void VuControlMethodManager::OnGamePadDisconnected(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);

	if ( !mMessageBoxActive )
	{
		if ( isGamePad() )
		{
			VuMessageBoxParams messageBoxParams;
			messageBoxParams.mType = "SimpleA";
			messageBoxParams.mHeading = "GamePadDisconnected_Heading";
			messageBoxParams.mTextA = "Common_OK";
			messageBoxParams.mBody = "GamePadDisconnected_Body";

			VuMessageBox *pMessageBox = VuMessageBoxManager::IF()->create(messageBoxParams);
			pMessageBox->setCallback(this);
			pMessageBox->removeRef();

			mMessageBoxActive = true;

			VuEventManager::IF()->broadcast("OnPauseActionGame");
		}
	}
}

//*****************************************************************************
void VuControlMethodManager::onTouchDown(const VuVector2 &touch)
{
	// this assumes that tilt, touch_a, and touch_b map directly from settings manager to control method manager
	setMethod(eMethod(METHOD_TILT + VuSettingsManager::IF()->getTouchMethod()));
}

//*****************************************************************************
void VuControlMethodManager::onKeyDown(VUUINT32 key)
{
	if ( setMethod(METHOD_KEYBOARD) )
		VuInputManager::IF()->setConfig(VuInputManager::CONFIG_KEYBOARD);
}

//*****************************************************************************
void VuControlMethodManager::onMessageBoxClosed(VuMessageBox *pMessageBox)
{
	mMessageBoxActive = false;
}

//*****************************************************************************
void VuControlMethodManager::tickInput(float fdt)
{
	VUUINT32 anyGamePadInput = 0;

	for ( int padIndex = 0; padIndex < VuGamePad::MAX_NUM_PADS; padIndex++ )
	{
		const VuGamePad::VuController &controller = VuGamePad::IF()->getController(padIndex);
		if ( controller.mIsConnected )
		{
			VUUINT32 thisGamePadInput = controller.mButtons;

			int axisCount = VuGamePad::IF()->getAxisCount();
			for ( int axisIndex = 0; axisIndex < axisCount; axisIndex++ )
				thisGamePadInput |= VuAbs(controller.mAxes[axisIndex]) > 0.5f;

			if ( thisGamePadInput )
				mRecentPadIndex = padIndex;

			anyGamePadInput |= thisGamePadInput;
		}
	}

	if ( anyGamePadInput )
	{
		if ( setMethod(METHOD_GAMEPAD) )
		{
			VuInputManager::IF()->setConfig(VuInputManager::CONFIG_GAMEPAD);
			mGamePadUsed = true;
		}
	}
}

//*****************************************************************************
bool VuControlMethodManager::setMethod(eMethod method)
{
	if ( mMethod != method )
	{
		mMethod = method;
		VuEventManager::IF()->broadcastDelayed(0.0f, false, "OnControlMethodChanged");

		return true;
	}

	return false;
}