//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Lounge Driver entity
// 
//*****************************************************************************

#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Entities/VuEntityFactory.h"
#include "VuEngine/Components/3dDraw/Vu3dDrawComponent.h"
#include "VuEngine/Components/3dLayout/Vu3dLayoutComponent.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Components/Transform/VuTransformComponent.h"
#include "VuEngine/Components/Attach/VuAnimatedAttachComponent.h"
#include "VuEngine/Properties/VuEnumProperty.h"
#include "VuEngine/Assets/VuAnimationAsset.h"
#include "VuEngine/Animation/VuAnimationControl.h"
#include "VuEngine/Animation/VuSkeleton.h"
#include "VuEngine/Animation/VuAnimation.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Managers/VuProfileManager.h"
#include "VuEngine/Gfx/Model/VuAnimatedModelInstance.h"
#include "VuEngine/Gfx/Shadow/VuBlobShadow.h"
#include "VuEngine/Gfx/GfxScene/VuGfxAnimatedScene.h"
#include "VuEngine/Animation/VuAnimatedSkeleton.h"
#include "VuEngine/Math/VuMathUtil.h"


//*****************************************************************************
//
// Driver Slot Entity
//
//*****************************************************************************

class VuUiDriverSlotEntity : public VuEntity, VuAnimationEventIF
{
	DECLARE_RTTI

public:
	VuUiDriverSlotEntity();
	~VuUiDriverSlotEntity();

	virtual void	onGameInitialize();
	virtual void	onGameRelease();

protected:
	// scripting
	VuRetVal		Show(const VuParams &params) { mp3dDrawComponent->show(); return VuRetVal(); }
	VuRetVal		Hide(const VuParams &params) { mp3dDrawComponent->hide(); return VuRetVal(); }
	VuRetVal		PlayAnimation(const VuParams &params);
	VuRetVal		AddAdditiveAnimation(const VuParams &params);
	VuRetVal		RemoveAdditiveAnimation(const VuParams &params);
	VuRetVal		FadeBlobShadowIn(const VuParams &params) { mBlobShadowFadeTarget = 1.0f; return VuRetVal(); }
	VuRetVal		FadeBlobShadowOut(const VuParams &params) { mBlobShadowFadeTarget = 0.0f; return VuRetVal(); }

	// VuAnimationEventIF
	virtual void	onAnimationEvent(const std::string &type, const VuJsonContainer &params);

	void			updateDriver();

	virtual void	tickDecision(float fdt);
	void			tickAnim(float fdt);
	void			tickBuild(float fdt);

	void			draw(const VuGfxDrawParams &params);
	void			drawShadow(const VuGfxDrawShadowParams &params);

	virtual void	updateCurrentConfig() {};

	// components
	Vu3dDrawComponent		*mp3dDrawComponent;
	Vu3dLayoutComponent		*mp3dLayoutComponent;
	VuScriptComponent		*mpScriptComponent;
	VuAttachComponent		*mpAttachComponent;

	VuAnimatedModelInstance	mModelInstance;
	VuAnimatedSkeleton		*mpAnimatedSkeleton;
	VuBlobShadow			*mpBlobShadow;
	float					mBlobShadowAlpha;
	float					mBlobShadowRadius;
	bool					mBlobShadowDirty;
	VuMatrix				mTransform;
	float					mBlobShadowFadeTarget;
	float					mBlobShadowFadeValue;
	float					mBlobShadowFadeRate;

	bool				mBlending;
	float				mBlendRate;

	struct Config
	{
		std::string		mDriver;
	};
	Config		mCurrentConfig;
	Config		mDisplayConfig;

	struct AdditiveAnimation
	{
		AdditiveAnimation() : mBlendRate(0) {}

		float				mBlendRate;
		VuAnimationControl	*mpAnimControl;
	};
	typedef std::map<VUUINT32, AdditiveAnimation> AdditiveAnimations;
	AdditiveAnimations	mAdditiveAnimations;
};

IMPLEMENT_RTTI(VuUiDriverSlotEntity, VuEntity);

