
require "Cocos2d"

-- cclog
cclog = function(...)
    print(string.format(...))
end

-- for CCLuaEngine traceback
function __G__TRACKBACK__(msg)
    cclog("----------------------------------------")
    cclog("LUA ERROR: " .. tostring(msg) .. "\n")
    cclog(debug.traceback())
    cclog("----------------------------------------")
    return msg
end

local function main()
    collectgarbage("collect")
    -- avoid memory leak
    collectgarbage("setpause", 100)
    collectgarbage("setstepmul", 5000)
    
    --Game engine configure
    cc.Director:getInstance():setAnimationInterval(1.0/30)
    local policy = cc.ResolutionPolicy.FIXED_HEIGHT
    local frameSize = cc.Director:getInstance():getOpenGLView():getFrameSize()
    if frameSize.width*1.0/960 < frameSize.height*1.0/640 then
        policy = cc.ResolutionPolicy.FIXED_WIDTH
    end
    cc.Director:getInstance():getOpenGLView():setDesignResolutionSize(960, 640, policy)
    cc.Director:getInstance():setDisplayStats(false)
    
    --add search path for LUA code and image resource
    --[[
    cc.FileUtils:getInstance():addSearchPath("src")
    cc.FileUtils:getInstance():addSearchPath("src/boot")
	cc.FileUtils:getInstance():addSearchPath("src/config")
    cc.FileUtils:getInstance():addSearchPath("res")
    cc.FileUtils:getInstance():addSearchPath("res/image")
    cc.FileUtils:getInstance():addSearchPath("res/image/common")
    cc.FileUtils:getInstance():addSearchPath("res/image/ui")
    cc.FileUtils:getInstance():addSearchPath("res/image/ui/home")
    cc.FileUtils:getInstance():addSearchPath("res/image/ui/mail")
    cc.FileUtils:getInstance():addSearchPath("res/image/ui/login")
    cc.FileUtils:getInstance():addSearchPath("res/image/ui/bag")
    cc.FileUtils:getInstance():addSearchPath("res/image/goods")
    cc.FileUtils:getInstance():addSearchPath("res/image/soul")
    cc.FileUtils:getInstance():addSearchPath("res/image/ui/movement")
    cc.FileUtils:getInstance():addSearchPath("res/image/ui/dailytask")
    cc.FileUtils:getInstance():addSearchPath("res/image/ui/shop")
    cc.FileUtils:getInstance():addSearchPath("res/image/ui/pve")
    cc.FileUtils:getInstance():addSearchPath("res/image/ui/tiejiangpu")
    cc.FileUtils:getInstance():addSearchPath("res/image/ui/setting")
    cc.FileUtils:getInstance():addSearchPath("res/image/ui/fromation")
    cc.FileUtils:getInstance():addSearchPath("res/image/ui/newhero")
    cc.FileUtils:getInstance():addSearchPath("res/image/ui/sport")
    cc.FileUtils:getInstance():addSearchPath("res/image/battle/battleuilayer")
    cc.FileUtils:getInstance():addSearchPath("res/image/battle/resultlayer/")
    cc.FileUtils:getInstance():addSearchPath("res/image/ui/pickCard/pickCardUIStart")
    ]]                                                
    

    --Initialize the whole run envirement
    local Initialization = require("src.boot.Initialization")
    Initialization:start()
    
    --first scene 
    local scene = require("scene.login.LoginScene"):create()

    --local scene = require("scene.battle2.BattleScene"):create()
    --local scene = require("scene.battle2.BattleScene"):create({teamL = {}, teamR = {}, member={}, firstmonster={}})
    --local scene = require("scene.battle2.BattleScene"):createPK({})

    
    --update support
    --local scene = require("src.boot.UpdateScene"):create()
    
    if cc.Director:getInstance():getRunningScene() then
        cc.Director:getInstance():replaceScene(scene)
    else
        cc.Director:getInstance():runWithScene(scene)
    end
end

local status, msg = xpcall(main, __G__TRACKBACK__)
if not status then
    error(msg)
end
