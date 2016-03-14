//*****************************************************************************
//
//  Copyright (c) 2012-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  GiftManager class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/VuSystemComponent.h"
#include "VuEngine/Util/VuFSM.h"

class VuMessageBox;


class VuGiftManager : public VuSystemComponent
{
	DECLARE_SYSTEM_COMPONENT(VuGiftManager)

protected:
	// called by game
	friend class VuPurple;
	virtual bool init();
	virtual void release();

public:
	VuGiftManager();

	void			redeemCode(const std::string &code);
	bool			isBusy();

	enum { CODE_LENGTH = 8 };

protected:
	void			tick(float fdt);

	// FSM
	void			onIdleEnter();
	void			onRedeemCodeEnter();
	void			onRedeemCodeExit();
	void			onRedeemCodeTick(float fdt);

	VuFSM			mFSM;
	std::string		mCode;
	VUHANDLE		mHttpRequest;

	VuMessageBox	*mpMessageBox;
};