//*****************************************************************************
VuUiDriverSlotEntity::VuUiDriverSlotEntity():
	mpAnimatedSkeleton(VUNULL),
	mpBlobShadow(VUNULL),
	mBlobShadowDirty(true),
	mTransform(VuMatrix::identity()),
	mBlending(false),
	mBlendRate(0.0f),
	mBlobShadowAlpha(0.75f),
	mBlobShadowRadius(1.0f),
	mBlobShadowFadeTarget(1.0f),
	mBlobShadowFadeValue(1.0f),
	mBlobShadowFadeRate(1.0f)
{
	// components
	addComponent(mp3dDrawComponent = new Vu3dDrawComponent(this));
	addComponent(mp3dLayoutComponent = new Vu3dLayoutComponent(this));
	addComponent(mpScriptComponent = new VuScriptComponent(this, 200));
	addComponent(mpAttachComponent = new VuAnimatedAttachComponent(this, &mModelInstance));

	mp3dDrawComponent->setDrawMethod(this, &VuUiDriverSlotEntity::draw);
	mp3dDrawComponent->setDrawShadowMethod(this, &VuUiDriverSlotEntity::drawShadow);

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuUiDriverSlotEntity, Show);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuUiDriverSlotEntity, Hide);
	ADD_SCRIPT_INPUT(mpScriptComponent, VuUiDriverSlotEntity, PlayAnimation, VuRetVal::Void, VuParamDecl(5, VuParams::Asset, VuParams::Float, VuParams::Float, VuParams::Float, VuParams::Bool));
	ADD_SCRIPT_INPUT(mpScriptComponent, VuUiDriverSlotEntity, AddAdditiveAnimation, VuRetVal::Void, VuParamDecl(5, VuParams::UnsignedInt, VuParams::Asset, VuParams::Float, VuParams::Float, VuParams::Bool));
	ADD_SCRIPT_INPUT(mpScriptComponent, VuUiDriverSlotEntity, RemoveAdditiveAnimation, VuRetVal::Void, VuParamDecl(2, VuParams::UnsignedInt, VuParams::Float));
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuUiDriverSlotEntity, FadeBlobShadowIn);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuUiDriverSlotEntity, FadeBlobShadowOut);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnAnimDone);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnAnimLoop);

	// blob shadow
	{
		mpBlobShadow = new VuBlobShadow;

		const VuJsonContainer &blobShadowData = VuGameUtil::IF()->constantDB()["Driver"]["BlobShadow"];
		mpBlobShadow->setTexture(blobShadowData["Texture"].asString());
		blobShadowData["Alpha"].getValue(mBlobShadowAlpha);
		blobShadowData["Radius"].getValue(mBlobShadowRadius);
		blobShadowData["FadeRate"].getValue(mBlobShadowFadeRate);
	}
}

//*****************************************************************************
VuUiDriverSlotEntity::~VuUiDriverSlotEntity()
{
	delete mpBlobShadow;
}

//*****************************************************************************
void VuUiDriverSlotEntity::onGameInitialize()
{
	VuTickManager::IF()->registerHandler(this, &VuUiDriverSlotEntity::tickDecision, "Decision");
	VuTickManager::IF()->registerHandler(this, &VuUiDriverSlotEntity::tickAnim, "Anim");
	VuTickManager::IF()->registerHandler(this, &VuUiDriverSlotEntity::tickBuild, "Build");

	updateCurrentConfig();
	updateDriver();

	mp3dDrawComponent->show();
}

//*****************************************************************************
void VuUiDriverSlotEntity::onGameRelease()
{
	VuTickManager::IF()->unregisterHandlers(this);

	mp3dDrawComponent->hide();

	VU_SAFE_RELEASE(mpAnimatedSkeleton);

	for ( auto &entry : mAdditiveAnimations )
		entry.second.mpAnimControl->removeRef();
	mAdditiveAnimations.clear();
}

