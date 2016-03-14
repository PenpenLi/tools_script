//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Game-implementation of dynamic prop entity
// 
//*****************************************************************************

#include "VuPurple/Dynamics/VuCollisionTypes.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/Transform/VuTransformComponent.h"
#include "VuEngine/Components/RigidBody/VuRigidBodyComponent.h"
#include "VuEngine/Components/3dDraw/Vu3dDrawStaticModelComponent.h"
#include "VuEngine/Components/3dLayout/Vu3dLayoutComponent.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Properties/VuBasicProperty.h"
#include "VuEngine/Properties/VuAudioProperty.h"
#include "VuEngine/Properties/VuDBEntryProperty.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Water/VuWaterUtil.h"
#include "VuEngine/Math/VuQuaternion.h"
#include "VuEngine/Math/VuMathUtil.h"
#include "VuEngine/Math/VuUnitConversion.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/HAL/Audio/VuAudio.h"
#include "VuEngine/Pfx/VuPfx.h"
#include "VuEngine/Pfx/VuPfxManager.h"
#include "VuEngine/Pfx/VuPfxEntity.h"


// constants

#define REPEAT_TIME 1.0f // seconds
#define SHADOW_TEST_DIST 10.0f // m
#define TOUGH_CAR_COLLISION_ADVANTAGE 2.0f


class VuDynamicGamePropEntity : public VuEntity, btMotionState, VuDynamicsStepCallback, VuRigidBodyContactCallback
{
	DECLARE_RTTI
public:
	VuDynamicGamePropEntity();

	virtual void	onPostLoad();
	virtual void	onGameInitialize();
	virtual void	onGameRelease();

protected:
	// btMotionState
	virtual void	getWorldTransform(btTransform& worldTrans) const;
	virtual void	setWorldTransform(const btTransform& worldTrans);

	// scripting
	VuRetVal		Show(const VuParams &params)	{ show(); return VuRetVal(); }
	VuRetVal		Hide(const VuParams &params)	{ hide(); return VuRetVal(); }

	void			show();
	void			hide();

	void			drawLayout(const Vu3dLayoutDrawParams &params);
	bool			collideLayout(const VuVector3 &v0, VuVector3 &v1);
	void			transformModified();
	void			massModified();
	void			typeModified();

	void			tickBuild(float fdt);

	// event handlers
	void			OnDeactivate(const VuParams &params) { hide(); }
	void			OnHitByMissile(const VuParams &params);

	// VuDynamicsStepCallback
	virtual void	onDynamicsApplyForces(float fdt);

	// VuRigidBodyContactCallback
	virtual bool	onRigidBodyContactAdded(VuContactPoint &cp);

	void			tickDecision(float fdt);

	// components
	Vu3dLayoutComponent				*mp3dLayoutComponent;
	VuScriptComponent				*mpScriptComponent;
	Vu3dDrawStaticModelComponent	*mp3dDrawStaticModelComponent;
	VuRigidBodyComponent			*mpRigidBodyComponent;

	// properties
	bool			mbInitiallyVisible;
	float			mMass;
	VuVector3		mCenterOfMass;
	bool			mCollideWithStaticProps;
	std::string		mType;
	float			mFluidsRadius;
	float			mFluidsDensity;
	float			mFluidsDragCoeff;
	float			mFluidsLiftCoeff;
	VuVector3		mHydroCenter;
	bool			mLinearAnchor;
	bool			mAngularAnchor;
	float			mAnchorSpringCoeff;
	float			mAnchorDampingCoeff;
	bool			mHardHit;
	std::string		mSfxName;
	bool			mThrowDriver;
	std::string		mSplashPfxName;
	float			mSplashSpeed;
	bool			mIsTough;
	bool			mCollisionLighting;

	VuDBEntryProperty	*mpTypeProperty;
	float				mThresholdSpeed;
	float				mCamShakeMagnitude;
	float				mCamShakeDuration;
	float				mCamShakeFalloffTime;
	float				mCamShakeFrequency;

	bool				mbVisible;
	VuVector3			mAnchorPos;
	VuQuaternion		mAnchorQuat;
	bool				mHardHitNow;
	VuWeakRef<VuEntity>	mHardHitEntity;
	float				mTimer;
	bool				mPrevSubmerged;
	bool				mPlaySplashPfx;
	bool				mHitByMissileOrExplosionThisFrame;
	bool				mbDeactivateNow;
};


IMPLEMENT_RTTI(VuDynamicGamePropEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuDynamicGamePropEntity);


