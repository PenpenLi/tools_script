//*****************************************************************************
//
//  Copyright (c) 2007-2008 Ralf Knoesel
//  Copyright (c) 2008-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  CarBody class
// 
//*****************************************************************************

#include "VuCarChassis.h"
#include "VuCarSuspension.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarEffectController.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Dynamics/VuCollisionTypes.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Dynamics/VuDynamics.h"
#include "VuEngine/Water/VuWater.h"
#include "VuEngine/Water/VuFluidsObject.h"
#include "VuEngine/HAL/Audio/VuAudio.h"
#include "VuEngine/Pfx/VuPfx.h"
#include "VuEngine/Pfx/VuPfxManager.h"
#include "VuEngine/Pfx/VuPfxEntity.h"
#include "VuEngine/Managers/VuLensWaterManager.h"
#include "VuEngine/Math/VuMatrix.h"
#include "VuEngine/Math/VuUnitConversion.h"
#include "VuEngine/Math/VuMathUtil.h"
#include "VuEngine/Json/VuJsonContainer.h"
#include "VuEngine/Util/VuDataUtil.h"
#include "VuEngine/Util/VuSpreadsheetQuery.h"


// constants

#define WAKE_SEGMENT_EMISSION_RATE 2.0f // per second

#define INITIAL_WATER_SELF_RIGHTING_TIME_THRESHOLD 0.0f
#define INITIAL_WATER_SELF_RIGHTING_BASE_ACCEL 360.0f

#define LARGE_IMPACT 10.0f // m/s
#define SUBMERGED_ANGULAR_DAMPING 0.5f //0.95f
#define LAVA_LINEAR_DAMPING 0.75f
#define LAVA_ANGULAR_DAMPING 0.8f
#define BIG_SPLASH_DROPLET_COUNT 256

#define RAMP_JUMP_ROLL_MULTIPLIER 4.0f
#define RAMP_JUMP_PITCH_MULTIPLIER 4.0f

#define POWERSLIDE_LATERAL_SLOWDOWN_RATIO (0.25f)

#define AIRBORNE_CORRECTION_TIME_THRESHOLD 0.25f


//*****************************************************************************
VuCarChassis::VuCarChassis(VuCarEntity *pCar):
	mpCar(pCar),
	mWaterSelfRightingTimeThreshold(INITIAL_WATER_SELF_RIGHTING_TIME_THRESHOLD),
	mWaterSelfRightingBaseAccel(INITIAL_WATER_SELF_RIGHTING_BASE_ACCEL),
	mHydrodynamicCenter(0,0,0),
	mAerodynamicCenter(0,0,0),
	mStability(0,0,0),
	mWaterDragDensity(1000.0f),
	mWaterBuoyancyDensity(1000.0f),
	mAirDensity(1.23f),
	mDensityRatio(1),
	mDensityRatioSpeed(0),
	mLinVelSpeed0(1.0f),
	mLinVelFactor0(1.0f),
	mLinVelSpeed1(1.0f),
	mLinVelFactor1(1.0f),
	mLatSkinFrictionCoeff(0),
	mLongSkinFrictionCoeff(0),
	mPowerSlideCoeff(0),
	mDragCoeff(0),
	mSlowSteeringBoatSpeed(0),
	mSlowSteeringSpeed(0),
	mFastSteeringBoatSpeed(0),
	mFastSteeringSpeed(0),
	mAirSteeringSpeed(0),
	mAeroLift(0),
	mAirborneSpringCoeff(0),
	mAirborneDampingCoeff(0),
	mAirborneControlCoeff(0),
	mWakePos(0,0,0),
	mAudioBounceThreshold(0),
	mAudioBigSplashThreshold(0),
	mWaterSelfRightingTimer(0),
	mAirborneTimer(0),
	mbBigSplash(false),
	mpFluidsObject(VUNULL),
	mpCurFlatWakeWave(VUNULL),
	mLooseSteeringActive(false),
	mInducedPowerSlideSteeringFactor(1.0f),
	mLastSubmergedVolume(FLT_MAX)
{
	mpFluidsObject = new VuFluidsObject;
	mpFluidsObject->setKillDownwardPressure(true);
}

