//*****************************************************************************
//
//  Copyright (c) 2009-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Track Plan Class
// 
//*****************************************************************************

#include "VuEngine/Math/VuRand.h"

#include "VuPurple/AI/VuAiUtils.h"
#include "VuPurple/Entities/Car/Driver/VuAiDriverEntity.h"
#include "VuPurple/Dynamics/VuCollisionTypes.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Track/VuTrackSector.h"
#include "VuPurple/Track/VuTrackPlan.h"

//*****************************************************************************
VuTrackPlan::VuTrackPlan(int maxDepth)
:
mPlanType(PLAN_RANDOM),
mMaxDepth(maxDepth),
mpCar(VUNULL),
mBendiness(0.0f)
{
	mPlanSectorInfos.reserve(MAX_PLAN_SECTOR_INFOS);
}

//*****************************************************************************
VuTrackPlan::~VuTrackPlan()
{
	deallocatePlanInfo();

	mPlanSectorInfos.clear();
	mSectors.clear();
}

//*****************************************************************************
void VuTrackPlan::deallocatePlanInfo()
{
	for (int i = 0; i < mPlanSectorInfos.size(); i++)
	{
		VuPlanSectorInfo* pPlan = mPlanSectorInfos[i];
		if (pPlan)
		{
			delete pPlan;
		}
	}
}

//*****************************************************************************
bool VuTrackPlan::createPlan(VuCarEntity* pCar, const VuTrackSector *pStartingSector)
{
	mpCar = pCar;

//	VUPRINTF("%s: VuTrackPlan::createPlan()\n", mpCar->getDriverName().c_str());

	mSectors.reserve(mMaxDepth);
	mSectors.clear();
	mSectors.push_back(pStartingSector);

	deallocatePlanInfo();

	mPlanSectorInfos.reserve(mMaxDepth);
	mPlanSectorInfos.clear();
	mPlanSectorInfos.push_back(new VuPlanSectorInfo());
	
	return fillOutPlan();
}

//*****************************************************************************
bool VuTrackPlan::update()
{
//	VUPRINTF("%s: VuTrackPlan::update()\n", mpCar->getDriverName().c_str());

	mSectors.erase(0, 1);

	VuPlanSectorInfo* pInfo = mPlanSectorInfos.front();
	delete pInfo;
	mPlanSectorInfos.erase(0, 1);

	// Now, extend the plan to the max depth by starting from the end of the 
	// existing plan.
	return fillOutPlan();
}

//*****************************************************************************
const VuTrackSector *VuTrackPlan::getSectorForStep(int step) const
{
	if(step >= mSectors.size())
	{
		return VUNULL;
	}

	return mSectors[step];
}

//*****************************************************************************
const VuPlanSectorInfo *VuTrackPlan::getSectorInfoForStep(int step) const
{
	if (step >= (int)mPlanSectorInfos.size())
	{
		return VUNULL;
	}

	return mPlanSectorInfos[step];
}

//*****************************************************************************
bool VuTrackPlan::includes(const VuTrackSector *pSector) const
{
	if(pSector)
	{
		for(int iStep = 0; iStep < mSectors.size(); iStep++)
		{
			if(mSectors[iStep] == pSector)
			{
				return true;
			}
		}
	}

	return false;
}

//*****************************************************************************
bool VuTrackPlan::hasSpeedHint()
{
	for (int iStep = 0; iStep < mSectors.size(); iStep++)
	{
		float enterSpeedHint = mSectors[iStep]->mEnterSpeedHint;
		float exitSpeedHint = mSectors[iStep]->mExitSpeedHint;
		if (enterSpeedHint != 0 && enterSpeedHint != mpCar->getMaxForwardSpeed() &&
			exitSpeedHint != 0 && exitSpeedHint != mpCar->getMaxForwardSpeed())
		{
			return true;
		}
	}

	return false;
}

