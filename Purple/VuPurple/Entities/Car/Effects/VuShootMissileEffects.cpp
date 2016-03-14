//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Shoot missile effects
// 
//*****************************************************************************

#include "VuCarEffect.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Dynamic/VuDumbMissileEntity.h"
#include "VuPurple/Entities/Dynamic/VuHomingMissileEntity.h"
#include "VuPurple/Entities/Dynamic/VuFirstMissileEntity.h"
#include "VuPurple/Entities/Dynamic/VuBestLineEffectEntity.h"
#include "VuPurple/Entities/Dynamic/VuEarthStrikeEntity.h"
#include "VuPurple/Entities/Dynamic/VuLightningEntity.h"
#include "VuPurple/PowerUps/VuPowerUpUtil.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuEngine/Entities/VuEntityRepository.h"
#include "VuEngine/Math/VuRand.h"


//*****************************************************************************
// VuCarShootMissileBaseEffect
//*****************************************************************************

class VuCarShootMissileBaseEffect : public VuCarEffect
{
protected:
	void	shootMissile(const VuJsonContainer &missileData);
};

//*****************************************************************************
void VuCarShootMissileBaseEffect::shootMissile(const VuJsonContainer &missileData)
{
	VuMatrix transform;
	VuPowerUpUtil::calcMissileLaunchTransform(mpCar, transform);

	VuCarEntity *pTarget = VUNULL;
	float targetAcquisitionCone = missileData["TargetAcquisitionCone"].asFloat();
	if ( targetAcquisitionCone > 0.0f )
	{
		float cosTargetAcquisitionCone = VuCos(0.5f*VuDegreesToRadians(targetAcquisitionCone));
		pTarget = VuPowerUpUtil::findBestMissileTarget(mpCar, missileData["Range"].asFloat(), cosTargetAcquisitionCone);
	}

	if ( pTarget )
	{
		VuEntityRepository::IF()->addManagedEntity(new VuHomingMissileEntity(missileData, mpCar, pTarget, transform));
	}
	else
	{
		VuEntityRepository::IF()->addManagedEntity(new VuDumbMissileEntity(missileData, mpCar, transform));
	}
}


//*****************************************************************************
// VuCarShootMissileEffect
//*****************************************************************************

class VuCarShootMissileEffect : public VuCarShootMissileBaseEffect
{
public:
	virtual void onApply(const VuJsonContainer &data)
	{
		shootMissile(data["MissileData"]);
	}
};

IMPLEMENT_CAR_EFFECT(VuCarShootMissileEffect)


//*****************************************************************************
// VuCarShootMissilesEffect
//*****************************************************************************

class VuCarShootMissilesEffect : public VuCarShootMissileBaseEffect
{
public:
	VuCarShootMissilesEffect() : mItemsRemaining(0) {}

	virtual void	onApply(const VuJsonContainer &data);
	virtual void	onTick(float fdt);

	int				mItemsRemaining;

	float			mPlantRate;
};

IMPLEMENT_CAR_EFFECT(VuCarShootMissilesEffect)


//*****************************************************************************
void VuCarShootMissilesEffect::onApply(const VuJsonContainer &data)
{
	int maxCount = data["MaxCount"].asInt();

	if ( mReapplyType == "Stack" )
		mItemsRemaining += maxCount;
	else
		mItemsRemaining = maxCount;

	mPlantRate = mItemsRemaining/mTimeRemaining;
}

//*****************************************************************************
void VuCarShootMissilesEffect::onTick(float fdt)
{
	int optimalItemsRemaining = VuMax(VuTruncate((mTimeRemaining - fdt)*mPlantRate), 0);

	// don't plant more than 1 per frame
	if ( mItemsRemaining > optimalItemsRemaining )
	{
		mItemsRemaining--;
		shootMissile((*mpData)["MissileData"]);
	}
}


//*****************************************************************************
// VuCarShootFirstMissileEffect
//*****************************************************************************

class VuCarShootFirstMissileEffect : public VuCarEffect
{
public:
	virtual void onApply(const VuJsonContainer &data)
	{
		VuEntityRepository::IF()->addManagedEntity(new VuFirstMissileEntity(data["MissileData"], mpCar));
	}
};

IMPLEMENT_CAR_EFFECT(VuCarShootFirstMissileEffect)


//*****************************************************************************
// VuCarShootBestLineMissileEffect
//*****************************************************************************

class VuCarShootBestLineMissileEffect : public VuCarEffect
{
public:
	virtual void onApply(const VuJsonContainer &data)
	{
		VuEntityRepository::IF()->addManagedEntity(new VuBestLineEffectEntity(data["MissileData"], mpCar));
	}
};

IMPLEMENT_CAR_EFFECT(VuCarShootBestLineMissileEffect)


//*****************************************************************************
// VuCarShootEarthStrikeEffect
//*****************************************************************************

class VuCarShootEarthStrikeEffect : public VuCarEffect
{
public:
	virtual void onApply(const VuJsonContainer &data)
	{
		VuMatrix transform;
		VuPowerUpUtil::calcMissileLaunchTransform(mpCar, transform);

		VuEntityRepository::IF()->addManagedEntity(new VuEarthStrikeEntity(data["MissileData"], mpCar, transform));
	}
};

IMPLEMENT_CAR_EFFECT(VuCarShootEarthStrikeEffect)


//*****************************************************************************
// VuCarShootLightningEffect
//*****************************************************************************

class VuCarShootLightningEffect : public VuCarEffect
{
public:
	virtual void onApply(const VuJsonContainer &data)
	{
		VuEntityRepository::IF()->addManagedEntity(new VuLightningEntity(data["LightningData"], mpCar));
	}
};

IMPLEMENT_CAR_EFFECT(VuCarShootLightningEffect)


//*****************************************************************************
// VuCarScatterShotEffect
//*****************************************************************************

class VuCarScatterShotEffect : public VuCarEffect
{
public:
	virtual void	onApply(const VuJsonContainer &data);
};

IMPLEMENT_CAR_EFFECT(VuCarScatterShotEffect)

//*****************************************************************************
void VuCarScatterShotEffect::onApply(const VuJsonContainer &data)
{
	int missileCount = data["MissileData"].size();

	VuMatrix launchTransform;
	VuPowerUpUtil::calcMissileLaunchTransform(mpCar, launchTransform);

	for ( int i = 0; i < missileCount; i++ )
	{
		const VuJsonContainer &missileData = data["MissileData"][i];

		float angularOffset = VuDegreesToRadians(missileData["AngularOffset"].asFloat());
		VuMatrix transform = launchTransform;
		transform.rotateZLocal(angularOffset);

		VuEntityRepository::IF()->addManagedEntity(new VuDumbMissileEntity(missileData, mpCar, transform));
	}
}


//*****************************************************************************
// VuCarShootHydraEffect
//*****************************************************************************

class VuCarShootHydraEffect : public VuCarEffect
{
public:
	virtual void	onApply(const VuJsonContainer &data);
};

IMPLEMENT_CAR_EFFECT(VuCarShootHydraEffect)

//*****************************************************************************
void VuCarShootHydraEffect::onApply(const VuJsonContainer &data)
{
	int missileCount = data["MissileCount"].asInt();
	float yawMin = VuDegreesToRadians(data["YawRange"][0].asFloat());
	float yawMax = VuDegreesToRadians(data["YawRange"][1].asFloat());
	float pitchMin = VuDegreesToRadians(data["PitchRange"][0].asFloat());
	float pitchMax = VuDegreesToRadians(data["PitchRange"][1].asFloat());

	// calculate launch transform
	VuMatrix launchTransform;
	VuPowerUpUtil::calcMissileLaunchTransform(mpCar, launchTransform);

	const VuJsonContainer &missileData = data["MissileData"];

	float range = missileData["Range"].asFloat();
	float cosTargetAcquisitionCone = VuCos(0.5f*VuDegreesToRadians(missileData["TargetAcquisitionCone"].asFloat()));

	// gather targets
	std::vector<VuCarEntity *> targets;
	VuVector3 launcherPos = mpCar->getModelMatrix().transform(mpCar->getCollisionAabb().getCenter());
	for ( int i = 0; i < VuCarManager::IF()->getCarCount(); i++ )
	{
		VuCarEntity *pOtherCar = VuCarManager::IF()->getCar(i);
		if ( pOtherCar != mpCar )
		{
			// check distance
			VuVector3 otherPos = pOtherCar->getModelMatrix().transform(pOtherCar->getCollisionAabb().getCenter());
			VuVector3 delta = otherPos - launcherPos;
			float dist = delta.normalize();
			if ( dist < range )
			{
				// check target acquisition cone
				if ( VuDot(delta, mpCar->getModelMatrix().getAxisY()) > cosTargetAcquisitionCone )
				{
					targets.push_back(pOtherCar);
				}
			}
		}
	}

	// it's possible to target nothing
	targets.push_back(VUNULL);

	// launch missiles
	for ( int i = 0; i < missileCount; i++ )
	{
		int targetIndex = VuRand::global().range(0, (int)targets.size());
		VuCarEntity *pTarget = targets[targetIndex];

		VuMatrix transform = launchTransform;
		transform.rotateZLocal(VuRand::global().range(yawMin, yawMax));
		transform.rotateXLocal(VuRand::global().range(pitchMin, pitchMax));

		if ( pTarget )
			VuEntityRepository::IF()->addManagedEntity(new VuHomingMissileEntity(missileData, mpCar, pTarget, transform));
		else
			VuEntityRepository::IF()->addManagedEntity(new VuDumbMissileEntity(missileData, mpCar, transform));
	}
}


