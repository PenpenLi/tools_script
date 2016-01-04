local Initialization = {}

local function addSearchPath(path)
    local updateDir = cc.FileUtils:getInstance():getWritablePath().."update_package"
    if cc.FileUtils:getInstance():isFileExist(updateDir) then
        cc.FileUtils:getInstance():addSearchPath(updateDir.."/"..path)
        --cclog("Add update search path:"..updateDir.."/"..path)
    end
    cc.FileUtils:getInstance():addSearchPath(path)
end

function Initialization:start()
    --1. add search path for LUA code and image resource
    addSearchPath("src")
    addSearchPath("src/boot")
    addSearchPath("src/config")
    addSearchPath("src/common")
    addSearchPath("res")
    addSearchPath("res/image")
    addSearchPath("res/image/battle/battleuilayer")
    addSearchPath("res/image/battle/resultlayer")
    addSearchPath("res/image/common")
    addSearchPath("res/image/goods")
    addSearchPath("res/image/ui")
    addSearchPath("res/image/ui/home")
    addSearchPath("res/image/ui/mail")
    addSearchPath("res/image/ui/login")
    addSearchPath("res/image/ui/bag")
    addSearchPath("res/image/ui/goods")
    addSearchPath("res/image/soul")
    addSearchPath("res/image/ui/pve")
    addSearchPath("res/image/ui/fromation")
    addSearchPath("res/image/ui/indiana")
    addSearchPath("res/image/ui/firstrecharge")
    addSearchPath("res/image/ui/chat")
    addSearchPath("res/image/battle/armature/")
    addSearchPath("res/image/ui/PickCard")
    addSearchPath("res/image/ui/newhero")
    addSearchPath("res/image/ui/union")
    addSearchPath("res/image/ui/task")
    addSearchPath("res/image/ui/gem")
    addSearchPath("res/image/ui/openServer")
    addSearchPath("res/image/ui/common")
    addSearchPath("res/image/ui/smelt")
    addSearchPath("res/image/ui/guard")
    addSearchPath("res/image/ui/movement")
    addSearchPath("res/image/ui/dailytask")
    addSearchPath("res/image/ui/shop")
    addSearchPath("res/image/ui/tiejiangpu")
    addSearchPath("res/image/ui/setting")
    addSearchPath("res/image/ui/sport")
    addSearchPath("res/image/ui/pickCard/pickCardUIStart")
    --2. require module
    require ("common.HelperFunc")
    require ("network.Http")
end

--require "lfs"
--local path = lfs.currentdir()
--cclog("[test-lfs]"..path)

return Initialization
