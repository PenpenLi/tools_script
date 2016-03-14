//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Windows CloudSave manager
//
//*****************************************************************************

#include "VuPurple/Managers/VuCloudSaveManager.h"


class VuWindowsCloudSaveManager : public VuCloudSaveManager
{
public:
	VuWindowsCloudSaveManager();

	// platform-specific functionality
	static VuWindowsCloudSaveManager *IF() { return static_cast<VuWindowsCloudSaveManager *>(VuCloudSaveManager::IF()); }

	void			onDataChanged();

protected:
	virtual void	startCloudLoad();
	virtual void	startCloudSave(const VuArray<VUBYTE> &blob);

	void			readFile(VuArray<VUBYTE> &blob);
	void			writeFile(const VuArray<VUBYTE> &blob);

	bool			mCloudLoadStarted;
};
