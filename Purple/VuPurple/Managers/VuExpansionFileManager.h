//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  ExpansionFileManager class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/VuSystemComponent.h"
#include "VuEngine/Managers/VuMessageBoxManager.h"


class VuExpansionFileManager : public VuSystemComponent, VuMessageBox::Callback
{
	DECLARE_SYSTEM_COMPONENT(VuExpansionFileManager)

public:
	virtual bool	init();

	bool			openExpansionFile();

	class VuListener
	{
	public:
		virtual void	onDownloadSuccess() = 0;
		virtual void	onDownloadFailure() = 0;
		virtual void	onDownloadProgress(float progress) = 0;
	};
	void			startDownload(VuListener *pListener);

protected:
	virtual void	startDownloadInternal() = 0;

	void			onDownloadResult(std::string errorMsg = "");
	void			onDownloadProgress(float progress);

	// VuMessageBox::Callback
	virtual void	onMessageBoxClosed(VuMessageBox *pMessageBox);

	bool			mTesting;
	std::string		mTestMsg;
	VuListener		*mpListener;
};
