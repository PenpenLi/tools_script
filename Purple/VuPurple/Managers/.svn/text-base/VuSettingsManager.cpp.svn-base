//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  SettingsManager class
// 
//*****************************************************************************

#include "VuSettingsManager.h"
#include "VuPurple/Gfx/GfxComposer/VuPurpleGfxComposer.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Managers/VuProfileManager.h"
#include "VuEngine/Managers/VuConfigManager.h"
#include "VuEngine/Assets/VuAssetFactory.h"
#include "VuEngine/HAL/Audio/VuAudio.h"
#include "VuEngine/Dev/VuDevConfig.h"

#ifdef VUANDROID
#include "VuEngine/HAL/Audio\Android\VuAndroidAudio.h"
#endif


// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuSettingsManager, VuSettingsManager);

static const char *sTouchMethodLookup[] =
{
	"Tilt",   // TILT
	"TouchA", // TOUCH_A
	"TouchB", // TOUCH_B,
};
VU_COMPILE_TIME_ASSERT(sizeof(sTouchMethodLookup)/sizeof(sTouchMethodLookup[0]) == VuSettingsManager::TOUCH_METHOD_COUNT);


//*****************************************************************************
bool VuSettingsManager::init()
{
	// event handlers
	REG_EVENT_HANDLER(VuSettingsManager, OnSaveProfile);

	return true;
}

//*****************************************************************************
void VuSettingsManager::postInit()
{
	std::string strValue;
	bool bValue;
	float fValue;

	setDefaultAudio();
	setDefaultGamePadControls();
	setDefaultKeyboardControls();
	setDefaultTouchControls();
	setDefaultGraphics();

	// load settings
	const VuJsonContainer &settingsData = VuProfileManager::IF()->dataRead()["Settings"];

	// controls
	{
		const VuJsonContainer &controls = settingsData["Controls"];

		if ( controls["AutoThrottleGamePad"].getValue(bValue) )
			setAutoThrottleGamePad(bValue);

		if ( controls["AutoThrottleKeyboard"].getValue(bValue) )
			setAutoThrottleKeyboard(bValue);

		if ( controls["TouchMethod"].getValue(strValue) )
			setTouchMethod(strValue.c_str());

		if ( controls["SteeringSensitivity"].getValue(fValue) )
			setSteeringSensitivity(fValue);

		if ( controls["CameraTilt"].getValue(bValue) )
			setCameraTilt(bValue);
	}

	// audio
	{
		const VuJsonContainer &audio = settingsData["Audio"];

		if ( audio["EffectVolume"].getValue(fValue) )
			setEffectVolume(fValue);

		if ( audio["MusicVolume"].getValue(fValue) )
			setMusicVolume(fValue);
	}

	// graphics
	if ( !VuDevConfig::IF()->hasParam("DeviceType") && VuConfigManager::IF()->stackSize() == 0 )
	{
		const VuJsonContainer &graphics = settingsData["Graphics"];

		loadConfigFloat(graphics, "Composer/DisplayScale");
		loadConfigFloat(graphics, "Composer/ShadowDetail");
		loadConfigBool(graphics, "Water/NormalMap");
		loadConfigFloat(graphics, "Water/Detail");
		loadConfigBool(graphics, "Water/Reflection");
		loadConfigBool(graphics, "Water/Wakes");
		loadConfigBool(graphics, "Effects/LensWater");
		loadConfigBool(graphics, "Effects/RadialBlur");
		loadConfigBool(graphics, "Effects/ColorCorrection");
		loadConfigBool(graphics, "Gfx/LowModelLOD");
		loadConfigBool(graphics, "Gfx/LowTextureLOD");
		loadConfigInt(graphics, "Gfx/ShaderLOD");
		loadConfigInt(graphics, "Gfx/FlipInterval");
		loadConfigInt(graphics, "Scene/Complexity");
		loadConfigFloat(graphics, "Gfx/SafeZone");
	}


#ifdef VUANDROID
	int nState = VuAndroidAudio::GetGameBaseAudioState();
	if (nState == 1)
	{
		setDefaultAudio();
	}
	else if (nState == 2)
	{
		setEffectVolume(0.0);
		setMusicVolume(0.0);
	}

#endif


	


}

//*****************************************************************************
void VuSettingsManager::setDefaultAudio()
{
	const VuJsonContainer &defaults = getDefaultSettings();

	setEffectVolume(defaults["EffectVolume"].asFloat());
	setMusicVolume(defaults["MusicVolume"].asFloat());
}

