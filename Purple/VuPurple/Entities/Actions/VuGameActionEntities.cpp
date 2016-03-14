//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Game action entities
// 
//*****************************************************************************

#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarHeadlights.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/Managers/VuAchievementManager.h"
#include "VuPurple/Managers/VuAnalyticsManager.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Properties/VuEnumProperty.h"
#include "VuEngine/Properties/VuDBEntryProperty.h"
#include "VuEngine/Managers/VuProfileManager.h"

#include "VuPurple/Managers/VuBillingManager.h"
#include "VuPurple/Managers/VuCloudSaveManager.h"

//*****************************************************************************
// Game action base class
//*****************************************************************************
class VuGameActionEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuGameActionEntity()
	{
		addComponent(mpScriptComponent = new VuScriptComponent(this));
		ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuGameActionEntity, Trigger);
	}

protected:
	virtual VuRetVal Trigger(const VuParams &params) = 0;

	VuScriptComponent	*mpScriptComponent;
};

IMPLEMENT_RTTI(VuGameActionEntity, VuEntity);


//*****************************************************************************
//  Unlock achievement entity
//*****************************************************************************
class VuUnlockAchievementEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuUnlockAchievementEntity();

private:
	VuRetVal			Unlock(const VuParams &params);
	VuRetVal			IsUnlocked(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;

	// properties
	int					mAchievementId;
};

IMPLEMENT_RTTI(VuUnlockAchievementEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuUnlockAchievementEntity);


//*****************************************************************************
VuUnlockAchievementEntity::VuUnlockAchievementEntity():
	mAchievementId(0)
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	// properties
	addProperty(new VuIntProperty("Achievement", mAchievementId));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuUnlockAchievementEntity, Unlock);
	ADD_SCRIPT_INPUT(mpScriptComponent, VuUnlockAchievementEntity, IsUnlocked, VuRetVal::Bool, VuParamDecl());
}

//*****************************************************************************
VuRetVal VuUnlockAchievementEntity::Unlock(const VuParams &params)
{
	VuAchievementManager::IF()->unlock(mAchievementId);

	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuUnlockAchievementEntity::IsUnlocked(const VuParams &params)
{
	bool isUnlocked = VuAchievementManager::IF()->isUnlocked(mAchievementId);

	return VuRetVal(isUnlocked);
}


//*****************************************************************************
//  Update achievement entity
//*****************************************************************************
class VuUpdateAchievementEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuUpdateAchievementEntity();

private:
	VuRetVal			Update(const VuParams &params);
	VuRetVal			IsUnlocked(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;

	// properties
	int					mAchievementId;
	float				mProgressAmount;
};

IMPLEMENT_RTTI(VuUpdateAchievementEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuUpdateAchievementEntity);


//*****************************************************************************
VuUpdateAchievementEntity::VuUpdateAchievementEntity():
	mAchievementId(0),
	mProgressAmount(0)
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	// properties
	addProperty(new VuIntProperty("Achievement", mAchievementId));
	addProperty(new VuFloatProperty("Progress Amount", mProgressAmount));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuUpdateAchievementEntity, Update);
	ADD_SCRIPT_INPUT(mpScriptComponent, VuUpdateAchievementEntity, IsUnlocked, VuRetVal::Bool, VuParamDecl());
}

