//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  power-up game entity
// 
//*****************************************************************************

#include "VuEngine/Assets/VuAssetFactory.h"
#include "VuEngine/Assets/VuAnimationAsset.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Entities/Objects/VuAnimatedPropEntity.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Components/Transform/VuTransformComponent.h"

#include "VuEngine/Managers/VuProfileManager.h"
#include "VuEngine/Pfx/VuPfx.h"
#include "VuEngine/Pfx/VuPfxManager.h"
#include "VuEngine/Pfx/VuPfxEntity.h"
#include "VuEngine/DB/VuStringDB.h"
#include "VuEngine/Math/VuMathUtil.h"
#include "VuEngine/Util/VuStringUtil.h"
#include "VuEngine/Math/VuRand.h"

#include "VuPurple/PowerUps/VuPowerUpManager.h"
#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Managers/VuCloudSaveManager.h"
#include "VuPurple/Util/VuGameFontMacros.h"
#include "VuPurple/Util/VuGameUtil.h"

class VuPowerUpGameEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuPowerUpGameEntity();
	~VuPowerUpGameEntity();

	virtual void					onGameInitialize();
	virtual void					onGameRelease();

	enum eCoconut
	{
		COCONUT_1,
		COCONUT_2,
		COCONUT_3,

		COCONUT_MAX
	};

	enum eGiftType
	{
		GIFT_POWERUP,
		GIFT_SC,
		GIFT_GREASEMONKEY,

		GIFT_MAX
	};

protected:
	// Helpers
	void							generateMoves();
	void							generateGifts();

	// Animation Handling
	void							loadAnimations();
	void							playInitialAnims();
	void							playCoconutAnims();
	void							playIdleCoconuts();
	void							playLiftAnim(eCoconut which);
	void							playLiftAnims();
	void							playDropAnims();
	void							pick(eCoconut which);
	void							giveGift(eCoconut which);

	// Powerup Handling
	enum eBoing {BOING_IN, BOING_OUT};
	void							initPowerups();
	void							startPowerup(VUINT index);
	void							preparePowerup(VUINT index, eBoing inOrOut);
	void							preparePowerups(eBoing inOrOut);

	// Scripting Functions

	// Inputs
	VuRetVal						StartGame(const VuParams &params = VuParams());
	VuRetVal						Pick1(const VuParams &params);
	VuRetVal						Pick2(const VuParams &params);
	VuRetVal						Pick3(const VuParams &params);
	VuRetVal						CoconutsAnimDone(const VuParams &params);
	VuRetVal						PickAnimDone(const VuParams &params);
	VuRetVal						CanAfford(const VuParams &params);

	// Outputs
	VuRetVal						ShuffleDone(const VuParams &params);

	// Animation Outputs
	VuRetVal						Coco1Play(const VuParams &params)	{ VuRetVal retval; return retval; }
	VuRetVal						Coco2Play(const VuParams &params)	{ VuRetVal retval; return retval; }
	VuRetVal						Coco3Play(const VuParams &params)	{ VuRetVal retval; return retval; }

	VuRetVal						Pick1Play(const VuParams &params)	{ VuRetVal retval; return retval; }
	VuRetVal						Pick2Play(const VuParams &params)	{ VuRetVal retval; return retval; }
	VuRetVal						Pick3Play(const VuParams &params)	{ VuRetVal retval; return retval; }

	// event handlers
	void							OnUITick(const VuParams &params);

	// components
	VuScriptComponent				*mpScriptComponent;

	// Instance Variables
	//

	// What State is this PowerUpGame in?
	enum eState { NOT_STARTED, DROPPING, SHUFFLING, PICKING, FINISHED };
	eState							mState;
	float							mStartDelay;

	// Gift handling
	//
	struct Gift
	{
		Gift() : mType(GIFT_MAX) {}
		Gift(eGiftType type, const std::string& name, const std::string& pfxName, VUUINT32 amount = 0) 
		{ 
			mType = type; 
			mName = name; 
			mPfxName = pfxName;
			mAmount = amount; 
		}

		eGiftType		mType;
		std::string		mName;
		std::string		mPfxName;
		VUUINT32		mAmount;
	};

	typedef std::vector<Gift> GiftList;
	GiftList						mEligibleGifts;
	GiftList						mGifts;

	// Powerup Handling
	//
	struct PowerupData
	{
		enum eState { NONE, DELAYING, ACTIVE, DONE };

		PowerupData() :
			mState(NONE),
			mPfxHandle(0), 
			mpLocator(VUNULL), 
			mScale(0.0f), 
			mScaleVelocity(0.0f), 
			mTargetScale(1.0f), 
			mDelayTimer(0.0f) {}

		eState				mState;
		VUUINT32			mPfxHandle;
		VuEntity*			mpLocator;
		float				mScale;
		float				mScaleVelocity;
		float				mTargetScale;
		float				mDelayTimer;
	};

	PowerupData						mPowerupData[COCONUT_MAX];

	VUINT							mPickCost[COCONUT_MAX];

	// Animations
	//
	float							mTimeFactor;
	bool							mPlayCoconutAnimsAtNextTick;

	// Shuffle Animation sets (3 animations per entry for the 3 coconuts)
	struct ShuffleAnimAsset
	{
		VuAnimationAsset* mpAsset[VuPowerUpGameEntity::COCONUT_MAX];
	};
	
	typedef std::vector<ShuffleAnimAsset> ShuffleAnimAssets;

	ShuffleAnimAssets				mShuffleAnimAssets;
	VUINT							mAnimDoneCounter;
	bool							mShuffleDone;

	// Various animations
	typedef std::vector<VuAnimationAsset*> AnimAssets;
	
	AnimAssets						mLiftAnimAssets;
	AnimAssets						mDropAnimAssets;
	AnimAssets						mIdleAnimAssets;

	VUINT32							mCurMove;
	std::vector<VUINT32>			mMoves;

	// Counts how many times the user has picked a coconut
	VUINT							mNumPicked;
	bool							mPicked[COCONUT_MAX];


	// Delays we get from editor Properties
	float							mOpenBoingDelay;
	float							mCloseBoingDelay;
};

