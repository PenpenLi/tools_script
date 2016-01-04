require "Cocos2d"

local pickCardLayer = require "scene.pickCard.PickCard"


local pickCardScene = class("pickCardScene", function()
    return cc.Scene:create()
end)

function pickCardScene:create()
    local scene = pickCardScene.new()
    scene:initM()
    return scene
end

function pickCardScene:ctor()
end

function pickCardScene:initM()
    self.pickCardLayer = pickCardLayer:create()
    self:addChild(self.pickCardLayer)
end

return pickCardScene