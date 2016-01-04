require "Cocos2d"

local BagUILayer = require "scene.bag.BagUILayer"
local BoxDisplayLayer = require "scene.bag.BagUILayer"


local HomeUILayer = require "scene.home.HomeUILayer"

local BagScene = class("BagScene", function()
    return cc.Scene:create()
end)

function BagScene:create(param)
    local scene = BagScene.new()
    scene:initM(param)
    return scene
end

function BagScene:ctor()
end

function BagScene:initM(param)
    self.bagUILayer = BagUILayer:create(param)
    --self.boxDisplayLayer = BoxDisplayLayer:create()
    self:addChild(self.bagUILayer)
    --self:addChild(self.boxDisplayLayer)
    
    self.homeUILayer = HomeUILayer:create()
    self:addChild(self.homeUILayer)
end

return BagScene
