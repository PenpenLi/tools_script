//*****************************************************************************
//
//  Copyright (c) 2008-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Pfx CarChassis Emitter
// 
//*****************************************************************************

#include "VuPfxEmitCarChassis.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Dynamics/VuCollisionTypes.h"
#include "VuEngine/Pfx/VuPfxPattern.h"
#include "VuEngine/Pfx/Particles/VuPfxQuadParticle.h"
#include "VuEngine/Properties/VuBasicProperty.h"
#include "VuEngine/Properties/VuAngleProperty.h"
#include "VuEngine/Assets/VuFluidsMeshAsset.h"
#include "VuEngine/Managers/VuViewportManager.h"
#include "VuEngine/Water/VuFluidsObject.h"
#include "VuEngine/Water/VuWater.h"
#include "VuEngine/Math/VuRand.h"


IMPLEMENT_RTTI(VuPfxEmitCarChassis, VuPfxProcess);
IMPLEMENT_PFX_PROCESS_REGISTRATION(VuPfxEmitCarChassis);


// defines

#define MAX_EDGE_LIST_SIZE 128


// local classes

struct Edge
{
	VuVector3	mPos0;
	VuVector3	mPos1;
	VuVector3	mWaterVel0;
	VuVector3	mWaterVel1;
	VuVector3	mNormal;
	float		mWeight;
};
class EdgeList
{
public:
	EdgeList() : mCount(0), mTotalWeight(0) {}

	void push_back(const Edge &edge)
	{
		if ( mCount < MAX_EDGE_LIST_SIZE )
		{
			mEdges[mCount++] = edge;
			mTotalWeight += edge.mWeight;
		}
		else
		{
			VUPRINTF("EdgeList::push_back() max exceeded!\n");
		}
	}

	void getPoint(float fRatio, VuVector3 &vPos, VuVector3 &vWaterVel, VuVector3 &vNormal)
	{
		float fWeight = fRatio*mTotalWeight;
		Edge *pEdge = mEdges;
		for ( int i = 0; i < mCount; i++ )
		{
			if ( fWeight < pEdge->mWeight )
			{
				vPos = VuLerp(pEdge->mPos0, pEdge->mPos1, fWeight/pEdge->mWeight);
				vWaterVel = VuLerp(pEdge->mWaterVel0, pEdge->mWaterVel1, fWeight/pEdge->mWeight);
				vNormal = pEdge->mNormal;
				return;
			}
			fWeight -= pEdge->mWeight;
			pEdge++;
		}

		// this can happen due to single-precision float rounding errors
		vPos = mEdges[0].mPos0;
		vWaterVel = mEdges[0].mWaterVel0;
		vNormal = mEdges[0].mNormal;
	}

	float		totalWeight() const				{ return mTotalWeight; }
	const Edge	&operator[](int index) const	{ return mEdges[index]; }

private:
	Edge	mEdges[MAX_EDGE_LIST_SIZE];
	int		mCount;
	float	mTotalWeight;
};


//*****************************************************************************
VuPfxEmitCarChassis::VuPfxEmitCarChassis():
	mMinLifespan(1),
	mMaxLifespan(1),
	mMinScale(1),
	mMaxScale(1),
	mMinRotation(0),
	mMaxRotation(0),
	mMinAngularVelocity(0),
	mMaxAngularVelocity(0),
	mMinSpawnCarSpeed(0),
	mMaxSpawnCarSpeed(40),
	mMaxSpawnPerSecond(100),
	mDiffuseColor(255,255,255),
	mLateralVelocityMultiplier(1),
	mVerticalVelocityMultiplier(1),
	mMinDirStretch(0),
	mMaxDirStretch(0),
	mSpawnDistance(200.0f)
{
	mProperties.add(new VuFloatProperty("Min Lifespan", mMinLifespan));
	mProperties.add(new VuFloatProperty("Max Lifespan", mMaxLifespan));
	mProperties.add(new VuFloatProperty("Min Scale", mMinScale));
	mProperties.add(new VuFloatProperty("Max Scale", mMaxScale));
	mProperties.add(new VuAngleProperty("Min Rotation", mMinRotation));
	mProperties.add(new VuAngleProperty("Max Rotation", mMaxRotation));
	mProperties.add(new VuAngleProperty("Min Angular Velocity", mMinAngularVelocity));
	mProperties.add(new VuAngleProperty("Max Angular Velocity", mMaxAngularVelocity));
	mProperties.add(new VuFloatProperty("Min Spawn Car Speed", mMinSpawnCarSpeed));
	mProperties.add(new VuFloatProperty("Max Spawn Car Speed", mMaxSpawnCarSpeed));
	mProperties.add(new VuFloatProperty("Max Spawn Per Second", mMaxSpawnPerSecond));
	mProperties.add(new VuColorProperty("Diffuse Color", mDiffuseColor));
	mProperties.add(new VuFloatProperty("Lateral Velocity Multiplier", mLateralVelocityMultiplier));
	mProperties.add(new VuFloatProperty("Vertical Velocity Multiplier", mVerticalVelocityMultiplier));
	mProperties.add(new VuFloatProperty("Min Directional Stretch", mMinDirStretch));
	mProperties.add(new VuFloatProperty("Max Directional Stretch", mMaxDirStretch));
	mProperties.add(new VuFloatProperty("Spawn Distance", mSpawnDistance));
}

//*****************************************************************************
void VuPfxEmitCarChassisInstance::start()
{
	mSpawnAccum = 0.0f;
}

//*****************************************************************************
void VuPfxEmitCarChassisInstance::tick(float fdt, bool ui)
{
	const VuPfxEmitCarChassis *pParams = static_cast<const VuPfxEmitCarChassis *>(mpParams);

	if ( fdt <= FLT_EPSILON )
		return;

	if ( !mpStateIF )
		return;

	VuMatrix transform;
	VuVector3 linVel;
	VUUINT32 waterSurfaceFlags;
	mpStateIF->getPfxEmitCarChassisState(transform, linVel, waterSurfaceFlags);

	// driving in water?
	if ( !(waterSurfaceFlags & WATER_SURFACE_WATER) )
		return;

	// check spawn distance
	VuVector3 systemPos = transform.getTrans();
	bool bSpawn = false;
	for ( int i = 0; i < VuViewportManager::IF()->getViewportCount(); i++ )
	{
		const VuVector3 &eyePos = VuViewportManager::IF()->getViewport(i).mCamera.getEyePosition();
		if ( VuDistSquared(eyePos, systemPos) < pParams->mSpawnDistance*pParams->mSpawnDistance )
			bSpawn = true;
	}
	if ( !bSpawn )
		return;

	// speed is calculated as average edge speed weighted by length
	float fCarSpeed = 0;
	float fTotalEdgeLength = 0;

	// build edge list
	EdgeList edges;

	// (fluids object outline is determined by partially submerged tris)
	mpFluidsObject->setTransform(transform);

	if ( !mpFluidsObject->isSubmerged() )
		return;

	const VuFluidsMeshAsset *pFluidsMeshAsset = mpFluidsObject->getFluidsMeshAsset();
	int triCount = pFluidsMeshAsset->getTriCount();
	const VuFluidsMeshAsset::Tri *pTri = pFluidsMeshAsset->getTris();
	const VuFluidsObject::TransformedEdge *pTransformedEdges = mpFluidsObject->getTransformedEdges();
	for ( int i = 0; i < triCount; i++ )
	{
		// make sure triangle is on outside (e.g. don't consider inside edges of catamaran)
		if ( VuDot(pTri->mCentroid, pTri->mNormal) > 0 )
		{
			// make sure triangle is facing the direction of centroid point velocity
			VuVector3 vCentroidWorld = transform.transform(pTri->mCentroid);
			VuVector3 vNormalWorld = transform.transformNormal(pTri->mNormal);

			if ( VuDot(vNormalWorld, linVel) > 0 )
			{
				const VuFluidsObject::TransformedEdge &transEdge0 = pTransformedEdges[pTri->ei0];
				const VuFluidsObject::TransformedEdge &transEdge1 = pTransformedEdges[pTri->ei1];
				const VuFluidsObject::TransformedEdge &transEdge2 = pTransformedEdges[pTri->ei2];

				if ( transEdge0.mPartiallySubmerged | transEdge1.mPartiallySubmerged | transEdge2.mPartiallySubmerged )
				{
					Edge edge;

					if ( transEdge0.mPartiallySubmerged )
					{
						edge.mPos0 = transEdge0.mIntersectionPos;
						edge.mWaterVel0 = transEdge0.mIntersectionWaterVel;
						if ( transEdge1.mPartiallySubmerged )
						{
							edge.mPos1 = transEdge1.mIntersectionPos;
							edge.mWaterVel1 = transEdge1.mIntersectionWaterVel;
						}
						else
						{
							edge.mPos1 = transEdge2.mIntersectionPos;
							edge.mWaterVel1 = transEdge2.mIntersectionWaterVel;
						}
					}
					else
					{
						edge.mPos0 = transEdge1.mIntersectionPos;
						edge.mWaterVel0 = transEdge1.mIntersectionWaterVel;
						edge.mPos1 = transEdge2.mIntersectionPos;
						edge.mWaterVel1 = transEdge2.mIntersectionWaterVel;
					}

					VuVector3 pos = 0.5f*(edge.mPos0 + edge.mPos1);
					VuVector3 vSpeed = linVel;
					vSpeed -= 0.5f*(edge.mWaterVel0 + edge.mWaterVel1);
					float speed = vSpeed.mag();
					float length = (edge.mPos1 - edge.mPos0).mag();

					edge.mWeight = speed*length;
					edge.mNormal = vNormalWorld;
					edges.push_back(edge);

					fCarSpeed += speed*length;
					fTotalEdgeLength += length;
				}
			}
		}

		pTri++;
	}

	if ( edges.totalWeight() == 0 )
		return;

	// apply weighting to speed
	fCarSpeed /= fTotalEdgeLength;
	if ( fCarSpeed < pParams->mMinSpawnCarSpeed )
		return;

	// calculate spawn per second
	float fRatio = (fCarSpeed - pParams->mMinSpawnCarSpeed)/(pParams->mMaxSpawnCarSpeed - pParams->mMinSpawnCarSpeed);
	fRatio = VuClamp(fRatio, 0.0f, 1.0f);
	float fSpawnPerSecond = fRatio*pParams->mMaxSpawnPerSecond;

	mSpawnAccum += fSpawnPerSecond*fdt;
	while ( mSpawnAccum > 0.0f )
	{
		// spawn particle
		if ( VuPfxParticle *pParticle = mpPatternInstance->createParticle() )
		{
			VuPfxQuadParticle *pQuadParticle = static_cast<VuPfxQuadParticle *>(pParticle);

			pParticle->mAge = mSpawnAccum/fSpawnPerSecond;
			pParticle->mLifespan = VuLerp(pParams->mMinLifespan, pParams->mMaxLifespan, VuRand::global().rand());
			pParticle->mColor = pParams->mDiffuseColor.toVector4();
			pParticle->mScale = VuLerp(pParams->mMinScale, pParams->mMaxScale, VuRand::global().rand());

			VuVector3 vPos, vWaterVel, vNormal;
			edges.getPoint(VuRand::global().rand(), vPos, vWaterVel, vNormal);
			pParticle->mPosition = vPos;

			VuWaterRenderVertex vertex = VuWater::IF()->getRenderVertex(vPos);
			VuVector3 vWaterNormal(vertex.mDzDxy.mX, vertex.mDzDxy.mY, 1);
			vWaterNormal.normalize();

			VuVector3 vVel = vNormal - vWaterNormal*VuDot(vNormal, vWaterNormal);
			vVel = vVel.normal();
			vVel = pParams->mLateralVelocityMultiplier*vVel + pParams->mVerticalVelocityMultiplier*vWaterNormal;

			vVel *= VuSqrt(linVel.mag());
			vVel += vWaterVel;
			vVel += linVel;
			pParticle->mLinearVelocity = vVel;

			pQuadParticle->mRotation = VuLerp(pParams->mMinRotation, pParams->mMaxRotation, VuRand::global().rand());
			pQuadParticle->mAngularVelocity = VuLerp(pParams->mMinAngularVelocity, pParams->mMaxAngularVelocity, VuRand::global().rand());

			pQuadParticle->mDirStretch = VuLerp(pParams->mMinDirStretch, pParams->mMaxDirStretch, VuRand::global().rand());
		}

		mSpawnAccum -= 1.0f;
	}
}
