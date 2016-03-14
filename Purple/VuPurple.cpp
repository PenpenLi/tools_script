//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Purple library entry point and interface class to Purple application.
// 
//*****************************************************************************

#include "VuPurple.h"
#include "VuPurple/Gfx/GfxComposer/VuPurpleGfxComposer.h"
#include "VuPurple/Modes/VuGameModeManager.h"
#include "VuPurple/Managers/VuSettingsManager.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Managers/VuProjectManager.h"
#include "VuPurple/Managers/VuFadeManager.h"
#include "VuPurple/Managers/VuStatsManager.h"
#include "VuPurple/Managers/VuAnalyticsManager.h"
#include "VuPurple/Managers/VuCollisionManager.h"
#include "VuPurple/Managers/VuCommunityManager.h"
#include "VuPurple/Managers/VuControlMethodManager.h"
#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Managers/VuCloudSaveManager.h"
#include "VuPurple/Managers/VuLeaderboardManager.h"
#include "VuPurple/Managers/VuAchievementManager.h"
#include "VuPurple/Managers/VuGameServicesManager.h"
#include "VuPurple/Managers/VuOutOfBoundsManager.h"
#include "VuPurple/Managers/VuBillingManager.h"
#include "VuPurple/Managers/VuGiftManager.h"
#include "VuPurple/Managers/VuTitleStorageManager.h"
#include "VuPurple/Managers/VuAdManager.h"
#include "VuPurple/Managers/VuVideoShareManager.h"
#include "VuPurple/Managers/VuExpansionFileManager.h"
#include "VuPurple/Managers/VuCloudTuningManager.h"
#include "VuPurple/Managers/VuTipManager.h"
#include "VuPurple/PowerUps/VuPowerUpManager.h"
#include "VuPurple/Entities/Car/Parts/VuCarEffectController.h"
#include "VuPurple/Track/VuTrackManager.h"
#include "VuPurple/AI/VuAiBehavior.h"
#include "VuPurple/AI/VuAiBrain.h"
#include "VuPurple/AI/VuAiManager.h"
#include "VuPurple/Gfx/VuGameGfxUtil.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuPurple/Util/VuGameFontMacros.h"
#include "VuEngine/UI/VuUI.h"
#include "VuEngine/VuApplicationRegistry.h"
#include "VuEngine/Assets/VuAssetFactory.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Dynamics/VuDynamics.h"
#include "VuEngine/Water/VuWater.h"
#include "VuEngine/HAL/Audio/VuAudio.h"


//tony.hu 2014.12.18
#include "VuPurple/Managers/VuSkyGiftManager.h"

// modify by xlm,[/2015/1/28 ] 
#include "VuPurple/Managers/VuNetManager.h"
#include "VuPurple/Managers/VuRankManager.h"
#include "VuPurple/Entities/UI/VuBroadcastManager.h"
#include "VuPurple/Managers/VuPurchaseOrderManager.h"
#include "VuPurple/Managers/VuRequestOrderManager.h"
#include "VuPurple/Managers/VuGiftDialogManager.h"

#include "VuPurple/Managers/VuPackManager.h"


// the game
VuPurple VuPurple::mPurple;


// register functionality
bool VuApplication::initGame()
{
	// create game
	if ( !VuPurple::IF()->initGame() )
		return VUERROR("Unable to init game.");

	return true;
}

bool VuApplication::initEditor()
{
	if ( !VuPurple::IF()->initEditor() )
		return VUERROR("Unable to init game.");
	
	return true;
}

bool VuApplication::initLauncher()
{
	// create game
	if ( !VuPurple::IF()->initLauncher() )
		return VUERROR("Unable to init game.");

	return true;
}

// unregister functionality
void VuApplication::release()
{
	VuPurple::IF()->release();
}

// get name/titleID of this game
const char *VuApplication::getName() { return "Purple"; }
VUUINT32 VuApplication::getTitleID() { return 0x00000000; }