//*****************************************************************************
VuRetVal VuUpdateAchievementEntity::Update(const VuParams &params)
{
	float progress = VuAchievementManager::IF()->getProgress(mAchievementId);
	progress += mProgressAmount;
	VuAchievementManager::IF()->setProgress(mAchievementId, progress);

	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuUpdateAchievementEntity::IsUnlocked(const VuParams &params)
{
	bool isUnlocked = VuAchievementManager::IF()->isUnlocked(mAchievementId);

	return VuRetVal(isUnlocked);
}


//*****************************************************************************
//  SetBranchProbabilities entity
//*****************************************************************************
class VuSetTrackBranchProbabilities : public VuGameActionEntity
{
	DECLARE_RTTI

public:
	VuSetTrackBranchProbabilities();

private:
	VuRetVal			Trigger(const VuParams &params);

	// properties
	bool				mSetBranchA;
	bool				mSetBranchB;
	float				mBranchAProbability;
	float				mBranchBProbability;
};

IMPLEMENT_RTTI(VuSetTrackBranchProbabilities, VuGameActionEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuSetTrackBranchProbabilities);


//*****************************************************************************
VuSetTrackBranchProbabilities::VuSetTrackBranchProbabilities()
: mSetBranchA(true)
, mSetBranchB(true)
, mBranchAProbability(1.0f)
, mBranchBProbability(1.0f)
{
	addProperty(new VuBoolProperty("Set Branch A", mSetBranchA));
	addProperty(new VuBoolProperty("Set Branch B", mSetBranchB));
	addProperty(new VuFloatProperty("Branch A Prob.", mBranchAProbability));
	addProperty(new VuFloatProperty("Branch B Prob.", mBranchBProbability));

	ADD_SCRIPT_OUTPUT(mpScriptComponent, Set, VuRetVal::Void, VuParamDecl(4, VuParams::Bool, VuParams::Bool, VuParams::Float, VuParams::Float));
}

//*****************************************************************************
VuRetVal VuSetTrackBranchProbabilities::Trigger(const VuParams &params)
{
	VuParams outParams;

	outParams.addBool(mSetBranchA);
	outParams.addBool(mSetBranchB);
	outParams.addFloat(mBranchAProbability);
	outParams.addFloat(mBranchBProbability);

	mpScriptComponent->getPlug("Set")->execute(outParams);

	return VuRetVal();
}

//*****************************************************************************
// ZimonLogAnalyticEvent
//*****************************************************************************
class VuZimonLogAnalyticEventEntity : public VuGameActionEntity
{
	DECLARE_RTTI

public:
	VuZimonLogAnalyticEventEntity();

private:
	VuRetVal			Trigger(const VuParams &params);

	// properties
	std::string			mEventName;
	std::string			mParamKey;
	int			mParamValue;
};
IMPLEMENT_RTTI(VuZimonLogAnalyticEventEntity, VuGameActionEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuZimonLogAnalyticEventEntity);
//*****************************************************************************
VuZimonLogAnalyticEventEntity::VuZimonLogAnalyticEventEntity()
{
	// properties
	addProperty(new VuStringProperty("Event Name", mEventName));
	addProperty(new VuStringProperty("Param Key", mParamKey));
	addProperty(new VuIntProperty("Param Value", mParamValue));
}

//*****************************************************************************
VuRetVal VuZimonLogAnalyticEventEntity::Trigger(const VuParams &params)
{
	VuAnalyticsManager::IF()->logEvent(mEventName.c_str(), mParamKey.c_str(), mParamValue);

	return VuRetVal();
}

//*****************************************************************************
//  LogAnalyticEvent
//*****************************************************************************
class VuLogAnalyticEventEntity : public VuGameActionEntity
{
	DECLARE_RTTI

public:
	VuLogAnalyticEventEntity();

private:
	VuRetVal			Trigger(const VuParams &params);

	// properties
	std::string			mEventName;
	std::string			mParamName;
	std::string			mParamValue;
};


IMPLEMENT_RTTI(VuLogAnalyticEventEntity, VuGameActionEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuLogAnalyticEventEntity);


//*****************************************************************************
VuLogAnalyticEventEntity::VuLogAnalyticEventEntity()
{
	// properties
	addProperty(new VuStringProperty("Event Name", mEventName));
	addProperty(new VuStringProperty("Param Name", mParamName));
	addProperty(new VuStringProperty("Param Value", mParamValue));
}

//*****************************************************************************
VuRetVal VuLogAnalyticEventEntity::Trigger(const VuParams &params)
{
	VuAnalyticsManager::IF()->logEvent(mEventName.c_str(), mParamName.c_str(), mParamValue.c_str());

	return VuRetVal();
}


//*****************************************************************************
//  Switch Headlights entity
//*****************************************************************************
class VuSwitchHeadlightsEntity : public VuGameActionEntity
{
	DECLARE_RTTI

public:
	VuSwitchHeadlightsEntity();

private:
	VuRetVal			Trigger(const VuParams &params);

	// properties
	bool				mTurnOn;
};


IMPLEMENT_RTTI(VuSwitchHeadlightsEntity, VuGameActionEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuSwitchHeadlightsEntity);


//*****************************************************************************
VuSwitchHeadlightsEntity::VuSwitchHeadlightsEntity():
	mTurnOn(true)
{
	// properties
	addProperty(new VuBoolProperty("Turn On", mTurnOn));
}

//*****************************************************************************
VuRetVal VuSwitchHeadlightsEntity::Trigger(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	if ( accessor.getNextType() == VuParams::Entity )
	{
		VuEntity *pEntity = accessor.getEntity();
		if ( pEntity->isDerivedFrom(VuCarEntity::msRTTI) )
		{
			VuCarEntity *pCar = static_cast<VuCarEntity *>(pEntity);
			pCar->getHeadlights()->setTurnedOn(mTurnOn);
		}
	}

	return VuRetVal();
}


//*****************************************************************************
//  VuFinishGameEntity
//*****************************************************************************
class VuFinishGameEntity : public VuGameActionEntity
{
	DECLARE_RTTI

private:
	VuRetVal Trigger(const VuParams &params) { VuEventManager::IF()->broadcast("FinishGame"); return VuRetVal(); }
};

IMPLEMENT_RTTI(VuFinishGameEntity, VuGameActionEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuFinishGameEntity);


//*****************************************************************************
//  CheatPurchaseCurrency
//*****************************************************************************
class VuCheatPurchaseCurrencyEntity : public VuGameActionEntity
{
	DECLARE_RTTI

public:
	VuCheatPurchaseCurrencyEntity();

private:
	VuRetVal	Trigger(const VuParams &params);

	int			mStandardAmount;
	int			mPremiumAmount;
};

IMPLEMENT_RTTI(VuCheatPurchaseCurrencyEntity, VuGameActionEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuCheatPurchaseCurrencyEntity);

//*****************************************************************************
VuCheatPurchaseCurrencyEntity::VuCheatPurchaseCurrencyEntity():
	mStandardAmount(0),
	mPremiumAmount(0)
{
	// properties
	addProperty(new VuIntProperty("Standard Amount", mStandardAmount));
	addProperty(new VuIntProperty("Premium Amount", mPremiumAmount));
}

//*****************************************************************************
VuRetVal VuCheatPurchaseCurrencyEntity::Trigger(const VuParams &params)
{
	VuGameManager::IF()->addCurrencyPurchased(VuGameManager::CT_STANDARD, mStandardAmount);
	VuGameManager::IF()->addCurrencyPurchased(VuGameManager::CT_PREMIUM, mPremiumAmount);
	return VuRetVal();
}


//*****************************************************************************
//  GiveCurrency
//*****************************************************************************
class VuGiveCurrencyEntity : public VuGameActionEntity
{
	DECLARE_RTTI

public:
	VuGiveCurrencyEntity();

private:
	VuRetVal	Trigger(const VuParams &params);
};

IMPLEMENT_RTTI(VuGiveCurrencyEntity, VuGameActionEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuGiveCurrencyEntity);

//*****************************************************************************
VuGiveCurrencyEntity::VuGiveCurrencyEntity()
{
	ADD_SCRIPT_OUTPUT(mpScriptComponent, Standard, VuRetVal::Int, VuParamDecl());
	ADD_SCRIPT_OUTPUT(mpScriptComponent, Premium, VuRetVal::Int, VuParamDecl());
}

//*****************************************************************************
VuRetVal VuGiveCurrencyEntity::Trigger(const VuParams &params)
{
	VuRetVal sc = mpScriptComponent->getPlug("Standard")->execute();
	if ( sc.getType() == VuRetVal::Int )
		VuGameManager::IF()->addCurrencyEarned(VuGameManager::CT_STANDARD, sc.asInt());

	VuRetVal pc = mpScriptComponent->getPlug("Premium")->execute();
	if ( pc.getType() == VuRetVal::Int )
		VuGameManager::IF()->addCurrencyEarned(VuGameManager::CT_PREMIUM, pc.asInt());

	return VuRetVal();
}


//*****************************************************************************
//  VuSetSetChampStageEntity
//*****************************************************************************
class VuSetSetChampStageEntity : public VuGameActionEntity
{
	DECLARE_RTTI

public:
	VuSetSetChampStageEntity() : mStage(0)
	{
		addProperty(new VuIntProperty("Stage", mStage));
	}

private:
	VuRetVal Trigger(const VuParams &params)
	{
		VuGameUtil::IF()->dataWrite()["EventData"]["ChampStage"].putValue(mStage);
		return VuRetVal();
	}

	// properties
	int		mStage;
};

IMPLEMENT_RTTI(VuSetSetChampStageEntity, VuGameActionEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuSetSetChampStageEntity);


//*****************************************************************************
//  VuSetFrontEndCameraTransitionTimeEntity
//*****************************************************************************
class VuSetFrontEndCameraTransitionTimeEntity : public VuGameActionEntity
{
	DECLARE_RTTI

public:
	VuSetFrontEndCameraTransitionTimeEntity() : mTime(0.0f) {
		addProperty(new VuFloatProperty("Time", mTime));
		ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, Then);
	}
	VuRetVal Trigger(const VuParams &params) {
		VuGameUtil::IF()->setFrontEndCameraTransitionTime(mTime);
		mpScriptComponent->getPlug("Then")->execute();
		return VuRetVal();
	}
	float	mTime;
};

