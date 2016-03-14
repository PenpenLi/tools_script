//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Stub ExpansionFile manager
//
//*****************************************************************************

#include "VuPurple/Managers/VuExpansionFileManager.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Math/VuMath.h"


#define SIMULATED_PROGRESS_DURATION 5.0f // seconds


class VuStubExpansionFileManager : public VuExpansionFileManager
{
public:
	VuStubExpansionFileManager();

	virtual void	startDownloadInternal();

protected:
	void			tick(float fdt);

	float			mSimulationTimer;
};


// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuExpansionFileManager, VuStubExpansionFileManager);


//*****************************************************************************
VuStubExpansionFileManager::VuStubExpansionFileManager():
	mSimulationTimer(0.0f)
{
}

//*****************************************************************************
void VuStubExpansionFileManager::startDownloadInternal()
{
	VuTickManager::IF()->registerHandler(this, &VuStubExpansionFileManager::tick, "Decision");
}

//*****************************************************************************
void VuStubExpansionFileManager::tick(float fdt)
{
	mSimulationTimer += fdt;

	float progress = VuMin(mSimulationTimer/SIMULATED_PROGRESS_DURATION, 1.0f);

	onDownloadProgress(progress);

	if ( mSimulationTimer >= SIMULATED_PROGRESS_DURATION )
	{
		onDownloadResult();

		VuTickManager::IF()->unregisterHandler(this, "Decision");
	}
}