//*****************************************************************************
VuDynamicGamePropEntity::VuDynamicGamePropEntity():
	mbInitiallyVisible(true),
	mMass(100.0f),
	mCenterOfMass(0,0,0),
	mCollideWithStaticProps(true),
	mFluidsRadius(0.0f),
	mFluidsDensity(0.5f),
	mFluidsDragCoeff(SPHERE_DRAG_COEFFICIENT),
	mFluidsLiftCoeff(0.2f),
	mHydroCenter(0,0,0),
	mLinearAnchor(false),
	mAngularAnchor(false),
	mAnchorSpringCoeff(1.0f),
	mAnchorDampingCoeff(1.0f),
	mbVisible(false),
	mAnchorPos(0,0,0),
	mAnchorQuat(VuQuaternion::identity()),
	mHardHit(false),
	mThrowDriver(false),
	mSplashSpeed(20.0f),
	mIsTough(false),
	mCollisionLighting(false),
	mHardHitNow(false),
	mTimer(0.0f),
	mPrevSubmerged(true),
	mPlaySplashPfx(false),
	mHitByMissileOrExplosionThisFrame(false),
	mbDeactivateNow(false)
{
	// components
	addComponent(mp3dLayoutComponent = new Vu3dLayoutComponent(this));
	addComponent(mpScriptComponent = new VuScriptComponent(this, 150, false));
	addComponent(mp3dDrawStaticModelComponent = new Vu3dDrawStaticModelComponent(this));
	addComponent(mpRigidBodyComponent = new VuRigidBodyComponent(this));

	mp3dLayoutComponent->setDrawMethod(this, &VuDynamicGamePropEntity::drawLayout);
	mp3dLayoutComponent->setCollideMethod(this, &VuDynamicGamePropEntity::collideLayout);

	// want to know when transform is changed
	mpTransformComponent->setWatcher(&VuDynamicGamePropEntity::transformModified);

	// properties
	addProperty(new VuBoolProperty("Initially Visible", mbInitiallyVisible));
	addProperty(new VuFloatProperty("Mass", mMass)) -> setWatcher(this, &VuDynamicGamePropEntity::massModified);
	addProperty(new VuVector3Property("Center of Mass", mCenterOfMass)) -> setWatcher(this, &VuDynamicGamePropEntity::massModified);
	addProperty(new VuBoolProperty("Collide With Static Props", mCollideWithStaticProps));
	addProperty(mpTypeProperty = new VuDBEntryProperty("Type", mType, "PropDB")) -> setWatcher(this, &VuDynamicGamePropEntity::typeModified);
	addProperty(new VuFloatProperty("Fluids Radius", mFluidsRadius));
	addProperty(new VuFloatProperty("Fluids Density", mFluidsDensity));
	addProperty(new VuFloatProperty("Fluids Drag Coeff", mFluidsDragCoeff));
	addProperty(new VuFloatProperty("Fluids Lift Coeff", mFluidsLiftCoeff));
	addProperty(new VuVector3Property("Hydrodynamic Center", mHydroCenter));
	addProperty(new VuBoolProperty("Linear Anchor", mLinearAnchor));
	addProperty(new VuBoolProperty("Angular Anchor", mAngularAnchor));
	addProperty(new VuFloatProperty("Anchor Spring Coeff", mAnchorSpringCoeff));
	addProperty(new VuFloatProperty("Anchor Damping Coeff", mAnchorDampingCoeff));
	addProperty(new VuBoolProperty("Hard Hit", mHardHit));
	addProperty(new VuAudioEventNameProperty("Sfx Name", mSfxName));
	addProperty(new VuBoolProperty("Throw Driver", mThrowDriver));
	addProperty(new VuStringProperty("Splash Pfx Name", mSplashPfxName));
	addProperty(new VuFloatProperty("Splash Speed", mSplashSpeed));
	addProperty(new VuBoolProperty("Is Tough", mIsTough));
	addProperty(new VuBoolProperty("Collision Lighting", mCollisionLighting));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuDynamicGamePropEntity, Show);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuDynamicGamePropEntity, Hide);

	// event handlers
	REG_EVENT_HANDLER(VuDynamicGamePropEntity, OnDeactivate);
	REG_EVENT_HANDLER(VuDynamicGamePropEntity, OnHitByMissile);
}

//*****************************************************************************
void VuDynamicGamePropEntity::onPostLoad()
{
	transformModified();
	massModified();
	typeModified();
}

