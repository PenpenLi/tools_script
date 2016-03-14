//*****************************************************************************
//
//  Copyright (c) 2012-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Car Suspension
// 
//*****************************************************************************

#include "BulletDynamics/ConstraintSolver/btContactConstraint.h"

#include "VuCarSuspension.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Dynamics/VuCollisionTypes.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Assets/VuAssetFactory.h"
#include "VuEngine/Assets/VuAnimationAsset.h"
#include "VuEngine/Assets/VuCollisionMeshAsset.h"
#include "VuEngine/Gfx/Model/VuAnimatedModelInstance.h"
#include "VuEngine/Animation/VuSkeleton.h"
#include "VuEngine/Animation/VuAnimatedSkeleton.h"
#include "VuEngine/Animation/VuAnimationControl.h"
#include "VuEngine/Animation/VuAnimationTransform.h"
#include "VuEngine/Animation/VuAnimation.h"
#include "VuEngine/Dynamics/Util/VuDynamicsRayTest.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/Camera/VuCamera.h"
#include "VuEngine/Util/VuSpreadsheetQuery.h"
#include "VuEngine/Util/VuAudioUtil.h"


class VuSuspensionRayTestResult : public VuDynamicsRayTest::VuClosestResult
{
public:
	VuSuspensionRayTestResult(VUUINT32 mask, VuRigidBody *pIgnore) : mMask(mask), mpIgnore(pIgnore) {}

	virtual bool needsCollision(VuRigidBody *pRigidBody)
	{
		if ( pRigidBody->getCollisionGroup() & mMask )
			if ( pRigidBody->getCollisionMask() & COL_GAME_CAR )
				if ( pRigidBody != mpIgnore )
					return true;

		return false;
	}
	VUUINT32	mMask;
	VuRigidBody *mpIgnore;
};


//*****************************************************************************
VuCarSuspension::VuCarSuspension(VuCarEntity *pCar):
	mpCar(pCar),
	mRolloverResistance(0.0f),
	mWheelieResistance(0.0f),
	mUpperSpringCoeff(0.0f),
	mLowerSpringCoeff(0.0f),
	mDampingCoeff(0.0f),
	mPowerSlideCoeff(0.0f),
	mVisualExtensionRate(1.0f),
	mpAnimatedSkeleton(VUNULL),
	mWheelContactCount(0),
	mAvgContactNormal(0,0,1),
	mBaseTractionFactor(1.0f),
	mSpringFactor(1.0f),
	mDampingFactor(1.0f),
	mTractionFactor(1.0f),
	mInducedPowerSlideTractionFactor(1.0f),
	mInducedPowerSlideCoeff(0.0f)
{
	mpModelInstance = new VuAnimatedModelInstance;
}

//*****************************************************************************
VuCarSuspension::~VuCarSuspension()
{
	clear();

	delete mpModelInstance;
}

//*****************************************************************************
void VuCarSuspension::setData(const VuJsonContainer &data)
{
	VuDataUtil::getValue(data["Model Asset"], mModelAssetName);
	VuDataUtil::getValue(data["Rollover Resistance"], mRolloverResistance);
	VuDataUtil::getValue(data["Wheelie Resistance"], mWheelieResistance);
	VuDataUtil::getValue(data["Upper Spring Coeff"], mUpperSpringCoeff);
	VuDataUtil::getValue(data["Lower Spring Coeff"], mLowerSpringCoeff);
	VuDataUtil::getValue(data["Damping Coeff"], mDampingCoeff);
	VuDataUtil::getValue(data["Power Slide Coeff"], mPowerSlideCoeff);
	VuDataUtil::getValue(data["Visual Extension Rate"], mVisualExtensionRate);

	// corners
	const VuJsonContainer &corners = data["Corners"];
	for ( int i = 0; i < 4; i++ )
	{
		corners[i]["WheelBone"].getValue(mCorners[i].mWheelBoneName);
		corners[i]["Animation"].getValue(mCorners[i].mAnimationName);
	}
}

//*****************************************************************************
void VuCarSuspension::preDataModified()
{
	clear();
}

