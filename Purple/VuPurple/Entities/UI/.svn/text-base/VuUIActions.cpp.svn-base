//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  UI action entities
// 
//*****************************************************************************

#include "VuPurple/Managers/VuCommunityManager.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Managers/VuAchievementManager.h"
#include "VuPurple/Managers/VuBillingManager.h"
#include "VuPurple/Managers/VuTipManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Properties/VuEnumProperty.h"
#include "VuEngine/Properties/VuAssetProperty.h"
#include "VuEngine/Properties/VuDBEntryProperty.h"
#include "VuEngine/Managers/VuProfileManager.h"
#include "VuEngine/Assets/VuProjectAsset.h"
#include "VuPurple/Managers/VuAnalyticsManager.h"


//*****************************************************************************
// UI Action base class
//*****************************************************************************
class VuUIAction : public VuEntity
{
	DECLARE_RTTI

public:
	VuUIAction()
	{
		addComponent(mpScriptComponent = new VuScriptComponent(this, 150));
		ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuUIAction, Trigger);
	}

protected:
	virtual VuRetVal Trigger(const VuParams &params) = 0;

	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuUIAction, VuEntity);


//*****************************************************************************
// StartActionGame action
//*****************************************************************************
class VuStartActionGameUIAction : public VuUIAction
{
	DECLARE_RTTI
public:
	VuStartActionGameUIAction();

private:
	VuRetVal	Trigger(const VuParams &params);

	// properties
	std::string	mType;
};

IMPLEMENT_RTTI(VuStartActionGameUIAction, VuUIAction);
IMPLEMENT_ENTITY_REGISTRATION(VuStartActionGameUIAction);

//*****************************************************************************
VuStartActionGameUIAction::VuStartActionGameUIAction():
	mType("AttractEvent")
{
	// properties
	static VuStaticStringEnumProperty::Choice sTypeChoices[] =
	{
		"AttractEvent",
		"DemoEvent",
		"SinglePlayerEvent",
		"TutorialDriveEvent",
		"TutorialPowerUpEvent",
		"TutorialRaceEvent",
		"DuelEvent",
		"CarChampEvent",
		VUNULL
	};
	addProperty(new VuStaticStringEnumProperty("Type", mType, sTypeChoices));
}

//*****************************************************************************
VuRetVal VuStartActionGameUIAction::Trigger(const VuParams &params)
{
	VuParams outParams;
	outParams.addString(mType.c_str());

	VuEventManager::IF()->broadcast("OnStartActionGame", outParams);

	const VuJsonContainer &eventData = VuGameUtil::IF()->dataRead()["EventData"];
	std::string eventNameValue;
	std::string seriousNameValue;
	eventData["EventName"].getValue(eventNameValue);
	eventData["SeriesName"].getValue(seriousNameValue);

	if (!eventNameValue.empty() && !seriousNameValue.empty())
		VuAnalyticsManager::IF()->logEvent("PlayGameIndex",(eventNameValue + seriousNameValue).c_str(), 0);

	if ( mType == "DuelEvent" )
	{
		char eventName[64];
		VU_SPRINTF(eventName, sizeof(eventName), "Duel_%s", VuGameUtil::IF()->getSelectedDriver().c_str());
		VuGameUtil::IF()->dataWrite()["EventData"]["EventName"].putValue(eventName);
	}

	return VuRetVal();
}

//*****************************************************************************
// EndActionGame action
//*****************************************************************************
class VuEndActionGameUIAction : public VuUIAction
{
	DECLARE_RTTI

public:
	VuEndActionGameUIAction();

private:
	VuRetVal Trigger(const VuParams &params);

	// properties
	std::string	mScreenStackName;
};
IMPLEMENT_RTTI(VuEndActionGameUIAction, VuUIAction);
IMPLEMENT_ENTITY_REGISTRATION(VuEndActionGameUIAction);

//*****************************************************************************
VuEndActionGameUIAction::VuEndActionGameUIAction():
	mScreenStackName("Title")
{
	// properties
	addProperty(new VuJsonObjectEnumProperty("Screen Stack", mScreenStackName, VuGameUtil::IF()->constantDB()["ScreenStacks"]));
}

