//*****************************************************************************
//
//  Copyright (c) 2012-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Android Billing manager
//
//*****************************************************************************

#include "VuAndroidBillingManager.h"
#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/Assets/VuAssetFactory.h"
#include "VuPurple/Managers/VuSettingsManager.h"

// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuBillingManager, VuAndroidBillingManager);

// static JAVA stuff
static JNIEnv		*s_jniEnv;
static jobject		s_helperObject;
static jmethodID	s_initialize;
static jmethodID	s_addItemId;
static jmethodID	s_startPurchase;

//当前是哪个simtype
static jmethodID    s_curSimType = 0;

static jmethodID   s_moreGame = 0;

static jmethodID	s_telecomExit = 0;

static jmethodID	s_getDeviceID = 0;
static jmethodID	s_getTransactionID = 0;
static jmethodID	s_getSDKType = 0;
static jmethodID	s_genOutTradeNo = 0;
/*
static jmethodID	s_enableSound = 0;
static jmethodID	s_isGameBase = 0;*/


static std::string sStoreItemKey;


//*****************************************************************************
void VuAndroidBillingManager::bindJavaMethods(JNIEnv *jniEnv, jobject classLoaderObject, jmethodID findClassMethod)
{
	__android_log_print(ANDROID_LOG_DEBUG, "Billing",  "VuAndroidBillingManager::bindJavaMethods()\n");

	s_jniEnv = jniEnv;

	// get reference to helper class object
	jstring helperClassName = jniEnv->NewStringUTF("com/vectorunit/VuBillingHelper");
	jclass helperClass = (jclass)jniEnv->CallObjectMethod(classLoaderObject, findClassMethod, helperClassName);
	jniEnv->DeleteLocalRef(helperClassName);

	jmethodID getInstance = jniEnv->GetStaticMethodID(helperClass, "getInstance", "()Lcom/vectorunit/VuBillingHelper;");
	jobject helperObject = jniEnv->CallStaticObjectMethod(helperClass, getInstance);
	s_helperObject = jniEnv->NewGlobalRef(helperObject);

	// methods
	s_initialize = jniEnv->GetMethodID(helperClass, "initialize", "()V");
	s_addItemId = jniEnv->GetMethodID(helperClass, "addItemId", "(Ljava/lang/String;)V");
	s_startPurchase = jniEnv->GetMethodID(helperClass, "startPurchase", "(Ljava/lang/String;Ljava/lang/String;)V");

//获取当前的sim类型
	s_curSimType = jniEnv->GetMethodID(helperClass, "getCurSimTypeForNative", "()I");


	s_moreGame = jniEnv->GetMethodID(helperClass, "moreGameForNative", "()V");

	s_telecomExit = jniEnv->GetMethodID(helperClass, "telecomExitForNative", "()V");
	//获取deviceid
	__android_log_print(ANDROID_LOG_DEBUG, "Billing", "VuAndroidBillingManager::getDeviceID()\n");
	s_getDeviceID = jniEnv->GetMethodID(helperClass, "getDeviceID", "()Ljava/lang/String;");
	//获取订单
	__android_log_print(ANDROID_LOG_DEBUG, "Billing", "VuAndroidBillingManager::getTransactionID()\n");
	s_getTransactionID = jniEnv->GetMethodID(helperClass, "getTransactionID", "()Ljava/lang/String;");
	s_getSDKType = jniEnv->GetMethodID(helperClass, "getSDKType", "()Ljava/lang/String;");
	s_genOutTradeNo = jniEnv->GetMethodID(helperClass, "genOutTradeNo", "()V");
/*
	s_enableSound = jniEnv->GetMethodID(helperClass, "enableSoundForNative", "()I");
	s_isGameBase = jniEnv->GetMethodID(helperClass, "isGameBaseForNative", "()I");*/
}

//*****************************************************************************
VuAndroidBillingManager::VuAndroidBillingManager()
{
	// event handlers
	REG_EVENT_HANDLER(VuAndroidBillingManager, OnAndroidPurchaseResult);
	REG_EVENT_HANDLER(VuAndroidBillingManager, OnAddOwnedItem);
	REG_EVENT_HANDLER(VuAndroidBillingManager, OnAndroidSetItemPrice);
	REG_EVENT_HANDLER(VuAndroidBillingManager, OnAndroidLogPurchaseAnalyticsEvent);


	if ( VuAssetFactory::IF()->getSku() == "Dev" )
		sStoreItemKey = "Name";
	else if ( VuAssetFactory::IF()->getSku() == "GooglePlay" )
		sStoreItemKey = "GooglePlayID";
	else if (VuAssetFactory::IF()->getSku() == "Amazon")
		sStoreItemKey = "AmazonID";
	else if (VuAssetFactory::IF()->getSku() == "SkyMobi")
		sStoreItemKey = "SkyMobi";
}

