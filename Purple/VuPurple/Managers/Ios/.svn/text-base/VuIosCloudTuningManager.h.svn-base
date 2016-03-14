//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
//
//  VuIosCloudTuningManager
//
//*****************************************************************************

#include "VuPurple/Managers/VuCloudTuningManager.h"


class VuIosCloudTuningManager : public VuCloudTuningManager
{
public:
	static void		setConfigurationReceived() { sGameConfigurationReceived = true; }
	
protected:
	virtual bool	wasGameConfigurationReceived() { return sGameConfigurationReceived; }
	virtual void	getGameConfigurationValue(const std::string &key, std::string &value);
	
	static bool		sGameConfigurationReceived;
};
