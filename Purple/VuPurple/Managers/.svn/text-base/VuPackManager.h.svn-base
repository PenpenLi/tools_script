//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Pack Manager
// 
//*****************************************************************************

#pragma once

#include "VuEngine/VuSystemComponent.h"
#include "VuEngine/Entities/VuEntity.h"



class VuPackManager : public VuSystemComponent
{
	DECLARE_SYSTEM_COMPONENT(VuPackManager)

protected:
	// called by game
	friend class VuPurple;
	virtual bool init();
	virtual void release();

public:
	VuPackManager();

	enum ePackUnit { Pack_Nothing = 0, Pack_LanXiang, Pack_SuperVip };

	void triggerLanXiangPack();
	void triggerSuperVipPack();

	VUINT getLanXiangDiscountTime();

	VUINT getSuperVipDiscountTime();

	bool isPackOk(ePackUnit unit);

	int getPackType();
protected:
	void			tick(float fdt);

	VUINT m_LastSuperVipTime;
	VUINT m_LastLanXiangTime;

};
