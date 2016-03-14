//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  TitleStorageManager class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/VuSystemComponent.h"
#include "VuEngine/Containers/VuArray.h"


class VuTitleStorageManager : public VuSystemComponent
{
	DECLARE_SYSTEM_COMPONENT(VuTitleStorageManager)

protected:
	// called by game
	friend class VuPurple;
	bool	init();
	void	release();

public:
	enum eResult { RESULT_SUCCESS, RESULT_ERROR, RESULT_NOT_FOUND };

	class LoadCallback { public: virtual void onTitleStorageLoadResult(eResult result, const std::string &data, int id) = 0; };
	void		startLoad(const std::string &gamerID, const char *fileName, LoadCallback *pCB, int id);

	class SaveCallback { public: virtual void onTitleStorageSaveResult(eResult result, int id) = 0; };
	void		startSave(const std::string &gamerID, const char *fileName, const std::string &fileData, SaveCallback *pCB, int id);

protected:
	void		tickNetwork(float fdt);

	class VuLoadAction
	{
	public:
		VuLoadAction(LoadCallback *pCB, int id) : mpCB(pCB), mId(id) {}
		LoadCallback	*mpCB;
		int				mId;
		VUHANDLE		mHttpRequest;
	};
	
	class VuSaveAction
	{
	public:
		VuSaveAction(SaveCallback *pCB, int id) : mpCB(pCB), mId(id) {}
		SaveCallback	*mpCB;
		int				mId;
		VUHANDLE		mHttpRequest;
	};
	
//	class VuLoadAction;
//	class VuSaveAction;

	typedef std::list<VuLoadAction> LoadActions;
	typedef std::list<VuSaveAction> SaveActions;

	LoadActions		mLoadActions;
	SaveActions		mSaveActions;
};


