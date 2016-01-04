require "Cocos2d"

local Load1Scene = class("Load1Scene", function()
    return cc.Scene:create()
end)

function Load1Scene:create(param)
    local scene = Load1Scene.new()
    scene:initM(param)
    return scene
end

function Load1Scene:ctor()
end

function Load1Scene:initM(param)
    self.stepNum = 0 
    ccs.ArmatureDataManager:destroyInstance()
    cc.Director:getInstance():getTextureCache():removeAllTextures()
    cc.SpriteFrameCache:getInstance():removeSpriteFrames()
    -- emulate dilog animation
    require "common.GuardCenter"
    local eventDispatcher = cc.Director:getInstance():getEventDispatcher()
    if guardCenter:canEnterGuardMode("moni") == true then
        self.dialogText = ccui.Text:create()
        self.dialogText:setFontSize(30)
        self.dialogText:setTextHorizontalAlignment(0)
        self.dialogText:setString("我：我在哪里，这里好像已经不是...")
        
        self.dialogText:setPosition(WIN_SIZE.width/2, WIN_SIZE.height/2)
        --self:addChild(self.dialogText)
        --Helper.setTimeout(self,3,function() self:stepCount() end)
        
        self:runAction(cc.Sequence:create(cc.DelayTime:create(2),
            cc.CallFunc:create(function() self:stepCount() end),
            cc.DelayTime:create(2), 
            cc.CallFunc:create(function()self:stepCount()end),
            cc.DelayTime:create(2),
            cc.CallFunc:create(function() self:stepCount() end),
            cc.DelayTime:create(2),
            cc.CallFunc:create(function() self:stepCount() end),
            cc.DelayTime:create(2),
            cc.CallFunc:create(function() self:stepCount() end),
            cc.DelayTime:create(2),
            cc.DelayTime:create(2),cc.CallFunc:create(function() self:stepCount() end))
        )
        
        local moniLayer = cc.LayerColor:create(cc.c4b(0, 0, 0, 110), WIN_SIZE.width, WIN_SIZE.height)
        local function onTouchBegan(touch, event) return true end
        local function onTouchMoved(touch, event) end
        local function onTouchEnded(touch, event) self:stepCount() end
        local listener = cc.EventListenerTouchOneByOne:create()
        listener:setSwallowTouches(true)
        listener:registerScriptHandler(onTouchBegan,cc.Handler.EVENT_TOUCH_BEGAN )
        listener:registerScriptHandler(onTouchMoved,cc.Handler.EVENT_TOUCH_MOVED )
        listener:registerScriptHandler(onTouchEnded,cc.Handler.EVENT_TOUCH_ENDED )
        local eventDispatcher = cc.Director:getInstance():getEventDispatcher()
        eventDispatcher:addEventListenerWithSceneGraphPriority(listener, moniLayer)
        moniLayer:addChild(self.dialogText)
        self:addChild(moniLayer)
        
        
       --[[ self:runAction(cc.Sequence:create(cc.DelayTime:create(1),
            cc.CallFunc:create(function() self.dialogText:setString("神秘人：命运之子，你终于来了!") end),
            cc.DelayTime:create(1), 
            cc.CallFunc:create(function()self.dialogText:setString("神秘人: 这个世界等你的到来已经很久很久了!")end),
            cc.DelayTime:create(1),
            cc.CallFunc:create(function() self.dialogText:setString("神秘人：这个世界的混乱需要你来解救!") end),
            cc.DelayTime:create(1),
            cc.CallFunc:create(function() self.dialogText:setString("我：我要怎样拯救这个世界？") end),
            cc.DelayTime:create(1),
            cc.CallFunc:create(function() self.dialogText:setString("我：咦? 前方好像有人爆发战斗，去瞧瞧！") end),
            cc.DelayTime:create(1),
            cc.DelayTime:create(1),cc.CallFunc:create(function() self:loadOk() end))
        )--]]
        return
    end
    --[[
    local bg = cc.Sprite:create("image/ui/load/load_bg.jpg")
    bg:setAnchorPoint(0,0)
    bg:setPosition(0,0)
    bg:setScaleX(1.2)
    --bg:setPosition(WIN_SIZE.width/2, WIN_SIZE.height/2)
    self:addChild(bg)
    ]]
    local armatureFile1 = "image/ui/login/LoadBg/LoadBg.csb"
    ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile1)
    local bg = ccs.Armature:create("LoadBg")
    bg:getAnimation():play("Animation1")
    bg:setPosition(WIN_SIZE.width/2, WIN_SIZE.height/2)
    bg:setScaleX(WIN_SIZE.width/960)
    bg:setScaleY(WIN_SIZE.height/640)
    self:addChild(bg,-1)
    ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile1)
    
    local loadingBarBg = cc.Sprite:create("image/ui/load/load_bar_bg.png")
    loadingBarBg:setPosition(WIN_SIZE.width/2, WIN_SIZE.height/6)
    loadingBarBg:setScaleX(1.5)
    self:addChild(loadingBarBg)
    
    local gamenameicon = cc.Sprite:create("image/ui/load/load_gameicon.png")
    gamenameicon:setPosition(WIN_SIZE.width-130, WIN_SIZE.height-100)
    gamenameicon:setScale(0.8)
    self:addChild(gamenameicon)
    
    local loadingBar = ccui.LoadingBar:create()
    loadingBar:setPosition(WIN_SIZE.width/2, WIN_SIZE.height/6)
    loadingBar:setScaleX(1.5)
    loadingBar:setTag(0)
    loadingBar:setName("LoadingBar")
    loadingBar:loadTexture("image/ui/load/load_bar.png")
    loadingBar:setPercent(0)
    self:addChild(loadingBar)
    self.loadingBar = loadingBar
    
    local leftIcon = cc.Sprite:create("image/ui/load/load_bar_icon.png")
    leftIcon:setPosition(WIN_SIZE.width/2 - loadingBar:getContentSize().width*1.5/2, WIN_SIZE.height/6)
    self:addChild(leftIcon)
    local rightIcon = cc.Sprite:create("image/ui/load/load_bar_icon.png")
    rightIcon:setPosition(WIN_SIZE.width/2 + loadingBar:getContentSize().width*1.5/2, WIN_SIZE.height/6)
    rightIcon:setScaleX(-1)
    self:addChild(rightIcon)
    
    self.runStepLen = (rightIcon:getPositionX()-leftIcon:getPositionX())*0.01
    Helper.addSpriteFrames("image/ui/load/load_run.plist","image/ui/load/load_run.png")
    local run_frameArr1 = Helper.newFrames("load_run_%d.png",1,9)
    local run_animation1 = Helper.newAnimation(run_frameArr1,1.0/14)
    local runboy = cc.Sprite:create()
    runboy:setPosition(leftIcon:getPositionX()+25, WIN_SIZE.height/6+60)
    runboy:runAction(cc.RepeatForever:create(cc.Animate:create(run_animation1)))
    self:addChild(runboy)
    self.runboy = runboy
    self.prePosX = runboy:getPositionX()
    
    local tip = ccui.Text:create()
    tip:setString("正在加载中...0%")
    --alert:setFontName(font_TextName)
    tip:setFontSize(24)
    tip:setColor(cc.c3b(159, 168, 176))    
    tip:setPosition(WIN_SIZE.width/2, WIN_SIZE.height/10)
    self:addChild(tip)
    self.tip = tip
    --load resAsyn
    self:loadAsync(param)