//*****************************************************************************
void VuDynamicGamePropEntity::onGameInitialize()
{
	mpRigidBodyComponent->setMass(mMass);
	mpRigidBodyComponent->setMotionState(this);
	mpRigidBodyComponent->setCollisionGroup(COL_ENGINE_DYNAMIC_PROP);
	mpRigidBodyComponent->setCollisionMask(mCollideWithStaticProps ? COL_EVERYTHING : ~COL_ENGINE_STATIC_PROP);
	mpRigidBodyComponent->setContactCallback(this);
	mpRigidBodyComponent->setExtendedFlags(mpRigidBodyComponent->getExtendedFlags() | EXT_COL_ENGINE_REACT_TO_EXPLOSIONS);

	if ( mThrowDriver )
		mpRigidBodyComponent->setExtendedFlags(mpRigidBodyComponent->getExtendedFlags() | EXT_COL_GAME_THROW_DRIVER);

	if ( mIsTough )
		mpRigidBodyComponent->setExtendedFlags(mpRigidBodyComponent->getExtendedFlags() | EXT_COL_GAME_TOUGH);

	mpRigidBodyComponent->createRigidBody();
	VUASSERT(mpRigidBodyComponent->getRigidBody(), "Dynamic Prop without collision!");

	if ( mbInitiallyVisible )
		show();

	// register phased ticks
	VuTickManager::IF()->registerHandler(this, &VuDynamicGamePropEntity::tickDecision, "Decision");
	VuTickManager::IF()->registerHandler(this, &VuDynamicGamePropEntity::tickBuild, "Build");
}

//*****************************************************************************
void VuDynamicGamePropEntity::onGameRelease()
{
	// unregister phased ticks
	VuTickManager::IF()->unregisterHandlers(this);

	hide();

	mpRigidBodyComponent->destroyRigidBody();
}

//*****************************************************************************
void VuDynamicGamePropEntity::getWorldTransform(btTransform& worldTrans) const
{
	VuMatrix matRB = mpTransformComponent->getWorldTransform();
	matRB.translateLocal(mpRigidBodyComponent->getCenterOfMass());

	worldTrans = VuDynamicsUtil::toBtTransform(matRB);
}

//*****************************************************************************
void VuDynamicGamePropEntity::setWorldTransform(const btTransform& worldTrans)
{
	VuMatrix matModel = VuDynamicsUtil::toVuMatrix(worldTrans);
	matModel.translateLocal(-mpRigidBodyComponent->getCenterOfMass());

	mpTransformComponent->setWorldTransform(matModel, false);
}

//*****************************************************************************
void VuDynamicGamePropEntity::show()
{
	if ( !mbVisible )
	{
		mbVisible = true;

		mp3dDrawStaticModelComponent->show();

		mpRigidBodyComponent->addToWorld();

		if ( mFluidsRadius > 0.0f || mLinearAnchor || mAngularAnchor )
			VuDynamics::IF()->registerStepCallback(this);

		mAnchorPos = mpTransformComponent->getWorldPosition();
		mAnchorQuat.fromRotationMatrix(mpTransformComponent->getWorldTransform());
	}
}

//*****************************************************************************
void VuDynamicGamePropEntity::hide()
{
	if ( mbVisible )
	{
		mbVisible = false;

		mp3dDrawStaticModelComponent->hide();

		mpRigidBodyComponent->removeFromWorld();

		VuDynamics::IF()->unregisterStepCallback(this);
	}
}

//*****************************************************************************
void VuDynamicGamePropEntity::drawLayout(const Vu3dLayoutDrawParams &params)
{
	if ( params.mbDrawCollision )
	{
		mpRigidBodyComponent->draw(VuColor(255,255,255), params.mCamera);
	}
	else
	{
		mp3dDrawStaticModelComponent->drawLayout(params);
	}
}

//*****************************************************************************
bool VuDynamicGamePropEntity::collideLayout(const VuVector3 &v0, VuVector3 &v1)
{
	return mp3dDrawStaticModelComponent->collideLayout(v0, v1);
}

//*****************************************************************************
void VuDynamicGamePropEntity::transformModified()
{
	VuMatrix mat = mpTransformComponent->getWorldTransform();
	mat.scaleLocal(mpTransformComponent->getWorldScale());
	mp3dDrawStaticModelComponent->updateVisibility(mat);

	mpRigidBodyComponent->transformModified(mpTransformComponent->getWorldTransform());
	mpRigidBodyComponent->scaleModified(mpTransformComponent->getWorldScale());
}

//*****************************************************************************
void VuDynamicGamePropEntity::massModified()
{
	mpRigidBodyComponent->setMass(mMass);
	mpRigidBodyComponent->setCenterOfMass(mCenterOfMass);
}

