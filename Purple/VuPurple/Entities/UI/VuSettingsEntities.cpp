//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  setting entities
// 
//*****************************************************************************

#include <ctype.h>
#include "VuGameTextEntity.h"
#include "VuPurple/Managers/VuSettingsManager.h"
#include "VuPurple/Entities/UI/VuCheckBoxEntity.h"
#include "VuPurple/Entities/UI/VuSliderEntity.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Properties/VuBasicProperty.h"
#include "VuEngine/DB/VuStringDB.h"
#include "VuEngine/HAL/Keyboard/VuKeyboard.h"
#include "VuEngine/Managers/VuProfileManager.h"
#include "VuEngine/Managers/VuInputManager.h"
#include "VuEngine/Managers/VuConfigManager.h"


//*****************************************************************************
// Set Default
//*****************************************************************************
class VuSetDefaultSettingsEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuSetDefaultSettingsEntity();

private:
	VuRetVal	Trigger(const VuParams &params);

	// properties
	bool		mAudio;
	bool		mGamePadControls;
	bool		mKeyboardControls;
	bool		mTouchTiltControls;
	bool		mGraphics;
	bool		mAdvancedGraphics;
};

IMPLEMENT_RTTI(VuSetDefaultSettingsEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuSetDefaultSettingsEntity);

//*****************************************************************************
VuSetDefaultSettingsEntity::VuSetDefaultSettingsEntity():
	mAudio(false),
	mGamePadControls(false),
	mKeyboardControls(false),
	mTouchTiltControls(false),
	mGraphics(false),
	mAdvancedGraphics(false)
{
	// components
	addComponent(new VuScriptComponent(this));

	// properties
	addProperty(new VuBoolProperty("Audio", mAudio));
	addProperty(new VuBoolProperty("GamePadControls", mGamePadControls));
	addProperty(new VuBoolProperty("KeyboardControls", mKeyboardControls));
	addProperty(new VuBoolProperty("TouchTiltControls", mTouchTiltControls));
	addProperty(new VuBoolProperty("Graphics", mGraphics));
	addProperty(new VuBoolProperty("AdvancedGraphics", mAdvancedGraphics));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(getComponent<VuScriptComponent>(), VuSetDefaultSettingsEntity, Trigger);
}

//*****************************************************************************
VuRetVal VuSetDefaultSettingsEntity::Trigger(const VuParams &params)
{
	if ( mAudio )
	{
		VuSettingsManager::IF()->setDefaultAudio();
	}
	if ( mGamePadControls )
	{
		VuSettingsManager::IF()->setDefaultGamePadControls();
		VuInputManager::IF()->setDefaultMapping(0, VuInputManager::CONFIG_GAMEPAD);
	}
	if ( mKeyboardControls )
	{
		VuSettingsManager::IF()->setDefaultKeyboardControls();
		VuInputManager::IF()->setDefaultMapping(0, VuInputManager::CONFIG_KEYBOARD);
	}
	if ( mTouchTiltControls )
	{
		VuSettingsManager::IF()->setDefaultTouchControls();
	}
	if ( mGraphics )
	{
		VuSettingsManager::IF()->setDefaultGraphics();
	}
	if ( mAdvancedGraphics )
	{
		VuSettingsManager::IF()->setDefaultAdvancedGraphics();
	}

	return VuRetVal();
}


//*****************************************************************************
// Save
//*****************************************************************************
class VuSaveSettingsEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuSaveSettingsEntity();

private:
	VuRetVal	Trigger(const VuParams &params);
};

IMPLEMENT_RTTI(VuSaveSettingsEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuSaveSettingsEntity);

//*****************************************************************************
VuSaveSettingsEntity::VuSaveSettingsEntity()
{
	// components
	addComponent(new VuScriptComponent(this));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(getComponent<VuScriptComponent>(), VuSaveSettingsEntity, Trigger);
}

//*****************************************************************************
VuRetVal VuSaveSettingsEntity::Trigger(const VuParams &params)
{
	VuProfileManager::IF()->save();

	return VuRetVal();
}


