//*****************************************************************************
//
//  Copyright (c) 2007-2008 Ralf Knoesel
//  Copyright (c) 2008-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  GameModeManager class
// 
//*****************************************************************************

#include "VuGameModeManager.h"
#include "VuPurple/Modes/VuFrontEndGameMode.h"
#include "VuPurple/Modes/VuActionGameMode.h"
#include "VuPurple/Modes/VuTestGameMode.h"
#include "VuPurple/Modes/VuPreviewGameMode.h"
#include "VuPurple/Modes/VuSandboxGameMode.h"
#include "VuPurple/Modes/VuAssetConvertGameMode.h"
#include "VuPurple/Modes/VuUnusedAssetsGameMode.h"
#include "VuPurple/Modes/VuTitleSequenceGameMode.h"
#include "VuPurple/Modes/VuPreloadGameMode.h"
#include "VuPurple/Modes/VuResultGameMode.h"
#include "VuPurple/Modes/VuSplitScreenGameMode.h"
#include "VuPurple/Modes/VuAiTestGameMode.h"
#include "VuPurple/Modes/VuExpansionFileGameMode.h"
#include "VuPurple/Managers/VuCommunityManager.h"
#include "VuPurple/Gfx/GfxComposer/VuPurpleGfxComposer.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/UI/VuUI.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Managers/VuDrawManager.h"
#include "VuEngine/Gfx/GfxSort/VuGfxSort.h"
#include "VuEngine/Json/VuJsonContainer.h"
#include "VuEngine/Util/VuCmdLineArgs.h"


// internal data
class VuGameModeManagerImpl : public VuGameModeManager
{
public:
	VuGameModeManagerImpl();

	virtual bool	init();
	virtual void	release();

	virtual bool	exitToOS()		{ return mpCurMode == VUNULL; }

private:
	void			tick(float fdt);
	void			draw();
	void			updateCropMatrix(VUHANDLE context);
	VuGameMode		*createMode(const std::string &mode);

	std::string		mCurMode;
	std::string		mNextMode;
	VuGameMode		*mpCurMode;
};

// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuGameModeManager, VuGameModeManagerImpl);


//*****************************************************************************
VuGameModeManagerImpl::VuGameModeManagerImpl():
	mpCurMode(VUNULL)
{
}

//*****************************************************************************
bool VuGameModeManagerImpl::init()
{
	// start by downloading expansion file (if necessary)
	//modify by liudan
	mNextMode = "ExpansionFile";
	//mNextMode = "Preload";
	

	// register phased tick
	VuTickManager::IF()->registerHandler(this, &VuGameModeManagerImpl::tick, "Decision");

	// register draw
	VuDrawManager::IF()->registerHandler(this, &VuGameModeManagerImpl::draw);

	return true;
}

//*****************************************************************************
void VuGameModeManagerImpl::release()
{
	// exit current state
	if ( mpCurMode )
		mpCurMode->exit();
	delete mpCurMode;

	// unregister phased tick
	VuTickManager::IF()->unregisterHandlers(this);

	// unregister draw
	VuDrawManager::IF()->unregisterHandler(this);
}

//*****************************************************************************
void VuGameModeManagerImpl::tick(float fdt)
{
	// handle state change
	if ( mNextMode.length() )
	{
		VuGfxSort::IF()->flush();

		// exit and destroy old mode
		if ( mpCurMode )
		{
			VUPRINTF("Exiting %s Game Mode\n", mCurMode.c_str());
			mpCurMode->exit();
			delete mpCurMode;
		}

		// create and enter new mode
		mpCurMode = createMode(mNextMode);
		if ( mpCurMode )
		{
			VUPRINTF("Entering %s Game Mode\n", mNextMode.c_str());
			if ( !mpCurMode->enter(mCurMode) )
				VuSys::IF()->exitWithError("Unable to enter new game mode.");
		}
		
		mCurMode = mNextMode;
		mNextMode.clear();
	}

	// tick FSM
	if ( mpCurMode )
		mNextMode = mpCurMode->tick(fdt);
}

//*****************************************************************************
void VuGameModeManagerImpl::draw()
{
	// update UI crop matrix
	updateCropMatrix(VUNULL);

	if ( mpCurMode )
		mpCurMode->draw();
}

//*****************************************************************************
void VuGameModeManagerImpl::updateCropMatrix(VUHANDLE context)
{
	float authAR = VuUI::IF()->getAuthoringAspectRatio();
	float realAR = VuGameUtil::IF()->calcDisplayAspectRatio(context);

	VuMatrix mat = VuMatrix::identity();
	float textScale = 1.0f;
	
	if ( realAR < authAR )
	{
		mat.scale(VuVector3(1.0f, realAR/authAR, 1.0f));
		mat.translate(VuVector3(0.0f, 0.5f*(1 - realAR/authAR), 0.0f));
		textScale = realAR/authAR;
	}
	else if ( realAR > authAR )
	{
		mat.scale(VuVector3(authAR/realAR, 1.0f, 1.0f));
		mat.translate(VuVector3(0.5f*(1 - authAR/realAR), 0.0f, 0.0f));
	}

	VuUI::IF()->setCropMatrix(mat);
	VuUI::IF()->setTextScale(textScale);
}

//*****************************************************************************
VuGameMode *VuGameModeManagerImpl::createMode(const std::string &mode)
{
	if ( mode == "FrontEnd" )		return new VuFrontEndGameMode;
	if ( mode == "Action" )			return new VuActionGameMode;
	if ( mode == "Test" )			return new VuTestGameMode;
	if ( mode == "Preview" )		return new VuPreviewGameMode;
	if ( mode == "Sandbox" )		return new VuSandboxGameMode;
	if ( mode == "AssetConvert" )	return new VuAssetConvertGameMode;
	if ( mode == "UnusedAssets" )	return new VuUnusedAssetsGameMode;
	if ( mode == "TitleSequence" )	return new VuTitleSequenceGameMode;
	if ( mode == "Preload" )		return new VuPreloadGameMode;
	if ( mode == "Result" )			return new VuResultGameMode;
	if ( mode == "SplitScreen" )	return new VuSplitScreenGameMode;
	if ( mode == "AiTest" )			return new VuAiTestGameMode;
	if ( mode == "ExpansionFile" )	return new VuExpansionFileGameMode;
	if ( mode == "Shutdown" )		return VUNULL;

	VUASSERT(0, "VuGameModeManagerImpl::createMode() invalid mode requested");

	return VUNULL;
}