//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Showroom Car entity
// 
//*****************************************************************************

#include "VuPurple/Entities/Car/VuUiCar.h"
#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Entities/VuEntityFactory.h"
#include "VuEngine/Components/Transform/VuTransformComponent.h"
#include "VuEngine/Components/3dLayout/Vu3dLayoutComponent.h"
#include "VuEngine/Components/3dDraw/Vu3dDrawComponent.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Components/Attach/VuOffsetAttachComponent.h"
#include "VuEngine/Properties/VuEnumProperty.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Managers/VuProfileManager.h"
#include "VuEngine/Math/VuMathUtil.h"


//*****************************************************************************
//
// Car Slot Entity
//
//*****************************************************************************

class VuUiCarSlotEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuUiCarSlotEntity();

	virtual void	onGameInitialize();
	virtual void	onGameRelease();

protected:
	void			updateCar();

	void			draw(const VuGfxDrawParams &params);
	void			drawShadow(const VuGfxDrawShadowParams &params);

	virtual void	tickDecision(float fdt);
	virtual void	updateCurrentConfig() {}

	// components
	Vu3dLayoutComponent		*mp3dLayoutComponent;
	Vu3dDrawComponent		*mp3dDrawComponent;
	VuScriptComponent		*mpScriptComponent;
	VuAttachComponent		*mpAttachComponent;

	VuUiCar			mUiCar;

	struct Config
	{
		Config() : mStage(0) {}
		std::string		mCar;
		int				mStage;
		std::string		mDecal;
		std::string		mDecalColor;
		std::string		mPaintColor;
	};
	Config		mCurrentConfig;
};

IMPLEMENT_RTTI(VuUiCarSlotEntity, VuEntity);


//*****************************************************************************
VuUiCarSlotEntity::VuUiCarSlotEntity():
	mUiCar(true)
{
	// components
	addComponent(mp3dLayoutComponent = new Vu3dLayoutComponent(this));
	addComponent(mp3dDrawComponent = new Vu3dDrawComponent(this));
	addComponent(mpScriptComponent = new VuScriptComponent(this, 200));
	addComponent(mpAttachComponent = new VuOffsetAttachComponent(this));

	mp3dDrawComponent->setDrawMethod(this, &VuUiCarSlotEntity::draw);
	mp3dDrawComponent->setDrawShadowMethod(this, &VuUiCarSlotEntity::drawShadow);
}

//*****************************************************************************
void VuUiCarSlotEntity::onGameInitialize()
{
	VuTickManager::IF()->registerHandler(this, &VuUiCarSlotEntity::tickDecision, "Decision");

	updateCurrentConfig();
	updateCar();

	mp3dDrawComponent->show();
}

//*****************************************************************************
void VuUiCarSlotEntity::onGameRelease()
{
	mp3dDrawComponent->hide();

	VuTickManager::IF()->unregisterHandlers(this);
}

//*****************************************************************************
void VuUiCarSlotEntity::updateCar()
{
	mUiCar.setCar(mCurrentConfig.mCar, mCurrentConfig.mStage);
	mUiCar.setSkin(mCurrentConfig.mDecal, mCurrentConfig.mPaintColor, mCurrentConfig.mDecalColor);
}

//*****************************************************************************
void VuUiCarSlotEntity::draw(const VuGfxDrawParams &params)
{
	mUiCar.draw(params);
}

//*****************************************************************************
void VuUiCarSlotEntity::drawShadow(const VuGfxDrawShadowParams &params)
{
	mUiCar.drawShadow(params);
}

//*****************************************************************************
void VuUiCarSlotEntity::tickDecision(float fdt)
{
	mpAttachComponent->update(mpTransformComponent->getWorldTransform());

	updateCurrentConfig();
	updateCar();

	mUiCar.setTransform(mpTransformComponent->getWorldTransform());
	mUiCar.tickDecision(fdt);

	mp3dDrawComponent->updateVisibility(mUiCar.getAabb(), mpTransformComponent->getWorldTransform());
}


//*****************************************************************************
//
// Showroom Car Entity
//
//*****************************************************************************

class VuShowroomCarEntity : public VuUiCarSlotEntity
{
	DECLARE_RTTI

public:
	VuShowroomCarEntity();

protected:
	virtual void	updateCurrentConfig();

	// properties
	std::string		mCar;
};

IMPLEMENT_RTTI(VuShowroomCarEntity, VuUiCarSlotEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuShowroomCarEntity);


//*****************************************************************************
VuShowroomCarEntity::VuShowroomCarEntity()
{
	// properties
	addProperty(new VuJsonStringArrayEnumProperty("Car", mCar, VuGameUtil::IF()->constantDB()["Names"]["Cars"]));
}

//*****************************************************************************
void VuShowroomCarEntity::updateCurrentConfig()
{
	mCurrentConfig.mCar = mCar;

	const VuGameManager::Car &car = VuGameManager::IF()->getCar(mCar);
	mCurrentConfig.mStage = car.mStage;
	mCurrentConfig.mDecal = car.mDecal;
	mCurrentConfig.mPaintColor = car.mPaintColor;
	mCurrentConfig.mDecalColor = car.mDecalColor;
}


//*****************************************************************************
//
// Garage Car Entity
//
//*****************************************************************************

class VuGarageCarEntity : public VuUiCarSlotEntity
{
	DECLARE_RTTI

public:
	VuGarageCarEntity();

	virtual void	onGameInitialize();

protected:
	// event handlers
	virtual void	OnPedestalControlBegin(const VuParams &params);
	virtual void	OnPedestalControlEnd(const VuParams &params);
	virtual void	OnPedestalControlUpdate(const VuParams &params);

	virtual void	tickDecision(float fdt);
	virtual void	updateCurrentConfig();

	// properties
	float			mRotLag;

	float			mTargetRot;
	float			mCurRot;
	float			mCurRotVel;
	VuMatrix		mOrigTransform;
};

IMPLEMENT_RTTI(VuGarageCarEntity, VuUiCarSlotEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuGarageCarEntity);


//*****************************************************************************
VuGarageCarEntity::VuGarageCarEntity():
	mRotLag(0.2f),
	mTargetRot(0),
	mCurRot(0),
	mCurRotVel(0)
{
	// properties
	addProperty(new VuFloatProperty("Rot Lag", mRotLag));

	// event handlers
	REG_EVENT_HANDLER(VuGarageCarEntity, OnPedestalControlBegin);
	REG_EVENT_HANDLER(VuGarageCarEntity, OnPedestalControlEnd);
	REG_EVENT_HANDLER(VuGarageCarEntity, OnPedestalControlUpdate);
}

//*****************************************************************************
void VuGarageCarEntity::onGameInitialize()
{
	VuUiCarSlotEntity::onGameInitialize();

	mOrigTransform = mpTransformComponent->getWorldTransform();
}

//*****************************************************************************
void VuGarageCarEntity::OnPedestalControlBegin(const VuParams &params)
{
}

//*****************************************************************************
void VuGarageCarEntity::OnPedestalControlEnd(const VuParams &params)
{
	mTargetRot = 0.0f;
}

//*****************************************************************************
void VuGarageCarEntity::OnPedestalControlUpdate(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	mTargetRot = accessor.getFloat();
}

//*****************************************************************************
void VuGarageCarEntity::tickDecision(float fdt)
{
	float delta = VuAngDiff(mCurRot, mTargetRot);
	mCurRot = mTargetRot - delta;
	mCurRot = VuMathUtil::smoothCD(mCurRot, mTargetRot, mCurRotVel, mRotLag, fdt);

	VuMatrix transform = mOrigTransform;
	transform.rotateZLocal(mCurRot);
	mpTransformComponent->setWorldTransform(transform);

	mUiCar.setBlobShadowDirty();

	VuUiCarSlotEntity::tickDecision(fdt);
}

//*****************************************************************************
void VuGarageCarEntity::updateCurrentConfig()
{
	const VuGameManager::Car &car = VuGameManager::IF()->getCurCar();

	mCurrentConfig.mCar = car.mName;
	mCurrentConfig.mStage = car.mStage;

	if ( VuGameUtil::IF()->getCarPaintMode() )
	{
		mCurrentConfig.mDecal = VuGameUtil::IF()->getCarPaintDecal();
		mCurrentConfig.mDecalColor = VuGameUtil::IF()->getCarPaintDecalColor();
		mCurrentConfig.mPaintColor = VuGameUtil::IF()->getCarPaintPaintColor();
	}
	else
	{
		mCurrentConfig.mDecal = car.mDecal;
		mCurrentConfig.mDecalColor = car.mDecalColor;
		mCurrentConfig.mPaintColor = car.mPaintColor;
	}
}


