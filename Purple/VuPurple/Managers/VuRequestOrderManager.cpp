
#include "VuRequestOrderManager.h"
#include "VuEngine\HAL\Thread\VuThread.h"
#include "VuEngine\Net\VuHttpClient.h"
#include "VuEngine\Json\VuJsonWriter.h"
#include "VuEngine\Json\VuJsonContainer.h"
#include "VuPurple\Util\VuMd5.h"
#include "VuEngine\Managers\VuTickManager.h"
#include "VuEngine\Json\VuJsonReader.h"
#include "VuPurple\Managers\VuBillingManager.h"
#include "VuPurple\Managers\VuGameManager.h"
#include "VuEngine\Util\VuLog.h"
#include "VuEngine\Managers\VuProfileManager.h"

#define requestOrderIdServerIP "http://order.miyigame.com:9001/getOrderId.php"
#define  requestSuccessVerificationIP "http://order.miyigame.com:9001/verifyOrder.php"

#define CLIENT_ID	"client_id"
#define GAME_NAME	"game_name"
#define SDK_PAY_TYPE		"sdk_pay_type"
#define TIME_STAMP	"time_stamp"
#define PRIVATE_KEY	"private_key"
#define TRANSACTION_ID "transaction_id"
#define ORDER_ID	"order_id"

// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuRequestOrderManager, VuRequestOrderManager);

bool	VuRequestOrderManager::init()
{
	return true;
}

void VuRequestOrderManager::postInit()
{

}

void	VuRequestOrderManager::release()
{
	
}

VuRequestOrderManager::VuRequestOrderManager() :m_delegate(VUNULL), mHttpRequest(VUNULL), m_isGetOrder(false)
{

	m_client_id = VuBillingManager::IF()->getDeviceId();
	m_game_name = "bbr";
	m_sdk_pay_type = VuBillingManager::IF()->getSDKType();
	m_time_stamp = "test";
	m_private_key = "sky_game_miyi";
	m_state = STATE_NULL;
	m_elapse = 0;
	m_processingElapseFlag = 0;
	m_requestOrderTime = 0;
	m_varificationTime = 0;
	m_processingMissOrderTime = 0;
	m_isConnect = true;
	mMB = 0;
}

VuRequestOrderManager::~VuRequestOrderManager()
{

}

void VuRequestOrderManager::startProcessMissingOrder(std::string orderId,std::string out_trade_no,std::string itemname)
{
	if (mHttpRequest)
	{
		VuHttpClient::IF()->releaseRequest(mHttpRequest);
		mHttpRequest = VUNULL;
	}
	m_currentPurchaseItem = itemname;
	m_current_orderId = orderId;
	std::string strTransactionId = out_trade_no;
	mHttpRequest = VuHttpClient::IF()->createRequest();
	VuJsonContainer data;
	std::map<std::string, std::string> tempMap;
	tempMap.insert(std::make_pair(CLIENT_ID, m_client_id));
	tempMap.insert(std::make_pair(ORDER_ID, orderId));
	tempMap.insert(std::make_pair(TRANSACTION_ID, strTransactionId));
	tempMap.insert(std::make_pair(TIME_STAMP, m_time_stamp));
	std::string md5Code = genMD5code(tempMap);

	data[CLIENT_ID].putValue(m_client_id);
	data[ORDER_ID].putValue(orderId);
	data[TRANSACTION_ID].putValue(strTransactionId);
	data[TIME_STAMP].putValue(m_time_stamp);
	data["sign"].putValue(md5Code);

	std::string strData;
	VuJsonWriter writer;
	writer.configCompact(true);
	writer.saveToString(data, strData);

	//// header
	VuHttpClient::IF()->setContentHeader(mHttpRequest, "Content-Type", "application/json");
	VuHttpClient::IF()->setContentHeader(mHttpRequest, "Content-Length", (int)strData.size());
	//// send request
	VuHttpClient::IF()->postAsync(mHttpRequest, requestSuccessVerificationIP, strData);
}

