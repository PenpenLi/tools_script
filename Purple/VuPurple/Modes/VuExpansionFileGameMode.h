//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuExpansionFileGameMode
// 
//*****************************************************************************

#pragma once

#include "VuGameMode.h"
#include "VuPurple/Managers/VuExpansionFileManager.h"

class VuUIScreenEntity;
class VuProject;


class VuExpansionFileGameMode : public VuGameMode, public VuExpansionFileManager::VuListener
{
	DECLARE_RTTI

public:
	VuExpansionFileGameMode();

	virtual bool		enter(const std::string &prevMode);
	virtual void		exit();
	virtual const char	*tick(float fdt);
	virtual void		draw();

protected:
	// VuExpansionFileManager::VuListener
	virtual void		onDownloadSuccess();
	virtual void		onDownloadFailure();
	virtual void		onDownloadProgress(float progress);

	VuUIScreenEntity	*getScreen();

	VuProject			*mpScreenProject;

	enum eState { STATE_DOWNLOADING, STATE_SUCCESS, STATE_FAILURE };
	eState				mState;
	std::string			mNextGameMode;
};
