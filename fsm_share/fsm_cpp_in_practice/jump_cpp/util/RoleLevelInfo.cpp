//
//  RoleLevelInfo.cpp
//  HiFly
//
//  Created by chris li on 14-4-22.
//
//

#include "RoleLevelInfo.h"

USING_NS_CC;

static RoleLevelInfo* g_RoleLevelInfo = nullptr;

RoleLevelInfo* RoleLevelInfo::getInstance()
{
    if (nullptr == g_RoleLevelInfo)
    {
        g_RoleLevelInfo = new RoleLevelInfo();
        g_RoleLevelInfo->init();
    }
    
    return g_RoleLevelInfo;
}

void RoleLevelInfo::init()
{
	//rapidjson::Document* p = JsonConfigUtil::loadConfig("VIP_BOOK.json");
	//if ((*p).IsObject()) {
 //       log("1:%s", (*p)["1"].GetString());
 //       log("2:%s", (*p)["2"].GetString());
 //   }
	//m_document = 0;
	//m_document = JsonConfigUtil::loadConfig("VIP_BOOK.json");
	//if ((*m_document).IsObject()) {
 //       log("1:%s", (*m_document)["1"].GetString());
 //       log("2:%s", (*m_document)["2"].GetString());
 //   }


	std::string json = cocos2d::FileUtils::getInstance()->getStringFromFile("LEVEL_INFO.json");

	if ("" != json)
    {
        m_document.Parse<0>(json.c_str());
    }
	CCASSERT(m_document.IsObject() == true, "JsonUtil load config file err");

	//if ((m_document).IsObject()) {
 //       log("0:%s", (m_document)["0"]["effect"].GetString());
 //       log("1:%s", (m_document)["1"]["effect"].GetString());
 //   }

}


int RoleLevelInfo::getLvUpCost(std::string level)
{
	//std::string _level = StringUtils::format("%d",level);
	return (m_document)[level.c_str()]["uplevel_cost_coin"].GetInt();
}

int RoleLevelInfo::getUpgradeCost(std::string level)
{
	//std::string _level = StringUtils::format("%d",level);
	return (m_document)[level.c_str()]["upgrade_cost_coin"].GetInt();
}

float RoleLevelInfo::getCoinExtra(std::string level)
{
	//std::string _level = StringUtils::format("%d",level);
	float _f = static_cast<float>((m_document)[level.c_str()]["coin_add_extra"].GetDouble());
	return _f;
}

float RoleLevelInfo::getScoreExtra(std::string level)
{
	//std::string _level = StringUtils::format("%d",level);
	float _f = static_cast<float>((m_document)[level.c_str()]["score_add_extra"].GetDouble());
	return _f;
}

std::string RoleLevelInfo::getEffect(std::string level)
{
	//std::string _level = StringUtils::format("%d",level);
	return (m_document)[level.c_str()]["effect"].GetString();
}

float RoleLevelInfo::getMagnetTime(std::string level)
{
	//std::string _level = StringUtils::format("%d",level);
	float _f = static_cast<float>((m_document)[level.c_str()]["magnet_time"].GetDouble());
	return _f;
}

float RoleLevelInfo::getShipTime(std::string level)
{
	//std::string _level = StringUtils::format("%d",level);
	float _f = static_cast<float>((m_document)[level.c_str()]["ship_time"].GetDouble());
	return _f;
}

float RoleLevelInfo::getHelicopterTime(std::string level)
{
	//std::string _level = StringUtils::format("%d",level);
	float _f = static_cast<float>((m_document)[level.c_str()]["helicopter_time"].GetDouble());
	return _f;
}

float RoleLevelInfo::getShieldTime(std::string level)
{
	//std::string _level = StringUtils::format("%d",level);
	float _f = static_cast<float>((m_document)[level.c_str()]["shield_time"].GetDouble());
	return _f;
}

std::string RoleLevelInfo::getRoleIntroduce(int roleId)
{
	std::string _roleId = StringUtils::format("%d",roleId);
	return (m_document)["roleIntroduce"][_roleId.c_str()].GetString();
}