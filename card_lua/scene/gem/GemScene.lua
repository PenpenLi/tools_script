require "Cocos2d"

local GemCityLayer = require "scene.gem.GemUI"

local HomeUILayer = require "scene.home.HomeUILayer"

local GemScene = class("GemScene", function()
    return cc.Scene:create()
end)

function GemScene:create()
    local scene = GemScene.new()
    scene:initM()
    return scene
end

function GemScene:ctor()
end

function GemScene:initM()
    self.gemCityLayer = GemCityLayer:create()
    self:addChild(self.gemCityLayer)
    
    self.homeUILayer = HomeUILayer:create()
    self:addChild(self.homeUILayer)
end

return GemScene
