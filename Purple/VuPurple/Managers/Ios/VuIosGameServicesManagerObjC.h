//*****************************************************************************
//
//  Copyright (c) 2012-2013 Vector Unit Inc
//  Confidential Trade Secrets
//
//  Objective-C support for Ios Game Services manager.
//
//*****************************************************************************

#pragma once


namespace VuIosGameServicesManagerObjC
{
	// Game Center Sign-In
	void	startSignInAsync();
	void	startSignOutAsync();
	void	checkState();
	void	onSignInSuccess(const char* pID, const char* pTag);
	void	onSignInFailure();
	void	onCheckStateResult(bool signedIn, const char* pId = VUNULL, const char* pTag = VUNULL);

	// Friends
	void	requestIosFriends();
	void	onIosFriendsReceived(const char* pFriendsList);
	
	// Leaderboards
	void	submitScore(const char* pLBName, VUUINT64 score);
	
	// Game Center Achievements
	void	showAchievements();
	void	unlockAchievement(const char *name);
	void	onUnlockAchievementResult(bool success);
}
