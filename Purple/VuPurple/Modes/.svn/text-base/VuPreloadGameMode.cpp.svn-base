//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  PreloadGameMode class
// 
//*****************************************************************************

#include "VuPreloadGameMode.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuPurple/Managers/VuProjectManager.h"
#include "VuPurple/Managers/VuFadeManager.h"
#include "VuEngine/Entities/UI/VuUIScreenEntity.h"
#include "VuEngine/Projects/VuProject.h"
#include "VuEngine/Assets/VuAssetFactory.h"
#include "VuEngine/Assets/VuDbAsset.h"
#include "VuEngine/Assets/VuProjectAsset.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Managers/VuViewportManager.h"
#include "VuEngine/Managers/VuMessageBoxManager.h"
#include "VuEngine/Managers/VuDialogManager.h"


IMPLEMENT_RTTI(VuPreloadGameMode, VuGameMode);

#define LOADING_SCREEN "Screens/Preload"
#define MIN_TIME_BETWEEN_FRAMES 0.1 // seconds


//*****************************************************************************
VuPreloadGameMode::VuPreloadGameMode():
	mPreloadIndex(0),
	mLastDrawTime(0.0)
{
}

//*****************************************************************************
VuPreloadGameMode::~VuPreloadGameMode()
{
}

//*****************************************************************************
bool VuPreloadGameMode::enter(const std::string &prevMode)
{
	VuViewportManager::IF()->reset();
	VuViewportManager::IF()->setViewportCount(1);

	// start music
	VuGameUtil::IF()->startMusic(VuGameUtil::MUSIC_MENU);

	VuDBAsset *pPreloadDB = VuAssetFactory::IF()->createAsset<VuDBAsset>("PreloadDB");

	// handle background project
	{
		const VuJsonContainer &projects = pPreloadDB->getDB()["Projects"];
		for ( int iProject = 0; iProject < projects.size(); iProject++ )
		{
			const std::string &project = projects[iProject].asString();

			VuProjectAsset *pProjectAsset = VuAssetFactory::IF()->createAsset<VuProjectAsset>(project);
			int assetCount = pProjectAsset->getAssetCount();
			std::string assetType, assetName;
			for ( int iAsset = 0; iAsset < assetCount; iAsset++ )
			{
				pProjectAsset->getAssetInfo(iAsset, assetType, assetName);
				addToQueue(assetType, assetName);
			}

			VuAssetFactory::IF()->releaseAsset(pProjectAsset);
			addToQueue("VuProjectAsset", project);
		}
	}

	// handle preload db
	{
		const VuJsonContainer &db = pPreloadDB->getDB()["Assets"];
		for ( int iPriority = 0; iPriority < db.size(); iPriority++ )
		{
			const VuJsonContainer &priority = db[iPriority];
			for ( int iType = 0; iType < priority.numMembers(); iType++ )
			{
				const std::string &type = priority.getMemberKey(iType);
				const VuJsonContainer &assets = priority[type];
				for ( int iAsset = 0; iAsset < assets.size(); iAsset++ )
				{
					const std::string &asset = assets[iAsset].asString();
					addToQueue(type, asset);
				}
			}

		}
	}

	VuAssetFactory::IF()->releaseAsset(pPreloadDB);

	// load loading screen
	if ( (mpLoadingScreenProject = VuProjectManager::IF()->load(LOADING_SCREEN)) )
		mpLoadingScreenProject->gameInitialize();

	VuFadeManager::IF()->startFadeIn(0.0f);

	return true;
}

//*****************************************************************************
void VuPreloadGameMode::exit()
{
	// unload loading screen
	if ( mpLoadingScreenProject )
	{
		mpLoadingScreenProject->gameRelease();
		VuProjectManager::IF()->unload(mpLoadingScreenProject);
		mpLoadingScreenProject = VUNULL;
	}

	VuFadeManager::IF()->startFadeOut(0.0f);

	VuViewportManager::IF()->reset();
}

//*****************************************************************************
const char *VuPreloadGameMode::tick(float fdt)
{
	if ( VuMessageBoxManager::IF()->getActiveMessageBox() == VUNULL && VuDialogManager::IF()->getActiveDialog() == VUNULL )
	{
		do
		{
			if ( mPreloadIndex >= mPreloadQueue.size() )
				break;

			const PreloadEntry &entry = mPreloadQueue[mPreloadIndex];
			VuAssetFactory::IF()->preloadAsset(entry.first, entry.second);

			mPreloadIndex++;
		}
		while ( VuSys::IF()->getTime() - mLastDrawTime < MIN_TIME_BETWEEN_FRAMES );
	}

	float progress = (mPreloadQueue.size() > 0) ? (float)mPreloadIndex/mPreloadQueue.size() : 1.0f;

	VuParams params;
	params.addFloat(progress);
	VuEventManager::IF()->broadcast("OnProgressUpdate", params);

	if ( VuUIScreenEntity *pScreen = getLoadingScreen() )
		pScreen->tick(fdt);

	if ( mPreloadIndex >= mPreloadQueue.size() )
		return "FrontEnd";

	return "";
}

//*****************************************************************************
void VuPreloadGameMode::draw()
{
	if ( VuUIScreenEntity *pScreen = getLoadingScreen() )
	{
		pScreen->draw();
	}

	mLastDrawTime = VuSys::IF()->getTime();
}

//*****************************************************************************
void VuPreloadGameMode::addToQueue(const std::string &assetType, const std::string &assetName)
{
	// already loaded?
	VUUINT32 hash = VuAssetFactory::calcAssetHashID(assetType.c_str(), assetName.c_str());
	if ( !VuAssetFactory::IF()->findAsset(hash) )
	{
		if ( mPreloadHashes.find(hash) == mPreloadHashes.end() )
		{
			mPreloadQueue.push_back(PreloadEntry(assetType, assetName));
			mPreloadHashes.insert(hash);
		}
	}
}

//*****************************************************************************
VuUIScreenEntity *VuPreloadGameMode::getLoadingScreen()
{
	if ( mpLoadingScreenProject && mpLoadingScreenProject->getRootEntity()->isDerivedFrom(VuUIScreenEntity::msRTTI) )
		return static_cast<VuUIScreenEntity *>(mpLoadingScreenProject->getRootEntity());

	return VUNULL;
}
