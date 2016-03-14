//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  AnalyticsManager class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/VuSystemComponent.h"
#include "VuEngine/Json/VuJsonContainer.h"


class VuAnalyticsManager : public VuSystemComponent
{
	DECLARE_SYSTEM_COMPONENT(VuAnalyticsManager)

protected:
	// created by game
	friend class VuPurple;
	virtual bool init() { return true; }

public:
	// events
	void				logEvent(const char *eventName, const char *paramName = "", const char *paramValue = "", const VuJsonContainer &variables = VuJsonContainer::null);
	void				logEvent(const char* eventName,const char* paramKey,int value);
	// optional
	void				setLevel(int level);
	void				setCurrency(int type, int balance);
	void				displayNotifications();

protected:
	virtual void		logEventInternal(const char *eventName, const char *paramName, const char *paramValue, const VuJsonContainer &variables) = 0;
	virtual void		logEventInternal(const char* eventName,const char* paramKey,int value) = 0;
	virtual void		setLevelInternal(int level) {}
	virtual void		setCurrencyInternal(int type, int balance) {}
	virtual void		displayNotificationsInternal() {}
};