//*****************************************************************************
VuCarChassis::~VuCarChassis()
{
	delete mpFluidsObject;

	// release wake waves
	if ( mpCurFlatWakeWave )
		mpCurFlatWakeWave->removeRef();
}

//*****************************************************************************
void VuCarChassis::setData(const VuJsonContainer &data)
{
	VuDataUtil::getValue(data["Fluids Mesh Asset"], mFluidsMeshAssetName);
	VuDataUtil::getValue(data["Hydrodynamic Center"], mHydrodynamicCenter);
	VuDataUtil::getValue(data["Aerodynamic Center"], mAerodynamicCenter);
	VuDataUtil::getValue(data["Stability"], mStability);
	VuDataUtil::getValue(data["Water Drag Density"], mWaterDragDensity);
	VuDataUtil::getValue(data["Water Buoyancy Density"], mWaterBuoyancyDensity);
	VuDataUtil::getValue(data["Air Density"], mAirDensity);
	VuDataUtil::getValue(data["Density Ratio"], mDensityRatio);
	VuDataUtil::getValue(data["Density Ratio Speed"], mDensityRatioSpeed);
	VuDataUtil::getValue(data["Lin Vel Speed 0"], mLinVelSpeed0);
	VuDataUtil::getValue(data["Lin Vel Factor 0"], mLinVelFactor0);
	VuDataUtil::getValue(data["Lin Vel Speed 1"], mLinVelSpeed1);
	VuDataUtil::getValue(data["Lin Vel Factor 1"], mLinVelFactor1);
	VuDataUtil::getValue(data["Lat Skin Friction Coeff"], mLatSkinFrictionCoeff);
	VuDataUtil::getValue(data["Long Skin Friction Coeff"], mLongSkinFrictionCoeff);
	VuDataUtil::getValue(data["Power Slide Coeff"], mPowerSlideCoeff);
	VuDataUtil::getValue(data["Drag Coeff"], mDragCoeff);
	VuDataUtil::getValue(data["Slow Steering Boat Speed"], mSlowSteeringBoatSpeed);
	VuDataUtil::getValue(data["Slow Steering Speed"], mSlowSteeringSpeed);
	VuDataUtil::getValue(data["Fast Steering Boat Speed"], mFastSteeringBoatSpeed);
	VuDataUtil::getValue(data["Fast Steering Speed"], mFastSteeringSpeed);
	VuDataUtil::getValue(data["Air Steering Speed"], mAirSteeringSpeed);
	VuDataUtil::getValue(data["Aero Lift"], mAeroLift);
	VuDataUtil::getValue(data["Airborne Spring Coeff"], mAirborneSpringCoeff);
	VuDataUtil::getValue(data["Airborne Damping Coeff"], mAirborneDampingCoeff);
	VuDataUtil::getValue(data["Airborne Control Coeff"], mAirborneControlCoeff);
	VuDataUtil::getValue(data["Wake Range Start Ratio"], mWakeWaveDesc.mRangeStartRatio);
	VuDataUtil::getValue(data["Wake Range Decay Ratio"], mWakeWaveDesc.mRangeDecayRatio);
	VuDataUtil::getValue(data["Wake Position"], mWakePos);
	VuDataUtil::getValue(data["Audio"]["Bounce Threshold"], mAudioBounceThreshold);
	VuDataUtil::getValue(data["Audio"]["Big Splash Threshold"], mAudioBigSplashThreshold);
	VuDataUtil::getValue(data["Pfx"]["Big Splash"], mBigSplashPFX);
	VuDataUtil::getValue(data["Pfx"]["Lava Splash"], mLavaSplashPFX);
	VuDataUtil::getValue(data["Pfx"]["Lava Burn"], mLavaBurnPFX);
}

//*****************************************************************************
void VuCarChassis::postDataModified()
{
	// create fluids object
	mpFluidsObject->setAsset(mFluidsMeshAssetName);

	// handling data
	float steeringFactor = VuGameUtil::IF()->interpolateCarLevel(mpCar->getCarName(), mpCar->getCarStage(), mpCar->getHandlingLevel(), "Steering");

	mSlowSteeringSpeed *= steeringFactor;
	mFastSteeringSpeed *= steeringFactor;
	mAirSteeringSpeed *= steeringFactor;
}

//*****************************************************************************
void VuCarChassis::onTick(float fdt)
{
	const VuVector3 &pos = mpCar->getModelPosition();
	const VuVector3 &vel = mpCar->getLinearVelocity();

	// is the boat upside-down and in the water?
	if ( mpCar->getModelMatrix().getAxisZ().mZ < 0.5f && mpFluidsObject->getSubmergedVolume() > 0 )
	{
		mWaterSelfRightingTimer += fdt;
	}
	else
	{
		mWaterSelfRightingTimer = 0;
	}

	// is the car airborne?
	if ( !mpCar->getCollidedThisFrame() && mpCar->getSuspension()->getWheelContactCount() == 0 && mpFluidsObject->getSubmergedVolume() == 0 )
		mAirborneTimer += fdt;
	else
		mAirborneTimer = 0;

	// splash sfx
	{
		float fImpact = 0;
		float fSubmergedVolume = mpFluidsObject->getSubmergedVolume();

		if ( mpCar->getIsCameraTarget() )
		{
			if ( mLastSubmergedVolume < mAudioBounceThreshold && fSubmergedVolume >= mAudioBounceThreshold )
			{
				fImpact = -VuDot(mpCar->getLinearVelocity(), mpFluidsObject->getAvgWaterNormal())/10.0f;
				fImpact = VuClamp((fImpact - 0.2f)*1.25f, 0.0f, 1.0f);

				if ( fImpact > 0.0f )
				{
					// play SFX
					if ( mSplashAudioEvent.create("Global/Water/Splash") )
					{
						mSplashAudioEvent.set3DAttributes(&pos, &vel, VUNULL);
						mSplashAudioEvent.start();
						mSplashAudioEvent.setParameterValue("intensity", fImpact);
					}
				}
			}
		}

		mLastSubmergedVolume = fSubmergedVolume;
	}

	// big splash
	if ( mbBigSplash )
	{
		// pfx
		const char *strPfx = mBigSplashPFX.c_str();
		const char *strSfx = "Global/Water/BigSplash";
		if ( mpCar->getWaterSurfaceFlags() & WATER_SURFACE_LAVA )
		{
			strPfx = mLavaSplashPFX.c_str();
			strSfx = "Global/Water/BigSplash";
		}

		if ( VUUINT32 hPfx = VuPfxManager::IF()->createEntity(strPfx, true) )
		{
			if ( VuPfxEntity *pPfxEntity = VuPfxManager::IF()->getEntity(hPfx) )
			{
				VuVector3 linVel = mpCar->getLinearVelocity();
				linVel.mZ = 0.0f;
				VuMatrix mat;
				VuMathUtil::buildOrientationMatrix(linVel, VuVector3(0,0,1), mat);
				mat.setTrans(mpCar->getModelPosition());

				pPfxEntity->getSystemInstance()->setColor(mpCar->getPfxColor());
				pPfxEntity->getSystemInstance()->setMatrix(mat);
				pPfxEntity->getSystemInstance()->setLinearVelocity(linVel);
				pPfxEntity->getSystemInstance()->start();
			}
		}

		// sfx
		if ( mBigSplashAudioEvent.create(strSfx) )
		{
			mBigSplashAudioEvent.set3DAttributes(&pos, &vel, VUNULL);
			mBigSplashAudioEvent.start();
		}

		// lens water
		if ( mpCar->getWaterSurfaceFlags() & WATER_SURFACE_WATER )
		{
			int viewport;
			if ( VuCarManager::IF()->getViewport(mpCar, viewport) )
				VuLensWaterManager::IF()->addDroplets(viewport, BIG_SPLASH_DROPLET_COUNT);
		}

		mbBigSplash = false;
	}

	if ( mSplashAudioEvent.active() )
		mSplashAudioEvent.set3DAttributes(&pos, &vel, VUNULL);

	if ( mBigSplashAudioEvent.active() )
		mBigSplashAudioEvent.set3DAttributes(&pos, &vel, VUNULL);

	// advance wake
	advanceWake();
}

//*****************************************************************************
void VuCarChassis::onApplyForces(float fdt)
{
	VuRigidBody &rb = *mpCar->getRigidBody();

	int tornadoActive = mpCar->getEffectController()->isActive(VuCarEffectController::TORNADO);

	// get current model matrix
	VuMatrix matModel = VuDynamicsUtil::toVuMatrix(rb.getCenterOfMassTransform());
	matModel.translateLocal(-mpCar->getCenterOfMass());

	// calculate forces
	VuVector3 vTotalForce(0,0,0), vTotalTorque(0,0,0);
	{
		calculateFluidsForces(fdt, matModel, vTotalForce, vTotalTorque);
		calculateDragForces(fdt, matModel, vTotalForce, vTotalTorque);

		if ( !mpCar->getDriver()->isRagdollActive() && !tornadoActive )
		{
			calculateLiftForces(fdt, matModel, vTotalForce, vTotalTorque);

			if ( mWaterSelfRightingTimer )
			{
				calculateWaterSelfRightingForces(fdt, matModel, vTotalForce, vTotalTorque);
			}
			else
			{
				calculateStabilityForces(fdt, matModel, vTotalForce, vTotalTorque);
				calculatePowerSlideForces(fdt, matModel, vTotalForce, vTotalTorque);
				applySteering(fdt, matModel);
			}
		}
	}

	// calculate airborne correction
	if ( mAirborneTimer > 0.0f && !tornadoActive )
		calculateAirborneCorrectionForces(fdt, matModel, vTotalForce, vTotalTorque);

	// for large impulses relative to water surface, clamp to velocity
	float fNormalForce = VuDot(vTotalForce, mpFluidsObject->getAvgWaterNormal());
	float fNormalVel = VuDot(mpCar->getLinearVelocity() - mpFluidsObject->getAvgWaterVel(), mpFluidsObject->getAvgWaterNormal());
	if ( fNormalForce > 0 && fNormalVel < 0 )
	{
		float fImpulse = fNormalForce*mpCar->getRigidBody()->getInvMass()*fdt;
		if ( fImpulse > LARGE_IMPACT && fImpulse > -fNormalVel)
		{
			float fRatio = -fNormalVel/fImpulse;
			vTotalForce *= fRatio;
			vTotalTorque *= fRatio;
		}
	}

	// handle big splashes
	if ( mpFluidsObject->getSubmergedVolume() > 0 && fNormalVel < -mAudioBigSplashThreshold )
		mbBigSplash = true;

	// damping
	float linearDamping = 0.0f;
	float angularDamping = 0.0f;
	if ( mpFluidsObject->getSubmergedVolume() > 0 )
	{
		if ( mpCar->getWaterSurfaceFlags() & WATER_SURFACE_LAVA )
		{
			linearDamping = LAVA_LINEAR_DAMPING;
			angularDamping = LAVA_ANGULAR_DAMPING;
		}
		else
		{
			angularDamping = SUBMERGED_ANGULAR_DAMPING;
		}
	}
	rb.setDamping(linearDamping, angularDamping);

	// apply to rigid body
	VuDynamicsUtil::applyCentralForce(rb, vTotalForce);
	VuDynamicsUtil::applyTorque(rb, vTotalTorque);
}

//*****************************************************************************
void VuCarChassis::pushLooseSteering()
{
	if ( !mLooseSteeringActive )
		mLooseSteeringTimer = 0.0f;

	mLooseSteeringActive++;
}

//*****************************************************************************
void VuCarChassis::popLooseSteering()
{
	mLooseSteeringActive--;
}

//*****************************************************************************
void VuCarChassis::advanceWake()
{
	VuWaterWakeWaveParams params;
	calculateWakeParams(params);

	if ( !mpCurFlatWakeWave )
	{
		mpCurFlatWakeWave = VuWater::IF()->createFlatWakeWave(mWakeWaveDesc, params);
		return;
	}

	// update current wake wave
	mpCurFlatWakeWave->update(params);

	// check if we should emit a new wave
	if ( mpCurFlatWakeWave->age() < 1.0f/WAKE_SEGMENT_EMISSION_RATE )
		return;

	// create new wake wave
	mpCurFlatWakeWave->removeRef();
	mpCurFlatWakeWave = VuWater::IF()->createFlatWakeWave(mWakeWaveDesc, params);
}

//*****************************************************************************
void VuCarChassis::calculateWakeParams(VuWaterWakeWaveParams &params)
{
	btRigidBody &rb = *mpCar->getRigidBody();
	VuMatrix matModel = mpCar->getModelMatrix();

	VuVector3 vWakePos = matModel.transform(mWakePos);
	VuVector3 vWakeVel = VuDynamicsUtil::toVuVector3(rb.getVelocityInLocalPoint(VuDynamicsUtil::toBtVector3(vWakePos - matModel.getTrans())));
	vWakeVel.mZ = 0;
	float fVel = vWakeVel.mag();
	if ( fVel > 0 )
		vWakeVel /= fVel;

	float fSpeedRatio = (fVel - 1.0f)/(10.0f - 1.0f);
	fSpeedRatio = VuClamp(fSpeedRatio, 0.0f, 1.0f);

	float fHeightRatio = 1.0f - mpFluidsObject->getMinWaterHeight()/2.0f;
	fHeightRatio = VuClamp(fHeightRatio, 0.0f, 1.0f);
	
	float fRatio = fSpeedRatio*fHeightRatio;

	params.mPosition = vWakePos;
	params.mDirection = VuVector2(vWakeVel.mX, vWakeVel.mY);
	params.mFalloffTime = fRatio*2.0f;
	params.mDecayTime = fRatio*1.0f;
	params.mRange = fRatio*12.0f;
	params.mSpeed = fRatio*10.0f + 0.1f;
}

//*****************************************************************************
void VuCarChassis::calculateFluidsForces(float fdt, const VuMatrix &matModel, VuVector3 &vTotalForce, VuVector3 &vTotalTorque)
{
	btRigidBody &rb = *mpCar->getRigidBody();

	// set densities
	{
		float fRatio = 1.0f;
		if ( mDensityRatioSpeed > 0.0f && mDensityRatio < 1.0f )
		{
			fRatio = VuLerp(1.0f, mDensityRatio, mpCar->getLinearVelocity().mag2d()/VuMphToMetersPerSecond(mDensityRatioSpeed));
			fRatio = VuClamp(fRatio, mDensityRatio, 1.0f);
		}

		mpFluidsObject->setWaterDragDensity(fRatio*mWaterDragDensity);
		mpFluidsObject->setWaterBuoyancyDensity(fRatio*mWaterBuoyancyDensity);
	}

	// set lin vel factor
	{
		float linVelFactor = 1.0f;
		float speedMph = VuMetersPerSecondToMph(mpCar->getLinearVelocity().mag());
		if ( speedMph <= mLinVelSpeed0 )
		{
			linVelFactor = mLinVelFactor0;
		}
		else if ( speedMph >= mLinVelSpeed1 )
		{
			linVelFactor = mLinVelFactor1;
		}
		else
		{
			linVelFactor = VuLerp(mLinVelFactor0, mLinVelFactor1, (speedMph - mLinVelSpeed0)/(mLinVelSpeed1 - mLinVelSpeed0));
		}
		mpFluidsObject->setLinearVelocityFactor(linVelFactor);
	}

	mpFluidsObject->setHydrodynamicCenter(mHydrodynamicCenter - mpCar->getCenterOfMass());
	mpFluidsObject->setSkinFrictionCoeff(VuVector3(mLatSkinFrictionCoeff, mLongSkinFrictionCoeff, mLatSkinFrictionCoeff));
	mpFluidsObject->setTransform(matModel);
	mpFluidsObject->updateForces(fdt, rb);

	vTotalForce += mpFluidsObject->getTotalForce();
	vTotalTorque += mpFluidsObject->getTotalTorque();
}

//*****************************************************************************
void VuCarChassis::calculateDragForces(float fdt, const VuMatrix &matModel, VuVector3 &vTotalForce, VuVector3 &vTotalTorque)
{
	VuVector3 vForce(0,0,0);
	VuVector3 vTorque(0,0,0);

	const VuVector3 &linVel = mpCar->getLinearVelocity();
	float velSquared = linVel.magSquared();
	if ( velSquared > FLT_EPSILON )
	{
		float drag = mDragCoeff*mpCar->getLinearVelocity().magSquared();
		drag *= mpCar->getSuspension()->getWheelContactCount()/4.0f;
		vForce = -drag*linVel.normal();
	}

	// apply force
	vTotalForce += vForce;
	vTotalTorque += vTorque;
}

//*****************************************************************************
void VuCarChassis::calculateLiftForces(float fdt, const VuMatrix &matModel, VuVector3 &vTotalForce, VuVector3 &vTotalTorque)
{
	VuVector3 vForce(0,0,0);
	VuVector3 vTorque(0,0,0);

	// is the car in water?
	if ( mpFluidsObject->getSubmergedVolume() > 0 )
	{
		VuRigidBody &rb = *mpCar->getRigidBody();
		VuMatrix matRB = VuDynamicsUtil::toVuMatrix(rb.getCenterOfMassTransform());

		// use velocity of center of mass relative to water velocity
		VuVector3 vVel = rb.getVuLinearVelocity() - mpFluidsObject->getAvgWaterVel();

		// calculate 'up' vector in velocity space
		VuVector3 vRight = VuCross(vVel, VuVector3(0,0,1));
		VuVector3 vUp = VuCross(vRight, vVel);
		if ( vUp.magSquared() > FLT_EPSILON )
		{
			vUp.normalize();
			float fForce = 0.5f*mAirDensity*vVel.magSquared()*mAeroLift;
			vForce = fForce*vUp;
		}

		// torque
		VuVector3 vAeroCenter = matRB.transform(mAerodynamicCenter);
		vTorque = VuCross(vAeroCenter - matRB.getTrans(), vForce);
	}

	// apply force
	vTotalForce += vForce;
	vTotalTorque += vTorque;
}

//*****************************************************************************
void VuCarChassis::calculateWaterSelfRightingForces(float fdt, const VuMatrix &matModel, VuVector3 &vTotalForce, VuVector3 &vTotalTorque)
{
	// is timer above threshold?
	if ( mWaterSelfRightingTimer > mWaterSelfRightingTimeThreshold )
	{
		btRigidBody &rb = *mpCar->getRigidBody();

		// calculate angular acceleration
		float fAccel = VuDegreesToRadians(mWaterSelfRightingBaseAccel)*(1.0f + mWaterSelfRightingTimer - mWaterSelfRightingTimeThreshold)*VuAbs(matModel.getAxisZ().mZ - 0.5f);
		fAccel = VuSelect(matModel.getAxisX().mZ, fAccel, -fAccel);
		VuVector3 vAccel = matModel.transformNormal(VuVector3(0, fAccel, 0));

		// calculate torque
		btMatrix3x3 inertiaTensor = rb.getInvInertiaTensorWorld().inverse();
		btVector3 vTorque = VuDynamicsUtil::toBtVector3(vAccel)*inertiaTensor;

		// add torque
		vTotalTorque += VuDynamicsUtil::toVuVector3(vTorque);
	}
}

