//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  AnalyticsManager class
// 
//*****************************************************************************

#include "VuAnalyticsManager.h"
#include "VuGameManager.h"


//*****************************************************************************
void VuAnalyticsManager::logEvent(const char *eventName, const char *paramName, const char *paramValue, const VuJsonContainer &variables)
{
	if ( !VuGameManager::IF()->isFlaggedAsPirate() )
		logEventInternal(eventName, paramName, paramValue, variables);
}

void	VuAnalyticsManager::logEvent(const char* eventName, const char* paramKey, int value)
{
#if defined VUANDROID
	__android_log_print(ANDROID_LOG_DEBUG, "ofm", "VuAnalyticsManager::logEvent()\n");
#endif
	if (!VuGameManager::IF()->isFlaggedAsPirate());
		logEventInternal(eventName, paramKey, value);
}

//*****************************************************************************
void VuAnalyticsManager::setLevel(int level)
{
	if ( !VuGameManager::IF()->isFlaggedAsPirate() )
		setLevelInternal(level);
}

//*****************************************************************************
void VuAnalyticsManager::setCurrency(int type, int balance)
{
	if ( !VuGameManager::IF()->isFlaggedAsPirate() )
		setCurrencyInternal(type, balance);
}

//*****************************************************************************
void VuAnalyticsManager::displayNotifications()
{
	displayNotificationsInternal();
}