//*****************************************************************************
VuRetVal VuUiDriverSlotEntity::PlayAnimation(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	VuAsset *pAsset = accessor.getAsset();
	float startTime = accessor.getFloat();
	float blendTime = accessor.getFloat();
	float timeFactor = accessor.getFloat();
	bool looping = accessor.getBool();

	VUASSERT(pAsset->isDerivedFrom(VuAnimationAsset::msRTTI), "VuAnimatedPropEntity::PlayAnimation() expecting VuAnimationAsset");
	VuAnimationAsset *pAnimationAsset = static_cast<VuAnimationAsset *>(pAsset);

	if ( mpAnimatedSkeleton )
	{
		VuAnimation *pAnimation = pAnimationAsset->getAnimation();
		VUASSERT(pAnimation->getBoneCount() == mpAnimatedSkeleton->getSkeleton()->mBoneCount, "VuUiDriverSlotEntity::PlayAnimation() bone count mismatch");

		VUASSERT(pAnimation->isAdditive() == false, "VuUiDriverSlotEntity::PlayAnimation() expected non-additive animation");
		if ( pAnimation->isAdditive() == false )
		{
			VuAnimationControl *pAnimationControl = new VuAnimationControl(pAnimation);
			pAnimationControl->setLocalTime(startTime);
			pAnimationControl->setTimeFactor(timeFactor);
			pAnimationControl->setLooping(looping);

			// start blending (if desired)
			if ( blendTime > 0 )
			{
				mBlending = true;
				mBlendRate = 1.0f/blendTime;
				pAnimationControl->setWeight(0.0f);
			}
			else
			{
				mpAnimatedSkeleton->clearBlendAnimationControls();
			}

			mpAnimatedSkeleton->addAnimationControl(pAnimationControl);

			pAnimationControl->setEventIF(this);

			pAnimationControl->removeRef();
		}
	}

	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuUiDriverSlotEntity::AddAdditiveAnimation(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	VUUINT32 id = accessor.getUnsignedInt();
	VuAsset *pAsset = accessor.getAsset();
	float blendInTime = accessor.getFloat();
	float timeFactor = accessor.getFloat();
	bool looping = accessor.getBool();

	VUASSERT(pAsset->isDerivedFrom(VuAnimationAsset::msRTTI), "VuAnimatedPropEntity::AddAdditiveAnimation() expecting VuAnimationAsset");
	VuAnimationAsset *pAnimationAsset = static_cast<VuAnimationAsset *>(pAsset);

	AdditiveAnimations::iterator iter = mAdditiveAnimations.find(id);
	if ( iter == mAdditiveAnimations.end() )
	{
		if ( mpAnimatedSkeleton )
		{
			VuAnimation *pAnimation = pAnimationAsset->getAnimation();
			VUASSERT(pAnimation->getBoneCount() == mpAnimatedSkeleton->getSkeleton()->mBoneCount, "VuUiDriverSlotEntity::AddAdditiveAnimation() bone count mismatch");

			VUASSERT(pAnimation->isAdditive() == true, "VuUiDriverSlotEntity::AddAdditiveAnimation() expected additive animation");
			if ( pAnimation->isAdditive() == true )
			{
				AdditiveAnimation &anim = mAdditiveAnimations[id];

				anim.mpAnimControl = new VuAnimationControl(pAnimation);

				anim.mpAnimControl->setTimeFactor(timeFactor);
				anim.mpAnimControl->setLooping(looping);

				if ( blendInTime > 0 )
				{
					anim.mBlendRate = 1.0f/blendInTime;
					anim.mpAnimControl->setWeight(0.0f);
				}

				mpAnimatedSkeleton->addAnimationControl(anim.mpAnimControl);
			}
		}
	}

	return VuRetVal();
}

//*****************************************************************************
VuRetVal VuUiDriverSlotEntity::RemoveAdditiveAnimation(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	VUUINT32 id = accessor.getUnsignedInt();
	float blendOutTime = accessor.getFloat();

	AdditiveAnimations::iterator iter = mAdditiveAnimations.find(id);
	if ( iter != mAdditiveAnimations.end() )
	{
		if ( blendOutTime > 0 )
		{
			iter->second.mBlendRate = -1.0f/blendOutTime;
		}
		else
		{
			mpAnimatedSkeleton->removeAnimationControl(iter->second.mpAnimControl);
			iter->second.mpAnimControl->removeRef();
			mAdditiveAnimations.erase(iter);
		}
	}

	return VuRetVal();
}

//*****************************************************************************
void VuUiDriverSlotEntity::updateDriver()
{
	if ( mDisplayConfig.mDriver != mCurrentConfig.mDriver )
	{
		mDisplayConfig.mDriver = mCurrentConfig.mDriver;

		const VuJsonContainer &driverData = VuGameUtil::IF()->driverDB()[mDisplayConfig.mDriver];

		const std::string &modelAsset = driverData["Model Asset"].asString();
		mModelInstance.setModelAsset(modelAsset);

		if ( (mpAnimatedSkeleton == VUNULL) && mModelInstance.getSkeleton() )
			mpAnimatedSkeleton = new VuAnimatedSkeleton(mModelInstance.getSkeleton());

		float offsetZ = driverData["UI Z Offset"].asFloat();
		mTransform = mpTransformComponent->getWorldTransform();
		mTransform.translateLocal(VuVector3(0.0f,0.0f,offsetZ));
	}
}

//*****************************************************************************
void VuUiDriverSlotEntity::tickDecision(float fdt)
{
	updateCurrentConfig();
	updateDriver();

	if ( mBlobShadowDirty && mpBlobShadow && mModelInstance.getGfxAnimatedScene() )
	{
		VuAabb aabb(VuVector3(-mBlobShadowRadius, -mBlobShadowRadius, 0.0f), VuVector3(mBlobShadowRadius, mBlobShadowRadius, 2.0f*mBlobShadowRadius));
		mpBlobShadow->calculate(mTransform, aabb);
		mBlobShadowDirty = false;
	}

	mBlobShadowFadeValue = VuMathUtil::chase(mBlobShadowFadeValue, mBlobShadowFadeTarget, mBlobShadowFadeRate*fdt);
}

//*****************************************************************************
void VuUiDriverSlotEntity::tickAnim(float fdt)
{
	// update animation
	if ( mpAnimatedSkeleton )
	{
		// handle blending
		if ( mBlending )
		{
			int controlCount = mpAnimatedSkeleton->getBlendAnimationControlCount();
			float blendDelta = mBlendRate*fdt;
			for ( int i = 0; i < controlCount - 1; i++ )
			{
				VuAnimationControl *pAnimationControl = mpAnimatedSkeleton->getBlendAnimationControl(i);
				float weight = pAnimationControl->getWeight();
				weight = VuMax(weight - blendDelta, 0.0f);
				pAnimationControl->setWeight(weight);
			}
			if ( controlCount > 0 )
			{
				VuAnimationControl *pAnimationControl = mpAnimatedSkeleton->getBlendAnimationControl(controlCount - 1);
				float weight = pAnimationControl->getWeight();
				weight = VuMin(weight + blendDelta, 1.0f);
				pAnimationControl->setWeight(weight);
				if ( weight >= 1.0f )
				{
					mBlending = false;
					mBlendRate = 0.0f;
				}
			}
		}

		// remove old anims that have reached a weight of 0
		for ( int i = mpAnimatedSkeleton->getBlendAnimationControlCount() - 2; i >= 0; i-- )
			if ( mpAnimatedSkeleton->getBlendAnimationControl(i)->getWeight() <= 0.0f )
				mpAnimatedSkeleton->removeAnimationControl(mpAnimatedSkeleton->getBlendAnimationControl(i));

		// additive animations
		for (auto iter = mAdditiveAnimations.begin(); iter != mAdditiveAnimations.end();)
		{
			float weight = iter->second.mpAnimControl->getWeight();
			weight = VuClamp(weight + iter->second.mBlendRate*fdt, 0.0f, 1.0f);
			iter->second.mpAnimControl->setWeight(weight);

			if (iter->second.mBlendRate < 0.0f && weight < FLT_EPSILON)
			{
				// Adjusted for compilation under NDK/Clang
				// Postincrement of the iterator in the parameter list to erase() guarantees
				// that the value of iter is pushed on the stack for the call to erase() and
				// then the increment to the iterator happens BEFORE the function call, so
				// the iterator will still be valid
				mpAnimatedSkeleton->removeAnimationControl(iter->second.mpAnimControl);
				iter->second.mpAnimControl->removeRef();
				mAdditiveAnimations.erase(iter++);
			}
			else
			{
				++iter;
			}
		}

		mpAnimatedSkeleton->advance(fdt);
		mpAnimatedSkeleton->build();

		mModelInstance.setPose(mpAnimatedSkeleton);
		mModelInstance.finalizePose();
	}

	mpAttachComponent->update(mTransform);
}

//*****************************************************************************
void VuUiDriverSlotEntity::onAnimationEvent(const std::string &type, const VuJsonContainer &params)
{
	if ( type == "AnimDone" )
	{
		mpScriptComponent->getPlug("OnAnimDone")->execute();
	}
	else if ( type == "AnimLoop" )
	{
		mpScriptComponent->getPlug("OnAnimLoop")->execute();
	}
}

//*****************************************************************************
void VuUiDriverSlotEntity::tickBuild(float fdt)
{
	// update visibility
	VuAabb aabb(mModelInstance.getLocalAabb(), mTransform);
	mp3dDrawComponent->updateVisibility(aabb);
}

//*****************************************************************************
void VuUiDriverSlotEntity::draw(const VuGfxDrawParams &params)
{
	mModelInstance.enableTranslucentDepth(true);
	mModelInstance.draw(mTransform, params);

	if ( mModelInstance.getColor().mA == 255 )
	{
		if ( mpBlobShadow && mModelInstance.getGfxAnimatedScene() )
		{
			mpBlobShadow->params().mAlpha = mBlobShadowAlpha*mBlobShadowFadeValue;
			mpBlobShadow->draw(params);
		}
	}
}

//*****************************************************************************
void VuUiDriverSlotEntity::drawShadow(const VuGfxDrawShadowParams &params)
{
	mModelInstance.drawShadow(mTransform, params);
}


//*****************************************************************************
//
// Lounge Driver Entity
//
//*****************************************************************************

class VuLoungeDriverEntity : public VuUiDriverSlotEntity
{
	DECLARE_RTTI

public:
	VuLoungeDriverEntity();
};

IMPLEMENT_RTTI(VuLoungeDriverEntity, VuUiDriverSlotEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuLoungeDriverEntity);


//*****************************************************************************
VuLoungeDriverEntity::VuLoungeDriverEntity()
{
	// properties
	addProperty(new VuJsonStringArrayEnumProperty("Driver", mCurrentConfig.mDriver, VuGameUtil::IF()->constantDB()["Names"]["Drivers"]));
}


//*****************************************************************************
//
// Garage Driver Entity
//
//*****************************************************************************

class VuGarageDriverEntity : public VuUiDriverSlotEntity
{
	DECLARE_RTTI

protected:
	virtual void updateCurrentConfig()
	{
		mCurrentConfig.mDriver = VuGameManager::IF()->getCurDriverName();
	}
};

IMPLEMENT_RTTI(VuGarageDriverEntity, VuUiDriverSlotEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuGarageDriverEntity);


//*****************************************************************************
//
// Podium Driver Entity
//
//*****************************************************************************

class VuPodiumDriverEntity : public VuUiDriverSlotEntity
{
	DECLARE_RTTI

public:
	VuPodiumDriverEntity();

	virtual void	onGameInitialize();

protected:
	// properties
	int			mPlace;
};

IMPLEMENT_RTTI(VuPodiumDriverEntity, VuUiDriverSlotEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuPodiumDriverEntity);


//*****************************************************************************
VuPodiumDriverEntity::VuPodiumDriverEntity():
	mPlace(1)
{
	// properties
	addProperty(new VuIntProperty("Place", mPlace));
}

//*****************************************************************************
void VuPodiumDriverEntity::onGameInitialize()
{
	const VuJsonContainer &champTable = VuGameUtil::IF()->dataRead()["ChampTable"];

	for ( int i = 0; i < champTable.size(); i++ )
		if ( champTable[i]["Rank"].asInt() == mPlace )
			mCurrentConfig.mDriver = champTable[i]["Driver"].asString();

	VuUiDriverSlotEntity::onGameInitialize();
}


//*****************************************************************************
//
// Daily Challenge Driver Entity
//
//*****************************************************************************

class VuDailyChallengeDriverEntity : public VuUiDriverSlotEntity
{
	DECLARE_RTTI

protected:
	virtual void updateCurrentConfig()
	{
		const VuJsonContainer &challengeData = VuProfileManager::IF()->dataRead()["DailyChallenge"];
		mCurrentConfig.mDriver = challengeData["Driver"].asString();
	}
};

IMPLEMENT_RTTI(VuDailyChallengeDriverEntity, VuUiDriverSlotEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuDailyChallengeDriverEntity);


//*****************************************************************************
//
// CarChampPodium Driver Entity
//
//*****************************************************************************

class VuCarChampPodiumDriverEntity : public VuUiDriverSlotEntity
{
	DECLARE_RTTI

public:
	VuCarChampPodiumDriverEntity();

	virtual void	onGameInitialize();

protected:
	// properties
	int			mPlace;
};

IMPLEMENT_RTTI(VuCarChampPodiumDriverEntity, VuUiDriverSlotEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuCarChampPodiumDriverEntity);


//*****************************************************************************
VuCarChampPodiumDriverEntity::VuCarChampPodiumDriverEntity():
	mPlace(1)
{
	// properties
	addProperty(new VuIntProperty("Place", mPlace));
}

//*****************************************************************************
void VuCarChampPodiumDriverEntity::onGameInitialize()
{
	const std::string &carName = VuGameUtil::IF()->dataRead()["CarChamp"]["Car"].asString();

	VuGameUtil::CarChampTable table;
	VuGameUtil::IF()->createCarChampTable(carName, table);

	int index = mPlace - 1;
	if ( index >= 0 && index < (int)table.size() )
	{
		mCurrentConfig.mDriver = table[index].mDriver;
	}

	VuUiDriverSlotEntity::onGameInitialize();
}
