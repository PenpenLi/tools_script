//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Ghost Car Playback entity
// 
//*****************************************************************************

#include "VuPurple/Entities/Car/VuGhostCarEntity.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Properties/VuAssetProperty.h"
#include "VuEngine/Assets/VuGenericAsset.h"
#include "VuEngine/Managers/VuTickManager.h"


// static variables
static int sNameCounter = 0;

class VuGhostCarPlaybackEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuGhostCarPlaybackEntity();

	void				onGameInitialize();
	void				onGameRelease();

private:
	VuRetVal			Start(const VuParams &params = VuParams());
	VuRetVal			Stop(const VuParams &params = VuParams());

	void				tickDecision(float fdt);

	// components
	VuScriptComponent	*mpScriptComponent;

	// properties
	std::string			mRecordingName;
	bool				mActiveAtStart;
	bool				mLooping;

	VuGhostCarEntity	*mpGhostCar;
};


IMPLEMENT_RTTI(VuGhostCarPlaybackEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuGhostCarPlaybackEntity);


//*****************************************************************************
VuGhostCarPlaybackEntity::VuGhostCarPlaybackEntity():
	mActiveAtStart(false),
	mLooping(false),
	mpGhostCar(VUNULL)
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	// properties
	addProperty(new VuAssetNameProperty(VuGenericAsset::msRTTI.mstrType, "Recording", mRecordingName));
	addProperty(new VuBoolProperty("Active At Start", mActiveAtStart));
	addProperty(new VuBoolProperty("Looping", mLooping));

	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuGhostCarPlaybackEntity, Start);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuGhostCarPlaybackEntity, Stop);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnFinished);
}

//*****************************************************************************
void VuGhostCarPlaybackEntity::onGameInitialize()
{
	VuGenericAsset *pRecording = VuAssetFactory::IF()->createAsset<VuGenericAsset>(mRecordingName, VuAssetFactory::OPTIONAL_ASSET);
	if ( pRecording )
	{
		char name[32];
		VU_SPRINTF(name, sizeof(name), "GhostCar%d", sNameCounter++);

		mpGhostCar = new VuGhostCarEntity(pRecording->data());
		mpGhostCar->setShortName(name);
		mpGhostCar->load(VuJsonContainer::null);
		mpGhostCar->postLoad();

		VuAssetFactory::IF()->releaseAsset(pRecording);

		VuTickManager::IF()->registerHandler(this, &VuGhostCarPlaybackEntity::tickDecision, "Decision");

		if ( mActiveAtStart )
			Start();
	}
}

//*****************************************************************************
void VuGhostCarPlaybackEntity::onGameRelease()
{
	if ( mpGhostCar )
	{
		mpGhostCar->gameRelease();
		mpGhostCar->removeRef();
		mpGhostCar = VUNULL;

		VuTickManager::IF()->unregisterHandler(this, "Decision");
	}
}

//*****************************************************************************
VuRetVal VuGhostCarPlaybackEntity::Start(const VuParams &params)
{
	if ( mpGhostCar && !mpGhostCar->getIsPlaybackActive() )
	{
		mpGhostCar->gameInitialize();
		mpGhostCar->startPlayback();
	}

	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuGhostCarPlaybackEntity::Stop(const VuParams &params)
{
	if ( mpGhostCar && mpGhostCar->getIsPlaybackActive() )
	{
		mpGhostCar->stopPlayback();
		mpGhostCar->gameRelease();
	}

	return VuRetVal();
}

//*****************************************************************************
void VuGhostCarPlaybackEntity::tickDecision(float fdt)
{
	if ( mpGhostCar && mpGhostCar->getIsPlaybackActive() )
	{
		if ( mpGhostCar->getPlaybackTime() >= mpGhostCar->getPlaybackEndTime() )
		{
			Stop();

			mpScriptComponent->getPlug("OnFinished")->execute();

			if ( mLooping )
				Start();
		}
	}
}
