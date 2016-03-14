//*****************************************************************************
//
//  Copyright (c) 2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuGameServicesManager
// 
//*****************************************************************************

#pragma once

#include "VuEngine/VuSystemComponent.h"


class VuGameServicesManager : public VuSystemComponent
{
	DECLARE_SYSTEM_COMPONENT(VuGameServicesManager)

public:
	VuGameServicesManager();
	~VuGameServicesManager();

	virtual bool	init();
	virtual void	postInit();
	virtual void	release();

	// sign-in/out
	void			startAutoSignIn();
	void			startSignIn();
	void			startSignOut();
	bool			isSignedIn()	{ return mIsSignedIn; }
	bool			isBusy()		{ return mIsBusy; }

	// info
	const std::string	&getAuthType()	{ return mAuthType; }
	const std::string	&getGamerID()	{ return mGamerID; }
	const std::string	&getGamerTag()	{ return mGamerTag; }

	// friends
	struct FriendInfo
	{
		std::string	mGamerID;
		std::string	mGamerTag;
	};
	int					getFriendCount() { return (int)mFriends.size(); }
	const FriendInfo	&getFriendInfo(int index) { return mFriends[index]; }
	const FriendInfo	*getFriendInfo(const char *gamerID);

	// listeners
	class VuListener
	{
	public:
		virtual void	onSignIn() {}
		virtual void	onSignOut() {}
		virtual void	onFriendsRetrieved() {}
	};

	void			addListener(VuListener *pListener)		{ mListeners.push_back(pListener); }
	void			removeListener(VuListener *pListener)	{ mListeners.remove(pListener); }

	// native ui
	virtual void	showAchievements() = 0;

	// leaderboards
	virtual void	submitScore(const char *lbName, int value);

protected:
	// system-specific implementation
	virtual void	startSignInAsync() = 0;
	virtual void	startSignOutAsync() = 0;
	virtual void	setAchievementUnlocked(const char *nativeId) = 0;
	virtual void	submitScoreAsync(const char *nativeId, int value) = 0;

	typedef std::vector<FriendInfo> Friends;

	void			onSignInSuccess(const char *gamerID, const char *gamerTag);
	void			onSignInFailure();
	void			onSignOut();
	void			onFriendsRetrieved(const Friends &friends);

	void			tickDecision(float fdt);
	void			syncAchievements();
	void			resetAchievements();

	struct VuAchievementInfo
	{
		VuAchievementInfo() : mUnlocked(false) {}
		std::string	mNativeId;
		bool		mUnlocked;
	};
	typedef std::map<int, VuAchievementInfo> Achievements;
	typedef std::list<VuListener *> Listeners;

	bool			mIsSignedIn;
	bool			mIsBusy;
	Listeners		mListeners;
	float			mTimer;
	std::string		mAuthType;
	std::string		mGamerID;
	std::string		mGamerTag;
	Friends			mFriends;
	Achievements	mAchievements;
};
