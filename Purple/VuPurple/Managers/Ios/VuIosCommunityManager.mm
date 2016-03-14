//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  iOS Community manager
//
//*****************************************************************************

#include "VuPurple/Managers/VuCommunityManager.h"


class VuIosCommunityManager : public VuCommunityManager
{
public:
	virtual void		showWebPage(const std::string &url);
};


// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuCommunityManager, VuIosCommunityManager);


//*****************************************************************************
void VuIosCommunityManager::showWebPage(const std::string &url)
{
	NSURL *nativeURL = [NSURL URLWithString:[NSString stringWithUTF8String:url.c_str()]];
	[[UIApplication sharedApplication] openURL:nativeURL];
}