//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Generic CloudSave manager
//
//*****************************************************************************

#include "VuPurple/Managers/VuCloudSaveManager.h"
#include "VuPurple/Managers/VuGameServicesManager.h"
#include "VuPurple/Managers/VuTitleStorageManager.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Memory/VuScratchPad.h"
#include "VuEngine/Util/VuFSM.h"
#include "VuEngine/Util/VuBase64.h"
#include "VuEngine/Util/VuZLibUtil.h"
#include "VuEngine/Util/VuHash.h"


#define SAVE_THROTTLE_TIME 30 // seconds
#define MAX_CLOUD_SAVE_COMPRESSED_SIZE 32*1024 // seconds
#define MAX_CLOUD_SAVE_UNCOMPRESSED_SIZE 1024*1024 // seconds

struct CloudSaveHeader
{
	VUUINT32	mUncompressedDataSize;
	VUUINT32	mCompressedDataSize;
	VUUINT32	mCompressedDataHash;
};


class VuGenericCloudSaveManager : public VuCloudSaveManager, VuGameServicesManager::VuListener, VuTitleStorageManager::LoadCallback, VuTitleStorageManager::SaveCallback
{
public:
	VuGenericCloudSaveManager();

	virtual bool	init();
	virtual void	release();

protected:
	// VuGameServicesManager::VuListener
	virtual void	onSignIn() { mFSM.setCondition("LoadRequested", true); }
	virtual void	onSignOut() { mFSM.clearAllConditions(); }

	virtual void	startCloudLoad() {}
	virtual void	startCloudSave(const VuArray<VUBYTE> &blob);

	// VuTitleStorageManager callbacks
	virtual void	onTitleStorageLoadResult(VuTitleStorageManager::eResult result, const std::string &data, int id);
	virtual void	onTitleStorageSaveResult(VuTitleStorageManager::eResult result, int id);

	void			tickNetwork(float fdt);

	// FSM
	void			onLoadEnter();
	void			onSaveEnter();
	void			onThrottleTick(float fdt);

	VuFSM			mFSM;
	VuArray<VUBYTE>	mBlobToSave;
	bool			mReceivedLoadResponse;
};


// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuCloudSaveManager, VuGenericCloudSaveManager);


//*****************************************************************************
VuGenericCloudSaveManager::VuGenericCloudSaveManager():
	mBlobToSave(0),
	mReceivedLoadResponse(false)
{
}

//*****************************************************************************
bool VuGenericCloudSaveManager::init()
{
	if (!VuCloudSaveManager::init())
		return false;

	// create FSM
	{
		VuFSM::VuState *pState = mFSM.addState("Idle");

		pState = mFSM.addState("Load");
		pState->setEnterMethod(this, &VuGenericCloudSaveManager::onLoadEnter);

		pState = mFSM.addState("Save");
		pState->setEnterMethod(this, &VuGenericCloudSaveManager::onSaveEnter);

		pState = mFSM.addState("Throttle");
		pState->setTickMethod(this, &VuGenericCloudSaveManager::onThrottleTick);

		// transitions
		mFSM.addTransition("Idle", "Load", "LoadRequested");
		mFSM.addTransition("Idle", "Save", "SaveRequested");

		mFSM.addTransition("Load", "Idle", "Done");

		mFSM.addTransition("Save", "Throttle", "Done");

		mFSM.addTransition("Throttle", "Load", "LoadRequested");
		mFSM.addTransition("Throttle", "Idle", "Expired");
	}

	mFSM.begin();

	VuTickManager::IF()->registerHandler(this, &VuGenericCloudSaveManager::tickNetwork, "Network");
	VuGameServicesManager::IF()->addListener(this);

	return true;
}

//*****************************************************************************
void VuGenericCloudSaveManager::release()
{
	VuTickManager::IF()->unregisterHandlers(this);
	VuGameServicesManager::IF()->removeListener(this);

	mFSM.end();

	VuCloudSaveManager::release();
}

//*****************************************************************************
void VuGenericCloudSaveManager::startCloudSave(const VuArray<VUBYTE> &blob)
{
	if (VuGameServicesManager::IF()->isSignedIn() && mReceivedLoadResponse)
	{
		mBlobToSave.resize(blob.size());
		VU_MEMCPY(&mBlobToSave.begin(), mBlobToSave.size(), &blob.begin(), blob.size());

		mFSM.setCondition("SaveRequested", true);
	}
}

