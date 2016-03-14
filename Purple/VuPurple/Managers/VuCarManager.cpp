//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  CarManager class
// 
//*****************************************************************************

#include "VuCarManager.h"
#include "VuPurple/Entities/Car/Driver/VuDriverEntity.h"
#include "VuPurple/Entities/Car/Camera/VuCarCamera.h"
#include "VuPurple/Managers/VuControlMethodManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Components/Instigator/VuInstigatorComponent.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Managers/VuTriggerManager.h"
#include "VuEngine/Managers/VuLensWaterManager.h"
#include "VuEngine/Managers/VuConfigManager.h"
#include "VuEngine/Managers/VuProfileManager.h"
#include "VuEngine/Water/VuWater.h"
#include "VuEngine/HAL/Gfx/VuGfx.h"
#include "VuEngine/HAL/Audio/VuAudio.h"
#include "VuEngine/Gfx/GfxSort/VuGfxSort.h"

// modify by xlm,[/2014/12/19 ] 
#include "VuPurple/Managers/VuGameManager.h"


// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuCarManager, VuCarManager);


// static functions
static bool GridPositionComp(const VuEntity *p1, const VuEntity *p2)
{
	return p1->getShortName() < p2->getShortName();
}

// entity used for active camera triggers
class VuActiveCameraTriggerEntity : public VuEntity
{
public:
	VuActiveCameraTriggerEntity();

	VuInstigatorComponent	*mpInstigatorComponent;
};

enum { DEV_MENU_SAVE, DEV_MENU_LOAD };


//*****************************************************************************
VuCarManager::VuCarManager():
	mpBoostBlitzStart(VUNULL),
	mpTutorialStart(VUNULL),
	mDebugCameraActive(false),
	mDevDrawHud(true),
	mCameraOverrideEnabled(false),
	mRefreshSkins(false),
	mMaxRadialBlur(0.1f)
{
	memset(mpCameraTargets, 0, sizeof(mpCameraTargets));
	memset(mpActiveCameraTriggerEntities, 0, sizeof(mpActiveCameraTriggerEntities));

	VuDevMenu::IF()->addBool("Dev/Draw HUD", mDevDrawHud);
	VuDevMenu::IF()->addCallback("DebugCamera/Save", this, DEV_MENU_SAVE);
	VuDevMenu::IF()->addCallback("DebugCamera/Load", this, DEV_MENU_LOAD);
}

//*****************************************************************************
bool VuCarManager::init()
{
	// register phased tick
	VuTickManager::IF()->registerHandler(this, &VuCarManager::tickViewports, "Viewports");

	// add keyboard callback
	VuKeyboard::IF()->addCallback(this);

	// configure debug camera proj matrix
	float aspectRatio = VuGameUtil::IF()->calcDisplayAspectRatio();
	float vertFov = VuGameUtil::IF()->calcCameraVertFov();
	mDebugCamera.setProjMatrixVert(vertFov, aspectRatio, 1.0f, 500.0f);

	// create active camera trigger entities
	for ( int i = 0; i < VuViewportManager::MAX_VIEWPORTS; i++ )
	{
		char strShortName[64];
		VU_SPRINTF(strShortName, sizeof(strShortName), "ActiveCameraTrigger%d", i);
		mpActiveCameraTriggerEntities[i] = new VuActiveCameraTriggerEntity();
		mpActiveCameraTriggerEntities[i]->setShortName(strShortName);
		mpActiveCameraTriggerEntities[i]->load(VuJsonContainer::null);
		mpActiveCameraTriggerEntities[i]->postLoad();
		mpActiveCameraTriggerEntities[i]->gameInitialize();
	}

	VuConfigManager::IF()->registerIntHandler("Gfx/ShaderLOD", this, &VuCarManager::configShaderLOD);

	// constants
	VuGameUtil::IF()->constantDB()["Effects"]["MaxRadialBlur"].getValue(mMaxRadialBlur);

	return true;
}






//*****************************************************************************
void VuCarManager::release()
{
	VuConfigManager::IF()->unregisterIntHandler("Gfx/ShaderLOD", this);

	// destroy active camera trigger entities
	for ( int i = 0; i < VuViewportManager::MAX_VIEWPORTS; i++ )
	{
		mpActiveCameraTriggerEntities[i]->gameRelease();
		mpActiveCameraTriggerEntities[i]->removeRef();
	}

	VUASSERT(mAllArray.size() == 0, "VuCarManager::release() dangling jet skis");

	// remove keyboard callback
	VuKeyboard::IF()->removeCallback(this);

	// unregister phased tick
	VuTickManager::IF()->unregisterHandlers(this);
}

