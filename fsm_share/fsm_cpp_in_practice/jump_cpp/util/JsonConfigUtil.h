//
//  JsonUtil.h
//  HiFly
//
//  Created by chris li on 14-4-23.
//
//

#ifndef __HiFly__JsonUtil__
#define __HiFly__JsonUtil__

#include "cocos2d.h"
#include "json/prettywriter.h"	// for stringify JSON
#include "json/filestream.h"	// wrapper of C stream for prettywriter as output
#include "json/stringbuffer.h"

#include "json/document.h"
using namespace rapidjson;

class JsonConfigUtil
{
public:
    static rapidjson::Document* loadConfig(std::string jsonFile);
};
#endif /* defined(__HiFly__JsonUtil__) */