//*****************************************************************************
VuRetVal VuEndActionGameUIAction::Trigger(const VuParams &params)
{
	VuGameUtil::IF()->dataWrite()["InitialScreenStack"].putValue(mScreenStackName);

	VuEventManager::IF()->broadcast("OnEndActionGame");

	return VuRetVal();
}


//*****************************************************************************
// SetDefaultPauseMenu action
//*****************************************************************************
class VuSetDefaultPauseMenuUIAction : public VuUIAction
{
	DECLARE_RTTI

public:
	VuSetDefaultPauseMenuUIAction() {
		addProperty(new VuAssetNameProperty(VuProjectAsset::msRTTI.mstrType, "Screen", mScreen));
	}

private:
	VuRetVal Trigger(const VuParams &params) {
		VuParams outParams;
		outParams.addString(mScreen.c_str());
		VuEventManager::IF()->broadcast("SetDefaultPauseMenu", outParams);
		return VuRetVal();
	}
	std::string	mScreen;
};

IMPLEMENT_RTTI(VuSetDefaultPauseMenuUIAction, VuUIAction);
IMPLEMENT_ENTITY_REGISTRATION(VuSetDefaultPauseMenuUIAction);


//*****************************************************************************
// PauseActionGame action
//*****************************************************************************
class VuPauseActionGameUIAction : public VuUIAction
{
	DECLARE_RTTI

public:
	VuPauseActionGameUIAction() : mFade(true) {
		addProperty(new VuAssetNameProperty(VuProjectAsset::msRTTI.mstrType, "Screen", mScreen));
		addProperty(new VuBoolProperty("Fade", mFade));
	}

private:
	VuRetVal Trigger(const VuParams &params) {
		VuParams outParams;
		outParams.addString(mScreen.c_str());
		outParams.addBool(mFade);
		VuEventManager::IF()->broadcast("OnPauseActionGame", outParams);
		return VuRetVal();
	}
	std::string	mScreen;
	bool		mFade;
};

IMPLEMENT_RTTI(VuPauseActionGameUIAction, VuUIAction);
IMPLEMENT_ENTITY_REGISTRATION(VuPauseActionGameUIAction);


//*****************************************************************************
// ResumeActionGame action
//*****************************************************************************
class VuResumeActionGameUIAction : public VuUIAction
{
	DECLARE_RTTI
private:
	VuRetVal	Trigger(const VuParams &params)	{ VuEventManager::IF()->broadcast("OnResumeActionGame"); return VuRetVal(); }
};

IMPLEMENT_RTTI(VuResumeActionGameUIAction, VuUIAction);
IMPLEMENT_ENTITY_REGISTRATION(VuResumeActionGameUIAction);

//*****************************************************************************
// RestartActionGame action
//*****************************************************************************
class VuRestartActionGameUIAction : public VuUIAction
{
	DECLARE_RTTI
private:
	VuRetVal	Trigger(const VuParams &params)	{ VuEventManager::IF()->broadcast("OnRestartActionGame"); return VuRetVal(); }
};

IMPLEMENT_RTTI(VuRestartActionGameUIAction, VuUIAction);
IMPLEMENT_ENTITY_REGISTRATION(VuRestartActionGameUIAction);


//*****************************************************************************
// StartSplitScreenGame action
//*****************************************************************************
class VuStartSplitScreenGameUIAction : public VuUIAction
{
	DECLARE_RTTI
private:
	VuRetVal	Trigger(const VuParams &params)	{ VuEventManager::IF()->broadcast("OnStartSplitScreenGame"); return VuRetVal(); }
};

IMPLEMENT_RTTI(VuStartSplitScreenGameUIAction, VuUIAction);
IMPLEMENT_ENTITY_REGISTRATION(VuStartSplitScreenGameUIAction);


//*****************************************************************************
// ShutdownApp action
//*****************************************************************************
class VuShutdownAppUIAction : public VuUIAction
{
	DECLARE_RTTI
private:
	VuRetVal	Trigger(const VuParams &params)	{ VuEventManager::IF()->broadcast("OnShutdownApp"); return VuRetVal(); }
};

IMPLEMENT_RTTI(VuShutdownAppUIAction, VuUIAction);
IMPLEMENT_ENTITY_REGISTRATION(VuShutdownAppUIAction);


//*****************************************************************************
// SetScreen action
//*****************************************************************************
class VuSetScreenUIAction : public VuUIAction
{
	DECLARE_RTTI
public:
	VuSetScreenUIAction();

private:
	VuRetVal	Trigger(const VuParams &params);

