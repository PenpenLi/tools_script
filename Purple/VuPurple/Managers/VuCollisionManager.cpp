//*****************************************************************************
//
//  Copyright (c) 2008-2008 Ralf Knoesel
//  Copyright (c) 2008-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  CollisionManager class
// 
//*****************************************************************************

#include "VuCollisionManager.h"
#include "VuPurple/Dynamics/VuCollisionTypes.h"
#include "VuPurple/Managers/VuCarManager.h"
#include "VuEngine/Dynamics/VuDynamics.h"
#include "VuEngine/HAL/Audio/VuAudio.h"
#include "VuEngine/Assets/VuAssetFactory.h"
#include "VuEngine/Assets/VuDBAsset.h"
#include "VuEngine/Assets/VuCollisionMeshAsset.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Pfx/VuPfxManager.h"
#include "VuEngine/Pfx/VuPfxEntity.h"
#include "VuEngine/Pfx/VuPfxSystem.h"
#include "VuEngine/Pfx/VuPfxPattern.h"
#include "VuEngine/Pfx/Processes/VuPfxEmitFountain.h"
#include "VuEngine/Dev/VuDevStat.h"


// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuCollisionManager, VuCollisionManager);


// constants
#define MIN_IMPACT_SPEED (2.5f)  // m/s
#define MAX_IMPACT_SPEED (35.0f) // m/s
#define MAX_SCRAPE_SPEED (35.0f) // m/s


//*****************************************************************************
VuCollisionManager::VuCollisionManager():
	mpImpactEventGroup(VUNULL),
	mpScrapeEventGroup(VUNULL),
	mSurfaceTypeCount(0)
{
	mImpactEvents.reserve(32);
	mManifolds.reserve(32);

	VuDevStat::IF()->addPage("CollisionManager", VuRect(50, 10, 40, 80));
}

//*****************************************************************************
VuCollisionManager::~VuCollisionManager()
{
}

//*****************************************************************************
bool VuCollisionManager::init()
{
	// add contact callback
	VuDynamics::IF()->registerContactCallback(this);

	// load surface tables
	mSurfaceTypeCount = VuDynamics::IF()->getSurfaceTypeCount();
	VuDBAsset *pSurfaceMapDBAsset = VuAssetFactory::IF()->createAsset<VuDBAsset>("SurfaceTableDB");
	{
		loadEventNameTable(pSurfaceMapDBAsset, "ImpactSfx", mImpactSfxTable);
		loadEventNameTable(pSurfaceMapDBAsset, "ScrapeSfx", mScrapeSfxTable);
		loadEventNameTable(pSurfaceMapDBAsset, "ImpactPfx", mImpactPfxTable, "Impact/");
		loadEventNameTable(pSurfaceMapDBAsset, "ScrapePfx", mScrapePfxTable, "Scrape/");
		loadEventNameTable(pSurfaceMapDBAsset, "WheelDrivePfx", mWheelDrivePfxTable, "Car/WheelDrive/");
		loadEventNameTable(pSurfaceMapDBAsset, "WheelSlidePfx", mWheelSlidePfxTable, "Car/WheelSlide/");
	}
	VuAssetFactory::IF()->releaseAsset(pSurfaceMapDBAsset);

	VuTickManager::IF()->registerHandler(this, &VuCollisionManager::tickDecision, "Decision");

	return true;
}

//*****************************************************************************
void VuCollisionManager::postInit()
{
#if !VU_DISABLE_AUDIO
	FMODCALL(VuAudio::IF()->eventSystem()->getGroup("Global/Collision/Impact", true, &mpImpactEventGroup));
	FMODCALL(VuAudio::IF()->eventSystem()->getGroup("Global/Collision/Scrape", true, &mpScrapeEventGroup));
#endif // !VU_DISABLE_AUDIO
}

//*****************************************************************************
void VuCollisionManager::release()
{
	// remove contact callback
	VuDynamics::IF()->unregisterContactCallback(this);

	VuTickManager::IF()->unregisterHandlers(this);
}

//*****************************************************************************
void VuCollisionManager::reset()
{
	// reset impact events
	mImpactEvents.clear();

	// reset manifolds
	for ( int i = 0; i < mManifolds.size(); i++ )
	{
		VuManifold *p = &mManifolds[i];
		#if !VU_DISABLE_AUDIO
		p->mpEvent->stop();
		#endif
		if ( VuPfxEntity *pPfxEntity = VuPfxManager::IF()->getEntity(p->mhPfx) )
			pPfxEntity->getSystemInstance()->stop();
	}
	mManifolds.clear();
}

