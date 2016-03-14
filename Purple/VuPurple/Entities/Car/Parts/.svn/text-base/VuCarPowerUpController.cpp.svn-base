//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  CarPowerUpController class
// 
//*****************************************************************************

#include "VuCarPowerUpController.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarEffectController.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/PowerUps/VuPowerUpManager.h"
#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Dev/VuDevConfig.h"
#include "VuPurple/managers/VuBillingManager.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/HAL/Audio/VuAudio.h"
#include "VuPurple/Managers/VuAnalyticsManager.h"
#include "VuEngine/Util/VuStringUtil.h"
#include "VuPurple/Managers/VuGiftDialogManager.h"

//*****************************************************************************
void VuCarPowerUpControllerProcessor::onMessageBoxClosed(VuMessageBox *pMessageBox)
{
	if (pMessageBox->getResult() != "OnA")
	{
		if (VuBillingManager::IF()->getCurrentBillingType() == VuBillingManager::WEIXIN)
		{
			VuRequestOrderManager::IF()->setDelegate(VuBillingManager::IF());
			VuRequestOrderManager::IF()->startGetOrderId(m_itemName);
		}
		else
		{
			VuBillingManager::IF()->startPurchase(m_itemName);
		}
	}
}
//*****************************************************************************
VuCarPowerUpController::VuCarPowerUpController(VuCarEntity *pCar):
	mpCar(pCar),
	mSlotCount(1),
	mPickupTimer(0.0f),
	mNextInstanceId(1),
	mGameCharges(0),
	mFrontExtraPowerUpPrice1(0)
{
	// constants
	{
		const VuJsonContainer &constants = VuGameUtil::IF()->constantDB()["PowerUps"];

		mMaxSlotMachineTime = constants["MaxSlotMachineTime"].asFloat();
		mMinPickupInterval = constants["MinPickupInterval"].asFloat();
		m_processor = new VuCarPowerUpControllerProcessor;
		mPickupTimer = mMinPickupInterval;
	}

	// power up slots
	mSlots.resize(MAX_POWER_UP_SLOT_COUNT);
}

//*****************************************************************************
VuCarPowerUpController::~VuCarPowerUpController()
{
	if (m_processor)
	{
		delete m_processor;
		m_processor = NULL;
	}
}

//*****************************************************************************
void VuCarPowerUpController::onGameInitialize(int slotCount)
{
	mSlotCount = slotCount;

	if ( mpCar->getDriver()->isHuman() && VuDevConfig::IF()->hasParam("ForcePowerUp") )
		collectPowerUp(0xffffffff);

	const std::string &gameType = VuGameUtil::IF()->dataRead()["GameData"]["GameType"].asString();
	const VuJsonContainer &data = VuGameUtil::IF()->constantDB()["Games"][gameType];
	data["PowerUpCharges"].getValue(mGameCharges);
	const VuJsonContainer &gamePowerUps = data["PowerUps"];
	for ( int i = 0; i < gamePowerUps.size(); i++ )
	{
		const std::string &powerUpName = gamePowerUps[i].asString();
		if ( i == 0 || VuGameManager::IF()->isPowerUpOwned(powerUpName) )
			mGamePowerUps.push_back(powerUpName);
	}

	//xlm modify
	if (mpCar->getDriver()->isHuman())
	{
		mFrontExtraPowerUpName1 = "";
		mFrontExtraPowerUpPrice1 = 0;
		const VuJsonContainer &powerupData = VuDataUtil::findArrayMember(VuGameUtil::IF()->constantDB()["PowerUpConfig"], "RaceType", gameType+"_front");

		mFrontExtraPowerUpName1 = powerupData["PowerUp"].asString();
		mFrontExtraPowerUpPrice1 = powerupData["Price"].asInt();


		const VuPowerUp* pFrontPowerUp = VuPowerUpManager::IF()->getPowerUpByName(mFrontExtraPowerUpName1.c_str());

		if (pFrontPowerUp)
		{
			mFrontExtraPowerUpSlot1.mSlotMachineActive = true;
			mFrontExtraPowerUpSlot1.mpPowerUp = pFrontPowerUp;
			mFrontExtraPowerUpSlot1.mChargesRemaining = 1;
			mFrontExtraPowerUpSlot1.mInstanceId = 1;
		}
		else
		{
			mFrontExtraPowerUpSlot1.mSlotMachineActive = false;
		}


		mBehindExtraPowerUpName1 = "";
		mBehindExtraPowerUpPrice1 = 0;
		const VuJsonContainer &behindPowerData = VuDataUtil::findArrayMember(VuGameUtil::IF()->constantDB()["PowerUpConfig"], "RaceType", gameType+"_behind");

		mBehindExtraPowerUpName1 = behindPowerData["PowerUp"].asString();
		mBehindExtraPowerUpPrice1 = behindPowerData["Price"].asInt();


		const VuPowerUp* pBehindPowerUp = VuPowerUpManager::IF()->getPowerUpByName(mBehindExtraPowerUpName1.c_str());

		if (pBehindPowerUp)
		{
			mBehindExtraPowerUpSlot1.mSlotMachineActive = true;
			mBehindExtraPowerUpSlot1.mpPowerUp = pBehindPowerUp;
			mBehindExtraPowerUpSlot1.mChargesRemaining = 1;
			mBehindExtraPowerUpSlot1.mInstanceId = 2;
		}
		else
		{
			mBehindExtraPowerUpSlot1.mSlotMachineActive = false;
		}
	}
	



}