//*****************************************************************************
void VuCarSuspension::postDataModified()
{
	// create model
	mpModelInstance->setModelAsset(mModelAssetName);

	if ( VuSkeleton *pSkeleton = mpModelInstance->getSkeleton() )
	{
		mpAnimatedSkeleton = new VuAnimatedSkeleton(mpModelInstance->getSkeleton());

		// corners
		for ( int i = 0; i < 4; i++ )
		{
			VuCorner &corner = mCorners[i];
			VuCarWheel &wheel = mpCar->getWheel(i);

			// handle wheel attachment
			int boneIndex = pSkeleton->getBoneIndex(corner.mWheelBoneName.c_str());
			if ( boneIndex >= 0 )
			{
				wheel.mAttachmentPointLS = mpModelInstance->getModelMatrices()[boneIndex].getTrans();

				// animation
				if ( VuAssetFactory::IF()->doesAssetExist<VuAnimationAsset>(corner.mAnimationName) )
				{
					corner.mpAnimationControl = new VuAnimationControl(corner.mAnimationName);
					if ( corner.mpAnimationControl->getAnimation() )
					{
						corner.mpAnimationControl->setWeight(1.0f);
						corner.mpAnimationControl->setTimeFactor(0.0f);
						corner.mpAnimationControl->setLooping(false);
						mpAnimatedSkeleton->addAnimationControl(corner.mpAnimationControl);

						// determine limits
						corner.mpAnimationControl->setLocalTime(1.0f);
						mpAnimatedSkeleton->build();
						corner.mUpperLimit = mpAnimatedSkeleton->getLocalPose()[boneIndex].mTranslation.mZ - wheel.mAttachmentPointLS.mZ;

						corner.mpAnimationControl->setLocalTime(0.0f);
						mpAnimatedSkeleton->build();
						corner.mLowerLimit = mpAnimatedSkeleton->getLocalPose()[boneIndex].mTranslation.mZ - wheel.mAttachmentPointLS.mZ;
					}
					else
					{
						corner.mpAnimationControl->removeRef();
						corner.mpAnimationControl = VUNULL;
					}
				}
			}
		}
	}

	// traction data
	mBaseTractionFactor = VuGameUtil::IF()->interpolateCarLevel(mpCar->getCarName(), mpCar->getCarStage(), mpCar->getHandlingLevel(), "Traction");
}

//*****************************************************************************
const VuAabb &VuCarSuspension::getAabb()
{
	return mpModelInstance->getLocalAabb();
}

