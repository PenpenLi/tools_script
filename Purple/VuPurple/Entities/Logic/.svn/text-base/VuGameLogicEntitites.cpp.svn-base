//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Game Logic entities
// 
//*****************************************************************************

#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Managers/VuControlMethodManager.h"
#include "VuPurple/Managers/VuSettingsManager.h"
#include "VuPurple/Managers/VuStatsManager.h"
#include "VuPurple/Managers/VuVideoShareManager.h"
#include "VuPurple/Managers/VuCloudTuningManager.h"
#include "VuPurple/Managers/VuBillingManager.h"
#include "VuPurple/Util/VuGameFontMacros.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Properties/VuEnumProperty.h"
#include "VuEngine/Properties/VuAssetProperty.h"
#include "VuEngine/Assets/VuAssetFactory.h"
#include "VuEngine/Assets/VuProjectAsset.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/HAL/GamePad/VuGamePad.h"
#include "VuEngine/Util/VuStringUtil.h"
#include "VuPurple/Managers/VuPackManager.h"
#include "VuPurple/Util/CommonUtility.h"
#include "VuEngine/Properties/VuDBEntryProperty.h"

#if defined VUANDROID || defined VUIOS


#include "VuEngine/Util/VuLog.h"

#else
#include "VuEngine/Prefix/VuDebugSettings.h"


#endif
#include "VuPurple/Managers/VuRankManager.h"
#include "VuPurple/Entities/UI//VuBroadcastManager.h"
#include "VuPurple/Managers/VuRequestOrderManager.h"
#include "VuPurple/Managers/VuGiftDialogManager.h"


//*****************************************************************************
// EventType
//*****************************************************************************
class VuIsEventTypeEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuIsEventTypeEntity();

private:
	VuRetVal			Trigger(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;

	// properties
	std::string			mType;
};

IMPLEMENT_RTTI(VuIsEventTypeEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuIsEventTypeEntity);


//*****************************************************************************
VuIsEventTypeEntity::VuIsEventTypeEntity():
	mType("AttractEvent")
{
	addComponent(mpScriptComponent = new VuScriptComponent(this, 100));

	static VuStaticStringEnumProperty::Choice sTypeChoices[] =
	{
		"AttractEvent",
		"DemoEvent",
		"SinglePlayerEvent",
		"ChallengeEvent",
		"TutorialDriveEvent",
		"TutorialPowerUpEvent",
		"TutorialRaceEvent",
		"DuelEvent",
		"CarChampEvent",
		VUNULL
	};
	addProperty(new VuStaticStringEnumProperty("Type", mType, sTypeChoices));

	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuIsEventTypeEntity, Trigger);
	ADD_SCRIPT_OUTPUT(mpScriptComponent, True, VuRetVal::Void, VuParamDecl());
	ADD_SCRIPT_OUTPUT(mpScriptComponent, False, VuRetVal::Void, VuParamDecl());
}

//*****************************************************************************
VuRetVal VuIsEventTypeEntity::Trigger(const VuParams &params)
{
	if ( mType == VuGameUtil::IF()->dataRead()["GameData"]["EventType"].asString() )
		mpScriptComponent->getPlug("True")->execute(params);
	else
		mpScriptComponent->getPlug("False")->execute(params);

	return VuRetVal();
}


//*****************************************************************************
// Human
//*****************************************************************************
class VuHumanFilterEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuHumanFilterEntity();

private:
	VuRetVal			In(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;

	// properties
	bool				mLocal;
	bool				mSinglePlayer;
};

IMPLEMENT_RTTI(VuHumanFilterEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuHumanFilterEntity);


//*****************************************************************************
VuHumanFilterEntity::VuHumanFilterEntity():
	mLocal(true),
	mSinglePlayer(false)
{
	addComponent(mpScriptComponent = new VuScriptComponent(this, 100));

	addProperty(new VuBoolProperty("Local", mLocal));
	addProperty(new VuBoolProperty("Single Player", mSinglePlayer));

	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuHumanFilterEntity, In);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, Out);
}

//*****************************************************************************
VuRetVal VuHumanFilterEntity::In(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);

	if ( accessor.getNextType() == VuParams::Entity )
	{
		VuEntity *pEntity = accessor.getEntity();
		if (pEntity && pEntity->isDerivedFrom(VuCarEntity::msRTTI) )
		{
			VuCarEntity *pCar = static_cast<VuCarEntity *>(pEntity);
			if ( pCar->getDriver()->isHuman() )
			{
				if ( mLocal == false || pCar->getDriver()->isLocal() )
				{
					if ( mSinglePlayer == false || VuCarManager::IF()->getLocalHumanCarCount() == 1 )
					{
						mpScriptComponent->getPlug("Out")->execute(params);
					}
				}
			}
		}
	}

	return VuRetVal();
}


//*****************************************************************************
// IsDemo
//*****************************************************************************
class VuIsDemoEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuIsDemoEntity();

private:
	VuRetVal			Trigger(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuIsDemoEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuIsDemoEntity);


//*****************************************************************************
VuIsDemoEntity::VuIsDemoEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this, 100));

	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuIsDemoEntity, Trigger);
	ADD_SCRIPT_OUTPUT(mpScriptComponent, True, VuRetVal::Void, VuParamDecl());
	ADD_SCRIPT_OUTPUT(mpScriptComponent, False, VuRetVal::Void, VuParamDecl());
}

//*****************************************************************************
VuRetVal VuIsDemoEntity::Trigger(const VuParams &params)
{
	if ( VuGameUtil::IF()->isDemoMode() )
		mpScriptComponent->getPlug("True")->execute(params);
	else
		mpScriptComponent->getPlug("False")->execute(params);

	return VuRetVal();
}


//*****************************************************************************
// IsSku
//*****************************************************************************
class VuIsSkuEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuIsSkuEntity();

private:
	VuRetVal			Trigger(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;

	// properties
	std::string			mSku;
};

IMPLEMENT_RTTI(VuIsSkuEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuIsSkuEntity);


//*****************************************************************************
VuIsSkuEntity::VuIsSkuEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this, 100));

	addProperty(new VuStringProperty("SKU", mSku));

	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuIsSkuEntity, Trigger);
	ADD_SCRIPT_OUTPUT(mpScriptComponent, True, VuRetVal::Void, VuParamDecl());
	ADD_SCRIPT_OUTPUT(mpScriptComponent, False, VuRetVal::Void, VuParamDecl());
}

//*****************************************************************************
VuRetVal VuIsSkuEntity::Trigger(const VuParams &params)
{
	bool result = false;

	char str[256];
	VU_STRNCPY(str, sizeof(str), mSku.c_str(), 255);
	str[255] = '\0';

	char *nextToken;
	const char *p = VU_STRTOK(str, ";", &nextToken);
	while ( p )
	{
		if ( VuAssetFactory::IF()->getSku() == p )
			result = true;
		p = VU_STRTOK(VUNULL, ";", &nextToken);
	}

	if ( result )
		mpScriptComponent->getPlug("True")->execute(params);
	else
		mpScriptComponent->getPlug("False")->execute(params);

	return VuRetVal();
}


//*****************************************************************************
// OncePerLapFilter
//*****************************************************************************
class VuOncePerLapFilterEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuOncePerLapFilterEntity();

private:
	VuRetVal			In(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;

	int					mCurLap;
};

IMPLEMENT_RTTI(VuOncePerLapFilterEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuOncePerLapFilterEntity);


//*****************************************************************************
VuOncePerLapFilterEntity::VuOncePerLapFilterEntity():
	mCurLap(-1)
{
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuOncePerLapFilterEntity, In);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, Out);
}

//*****************************************************************************
VuRetVal VuOncePerLapFilterEntity::In(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);

	if ( accessor.getNextType() == VuParams::Entity )
	{
		VuEntity *pEntity = accessor.getEntity();

		if ( pEntity->isDerivedFrom(VuCarEntity::msRTTI) )
		{
			VuCarEntity *pCar = static_cast<VuCarEntity *>(pEntity);
			const VuCarStats &stats = pCar->getStats();
			if ( stats.mCurLap > mCurLap )
			{
				mCurLap = stats.mCurLap;
				mpScriptComponent->getPlug("Out")->execute(params);
			}
		}
	}

	return VuRetVal();
}


//*****************************************************************************
// Player Result
//*****************************************************************************
class VuPlayerResultEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuPlayerResultEntity();

private:
	VuRetVal			Trigger(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuPlayerResultEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuPlayerResultEntity);


//*****************************************************************************
VuPlayerResultEntity::VuPlayerResultEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this, 100));

	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuPlayerResultEntity, Trigger);
	ADD_SCRIPT_OUTPUT(mpScriptComponent, 1, VuRetVal::Void, VuParamDecl());
	ADD_SCRIPT_OUTPUT(mpScriptComponent, 2, VuRetVal::Void, VuParamDecl());
	ADD_SCRIPT_OUTPUT(mpScriptComponent, 3, VuRetVal::Void, VuParamDecl());
	ADD_SCRIPT_OUTPUT(mpScriptComponent, 4+, VuRetVal::Void, VuParamDecl());
}

//*****************************************************************************
VuRetVal VuPlayerResultEntity::Trigger(const VuParams &params)
{
	if ( VuCarManager::IF()->getLocalHumanCarCount() )
	{
		VuCarEntity *pCar = VuCarManager::IF()->getLocalHumanCar(0);

		int place = pCar->getStats().mPlace;

		if ( place == 1 )
			mpScriptComponent->getPlug("1")->execute(params);
		else if ( place == 2 )
			mpScriptComponent->getPlug("2")->execute(params);
		else if ( place == 3 )
			mpScriptComponent->getPlug("3")->execute(params);
		else
			mpScriptComponent->getPlug("4+")->execute(params);
	}

	return VuRetVal();
}


//*****************************************************************************
// GamePadConnected
//*****************************************************************************
class VuIsGamePadConnectedEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuIsGamePadConnectedEntity();

private:
	VuRetVal			Trigger(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuIsGamePadConnectedEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuIsGamePadConnectedEntity);

//*****************************************************************************
VuIsGamePadConnectedEntity::VuIsGamePadConnectedEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuIsGamePadConnectedEntity, Trigger);
	ADD_SCRIPT_OUTPUT(mpScriptComponent, True, VuRetVal::Void, VuParamDecl());
	ADD_SCRIPT_OUTPUT(mpScriptComponent, False, VuRetVal::Void, VuParamDecl());
}

//*****************************************************************************
VuRetVal VuIsGamePadConnectedEntity::Trigger(const VuParams &params)
{
	if ( VuControlMethodManager::IF()->wasGamePadUsed() )
		mpScriptComponent->getPlug("True")->execute(params);
	else
		mpScriptComponent->getPlug("False")->execute(params);

	return VuRetVal();
}


//*****************************************************************************
// GamePadDeviceType
//*****************************************************************************
class VuIsGamePadDeviceTypeEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuIsGamePadDeviceTypeEntity();

private:
	VuRetVal			Trigger(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;

	// properties
	std::string			mType;
};

