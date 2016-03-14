//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  TipManager class
// 
//*****************************************************************************

#include "VuGiftDialogManager.h"
#include "VuEngine/Assets/VuAssetFactory.h"
#include "VuEngine/Assets/VuDBAsset.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Managers/VuProfileManager.h"
#include "VuEngine/Math/VuRand.h"
#include "VuEngine/Util/VuDataUtil.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuCarManager.h"
#include "VuStatsManager.h"
#include "VuEngine/Util/VuStringUtil.h"
#include "VuBillingManager.h"
#include "VuEngine/Net/VuHttpClient.h"
#include "VuEngine/Json/VuJsonWriter.h"
#include "VuEngine/Json/VuJsonReader.h"
#include "VuGameManager.h"
#include "VuPackManager.h"


#define giftDialogRequestIP "http://itcommon.miyigame.com:35001/getipinfo.php"

// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuGiftDialogManager, VuGiftDialogManager);

VuGiftDialogManager::VuGiftDialogManager() :mHttpRequest(NULL), currentDialogAsset()
{
	mSimTypeMap[1] = "yidong";
	mSimTypeMap[2] = "liantong";
	mSimTypeMap[3] = "dianxin";
}
//*****************************************************************************
void	VuGiftDialogManager::loadFromProfile()
{

}
//*****************************************************************************
void VuGiftDialogManager::postInit()
{
	mHttpRequest = VuHttpClient::IF()->createRequest();
	VuJsonContainer data;
	
	
	
#if defined VUANDROID 
	std::string simType = mSimTypeMap[VuBillingManager::IF()->getCurSimType()];
	data["gamename"].putValue("bbr");
	data["platform"].putValue(VuBillingManager::IF()->getSDKType().c_str());
	data["wirelesstype"].putValue(simType.c_str());
	data["version"].putValue(VuGameManager::IF()->getGiftDialogVersion());
#else
	data["gamename"].putValue("bbr");
	data["platform"].putValue("tencentApp");
	data["wirelesstype"].putValue("liantong");
	data["version"].putValue(VuGameManager::IF()->getGiftDialogVersion());
#endif
	

	std::string strData;
	VuJsonWriter writer;
	writer.configCompact(true);
	writer.saveToString(data, strData);

	//// header
	VuHttpClient::IF()->setContentHeader(mHttpRequest, "Content-Type", "application/json");
	VuHttpClient::IF()->setContentHeader(mHttpRequest, "Content-Length", (int)strData.size());
	//// send request
	VuHttpClient::IF()->postAsync(mHttpRequest, giftDialogRequestIP, strData);

	VuTickManager::IF()->registerHandler(this, &VuGiftDialogManager::tickWork, "Network");
}
//*****************************************************************************
void VuGiftDialogManager::tickWork(float fdt)
{
	if (mHttpRequest)
	{
		VuHttpClient::eStatus status = VuHttpClient::IF()->getStatus(mHttpRequest);
		
		if (status == VuHttpClient::STATUS_RESPONSE_RECEIVED)
		{
			VuJsonContainer response;
			VuJsonReader jsonReader;
			if (jsonReader.loadFromString(response, VuHttpClient::IF()->getResponse(mHttpRequest)))
			{
				VuJsonContainer itemInfo = response["iteminfo"];
				int version = response["version"].asInt();
				int size = itemInfo.numMembers();
				if (size != 0)
				{
					std::map<std::string, int> giftDialogStatusMap;
					std::map<std::string, int> giftDialogButtonStatusMap;
					
					for (int i = 0; i < size; i++)
					{
						std::string strItem = itemInfo.getMemberKey(i);
						const std::string strStatus = itemInfo[strItem].asCString();
						int giftstatus = VuStringUtil::readInt(strStatus.c_str());

						if (strItem.find("bbr_Dialog") != std::string::npos)
						{
							VuStringUtil::replace(strItem, "bbr_", "");
							giftDialogStatusMap[strItem] = giftstatus;
						}
						
						if (strItem.find("bbr_Button_") != std::string::npos)
						{
							VuStringUtil::replace(strItem, "bbr_Button_", "");
							giftDialogButtonStatusMap[strItem] = giftstatus;
						}
					}

					//跟新数据
					VuGameManager::IF()->setGiftDialogStates(giftDialogStatusMap, giftDialogButtonStatusMap,version);

				}
				VuHttpClient::IF()->releaseRequest(mHttpRequest);
			}
		}
	}
	else
	{
		VuTickManager::IF()->unregisterHandlers(this);
	}
}
//*****************************************************************************
bool VuGiftDialogManager::init()
{
	return true;
}
//*****************************************************************************
void VuGiftDialogManager::release()
{

}
//*****************************************************************************
void	VuGiftDialogManager::onDialogClosed(VuDialog *pDialog)
{
	if (pDialog->getResult() == "CanNotAfford")
	{
		VuDialog* mpDialog = VuDialogManager::IF()->create("Dialogs/Dialog_Buy_PC_small");

		VUASSERT(mpDialog != nullptr, "dialog could not be null");
		if (mpDialog != NULL)
		{
			mpDialog->setCallback(this);
			mpDialog->setPauseGame(false);
		}
		return;
	}

	if (pDialog->getProjectAssetStr() == "Dialogs/Dialog_Buy_PC_small" && pDialog->getResult() == "OnCancel")
	{
		if (currentDialogAsset != "")
		{
			VuDialog* mpDialog = VuDialogManager::IF()->create(currentDialogAsset.c_str());

			VUASSERT(mpDialog != nullptr, "dialog could not be null");
			if (mpDialog != NULL)
			{
				mpDialog->setCallback(this);
				mpDialog->setPauseGame(false);
			}
			
		}
		return;
	}
}
std::string	VuGiftDialogManager::getDialogInPlayerHub()
{
	return "Dialogs/Dialog_SCAndPCPack";

	int playerTimes = 0;
	VuStatsManager::IF()->getEventTimes("PlayerHub", playerTimes);

	std::vector<std::string> DialogVec;
	
	std::string superVipItem = "SuperVipPack";
	std::string disabelAdsItem = "DisableAds";
	std::string propsPackItem = "PropsPack";
	std::string tuhaoPackItem = "TuhaoPack";

	if (!VuBillingManager::IF()->isOwned(superVipItem.c_str()))
	{
		DialogVec.push_back(superVipItem);
	}

	if (!VuBillingManager::IF()->isOwned(disabelAdsItem.c_str()))
	{
		DialogVec.push_back(disabelAdsItem);
	}
	if (!VuBillingManager::IF()->isOwned(propsPackItem.c_str()))
	{
		DialogVec.push_back(propsPackItem);
	}

	if (DialogVec.size() == 0)
	{
		if (!VuBillingManager::IF()->isOwned(tuhaoPackItem.c_str()))
		{
			return "Dialogs/Dialog_Tuhao_PackByPC";
		}
		else
		{
			return "Dialogs/Dialog_Buy_TuneUps_Pack";
		}
	}
	else
	{
		int index = playerTimes % DialogVec.size();
		if (DialogVec[index] == disabelAdsItem)
		{
			return "Dialogs/Dialog_DisableAdsPack";
		}
		else if (DialogVec[index] == superVipItem)
		{
			return "Dialogs/Dialog_SuperVip_Pack";
		}
		else if (DialogVec[index] == propsPackItem)
		{
			return "Dialogs/Dialog_PropsPack";
		}
		else
		{
			return "";
		}
	}
}

