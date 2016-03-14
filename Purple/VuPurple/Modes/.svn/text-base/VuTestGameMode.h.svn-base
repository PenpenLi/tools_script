//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  TestGameMode class
// 
//*****************************************************************************

#pragma once

#include "VuGameMode.h"

class VuGfxSortMesh;
class VuTextureAsset;


class VuTestGameMode : public VuGameMode
{
public:
	VuTestGameMode();
	~VuTestGameMode();

	virtual bool		enter(const std::string &prevMode);
	virtual void		exit();
	virtual const char	*tick(float fdt);
	virtual void		draw();

private:
	struct Vertex2dXyzUv
	{
		float		mXyz[3];
		float		mUv[2];
	};

	static void			staticDrawCallback(void *data) { (*static_cast<VuTestGameMode **>(data))->drawCallback(); }
	void				drawCallback();

	VuGfxSortMesh		*mpGfxSortMesh;
	VuTextureAsset		*mpTextureAsset;

	Vertex2dXyzUv		mVerts[4];
	VUUINT16			mInds[4];
};
