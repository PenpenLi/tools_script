//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Award Entities
// 
//*****************************************************************************

#include "VuGameTextEntity.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Managers/VuStatsManager.h"
#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Managers/VuBillingManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Properties/VuEnumProperty.h"
#include "VuEngine/Properties/VuAudioProperty.h"
#include "VuEngine/Assets/VuSpreadsheetAsset.h"
#include "VuEngine/DB/VuStringDB.h"
#include "VuEngine/Util/VuStringUtil.h"
#include "VuEngine/Util/VuAudioUtil.h"
#include "VuEngine/Util/VuSpreadsheetQuery.h"


//*****************************************************************************
// StarsEarned
//*****************************************************************************
class VuStarsEarnedEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuStarsEarnedEntity();

private:
	VuRetVal			GetCount(const VuParams &params);
	VuRetVal			In(const VuParams &params);

	int					getCount();

	// components
	VuScriptComponent	*mpScriptComponent;

	enum eType { CURRENT_GAME, PREVIOUS_BEST };

	// properties
	int					mType;
};

IMPLEMENT_RTTI(VuStarsEarnedEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuStarsEarnedEntity);

//*****************************************************************************
VuStarsEarnedEntity::VuStarsEarnedEntity():
	mType(CURRENT_GAME)
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this, 100));

	// properties
	static VuStaticIntEnumProperty::Choice sTypeChoices[] =
	{
		{ "Current Game", CURRENT_GAME},
		{ "Previous Best", PREVIOUS_BEST},
		{ VUNULL }
	};
	addProperty(new VuStaticIntEnumProperty("Type", mType, sTypeChoices));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuStarsEarnedEntity, GetCount);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuStarsEarnedEntity, In);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, 0);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, 1);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, 2);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, 3);
}

//*****************************************************************************
VuRetVal VuStarsEarnedEntity::GetCount(const VuParams &params)
{
	return VuRetVal(getCount());
}

//*****************************************************************************
VuRetVal VuStarsEarnedEntity::In(const VuParams &params)
{
	int count = getCount();

	if ( count == 0 ) mpScriptComponent->getPlug("0")->execute();
	if ( count == 1 ) mpScriptComponent->getPlug("1")->execute();
	if ( count == 2 ) mpScriptComponent->getPlug("2")->execute();
	if ( count == 3 ) mpScriptComponent->getPlug("3")->execute();

	return VuRetVal();
}

//*****************************************************************************
int VuStarsEarnedEntity::getCount()
{
	int place = 0;

	if ( mType == CURRENT_GAME )
	{
		if ( VuCarManager::IF()->getLocalHumanCarCount() )
		{
			VuCarEntity *pCar = VuCarManager::IF()->getLocalHumanCar(0);
			place = pCar->getStats().mPlace;
		}
	}
	else if ( mType == PREVIOUS_BEST )
	{
		const std::string &eventName = VuGameUtil::IF()->getEventName();
		VuStatsManager::IF()->getBestPlace(eventName.c_str(), place);
	}

	if ( place >= 1 && place <= 3 )
		return 4 - place;

	return 0;
}


//*****************************************************************************
// RewardText
//*****************************************************************************
class VuRewardTextEntity : public VuGameTextBaseEntity
{
	DECLARE_RTTI

public:
	VuRewardTextEntity();

	virtual void		onGameInitialize();

	virtual const char	*getText();

private:
	// scripting
	VuRetVal			Start(const VuParams &params);
	VuRetVal			Skip(const VuParams &params);

	// events
	virtual void		OnUITick(const VuParams &params);
	void				OnPurchaseMade(const VuParams &params);

	void				giveAmount(int amount);

	enum eType { TYPE_STANDARD_CURRENCY, TYPE_PREMIUM_CURRENCY };
	enum eState { STATE_INITIAL, STATE_COUNTING, STATE_DONE };

	// properties
	int					mType;
	float				mCountTime;
	std::string			mStringID;
	std::string			mTickSfx;
	bool				mCarChamp;

