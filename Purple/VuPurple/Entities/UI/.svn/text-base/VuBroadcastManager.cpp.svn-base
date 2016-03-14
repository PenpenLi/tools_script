#include "VuBroadcastManager.h"
#include "VuEngine\Net\VuHttpClient.h"
#include "VuEngine\Managers\VuTickManager.h"
#include "VuEngine\Json\VuJsonContainer.h"
#include "VuEngine\Json\VuJsonReader.h"
#include "..\..\Managers\VuGameManager.h"

#define BASE_URL "http://172.16.3.21:8080/bbr_test/news.json"

IMPLEMENT_SYSTEM_COMPONENT(VuBroadcastManager, VuBroadcastManager);

VuBroadcastManager::VuBroadcastManager():
mHttpRequest(NULL),
currentRequestIndex(0),
currentRequestNameIndex(0),
mIsPicDataOK(false),
mIsNamePicDataOK(false),
requestStates(REQUEST_JSON),
mVersion(0),
mBroadcastDataSize(0)
{

}
VuBroadcastManager::~VuBroadcastManager()
{
	for (int i = 0; i < mTempBroadcastImages.size(); i++)
	{
		if (mTempBroadcastImages[i])
		{
			delete mTempBroadcastImages[i];
			mTempBroadcastImages[i] = NULL;
		}		
	}
	for (int i = 0; i < mTempBroadcastNameImages.size(); i++)
	{
		if (mTempBroadcastNameImages[i])
		{
			delete mTempBroadcastNameImages[i];
			mTempBroadcastNameImages[i] = NULL;
		}
	}

	for (int i = 0; i < items.size(); i++)
	{
		if (items[i].mImage)
		{
			delete items[i].mImage;
			items[i].mImage = NULL;
		}
		if (items[i].mNameImage)
		{
			delete items[i].mNameImage;
			items[i].mNameImage = NULL;
		}
	}
}
bool	VuBroadcastManager::init()
{
	return true;
}

void VuBroadcastManager::postInit()
{
	mHttpRequest = VuHttpClient::IF()->createRequest();
	std::string strData;
	// header
	VuHttpClient::IF()->setContentHeader(mHttpRequest, "Content-Type", "application/json");
	VuHttpClient::IF()->setContentHeader(mHttpRequest, "Content-Length", (int)strData.size());
	// send request
	VuHttpClient::IF()->postAsync(mHttpRequest, BASE_URL, strData);
	VuTickManager::IF()->registerHandler(this, &VuBroadcastManager::tickNetwork, "Network");
	
}

void	VuBroadcastManager::release()
{

}

void VuBroadcastManager::updateData()
{

}
void VuBroadcastManager::saveToCache()
{
	if (isTempDataOK())
	{
		
		VuGameManager::IF()->setIsBroadcastDataOk(true);
		VuGameManager::IF()->setBroadCastVersion(mVersion);
		VuGameManager::IF()->setBroadCastSize(mBroadcastDataSize);
		if (mTempBroadcastImages.size() == mBroadcastDataSize && mTempBroadcastNameImages.size() == mBroadcastDataSize)
		{
			VuGameManager::IF()->setBroadcastImages(mTempBroadcastImages);
			VuGameManager::IF()->setBroadcastNameImages(mTempBroadcastNameImages);
			VuGameManager::IF()->setBroadcastImageSizes(mTempBroadcastImageSizes);
			VuGameManager::IF()->setBroadcastNameImageSizes(mTempBroadcastNameImageSizes);
		}
		VuGameManager::IF()->saveInterface();
	}
}

