//*****************************************************************************
//
//  Copyright (c) 2012-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Windows Community manager
//
//*****************************************************************************

#include "VuPurple/Managers/VuCommunityManager.h"
#include "VuEngine/Util/VuUtf8.h"

using namespace Windows::Foundation;


class VuWindowsCommunityManager : public VuCommunityManager
{
public:
	virtual void	showWebPage(const std::string &url);
	virtual void	showMoreGames(const std::string &packageName, const std::string &publisherName);
	virtual void	rateGame(const std::string &packageName);
};


// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuCommunityManager, VuWindowsCommunityManager);


//*****************************************************************************
void VuWindowsCommunityManager::showWebPage(const std::string &url)
{
	std::wstring wideUrl;
	VuUtf8::convertUtf8StringToWCharString(url.c_str(), wideUrl);
	Platform::String^ nativeUrl = ref new Platform::String(wideUrl.c_str());

	Uri^ uri = ref new Uri(nativeUrl);
	Windows::System::Launcher::LaunchUriAsync(uri);
}

//*****************************************************************************
void VuWindowsCommunityManager::showMoreGames(const std::string &packageName, const std::string &publisherName)
{
#if defined VUWINSTORE
	showWebPage("ms-windows-store:Publisher?name=Vector Unit");
#elif defined VUWINPHONE
	showWebPage("zune:search?publisher=Vector Unit");
#else
	showWebPage("http://www.vectorunit.com/games");
#endif
}

//*****************************************************************************
void VuWindowsCommunityManager::rateGame(const std::string &packageName)
{
#if defined VUWINSTORE
	showWebPage("ms-windows-store:Review?PFN=VectorUnit.BeachBuggyRacing_hvbhrzr8672s2");
#elif defined VUWINPHONE
	showWebPage("zune:reviewapp");
#else
	showWebPage("http://www.vectorunit.com/games");
#endif
}
