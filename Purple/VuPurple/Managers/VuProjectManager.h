//*****************************************************************************
//
//  Copyright (c) 2009-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  ProjectManager class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/VuSystemComponent.h"

class VuProject;
class VuProjectAsset;


class VuProjectManager : public VuSystemComponent
{
	DECLARE_SYSTEM_COMPONENT(VuProjectManager)

protected:
	// called by game
	friend class VuPurple;
	virtual bool init() { return true; }
	virtual void release();

public:
	VuProject	*load(const std::string &assetName);
	VuProject	*load(VuProjectAsset *pProjectAsset);
	void		unload(VuProject *pProject);
};
