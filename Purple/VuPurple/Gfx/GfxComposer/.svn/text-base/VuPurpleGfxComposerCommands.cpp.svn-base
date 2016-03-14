//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Game-specific composer commands
// 
//*****************************************************************************

#include "VuPurpleGfxComposerCommands.h"
#include "VuPurple/Managers/VuVideoShareManager.h"
#include "VuEngine/Gfx/GfxSort/VuGfxSort.h"


//*****************************************************************************
void VuPurpleGfxComposerCommands::submitVideoShareBegin()
{
	struct CommandData
	{
		static void callback(void *data)
		{
			VuVideoShareManager::IF()->beginDraw();
		}
	};

	VuGfxSort::IF()->submitCommand(VuGfxSort::TRANS_BEGIN, 0, &CommandData::callback);
}

//*****************************************************************************
void VuPurpleGfxComposerCommands::submitVideoShareEnd()
{
	struct CommandData
	{
		static void callback(void *data)
		{
			VuVideoShareManager::IF()->endDraw();
		}
	};

	VuGfxSort::IF()->submitCommand(VuGfxSort::TRANS_BEGIN, 0, &CommandData::callback);
}