IMPLEMENT_RTTI(VuIsGamePadDeviceTypeEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuIsGamePadDeviceTypeEntity);

static VuStaticStringEnumProperty::Choice sGamePadDeviceTypeChoices[] =
{
	"UNKNOWN",        // DEVICE_UNKNOWN,
	"GAMEPAD",        // DEVICE_GAMEPAD,
	"SIMPLE_GAMEPAD", // DEVICE_SIMPLE_GAMEPAD,
	"REMOTE_CONTROL", // DEVICE_REMOTE_CONTROL,
	"STEERING_WHEEL", // DEVICE_STEERING_WHEEL,
	VUNULL 
};
VU_COMPILE_TIME_ASSERT(sizeof(sGamePadDeviceTypeChoices)/sizeof(sGamePadDeviceTypeChoices[0]) == VuGamePad::NUM_DEVICE_TYPES + 1);

//*****************************************************************************
VuIsGamePadDeviceTypeEntity::VuIsGamePadDeviceTypeEntity()
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));
	
	// properties
	addProperty(new VuStaticStringEnumProperty("Type", mType, sGamePadDeviceTypeChoices));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuIsGamePadDeviceTypeEntity, Trigger);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, True);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, False);
}

//*****************************************************************************
VuRetVal VuIsGamePadDeviceTypeEntity::Trigger(const VuParams &params)
{
	int padIndex = VuControlMethodManager::IF()->recentPadIndex();
	VuGamePad::eDeviceType deviceType = VuGamePad::IF()->getController(padIndex).mDeviceType;
	if ( mType == sGamePadDeviceTypeChoices[deviceType].mpName )
		mpScriptComponent->getPlug("True")->execute(params);
	else
		mpScriptComponent->getPlug("False")->execute(params);

	return VuRetVal();
}


//*****************************************************************************
// GetTouchMethod
//*****************************************************************************
class VuGetTouchMethodEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuGetTouchMethodEntity() {
		addComponent(mpScriptComponent = new VuScriptComponent(this, 100));

		ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuGetTouchMethodEntity, Trigger);
		ADD_SCRIPT_OUTPUT(mpScriptComponent, Tilt, VuRetVal::Void, VuParamDecl());
		ADD_SCRIPT_OUTPUT(mpScriptComponent, TouchA, VuRetVal::Void, VuParamDecl());
		ADD_SCRIPT_OUTPUT(mpScriptComponent, TouchB, VuRetVal::Void, VuParamDecl());
	}

private:
	VuRetVal Trigger(const VuParams &params) {
		if ( VuSettingsManager::IF()->getTouchMethod() == VuSettingsManager::TILT )
			mpScriptComponent->getPlug("Tilt")->execute(params);
		else if ( VuSettingsManager::IF()->getTouchMethod() == VuSettingsManager::TOUCH_A )
			mpScriptComponent->getPlug("TouchA")->execute(params);
		else if ( VuSettingsManager::IF()->getTouchMethod() == VuSettingsManager::TOUCH_B )
			mpScriptComponent->getPlug("TouchB")->execute(params);

		return VuRetVal();
	}

	// components
	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuGetTouchMethodEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuGetTouchMethodEntity);


//*****************************************************************************
// IsControllerActive
//*****************************************************************************
class VuIsControllerActiveEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuIsControllerActiveEntity();

	virtual void		onGameInitialize();
	virtual void		onGameRelease();

private:
	VuRetVal			IsActive(const VuParams &params);

	void				tickDecision(float fdt);

	// components
	VuScriptComponent	*mpScriptComponent;

	bool				mIsActive;
};

IMPLEMENT_RTTI(VuIsControllerActiveEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuIsControllerActiveEntity);

//*****************************************************************************
VuIsControllerActiveEntity::VuIsControllerActiveEntity():
	mIsActive(false)
{
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	ADD_SCRIPT_INPUT(mpScriptComponent, VuIsControllerActiveEntity, IsActive, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_OUTPUT(mpScriptComponent, OnActivate, VuRetVal::Void, VuParamDecl());
	ADD_SCRIPT_OUTPUT(mpScriptComponent, OnDeactivate, VuRetVal::Void, VuParamDecl());
}

//*****************************************************************************
void VuIsControllerActiveEntity::onGameInitialize()
{
	mIsActive = VuControlMethodManager::IF()->isGamePadOrKeyboard();

	VuTickManager::IF()->registerHandler(this, &VuIsControllerActiveEntity::tickDecision, "Decision");
}

//*****************************************************************************
void VuIsControllerActiveEntity::onGameRelease()
{
	VuTickManager::IF()->unregisterHandler(this, "Decision");
}

//*****************************************************************************
void VuIsControllerActiveEntity::tickDecision(float fdt)
{
	bool isActive = VuControlMethodManager::IF()->isGamePadOrKeyboard();
	if ( isActive != mIsActive )
	{
		mIsActive = isActive;
		if ( isActive )
			mpScriptComponent->getPlug("OnActivate")->execute();
		else
			mpScriptComponent->getPlug("OnDeactivate")->execute();
	}
}

//*****************************************************************************
VuRetVal VuIsControllerActiveEntity::IsActive(const VuParams &params)
{
	bool isActive = VuControlMethodManager::IF()->isGamePadOrKeyboard();
	return VuRetVal(isActive);
}


//*****************************************************************************
// GetBestPlace
//*****************************************************************************
class VuGetBestPlaceEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuGetBestPlaceEntity();

private:
	VuRetVal			Get(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;

	int					mBestPlace;
};

IMPLEMENT_RTTI(VuGetBestPlaceEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuGetBestPlaceEntity);


//*****************************************************************************
VuGetBestPlaceEntity::VuGetBestPlaceEntity():
	mBestPlace(-1)
{
	addComponent(mpScriptComponent = new VuScriptComponent(this, 100));

	ADD_SCRIPT_INPUT(mpScriptComponent, VuGetBestPlaceEntity, Get, VuRetVal::Int, VuParamDecl());

	if ( VuStatsManager::IF() )
	{
		const std::string &eventName = VuGameUtil::IF()->getEventName();
		VuStatsManager::IF()->getBestPlace(eventName.c_str(), mBestPlace);
	}
}

//*****************************************************************************
VuRetVal VuGetBestPlaceEntity::Get(const VuParams &params)
{
	return VuRetVal(mBestPlace);
}


//*****************************************************************************
// VuGetPlaceEntity
//*****************************************************************************
class VuGetPlaceEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuGetPlaceEntity();

private:
	VuRetVal			GetPlace(const VuParams &params);
	VuRetVal			In(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;

	// properties
	int					mViewport;
};

IMPLEMENT_RTTI(VuGetPlaceEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuGetPlaceEntity);

//*****************************************************************************
VuGetPlaceEntity::VuGetPlaceEntity():
	mViewport(0)
{
	// properties
	addProperty(new VuIntProperty("Viewport", mViewport));

	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this, 100));

	// scripting
	ADD_SCRIPT_INPUT(mpScriptComponent, VuGetPlaceEntity, GetPlace, VuRetVal::Int, VuParamDecl());
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuGetPlaceEntity, In);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, Won);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, Lost);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, 1st);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, 2nd);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, 3rd);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, 4th);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, 5th);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, 6th);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, DNF);
}

//*****************************************************************************
VuRetVal VuGetPlaceEntity::GetPlace(const VuParams &params)
{
	int place = 0;

	if ( VuCarEntity *pCar = VuCarManager::IF()->getCameraTargetForViewport(mViewport) )
		place = pCar->getStats().mPlace;

	return VuRetVal(place);
}

//*****************************************************************************
VuRetVal VuGetPlaceEntity::In(const VuParams &params)
{
	if ( VuCarEntity *pCar = VuCarManager::IF()->getCameraTargetForViewport(mViewport) )
	{
		int place = pCar->getStats().mPlace;

		if ( place == 1 )
			mpScriptComponent->getPlug("Won")->execute();
		else
			mpScriptComponent->getPlug("Lost")->execute();

		if ( pCar->getStats().mDNF )
			mpScriptComponent->getPlug("DNF")->execute();
		else if ( place == 1 )
			mpScriptComponent->getPlug("1st")->execute();
		else if ( place == 2 )
			mpScriptComponent->getPlug("2nd")->execute();
		else if ( place == 3 )
			mpScriptComponent->getPlug("3rd")->execute();
		else if ( place == 4 )
			mpScriptComponent->getPlug("4th")->execute();
		else if ( place == 5 )
			mpScriptComponent->getPlug("5th")->execute();
		else if ( place == 6 )
			mpScriptComponent->getPlug("6th")->execute();
	}

	return VuRetVal();
}


//*****************************************************************************
// VuGetCarChampPlaceEntity
//*****************************************************************************
class VuGetCarChampPlaceEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuGetCarChampPlaceEntity();

	virtual void		onGameInitialize();

private:
	VuRetVal			GetPlace(const VuParams &params) { return VuRetVal(mPlace); }
	VuRetVal			In(const VuParams &params);
	VuRetVal			IsFirstPlace(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;

	// properties
	int					mViewport;

	int					mPlace;
};

IMPLEMENT_RTTI(VuGetCarChampPlaceEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuGetCarChampPlaceEntity);

//*****************************************************************************
VuGetCarChampPlaceEntity::VuGetCarChampPlaceEntity():
	mViewport(0),
	mPlace(0)
{
	// properties
	addProperty(new VuIntProperty("Viewport", mViewport));

	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	// scripting
	ADD_SCRIPT_INPUT(mpScriptComponent, VuGetCarChampPlaceEntity, GetPlace, VuRetVal::Int, VuParamDecl());
	ADD_SCRIPT_INPUT(mpScriptComponent, VuGetCarChampPlaceEntity, IsFirstPlace, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuGetCarChampPlaceEntity, In);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, Won);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, Lost);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, 1st);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, 2nd);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, 3rd);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, 4th);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, 5th);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, 6th);
}

VuRetVal	VuGetCarChampPlaceEntity::IsFirstPlace(const VuParams &params)
{
	bool result = false;

	if (mPlace == 1)
		result = true;
	return VuRetVal(result);
}
//*****************************************************************************
void VuGetCarChampPlaceEntity::onGameInitialize()
{
	const std::string &carName = VuGameUtil::IF()->dataRead()["CarChamp"]["Car"].asString();

	VuGameUtil::CarChampTable table;
	VuGameUtil::IF()->createCarChampTable(carName, table);

	for ( int i = 0; i < (int)table.size(); i++ )
		if ( table[i].mPlayer )
			mPlace = i + 1;
}

//*****************************************************************************
VuRetVal VuGetCarChampPlaceEntity::In(const VuParams &params)
{
	if ( mPlace == 1 )
		mpScriptComponent->getPlug("Won")->execute();
	else
		mpScriptComponent->getPlug("Lost")->execute();

	if ( mPlace == 1 )
		mpScriptComponent->getPlug("1st")->execute();
	else if ( mPlace == 2 )
		mpScriptComponent->getPlug("2nd")->execute();
	else if ( mPlace == 3 )
		mpScriptComponent->getPlug("3rd")->execute();
	else if ( mPlace == 4 )
		mpScriptComponent->getPlug("4th")->execute();
	else if ( mPlace == 5 )
		mpScriptComponent->getPlug("5th")->execute();
	else if ( mPlace == 6 )
		mpScriptComponent->getPlug("6th")->execute();

	return VuRetVal();
}


