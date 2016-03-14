//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  GameRegistry class
// 
//*****************************************************************************

#include "VuEngine/VuApplicationRegistry.h"
#include "VuEngine/Entities/VuEntityFactory.h"
#include "VuEngine/Pfx/VuPfx.h"
#include "VuEngine/Pfx/VuPfxRegistry.h"
#include "VuEngine/Managers/VuTriggerManager.h"


//*****************************************************************************
void VuApplicationRegistry::addEntityTypes()
{
	if ( VuEntityFactory::IF() == VUNULL )
		return;

	// Engine

	REGISTER_ENTITY(VuPlayAnimationEntity)
	REGISTER_ENTITY(VuPlayAdditiveAnimationEntity)
	REGISTER_ENTITY(VuAttachEntity)
	REGISTER_ENTITY(VuFadeEntity)
	REGISTER_ENTITY(VuSubstituteAssetEntity)
	REGISTER_ENTITY(VuSwapSkyboxEntity)

	REGISTER_ENTITY(VuAudioEventEntity)
	REGISTER_ENTITY(VuAudioEmitterEntity)
	REGISTER_ENTITY(VuAudioReverbEntity)
	REGISTER_ENTITY(VuAudioAmbientReverbEntity)
	REGISTER_ENTITY(VuAudioDspEntity)
	REGISTER_ENTITY(VuAudioDuckingEntity)
	REGISTER_ENTITY(VuDolbyAudioEntity)

	REGISTER_ENTITY(VuContainerEntity)
	REGISTER_ENTITY(VuTransitionEntity)
	REGISTER_ENTITY(VuGroupEntity)
	REGISTER_ENTITY(VuScriptGroupEntity)
	REGISTER_ENTITY(VuZoneEntity)

	REGISTER_ENTITY(VuOnProjectInitializedEntity)
	REGISTER_ENTITY(VuOnProjectReleaseEntity)
	REGISTER_ENTITY(VuOnKeyboardEventEntity)
	REGISTER_ENTITY(VuGenericEventEntity)

	REGISTER_ENTITY(VuAmbientLightEntity)
	REGISTER_ENTITY(VuDirectionalLightEntity)
	REGISTER_ENTITY(VuDynamicLightEntity)

	REGISTER_ENTITY(VuPropEntity)
	REGISTER_ENTITY(VuAnimatedPropEntity)
	REGISTER_ENTITY(VuDynamicPropEntity)
	REGISTER_ENTITY(VuSkyBoxEntity)
	REGISTER_ENTITY(VuStaticPfxEntity)
	REGISTER_ENTITY(VuOneShotPfxEntity)
	REGISTER_ENTITY(VuCoronaEntity)
	REGISTER_ENTITY(VuCoronaOccluderEntity)
	REGISTER_ENTITY(VuDirectionalCoronaEntity)
	REGISTER_ENTITY(VuFoliageEntity)
	REGISTER_ENTITY(VuLensWaterEmitterEntity)
	REGISTER_ENTITY(VuExplosionEntity)
	REGISTER_ENTITY(VuZoneMaskBoxEntity)
	REGISTER_ENTITY(VuLensFlareEntity)
	REGISTER_ENTITY(VuLocatorEntity)

	REGISTER_ENTITY(VuBooleanEntity)
	REGISTER_ENTITY(VuStringEntity)
	REGISTER_ENTITY(VuCounterEntity)
	REGISTER_ENTITY(VuIfEntity)
	REGISTER_ENTITY(VuCompareBooleansEntity)
	REGISTER_ENTITY(VuCompareIntegersEntity)
	REGISTER_ENTITY(VuCompareIntegerToConstantEntity)
	REGISTER_ENTITY(VuCompareStringsEntity)
	REGISTER_ENTITY(VuPersistentBooleanEntity)
	REGISTER_ENTITY(VuPersistentIntegerEntity)
	REGISTER_ENTITY(VuPersistentStringEntity)
	REGISTER_ENTITY(VuSetBooleanEntity)
	REGISTER_ENTITY(VuSetIntegerEntity)
	REGISTER_ENTITY(VuSetStringEntity)
	REGISTER_ENTITY(VuOneShotFilterEntity)
	REGISTER_ENTITY(VuSequencerEntity)
	REGISTER_ENTITY(VuDelayEntity)
	REGISTER_ENTITY(VuMultiDelayEntity)
	REGISTER_ENTITY(VuTriggerArrayEntity)
	REGISTER_ENTITY(VuTriggerForwarderEntity)
	REGISTER_ENTITY(VuRandomTriggerEntity)
	REGISTER_ENTITY(VuPlatformFilterEntity)
	REGISTER_ENTITY(VuBuildFilterEntity)
	REGISTER_ENTITY(VuSysCapsEntity)
	REGISTER_ENTITY(VuLicenseEntity)
	REGISTER_ENTITY(VuCapacitorEntity)
	REGISTER_ENTITY(VuFrequencyCapEntity)
	REGISTER_ENTITY(VuScriptGroupInputEntity)
	REGISTER_ENTITY(VuScriptGroupOutputEntity)

	REGISTER_ENTITY(VuIntegerEntity)
	REGISTER_ENTITY(VuAddIntegerConstantEntity)
	REGISTER_ENTITY(VuAddIntegersEntity)
	REGISTER_ENTITY(VuSubtractIntegersEntity)

	REGISTER_ENTITY(VuWaterSurfaceEntity)
	REGISTER_ENTITY(VuDirectionalWaveEntity)
	REGISTER_ENTITY(VuInfiniteOceanWaveEntity)
	REGISTER_ENTITY(VuRectangularOceanWaveEntity)
	REGISTER_ENTITY(VuCircularOceanWaveEntity)
	REGISTER_ENTITY(VuWhirlpoolWaveEntity)
	REGISTER_ENTITY(VuWaterRampEntity)
	REGISTER_ENTITY(VuDirectionalFlowWaveEntity)
	REGISTER_ENTITY(VuStaticWakeWaveEntity)
	REGISTER_ENTITY(VuWakeWaveEntity)
	REGISTER_ENTITY(VuPointWaveEntity)
	REGISTER_ENTITY(VuBumpWaveEntity)
	REGISTER_ENTITY(VuBankedTurnWaveEntity)

	REGISTER_ENTITY(VuWaterTextureSettingsEntity)
	REGISTER_ENTITY(VuGlobalGfxSettingsEntity)
	REGISTER_ENTITY(VuEllipticalGfxSettingsEntity)
	REGISTER_ENTITY(VuRectangularGfxSettingsEntity)

	REGISTER_ENTITY(VuTriggerBoxEntity)
	REGISTER_ENTITY(VuTriggerPlaneEntity)

REGISTER_ENTITY(VuRotateMotionEntity)
REGISTER_ENTITY(VuKeyframeMotionEntity)
REGISTER_ENTITY(VuKeyframeEntity)
REGISTER_ENTITY(VuTransformEntity)

REGISTER_ENTITY(VuPfxProjectEntity)
REGISTER_ENTITY(VuAudioBankEntity)
REGISTER_ENTITY(VuAudioProjectEntity)

REGISTER_ENTITY(VuUIScreenEntity)
REGISTER_ENTITY(VuUIImageEntity)
REGISTER_ENTITY(VuUITextEntity)
REGISTER_ENTITY(VuUIScrollingTextEntity)
REGISTER_ENTITY(VuUIDebugTextEntity)
REGISTER_ENTITY(VuUIGamePadInputEntity)
REGISTER_ENTITY(VuUIPageLayoutEntity)
REGISTER_ENTITY(VuUITouchButtonEntity)
REGISTER_ENTITY(VuUITouchSwipeEntity)
REGISTER_ENTITY(VuUIPfxEntity)
REGISTER_ENTITY(VuUIToastImageEntity)
REGISTER_ENTITY(VuUINewsImageEntity)
REGISTER_ENTITY(VuUINewsTextEntity)

REGISTER_ENTITY(VuDialogEntity)
REGISTER_ENTITY(VuCloseDialogEntity)

REGISTER_ENTITY(VuMessageBoxEntity)
REGISTER_ENTITY(VuCloseMessageBoxEntity)


// Game

REGISTER_ENTITY(VuGameSceneEntity)

REGISTER_ENTITY(VuUnlockAchievementEntity)
REGISTER_ENTITY(VuUpdateAchievementEntity)
REGISTER_ENTITY(VuSetTrackBranchProbabilities)
REGISTER_ENTITY(VuLogAnalyticEventEntity)
REGISTER_ENTITY(VuZimonLogAnalyticEventEntity)
REGISTER_ENTITY(VuSwitchHeadlightsEntity)
REGISTER_ENTITY(VuFinishGameEntity)
REGISTER_ENTITY(VuCheatPurchaseCurrencyEntity)
REGISTER_ENTITY(VuGiveCurrencyEntity)
REGISTER_ENTITY(VuSetSetChampStageEntity)
REGISTER_ENTITY(VuSetFrontEndCameraTransitionTimeEntity)
REGISTER_ENTITY(VuDisplayNotificationsEntity)


REGISTER_ENTITY(VuSetOwnedBigGiftEntity)
REGISTER_ENTITY(VuCheckLoginRewardEntity)




REGISTER_ENTITY(VuOnTouchSpecialEntity)
REGISTER_ENTITY(VuOnAnyButtonOrKeyEntity)
REGISTER_ENTITY(VuStartLightsEntity)
REGISTER_ENTITY(VuOnStartCinematicIntroEntity)
REGISTER_ENTITY(VuOnPreGameEnterEntity)
REGISTER_ENTITY(VuOnCarFinishedEntity)
REGISTER_ENTITY(VuPedestalControlEntity)

REGISTER_ENTITY(VuIsEventTypeEntity)
REGISTER_ENTITY(VuHumanFilterEntity)
REGISTER_ENTITY(VuIsDemoEntity)
REGISTER_ENTITY(VuIsSkuEntity)
REGISTER_ENTITY(VuOncePerLapFilterEntity)
REGISTER_ENTITY(VuStarsEarnedEntity)
REGISTER_ENTITY(VuGameServicesEntity)
REGISTER_ENTITY(VuPlayerResultEntity)
REGISTER_ENTITY(VuIsGamePadConnectedEntity)
REGISTER_ENTITY(VuIsGamePadDeviceTypeEntity)
REGISTER_ENTITY(VuGetTouchMethodEntity)
REGISTER_ENTITY(VuIsControllerActiveEntity)
REGISTER_ENTITY(VuGetBestPlaceEntity)
REGISTER_ENTITY(VuGetPlaceEntity)
REGISTER_ENTITY(VuGetCarChampPlaceEntity)
REGISTER_ENTITY(VuGetCarChampBestPlaceEntity)
REGISTER_ENTITY(VuGetCarChampStageEntity)
REGISTER_ENTITY(VuTuneUpsEntity)
REGISTER_ENTITY(VuTokensEntity)
REGISTER_ENTITY(VuChampStateEntity)
	REGISTER_ENTITY(VuCarChampStateEntity)
	REGISTER_ENTITY(VuNextCarChampTrackImageEntity)
	REGISTER_ENTITY(VuCreateCarChampEntity)
	REGISTER_ENTITY(VuCarChampStandingsTableEntity)
	REGISTER_ENTITY(VuWasLastScreenEntity)
REGISTER_ENTITY(VuIsScreenStackTopEntity)
REGISTER_ENTITY(VuDailyChallengeEntity)
REGISTER_ENTITY(VuCreateDailyChallengeEntity)
REGISTER_ENTITY(VuDailyChallengeDayEntity)
REGISTER_ENTITY(VuVideoShareEntity)
REGISTER_ENTITY(VuAreAdsDisabledEntity)
REGISTER_ENTITY(VuAreCoinsDoubledEntity)
REGISTER_ENTITY(VuIsSplitScreenUnlockedEntity)
REGISTER_ENTITY(VuDisallowPauseMenuEntity)
REGISTER_ENTITY(VuUnlockedStageEntity)
REGISTER_ENTITY(VuCheckCurrencyEntity)
REGISTER_ENTITY(VuBossFilterEntity)
REGISTER_ENTITY(VuCurCarFilterEntity)
REGISTER_ENTITY(VuIsConfirmedPirateEntity)
REGISTER_ENTITY(VuIsBossBeatenEntity)
REGISTER_ENTITY(VuIsSpecialSeriesEventEntity)
REGISTER_ENTITY(VuIsNoStarSeriesEventEntity)
REGISTER_ENTITY(VuIsNoStarSpecialSeriesEntity)

REGISTER_ENTITY(VuIsNo3StarNoRewardSeriesEventEntity)
REGISTER_ENTITY(VuIsSeriesStageEventEntity);
REGISTER_ENTITY(VuIsHaveSpecialCarCheckEntity);
REGISTER_ENTITY(VuBillingCheckEntity)

	REGISTER_ENTITY(VuIsShowImmButtonTipEntity)
	REGISTER_ENTITY(VuCanCarChampChallengeEntity)
	REGISTER_ENTITY(VuCanReceiveLoginRewardEntity)
	REGISTER_ENTITY(VuIsShow15DayLoginEntity)
	REGISTER_ENTITY(VuCanConnectServerEntity)
	REGISTER_ENTITY(VuPlayerInfoEntity)
	REGISTER_ENTITY(VuIsCanFullLevelCarEntity)
	REGISTER_ENTITY(VuCheckTunesUpNumEntity)

	REGISTER_ENTITY(VuTuningBooleanEntity)
	REGISTER_ENTITY(VuTuningIntegerEntity)
	REGISTER_ENTITY(VuTuningPriceEntity)

	REGISTER_ENTITY(VuCloudSpecialEntity)
	REGISTER_ENTITY(VuSpecialTimerEntity)
	REGISTER_ENTITY(VuCheckSpecialsEntity)
	REGISTER_ENTITY(VuCheckSpecialEntity)
	REGISTER_ENTITY(VuSpecialsEntity)

	REGISTER_ENTITY(VuCarEntity)
	REGISTER_ENTITY(VuShowroomCarEntity)
	REGISTER_ENTITY(VuLoungeDriverEntity)
	REGISTER_ENTITY(VuGarageCarEntity)
	REGISTER_ENTITY(VuPodiumCarEntity)
	REGISTER_ENTITY(VuCarChampPodiumCarEntity)
	REGISTER_ENTITY(VuDailyChallengeCarEntity)
	REGISTER_ENTITY(VuGarageDriverEntity)
	REGISTER_ENTITY(VuPodiumDriverEntity)
	REGISTER_ENTITY(VuCarChampPodiumDriverEntity)
	REGISTER_ENTITY(VuDailyChallengeDriverEntity)
	REGISTER_ENTITY(VuRampEntity)
	REGISTER_ENTITY(VuAiWaypointEntity)
	REGISTER_ENTITY(VuFrontEndCameraEntity)
	REGISTER_ENTITY(VuTestCameraEntity)
	REGISTER_ENTITY(VuJumpPointEntity)
	REGISTER_ENTITY(VuDevJumpPointEntity)
	REGISTER_ENTITY(VuGridPositionEntity)
	REGISTER_ENTITY(VuGamePropEntity)
	REGISTER_ENTITY(VuBreakableGamePropEntity)
	REGISTER_ENTITY(VuDynamicGamePropEntity)
	REGISTER_ENTITY(VuDynamicBreakableGamePropEntity)
	REGISTER_ENTITY(VuAnimatedGamePropEntity)
	REGISTER_ENTITY(VuRectangularDirectionalForceAffectorEntity)
	REGISTER_ENTITY(VuOutOfBoundsBoxEntity)
	REGISTER_ENTITY(VuCinematicIntroCameraEntity)
	REGISTER_ENTITY(VuLavaSurfaceEntity)
	REGISTER_ENTITY(VuAnimatedBreakableRagdollEntity)
	REGISTER_ENTITY(VuRigidBodyDeactivationEntity)
	REGISTER_ENTITY(VuPowerUpEntity)
	REGISTER_ENTITY(VuCarBarrierEntity)
	REGISTER_ENTITY(VuSpringEntity)
	REGISTER_ENTITY(VuCarEffectEntity)
	REGISTER_ENTITY(VuGhostCarPlaybackEntity)

	REGISTER_ENTITY(VuGameWhirlpoolEntity)

	REGISTER_ENTITY(VuSetScreenUIAction)
	REGISTER_ENTITY(VuPushScreenUIAction)
	REGISTER_ENTITY(VuPopScreenUIAction)
	REGISTER_ENTITY(VuSetScreenStackUIAction)
	REGISTER_ENTITY(VuNextScreenUIAction)
	REGISTER_ENTITY(VuPushNewsScreenUIAction)
	REGISTER_ENTITY(VuStartActionGameUIAction)
	REGISTER_ENTITY(VuEndActionGameUIAction)
	REGISTER_ENTITY(VuSetDefaultPauseMenuUIAction)
	REGISTER_ENTITY(VuPauseActionGameUIAction)
	REGISTER_ENTITY(VuResumeActionGameUIAction)
	REGISTER_ENTITY(VuRestartActionGameUIAction)
	REGISTER_ENTITY(VuStartSplitScreenGameUIAction)
	REGISTER_ENTITY(VuShutdownAppUIAction)
	REGISTER_ENTITY(VuShowWebPageUIAction)
	REGISTER_ENTITY(VuShowMarketUIAction)
	REGISTER_ENTITY(VuShowTwitterPageUIAction)
	REGISTER_ENTITY(VuShowFacebookPageUIAction)
	REGISTER_ENTITY(VuShowGooglePlusPageUIAction)
	REGISTER_ENTITY(VuRateGameUIAction)
	REGISTER_ENTITY(VuGameRematchUIAction)
	REGISTER_ENTITY(VuShowStandingsUIAction)
	REGISTER_ENTITY(VuSkipIntroUIAction)
	REGISTER_ENTITY(VuSkipLoadingScreenUIAction)
	REGISTER_ENTITY(VuPaintUIAction)
	REGISTER_ENTITY(VuDisableInputUIAction)
	REGISTER_ENTITY(VuEnableInputUIAction)
	REGISTER_ENTITY(VuRestorePurchasesUIAction)
	REGISTER_ENTITY(VuShowTipUIAction)

	REGISTER_ENTITY(VuGameButtonEntity)
	REGISTER_ENTITY(VuCarButtonEntity)
	REGISTER_ENTITY(VuDriverButtonEntity)
	REGISTER_ENTITY(VuDriverLockedButtonEntity)
	REGISTER_ENTITY(VuPromoteGameButtonEntity)


	REGISTER_ENTITY(VuBigGiftButtonEntity)
	REGISTER_ENTITY(VuFirstChargeButtonEntity)
	REGISTER_ENTITY(VuLoginRewardButtonEntity)


	REGISTER_ENTITY(Vu15DayRewardListEntity)
	REGISTER_ENTITY(VuFullLvlCarListEntity)
	REGISTER_ENTITY(VuGameImageEntity)
	REGISTER_ENTITY(VuGameTextEntity)
	REGISTER_ENTITY(VuChampListEntity)
	REGISTER_ENTITY(VuCarChampListEntity)
	REGISTER_ENTITY(VuSeriesListEntity)
	REGISTER_ENTITY(VuEventListEntity)
	REGISTER_ENTITY(VuCustomListEntity)
	REGISTER_ENTITY(VuCustomListItemEntity)
	REGISTER_ENTITY(VuRewardTextEntity)
	REGISTER_ENTITY(VuGameStatsEntity)
	REGISTER_ENTITY(VuAchievementsEntity)
	REGISTER_ENTITY(VuRankListsEntity)
	REGISTER_ENTITY(VuBroadcastEntity)
	REGISTER_ENTITY(VuCheckBoxEntity)
	REGISTER_ENTITY(VuProgressBarEntity)
	REGISTER_ENTITY(VuEventImageEntity)
	REGISTER_ENTITY(VuNextEventImageEntity)
	REGISTER_ENTITY(VuCharacterAbilityImageEntity)
	REGISTER_ENTITY(VuBossImageEntity)
	REGISTER_ENTITY(VuBossAbilityImageEntity)
	REGISTER_ENTITY(VuGameTypeImageEntity)
	REGISTER_ENTITY(VuSeriesStageImageEntity)
	REGISTER_ENTITY(VuChampStageImageEntity)
	REGISTER_ENTITY(VuCurrentStageImageEntity)
	REGISTER_ENTITY(VuNextStageImageEntity)
	REGISTER_ENTITY(VuCarChampStageImageEntity)
	REGISTER_ENTITY(VuSplitScreenSelectEntity)
	REGISTER_ENTITY(VuSplitScreenConfigEntity)
	REGISTER_ENTITY(VuCarDriverConfigEntity)
	REGISTER_ENTITY(VuShowroomListEntity)
	REGISTER_ENTITY(VuShowroomListItemEntity)
	REGISTER_ENTITY(VuLoungeListEntity)
	REGISTER_ENTITY(VuLoungeListItemEntity)
	REGISTER_ENTITY(VuPowerUpListEntity)
	REGISTER_ENTITY(VuPowerUpGameEntity)
	REGISTER_ENTITY(VuLeaderboardEntity)
	REGISTER_ENTITY(VuCalendarEntity)
	REGISTER_ENTITY(VuCheckCalendarEntity)
	REGISTER_ENTITY(VuColorWheelEntity)
	REGISTER_ENTITY(VuDecalWheelEntity)
	REGISTER_ENTITY(VuRewardWheelEntity)
	REGISTER_ENTITY(VuPowerUpImageEntity)
	REGISTER_ENTITY(VuGameToastImageEntity)
	REGISTER_ENTITY(VuTuneUpButtonEntity)

	REGISTER_ENTITY(VuRaceResultsTableEntity)
	REGISTER_ENTITY(VuBoostBlitzResultsTableEntity)
	REGISTER_ENTITY(VuGameResultsTableEntity)
	REGISTER_ENTITY(VuSplitScreenResultsTableEntity)
	REGISTER_ENTITY(VuSplitScreenStandingsTableEntity)


	
	REGISTER_ENTITY(VuPurchaseCarEntity)
	REGISTER_ENTITY(VuIsRmbPurchaseCarEntity)
	REGISTER_ENTITY(VuRMBPurchaseCarEntity)
	REGISTER_ENTITY(VuPurchaseUpgradedCarEntity)
	REGISTER_ENTITY(VuPurchaseUpgradeEntity)
	REGISTER_ENTITY(VuGiveStageEntity)
	REGISTER_ENTITY(VuPurchaseDriverEntity)
	REGISTER_ENTITY(VuPurchasePowerUpEntity)
	REGISTER_ENTITY(VuPurchaseTuneUpsEntity)
	REGISTER_ENTITY(VuPurchaseTokenRefillEntity)
	REGISTER_ENTITY(VuPurchaseTokenRefillExEntity)

	REGISTER_ENTITY(VuPurchaseInfoEntity)
	REGISTER_ENTITY(VuStoreItemEntity)

	REGISTER_ENTITY(VuPurchaseNonRmbItemEntity)

	REGISTER_ENTITY(VuReceiveLoginItemEntity)

	REGISTER_ENTITY(VuGameTimerTextEntity)
	REGISTER_ENTITY(VuPackTimerTextEntity)

	REGISTER_ENTITY(VuGameAlternativeTextEntity)


	REGISTER_ENTITY(VuSetDefaultSettingsEntity)
	REGISTER_ENTITY(VuSaveSettingsEntity)
	REGISTER_ENTITY(VuTouchMethodSettingsEntity)
	REGISTER_ENTITY(VuSteeringSensitivitySettingsEntity)
	REGISTER_ENTITY(VuCameraTiltSettingsEntity)
	REGISTER_ENTITY(VuAutoThrottleGamePadSettingsEntity)
	REGISTER_ENTITY(VuAutoThrottleKeyboardSettingsEntity)
	REGISTER_ENTITY(VuSetAutoThrottleGamePadEntity)
	REGISTER_ENTITY(VuEffectVolumeSettingsEntity)
	REGISTER_ENTITY(VuMusicVolumeSettingsEntity)
	REGISTER_ENTITY(VuDisplayScaleSettingsEntity)
	REGISTER_ENTITY(VuShadowDetailSettingsEntity)
	REGISTER_ENTITY(VuWaterNormalMapSettingsEntity)
	REGISTER_ENTITY(VuWaterHighDetailSettingsEntity)
	REGISTER_ENTITY(VuWaterReflectionSettingsEntity)
	REGISTER_ENTITY(VuWaterWakesSettingsEntity)
	REGISTER_ENTITY(VuLensWaterEffectSettingsEntity)
	REGISTER_ENTITY(VuRadialBlurEffectSettingsEntity)
	REGISTER_ENTITY(VuColorCorrectionEffectSettingsEntity)
	REGISTER_ENTITY(VuHighModelLODSettingsEntity)
	REGISTER_ENTITY(VuHighTextureLODSettingsEntity)
	REGISTER_ENTITY(VuShaderLODSettingsEntity)
	REGISTER_ENTITY(Vu60HzSettingsEntity)
	REGISTER_ENTITY(VuSceneComplexitySettingsEntity)
	REGISTER_ENTITY(VuSafeZoneSettingsEntity)

	REGISTER_ENTITY(VuNewsInfoEntity)
	REGISTER_ENTITY(VuNewsTrackEntity)

	REGISTER_ENTITY(VuCarStatEntity)
	REGISTER_ENTITY(VuLevelUpStatEntity)

	REGISTER_ENTITY(VuGiftCodeEntity)
	REGISTER_ENTITY(VuUINavigationNodeEntity)

	REGISTER_ENTITY(VuHUDEntity)
	REGISTER_ENTITY(VuHUDTextElementEntity)
	REGISTER_ENTITY(VuHUDPauseButtonEntity)
	REGISTER_ENTITY(VuHUDOnScreenButtonEntity)
	REGISTER_ENTITY(VuHUDOnScreenAxisEntity)
	REGISTER_ENTITY(VuHUDLineMapEntity)
	REGISTER_ENTITY(VuHUDCountdownTimerEntity)
	REGISTER_ENTITY(VuHUDEliminatedNameEntity)
	REGISTER_ENTITY(VuHUDPowerUpSlotEntity)
	REGISTER_ENTITY(VuHUDFixedPowerUpSlotEntity)

	REGISTER_ENTITY(VuHUDScorePopupEntity)
	REGISTER_ENTITY(VuHUDScoreThermometerEntity)
	REGISTER_ENTITY(VuHUDCharacterAbilityEntity)
	REGISTER_ENTITY(VuHUDHitNotificationEntity)
	REGISTER_ENTITY(VuBossDialogStausEntity)
	REGISTER_ENTITY(VuPackInfoEntity)

	REGISTER_ENTITY(VuAdUnitEntity)
	REGISTER_ENTITY(VuCheckForAdsEntity)

	REGISTER_ENTITY(VuCheckSimTypeEntity)

	REGISTER_ENTITY(VuMoreGameEntity)
//SKYCN add by tony.hu
	REGISTER_ENTITY(VuSkyGiftEntity)
	REGISTER_ENTITY(VuIsBigPaymentEntity)
	REGISTER_ENTITY(VuIsPopDialogVersionEntity)
	//add by zc
	REGISTER_ENTITY(VuGiftDialogEntity)
	REGISTER_ENTITY(VuDialogBeforeGameEntity)
	REGISTER_ENTITY(VuDialogInPlayerHubEntity)
	REGISTER_ENTITY(VuDialogInUpgradeCarEntity)
	REGISTER_ENTITY(VuDialogInLoungeEntity)
	REGISTER_ENTITY(VuDialogInShowRoomEntity)
	VuEntityFactory::IF()->finalizeRegistration();
}

//*****************************************************************************
void VuApplicationRegistry::addAssetTypes()
{
}

//*****************************************************************************
void VuApplicationRegistry::addPfxTypes()
{
	if ( VuPfx::IF() == VUNULL )
		return;

	{
		REGISTER_PFX_PATTERN(VuPfxCarThrustGeomPattern, "Car Thrust Geom");
		REGISTER_PFX_PROCESS(VuPfxEmitGeomFountain, "Emit Fountain", "VuPfxCarThrustGeomPattern");
	}

	REGISTER_PFX_PROCESS(VuPfxEmitCarChassis, "Emit Car Chassis", "VuPfxQuadPattern");
	REGISTER_PFX_PROCESS(VuPfxEmitRagdollSplashQuadFountain, "Emit Ragdoll Splash Quad Fountain", "VuPfxQuadPattern");
}

//*****************************************************************************
void VuApplicationRegistry::addInstigatorTypes()
{
	VuTriggerManager::addType("<none>"); // default
	VuTriggerManager::addType("Any Jet Ski");
	VuTriggerManager::addType("Local Racer");
	VuTriggerManager::addType("Any Racer");
	VuTriggerManager::addType("Camera");
}