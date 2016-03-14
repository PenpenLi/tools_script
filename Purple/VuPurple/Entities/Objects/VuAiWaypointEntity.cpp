//*****************************************************************************
//
//  Copyright (c) 2007-2008 Ralf Knoesel
//  Copyright (c) 2008-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  AIWaypoint entity
// 
//*****************************************************************************

#include "VuAiWaypointEntity.h"
#include "VuPurple/AI/VuAiManager.h"
#include "VuPurple/Track/VuTrackManager.h"
#include "VuPurple/Track/VuTrackSector.h"
#include "VuEngine/Components/3dLayout/Vu3dLayoutComponent.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Properties/VuBasicProperty.h"
#include "VuEngine/Properties/VuBitFieldProperty.h"
#include "VuEngine/Gfx/Camera/VuCamera.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/HAL/Gfx/VuGfx.h"
#include "VuEngine/Math/VuMathUtil.h"


IMPLEMENT_RTTI(VuAiWaypointEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuAiWaypointEntity);


//*****************************************************************************
VuAiWaypointEntity::VuAiWaypointEntity():
	mLeftRightRatio(0.0f),
	mSpeedHintMPH(0.0f),
	mFlags(ALLOW_RECOVERY),
	mPrimaryBranchProbability(1.0f),
	mAlternateBranchProbability(0.0f),
	mCheckPointMinExtents(-50,-10),
	mCheckPointMaxExtents(50,50),
	mpNextPrimaryWaypoint(VUNULL),
	mpNextAlternateWaypoint(VUNULL),
	mpPrimarySector(VUNULL),
	mpAlternateSector(VUNULL)
{
	// components
	addComponent(mp3dLayoutComponent = new Vu3dLayoutComponent(this));
	addComponent(mpScriptComponent = new VuScriptComponent(this, 150));

	mpTransformComponent->setMask(VuTransformComponent::TRANS|VuTransformComponent::ROT_Z|VuTransformComponent::SCALE_X|VuTransformComponent::SCALE_Z);
	mp3dLayoutComponent->setDrawMethod(this, &VuAiWaypointEntity::drawLayout);
	mp3dLayoutComponent->setLocalBounds(VuAabb(VuVector3(-0.5f, 0, 0), VuVector3(0.5f, 0, 1)));
	mp3dLayoutComponent->setForceVisible(true);

	// properties
	addProperty(new VuFloatProperty("Left/Right Ratio", mLeftRightRatio));	
	addProperty(new VuFloatProperty("Speed Hint", mSpeedHintMPH));
	addProperty(new VuBitFieldProperty("Start", mFlags, IS_START));
	addProperty(new VuBitFieldProperty("CheckPoint", mFlags, IS_CHECKPOINT));
	addProperty(new VuBitFieldProperty("Finish", mFlags, IS_FINISH));
	addProperty(new VuBitFieldProperty("Allow Recovery", mFlags, ALLOW_RECOVERY));
	addProperty(new VuBitFieldProperty("No AI on Branch", mFlags, IS_NOAI_BRANCH));
	addProperty(new VuFloatProperty("Primary Branch Probability", mPrimaryBranchProbability));
	addProperty(new VuVector2Property("CheckPoint Min Extents", mCheckPointMinExtents));
	addProperty(new VuVector2Property("CheckPoint Max Extents", mCheckPointMaxExtents));

	// plugs/refs
	ADD_SCRIPT_REF(mpScriptComponent, NextPrimary, VuAiWaypointEntity::msRTTI);
	ADD_SCRIPT_REF(mpScriptComponent, NextAlternate, VuAiWaypointEntity::msRTTI);

	ADD_SCRIPT_INPUT(mpScriptComponent, VuAiWaypointEntity, UpdateBranchProbabilities, VuRetVal::Void, VuParamDecl(4, VuParams::Bool, VuParams::Bool, VuParams::Float, VuParams::Float));
}

//*****************************************************************************
void VuAiWaypointEntity::onGameInitialize()
{
	mpNextPrimaryWaypoint = mpScriptComponent->getRef("NextPrimary")->getRefEntity<VuAiWaypointEntity>();
	mpNextAlternateWaypoint = mpScriptComponent->getRef("NextAlternate")->getRefEntity<VuAiWaypointEntity>();

	normalizeProbabilities(mPrimaryBranchProbability, mAlternateBranchProbability);

	createSectors();
}

//*****************************************************************************
void VuAiWaypointEntity::onGameRelease()
{
	releaseSectors();
}

//*****************************************************************************
VuVector3 VuAiWaypointEntity::getPosition() const
{
	const VuMatrix &transform = mpTransformComponent->getWorldTransform();
	return transform.getTrans() + (0.5f*mLeftRightRatio*getWidth())*transform.getAxisX();
}

//*****************************************************************************
VuVector3 VuAiWaypointEntity::getRightEndPoint() const
{
	const VuMatrix &transform = mpTransformComponent->getWorldTransform();
	return transform.getTrans() + (0.5f*getWidth())*transform.getAxisX();
}

//*****************************************************************************
VuVector3 VuAiWaypointEntity::getLeftEndPoint() const
{
	const VuMatrix &transform = mpTransformComponent->getWorldTransform();
	return transform.getTrans() - (0.5f*getWidth())*transform.getAxisX();
}

//*****************************************************************************
bool VuAiWaypointEntity::testIntersection(const VuVector3 &prevPos, const VuVector3 &nextPos, float radius)
{
	const VuMatrix &transform = mpTransformComponent->getWorldTransform();
	VuVector4 planeXZ = VuMathUtil::planeFromNormalPoint(transform.getAxisY(), transform.getTrans());

	float prevDist = VuMathUtil::distPointPlane(prevPos, planeXZ);
	float nextDist = VuMathUtil::distPointPlane(nextPos, planeXZ);

	if ( prevDist < 0.0f && nextDist >= 0.0f )
	{
		// check bounds
		prevDist = -prevDist;
		float ratio = prevDist/(prevDist + nextDist);
		VuVector3 projPos = prevPos + (nextPos - prevPos)*ratio;

		// check width
		VuVector4 planeYZ = VuMathUtil::planeFromNormalPoint(transform.getAxisX(), transform.getTrans());
		float distX = VuMathUtil::distPointPlane(projPos, planeYZ);
		if ( (distX + radius > mCheckPointMinExtents.mX) && (distX - radius < mCheckPointMaxExtents.mX) )
		{
			// check height
			VuVector4 planeXY = VuMathUtil::planeFromNormalPoint(transform.getAxisZ(), transform.getTrans());
			float distZ = VuMathUtil::distPointPlane(projPos, planeXY);
			if ( (distZ + radius > mCheckPointMinExtents.mY) && (distZ - radius < mCheckPointMaxExtents.mY) )
			{
				return true;
			}
		}
	}

	return false;
}

//*****************************************************************************
VuRetVal VuAiWaypointEntity::UpdateBranchProbabilities(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);

	bool setA = accessor.getBool();
	bool setB = accessor.getBool();
	float a = accessor.getFloat();
	float b = accessor.getFloat();

	a = setA ? a : mPrimaryBranchProbability;
	b = setB ? b : mAlternateBranchProbability;

	setBranchProbabilities(a, b);

	return VuRetVal();
}

//*****************************************************************************
void VuAiWaypointEntity::setBranchProbabilities(float a, float b)
{
	mPrimaryBranchProbability = mpPrimarySector ? a : 0.0f;
	mAlternateBranchProbability = mpAlternateSector ? b : 0.0f;

	normalizeProbabilities(mPrimaryBranchProbability, mAlternateBranchProbability);

	const VuTrackSector *apSectors[2] = { VUNULL, VUNULL };
	int changes = 0;

	if ( mpPrimarySector )
	{
		mpPrimarySector->mBranchProbability = mPrimaryBranchProbability;
		apSectors[changes++] = mpPrimarySector;
	}

	if ( mpAlternateSector )
	{
		mpAlternateSector->mBranchProbability = mAlternateBranchProbability;
		apSectors[changes++] = mpAlternateSector;
	}

	VuAiManager::IF()->notifyBranchChanges(2, apSectors);
}

//*****************************************************************************
void VuAiWaypointEntity::normalizeProbabilities(float &a, float &b)
{
	VUASSERT(a != 0.0f || b != 0.0f, "Invalid branch probabilities");

	float sum = a + b;

	a /= sum;
	b /= sum;
}

//*****************************************************************************
void VuAiWaypointEntity::createSectors()
{
	if ( mpNextPrimaryWaypoint )
		mpPrimarySector = VuTrackManager::IF()->createSector(this, mpNextPrimaryWaypoint, mPrimaryBranchProbability);

	if ( mpNextAlternateWaypoint )
		mpAlternateSector = VuTrackManager::IF()->createSector(this, mpNextAlternateWaypoint, mAlternateBranchProbability);
}

//*****************************************************************************
void VuAiWaypointEntity::releaseSectors()
{
	if ( mpPrimarySector )
	{
		VuTrackManager::IF()->releaseSector(mpPrimarySector);
		mpPrimarySector = VUNULL;
	}

	if ( mpAlternateSector )
	{
		VuTrackManager::IF()->releaseSector(mpAlternateSector);
		mpAlternateSector = VUNULL;
	}
}

//*****************************************************************************
void VuAiWaypointEntity::drawLayout(const Vu3dLayoutDrawParams &params)
{
	if ( params.mbSelected )
	{
		VuMatrix mat = mpTransformComponent->getWorldTransform();
		mat.scaleLocal(mpTransformComponent->getWorldScale());

		VuGfxUtil::IF()->drawAabbSolid(VuColor(255,255,255,128), mp3dLayoutComponent->getLocalBounds(), mat, params.mCamera.getViewProjMatrix());
	}

	if ( VuAiWaypointEntity *pNextPrimaryWaypoint = mpScriptComponent->getRef("NextPrimary")->getRefEntity<VuAiWaypointEntity>() )
	{
		VuColor sectorColor(255,255,255); // white

		if ( !(allowRecovery() && pNextPrimaryWaypoint->allowRecovery()) )
			sectorColor = VuColor(255,0,0);

		if ( !params.mbSelected )
			sectorColor *= 0.5f;

		VuGfxUtil::IF()->drawLine3d(sectorColor, getPosition(), pNextPrimaryWaypoint->getPosition(), params.mCamera.getViewProjMatrix());
		VuGfxUtil::IF()->drawLine3d(sectorColor, getRightEndPoint(), pNextPrimaryWaypoint->getRightEndPoint(), params.mCamera.getViewProjMatrix());
		VuGfxUtil::IF()->drawLine3d(sectorColor, getLeftEndPoint(), pNextPrimaryWaypoint->getLeftEndPoint(), params.mCamera.getViewProjMatrix());
	}
	
	if ( VuAiWaypointEntity *pNextAlternateWaypoint = mpScriptComponent->getRef("NextAlternate")->getRefEntity<VuAiWaypointEntity>() )
	{
		VuColor sectorColor(255,255,255); // white

		if ( !(allowRecovery() && pNextAlternateWaypoint->allowRecovery()) )
			sectorColor = VuColor(255,0,0);

		if ( !params.mbSelected )
			sectorColor *= 0.5f;

		VuGfxUtil::IF()->drawLine3d(sectorColor, getPosition(), pNextAlternateWaypoint->getPosition(), params.mCamera.getViewProjMatrix());
		VuGfxUtil::IF()->drawLine3d(sectorColor, getRightEndPoint(), pNextAlternateWaypoint->getRightEndPoint(), params.mCamera.getViewProjMatrix());
		VuGfxUtil::IF()->drawLine3d(sectorColor, getLeftEndPoint(), pNextAlternateWaypoint->getLeftEndPoint(), params.mCamera.getViewProjMatrix());
	}

	if ( isCheckPoint() )
	{
		VuAabb aabb;
		aabb.mMin.mX = mCheckPointMinExtents.mX;
		aabb.mMax.mX = mCheckPointMaxExtents.mX;
		aabb.mMin.mY = 0.0f;
		aabb.mMax.mY = 0.0f;
		aabb.mMin.mZ = mCheckPointMinExtents.mY;
		aabb.mMax.mZ = mCheckPointMaxExtents.mY;

		VuGfxUtil::IF()->drawAabbSolid(VuColor(128,255,128,128), aabb, mpTransformComponent->getWorldTransform(), params.mCamera.getViewProjMatrix());
	}
}