//*****************************************************************************
void VuCarManager::addCar(VuCarEntity *pCar)
{
	if ( mLocalHumanArray.size() == 0 )
		mpCameraTargets[0] = pCar;

	if ( pCar->getDriver()->isHuman() && pCar->getDriver()->isLocal() )
	{
		int viewport = VuMin(mLocalHumanArray.size(), VuViewportManager::MAX_VIEWPORTS - 1);
		mpCameraTargets[viewport] = pCar;

		mLocalHumanArray.push_back(pCar);
	}
	else if ( pCar->getDriver()->isAi() )
	{
		mAiArray.push_back(pCar);
	}

	if ( pCar->getDriver()->isRemote() )
	{
		mRemoteArray.push_back(pCar);
	}

	mAllArray.push_back(pCar);

	updateCameraTargets();
}

//*****************************************************************************
void VuCarManager::removeCar(VuCarEntity *pCar)
{
	mAllArray.remove(pCar);
	mLocalHumanArray.remove(pCar);
	mAiArray.remove(pCar);
	mRemoteArray.remove(pCar);

	for ( int i = 0; i < VuViewportManager::MAX_VIEWPORTS; i++ )
		if ( mpCameraTargets[i] == pCar )
			mpCameraTargets[i] = VUNULL;

	updateCameraTargets();
}

//*****************************************************************************
void VuCarManager::updateCameraTargets()
{
	if ( mLocalHumanArray.size() )
	{
		for ( int i = 0; i < mLocalHumanArray.size(); i++ )
			mpCameraTargets[i] = mLocalHumanArray[i];

		for ( int i = 0; i < mAllArray.size(); i++ )
			mAllArray[i]->setIsCameraTarget(mAllArray[i]->getDriver()->isLocal() && mAllArray[i]->getDriver()->isHuman());
	}
	else
	{
		for ( int i = 0; i < VuViewportManager::MAX_VIEWPORTS; i++ )
			mpCameraTargets[i] = VUNULL;

		if ( mAllArray.size() )
		{
			int lastIndex = mAllArray.size() - 1;

			mpCameraTargets[0] = mAllArray[lastIndex];
			mAllArray[lastIndex]->setIsCameraTarget(true);

			for ( int i = 0; i < lastIndex; i++ )
				mAllArray[i]->setIsCameraTarget(false);
		}
	}
}

//*****************************************************************************
void VuCarManager::tickViewports(float fdt)
{
	if ( mRefreshSkins )
	{
		for ( int i = 0; i < mAllArray.size(); i++ )
			mAllArray[i]->updateSkin();
	}

	// update debug camera using real delta time
	if ( mDebugCameraActive )
		mDebugCamera.tick(VuTickManager::IF()->getRealDeltaTime(), VuControlMethodManager::IF()->recentPadIndex());

	if ( mAllArray.size() )
	{
		for ( int iViewport = 0; iViewport < VuViewportManager::IF()->getViewportCount(); iViewport++ )
		{
			if ( mCameraOverrideEnabled )
			{
				VuViewportManager::IF()->setCamera(iViewport, mCameraOverride);
			}
			else if ( mDebugCameraActive )
			{
				// update debug camera using real delta time
				VuViewportManager::IF()->setCamera(iViewport, mDebugCamera);
			}
			else
			{
				VuCarEntity *pCar = mpCameraTargets[iViewport];

				VuViewportManager::IF()->setCamera(iViewport, *pCar->getCamera());

				// set full-screen effects
				VuViewportManager::IF()->setRadialBlur(iViewport, mMaxRadialBlur*pCar->getRadialBlurAmount());
				VuLensWaterManager::IF()->setRadialSpread(iViewport, pCar->getLensWaterRadialSpreadAmount());
			}
		}
	}

	// ui camera
	{
		float vertFov = VuGameUtil::IF()->calcCameraVertFov();
		float aspectRatio = VuGameUtil::IF()->calcDisplayAspectRatio();
		float nearDist = VuGameUtil::IF()->constantDB()["FrontEnd"]["CameraNearDist"].asFloat();
		float farDist = VuGameUtil::IF()->constantDB()["FrontEnd"]["CameraFarDist"].asFloat();
		VuViewportManager::IF()->setUiCameraVert(vertFov, aspectRatio, nearDist, farDist);
	}

	// listeners and active camera trigger entities
	VuAudio::IF()->setListenerCount(VuViewportManager::IF()->getViewportCount());
	for ( int i = 0; i < VuViewportManager::IF()->getViewportCount(); i++ )
	{
		const VuCamera &camera = VuViewportManager::IF()->getViewport(i).mCamera;
		const VuMatrix &matCam = camera.getTransform();

		VuAudio::IF()->setListenerAttributes(i, matCam.getTrans(), camera.getListenerVelocity(), matCam.getAxisY(), matCam.getAxisZ());

		mpActiveCameraTriggerEntities[i]->getTransformComponent()->setWorldPosition(matCam.getTrans(), false);

		if ( mAllArray.size() )
			mpActiveCameraTriggerEntities[i]->mpInstigatorComponent->enable();
		else
			mpActiveCameraTriggerEntities[i]->mpInstigatorComponent->disable();
	}

	for ( int i = VuViewportManager::IF()->getViewportCount(); i < VuViewportManager::MAX_VIEWPORTS; i++ )
		mpActiveCameraTriggerEntities[i]->mpInstigatorComponent->disable();
}