//*****************************************************************************
// GetCarChampBestPlace
//*****************************************************************************
class VuGetCarChampBestPlaceEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuGetCarChampBestPlaceEntity();

private:
	VuRetVal			Get(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;

	int					mBestPlace;
};

IMPLEMENT_RTTI(VuGetCarChampBestPlaceEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuGetCarChampBestPlaceEntity);


//*****************************************************************************
VuGetCarChampBestPlaceEntity::VuGetCarChampBestPlaceEntity():
	mBestPlace(-1)
{
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	ADD_SCRIPT_INPUT(mpScriptComponent, VuGetCarChampBestPlaceEntity, Get, VuRetVal::Int, VuParamDecl());

	if ( VuStatsManager::IF() )
	{
		const std::string &carName = VuGameUtil::IF()->dataRead()["CarChamp"]["Car"].asString();
		int stage = VuGameUtil::IF()->dataRead()["CarChamp"]["Stage"].asInt();
		VuStatsManager::IF()->getCarChampBestPlace(carName.c_str(), stage, mBestPlace);
	}
}

//*****************************************************************************
VuRetVal VuGetCarChampBestPlaceEntity::Get(const VuParams &params)
{
	return VuRetVal(mBestPlace);
}


//*****************************************************************************
// GetStageBestPlace
//*****************************************************************************
class VuGetCarChampStageEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuGetCarChampStageEntity();

private:
	VuRetVal			Trigger(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuGetCarChampStageEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuGetCarChampStageEntity);


static const char *sStages[] =
{
	"100hp",
	"250hp",
	"500hp",
	"1000hp",
};
static const int sStageCount = sizeof(sStages)/sizeof(sStages[0]);

//*****************************************************************************
VuGetCarChampStageEntity::VuGetCarChampStageEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuGetCarChampStageEntity, Trigger);

	for ( int i = 0; i < sStageCount; i++ )
		mpScriptComponent->addPlug(new VuScriptOutputPlug(sStages[i]));
}

//*****************************************************************************
VuRetVal VuGetCarChampStageEntity::Trigger(const VuParams &params)
{
	int stage = VuGameUtil::IF()->dataRead()["CarChamp"]["Stage"].asInt();

	if ( stage >= 0 && stage < sStageCount )
		mpScriptComponent->getPlug(sStages[stage])->execute(params);

	return VuRetVal();
}


//*****************************************************************************
// TuneUps
//*****************************************************************************
class VuTuneUpsEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuTuneUpsEntity();

private:
	VuRetVal			HaveTuneUp(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuTuneUpsEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuTuneUpsEntity);

//*****************************************************************************
VuTuneUpsEntity::VuTuneUpsEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this, 100));

	ADD_SCRIPT_INPUT(mpScriptComponent, VuTuneUpsEntity, HaveTuneUp, VuRetVal::Bool, VuParamDecl());
}

//*****************************************************************************
VuRetVal VuTuneUpsEntity::HaveTuneUp(const VuParams &params)
{
	bool haveTuneUps = VuGameManager::IF()->getTuneUps() > 0;
	return VuRetVal(haveTuneUps);
}

//*****************************************************************************
// PlayerInfo
//*****************************************************************************
class VuPlayerInfoEntity : public VuEntity
{
	DECLARE_RTTI
public:
	VuPlayerInfoEntity();
private:
	VuRetVal		HavePlayerName(const VuParams& params);
	VuRetVal		IsBroadcastEnabled(const VuParams& params);
	VuRetVal		clickBroadcast(const VuParams& params);
	VuRetVal		IsFirstEnterMainMenu(const VuParams& params);
	VuRetVal		IsBroadcastDataIsOK(const VuParams& params);
	VuRetVal		StartRequestPlayerName(const VuParams& params);
	VuRetVal		StartRequestOrder(const VuParams& params);
	VuRetVal		CommitChoosePlayerName(const VuParams& params);
	VuRetVal		Is1_8VersionFirstLogin(const VuParams& params);
	// components
	VuScriptComponent	*mpScriptComponent;
};
IMPLEMENT_RTTI(VuPlayerInfoEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuPlayerInfoEntity);
//*****************************************************************************
VuPlayerInfoEntity::VuPlayerInfoEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this, 100));
	ADD_SCRIPT_INPUT(mpScriptComponent, VuPlayerInfoEntity, HavePlayerName, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT(mpScriptComponent, VuPlayerInfoEntity, IsFirstEnterMainMenu, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT(mpScriptComponent, VuPlayerInfoEntity, IsBroadcastEnabled, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT(mpScriptComponent, VuPlayerInfoEntity, IsBroadcastDataIsOK, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT(mpScriptComponent, VuPlayerInfoEntity, Is1_8VersionFirstLogin, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuPlayerInfoEntity, clickBroadcast);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuPlayerInfoEntity, StartRequestPlayerName);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuPlayerInfoEntity, StartRequestOrder);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuPlayerInfoEntity, CommitChoosePlayerName);


	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, On1_8VersionFirstLogin);
}
//*****************************************************************************
VuRetVal	VuPlayerInfoEntity::clickBroadcast(const VuParams& params)
{
	VuGameManager::IF()->clickBroadcast();
	return VuRetVal();
}
//*****************************************************************************
VuRetVal	VuPlayerInfoEntity::IsBroadcastEnabled(const VuParams& params)
{
	bool result = VuGameManager::IF()->isClickBroadcast();
	return VuRetVal(!result);
}
//*****************************************************************************
VuRetVal	VuPlayerInfoEntity::IsFirstEnterMainMenu(const VuParams& params)
{
	unsigned int enterTimes = VuGameManager::IF()->getEnterMainMenuTimes();
	VuGameManager::IF()->addEnterMainMenuTimes();
	bool result;
	result = (enterTimes == 0) ? true : false;
	return VuRetVal(result);
}
//*****************************************************************************
VuRetVal VuPlayerInfoEntity::HavePlayerName(const VuParams& params)
{
	bool havePlayerName = VuGameManager::IF()->IsHavePlayerName();
	return VuRetVal(havePlayerName);
}

VuRetVal	VuPlayerInfoEntity::StartRequestOrder(const VuParams& params)
{
	VuRequestOrderManager::IF()->startWork();
	return VuRetVal();
}

VuRetVal	VuPlayerInfoEntity::StartRequestPlayerName(const VuParams& params)
{
	if (VuGameManager::IF()->getNameTimes() >= 1)
		VuRankManager::IF()->startGetPlayerName();
	return VuRetVal();
}
VuRetVal VuPlayerInfoEntity::CommitChoosePlayerName(const VuParams& params)
{
	VuRankManager::IF()->commitTempPlayerName();
	return VuRetVal();
}
VuRetVal	VuPlayerInfoEntity::IsBroadcastDataIsOK(const VuParams& params)
{
	bool result = VuBroadcastManager::IF()->isDataOK();
	return VuRetVal(result);
}
VuRetVal	VuPlayerInfoEntity::Is1_8VersionFirstLogin(const VuParams& params)
{
	bool result = VuGameManager::IF()->getIs1_8VersionFirstLogin();
	if (result == true)
	{
		VuGameManager::IF()->set1_8VersionFirstLogin();
		mpScriptComponent->getPlug("On1_8VersionFirstLogin")->execute();
	}
		
	return VuRetVal(result);
}
//*****************************************************************************
// Tokens
//*****************************************************************************
class VuTokensEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuTokensEntity();

private:
	VuRetVal			HaveToken(const VuParams &params);
	VuRetVal			UseToken(const  VuParams &params);
	VuRetVal			AddToken(const  VuParams &params);
	VuRetVal			RefillTokens(const  VuParams &params);
	VuRetVal			FullTokens(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuTokensEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuTokensEntity);

//*****************************************************************************
VuTokensEntity::VuTokensEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this, 100));

	ADD_SCRIPT_INPUT(mpScriptComponent, VuTokensEntity, HaveToken, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT(mpScriptComponent, VuTokensEntity, UseToken, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuTokensEntity, AddToken);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuTokensEntity, RefillTokens);
	ADD_SCRIPT_INPUT(mpScriptComponent, VuTokensEntity, FullTokens, VuRetVal::Bool, VuParamDecl());
}

//*****************************************************************************
VuRetVal VuTokensEntity::HaveToken(const VuParams &params)
{
	bool haveToken = VuGameManager::IF()->getTokens() > 0;
	return VuRetVal(haveToken);
}

//*****************************************************************************
VuRetVal VuTokensEntity::UseToken(const VuParams &params)
{
	bool success = VuGameManager::IF()->useToken();
	return VuRetVal(success);
}

//*****************************************************************************
VuRetVal VuTokensEntity::AddToken(const VuParams &params)
{
	VuGameManager::IF()->addToken();
	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuTokensEntity::RefillTokens(const VuParams &params)
{
	VuGameManager::IF()->refillTokens();
	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuTokensEntity::FullTokens(const VuParams &params)
{
	bool fullTokens = VuGameManager::IF()->getTokens() >= VuCloudTuningManager::IF()->variables().mMaxTokens;
	return VuRetVal(fullTokens);
}


//*****************************************************************************
// VuChampStateEntity
//*****************************************************************************
class VuChampStateEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuChampStateEntity();

private:
	VuRetVal			IsFinished(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuChampStateEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuChampStateEntity);

//*****************************************************************************
VuChampStateEntity::VuChampStateEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this, 100));

	ADD_SCRIPT_INPUT(mpScriptComponent, VuChampStateEntity, IsFinished, VuRetVal::Bool, VuParamDecl());
}

//*****************************************************************************
VuRetVal VuChampStateEntity::IsFinished(const VuParams &params)
{
	const std::string &champName = VuGameUtil::IF()->getChampName();
	int totalEvents = VuGameUtil::IF()->champDB()[champName]["Events"].size();
	int eventNumber = VuGameUtil::IF()->dataRead()["GameData"]["EventIndex"].asInt();

	bool finished = (eventNumber + 1 >= totalEvents);

	return VuRetVal(finished);
}


//*****************************************************************************
// VuWasLastScreenEntity
//*****************************************************************************
class VuWasLastScreenEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuWasLastScreenEntity();

private:
	VuRetVal			Trigger(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;

	// properties
	std::string			mScreen;
};

IMPLEMENT_RTTI(VuWasLastScreenEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuWasLastScreenEntity);

//*****************************************************************************
VuWasLastScreenEntity::VuWasLastScreenEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	addProperty(new VuAssetNameProperty(VuProjectAsset::msRTTI.mstrType, "Screen", mScreen));

	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuWasLastScreenEntity, Trigger);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, True);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, False);
}

