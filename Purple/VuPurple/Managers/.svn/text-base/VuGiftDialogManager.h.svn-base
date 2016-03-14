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
#include "VuEngine/Managers/VuDialogManager.h"


class VuGiftDialogManager : public VuSystemComponent, VuDialog::Callback
{
	DECLARE_SYSTEM_COMPONENT(VuGiftDialogManager)

protected:
	// called by game
	friend class VuPurple;
	virtual bool init();
	virtual void release();
	// VuDialog::Callback
	virtual void		onDialogClosed(VuDialog *pDialog);
	void tickWork(float fdt);
	void	loadFromProfile();
	virtual void postInit();
	void processDiscountDialog(std::string& dialogStr, std::string& itemStr, std::string discountDialog);

public:
	std::string			getPopDialogs();
	std::string			getDialogBeforeGame(); //
	std::string			getDialogInPlayerHub(); 
	std::string			getDialogInUpgradeCar();
	std::string			getDialogInLounge();
	std::string			getDialogInShowRoom();

	void recordDialogBeforeGame();  //记录进入开始游戏的次数；
	void recordDialogInPlayerHub();
	void recordDialogInUpgradeCar();
	void recordDialogInLounge();
	void recordDialogInShowRoom();
	VuGiftDialogManager();
	void popUpGiftDialog(std::string dialogStr); //比赛结束后弹礼包逻辑
	void popUpDialogBeforGame(); //比赛开始前弹礼包逻辑
	void popUpDialogInPlayerHub();  //主界面弹出礼包逻辑
	void popUpDialogInUpgradeCar(); //升级车辆界面弹出逻辑
	void popUpDialogInLounge();
	void popUpDialogInShowRoom();
private:
	VUHANDLE	mHttpRequest;
	std::map<std::string, std::string> seriesToNumMap;
	std::map<std::string, std::string> eventToNumMap;
	std::string currentDialogAsset;
	std::map<int, std::string> mSimTypeMap;


};
