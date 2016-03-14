#pragma once

#include "VuEngine/VuSystemComponent.h"
#include <map>
#include "VuEngine/Managers/VuMessageBoxManager.h"




class RequestOrderDelegate
{
public:
	virtual void onGetOrderId(std::string strOrderId,std::string outTradeNo,std::string purchaseItem) = 0;
	virtual void onSuccessVerification(std::string itemName) = 0;
};

class VuRequestOrderManager : public VuSystemComponent
{
	DECLARE_SYSTEM_COMPONENT(VuRequestOrderManager)
public:
	enum RequestOrderState
	{
		START_REQUEST_ORDER = 0,
		START_SUCCESS_VERIFICATION,
		START_PROCESS_MISSINGORDER,
		STATE_NULL,
		STATE_COUNT
	};
public:
	VuRequestOrderManager();
	bool isGetOrder() const { return m_isGetOrder; }
	void setDelegate(RequestOrderDelegate* delegate);
	std::string genMD5code(std::map<std::string, std::string> strMap);
	~VuRequestOrderManager();
	void startGetOrderId(std::string purchaseItem);
	void startSuccessVerification(std::string itemname);
	void startProcessMissingOrder(std::string orderId,std::string out_trade_no,std::string itemname);
	std::string getOrderId() { return m_current_orderId; }
	void startWork();
protected:
	friend class VuPurple;
	virtual bool	init();
	virtual void postInit();
	virtual void	release();
	void tickWork(float fdt);
	void	tickNetwork(float fdt);
	void tickSuccesssVerificationWork(float fdt);
	void tickProcessOrderWork(float fdt);
	void conncectFail();
	void startConectingDialog();
	void detroyConnectingDialog();
protected:
	std::string m_orderId;
	bool m_isGetOrder;
	RequestOrderDelegate* m_delegate;
	VUHANDLE	mHttpRequest;
	VUHANDLE	mProcesssedOrderHttpRequest;

	RequestOrderState m_state;
	std::string		m_currentPurchaseItem;
	std::string		m_verificationItem;

	std::string		m_client_id;
	std::string		m_game_name;
	std::string		m_sdk_pay_type;
	std::string		m_time_stamp;
	std::string		m_private_key;
	std::string		m_current_orderId;


	 float	m_elapse;
	 float	m_processingElapseFlag;

	 float m_requestOrderTime;
	 float m_varificationTime;
	 float m_processingMissOrderTime;
	 float m_isConnect;
	 VuMessageBox *mMB;
};