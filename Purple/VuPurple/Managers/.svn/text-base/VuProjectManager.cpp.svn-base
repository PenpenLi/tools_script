//*****************************************************************************
//
//  Copyright (c) 2009-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  ProjectManager class
// 
//*****************************************************************************

#include "VuProjectManager.h"
#include "VuEngine/Projects/VuProject.h"
#include "VuEngine/Assets/VuAssetFactory.h"
#include "VuEngine/Assets/VuProjectAsset.h"
#include "VuEngine/Gfx/GfxSort/VuGfxSort.h"


// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuProjectManager, VuProjectManager);


//*****************************************************************************
void VuProjectManager::release()
{
}

//*****************************************************************************
VuProject *VuProjectManager::load(const std::string &assetName)
{
    	VuGfxSort::IF()->flush();

	VuProject *pProject = VUNULL;

	if ( VuAssetFactory::IF()->doesAssetExist<VuProjectAsset>(assetName) )
	{
		pProject = new VuProject;

		VuProjectAsset *pProjectAsset = VuAssetFactory::IF()->createAsset<VuProjectAsset>(assetName);
		pProject->load(pProjectAsset);
		VuAssetFactory::IF()->releaseAsset(pProjectAsset);
	}

	return pProject;
}

//*****************************************************************************
VuProject *VuProjectManager::load(VuProjectAsset *pProjectAsset)
{
	VuGfxSort::IF()->flush();
	VuProject *pProject = new VuProject;
	pProject->load(pProjectAsset);
	return pProject;
}

//*****************************************************************************
void VuProjectManager::unload(VuProject *pProject)
{
	if ( pProject )
	{
		VuGfxSort::IF()->flush();
		pProject->removeRef();
	}
}