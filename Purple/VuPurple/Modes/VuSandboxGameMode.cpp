//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  SandboxGameMode class
// 
//*****************************************************************************

#include "VuSandboxGameMode.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Projects/VuProject.h"
#include "VuEngine/HAL/Gfx/VuGfx.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Managers/VuViewportManager.h"
#include "VuEngine/Gfx/Camera/VuDebugCamera.h"
#include "VuEngine/Dev/VuDevConfig.h"


IMPLEMENT_RTTI(VuSandboxGameMode, VuGameMode);


// static variables
static VuDebugCamera sDebugCamera;


//*****************************************************************************
VuSandboxGameMode::VuSandboxGameMode():
	mpProject(VUNULL)
{
	// add keyboard callback
	VuKeyboard::IF()->addCallback(this);

	// register phased ticks
	VuTickManager::IF()->registerHandler(this, &VuSandboxGameMode::tickViewports, "Viewports");
}

//*****************************************************************************
VuSandboxGameMode::~VuSandboxGameMode()
{
	// remove keyboard callback
	VuKeyboard::IF()->removeCallback(this);

	// unregister phased tick
	VuTickManager::IF()->unregisterHandlers(this);
}

//*****************************************************************************
bool VuSandboxGameMode::enter(const std::string &prevMode)
{
	// load project directly from dev config
	const std::string &fileName = VuDevConfig::IF()->getParam("SandboxProject").asString();

	mpProject = new VuProject;
	if ( !mpProject->load(fileName) )
	{
		mpProject->removeRef();
		mpProject = VUNULL;

		return false;
	}

	mpProject->gameInitialize();

	// reset viewports
	VuViewportManager::IF()->reset();
	VuViewportManager::IF()->setViewportCount(1);

	// configure debug camera
	float aspectRatio = VuGameUtil::IF()->calcDisplayAspectRatio();
	float vertFov = VuGameUtil::IF()->calcCameraVertFov();
	sDebugCamera.setProjMatrixVert(vertFov, aspectRatio, 0.5f, 500.0f);

	return true;
}

//*****************************************************************************
void VuSandboxGameMode::exit()
{
	// unload project
	if ( mpProject )
	{
		mpProject->gameRelease();
		mpProject->removeRef();
	}

	// reset viewports
	VuViewportManager::IF()->reset();
}

//*****************************************************************************
const char *VuSandboxGameMode::tick(float fdt)
{
	return mNextGameMode.c_str();
}

//*****************************************************************************
void VuSandboxGameMode::draw()
{
}

//*****************************************************************************
void VuSandboxGameMode::onKeyDown(VUUINT32 key)
{
	if ( key == VUKEY_SPACE )
	{
		mNextGameMode = "Sandbox";
	}
}

//*****************************************************************************
void VuSandboxGameMode::tickViewports(float fdt)
{
	// update debug camera using real delta time
	sDebugCamera.tick(VuTickManager::IF()->getRealDeltaTime(), 0);
	VuViewportManager::IF()->setCamera(0, sDebugCamera);
	VuViewportManager::IF()->setUiCameraVert(sDebugCamera.getFovVert(), sDebugCamera.getAspectRatio(), sDebugCamera.getNearPlane(), sDebugCamera.getFarPlane());
}
