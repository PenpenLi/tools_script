//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  CommunityManager class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/VuSystemComponent.h"

class VuJsonContainer;


class VuCommunityManager : public VuSystemComponent
{
	DECLARE_SYSTEM_COMPONENT(VuCommunityManager)

protected:
	// created by game
	friend class VuPurple;
	virtual bool init() { return true; }

public:
	virtual void		showWebPage(const std::string &url) {}
	virtual void		showMoreGames(const std::string &packageName, const std::string &publisherName);
	virtual void		showTwitterPage(const std::string &userName);
	virtual void		showFacebookPage(const std::string &id, const std::string &userName);
	virtual void		showGooglePlusPage(const std::string &id);
	virtual void		rateGame(const std::string &packageName);
};
