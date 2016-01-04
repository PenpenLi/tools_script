//
//  PropsInfo.cpp
//  HiFly
//
//  Created by gong jun on 14-7-2.
//
//

#include "PropsInfo.h"

USING_NS_CC;

static PropsInfo* g_PropsInfo = nullptr;

PropsInfo* PropsInfo::getInstance()
{
    if (nullptr == g_PropsInfo)
    {
        g_PropsInfo = new PropsInfo();
        g_PropsInfo->init();
    }
    
    return g_PropsInfo;
}

void PropsInfo::init()
{
	std::string json = cocos2d::FileUtils::getInstance()->getStringFromFile("PROPS_INFO.json");

	if ("" != json)
    {
        m_document.Parse<0>(json.c_str());
    }
	CCASSERT(m_document.IsObject() == true, "JsonUtil load config file err");
}

std::string PropsInfo::getProps(int id)
{
	std::string _id = StringUtils::format("%d",id);
	return (m_document)[_id.c_str()].GetString();
}