// macros
#define CREATE_COMPONENT(name, initResult)				\
{														\
	if ( VuSys::IF() )									\
		VUPRINTF("Creating %s\n", #name);				\
	extern name *Create##name##Interface();				\
	mSysComponents.push_back(Create##name##Interface());\
	if ( !(initResult) )								\
		return VUERROR("Failed to init %s.", #name);	\
}

//*****************************************************************************
VuPurple::VuPurple()
{
}

//*****************************************************************************
bool VuPurple::initGame()
{
	// register types
	VuApplicationRegistry::addEntityTypes();
	VuApplicationRegistry::addPfxTypes();
	VuApplicationRegistry::addInstigatorTypes();

	VuUI::IF()->setAuthoringScreenScale(1080.0f, 720.0f);

	// set max clock dt
	VuTickManager::IF()->setMaxClockDelta(1.0f/15.0f);

	// create dynamics world
	VuDynamics::IF()->setGravity(VuVector3(0, 0, -2.0f*9.806f));

	// create components
	CREATE_COMPONENT(VuGameUtil, VuGameUtil::IF()->init());
	CREATE_COMPONENT(VuGameGfxUtil, VuGameGfxUtil::IF()->init());
	CREATE_COMPONENT(VuGameServicesManager, VuGameServicesManager::IF()->init());
	CREATE_COMPONENT(VuPurpleGfxComposer, VuPurpleGfxComposer::IF()->init());
	CREATE_COMPONENT(VuCloudTuningManager, VuCloudTuningManager::IF()->init());
	CREATE_COMPONENT(VuPowerUpManager, VuPowerUpManager::IF()->init());
	CREATE_COMPONENT(VuCollisionManager, VuCollisionManager::IF()->init());
	CREATE_COMPONENT(VuSettingsManager, VuSettingsManager::IF()->init());
	CREATE_COMPONENT(VuFadeManager, VuFadeManager::IF()->init());
	CREATE_COMPONENT(VuProjectManager, VuProjectManager::IF()->init());
	CREATE_COMPONENT(VuCarManager, VuCarManager::IF()->init());
	CREATE_COMPONENT(VuTrackManager, VuTrackManager::IF()->init());
	CREATE_COMPONENT(VuAiManager, VuAiManager::IF()->init());
	CREATE_COMPONENT(VuAiBehaviorFactory, VuAiBehaviorFactory::IF()->init());
	CREATE_COMPONENT(VuAiBrainFactory, VuAiBrainFactory::IF()->init());
	CREATE_COMPONENT(VuStatsManager, VuStatsManager::IF()->init());
	CREATE_COMPONENT(VuAnalyticsManager, VuAnalyticsManager::IF()->init());
	CREATE_COMPONENT(VuCommunityManager, VuCommunityManager::IF()->init());
	CREATE_COMPONENT(VuControlMethodManager, VuControlMethodManager::IF()->init());
	CREATE_COMPONENT(VuGameManager, VuGameManager::IF()->init());
	CREATE_COMPONENT(VuCloudSaveManager, VuCloudSaveManager::IF()->init());
	CREATE_COMPONENT(VuLeaderboardManager, VuLeaderboardManager::IF()->init());
	CREATE_COMPONENT(VuAchievementManager, VuAchievementManager::IF()->init());
	CREATE_COMPONENT(VuOutOfBoundsManager, VuOutOfBoundsManager::IF()->init());
	CREATE_COMPONENT(VuBillingManager, VuBillingManager::IF()->init());
	CREATE_COMPONENT(VuGiftManager, VuGiftManager::IF()->init());
	CREATE_COMPONENT(VuTitleStorageManager, VuTitleStorageManager::IF()->init());
	CREATE_COMPONENT(VuAdManager, VuAdManager::IF()->init());
	CREATE_COMPONENT(VuPackManager, VuPackManager::IF()->init());
	CREATE_COMPONENT(VuVideoShareManager, VuVideoShareManager::IF()->init());
	CREATE_COMPONENT(VuExpansionFileManager, VuExpansionFileManager::IF()->init());
	CREATE_COMPONENT(VuGameFontMacros, VuGameFontMacros::IF()->init());
	CREATE_COMPONENT(VuTipManager, VuTipManager::IF()->init());
	CREATE_COMPONENT(VuRankManager, VuRankManager::IF()->init());
	CREATE_COMPONENT(VuBroadcastManager, VuBroadcastManager::IF()->init());
	CREATE_COMPONENT(VuGameModeManager, VuGameModeManager::IF()->init());
	//CREATE_COMPONENT(VuPurchaseOrderManager,VuPurchaseOrderManager::IF()->init());
	CREATE_COMPONENT(VuRequestOrderManager, VuRequestOrderManager::IF()->init());
	CREATE_COMPONENT(VuGiftDialogManager, VuGiftDialogManager::IF()->init());
	////SKYCN add by tony.hu
	CREATE_COMPONENT(VuSkyGiftManager, VuSkyGiftManager::IF()->init());

	// modify by xlm,[/2015/1/28 ] 
	CREATE_COMPONENT(VuNetManager, VuNetManager::IF()->init());

	// no matter what, we want to preload the global audio bank
	VuAssetFactory::IF()->preloadAsset("VuAudioBankAsset", "Global");
	VuAssetFactory::IF()->preloadAsset("VuAudioProjectAsset", "Global");

	// post-init
	for ( SysComponents::iterator iter = mSysComponents.begin(); iter != mSysComponents.end(); iter++ )
		(*iter)->postInit();

	return true;
}

//*****************************************************************************
bool VuPurple::initEditor()
{
	// register types
	VuApplicationRegistry::addEntityTypes();
	VuApplicationRegistry::addPfxTypes();
	VuApplicationRegistry::addInstigatorTypes();

	VuUI::IF()->setAuthoringScreenScale(1080.0f, 720.0f);

	// create components
	CREATE_COMPONENT(VuGameUtil, VuGameUtil::IF()->init());
	CREATE_COMPONENT(VuGameGfxUtil, VuGameGfxUtil::IF()->init());
	CREATE_COMPONENT(VuPowerUpManager, VuPowerUpManager::IF()->init());

	// post-init
	for ( SysComponents::iterator iter = mSysComponents.begin(); iter != mSysComponents.end(); iter++ )
		(*iter)->postInit();

	return true;
}

//*****************************************************************************
bool VuPurple::initLauncher()
{
	// create components
	CREATE_COMPONENT(VuGameUtil, VuGameUtil::IF()->init());
	CREATE_COMPONENT(VuGameGfxUtil, VuGameGfxUtil::IF()->init());

	// post-init
	for ( SysComponents::iterator iter = mSysComponents.begin(); iter != mSysComponents.end(); iter++ )
		(*iter)->postInit();

	return true;
}

//*****************************************************************************
void VuPurple::release()
{
	// broadcast
	VuEventManager::IF()->broadcast("OnExitApp");

	// pre-release
	for ( SysComponents::reverse_iterator iter = mSysComponents.rbegin(); iter != mSysComponents.rend(); iter++ )
		(*iter)->preRelease();

	// release interfaces (in reverse order)
	for ( SysComponents::reverse_iterator iter = mSysComponents.rbegin(); iter != mSysComponents.rend(); iter++ )
	{
		(*iter)->release();
		(*iter)->resetIF();
		delete (*iter);
	}

	mSysComponents.clear();
}

//*****************************************************************************
bool VuPurple::exitToOS()
{
	return VuGameModeManager::IF()->exitToOS();
}
