//*****************************************************************************
//
//  Copyright (c) 2008-2008 Ralf Knoesel
//  Copyright (c) 2008-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  UnusedAssetsGameMode class
// 
//*****************************************************************************

#include "VuUnusedAssetsGameMode.h"
#include "VuPurple/Managers/VuFadeManager.h"
#include "VuPurple/Managers/VuProjectManager.h"
#include "VuPurple/Track/VuTrackManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Entities/VuEntityFactory.h"
#include "VuEngine/Entities/VuEntityRepository.h"
#include "VuEngine/Managers/VuViewportManager.h"
#include "VuEngine/Assets/VuAssetFactory.h"
#include "VuEngine/Assets/VuProjectAsset.h"
#include "VuEngine/Projects/VuProject.h"
#include "VuEngine/Dev/VuDevConsole.h"


IMPLEMENT_RTTI(VuUnusedAssetsGameMode, VuGameMode);


//*****************************************************************************
bool VuUnusedAssetsGameMode::enter(const std::string &prevMode)
{
	// instantly fade out
	VuFadeManager::IF()->startFadeOut(0.0f);

	VuDevConsole::IF()->show(true);
	VuViewportManager::IF()->reset();
	VuViewportManager::IF()->setViewportCount(1);

	// create FSM
	{
		VuFSM::VuState *pState = mFSM.addState("JetSkis");
		pState->setEnterMethod(this, &VuUnusedAssetsGameMode::onJetSkisEnter);
		pState->setTickMethod(this, &VuUnusedAssetsGameMode::onJetSkisTick);

		pState = mFSM.addState("Riders");
		pState->setEnterMethod(this, &VuUnusedAssetsGameMode::onRidersEnter);
		pState->setTickMethod(this, &VuUnusedAssetsGameMode::onRidersTick);

		pState = mFSM.addState("Projects");
		pState->setEnterMethod(this, &VuUnusedAssetsGameMode::onProjectsEnter);
		pState->setTickMethod(this, &VuUnusedAssetsGameMode::onProjectsTick);

		pState = mFSM.addState("Result");
		pState->setEnterMethod(this, &VuUnusedAssetsGameMode::onResultEnter);
		pState->setTickMethod(this, &VuUnusedAssetsGameMode::onResultTick);

		// transitions
		mFSM.addTransition("JetSkis", "Riders", "Done");
		mFSM.addTransition("Riders", "Projects", "Done");
		mFSM.addTransition("Projects", "Result", "Done");
	}

	mFSM.begin();

	return true;
}

//*****************************************************************************
void VuUnusedAssetsGameMode::exit()
{
	mFSM.end();

	VuViewportManager::IF()->reset();
	VuDevConsole::IF()->show(false);
}

//*****************************************************************************
const char *VuUnusedAssetsGameMode::tick(float fdt)
{
	mFSM.evaluate();
	mFSM.tick(fdt);

	return "";
}

//*****************************************************************************
void VuUnusedAssetsGameMode::onJetSkisEnter()
{
}

//*****************************************************************************
void VuUnusedAssetsGameMode::onJetSkisTick(float fdt)
{
	mFSM.pulseCondition("Done");
}

//*****************************************************************************
void VuUnusedAssetsGameMode::onRidersEnter()
{
}

//*****************************************************************************
void VuUnusedAssetsGameMode::onRidersTick(float fdt)
{
	mFSM.pulseCondition("Done");
}

//*****************************************************************************
void VuUnusedAssetsGameMode::onProjectsEnter()
{
	const VuAssetFactory::AssetNames &assetNames = VuAssetFactory::IF()->getAssetNames<VuProjectAsset>();
	for ( int i = 0; i < (int)assetNames.size(); i++ )
		mQueue.push(assetNames[i]);
}

//*****************************************************************************
void VuUnusedAssetsGameMode::onProjectsTick(float fdt)
{
	if ( mQueue.size() )
	{
		std::string assetName = mQueue.front();
		mQueue.pop();

		VUPRINTF("Analyzing Project %s\n", assetName.c_str());
		VuProject *pProject = VuProjectManager::IF()->load(assetName);
		if ( !VuEntityRepository::IF()->isProjectLoaded(pProject->getName().c_str()) )
		{
			pProject->gameInitialize();
			pProject->gameRelease();
		}
		VuProjectManager::IF()->unload(pProject);

		VuTrackManager::IF()->reset(); // todo: cleanup
	}
	else
	{
		mFSM.pulseCondition("Done");
	}
}

//*****************************************************************************
void VuUnusedAssetsGameMode::onResultEnter()
{
	const VuAssetFactory::AssetTypes &assetTypes = VuAssetFactory::IF()->getAssetTypes();
	for ( int iType = 0; iType < (int)assetTypes.size(); iType++ )
	{
		const std::string &assetType = assetTypes[iType];
		const VuAssetFactory::AssetNames &assetNames = VuAssetFactory::IF()->getAssetNames(assetTypes[iType]);
		for ( int iName = 0; iName < (int)assetNames.size(); iName++ )
		{
			const std::string &assetName = assetNames[iName];
			if ( !VuAssetFactory::IF()->wasAssetUsed(assetType.c_str(), assetName.c_str()) )
			{
				mQueue.push(assetType + ":" + assetName);
			}
		}
	}

	mUnusedAssetCount = (int)mQueue.size();
	VUPRINTF("Unused assets:\n", mQueue.size());
}

//*****************************************************************************
void VuUnusedAssetsGameMode::onResultTick(float fdt)
{
	if ( mQueue.size() )
	{
		VUPRINTF("\t%s\n", mQueue.front().c_str());
		mQueue.pop();
		
		if ( mQueue.empty() )
			VUPRINTF("Total Unused Assets: %d\n", mUnusedAssetCount);
	}
}