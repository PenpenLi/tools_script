require "Cocos2d"

local GuardUiLayer = require "scene.guard.GuardUi"


local GuardScene = class("GuardScene", function()
    return cc.Scene:create()
end)

function GuardScene:create()
    local scene = GuardScene.new()
    scene:initM()
    return scene
end

function GuardScene:ctor()
end

function GuardScene:initM()
    self.guardUiLayer = GuardUiLayer:create()
    self:addChild(self.guardUiLayer)
end

return GuardScene