//*****************************************************************************
// Steering Sensitivity
//*****************************************************************************
class VuSteeringSensitivitySettingsEntity : public VuSliderEntity
{
	DECLARE_RTTI

private:
	virtual float	getValue()				{ return VuSettingsManager::IF() ? VuSettingsManager::IF()->getSteeringSensitivity() : 0.5f; }
	virtual void	setValue(float value)	{ if ( VuSettingsManager::IF() ) VuSettingsManager::IF()->setSteeringSensitivity(value); }
	virtual bool	isEnabled()				{ return VuSettingsManager::IF() ? VuSettingsManager::IF()->getTouchMethod() == VuSettingsManager::TILT : true; }
};

IMPLEMENT_RTTI(VuSteeringSensitivitySettingsEntity, VuSliderEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuSteeringSensitivitySettingsEntity);


//*****************************************************************************
// Camera Tilt
//*****************************************************************************
class VuCameraTiltSettingsEntity : public VuCheckBoxBaseEntity
{
	DECLARE_RTTI

private:
	virtual bool	isChecked()					{ return VuSettingsManager::IF() ? VuSettingsManager::IF()->getCameraTilt() : false; }
	virtual void	setChecked(bool checked)	{ if ( VuSettingsManager::IF() ) VuSettingsManager::IF()->setCameraTilt(checked); }
};

IMPLEMENT_RTTI(VuCameraTiltSettingsEntity, VuCheckBoxBaseEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuCameraTiltSettingsEntity);


//*****************************************************************************
// Auto Throttle Game Pad
//*****************************************************************************
class VuAutoThrottleGamePadSettingsEntity : public VuCheckBoxBaseEntity
{
	DECLARE_RTTI

private:
	virtual bool	isChecked()					{ return VuSettingsManager::IF() ? VuSettingsManager::IF()->getAutoThrottleGamePad() : false; }
	virtual void	setChecked(bool checked)	{ if ( VuSettingsManager::IF() ) VuSettingsManager::IF()->setAutoThrottleGamePad(checked); }
};

IMPLEMENT_RTTI(VuAutoThrottleGamePadSettingsEntity, VuCheckBoxBaseEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuAutoThrottleGamePadSettingsEntity);


//*****************************************************************************
// Auto Throttle Keyboard
//*****************************************************************************
class VuAutoThrottleKeyboardSettingsEntity : public VuCheckBoxBaseEntity
{
	DECLARE_RTTI

private:
	virtual bool	isChecked()					{ return VuSettingsManager::IF() ? VuSettingsManager::IF()->getAutoThrottleKeyboard() : false; }
	virtual void	setChecked(bool checked)	{ if ( VuSettingsManager::IF() ) VuSettingsManager::IF()->setAutoThrottleKeyboard(checked); }
};

IMPLEMENT_RTTI(VuAutoThrottleKeyboardSettingsEntity, VuCheckBoxBaseEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuAutoThrottleKeyboardSettingsEntity);


//*****************************************************************************
// Auto Throttle Game Pad
//*****************************************************************************
class VuSetAutoThrottleGamePadEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuSetAutoThrottleGamePadEntity()
	{
		addComponent(mpScriptComponent = new VuScriptComponent(this));
		ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuSetAutoThrottleGamePadEntity, Enable);
		ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuSetAutoThrottleGamePadEntity, Disable);
		ADD_SCRIPT_INPUT(mpScriptComponent, VuSetAutoThrottleGamePadEntity, IsEnabled, VuRetVal::Bool, VuParamDecl());
	}

private:
	VuRetVal			Enable(const VuParams &params) { VuSettingsManager::IF()->setAutoThrottleGamePad(true); return VuRetVal(); }
	VuRetVal			Disable(const VuParams &params) { VuSettingsManager::IF()->setAutoThrottleGamePad(false); return VuRetVal(); }
	VuRetVal			IsEnabled(const VuParams &params) { return VuRetVal(VuSettingsManager::IF()->getAutoThrottleGamePad()); }

	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuSetAutoThrottleGamePadEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuSetAutoThrottleGamePadEntity);


//*****************************************************************************
// Effect Volume
//*****************************************************************************
class VuEffectVolumeSettingsEntity : public VuSliderEntity
{
	DECLARE_RTTI

private:
	virtual float	getValue()				{ return VuSettingsManager::IF() ? VuSettingsManager::IF()->getEffectVolume() : 0.5f; }
	virtual void	setValue(float value)	{ if ( VuSettingsManager::IF() ) VuSettingsManager::IF()->setEffectVolume(value); }
};

IMPLEMENT_RTTI(VuEffectVolumeSettingsEntity, VuSliderEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuEffectVolumeSettingsEntity);


//*****************************************************************************
// Music Volume
//*****************************************************************************
class VuMusicVolumeSettingsEntity : public VuSliderEntity
{
	DECLARE_RTTI

private:
	virtual float	getValue()				{ return VuSettingsManager::IF() ? VuSettingsManager::IF()->getMusicVolume() : 0.5f; }
	virtual void	setValue(float value)	{ if ( VuSettingsManager::IF() ) VuSettingsManager::IF()->setMusicVolume(value); }
};

IMPLEMENT_RTTI(VuMusicVolumeSettingsEntity, VuSliderEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuMusicVolumeSettingsEntity);


//*****************************************************************************
// Display Scale (0.5 -> 1.0)
//*****************************************************************************
class VuDisplayScaleSettingsEntity : public VuSliderEntity
{
	DECLARE_RTTI

private:
	virtual float	getValue()				{ return (VuConfigManager::IF()->getFloat("Composer/DisplayScale")->mValue - 0.5f)/0.5f; }
	virtual void	setValue(float value)	{ VuConfigManager::IF()->getFloat("Composer/DisplayScale")->mValue = 0.5f + 0.5f*value; }
};

IMPLEMENT_RTTI(VuDisplayScaleSettingsEntity, VuSliderEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuDisplayScaleSettingsEntity);


//*****************************************************************************
// Shadow Detail (0.5 -> 1.0)
//*****************************************************************************
class VuShadowDetailSettingsEntity : public VuSliderEntity
{
	DECLARE_RTTI

private:
	virtual float	getValue()				{ return (VuConfigManager::IF()->getFloat("Composer/ShadowDetail")->mValue - 0.5f)/0.5f; }
	virtual void	setValue(float value)	{ VuConfigManager::IF()->getFloat("Composer/ShadowDetail")->mValue = 0.5f + 0.5f*value; }
	virtual bool	isEnabled()				{ return VuConfigManager::IF()->getInt("Gfx/ShaderLOD")->mValue == 0; }
};

IMPLEMENT_RTTI(VuShadowDetailSettingsEntity, VuSliderEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuShadowDetailSettingsEntity);


//*****************************************************************************
// Water Normal Map
//*****************************************************************************
class VuWaterNormalMapSettingsEntity : public VuCheckBoxBaseEntity
{
	DECLARE_RTTI

private:
	virtual bool	isChecked()					{ return VuConfigManager::IF()->getBool("Water/NormalMap")->mValue; }
	virtual void	setChecked(bool checked)	{ VuConfigManager::IF()->getBool("Water/NormalMap")->mValue = checked; }
};

IMPLEMENT_RTTI(VuWaterNormalMapSettingsEntity, VuCheckBoxBaseEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuWaterNormalMapSettingsEntity);


//*****************************************************************************
// Water Detail
//*****************************************************************************
class VuWaterHighDetailSettingsEntity : public VuCheckBoxBaseEntity
{
	DECLARE_RTTI

private:
	virtual bool	isChecked()					{ return VuConfigManager::IF()->getFloat("Water/Detail")->mValue > 0.75f; }
	virtual void	setChecked(bool checked)	{ VuConfigManager::IF()->getFloat("Water/Detail")->mValue = checked ? 1.0f : 0.5f; }
};

IMPLEMENT_RTTI(VuWaterHighDetailSettingsEntity, VuCheckBoxBaseEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuWaterHighDetailSettingsEntity);


//*****************************************************************************
// Water Reflection
//*****************************************************************************
class VuWaterReflectionSettingsEntity : public VuCheckBoxBaseEntity
{
	DECLARE_RTTI

private:
	virtual bool	isChecked()					{ return VuConfigManager::IF()->getBool("Water/Reflection")->mValue; }
	virtual void	setChecked(bool checked)	{ VuConfigManager::IF()->getBool("Water/Reflection")->mValue = checked; }
};

