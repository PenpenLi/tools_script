//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  BoostBlitzGame class
// 
//*****************************************************************************

#pragma once

#include "VuGame.h"

class VuGhostRecorder;
class VuGhostCarEntity;


class VuBoostBlitzGame : public VuGame
{
	DECLARE_RTTI

public:
	VuBoostBlitzGame(VuProject *pProject);
	~VuBoostBlitzGame();

private:
	virtual int		getNumOpponents() { return 0; }
	virtual void	onLoad(const VuJsonContainer &data);
	virtual void	onPreBegin();
	virtual void	onEnd();
	virtual void	onCarFinished(VuCarEntity *pCar);

	void			onIntroEnter();
	void			onIntroExit();
	void			onIntroTick(float fdt);

	void			onPreGameEnter();
	void			onPreGameExit();
	void			onPreGameTick(float fdt);

	void			onGameEnter();
	void			onGameExit();
	void			onGameTick(float fdt);

	void			onPostGameEnter();
	void			onPostGameExit();

	void			saveRecording(const VuArray<VUBYTE> &ghostData);
	bool			loadRecording(VuArray<VUBYTE> &ghostData);

	VuGhostRecorder		*mpRecorder;
	VuGhostCarEntity	*mpGhostCar;
};