//*****************************************************************************
void VuSettingsManager::setDefaultGamePadControls()
{
	const VuJsonContainer &defaults = getDefaultSettings();

	setAutoThrottleGamePad(defaults["AutoThrottleGamePad"].asBool());
}

//*****************************************************************************
void VuSettingsManager::setDefaultKeyboardControls()
{
	const VuJsonContainer &defaults = getDefaultSettings();

	setAutoThrottleKeyboard(defaults["AutoThrottleKeyboard"].asBool());
}

//*****************************************************************************
void VuSettingsManager::setDefaultTouchControls()
{
	const VuJsonContainer &defaults = getDefaultSettings();

	setTouchMethod(defaults["TouchMethod"].asCString());
	setSteeringSensitivity(defaults["SteeringSensitivity"].asFloat());
	setCameraTilt(defaults["CameraTilt"].asBool());
}

//*****************************************************************************
void VuSettingsManager::setDefaultGraphics()
{
	setDefaultConfigFloat("Composer/DisplayScale");
}

//*****************************************************************************
void VuSettingsManager::setDefaultAdvancedGraphics()
{
	setDefaultConfigFloat("Composer/ShadowDetail");
	setDefaultConfigBool("Water/NormalMap");
	setDefaultConfigFloat("Water/Detail");
	setDefaultConfigBool("Water/Reflection");
	setDefaultConfigBool("Water/Wakes");
	setDefaultConfigBool("Effects/LensWater");
	setDefaultConfigBool("Effects/RadialBlur");
	setDefaultConfigBool("Effects/ColorCorrection");
	setDefaultConfigBool("Gfx/LowModelLOD");
	setDefaultConfigBool("Gfx/LowTextureLOD");
	setDefaultConfigInt("Gfx/ShaderLOD");
	setDefaultConfigInt("Gfx/FlipInterval");
	setDefaultConfigInt("Scene/Complexity");
}

//*****************************************************************************
void VuSettingsManager::setTouchMethod(const char *touchMethod)
{
	for ( int i = 0; i < TOUCH_METHOD_COUNT; i++ )
	{
		if ( strcmp(touchMethod, sTouchMethodLookup[i]) == 0 )
		{
			setTouchMethod(static_cast<eTouchMethod>(i));
			return;
		}
	}

	setTouchMethod(TILT);
}

//*****************************************************************************
VuSettingsManager::eTouchMethod VuSettingsManager::getTouchMethod()
{
	if ( !VuSys::IF()->hasTouch() )
		return TILT;

	if ( !VuSys::IF()->hasAccel() && mTouchMethod == TILT )
		return TOUCH_A;

	return mTouchMethod;
}

//*****************************************************************************
void VuSettingsManager::setEffectVolume(float value)
{
	mEffectVolume = value;

	setCategoryVolume("game", value);
	setCategoryVolume("ui", value);
}

//*****************************************************************************
void VuSettingsManager::setMusicVolume(float value)
{
	mMusicVolume = value;

	setCategoryVolume("music", value);
}

//*****************************************************************************
void VuSettingsManager::OnSaveProfile(const VuParams &params)
{
	VuJsonContainer &settingsData = VuProfileManager::IF()->dataWrite()["Settings"];

	// controls
	{
		VuJsonContainer &controls = settingsData["Controls"];
		controls["AutoThrottleGamePad"].putValue(getAutoThrottleGamePad());
		controls["AutoThrottleKeyboard"].putValue(getAutoThrottleKeyboard());
		controls["TouchMethod"].putValue(sTouchMethodLookup[getTouchMethod()]);
		controls["SteeringSensitivity"].putValue(getSteeringSensitivity());
		controls["CameraTilt"].putValue(getCameraTilt());
	}

	// audio
	{
		VuJsonContainer &audio = settingsData["Audio"];
		audio["EffectVolume"].putValue(getEffectVolume());
		audio["MusicVolume"].putValue(getMusicVolume());
	}

	// graphics
	if ( !VuDevConfig::IF()->hasParam("DeviceType") && VuConfigManager::IF()->stackSize() == 0 )
	{
		VuJsonContainer &graphics = settingsData["Graphics"];

		saveConfigFloat(graphics, "Composer/DisplayScale");
		saveConfigFloat(graphics, "Composer/ShadowDetail");
		saveConfigBool(graphics, "Water/NormalMap");
		saveConfigFloat(graphics, "Water/Detail");
		saveConfigBool(graphics, "Water/Reflection");
		saveConfigBool(graphics, "Water/Wakes");
		saveConfigBool(graphics, "Effects/LensWater");
		saveConfigBool(graphics, "Effects/RadialBlur");
		saveConfigBool(graphics, "Effects/ColorCorrection");
		saveConfigBool(graphics, "Gfx/LowModelLOD");
		saveConfigBool(graphics, "Gfx/LowTextureLOD");
		saveConfigInt(graphics, "Gfx/ShaderLOD");
		saveConfigInt(graphics, "Gfx/FlipInterval");
		saveConfigInt(graphics, "Scene/Complexity");
		saveConfigFloat(graphics, "Gfx/SafeZone");
	}
}

