require "Cocos2d"

local TiejiangpuCityLayer = require "scene.tiejiangpu.TiejiangpuUI"

local HomeUILayer = require "scene.home.HomeUILayer"

local TiejiangpuScene = class("TiejiangpuScene", function()
    return cc.Scene:create()
end)

function TiejiangpuScene:create(data)
    local scene = TiejiangpuScene.new()
    scene:initM(data)
    return scene
end

function TiejiangpuScene:ctor()
end

function TiejiangpuScene:initM(data)
    self.TiejiangpuCityLayer = TiejiangpuCityLayer:create(data)
    self:addChild(self.TiejiangpuCityLayer)
    
    self.homeUILayer = HomeUILayer:create()
    self:addChild(self.homeUILayer)
end

return TiejiangpuScene