int	VuAndroidBillingManager::getCurSimType()
{
	if (s_curSimType != 0)
	{
		jint val = s_jniEnv->CallIntMethod(s_helperObject, s_curSimType);
		int nType = (int)val;
		return nType;
	}
	return 0;
}

void VuAndroidBillingManager::moreGame()
{
	if (s_moreGame != 0)
	{
		s_jniEnv->CallVoidMethod(s_helperObject, s_moreGame);
		
	}
}

void VuAndroidBillingManager::telecomExitGame()
{
	if (s_telecomExit != 0)
	{
		s_jniEnv->CallVoidMethod(s_helperObject, s_telecomExit);
		
	}
}

//*****************************************************************************
std::string	 VuAndroidBillingManager::getDeviceId()
{
	std::string result;
	if (s_getDeviceID != 0)
	{
		jstring val = (jstring)s_jniEnv->CallObjectMethod(s_helperObject, s_getDeviceID, 0);
		const char* strtemp = s_jniEnv->GetStringUTFChars(val, 0);
		result = strtemp;
		s_jniEnv->ReleaseStringUTFChars(val, strtemp);
	}
	__android_log_print(ANDROID_LOG_DEBUG, "getDeviceId", "%s", result.c_str());
	return result;
}

std::string	 VuAndroidBillingManager::getTransactionId()
{
	std::string result;
	if (s_getTransactionID != 0)
	{
		jstring val = (jstring)s_jniEnv->CallObjectMethod(s_helperObject, s_getTransactionID, 0);
		const char* strtemp = s_jniEnv->GetStringUTFChars(val, 0);
		result = strtemp;
		s_jniEnv->ReleaseStringUTFChars(val, strtemp);
	}
	__android_log_print(ANDROID_LOG_DEBUG, "getTransactionId", "%s", result.c_str());
	return result;
}
//*****************************************************************************
std::string	 VuAndroidBillingManager::getSDKType()
{
	std::string result;
	if (s_getTransactionID != 0)
	{
		jstring val = (jstring)s_jniEnv->CallObjectMethod(s_helperObject, s_getSDKType, 0);
		const char* strtemp = s_jniEnv->GetStringUTFChars(val, 0);
		result = strtemp;
		s_jniEnv->ReleaseStringUTFChars(val, strtemp);
	}
	return result;
}

void VuAndroidBillingManager::genOutTradeNo()
{
	if (s_genOutTradeNo != 0)
	{
		s_jniEnv->CallVoidMethod(s_helperObject, s_genOutTradeNo);
	}
}

//*****************************************************************************
void VuAndroidBillingManager::postInit()
{
	VuBillingManager::postInit();

	const VuJsonContainer &items = VuGameUtil::IF()->storeDB();
	for ( int i = 0; i < items.size(); i++ )
	{
		if ( items[i][sStoreItemKey].isString() )
		{
			const std::string &itemId = items[i][sStoreItemKey].asString();

			jstring jItemId = s_jniEnv->NewStringUTF(itemId.c_str());
			s_jniEnv->CallVoidMethod(s_helperObject, s_addItemId, jItemId);
			s_jniEnv->DeleteLocalRef(jItemId);
		}
	}
	s_jniEnv->CallVoidMethod(s_helperObject, s_initialize);

	//CheckGameBaseSetting();
}

/*
void VuAndroidBillingManager::CheckGameBaseSetting()
{


	int isGameBase = 0;
	int enableSound = 1;

	int nSimCard = getCurSimType();

	if (nSimCard == 1)
	{
		if (s_isGameBase)
		{
			jint val = s_jniEnv->CallIntMethod(s_helperObject, s_isGameBase);
			isGameBase = (int)val;
			if (isGameBase == 1)
			{
				if (s_enableSound)
				{
					jint val = s_jniEnv->CallIntMethod(s_helperObject, s_isGameBase);
					enableSound = (int)val;

					if (enableSound == 1) //开启声音
					{
						if (VuSettingsManager::IF())
						{
							VuSettingsManager::IF()->setEffectVolume(1.0);
							VuSettingsManager::IF()->setMusicVolume(1.0);

						}


					}
					else //mute
					{
						if (VuSettingsManager::IF())
						{
							VuSettingsManager::IF()->setEffectVolume(0.0);
							VuSettingsManager::IF()->setMusicVolume(0.0);

						}

					}
				}

			}

		}





	}

}
*/