	int					mTargetAmount;
	int					mDisplayAmount;
	float				mSmoothAmount;
	std::string			mText;
	eState				mState;
	bool				mCoinsDoubled;
};

IMPLEMENT_RTTI(VuRewardTextEntity, VuGameTextBaseEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuRewardTextEntity);

//*****************************************************************************
VuRewardTextEntity::VuRewardTextEntity():
	mType(TYPE_STANDARD_CURRENCY),
	mCarChamp(false),
	mCountTime(1.0f),
	mTargetAmount(0),
	mDisplayAmount(0),
	mSmoothAmount(0.0f),
	mState(STATE_INITIAL),
	mCoinsDoubled(false)
{
	// properties
	static VuStaticIntEnumProperty::Choice sTypeChoices[] =
	{
		{ "Standard Currency", TYPE_STANDARD_CURRENCY},
		{ "Premium Currency", TYPE_PREMIUM_CURRENCY},
		{ VUNULL }
	};
	addProperty(new VuStaticIntEnumProperty("Type", mType, sTypeChoices));
	addProperty(new VuFloatProperty("Count Time", mCountTime));
	addProperty(new VuStringProperty("String ID", mStringID));
	addProperty(new VuAudioEventNameProperty("Tick Sfx", mTickSfx));
	addProperty(new VuBoolProperty("Car Champ", mCarChamp));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuRewardTextEntity, Start);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuRewardTextEntity, Skip);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnStart);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnTick);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, Done);

	// events
	REG_EVENT_HANDLER(VuRewardTextEntity, OnPurchaseMade);
}

//*****************************************************************************
void VuRewardTextEntity::onGameInitialize()
{
	VuGameTextBaseEntity::onGameInitialize();

	if ( mCarChamp )
	{
		const std::string &carName = VuGameUtil::IF()->dataRead()["CarChamp"]["Car"].asString();
		int stage = VuGameUtil::IF()->dataRead()["CarChamp"]["Stage"].asInt();

		// determine place
		int place = 0;
		VuGameUtil::CarChampTable table;
		VuGameUtil::IF()->createCarChampTable(carName, table);
		for ( int i = 0; i < (int)table.size(); i++ )
			if ( table[i].mPlayer )
				place = i + 1;

		if ( mType == TYPE_STANDARD_CURRENCY )
		{
			mTargetAmount = VuGameUtil::IF()->getCarChampRewardSC(stage, place);
		}
		else if ( mType == TYPE_PREMIUM_CURRENCY )
		{
			mTargetAmount = VuGameUtil::IF()->getCarChampRewardPC(stage);
		}
	}
	else if ( VuCarManager::IF()->getLocalHumanCarCount() )
	{
		// determine place
		int place = VuCarManager::IF()->getLocalHumanCar(0)->getStats().mPlace;

		const std::string &gameType = VuGameUtil::IF()->getEventData()["GameType"].asString();

		if ( gameType == "QuickRace" )
		{
			if ( mType == TYPE_STANDARD_CURRENCY )
			{
				//int stage = VuGameUtil::IF()->dataRead()["QuickRace"]["Stage"].asInt();
				//mTargetAmount = VuGameUtil::IF()->getQuickRaceRewardSC(stage, place);
			}
		}
		else if ( VuGameUtil::IF()->getEventData()["IsChallenge"].asBool() )
		{
			if ( mType == TYPE_STANDARD_CURRENCY )
			{
				if ( place == 1 )
				{
					mTargetAmount = VuProfileManager::IF()->dataRead()["DailyChallenge"]["Reward"].asInt();
				}
			}
		}
		else
		{
			const std::string &eventName = VuGameUtil::IF()->getEventName();

			const VuSpreadsheetAsset *pSA = VuGameUtil::IF()->eventSpreadsheet();
			int rowIndex = VuSpreadsheetQuery::findFirstRow(pSA, VuSpreadsheetQuery::VuStringEqual("Event", eventName.c_str()));

			if ( mType == TYPE_STANDARD_CURRENCY )
			{
				if ( place >= 1 && place <= 6 )
				{
					int columnIndex = pSA->getColumnIndex("SC") + place - 1;
					mTargetAmount = pSA->getField(rowIndex, columnIndex).asInt();
				}
			}
			else if ( mType == TYPE_PREMIUM_CURRENCY )
			{
				if ( place == 1 )
					mTargetAmount = pSA->getField(rowIndex, "PC").asInt();
			}
		}
	}

	if ( mType == TYPE_STANDARD_CURRENCY && !mCoinsDoubled && VuBillingManager::IF()->isUnlocked("DoubleCoins") )
	{
		mCoinsDoubled = true;
		mTargetAmount *= 2;
	}

	mState = STATE_INITIAL;
	mDisplayAmount = 0;
	mSmoothAmount = 0.0f;
}