//*****************************************************************************
void VuCarSuspension::onTickDecision(float fdt)
{
	const VuMatrix &matModel = mpCar->getModelMatrix();
	int prevWheelContactCount = mWheelContactCount;

	VUUINT32 rayTestMask = COL_ENGINE_STATIC_PROP|COL_ENGINE_DYNAMIC_PROP|COL_GAME_CAR;
	rayTestMask &= mpCar->getRigidBody()->getCollisionMask();

	int wheelContactCount = 0;
	VuVector3 avgContactNormal = VuVector3(0,0,0);

	// recalculate suspension for visual representation
	for ( int iWheel = 0; iWheel < 4; iWheel++ )
	{
		VuCarWheel &wheel = mpCar->getWheel(iWheel);
		VuCorner &corner = mCorners[iWheel];

		VuVector3 attachmentPointWS = matModel.transform(wheel.mAttachmentPointLS);

		float wheelRadius = wheel.getRadius();

		// cast ray
		VuVector3 upperTestPosWS = attachmentPointWS + (corner.mUpperLimit)*matModel.getAxisZ();
		VuVector3 lowerTestPosWS = attachmentPointWS + (corner.mLowerLimit - wheelRadius)*matModel.getAxisZ();

		VuSuspensionRayTestResult rayTestResult(rayTestMask, mpCar->getRigidBody());
		VuDynamicsRayTest::test(upperTestPosWS, lowerTestPosWS, rayTestResult);

		float suspensionDist = corner.mLowerLimit;
		float scaleZ = 1.0f;

		if ( (wheel.mVisualContact = rayTestResult.mbHasHit) )
		{
			wheelContactCount++;
			avgContactNormal += rayTestResult.mHitNormal;

			wheel.mVisualContactPointWS = VuLerp(upperTestPosWS, lowerTestPosWS, rayTestResult.mHitFraction);
			wheel.mVisualContactNormal = rayTestResult.mHitNormal;
			suspensionDist = VuLerp(corner.mUpperLimit, corner.mLowerLimit - wheelRadius, rayTestResult.mHitFraction) + wheelRadius;

			// limit visual suspension dist
			if ( suspensionDist > corner.mUpperLimit )
			{
				scaleZ = (0.5f*(corner.mUpperLimit - suspensionDist) + wheelRadius)/wheelRadius;
				suspensionDist = 0.5f*(corner.mUpperLimit + suspensionDist);
			}

			if ( wheel.mSurfaceTypeOverride != 255 )
				wheel.mVisualContactSurfaceType = wheel.mSurfaceTypeOverride;
			else if ( VuCollisionMeshAsset *pAsset = static_cast<VuCollisionMeshAsset *>(rayTestResult.mpRigidBody->getCollisionShape()->getUserPointer()) )
				wheel.mVisualContactSurfaceType = pAsset->getTriangleMaterial(rayTestResult.mTriangleIndex).mSurfaceTypeID;
			else
				wheel.mVisualContactSurfaceType = rayTestResult.mpRigidBody->getSurfaceType();

			wheel.mVisualContactCollisionGroup = rayTestResult.mpRigidBody->getCollisionGroup();
			wheel.mVisualContactExtendedFlags = rayTestResult.mpRigidBody->getExtendedFlags();
		}
		else
		{
			suspensionDist = VuMax(wheel.mVisualSuspensionDist - mVisualExtensionRate*fdt, suspensionDist);
		}

		wheel.mVisualSuspensionDist = suspensionDist;
		wheel.mVisualScaleZ = scaleZ;

		// update animation control
		if ( corner.mpAnimationControl )
		{
			corner.mTravel = (wheel.mVisualSuspensionDist - corner.mLowerLimit)/(corner.mUpperLimit - corner.mLowerLimit);
			float animTime = corner.mTravel*corner.mpAnimationControl->getAnimation()->getEndTime();
			corner.mpAnimationControl->setLocalTime(animTime);
		}

		// roate wheels
		wheel.mCurRotation += wheel.mCurAngVel*fdt;
	}

	mWheelContactCount = wheelContactCount;

	if ( avgContactNormal.magSquared() > FLT_EPSILON )
		mAvgContactNormal = avgContactNormal.normal();
	else
		mAvgContactNormal = VuVector3(0,0,1);

	// bounce sfx
	if ( mBounceSfx.length() )
	{
		mBounceSfxTimer -= fdt;
		if ( (prevWheelContactCount == 0) && (wheelContactCount > 0) && (mBounceSfxTimer <= 0.0f) )
		{
			VuAudioUtil::playSfx(mBounceSfx.c_str(), mpCar->getModelPosition());
			mBounceSfxTimer = mBounceSfxThreshold;
		}
	}
}

//*****************************************************************************
void VuCarSuspension::onTickBuild(float fdt)
{
	// update animation
	if ( mpAnimatedSkeleton )
	{
		mpAnimatedSkeleton->advance(fdt);
		mpAnimatedSkeleton->build();
	}

	mpModelInstance->setPose(mpAnimatedSkeleton);

	mpModelInstance->finalizePose();
}