//*****************************************************************************
void VuCarPowerUpController::onGameRelease()
{
}

//*****************************************************************************
void VuCarPowerUpController::onTickDecision(float fdt)
{
	updateSlots(fdt);
}

//*****************************************************************************
bool VuCarPowerUpController::collectPowerUp(VUUINT32 mask)
{
	// check min time requirement
	if ( mPickupTimer >= mMinPickupInterval )
	{
		if ( mGameCharges )
			return collectPowerUpGame();
		else
			return collectPowerUpStandard(mask);
	}

	return false;
}

//*****************************************************************************
void VuCarPowerUpController::consumeNextPowerUp()
{
	Slot &slot = mSlots[0];
	if ( slot.mSlotMachineActive || slot.mChargesRemaining )
	{
		mSlots.pop_front();
		mSlots.push_back(Slot());
	}
}

//*****************************************************************************
void VuCarPowerUpController::updateSlots(float fdt)
{
	// to simplify logic, breaking this down into more clear cases, duplicating some code

	// handle slot-machine time-out
	for ( Slots::iterator iter = mSlots.begin(); iter != mSlots.end(); iter++ )
	{
		if ( iter->mSlotMachineActive )
		{
			iter->mSlotMachineTimer += fdt;
			if ( iter->mSlotMachineTimer >= mMaxSlotMachineTime )
			{
				iter->mSlotMachineActive = false;
			}
		}
	}

	// handle user input for slot 0
	if ( mpCar->getPowerUpControl() && mpCar->canUsePowerUp() )
	{
		Slot &slot = mSlots[0];
		if ( slot.mSlotMachineActive )
		{
			// user-interrupted slot machine on first slot
			slot.mSlotMachineActive = false;
		}
		else if ( slot.mChargesRemaining )
		{
			// perform action for powerup in first slot
			mpCar->getEffectController()->applyEffect(slot.mpPowerUp->mName.c_str(), VUNULL);
			VuAnalyticsManager::IF()->logEvent("useitem", slot.mpPowerUp->mName.c_str(), "1");

			slot.mChargesRemaining--;
			if ( slot.mChargesRemaining == 0 )
			{
				mSlots.pop_front();
				mSlots.push_back(Slot());
				mSlots[0].mSlotMachineActive = false;
			}

			mpCar->getStats().mPowerUpsUsedCount++;
		}
	}

	mPickupTimer += fdt;

	//handler extra powerup
	if (mpCar->getExtraPowerUpControl1() && mpCar->canUsePowerUp())
	{
		//判断钱是否足够，如果不够，则弹出购买窗口
		//int nNowPC = VuGameManager::IF()->getCurrency(VuGameManager::CT_PREMIUM);


		if (mFrontExtraPowerUpSlot1.mpPowerUp == NULL )
		{
			return;
		}

		const std::string &gameType = VuGameUtil::IF()->dataRead()["GameData"]["GameType"].asString();
		//获取当前场景中使用的额外道具信息
		std::string pGameType = gameType;
		VuCarEntity *pCar = VuCarManager::IF()->getLocalHumanCar(0);
		VuCarStats *stas = &pCar->getStats();

		bool bFront = false;
		if (stas->mPlace == 1)
		{
			pGameType = pGameType + "_front";
			bFront = true;
		}
		else
		{
			pGameType = pGameType + "_behind";
			bFront = false;
		}
		const VuJsonContainer &powerupData = VuDataUtil::findArrayMember(VuGameUtil::IF()->constantDB()["PowerUpConfig"], "RaceType", pGameType);

		int nPCCount = powerupData["Price"].asInt();

		if (nPCCount == 0)
		{
			return;
		}


		if (VuGameManager::IF()->canAfford(0, nPCCount))
		{
			//否则直接扣掉，
			VuGameManager::IF()->spendCurrency(0, nPCCount);
			if (bFront)
			{
				mpCar->getEffectController()->applyEffect(mFrontExtraPowerUpSlot1.mpPowerUp->mName.c_str(), VUNULL);

				VuAnalyticsManager::IF()->logEvent("useitem", mFrontExtraPowerUpSlot1.mpPowerUp->mName.c_str(), "1");
				if (mFrontExtraPowerUpSlot1.mSlotMachineActive)
				{
					//mFrontExtraPowerUpSlot1.mSlotMachineActive = false;
				}
			}
			else
			{
				mpCar->getEffectController()->applyEffect(mBehindExtraPowerUpSlot1.mpPowerUp->mName.c_str(), VUNULL);

				VuAnalyticsManager::IF()->logEvent("useitem", mBehindExtraPowerUpSlot1.mpPowerUp->mName.c_str(), "1");
				if (mBehindExtraPowerUpSlot1.mSlotMachineActive)
				{
					//mBehindExtraPowerUpSlot1.mSlotMachineActive = false;
				}
			}
		}
		else
		{
			if (!VuBillingManager::IF()->isBusy())
			{
				/*
				if (!VuTickManager::IF()->isPaused())
				{
					//VuTickManager::IF()->pushPauseRequest();
				}
				const VuJsonContainer &itemData = VuDataUtil::findArrayMember(VuGameUtil::IF()->storeDB(), "Name", "PremiumCurrencyD");

				std::string strNeedMoney = itemData["StubPrice"].asString();

				float needMoney = VuStringUtil::readFloat(strNeedMoney.c_str());

				VuBillingManager::IF()->startPurchaseInternal("PremiumCurrencyD", needMoney);
				*/

				VuDialog* mpDialog = VuDialogManager::IF()->create("Dialogs/Dialog_Buy_PC_small");

				VUASSERT(mpDialog != nullptr, "dialog could not be null");
				if (mpDialog != NULL)
				{
					mpDialog->setPauseGame(true);
				}
				return;


			}


		}




		
	}
}

