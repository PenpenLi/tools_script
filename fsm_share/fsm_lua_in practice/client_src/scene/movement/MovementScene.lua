require "Cocos2d"

local MovementUILayer = require "scene.movement.MovementUILayer"

local MovementScene = class("MovementScene", function()
    return cc.Scene:create()
end)

function MovementScene:create()
    local scene = MovementScene.new()
    scene:initM()
    return scene
end

function MovementScene:ctor()
end

function MovementScene:initM()
    self.movementUILayer = MovementUILayer:create()
    self:addChild(self.movementUILayer)
end

return MovementScene
