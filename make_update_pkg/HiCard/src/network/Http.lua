require "Cocos2d"
require ("json")

local CMDMap = require "network.CMDMap"
LOAD_DLG = (require "common.LoadingDialog"):create()
LOAD_DLG:retain()
TIP_DLG = (require "common.PopDialog")

local HttpClass = class("Http")
--local URL = "http://192.168.16.46:12000/sl"
local URL = "http://115.231.81.10:12000/sl"
local TOKEN_KEY = "0x0"

--------------------------------------------------------------------
--- function: Http:req
--- params: cmd         string
--- params: data        {}
--- params: callback    function
--------------------------------------------------------------------
function HttpClass:req(cmd, data, ntfCallback)
    assert(type(cmd) == "string", "[Http] cmd invalid")
    assert(type(data) == "table" or data==nil, "[Http] data invalid")
    assert(type(ntfCallback) == "function", "[Http] callback invalid")
    
    local cmdId = CMDMap[cmd].cmdId
    local cmdHandler = CMDMap[cmd].cmdHandler
    
    assert(cmdId ~= nil and cmdHandler~=nil, "[Http]no cmdId in CMDMap table for this cmd")
    
    local httpContent = {key=TOKEN_KEY, cmd=cmdId, data=data}
    
    local http = cc.XMLHttpRequest:new()
    
    local function httpCallback()
        local result = {}
        local responseStr = http.response
        
        cclog("--[Http_Recv]--"..responseStr)
        LOAD_DLG:stop()
        if responseStr == nil or responseStr == "" then
            result.err = 1
            return
        end
        
        local responseData = json.decode(responseStr)
        
        --when cmd is Login, get the token key, which is used to be client ID for sever
        if cmd == "Login" then TOKEN_KEY = responseData.key end
        
        if responseData.err ~= nil and responseData.err ~=0 then
            result.err = responseData.err
            TIP_DLG:getInstance():popText(result.err)
        else
            cmdHandler(responseData.data)
            result.data = responseData.data
        end
        
        if result.data ~= nil then
            ntfCallback(result)
        end
        --push
        if  responseData.push  ~= nil then
            local Roler = require "data.Roler"
            Roler:setData(responseData.push)
        end
        
    end
    
    http.responseType = cc.XMLHTTPREQUEST_RESPONSE_STRING
    http:open("POST", URL)
    http:registerScriptHandler(httpCallback)
    
    local jsonStr = json.encode(httpContent)
    cclog("--[Http_Send]--"..jsonStr)
    http:send(jsonStr)
    LOAD_DLG:start()
end

Http = HttpClass.new()

