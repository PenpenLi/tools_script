//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuExpansionFileGameMode
// 
//*****************************************************************************

#include "VuExpansionFileGameMode.h"
#include "VuPurple/Managers/VuProjectManager.h"
#include "VuPurple/Managers/VuFadeManager.h"
#include "VuPurple/Util/VuGameFontMacros.h"
#include "VuEngine/VuEngine.h"
#include "VuEngine/Entities/UI/VuUIScreenEntity.h"
#include "VuEngine/Projects/VuProject.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/GfxSort/VuGfxSort.h"
#include "VuEngine/Dev/VuDevConfig.h"


IMPLEMENT_RTTI(VuExpansionFileGameMode, VuGameMode);


//*****************************************************************************
VuExpansionFileGameMode::VuExpansionFileGameMode():
	mpScreenProject(VUNULL)
{
}

//*****************************************************************************
bool VuExpansionFileGameMode::enter(const std::string &prevMode)
{
	mState = STATE_SUCCESS;

	if ( !VuExpansionFileManager::IF()->openExpansionFile() )
	{
		// load screen
		if ( (mpScreenProject = VuProjectManager::IF()->load("Screens/Downloading")) )
			mpScreenProject->gameInitialize();

		VuFadeManager::IF()->startFadeIn(0.0f);

		// kick off download
		VuExpansionFileManager::IF()->startDownload(this);
		mState = STATE_DOWNLOADING;
	}

	return true;
}

//*****************************************************************************
void VuExpansionFileGameMode::exit()
{
	// unload loading screen
	if ( mpScreenProject )
	{
		mpScreenProject->gameRelease();
		VuProjectManager::IF()->unload(mpScreenProject);
		mpScreenProject = VUNULL;
	}

	VuFadeManager::IF()->startFadeOut(0.0f);
}

//*****************************************************************************
const char *VuExpansionFileGameMode::tick(float fdt)
{
	if ( mState == STATE_DOWNLOADING )
	{
		if ( VuUIScreenEntity *pScreen = getScreen() )
			pScreen->tick(fdt);
	}
	else if ( mState == STATE_SUCCESS )
	{
		//modify by liudan
		mNextGameMode = "Preload";
		// for some platforms skip title sequence (splash screen is shown at launch)
//#if defined VUIOS || defined VUANDROID || defined VUWINPHONE
//		mNextGameMode = "Preload";
//#endif
//
		
		VuDevConfig::IF()->getParam("GameMode").getValue(mNextGameMode);
		VuEngine::IF()->cmdLineArgs().getValue("GameMode", mNextGameMode);

		return mNextGameMode.c_str();
	}
	else if ( mState == STATE_FAILURE )
	{
		return "Shutdown";
	}

	return "";
}

//*****************************************************************************
void VuExpansionFileGameMode::draw()
{
	if ( VuUIScreenEntity *pScreen = getScreen() )
	{
		pScreen->draw();
	}
	else
	{
		VuGfxSort::IF()->setFullScreenLayer(VuGfxSort::FSL_UI);
		VuGfxSort::IF()->setViewport(0);
		VuGfxSort::IF()->setReflectionLayer(0);
		VuGfxSort::IF()->setViewportLayer(VuGfxSort::VPL_UI);

		VuGfxUtil::IF()->submitClearCommand(VUGFX_CLEAR_COLOR, VuColor(0,0,0), 1.0f);
	}
}

//*****************************************************************************
VuUIScreenEntity *VuExpansionFileGameMode::getScreen()
{
	if ( mpScreenProject && mpScreenProject->getRootEntity()->isDerivedFrom(VuUIScreenEntity::msRTTI) )
		return static_cast<VuUIScreenEntity *>(mpScreenProject->getRootEntity());

	return VUNULL;
}

//*****************************************************************************
void VuExpansionFileGameMode::onDownloadSuccess()
{
	mState = STATE_SUCCESS;
}

//*****************************************************************************
void VuExpansionFileGameMode::onDownloadFailure()
{
	mState = STATE_FAILURE;
}

//*****************************************************************************
void VuExpansionFileGameMode::onDownloadProgress(float progress)
{
	char str[16];
	VU_SPRINTF(str, sizeof(str), "%d", VuRound(progress*100));
	VuGameFontMacros::IF()->setMacro("DOWNLOAD_PERCENT", str);
}