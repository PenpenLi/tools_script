//*****************************************************************************
//
//  Copyright (c) 2010-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  PreviewGameMode class
// 
//*****************************************************************************

#pragma once

#include "VuGameMode.h"
#include "VuEngine/Gfx/Camera/VuDebugCamera.h"

class VuPreviewOptions;
class VuPreviewEntity;
class VuColor;


class VuPreviewGameMode : public VuGameMode
{
	DECLARE_RTTI

public:
	VuPreviewGameMode();
	~VuPreviewGameMode();

	virtual bool		enter(const std::string &prevMode);
	virtual void		exit();
	virtual const char	*tick(float fdt);
	virtual void		draw();

private:
	void				drawGrid(const VuMatrix &mat, const VuColor &color);
	void				drawLightVector(const VuCamera &camera, const VuColor &color);

	void				tickLight(float fdt);
	VuVector3			calcLightDirection();

	// options
	VuPreviewOptions	*mpOptions;

	// the scene
	VuPreviewEntity		*mpPreviewEntity;

	// camera
	VuDebugCamera		mDebugCamera;

	// directional light
	float				mLightRotX;
	float				mLightRotZ;
};