static const char* sCoconutPlugs[] =
{
	"Coco1Play",
	"Coco2Play",
	"Coco3Play"
};

static const char* sPickPlugs[] =
{
	"Pick1Play",
	"Pick2Play",
	"Pick3Play"
};

static const VUINT scNumPicks = 3;

IMPLEMENT_RTTI(VuPowerUpGameEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuPowerUpGameEntity);

//*****************************************************************************
VuPowerUpGameEntity::VuPowerUpGameEntity()
	:
	mPlayCoconutAnimsAtNextTick(false),
	mCurMove(0),
	mTimeFactor(1.0f),
	mState(NOT_STARTED),
	mOpenBoingDelay(0.0f),
	mCloseBoingDelay(0.0f),
	mNumPicked(0),
	mAnimDoneCounter(0),
	mShuffleDone(false)
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	mPickCost[0] = 0;
	mPickCost[1] = 10;
	mPickCost[2] = 20;

	// properties
	addProperty(new VuFloatProperty("Open Boing Delay", mOpenBoingDelay));
	addProperty(new VuFloatProperty("Close Boing Delay", mCloseBoingDelay));
	addProperty(new VuIntProperty("2nd Spin Cost", mPickCost[1]));
	addProperty(new VuIntProperty("3rd Spin Cost", mPickCost[2]));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuPowerUpGameEntity, StartGame);

	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuPowerUpGameEntity, CoconutsAnimDone);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuPowerUpGameEntity, PickAnimDone);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuPowerUpGameEntity, Pick1);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuPowerUpGameEntity, Pick2);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuPowerUpGameEntity, Pick3);
	ADD_SCRIPT_INPUT(mpScriptComponent, VuPowerUpGameEntity, CanAfford, VuRetVal::Bool, VuParamDecl());

	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, ShuffleDone);

	ADD_SCRIPT_OUTPUT(mpScriptComponent, Coco1Play, VuRetVal::Void, VuParamDecl(5, VuParams::Asset, VuParams::Float, VuParams::Float, VuParams::Float, VuParams::Bool));
	ADD_SCRIPT_OUTPUT(mpScriptComponent, Coco2Play, VuRetVal::Void, VuParamDecl(5, VuParams::Asset, VuParams::Float, VuParams::Float, VuParams::Float, VuParams::Bool));
	ADD_SCRIPT_OUTPUT(mpScriptComponent, Coco3Play, VuRetVal::Void, VuParamDecl(5, VuParams::Asset, VuParams::Float, VuParams::Float, VuParams::Float, VuParams::Bool));

	ADD_SCRIPT_OUTPUT(mpScriptComponent, Pick1Play, VuRetVal::Void, VuParamDecl(5, VuParams::Asset, VuParams::Float, VuParams::Float, VuParams::Float, VuParams::Bool));
	ADD_SCRIPT_OUTPUT(mpScriptComponent, Pick2Play, VuRetVal::Void, VuParamDecl(5, VuParams::Asset, VuParams::Float, VuParams::Float, VuParams::Float, VuParams::Bool));
	ADD_SCRIPT_OUTPUT(mpScriptComponent, Pick3Play, VuRetVal::Void, VuParamDecl(5, VuParams::Asset, VuParams::Float, VuParams::Float, VuParams::Float, VuParams::Bool));

	ADD_SCRIPT_REF(mpScriptComponent, PfxLoc1, VuEntity::msRTTI);
	ADD_SCRIPT_REF(mpScriptComponent, PfxLoc2, VuEntity::msRTTI);
	ADD_SCRIPT_REF(mpScriptComponent, PfxLoc3, VuEntity::msRTTI);

	// event handlers
	REG_EVENT_HANDLER(VuPowerUpGameEntity, OnUITick);

	// constants
}