void VuBroadcastManager::loadBroadcastDataFromCache()
{
	std::vector<VUBYTE*> broadcastImages = VuGameManager::IF()->getBroadcastImages();
	std::vector<VUBYTE*> broadcastNameImages = VuGameManager::IF()->getBroadcastNameImages();
	std::vector<int> broadcastImageSizes = VuGameManager::IF()->getBroadcastImageSizes();
	std::vector<int> broadcastNameImageSizes = VuGameManager::IF()->getBroadcastNameImageSizes();
	int cacheSize = VuGameManager::IF()->getBroadcastSize();
	items.clear();
	
	items.resize(cacheSize);

	for (int i = 0; i < cacheSize; i++)
	{
		int broadcastImageSize = broadcastImageSizes[i];
		int broadcastNameImageSize = broadcastNameImageSizes[i];

		VUBYTE* imageData = broadcastImages[i];
		VUBYTE* nameImageData = broadcastNameImages[i];

		items[i].mImage = new VUBYTE[(broadcastImageSize + 1)*sizeof(VUBYTE)];
		items[i].mNameImage = new VUBYTE[(broadcastNameImageSize + 1) * sizeof(VUBYTE)];
		
		VU_MEMCPY(items[i].mImage, broadcastImageSize, imageData, broadcastImageSize);
		VU_MEMCPY(items[i].mNameImage, broadcastNameImageSize, nameImageData, broadcastNameImageSize);

		items[i].mImage[broadcastImageSize] = '/0';
		items[i].mNameImage[broadcastNameImageSize] = '/0';
		items[i].imageSize = broadcastImageSize + 1;
		items[i].nameImageSize = broadcastNameImageSize + 1;
	}
}

VuBroadcastManager::BroadcastItems VuBroadcastManager::getBroadcastData()
{
	loadBroadcastDataFromCache();
	return items;
}

bool VuBroadcastManager::isDataOK()
{
	return VuGameManager::IF()->getBroadcastDataOK();
}

void VuBroadcastManager::requestNameImageData()
{
	if (mHttpRequest != NULL)
	{
		VuHttpClient::IF()->releaseRequest(mHttpRequest);
		mHttpRequest = VUNULL;
	}
	requestStates = REQUEST_NAMEIMAGE;
	mHttpRequest = VuHttpClient::IF()->createRequest();
	std::string strData;
	// header
	VuHttpClient::IF()->setContentHeader(mHttpRequest, "Content-Type", "application/json");
	VuHttpClient::IF()->setContentHeader(mHttpRequest, "Content-Length", (int)strData.size());
	// send request
	if (currentRequestNameIndex >= rawItems.size())
		return;
	std::string url = rawItems[currentRequestNameIndex].m_name;
	VuHttpClient::IF()->postAsync(mHttpRequest, url.c_str(), strData);
}

void VuBroadcastManager::requestPicData()
{
	if (mHttpRequest != NULL)
	{
		VuHttpClient::IF()->releaseRequest(mHttpRequest);
		mHttpRequest = VUNULL;
	}
	requestStates = REQUEST_IMAGE;
	mHttpRequest = VuHttpClient::IF()->createRequest();
	std::string strData;
	// header
	VuHttpClient::IF()->setContentHeader(mHttpRequest, "Content-Type", "application/json");
	VuHttpClient::IF()->setContentHeader(mHttpRequest, "Content-Length", (int)strData.size());
	// send request
	if (currentRequestIndex >= rawItems.size())
		return;
	std::string url = rawItems[currentRequestIndex].m_url;
	VuHttpClient::IF()->postAsync(mHttpRequest, url.c_str(), strData);
}