void VuRequestOrderManager::startGetOrderId(std::string purchaseItem)
{
	if (m_state == START_REQUEST_ORDER)
		return;
	m_state = START_REQUEST_ORDER;

	if (mHttpRequest)
	{
		VuHttpClient::IF()->releaseRequest(mHttpRequest);
		mHttpRequest = VUNULL;
	}
	
	m_currentPurchaseItem = purchaseItem;
	mHttpRequest = VuHttpClient::IF()->createRequest();
	VuJsonContainer data;

	std::map<std::string, std::string> tempMap;
	tempMap.insert(std::make_pair(CLIENT_ID, m_client_id));
	tempMap.insert(std::make_pair(GAME_NAME, m_game_name));
	tempMap.insert(std::make_pair(SDK_PAY_TYPE, m_sdk_pay_type));
	tempMap.insert(std::make_pair(TIME_STAMP, m_time_stamp));
	
	std::string md5Code = genMD5code(tempMap);

	data[CLIENT_ID].putValue(m_client_id);
	data[GAME_NAME].putValue(m_game_name);
	data[SDK_PAY_TYPE].putValue(m_sdk_pay_type);
	data[TIME_STAMP].putValue(m_time_stamp);
	data["sign"].putValue(md5Code);

	std::string strData;
	VuJsonWriter writer;
	writer.configCompact(true);
	writer.saveToString(data, strData);

	//// header
	VuHttpClient::IF()->setContentHeader(mHttpRequest, "Content-Type", "application/json");
	VuHttpClient::IF()->setContentHeader(mHttpRequest, "Content-Length", (int)strData.size());
	//// send request
	VuHttpClient::IF()->postAsync(mHttpRequest, requestOrderIdServerIP, strData);
	startConectingDialog();
}


void VuRequestOrderManager::startSuccessVerification(std::string itemname)
{
	if (m_state == START_SUCCESS_VERIFICATION)
		return;
	if (mHttpRequest)
	{
		VuHttpClient::IF()->releaseRequest(mHttpRequest);
		mHttpRequest = VUNULL;
	}
	m_state = START_SUCCESS_VERIFICATION;
	std::string strTransactionId = VuBillingManager::IF()->getTransactionId();
#if defined VUANDROID 
	__android_log_print(ANDROID_LOG_DEBUG, "test","testtest");
	__android_log_print(ANDROID_LOG_DEBUG, "test","%s",strTransactionId.c_str());
#endif
	m_verificationItem = itemname;


	mHttpRequest = VuHttpClient::IF()->createRequest();
	VuJsonContainer data;

#if defined VUANDROID 
	__android_log_print(ANDROID_LOG_DEBUG, "test", "testtest2");
#endif
#if defined VUANDROID 
	__android_log_print(ANDROID_LOG_DEBUG, "VuRequestOrderManager", "%s",m_client_id.c_str());
	__android_log_print(ANDROID_LOG_DEBUG, "VuRequestOrderManager", "%s", m_current_orderId.c_str());
	__android_log_print(ANDROID_LOG_DEBUG, "VuRequestOrderManager", "%s", strTransactionId.c_str());
	__android_log_print(ANDROID_LOG_DEBUG, "VuRequestOrderManager", "%s", m_time_stamp.c_str());
#endif

	std::map<std::string, std::string> tempMap;
	tempMap.insert(std::make_pair(CLIENT_ID, m_client_id));
	tempMap.insert(std::make_pair(ORDER_ID, m_current_orderId));
	tempMap.insert(std::make_pair(TRANSACTION_ID, strTransactionId));
	tempMap.insert(std::make_pair(TIME_STAMP, m_time_stamp));
	std::string md5Code = genMD5code(tempMap);

	data[CLIENT_ID].putValue(m_client_id);
	data[ORDER_ID].putValue(m_current_orderId);
	data[TRANSACTION_ID].putValue(strTransactionId);
	data[TIME_STAMP].putValue(m_time_stamp);
	data["sign"].putValue(md5Code);

	std::string strData;
	VuJsonWriter writer;
	writer.configCompact(true);
	writer.saveToString(data, strData);

	//// header
	VuHttpClient::IF()->setContentHeader(mHttpRequest, "Content-Type", "application/json");
	VuHttpClient::IF()->setContentHeader(mHttpRequest, "Content-Length", (int)strData.size());
	//// send request
	VuHttpClient::IF()->postAsync(mHttpRequest, requestSuccessVerificationIP, strData);
}

void VuRequestOrderManager::setDelegate(RequestOrderDelegate* delegate)
{
	m_delegate = delegate;
}

std::string VuRequestOrderManager::genMD5code(std::map<std::string,std::string> strMap)
{
	std::string result;
	std::string resultFinal;

	for (auto i : strMap)
	{
		result.append(i.first);
		result.append("=");
		result.append(i.second);
		result.append("&");
	}

	result.append(PRIVATE_KEY);
	result.append("=");
	result.append(m_private_key);

	resultFinal = MD5(result).hexdigest();
	return resultFinal;
}

void VuRequestOrderManager::tickWork(float fdt)
{
	m_elapse += fdt;
	switch (m_state)
	{
	case START_REQUEST_ORDER:
		tickNetwork(fdt);
		break;
	case START_SUCCESS_VERIFICATION:
		tickSuccesssVerificationWork(fdt);
		break;
	case START_PROCESS_MISSINGORDER:
		tickProcessOrderWork(fdt);
		break;
	case STATE_NULL:
		{
			if (m_elapse > 10 && m_isConnect == true)
			{
				m_elapse = 0;
			
				if (VuGameManager::IF()->getOrders().size() > 0)
				{
					m_state = START_PROCESS_MISSINGORDER;
					VuGameManager::Order tempOrder = (*VuGameManager::IF()->getOrders().begin());
					startProcessMissingOrder(tempOrder.m_orderId, tempOrder.m_out_trade_no,tempOrder.m_itemName);
				}
			 }
		}
		break;
	}
}

void	VuRequestOrderManager::tickNetwork(float fdt)
{
	
	if (mHttpRequest)
	{
		m_requestOrderTime += 1;
		VuHttpClient::eStatus status = VuHttpClient::IF()->getStatus(mHttpRequest);
		if (status == VuHttpClient::STATUS_RESPONSE_RECEIVED)
		{
			VuJsonContainer response;
			VuJsonReader jsonReader;

			if (jsonReader.loadFromString(response, VuHttpClient::IF()->getResponse(mHttpRequest)))
			{

				int orderValue = response["order_id"].asInt();
				char c[16];
				sprintf(c, "%d", orderValue);
				m_current_orderId = c;
				VuBillingManager::IF()->genOutTradeNo();
				std::string strOutTradeNo = VuBillingManager::IF()->getTransactionId();
				m_delegate->onGetOrderId(c, strOutTradeNo,m_currentPurchaseItem);
				VuProfileManager::IF()->save();
			}
			//VuTickManager::IF()->unregisterHandlers(this);
			m_state = STATE_NULL;
			VuHttpClient::IF()->releaseRequest(mHttpRequest);
			mHttpRequest = VUNULL;
			m_requestOrderTime = 0;
			detroyConnectingDialog();
		}
		else
		{
			if (m_requestOrderTime > 150)
			{
				m_requestOrderTime = 0;
				detroyConnectingDialog();
				conncectFail();
				m_state = STATE_NULL;
				VuHttpClient::IF()->releaseRequest(mHttpRequest);
				mHttpRequest = VUNULL;
			}
			
		}
	}
}

void VuRequestOrderManager::startConectingDialog()
{
	// create message box
	VuMessageBoxParams messageBoxParams;
	messageBoxParams.mType = "SimpleWait";
	messageBoxParams.mHeading = "NetworkFail_Connecting";
	messageBoxParams.mBody = "Store_RedeemCode_Busy";
	messageBoxParams.mPauseGame = true;

	mMB = VuMessageBoxManager::IF()->create(messageBoxParams);
}

void VuRequestOrderManager::detroyConnectingDialog()
{
	if (mMB)
	{
		VuMessageBoxManager::IF()->destroy(mMB);
		mMB = VUNULL;
	}
		
}

void VuRequestOrderManager::conncectFail()
{
	VuMessageBoxParams messageBoxParams;
	messageBoxParams.mType = "SimpleA";
	messageBoxParams.mHeading = "NetworkFail_Title";
	messageBoxParams.mTextA = "Common_Yes";
	messageBoxParams.mBody = "Store_RedeemCode_FailureNetwork";
	messageBoxParams.mPauseGame = true;

	//messageBoxParams.mUserData = (nCoinTotal << 16) + nDiamTotal;

	VuMessageBox *pMB = VuMessageBoxManager::IF()->create(messageBoxParams);
	pMB->removeRef();
}