//*****************************************************************************
float VuCollisionManager::calculateImpactIntensity(const VuRigidBody *pRB0, const VuRigidBody *pRB1, const VuVector3 &pos, const VuVector3 &nor1)
{
	float fIntensity = 0.0f;

	if ( pRB0->hasContactResponse() && pRB1->hasContactResponse() )
	{
		// compute relative velocity
		VuVector3 vRelV = VuDynamicsUtil::pointVelocityWorld(*pRB0, pos) - VuDynamicsUtil::pointVelocityWorld(*pRB1, pos);
		float fRelVel = VuDot(nor1, vRelV);
		fIntensity = (-fRelVel - MIN_IMPACT_SPEED)/(MAX_IMPACT_SPEED - MIN_IMPACT_SPEED);
		fIntensity = VuClamp(fIntensity, 0.0f, 1.0f);
	}

	return fIntensity;
}

//*****************************************************************************
void VuCollisionManager::onGlobalContactAdded(VuContactPoint &cp)
{
	// we only care about camera target jet ski
	if ( (cp.mpBody0->getCollisionGroup() | cp.mpBody1->getCollisionGroup()) & COL_GAME_CAR )
	{
		bool bCameraTarget = false;
		VUUINT8 surfaceType = 0;
		if ( cp.mpBody0->getCollisionGroup() & COL_GAME_CAR )
		{
			bCameraTarget |= static_cast<VuCarEntity *>(cp.mpBody0->getEntity())->getIsCameraTarget();
			surfaceType = cp.mSurfaceType1;
		}
		if ( cp.mpBody1->getCollisionGroup() & COL_GAME_CAR )
		{
			bCameraTarget |= static_cast<VuCarEntity *>(cp.mpBody1->getEntity())->getIsCameraTarget();
			surfaceType = cp.mSurfaceType0;
		}

		if ( bCameraTarget )
		{
			if ( float fIntensity = calculateImpactIntensity(cp.mpBody0, cp.mpBody1, cp.mPosWorld, cp.mNorWorld) )
			{
				// if intensity is less than some threshold, don't add contact if a manifold already exists
				if ( fIntensity < 1.0f )
				{
					// find manifold
					for ( VuManifold *p = &mManifolds.begin(); p != &mManifolds.end(); p++ )
						if ( p->mpBody0 == cp.mpBody0 && p->mpBody1 == cp.mpBody1 && p->mSurfaceType0 == cp.mSurfaceType0 && p->mSurfaceType1 == cp.mSurfaceType1 )
							return;
				}

				mImpactEvents.resize(mImpactEvents.size() + 1);
				VuImpactEvent *pIE = &mImpactEvents.back();

				pIE->mSurfaceType = surfaceType;
				pIE->mPosition = cp.mPosWorld;
				pIE->mIntensity = fIntensity;
			}
		}
	}
}

//*****************************************************************************
void VuCollisionManager::loadEventNameTable(const VuDBAsset *pDBAsset, const char *strTableName, EventNameTable &table, const char *strPrefix)
{
	// initialize table
	table.resize(mSurfaceTypeCount);
	memset(&table.begin(), 0, sizeof(table[0])*table.size());

	const VuJsonContainer &data = pDBAsset->getDB()[strTableName];

	// populate table
	for ( int i = 0; i < data.numMembers(); i++ )
	{
		const std::string &key = data.getMemberKey(i);
		int surfaceTypeID = VuDynamics::IF()->getSurfaceTypeID(key.c_str());
		VU_SNPRINTF(table[surfaceTypeID], MAX_NAME_LEN, MAX_NAME_LEN - 1, "%s%s", strPrefix, data[key].asCString());
	}
}

//*****************************************************************************
void VuCollisionManager::tickDecision(float fdt)
{
	playImpactEvents();
	updateManifolds();
	updateDevStats();
}