std::string	VuGiftDialogManager::getDialogInUpgradeCar()
{
	int playerTimes = 0;
	VuStatsManager::IF()->getEventTimes("UpgradeCar", playerTimes);

	if (playerTimes % 2 == 0)
	{
		return "Dialogs/Dialog_SCAndPCPack";
	}
	else
	{
		return "";
	}
}

std::string			VuGiftDialogManager::getDialogInLounge()
{
	int playerTimes = 0;
	VuStatsManager::IF()->getEventTimes("InLounge", playerTimes);

	if (playerTimes % 2 == 0)
	{
		std::vector<std::string> DialogVec;

		std::string  EuroItem = "Euro";
		std::string MuscleItem = "Muscle";
		std::string MoonItem = "Moon";
		std::string TuhaoItem = "TuhaoPack";

		if (VuGameManager::IF()->getCar(EuroItem).mIsOwned == false)
		{
			DialogVec.push_back(EuroItem);
		}
		else if (VuGameManager::IF()->getCar(MuscleItem).mIsOwned == false)
		{
			DialogVec.push_back(MuscleItem);
		}
		else  if (VuGameManager::IF()->getCar(MoonItem).mIsOwned == false)
		{
			DialogVec.push_back(MoonItem);
		}

		if (!VuBillingManager::IF()->isOwned(TuhaoItem.c_str()))
		{
			DialogVec.push_back(TuhaoItem);
		}

		if (DialogVec.size() > 0)
		{
			int index = (playerTimes / 2) % DialogVec.size();
			if (DialogVec[index] == EuroItem)
			{
				return "Dialogs/Dialog_Euro";
			}
			else if (DialogVec[index] == MuscleItem)
			{
				return "Dialogs/Dialog_Muscle";
			}
			else if (DialogVec[index] == MoonItem)
			{
				return "Dialogs/Dialog_Moon";
			}
			else if (DialogVec[index] == TuhaoItem)
			{
				return "Dialogs/Dialog_Tuhao_PackByPC";
			}
		}
		else
		{
			return "";
		}

	
	}
	else
	{
		return "";
	}
}