//*****************************************************************************
VuRetVal VuWasLastScreenEntity::Trigger(const VuParams &params)
{
	if ( VuGameUtil::IF()->getLastScreen() == mScreen )
		mpScriptComponent->getPlug("True")->execute(params);
	else
		mpScriptComponent->getPlug("False")->execute(params);

	return VuRetVal();
}


//*****************************************************************************
// VuIsScreenStackTopEntity
//*****************************************************************************
class VuIsScreenStackTopEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuIsScreenStackTopEntity();

private:
	VuRetVal			Trigger(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;
	
	// properties
	std::string			mScreen;
};

IMPLEMENT_RTTI(VuIsScreenStackTopEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuIsScreenStackTopEntity);

//*****************************************************************************
VuIsScreenStackTopEntity::VuIsScreenStackTopEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	addProperty(new VuAssetNameProperty(VuProjectAsset::msRTTI.mstrType, "Screen", mScreen));

	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuIsScreenStackTopEntity, Trigger);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, True);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, False);
}

//*****************************************************************************
VuRetVal VuIsScreenStackTopEntity::Trigger(const VuParams &params)
{
	if ( VuGameUtil::IF()->screenStackSize() && VuGameUtil::IF()->screenStackTop() == mScreen )
		mpScriptComponent->getPlug("True")->execute(params);
	else
		mpScriptComponent->getPlug("False")->execute(params);

	return VuRetVal();
}


//*****************************************************************************
//  VuVideoShareEntity
//*****************************************************************************

class VuVideoShareEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuVideoShareEntity();

protected:
	// scripting
	virtual VuRetVal	IsSupported(const VuParams &params) { return VuRetVal(VuVideoShareManager::IF()->isSupported()); }
	virtual VuRetVal	StartRecording(const VuParams &params) { VuVideoShareManager::IF()->startRecording(); return VuRetVal(); }
	virtual VuRetVal	StopRecording(const VuParams &params) { VuVideoShareManager::IF()->stopRecording(); return VuRetVal(); }
	virtual VuRetVal	IsRecording(const VuParams &params) { return VuRetVal(VuVideoShareManager::IF()->isRecording()); }
	virtual VuRetVal	ResetRecording(const VuParams &params) { VuVideoShareManager::IF()->resetRecording(); return VuRetVal(); }
	virtual VuRetVal	HasRecording(const VuParams &params) { return VuRetVal(VuVideoShareManager::IF()->hasRecording()); }
	virtual VuRetVal	ShowShareUI(const VuParams &params) { VuVideoShareManager::IF()->showShareUI(); return VuRetVal(); }
	virtual VuRetVal	ShowWatchUI(const VuParams &params) { VuVideoShareManager::IF()->showWatchUI(); return VuRetVal(); }

	// components
	VuScriptComponent	*mpScriptComponent;
};


IMPLEMENT_RTTI(VuVideoShareEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuVideoShareEntity);


//*****************************************************************************
VuVideoShareEntity::VuVideoShareEntity()
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	// scripting
	ADD_SCRIPT_INPUT(mpScriptComponent, VuVideoShareEntity, IsSupported, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuVideoShareEntity, StartRecording);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuVideoShareEntity, StopRecording);
	ADD_SCRIPT_INPUT(mpScriptComponent, VuVideoShareEntity, IsRecording, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuVideoShareEntity, ResetRecording);
	ADD_SCRIPT_INPUT(mpScriptComponent, VuVideoShareEntity, HasRecording, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuVideoShareEntity, ShowShareUI);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuVideoShareEntity, ShowWatchUI);
}


//*****************************************************************************
// VuAreAdsDisabledEntity
//*****************************************************************************
class VuAreAdsDisabledEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuAreAdsDisabledEntity();

private:
	VuRetVal			Trigger(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuAreAdsDisabledEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuAreAdsDisabledEntity);


//*****************************************************************************
VuAreAdsDisabledEntity::VuAreAdsDisabledEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuAreAdsDisabledEntity, Trigger);
	ADD_SCRIPT_OUTPUT(mpScriptComponent, Yes, VuRetVal::Void, VuParamDecl());
	ADD_SCRIPT_OUTPUT(mpScriptComponent, No, VuRetVal::Void, VuParamDecl());
}

//*****************************************************************************
VuRetVal VuAreAdsDisabledEntity::Trigger(const VuParams &params)
{
	if ( VuBillingManager::IF()->isUnlocked("DisableAds") )
		mpScriptComponent->getPlug("Yes")->execute(params);
	else
		mpScriptComponent->getPlug("No")->execute(params);

	return VuRetVal();
}


//*****************************************************************************
// VuAreCoinsDoubledEntity
//*****************************************************************************
class VuAreCoinsDoubledEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuAreCoinsDoubledEntity();

private:
	VuRetVal			Trigger(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuAreCoinsDoubledEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuAreCoinsDoubledEntity);


//*****************************************************************************
VuAreCoinsDoubledEntity::VuAreCoinsDoubledEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuAreCoinsDoubledEntity, Trigger);
	ADD_SCRIPT_OUTPUT(mpScriptComponent, Yes, VuRetVal::Void, VuParamDecl());
	ADD_SCRIPT_OUTPUT(mpScriptComponent, No, VuRetVal::Void, VuParamDecl());
}

//*****************************************************************************
VuRetVal VuAreCoinsDoubledEntity::Trigger(const VuParams &params)
{
// modify by xlm,[/2014/12/10 ] 
	//原版
/*
	if ( VuBillingManager::IF()->isUnlocked("DoubleCoins") )
		mpScriptComponent->getPlug("Yes")->execute(params);
	else
		mpScriptComponent->getPlug("No")->execute(params);*/

	//去掉金币双倍

	mpScriptComponent->getPlug("No")->execute(params);





	return VuRetVal();
}


//*****************************************************************************
// VuIsSplitScreenUnlockedEntity
//*****************************************************************************
class VuIsSplitScreenUnlockedEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuIsSplitScreenUnlockedEntity();

private:
	VuRetVal			Trigger(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuIsSplitScreenUnlockedEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuIsSplitScreenUnlockedEntity);


//*****************************************************************************
VuIsSplitScreenUnlockedEntity::VuIsSplitScreenUnlockedEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuIsSplitScreenUnlockedEntity, Trigger);
	ADD_SCRIPT_OUTPUT(mpScriptComponent, Yes, VuRetVal::Void, VuParamDecl());
	ADD_SCRIPT_OUTPUT(mpScriptComponent, No, VuRetVal::Void, VuParamDecl());
}

//*****************************************************************************
VuRetVal VuIsSplitScreenUnlockedEntity::Trigger(const VuParams &params)
{
	if ( VuBillingManager::IF()->isUnlocked("SplitScreen") )
		mpScriptComponent->getPlug("Yes")->execute(params);
	else
		mpScriptComponent->getPlug("No")->execute(params);

	return VuRetVal();
}


//*****************************************************************************
// DisallowPauseMenu
//*****************************************************************************
class VuDisallowPauseMenuEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuDisallowPauseMenuEntity():
		mPauseDisallowed(false)
	{
		addComponent(mpScriptComponent = new VuScriptComponent(this));

		ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuDisallowPauseMenuEntity, Disallow);
		ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuDisallowPauseMenuEntity, Allow);
	}
	virtual void onGameRelease()
	{
		allow();
	}

private:
	VuRetVal Disallow(const VuParams &params) { disallow(); return VuRetVal(); }
	VuRetVal Allow(const VuParams &params) { allow(); return VuRetVal(); }

	void disallow()
	{
		if ( !mPauseDisallowed )
		{
			VuGameUtil::IF()->pushDisallowPauseMenu();
			mPauseDisallowed = true;
		}
	}

	void allow()
	{
		if ( mPauseDisallowed )
		{
			VuGameUtil::IF()->popDisallowPauseMenu();
			mPauseDisallowed = false;
		}
	}

	// components
	VuScriptComponent	*mpScriptComponent;

	bool				mPauseDisallowed;
};

IMPLEMENT_RTTI(VuDisallowPauseMenuEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuDisallowPauseMenuEntity);


//*****************************************************************************
// VuUnlockedStageEntity
//*****************************************************************************
class VuUnlockedStageEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuUnlockedStageEntity();

private:
	VuRetVal			Trigger(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuUnlockedStageEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuUnlockedStageEntity);


//*****************************************************************************
VuUnlockedStageEntity::VuUnlockedStageEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuUnlockedStageEntity, Trigger);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, Stage0);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, Stage1);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, Stage2);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, Stage3);
}

//*****************************************************************************
VuRetVal VuUnlockedStageEntity::Trigger(const VuParams &params)
{
	char strPlug[16];
	VU_SPRINTF(strPlug, sizeof(strPlug), "Stage%d", VuClamp(VuGameManager::IF()->getUnlockedStage(), 0, 3));
	mpScriptComponent->getPlug(strPlug)->execute();

	return VuRetVal();
}


//*****************************************************************************
//  VuCheckCurrencyEntity
//*****************************************************************************
class VuCheckCurrencyEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuCheckCurrencyEntity();

private:
	VuRetVal	GetSC(const VuParams &params) { return VuRetVal(VuGameManager::IF()->getCurrency(VuGameManager::CT_STANDARD)); }
	VuRetVal	GetPC(const VuParams &params) { return VuRetVal(VuGameManager::IF()->getCurrency(VuGameManager::CT_PREMIUM)); }

	// components
	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuCheckCurrencyEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuCheckCurrencyEntity);

//*****************************************************************************
VuCheckCurrencyEntity::VuCheckCurrencyEntity()
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	// scripting
	ADD_SCRIPT_INPUT(mpScriptComponent, VuCheckCurrencyEntity, GetSC, VuRetVal::Int, VuParamDecl());
	ADD_SCRIPT_INPUT(mpScriptComponent, VuCheckCurrencyEntity, GetPC, VuRetVal::Int, VuParamDecl());
}


//*****************************************************************************
// VuBossFilterEntity
//*****************************************************************************
class VuBossFilterEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuBossFilterEntity();

private:
	VuRetVal			In(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuBossFilterEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuBossFilterEntity);

//*****************************************************************************
VuBossFilterEntity::VuBossFilterEntity()
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this, 100));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuBossFilterEntity, In);

	const VuGameUtil::Names &driverNames = VuGameUtil::IF()->getDriverNames();
	for (VUUINT i = 0; i < driverNames.size(); i++)
		mpScriptComponent->addPlug(new VuScriptOutputPlug(driverNames[i].c_str()));
}

//*****************************************************************************
VuRetVal VuBossFilterEntity::In(const VuParams &params)
{
	const char *bossName = VuGameUtil::IF()->getSeriesBossName(VuGameUtil::IF()->getSeriesName());
	if ( VuScriptPlug *pPlug = mpScriptComponent->getPlug(bossName) )
		pPlug->execute(params);

	return VuRetVal();
}


//*****************************************************************************
// VuCurCarFilterEntity
//*****************************************************************************
class VuCurCarFilterEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuCurCarFilterEntity();

