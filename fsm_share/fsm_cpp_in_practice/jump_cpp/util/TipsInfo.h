//
//  TipsInfo.h
//  HiFly
//
//  Created by gong jun on 14-7-2.
//
//

#ifndef __HiFly__TipsInfo__
#define __HiFly__TipsInfo__
#include "cocos2d.h"
#include "JsonConfigUtil.h"

class TipsInfo
{
public:
    static TipsInfo* getInstance();

	std::string getTips(int id);

	std::string getMessage(std::string name);

private:
    TipsInfo() {}
    virtual ~TipsInfo() {}
    void init();

//
	rapidjson::Document m_document;
};

#endif /* defined(__HiFly__TipsInfo__) */