	std::string	mScreen;
};

IMPLEMENT_RTTI(VuSetScreenUIAction, VuUIAction);
IMPLEMENT_ENTITY_REGISTRATION(VuSetScreenUIAction);

//*****************************************************************************
VuSetScreenUIAction::VuSetScreenUIAction()
{
	addProperty(new VuAssetNameProperty(VuProjectAsset::msRTTI.mstrType, "Screen", mScreen));
}

//*****************************************************************************
VuRetVal VuSetScreenUIAction::Trigger(const VuParams &params)
{
	VuParams outParams;
	outParams.addString(mScreen.c_str());
	VuEventManager::IF()->broadcast("OnSetScreen", outParams);
	return VuRetVal();
}

//*****************************************************************************
// PushScreen action
//*****************************************************************************
class VuPushScreenUIAction : public VuUIAction
{
	DECLARE_RTTI
public:
	VuPushScreenUIAction();

private:
	VuRetVal	Trigger(const VuParams &params);

	std::string	mScreen;
};

IMPLEMENT_RTTI(VuPushScreenUIAction, VuUIAction);
IMPLEMENT_ENTITY_REGISTRATION(VuPushScreenUIAction);

//*****************************************************************************
VuPushScreenUIAction::VuPushScreenUIAction()
{
	addProperty(new VuAssetNameProperty(VuProjectAsset::msRTTI.mstrType, "Screen", mScreen));
}

//*****************************************************************************
VuRetVal VuPushScreenUIAction::Trigger(const VuParams &params)
{
	VuParams outParams;
	outParams.addString(mScreen.c_str());
	VuEventManager::IF()->broadcast("OnPushScreen", outParams);
	return VuRetVal();
}

//*****************************************************************************
// PopScreen action
//*****************************************************************************
class VuPopScreenUIAction : public VuUIAction
{
	DECLARE_RTTI
private:
	VuRetVal	Trigger(const VuParams &params)	{ VuEventManager::IF()->broadcast("OnPopScreen"); return VuRetVal(); }
};

IMPLEMENT_RTTI(VuPopScreenUIAction, VuUIAction);
IMPLEMENT_ENTITY_REGISTRATION(VuPopScreenUIAction);

//*****************************************************************************
// SetScreenStack action
//*****************************************************************************
class VuSetScreenStackUIAction : public VuUIAction
{
	DECLARE_RTTI

public:
	VuSetScreenStackUIAction();

private:
	VuRetVal Trigger(const VuParams &params);

	// properties
	std::string	mScreenStackName;
};
IMPLEMENT_RTTI(VuSetScreenStackUIAction, VuUIAction);
IMPLEMENT_ENTITY_REGISTRATION(VuSetScreenStackUIAction);

//*****************************************************************************
VuSetScreenStackUIAction::VuSetScreenStackUIAction():
	mScreenStackName("Title")
{
	// properties
	addProperty(new VuJsonObjectEnumProperty("Screen Stack", mScreenStackName, VuGameUtil::IF()->constantDB()["ScreenStacks"]));
}

//*****************************************************************************
VuRetVal VuSetScreenStackUIAction::Trigger(const VuParams &params)
{
	VuParams outParams;
	outParams.addString(mScreenStackName.c_str());
	VuEventManager::IF()->broadcast("OnSetScreenStack", outParams);

	return VuRetVal();
}

//*****************************************************************************
// NextScreen action
//*****************************************************************************
class VuNextScreenUIAction : public VuUIAction
{
	DECLARE_RTTI
private:
	VuRetVal	Trigger(const VuParams &params)	{ VuEventManager::IF()->broadcast("OnNextScreen"); return VuRetVal(); }
};

IMPLEMENT_RTTI(VuNextScreenUIAction, VuUIAction);
IMPLEMENT_ENTITY_REGISTRATION(VuNextScreenUIAction);

//*****************************************************************************
// PushNews action
//*****************************************************************************
class VuPushNewsScreenUIAction : public VuUIAction
{
	DECLARE_RTTI
private:
	VuRetVal	Trigger(const VuParams &params) { VuEventManager::IF()->broadcast("OnPushNewsScreen"); return VuRetVal(); }
};

