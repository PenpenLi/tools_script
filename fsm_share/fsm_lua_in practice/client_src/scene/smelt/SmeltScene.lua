require "Cocos2d"

local SmeltCityLayer = require "scene.smelt.Smelt"

local HomeUILayer = require "scene.home.HomeUILayer"

local SmeltScene = class("SmeltScene", function()
    return cc.Scene:create()
end)

function SmeltScene:create()
    local scene = SmeltScene.new()
    scene:initM()
    return scene
end

function SmeltScene:ctor()
end

function SmeltScene:initM()
    
    self.smeltCityLayer = SmeltCityLayer:create()
    self:addChild(self.smeltCityLayer)
    

    self.homeUILayer = HomeUILayer:create()
    self:addChild(self.homeUILayer)
end

return SmeltScene
