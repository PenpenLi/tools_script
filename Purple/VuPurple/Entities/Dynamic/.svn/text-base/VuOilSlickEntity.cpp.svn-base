//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuOilSlickEntity
// 
//*****************************************************************************

#include "VuOilSlickEntity.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarSuspension.h"
#include "VuPurple/Entities/Car/Parts/VuCarEffectController.h"
#include "VuPurple/Dynamics/VuCollisionTypes.h"
#include "VuEngine/Entities/VuEntityRepository.h"
#include "VuEngine/Components/3dDraw/Vu3dDrawComponent.h"
#include "VuEngine/Assets/VuAssetFactory.h"
#include "VuEngine/Assets/VuCollisionMeshAsset.h"
#include "VuEngine/Assets/VuMaterialAsset.h"
#include "VuEngine/Dynamics/Util/VuDynamicsRayTest.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Gfx/GfxSort/VuGfxSort.h"
#include "VuEngine/HAL/Gfx/VuGfx.h"
#include "VuEngine/Math/VuMathUtil.h"
#include "VuEngine/Util/VuDataUtil.h"


// constants
#define OIL_SLICK_TEST_DIST 5.0f // m
#define SELF_COLLISION_TIME 5.0f

IMPLEMENT_RTTI(VuOilSlickEntity, VuEntity);


//*****************************************************************************
class VuOilSlickRayTestResult : public VuDynamicsRayTest::VuClosestResult
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
class VuOilSlickConvexResult : public btCollisionWorld::ConvexResultCallback
{
public:
	typedef std::deque<VuVector3> Verts;

	VuOilSlickConvexResult(Verts &verts) : mVerts(verts), mHitFraction(1.0f) {}

	virtual bool needsCollision(btBroadphaseProxy *proxy0) const
	{
		const btCollisionObject *pColObj = static_cast<const btCollisionObject *>(proxy0->m_clientObject);
		if ( pColObj->getInternalType() != btCollisionObject::CO_RIGID_BODY )
			return false;

		if ( !pColObj->getCollisionShape()->getUserPointer() )
			return false;

		const VuRigidBody *pRigidBody = static_cast<const VuRigidBody *>(pColObj);
		if ( pRigidBody->getExtendedFlags() & EXT_COL_ENGINE_BREAKABLE )
			return false;

		return true;
	}

	virtual	btScalar addSingleResult(btCollisionWorld::LocalConvexResult &convexResult, bool normalInWorldSpace)
	{
		const VuRigidBody *pRigidBody = static_cast<const VuRigidBody *>(convexResult.m_hitCollisionObject);
		const VuCollisionMeshAsset *pAsset = static_cast<const VuCollisionMeshAsset *>(pRigidBody->getCollisionShape()->getUserPointer());

		int triIndex = convexResult.m_localShapeInfo->m_triangleIndex;

		VuMatrix transform = pRigidBody->getVuCenterOfMassTransform();

		VuVector3 v0 = pAsset->getVert(pAsset->getIndex(triIndex*3 + 0));
		VuVector3 v1 = pAsset->getVert(pAsset->getIndex(triIndex*3 + 1));
		VuVector3 v2 = pAsset->getVert(pAsset->getIndex(triIndex*3 + 2));

		if ( VuCross(v1 - v0, v2 - v0).mZ > 0.0f )
		{
			mVerts.push_back(transform.transform(v0));
			mVerts.push_back(transform.transform(v1));
			mVerts.push_back(transform.transform(v2));

			mHitFraction = VuMin(mHitFraction, convexResult.m_hitFraction);
		}

		return 1.0f;
	}

	Verts	&mVerts;
	float	mHitFraction;
};

//*****************************************************************************
class VuOilSlickRigidBody : public VuRigidBody
{
public:
	VuOilSlickRigidBody(const btRigidBody::btRigidBodyConstructionInfo &info, VuEntity *pEntity, VUINT16 collisionGroup, VUINT16 collisionMask):
		VuRigidBody(info, pEntity, collisionGroup, collisionMask),
		mpIgnoreBody(VUNULL)
	{
		m_checkCollideWith = true;
	}

	virtual bool checkCollideWithOverride(const  btCollisionObject* co) const
	{
		const VuRigidBody *pOtherBody = static_cast<const VuRigidBody *>(co);
		if ( pOtherBody == mpIgnoreBody )
			return false;

		return true;
	}

	VuRigidBody	*mpIgnoreBody;
};