//*****************************************************************************
bool VuAndroidBillingManager::getItemName(const std::string &storeItemName, std::string &itemName)
{
	const VuJsonContainer &items = VuGameUtil::IF()->storeDB();
	for ( int i = 0; i < items.size(); i++ )
	{
		if ( items[i][sStoreItemKey].asString() == storeItemName )
		{
			itemName = items[i]["Name"].asString();
			return true;
		}
	}

	return false;
}


//*****************************************************************************
void VuAndroidBillingManager::OnAndroidPurchaseResult(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	std::string storeItemName = accessor.getString();
	std::string result = accessor.getString();

	std::string itemName;

	/*!
 	* \author: xlm 2014/12/9  
 	* \brief: 原版是根据不同的平台来做的
 	*/
	if ( getItemName(storeItemName, itemName) )
		onPurchaseResult(itemName, result);



}

//*****************************************************************************
void VuAndroidBillingManager::OnAddOwnedItem(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	std::string storeItemName = accessor.getString();

	std::string itemName;
	if ( getItemName(storeItemName, itemName) )
	{
		const VuJsonContainer &itemData = VuDataUtil::findArrayMember(VuGameUtil::IF()->storeDB(), "Name", itemName);
		if ( itemData["Type"].asString() == "Managed" )
		{
			processItem(itemName.c_str());
		}
	}
}

//*****************************************************************************
void VuAndroidBillingManager::OnAndroidSetItemPrice(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	std::string storeItemName = accessor.getString();
	std::string price = accessor.getString();

	std::string itemName;
	if ( VuAndroidBillingManager::getItemName(storeItemName, itemName) )
		setPrice(itemName.c_str(), price.c_str());
}

//*****************************************************************************
void VuAndroidBillingManager::OnAndroidLogPurchaseAnalyticsEvent(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	std::string storeItemName = accessor.getString();

	std::string itemName;
	if ( VuAndroidBillingManager::getItemName(storeItemName, itemName) )
		logPurchaseAnalyticsEvent(itemName.c_str());
}

//*****************************************************************************
void VuAndroidBillingManager::startPurchaseInternal(const std::string &itemName)
{

	const VuJsonContainer &itemData = VuDataUtil::findArrayMember(VuGameUtil::IF()->storeDB(), "Name", itemName);
	const std::string &storeItemName = itemData[sStoreItemKey].asString();


	JNIEnv *jniEnv = s_jniEnv;

	jstring jItemName = jniEnv->NewStringUTF(storeItemName.c_str());

	VuBillingManager::BILLING_TYPE type = VuBillingManager::IF()->getCurrentBillingType();
	jstring jItemType = (type == VuBillingManager::LOCAL_MESSAGE)?jniEnv->NewStringUTF("msg") :jniEnv->NewStringUTF("weixin");


	jniEnv->CallVoidMethod(s_helperObject, s_startPurchase, jItemName, jItemType);
	jniEnv->DeleteLocalRef(jItemName);
	jniEnv->DeleteLocalRef(jItemType);


	/*const std::string storeItemName = itemName;

	JNIEnv *jniEnv = s_jniEnv;

	jstring jItemName = jniEnv->NewStringUTF(storeItemName.c_str());
	jniEnv->CallVoidMethod(s_helperObject, s_startPurchase, jItemName);
	jniEnv->DeleteLocalRef(jItemName);*/




	//xlm test
	/*const std::string storeItemName = "123";

	JNIEnv *jniEnv = s_jniEnv;

	jstring jItemName = jniEnv->NewStringUTF(storeItemName.c_str());
	jniEnv->CallVoidMethod(s_helperObject, s_startPurchase, jItemName);
	jniEnv->DeleteLocalRef(jItemName);*/
}


extern "C"
{
	JNIEXPORT void JNICALL Java_com_vectorunit_VuBillingHelper_onPurchaseResult(JNIEnv *env, jobject obj, jstring jItemId, jstring jResult);
	JNIEXPORT void JNICALL Java_com_vectorunit_VuBillingHelper_addOwnedItem(JNIEnv *env, jobject obj, jstring jItemId);
	JNIEXPORT jboolean JNICALL Java_com_vectorunit_VuBillingHelper_isConsumable(JNIEnv *env, jobject obj, jstring jItemId);
	JNIEXPORT void JNICALL Java_com_vectorunit_VuBillingHelper_setItemPrice(JNIEnv *env, jobject obj, jstring jItemId, jstring jPrice);
	JNIEXPORT void JNICALL Java_com_vectorunit_VuBillingHelper_flagAsPirate(JNIEnv *env, jobject obj);
	JNIEXPORT void JNICALL Java_com_vectorunit_VuBillingHelper_logPurchaseAnalyticsEvent(JNIEnv *env, jobject obj, jstring jItemId);


	JNIEXPORT void JNICALL Java_com_vectorunit_VuBillingHelper_onShutdownGame(JNIEnv *env, jobject obj, jstring jItemId, jstring jResult);
	JNIEXPORT jstring JNICALL Java_com_vectorunit_VuBillingHelper_getOrderNo(JNIEnv* env, jobject obj);
}

//*****************************************************************************
JNIEXPORT void JNICALL Java_com_vectorunit_VuBillingHelper_onPurchaseResult(JNIEnv *env, jobject obj, jstring jItemId, jstring jResult)
{
	// get itemId
	const char *strItemId = env->GetStringUTFChars(jItemId, 0);
	std::string itemId = strItemId;
	env->ReleaseStringUTFChars(jItemId, strItemId);

	// get result
	const char *strResult = env->GetStringUTFChars(jResult, 0);
	std::string result = strResult;
	env->ReleaseStringUTFChars(jResult, strResult);

	__android_log_print(ANDROID_LOG_DEBUG, "Billing", "Java_com_vectorunit_VuBillingHelper_onPurchaseResult\n");



	VuParams params;
	params.addString(itemId.c_str());
	params.addString(result.c_str());
	VuEventManager::IF()->broadcastDelayed(0.0f, true, "OnAndroidPurchaseResult", params);
}

//*****************************************************************************
JNIEXPORT void JNICALL Java_com_vectorunit_VuBillingHelper_addOwnedItem(JNIEnv *env, jobject obj, jstring jItemId)
{
	// get itemId
	const char *strItemId = env->GetStringUTFChars(jItemId, 0);
	std::string itemId = strItemId;
	env->ReleaseStringUTFChars(jItemId, strItemId);

	VuParams params;
	params.addString(itemId.c_str());
	VuEventManager::IF()->broadcastDelayed(0.0f, true, "OnAddOwnedItem", params);
}

//*****************************************************************************
JNIEXPORT jboolean JNICALL Java_com_vectorunit_VuBillingHelper_isConsumable(JNIEnv *env, jobject obj, jstring jItemId)
{
	// get itemId
	const char *strItemId = env->GetStringUTFChars(jItemId, 0);
	std::string storeItemName = strItemId;
	env->ReleaseStringUTFChars(jItemId, strItemId);

	std::string itemName;
	if ( VuAndroidBillingManager::getItemName(storeItemName, itemName) )
	{
		const VuJsonContainer &itemData = VuDataUtil::findArrayMember(VuGameUtil::IF()->storeDB(), "Name", itemName);

		return itemData["Type"].asString() == "Consumable";
	}

	return false;
}

//*****************************************************************************
JNIEXPORT void JNICALL Java_com_vectorunit_VuBillingHelper_setItemPrice(JNIEnv *env, jobject obj, jstring jItemId, jstring jPrice)
{
	// get itemId
	const char *strItemId = env->GetStringUTFChars(jItemId, 0);
	std::string storeItemName = strItemId;
	env->ReleaseStringUTFChars(jItemId, strItemId);

	// get price
	const char *strPrice = env->GetStringUTFChars(jPrice, 0);
	std::string price = strPrice;
	env->ReleaseStringUTFChars(jPrice, strPrice);

	VuParams params;
	params.addString(storeItemName.c_str());
	params.addString(price.c_str());
	VuEventManager::IF()->broadcastDelayed(0.0f, true, "OnAndroidSetItemPrice", params);
}

//*****************************************************************************
JNIEXPORT void JNICALL Java_com_vectorunit_VuBillingHelper_flagAsPirate(JNIEnv *env, jobject obj)
{
	VuGameManager::IF()->setFlaggedAsPirate();
}

//*****************************************************************************
JNIEXPORT void JNICALL Java_com_vectorunit_VuBillingHelper_logPurchaseAnalyticsEvent(JNIEnv *env, jobject obj, jstring jItemId)
{
	// get itemId
	const char *strItemId = env->GetStringUTFChars(jItemId, 0);
	std::string storeItemName = strItemId;
	env->ReleaseStringUTFChars(jItemId, strItemId);

	VuParams params;
	params.addString(storeItemName.c_str());
	VuEventManager::IF()->broadcastDelayed(0.0f, true, "OnAndroidLogPurchaseAnalyticsEvent", params);
}


JNIEXPORT void JNICALL Java_com_vectorunit_VuBillingHelper_onShutdownGame(JNIEnv *env, jobject obj, jstring jItemId, jstring jResult)
{
	VuEventManager::IF()->broadcast("OnShutdownApp");
}

JNIEXPORT jstring JNICALL Java_com_vectorunit_VuBillingHelper_getOrderNo(JNIEnv* env, jobject obj)
{
	std::string strOrderId = VuRequestOrderManager::IF()->getOrderId();
	jstring jItemName = env->NewStringUTF(strOrderId.c_str());
	return jItemName;
}