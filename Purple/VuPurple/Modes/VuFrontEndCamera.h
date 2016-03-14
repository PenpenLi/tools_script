//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Front End Camera
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Gfx/Camera/VuCamera.h"
#include "VuEngine/Events/VuEventMap.h"


class VuFrontEndCamera : public VuCamera
{
	DECLARE_EVENT_MAP

public:
	VuFrontEndCamera();
	~VuFrontEndCamera();

protected:
	struct State
	{
		State() : mEye(0,0,0), mTarget(0,1,0), mUp(0,0,1) {}
		VuVector3	mEye;
		VuVector3	mTarget;
		VuVector3	mUp;
	};

	// event handlers
	void		OnSetFrontEndCamera(const VuParams &params);

	void		tickViewports(float fdt);

	bool		isDifferent(const State &stateA, const State &stateB);

	State		mFromState;
	State		mCurState;
	State		mToState;
	float		mTransitionTime;
	float		mTimer;
};