//*****************************************************************************
VuOilSlickEntity *VuOilSlickEntity::drop(const VuJsonContainer &data, VuCarEntity *pCar)
{
	VuOilSlickEntity *pOilSlick = VUNULL;

	// first do a line test straight down
	VuOilSlickRayTestResult rayTestResult;
	VuVector3 pos = pCar->getModelMatrix().transform(pCar->getCollisionAabb().getCenter());
	VuVector3 ray(0.0f, 0.0f, -OIL_SLICK_TEST_DIST);
	VuDynamicsRayTest::test(pos, pos + ray, rayTestResult);
	if ( rayTestResult.mbHasHit )
	{
		pos = pos + ray*rayTestResult.mHitFraction;

		// now do a sphere test near the ground
		VuMatrix transform;
		VuMathUtil::buildOrientationMatrix(pCar->getModelMatrix().getAxisY(), VuVector3(0.0f, 0.0f, 1.0f), transform);
		transform.setTrans(pos);

		VuVector2 size(0,0);
		VuDataUtil::getValue(data["Size"], size);
		float radius = (0.5f*size).mag();
		btSphereShape sphereShape(radius);

		btTransform from = VuDynamicsUtil::toBtTransform(transform);
		btTransform to = from;
		from.getOrigin().setZ(transform.getTrans().mZ + radius);
		to.getOrigin().setZ(transform.getTrans().mZ - radius);

		std::deque<VuVector3> verts;
		VuOilSlickConvexResult result(verts);
		result.m_collisionFilterMask = COL_ENGINE_STATIC_PROP;

		VuDynamics::IF()->getDynamicsWorld()->convexSweepTest(&sphereShape, from, to, result);

		if ( verts.size() )
		{
			pOilSlick = new VuOilSlickEntity(data, pCar, transform, 0.5f*radius, verts);
		}
	}

	return pOilSlick;
}

//*****************************************************************************
VuOilSlickEntity::VuOilSlickEntity(const VuJsonContainer &data, VuCarEntity *pCar, const VuMatrix &transform, float radius, const std::deque<VuVector3> &verts):
	mpLauncher(pCar),
	mTransform(transform),
	mRadius(radius),
	mAge(0.0f),
	mAddedToWorld(false),
	mRunOver(false)
{
	// components
	addComponent(mp3dDrawComponent = new Vu3dDrawComponent(this));

	mp3dDrawComponent->setDrawMethod(this, &VuOilSlickEntity::draw);

	// collision shape
	btCollisionShape *pShape = new btSphereShape(radius);

	// rigid body
	btRigidBody::btRigidBodyConstructionInfo info(0.0f, VUNULL, pShape);
	info.m_startWorldTransform.setIdentity();
	info.m_startWorldTransform.setOrigin(VuDynamicsUtil::toBtVector3(transform.getTrans() + VuVector3(0.0f, 0.0f, radius)));
	mpRigidBody = new VuOilSlickRigidBody(info, this, COL_GAME_OIL_SLICK, COL_GAME_CAR);
	mpRigidBody->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	mpRigidBody->setContactCallback(this);
	mpRigidBody->mpIgnoreBody = pCar->getRigidBody();

	VuColor ambientTint(255,255,255);
	VuColor diffuseTint(255,255,255);
	VuDataUtil::getValue(data["AmbientTint"], ambientTint);
	VuDataUtil::getValue(data["DiffuseTint"], diffuseTint);
	VuColor tint = VuLerp(ambientTint, diffuseTint, pCar->getShadowValue());

	mCarEffect = data["CarEffect"].asCString();;
	mpMaterialAsset = VuAssetFactory::IF()->createAsset<VuMaterialAsset>(data["Material"].asString());
	mDrawDist = data["DrawDist"].asFloat();

	// verts
	VuVector2 size(0,0);
	VuDataUtil::getValue(data["Size"], size);
	float offsetZ = data["OffsetZ"].asFloat();

	VuVector2 texAxisX(transform.getAxisX().mX, transform.getAxisX().mY);
	VuVector2 texAxisY(transform.getAxisY().mX, transform.getAxisY().mY);
	VuVector2 texSize(0.5f*size);

	VuVector3 pos = mTransform.getTrans();

	mVerts.resize(verts.size());
	VuOilSlickVertex *pVert = &mVerts[0];
	for ( const auto &rawVert : verts )
	{
		pVert->mXyz[0] = rawVert.mX;
		pVert->mXyz[1] = rawVert.mY;
		pVert->mXyz[2] = rawVert.mZ + offsetZ;

		VuVector2 texDelta(rawVert.mX - pos.mX, rawVert.mY - pos.mY);
		float texCoordX = VuDot(texAxisX, texDelta)/texSize.mX;
		float texCoordY = VuDot(texAxisY, texDelta)/texSize.mY;
		texCoordX = 0.5f + texCoordX*0.5f;
		texCoordY = 0.5f + texCoordY*0.5f;

		pVert->mUv[0] = texCoordX;
		pVert->mUv[1] = texCoordY;

		pVert->mColor = tint;

		pVert++;
	}
}

