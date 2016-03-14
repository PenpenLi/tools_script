//*****************************************************************************
//
//  Copyright (c) 2012-2013 Vector Unit Inc
//  Confidential Trade Secrets
//
//  Ios CloudDataManager class
//
//*****************************************************************************

#include "VuIosCloudSaveManagerObjC.h"
#include "VuPurple/Managers/VuCloudSaveManager.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/HAL/File/Ios/VuIosFile.h"
#include "VuEngine/Util/VuBase64.h"


class VuIosCloudSaveManager : public VuCloudSaveManager, VuIosCloudSaveManagerObjC::CallbackIF
{
public:
	VuIosCloudSaveManager();
	
	virtual bool	init();
	virtual void	release();
	
protected:
	virtual void	startCloudLoad();
	virtual void	startCloudSave(const VuArray<VUBYTE> &blob);
	
	void			tick(float fdt);
	
	// VuIosCloudDataManagerObjC::CallbackIF
	virtual bool	onCloudMerge(const void *data, int dataSize);
	
	bool			mCloudAvailable;
	bool			mCloudLoadStarted;
	VuArray<VUBYTE>	mCloudData;
};

// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuCloudSaveManager, VuIosCloudSaveManager);


//*****************************************************************************
VuIosCloudSaveManager::VuIosCloudSaveManager():
	mCloudAvailable(false),
	mCloudLoadStarted(false)
{
}

//*****************************************************************************
bool VuIosCloudSaveManager::init()
{
	if ( !VuCloudSaveManager::init() )
		return false;
	
	VuIosCloudSaveManagerObjC::setCallback(this);
	
	mCloudAvailable = VuIosCloudSaveManagerObjC::start("BBRCloudData.bin", true);
	
	VuTickManager::IF()->registerHandler(this, &VuIosCloudSaveManager::tick, "Decision");
	
	return true;
}

//*****************************************************************************
void VuIosCloudSaveManager::release()
{
	VuTickManager::IF()->unregisterHandlers(this);
	
	VuIosCloudSaveManagerObjC::stop();
	
	VuCloudSaveManager::release();
}

//*****************************************************************************
void VuIosCloudSaveManager::startCloudLoad()
{
	onCloudLoadResult(mCloudData);
	mCloudLoadStarted = true;
}

//*****************************************************************************
void VuIosCloudSaveManager::startCloudSave(const VuArray<VUBYTE> &blob)
{
	if ( mCloudLoadStarted )
		VuIosCloudSaveManagerObjC::write(&blob[0], blob.size());
}

//*****************************************************************************
void VuIosCloudSaveManager::tick(float fdt)
{
	VuIosCloudSaveManagerObjC::tick(fdt);
}

//*****************************************************************************
bool VuIosCloudSaveManager::onCloudMerge(const void *data, int dataSize)
{
	mCloudData.resize(dataSize);
	
	if ( dataSize )
	{
		memcpy(&mCloudData[0], data, dataSize);

		if ( mCloudLoadStarted )
			onCloudLoadResult(mCloudData);
	}
	
	return true;
}
