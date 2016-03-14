//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Win32 Community manager
//
//*****************************************************************************

#include "VuPurple/Managers/VuCommunityManager.h"


class VuWin32CommunityManager : public VuCommunityManager
{
public:
	virtual void		showWebPage(const std::string &url);
};


// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuCommunityManager, VuWin32CommunityManager);


//*****************************************************************************
void VuWin32CommunityManager::showWebPage(const std::string &url)
{
	int result = (int)ShellExecute(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
	if ( result < 32 )
	{
		const char *msg = "Unknown error";
		if ( result == ERROR_FILE_NOT_FOUND ) msg = "File not found";
		if ( result == SE_ERR_NOASSOC ) msg = "No association for .json file extension";

		VUWARNING(msg);
	}
}