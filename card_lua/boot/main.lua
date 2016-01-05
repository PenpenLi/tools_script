
require "Cocos2d"

--file.lua
--定义lua文件操作的方法
--打开文件：参数(文件名，打开模式)，返回文件句柄或nil
local function log_openFile(filename, mode)
    local logfile = cc.FileUtils:getInstance():getWritablePath()..filename
    local f = io.open(logfile,mode)
    if f then
        return f
    else
        return nil
    end
end
--读取文件全部内容：参数(文件句柄)，返回一个string
local function log_readAllFile(f)
    return f:read("*a")
end

--读取文件一行内容：参数(文件句柄)，返回一个string
local function log_readLineFile(f)
    return f:read("*l")
end
--写文件内容：参数(文件句柄，字符串)
local function log_writeFile(f, ...)
    local arg = {...}
    f:write(unpack(arg))
end
--关闭文件,参数(文件句柄()
local function log_closeFile(f)
    f:close()
end

log_remove_logstring = function()
    local file = log_openFile("client.log", "w")
    if file then
        log_writeFile(file, "")
        log_closeFile(file)
    end
end

log_get_logstring = function()
    local strlines = {}
    print_raw("~~~~~log_get_logstring~~~~~~~")
    local file = log_openFile("client.log", "r")
    local line = log_readLineFile(file)
    --print_raw("~~~~~~~~~~~~")
    --print_raw(line)
    
    local num = 0
    while line ~= nil do
        table.insert(strlines, line)
        
        line = log_readLineFile(file)
        --print_raw(line)
        if line == nil then
            log_closeFile(file)
            return strlines
        end
        num = num + 1
        if num == 5000 then
            log_closeFile(file)
            return strlines
        end
    end
    log_closeFile(file)
    return strlines
    --[[
    if file then
        local logstr = log_readAllFile(file)
        log_closeFile(file)
        return logstr
    else
        print_raw("no such log file")
    end
    ]]
end
--[[
print_raw = print
-- print
print = function(...)
    --require "base.file"
    print_raw(string.format(...))
    
    local file = log_openFile("client.log", "a")
    if file then
        log_writeFile(file, string.format(...).."\n")
        log_closeFile(file)
    end
end
]]
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
    
    --os.execute("ls -l ~/")
    --os.execute("cd E:\excel2lua")
    --os.execute("dir")
    --os.execute("php do_execel2lua.php")
    --os.execute("copy excel2lua_script\output_lua\define_npc_skill.lua E:\\")
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
    --local Initialization = require("src.boot.Initialization")
    --Initialization:start()
    
    --first scene 
    --local scene = require("scene.login.LoginScene"):create()

    --local scene = require("scene.battle2.BattleScene"):create()
    --local scene = require("scene.battle2.BattleScene"):create({teamL = {}, teamR = {}, member={}, firstmonster={}})
    --local scene = require("scene.battle2.BattleScene"):createPK({})

    local function isModuleAvailable(name)
        if package.loaded[name] then
            return true
        else
            for _, searcher in ipairs(package.searchers or package.loaders) do
                local loader = searcher(name)
                if type(loader) == 'function' then
                    package.preload[name] = loader
                    return true
                end
            end
            return false
        end
    end

    local scene = {}
    if isModuleAvailable("lfs") then
        --update support
        scene = require("src.boot.UpdateScene"):create()
    else
        --Initialize the whole run envirement
        local Initialization = require("src.boot.Initialization")
        Initialization:start()

        --first scene 
        scene = require("scene.login.LoginScene"):create()
    end
    
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