//*****************************************************************************
void VuCollisionManager::playImpactEvents()
{
	for ( int i = 0; i < mImpactEvents.size(); i++ )
	{
		VuImpactEvent *pIE = &mImpactEvents[i];

		// sfx
		#if !VU_DISABLE_AUDIO
		{
			const char *strEventName = mImpactSfxTable[pIE->mSurfaceType];

			FMOD::Event *pEvent;
			if ( mpImpactEventGroup->getEvent(strEventName, FMOD_EVENT_NONBLOCKING, &pEvent) == FMOD_OK )
			{
				FMOD_VECTOR pos = VuAudio::toFmodVector(pIE->mPosition);
				pEvent->set3DAttributes(&pos, VUNULL, VUNULL);

				FMOD::EventParameter *pParam;
				if ( pEvent->getParameter("intensity", &pParam) == FMOD_OK )
					pParam->setValue(pIE->mIntensity);

				pEvent->start();
			}
		}
		#endif // !VU_DISABLE_AUDIO

		//// pfx
		//{
		//	const char *strPath = mImpactPfxTable[pIE->mSurfaceType];

		//	if ( VUUINT32 hPfx = VuPfxManager::IF()->createEntity(strPath, true) )
		//	{
		//		if ( VuPfxEntity *pPfxEntity = VuPfxManager::IF()->getEntity(hPfx) )
		//		{
		//			// set custom pfx process data
		//			VuPfxSystemInstance *pSystemInst = pPfxEntity->getSystemInstance()->getSystemInstance();
		//			for ( VuPfxPatternInstance *pPatternInst = pSystemInst->mPatterns.front(); pPatternInst; pPatternInst = pPatternInst->next() )
		//				for ( VuPfxProcessInstance *pProcessInst = pPatternInst->mProcesses.front(); pProcessInst; pProcessInst = pProcessInst->next() )
		//					if ( pProcessInst->mpParams->isDerivedFrom(VuPfxEmitFountain::msRTTI) )
		//						static_cast<VuPfxEmitFountainInstance *>(pProcessInst)->mMaxSpawnCountMultiplier = pIE->mIntensity;

		//			pPfxEntity->getSystemInstance()->setPosition(pIE->mPosition);
		//			pPfxEntity->getSystemInstance()->start();
		//		}
		//	}
		//}
	}

	mImpactEvents.clear();
}

