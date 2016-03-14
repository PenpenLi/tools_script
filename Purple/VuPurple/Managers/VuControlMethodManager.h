//*****************************************************************************
//
//  Copyright (c) 2012-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  ControlMethodManager class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/VuSystemComponent.h"
#include "VuEngine/Events/VuEventMap.h"
#include "VuEngine/HAL/Touch/VuTouch.h"
#include "VuEngine/HAL/Keyboard/VuKeyboard.h"
#include "VuEngine/Managers/VuMessageBoxManager.h"


class VuControlMethodManager : public VuSystemComponent, VuTouch::Callback, VuKeyboard::Callback, VuMessageBox::Callback
{
	DECLARE_SYSTEM_COMPONENT(VuControlMethodManager)
	DECLARE_EVENT_MAP

protected:
	// called by game
	friend class VuPurple;
	virtual bool init();
	virtual void release();

public:
	VuControlMethodManager();
	~VuControlMethodManager();

	bool			isGamePad()  { return mMethod == METHOD_GAMEPAD; }
	bool			isKeyboard() { return mMethod == METHOD_KEYBOARD; }
	bool			isTilt()     { return mMethod == METHOD_TILT; }
	bool			isTouchA()   { return mMethod == METHOD_TOUCH_A; }
	bool			isTouchB()   { return mMethod == METHOD_TOUCH_B; }

	bool			isGamePadOrKeyboard() { return (mMethod == METHOD_GAMEPAD) | (mMethod == METHOD_KEYBOARD); }

	bool			wasGamePadUsed() { return mGamePadUsed; }
	int				recentPadIndex() { return mRecentPadIndex; }

	enum eMethod { METHOD_GAMEPAD, METHOD_KEYBOARD, METHOD_TILT, METHOD_TOUCH_A, METHOD_TOUCH_B, NUM_METHODS };

	eMethod			getMethod() { return mMethod; }
	const char		*getMethodName();

private:
	// event handlers
	void			OnGamePadDisconnected(const VuParams &params);

	// VuTouch::Callback
	virtual void	onTouchDown(const VuVector2 &touch);

	// VuKeyboard::Callback
	virtual void	onKeyDown(VUUINT32 key);

	// VuMessageBox::Callback
	virtual void	onMessageBoxClosed(VuMessageBox *pMessageBox);

	void			tickInput(float fdt);
	bool			setMethod(eMethod method);

	eMethod			mMethod;
	bool			mMessageBoxActive;
	bool			mGamePadUsed;
	int				mRecentPadIndex;
};