//*****************************************************************************
void VuDynamicGamePropEntity::typeModified()
{
	// get data from db
	const VuJsonContainer &data = mpTypeProperty->getEntryData();
	mThresholdSpeed = VuMphToMetersPerSecond(data["Threshold Speed"].asFloat());

	const VuJsonContainer &cameraShakeData = data["Camera Shake"];
	mCamShakeMagnitude = cameraShakeData["Magnitude"].asFloat();
	mCamShakeDuration = cameraShakeData["Duration"].asFloat();
	mCamShakeFalloffTime = cameraShakeData["Falloff Time"].asFloat();
	mCamShakeFrequency = cameraShakeData["Frequency"].asFloat();
}

//*****************************************************************************
void VuDynamicGamePropEntity::OnHitByMissile(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	VuVector3 missilePos = accessor.getVector3();
	VuVector3 missileVel = accessor.getVector3();
	const char *carEffect = accessor.getString();
	VuEntity *pOriginator = accessor.getEntity();

	if ( !mHitByMissileOrExplosionThisFrame )
	{
		mHitByMissileOrExplosionThisFrame = true;

		VuVector3 linVel = missileVel;
		linVel.mZ = VuMax(VuAbs(linVel.mZ), linVel.mag2d());
		linVel.normalize();

		linVel *= 0.25f*missileVel.mag();

		linVel += mpRigidBodyComponent->getRigidBody()->getVuLinearVelocity();
		mpRigidBodyComponent->getRigidBody()->setVuLinearVelocity(linVel);

		mpRigidBodyComponent->getRigidBody()->activate();
	}
}

//*****************************************************************************
void VuDynamicGamePropEntity::onDynamicsApplyForces(float fdt)
{
	VuRigidBody *pRigidBody = mpRigidBodyComponent->getRigidBody();
	VuMatrix xform = pRigidBody->getVuCenterOfMassTransform();
	VuVector3 pos = xform.transform(-mpRigidBodyComponent->getCenterOfMass());

	if ( mFluidsRadius > 0.0f )
	{
		VuWaterUtil::VuEstimateSphereForcesParams params;
		params.mpRigidBody = mpRigidBodyComponent->getRigidBody();
		params.mPosition = pos;
		params.mMass = mMass;
		params.mRadius = mFluidsRadius;
		params.mBuoyancy = mFluidsDensity;
		params.mDragCoeff = mFluidsDragCoeff;
		params.mLiftCoeff = mFluidsLiftCoeff;

		VuWaterUtil::estimateSphereForces(params);

		VuVector3 forcePos = xform.transform(mHydroCenter - mpRigidBodyComponent->getCenterOfMass());
		VuDynamicsUtil::applyForceWorld(*pRigidBody, params.mTotalForce, forcePos);

		if ( params.mSubmerged && !mPrevSubmerged )
		{
			VuVector3 linVel = params.mpRigidBody->getVuLinearVelocity() - params.mWaterVel;
			if ( linVel.mag() > VuMphToMetersPerSecond(mSplashSpeed) )
				mPlaySplashPfx = true;
		}
		mPrevSubmerged = params.mSubmerged;
	}

	// anchor force
	if ( mLinearAnchor )
	{
		const VuVector3 &linVel = pRigidBody->getVuLinearVelocity();
		VuVector3 anchorForce(0,0,0);

		// spring
		VuVector3 delta = mAnchorPos - pos;
		anchorForce += mAnchorSpringCoeff*mMass*VuVector3(delta.mX, delta.mY, 0);

		// damping
		anchorForce -= mAnchorDampingCoeff*mMass*VuVector3(linVel.mX, linVel.mY, 0);

		// apply
		pRigidBody->applyCentralForce(VuDynamicsUtil::toBtVector3(anchorForce));
	}

	// anchor torque
	if ( mAngularAnchor )
	{
		VuQuaternion quat;
		quat.fromRotationMatrix(xform);

		VuQuaternion deltaQuat = VuMathUtil::rotationDelta(quat, mAnchorQuat);
		VuVector3 deltaAxis;
		float deltaAngle;
		deltaQuat.toAxisAngle(deltaAxis, deltaAngle);

		VuVector3 deltaRot = -deltaAngle*deltaAxis;

		// spring
		deltaRot = xform.transformNormal(deltaRot);
		VuVector3 anchorTorque = mAnchorSpringCoeff*deltaRot;

		// damping
		VuVector3 deltaRotVel = -pRigidBody->getVuAngularVelocity();
		anchorTorque += mAnchorDampingCoeff*deltaRotVel;

		// apply
		btMatrix3x3 inertiaTensor = pRigidBody->getInvInertiaTensorWorld().inverse();
		pRigidBody->applyTorque(VuDynamicsUtil::toBtVector3(anchorTorque)*inertiaTensor);
	}
}