//*****************************************************************************
void VuCollisionManager::updateManifolds()
{
	// deactivate all manifolds
	for ( VuManifold *p = &mManifolds.begin(); p != &mManifolds.end(); p++ )
		p->mbActive = false;

	btDispatcher *pDispatcher = VuDynamics::IF()->getDynamicsWorld()->getDispatcher();
	int manifoldCount = pDispatcher->getNumManifolds();
	for ( int i = 0; i < manifoldCount; i++ )
	{
		btPersistentManifold *pManifold = pDispatcher->getManifoldByIndexInternal(i);
		if ( int contactCount = pManifold->getNumContacts() )
		{
			// non-rigid bodies not supported yet
			const btCollisionObject *colObj0 = static_cast<const btCollisionObject *>(pManifold->getBody0());
			const btCollisionObject *colObj1 = static_cast<const btCollisionObject *>(pManifold->getBody1());
			if ( colObj0->getInternalType() == btCollisionObject::CO_RIGID_BODY && colObj1->getInternalType() == btCollisionObject::CO_RIGID_BODY )
			{
				const VuRigidBody *pBody0 = static_cast<const VuRigidBody *>(colObj0);
				const VuRigidBody *pBody1 = static_cast<const VuRigidBody *>(colObj1);

				// we only care about camera target jet ski
				if ( (pBody0->getCollisionGroup() | pBody1->getCollisionGroup()) & COL_GAME_CAR )
				{
					bool bLocalPlayerBoat = false;
					int otherObject;
					if ( pBody0->getCollisionGroup() & COL_GAME_CAR )
					{
						bLocalPlayerBoat |= static_cast<VuCarEntity *>(pBody0->getEntity())->getIsCameraTarget();
						otherObject = 1;
					}
					if ( pBody1->getCollisionGroup() & COL_GAME_CAR )
					{
						bLocalPlayerBoat |= static_cast<VuCarEntity *>(pBody1->getEntity())->getIsCameraTarget();
						otherObject = 0;
					}

					if ( bLocalPlayerBoat )
					{
						for ( int j = 0; j < contactCount; j++ )
						{
							btManifoldPoint& pt = pManifold->getContactPoint(j);
							if ( !pt.m_isPredictive )
							{
								int surfaceType0 = pBody0->getSurfaceType();
								int surfaceType1 = pBody1->getSurfaceType();
								if ( const VuCollisionMeshAsset *pAsset = static_cast<const VuCollisionMeshAsset *>(pBody0->getCollisionShape()->getUserPointer()) )
									surfaceType0 = pAsset->getTriangleMaterial(pt.m_index0).mSurfaceTypeID;
								if ( const VuCollisionMeshAsset *pAsset = static_cast<const VuCollisionMeshAsset *>(pBody1->getCollisionShape()->getUserPointer()) )
									surfaceType1 = pAsset->getTriangleMaterial(pt.m_index1).mSurfaceTypeID;

								// find manifold
								VuManifold *p;
								for ( p = &mManifolds.begin(); p != &mManifolds.end(); p++ )
									if ( p->mpBody0 == pBody0 && p->mpBody1 == pBody1 && p->mSurfaceType0 == surfaceType0 && p->mSurfaceType1 == surfaceType1 )
										break;

								bool bNewManifold = false;

								// not found, add new manifold
								if ( p == &mManifolds.end() )
								{
									mManifolds.resize(mManifolds.size() + 1);
									p = &mManifolds.back();
									p->mpBody0 = pBody0;
									p->mpBody1 = pBody1;
									p->mSurfaceType0 = surfaceType0;
									p->mSurfaceType1 = surfaceType1;
									p->mbActive = false;

									#if !VU_DISABLE_AUDIO
									int surfaceType = otherObject == 0 ? surfaceType0 : surfaceType1;
									const char *strEventName = mScrapeSfxTable[surfaceType];
									mpScrapeEventGroup->getEvent(strEventName, FMOD_EVENT_NONBLOCKING, &p->mpEvent);
									#endif

									//const char *strPath = mScrapePfxTable[surfaceType];
									//p->mhPfx = VuPfxManager::IF()->createEntity(strPath, false);

									bNewManifold = true;
								}

								VuVector3 vPos0 = VuDynamicsUtil::toVuVector3(pt.m_positionWorldOnA);
								VuVector3 vPos1 = VuDynamicsUtil::toVuVector3(pt.m_positionWorldOnB);
								VuVector3 vVel0 = VuDynamicsUtil::pointVelocityWorld(*pBody0, vPos0);
								VuVector3 vVel1 = VuDynamicsUtil::pointVelocityWorld(*pBody1, vPos1);
								//VuVector3 vNormal = VuDynamicsUtil::toVuVector3(pt.m_normalWorldOnB);

								VuVector3 vPos = 0.5f*(vPos0 + vPos1);

								float fSpeed = (vVel0 - vVel1).mag();
								fSpeed = VuMin(fSpeed/MAX_SCRAPE_SPEED, 1.0f);

								#if !VU_DISABLE_AUDIO
								if ( p->mpEvent )
								{
									FMOD_VECTOR pos = VuAudio::toFmodVector(vPos);
									p->mpEvent->set3DAttributes(&pos, VUNULL, VUNULL);

									FMOD::EventParameter *pParam;
									if ( p->mpEvent->getParameter("speed", &pParam) == FMOD_OK )
									{
										pParam->setValue(fSpeed);
									}

									if ( bNewManifold )
										p->mpEvent->start();
								}
								#endif // !VU_DISABLE_AUDIO

								//if ( VuPfxEntity *pPfxEntity = VuPfxManager::IF()->getEntity(p->mhPfx) )
								//{
								//	// set custom pfx process data
								//	VuPfxSystemInstance *pSystemInst = pPfxEntity->getSystemInstance()->getSystemInstance();
								//	for ( VuPfxPatternInstance *pPatternInst = pSystemInst->mPatterns.front(); pPatternInst; pPatternInst = pPatternInst->next() )
								//		for ( VuPfxProcessInstance *pProcessInst = pPatternInst->mProcesses.front(); pProcessInst; pProcessInst = pProcessInst->next() )
								//			if ( pProcessInst->mpParams->isDerivedFrom(VuPfxEmitFountain::msRTTI) )
								//				static_cast<VuPfxEmitFountainInstance *>(pProcessInst)->mSpawnPerSecondMultiplier = fSpeed;

								//	pPfxEntity->getSystemInstance()->setPosition(vPos);
								//	if ( bNewManifold )
								//		pPfxEntity->getSystemInstance()->start();
								//}

								p->mbActive = true;
							}
						}
					}
				}
			}
		}
	}

	// remove inactive manifolds
	for ( int i = 0; i < mManifolds.size(); i++ )
	{
		VuManifold *p = &mManifolds[i];
		if ( !p->mbActive )
		{
			#if !VU_DISABLE_AUDIO
			p->mpEvent->stop();
			#endif
			if ( VuPfxEntity *pPfxEntity = VuPfxManager::IF()->getEntity(p->mhPfx) )
				pPfxEntity->getSystemInstance()->stop();
			mManifolds.eraseSwap(i);
			i--;
		}
	}
}

//*****************************************************************************
void VuCollisionManager::updateDevStats()
{
	// dev stats
	if ( VuDevStatPage *pPage = VuDevStat::IF()->getCurPage() )
	{
		if ( pPage->getName() == "CollisionManager" )
		{
			pPage->clear();
			pPage->printf("Manifolds: %d\n", mManifolds.size());
		}
	}
}