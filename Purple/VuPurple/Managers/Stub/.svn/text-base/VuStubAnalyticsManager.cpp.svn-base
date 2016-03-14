//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Stub Analytics manager
//
//*****************************************************************************

#include "VuPurple/Managers/VuAnalyticsManager.h"
#include "VuEngine/Json/VuJsonWriter.h"


class VuStubAnalyticsManager : public VuAnalyticsManager
{
public:
	virtual void		logEventInternal(const char *eventName, const char *paramName, const char *paramValue, const VuJsonContainer &variables);
	virtual void		logEventInternal(const char* eventName, const char* paramKey, int value);
	virtual void		setLevelInternal(int level);
	virtual void		setCurrencyInternal(int type, int balance);
	virtual void		displayNotificationsInternal();
};


// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuAnalyticsManager, VuStubAnalyticsManager);


//*****************************************************************************
void VuStubAnalyticsManager::logEventInternal(const char *eventName, const char *paramName, const char *paramValue, const VuJsonContainer &variables)
{
	std::string strVariables;
	VuJsonWriter writer;
	writer.configCompact(true);
	writer.saveToString(variables, strVariables);

	VUPRINTF("Analytics Event: %s [%s = %s] %s\n", eventName, paramName, paramValue, strVariables.c_str());
}
//*****************************************************************************
void	VuStubAnalyticsManager::logEventInternal(const char* eventName, const char* paramKey, int value)
{
	VUPRINTF("Analytics Event: %s [%s = %d]\n", eventName, paramKey, value);
}
//*****************************************************************************
void VuStubAnalyticsManager::setLevelInternal(int level)
{
	VUPRINTF("Analytics Set Level: %d\n", level);
}

//*****************************************************************************
void VuStubAnalyticsManager::setCurrencyInternal(int type, int balance)
{
	VUPRINTF("Analytics Set Currency: Type %d, Balance %d\n", type, balance);
}

//*****************************************************************************
void VuStubAnalyticsManager::displayNotificationsInternal()
{
	VUPRINTF("Display Notifications!!!\n");
}