IMPLEMENT_RTTI(VuPushNewsScreenUIAction, VuUIAction);
IMPLEMENT_ENTITY_REGISTRATION(VuPushNewsScreenUIAction);

//*****************************************************************************
// WebPage action
//*****************************************************************************
class VuShowWebPageUIAction : public VuUIAction
{
	DECLARE_RTTI
public:
	VuShowWebPageUIAction();
private:
	VuRetVal	Trigger(const VuParams &params)	{ VuCommunityManager::IF()->showWebPage(mURL); return VuRetVal(); }

	std::string	mURL;
};

IMPLEMENT_RTTI(VuShowWebPageUIAction, VuUIAction);
IMPLEMENT_ENTITY_REGISTRATION(VuShowWebPageUIAction);

//*****************************************************************************
VuShowWebPageUIAction::VuShowWebPageUIAction()
{
	addProperty(new VuStringProperty("URL", mURL));
}

//*****************************************************************************
// Market action
//*****************************************************************************
class VuShowMarketUIAction : public VuUIAction
{
	DECLARE_RTTI
public:
	VuShowMarketUIAction();
private:
	VuRetVal	Trigger(const VuParams &params)	{ VuCommunityManager::IF()->showMoreGames(mPackageName, mPublisherName); return VuRetVal(); }

	std::string	mPackageName;
	std::string	mPublisherName;
};

IMPLEMENT_RTTI(VuShowMarketUIAction, VuUIAction);
IMPLEMENT_ENTITY_REGISTRATION(VuShowMarketUIAction);

//*****************************************************************************
VuShowMarketUIAction::VuShowMarketUIAction()
{
	addProperty(new VuStringProperty("Package Name", mPackageName));
	addProperty(new VuStringProperty("Publisher Name", mPublisherName));
}

//*****************************************************************************
// TwitterPage action
//*****************************************************************************
class VuShowTwitterPageUIAction : public VuUIAction
{
	DECLARE_RTTI
public:
	VuShowTwitterPageUIAction();
private:
	VuRetVal	Trigger(const VuParams &params)	{ VuCommunityManager::IF()->showTwitterPage(mUserName); return VuRetVal(); }

	std::string	mUserName;
};

IMPLEMENT_RTTI(VuShowTwitterPageUIAction, VuUIAction);
IMPLEMENT_ENTITY_REGISTRATION(VuShowTwitterPageUIAction);

//*****************************************************************************
VuShowTwitterPageUIAction::VuShowTwitterPageUIAction()
{
	addProperty(new VuStringProperty("User Name", mUserName));
}

//*****************************************************************************
// FacebookPage action
//*****************************************************************************
class VuShowFacebookPageUIAction : public VuUIAction
{
	DECLARE_RTTI
public:
	VuShowFacebookPageUIAction();
private:
	VuRetVal	Trigger(const VuParams &params)	{ VuCommunityManager::IF()->showFacebookPage(mId, mUserName); return VuRetVal(); }

	std::string	mId;
	std::string	mUserName;
};

IMPLEMENT_RTTI(VuShowFacebookPageUIAction, VuUIAction);
IMPLEMENT_ENTITY_REGISTRATION(VuShowFacebookPageUIAction);

//*****************************************************************************
VuShowFacebookPageUIAction::VuShowFacebookPageUIAction()
{
	addProperty(new VuStringProperty("ID", mId));
	addProperty(new VuStringProperty("User Name", mUserName));
}

//*****************************************************************************
// GooglePlusPage action
//*****************************************************************************
class VuShowGooglePlusPageUIAction : public VuUIAction
{
	DECLARE_RTTI
public:
	VuShowGooglePlusPageUIAction();
private:
	VuRetVal	Trigger(const VuParams &params)	{ VuCommunityManager::IF()->showGooglePlusPage(mId); return VuRetVal(); }

	std::string	mId;
};

IMPLEMENT_RTTI(VuShowGooglePlusPageUIAction, VuUIAction);
IMPLEMENT_ENTITY_REGISTRATION(VuShowGooglePlusPageUIAction);

//*****************************************************************************
VuShowGooglePlusPageUIAction::VuShowGooglePlusPageUIAction()
{
	addProperty(new VuStringProperty("ID", mId));
}

