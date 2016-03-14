//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  TipManager class
// 
//*****************************************************************************

#include "VuTipManager.h"
#include "VuEngine/Assets/VuAssetFactory.h"
#include "VuEngine/Assets/VuDBAsset.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Managers/VuProfileManager.h"
#include "VuEngine/Math/VuRand.h"
#include "VuEngine/Util/VuDataUtil.h"


// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuTipManager, VuTipManager);


//*****************************************************************************
bool VuTipManager::init()
{
	mpTipDBAsset = VuAssetFactory::IF()->createAsset<VuDBAsset>("TipDB", VuAssetFactory::OPTIONAL_ASSET);

	return true;
}

//*****************************************************************************
void VuTipManager::release()
{
	VuAssetFactory::IF()->releaseAsset(mpTipDBAsset);
}

//*****************************************************************************
void VuTipManager::showTip()
{
	if ( mpTipDBAsset == VUNULL )
		return;

	if ( VuProfileManager::IF()->dataRead()["Tips"]["Disabled"].asBool() )
		return;

	// build highest priority list of tips that haven't been shown
	std::vector<std::string> tipNames;
	int highestPriority = 0;
	const VuJsonContainer &tipsData = mpTipDBAsset->getDB()["Tips"];
	for ( int i = 0; i < tipsData.numMembers(); i++ )
	{
		const std::string &tipName = tipsData.getMemberKey(i);
		const VuJsonContainer &tipData = tipsData[tipName];

		int priority = tipData["Priority"].asInt();
		if ( priority >= highestPriority && needToShow(tipName) )
		{
			if ( priority > highestPriority )
			{
				highestPriority = priority;
				tipNames.clear();
			}
			tipNames.push_back(tipName);
		}
	}

	// now randomly choose a tip
	if ( tipNames.size() )
	{
		const std::string &tipName = tipNames[VuTruncate(tipNames.size()*VuRand::global().rand())];
		const VuJsonContainer &tipData = tipsData[tipName];

		VuVector2 textureScale(1,1);
		VuDataUtil::getValue(tipData["IconScale"], textureScale);

		VuMessageBoxParams messageBoxParams;
		messageBoxParams.mType = "SimpleAB";
		messageBoxParams.mHeading = "Tip_Heading";
		messageBoxParams.mTextA = "Common_OK";
		messageBoxParams.mTextB = "Tip_Hide";
		messageBoxParams.mBody = tipData["StringID"].asString();

		VuMessageBox *pMessageBox = VuMessageBoxManager::IF()->create(messageBoxParams);
		pMessageBox->setCallback(this);
		pMessageBox->removeRef();
		VuTickManager::IF()->pushPauseRequest();

		// mark tip as shown
		if ( !tipData["CustomRepeat"].asBool() )
			setTipShown(tipName);
	}
}

//*****************************************************************************
void VuTipManager::setTipShown(const std::string &tipName)
{
	VuProfileManager::IF()->dataWrite()["Tips"]["Shown"][tipName].putValue(true);
}

//*****************************************************************************
void VuTipManager::onMessageBoxClosed(VuMessageBox *pMessageBox)
{
	VuTickManager::IF()->popPauseRequest();

	if ( pMessageBox->getResult() == "OnB" )
	{
		VuProfileManager::IF()->dataWrite()["Tips"]["Disabled"].putValue(true);
	}
}

//*****************************************************************************
bool VuTipManager::needToShow(const std::string &tipName)
{
	// marked as shown?
	if ( VuProfileManager::IF()->dataRead()["Tips"]["Shown"][tipName].asBool() )
		return false;

	return true;
}