void VuBroadcastManager::tickGetNamePic(float fdt)
{
	if (mHttpRequest)
	{
		VuHttpClient::eStatus status = VuHttpClient::IF()->getStatus(mHttpRequest);
		if (status == VuHttpClient::STATUS_RESPONSE_RECEIVED)
		{
			VuJsonContainer response;
			VuJsonReader jsonReader;

			int size = VuHttpClient::IF()->getResponse(mHttpRequest).size();
			const char* data = VuHttpClient::IF()->getResponse(mHttpRequest).c_str();
			if (size > 0)
			{
				const char* dataNameImages = VuHttpClient::IF()->getResponse(mHttpRequest).c_str();
				VUBYTE* tempByteData = new VUBYTE[(size + 1)*sizeof(VUBYTE)];
				VU_MEMCPY(tempByteData, size, dataNameImages, size);
				mTempBroadcastNameImages.push_back(tempByteData);
				mTempBroadcastNameImageSizes.push_back(size);
				//delete tempByteData;
				//items[currentRequestNameIndex].mNameImage = new VUBYTE[(size + 1)*sizeof(VUBYTE)];
				//VU_MEMCPY(items[currentRequestNameIndex].mNameImage, size, data, size);
				//items[currentRequestNameIndex].mNameImage[size] = '/0';
				//items[currentRequestNameIndex].nameImageSize = size + 1;
			}
			currentRequestNameIndex++;
			if (currentRequestNameIndex < rawItems.size())
			{
				requestNameImageData();
			}
			else
			{
				mIsNamePicDataOK = true;
				requestStates = REQUEST_NULL;
				if (mHttpRequest)
				{
					VuHttpClient::IF()->releaseRequest(mHttpRequest);
					mHttpRequest = VUNULL;
				}
				if (mVersion > VuGameManager::IF()->getBroadcastIndex())
				saveToCache();
			}
		}
	}
}

void VuBroadcastManager::tickGetPic(float fdt)
{
	if (mHttpRequest)
	{
		VuHttpClient::eStatus status = VuHttpClient::IF()->getStatus(mHttpRequest);
		if (status == VuHttpClient::STATUS_RESPONSE_RECEIVED)
		{
			VuJsonContainer response;
			VuJsonReader jsonReader;

			int size = VuHttpClient::IF()->getResponse(mHttpRequest).size();
			const char* data = VuHttpClient::IF()->getResponse(mHttpRequest).c_str();
			if (size > 0)
			{
				const char* dataImages = VuHttpClient::IF()->getResponse(mHttpRequest).c_str();
				VUBYTE* tempByteData = new VUBYTE[(size + 1)*sizeof(VUBYTE)];
				VU_MEMCPY(tempByteData, size, dataImages, size);
				mTempBroadcastImages.push_back(tempByteData);
				//mTempBroadcastImages.push_back(VuHttpClient::IF()->getResponse(mHttpRequest));
				mTempBroadcastImageSizes.push_back(size);
				//delete tempByteData;
				//items[currentRequestIndex].mImage = new VUBYTE[(size + 1)*sizeof(VUBYTE)];
				//VU_MEMCPY(items[currentRequestIndex].mImage, size, data, size);
				//items[currentRequestIndex].mImage[size] = '/0';
				//items[currentRequestIndex].imageSize = size + 1;
			}
			currentRequestIndex++;
			if (currentRequestIndex < rawItems.size())
			{
				requestPicData();
			}
			else
			{
				mIsPicDataOK = true;
				requestNameImageData();
			}
		}
	}
}

void VuBroadcastManager::tickGetJson(float fdt)
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
				VUPRINTF(response["msg"].asCString());
				std::string msg = response["msg"].asCString();
				if (msg == "10086")
				{
					
					mVersion = atoi(response["version"].asString().c_str());
					VuJsonContainer dataContainer = response["data"];
					int size = dataContainer.size();
					if (size > 0)
					{
						mBroadcastDataSize = size;
						for (int i = 0; i < size; i++)
						{
							const char* name = dataContainer[i]["name"].asCString();
							std::string url = dataContainer[i]["url"].asCString();

							rawItems.push_back(BroadcastRawItem(name, url));
						}
					}
				}
				if (mHttpRequest)
				{
					VuHttpClient::IF()->releaseRequest(mHttpRequest);
					mHttpRequest = VUNULL;
				}

				if (mVersion > VuGameManager::IF()->getBroadcastIndex())
					requestPicData();
			}
		}
		else
		{

		}
	}
}

void	VuBroadcastManager::tickNetwork(float fdt)
{
	switch (requestStates)
	{
	case REQUEST_JSON:
		tickGetJson(fdt);
		break;
	case REQUEST_IMAGE:
		tickGetPic(fdt);
		break;
	case  REQUEST_NAMEIMAGE:
		tickGetNamePic(fdt);
		break;
	}
}