IMPLEMENT_RTTI(VuWaterReflectionSettingsEntity, VuCheckBoxBaseEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuWaterReflectionSettingsEntity);


//*****************************************************************************
// Water Wakes
//*****************************************************************************
class VuWaterWakesSettingsEntity : public VuCheckBoxBaseEntity
{
	DECLARE_RTTI

private:
	virtual bool	isChecked()					{ return VuConfigManager::IF()->getBool("Water/Wakes")->mValue; }
	virtual void	setChecked(bool checked)	{ VuConfigManager::IF()->getBool("Water/Wakes")->mValue = checked; }
	virtual bool	isEnabled()					{ return VuConfigManager::IF()->getBool("Water/Wakes")->mValue == false; }
};

IMPLEMENT_RTTI(VuWaterWakesSettingsEntity, VuCheckBoxBaseEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuWaterWakesSettingsEntity);


//*****************************************************************************
// Lens Water Effect
//*****************************************************************************
class VuLensWaterEffectSettingsEntity : public VuCheckBoxBaseEntity
{
	DECLARE_RTTI

private:
	virtual bool	isChecked()					{ return VuConfigManager::IF()->getBool("Effects/LensWater")->mValue; }
	virtual void	setChecked(bool checked)	{ VuConfigManager::IF()->getBool("Effects/LensWater")->mValue = checked; }
};

IMPLEMENT_RTTI(VuLensWaterEffectSettingsEntity, VuCheckBoxBaseEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuLensWaterEffectSettingsEntity);


//*****************************************************************************
// Radial Blur Effect
//*****************************************************************************
class VuRadialBlurEffectSettingsEntity : public VuCheckBoxBaseEntity
{
	DECLARE_RTTI

private:
	virtual bool	isChecked()					{ return VuConfigManager::IF()->getBool("Effects/RadialBlur")->mValue; }
	virtual void	setChecked(bool checked)	{ VuConfigManager::IF()->getBool("Effects/RadialBlur")->mValue = checked; }
};

IMPLEMENT_RTTI(VuRadialBlurEffectSettingsEntity, VuCheckBoxBaseEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuRadialBlurEffectSettingsEntity);


//*****************************************************************************
// Color Correction Effect
//*****************************************************************************
class VuColorCorrectionEffectSettingsEntity : public VuCheckBoxBaseEntity
{
	DECLARE_RTTI

private:
	virtual bool	isChecked()					{ return VuConfigManager::IF()->getBool("Effects/ColorCorrection")->mValue; }
	virtual void	setChecked(bool checked)	{ VuConfigManager::IF()->getBool("Effects/ColorCorrection")->mValue = checked; }
};

IMPLEMENT_RTTI(VuColorCorrectionEffectSettingsEntity, VuCheckBoxBaseEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuColorCorrectionEffectSettingsEntity);


//*****************************************************************************
// High Model LOD
//*****************************************************************************
class VuHighModelLODSettingsEntity : public VuCheckBoxBaseEntity
{
	DECLARE_RTTI

private:
	virtual bool	isChecked()					{ return !VuConfigManager::IF()->getBool("Gfx/LowModelLOD")->mValue; }
	virtual void	setChecked(bool checked)	{ VuConfigManager::IF()->getBool("Gfx/LowModelLOD")->mValue = !checked; }
};

IMPLEMENT_RTTI(VuHighModelLODSettingsEntity, VuCheckBoxBaseEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuHighModelLODSettingsEntity);


//*****************************************************************************
// High Texture LOD
//*****************************************************************************
class VuHighTextureLODSettingsEntity : public VuCheckBoxBaseEntity
{
	DECLARE_RTTI

private:
	virtual bool	isChecked()					{ return !VuConfigManager::IF()->getBool("Gfx/LowTextureLOD")->mValue; }
	virtual void	setChecked(bool checked)	{ VuConfigManager::IF()->getBool("Gfx/LowTextureLOD")->mValue = !checked; }
};

IMPLEMENT_RTTI(VuHighTextureLODSettingsEntity, VuCheckBoxBaseEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuHighTextureLODSettingsEntity);


//*****************************************************************************
// Shader LOD
//*****************************************************************************
class VuShaderLODSettingsEntity : public VuGameTextBaseEntity
{
	DECLARE_RTTI

public:
	VuShaderLODSettingsEntity();

	virtual const char	*getText();

private:
	VuRetVal		Increase(const VuParams &params);
	VuRetVal		Decrease(const VuParams &params);

	// properties
	std::string		mStringIDs[3];
};

IMPLEMENT_RTTI(VuShaderLODSettingsEntity, VuGameTextBaseEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuShaderLODSettingsEntity);


//*****************************************************************************
VuShaderLODSettingsEntity::VuShaderLODSettingsEntity()
{
	// properties
	addProperty(new VuStringProperty("LOD 0 String ID", mStringIDs[0]));
	addProperty(new VuStringProperty("LOD 1 String ID", mStringIDs[1]));
	addProperty(new VuStringProperty("LOD 2 String ID", mStringIDs[2]));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuShaderLODSettingsEntity, Increase);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuShaderLODSettingsEntity, Decrease);
}

//*****************************************************************************
const char *VuShaderLODSettingsEntity::getText()
{
	int value = VuConfigManager::IF()->getInt("Gfx/ShaderLOD")->mValue;
	value = VuClamp(value, 0, 2);

	return VuStringDB::IF()->getString(mStringIDs[value]).c_str();
}

//*****************************************************************************
VuRetVal VuShaderLODSettingsEntity::Increase(const VuParams &params)
{
	VuConfigManager::Int *pInt = VuConfigManager::IF()->getInt("Gfx/ShaderLOD");
	pInt->mValue = VuMin(pInt->mValue + pInt->mStep, pInt->mMax);

	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuShaderLODSettingsEntity::Decrease(const VuParams &params)
{
	VuConfigManager::Int *pInt = VuConfigManager::IF()->getInt("Gfx/ShaderLOD");
	pInt->mValue = VuMax(pInt->mValue - pInt->mStep, pInt->mMin);

	return VuRetVal();
}


//*****************************************************************************
// 60 hz
//*****************************************************************************
class Vu60HzSettingsEntity : public VuCheckBoxBaseEntity
{
	DECLARE_RTTI

private:
	virtual bool	isChecked()					{ return VuSettingsManager::IF() ? VuConfigManager::IF()->getInt("Gfx/FlipInterval")->mValue == 1 : false; }
	virtual void	setChecked(bool checked)	{ if ( VuSettingsManager::IF() ) VuConfigManager::IF()->getInt("Gfx/FlipInterval")->mValue = checked ? 1 : 2; }
};

IMPLEMENT_RTTI(Vu60HzSettingsEntity, VuCheckBoxBaseEntity);
IMPLEMENT_ENTITY_REGISTRATION(Vu60HzSettingsEntity);


//*****************************************************************************
// Scene Complexity
//*****************************************************************************
class VuSceneComplexitySettingsEntity : public VuGameTextBaseEntity
{
	DECLARE_RTTI

public:
	VuSceneComplexitySettingsEntity();

	virtual const char	*getText();

private:
	VuRetVal		Increase(const VuParams &params);
	VuRetVal		Decrease(const VuParams &params);

	// properties
	std::string		mStringIDs[3];
};

IMPLEMENT_RTTI(VuSceneComplexitySettingsEntity, VuGameTextBaseEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuSceneComplexitySettingsEntity);


//*****************************************************************************
VuSceneComplexitySettingsEntity::VuSceneComplexitySettingsEntity()
{
	// properties
	addProperty(new VuStringProperty("Level 0 String ID", mStringIDs[0]));
	addProperty(new VuStringProperty("Level 1 String ID", mStringIDs[1]));
	addProperty(new VuStringProperty("Level 2 String ID", mStringIDs[2]));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuSceneComplexitySettingsEntity, Increase);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuSceneComplexitySettingsEntity, Decrease);
}

//*****************************************************************************
const char *VuSceneComplexitySettingsEntity::getText()
{
	int value = VuConfigManager::IF()->getInt("Scene/Complexity")->mValue;
	value = VuClamp(value, 0, 2);

	return VuStringDB::IF()->getString(mStringIDs[value]).c_str();
}