//*****************************************************************************
void VuCarSuspension::onApplyForces(float fdt)
{
	VuRigidBody &rb = *mpCar->getRigidBody();

	// handle wheels
	VuVector3 vCenterOfMass = rb.getVuCenterOfMassPosition();
	VuMatrix matModel = rb.getVuCenterOfMassTransform();
	matModel.translateLocal(-mpCar->getCenterOfMass());

	float neutralSuspensionForce = mpCar->getMass()*(-VuDynamics::IF()->getGravity().mZ)/4;

	VUUINT32 rayTestMask = COL_ENGINE_STATIC_PROP|COL_ENGINE_DYNAMIC_PROP|COL_GAME_CAR;
	rayTestMask &= mpCar->getRigidBody()->getCollisionMask();

	int wheelContactCount = 0;
	VuVector3 avgContactNormal = VuVector3(0,0,0);
	int contactMask = 0;
	bool allWheelsInProximityOfGround = true;

	for ( int iWheel = 0; iWheel < 4; iWheel++ )
	{
		VuCarWheel &wheel = mpCar->getWheel(iWheel);
		VuCorner &corner = mCorners[iWheel];

		// init some outputs
		bool sideSlide = false;
		bool engineSlide = false;
		bool brakeSlide = false;

		VuVector3 attachmentPointWS = matModel.transform(wheel.mAttachmentPointLS);

		float wheelRadius = wheel.getRadius();
		float extraTestDist = 5.0f;

		// cast ray
		VuVector3 upperTestPosWS = attachmentPointWS + (corner.mUpperLimit)*matModel.getAxisZ();
		VuVector3 lowerTestPosWS = attachmentPointWS + (corner.mLowerLimit - wheelRadius - extraTestDist)*matModel.getAxisZ();

		VuSuspensionRayTestResult rayTestResult(rayTestMask, &rb);
		VuDynamicsRayTest::test(upperTestPosWS, lowerTestPosWS, rayTestResult);

		if ( rayTestResult.mbHasHit )
		{
			float totalTestDist = corner.mUpperLimit - corner.mLowerLimit + wheelRadius + extraTestDist;
			float resultDist = rayTestResult.mHitFraction*totalTestDist;
			if ( resultDist < totalTestDist - extraTestDist )
			{
				wheelContactCount++;
				avgContactNormal += rayTestResult.mHitNormal;
				contactMask |= 1<<iWheel;

				VuVector3 contactPointWS = VuLerp(upperTestPosWS, lowerTestPosWS, rayTestResult.mHitFraction);
				float suspensionDist = VuLerp(corner.mUpperLimit, corner.mLowerLimit - wheelRadius - extraTestDist, rayTestResult.mHitFraction) + wheelRadius;

				VuVector3 pointVel = VuDynamicsUtil::pointVelocityWorld(rb, contactPointWS);
				VuVector3 otherPointVel = VuDynamicsUtil::pointVelocityWorld(*rayTestResult.mpRigidBody, contactPointWS);
				VuVector3 contactVel = pointVel - otherPointVel;

				VuMatrix wheelTransform = matModel;
				wheelTransform.translateLocal(wheel.mAttachmentPointLS + VuVector3(0.0f, 0.0f, suspensionDist));
				//wheelTransform.rotateZLocal(wheel.mSteeringAngle);

				VuVector3 groundZ = rayTestResult.mHitNormal;
				VuVector3 groundX = (wheelTransform.getAxisX() - rayTestResult.mHitNormal*VuDot(rayTestResult.mHitNormal, wheelTransform.getAxisX())).normal();
				VuVector3 groundY = VuCross(groundZ, groundX);

				VUUINT8 surfaceType;
				if ( wheel.mSurfaceTypeOverride != 255 )
					surfaceType = wheel.mSurfaceTypeOverride;
				else if ( VuCollisionMeshAsset *pAsset = static_cast<VuCollisionMeshAsset *>(rayTestResult.mpRigidBody->getCollisionShape()->getUserPointer()) )
					surfaceType = pAsset->getTriangleMaterial(rayTestResult.mTriangleIndex).mSurfaceTypeID;
				else
					surfaceType = rayTestResult.mpRigidBody->getSurfaceType();

				float tractionFactor = mBaseTractionFactor*mTractionFactor*mInducedPowerSlideTractionFactor*mpCar->getAiBuffHandling();
				float surfaceFrictionCoeff = VuDynamics::IF()->getSurfaceFriction(surfaceType)*tractionFactor;
				float latFrictionCoeff = surfaceFrictionCoeff*wheel.mLatFrictionCoeff;
				float longFrictionCoeff = surfaceFrictionCoeff*wheel.mLongFrictionCoeff;

				// suspension force
				float springCoeff = VuSelect(suspensionDist, mUpperSpringCoeff, mLowerSpringCoeff)*mSpringFactor;
				float springForce = suspensionDist*springCoeff + neutralSuspensionForce;
				float dampingForce = VuMax(-VuDot(groundZ, contactVel)*mDampingCoeff*mDampingFactor, 0.0f);
				float suspensionImpulse = VuMax(springForce + dampingForce, 0.0f)*fdt;
				VuVector3 vSuspensionImpulse = groundZ*suspensionImpulse;

				// side force
				float sideImpulse = 0.0f;
				resolveSingleBilateral(rb,
					VuDynamicsUtil::toBtVector3(contactPointWS),
					*rayTestResult.mpRigidBody,
					VuDynamicsUtil::toBtVector3(contactPointWS),
					0.0f,
					VuDynamicsUtil::toBtVector3(groundX),
					sideImpulse,
					fdt);
				VuVector3 vSideImpulse = groundX*sideImpulse;

				// forward force
				float forwardForce = wheel.mEngineForce;
				float fwdVel = VuDot(contactVel, groundY);
				float brakingForce = VuSelect(fwdVel, -wheel.mBrakingForce, wheel.mBrakingForce);
				if ( VuAbs(fwdVel) < 0.0f )
					brakingForce *= VuAbs(fwdVel);
				VuVector3 vForwardImpulse = groundY*(forwardForce + brakingForce)*fdt;

				// traction
				//float maxTractionImpulse = combinedFrictionCoeff*suspensionImpulse;
				float maxLatTractionImpulse = latFrictionCoeff*neutralSuspensionForce*fdt;
				if ( vSideImpulse.magSquared() > maxLatTractionImpulse*maxLatTractionImpulse )
				{
					vSideImpulse *= maxLatTractionImpulse/vSideImpulse.mag();
					sideSlide = true;
				}
				float maxLongTractionImpulse = longFrictionCoeff*neutralSuspensionForce*fdt;
				if ( vForwardImpulse.magSquared() > maxLongTractionImpulse*maxLongTractionImpulse )
				{
					vForwardImpulse *= maxLongTractionImpulse/vForwardImpulse.mag();
					if ( forwardForce + brakingForce > 0.0f )
						engineSlide = true;
					else
						brakeSlide = true;
				}

				// suspension/forward impulse
				{
					VuVector3 adjContactPointWS = contactPointWS + matModel.getAxisZ()*VuDot(vCenterOfMass - contactPointWS, matModel.getAxisZ())*mWheelieResistance;
					VuDynamicsUtil::applyImpulseWorld(rb, vSuspensionImpulse + vForwardImpulse, adjContactPointWS);
				}

				// side impulse
				{
					VuVector3 adjContactPointWS = contactPointWS + matModel.getAxisZ()*VuDot(vCenterOfMass - contactPointWS, matModel.getAxisZ())*mRolloverResistance;
					VuDynamicsUtil::applyImpulseWorld(rb, vSideImpulse, adjContactPointWS);
				}

				// angular velocity
				wheel.mCurAngVel = -VuDot(groundY, contactVel)/wheelRadius;
			}
		}
		else
		{
			allWheelsInProximityOfGround = false;
		}

		wheel.mSideSlide = sideSlide;
		wheel.mEngineSlide = engineSlide;
		wheel.mBrakeSlide = brakeSlide;
	}

	if ( avgContactNormal.magSquared() > FLT_EPSILON )
		avgContactNormal = avgContactNormal.normal();
	else
		avgContactNormal = VuVector3(0,0,1);

	// power sliding
	if ( wheelContactCount )
	{
		const VuVector3 &vNormal = getAvgContactNormal();

		// determine velocity on ground plane
		VuVector3 vVel = mpCar->getLinearVelocity();
		vVel -= vNormal*VuDot(vVel, vNormal);

		// determine heading on ground plane
		VuVector3 vHeading = matModel.getAxisY();
		vHeading -= vNormal*VuDot(vHeading, vNormal);

		// transform velocity by 90 degrees
		VuVector3 vVelRight = VuCross(vVel, vNormal);
		if ( vVelRight.magSquared() > FLT_EPSILON )
		{
			float fForce = (mPowerSlideCoeff + mInducedPowerSlideCoeff)*VuDot(vHeading, vVelRight)/rb.getInvMass();
			VuVector3 vForce = fForce*vVelRight.normal();

			// apply force
			VuDynamicsUtil::applyCentralImpulse(rb, vForce*fdt);
		}
	}

	// self-righting
	if ( allWheelsInProximityOfGround && (wheelContactCount == 2 || wheelContactCount == 1) )
	{
		float dot = VuDot(avgContactNormal, matModel.getAxisZ());
		if ( dot < 0.707f )
		{
			float amount = VuLinStep(0.0f, 0.707f, dot);

			VuMatrix matModelInv = matModel;
			matModelInv.invert();
			VuVector3 vRate = matModelInv.transformNormal(mpCar->getAngularVelocity());

			#define FRONT_MASK ((1<<VuCarWheel::FRONT_LEFT) | (1<<VuCarWheel::FRONT_RIGHT))
			#define REAR_MASK ((1<<VuCarWheel::REAR_LEFT) | (1<<VuCarWheel::REAR_RIGHT))
			#define LEFT_MASK ((1<<VuCarWheel::FRONT_LEFT) | (1<<VuCarWheel::REAR_LEFT))
			#define RIGHT_MASK ((1<<VuCarWheel::FRONT_RIGHT) | (1<<VuCarWheel::REAR_RIGHT))

			if ( (contactMask & FRONT_MASK) == 0 )
				vRate.mX = VuMin(vRate.mX, -amount*VuDegreesToRadians(180.0f));

			if ( (contactMask & REAR_MASK) == 0 )
				vRate.mX = VuMax(vRate.mX, amount*VuDegreesToRadians(180.0f));

			if ( (contactMask & LEFT_MASK) == 0 )
				vRate.mY = VuMin(vRate.mY, -amount*VuDegreesToRadians(180.0f));

			if ( (contactMask & RIGHT_MASK) == 0 )
				vRate.mY = VuMax(vRate.mY, amount*VuDegreesToRadians(180.0f));
			
			rb.setAngularVelocity(VuDynamicsUtil::toBtVector3(matModel.transformNormal(vRate)));
		}
	}
}

//*****************************************************************************
void VuCarSuspension::drawDebug(const VuCamera &camera)
{
	const VuMatrix &matModel = mpCar->getModelMatrix();

	for ( int iWheel= 0; iWheel < 4; iWheel++ )
	{
		VuCarWheel &wheel = mpCar->getWheel(iWheel);
		VuCorner &corner = mCorners[iWheel];

		VuMatrix transform = matModel;
		transform.translateLocal(wheel.mAttachmentPointLS + VuVector3(0.0f, 0.0f, corner.mUpperLimit));

		VuGfxUtil::IF()->drawSphereLines(VuColor(255,255,0), 0.25f*wheel.getRadius(), 8, 8, transform*camera.getViewProjMatrix());
	}
}

//*****************************************************************************
void VuCarSuspension::draw(const VuGfxDrawParams &params, const VuColor &color, float dist, VUUINT32 dynamicLightGroupMask)
{
	const VuMatrix &modelMat = mpCar->getModelMatrix();

	mpModelInstance->setColor(color);
	mpModelInstance->setDynamicLightGroupMask(dynamicLightGroupMask);
	mpModelInstance->draw(modelMat, params);
}

//*****************************************************************************
void VuCarSuspension::drawPrefetch()
{
	mpModelInstance->drawPrefetch();
}

//*****************************************************************************
void VuCarSuspension::drawShadow(const VuGfxDrawShadowParams &params, float dist)
{
	const VuMatrix &modelMat = mpCar->getModelMatrix();

	mpModelInstance->drawShadow(modelMat, params);
}

//*****************************************************************************
void VuCarSuspension::drawDropShadow(const VuGfxDrawShadowParams &params, float dist)
{
	const VuMatrix &modelMat = mpCar->getModelMatrix();

	mpModelInstance->drawDropShadow(modelMat, params);
}

//*****************************************************************************
void VuCarSuspension::clear()
{
	if ( mpAnimatedSkeleton )
	{
		mpAnimatedSkeleton->removeRef();
		mpAnimatedSkeleton = VUNULL;
	}

	for ( int i = 0; i < 4; i++ )
	{
		VuCorner &corner = mCorners[i];

		if ( corner.mpAnimationControl )
		{
			corner.mpAnimationControl->removeRef();
			corner.mpAnimationControl = VUNULL;
		}
	}
}