//*****************************************************************************
VuPowerUpGameEntity::~VuPowerUpGameEntity()
{
}

//*****************************************************************************
void VuPowerUpGameEntity::loadAnimations()
{
	const VuJsonContainer& constants = VuGameUtil::IF()->constantDB()["PowerUpGame"];
	
	// Load the animations
	for (VUINT i = 0; i < constants["ShuffleAnims"].size(); i++)
	{
		const VuJsonContainer& anims = constants["ShuffleAnims"][i];

		ShuffleAnimAsset asset;
		for (int j = 0; j < COCONUT_MAX; j++)
		{
			asset.mpAsset[j] = VuAssetFactory::IF()->createAsset<VuAnimationAsset>(anims[j].asString());
		}

		mShuffleAnimAssets.push_back(asset);
	}

	for (VUINT i = 0; i < constants["LiftAnims"].size(); i++)
	{
		mLiftAnimAssets.push_back(VuAssetFactory::IF()->createAsset<VuAnimationAsset>(constants["LiftAnims"][i].asString()));
	}

	for (VUINT i = 0; i < constants["DropAnims"].size(); i++)
	{
		mDropAnimAssets.push_back(VuAssetFactory::IF()->createAsset<VuAnimationAsset>(constants["DropAnims"][i].asString()));
	}

	for (VUINT i = 0; i < constants["IdleAnims"].size(); i++)
	{
		mIdleAnimAssets.push_back(VuAssetFactory::IF()->createAsset<VuAnimationAsset>(constants["IdleAnims"][i].asString()));
	}
}