std::string			VuGiftDialogManager::getDialogInShowRoom()
{
	int playerTimes = 0;
	VuStatsManager::IF()->getEventTimes("InShowRoom", playerTimes);

	if (playerTimes % 2 == 0)
	{
		std::vector<std::string> DialogVec;

		std::string  EuroItem = "Euro";
		std::string MuscleItem = "Muscle";
		std::string MoonItem = "Moon";
		std::string SuperVipItem = "SuperVipPack";

		if (VuGameManager::IF()->getCar(EuroItem).mIsOwned == false)
		{
			DialogVec.push_back(EuroItem);
		}
		else if (VuGameManager::IF()->getCar(MuscleItem).mIsOwned == false)
		{
			DialogVec.push_back(MuscleItem);
		}
		else  if (VuGameManager::IF()->getCar(MoonItem).mIsOwned == false)
		{
			DialogVec.push_back(MoonItem);
		}

		if (!VuBillingManager::IF()->isOwned(SuperVipItem.c_str()))
		{
			DialogVec.push_back(SuperVipItem);
		}

		if (DialogVec.size() > 0)
		{
			int index = (playerTimes / 2) % DialogVec.size();
			if (DialogVec[index] == EuroItem)
			{
				return "Dialogs/Dialog_Euro";
			}
			else if (DialogVec[index] == MuscleItem)
			{
				return "Dialogs/Dialog_Muscle";
			}
			else if (DialogVec[index] == MoonItem)
			{
				return "Dialogs/Dialog_Moon";
			}
			else if (DialogVec[index] == SuperVipItem)
			{
				return "Dialogs/Dialog_SuperVip_Pack";
			}


		}
		else
		{
			return "";
		}

		return "Dialogs/Dialog_SCAndPCPack";
	}
	else
	{
		return "";
	}
}


std::string	VuGiftDialogManager::getDialogBeforeGame()
{
	//第一章第二关卡特殊处理
	const std::string strEvent = VuGameUtil::IF()->getEventName();
	if (strEvent == "SeriesA_Event02")
	{
		if (VuGameManager::IF()->getCar("MonsterTruck").mIsOwned == false)
		{
			return "Dialogs/Dialog_DisableAdsPack";
		}
		else
		{
			return "";
		}
	}

	//
	int enterTimes = 0;
	VuStatsManager::IF()->getEventTimes("EnterGame",enterTimes);
	if (enterTimes % 3 == 0 && enterTimes != 0)
	{
		std::vector<std::string> DialogVec;

		std::string SCAndPCPackItem = "SCAndPCPack";
		std::string EuroPackItem = "EuroPack";
		std::string MusclePackItem = "MusclePack";
		std::string MoonPackItem = "MoonPack";
		std::string DisableAdsItem = "DisableAds";

		if (VuGameManager::IF()->getCar("Euro").mIsOwned == false)
		{
			DialogVec.push_back(EuroPackItem);
		}
		else if (VuGameManager::IF()->getCar("Muscle").mIsOwned == false)
		{
			DialogVec.push_back(MusclePackItem);
		}
		else  if (VuGameManager::IF()->getCar("Moon").mIsOwned == false)
		{
			DialogVec.push_back(MoonPackItem);
		}

		//取VIP 礼包

		if (VuGameManager::IF()->getCar("MonsterTruck").mIsOwned == false)
		{
			DialogVec.push_back(DisableAdsItem);
		}

		//取钻石+金币礼包
		DialogVec.push_back(SCAndPCPackItem);

		if (DialogVec.size() > 1) //
		{
			int index = (enterTimes / 3) % DialogVec.size();
			if (DialogVec[index] == SCAndPCPackItem)
			{
				return "Dialogs/Dialog_SCAndPCPack";
			}
			else if (DialogVec[index] == DisableAdsItem)
			{
				return "Dialogs/Dialog_DisableAdsPack";
			}
			else  if (DialogVec[index] == EuroPackItem)
			{
				return "Dialogs/Dialog_EuroPack";
			}
			else  if (DialogVec[index] == MusclePackItem)
			{
				return "Dialogs/Dialog_MusclePack";
			}
			else  if (DialogVec[index] == MoonPackItem)
			{
				return "Dialogs/Dialog_MoonPack";
			}
			else
			{
				return "";
			}
		}
		else //如果赛车礼包 和 VIP礼包都已经被买了，则不弹
		{
			return "";
		}

		/*
		std::string superVipItem = "SuperVipPack";
		std::string disabelAdsItem = "DisableAds";
		std::string propsPackItem = "PropsPack";
		std::string tuhaoPackItem = "TuhaoPack";

		if (!VuBillingManager::IF()->isOwned(superVipItem.c_str()))
		{
			DialogVec.push_back(superVipItem);
		}

		if (!VuBillingManager::IF()->isOwned(disabelAdsItem.c_str()))
		{
			DialogVec.push_back(disabelAdsItem);
		}
		if (!VuBillingManager::IF()->isOwned(propsPackItem.c_str()))
		{
			DialogVec.push_back(propsPackItem);
		}

		if (DialogVec.size() == 0)
		{
			if (!VuBillingManager::IF()->isOwned(tuhaoPackItem.c_str()))
			{
				return "Dialogs/Dialog_Tuhao_PackByPC";
			}
			else
			{
				return "Dialogs/Dialog_Buy_TuneUps_Pack";
			}
		}
		else
		{
			int index = (enterTimes/3) % DialogVec.size();
			if (DialogVec[index] == disabelAdsItem)
			{
				return "Dialogs/Dialog_DisableAdsPack";
			}
			else if (DialogVec[index] == superVipItem)
			{
				return "Dialogs/Dialog_SuperVip_Pack";
			}
			else if (DialogVec[index] == propsPackItem)
			{
				return "Dialogs/Dialog_PropsPack";
			}
			else
			{
				return "";
			}
		}
		*/

	}
	else
	{
		return "";
	}
}