private:
	VuRetVal			In(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;

	// properties
	int					mViewport;
};

IMPLEMENT_RTTI(VuCurCarFilterEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuCurCarFilterEntity);

//*****************************************************************************
VuCurCarFilterEntity::VuCurCarFilterEntity()
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this, 100));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuCurCarFilterEntity, In);

	const VuGameUtil::Names &carNames = VuGameUtil::IF()->getCarNames();
	for (VUUINT i = 0; i < carNames.size(); i++)
		mpScriptComponent->addPlug(new VuScriptOutputPlug(carNames[i].c_str()));
}

//*****************************************************************************
VuRetVal VuCurCarFilterEntity::In(const VuParams &params)
{
	const std::string &carName = VuGameManager::IF()->getCurCarName();
	if ( VuScriptPlug *pPlug = mpScriptComponent->getPlug(carName) )
		pPlug->execute(params);

	return VuRetVal();
}


//*****************************************************************************
// VuIsConfirmedPirateEntity
//*****************************************************************************
class VuIsConfirmedPirateEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuIsConfirmedPirateEntity();

private:
	VuRetVal			Trigger(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuIsConfirmedPirateEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuIsConfirmedPirateEntity);


//*****************************************************************************
VuIsConfirmedPirateEntity::VuIsConfirmedPirateEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this, 100));

	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuIsConfirmedPirateEntity, Trigger);
	ADD_SCRIPT_OUTPUT(mpScriptComponent, True, VuRetVal::Void, VuParamDecl());
	ADD_SCRIPT_OUTPUT(mpScriptComponent, False, VuRetVal::Void, VuParamDecl());
}

//*****************************************************************************
VuRetVal VuIsConfirmedPirateEntity::Trigger(const VuParams &params)
{
	if ( VuGameManager::IF()->isConfirmedPirate() )
		mpScriptComponent->getPlug("True")->execute(params);
	else
		mpScriptComponent->getPlug("False")->execute(params);

	return VuRetVal();
}




























//*****************************************************************************
// VuIsSpecialSeries
//*****************************************************************************
class VuIsSpecialSeriesEventEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuIsSpecialSeriesEventEntity();

private:
	VuRetVal			Trigger(const VuParams &params);

	std::string			mSeriesEvent;

	// components
	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuIsSpecialSeriesEventEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuIsSpecialSeriesEventEntity);


//*****************************************************************************
VuIsSpecialSeriesEventEntity::VuIsSpecialSeriesEventEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	addProperty(new VuStringProperty("SeriesEvent", mSeriesEvent));

	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuIsSpecialSeriesEventEntity, Trigger);
	ADD_SCRIPT_OUTPUT(mpScriptComponent, Yes, VuRetVal::Void, VuParamDecl());
	ADD_SCRIPT_OUTPUT(mpScriptComponent, No, VuRetVal::Void, VuParamDecl());
}

//*****************************************************************************
VuRetVal VuIsSpecialSeriesEventEntity::Trigger(const VuParams &params)
{
	// modify by xlm,[/2014/12/10 ] 
	//原版
	/*
	if ( VuBillingManager::IF()->isUnlocked("DoubleCoins") )
	mpScriptComponent->getPlug("Yes")->execute(params);
	else
	mpScriptComponent->getPlug("No")->execute(params);*/

	//去掉金币双倍

	const std::string strSeriesEvent = VuGameUtil::IF()->getEventName();

	bool bRet = VuBillingManager::IF()->isOwned("StarterPackEx");
	if (strSeriesEvent == mSeriesEvent && !bRet)
	{
		mpScriptComponent->getPlug("Yes")->execute(params);
	}
	else
		mpScriptComponent->getPlug("No")->execute(params);
	return VuRetVal();
}


//*****************************************************************************
// VuIsBossBeatenEntity
//*****************************************************************************
class VuIsBossBeatenEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuIsBossBeatenEntity();

private:
	VuRetVal			In(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;

	// properties
	std::string			mBossName;
};

IMPLEMENT_RTTI(VuIsBossBeatenEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuIsBossBeatenEntity);

//*****************************************************************************
VuIsBossBeatenEntity::VuIsBossBeatenEntity()
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this, 100));

	// properties
	addProperty(new VuConstStringEnumProperty("Boss Name", mBossName, VuGameUtil::IF()->getDriverNames()));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuIsBossBeatenEntity, In);

	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, Yes);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, No);
}

//*****************************************************************************
VuRetVal VuIsBossBeatenEntity::In(const VuParams &params)
{
	if ( VuGameManager::IF()->getDriverBeaten(mBossName) )
		mpScriptComponent->getPlug("Yes")->execute();
	else
		mpScriptComponent->getPlug("No")->execute();

	return VuRetVal();
}
//*****************************************************************************
// IsHaveSpecialCarCheck
// by chao.zou(Chukie)
//*****************************************************************************
class VuIsHaveSpecialCarCheckEntity :public VuEntity
{
	DECLARE_RTTI
public:
	VuIsHaveSpecialCarCheckEntity();
private:
	VuRetVal			doCheck(const VuParams &params);
	std::string mCarEvent;

	// components
	VuScriptComponent	*mpScriptComponent;
};
IMPLEMENT_RTTI(VuIsHaveSpecialCarCheckEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuIsHaveSpecialCarCheckEntity);
//*****************************************************************************
VuIsHaveSpecialCarCheckEntity::VuIsHaveSpecialCarCheckEntity()
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	// scripting
	ADD_SCRIPT_INPUT(mpScriptComponent, VuIsHaveSpecialCarCheckEntity, doCheck, VuRetVal::Bool, VuParamDecl());

	addProperty(new VuStringProperty("condition", mCarEvent));
}

VuRetVal VuIsHaveSpecialCarCheckEntity::doCheck(const VuParams &params)
{
	bool bRet = false;
	bRet = VuGameManager::IF()->isCarOwned(mCarEvent);

	return VuRetVal(bRet);
}

//*****************************************************************************
// VuIsSeriesStage
// by chao.zou(Chukie)
//*****************************************************************************
class VuIsSeriesStageEventEntity : public VuEntity
{
	DECLARE_RTTI
public:
	VuIsSeriesStageEventEntity();
private:
	VuRetVal			Trigger(const VuParams &params);

	std::string			mSeriesEvent;
	// components
	VuScriptComponent	*mpScriptComponent;
};
IMPLEMENT_RTTI(VuIsSeriesStageEventEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuIsSeriesStageEventEntity);
//*****************************************************************************
VuIsSeriesStageEventEntity::VuIsSeriesStageEventEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	addProperty(new VuStringProperty("SeriesEvent", mSeriesEvent));

	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuIsSeriesStageEventEntity, Trigger);
	ADD_SCRIPT_OUTPUT(mpScriptComponent, Yes, VuRetVal::Void, VuParamDecl());
	ADD_SCRIPT_OUTPUT(mpScriptComponent, No, VuRetVal::Void, VuParamDecl());
}
//*****************************************************************************
VuRetVal VuIsSeriesStageEventEntity::Trigger(const VuParams &params)
{
	const std::string strSeriesEvent = VuGameUtil::IF()->getEventName();
	int nIndex = strSeriesEvent.find(mSeriesEvent);
	if (nIndex == std::string::npos)
	{
		mpScriptComponent->getPlug("No")->execute(params);
	}
	else
	{
		mpScriptComponent->getPlug("Yes")->execute(params);
	}

	return VuRetVal();
}
//*****************************************************************************


//*****************************************************************************
// VuIsNo3StarNoReward
// by chao.zou(Chukie)
//*****************************************************************************
class VuIsNo3StarNoRewardSeriesEventEntity :public VuEntity
{
	DECLARE_RTTI
public:
	VuIsNo3StarNoRewardSeriesEventEntity();
private:
	VuRetVal			Trigger(const VuParams &params);

	std::string			mSeriesEvent;
	// components
	VuScriptComponent	*mpScriptComponent;
};
IMPLEMENT_RTTI(VuIsNo3StarNoRewardSeriesEventEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuIsNo3StarNoRewardSeriesEventEntity);
//*****************************************************************************
VuIsNo3StarNoRewardSeriesEventEntity::VuIsNo3StarNoRewardSeriesEventEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	addProperty(new VuStringProperty("SeriesEvent", mSeriesEvent));

	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuIsNo3StarNoRewardSeriesEventEntity, Trigger);
	ADD_SCRIPT_OUTPUT(mpScriptComponent, Yes, VuRetVal::Void, VuParamDecl());
	ADD_SCRIPT_OUTPUT(mpScriptComponent, No, VuRetVal::Void, VuParamDecl());
}
//*****************************************************************************
VuRetVal VuIsNo3StarNoRewardSeriesEventEntity::Trigger(const VuParams &params)
{
	const std::string strSeriesEvent = VuGameUtil::IF()->getEventName();
	int nIndex = strSeriesEvent.find(mSeriesEvent);
	if (nIndex == std::string::npos)
	{
		mpScriptComponent->getPlug("No")->execute(params);
		return VuRetVal();
	}

	int place = 0;

	if (VuCarManager::IF()->getLocalHumanCarCount())
	{
		VuCarEntity *pCar = VuCarManager::IF()->getLocalHumanCar(0);
		place = pCar->getStats().mPlace;
	}
	
	if (place > 1)
	{
		mpScriptComponent->getPlug("Yes")->execute(params);
	}
	else
	{
		mpScriptComponent->getPlug("No")->execute(params);
	}

	return VuRetVal();
}
//*****************************************************************************



//*****************************************************************************
// VuIsSpecialSeries
//*****************************************************************************
class VuIsNoStarSeriesEventEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuIsNoStarSeriesEventEntity();

private:
	VuRetVal			Trigger(const VuParams &params);

	std::string			mSeriesEvent;

	std::string			mCarName;  //获赠汽车的名字

	// components
	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuIsNoStarSeriesEventEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuIsNoStarSeriesEventEntity);


//*****************************************************************************
VuIsNoStarSeriesEventEntity::VuIsNoStarSeriesEventEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	addProperty(new VuStringProperty("SeriesEvent", mSeriesEvent));
	addProperty(new VuStringProperty("CarName", mCarName));


	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuIsNoStarSeriesEventEntity, Trigger);
	ADD_SCRIPT_OUTPUT(mpScriptComponent, Yes, VuRetVal::Void, VuParamDecl());
	ADD_SCRIPT_OUTPUT(mpScriptComponent, No, VuRetVal::Void, VuParamDecl());
}