//*****************************************************************************
VuOilSlickEntity::~VuOilSlickEntity()
{
	delete mpRigidBody->getCollisionShape();
	delete mpRigidBody;

	VuAssetFactory::IF()->releaseAsset(mpMaterialAsset);
}

//*****************************************************************************
void VuOilSlickEntity::onGameInitialize()
{
	mpTransformComponent->setWorldTransform(mTransform);

	mp3dDrawComponent->updateVisibility(VuAabb(mTransform.getTrans(), mRadius));
	mp3dDrawComponent->show();

	addToWorld();

	// register phased ticks
	VuTickManager::IF()->registerHandler(this, &VuOilSlickEntity::tickDecision, "Decision");
}

//*****************************************************************************
void VuOilSlickEntity::onGameRelease()
{
	mp3dDrawComponent->hide();

	removeFromWorld();

	// unregister phased tick
	VuTickManager::IF()->unregisterHandlers(this);
}

//*****************************************************************************
bool VuOilSlickEntity::onRigidBodyContactAdded(VuContactPoint &cp)
{
	// check if we should break
	if ( cp.mpOtherBody->hasContactResponse() && !mRunOver )
	{
		VuEntity *pOtherEntity = cp.mpOtherBody->getEntity();
		if ( pOtherEntity && pOtherEntity->isDerivedFrom(VuCarEntity::msRTTI) )
		{
			VuCarEntity *pCar = static_cast<VuCarEntity *>(pOtherEntity);
			if ( pCar->getSuspension()->getWheelContactCount() )
			{
				mRunOverEntity = pCar;
				mRunOver = true;
			}
		}
	}

	return false;
}

//*****************************************************************************
void VuOilSlickEntity::addToWorld()
{
	if ( !mAddedToWorld )
	{
		VuDynamics::IF()->addRigidBody(mpRigidBody);
		mAddedToWorld = true;
	}
}

//*****************************************************************************
void VuOilSlickEntity::removeFromWorld()
{
	if ( mAddedToWorld )
	{
		VuDynamics::IF()->removeRigidBody(mpRigidBody);
		mAddedToWorld = false;
	}
}

//*****************************************************************************
void VuOilSlickEntity::tickDecision(float fdt)
{
	mAge += fdt;
	if ( mAge > SELF_COLLISION_TIME )
		mpRigidBody->mpIgnoreBody = VUNULL;

	if ( mRunOver )
	{
		removeFromWorld();

		if ( mRunOverEntity.get() )
			mRunOverEntity.get()->getEffectController()->applyEffect(mCarEffect.c_str(), mpLauncher);

		VuEntityRepository::IF()->removeManagedEntity(this);
	}
}

//*****************************************************************************
void VuOilSlickEntity::draw(const VuGfxDrawParams &params)
{
	struct DrawData
	{
		static void callback(void *data)
		{
			DrawData *pData = static_cast<DrawData *>(data);

			VuGfx::IF()->drawPrimitiveUP(VUGFX_PT_TRIANGLELIST, pData->mVertexCount/3, pData + 1);
		}

		int		mVertexCount;
	};

	if ( params.mbDrawReflection )
		return;

	float distSquared = (mTransform.getTrans() - params.mEyePos).magSquared();
	if ( distSquared > VuSquare(mDrawDist) )
		return;

	float distance = VuSqrt(distSquared);

	int vertexCount = (int)mVerts.size();

	// submit draw command
	int dataSize = sizeof(DrawData) + vertexCount*sizeof(VuOilSlickVertex);
	DrawData *pData = static_cast<DrawData *>(VuGfxSort::IF()->allocateCommandMemory(dataSize));
	pData->mVertexCount = vertexCount;

	// copy vertex data
	VU_MEMCPY(pData + 1, dataSize, &mVerts[0], dataSize);

	// draw
	VuGfxSort::IF()->submitDrawCommand<false>(VuGfxSort::TRANS_BLOB_SHADOW, mpMaterialAsset->mpGfxSortMaterials[VuMaterialAsset::FLV_MODULATED], VUNULL, DrawData::callback);
}