//*****************************************************************************
bool VuTrackPlan::fillOutPlan()
{
//	VUPRINTF("%s: VuTrackPlan::fillOutPlan() called.\n", mpCar->getDriverName().c_str());

	// Build our list that goes along with the plan using array storage
	// within the class' object
	int numSectors = mSectors.size();
	if (numSectors > 0)
	{
		// First is starting sector
		const VuTrackSector *pSector = mSectors.back();
		VuPlanSectorInfo* pInfo = VUNULL;

		// Loop through sectors up to depth
		while(pSector && mSectors.size() < mMaxDepth)
		{
			const VuTrackSector *pNextSector = onChooseNextSector(pSector);

			if(pNextSector)
			{
				VuPlanSectorInfo* pInfo = new VuPlanSectorInfo();

				// See if we were given the shortcut or the main path
				if (pSector && pSector->mNextSectorCount > 1 && pSector->mpNextSectors[1] == pNextSector)
				{
					pInfo->mShortCut = true;
				}

				mPlanSectorInfos.push_back(pInfo);
				mSectors.push_back(pNextSector);
			}

			pSector = pNextSector;
		}
	}

	// Do some track analysis on the planned route to compute bendiness
	analyzePlan();

	// Generate list of obstacles that exist in the current plan
//	findObstacles();

	return true;
}

//*****************************************************************************
// O(n^2) search, need to refactor
//
void VuTrackPlan::findObstacles()
{
	// TODO: broad phase sphere query for breakables/props
	struct VuQueryCallback : btBroadphaseAabbCallback
	{
		virtual bool process(const btBroadphaseProxy* proxy)
		{
			btCollisionObject *pColObj = static_cast<btCollisionObject *>(proxy->m_clientObject);
			if (pColObj->getInternalType() == btCollisionObject::CO_RIGID_BODY)
			{
				VuRigidBody *pRigidBody = static_cast<VuRigidBody *>(pColObj);

				if (pRigidBody != mpMe->getRigidBody())
				{
					VUUINT32 interestingObjectsFlags = EXT_COL_GAME_AI_DRIVER | EXT_COL_GAME_HUMAN_DRIVER | EXT_COL_ENGINE_BREAKABLE;
					if (pRigidBody->getExtendedFlags() & interestingObjectsFlags)
					{
						btTransform ident;
						ident.setIdentity();
						btVector3 aabbMin, aabbMax;
						pColObj->getCollisionShape()->getAabb(ident, aabbMin, aabbMax);

						pRigidBody->getAabb(aabbMin, aabbMax);

						if (mpSector->pointInside(VuVector3(aabbMin.getX(), aabbMin.getY(), aabbMin.getZ())) ||
							mpSector->pointInside(VuVector3(aabbMin.getX(), aabbMax.getY(), aabbMin.getZ())) ||
							mpSector->pointInside(VuVector3(aabbMax.getX(), aabbMin.getY(), aabbMin.getZ())) ||
							mpSector->pointInside(VuVector3(aabbMax.getX(), aabbMax.getY(), aabbMin.getZ())))
						{
							VuVector3 pos = pRigidBody->getVuCenterOfMassPosition();

							eAiLane lane = VuAiUtils::getLaneFromPos(pos);

							mpInfo->mBlockedLane = lane;
						}
					}
				}
			}

			return true;
		}
		const VuTrackSector*	mpSector;
		VuTrackPlan*			mpTrackPlan;
		VuCarEntity*			mpMe;
		VuPlanSectorInfo*		mpInfo;
	};

	// Iterate through the plan and locate cars and breakable objects that
	// may be obstacles
	int numSectors = mSectors.size();
	for (int i = 0; i<numSectors; i++)
	{
		const VuTrackSector* pSector = mSectors[i];
		VuPlanSectorInfo* pInfo = mPlanSectorInfos[i];

		// Create axis aligned bounding box that contains the sector
		VuAabb sectorAABB;
		sectorAABB.addPoint(pSector->mEnterLeft);
		sectorAABB.addPoint(pSector->mEnterRight);
		sectorAABB.addPoint(pSector->mExitLeft);
		sectorAABB.addPoint(pSector->mExitRight);

		btVector3 aabbMin = VuDynamicsUtil::toBtVector3(sectorAABB.mMin);
		btVector3 aabbMax = VuDynamicsUtil::toBtVector3(sectorAABB.mMax);

		VuQueryCallback callback;
		callback.mpSector = pSector;
		callback.mpTrackPlan = this;
		callback.mpMe = getCar();
		callback.mpInfo = pInfo;

		VuDynamics::IF()->getDynamicsWorld()->getBroadphase()->aabbTest(aabbMin, aabbMax, callback);
	}
}

//*****************************************************************************
// O(n) where n = # sectors
//
void VuTrackPlan::analyzePlan()
{
	float bendDensity = 0.0f;		// # of sectors over sum of sector distances
	float detourRatio = 0.0f;		// sum of sector distances over direct distance from start to goal point
	float cumulativeAngle = 0.0f;	// sum of angles amounts (absolute values)
	float cumulativeAngleRatio = 0.0f;	// sum of angles over sum of sector distances
	float sumOfAngles = 0.0f;

	// Loop through all sectors in the plan. Any branches have already been chosen

	// Total sector distance for plan
	//
	int numSectors = mSectors.size();

	if (numSectors == 0)
	{
		return;
	}

	float sumOfSectorDistances = 0.0f;
	for (int i = 0; i<numSectors; i++)
	{
		const VuTrackSector* pSector = mSectors[i];
		sumOfSectorDistances += pSector->mLength;
	}

	// Now calculate cumulative values
	//
	for (int i = 1; i<numSectors; i++)
	{
		const VuTrackSector* pPrevSector = mSectors[i-1];
		VuVector3 prevDir = pPrevSector->mUnitDir;

		const VuTrackSector* pCurSector = mSectors[i];
		VuVector3 curDir = pCurSector->mUnitDir;
		VuPlanSectorInfo* pCurInfo = mPlanSectorInfos[i];

		// Angle of difference between sectors
		float angle = VuACos(VuDot(prevDir, curDir));
		
		// Sum angle magnitide into cumulativeAngle
		cumulativeAngle += angle;

		// Cross prevDir with up to get prevDirRight
		VuVector3 prevDirRight = VuCross(prevDir, VuVector3(0.0f, 0.0f, 1.0f));

		// Dot right vector with curDir
		// Dot is positive if curDir is to the left of prevDir, negative if it's to the right
		float dotPrevRightCurDir = VuDot(prevDirRight, curDir);

		// Fix sign of angle,
		angle *= (dotPrevRightCurDir < 0) ? 1.0f : -1.0f;

		sumOfAngles += angle;

		// Additive as we go through the track sectors
		pCurInfo->mCumulativeTurn += cumulativeAngle;
		pCurInfo->mTurnAngleHere = angle;
	}

	float crowFliesDistance = VuDist(mSectors[0]->mEnterPoint, mSectors[numSectors - 1]->mExitPoint);

	bendDensity = (float)numSectors / sumOfSectorDistances;
	cumulativeAngleRatio = cumulativeAngle / sumOfSectorDistances;
	detourRatio = sumOfSectorDistances / crowFliesDistance;

	mBendiness = cumulativeAngleRatio * 100.0f;

//	VUPRINTF("Replan: cumulativeAngleRatio:%#.5f, bendDensity:%#.3f, detourRatio:%#.3f\n", cumulativeAngleRatio, bendDensity, detourRatio);
}

//*****************************************************************************
float VuTrackPlan::computeBendinessAt(int depth)
{
	return computeBendinessAt(1, depth);
}

//*****************************************************************************
float VuTrackPlan::computeBendinessAt(int start, int end)
{
	int depth = end - start;

	float angle = 0.0f;
	float dist = 0.0f;

	int last = VuMin(depth, (int)mPlanSectorInfos.size());

	for (int i = start; i < last; i++)
	{
		angle += VuAbs(mPlanSectorInfos[i]->mTurnAngleHere);

		dist += mSectors[i]->mLength;

		if (i >= depth)
		{
			break;
		}
	}

	return (angle / dist) * 100.0f;
}

//*****************************************************************************
float VuTrackPlan::computeBendinessAtDist(float totalDistance)
{
	float angle = 0.0f;
	float dist = 0.0f;

	for (int i = 1; i < (int)mPlanSectorInfos.size(); i++)
	{
		angle += VuAbs(mPlanSectorInfos[i]->mTurnAngleHere);

		dist += mSectors[i]->mLength;

		if (dist >= totalDistance)
		{
			break;
		}
	}

	return (angle / dist) * 100.0f;
}

//*****************************************************************************
float VuTrackPlan::computeBendinessAtTime(float lookAheadTime)
{
	float decayFactor = 0.90f;

	float totalDistance = lookAheadTime * getCar()->getLinearVelocity().mag2d() * decayFactor;
	
	totalDistance = VuMin(totalDistance, 25.0f);

	return computeBendinessAtDist(totalDistance);
}

//*****************************************************************************
float VuTrackPlan::computeCornerAngle(float lookAheadTime)
{
	float decayFactor = 0.90f;

	float totalDistance = lookAheadTime * getCar()->getLinearVelocity().mag2d() * decayFactor;

	totalDistance = VuMin(totalDistance, 25.0f);

	float angle = mPlanSectorInfos[1]->mTurnAngleHere;
	float dist = mSectors[1]->mLength;
	
	for (int i = 2; i < (int)mPlanSectorInfos.size(); i++)
	{
		float turnAngleHere = mPlanSectorInfos[i]->mTurnAngleHere;

		if ((turnAngleHere > 0.0f && angle < 0.0f) ||
			(turnAngleHere < 0.0f && angle > 0.0f))
		{
			// Detect sign change and abort, we're at the end of the curve
			break;
		}

		angle += turnAngleHere;

		dist += mSectors[i]->mLength;

		if (dist >= totalDistance)
		{
			break;
		}
	}

	return VuAbs(angle);
}

//*****************************************************************************
float VuTrackPlan::getDistanceToNextShortcut()
{
	float distance = 0.0f;
	for (int i = 0; i < mSectors.size(); i++)
	{
		if (mPlanSectorInfos[i]->mShortCut)
		{
			return distance;
		}
		distance += mSectors[i]->mLength;
	}

	// 0.0f means no useable shortcut found. Also returned when shortcut is
	// in the first sector int the list (too late to turn)
	//
	return 0.0f;
}

//*****************************************************************************
const VuTrackSector *VuTrackPlan::onChooseNextSector(const VuTrackSector *pSector)
{
	// If the branch is not for AI, just follow the main track
	if (pSector->mNoAiOnBranch)
	{
		return pSector->mNextSectorCount ? pSector->mpNextSectors[0] : VUNULL;
	}

	switch (mPlanType)
	{
		case PLAN_MAIN_TRACK:
		{
			return pSector->mNextSectorCount ? pSector->mpNextSectors[0] : VUNULL;
		}
		break;
		case PLAN_RANDOM:
		{
			VuRand rand;

			int nextCount = pSector->mNextSectorCount;

			if (nextCount)
			{
				float randValue = rand.rand();

				float value = 0.0f;
				int choice = 0;

				for (int i = 0; i < nextCount; i++)
				{
					value += pSector->mpNextSectors[i]->mBranchProbability;

					if (randValue < value)
					{
						choice = i;
						break;
					}
				}

				if (choice > 0)
				{
					VuAiDriver* pDriver = static_cast<VuAiDriver*>(mpCar->getDriver());
					if (pDriver)
					{
						VuAiUtils::reportEvent(pDriver->getAiInstance(), "TakeShortcut", "true");
					}
				}

				return pSector->mpNextSectors[choice];
			}
			else
			{
				return VUNULL;
			}
		}
		break;
		case PLAN_SHORTCUTS:
		{
			if (pSector->mNextSectorCount >= 2)
			{
				VuAiDriver* pDriver = static_cast<VuAiDriver*>(mpCar->getDriver());
				if (pDriver)
				{
					VuAiUtils::reportEvent(pDriver->getAiInstance(), "TakeShortcut", "true");
				}

				return pSector->mpNextSectors[1];
			}
			else
			{
				return pSector->mpNextSectors[0];
			}
		}
		break;
		case PLAN_RANDOM_SHORTCUTS:
		{
			if (pSector->mNextSectorCount >= 2)
			{
				bool useShortcut = false;

				// Car class (A, B, C, D)
				float stage = (float)mpCar->getCarStage();

				float ratio = (float)stage / 6.0f;	// ranges from 0/8ths to 3/6ths, with 3/6ths being highest. 50% chance for a Class D AI to take a shortcut

				if (VuRand::global().rand() < ratio)
				{
					useShortcut = true;
				}

				if (useShortcut)
				{
					VuAiDriver* pDriver = static_cast<VuAiDriver*>(mpCar->getDriver());
					if (pDriver)
					{
						VuAiUtils::reportEvent(pDriver->getAiInstance(), "TakeShortcut", "true");
					}

					return pSector->mpNextSectors[1];
				}
				else
				{
					return pSector->mpNextSectors[0];
				}
			}
			else
			{
				return pSector->mpNextSectors[0];
			}
		}
	}

	return VUNULL;
}
