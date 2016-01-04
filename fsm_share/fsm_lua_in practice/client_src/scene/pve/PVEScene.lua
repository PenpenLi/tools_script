require "Cocos2d"

local PVEUILayer = require "scene.pve.PVEUILayer"

local PVEScene = class("PVEScene", function()
    return cc.Scene:create()
end)

function PVEScene:create(param)
    local scene = PVEScene.new()
    scene:initM(param)
    return scene
end

function PVEScene:ctor()
end

function PVEScene:initM(param)
    self.pveUILayer = PVEUILayer:create(param)
    self:addChild(self.pveUILayer)
end

return PVEScene