//*****************************************************************************
void VuCarChassis::calculateStabilityForces(float fdt, const VuMatrix &matModel, VuVector3 &vTotalForce, VuVector3 &vTotalTorque)
{
	btRigidBody &rb = *mpCar->getRigidBody();

	// current angular rate
	float fCurRateX = VuDot(matModel.getAxisX(), mpCar->getAngularVelocity());
	float fCurRateY = VuDot(matModel.getAxisY(), mpCar->getAngularVelocity());
	float fCurRateZ = VuDot(matModel.getAxisZ(), mpCar->getAngularVelocity());

	// calculate angular acceleration
	VuVector3 vAccel = VuVector3(-fCurRateX*mStability.mX, -fCurRateY*mStability.mY, -fCurRateZ*mStability.mZ);
	vAccel = matModel.transformNormal(vAccel);

	// calculate torque
	btMatrix3x3 inertiaTensor = rb.getInvInertiaTensorWorld().inverse();
	btVector3 vTorque = VuDynamicsUtil::toBtVector3(vAccel)*inertiaTensor;

	// add torque
	vTotalTorque += VuDynamicsUtil::toVuVector3(vTorque);
}

//*****************************************************************************
void VuCarChassis::calculatePowerSlideForces(float fdt, const VuMatrix &matModel, VuVector3 &vTotalForce, VuVector3 &vTotalTorque)
{
	// is the car in water?
	if ( mpFluidsObject->getSubmergedVolume() > 0 )
	{
		btRigidBody &rb = *mpCar->getRigidBody();
		const VuVector3 &vNormal = mpFluidsObject->getAvgWaterNormal();

		// determine velocity on water plane
		VuVector3 vVel = mpCar->getLinearVelocity();
		vVel -= vNormal*VuDot(vVel, vNormal);

		// determine heading on water plane
		VuVector3 vHeading = matModel.getAxisY();
		vHeading -= vNormal*VuDot(vHeading, vNormal);

		// transform velocity by 90 degrees
		VuVector3 vVelRight = VuCross(vVel, vNormal);
		if ( vVelRight.magSquared() > FLT_EPSILON )
		{
			float fForce = mPowerSlideCoeff*VuDot(vHeading, vVelRight)/rb.getInvMass();
			VuVector3 vForce = fForce*vVelRight.normal();

			// some minor slowdown so that cars don't get too fast by powersliding
			vForce -= (POWERSLIDE_LATERAL_SLOWDOWN_RATIO*vForce.mag())*vVel.normal();

			// apply force
			vTotalForce += vForce;
		}
	}
}

