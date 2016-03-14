//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  CommunityManager class
// 
//*****************************************************************************

#include "VuCommunityManager.h"


//*****************************************************************************
void VuCommunityManager::showMoreGames(const std::string &packageName, const std::string &publisherName)
{
	showWebPage("http://www.vectorunit.com/games");
}

//*****************************************************************************
void VuCommunityManager::showTwitterPage(const std::string &userName)
{
	char str[256];
	VU_SPRINTF(str, sizeof(str), "http://twitter.com/%s", userName.c_str());
	showWebPage(str);
}

//*****************************************************************************
void VuCommunityManager::showFacebookPage(const std::string &id, const std::string &userName)
{
	char str[256];
	VU_SPRINTF(str, sizeof(str), "https://facebook.com/%s", userName.c_str());
	showWebPage(str);
}

//*****************************************************************************
void VuCommunityManager::showGooglePlusPage(const std::string &id)
{
	char str[256];
	VU_SPRINTF(str, sizeof(str), "https://plus.google.com/%s", id.c_str());
	showWebPage(str);
}

//*****************************************************************************
void VuCommunityManager::rateGame(const std::string &packageName)
{
	showWebPage("http://www.vectorunit.com/games");
}