//*****************************************************************************
bool VuCarPowerUpController::collectPowerUpGame()
{
	if ( mpCar->getDriver()->isHuman() && mGamePowerUps.size() )
	{
		if ( mSlotCount == 1 )
		{
			Slot &slot = mSlots[0];
			if ( !slot.mSlotMachineActive && slot.mChargesRemaining < mGameCharges )
			{
				if ( slot.mChargesRemaining == 0 )
				{
					const std::string &powerUpName = mGamePowerUps[VuRand::global().range(0, (int)mGamePowerUps.size())];
					const VuPowerUp *pPowerUp = VuPowerUpManager::IF()->getPowerUpByName(powerUpName.c_str());
					if ( pPowerUp )
					{
						slot.mpPowerUp = pPowerUp;
						slot.mInstanceId = mNextInstanceId++;
						//slot.mSlotMachineActive = true;
						//slot.mSlotMachineTimer = 0.0f;
					}
				}
				slot.mChargesRemaining = mGameCharges;
				mPickupTimer = 0.0f;

				return true;
			}
		}
		else
		{
			// check for available slot
			for ( int i = 0; i < mSlotCount; i++ )
			{
				Slot &slot = mSlots[i];
				if ( slot.mChargesRemaining == 0 && !slot.mSlotMachineActive )
				{
					const std::string &powerUpName = mGamePowerUps[VuRand::global().range(0, (int)mGamePowerUps.size())];
					const VuPowerUp *pPowerUp = VuPowerUpManager::IF()->getPowerUpByName(powerUpName.c_str());
					if ( pPowerUp )
					{
						slot.mpPowerUp = pPowerUp;
						slot.mInstanceId = mNextInstanceId++;
						slot.mChargesRemaining = mGameCharges;
						//slot.mSlotMachineActive = true;
						//slot.mSlotMachineTimer = 0.0f;
						mPickupTimer = 0.0f;
					}

					return true;
				}
			}
		}
	}

	return false;
}

//*****************************************************************************
bool VuCarPowerUpController::collectPowerUpStandard(VUUINT32 mask)
{
	// check for available slot
	for ( int i = 0; i < mSlotCount; i++ )
	{
		Slot &slot = mSlots[i];
		if ( slot.mChargesRemaining == 0 && !slot.mSlotMachineActive )
		{
			int charges;
			const VuPowerUp *pPowerUp = VuPowerUpManager::IF()->choosePowerUp(mpCar, mask, charges);
			if ( pPowerUp )
			{
				slot.mpPowerUp = pPowerUp;
				slot.mInstanceId = mNextInstanceId++;
				slot.mChargesRemaining = charges;
				slot.mSlotMachineActive = mpCar->getDriver()->isHuman();
				slot.mSlotMachineTimer = 0.0f;
				mPickupTimer = 0.0f;
			}

			return true;
		}
	}

	return false;
}
