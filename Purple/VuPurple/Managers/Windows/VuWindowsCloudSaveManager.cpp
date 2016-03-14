//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Windows CloudSave manager
//
//*****************************************************************************

#include "VuWindowsCloudSaveManager.h"
#include "VuEngine/Util/VuUtf8.h"
#include "VuEngine/Util/VuFileUtil.h"
#include "VuEngine/Util/VuZLibUtil.h"
#include "VuEngine/Util/VuHash.h"


#define ROAMING_FILE_NAME "cloudsave"
#define MAX_CLOUD_SAVE_COMPRESSED_SIZE 32*1024
#define MAX_CLOUD_SAVE_UNCOMPRESSED_SIZE 1024*1024

struct CloudSaveHeader
{
	VUUINT32	mUncompressedDataSize;
	VUUINT32	mCompressedDataSize;
	VUUINT32	mCompressedDataHash;
};


// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuCloudSaveManager, VuWindowsCloudSaveManager);


//*****************************************************************************
VuWindowsCloudSaveManager::VuWindowsCloudSaveManager():
	mCloudLoadStarted(false)
{
}

//*****************************************************************************
void VuWindowsCloudSaveManager::onDataChanged()
{
	if ( mCloudLoadStarted )
	{
		// load file from roaming storage
		VuArray<VUBYTE>	blob;
		readFile(blob);

		onCloudLoadResult(blob);
	}
}

//*****************************************************************************
void VuWindowsCloudSaveManager::startCloudLoad()
{
	// load file from roaming storage
	VuArray<VUBYTE>	blob;
	readFile(blob);

	onCloudLoadResult(blob);
	mCloudLoadStarted = true;
}

//*****************************************************************************
void VuWindowsCloudSaveManager::startCloudSave(const VuArray<VUBYTE> &blob)
{
	if ( mCloudLoadStarted )
	{
		writeFile(blob);
	}
}

//*****************************************************************************
void VuWindowsCloudSaveManager::readFile(VuArray<VUBYTE> &blob)
{
	std::string path;
	VuUtf8::convertWCharStringToUtf8String(Windows::Storage::ApplicationData::Current->RoamingFolder->Path->Data(), path);
	VuFileUtil::VuRootPathPushPop rootPathPushPop(path);

	VuArray<VUBYTE> compressedBlob(0);
	if ( VuFileUtil::loadFile(ROAMING_FILE_NAME, compressedBlob) )
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

//*****************************************************************************
void VuWindowsCloudSaveManager::writeFile(const VuArray<VUBYTE> &blob)
{
	std::string path;
	VuUtf8::convertWCharStringToUtf8String(Windows::Storage::ApplicationData::Current->RoamingFolder->Path->Data(), path);
	VuFileUtil::VuRootPathPushPop rootPathPushPop(path);

	int headerSize = sizeof(CloudSaveHeader);

	// compress data
	VuArray<VUBYTE> compressedBlob(0);
	VUUINT32 compressedSize = VuZLibUtil::calcCompressBound(blob.size());
	compressedBlob.resize(headerSize + compressedSize);
	if ( VuZLibUtil::compressToMemory(&compressedBlob[headerSize], &compressedSize, &blob.begin(), blob.size()) )
	{
		compressedBlob.resize(headerSize + compressedSize);

		// header
		CloudSaveHeader header;
		header.mUncompressedDataSize = blob.size();
		header.mCompressedDataSize = compressedSize;
		header.mCompressedDataHash = VuHash::fnv32(&compressedBlob[headerSize], compressedSize);
		VU_MEMCPY(&compressedBlob[0], compressedBlob.size(), &header, headerSize);

		if (compressedBlob.size() <= MAX_CLOUD_SAVE_COMPRESSED_SIZE)
		{
			if ( !VuFileUtil::saveFile(ROAMING_FILE_NAME, &compressedBlob[0], compressedBlob.size()) )
				VUPRINTF("Warning: Cloud save failed\n");
		}
		else
		{
			VUPRINTF("Warning: Cloud save exceeds max compressed size (%d bytes)\n", MAX_CLOUD_SAVE_COMPRESSED_SIZE);
		}
	}
	else
	{
		VUPRINTF("Warning: Cloud save compression failed\n");
	}
}
