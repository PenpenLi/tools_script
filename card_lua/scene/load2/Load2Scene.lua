require "Cocos2d"

local Load2Scene = class("Load2Scene", function()
    return cc.Scene:create()
end)

function Load2Scene:create(param)
    local scene = Load2Scene.new()
    scene:initM(param)
    return scene
end

function Load2Scene:ctor()
end

function Load2Scene:initM(param)
    local bg = cc.Sprite:create("image/ui/load/load_bg.jpg")
    bg:setPosition(WIN_SIZE.width/2, WIN_SIZE.height/2)
    self:addChild(bg)
    
    local loadingBarBg = cc.Sprite:create("image/ui/load/load_bar_bg.png")
    loadingBarBg:setPosition(WIN_SIZE.width/2, WIN_SIZE.height/6)
    self:addChild(loadingBarBg)
    
    local loadingBar = ccui.LoadingBar:create()
    loadingBar:setPosition(WIN_SIZE.width/2, WIN_SIZE.height/6)
    loadingBar:setTag(0)
    loadingBar:setName("LoadingBar")
    loadingBar:loadTexture("image/ui/load/load_bar.png")
    loadingBar:setPercent(50)
    self:addChild(loadingBar)
    self.loadingBar = loadingBar
    
    local leftIcon = cc.Sprite:create("image/ui/load/load_bar_icon.png")
    leftIcon:setPosition(WIN_SIZE.width/3.5, WIN_SIZE.height/6)
    self:addChild(leftIcon)
    local rightIcon = cc.Sprite:create("image/ui/load/load_bar_icon.png")
    rightIcon:setPosition(WIN_SIZE.width/1.4, WIN_SIZE.height/6)
    rightIcon:setScaleX(-1)
    self:addChild(rightIcon)
    
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

function Load2Scene:loadAsync(param)
    local imageList = {
        "image/ui/pve/PVEUI0.png",
        "image/ui/pve/map1.png",
        "image/ui/pve/map2.png",
        "image/ui/pve/map3.png",
        "image/ui/pve/map4.png",
        "image/ui/pve/map5.png",
        "image/ui/pve/map6.png",
        "image/ui/pve/map7.png",
        "image/ui/pve/map7.png",
    }
    local function loadOk()
        self.loadNum = self.loadNum or 0
        self.loadNum = self.loadNum + 1
        local percent = math.min(self.loadNum*100/(#imageList-1), 100)
        self.loadingBar:setPercent(percent)
        self.tip:setString("正在加载中..."..percent.."%")
        if self.loadNum >= #imageList then
            self:loadOk()
        end
    end
    for i=1, #imageList do
        cc.Director:getInstance():getTextureCache():addImageAsync(imageList[i], loadOk)
    end
end

function Load2Scene:loadOk()
end

return Load2Scene