//*****************************************************************************
bool VuCarManager::getViewport(VuEntity *pEntity, int &viewport)
{
	for ( int i = 0; i < VuViewportManager::MAX_VIEWPORTS; i++ )
	{
		if ( pEntity == mpCameraTargets[i] )
		{
			viewport = i;
			return true;
		}
	}

	return false;
}

//*****************************************************************************
void VuCarManager::addGridPosition(VuEntity *pEntity)
{
	mGridPositions.push_back(pEntity);
	mGridPositions.quickSort(GridPositionComp);
}

//*****************************************************************************
void VuCarManager::removeGridPosition(VuEntity *pEntity)
{
	mGridPositions.remove(pEntity);
}

//*****************************************************************************
void VuCarManager::setCameraOverride(const VuCamera *pCamera)
{
	if ( pCamera )
	{
		mCameraOverrideEnabled = true;
		mCameraOverride = *pCamera;
	}
	else
	{
		mCameraOverrideEnabled = false;
	}
}

//*****************************************************************************
void VuCarManager::onKeyDown(VUUINT32 key)
{
	if ( mAllArray.size() )
	{
		if ( key == VUKEY_C )
		{
			mDebugCameraActive = !mDebugCameraActive;
			if ( mDebugCameraActive )
			{
				mDebugCamera = *mpCameraTargets[0]->getCamera();
			}
			else
			{
				if ( VuKeyboard::IF()->isKeyDown(VUKEY_SHIFT) )
				{
					VuMatrix mat = mDebugCamera.getTransform();
					mat.setTrans(mDebugCamera.getTargetPosition());
					mpCameraTargets[0]->recoverTo(mat.getTrans(), mat.getEulerAngles(), false, true);
				}
			}
		}
		else if ( key == VUKEY_V )
		{
			mDebugCameraActive = false;
			for ( int i = 0; i < mAllArray.size(); i++ )
			{
				if ( mpCameraTargets[0] == mAllArray[i] )
				{
					mpCameraTargets[0] = mAllArray[(i+1)%mAllArray.size()];
					break;
				}
			}
		}
		else if ( key == VUKEY_X )
		{
			mpCameraTargets[0]->startCrash();
		}
	}
}

//*****************************************************************************
void VuCarManager::onDevMenu(int param)
{
	if ( param == DEV_MENU_SAVE )
	{
		VuJsonContainer &data = VuProfileManager::IF()->dataWrite()["DebugCamera"];
		VuDataUtil::putValue(data["Eye"], mDebugCamera.getEyePosition());
		VuDataUtil::putValue(data["Target"], mDebugCamera.getTargetPosition());
	}
	else if ( param == DEV_MENU_LOAD )
	{
		const VuJsonContainer &data = VuProfileManager::IF()->dataRead()["DebugCamera"];

		VuVector3 eyePos = mDebugCamera.getEyePosition();
		VuVector3 targetPos = mDebugCamera.getTargetPosition();

		VuDataUtil::getValue(data["Eye"], eyePos);
		VuDataUtil::getValue(data["Target"], targetPos);

		mDebugCamera.setViewMatrix(eyePos, targetPos, VuVector3(0,0,1));
	}
}

//*****************************************************************************
VuActiveCameraTriggerEntity::VuActiveCameraTriggerEntity()
{
	addComponent(mpInstigatorComponent = new VuInstigatorComponent(this));

	VUUINT32 mask = VuTriggerManager::getTypeMask("Camera");
	mpInstigatorComponent->setMask(mask);
}




/*!
* \author: xlm 2014/12/19
* \brief: 获取当前本地汽车
*/
VuCarEntity*	VuCarManager::getCurrLocalHumanCar()
{
	std::string strCarName = VuGameManager::IF()->getCurCarName();
	for (int i = 0; i < mLocalHumanArray.size(); i++)
	{
		if (mLocalHumanArray[i] != NULL && mLocalHumanArray[i]->getCarName() == strCarName)
		{
			return mLocalHumanArray[i];
		}
	}
	return NULL;
}



// modify by xlm,[/2014/12/19 ] 
VuCarEntity*	VuCarManager::getLocalHumanCar(const std::string strCarName)
{
	for (int i = 0; i < mLocalHumanArray.size(); i++)
	{
		if (mLocalHumanArray[i] != NULL && mLocalHumanArray[i]->getCarName() == strCarName)
		{
			return mLocalHumanArray[i];
		}
	}
	return NULL;
}