//*****************************************************************************
VuRetVal VuSceneComplexitySettingsEntity::Increase(const VuParams &params)
{
	VuConfigManager::Int *pInt = VuConfigManager::IF()->getInt("Scene/Complexity");
	pInt->mValue = VuMin(pInt->mValue + pInt->mStep, pInt->mMax);

	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuSceneComplexitySettingsEntity::Decrease(const VuParams &params)
{
	VuConfigManager::Int *pInt = VuConfigManager::IF()->getInt("Scene/Complexity");
	pInt->mValue = VuMax(pInt->mValue - pInt->mStep, pInt->mMin);

	return VuRetVal();
}


//*****************************************************************************
// Touch Method
//*****************************************************************************
class VuTouchMethodSettingsEntity : public VuGameTextBaseEntity
{
	DECLARE_RTTI

public:
	VuTouchMethodSettingsEntity();

	virtual const char	*getText();

private:
	VuRetVal		Next(const VuParams &params);
	VuRetVal		Prev(const VuParams &params);
	VuRetVal		SetTilt(const VuParams &params) { VuSettingsManager::IF()->setTouchMethod(VuSettingsManager::TILT); return VuRetVal(); }
	VuRetVal		SetTouchA(const VuParams &params) { VuSettingsManager::IF()->setTouchMethod(VuSettingsManager::TOUCH_A); return VuRetVal(); }
	VuRetVal		SetTouchB(const VuParams &params) { VuSettingsManager::IF()->setTouchMethod(VuSettingsManager::TOUCH_B); return VuRetVal(); }

	// properties
	std::string		mStringIDs[3];
};

IMPLEMENT_RTTI(VuTouchMethodSettingsEntity, VuGameTextBaseEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuTouchMethodSettingsEntity);


//*****************************************************************************
VuTouchMethodSettingsEntity::VuTouchMethodSettingsEntity()
{
	// properties
	addProperty(new VuStringProperty("Tilt String ID", mStringIDs[0]));
	addProperty(new VuStringProperty("Touch A String ID", mStringIDs[1]));
	addProperty(new VuStringProperty("Touch B String ID", mStringIDs[2]));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuTouchMethodSettingsEntity, Next);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuTouchMethodSettingsEntity, Prev);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuTouchMethodSettingsEntity, SetTilt);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuTouchMethodSettingsEntity, SetTouchA);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuTouchMethodSettingsEntity, SetTouchB);
}

//*****************************************************************************
const char *VuTouchMethodSettingsEntity::getText()
{
	int value = VuSettingsManager::IF() ? VuSettingsManager::IF()->getTouchMethod() : 0;
	value = VuClamp(value, 0, 2);

	return VuStringDB::IF()->getString(mStringIDs[value]).c_str();
}

//*****************************************************************************
VuRetVal VuTouchMethodSettingsEntity::Next(const VuParams &params)
{
	int value = VuSettingsManager::IF()->getTouchMethod();
	value = VuMin(value + 1, VuSettingsManager::TOUCH_METHOD_COUNT - 1);
	VuSettingsManager::IF()->setTouchMethod((VuSettingsManager::eTouchMethod)value);

	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuTouchMethodSettingsEntity::Prev(const VuParams &params)
{
	int value = VuSettingsManager::IF()->getTouchMethod();
	value = VuMax(value - 1, 0);
	VuSettingsManager::IF()->setTouchMethod((VuSettingsManager::eTouchMethod)value);

	return VuRetVal();
}


//*****************************************************************************
// Safe Zone (0.9 -> 1.0)
//*****************************************************************************
class VuSafeZoneSettingsEntity : public VuSliderEntity
{
	DECLARE_RTTI

private:
	virtual float	getValue()				{ return (VuConfigManager::IF()->getFloat("Gfx/SafeZone")->mValue - 0.9f)/0.1f; }
	virtual void	setValue(float value)	{ VuConfigManager::IF()->getFloat("Gfx/SafeZone")->mValue = 0.9f + 0.1f*value; }
};

IMPLEMENT_RTTI(VuSafeZoneSettingsEntity, VuSliderEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuSafeZoneSettingsEntity);
