#pragma once

#include "VuEngine/VuSystemComponent.h"





class PurchaseOrderDelegate
{
public:
	virtual void onGetOrderId(std::string strOrderId);
};

class VuPurchaseOrderManager : public VuSystemComponent
{
	DECLARE_SYSTEM_COMPONENT(VuPurchaseOrderManager)
public:
	VuPurchaseOrderManager();
	bool isGetOrder() const { return m_isGetOrder; }
	void setDelegate(PurchaseOrderDelegate* delegate);
	void htttpProcess();
	std::string genMD5code();
	~VuPurchaseOrderManager();
	void startGetOrderId();
protected:
	friend class VuPurple;
	virtual bool	init();
	virtual void postInit();
	virtual void	release();
	
protected:
	std::string m_orderId;
	bool m_isGetOrder;
	PurchaseOrderDelegate* m_delegate;
	VUHANDLE	mHttpRequest;
	VUHANDLE	mhThread;
	
	
	std::string		m_client_id;
	std::string		m_game_name;
	std::string		m_sdk_pay_type;
	std::string		m_time_stamp;
	std::string		m_private_key;

};