//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  CloudSave Manager
// 
//*****************************************************************************

#include "VuCloudSaveManager.h"
#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Managers/VuProfileManager.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Json/VuJsonBinaryReader.h"
#include "VuEngine/Json/VuJsonBinaryWriter.h"
#include "VuEngine/DB/VuStringDB.h"
#include "VuEngine/Util/VuStringUtil.h"


//*****************************************************************************
VuCloudSaveManager::VuCloudSaveManager():
	mState(STATE_LOADING)
{
}

//*****************************************************************************
void VuCloudSaveManager::postInit()
{
	if ( VuGameUtil::IF()->isDemoMode() )
		return;

	startCloudLoad();
}

//*****************************************************************************
void VuCloudSaveManager::save()
{
	if ( mState != STATE_RESOLVED )
		return;

	if ( VuGameUtil::IF()->isDemoMode() )
		return;

	// save data to blob
	VuArray<VUBYTE> blob;
	saveToBlob(VuProfileManager::IF()->dataRead(), blob);

	// only save if changed
	if ( blob.size() != mBlob.size() || memcmp(&blob.begin(), &mBlob.begin(), blob.size()) )
	{
		mBlob.resize(blob.size());
		VU_MEMCPY(&mBlob.begin(), mBlob.size(), &blob.begin(), blob.size());

		startCloudSave(blob);
	}
}

//*****************************************************************************
void VuCloudSaveManager::onCloudLoadResult(const VuArray<VUBYTE> &blob)
{
	if ( mState == STATE_ASKING )
		return;

	// load cloud data
	VuJsonBinaryReader reader;
	if ( blob.size() && reader.loadFromMemory(mCloudData, &blob[0], blob.size()) )
	{
		const VuJsonContainer &localData = VuProfileManager::IF()->dataRead();

		// is cloud data better than local data?
		int cloudSC = mCloudData["Game"]["SC"]["Earned"].asInt() + mCloudData["Game"]["SC"]["Purchased"].asInt();
		int localSC = localData["Game"]["SC"]["Earned"].asInt() + localData["Game"]["SC"]["Purchased"].asInt();

		int cloudPC = mCloudData["Game"]["PC"]["Earned"].asInt() + mCloudData["Game"]["PC"]["Purchased"].asInt();
		int localPC = localData["Game"]["PC"]["Earned"].asInt() + localData["Game"]["PC"]["Purchased"].asInt();

		float cloudTotalTime = mCloudData["Stats"]["TotalTime"].asFloat();
		float localTotalTime = localData["Stats"]["TotalTime"].asFloat();

		if ( (cloudSC > localSC) || (cloudPC > localPC) || (cloudTotalTime > localTotalTime) )
		{
			createChoiceMessageBox();
			mState = STATE_ASKING;
		}
		else
		{
			mCloudData.clear();
			mState = STATE_RESOLVED;
		}
	}
	else
	{
		mCloudData.clear();
		mState = STATE_RESOLVED;
	}
}

//*****************************************************************************
void VuCloudSaveManager::onMessageBoxClosed(VuMessageBox *pMessageBox)
{
	if ( pMessageBox->params().mUserData == VuHash::fnv32String("Choice") )
	{
		if ( pMessageBox->getResult() == "OnA" )
		{
			createRestoreMessageBox();
		}
		else if ( pMessageBox->getResult() == "OnB" )
		{
			createContinueMessageBox();
		}
	}
	else if ( pMessageBox->params().mUserData == VuHash::fnv32String("Restore") )
	{
		if ( pMessageBox->getResult() == "OnA" )
		{
			VuProfileManager::IF()->dataWrite()["PersistentCloudData"] = mCloudData["PersistentCloudData"];
			VuProfileManager::IF()->dataWrite()["Achievements"] = mCloudData["Achievements"];
			VuProfileManager::IF()->dataWrite()["Game"] = mCloudData["Game"];
			VuProfileManager::IF()->dataWrite()["Stats"] = mCloudData["Stats"];

			saveToBlob(VuProfileManager::IF()->dataRead(), mBlob);

			VuEventManager::IF()->broadcast("OnCloudDataPostSync");

			mCloudData.clear();
			mState = STATE_RESOLVED;
		}
		else if ( pMessageBox->getResult() == "OnB" )
		{
			createChoiceMessageBox();
		}
	}
	else if ( pMessageBox->params().mUserData == VuHash::fnv32String("Continue") )
	{
		if ( pMessageBox->getResult() == "OnA" )
		{
			mCloudData.clear();
			mState = STATE_RESOLVED;
		}
		else if ( pMessageBox->getResult() == "OnB" )
		{
			createChoiceMessageBox();
		}
	}
}

