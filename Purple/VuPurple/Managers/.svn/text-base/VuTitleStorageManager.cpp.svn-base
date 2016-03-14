//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  TitleStorageManager class
// 
//*****************************************************************************

#include "VuTitleStorageManager.h"
#include "VuGameServicesManager.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Net/VuHttpClient.h"
#include "VuEngine/Json/VuJsonContainer.h"
#include "VuEngine/Json/VuJsonWriter.h"


#define BASE_URL "https://vectorunit-bbr.appspot.com/title_storage"
//#define BASE_URL "http://localhost:8080/title_storage"


IMPLEMENT_SYSTEM_COMPONENT(VuTitleStorageManager, VuTitleStorageManager);

//*****************************************************************************
bool VuTitleStorageManager::init()
{
	VuTickManager::IF()->registerHandler(this, &VuTitleStorageManager::tickNetwork, "Network");

	return true;
}

//*****************************************************************************
void VuTitleStorageManager::release()
{
	if ( mLoadActions.size() )
		VUPRINTF("WARNING: Still loading from title storage!\n");

	if ( mSaveActions.size() )
		VUPRINTF("WARNING: Still saving to title storage!\n");

	VuTickManager::IF()->unregisterHandlers(this);
}

//*****************************************************************************
void VuTitleStorageManager::startLoad(const std::string &gamerID, const char *fileName, LoadCallback *pCB, int id)
{
	bool success = false;

	if ( VuGameServicesManager::IF()->isSignedIn() )
	{
		VUPRINTF("Start Title Storage Load: %s/%s/%s\n", VuGameServicesManager::IF()->getAuthType().c_str(), gamerID.c_str(), fileName);

		VuLoadAction action(pCB, id);

		// create http request
		action.mHttpRequest = VuHttpClient::IF()->createRequest();

		// URL
		char url[256] = BASE_URL"/load";

		// data
		VuJsonContainer data;
		data["AuthType"].putValue(VuGameServicesManager::IF()->getAuthType());
		data["AuthID"].putValue(gamerID);
		data["FileName"].putValue(fileName);

		std::string strData;
		VuJsonWriter writer;
		writer.configCompact(true);
		writer.saveToString(data, strData);

		// header
		VuHttpClient::IF()->setContentHeader(action.mHttpRequest, "Content-Type", "application/json");
		VuHttpClient::IF()->setContentHeader(action.mHttpRequest, "Content-Length", (int)strData.size());

		// send request
		VuHttpClient::IF()->postAsync(action.mHttpRequest, url, strData);

		// add to list
		mLoadActions.push_back(action);
	}
	else if ( pCB )
	{
		pCB->onTitleStorageLoadResult(RESULT_ERROR, "", id);
	}
}

//*****************************************************************************
void VuTitleStorageManager::startSave(const std::string &gamerID, const char *fileName, const std::string &fileData, SaveCallback *pCB, int id)
{
	bool success = false;

	if ( VuGameServicesManager::IF()->isSignedIn() )
	{
		VUPRINTF("Start Title Storage Save: %s/%s/%s\n", VuGameServicesManager::IF()->getAuthType().c_str(), gamerID.c_str(), fileName);

		VuSaveAction action(pCB, id);

		// create http request
		action.mHttpRequest = VuHttpClient::IF()->createRequest();

		// URL
		char url[256] = BASE_URL"/save";

		// data
		VuJsonContainer data;
		data["AuthType"].putValue(VuGameServicesManager::IF()->getAuthType());
		data["AuthID"].putValue(gamerID);
		data["FileName"].putValue(fileName);
		data["FileData"].putValue(fileData);

		std::string strData;
		VuJsonWriter writer;
		writer.configCompact(true);
		writer.saveToString(data, strData);

		// header
		VuHttpClient::IF()->setContentHeader(action.mHttpRequest, "Content-Type", "application/json");
		VuHttpClient::IF()->setContentHeader(action.mHttpRequest, "Content-Length", (int)strData.size());

		// send request
		VuHttpClient::IF()->postAsync(action.mHttpRequest, url, strData);

		// add to list
		mSaveActions.push_back(action);
	}
	else if ( pCB )
	{
		pCB->onTitleStorageSaveResult(RESULT_ERROR, id);
	}
}

//*****************************************************************************
void VuTitleStorageManager::tickNetwork(float fdt)
{
	{
		LoadActions::iterator iter = mLoadActions.begin();
		while ( iter != mLoadActions.end() )
		{
			VuLoadAction &action = *iter;

			VuHttpClient::eStatus status = VuHttpClient::IF()->getStatus(action.mHttpRequest);
			if ( status != VuHttpClient::STATUS_WAITING_FOR_RESPONSE )
			{
				if ( action.mpCB )
				{
					if ( status == VuHttpClient::STATUS_RESPONSE_RECEIVED )
					{
						const std::string &response = VuHttpClient::IF()->getResponse(action.mHttpRequest);
						if ( response == "NotFound" )
						{
							VUPRINTF("Title Storage Load Error:  Not found\n");
							action.mpCB->onTitleStorageLoadResult(RESULT_NOT_FOUND, "", action.mId);
						}
						else
						{
							VUPRINTF("Title Storage Load Success\n");
							action.mpCB->onTitleStorageLoadResult(RESULT_SUCCESS, response, action.mId);
						}
					}
					else
					{
						VUPRINTF("Title Storage Load Error:  No response\n");
						action.mpCB->onTitleStorageLoadResult(RESULT_ERROR, "", action.mId);
					}
				}

				VuHttpClient::IF()->releaseRequest(action.mHttpRequest);
				iter = mLoadActions.erase(iter);
			}
			else
			{
				iter++;
			}
		}
	}

	{
		SaveActions::iterator iter = mSaveActions.begin();
		while ( iter != mSaveActions.end() )
		{
			VuSaveAction &action = *iter;

			VuHttpClient::eStatus status = VuHttpClient::IF()->getStatus(action.mHttpRequest);
			if ( status != VuHttpClient::STATUS_WAITING_FOR_RESPONSE )
			{
				if ( action.mpCB )
				{
					if ( status == VuHttpClient::STATUS_RESPONSE_RECEIVED )
					{
						const std::string &response = VuHttpClient::IF()->getResponse(action.mHttpRequest);
						if ( response == "true" )
						{
							VUPRINTF("Title Storage Save Success\n");
							action.mpCB->onTitleStorageSaveResult(RESULT_SUCCESS, action.mId);
						}
						else
						{
							VUPRINTF("Title Storage Save Error:  Unknown\n");
							action.mpCB->onTitleStorageSaveResult(RESULT_ERROR, action.mId);
						}
					}
					else
					{
						VUPRINTF("Title Storage Save Error:  No response\n");
						action.mpCB->onTitleStorageSaveResult(RESULT_ERROR, action.mId);
					}
				}

				VuHttpClient::IF()->releaseRequest(action.mHttpRequest);
				iter = mSaveActions.erase(iter);
			}
			else
			{
				iter++;
			}
		}
	}
}
