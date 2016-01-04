require "Cocos2d"

local HomeCityLayer = require "scene.home.HomeCityLayer"
local HomeUILayer = require "scene.home.HomeUILayer"

--local BattleLayer = require "scene.battle.actors.BattleUILayer"

local HomeScene = class("HomeScene", function()
    return cc.Scene:create()
end)

function HomeScene:create()
    local scene = HomeScene.new()
    scene:initM()
    return scene
end

function HomeScene:ctor()
end

function HomeScene:initM()
    self.homeCityLayer = HomeCityLayer:create()
    self.homeUILayer = HomeUILayer:create()
    self.homeUILayer:setName("uilayer")
    
  --  self.battleLayer = BattleLayer:create()
    
    
    self:addChild(self.homeCityLayer)
    self:addChild(self.homeUILayer)
  --  self:addChild(self.battleLayer)
  
  --load resAsyn
  self:loadAsync()
end

function HomeScene:loadAsync()
    local imageList = {
        "image/ui/pve/PVEUI0.png",
    }
    local function loadOk()
        self.loadNum = self.loadNum or 0
        self.loadNum = self.loadNum + 1
        if self.loadNum == #imageList then
            self.loadOk = true
        end
    end
    for i=1, #imageList do
        cc.Director:getInstance():getTextureCache():addImageAsync(imageList[i], loadOk)
    end
end

return HomeScene