//*****************************************************************************
//
// Podium Car Entity
//
//*****************************************************************************

class VuPodiumCarEntity : public VuUiCarSlotEntity
{
	DECLARE_RTTI

public:
	VuPodiumCarEntity();

	virtual void	onGameInitialize();

protected:
	// properties
	int			mPlace;
};

IMPLEMENT_RTTI(VuPodiumCarEntity, VuUiCarSlotEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuPodiumCarEntity);


//*****************************************************************************
VuPodiumCarEntity::VuPodiumCarEntity():
	mPlace(1)
{
	// properties
	addProperty(new VuIntProperty("Place", mPlace));
}

//*****************************************************************************
void VuPodiumCarEntity::onGameInitialize()
{
	const VuJsonContainer &champTable = VuGameUtil::IF()->dataRead()["ChampTable"];

	for ( int i = 0; i < champTable.size(); i++ )
	{
		if ( champTable[i]["Rank"].asInt() == mPlace )
		{
			mCurrentConfig.mCar = champTable[i]["Car"].asString();
			mCurrentConfig.mStage = champTable[i]["Stage"].asInt();
			mCurrentConfig.mDecal = champTable[i]["Decal"].asString();
			mCurrentConfig.mPaintColor = champTable[i]["PaintColor"].asString();
			mCurrentConfig.mDecalColor = champTable[i]["DecalColor"].asString();
		}
	}

	VuUiCarSlotEntity::onGameInitialize();
}


//*****************************************************************************
//
// Daily Challenge Car Entity
//
//*****************************************************************************

class VuDailyChallengeCarEntity : public VuUiCarSlotEntity
{
	DECLARE_RTTI

protected:
	virtual void	updateCurrentConfig();
};

IMPLEMENT_RTTI(VuDailyChallengeCarEntity, VuUiCarSlotEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuDailyChallengeCarEntity);


//*****************************************************************************
void VuDailyChallengeCarEntity::updateCurrentConfig()
{
	const VuJsonContainer &challengeData = VuProfileManager::IF()->dataRead()["DailyChallenge"];

	mCurrentConfig.mCar = challengeData["Car"].asString();
	mCurrentConfig.mStage = challengeData["Stage"].asInt();
	mCurrentConfig.mDecal = challengeData["Decal"].asString();
	mCurrentConfig.mPaintColor = challengeData["PaintColor"].asString();
	mCurrentConfig.mDecalColor = challengeData["DecalColor"].asString();
}


//*****************************************************************************
//
// CarChampPodium Car Entity
//
//*****************************************************************************

class VuCarChampPodiumCarEntity : public VuUiCarSlotEntity
{
	DECLARE_RTTI

public:
	VuCarChampPodiumCarEntity();

	virtual void	onGameInitialize();

protected:
	// properties
	int			mPlace;
};

IMPLEMENT_RTTI(VuCarChampPodiumCarEntity, VuUiCarSlotEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuCarChampPodiumCarEntity);


//*****************************************************************************
VuCarChampPodiumCarEntity::VuCarChampPodiumCarEntity():
	mPlace(1)
{
	// properties
	addProperty(new VuIntProperty("Place", mPlace));
}

//*****************************************************************************
void VuCarChampPodiumCarEntity::onGameInitialize()
{
	const std::string &carName = VuGameUtil::IF()->dataRead()["CarChamp"]["Car"].asString();

	VuGameUtil::CarChampTable table;
	VuGameUtil::IF()->createCarChampTable(carName, table);

	int index = mPlace - 1;
	if ( index >= 0 && index < (int)table.size() )
	{
		mCurrentConfig.mCar = table[index].mCar;
		mCurrentConfig.mStage = table[index].mStage;
		mCurrentConfig.mDecal = table[index].mDecal;
		mCurrentConfig.mDecalColor = table[index].mDecalColor;
		mCurrentConfig.mPaintColor = table[index].mPaintColor;
		//mCurrentConfig.mIsGold = table[index].mIsGold;
	}

	VuUiCarSlotEntity::onGameInitialize();
}