IMPLEMENT_RTTI(VuSetFrontEndCameraTransitionTimeEntity, VuGameActionEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuSetFrontEndCameraTransitionTimeEntity);


//*****************************************************************************
//  VuDisplayNotificationsEntity
//*****************************************************************************
class VuDisplayNotificationsEntity : public VuGameActionEntity
{
	DECLARE_RTTI

public:
	VuRetVal Trigger(const VuParams &params) {
		VuAnalyticsManager::IF()->displayNotifications();
		return VuRetVal();
	}
};

IMPLEMENT_RTTI(VuDisplayNotificationsEntity, VuGameActionEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuDisplayNotificationsEntity);











//*****************************************************************************
//  VuSetSetChampStageEntity
//*****************************************************************************
class VuSetOwnedBigGiftEntity : public VuGameActionEntity
{
	DECLARE_RTTI

public:
	VuSetOwnedBigGiftEntity() : mOwnedObj(""), mGameCoin(2000), mGameDiamand(20), mTicket(5)
	{
		addProperty(new VuStringProperty("OwndObj", mOwnedObj));
		addProperty(new VuIntProperty("Coin", mGameCoin));
		addProperty(new VuIntProperty("Diamond", mGameDiamand));
		addProperty(new VuIntProperty("Ticket", mTicket));

	}

private:
	VuRetVal Trigger(const VuParams &params)
	{
		if (mOwnedObj != "")
			VuBillingManager::IF()->setOwned(mOwnedObj.c_str());

		//·¢½±Àø
		VuGameManager::IF()->addCurrencyPurchased(VuGameManager::CT_STANDARD, mGameCoin);
		VuGameManager::IF()->addCurrencyPurchased(VuGameManager::CT_PREMIUM, mGameDiamand);
		VuGameManager::IF()->addTokenNoLimit(mTicket);
		//VuGameUtil::IF()->dataWrite()["EventData"]["ChampStage"].putValue(mStage);

		VuProfileManager::IF()->save();
		VuCloudSaveManager::IF()->save();

		return VuRetVal();
	}

	// properties
	std::string		mOwnedObj;
	int				mGameCoin;
	int				mGameDiamand;
	int				mTicket;
};

IMPLEMENT_RTTI(VuSetOwnedBigGiftEntity, VuGameActionEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuSetOwnedBigGiftEntity);