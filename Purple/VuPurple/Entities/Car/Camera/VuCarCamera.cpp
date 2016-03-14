//*****************************************************************************
//
//  Copyright (c) 2007-2008 Ralf Knoesel
//  Copyright (c) 2008-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  CarCamera class
// 
//*****************************************************************************

#include "VuCarCamera.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/Managers/VuControlMethodManager.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Dynamics/Util/VuDynamicsRayTest.h"
#include "VuEngine/HAL/Gfx/VuGfx.h"
#include "VuEngine/Math/VuUnitConversion.h"
#include "VuEngine/Math/VuMathUtil.h"
#include "VuEngine/Water/VuWater.h"
#include "VuEngine/Managers/VuViewportManager.h"
#include "VuEngine/Util/VuDataUtil.h"
#include "VuEngine/Json/VuJsonContainer.h"


// constants

#define STANDARD_NEAR_PLANE	0.5f
#define COLLISION_DIST		1.0f
#define STAY_BEHIND_DAMPING	1.0f
#define STAY_BEHIND_REVERSE_SPEED 10.0f // mph


class VuCarCameraRayTestResult : public VuDynamicsRayTest::VuClosestResult
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
VuCarCamera::VuCarCamera(VuCarEntity *pCar):
	mpCar(pCar),
	mCarLinearVelocityHistoryIndex(0),
	mCarLinearVelocity(0,0,0),
	mTargetLocalOffset(0,-1,0),
	mTargetWorldZOffset(1.5f),
	mIdealDist(4.5f),
	mIdealPitch(-10),
	mLagDist(4.0f),
	mTiltStayBehindFactor(0.5f),
	mIdealPosition(0,0,0),
	mPosition(0,0,0),
	mbSnap(true),
	mRollControl(0.0f),
	mbReverseControl(false),
	mCarVelocity(0.0f),
	mShakeMagnitude(0.0f),
	mShakeTotalTime(0.0f),
	mShakeFalloffTime(0.0f),
	mShakeFreq(1.0f),
	mShakeAxis(0.0f, 0.0f, 1.0f),
	mStayBehindFactor(0.0f)
{
	memset(mCarLinearVelocityHistory, 0, sizeof(mCarLinearVelocityHistory));
}

//*****************************************************************************
VuCarCamera::~VuCarCamera()
{
}

//*****************************************************************************
void VuCarCamera::setData(const VuJsonContainer &data)
{
	VuDataUtil::getValue(data["Target Local Offset"], mTargetLocalOffset);
	VuDataUtil::getValue(data["Target World Z Offset"], mTargetWorldZOffset);
	VuDataUtil::getValue(data["Ideal Distance"], mIdealDist);
	VuDataUtil::getValue(data["Ideal Pitch"], mIdealPitch);
	VuDataUtil::getValue(data["Lag Distance"], mLagDist);
	VuDataUtil::getValue(data["Tilt Stay Behind Factor"], mTiltStayBehindFactor);
}

//*****************************************************************************
void VuCarCamera::onTick(float fdt)
{
	// smooth car velocity over time
	if ( fdt > FLT_EPSILON )
	{
		mCarLinearVelocityHistory[mCarLinearVelocityHistoryIndex] = mpCar->getLinearVelocity();
		mCarLinearVelocityHistoryIndex = (mCarLinearVelocityHistoryIndex + 1)%HISTORY_SIZE;
		mCarLinearVelocity = VuVector3(0,0,0);
		for ( int i = 0; i < HISTORY_SIZE; i++ )
			mCarLinearVelocity += mCarLinearVelocityHistory[i];
		mCarLinearVelocity /= HISTORY_SIZE;
	}

	VuVector3 vPos;
	VuVector3 vTarget;
	VuVector3 vUp;
	float fNearPlane;

	if ( mpCar->getDriver()->isRagdollActive() )
	{
		tickRagdoll(fdt, vPos, vTarget, vUp, fNearPlane);
	}
	else if ( mpCar->getDriver()->isAi() )
	{
		tickStandard(fdt, vPos, vTarget, vUp, fNearPlane);
	}
	else
	{
		if ( VuControlMethodManager::IF()->isTilt() )
			tickTilt(fdt, vPos, vTarget, vUp, fNearPlane);
		else
			tickStandard(fdt, vPos, vTarget, vUp, fNearPlane);
	}

	tickShake(fdt, vPos, vTarget, vUp);
	collide(vPos, vTarget, vUp);

	float fFarPlane = mpCar->getGfxSettings().mCameraFar;

	int viewport = 0;
	VuCarManager::IF()->getViewport(mpCar, viewport);
	const VuRect &viewportRect = VuViewportManager::IF()->getViewport(viewport).mRect;

	float aspectRatio = VuGameUtil::IF()->calcDisplayAspectRatio()*viewportRect.mWidth/viewportRect.mHeight;
	float vertFov = VuGameUtil::IF()->calcCameraVertFov();
	setProjMatrixVert(vertFov, aspectRatio, fNearPlane, fFarPlane);
	setViewMatrix(vPos, vTarget, vUp);

	// listener velocity
	VuVector3 vListenerVelocity = mCarLinearVelocity;
	if ( mpCar->getDriver()->isRagdollActive() )
		mpCar->getDriver()->getRagdollCameraVelocity(vListenerVelocity);
	setListenerVelocity(vListenerVelocity);

	mPosition = vPos;

	mbSnap = false;
}

//*****************************************************************************
void VuCarCamera::snap()
{
	mbSnap = true;
	memset(mCarLinearVelocityHistory, 0, sizeof(mCarLinearVelocityHistory));
	mCarLinearVelocity = VuVector3(0,0,0);
}

//*****************************************************************************
void VuCarCamera::control(float roll, bool bReverse)
{
	mRollControl = roll;
	mbReverseControl = bReverse;
}

//*****************************************************************************
void VuCarCamera::beginShake(float magnitude, float duration, float falloffTime, float frequency, int priority)
{
	if ( mShakeTotalTime <= 0.0f || (magnitude >= mShakeMagnitude && priority >= mShakePriority) )
	{
		mShakePriority = priority;
		mShakeMagnitude = magnitude;
		mShakeTotalTime = duration + falloffTime;
		mShakeFalloffTime = falloffTime;
		mShakeFreq = frequency;
		mShakeAxis.mX = 0.0f;
		mShakeAxis.mY = 0.0f;
		mShakeAxis.mZ = 1.0f;
	}
}

//*****************************************************************************
void VuCarCamera::stopShake()
{
	mShakeTotalTime = 0.0f;
}

//*****************************************************************************
bool VuCarCamera::hideCarForMode()
{
	return false;
}

//*****************************************************************************
void VuCarCamera::tickTilt(float fdt, VuVector3 &vPos, VuVector3 &vTarget, VuVector3 &vUp, float &fNearPlane)
{
	VuMatrix matCar = mpCar->getModelMatrix();

	// tick standard camera
	tickStandard(fdt, vPos, vTarget, vUp, fNearPlane);

	// determine if we can stay behind
	bool bStayBehind = true;
	if ( matCar.getAxisZ().mZ < 0.5f )
		bStayBehind = false;
	if ( VuDot(matCar.getAxisY(), mCarVelocity) < -VuMphToMetersPerSecond(STAY_BEHIND_REVERSE_SPEED) )
		bStayBehind = false;

	// udpate stay-behind factor
	if ( bStayBehind )
		mStayBehindFactor = VuLerp(mStayBehindFactor, mTiltStayBehindFactor, STAY_BEHIND_DAMPING*fdt);
	else
		mStayBehindFactor = VuLerp(mStayBehindFactor, 0.0f, STAY_BEHIND_DAMPING*fdt);

	// calculate ideal position for tilt control
	float azimuth = matCar.getEulerAngles().mZ - 0.5f*VU_PI;
	VuVector3 vIdealDir = VuMathUtil::sphericalToCartesian(VuVector3(1.0f, azimuth, VuDegreesToRadians(mIdealPitch) + 0.5f*VU_PI));
	VuVector3 idealPosition = vTarget + mIdealDist*vIdealDir;
	VuVector3 vLag = (mLagDist*mCarVelocity.mag()/mpCar->getMaxForwardSpeed())*vIdealDir;
	idealPosition += vLag;

	// interpolate from standard to ideal tilt position
	vPos = VuLerp(vPos, idealPosition, mStayBehindFactor);

	// update roll
	float rollAngle = mRollControl;
	VuMatrix mat;
	VuMathUtil::buildOrientationMatrix(vTarget - vPos, VuVector3(0,0,1), mat);
	mat.rotateYLocal(rollAngle);

	vUp = mat.getAxisZ();

	fNearPlane = STANDARD_NEAR_PLANE;
}

//*****************************************************************************
void VuCarCamera::tickStandard(float fdt, VuVector3 &vPos, VuVector3 &vTarget, VuVector3 &vUp, float &fNearPlane)
{
	VuMatrix matCar = mpCar->getModelMatrix();

	// calculate target location
	vTarget = matCar.transform(mTargetLocalOffset);
	vTarget.mZ += mTargetWorldZOffset;

	// calculate azimuth
	float azimuth;
	if ( mbSnap )
		azimuth = matCar.getEulerAngles().mZ - 0.5f*VU_PI;
	else
		azimuth = VuATan2(mIdealPosition.mY - vTarget.mY, mIdealPosition.mX - vTarget.mX);

	// calculate ideal position in world space
	mIdealPosition = vTarget + VuMathUtil::sphericalToCartesian(VuVector3(mIdealDist, azimuth, VuDegreesToRadians(mIdealPitch) + 0.5f*VU_PI));

	// update Car velocity
	if ( mbSnap )
		mCarVelocity = mCarLinearVelocity;
	else
		mCarVelocity = VuLerp(mCarVelocity, mCarLinearVelocity, VuMin(4.0f*fdt, 1.0f));

	// interpolate position
	VuVector3 vLag = (-mLagDist/mpCar->getMaxForwardSpeed())*mCarVelocity;
	vPos = mIdealPosition + vLag;
	vPos.mZ += vLag.mZ;

	// update roll
	float rollAngle = mRollControl*VU_PIDIV2;
	VuMatrix mat;
	VuMathUtil::buildOrientationMatrix(vTarget - vPos, VuVector3(0,0,1), mat);
	mat.rotateYLocal(rollAngle);

	vUp = mat.getAxisZ();

	fNearPlane = STANDARD_NEAR_PLANE;
}

//*****************************************************************************
void VuCarCamera::tickRagdoll(float fdt, VuVector3 &vPos, VuVector3 &vTarget, VuVector3 &vUp, float &fNearPlane)
{
	// calculate target location
	mpCar->getDriver()->getRagdollCameraTarget(vTarget);
	vTarget.mZ += mTargetWorldZOffset;

	// calculate azimuth
	float azimuth = VuATan2(mIdealPosition.mY - vTarget.mY, mIdealPosition.mX - vTarget.mX);

	// calculate ideal position in world space
	mIdealPosition = vTarget + VuMathUtil::sphericalToCartesian(VuVector3(mIdealDist, azimuth, VuDegreesToRadians(mIdealPitch) + 0.5f*VU_PI));

	// update Car velocity
	VuVector3 vel;
	mpCar->getDriver()->getRagdollCameraVelocity(vel);
	mCarVelocity = VuLerp(mCarVelocity, vel, VuMin(4.0f*fdt, 1.0f));

	// interpolate position
	VuVector3 vLag = (-mLagDist/mpCar->getMaxForwardSpeed())*mCarVelocity;
	vPos = mIdealPosition + vLag;
	vPos.mZ += vLag.mZ;

	// clamp to water surface
	VuWaterPhysicsVertex waterVert = VuWater::IF()->getPhysicsVertex(vPos);
	vPos.mZ = VuMax(vPos.mZ, waterVert.mHeight + 2*STANDARD_NEAR_PLANE);

	// update roll
	float rollAngle = mRollControl*VU_PIDIV2;
	VuMatrix mat;
	VuMathUtil::buildOrientationMatrix(vTarget - vPos, VuVector3(0,0,1), mat);
	mat.rotateYLocal(rollAngle);

	vUp = mat.getAxisZ();

	fNearPlane = STANDARD_NEAR_PLANE;
}

//*****************************************************************************
void VuCarCamera::tickShake(float fdt, VuVector3 &vPos, VuVector3 &vTarget, VuVector3 &vUp)
{
	if ( mShakeTotalTime > 0.0f )
	{
		VuVector3 shakeOffset = (mShakeAxis * mShakeMagnitude * VuSin(mShakeTotalTime * mShakeFreq * VU_2PI));
		if ( mShakeTotalTime < mShakeFalloffTime )
			shakeOffset *= mShakeTotalTime/mShakeFalloffTime;
		
		VuVector3 unitToTarget = (vTarget - vPos).normal();

		vPos = vPos + 0.5f*shakeOffset;
		vTarget = vPos + unitToTarget - 0.5f*shakeOffset;

		mShakeTotalTime -= fdt;
	}
}

//*****************************************************************************
void VuCarCamera::collide(VuVector3 &vPos, VuVector3 &vTarget, VuVector3 &vUp)
{
	VuVector3 vAxisForward = (vTarget - vPos);
	VuVector3 vAxisRight = VuCross(vAxisForward, VuVector3(0,0,1));

	if ( vAxisForward.magSquared() < FLT_MIN || vAxisRight.magSquared() < FLT_MIN )
	{
		vAxisRight = VuVector3(1,0,0);
		vAxisForward = VuVector3(0,1,0);
	}

	vAxisRight = vAxisRight.normal();
	vAxisForward = vAxisForward.normal();

	// back
	{
		VuCarCameraRayTestResult result;
		VuVector3 vTest = vPos - COLLISION_DIST*vAxisForward;
		VuDynamicsRayTest::test(vTarget, vTest, result);
		if ( result.mbHasHit && VuDot(result.mHitNormal, vAxisForward) > 0 )
		{
			vPos = VuLerp(vTarget, vTest, result.mHitFraction) + COLLISION_DIST*vAxisForward;
		}
	}

	// right
	{
		VuCarCameraRayTestResult result;
		VuDynamicsRayTest::test(vPos, vPos + COLLISION_DIST*vAxisRight, result);
		if ( result.mbHasHit && VuDot(result.mHitNormal, vAxisRight) < 0 )
		{
			vPos -= (1.0f - result.mHitFraction)*COLLISION_DIST*vAxisRight;
		}
	}

	// left
	{
		VuCarCameraRayTestResult result;
		VuDynamicsRayTest::test(vPos, vPos - COLLISION_DIST*vAxisRight, result);
		if ( result.mbHasHit && VuDot(result.mHitNormal, vAxisRight) > 0 )
		{
			vPos += (1.0f - result.mHitFraction)*COLLISION_DIST*vAxisRight;
		}
	}
}