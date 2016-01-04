require "Cocos2d"

local UnionUi = require "scene.union.UnionUi"


local UnionScene = class("UnionScene", function()
    return cc.Scene:create()
end)

function UnionScene:create()
    local scene = UnionScene.new()
    scene:initM()
    return scene
end

function UnionScene:ctor()
end

function UnionScene:initM()
    self.UnionUi = UnionUi:create()
    self:addChild(self.UnionUi)
end

return UnionScene