end

function Load1Scene:stepCount()
    self.stepNum = self.stepNum + 1 
    if self.stepNum > 6 then
        self.stepNum = 7
    end

    if  self.stepNum == 1 then 
        self.dialogText:setString("神秘人：命运之子，你终于来了!")
    elseif  self.stepNum == 2 then 
        self.dialogText:setString("神秘人: 这个世界等你的到来已经很久很久了!")
    elseif  self.stepNum == 3 then 
        self.dialogText:setString("神秘人：这个世界的混乱需要你来解救!")
    elseif  self.stepNum == 4 then 
        self.dialogText:setString("我：我要怎样拯救这个世界？")
    elseif  self.stepNum == 5 then 
        self.dialogText:setString("我：咦? 前方好像有人爆发战斗，去瞧瞧！")
    elseif  self.stepNum == 6 then 
        self:loadOk()
    end

end

function Load1Scene:loadAsync(param)
    local imageList = {
        "image/ui/home/Home0.png",
        "image/ui/home/Home1.png",
        "image/ui/home/pve/pveAnimation.pvr.ccz",
        "image/ui/home/guild/guildAnimation.pvr.ccz",
        "image/ui/home/hero/heroAnimation.pvr.ccz",
        "image/ui/home/huodong/huodongAnimation.pvr.ccz",
        "image/ui/home/keji/kejiAnimation.pvr.ccz",
        "image/ui/home/mail/mailAnimation.pvr.ccz",
        "image/ui/home/pickcard/pickcardAnimation.pvr.ccz",
        "image/ui/home/pvp/pvpAnimation.pvr.ccz",
        "image/ui/home/shop/shopAnimation.pvr.ccz",
        "image/ui/home/task/taskAnimation.pvr.ccz",
        "image/ui/home/tiejiang/tjAnimation.pvr.ccz"
    }
    local function loadOk()
        self.loadNum = self.loadNum or 0
        self.loadNum = self.loadNum + 1
        local percent = math.min(math.floor(self.loadNum*100/(#imageList-1)), 100)
        self.loadingBar:setPercent(percent)
        self.tip:setString("正在加载中..."..percent.."%")
        self.runboy:setPositionX(percent*self.runStepLen+self.prePosX)
        print("[Load1]"..self.loadNum.."/"..self.loadNum)
        if self.loadNum >= #imageList then
            self:loadOk()
        end
    end
    for i=1, #imageList do
        cc.Director:getInstance():getTextureCache():addImageAsync(imageList[i], loadOk)
    end
end

function Load1Scene:loadOk()
    print("[Load1Scene] loadOk")
    if guardCenter:canEnterGuardMode("moni") == true then
        local BattleScene = require("scene.battle2.BattleScene")
        local scene = BattleScene:createEmulate()
        cc.Director:getInstance():replaceScene(scene)
    else
       local scene = require("scene.home.HomeScene"):create()
        cc.Director:getInstance():replaceScene(scene) 
        Helper.PlayBackGroundMuisc("sound/background.mp3")   
    end

end

return Load1Scene

