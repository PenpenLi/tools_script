//
//  TipsInfo.cpp
//  HiFly
//
//  Created by gong jun on 14-7-2.
//
//

#include "TipsInfo.h"

USING_NS_CC;

static TipsInfo* g_TipsInfo = nullptr;

TipsInfo* TipsInfo::getInstance()
{
    if (nullptr == g_TipsInfo)
    {
        g_TipsInfo = new TipsInfo();
        g_TipsInfo->init();
    }
    
    return g_TipsInfo;
}

void TipsInfo::init()
{
	std::string json = cocos2d::FileUtils::getInstance()->getStringFromFile("TIPS_INFO.json");

	if ("" != json)
    {
        m_document.Parse<0>(json.c_str());
    }
	CCASSERT(m_document.IsObject() == true, "JsonUtil load config file err");
}

std::string TipsInfo::getTips(int id)
{
	std::string _level = StringUtils::format("%d",id);
	return (m_document)[_level.c_str()].GetString();
}

std::string TipsInfo::getMessage(std::string name)
{
	return (m_document)["message"][name.c_str()].GetString();
}