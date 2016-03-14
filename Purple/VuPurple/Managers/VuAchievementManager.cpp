//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  AchievementManager class
// 
//*****************************************************************************

#include "VuAchievementManager.h"
#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Managers/VuAnalyticsManager.h"
#include "VuPurple/Managers/VuStatsManager.h"
#include "VuPurple/Util/VuAchievementUtil.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Assets/VuAssetFactory.h"
#include "VuEngine/Assets/VuSpreadsheetAsset.h"
#include "VuEngine/Assets/VuTextureAsset.h"
#include "VuEngine/Managers/VuProfileManager.h"
#include "VuEngine/DB/VuStringDB.h"
#include "VuEngine/Util/VuFilterExpression.h"


// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuAchievementManager, VuAchievementManager);

IMPLEMENT_RTTI(VuAchievementToast, VuToast);


//*****************************************************************************
bool VuAchievementManager::init()
{
	VuToastManager::IF()->registerToastType(VuAchievementToast::msRTTI, "Screens/AchievementToast");

	VuFilterExpression filterExpression;
	filterExpression.addVariable("platform", VUPLATFORM);
	filterExpression.addVariable("sku", VuAssetFactory::IF()->getSku().c_str());

	const VuSpreadsheetAsset *pSA = VuGameUtil::IF()->achievementSpreadsheet();

	int idColumn = pSA->getColumnIndex("ID");
	int pointsColumn = pSA->getColumnIndex("Points");
	int pcColumn = pSA->getColumnIndex("PC");
	int filterColumn = pSA->getColumnIndex("Filter");
	for ( int iRow = 0; iRow < pSA->getRowCount(); iRow++ )
	{
		const char *filterText = pSA->getField(iRow, filterColumn).asCString();

		bool filterResult = true;
		if ( filterText[0] != '\0' )
		{
			if ( filterExpression.evaluate(filterText) )
				filterResult = filterExpression.result();
			else
				VUPRINTF("Achievement filter parsing error:  %s\n", filterText);
		}

		if ( filterResult )
		{
			mAchievements.resize(mAchievements.size() + 1);
			VuAchievement &ach = mAchievements.back();

			ach.mId = pSA->getField(iRow, idColumn).asInt();
			ach.mPoints = pSA->getField(iRow, pointsColumn).asInt();
			ach.mPC = pSA->getField(iRow, pcColumn).asInt();
		}
	}

	return true;
}

//*****************************************************************************
void VuAchievementManager::setProgress(int achievementId, float progress)
{
	progress = VuClamp(progress, 0.0f, 1.0f);

	if ( progress <= getProgress(achievementId) )
		return;

	auto result = std::find_if(mAchievements.begin(), mAchievements.end(), [achievementId](const VuAchievement &ach) { return ach.mId == achievementId; } );
	if ( result != mAchievements.end() )
	{
		const VuAchievement &ach = *result;

		VuProfileManager::IF()->dataWrite()["Achievements"][achievementId].putValue(progress);

		if ( progress == 1.0f )
		{
			char strStringId[32];
			VU_SPRINTF(strStringId, sizeof(strStringId), "Ach%02d", ach.mId);

			VuAchievementToast *pToast = new VuAchievementToast;
			pToast->mText = VuStringDB::IF()->getString(strStringId);
			pToast->mReward = ach.mPC;
			VuToastManager::IF()->showToast(pToast);

			VuGameManager::IF()->addCurrencyEarned(VuGameManager::CT_PREMIUM, ach.mPC);

			// analytics - How often is each achievement earned?
			{
				VuJsonContainer variables;
				variables["Highest Event Played"].putValue(VuStatsManager::IF()->getHighestEventPlayedIndex());

				char strAchId[16];
				VU_SPRINTF(strAchId, sizeof(strAchId), "%d", ach.mId);
				VuAnalyticsManager::IF()->logEvent("Achievement Unlocked", "Achievement Id", strAchId, variables);
			}
		}
	}
}

//*****************************************************************************
float VuAchievementManager::getProgress(int achievementId)
{
	return VuProfileManager::IF()->dataRead()["Achievements"][achievementId].asFloat();
}

//*****************************************************************************
int VuAchievementManager::unlockedCount()
{
	int unlockedCount = 0;

	for ( const auto &ach : mAchievements )
		if ( isUnlocked(ach.mId) )
			unlockedCount++;

	return unlockedCount;
}

//*****************************************************************************
int VuAchievementManager::totalScore()
{
	int unlockedScore = 0;

	for ( const auto &ach : mAchievements )
		unlockedScore += ach.mPoints;

	return unlockedScore;
}

//*****************************************************************************
int VuAchievementManager::unlockedScore()
{
	int unlockedScore = 0;

	for ( const auto &ach : mAchievements )
		if ( isUnlocked(ach.mId) )
			unlockedScore += ach.mPoints;

	return unlockedScore;
}
