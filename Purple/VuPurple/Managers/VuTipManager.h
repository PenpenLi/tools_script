//*****************************************************************************
//
//  Copyright (c) 2011-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  TipManager class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/VuSystemComponent.h"
#include "VuEngine/Managers/VuMessageBoxManager.h"


class VuTipManager : public VuSystemComponent, VuMessageBox::Callback
{
	DECLARE_SYSTEM_COMPONENT(VuTipManager)

protected:
	// called by game
	friend class VuPurple;
	virtual bool init();
	virtual void release();

public:
	void			showTip();
	void			setTipShown(const std::string &tipName);

private:
	// VuMessageBox::Callback
	virtual void	onMessageBoxClosed(VuMessageBox *pMessageBox);

	bool			needToShow(const std::string &tipName);

	VuDBAsset		*mpTipDBAsset;
};