//*****************************************************************************
bool VuDynamicGamePropEntity::onRigidBodyContactAdded(VuContactPoint &cp)
{
	VUINT16 otherCollisionGroup = cp.mpOtherBody->getCollisionGroup();

	if ( otherCollisionGroup & COL_GAME_CAR )
	{
		VuRigidBody *pRigidBody = mpRigidBodyComponent->getRigidBody();

		// check for hard hit
		if ( cp.mpOtherBody->hasContactResponse() && mTimer >= REPEAT_TIME )
		{
			// compute relative velocity
			VuVector3 vRelV = pRigidBody->getVuLinearVelocity() - cp.mpOtherBody->getVuLinearVelocity();
			float fRelVel = VuDot(cp.mNorWorld, vRelV);
			if ( fRelVel < -mThresholdSpeed )
			{
				mHardHitEntity = cp.mpOtherBody->getEntity();
				mHardHitNow = true;
				mTimer = 0.0f;
			}
		}

		// tough car?
		if ( cp.mpOtherBody->getExtendedFlags() & EXT_COL_GAME_TOUGH )
		{
			float fImpulse = VuDynamicsUtil::collisionImpulse(pRigidBody, cp.mpOtherBody, cp.mPosWorld, cp.mNorWorld);
			VuVector3 vImpulse = (TOUGH_CAR_COLLISION_ADVANTAGE*fImpulse)*cp.mNorWorld;
			VuDynamicsUtil::applyImpulseWorld(*pRigidBody, vImpulse, cp.mPosWorld);
		}
	}

	if ( otherCollisionGroup & COL_GAME_DEACTIVATION )
		mbDeactivateNow = true;

	return true;
}

//*****************************************************************************
void VuDynamicGamePropEntity::tickDecision(float fdt)
{
	mTimer += fdt;

	if ( mbDeactivateNow )
	{
		hide();
		mbDeactivateNow = false;
	}

	if ( mHardHitNow )
	{
		// camera shake
		if ( mHardHitEntity )
		{
			VuParams params;
			params.addFloat(mCamShakeMagnitude);
			params.addFloat(mCamShakeDuration);
			params.addFloat(mCamShakeFalloffTime);
			params.addFloat(mCamShakeFrequency);
			mHardHitEntity->handleEvent("OnCameraShake", params);
		}

		// sfx
		#if !VU_DISABLE_AUDIO
		if ( mSfxName.length() )
		{
			FMOD::Event *pEvent;
			if ( VuAudio::IF()->eventSystem()->getEvent(mSfxName.c_str(), FMOD_EVENT_NONBLOCKING, &pEvent) == FMOD_OK )
			{
				FMOD_VECTOR pos = VuAudio::toFmodVector(mpTransformComponent->getWorldPosition());
				pEvent->set3DAttributes(&pos, VUNULL, VUNULL);
				pEvent->start();
			}
		}
		#endif

		mHardHitNow = false;
		mHardHitEntity = VUNULL;
	}

	// pfx
	if ( mPlaySplashPfx )
	{
		mPlaySplashPfx = false;

		// create effect
		if ( VUUINT32 hPfx = VuPfxManager::IF()->createEntity(mSplashPfxName.c_str(), true) )
		{
			if ( VuPfxEntity *pPfxEntity = VuPfxManager::IF()->getEntity(hPfx) )
			{
				pPfxEntity->getSystemInstance()->setPosition(mpTransformComponent->getWorldPosition());
				pPfxEntity->getSystemInstance()->start();
			}
		}
	}

	// collision lighting
	if ( mCollisionLighting )
	{
		float shadowValue;
		if ( VuDynamicsUtil::getShadowValue(mpTransformComponent->getWorldPosition(), VuVector3(0.0f, 0.0f, -SHADOW_TEST_DIST), shadowValue) )
			mp3dDrawStaticModelComponent->setShadowValue(shadowValue);
	}

	mHitByMissileOrExplosionThisFrame = false;

	if (mbVisible
		&& mpTransformComponent
		&& mpTransformComponent->getWorldTransform().getTrans().mZ < -1000.0f)
	{
		VUPRINTF("VuDynamicGamePropEntity %08lx falling out of world.\n", (VUUINT32)this);
		hide();
	}
}

//*****************************************************************************
void VuDynamicGamePropEntity::tickBuild(float fdt)
{
	// update visibility
	{
		VuMatrix mat = mpTransformComponent->getWorldTransform();
		mat.scaleLocal(mpTransformComponent->getWorldScale());
		mp3dDrawStaticModelComponent->updateVisibility(mat);
	}
}