//*****************************************************************************
void VuPowerUpGameEntity::onGameInitialize()
{
	// Initial State
	mState = NOT_STARTED;

	VuGameUtil::IF()->dataWrite()["PowerUpGame"]["BonusBPrice"].putValue(mPickCost[1]);
	VuGameUtil::IF()->dataWrite()["PowerUpGame"]["BonusCPrice"].putValue(mPickCost[2]);

	mEligibleGifts.clear();

	// Build powerup list
	for (int i = 0; i < VuPowerUpManager::IF()->getPowerUpCount(); i++)
	{
		std::string name = VuPowerUpManager::IF()->getPowerUp(i)->mName;

		if (VuGameManager::IF()->isPowerUpOwned(name))
		{
			continue;
		}

		const Gift gift(GIFT_POWERUP, name, "PowerupGame/" + name);

		mEligibleGifts.push_back(gift);
	}

	int highestStage = VuGameManager::IF()->getHighestCarStage();

	int coinsAGift = VuGameUtil::IF()->constantDB()["PowerUpGame"]["CoinsA"][highestStage].asInt();
	int coinsBGift = VuGameUtil::IF()->constantDB()["PowerUpGame"]["CoinsB"][highestStage].asInt();

	// Pad the list with non-powerup choices if it's too short
	mEligibleGifts.push_back(Gift(GIFT_SC, "SC", "PowerupGame/CoinsA", coinsAGift));
	mEligibleGifts.push_back(Gift(GIFT_SC, "SC", "PowerupGame/CoinsB", coinsBGift));
	mEligibleGifts.push_back(Gift(GIFT_GREASEMONKEY, "GM", "PowerupGame/GreaseMonkey"));

	// Load in all the coconut animations
	loadAnimations();

	// Create the gifts we're going to give
	generateGifts();

	for (int i = 0; i <COCONUT_MAX; i++)
	{
		mPicked[i] = false;
	}

	mStartDelay = 1.0f;
	mPlayCoconutAnimsAtNextTick = false;
	mAnimDoneCounter = 0;
	mShuffleDone = false;

	playIdleCoconuts();
}

//*****************************************************************************
void VuPowerUpGameEntity::onGameRelease()
{
	for (auto anim : mShuffleAnimAssets)
	{
		for (int j = 0; j < COCONUT_MAX; j++)
		{
			VuAssetFactory::IF()->releaseAsset(anim.mpAsset[j]);
		}
	}

	for (auto asset : mLiftAnimAssets)
	{
		VuAssetFactory::IF()->releaseAsset(asset);
	}

	for (auto asset : mDropAnimAssets)
	{
		VuAssetFactory::IF()->releaseAsset(asset);
	}

	for (auto asset : mIdleAnimAssets)
	{
		VuAssetFactory::IF()->releaseAsset(asset);
	}

	for (int i = 0; i < COCONUT_MAX; i++)
	{
		if (mPowerupData[i].mPfxHandle != 0)
		{
			VuPfxManager::IF()->releaseEntity(mPowerupData[i].mPfxHandle, true);
		}
	}
}

//*****************************************************************************
void VuPowerUpGameEntity::generateGifts()
{
	for (int i = 0; i < COCONUT_MAX; i++)
	{
		int which = VuRand::global().range(0, (int)mEligibleGifts.size());

		Gift gift = mEligibleGifts[which];
		mEligibleGifts.erase(mEligibleGifts.begin() + which);

		mGifts.push_back(gift);
	}
}

//*****************************************************************************
void VuPowerUpGameEntity::initPowerups()
{
	for (int i = 0; i < COCONUT_MAX; i++)
	{
		PowerupData* pData = &mPowerupData[i];

		pData->mState = PowerupData::NONE;
		pData->mpLocator = VUNULL;
		pData->mPfxHandle = 0;
		pData->mScale = 0.0f;
		pData->mScaleVelocity = 0.0f;
		pData->mTargetScale = 1.0f;
		pData->mDelayTimer = mOpenBoingDelay;
	}
}

//*****************************************************************************
void VuPowerUpGameEntity::startPowerup(VUINT index)
{
	PowerupData* pData = &mPowerupData[index];

	if (pData->mPfxHandle == 0)
	{
		char locatorName[64];
		VU_SPRINTF(locatorName, 32, "PfxLoc%d", index + 1);

		VuScriptRef* pRef = mpScriptComponent->getRef(locatorName);
		VuEntity* pLocator = pRef->getRefEntity();

		VUUINT32 handle = VuPfxManager::IF()->createEntity(mGifts[index].mPfxName.c_str(), false);
		VuPfxEntity* pPfxEntity = VuPfxManager::IF()->getEntity(handle);

		VUASSERT(pPfxEntity != VUNULL, "Could not create Pfx Entity.");

		VuVector3 pos = pLocator->getTransformComponent()->getWorldPosition();
		pPfxEntity->getSystemInstance()->setPosition(pos);
		pPfxEntity->getSystemInstance()->start();

		pData->mpLocator = pLocator;
		pData->mPfxHandle = handle;
	}

	// Set up the PowerupData for "active"
	pData->mState = PowerupData::ACTIVE;
	pData->mDelayTimer = 0.0f;

}

