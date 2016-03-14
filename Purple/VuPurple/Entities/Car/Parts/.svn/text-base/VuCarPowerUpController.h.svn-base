//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  CarPowerUpController class
// 
//*****************************************************************************

#pragma once
#include "VuEngine\Managers\VuMessageBoxManager.h"
#include "VuPurple\Managers\VuCarManager.h"

class VuCarEntity;
class VuPowerUp;


class VuCarPowerUpControllerProcessor :public VuMessageBox::Callback
{
public:
	virtual void onMessageBoxClosed(VuMessageBox *pMessageBox);
	void setItemName(std::string itemName) { m_itemName = itemName; }
protected:
	std::string m_itemName;
};




class VuCarPowerUpController
{
public:
	VuCarPowerUpController(VuCarEntity *pCar);
	~VuCarPowerUpController();

	void			onGameInitialize(int slotCount);
	void			onGameRelease();

	void			onTickDecision(float fdt);

	enum { MAX_POWER_UP_SLOT_COUNT = 3 };

	struct Slot
	{
		Slot() : mpPowerUp(VUNULL), mInstanceId(0), mChargesRemaining(0), mSlotMachineActive(false), mSlotMachineTimer(0.0f) {}
		const VuPowerUp	*mpPowerUp;
		VUUINT32		mInstanceId;
		int				mChargesRemaining;
		bool			mSlotMachineActive;
		float			mSlotMachineTimer;
	};
	bool			collectPowerUp(VUUINT32 mask);
	const Slot		&getSlot(int slot) { return mSlots[slot]; }

	//xlm	
	const Slot&		GetExtraSlot() { 
		VuCarEntity *pCar = VuCarManager::IF()->getLocalHumanCar(0);
		if (pCar)
		{
			VuCarStats *stas = &pCar->getStats();
			if (stas->mPlace == 1)
			{
				return mFrontExtraPowerUpSlot1;
			}
			else
			{
				return mBehindExtraPowerUpSlot1;
			}
		}
		else
		{
			return mFrontExtraPowerUpSlot1;
		}
		
	}
	const Slot&		GetExtraFrontSlot() {
		return mFrontExtraPowerUpSlot1;
	}
	const Slot&		GetExtraBehindSlot() {
		return mBehindExtraPowerUpSlot1;
	}

	const std::string& GetExtraPowerupName() {
		VuCarEntity *pCar = VuCarManager::IF()->getLocalHumanCar(0);
		VuCarStats *stas = &pCar->getStats();
		if (stas->mPlace == 1)
		{
			return mFrontExtraPowerUpName1;
		}
		else
		{
			return mBehindExtraPowerUpName1;
		}
	}
	const int		GetExtraPowerupPrice() 
	{ 
		VuCarEntity *pCar = VuCarManager::IF()->getLocalHumanCar(0);
		VuCarStats *stas = &pCar->getStats();
		if (stas->mPlace == 1)
		{
			return mFrontExtraPowerUpPrice1;
		}
		else
		{
			return mBehindExtraPowerUpPrice1;
		}
		
	}

	void			consumeNextPowerUp();

private:
	void			updateSlots(float fdt);

	bool			collectPowerUpGame();
	bool			collectPowerUpStandard(VUUINT32 mask);

	VuCarEntity		*mpCar;

	typedef std::deque<Slot> Slots;
	int				mSlotCount;
	Slots			mSlots;

	//modify by xlm
	Slot			mFrontExtraPowerUpSlot1;   //left slot
	std::string     mFrontExtraPowerUpName1;
	int				mFrontExtraPowerUpPrice1;

	Slot			mBehindExtraPowerUpSlot1;   //left slot
	std::string     mBehindExtraPowerUpName1;
	int				mBehindExtraPowerUpPrice1;


	VuCarPowerUpControllerProcessor* m_processor;

	//----end xlm modify

	float			mPickupTimer;
	float			mMaxSlotMachineTime;
	float			mMinPickupInterval;
	VUUINT32		mNextInstanceId;

	typedef std::vector<std::string> GamePowerUps;
	GamePowerUps	mGamePowerUps;
	int				mGameCharges;
};
