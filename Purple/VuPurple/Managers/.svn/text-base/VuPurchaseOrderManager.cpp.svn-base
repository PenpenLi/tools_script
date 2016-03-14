
#include "VuPurchaseOrderManager.h"
#include "VuEngine\HAL\Thread\VuThread.h"
#include "VuEngine\Net\VuHttpClient.h"
#include "VuEngine\Json\VuJsonWriter.h"
#include "VuEngine\Json\VuJsonContainer.h"
#include "VuPurple\Util\VuMd5.h"

#define requestOrderIdServerIP "http://gamepms.sky-mobi.com/getOrderId.php"

#define CLIENT_ID	"client_id"
#define GAME_NAME	"game_name"
#define SDK_PAY_TYPE		"sdk_pay_type"
#define TIME_STAMP	"time_stamp"
#define PRIVATE_KEY	"private_key"

void OrderThreadProc(void* pArg)
{
	/*VuPurchaseOrderManager* pMgr = (VuPurchaseOrderManager*)pArg;
	while (!pMgr->isGetOrder())
	{
	pMgr->htttpProcess();
	#if defined VUANDROID || defined VUIOS
	usleep(100);
	#else
	Sleep(100);

	#endif
	}
	VuThread::IF()->endThread();*/
}


// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuPurchaseOrderManager, VuPurchaseOrderManager);

bool	VuPurchaseOrderManager::init()
{
	return true;
}

void VuPurchaseOrderManager::postInit()
{
	mHttpRequest = VuHttpClient::IF()->createRequest();
	VuJsonContainer data;

	std::string md5Code = genMD5code();

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

	mhThread = VuThread::IF()->createThread(OrderThreadProc, this);
}

void	VuPurchaseOrderManager::release()
{
	VuThread::IF()->joinThread(mhThread);
}

VuPurchaseOrderManager::VuPurchaseOrderManager() :m_delegate(VUNULL), mHttpRequest(VUNULL), m_isGetOrder(false)
{
	m_client_id = "test";
	m_game_name = "test";
	m_sdk_pay_type = "test";
	m_time_stamp = "test";
	m_private_key = "sky_game_miyi";
}

VuPurchaseOrderManager::~VuPurchaseOrderManager()
{

}

void VuPurchaseOrderManager::startGetOrderId()
{
	mHttpRequest = VuHttpClient::IF()->createRequest();

	VuJsonContainer data;
	/*data["uid"].putValue(VuGameManager::IF()->getUid());

	data["method"].putValue("getRanking");
	if (mRankDataType == CUR_WEEK)
	data["listtype"].putValue("playerscore");
	else
	data["listtype"].putValue("weekscore");*/
	std::string strData;
	VuJsonWriter writer;
	/*writer.configCompact(true);
	writer.saveToString(data, strData);*/

	// header
	VuHttpClient::IF()->setContentHeader(mHttpRequest, "Content-Type", "application/json");
	VuHttpClient::IF()->setContentHeader(mHttpRequest, "Content-Length", (int)strData.size());
	// send request
	VuHttpClient::IF()->postAsync(mHttpRequest, requestOrderIdServerIP, strData);

	mhThread = VuThread::IF()->createThread(OrderThreadProc, this);
}

void VuPurchaseOrderManager::setDelegate(PurchaseOrderDelegate* delegate)
{
	m_delegate = delegate;
}

void VuPurchaseOrderManager::htttpProcess()
{
	if (mHttpRequest)
	{
		VuHttpClient::eStatus status = VuHttpClient::IF()->getStatus(mHttpRequest);
		if (status == VuHttpClient::STATUS_RESPONSE_RECEIVED)
		{
			int i = 1;
			int asd = 1;
		}
	}
}

std::string VuPurchaseOrderManager::genMD5code()
{
	std::string result;
	std::string resultFinal;
	std::map<std::string, std::string> tempMap;
	tempMap.insert(std::make_pair(CLIENT_ID, m_client_id));
	tempMap.insert(std::make_pair(GAME_NAME, m_game_name));
	tempMap.insert(std::make_pair(SDK_PAY_TYPE, m_sdk_pay_type));
	tempMap.insert(std::make_pair(TIME_STAMP, m_time_stamp));
	
	for (auto i : tempMap)
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