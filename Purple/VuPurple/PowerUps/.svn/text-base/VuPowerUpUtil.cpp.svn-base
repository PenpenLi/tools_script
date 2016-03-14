//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  PowerUp util
// 
//*****************************************************************************

#include "VuPowerUpUtil.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarSuspension.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuPurple/Services/VuCarSfxAttachService.h"
#include "VuEngine/Pfx/VuPfxManager.h"
#include "VuEngine/Pfx/VuPfxEntity.h"
#include "VuEngine/Pfx/VuPfx.h"
#include "VuEngine/Services/VuServiceManager.h"
#include "VuEngine/Services/VuPfxAttachService.h"
#include "VuEngine/Animation/VuSkeleton.h"
#include "VuEngine/Math/VuMathUtil.h"
#include "VuEngine/Json/VuJsonContainer.h"
#include "VuEngine/Util/VuDataUtil.h"


//*****************************************************************************
VUUINT32 VuPowerUpUtil::createPfx(VuCarEntity *pCar, const char *pfxName, bool oneShot)
{
	VUUINT32 hPfx = 0;

	if ( (hPfx = VuPfxManager::IF()->createEntity(pfxName, true)) )
	{
		if ( VuPfxEntity *pPfxEntity = VuPfxManager::IF()->getEntity(hPfx) )
		{
			if ( VuPfxAttachService *pService = VuServiceManager::IF()->createPfxService<VuPfxAttachService>() )
				pService->init(pCar, hPfx, VuMatrix::identity());
			pPfxEntity->getSystemInstance()->start();
		}
	}

	return hPfx;
}

//*****************************************************************************
VUUINT32 VuPowerUpUtil::createPfx(VuCarEntity *pCar, const VuJsonContainer &data, bool oneShot)
{
	VUUINT32 hPfx = 0;

	if ( data.isObject() )
	{
		if ( (hPfx = VuPfxManager::IF()->createEntity(data["Name"].asCString(), oneShot)) )
		{
			if ( VuPfxEntity *pPfxEntity = VuPfxManager::IF()->getEntity(hPfx) )
			{
				if ( VuPfxAttachService *pService = VuServiceManager::IF()->createPfxService<VuPfxAttachService>() )
				{
					const std::string &bone = data["Bone"].asString();
					const std::string &mount = data["Mount"].asString();
					if ( bone.length() )
					{
						int boneIndex = pCar->getDriver()->getSkeleton()->getBoneIndex(bone.c_str());
						VuMatrix transform;
						readTransform(data, transform);
						pService->init(pCar->getDriver(), hPfx, transform, pCar->getDriver()->getAnimatedModelInstance(), boneIndex);
					}
					else if ( mount.length() )
					{
						pService->init(pCar, hPfx, pCar->getMountPoint(mount.c_str()));
					}
					else
					{
						VuMatrix transform;
						readTransform(data, transform);
						pService->init(pCar, hPfx, transform);
					}
				}

				pPfxEntity->getSystemInstance()->start();
			}
		}
	}

	return hPfx;
}

//*****************************************************************************
void VuPowerUpUtil::readTransform(const VuJsonContainer &data, VuMatrix &transform)
{
	VuVector3 pos(0,0,0), rot(0,0,0);
	float scale(1.0f);

	VuDataUtil::getValue(data["Pos"], pos);
	VuDataUtil::getValue(data["Rot"], rot);
	VuDataUtil::getValue(data["Scale"], scale);

	rot = VuDegreesToRadians(rot);

	transform.setEulerAngles(rot);
	transform.scaleLocal(scale);
	transform.setTrans(pos);
}

//*****************************************************************************
void VuPowerUpUtil::createSfx(VuCarEntity *pCar, const char *sfxName)
{
#if !VU_DISABLE_AUDIO
	FMOD::Event *pEvent;
	if ( VuAudio::IF()->eventSystem()->getEvent(sfxName, FMOD_EVENT_NONBLOCKING, &pEvent) == FMOD_OK )
	{
		const VuVector3 *pos = &pCar->getModelPosition();
		const VuVector3 *vel = &pCar->getLinearVelocity();

		pEvent->set3DAttributes(reinterpret_cast<const FMOD_VECTOR *>(pos), reinterpret_cast<const FMOD_VECTOR *>(vel), VUNULL);
		pEvent->start();

		if ( VuCarSfxAttachService *pService = VuServiceManager::IF()->createService<VuCarSfxAttachService>() )
		{
			pService->init(pCar, pEvent);
		}
	}
#endif
}

//*****************************************************************************
void VuPowerUpUtil::calcMissileLaunchTransform(VuCarEntity *pLauncher, VuMatrix &transform)
{
	const VuMatrix &carTransform = pLauncher->getModelMatrix();

	transform = carTransform;
	if ( pLauncher->getSuspension()->getWheelContactCount() )
	{
		VuVector3 fwd = VuCross(pLauncher->getSuspension()->getAvgContactNormal(), carTransform.getAxisX());
		VuMathUtil::buildOrientationMatrix(fwd, carTransform.getAxisZ(), transform);
	}

	VuVector3 missilePos = carTransform.transform(pLauncher->getCollisionAabb().getCenter());
	transform.setTrans(missilePos);
}

//*****************************************************************************
VuCarEntity *VuPowerUpUtil::findBestMissileTarget(VuCarEntity *pLauncher, float range, float cosTargetAcquisitionCone)
{
	VuCarEntity *pTarget = VUNULL;

	VuVector3 launcherPos = pLauncher->getModelMatrix().transform(pLauncher->getCollisionAabb().getCenter());
	float bestDist = range;
	for ( int i = 0; i < VuCarManager::IF()->getCarCount(); i++ )
	{
		VuCarEntity *pOtherCar = VuCarManager::IF()->getCar(i);
		if ( pOtherCar != pLauncher )
		{
			if ( !pOtherCar->isGhostly() && !pOtherCar->hasCrashed() )
			{
				// check distance
				VuVector3 otherPos = pOtherCar->getModelMatrix().transform(pOtherCar->getCollisionAabb().getCenter());
				VuVector3 delta = otherPos - launcherPos;
				float dist = delta.normalize();
				if ( dist < bestDist )
				{
					// check target acquisition cone
					if ( VuDot(delta, pLauncher->getModelMatrix().getAxisY()) > cosTargetAcquisitionCone )
					{
						pTarget = pOtherCar;
						bestDist = dist;
					}
				}
			}
		}
	}

	return pTarget;
}