void VuRequestOrderManager::tickProcessOrderWork(float fdt)
{
	m_processingElapseFlag += fdt;
	
	if (mHttpRequest && m_processingElapseFlag > 5)
	{
		m_processingElapseFlag = 0;
		m_processingMissOrderTime += 1;
		VuHttpClient::eStatus status = VuHttpClient::IF()->getStatus(mHttpRequest);
		if (status == VuHttpClient::STATUS_RESPONSE_RECEIVED)
		{
			m_processingMissOrderTime = 0;
			VuJsonContainer response;
			VuJsonReader jsonReader;

#if defined VUANDROID 
			__android_log_print(ANDROID_LOG_DEBUG, "VuRequestOrderManager", "VuRequestOrderManager\n");
#endif
			if (jsonReader.loadFromString(response, VuHttpClient::IF()->getResponse(mHttpRequest)))
			{
				std::string strPayResult = response["pay_result"].asCString();
				//VUPRINTF(response["time_stamp"].asCString());
				//VUPRINTF(response["sign"].asCString());
#if defined VUANDROID 
				__android_log_print(ANDROID_LOG_DEBUG, "VuRequestOrderManager", "%s", strPayResult.c_str());
#endif
				if (strPayResult == "SUCCESS" && VuGameManager::IF()->isExistOrder(m_current_orderId))
				{
#if defined VUANDROID 
					__android_log_print(ANDROID_LOG_DEBUG, "VuRequestOrderManager", "%s",m_currentPurchaseItem.c_str());
#endif
					m_delegate->onSuccessVerification(m_currentPurchaseItem);
#if defined VUANDROID 
					__android_log_print(ANDROID_LOG_DEBUG, "VuRequestOrderManager", "step2");
#endif
					VuGameManager::IF()->deleteProcessedOrder(m_current_orderId);
#if defined VUANDROID 
					__android_log_print(ANDROID_LOG_DEBUG, "VuRequestOrderManager", "step3");
#endif
				}
				else if (strPayResult == "FAIL" && VuGameManager::IF()->isExistOrder(m_current_orderId))
				{
					VuGameManager::IF()->deleteProcessedOrder(m_current_orderId);
				}
				else
				{
					VuGameManager::IF()->addSpecialOrderNum(m_current_orderId);
				}
#if defined VUANDROID 
				__android_log_print(ANDROID_LOG_DEBUG, "VuRequestOrderManager", "step4");
#endif
				VuProfileManager::IF()->save();
#if defined VUANDROID 
				__android_log_print(ANDROID_LOG_DEBUG, "VuRequestOrderManager", "step5");
#endif
			}

			VuHttpClient::IF()->releaseRequest(mHttpRequest);
			mHttpRequest = VUNULL;

			if (VuGameManager::IF()->getOrders().size() > 0)
			{
				std::vector<VuGameManager::Order> orders = VuGameManager::IF()->getOrders();
				VuGameManager::Order tempOrder = (*orders.begin());
				startProcessMissingOrder(tempOrder.m_orderId, tempOrder.m_out_trade_no,tempOrder.m_itemName);
			}
			else
			{
				m_state = STATE_NULL;
			}
			//VuTickManager::IF()->unregisterHandlers(this);
		}
		else
		{
			if (m_processingMissOrderTime >= 2)
			{
				VuHttpClient::IF()->releaseRequest(mHttpRequest);
				mHttpRequest = VUNULL;
				m_state = STATE_NULL;
				m_processingMissOrderTime = 0;
				m_isConnect = false;
			}
		}
	}
}

void VuRequestOrderManager::startWork()
{
	VuTickManager::IF()->registerHandler(this, &VuRequestOrderManager::tickWork, "Network");
	setDelegate(VuBillingManager::IF());
}

void VuRequestOrderManager::tickSuccesssVerificationWork(float fdt)
{
#if defined VUANDROID 
	__android_log_print(ANDROID_LOG_DEBUG, "VuRequestOrderManager", "tickSuccesssVerificationWork\n");
#endif
	if (mHttpRequest)
	{
		VuHttpClient::eStatus status = VuHttpClient::IF()->getStatus(mHttpRequest);
		m_varificationTime += fdt;
		if (status == VuHttpClient::STATUS_RESPONSE_RECEIVED)
		{
			VuJsonContainer response;
			VuJsonReader jsonReader;
			m_varificationTime = 0;
#if defined VUANDROID 
			__android_log_print(ANDROID_LOG_DEBUG, "VuRequestOrderManager", "VuRequestOrderManager\n");
#endif
			if (jsonReader.loadFromString(response, VuHttpClient::IF()->getResponse(mHttpRequest)))
			{
				std::string strPayResult =  response["pay_result"].asCString();
				//VUPRINTF(response["time_stamp"].asCString());
				//VUPRINTF(response["sign"].asCString());
#if defined VUANDROID 
				__android_log_print(ANDROID_LOG_DEBUG, "VuRequestOrderManager", "%s",strPayResult.c_str());
#endif
				if (strPayResult == "SUCCESS" && VuGameManager::IF()->isExistOrder(m_current_orderId))
				{
					m_delegate->onSuccessVerification(m_verificationItem);
					VuGameManager::IF()->deleteProcessedOrder(m_current_orderId);
				}
				else if (strPayResult == "FAIL" && VuGameManager::IF()->isExistOrder(m_current_orderId))
				{
					VuGameManager::IF()->deleteProcessedOrder(m_current_orderId);
				}
				else
				{
					VuGameManager::IF()->addSpecialOrderNum(m_current_orderId);
				}
				VuProfileManager::IF()->save();
			}			
			
			m_state = STATE_NULL;
			VuHttpClient::IF()->releaseRequest(mHttpRequest);
			mHttpRequest = VUNULL;
			//VuTickManager::IF()->unregisterHandlers(this);
		}
		else
		{
#if defined VUANDROID 
			__android_log_print(ANDROID_LOG_DEBUG, "VuRequestOrderManager", "error\n");
#endif
			if (m_varificationTime > 5)
			{
				m_varificationTime = 0;
				m_state = STATE_NULL;
				VuHttpClient::IF()->releaseRequest(mHttpRequest);
				mHttpRequest = VUNULL;
			}
		}
	}
}