//*****************************************************************************
void VuCloudSaveManager::createChoiceMessageBox()
{
	VuMessageBoxParams messageBoxParams;
	messageBoxParams.mType = "SimpleAB";
	messageBoxParams.mHeading = "CloudSave_ConflictHeading";
	messageBoxParams.mBody = VuStringDB::IF()->getString("CloudSave_ConflictBody");
	messageBoxParams.mTextA = "CloudSave_CloudChoice";
	messageBoxParams.mTextB = "CloudSave_LocalChoice";
	messageBoxParams.mPauseGame = true;
	messageBoxParams.mUserData = VuHash::fnv32String("Choice");

	formatMessageBoxBody(messageBoxParams.mBody);

	VuMessageBox *pMessageBox = VuMessageBoxManager::IF()->create(messageBoxParams);
	pMessageBox->setCallback(this);
	pMessageBox->removeRef();
}

//*****************************************************************************
void VuCloudSaveManager::createRestoreMessageBox()
{
	VuMessageBoxParams messageBoxParams;
	messageBoxParams.mType = "SimpleAB";
	messageBoxParams.mHeading = "CloudSave_CloudChoice";
	messageBoxParams.mBody = VuStringDB::IF()->getString("CloudSave_RestoreBody");
	messageBoxParams.mTextA = "Common_Yes";
	messageBoxParams.mTextB = "Common_No";
	messageBoxParams.mPauseGame = true;
	messageBoxParams.mUserData = VuHash::fnv32String("Restore");

	formatMessageBoxBody(messageBoxParams.mBody);

	VuMessageBox *pMessageBox = VuMessageBoxManager::IF()->create(messageBoxParams);
	pMessageBox->setCallback(this);
	pMessageBox->removeRef();
}

//*****************************************************************************
void VuCloudSaveManager::createContinueMessageBox()
{
	VuMessageBoxParams messageBoxParams;
	messageBoxParams.mType = "SimpleAB";
	messageBoxParams.mHeading = "CloudSave_LocalChoice";
	messageBoxParams.mBody = VuStringDB::IF()->getString("CloudSave_ContinueBody");
	messageBoxParams.mTextA = "Common_Yes";
	messageBoxParams.mTextB = "Common_No";
	messageBoxParams.mPauseGame = true;
	messageBoxParams.mUserData = VuHash::fnv32String("Continue");

	formatMessageBoxBody(messageBoxParams.mBody);

	VuMessageBox *pMessageBox = VuMessageBoxManager::IF()->create(messageBoxParams);
	pMessageBox->setCallback(this);
	pMessageBox->removeRef();
}

//*****************************************************************************
void VuCloudSaveManager::formatMessageBoxBody(std::string &bodyText)
{
	const VuJsonContainer &localData = VuProfileManager::IF()->dataRead();

	char strCloudDate[32];
	VuStringUtil::dateFormat(VuStringUtil::YYYY_MM_DD, mCloudData["Date"][0].asInt(), mCloudData["Date"][1].asInt(), mCloudData["Date"][2].asInt(), strCloudDate, sizeof(strCloudDate));

	float cloudDistance = mCloudData["Stats"]["TotalDistance"].asFloat();
	float localDistance = localData["Stats"]["TotalDistance"].asFloat();

	char strCloudDistance[32], strLocalDistance[32];
	VuGameUtil::distanceFormatKm(cloudDistance, strCloudDistance, sizeof(strCloudDistance));
	VuGameUtil::distanceFormatKm(localDistance, strLocalDistance, sizeof(strLocalDistance));

	VuStringUtil::replace(bodyText, "[CLOUD_DATE]", strCloudDate);
	VuStringUtil::replace(bodyText, "[CLOUD_DISTANCE]", strCloudDistance);
	VuStringUtil::replace(bodyText, "[LOCAL_DISTANCE]", strLocalDistance);
}

//*****************************************************************************
void VuCloudSaveManager::saveToBlob(const VuJsonContainer &profileData, VuArray<VUBYTE> &blob)
{
	VuJsonContainer blobData;
	blobData["PersistentCloudData"] = VuProfileManager::IF()->dataRead()["PersistentCloudData"];
	blobData["Achievements"] = VuProfileManager::IF()->dataRead()["Achievements"];
	blobData["Game"] = VuProfileManager::IF()->dataRead()["Game"];
	blobData["Stats"] = VuProfileManager::IF()->dataRead()["Stats"];

	VuTimeUtil::VuTimeStruct localTime;
	VuTimeUtil::getLocalTime(localTime);
	blobData["Date"][0].putValue((int)localTime.mYear);
	blobData["Date"][1].putValue((int)localTime.mMonth);
	blobData["Date"][2].putValue((int)localTime.mDay);

	VuJsonBinaryWriter writer;
	int dataSize = VuJsonBinaryWriter::calculateDataSize(blobData);
	blob.resize(dataSize);
	writer.saveToMemory(blobData, &blob[0], dataSize);
}
