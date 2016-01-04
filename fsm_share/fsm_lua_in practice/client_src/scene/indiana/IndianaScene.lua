require "Cocos2d"

local IndianaCityLayer = require "scene.indiana.IndianaUi"


local IndianaScene = class("IndianaScene", function()
    return cc.Scene:create()
end)

function IndianaScene:create()
    local scene = IndianaScene.new()
    scene:initM()
    return scene
end

function IndianaScene:ctor()
end

function IndianaScene:initM()
    self.indianaCityLayer = IndianaCityLayer:create()
    self:addChild(self.indianaCityLayer)
end

return IndianaScene
