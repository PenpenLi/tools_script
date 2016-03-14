//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuMultiBoingTransitionComponent
// 
//*****************************************************************************

#include "VuMultiBoingTransitionComponent.h"


IMPLEMENT_RTTI(VuMultiBoingTransitionComponent, VuTransitionBaseComponent);


//*****************************************************************************
VuMultiBoingTransitionComponent::VuMultiBoingTransitionComponent(VuEntity *pOwnerEntity) : VuTransitionBaseComponent(pOwnerEntity),
	mTransitionTimer(0.0f)
{
}

//*****************************************************************************
void VuMultiBoingTransitionComponent::onGameInitialize()
{
	if ( mBehavior == BEHAVIOR_MANUAL_IN )
	{
		mState = STATE_ACTIVE;
	}
	else if ( mBehavior == BEHAVIOR_MANUAL_OUT )
	{
		mState = STATE_OFF;
	}
}

//*****************************************************************************
void VuMultiBoingTransitionComponent::tick(float fdt)
{
	if ( mState != STATE_OFF )
		mTransitionTimer += fdt;

	if ( mState == STATE_TRANS_IN )
	{
		if ( mTransitionTimer > mTransitionDuration )
			mState = STATE_ACTIVE;
	}
	else if ( mState == STATE_TRANS_OUT )
	{
		if ( mTransitionTimer > mTransitionDuration )
			mState = STATE_OFF;
	}
}

//*****************************************************************************
void VuMultiBoingTransitionComponent::transitionIn(bool force)
{
	if ( mBehavior != BEHAVIOR_AUTOMATIC && !force)
		return;

	mState = STATE_TRANS_IN;
	mTransitionTimer = 0.0f;
}

//*****************************************************************************
void VuMultiBoingTransitionComponent::transitionOut(bool force)
{
	if ( mBehavior != BEHAVIOR_AUTOMATIC && !force)
		return;

	mState = STATE_TRANS_OUT;
	mTransitionTimer = 0.0f;
}