//*****************************************************************************
void VuPowerUpGameEntity::preparePowerup(VUINT index, eBoing inOrOut)
{
	VUASSERT(index >= COCONUT_1 && index < COCONUT_MAX, "Illegal PowerupData index.");

	PowerupData* pData = &mPowerupData[index];

	pData->mState = PowerupData::DELAYING;

	switch (inOrOut)
	{
	case BOING_IN:
	{
		pData->mDelayTimer = mOpenBoingDelay;
		pData->mScale = 0.0f;
		pData->mTargetScale = 1.0f;
		pData->mScaleVelocity = 0.0f;
	}
	break;
	case BOING_OUT:
	{
		pData->mDelayTimer = mCloseBoingDelay;
		pData->mScale = 1.0f;
		pData->mTargetScale = 0.0f;
		pData->mScaleVelocity = 0.0f;
	}
	break;
	}
}

//*****************************************************************************
void VuPowerUpGameEntity::preparePowerups(eBoing inOrOut)
{
	for (int i = 0; i < COCONUT_MAX; i++)
	{
		preparePowerup(i, inOrOut);
	}
}

//*****************************************************************************
void VuPowerUpGameEntity::generateMoves()
{
	static const int scMaxMoves = 40;

	mMoves.clear();
	for (int i = 0; i < scMaxMoves; i++)
	{
		mMoves.push_back(VuRand::global().range(0, (int)mShuffleAnimAssets.size()));
	}
}

//*****************************************************************************
void VuPowerUpGameEntity::playInitialAnims()
{
	// Set initial timers
	preparePowerups(BOING_IN);

	// Lift the coconuts up to at the start
	playLiftAnims();
}

//*****************************************************************************
void VuPowerUpGameEntity::playIdleCoconuts()
{
	for (int i = COCONUT_1; i < COCONUT_MAX; i++)
	{
		VuAsset* pAsset = mIdleAnimAssets[i];

		if (pAsset)
		{
			VuParams params;
			params.addAsset(pAsset);
			params.addFloat(0.0f);			// start time
			params.addFloat(0.0f);			// blend time
			params.addFloat(1.0f);
			params.addBool(false);			// looping

			mpScriptComponent->getPlug(sCoconutPlugs[i])->execute(params);
		}
	}
}

//*****************************************************************************
void VuPowerUpGameEntity::playCoconutAnims()
{
	if (mCurMove >= (int)mMoves.size())
	{
		return;
	}

	for (int i = COCONUT_1; i < COCONUT_MAX; i++)
	{
		VuAsset* pAsset = mShuffleAnimAssets[mMoves[mCurMove]].mpAsset[i];

		if (pAsset)
		{
			VuParams params;
			params.addAsset(pAsset);
			params.addFloat(0.0f);			// start time
			params.addFloat(0.0f);			// blend time
			params.addFloat(mTimeFactor);
			params.addBool(false);			// looping

			mpScriptComponent->getPlug(sCoconutPlugs[i])->execute(params);
		}
	}
}

//*****************************************************************************
void VuPowerUpGameEntity::giveGift(eCoconut which)
{
	// Award the gift
	//
	Gift gift = mGifts[which];

	char str[256] = "";

	char* pType = "Unknown";

	switch (gift.mType)
	{
	case GIFT_SC:
		VuStringUtil::integerFormat(gift.mAmount, str, sizeof(str));
		pType = "SC";
		VuGameManager::IF()->addCurrencyEarned(VuGameManager::CT_STANDARD, gift.mAmount);
		break;
	case GIFT_POWERUP:
		VU_STRCPY(str, sizeof(str), VuStringDB::IF()->getString("PowerUp_" + gift.mName).c_str());
		pType = "PowerUp";
		VuGameManager::IF()->givePowerUp(gift.mName);
		break;
	case GIFT_GREASEMONKEY:
		VU_STRCPY(str, sizeof(str), VuStringDB::IF()->getString("TuneUp").c_str());
		pType = "TuneUp";
		VuGameManager::IF()->addTuneUps(1);
		break;
	default:
		VUASSERT(false, "Unknown gift type.");
	}

	VuGameFontMacros::IF()->setMacro("POWER_UP_GAME_REWARD", str);
	
	VU_SPRINTF(str, sizeof(str), "PowerUpGame_Reward_Text_%s", pType);
	VuGameFontMacros::IF()->setMacro("POWER_UP_GAME_REWARD_TEXT", VuStringDB::IF()->getString(str).c_str());

	VuProfileManager::IF()->save();
	VuCloudSaveManager::IF()->save();
}