void VuGiftDialogManager::recordDialogBeforeGame()
{
	VuStatsManager::IF()->recordEnterEventTimes("EnterGame");
}

void VuGiftDialogManager::recordDialogInPlayerHub()
{
	VuStatsManager::IF()->recordEnterEventTimes("PlayerHub");
}
void VuGiftDialogManager::recordDialogInUpgradeCar()
{
	VuStatsManager::IF()->recordEnterEventTimes("UpgradeCar");
}
void VuGiftDialogManager::recordDialogInLounge()
{
	VuStatsManager::IF()->recordEnterEventTimes("InLounge");
}

void VuGiftDialogManager::recordDialogInShowRoom()
{
	VuStatsManager::IF()->recordEnterEventTimes("InShowRoom");
}

std::string	VuGiftDialogManager::getPopDialogs()
{
	const std::string strEvent = VuGameUtil::IF()->getEventName();
	do 
	{
		int place = 0;
		std::string	mGameType = VuGameUtil::IF()->getGameType();;
		if (VuCarManager::IF()->getLocalHumanCarCount())
		{
			VuCarEntity *pCar = VuCarManager::IF()->getLocalHumanCar(0);
			place = pCar->getStats().mPlace;
		}
		else
		{
			break;
		}

		//删除新手礼包
		/*if (strEvent == "SeriesA_Event01" || strEvent == "SeriesA_Event02")
		{
		if (place > 1 &&(!VuBillingManager::IF()->isOwned("StarterPackEx")))
		{
		return "Dialogs/Dialog_StarterPackEx";
		}
		}*/

		int failTimes = 0;
		VuStatsManager::IF()->getFailTimes(strEvent.c_str(), failTimes);

		std::string popDialogRaw;
		bool bFailed = false;
		if (place <= 3)
		{
			if (mGameType == "BossBattle" && place != 1)
			{
				bFailed = true;
			}
		}
		else
		{
			bFailed = true;
		}

		if (bFailed == true)
		{
			/*
			std::vector<std::string> DialogVec;
			std::string superVipItem = "SuperVipPack";
			std::string DisabelAdsItem = "DisableAds";

			if (!VuBillingManager::IF()->isOwned(superVipItem.c_str()))
			{
				DialogVec.push_back(superVipItem);
			}

			if (!VuBillingManager::IF()->isOwned(DisabelAdsItem.c_str()))
			{
				DialogVec.push_back(DisabelAdsItem);
			}

			if (DialogVec.size() == 0)
			{
				popDialogRaw = "WKPack";
			}
			else
			{
				int index = failTimes%DialogVec.size();
				popDialogRaw = DialogVec[index];
			}
			*/
			popDialogRaw = "SDAndPCPack";
		}
		else
		{
			popDialogRaw = "";
		}

		
		if (popDialogRaw == "")
		{
			return "";
		}
		else if (popDialogRaw == "SDAndPCPack")
		{
			return "Dialogs/Dialog_SCAndPCPack";
		}
		

	} while (0);
	
	return "";
}