//*****************************************************************************
// RateGame action
//*****************************************************************************
class VuRateGameUIAction : public VuUIAction
{
	DECLARE_RTTI
public:
	VuRateGameUIAction();
private:
	VuRetVal	Trigger(const VuParams &params)	{ VuCommunityManager::IF()->rateGame(mPackageName); return VuRetVal(); }

	std::string	mPackageName;
};

IMPLEMENT_RTTI(VuRateGameUIAction, VuUIAction);
IMPLEMENT_ENTITY_REGISTRATION(VuRateGameUIAction);

//*****************************************************************************
VuRateGameUIAction::VuRateGameUIAction()
{
	addProperty(new VuStringProperty("Package Name", mPackageName));
}


//*****************************************************************************
// GameRematch action
//*****************************************************************************
class VuGameRematchUIAction : public VuUIAction
{
	DECLARE_RTTI
private:
	VuRetVal Trigger(const VuParams &params) { VuEventManager::IF()->broadcast("OnGameRematch"); return VuRetVal(); }
};

IMPLEMENT_RTTI(VuGameRematchUIAction, VuUIAction);
IMPLEMENT_ENTITY_REGISTRATION(VuGameRematchUIAction);


//*****************************************************************************
// ShowStandings action
//*****************************************************************************
class VuShowStandingsUIAction : public VuUIAction
{
	DECLARE_RTTI
private:
	VuRetVal	Trigger(const VuParams &params)	{ VuEventManager::IF()->broadcast("OnShowStandings"); return VuRetVal(); }
};

IMPLEMENT_RTTI(VuShowStandingsUIAction, VuUIAction);
IMPLEMENT_ENTITY_REGISTRATION(VuShowStandingsUIAction);


//*****************************************************************************
// SkipIntro action
//*****************************************************************************
class VuSkipIntroUIAction : public VuUIAction
{
	DECLARE_RTTI
private:
	VuRetVal	Trigger(const VuParams &params)	{ VuEventManager::IF()->broadcast("OnSkipIntro"); return VuRetVal(); }
};

IMPLEMENT_RTTI(VuSkipIntroUIAction, VuUIAction);
IMPLEMENT_ENTITY_REGISTRATION(VuSkipIntroUIAction);


//*****************************************************************************
// SkipLoadingScreen action
//*****************************************************************************
class VuSkipLoadingScreenUIAction : public VuUIAction
{
	DECLARE_RTTI
private:
	VuRetVal	Trigger(const VuParams &params)	{ VuEventManager::IF()->broadcast("OnSkipLoadingScreen"); return VuRetVal(); }
};

IMPLEMENT_RTTI(VuSkipLoadingScreenUIAction, VuUIAction);
IMPLEMENT_ENTITY_REGISTRATION(VuSkipLoadingScreenUIAction);


//*****************************************************************************
// VuPaintUIAction
//*****************************************************************************
class VuPaintUIAction : public VuEntity
{
	DECLARE_RTTI

public:
	VuPaintUIAction();

	virtual void		onGameInitialize();
	virtual void		onGameRelease();

private:
	VuRetVal			HasChanged(const VuParams &params);
	VuRetVal			Undo(const VuParams &params);
	VuRetVal			Accept(const VuParams &params);
	VuRetVal			Randomize(const VuParams &params);

	VuScriptComponent	*mpScriptComponent;

	int					mShuffleIndex;
};

IMPLEMENT_RTTI(VuPaintUIAction, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuPaintUIAction);

//*****************************************************************************
VuPaintUIAction::VuPaintUIAction():
	mShuffleIndex(0)
{
	addComponent(mpScriptComponent = new VuScriptComponent(this, 150));

	ADD_SCRIPT_INPUT(mpScriptComponent, VuPaintUIAction, HasChanged, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuPaintUIAction, Undo);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuPaintUIAction, Accept);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuPaintUIAction, Randomize);
}

//*****************************************************************************
void VuPaintUIAction::onGameInitialize()
{
	mShuffleIndex = VuRand::global().range(0, VuGameUtil::IF()->constantDB()["Names"]["CarSkins"].size());

	VuGameUtil::IF()->pushCarPaintMode();
	VuGameUtil::IF()->setCarPaint(VuGameManager::IF()->getCurCar().mDecal, VuGameManager::IF()->getCurCar().mDecalColor, VuGameManager::IF()->getCurCar().mPaintColor);
}