//*****************************************************************************
VuRetVal VuIsNoStarSeriesEventEntity::Trigger(const VuParams &params)
{
	VuCarEntity* pCarEntity = VuCarManager::IF()->getCurrLocalHumanCar();
	if (!pCarEntity)
	{
		mpScriptComponent->getPlug("No")->execute(params);
		return VuRetVal();
	}
		

	const std::string strSeriesEvent = VuGameUtil::IF()->getEventName();
	int nIndex = strSeriesEvent.find(mSeriesEvent);
	if (nIndex == std::string::npos)
	{
		mpScriptComponent->getPlug("No")->execute(params);
		return VuRetVal();
	}

	const std::string strEventType = VuGameUtil::IF()->getGameType();
	if (strEventType == "BossBattle")
	{
		mpScriptComponent->getPlug("No")->execute(params);
		return VuRetVal();
	}
	

	bool bOwnedCar = VuGameManager::IF()->isCarOwned(mCarName);


	int place = 0;

	if (VuCarManager::IF()->getLocalHumanCarCount())
	{
		VuCarEntity *pCar = VuCarManager::IF()->getLocalHumanCar(0);
		place = pCar->getStats().mPlace;
	}

	


	if (bOwnedCar || place < 4)
		mpScriptComponent->getPlug("No")->execute(params);
	else
	{

		//int nRandNumber = rand() % 2 + 0;
		//int nRandNumber = VuRand::global().range(0, 2);
		//if (nRandNumber == 1)
		{
			mpScriptComponent->getPlug("Yes")->execute(params);
		}
	}

/*
	int nPlace = pCarEntity->getPlace();
	if (nPlace >= 4 && !bOwnedCar)
		mpScriptComponent->getPlug("Yes")->execute(params);
	else
		mpScriptComponent->getPlug("No")->execute(params);*/
	return VuRetVal();
}

//*****************************************************************************
// VuIsSpecialSeries
//*****************************************************************************
class VuIsNoStarSpecialSeriesEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuIsNoStarSpecialSeriesEntity();

private:
	VuRetVal			Trigger(const VuParams &params);

	std::string			mSeriesEvent;



	// components
	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuIsNoStarSpecialSeriesEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuIsNoStarSpecialSeriesEntity);


//*****************************************************************************
VuIsNoStarSpecialSeriesEntity::VuIsNoStarSpecialSeriesEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	addProperty(new VuStringProperty("SeriesEvent", mSeriesEvent));



	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuIsNoStarSpecialSeriesEntity, Trigger);
	ADD_SCRIPT_OUTPUT(mpScriptComponent, Yes, VuRetVal::Void, VuParamDecl());
	ADD_SCRIPT_OUTPUT(mpScriptComponent, No, VuRetVal::Void, VuParamDecl());
}

//*****************************************************************************
VuRetVal VuIsNoStarSpecialSeriesEntity::Trigger(const VuParams &params)
{
	VuCarEntity* pCarEntity = VuCarManager::IF()->getCurrLocalHumanCar();
	if (!pCarEntity)
	{
		mpScriptComponent->getPlug("No")->execute(params);
		return VuRetVal();
	}
		

	bool bHit = false;
	const std::string strSeriesEvent = VuGameUtil::IF()->getEventName();
	if (strcmp(strSeriesEvent.c_str(), mSeriesEvent.c_str()) == 0)  //相同的管卡
	{
		int place = 0;

		if (VuCarManager::IF()->getLocalHumanCarCount())
		{
			VuCarEntity *pCar = VuCarManager::IF()->getLocalHumanCar(0);
			place = pCar->getStats().mPlace;
		}

		if (place >= 4)
			bHit = true;

	}

	if (bHit)
	{
		mpScriptComponent->getPlug("Yes")->execute(params);
	}
	else
	{
		mpScriptComponent->getPlug("No")->execute(params);
	}
	

	return VuRetVal();
}











//*****************************************************************************
// 是不是rmb用户
//*****************************************************************************
class VuBillingCheckEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuBillingCheckEntity();

protected:
	/*!
	* \author: xlm 2014/11/29
	* \brief: is a game coin item or rmb item
	*/
	VuRetVal			doCheck(const VuParams &params); //return a bool value to the VuIfEntity
	VuRetVal			doIsHaveCar(const VuParams &params);
	VuRetVal			setBillingMessage(const VuParams &params);
	VuRetVal			setBillingWeixin(const VuParams& params);
	VuRetVal			setCurrentClickItemNames(const VuParams &params);
	VuRetVal			isBigPayment(const VuParams &params);
	std::string			mCheckCondition;

	// components
	VuScriptComponent	*mpScriptComponent;
};


IMPLEMENT_RTTI(VuBillingCheckEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuBillingCheckEntity);


//*****************************************************************************
VuBillingCheckEntity::VuBillingCheckEntity()
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	// scripting
	ADD_SCRIPT_INPUT(mpScriptComponent, VuBillingCheckEntity, doCheck, VuRetVal::Bool, VuParamDecl());

	ADD_SCRIPT_INPUT(mpScriptComponent, VuBillingCheckEntity, doIsHaveCar, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuBillingCheckEntity, setCurrentClickItemNames);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuBillingCheckEntity, setBillingMessage);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuBillingCheckEntity, setBillingWeixin);
	ADD_SCRIPT_INPUT(mpScriptComponent, VuBillingCheckEntity, isBigPayment, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_OUTPUT(mpScriptComponent, Triggle, VuRetVal::Void, VuParamDecl());
	addProperty(new VuStringProperty("condition", mCheckCondition));

}

VuRetVal VuBillingCheckEntity::isBigPayment(const VuParams &params)
{
	bool result = false;
	result = VuBillingManager::IF()->isCurrentBigPayment();
	return VuRetVal(result);
}

//*****************************************************************************
VuRetVal VuBillingCheckEntity::doCheck(const VuParams &params)
{
	bool bRet = false;
	bRet = VuBillingManager::IF()->isOwned(mCheckCondition.c_str());
	
	return VuRetVal(bRet);

}
//*****************************************************************************
VuRetVal VuBillingCheckEntity::doIsHaveCar(const VuParams &params)
{
	bool isOwned = VuGameManager::IF()->getCar(mCheckCondition.c_str()).mIsOwned;

	return VuRetVal(isOwned);

}
//*****************************************************************************

VuRetVal	VuBillingCheckEntity::setBillingMessage(const VuParams &params)
{
	VuBillingManager::IF()->setBillingType(VuBillingManager::LOCAL_MESSAGE);
	return VuRetVal();
}
//*****************************************************************************
VuRetVal	VuBillingCheckEntity::setBillingWeixin(const VuParams& params)
{
	VuBillingManager::IF()->setBillingType(VuBillingManager::WEIXIN);
	return VuRetVal();
}

VuRetVal	VuBillingCheckEntity::setCurrentClickItemNames(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	std::string value = accessor.getString();
	VuBillingManager::IF()->setCurrentClickItem(value);
	mpScriptComponent->getPlug("Triggle")->execute(params);
	return VuRetVal();
}


//*****************************************************************************
// VuIsShowImmButtonTipEntity
//*****************************************************************************

class VuIsShowImmButtonTipEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuIsShowImmButtonTipEntity();

private:

	VuRetVal			doCheck(const VuParams &params); //return a bool value to the VuIfEntity


	//VuRetVal			Trigger(const VuParams &params);

	std::string			mDialogType;
	// components
	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuIsShowImmButtonTipEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuIsShowImmButtonTipEntity);


// *****************************************************************************
VuIsShowImmButtonTipEntity::VuIsShowImmButtonTipEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	addProperty(new VuDBEntryProperty("Type", mDialogType, "DialogDB"));


	ADD_SCRIPT_INPUT(mpScriptComponent, VuIsShowImmButtonTipEntity, doCheck, VuRetVal::Bool, VuParamDecl());

	/*ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuIsShowImmButtonTipEntity, Trigger);
	ADD_SCRIPT_OUTPUT(mpScriptComponent, Yes, VuRetVal::Void, VuParamDecl());
	ADD_SCRIPT_OUTPUT(mpScriptComponent, No, VuRetVal::Void, VuParamDecl());*/
}

// *****************************************************************************

//*****************************************************************************
VuRetVal VuIsShowImmButtonTipEntity::doCheck(const VuParams &params)
{
/*
	bool bRet = false;
	bRet = VuBillingManager::IF()->isOwned(mCheckCondition.c_str());*/

	std::map<std::string, int> items = VuGameManager::IF()->getGiftDialogButtonStates();
	
	//切换领取
	std::vector<std::string> targetItems;
	for (std::map<std::string,int>::iterator iter = items.begin(); iter != items.end(); iter++)
	{
		if (iter->second == 2)
		{
			targetItems.push_back(iter->first);
		}	
	}

	bool bShow = false;
	for (int i = 0; i < targetItems.size(); i++)
	{
		//int posion = projectAssetStr.find(deleteDialogs[i]);
		if (mDialogType.find(targetItems[i]) != std::string::npos)
		{
			bShow = true;
			break;
		}
	}

	//mDialogType
	//if (VuGameManager::IF())
	//{
	//	bShow = VuGameManager::IF()->showImmediateGetButtonTip();
	//}

	return VuRetVal(bShow);

}

//*****************************************************************************
// VuCanConnectServerEntity
//*****************************************************************************
class VuCanConnectServerEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuCanConnectServerEntity();

private:
	VuRetVal			doCheck(const VuParams &params); //return a bool value to the VuIfEntity

	// components
	VuScriptComponent	*mpScriptComponent;
};
IMPLEMENT_RTTI(VuCanConnectServerEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuCanConnectServerEntity);


// *****************************************************************************
VuCanConnectServerEntity::VuCanConnectServerEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	ADD_SCRIPT_INPUT(mpScriptComponent, VuCanConnectServerEntity, doCheck, VuRetVal::Bool, VuParamDecl());
}

//*****************************************************************************
VuRetVal VuCanConnectServerEntity::doCheck(const VuParams &params)
{
	bool bCan = false;
	do 
	{
		VUUINT64 uNowServerTime = 0;
		uNowServerTime = VuGameManager::IF()->getCurrentServerTime();
		if (uNowServerTime != 0)
		{
			bCan = true;
		}
	} while (0);
	return VuRetVal(bCan);
}

//*****************************************************************************
// VuCanCarChampChallengeEntity
//*****************************************************************************
class VuCanCarChampChallengeEntity : public VuEntity
{
	DECLARE_RTTI
public:
	VuCanCarChampChallengeEntity();
private:
	VuRetVal			doCheck(const VuParams &params); //return a bool value to the VuIfEntity
	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuCanCarChampChallengeEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuCanCarChampChallengeEntity);
//*****************************************************************************
VuCanCarChampChallengeEntity::VuCanCarChampChallengeEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this));


	ADD_SCRIPT_INPUT(mpScriptComponent, VuCanCarChampChallengeEntity, doCheck, VuRetVal::Bool, VuParamDecl());
}
//*****************************************************************************
VuRetVal	VuCanCarChampChallengeEntity::doCheck(const VuParams &params)
{
	bool bcan = false;

	VUINT64 curLocalTime = VuTimeUtil::calcSecondsSince2000();
	VUINT64 lastCarChampChallengeTime = VuGameManager::IF()->getLastCarChampChallengeTime();

	do 
	{
		if (lastCarChampChallengeTime == 0 && curLocalTime != 0)
		{
			bcan = true;
			break;
		}
		if (IsSameDay(curLocalTime, lastCarChampChallengeTime))
		{
			bcan = false;
		}
		else
		{
			bcan = true;
		}
	} while (0);
	
	return VuRetVal(bcan);
}
//*****************************************************************************
// VuCanReceiveLoginRewardEntity
//*****************************************************************************

class VuCanReceiveLoginRewardEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuCanReceiveLoginRewardEntity();

private:
	VuRetVal			doCheck(const VuParams &params); //return a bool value to the VuIfEntity
	//VuRetVal			Trigger(const VuParams &params);

	//std::string			mSeriesEvent;
	int					mReceiveDays;



	// components
	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuCanReceiveLoginRewardEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuCanReceiveLoginRewardEntity);


// *****************************************************************************
VuCanReceiveLoginRewardEntity::VuCanReceiveLoginRewardEntity() : mReceiveDays(1)
{
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	//addProperty(new VuStringProperty("SeriesEvent", mSeriesEvent));
	addProperty(new VuIntProperty("Days", mReceiveDays));


	ADD_SCRIPT_INPUT(mpScriptComponent, VuCanReceiveLoginRewardEntity, doCheck, VuRetVal::Bool, VuParamDecl());


/*
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuCanReceiveLoginRewardEntity, Trigger);
	ADD_SCRIPT_OUTPUT(mpScriptComponent, Yes, VuRetVal::Void, VuParamDecl());
	ADD_SCRIPT_OUTPUT(mpScriptComponent, No, VuRetVal::Void, VuParamDecl());*/
}



//*****************************************************************************
VuRetVal VuCanReceiveLoginRewardEntity::doCheck(const VuParams &params)
{
	/*
	bool bRet = false;
	bRet = VuBillingManager::IF()->isOwned(mCheckCondition.c_str());*/

	bool bCan = false;
	if (VuGameManager::IF())
	{

		int nAvailableDay = VuGameManager::IF()->getAvailableLoginDay();
		if (nAvailableDay == 0)
		{
			bCan = false;
			return VuRetVal(bCan);
		}
		else
		{
			return VuRetVal(true);
		}

		//bShow = VuGameManager::IF()->showImmediateGetButtonTip();
		int nHasReceivedDay = VuGameManager::IF()->getCurrReceivedDays();
		VUUINT64 uNowServerTime = VuGameManager::IF()->getCurrentServerTime();
		VUUINT64 uLastServerTime = VuGameManager::IF()->getLastServerTime();

		if (uLastServerTime == 0 && uNowServerTime != 0)
		{
			bCan = true;
			return VuRetVal(bCan);
		}
		if (mReceiveDays != (nHasReceivedDay + 1))
		{
			VUPRINT("mReceiveDays != (nHasReceivedDay + 1)");

			bCan = false;
			return VuRetVal(bCan);

		}

		//比对是否同一天
		if (uNowServerTime == 0)
		{
			VUPRINT("uNowServerTime == 0");

			bCan = false;
			//mpScriptComponent->getPlug("No")->execute(params);
			return VuRetVal(bCan);

		}
		else if (IsSameDay(uNowServerTime, uLastServerTime))
		{
			VUPRINT("IsSameDay(uNowServerTime, uLastServerTime)");

			bCan = false;
			//mpScriptComponent->getPlug("No")->execute(params);
			return VuRetVal(bCan);


		}
		else
		{
			bCan = true;
			//mpScriptComponent->getPlug("Yes")->execute(params);
			return VuRetVal(bCan);
		}
	}
	else
		VUPRINT("VuGameManager::IF() is null");

	return VuRetVal(bCan);

}













//show 15days login dlg


//*****************************************************************************
// VuIsShowImmButtonTipEntity
//*****************************************************************************

class VuIsShow15DayLoginEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuIsShow15DayLoginEntity();

private:

	VuRetVal			doCheck(const VuParams &params); //return a bool value to the VuIfEntity


	//VuRetVal			Trigger(const VuParams &params);

	//std::string			mSeriesEvent;



	// components
	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuIsShow15DayLoginEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuIsShow15DayLoginEntity);


// *****************************************************************************
VuIsShow15DayLoginEntity::VuIsShow15DayLoginEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	//addProperty(new VuStringProperty("SeriesEvent", mSeriesEvent));



	ADD_SCRIPT_INPUT(mpScriptComponent, VuIsShow15DayLoginEntity, doCheck, VuRetVal::Bool, VuParamDecl());

	/*ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuIsShowImmButtonTipEntity, Trigger);
	ADD_SCRIPT_OUTPUT(mpScriptComponent, Yes, VuRetVal::Void, VuParamDecl());
	ADD_SCRIPT_OUTPUT(mpScriptComponent, No, VuRetVal::Void, VuParamDecl());*/
}

// *****************************************************************************

//*****************************************************************************
VuRetVal VuIsShow15DayLoginEntity::doCheck(const VuParams &params)
{
	/*
	bool bRet = false;
	bRet = VuBillingManager::IF()->isOwned(mCheckCondition.c_str());*/

	bool bShow = false;
	if (VuGameManager::IF())
	{
		int nDays = VuGameManager::IF()->getCurrReceivedDays();
		if (nDays >= 15)
			bShow = false;
		else
			bShow = true;
		//bShow = VuGameManager::IF()->showImmediateGetButtonTip();
	}

	return VuRetVal(bShow);

}

/*******************************
* 是否是弹框版本；
**/

class VuIsPopDialogVersionEntity :public VuEntity
{
	DECLARE_RTTI
public:
	VuIsPopDialogVersionEntity();
private:
	VuRetVal IsPopDialogVersion(const VuParams& params);
	// components
	VuScriptComponent	*mpScriptComponent;
	bool m_isPopDialogVersion;
};
IMPLEMENT_RTTI(VuIsPopDialogVersionEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuIsPopDialogVersionEntity);

VuIsPopDialogVersionEntity::VuIsPopDialogVersionEntity()
{
	//当前是无弹框版本
	m_isPopDialogVersion = IS_POPDIALOG_VERSION;
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	ADD_SCRIPT_INPUT(mpScriptComponent, VuIsPopDialogVersionEntity, IsPopDialogVersion, VuRetVal::Bool, VuParamDecl());
}

VuRetVal VuIsPopDialogVersionEntity::IsPopDialogVersion(const VuParams& params)
{
	bool result;
	result = m_isPopDialogVersion;
	return VuRetVal(result);
}

/*******************************
* 是否是大额支付；
**/
class VuIsBigPaymentEntity :public VuEntity
{
	DECLARE_RTTI
public:
	VuIsBigPaymentEntity();
private:
	VuRetVal IsBigPayment(const VuParams& params);
	// components
	VuScriptComponent	*mpScriptComponent;
	bool m_isBigPayment;
};

IMPLEMENT_RTTI(VuIsBigPaymentEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuIsBigPaymentEntity);

VuIsBigPaymentEntity::VuIsBigPaymentEntity()
{
	//当前是小额版本
	m_isBigPayment = IS_BIGPAYMENT;
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	ADD_SCRIPT_INPUT(mpScriptComponent, VuIsBigPaymentEntity, IsBigPayment, VuRetVal::Bool, VuParamDecl());
}

VuRetVal VuIsBigPaymentEntity::IsBigPayment(const VuParams& params)
{
	bool result;
	result = m_isBigPayment;
	return VuRetVal(result);
}

/************************************************************************/
/* 检查TunesUp的数量                                                                     */
/************************************************************************/
class VuCheckTunesUpNumEntity :public VuEntity
{
	DECLARE_RTTI
public:
	VuCheckTunesUpNumEntity();

private:
	VuRetVal isCanUseTunesUp(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuCheckTunesUpNumEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuCheckTunesUpNumEntity);


VuCheckTunesUpNumEntity::VuCheckTunesUpNumEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	ADD_SCRIPT_INPUT(mpScriptComponent, VuCheckTunesUpNumEntity, isCanUseTunesUp, VuRetVal::Bool, VuParamDecl());
}


VuRetVal VuCheckTunesUpNumEntity::isCanUseTunesUp(const VuParams &params)
{
	int iVal = VuGameManager::IF()->getTuneUps();
	if (iVal > 0)
		return VuRetVal(true);
	else
		return VuRetVal(false);
}


/*******************************
* 是否可以 直接购买满级的车辆；
**/

class VuIsCanFullLevelCarEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuIsCanFullLevelCarEntity();

private:

	//VuRetVal			doCheckSelectedCar(const VuParams &params); //return a bool value to the VuIfEntity
	VuRetVal			isCanBuy(const VuParams & params);

	VuRetVal			doCheckmCar(const VuParams &params);

	VuRetVal			doSetCar(const VuParams &params);
	// components
	VuScriptComponent	*mpScriptComponent;

	std::string mCar;   
};

IMPLEMENT_RTTI(VuIsCanFullLevelCarEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuIsCanFullLevelCarEntity);


// *****************************************************************************
VuIsCanFullLevelCarEntity::VuIsCanFullLevelCarEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	addProperty(new VuStringProperty("CarName", mCar));

	ADD_SCRIPT_INPUT(mpScriptComponent, VuIsCanFullLevelCarEntity, isCanBuy, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT(mpScriptComponent, VuIsCanFullLevelCarEntity, doCheckmCar, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuIsCanFullLevelCarEntity, doSetCar);
}

// *****************************************************************************

//*****************************************************************************
VuRetVal VuIsCanFullLevelCarEntity::isCanBuy(const VuParams &params)
{
	std::vector<std::string> mFullLvlCarNames = VuGameManager::IF()->getFullLvlCarNames();
	bool bCanBuy = false;
	for (int i = 0; i < mFullLvlCarNames.size(); i++)
	{
		bool isOwned = VuGameManager::IF()->getCar(mFullLvlCarNames[i]).mIsOwned;
		if (isOwned == false)
		{
			bCanBuy = true;
			break;
		}
	}

	return VuRetVal(bCanBuy);
}

VuRetVal VuIsCanFullLevelCarEntity::doCheckmCar(const VuParams &params)
{
	int price = VuGameUtil::IF()->getFullLevelCarCurPrice(mCar);
	if (price > 0)
	{
		return VuRetVal(true);
	}
	else
	{
		return VuRetVal(false);
	}
}

VuRetVal		VuIsCanFullLevelCarEntity::doSetCar(const VuParams &params)
{
	const VuJsonContainer &eventData = VuProfileManager::IF()->dataRead()["eventData"];
	std::string car = VuGameManager::IF()->getPurchaseFullLvlCarName();
	if (car.size() > 0)
	{
		VuGameManager::IF()->setCurCar(car);
	}
	return VuRetVal();
}

//modify by zc   2015/10/28
class VuGiftDialogEntity :public VuEntity
{
	DECLARE_RTTI
public:
	VuGiftDialogEntity();
private:
	VuRetVal startGiftDialogPopUps(const VuParams &params);
	VuScriptComponent *mpScriptComponent;
};
IMPLEMENT_RTTI(VuGiftDialogEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuGiftDialogEntity);

VuGiftDialogEntity::VuGiftDialogEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this, 100));
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuGiftDialogEntity, startGiftDialogPopUps);
}