//*****************************************************************************
VuRetVal VuPowerUpGameEntity::StartGame(const VuParams &params)
{
	VuRetVal retval;

	mState = DROPPING;

	// Create a list of moves
	generateMoves();

	// Start at first move
	mCurMove = 0;

	// Speed scaling
	mTimeFactor = 1.0f;

	playDropAnims();

	preparePowerups(BOING_OUT);

	return retval;
}

//*****************************************************************************
VuRetVal VuPowerUpGameEntity::CoconutsAnimDone(const VuParams &params)
{
	VuRetVal retval;

	if (++mAnimDoneCounter >= COCONUT_MAX)
	{
		mAnimDoneCounter = 0;
	}
	else
	{
		return retval;
	}

	if (mState == DROPPING)
	{
		mState = SHUFFLING;

		mPlayCoconutAnimsAtNextTick = true;
	}
	else if (mState == SHUFFLING)
	{
		if (++mCurMove >= (VUINT32)mMoves.size())
		{
			// Shuffling is finished
			mShuffleDone = true;
		}
		else
		{
			mTimeFactor = 3.0f - VuCos((float)mCurMove / (float)mMoves.size() * VU_2PI) * 2.0f;

			mPlayCoconutAnimsAtNextTick = true;
		}
	}

	return retval;
}

//*****************************************************************************
VuRetVal VuPowerUpGameEntity::PickAnimDone(const VuParams &params)
{
	VuRetVal retval;

	return retval;
}

//*****************************************************************************
void VuPowerUpGameEntity::pick(eCoconut which)
{
	if (mState != PICKING)
	{
		return;
	}

	VUASSERT((which >= COCONUT_1 && which < COCONUT_MAX), "Coconut index out of range.");

	if (mPicked[which])
	{
		return;
	}

	// We're on our second and third pick
	VUINT cost = mPickCost[mNumPicked];
		
	if (!VuGameManager::IF()->canAfford(0, cost))
	{
		return;
	}

	if (mNumPicked <= scNumPicks)
	{
		// Charge the user for the xtra spins
		VuGameManager::IF()->spendCurrency(0, cost);

		// Pick one
		mNumPicked++;

		// Been picked
		mPicked[which] = true;

		// Play the lift animation
		playLiftAnim(which);

		// Draw Powerups
		preparePowerup(which, BOING_IN);

		// Award the gift in game data
		giveGift(which);
	}
}

//*****************************************************************************
VuRetVal VuPowerUpGameEntity::Pick1(const VuParams &params)
{
	VuRetVal	retval;

	pick(COCONUT_1);

	return retval;

}

//*****************************************************************************
VuRetVal VuPowerUpGameEntity::Pick2(const VuParams &params)
{
	VuRetVal	retval;

	pick(COCONUT_2);

	return retval;

}

//*****************************************************************************
VuRetVal VuPowerUpGameEntity::Pick3(const VuParams &params)
{
	VuRetVal retval;

	pick(COCONUT_3);

	return retval;
}

// Outputs

//*****************************************************************************
VuRetVal VuPowerUpGameEntity::ShuffleDone(const VuParams &params)
{
	mState = PICKING;

	VuRetVal retval;

	// Shuffle rewards randomly
	std::vector<int> shuffleArray;
	shuffleArray.resize(mGifts.size());
	VuRand::global().createShuffleArray((int)shuffleArray.size(), &shuffleArray[0]);

	GiftList gifts = mGifts;
	for ( int i = 0; i < (int)mGifts.size(); i++ )
		mGifts[i] = gifts[shuffleArray[i]];


	// Reset the coconuts to where they're supposed to be
	playIdleCoconuts();

	// Tell the scripts that the shuffling is over
	mpScriptComponent->getPlug("ShuffleDone")->execute(params);

	return retval;
}

