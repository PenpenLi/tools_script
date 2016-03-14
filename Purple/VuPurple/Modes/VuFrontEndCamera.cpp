//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Front End Camera
// 
//*****************************************************************************

#include "VuFrontEndCamera.h"
#include "VuPurple/Gfx/GfxComposer/VuPurpleGfxComposer.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Managers/VuTickManager.h"
#include "VuEngine/Managers/VuViewportManager.h"


//*****************************************************************************
VuFrontEndCamera::VuFrontEndCamera():
	mTransitionTime(0.0f),
	mTimer(0.0f)
{
	// event handlers
	REG_EVENT_HANDLER(VuFrontEndCamera, OnSetFrontEndCamera);

	VuTickManager::IF()->registerHandler(this, &VuFrontEndCamera::tickViewports, "Viewports");
}

//*****************************************************************************
VuFrontEndCamera::~VuFrontEndCamera()
{
	VuTickManager::IF()->unregisterHandlers(this);
}

//*****************************************************************************
void VuFrontEndCamera::OnSetFrontEndCamera(const VuParams &params)
{
	State newState;
	VuParams::VuAccessor accessor(params);
	newState.mEye = accessor.getVector3();
	newState.mTarget = accessor.getVector3();
	newState.mUp = accessor.getVector3();

	if ( isDifferent(newState, mToState) )
	{
		mToState = newState;

		mTransitionTime = VuGameUtil::IF()->getFrontEndCameraTransitionTime();
		if ( mTransitionTime <= FLT_EPSILON )
			mCurState = mToState;

		mFromState = mCurState;

		mTimer = 0.0f;
	}
}

//*****************************************************************************
void VuFrontEndCamera::tickViewports(float fdt)
{
	// interpolate state
	if ( mTimer < mTransitionTime )
	{
		float ratio = mTimer/mTransitionTime;

		// apply ease-in/ease-out
		ratio = 3*ratio*ratio - 2*ratio*ratio*ratio;

		mCurState.mEye = VuLerp(mFromState.mEye, mToState.mEye, ratio);
		mCurState.mTarget = VuLerp(mFromState.mTarget, mToState.mTarget, ratio);
		mCurState.mUp = VuLerp(mFromState.mUp, mToState.mUp, ratio);
		mCurState.mUp.normalize();

		mTimer += fdt;
	}
	else
	{
		mCurState = mToState;
	}

	setViewMatrix(mCurState.mEye, mCurState.mTarget, mCurState.mUp);

	// configure camera proj matrix
	float aspectRatio = VuGameUtil::IF()->calcDisplayAspectRatio();
	float vertFov = VuGameUtil::IF()->calcCameraVertFov();
	float nearDist = VuGameUtil::IF()->constantDB()["FrontEnd"]["CameraNearDist"].asFloat();
	float farDist = VuGameUtil::IF()->constantDB()["FrontEnd"]["CameraFarDist"].asFloat();
	setProjMatrixVert(vertFov, aspectRatio, nearDist, farDist);

	VuViewportManager::IF()->setCamera(0, *this);
	VuViewportManager::IF()->setUiCameraVert(getFovVert(), getAspectRatio(), getNearPlane(), getFarPlane());
}

//*****************************************************************************
bool VuFrontEndCamera::isDifferent(const State &stateA, const State &stateB)
{
	if ( VuDistSquared(stateA.mEye, stateB.mEye) > FLT_EPSILON )
		return true;

	if ( VuDistSquared(stateA.mTarget, stateB.mTarget) > FLT_EPSILON )
		return true;

	if ( VuDistSquared(stateA.mUp, stateB.mUp) > FLT_EPSILON )
		return true;

	return false;
}
