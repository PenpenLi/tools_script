//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Fade Manager
// 
//*****************************************************************************

#include "VuFadeManager.h"
#include "VuPurple/Gfx/GfxComposer/VuPurpleGfxComposer.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Managers/VuDrawManager.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/GfxSort/VuGfxSort.h"
#include "VuEngine/Math/VuMath.h"


// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuFadeManager, VuFadeManager);


//*****************************************************************************
VuFadeManager::VuFadeManager():
	mDefaultDuration(1.0f),
	mFadeAmount(0.0f),
	mFadeRate(0.0f),
	mSkipFrameCount(0)
{
}

//*****************************************************************************
bool VuFadeManager::init()
{
	VuTickManager::IF()->registerHandler(this, &VuFadeManager::tick, "Final");
	VuDrawManager::IF()->registerHandler(this, &VuFadeManager::draw);

	// read constants
	VuGameUtil::IF()->constantDB()["UI"]["FadeManagerDefaultDuration"].getValue(mDefaultDuration);

	return true;
}

//*****************************************************************************
void VuFadeManager::release()
{
	VuTickManager::IF()->unregisterHandlers(this);
	VuDrawManager::IF()->unregisterHandler(this);
}

//*****************************************************************************
void VuFadeManager::startFadeIn(float duration)
{
	if ( duration < 0 )
		duration = mDefaultDuration;

	if ( duration > 0 )
	{
		mFadeAmount = 1.0f;
		mFadeRate = -1.0f/duration;
	}
	else
	{
		mFadeAmount = 0.0f;
		mFadeRate = 0.0f;
	}
}

//*****************************************************************************
void VuFadeManager::startFadeOut(float duration)
{
	if ( duration < 0 )
		duration = mDefaultDuration;

	if ( duration > 0 )
	{
		mFadeAmount = 0.0f;
		mFadeRate = 1.0f/duration;
	}
	else
	{
		mFadeAmount = 1.0f;
		mFadeRate = 0.0f;
	}
}

//*****************************************************************************
void VuFadeManager::tick(float fdt)
{
	if ( mSkipFrameCount )
	{
		mSkipFrameCount--;
	}
	else
	{
		// use real dt
		fdt = VuTickManager::IF()->getRealDeltaTime();

		if ( mFadeRate != 0 )
		{
			mFadeAmount += mFadeRate*fdt;
			if ( mFadeAmount > 1 )
			{
				VuEventManager::IF()->broadcast("OnFadeOutComplete");
				mFadeAmount = 1;
				mFadeRate = 0;
			}
			else if ( mFadeAmount < 0 )
			{
				VuEventManager::IF()->broadcast("OnFadeInComplete");
				mFadeAmount = 0;
				mFadeRate = 0;
			}
		}
	}
}

//*****************************************************************************
void VuFadeManager::draw()
{
	if ( mFadeAmount > 0 )
	{
		VuGfxSort::IF()->setFullScreenLayer(VuGfxSort::FSL_FADE);
		VuGfxSort::IF()->setViewport(0);
		VuGfxSort::IF()->setReflectionLayer(0);
		VuGfxSort::IF()->setViewportLayer(VuGfxSort::VPL_BEGIN);

		VuGfxUtil::IF()->submitSetViewportCommand(VuRect(0,0,1,1));

		VuGfxSort::IF()->setViewportLayer(VuGfxSort::VPL_UI);

		VuColor color;
		color.fromFloat4(0.0f, 0.0f, 0.0f, mFadeAmount);
		VuGfxUtil::IF()->drawFilledRectangle2d(0.0f, color);
	}
}