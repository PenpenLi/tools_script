//*****************************************************************************
//
//  Copyright (c) 2012-2013 Vector Unit Inc
//  Confidential Trade Secrets
//
//  Objective-C support for Ios cloud data manager.
//
//*****************************************************************************

#pragma once


namespace VuIosCloudSaveManagerObjC
{
	class CallbackIF
	{
	public:
		virtual bool onCloudMerge(const void *data, int dataSize) { return false; }
	};
	
	void	setCallback(CallbackIF *pCB);
	
	bool	start(const char *fileName, bool supportConflictResolution);
	void	stop();
	void	tick(float fdt);
	
	bool	write(const void *data, int dataSize);
}
