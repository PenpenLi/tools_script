//
//  PropsInfo.h
//  HiFly
//
//  Created by gong jun on 14-7-2.
//
//

#ifndef __HiFly__PropsInfo__
#define __HiFly__PropsInfo__
#include "cocos2d.h"
#include "JsonConfigUtil.h"

class PropsInfo
{
public:
    static PropsInfo* getInstance();

	std::string getProps(int id);

private:
    PropsInfo() {}
    virtual ~PropsInfo() {}
    void init();

//
	rapidjson::Document m_document;
};

#endif /* defined(__HiFly__PropsInfo__) */