//*****************************************************************************
VuRetVal VuPowerUpGameEntity::CanAfford(const VuParams &params)
{
	VUINT cost = mPickCost[mNumPicked];

	// Costs zero secondary currency, and 'cost' primary currency
	bool canAfford = VuGameManager::IF()->canAfford(0, cost);

	// Return value to input plug for script testing
	return VuRetVal(canAfford);
}

//*****************************************************************************
void VuPowerUpGameEntity::playLiftAnim(eCoconut which)
{
	VuAsset *pAsset = VUNULL;

	pAsset = mLiftAnimAssets[which];

	if (pAsset)
	{
		VuParams params;
		params.addAsset(pAsset);
		params.addFloat(0.0f);	// startTime
		params.addFloat(0.0f);	// blendTime
		params.addFloat(1.0f);	// timeFactor
		params.addBool(false);	// looping

		mpScriptComponent->getPlug(sPickPlugs[which])->execute(params);
	}
}

//*****************************************************************************
void VuPowerUpGameEntity::playLiftAnims()
{
	VuAsset *pAsset = VUNULL;

	for (int i = COCONUT_1; i < COCONUT_MAX; i++)
	{
		playLiftAnim(static_cast<eCoconut>(i));
	}
}

//*****************************************************************************

void VuPowerUpGameEntity::playDropAnims()
{
	VuAsset *pAsset = VUNULL;

	for (int i = COCONUT_1; i < COCONUT_MAX; i++)
	{
		pAsset = mDropAnimAssets[i];
		if (pAsset)
		{
			VuParams params;
			params.addAsset(pAsset);
			params.addFloat(0.0f);	// startTime
			params.addFloat(0.0f);	// blendTime
			params.addFloat(1.0f);	// timeFactor
			params.addBool(false);	// looping

			mpScriptComponent->getPlug(sCoconutPlugs[i])->execute(params);
		}
	}
}
//*****************************************************************************
void VuPowerUpGameEntity::OnUITick(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	float fdt = accessor.getFloat();

	static const float scTempAngularFrequency = 2.0f;
	static const float scTempDampingRatio = 0.25f;

	if (mStartDelay > FLT_EPSILON)
	{
		mStartDelay -= fdt;

		if (mStartDelay < FLT_EPSILON)
		{
			mStartDelay = 0.0f;
			
			playInitialAnims();
		}
	}

	if (mPlayCoconutAnimsAtNextTick)
	{
		mPlayCoconutAnimsAtNextTick = false;

		playCoconutAnims();
	}

	if (mState == SHUFFLING && mShuffleDone)
	{
		mShuffleDone = false;
		VuParams params;
		ShuffleDone(params);
	}
	for (int i = 0; i < COCONUT_MAX; i++)
	{
		// Handle Start Delay
		PowerupData* pData = &mPowerupData[i];

		if (pData->mState == PowerupData::DELAYING)
		{	
			if (pData->mDelayTimer >= FLT_EPSILON)
			{
				pData->mDelayTimer -= fdt;
			}

			if (pData->mDelayTimer < 0.0f)
			{
				startPowerup(i);
				pData->mDelayTimer = 0.0f;
			}
		}

		// Update the Pfx entity with boinging if it's not in delay
		VuPfxEntity* pPfxEntity = VuPfxManager::IF()->getEntity(mPowerupData[i].mPfxHandle);
		if (pPfxEntity && pData && pData->mState != PowerupData::DELAYING)
		{
			VuMathUtil::calcDampedSimpleHarmonicMotion(&mPowerupData[i].mScale, &mPowerupData[i].mScaleVelocity, mPowerupData[i].mTargetScale, fdt, scTempAngularFrequency*VU_2PI, scTempDampingRatio);

			if (mPowerupData[i].mTargetScale < FLT_EPSILON && mPowerupData[i].mScale <= FLT_EPSILON)
			{
				VuPfxManager::IF()->releaseEntity(mPowerupData[i].mPfxHandle, true);

				pData->mState = PowerupData::DONE;
				pData->mPfxHandle = 0;
			}
			else
			{
				pPfxEntity->getSystemInstance()->setScale(mPowerupData[i].mScale);
			}
		}
	}
}
