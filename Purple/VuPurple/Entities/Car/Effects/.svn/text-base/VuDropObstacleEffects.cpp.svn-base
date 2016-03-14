//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Drop obstacle car effects
// 
//*****************************************************************************

#include "VuCarEffect.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Dynamic/VuObstacleEntity.h"
#include "VuPurple/Entities/Dynamic/VuFakePowerUpEntity.h"
#include "VuPurple/Entities/Dynamic/VuSpringObstacleEntity.h"
#include "VuPurple/Entities/Dynamic/VuOilSlickEntity.h"
#include "VuEngine/Entities/VuEntityRepository.h"
#include "VuEngine/Dynamics/Util/VuDynamicsRayTest.h"
#include "VuEngine/Math/VuMathUtil.h"


// constants
#define OBSTACLE_TEST_DIST 5.0f // m


//*****************************************************************************
class VuObstacleRayTestResult : public VuDynamicsRayTest::VuClosestResult
{
public:
	virtual bool needsCollision(VuRigidBody *pRigidBody)
	{
		if ( pRigidBody->getCollisionGroup() & COL_ENGINE_STATIC_PROP )
			return true;

		return false;
	}
};

//*****************************************************************************
static bool CalcObstacleTransform(VuCarEntity *pCar, VuMatrix &transform)
{
	float backDist = -pCar->getCollisionAabb().mMin.mY;

	// first do a line test straight down
	VuObstacleRayTestResult rayTestResult;
	VuVector3 pos = pCar->getModelMatrix().transform(pCar->getCollisionAabb().getCenter() - VuVector3(0.0f, backDist, 0.0f));
	VuVector3 ray(0.0f, 0.0f, -OBSTACLE_TEST_DIST);
	VuDynamicsRayTest::test(pos, pos + ray, rayTestResult);
	if ( rayTestResult.mbHasHit )
	{
		pos = pos + ray*rayTestResult.mHitFraction;

		VuMathUtil::buildOrientationMatrixUp(pCar->getModelMatrix().getAxisY(), rayTestResult.mHitNormal, transform);
		transform.setTrans(pos);
		
		return true;
	}

	return false;
}


//*****************************************************************************
// VuCarDropBreakableEffect
//*****************************************************************************

class VuCarDropBreakableEffect : public VuCarEffect
{
public:
	virtual void onApply(const VuJsonContainer &data)
	{
		VuMatrix transform;
		if ( CalcObstacleTransform(mpCar, transform) )
			VuEntityRepository::IF()->addManagedEntity(new VuObstacleEntity(data["BreakableData"], mpCar, transform));
	}
};

IMPLEMENT_CAR_EFFECT(VuCarDropBreakableEffect)


//*****************************************************************************
// VuCarDropFakeEffect
//*****************************************************************************

class VuCarDropFakeEffect : public VuCarEffect
{
public:
	virtual void onApply(const VuJsonContainer &data)
	{
		VuMatrix transform;
		if ( CalcObstacleTransform(mpCar, transform) )
			VuEntityRepository::IF()->addManagedEntity(new VuFakePowerUpEntity(data["FakeData"], mpCar, transform));
	}
};

IMPLEMENT_CAR_EFFECT(VuCarDropFakeEffect)


//*****************************************************************************
// VuCarDropSpringEffect
//*****************************************************************************

class VuCarDropSpringEffect : public VuCarEffect
{
public:
	virtual void onApply(const VuJsonContainer &data)
	{
		VuMatrix transform;
		if ( CalcObstacleTransform(mpCar, transform) )
			VuEntityRepository::IF()->addManagedEntity(new VuSpringObstacleEntity(data["SpringData"], mpCar, transform));
	}
};

IMPLEMENT_CAR_EFFECT(VuCarDropSpringEffect)


//*****************************************************************************
// VuCarDropOilSlickEffect
//*****************************************************************************

class VuCarDropOilSlickEffect : public VuCarEffect
{
public:
	virtual void onApply(const VuJsonContainer &data)
	{
		if ( VuOilSlickEntity *pOilSlick = VuOilSlickEntity::drop(data["OilSlickData"], mpCar) )
			VuEntityRepository::IF()->addManagedEntity(pOilSlick);
	}
};

IMPLEMENT_CAR_EFFECT(VuCarDropOilSlickEffect)