//*****************************************************************************
void VuPaintUIAction::onGameRelease()
{
	VuGameUtil::IF()->popCarPaintMode();
}

//*****************************************************************************
VuRetVal VuPaintUIAction::HasChanged(const VuParams &params)
{
	bool hasChanged = false;

	hasChanged |= VuGameManager::IF()->getCurCar().mDecal != VuGameUtil::IF()->getCarPaintDecal();
	hasChanged |= VuGameManager::IF()->getCurCar().mDecalColor != VuGameUtil::IF()->getCarPaintDecalColor();
	hasChanged |= VuGameManager::IF()->getCurCar().mPaintColor != VuGameUtil::IF()->getCarPaintPaintColor();

	return VuRetVal(hasChanged);
}

//*****************************************************************************
VuRetVal VuPaintUIAction::Undo(const VuParams &params)
{
	VuGameUtil::IF()->setCarPaint(VuGameManager::IF()->getCurCar().mDecal, VuGameManager::IF()->getCurCar().mDecalColor, VuGameManager::IF()->getCurCar().mPaintColor);

	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuPaintUIAction::Accept(const VuParams &params)
{
	VuGameManager::IF()->paintCar(VuGameUtil::IF()->getCarPaintDecal(), VuGameUtil::IF()->getCarPaintDecalColor(), VuGameUtil::IF()->getCarPaintPaintColor());

	VuAchievementManager::IF()->unlock(13); // Paint a car

	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuPaintUIAction::Randomize(const VuParams &params)
{
	const VuJsonContainer &skinNames = VuGameUtil::IF()->constantDB()["Names"]["CarSkins"];
	if ( skinNames.size() )
	{
		const std::string &skinName = skinNames[mShuffleIndex].asString();
		const VuJsonContainer &skinData = VuGameUtil::IF()->carSkinDB()[skinName];

		const std::string &decal = skinData["Decal"].asString();
		const std::string &decalColor = skinData["DecalColor"].asString();
		const std::string &paintColor = skinData["PaintColor"].asString();

		VuGameUtil::IF()->setCarPaint(decal, decalColor, paintColor);

		mShuffleIndex = (mShuffleIndex + 1)%skinNames.size();
	}

	return VuRetVal();
}


//*****************************************************************************
// VuDisableInputUIAction
//*****************************************************************************
class VuDisableInputUIAction : public VuUIAction
{
	DECLARE_RTTI
private:
	VuRetVal Trigger(const VuParams &params)
	{
		getRootEntity()->handleEvent("DisableInput");
		return VuRetVal();
	}
};

IMPLEMENT_RTTI(VuDisableInputUIAction, VuUIAction);
IMPLEMENT_ENTITY_REGISTRATION(VuDisableInputUIAction);


//*****************************************************************************
// VuEnableInputUIAction
//*****************************************************************************
class VuEnableInputUIAction : public VuUIAction
{
	DECLARE_RTTI
private:
	VuRetVal Trigger(const VuParams &params)
	{
		getRootEntity()->handleEvent("EnableInput");
		return VuRetVal();
	}
};

IMPLEMENT_RTTI(VuEnableInputUIAction, VuUIAction);
IMPLEMENT_ENTITY_REGISTRATION(VuEnableInputUIAction);


//*****************************************************************************
// VuRestorePurchasesUIAction
//*****************************************************************************
class VuRestorePurchasesUIAction : public VuUIAction
{
	DECLARE_RTTI
private:
	VuRetVal Trigger(const VuParams &params)
	{
		VuBillingManager::IF()->restoreManagedItems();
		return VuRetVal();
	}
};

IMPLEMENT_RTTI(VuRestorePurchasesUIAction, VuUIAction);
IMPLEMENT_ENTITY_REGISTRATION(VuRestorePurchasesUIAction);


//*****************************************************************************
// VuShowTipUIAction
//*****************************************************************************
class VuShowTipUIAction : public VuUIAction
{
	DECLARE_RTTI
private:
	VuRetVal Trigger(const VuParams &params)
	{
		VuTipManager::IF()->showTip();
		return VuRetVal();
	}
};

IMPLEMENT_RTTI(VuShowTipUIAction, VuUIAction);
IMPLEMENT_ENTITY_REGISTRATION(VuShowTipUIAction);