//*****************************************************************************
void VuCarChassis::calculateAirborneCorrectionForces(float fdt, const VuMatrix &matModel, VuVector3 &vTotalForce, VuVector3 &vTotalTorque)
{
	VuRigidBody &rb = *mpCar->getRigidBody();

	// orientation
	{
		VuQuaternion quatModel;
		quatModel.fromRotationMatrix(matModel);

		VuMatrix matUpright;
		VuMathUtil::buildOrientationMatrixUp(matModel.getAxisY(), VuVector3(0.0f, 0.0f, 1.0f), matUpright);

		VuQuaternion quatUpright;
		quatUpright.fromRotationMatrix(matUpright);

		VuQuaternion deltaQuat = VuMathUtil::rotationDelta(quatModel, quatUpright);
		VuVector3 deltaAxis;
		float deltaAngle;
		deltaQuat.toAxisAngle(deltaAxis, deltaAngle);

		VuVector3 deltaRot = -deltaAngle*deltaAxis;
	
		// spring
		deltaRot = matModel.transformNormal(deltaRot);
		VuVector3 accel = mAirborneSpringCoeff*deltaRot;

		// damping
		VuVector3 deltaRotVel = -rb.getVuAngularVelocity();
		accel += mAirborneDampingCoeff*deltaRotVel;

		// ramp up
		float ramp = VuMin(mAirborneTimer/AIRBORNE_CORRECTION_TIME_THRESHOLD, 1.0f);
		accel *= ramp;

		// calculate torque
		btMatrix3x3 inertiaTensor = rb.getInvInertiaTensorWorld().inverse();
		btVector3 torque = VuDynamicsUtil::toBtVector3(accel)*inertiaTensor;

		// add torque
		vTotalTorque += VuDynamicsUtil::toVuVector3(torque);
	}

	// steering
	{
		// determine veloicty in xy plane
		VuVector3 velXY = mpCar->getLinearVelocity();
		velXY.mZ = 0.0f;

		// determine heading in xy plane
		VuVector3 headingXY = matModel.getAxisY();
		headingXY.mZ = 0.0f;

		// transform velocity by 90 degrees
		VuVector3 normal(0.0f, 0.0f, 1.0f);
		VuVector3 velRightXY = VuCross(velXY, normal);
		if ( velRightXY.magSquared() > FLT_EPSILON )
		{
			float airborneControlMultiplier = mAirborneControlCoeff;

			float fForce = airborneControlMultiplier * VuDot(headingXY, velRightXY)/rb.getInvMass();
			VuVector3 vForce = fForce*velRightXY.normal();

			// apply force
			vTotalForce += vForce;
		}
	}
}

//*****************************************************************************
void VuCarChassis::applySteering(float fdt, const VuMatrix &matModel)
{
	btRigidBody &rb = *mpCar->getRigidBody();
	VuMatrix matModelInv = matModel;
	matModelInv.invert();

	// determine steering speed

	// SAM 081314 added Handling Buff multiplier in for air steering as it's used
	float fSteeringSpeed = mAirSteeringSpeed * mpCar->getAiBuffHandling();

	if ( mpCar->getSuspension()->getWheelContactCount() )
	{
		float fBoatSpeedMPH = VuMetersPerSecondToMph(mpCar->getLinearVelocity().mag());
		float fRatio = (fBoatSpeedMPH - mSlowSteeringBoatSpeed)/(mFastSteeringBoatSpeed - mSlowSteeringBoatSpeed);
		fRatio = VuClamp(fRatio, 0.0f, 1.0f);
		fSteeringSpeed = VuLerp(mSlowSteeringSpeed, mFastSteeringSpeed, fRatio);
	}

	float fMaxRate = VuDegreesToRadians(fSteeringSpeed)*mInducedPowerSlideSteeringFactor;
	float fControl = -mpCar->getYawControl();

	// square
	float fControlSign = VuSelect(fControl, 1.0f, -1.0f);
	fControl = VuAbs(fControl);
	fControl *= fControl;
	fControl *= fControlSign;

	// handle loose steering
	if ( mLooseSteeringActive )
	{
		if ( mpCar->getSuspension()->getWheelContactCount() )
		{
			fMaxRate *= mLooseSteeringParams.mSteeringFactor;
			fControl += mLooseSteeringParams.mSteeringErrorAmount*VuSin(mLooseSteeringTimer*VU_2PI/mLooseSteeringParams.mSteeringErrorInterval);
		}
		mLooseSteeringTimer += fdt;
	}

	// determine rate
	VuVector3 vRate = matModelInv.transformNormal(mpCar->getAngularVelocity());
	vRate.mZ = fMaxRate*fControl;

	rb.setAngularVelocity(VuDynamicsUtil::toBtVector3(matModel.transformNormal(vRate)));
}