//*****************************************************************************
void VuGenericCloudSaveManager::tickNetwork(float fdt)
{
	mFSM.evaluate();
	mFSM.tick(fdt);
}

//*****************************************************************************
void VuGenericCloudSaveManager::onLoadEnter()
{
	mFSM.setCondition("LoadRequested", false);

	VuTitleStorageManager::IF()->startLoad(VuGameServicesManager::IF()->getGamerID(), "cloudsave", this, 0);
}

//*****************************************************************************
void VuGenericCloudSaveManager::onSaveEnter()
{
	mFSM.setCondition("SaveRequested", false);

	int headerSize = sizeof(CloudSaveHeader);

	// compress data
	VuArray<VUBYTE> compressedBlob(0);
	VUUINT32 compressedSize = VuZLibUtil::calcCompressBound(mBlobToSave.size());
	compressedBlob.resize(headerSize + compressedSize);
	if (VuZLibUtil::compressToMemory(&compressedBlob[headerSize], &compressedSize, &mBlobToSave.begin(), mBlobToSave.size()))
	{
		compressedBlob.resize(headerSize + compressedSize);

		// header
		CloudSaveHeader header;
		header.mUncompressedDataSize = mBlobToSave.size();
		header.mCompressedDataSize = compressedSize;
		header.mCompressedDataHash = VuHash::fnv32(&compressedBlob[headerSize], compressedSize);
		VU_MEMCPY(&compressedBlob[0], compressedBlob.size(), &header, headerSize);

		// encode
		std::string data;
		VuBase64::encode(compressedBlob, data);

		if (data.size() <= MAX_CLOUD_SAVE_COMPRESSED_SIZE)
		{
			VuTitleStorageManager::IF()->startSave(VuGameServicesManager::IF()->getGamerID(), "cloudsave", data, this, 0);
		}
		else
		{
			VUPRINTF("Warning: Cloud save exceeds max compressed size (%d bytes)\n", MAX_CLOUD_SAVE_COMPRESSED_SIZE);
			mFSM.pulseCondition("Done");
		}
	}
	else
	{
		VUPRINTF("Warning: Cloud save compression failed\n");
		mFSM.pulseCondition("Done");
	}
}

//*****************************************************************************
void VuGenericCloudSaveManager::onTitleStorageLoadResult(VuTitleStorageManager::eResult result, const std::string &data, int id)
{
	VuArray<VUBYTE> blob(0);

	if (result == VuTitleStorageManager::RESULT_SUCCESS)
	{
		if (VuGameServicesManager::IF()->isSignedIn())
		{
			// decode
			VuArray<VUBYTE> compressedBlob(0);
			if (VuBase64::decode(data, compressedBlob))
			{
				// verify header
				int headerSize = sizeof(CloudSaveHeader);
				if (compressedBlob.size() >= headerSize)
				{
					CloudSaveHeader header;
					VU_MEMCPY(&header, headerSize, &compressedBlob[0], sizeof(header));
					if ((int)header.mCompressedDataSize + headerSize == compressedBlob.size())
					{
						if (header.mCompressedDataHash == VuHash::fnv32(&compressedBlob[headerSize], header.mCompressedDataSize))
						{
							// uncompress
							VUUINT32 uncompressedSize = header.mUncompressedDataSize;
							if (uncompressedSize <= MAX_CLOUD_SAVE_UNCOMPRESSED_SIZE) // sanity check
							{
								blob.resize(uncompressedSize);
								if (!VuZLibUtil::uncompressFromMemory(&blob[0], &uncompressedSize, &compressedBlob[headerSize], header.mCompressedDataSize))
									blob.clear();
							}
						}
					}
				}
			}

		}
	}

	if (result != VuTitleStorageManager::RESULT_ERROR)
		mReceivedLoadResponse = true;

	onCloudLoadResult(blob);

	mFSM.pulseCondition("Done");
}

//*****************************************************************************
void VuGenericCloudSaveManager::onTitleStorageSaveResult(VuTitleStorageManager::eResult result, int id)
{
	mFSM.pulseCondition("Done");
}

//*****************************************************************************
void VuGenericCloudSaveManager::onThrottleTick(float fdt)
{
	if (mFSM.getTimeInState() >= SAVE_THROTTLE_TIME)
		mFSM.pulseCondition("Expired");
}