void VuGiftDialogManager::processDiscountDialog(std::string& dialogStr,std::string& itemStr, std::string discountDialog)
{

	std::vector<std::string> strPopDialogResult;
	VuStringUtil::tokenize(discountDialog, ',', strPopDialogResult);
	if (strPopDialogResult.size() != 2)
	{
		return;
	}
	std::string strDialog = strPopDialogResult[0];
	std::string strBillingItem = strPopDialogResult[1];
	if (dialogStr == "Dialogs/Dialog_Buy_MonsterTruck_Pack_Discount" || dialogStr == "Dialogs/Dialog_SuperVip_Pack")
	{
		if (dialogStr == "Dialogs/Dialog_Buy_MonsterTruck_Pack_Discount")
		{
			VuPackManager::IF()->triggerLanXiangPack();
			if (!VuPackManager::IF()->isPackOk(VuPackManager::Pack_LanXiang))
			{
				dialogStr = strDialog;
				itemStr = strBillingItem;
			}
		}
		else
		{
			VuPackManager::IF()->triggerSuperVipPack();
			if (!VuPackManager::IF()->isPackOk(VuPackManager::Pack_SuperVip))
			{
				dialogStr = strDialog;
				itemStr = strBillingItem;
			}
		}
	}
}

void VuGiftDialogManager::popUpGiftDialog(std::string dialogStr)
{
	std::string popDialogStr = getPopDialogs();
	if (popDialogStr == "")
	{
		return;
	}
	
	VuDialog* mpDialog = VuDialogManager::IF()->create(popDialogStr.c_str());

	VUASSERT(mpDialog != nullptr, "dialog could not be null");
	if (mpDialog != NULL)
	{
		currentDialogAsset = popDialogStr;
		mpDialog->setCallback(this);
		mpDialog->setPauseGame(false);
	}
}
void VuGiftDialogManager::popUpDialogInPlayerHub()
{
	std::string popDialogStr = getDialogInPlayerHub();
	if (popDialogStr == "")
	{
		return;
	}

	VuDialog* mpDialog = VuDialogManager::IF()->create(popDialogStr.c_str());

	VUASSERT(mpDialog != nullptr, "dialog could not be null");
	if (mpDialog != NULL)
	{
		currentDialogAsset = popDialogStr;
		mpDialog->setCallback(this);
		mpDialog->setPauseGame(false);
	}
}

void VuGiftDialogManager::popUpDialogInUpgradeCar()
{
	std::string popDialogStr = getDialogInUpgradeCar();
	if (popDialogStr == "")
	{
		return;
	}

	VuDialog* mpDialog = VuDialogManager::IF()->create(popDialogStr.c_str());

	VUASSERT(mpDialog != nullptr, "dialog could not be null");
	if (mpDialog != NULL)
	{
		currentDialogAsset = popDialogStr;
		mpDialog->setCallback(this);
		mpDialog->setPauseGame(false);
	}
}


//车手界面弹框
void VuGiftDialogManager::popUpDialogInLounge()
{
	std::string popDialogStr = getDialogInLounge();
	if (popDialogStr == "")
	{
		return;
	}

	VuDialog* mpDialog = VuDialogManager::IF()->create(popDialogStr.c_str());

	VUASSERT(mpDialog != nullptr, "dialog could not be null");
	if (mpDialog != NULL)
	{
		currentDialogAsset = popDialogStr;
		mpDialog->setCallback(this);
		mpDialog->setPauseGame(false);
	}
}

void VuGiftDialogManager::popUpDialogInShowRoom()
{
	std::string popDialogStr = getDialogInShowRoom();
	if (popDialogStr == "")
	{
		return;
	}

	VuDialog* mpDialog = VuDialogManager::IF()->create(popDialogStr.c_str());

	VUASSERT(mpDialog != nullptr, "dialog could not be null");
	if (mpDialog != NULL)
	{
		currentDialogAsset = popDialogStr;
		mpDialog->setCallback(this);
		mpDialog->setPauseGame(false);
	}
}


void VuGiftDialogManager::popUpDialogBeforGame()
{
	std::string popDialogStr = getDialogBeforeGame();
	if (popDialogStr == "")
	{
		return;
	}

	VuDialog* mpDialog = VuDialogManager::IF()->create(popDialogStr.c_str());

	VUASSERT(mpDialog != nullptr, "dialog could not be null");
	if (mpDialog != NULL)
	{
		currentDialogAsset = popDialogStr;
		mpDialog->setCallback(this);
		mpDialog->setPauseGame(true);
	}
}


