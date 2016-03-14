//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  TutorialDriveGame class
// 
//*****************************************************************************

#pragma once

#include "VuGame.h"


class VuTutorialDriveGame : public VuGame
{
	DECLARE_RTTI

public:
	VuTutorialDriveGame(VuProject *pProject);
	~VuTutorialDriveGame();

private:
	virtual void	onLoad(const VuJsonContainer &data);
	virtual void	onPreBegin();
	virtual void	onPostBegin();

	void			onIntroEnter();
	void			onIntroExit();
	void			onIntroTick(float fdt);

	void			onPreGameEnter();
	void			onPreGameExit();
	void			onPreGameTick(float fdt);

	void			onGameEnter();
	void			onGameTick(float fdt);

	void			onPostGameEnter();
	void			onPostGameExit();
	void			onPostGameTick(float fdt);
};