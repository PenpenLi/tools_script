//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  SettingsManager class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/VuSystemComponent.h"
#include "VuEngine/Events/VuEventMap.h"

class VuJsonContainer;


class VuSettingsManager : public VuSystemComponent
{
	DECLARE_SYSTEM_COMPONENT(VuSettingsManager)
	DECLARE_EVENT_MAP

protected:
	// called by game
	friend class VuPurple;
	virtual bool init();
	virtual void postInit();

public:
	enum eTouchMethod { TILT, TOUCH_A, TOUCH_B, TOUCH_METHOD_COUNT };

	// defaults
	void				setDefaultAudio();
	void				setDefaultGamePadControls();
	void				setDefaultKeyboardControls();
	void				setDefaultTouchControls();
	void				setDefaultGraphics();
	void				setDefaultAdvancedGraphics();

	// controls
	void				setAutoThrottleGamePad(bool value)	{ mAutoThrottleGamePad = value; }
	bool				getAutoThrottleGamePad()			{ return mAutoThrottleGamePad; }
	void				setAutoThrottleKeyboard(bool value)	{ mAutoThrottleKeyboard = value; }
	bool				getAutoThrottleKeyboard()			{ return mAutoThrottleKeyboard; }
	void				setTouchMethod(const char *touchMethod);
	void				setTouchMethod(eTouchMethod touchMethod) { mTouchMethod = touchMethod; }
	eTouchMethod		getTouchMethod();
	void				setSteeringSensitivity(float value)			{ mSteeringSensitivity = value; }
	float				getSteeringSensitivity()					{ return mSteeringSensitivity; }
	void				setCameraTilt(bool value)					{ mCameraTilt = value; }
	bool				getCameraTilt()								{ return mCameraTilt; }

	// audio
	void				setEffectVolume(float value);
	float				getEffectVolume()				{ return mEffectVolume; }
	void				setMusicVolume(float value);
	float				getMusicVolume()				{ return mMusicVolume; }


private:
	// event handlers
	void			OnSaveProfile(const VuParams &params);

	void			setCategoryVolume(const char *name, float vol) const;

	void			loadConfigBool(const VuJsonContainer &data, const char *name);
	void			loadConfigFloat(const VuJsonContainer &data, const char *name);
	void			loadConfigInt(const VuJsonContainer &data, const char *name);

	void			saveConfigBool(VuJsonContainer &data, const char *name);
	void			saveConfigFloat(VuJsonContainer &data, const char *name);
	void			saveConfigInt(VuJsonContainer &data, const char *name);

	void			setDefaultConfigBool(const char *name);
	void			setDefaultConfigFloat(const char *name);
	void			setDefaultConfigInt(const char *name);

	const VuJsonContainer	&getDefaultSettings();


	bool			mAutoThrottleGamePad;
	bool			mAutoThrottleKeyboard;
	eTouchMethod	mTouchMethod;
	float			mSteeringSensitivity;
	bool			mCameraTilt;
	float			mEffectVolume;
	float			mMusicVolume;
};