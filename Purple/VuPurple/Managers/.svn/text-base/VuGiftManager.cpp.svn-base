//*****************************************************************************
//
//  Copyright (c) 2012-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  GiftManager class
// 
//*****************************************************************************

#include "VuGiftManager.h"
#include "VuPurple/Managers/VuCloudSaveManager.h"
#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Managers/VuBillingManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Managers/VuProfileManager.h"
#include "VuEngine/Managers/VuMessageBoxManager.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Net/VuHttpClient.h"
#include "VuEngine/Json/VuJsonContainer.h"
#include "VuEngine/Json/VuJsonReader.h"
#include "VuEngine/Json/VuJsonWriter.h"
#include "VuEngine/DB/VuStringDB.h"
#include "VuEngine/Util/VuStringUtil.h"


// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuGiftManager, VuGiftManager);

#define MIN_MESSAGE_TIME 1.0f // seconds


//*****************************************************************************
VuGiftManager::VuGiftManager():
	mHttpRequest(VUNULL),
	mpMessageBox(VUNULL)
{
}

//*****************************************************************************
bool VuGiftManager::init()
{
	// create FSM
	{
		VuFSM::VuState *pState = mFSM.addState("Idle");

		pState = mFSM.addState("RedeemCode");
		pState->setEnterMethod(this, &VuGiftManager::onRedeemCodeEnter);
		pState->setExitMethod(this, &VuGiftManager::onRedeemCodeExit);
		pState->setTickMethod(this, &VuGiftManager::onRedeemCodeTick);

		pState = mFSM.addState("Idle");

		// transitions
		mFSM.addTransition("Idle", "RedeemCode", "Start");

		mFSM.addTransition("RedeemCode", "Idle", "Done");
	}

	mFSM.begin();

	VuTickManager::IF()->registerHandler(this, &VuGiftManager::tick, "Final");

	return true;
}

//*****************************************************************************
void VuGiftManager::release()
{
	VuTickManager::IF()->unregisterHandlers(this);

	mFSM.end();
}

//*****************************************************************************
void VuGiftManager::redeemCode(const std::string &code)
{
	if ( code.length() == CODE_LENGTH )
	{
		mCode = code;
		mFSM.pulseCondition("Start");
	}
	else
	{
		VuMessageBoxParams messageBoxParams;
		messageBoxParams.mType = "SimpleA";
		messageBoxParams.mHeading = "Store_RedeemCode";
		messageBoxParams.mTextA = "Common_OK";
		messageBoxParams.mBody = "Store_RedeemCode_FailureCode";

		VuMessageBox *pMB = VuMessageBoxManager::IF()->create(messageBoxParams);
		pMB->removeRef();
	}
}

//*****************************************************************************
bool VuGiftManager::isBusy()
{
	bool idle = mFSM.getCurState()->getName().compare("Idle") == 0;

	return !idle;
}

//*****************************************************************************
void VuGiftManager::tick(float fdt)
{
	mFSM.evaluate();
	mFSM.tick(fdt);
}

//*****************************************************************************
void VuGiftManager::onRedeemCodeEnter()
{
	// create http request
	mHttpRequest = VuHttpClient::IF()->createRequest();

	// URL
	char url[256] = "https://vectorunit-cloud.appspot.com/gifts";

	// data
	VuJsonContainer data;
	data["Code"].putValue(mCode);

	std::string strData;
	VuJsonWriter writer;
	writer.configCompact(true);
	writer.saveToString(data, strData);

	// header
	VuHttpClient::IF()->setContentHeader(mHttpRequest, "Content-Type", "application/json");
	VuHttpClient::IF()->setContentHeader(mHttpRequest, "Content-Length", (int)strData.size());

	// send request
	VuHttpClient::IF()->postAsync(mHttpRequest, url, strData);

	// create message box
	VuMessageBoxParams messageBoxParams;
	messageBoxParams.mType = "SimpleWait";
	messageBoxParams.mHeading = "Store_RedeemCode";
	messageBoxParams.mBody = "Store_RedeemCode_Busy";

	mpMessageBox = VuMessageBoxManager::IF()->create(messageBoxParams);
}

//*****************************************************************************
void VuGiftManager::onRedeemCodeExit()
{
	VuMessageBoxManager::IF()->destroy(mpMessageBox);
	mpMessageBox = VUNULL;
}

//*****************************************************************************
void VuGiftManager::onRedeemCodeTick(float fdt)
{
	if ( mHttpRequest )
	{
		VuHttpClient::eStatus status = VuHttpClient::IF()->getStatus(mHttpRequest);
		if ( status != VuHttpClient::STATUS_WAITING_FOR_RESPONSE )
		{
			bool success = false;
			std::string message = "Store_RedeemCode_FailureCode";

			if ( status == VuHttpClient::STATUS_RESPONSE_RECEIVED )
			{
				VuJsonContainer response;
				VuJsonReader jsonReader;
				if ( jsonReader.loadFromString(response, VuHttpClient::IF()->getResponse(mHttpRequest)) )
				{
					std::string itemName = response["Item"].asString();

					const VuJsonContainer &itemData = VuDataUtil::findArrayMember(VuGameUtil::IF()->storeDB(), "Name", itemName.c_str());
					if ( itemData.isObject() )
					{
						message = VuStringDB::IF()->getString("Store_RedeemCode_Success");
						std::string itemText = VuStringDB::IF()->getString("StoreItem_" + itemName);
						VuStringUtil::replace(message, "[ITEM_NAME]", itemText.c_str());

						VuBillingManager::IF()->receiveGift(itemName.c_str());

						success = true;
					}
				}
			}
			else
			{
				message = "Store_RedeemCode_FailureNetwork";
			}

			VuHttpClient::IF()->releaseRequest(mHttpRequest);
			mHttpRequest = VUNULL;

			VuMessageBoxParams messageBoxParams;
			messageBoxParams.mType = "SimpleA";
			messageBoxParams.mHeading = "Store_RedeemCode";
			messageBoxParams.mTextA = "Common_OK";
			messageBoxParams.mBody = message;

			VuMessageBox *pMB = VuMessageBoxManager::IF()->create(messageBoxParams);
			pMB->removeRef();

			VuParams params;
			params.addBool(success);
			VuEventManager::IF()->broadcast("OnRedeemCodeResult", params);
		}
	}
	else if ( mFSM.getTimeInState() > MIN_MESSAGE_TIME )
	{
		mFSM.pulseCondition("Done");
	}
}