//*****************************************************************************
void VuSettingsManager::setCategoryVolume(const char *name, float vol) const
{
#if !VU_DISABLE_AUDIO
	FMOD::EventCategory *pCategory;
	if ( VuAudio::IF()->eventSystem()->getCategory(name, &pCategory) == FMOD_OK )
		pCategory->setVolume(vol);
#endif
}

//*****************************************************************************
void VuSettingsManager::loadConfigBool(const VuJsonContainer &data, const char *name)
{
	bool value;
	if ( data[name].getValue(value) )
		VuConfigManager::IF()->setBoolSetting(name, value);
}

//*****************************************************************************
void VuSettingsManager::loadConfigFloat(const VuJsonContainer &data, const char *name)
{
	float value;
	if ( data[name].getValue(value) )
		VuConfigManager::IF()->setFloatSetting(name, value);
}

//*****************************************************************************
void VuSettingsManager::loadConfigInt(const VuJsonContainer &data, const char *name)
{
	int value;
	if ( data[name].getValue(value) )
		VuConfigManager::IF()->setIntSetting(name, value);
}

//*****************************************************************************
void VuSettingsManager::saveConfigBool(VuJsonContainer &data, const char *name)
{
	VuConfigManager::Bool *pValue = VuConfigManager::IF()->getBool(name);
	if ( pValue->mValue != pValue->mDefaultValue )
		data[name].putValue(pValue->mValue);
	else
		data.removeMember(name);
}

//*****************************************************************************
void VuSettingsManager::saveConfigFloat(VuJsonContainer &data, const char *name)
{
	VuConfigManager::Float *pValue = VuConfigManager::IF()->getFloat(name);
	if ( pValue->mValue != pValue->mDefaultValue )
		data[name].putValue(pValue->mValue);
	else
		data.removeMember(name);
}

//*****************************************************************************
void VuSettingsManager::saveConfigInt(VuJsonContainer &data, const char *name)
{
	VuConfigManager::Int *pValue = VuConfigManager::IF()->getInt(name);
	if ( pValue->mValue != pValue->mDefaultValue )
		data[name].putValue(pValue->mValue);
	else
		data.removeMember(name);
}

//*****************************************************************************
void VuSettingsManager::setDefaultConfigBool(const char *name)
{
	VuConfigManager::Bool *pValue = VuConfigManager::IF()->getBool(name);
	pValue->mValue = pValue->mDefaultValue;
}

//*****************************************************************************
void VuSettingsManager::setDefaultConfigFloat(const char *name)
{
	VuConfigManager::Float *pValue = VuConfigManager::IF()->getFloat(name);

// modify by xlm,[/2014/12/18 ] 
	//pValue->mValue = pValue->mDefaultValue;

	pValue->mValue = 0.75;
}

//*****************************************************************************
void VuSettingsManager::setDefaultConfigInt(const char *name)
{
	VuConfigManager::Int *pValue = VuConfigManager::IF()->getInt(name);
	pValue->mValue = pValue->mDefaultValue;
}

//*****************************************************************************
const VuJsonContainer &VuSettingsManager::getDefaultSettings()
{
	const VuJsonContainer &defaultSettings = VuGameUtil::IF()->constantDB()["DefaultSettings"][VUPLATFORM];
	const std::string &sku = VuAssetFactory::IF()->getSku();

	const VuJsonContainer &defaults = defaultSettings.hasMember(sku) ? defaultSettings[sku] : defaultSettings;

	return defaults;
}