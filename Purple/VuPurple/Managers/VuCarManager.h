//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  CarManager class
// 
//*****************************************************************************

#pragma once

#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuEngine/VuSystemComponent.h"
#include "VuEngine/Containers/VuArray.h"
#include "VuEngine/Containers/VuObjectArray.h"
#include "VuEngine/HAL/Keyboard/VuKeyboard.h"
#include "VuEngine/Gfx/Camera/VuDebugCamera.h"
#include "VuEngine/Managers/VuViewportManager.h"
#include "VuEngine/Dev/VuDevMenu.h"

class VuEntity;
class VuCarEntity;
class VuGhostCarEntity;
class VuActiveCameraTriggerEntity;

// constants
#define MAX_CAR_COUNT 16


class VuCarManager : public VuSystemComponent, VuKeyboard::Callback, VuDevMenu::Callback
{
	DECLARE_SYSTEM_COMPONENT(VuCarManager)

protected:
	// called by game
	friend class VuPurple;
	virtual bool init();
	virtual void release();

public:
	VuCarManager();

	void			addCar(VuCarEntity *pCar);
	void			removeCar(VuCarEntity *pCar);

	int				getCarCount()			{ return mAllArray.size(); }
	int				getLocalHumanCarCount()	{ return mLocalHumanArray.size(); }
	int				getAiCarCount()			{ return mAiArray.size(); }
	int				getRemoteCarCount()		{ return mRemoteArray.size(); }

	/*!
 	* \author: xlm 2014/12/19  
 	* \brief: 获取当前本地汽车
 	*/
	VuCarEntity*	getCurrLocalHumanCar();

	/*!
 	* \author: xlm 2014/12/19  
 	* \brief: 根据车名获取localhuman车的entity
 	*/
	VuCarEntity*	getLocalHumanCar(const std::string strCarName);


	VuCarEntity		*getCar(int index)				{ return mAllArray[index]; }
	VuCarEntity		*getLocalHumanCar(int index)	{ return mLocalHumanArray[index]; }
	VuCarEntity		*getAiCar(int index)			{ return mAiArray[index]; }
	VuCarEntity		*getRemoteCar(int index)		{ return mRemoteArray[index]; }

	VuCarEntity		*getCameraTargetForViewport(int viewport)		{ return mpCameraTargets[viewport]; }
	bool			getViewport(VuEntity *pEntity, int &viewport);

	void			addGridPosition(VuEntity *pEntity);
	void			removeGridPosition(VuEntity *pEntity);
	int				getGridPositionCount()		{ return mGridPositions.size(); }
	const VuEntity	*getGridPosition(int index)	{ return mGridPositions[index]; }

	void			setBoostBlitzStart(VuEntity *pEntity)	{ mpBoostBlitzStart = pEntity; }
	const VuEntity	*getBoostBlitzStart()					{ return mpBoostBlitzStart; }

	bool			isDevHudEnabled() { return mDevDrawHud; }

	void			setCameraOverride(const VuCamera *pCamera);
	bool			isCameraOverrideEnabled() { return mCameraOverrideEnabled; }

	int					getGhostCarCount()		{ return mGhostArray.size(); }
	VuGhostCarEntity	*getGhostCar(int index)	{ return mGhostArray[index]; }
	void				addGhostCar(VuGhostCarEntity *pGhostCar) { mGhostArray.push_back(pGhostCar); }
	void				removeGhostCar(VuGhostCarEntity *pGhostCar) { mGhostArray.remove(pGhostCar); }

private:
	// VuKeyboard::Callback
	virtual void	onKeyDown(VUUINT32 key);

	// VuDevMenu::Callback
	virtual void	onDevMenu(int param);

	void			updateCameraTargets();
	void			tickViewports(float fdt);

	void			configShaderLOD(int value) { mRefreshSkins = true; }

	typedef VuArray<VuCarEntity *> CarArray;
	typedef VuArray<VuGhostCarEntity *> GhostCarArray;
	typedef VuObjectArray<VuEntity *> GridPositions;

	CarArray		mAllArray;
	CarArray		mLocalHumanArray;
	CarArray		mAiArray;
	CarArray		mRemoteArray;
	GhostCarArray	mGhostArray;

	GridPositions	mGridPositions;
	VuEntity		*mpBoostBlitzStart;
	VuEntity		*mpTutorialStart;

	VuCarEntity		*mpCameraTargets[VuViewportManager::MAX_VIEWPORTS];
	bool			mDebugCameraActive;
	VuDebugCamera	mDebugCamera;
	bool			mDevDrawHud;

	VuActiveCameraTriggerEntity	*mpActiveCameraTriggerEntities[VuViewportManager::MAX_VIEWPORTS];

	bool			mCameraOverrideEnabled;
	VuCamera		mCameraOverride;

	bool			mRefreshSkins;

	// constants
	float			mMaxRadialBlur;
};
