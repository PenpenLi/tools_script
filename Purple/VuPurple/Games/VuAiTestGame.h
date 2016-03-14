//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  RaceGame class
// 
//*****************************************************************************

#pragma once

#include "VuGame.h"


class VuAiTestGame : public VuGame
{
	DECLARE_RTTI

public:
	VuAiTestGame(VuProject *pProject);
	~VuAiTestGame();

private:
	virtual void	onLoad(const VuJsonContainer &data);
	virtual void	onPreBegin();
	virtual void	onPostBegin();
	virtual void	onEnd();
	virtual void	onCarFinished(VuCarEntity *pCar);

	void			onPreGameEnter();
	void			onPreGameExit();
	void			onPreGameTick(float fdt);

	void			onGameEnter();
	void			onGameTick(float fdt);

	void			updatePlacing();

	// placing
	class VuPlacingComp
	{
	public:
		VuPlacingComp(const Cars &cars) : mCars(cars) {}
		bool operator()(int i0, int i1);
		const Cars &mCars;
	};
	typedef std::vector<int> Placing;
	Placing		mPlacing;
	int mNumOpponents;
};