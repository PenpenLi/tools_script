//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuMultiBoingTransitionComponent
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Components/VuTransitionComponent.h"


class VuMultiBoingTransitionComponent : public VuTransitionBaseComponent
{
	DECLARE_SHORT_COMPONENT_TYPE(Transition)
	DECLARE_RTTI

public:
	VuMultiBoingTransitionComponent(VuEntity *pOwnerEntity);

	virtual void	onGameInitialize();

	virtual void	tick(float fdt);

	virtual void	transitionIn(bool force);
	virtual void	transitionOut(bool force);

	float			getTransitionTimer() { return mTransitionTimer; }

private:

	float			mTransitionTimer;
};
