//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  CloudSave Manager class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/VuSystemComponent.h"
#include "VuEngine/Containers/VuArray.h"
#include "VuEngine/Managers/VuMessageBoxManager.h"
#include "VuEngine/Json/VuJsonContainer.h"

class VuJsonContainer;


class VuCloudSaveManager : public VuSystemComponent, VuMessageBox::Callback
{
	DECLARE_SYSTEM_COMPONENT(VuCloudSaveManager)

protected:
	// created by game
	friend class VuPurple;
	virtual bool init() { return true; }
	virtual void postInit();

public:
	VuCloudSaveManager();

	void			save();

protected:
	// implemented by system-specific class
	virtual void	startCloudLoad() = 0;
	virtual void	startCloudSave(const VuArray<VUBYTE> &blob) = 0;

	// called by system-specific class
	void			onCloudLoadResult(const VuArray<VUBYTE> &blob);

	// VuMessageBox::Callback
	virtual void	onMessageBoxClosed(VuMessageBox *pMessageBox);

	void			createChoiceMessageBox();
	void			createRestoreMessageBox();
	void			createContinueMessageBox();
	void			formatMessageBoxBody(std::string &bodyText);

	void			saveToBlob(const VuJsonContainer &profileData, VuArray<VUBYTE> &blob);

	enum eState { STATE_LOADING, STATE_ASKING, STATE_RESOLVED };
	eState			mState;
	VuArray<VUBYTE>	mBlob;
	VuJsonContainer	mCloudData;
};