//*****************************************************************************
const char *VuRewardTextEntity::getText()
{
	char strPrice[16];
	VuStringUtil::integerFormat(mDisplayAmount, strPrice, sizeof(strPrice));

	mText = VuStringDB::IF()->getString(mStringID);
	VuStringUtil::replace(mText, "[[REWARD_CURRENT]]", strPrice);
	VuStringUtil::replace(mText, "[[CHALLENGE_REWARD]]", strPrice);

	return mText.c_str();
}

//*****************************************************************************
VuRetVal VuRewardTextEntity::Start(const VuParams &params)
{
	if ( mState == STATE_INITIAL )
	{
		mState = STATE_COUNTING;

		mpScriptComponent->getPlug("OnStart")->execute();
	}

	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuRewardTextEntity::Skip(const VuParams &params)
{
	if ( mState != STATE_DONE )
	{
		mState = STATE_DONE;

		giveAmount(mTargetAmount - mDisplayAmount);
		mDisplayAmount = mTargetAmount;

		mpScriptComponent->getPlug("Done")->execute();
	}

	return VuRetVal();
}

//*****************************************************************************
void VuRewardTextEntity::OnUITick(const VuParams &params)
{
	VuGameTextBaseEntity::OnUITick(params);

	VuParams::VuAccessor accessor(params);
	float fdt = accessor.getFloat();

	if ( mState == STATE_COUNTING )
	{
		int prevAmount = mDisplayAmount;
		bool done = false;

		mSmoothAmount += fdt*mTargetAmount/mCountTime;
		if ( mSmoothAmount >= mTargetAmount )
		{
			mSmoothAmount = (float)mTargetAmount;
			mState = STATE_DONE;
			done = true;
		}
		mDisplayAmount = VuRound(mSmoothAmount);

		if ( mDisplayAmount > prevAmount )
		{
			giveAmount(mDisplayAmount - prevAmount);
			if ( mTickSfx.length() )
				VuAudioUtil::playSfx(mTickSfx.c_str());

			mpScriptComponent->getPlug("OnTick")->execute();
		}

		if ( done )
			mpScriptComponent->getPlug("Done")->execute();
	}
}

//*****************************************************************************
void VuRewardTextEntity::OnPurchaseMade(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	const char *itemName = accessor.getString();

	const VuJsonContainer &itemData = VuDataUtil::findArrayMember(VuGameUtil::IF()->storeDB(), "Name", itemName);
	if ( !mCoinsDoubled && itemData["DoubleCoins"].asBool() )
	{
		mCoinsDoubled = true;
		if ( mType == TYPE_STANDARD_CURRENCY )
		{
			if ( mState == STATE_DONE )
			{
				giveAmount(mTargetAmount);
				mTargetAmount *= 2;
				mDisplayAmount *= 2;
			}
			else
			{
				mTargetAmount *= 2;
			}
		}
	}
}

//*****************************************************************************
void VuRewardTextEntity::giveAmount(int amount)
{
	if ( mType == TYPE_STANDARD_CURRENCY )
	{
		VuGameManager::IF()->addCurrencyEarned(VuGameManager::CT_STANDARD, amount);
	}
	else if ( mType == TYPE_PREMIUM_CURRENCY )
	{
		VuGameManager::IF()->addCurrencyEarned(VuGameManager::CT_PREMIUM, amount);
	}
}
