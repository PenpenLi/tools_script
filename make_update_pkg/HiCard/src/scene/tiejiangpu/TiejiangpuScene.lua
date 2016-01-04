require "Cocos2d"

local TiejiangpuCityLayer = require "scene.tiejiangpu.TiejiangpuUI"

local HomeUILayer = require "scene.home.HomeUILayer"

local TiejiangpuScene = class("TiejiangpuScene", function()
    return cc.Scene:create()
end)

function TiejiangpuScene:create()
    local scene = TiejiangpuScene.new()
    scene:initM()
    return scene
end

function TiejiangpuScene:ctor()
end

function TiejiangpuScene:initM()
    self.TiejiangpuCityLayer = TiejiangpuCityLayer:create()
    self:addChild(self.TiejiangpuCityLayer)
    
    self.homeUILayer = HomeUILayer:create()
    self:addChild(self.homeUILayer)
end

return TiejiangpuScene
