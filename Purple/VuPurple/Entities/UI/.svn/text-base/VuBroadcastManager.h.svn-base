class VuBroadcastManager : public VuSystemComponent
{
	DECLARE_SYSTEM_COMPONENT(VuBroadcastManager)
public:
	struct BroadcastItem
	{
		BroadcastItem(std::string name, std::string url)
		{
			m_name = name;
			m_url = url;
		}
		BroadcastItem(){}
		std::string m_name;
		std::string m_url;
		VUBYTE* mImage;
		VUBYTE* mNameImage;
		int imageSize;
		int nameImageSize;
	};

	struct BroadcastRawItem
	{
		BroadcastRawItem(std::string name, std::string url)
		{
			m_name = name;
			m_url = url;
		}
		std::string m_name;
		std::string m_url;
	};

	typedef std::vector<BroadcastItem> BroadcastItems;
	typedef std::vector<BroadcastRawItem> BroadcastRawItems;
	BroadcastItems getBroadcastData();
	void loadBroadcastDataFromCache();
	bool isDataOK();
	bool isTempDataOK() const { return mIsPicDataOK&&mIsNamePicDataOK; }
public:
	//////////////////////////////////////////////////////////////////////////
	enum eStatus{ RETRIEVE_PENDING, RETRIEVE_SUCCESS, RETRIEVE_FAILURE };
	enum eStatesRequest{REQUEST_JSON,REQUEST_IMAGE,REQUEST_NAMEIMAGE,REQUEST_NULL};
	//////////////////////////////////////////////////////////////////////////
	VuBroadcastManager();
	~VuBroadcastManager();
protected:
	friend class VuPurple;
	virtual bool	init();
	virtual void postInit();
	virtual void	release();

	void updateData();//hourly refresh
	void requestPicData();
	void requestNameImageData();
	void tickGetPic(float fdt);
	void tickGetNamePic(float fdt);
	void tickGetJson(float fdt);
	void saveToCache();

private:
	void		tickNetwork(float fdt);
	VUHANDLE mHttpRequest;
	BroadcastItems items;
	BroadcastRawItems rawItems;
	int currentRequestIndex;
	int currentRequestNameIndex;
	int mVersion;
	int mBroadcastDataSize;
	bool mIsPicDataOK;
	bool mIsNamePicDataOK;
	eStatesRequest requestStates;
	std::vector<VUBYTE*> mTempBroadcastImages;
	std::vector<VUBYTE*> mTempBroadcastNameImages;

	std::vector<int>			mTempBroadcastImageSizes;
	std::vector<int>			mTempBroadcastNameImageSizes;
};