VuRetVal VuGiftDialogEntity::startGiftDialogPopUps(const VuParams &params)
{
	VuGiftDialogManager::IF()->popUpGiftDialog("Dialogs/Dialog_Tuhao_Pack");
	return VuRetVal();
}

// modify by liudan 关卡游戏开始前的礼包系统

class VuDialogBeforeGameEntity :public VuEntity
{
	DECLARE_RTTI
public:
	VuDialogBeforeGameEntity();
private:
	VuRetVal startDialogBeforGame(const VuParams &params);
	//VuRetVal recordEnterGame(const VuParams &params);
	VuScriptComponent *mpScriptComponent;
};
IMPLEMENT_RTTI(VuDialogBeforeGameEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuDialogBeforeGameEntity);

VuDialogBeforeGameEntity::VuDialogBeforeGameEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this, 100));
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuDialogBeforeGameEntity, startDialogBeforGame);
}

VuRetVal VuDialogBeforeGameEntity::startDialogBeforGame(const VuParams &params)
{
	VuGiftDialogManager::IF()->recordDialogBeforeGame();
	VuGiftDialogManager::IF()->popUpDialogBeforGame();
	return VuRetVal();
}

//
class VuDialogInPlayerHubEntity :public VuEntity
{
	DECLARE_RTTI
public:
	VuDialogInPlayerHubEntity();
private:
	VuRetVal startDialogInPlayerHub(const VuParams &params);
	//VuRetVal recordEnterGame(const VuParams &params);
	VuScriptComponent *mpScriptComponent;
};
IMPLEMENT_RTTI(VuDialogInPlayerHubEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuDialogInPlayerHubEntity);

VuDialogInPlayerHubEntity::VuDialogInPlayerHubEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this, 100));
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuDialogInPlayerHubEntity, startDialogInPlayerHub);
}

VuRetVal VuDialogInPlayerHubEntity::startDialogInPlayerHub(const VuParams &params)
{

	VuGiftDialogManager::IF()->recordDialogInPlayerHub();
	VuGiftDialogManager::IF()->popUpDialogInPlayerHub();

	return VuRetVal();
}

//升级车辆界面
class VuDialogInUpgradeCarEntity :public VuEntity
{
	DECLARE_RTTI
public:
	VuDialogInUpgradeCarEntity();
private:
	VuRetVal startDialogInUpgradeCar(const VuParams &params);
	//VuRetVal recordEnterGame(const VuParams &params);
	VuScriptComponent *mpScriptComponent;
};
IMPLEMENT_RTTI(VuDialogInUpgradeCarEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuDialogInUpgradeCarEntity);

VuDialogInUpgradeCarEntity::VuDialogInUpgradeCarEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this, 100));
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuDialogInUpgradeCarEntity, startDialogInUpgradeCar);
}

VuRetVal VuDialogInUpgradeCarEntity::startDialogInUpgradeCar(const VuParams &params)
{
	
	VuGiftDialogManager::IF()->recordDialogInUpgradeCar();
	VuGiftDialogManager::IF()->popUpDialogInUpgradeCar();
	return VuRetVal();
}

//更换车手界面
class VuDialogInLoungeEntity :public VuEntity
{
	DECLARE_RTTI
public:
	VuDialogInLoungeEntity();
private:
	VuRetVal startDialogInLounge(const VuParams &params);
	//VuRetVal recordEnterGame(const VuParams &params);
	VuScriptComponent *mpScriptComponent;
};
IMPLEMENT_RTTI(VuDialogInLoungeEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuDialogInLoungeEntity);

VuDialogInLoungeEntity::VuDialogInLoungeEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this, 100));
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuDialogInLoungeEntity, startDialogInLounge);
}

VuRetVal VuDialogInLoungeEntity::startDialogInLounge(const VuParams &params)
{

	VuGiftDialogManager::IF()->recordDialogInLounge();
	VuGiftDialogManager::IF()->popUpDialogInLounge();

	return VuRetVal();
}

//展现车辆的界面！
class VuDialogInShowRoomEntity :public VuEntity
{
	DECLARE_RTTI
public:
	VuDialogInShowRoomEntity();
private:
	VuRetVal startDialogInShowRoom(const VuParams &params);
	//VuRetVal recordEnterGame(const VuParams &params);
	VuScriptComponent *mpScriptComponent;
};
IMPLEMENT_RTTI(VuDialogInShowRoomEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuDialogInShowRoomEntity);

VuDialogInShowRoomEntity::VuDialogInShowRoomEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this, 100));
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuDialogInShowRoomEntity, startDialogInShowRoom);
}

VuRetVal VuDialogInShowRoomEntity::startDialogInShowRoom(const VuParams &params)
{
	VuGiftDialogManager::IF()->recordDialogInShowRoom();
	VuGiftDialogManager::IF()->popUpDialogInShowRoom();
	return VuRetVal();
}

//modify by liudan   2015/6/11  16:32
class VuBossDialogStausEntity :public VuEntity
{
	DECLARE_RTTI
public:
	VuBossDialogStausEntity();
private:
	VuRetVal getDialogStatus(const VuParams &params);
	VuRetVal setStatusTrue(const VuParams &params);
	VuRetVal setStatusFalse(const VuParams &params);
	VuScriptComponent *mpScriptComponent;

	bool mMialogStatus;
};

IMPLEMENT_RTTI(VuBossDialogStausEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuBossDialogStausEntity);


VuBossDialogStausEntity::VuBossDialogStausEntity()
{
	mMialogStatus = false;
	addComponent(mpScriptComponent = new VuScriptComponent(this, 100));
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuBossDialogStausEntity, setStatusFalse);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuBossDialogStausEntity, setStatusTrue);
	ADD_SCRIPT_INPUT(mpScriptComponent, VuBossDialogStausEntity, getDialogStatus, VuRetVal::Bool, VuParamDecl());
}

VuRetVal VuBossDialogStausEntity::getDialogStatus(const VuParams &params)
{
	return VuRetVal(mMialogStatus);
}


VuRetVal VuBossDialogStausEntity::setStatusTrue(const VuParams &params)
{
	mMialogStatus = true;
	return VuRetVal();
}
VuRetVal VuBossDialogStausEntity::setStatusFalse(const VuParams &params)
{
	mMialogStatus = false;
	return VuRetVal();
}

/************************************************************************/
/* 检测礼包是否打折                                                                     */
/************************************************************************/

class VuPackInfoEntity :public VuEntity
{
	DECLARE_RTTI
public:
	VuPackInfoEntity();
private:
	VuRetVal CheckDiscount(const VuParams &params);

	VuRetVal CheckLanXiangIsDiscount(const VuParams &params);

	VuScriptComponent *mpScriptComponent;

	VuRetVal TriggerLanXiang(const VuParams &params);
	VuRetVal TriggerSuperVip(const VuParams &params);
};

IMPLEMENT_RTTI(VuPackInfoEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuPackInfoEntity);

VuPackInfoEntity::VuPackInfoEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this));


	ADD_SCRIPT_INPUT(mpScriptComponent, VuPackInfoEntity, CheckLanXiangIsDiscount, VuRetVal::Bool, VuParamDecl());

	ADD_SCRIPT_INPUT(mpScriptComponent, VuPackInfoEntity, CheckDiscount, VuRetVal::Void, VuParamDecl());

	ADD_SCRIPT_INPUT(mpScriptComponent, VuPackInfoEntity, TriggerLanXiang, VuRetVal::Void, VuParamDecl());

	ADD_SCRIPT_INPUT(mpScriptComponent, VuPackInfoEntity, TriggerSuperVip, VuRetVal::Void, VuParamDecl());

	ADD_SCRIPT_OUTPUT(mpScriptComponent, ShowLanXiang, VuRetVal::Void, VuParamDecl());
	ADD_SCRIPT_OUTPUT(mpScriptComponent, ShowSuperVip, VuRetVal::Void, VuParamDecl());
	ADD_SCRIPT_OUTPUT(mpScriptComponent, ShowNothing, VuRetVal::Void, VuParamDecl());
}

VuRetVal VuPackInfoEntity::CheckDiscount(const VuParams &params)
{
	bool isLanXiangOK = VuPackManager::IF()->isPackOk(VuPackManager::Pack_LanXiang);
	bool isSuperVipOK = VuPackManager::IF()->isPackOk(VuPackManager::Pack_SuperVip);

	if (isLanXiangOK && isSuperVipOK)
	{
		mpScriptComponent->getPlug("ShowSuperVip")->execute(params);
	}
	else if (isLanXiangOK)
	{
		mpScriptComponent->getPlug("ShowLanXiang")->execute(params);
	}
	else if (isSuperVipOK)
	{
		mpScriptComponent->getPlug("ShowSuperVip")->execute(params);
	}
	else
	{
		mpScriptComponent->getPlug("ShowNothing")->execute(params);
	}

	return VuRetVal();
}

VuRetVal VuPackInfoEntity::CheckLanXiangIsDiscount(const VuParams &params)
{
	bool isLanXiangOK = VuPackManager::IF()->isPackOk(VuPackManager::Pack_LanXiang);
	return VuRetVal(isLanXiangOK);
}

VuRetVal VuPackInfoEntity::TriggerLanXiang(const VuParams &params)
{
	VuPackManager::IF()->triggerLanXiangPack();

	//if lanxiang ok
	if (VuPackManager::IF()->isPackOk(VuPackManager::Pack_LanXiang))
	{
		mpScriptComponent->getPlug("ShowLanXiang")->execute(params);
	}
	return VuRetVal();
}
VuRetVal VuPackInfoEntity::TriggerSuperVip(const VuParams &params)
{
	VuPackManager::IF()->triggerSuperVipPack();
	if (VuPackManager::IF()->isPackOk(VuPackManager::Pack_SuperVip))
	{
		mpScriptComponent->getPlug("ShowSuperVip")->execute(params);
	}
	return VuRetVal();
}

/************************************************************************/
/* 检测是否已经领取某天的登录奖励！                                                                     */
/************************************************************************/
class VuCheckLoginRewardEntity :public VuEntity
{
	DECLARE_RTTI
public:
	VuCheckLoginRewardEntity();

private:
	VuRetVal isHaveGotReward(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;
	int mDayNo;
};

IMPLEMENT_RTTI(VuCheckLoginRewardEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuCheckLoginRewardEntity);


VuCheckLoginRewardEntity::VuCheckLoginRewardEntity()
{
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	addProperty(new VuIntProperty("DayNo", mDayNo));
	ADD_SCRIPT_INPUT(mpScriptComponent, VuCheckLoginRewardEntity, isHaveGotReward, VuRetVal::Bool, VuParamDecl());
}


VuRetVal VuCheckLoginRewardEntity::isHaveGotReward(const VuParams &params)
{
	int iVal = VuGameManager::IF()->getCurrDayState(mDayNo);
	//1 表示已经领取  2 表示可领取  3表示未领取（不能领取）

	if (iVal == 1)
		return VuRetVal(true);
	else
		return VuRetVal(false);

}
