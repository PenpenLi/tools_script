require "Cocos2d"

local SportUILayer = require "scene.sport.SportUILayer"

local SportScene = class("SportScene", function()
    return cc.Scene:create()
end)

function SportScene:create()
    local scene = SportScene.new()
    scene:initM()
    return scene
end

function SportScene:ctor()
end

function SportScene:initM()
    self.sportUILayer = SportUILayer:create()
    self:addChild(self.sportUILayer)
end

return SportScene
