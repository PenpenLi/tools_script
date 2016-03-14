//*****************************************************************************
//
//  Copyright (c) 2008-2008 Ralf Knoesel
//  Copyright (c) 2008-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  ResultGameMode class
// 
//*****************************************************************************

#include "VuResultGameMode.h"
#include "VuPurple/Managers/VuFadeManager.h"
#include "VuEngine/Managers/VuViewportManager.h"
#include "VuEngine/Dev/VuDevConsole.h"


IMPLEMENT_RTTI(VuResultGameMode, VuGameMode);


//*****************************************************************************
bool VuResultGameMode::enter(const std::string &prevMode)
{
	// instantly fade out
	VuFadeManager::IF()->startFadeOut(0.0f);

	VuDevConsole::IF()->show(true);
	VuViewportManager::IF()->reset();
	VuViewportManager::IF()->setViewportCount(1);

	return true;
}

//*****************************************************************************
void VuResultGameMode::exit()
{
	VuViewportManager::IF()->reset();
	VuDevConsole::IF()->show(false);
}
